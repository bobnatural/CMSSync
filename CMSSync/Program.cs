using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Security.Cryptography.X509Certificates;
using System.Runtime.InteropServices;
using AdPoolService;
using AdPoolService.Extensions;
using AdPoolService.Logging;

namespace Cmssync
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        public static int Main(string[] args)
        {
            ILog eventLog = new NullLog();
            try
            {
                eventLog = new WinEventLog("CMSSync");
            }
            catch (Exception e)
            {
                Console.Error.WriteLine("Could not write to event log - aborting. Details: {0} - {1}", e.GetType().Name, e.Message);
                return -2;
            }

            if (args.Length > 0)
            {
                try {
                    string firstArg = args[0].ToLowerInvariant();
                    string secondArg = null;
                    if (args.Length > 1)
                        secondArg = args[1].ToLowerInvariant();
                    switch (firstArg)
                    {
                        case "/d":
                        case "-d":
                            return RunConsole();
                        //case "/init":
                        //    return RunConsole(true);
                        case "/setcredential":
                            return SetCredentials(eventLog);
                        case "/update":
                            return UpdateCredentials(eventLog);
                        default:
                            var myName = System.Reflection.Assembly.GetEntryAssembly().GetName().Name;
                            Console.Error.WriteLine("You must either run it as a service or specify the /setCredential argument to configure the credentials.");
                            //Console.Error.WriteLine("USAGE: " + myName + "\n\t/setcredential\t\tconfigure the credentials.\n\t/update\t\tupdate credentials\n\t/d\t\tdebug mode");
                            Console.Error.WriteLine(@"To register it as a service: %Windir%\Microsoft.NET\Framework\v4.0.30319\installutil /LogFile=inst.log /LogToConsole=true " + myName + ".exe");
                            Console.Error.WriteLine(@"To unregister: %Windir%\Microsoft.NET\Framework\v4.0.30319\installutil /u " + myName + ".exe");
                            return 1;
                    }                   
                }
                catch (Exception e)
                {
                    ReportFailure("Unhandled exception: {0}".Fmt(e), eventLog);
                    return -1;
                }
            }
            else
            {
                // No command args - run as a service
                ServiceBase[] ServicesToRun;
                ServicesToRun = new ServiceBase[] 
                { 
                    new AdPool() 
                };
                ServiceBase.Run(ServicesToRun);
                return 0;
            }
        }

        private static int UpdateCredentials(ILog eventLog)
        {
            var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);

            QueryCertificate(configFile);

            var sectionSourceAD = configFile.GetSection("SourceADCredentials") as ADCredentialsConfigurationSection;
            var sectionDestAD = configFile.GetSection("DestinationADCredentials") as ADCredentialsConfigurationSection;

            if (sectionSourceAD == null || sectionDestAD == null)
            {
                ReportFailure("No SourceADCredentials or DestinationADCredentials section(s) supplied in the application config file.", eventLog);
                return 4;
            }
            sectionSourceAD.Decrypt();
            sectionDestAD.Decrypt();

            Func<ADServersElement, string, bool> queryServersPwd = (servers, srvType) =>
            {
                foreach (ADServerElement srv in servers)
                {
                    Console.Out.WriteLine("Enter password for user '" + srv.User + "' to access " + srvType + " AD '" + srv.Url + "'. Empty to leave unchanched.");
                    var newPass = QueryPassword();
                    if (!string.IsNullOrEmpty(newPass))
                        srv.Password = newPass;
                }
                return true;
            };
            queryServersPwd(sectionSourceAD.ADServers, "Source");
            queryServersPwd(sectionDestAD.ADServers, "Destination");
                        
            sectionSourceAD.Encrypt();
            sectionDestAD.Encrypt();

            configFile.Save(ConfigurationSaveMode.Modified);

            //LockFileAccessForRsaKey(eventLog);

            Console.Out.WriteLine("Encrypted AD server(s) password(s) saved in the application config file.");
            eventLog.LogInfo("AD server(s) authentication credentials set successfully.");

            return 0;
        }

        private static int SetCredentials(ILog eventLog)
        {          
            Console.Out.WriteLine("Set credentials ...");
            var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);

            QueryCertificate(configFile);

            var sectionSourceAD = configFile.GetSection("SourceADCredentials") as ADCredentialsConfigurationSection;
            var sectionDestAD = configFile.GetSection("DestinationADCredentials") as ADCredentialsConfigurationSection;

            if (sectionSourceAD == null || sectionDestAD == null)
            {
                ReportFailure("No SourceADCredentials or DestinationADCredentials section(s) supplied in the application config file.", eventLog);
                return 4;
            }
            Console.Out.WriteLine("Decrypt ...");

            sectionSourceAD.Decrypt();
            sectionDestAD.Decrypt();

            Console.Error.WriteLine("");
            Console.Error.WriteLine("Enter Source AD server(s) configuration:");
            sectionSourceAD.ADServers = QueryAD();
            Console.Error.WriteLine("");
            Console.Error.WriteLine("Enter Destination AD server(s) configuration:");
            sectionDestAD.ADServers = QueryAD();

            //section.SectionInformation.ProtectSection("BITSProtectionProvider");

            sectionSourceAD.Encrypt();
            sectionDestAD.Encrypt();

            configFile.Save(ConfigurationSaveMode.Modified);
            
            //LockFileAccessForRsaKey(eventLog);

            Console.Out.WriteLine("Encrypted AD server(s) credentials saved in the application config file.");
            eventLog.LogInfo("AD server(s) authentication credentials set successfully.");
            return 0;
        }

        private static void QueryCertificate(Configuration configFile)
        {
            var appsettings = configFile.GetSectionGroup("applicationSettings") as ApplicationSettingsGroup;
            var serviceSettings = appsettings.Sections["Cmssync.Settings"] as ClientSettingsSection;
            var certSerial = serviceSettings.Settings.Get("CCMCertificateSerial");
            var certIssuer = serviceSettings.Settings.Get("CCMCertificateIssuer");
            string promptSerial = null, promptIssuer = null;
            //var promptSerial = ReadString("Enter Certificate Serial (empty to stop, escape to keep unchaged)");
            //var promptIssuer = ReadString("Enter Certificate Issuer (empty to stop, escape to keep unchaged)");

            try
            {
                List<X509Certificate2> certList = new List<X509Certificate2>();
                int i = 0;
                Func<StoreName, StoreLocation, bool> GetFromStore = (StoreName storeName, StoreLocation storeLocation) =>
                {
                    try
                    {
                        Console.WriteLine("Cert from " + storeName + " " + storeLocation);
                        X509Store store = new X509Store(storeName, storeLocation);
                        store.Open(OpenFlags.OpenExistingOnly | OpenFlags.ReadOnly);
                        foreach (X509Certificate2 cert in store.Certificates)
                        {
                            certList.Add(cert);
                            //cert.GetNameInfo(X509NameType.SimpleName, true);
                            Console.WriteLine((++i) + ". " + cert.GetNameInfo(X509NameType.SimpleName, true) + "; " + cert.SerialNumber + "; " + cert.NotAfter);
                        }
                    }
                    catch (Exception e)
                    {
                        Console.Error.WriteLine("error to open certificate store: " + storeName + " " + storeLocation + ": " + e.Message);
                    }
                    return true;
                };
               
                GetFromStore(StoreName.My, StoreLocation.CurrentUser);
                //GetFromStore(StoreName.My, StoreLocation.LocalMachine);

                //certList.ForEach(cert => Console.WriteLine((certList.IndexOf(cert) + 1) + ". " + cert.GetNameInfo(X509NameType.SimpleName, true) + "; " + cert.SerialNumber + "; " + cert.NotAfter));
                //foreach(var cert in certList)
                //    Console.WriteLine((++i) + ". " + cert.GetNameInfo(X509NameType.SimpleName, true) + "; " + cert.SerialNumber + "; " + cert.NotAfter);

                var n = ReadString("Choose Certificate 1.." + certList.Count + " (escape to keep unchaged)");
                int choosed;
                if (n != null && Int32.TryParse(n, out choosed) && 1 <= choosed && choosed <= certList.Count)
                {
                    var cert = certList.ElementAt(choosed - 1);
                    promptSerial = cert.SerialNumber;
                    promptIssuer = cert.GetNameInfo(X509NameType.SimpleName, true);
                    Console.WriteLine("Choosen Certificate: " + promptSerial + "; " + promptIssuer + "; " + cert.NotAfter);
                }                    
            }
            catch(Exception ex)
            {
                Console.Error.WriteLine(ex.Message);
            }
            
            if (promptSerial != null)
                certSerial.Value.ValueXml.InnerText = promptSerial;
            if (promptIssuer != null)
                certIssuer.Value.ValueXml.InnerText = "CN = " + promptIssuer;

            serviceSettings.SectionInformation.ForceSave = true;
        }

        private static ADServersElement QueryAD()
        {
            var servers = new ADServersElement();
            int i = 0;
            while (true)
            {
                var url = QueryUrl();
                if (string.IsNullOrEmpty(url))
                {
                    break;
                }

                var user = QueryUsername();
                if (string.IsNullOrEmpty(user))
                {
                    user = QueryUsername();
                }

                var pass = QueryPassword();
                while (string.IsNullOrEmpty(pass))
                {
                    pass = QueryPassword();
                }

                //var type = QueryType();
                //while (string.IsNullOrEmpty(type))
                //{
                //    type = QueryType();
                //}

                var allowClearLDAP = QueryLDAPSecure();
                while (allowClearLDAP == null)
                    allowClearLDAP = QueryLDAPSecure();

                servers[i++] = new ADServerElement()
                {
                    Url = url,
                    User = user,
                    Password = pass,
                    allowClearLDAP = allowClearLDAP
                };
            }
            return servers;
        }


        static int RunConsole()
        {
            BackgroundPool.Run(true);
            do
            {
                Console.Write("press Enter to exit.\n");
            } while (Console.ReadKey(false).Key != ConsoleKey.Enter);

            BackgroundPool.Stop();
            return 0;
        }

        private static void ReportFailure(string failureMessage, ILog eventLog)
        {
            Console.Error.WriteLine(failureMessage);
            eventLog.LogFatal("Failed to set ADServer credentials: {0}", failureMessage);
        }

        private static string QueryUrl()
        {
            return ReadString("Enter Server Domain Name/IP Address (empty to stop)");
        }

        private static string QueryUsername()
        {
            return ReadString("Enter Server user name");
        }

        private static string QueryType()
        {
            var res = ReadString("Enter authentication scheme (basic|digest|ntlm|negotiate)");

            if (!res.Equals("ntlm") && !res.Equals("basic") && !res.Equals("negotiate") && !res.Equals("digest"))
            {
                Console.Error.WriteLine("Unsupported authentication scheme! Use one of: ntlm,basic,negotiate,digest");
                return null;
            }

            return res;
        }
        private static string QueryLDAPSecure()
        {
            var res = ReadString("Would you like to use encrypted LDAP? [y,n]");
            var resLow = res.Trim().ToLower();

            if (!resLow.Equals("y") && !resLow.Equals("n"))
            {
                Console.Error.WriteLine("Unsupported authentication scheme! Use one of: true,false");
                return null;
            }
            return resLow.Equals("n")? "y": "n"; // is backwards. SSL = Not ClearLDAP.
        }

        private static string QueryPassword()
        {
            do
            {
                var p1 = ReadString("Enter password", true);
                if (string.IsNullOrEmpty(p1))
                {
                    return null;
                }

                var p2 = ReadString("Re-enter password", true);
                if (string.Equals(p1, p2))
                {
                    return p1;
                }
                Console.Error.WriteLine("Passwords don't match!");
            } while (true);
        }

        private static string ReadString(string prompt, bool hide = false)
        {
            Console.Out.Write("{0}: ", prompt);

            var builder = new StringBuilder();
            ConsoleKeyInfo key;
            do
            {
                key = Console.ReadKey(true);
                if (key.Key == ConsoleKey.Escape)
                {
                    Console.Out.WriteLine();
                    return null;
                }
                if (key.Key == ConsoleKey.Backspace)
                {
                    if (builder.Length > 0)
                    {
                        builder.Length -= 1;
                        Console.Out.Write("\b \b");
                    }
                }
                else if (key.Key != ConsoleKey.Enter)
                {
                    builder.Append(key.KeyChar);
                    Console.Out.Write(hide ? '*' : key.KeyChar);
                }
            } while (key.Key != ConsoleKey.Enter);

            Console.WriteLine();
            return builder.ToString();
        }
    }
}
