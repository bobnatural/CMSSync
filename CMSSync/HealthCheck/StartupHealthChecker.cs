using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Security.AccessControl;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Security.Principal;
using AdPoolService.Extensions;
using AdPoolService.Logging;
using Cmssync;
using System.DirectoryServices;
using System.Threading;

namespace AdPoolService.HealthCheck {
    public class StartupHealthChecker {
        private readonly IEnumerable<Func<CheckResult>> scheduledChecks;
        private readonly IConfiguration configuration;
        private readonly ILog log;
        private readonly string homeFolder;

        public StartupHealthChecker(IConfiguration configuration, ILog log, string homeFolder)
        {
            if (configuration == null) {
                throw new ArgumentNullException("configuration");
            }
            this.configuration = configuration;
            this.log = log;
            this.homeFolder = homeFolder;

            var checks = new List<Func<CheckResult>> {
                CheckConfigFileExist,
                CheckCertificateExists, 
                CheckCPRSettings, 
#if !DEBUG
                CheckCCMHealth,
#endif
                () => CheckServerCredentialsSupplied(ADCredentialsConfigurationSection.SourceADSettings),
                () => CheckServerCredentialsSupplied(ADCredentialsConfigurationSection.DestADSettings),
                () => CheckServerCredentialsAreEncrypted(ADCredentialsConfigurationSection.SourceADSettings),
                () => CheckServerCredentialsAreEncrypted(ADCredentialsConfigurationSection.DestADSettings),
                CheckDestOUsExists
            };
            scheduledChecks = checks;
        }

        private CheckResult CheckCCMHealth()
        {
            if(0 != Cmssync.CCMApi.CheckHealth(configuration.CCMHost, configuration.CCMPort,
                                            configuration.CCMCertificateSerial.Trim().ToLower(), // CMS required lowcase letters !
                                            configuration.CCMCertificateIssuer.Trim()))
                return CheckResult.FailWith(true, "Unable connect to CCM ([" +
                    configuration.CCMHost + "][" + configuration.CCMPort + "][" +
                    configuration.CCMCertificateSerial.Trim() + "][" +
                    configuration.CCMCertificateIssuer.Trim() + "]): ");
            return CheckResult.Success;
        }

        //// http://stackoverflow.com/questions/11709862/check-if-directory-is-accessible-in-c
        //private static bool CanRead(string path) {
        //    try {
        //        var readAllow = false;
        //        var readDeny = false;
        //        var accessControlList = Directory.GetAccessControl(path);
        //        var accessRules = accessControlList.GetAccessRules(true, true, typeof(SecurityIdentifier));

        //        foreach (
        //            var rule in
        //                accessRules.Cast<FileSystemAccessRule>().Where(rule => (FileSystemRights.Read & rule.FileSystemRights) == FileSystemRights.Read)) {
        //            switch (rule.AccessControlType) {
        //                case AccessControlType.Allow:
        //                    readAllow = true;
        //                    break;
        //                case AccessControlType.Deny:
        //                    readDeny = true;
        //                    break;
        //            }
        //        }

        //        return readAllow && !readDeny;
        //    } catch (UnauthorizedAccessException) {
        //        return false;
        //    }
        //}

        private CheckResult CheckConfigFileExist() {
            var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            if (!configFile.HasFile)
            {
                LogHelper.GetLogger().WriteEntry("CMSSync cannot find it's configuration file: " + configFile.FilePath, EventLogEntryType.Error, LogHelper.EVT_ERROR_CONFIG);
                return CheckResult.FailWith(true, "Config file " + configFile + " doesn't exist!");
            }

            //checks unknown settings
            //var settGroup = configFile.GetSectionGroup("applicationSettings") as System.Configuration.ApplicationSettingsGroup;
            //foreach (ClientSettingsSection s in settGroup.Sections)
            //{
            //    foreach (SettingElement ss in s.Settings)
            //    {
            //        string t = ((SettingElement)ss).Name.ToString();
            //    }
            //}

            return CheckResult.Success;
        }        
        
       

        private CheckResult CheckRemotePortAvailable() {
            return CheckResult.Success; 
            /*
            try {
                var uri = new Uri(configuration.RemoteBITSDestination);

                using (var tcpClient = new TcpClient()) {
                    tcpClient.Connect(uri.Host, uri.Port);
                }
                return CheckResult.Success;
            } catch (UriFormatException) {
                return CheckResult.FailWith("Remote BITS destination URI is not a valid URI: {0}", configuration.RemoteBITSDestination);
            } catch (Exception) {
                return CheckResult.FailWith("Cannot connect to remote BITS destination port: {0}", configuration.RemoteBITSDestination);
            }*/
        }

        private CheckResult CheckServerCredentialsSupplied(ADCredentialsConfigurationSection section)
        {
            try {
                if (section == null) {
                    return CheckResult.FailWith("{0} section is not declared in the config file", section.SectionName);
                }

                if (section.ADServers.Count == 0)
                {
                    return CheckResult.FailWith("No AD servers specified");
                }

                foreach (ADServerElement srv in section.ADServers)
                {
                    if (string.IsNullOrEmpty(srv.Password))
                    {
                        return CheckResult.FailWith("password is not specified for server " + srv.Url);
                    }
                }                

                return CheckResult.Success;
            } catch (Exception ex) {
                return CheckResult.FailWith("AD Server credentials could not be read: " + ex.Message);
            }
        }

        private static CheckResult CheckServerCredentialsAreEncrypted(ADCredentialsConfigurationSection section)
        {
            try {
                if (section == null) {
                    return CheckResult.FailWith("{0} section is not declared in the config file", section.SectionName);
                }

                if (section.ADServers.Count == 0) {
                    return CheckResult.FailWith("No AD Server servers specified in section " + section.SectionName);
                }

                foreach (ADServerElement bitsServer in section.ADServers)
                {
                    if (!bitsServer.PasswordEncrypted) {
                        return CheckResult.FailWith("Password is not encrypted for server '" + bitsServer.Url + "' in section " + section.SectionName);
                    }
                }

                return CheckResult.Success;
            } catch (Exception ex) {
                return CheckResult.FailWith("AD Server credentials could not be read: " + ex.Message);
            }
        }

        private CheckResult CheckCertificateExists()
        {
            var issuer = configuration.CCMCertificateIssuer.Trim();
            var serial = configuration.CCMCertificateSerial.Trim();
            //if (string.IsNullOrEmpty(issuer) || string.IsNullOrEmpty(serial))
            //    return CheckResult.Success;

            if(SearchCertificate(StoreName.My, StoreLocation.CurrentUser, serial, issuer)
                || SearchCertificate(StoreName.My, StoreLocation.LocalMachine, serial, issuer)
                )
                return CheckResult.Success;
            return CheckResult.FailWith("Certificate is not found. CCMCertificateIssuer=[" + issuer + "]. CCMCertificateSerial=[" + serial + "]");
        }

        private bool SearchCertificate(StoreName storeName, StoreLocation storeLocation, string serial, string issuer)
        {
            try{
                X509Store store = new X509Store(storeName, storeLocation);
                store.Open(OpenFlags.OpenExistingOnly | OpenFlags.ReadOnly);

                foreach (var cert in store.Certificates)
                {
                    var sertIssuer = "CN = " + cert.GetNameInfo(X509NameType.SimpleName, true);
                    if (serial.Equals(cert.SerialNumber, StringComparison.OrdinalIgnoreCase) && issuer.Equals(sertIssuer, StringComparison.OrdinalIgnoreCase))
                    {
                        log.LogDebug("found certificate: '" + cert.Issuer + "' in " + storeLocation + " " + storeName);
                        return true;
                    }
                }
            }
            catch (CryptographicException ex)
            {
                log.LogDebug("error to open certificate store: " + storeName + " " + storeLocation + ". " + ex.Message);
            }
            return false;
        }


        public IEnumerable<CheckResult> PerformChecks() {
            return scheduledChecks.Select(c => c()).ToList();
        }

        public CheckResult CheckCPRSettings()
        {
            //if (string.IsNullOrEmpty(configuration.CCMCPRFile.Trim()))
            //    return CheckResult.FailWith("File [CCMCPRFile] is not set in configuration");
            if(!string.IsNullOrEmpty(configuration.CCMCPRFile.Trim()) && !File.Exists(Path.Combine(homeFolder, configuration.CCMCPRFile)))
                CheckResult.FailWith("File [CCMCPRFile] is not found: " + configuration.CCMCPRFile);
#if !DEBUG
            if (string.IsNullOrEmpty(configuration.CCMHost.Trim()))
                return CheckResult.FailWith("[CCMHost] is not set in configuration");
#endif
            if (string.IsNullOrEmpty(configuration.CCMPort.Trim()))
                return CheckResult.FailWith("[CCMPort] is not set in configuration");

            return CheckResult.Success;
        }

        public CheckResult CheckDestOUsExists()
        {
            string errors = string.Empty;

            foreach (var server in configuration.DestADServers)
            {
                log.LogDebug("check ADHints in Destination AD server " + server.Name + " ...");
                bool hasSuccessHintOU = false;
                var msg = CheckOUExists(server, ADHintsConfigurationSection.GetAllOUs(), out hasSuccessHintOU);
                errors += msg;
                if (!hasSuccessHintOU)
                    return CheckResult.FailWith("No valid DestOUs found in ADHintSettings." + Environment.NewLine + errors);
            }

            foreach (var server in configuration.SourceADServers)
            {
                log.LogDebug("check OUsDNToMonitor in Source AD server " + server.Name + " ...");
                bool hasSuccessMonitor = configuration.OUsDNToMonitor.Count == 0;
                var msg = CheckOUExists(server, configuration.OUsDNToMonitor.ToArray(), out hasSuccessMonitor);
                errors += msg;
                if (!hasSuccessMonitor)
                    return CheckResult.FailWith("No valid OUs found in OUsDNToMonitorSettings." + Environment.NewLine + errors);
            }

            if (!string.IsNullOrEmpty(errors))
                return CheckResult.WarningWith(errors);

            return CheckResult.Success;
        }

        private string CheckOUExists(ADServer server, string[] OUs, out bool success)
        {
            var starTime = DateTime.Now;
            string errors = "";
            success = false;
            foreach (var OU in OUs)
            {
                int port = 389;  // 636 for LDAPS. 389 for LDAP.
                AuthenticationTypes authTypes = AuthenticationTypes.Secure;
                if (server.SSL)
                {
                    port = 636;  //default port: 636 for LDAPS. 389 for LDAP.
                    authTypes |= AuthenticationTypes.SecureSocketsLayer; // SSL
                }
                string searchPath = @"LDAP://" + server.Name + ":" + port;
                string destPath = searchPath + "/" + OU;
                for (int trials = 0; trials < 6; trials++)
                {
                    if (DateTime.Now.Subtract(starTime).TotalSeconds > 60)
                    {
                        success = false;
                        return "Unable to connect to AD server [" + server + "]"; 
                    }
                    try
                    {
                        using (var destOU = new DirectoryEntry(destPath, server.ServerUserName, server.ServerPassword, authTypes))
                        {
                            var n = destOU.Name.ToString();
                            success = true;
                            log.LogDebug(" found OU: " + destPath);
                            break;
                        }
                    }
                    catch (Exception e)
                    {
                        var msg = "Fail to check OU [" + OU + "] in AD " + server.Name; // ""specified in config";
                        //                    if (e.HResult == -2147016661 || e.HResult == -2147016656) //0x8007202b. A referral was returned from the server
                        errors += msg + ". " + e.Message;

                        if (e.HResult == -2147016656  // object not found
                            || e.HResult == -2147023570 // user name or passw incorrect
                            || e.HResult == -2147016672 // An operations error occurred.
                            || e.HResult == -2147016661 // 0x8007202b. A referral was returned from the server
                            )
                            break;
                        else
                        {
                            // if -2147016646 //server is not operational
                            // and other errors means that server is down or not responding
                            Thread.Sleep(10 * 1000);
                        }
                    }
                } // tries
            }
            return errors;
        }
    }
}