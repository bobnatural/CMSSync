using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Cmssync;
using Cmssync.Configure;
using System.Reflection;
using System.IO;

namespace AdPoolService
{
    public class SettingsConfiguration : IConfiguration
    {
        public int PoolsInterval
        {
            get { return Settings.Default.ADPollingInterval; }
        }

        public int ServiceHealthCheckInterval
        {
            get { return Settings.Default.ServiceHealthCheckInterval; }
        }

               
        public ADServer[] SourceADServers
        {
            get
            {
                ADCredentialsConfigurationSection.Refresh();
                return ReadServersFromConfig(ADCredentialsConfigurationSection.SourceADSettings.ADServers, "Source");
            }
        }
        public ADServer[] DestADServers
        {
            get
            {
                ADCredentialsConfigurationSection.Refresh();
                return ReadServersFromConfig(ADCredentialsConfigurationSection.DestADSettings.ADServers, "Destination");
            }
        }

        private ADServer[] ReadServersFromConfig(ADServersElement confServers, string typeOfSource)
        {
            var res = new ADServer[confServers.Count];
            for (int i = 0; i < res.Length; i++)
            {
                var serverElement = confServers[i];
                res[i] = new ADServer()
                {
                    Name = serverElement.Url,
                    ServerUserName = serverElement.User,
                    ServerPassword = serverElement.Password,
                    SSL = !serverElement.allowClearLDAP.Trim().ToLower().Equals("y", StringComparison.OrdinalIgnoreCase),
                    SourceDest = typeOfSource
                };
            }
            return res;
        }

        public bool LogVerbose
        {
            get { return Settings.Default.LogVerbose; }
        }

        public bool DataMismatchLogging
        {
            get { return Settings.Default.DataMismatchLogging; }
        }

        public string CCMCertificateSerial
        {
            get { return Settings.Default.CCMCertificateSerial; }
        }

        public string CCMCertificateIssuer
        {
            get { return Settings.Default.CCMCertificateIssuer; }
        }

        //public ISet<string> SamAccountsToMonitor
        //{
        //    get {
        //        var list = Settings.Default.SamAccountsToMonitor.Split(';');
        //        var h = new HashSet<string>(list);
        //        h.RemoveWhere(p => string.IsNullOrEmpty(p.Trim()));
        //        return h;
        //    }
        //}

        public ISet<string> OUsToMonitor
        {
            get {
                var list = Settings.Default.OUsToMonitor.Split(';');
                var h = new HashSet<string>(list, StringComparer.OrdinalIgnoreCase);
                h.RemoveWhere(p => string.IsNullOrEmpty(p.Trim()));
                return h;
            }
        }
        public ISet<string> OUsDNToMonitor
        {
            get
            {
                return OUsDNToMonitorConfigurationSection.GetOUsDNsToMonitor();
            }
        }

        public string CCMCPRFile
        {
            get { return Settings.Default.CCMCPRFile; }
        }

        public string CCMHost
        {
            get { return Settings.Default.CCMHost; }
        }

        public string CCMPort
        {
            get { return Settings.Default.CCMPort; }
        }

        public static string CPRFileContent
        {
            get
            {
                string currentYear = (3 + DateTime.Now.Year).ToString();
                var assembly = Assembly.GetExecutingAssembly();
                var resourceName = "Cmssync.Configure.CPRsample.xml";

                using (Stream stream = assembly.GetManifestResourceStream(resourceName))
                using (StreamReader reader = new StreamReader(stream))
                {
                    return reader.ReadToEnd().Replace(@"validityEndDate=""2015", @"validityEndDate=""" + currentYear)
                        .Replace(@"<hsp:ExpirationDate>2015", @"<hsp:ExpirationDate>" + currentYear);
                }
            }
        }
        public static string LoadCPRFile(string cprFile)
        {
            string currentYear = (3 + DateTime.Now.Year).ToString();
            using (var r = new System.IO.StreamReader(cprFile))
            {
                string cprString = File.ReadAllText(cprFile);

                return cprString.Replace(@"validityEndDate=""2015", @"validityEndDate=""" + currentYear)
                    .Replace(@"<hsp:ExpirationDate>2015", @"<hsp:ExpirationDate>" + currentYear);
                // validityEndDate="20151230134837"
                // <hsp:ExpirationDate>20151230</hsp:ExpirationDate>
            }
        }              
    }
}