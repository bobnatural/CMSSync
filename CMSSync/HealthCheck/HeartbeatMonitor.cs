using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Text;
using System.Threading;
using AdPoolService.Logging;
using System.DirectoryServices.Protocols;
using Cmssync;

namespace AdPoolService.HealthCheck {
    public class HeartbeatMonitor
    {
        private bool _running;
        private Thread _thread;

        public IConfiguration configuration { get; set; }

        public void Start()
        {
            if (_running)
            {
                return;
            }
            _running = true;
            _thread = new Thread(Work);
            _thread.Start();
        }

        public void Stop()
        {
            _running = false;
        }

        private string SendHb(ADServer server)
        {
            try {
                using (LdapConnection connection = server.getLapConnection)
                {
                    var filter = "(&(objectClass=*))";
                    var searchRequest = new SearchRequest(null, filter, System.DirectoryServices.Protocols.SearchScope.Base, "highestCommittedUSN", "DnsHostName", "dsServiceName");
                    var response = connection.SendRequest(searchRequest) as SearchResponse;
                    var result = !string.IsNullOrEmpty(Convert.ToString(response.Entries[0].Attributes["highestcommittedusn"][0]));
                }               
            } catch (Exception ex) {
                return ex.Message;
            }
            return string.Empty;
        }

        private void Work()
        {
            var lastTime = DateTime.Now.AddMinutes(1); //Start in 1 minute

            while (_running)
            {
                if (DateTime.Now.Subtract(lastTime).TotalSeconds > configuration.ServiceHealthCheckInterval) // check each 10 seconds
                {
                    List<string> failedServers = new List<string>();
                    failedServers.AddRange(TryServers(configuration.SourceADServers));
                    failedServers.AddRange(TryServers(configuration.DestADServers));
                    if (failedServers.Count > 0)
                        LogHelper.GetLogger().WriteEntry("Server(s) failed to responce:" + Environment.NewLine + string.Join(Environment.NewLine, failedServers), EventLogEntryType.Warning, LogHelper.EVT_FAILED_HB);
                    TryCMS();
                    lastTime = DateTime.Now;
                }
                Thread.Sleep(1000);
            }
        }

        private List<string> TryServers(ADServer[] srvList)
        {
            List<string> failedServers = new List<string>();
            foreach (var srv in srvList)
            {
                string msg;
                if (!string.IsNullOrEmpty(msg = SendHb(srv)))
                    failedServers.Add(srv.ToString() + ". " + msg);
            }
            return failedServers;
        }
        private bool TryCMS()
        {
            if (configuration.CCMHost.TrimEnd().Length == 0)
                return true;
            try
            {
                return 0 == Cmssync.CCMApi.CheckHealth(configuration.CCMHost, configuration.CCMPort,
                                configuration.CCMCertificateSerial.Trim().ToLower(), // CMS required lowcase letters !
                                configuration.CCMCertificateIssuer.Trim());
            }
            catch(Exception ex)
            {
                LogHelper.GetLogger().WriteEntry("Unable connect to CCM ([" + 
                    configuration.CCMHost+"]["+configuration.CCMPort+"]["+
                    configuration.CCMCertificateSerial.Trim() + "][" +
                    configuration.CCMCertificateIssuer.Trim() + "]): " + 
                    ex.Message, 
                    EventLogEntryType.Warning, LogHelper.EVT_FAILED_HB);
            }
            return false;
        }
    }
}
