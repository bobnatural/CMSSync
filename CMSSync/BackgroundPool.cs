﻿using System;
using System.Collections.Generic;
using System.DirectoryServices;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using AdPoolService.Logging;
using AdPoolService.Extensions;
using AdPoolService.HealthCheck;
using System.Runtime.InteropServices;
using System.DirectoryServices.Protocols;
using Cmssync;
using Cmssync.Extensions;

namespace AdPoolService
{
    class BackgroundPool
    {
        private static ILog log = new NullLog();

        static bool canWork = true;
        static Thread threadBackgroundWorker = new Thread(BackgroundPoll);
        static ManualResetEvent stopEvent = new ManualResetEvent(false);

        private static HeartbeatMonitor hb;
        private static SettingsConfiguration config;
        private static string cprContent;

        private static ISet<string> oUsToMonitor;
        private static ISet<string> oUsDNToMonitor;

        /// <summary>
        /// lastHighUSNs by AD instance
        /// </summary>
        private static IDictionary<string, string> lastHighUSNs = new Dictionary<string, string>(); //LoadCurrentHighUSN();

        /// <summary>
        /// users which failes during initialization. We must try them agin till success.
        /// </summary>
        private static Dictionary<string, UserProperties> initializationFails = new Dictionary<string, UserProperties>();

        public static void Run(bool console = false)
        {
            BackgroundPool.config = new SettingsConfiguration();

            oUsToMonitor = config.OUsToMonitor;
            oUsDNToMonitor = config.OUsDNToMonitor;

            try
            {
                InitializeLogging(console);

                PerformStartupChecks(config);

                hb = new HeartbeatMonitor();
                hb.configuration = config;
                hb.Start();

                threadBackgroundWorker.IsBackground = true;
                if (threadBackgroundWorker.ThreadState.HasFlag(System.Threading.ThreadState.Unstarted))
                    threadBackgroundWorker.Start();
            }
            catch (Exception e)
            {
                if (e.InnerException != null)
                {
                    Console.Error.WriteLine(e.InnerException.Message);
                    log.LogFatal(e.InnerException.Message);
                }
                Console.Error.WriteLine(e.Message);
                log.LogFatal(e.Message);
                throw;
            }
        }

        private static void BackgroundPoll()
        {
            bool logVerbose = config.LogVerbose;
            //var accountToMonitor = config.SamAccountsToMonitor;
            //if (accountToMonitor.Count > 0)
            //    log.LogDebug("Monitoring only " + accountToMonitor.Count + " accounts: " + string.Join(";", accountToMonitor));

            if (oUsToMonitor.Count > 0)
                log.LogDebug("Monitoring only " + oUsToMonitor.Count + " OU(s): " + string.Join(";", oUsToMonitor));
            if (oUsDNToMonitor.Count > 0)
                log.LogDebug("Monitoring only " + oUsDNToMonitor.Count + " OU(s) DNs: " + string.Join(";", oUsDNToMonitor));

            // get attributes for replicate:
            var allAtributes = ADHintsConfigurationSection.GetAllAttributeNames();
            PollAD.AddSourcePropNames(allAtributes);
            PollAD.AddDestinationPropNames(allAtributes);

            PollAD.Log = log;
            CCMApi.log = log;
            CCMApi.interCcmSpacing = config.interCcmSpacing;

#if DEBUG
            uniTest();
#endif
            InitializeAllAccounts();

            while (true)
            {
                try
                {
                    if (!canWork)
                        break;

                    if (File.Exists(Path.Combine(HomeFolder, config.CCMCPRFile)))
                        cprContent = SettingsConfiguration.LoadCPRFile(Path.Combine(HomeFolder, config.CCMCPRFile));
                    else
                        cprContent = SettingsConfiguration.CPRFileContent;

                    PollAD ad = GetAvailableAD(config.SourceADServers, lastHighUSNs, false);
                    bool success = false;

                    if (ad == null)
                        log.LogError("Unable to connect to any AD Servers");
                    else
                    {
                        if (ad.ChangedUsersProperties.Count == 0)
                            success = true; // no changes 
                        else
                        {
                            int cnt = ad.ChangedUsersProperties.Count;
                            //log.LogInfo("Found update for " + cnt + " user(s) in Source AD. Current USN = " + ad.CurrentHighUSN + ". InvocationID=" + ad.GetInvocationID);

                            FilterAccounts(ad.ChangedUsersProperties, config.DestADServers.Select(s => s.ServerUserName), oUsToMonitor, oUsDNToMonitor);
                            if (cnt - ad.ChangedUsersProperties.Count > 0)
                                log.LogInfo("Filtered out " + (cnt - ad.ChangedUsersProperties.Count) + " accounts");

                            if (ad.ChangedUsersProperties.Count == 0)
                                success = true; // no changes 
                            else if (0 != PutToDestinationAD(config.DestADServers, ad.ChangedUsersProperties, false))
                                success = true; // saved to Dest AD
                            //  if at least 1 user is succeeded then assume that whole iteration is succeeded
                            //  If all users are failed than seems to be CCM or AD is down and iteration should run again.
                        }

                        // if iteration is secceeded and highUSN is changed than save it to DB
                        if (success && (!lastHighUSNs.ContainsKey(ad.GetInvocationID) || lastHighUSNs[ad.GetInvocationID] != ad.CurrentHighUSN))
                        {
                            lastHighUSNs[ad.GetInvocationID] = ad.CurrentHighUSN;
                            // SaveCurrentHighUSN(ad.GetInvocationID, ad.CurrentHighUSN);
                        }
                    }
                    if (initializationFails.Count > 0)
                    {
                        log.LogInfo("Process " + initializationFails.Count + " accounts which failed on startup ...");
                        PutToDestinationAD(config.DestADServers, initializationFails.Values.ToList(), false);
                    }
                }
                catch (Exception ex)
                {
                    log.LogError(ex, "BackgroundPoll");
                }
                finally
                {
                    stopEvent.WaitOne(config.PoolsInterval * 1000); // each 2 sec
                }
            }
        }

        private static void uniTest()
        {
            //using (var fs = new System.IO.StreamWriter(Path.Combine(HomeFolder, "cpr.txt"), false))
            //{
            //    fs.Write(cprContent);
            //    fs.Close();
            //}

            UserProperties userPropsFrom = new UserProperties() { 
            { "L", new string[]{"Boston","Tokio"} }, 
            { "facsimileTelephoneNumber", null },  // new string[]{"123"}
            { "mobile", new string[]{"7"} }, 
            { "memberOf", new string[]{"CN=Exchange Services,CN=Users,DC=egar,DC=egartech,DC=com", "CN=IIS_IUSRS,CN=Builtin,DC=kireev,DC=local"} }, 
            { "mail", new string[]{"123DfG123"} }, 
            { "userPrincipalName", new string[]{"123"} }, };
            UserProperties userPropsTo = new UserProperties() { 
            { "L", new string[] { "Boston" } }, 
            { "facsimileTelephoneNumber", new string[] { "456" } }, 
            { "userAccountControl", new string[] { Convert.ToString((uint)Utils.UserAccountControl.SMARTCARD_REQUIRED) } }, 
            { "mobile", new string[] { "4" } },
            { "memberOf", new string[]{"CN=Exchange Services,CN=Users,DC=egar,DC=egartech,DC=com","CN=IIS_IUSRS,CN=Builtin,DC=kireev,DC=local"} }, 
            };

            string transResult;
            ADHintElement adHint = ADHintsConfigurationSection.GetOUByAttributes(userPropsTo, userPropsFrom, out transResult);
            if (adHint != null)
            {
                var qualityCheck = adHint.QualityCheck(userPropsFrom);
                //var transResult = adHint.GetTransitionByUserAttributes(userPropsFrom, userPropsTo);
            }
        }

        private static void InitializeAllAccounts()
        {
            CacheAllGroups();

            log.LogInfo("Initialize accounts ...");
            PollAD adSource = GetAvailableAD(config.SourceADServers, lastHighUSNs, true);
            PollAD adDest = GetAvailableAD(config.DestADServers, lastHighUSNs, true);

            lastHighUSNs[adSource.GetInvocationID] = adSource.CurrentHighUSN;

            try
            {
                IDictionary<string, UserProperties> userByObjectSID = new Dictionary<string, UserProperties>(adDest.ChangedUsersProperties.Count);
                IDictionary<string, UserProperties> userBySamAccount = new Dictionary<string, UserProperties>(adDest.ChangedUsersProperties.Count);
                log.LogInfo("Loaded " + adSource.ChangedUsersProperties.Count + " accounts from SourceAD " + adSource.DnsHostName + " and " + adDest.ChangedUsersProperties.Count + " accounts from Destination AD " + adDest.DnsHostName);
                foreach (var userProps in adDest.ChangedUsersProperties)
                {
                    userProps.Remove("ObjectSID");
                    if (userProps.ContainsKey("Pager") && userProps["Pager"] != null)
                        userByObjectSID[userProps["Pager"][0]] = userProps;
                    userBySamAccount[userProps["samAccountName"][0]] = userProps;
                }

                log.LogDebug("  " + userByObjectSID.Count + " users has initialized ObjectSID in DestAD");
                var changedUsers = new List<UserProperties>();

                var cnt = adSource.ChangedUsersProperties.Count;
                FilterAccounts(adSource.ChangedUsersProperties, config.DestADServers.Select(s => s.ServerUserName), oUsToMonitor, oUsDNToMonitor);
                if (cnt - adSource.ChangedUsersProperties.Count > 0)
                    log.LogInfo("Filtered out " + (cnt - adSource.ChangedUsersProperties.Count) + " accounts");

                // Compare Source and Destination users ...
                foreach (var userProps in adSource.ChangedUsersProperties)
                {
                    UserProperties destUser = null;
                    if (!userByObjectSID.TryGetValue(userProps["ObjectSID"][0], out destUser) || destUser == null)
                        userBySamAccount.TryGetValue(userProps["samAccountName"][0], out destUser);

                    if (destUser == null) // not found by SID nor samAccountName
                    {
                        log.LogDebug("  '" + userProps["samAccountName"][0] + "' is new user");
                        changedUsers.Add(userProps); // new user
                    }
                    else
                    {
                        ADHintElement adHint = null;
                        try
                        {
                            string notUsed;
                            adHint = ADHintsConfigurationSection.GetOUByAttributes(userProps, destUser, out notUsed);
                        }
                        catch (Exception)
                        {
                        }

                        // {[distinguishedName, CN=user3. sdfsdf,OU=Office31,OU=Office3,OU=Domain Controllers,DC=kireev,DC=local]}
                        // simple way to determine if OU is changed
                        if (adHint != null && destUser["distinguishedName"][0].IndexOf(adHint.DestOU, StringComparison.OrdinalIgnoreCase) < 0)
                        {
                            log.LogDebug("  '" + destUser["distinguishedName"][0] + "' need to move to " + adHint.DestOU);
                            changedUsers.Add(userProps); //  OU is changed
                        }
                        else
                            foreach (var prop in userProps)
                            {
                                string[] destPropVal;
                                if (!PollAD.propIgnoreDest.Contains(prop.Key)
                                    && destUser.TryGetValue(prop.Key, out destPropVal) && prop.Value != null && !Utils.CheckEquals(prop.Value, destPropVal))
                                {
                                    log.LogDebug("  '" + userProps["samAccountName"][0] + "' changed [" + prop.Key + "]='" + Utils.PropVal(destPropVal) + "' -> '" + Utils.PropVal(prop.Value) + "'");
                                    changedUsers.Add(userProps);
                                    break;
                                }
                            }
                    }
                }

                if (changedUsers.Count == 0)
                    log.LogInfo("Initialization complete. No differences between SourceAD and DestinationAD found.");
                else
                {
                    log.LogInfo("Need to update " + changedUsers.Count + " accounts...");
                    var updatedCnt = PutToDestinationAD(config.DestADServers, changedUsers, true);
                    log.LogInfo("Initialization complete. Successfully updated " + updatedCnt + " of " + changedUsers.Count + " accounts.");
                }
            }
            catch (Exception ex)
            {
                log.LogError(ex, "Failed to Initialize: " + ex.Message);
                return;
            }
        }

        private static void CacheAllGroups()
        {
            foreach (var server in config.SourceADServers)
            {
                try
                {
                    PollAD.GetGroupMembers(server, ADHintsConfigurationSection.GetAllGroupsFromConfig());
                    return; // first available source AD
                }
                catch (Exception ex)
                {
                    log.LogDebug("Failed to connect to " + server.ToString() + ": " + ex.Message);
                }
            }
            throw new Exception("No Source AD Servers available");
        }

        private static PollAD GetAvailableAD(ADServer[] servers, IDictionary<string, string> successHighUSNs, bool loadAll)
        {
            foreach (var server in servers)
            {
                try
                {
                    // first available AD
                    return new PollAD(server, successHighUSNs, loadAll);
                }
                catch (Exception ex)
                {
                    log.LogDebug("Failed to connect to " + server.ToString() + ": " + ex.Message);
                }
            }
            throw new Exception("No " + servers[0].SourceDest + " AD Servers available");
        }


        private static void FilterAccounts(List<UserProperties> changedUsersProperties, IEnumerable<string> vipAccounts, ISet<string> oUsToMonitor, ISet<string> OUsDNToMonitor)
        {
            try
            {
                int removedCnt = 0;
                // filter out VIP accounts
                foreach (var vip in vipAccounts)
                    removedCnt += changedUsersProperties.RemoveAll(p => vip.Equals(p["samAccountName"][0], StringComparison.OrdinalIgnoreCase));
                // filter out accounts that not in List:
                //ISet<string> accountToMonitor, 
                //if (accountToMonitor.Count > 0)
                //    removedCnt += changedUsersProperties.RemoveAll(p => !accountToMonitor.Contains(p["samAccountName"], StringComparer.OrdinalIgnoreCase));

                // filter by OU
                if (oUsToMonitor.Count > 0)
                {
                    Func<string, bool> accountInOU = (dn) =>
                    {
                        foreach (var dnc in new DistinguishedName(dn).Components)
                        {
                            if (dnc.Type == "OU" && oUsToMonitor.Contains(dnc.UnescapedValue))
                                return true;
                        }
                        return false;
                    };
                    removedCnt += changedUsersProperties.RemoveAll(p => !accountInOU(p["distinguishedName"][0]));
                }

                // filter by DN of OU
                Func<string, bool> accountInOUDN = (userDn) =>
                {
                    var userParentOU = userDn.Substring(userDn.IndexOf(',') + 1);
                    foreach (var ouDN in OUsDNToMonitor)
                        if (userParentOU.Equals(ouDN, StringComparison.OrdinalIgnoreCase))
                            return true;
                    return false;
                };
                removedCnt += changedUsersProperties.RemoveAll(p => !accountInOUDN(p["distinguishedName"][0]));
            }
            catch (Exception ex)
            {
                log.LogError(ex, "Error to filter accounts");
            }
        }

        /// <summary>
        /// Put to Destination and put to CCM
        /// </summary>
        /// <param name="servers"></param>
        /// <param name="usersProps"></param>
        /// <returns>count of successed users</returns>
        private static int PutToDestinationAD(ADServer[] servers, List<UserProperties> usersProps, bool initializeMode)
        {
            int updatedCnt = 0;
            foreach (var props in usersProps) // for each user
            {
                var samAccount = props["samAccountName"][0];
                var objectSID = props["objectSID"][0];
                foreach (ADServer server in servers)
                {
                    try
                    {
                        var pollRes = PollAD.AddUser(server, props, cprContent, servers.Where(s => s.Name != server.Name).ToArray());
                        if (pollRes == 0)
                        {
                            updatedCnt++; // success
                            initializationFails.Remove(objectSID);
                        }
                        else if (pollRes != 15 && initializeMode) // 15 if qualityCheck fails
                            initializationFails.Add(objectSID, props);
                        break; // success
                    }
                    catch (Exception ex)
                    {
                        log.LogError(ex, "Save user '" + samAccount + "' to Destination AD Server: " + (server != null ? server.Name : "null" + ": " + ex.Message));
                        if (ex.HResult != -2147016646) //server is not operational
                            break; // no need to try another server if server is available
                    }
                    // try next server
                }
            }
            if (updatedCnt == usersProps.Count)
                log.LogInfo("Updated " + updatedCnt + " user(s) of " + usersProps.Count);
            else
                log.LogError("Updated " + updatedCnt + " user(s) of " + usersProps.Count + ". See previouse log records for errors.");

            return updatedCnt; // == usersProps.Count;

            throw new Exception("Unable to connect to any Destination servers");
        }

        public static void Stop()
        {
            log.LogDebug("Stopped");
            canWork = false;
            stopEvent.Set();
            if (hb != null)
                hb.Stop();
        }
        private static void SaveCurrentHighUSN(string InvocationID, string CurrentHighUSN)
        {
            using (var fs = new System.IO.StreamWriter(Path.Combine(HomeFolder, "Invocation_" + InvocationID + ".txt"), false))
            {
                fs.WriteLine(CurrentHighUSN);
                fs.Close();
            }
        }
        private static IDictionary<string, string> LoadCurrentHighUSN()
        {
            var successHighUSNs = new Dictionary<string, string>();
            foreach (var f in System.IO.Directory.GetFiles(HomeFolder, @"Invocation_*.txt"))
            {
                using (var r = new System.IO.StreamReader(f))
                {
                    var invocationID = System.IO.Path.GetFileName(f).Replace("Invocation_", "").Replace(".txt", "");
                    var usn = r.ReadLine();
                    successHighUSNs[invocationID] = usn;
                }
            }
            return successHighUSNs;
        }

        private static void PerformStartupChecks(IConfiguration configuration)
        {
            var healthChecker = new StartupHealthChecker(configuration, log, HomeFolder);

            var results = healthChecker.PerformChecks();

            bool logsWritten = results.Any(c => !c.IsSuccess && c.LogsWritten);

            var warnings = results.Where(c => c.IsSuccess && !string.IsNullOrEmpty(c.Message)).Select(c => c.Message).ToList();
            if (warnings.Any() && !logsWritten)
            {
                log.LogWarn(string.Join("\r\n- ", warnings));
            }

            var failures = results.Where(c => !c.IsSuccess).Select(c => c.FailureMessage).ToList();
            if (!failures.Any())
            {
                return;
            }

            var message =
                    "Service cannot be started due to following reasons:\r\n- {0}".Fmt(string.Join("\r\n- ", failures));
            if (!logsWritten)
            {
                log.LogFatal(message);
            }

            throw new ServiceStartupException(message);
        }

        private static void InitializeLogging(bool console)
        {
            WinEventLog eventLog;
            try
            {
                eventLog = new WinEventLog("CMSSync");
                eventLog.LogInfo("Initializing CMSSync Service");
            }
            catch (Exception e)
            {
                throw new ServiceStartupException("Service cannot be started because it cannot write to event log: {0} - {1}".Fmt(e.GetType().Name, e.Message));
            }

            bool logVerbose = config.LogVerbose;

#if DEBUG
            logVerbose = true;
#endif
            if (!logVerbose && !console)
                log = eventLog;
            else
            {
                var clog = new CompositeLog();
                if (console)
                    clog.AddLog(new ConsoleLog());
                if (logVerbose)
                    clog.AddLog(new FileLog(new FileInfo(Path.Combine(HomeFolder, "log.txt"))));
                clog.AddLog(eventLog);
                log = clog;
            }

            log.LogDebug("");
            log.LogDebug(DateTime.Now + " Starting service in folder " + HomeFolder);
        }

        private static string HomeFolder
        {
            get { return System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location); }
        }
    }
}
