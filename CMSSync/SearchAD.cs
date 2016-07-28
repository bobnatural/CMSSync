using AdPoolService.Logging;
using Cmssync;
using Cmssync.Extensions;
using System;
using System.Collections.Generic;
using System.DirectoryServices;
using System.DirectoryServices.Protocols;
using System.Linq;
using System.Security.Principal;
using System.Threading;

namespace AdPoolService
{
    class PollAD
    {
        private List<UserProperties> usersProperties; // changed users

        private string workInvocationID; // database ID that was used in search
        private string currentHighUSN;
        private string dnsHostName;
        private string dsServiceName;

        private static ILog log = new NullLog();

        // what properties we need from SourceAD (Pager is needed in initialization DestAD to compare with ObjectSID)
        static readonly ISet<string> propLoadSourceHard = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "samAccountName", "displayName", "givenName", "sn", "cn", "distinguishedName", "userPrincipalName", "initials", "mail", "uSNChanged", "objectSID", "userAccountControl" };
        // what properties we need from SourceAD (Pager is needed in initialization DestAD to compare with ObjectSID)
        static readonly ISet<string> propLoadDestHard = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "samAccountName", "displayName", "givenName", "sn", "cn", "distinguishedName", "userPrincipalName", "initials", "mail", "Pager", "userAccountControl" };

        // ignored properties to copy from Source to Destination:
        private static readonly ISet<string> _propIgnoreDest = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "objectSID", "userAccountControl", "password", "pager", "uSNChanged", "distinguishedName", "cn", "memberOf" };
        public static ISet<string> propIgnoreDest
        {
            get
            {
                var res = new HashSet<string>(_propIgnoreDest);
                res.UnionWith(Utils.UserAccountControlFlags);
                return res;
            }
        }

        public static List<string> propNamesSource; // hard + hint + transition props for SourceAD
        public static List<string> propNamesDestination; // hard + transition props for DestAD

        private static IDictionary<string, ISet<string>> groupCache; // cache of groups. [Group distinguishedname] -> [Users distinguishedname]

        static public void AddSourcePropNames(string[] addProps)
        {
            if (propNamesSource == null)
                propNamesSource = new List<string>(propLoadSourceHard);
            propNamesSource.AddRange(addProps);
            propNamesSource = propNamesSource.Distinct(StringComparer.OrdinalIgnoreCase).ToList();
        }
        static public void AddDestinationPropNames(string[] addProps)
        {
            if (propNamesDestination == null)
                propNamesDestination = new List<string>(propLoadDestHard);
            propNamesDestination.AddRange(addProps);
            propNamesDestination = propNamesDestination.Distinct(StringComparer.OrdinalIgnoreCase).ToList();
        }

        public static ILog Log
        {
            set { log = value; }
        }


        public PollAD(ADServer server, IDictionary<string, string> prevHighUSNs, bool loadAll)
        {
            usersProperties = new List<UserProperties>();
            currentHighUSN = null;
            string prevHighUSN;

            using (LdapConnection connection = server.getLapConnection)
            {
                var filter = "(&(objectClass=*))";
                var searchRequest = new SearchRequest(null, filter, System.DirectoryServices.Protocols.SearchScope.Base, "highestCommittedUSN", "DnsHostName", "dsServiceName", "objectSID");
                var response = connection.SendRequest(searchRequest) as SearchResponse;

                currentHighUSN = Convert.ToString(response.Entries[0].Attributes["highestcommittedusn"][0]);
                //Console.WriteLine(" currentHighUSN: " + currentHighUSN);

                // Get the name of the DC connected to.
                dnsHostName = Convert.ToString(response.Entries[0].Attributes["DnsHostName"][0]);
                //Console.WriteLine(" dns: " + dnsHostName);

                // Bind to the DC service object to get the invocationID.
                // The dsServiceName property of root DSE contains the distinguished
                // name of this DC service object.
                dsServiceName = Convert.ToString(response.Entries[0].Attributes["dsServiceName"][0]);
            }

            using (DirectoryEntry rootDSE = new DirectoryEntry(server.path, server.ServerUserName, server.ServerPassword, server.authTypes))
            {
                using (DirectoryEntry dcService = new DirectoryEntry(server.path + @"/" + dsServiceName, server.ServerUserName, server.ServerPassword, server.authTypes))
                {
                    // instanceID of AD database:
                    workInvocationID = BitConverter.ToString((byte[])dcService.Properties["invocationID"].Value);
                    //Console.WriteLine(" InvocationID = " + workInvocationID);
                    // Compare it to the DC name from the previous USN sync operation.
                    // Each invocationID has each own highestCommittedUSN !
                    if (loadAll || prevHighUSNs == null)
                        prevHighUSN = "0"; // initialize mode. Load All accounts.
                    else if (!prevHighUSNs.TryGetValue(workInvocationID, out prevHighUSN))
                        prevHighUSN = null; // AD server is polled first time. 
#if DEBUG
                    //foreach (var prop in dcService.SchemaEntry.Properties)
                    //{
                    //    string propName = prop.ToString();
                    //    var propValue = dcService.SchemaEntry.Properties[propName].Value;
                    //    Console.WriteLine(propName + "=" + propValue);
                    //}
#endif
                }

                log.LogDebug(server.ToString() + ". currentUSN=" + currentHighUSN);

                if (String.IsNullOrEmpty(prevHighUSN))
                {
                    log.LogDebug("FIRST launch. Skip polling. Set currentUSN=" + currentHighUSN);
                    return; // first launch. Just set currentHighUSN and return.
                }

                if (Equals(prevHighUSN, currentHighUSN))
                    return; // no changes since last update

                if (prevHighUSN == "0")
                    log.LogWarn("Load all users from [" + server.ToString() + "] ...");

                LoadUsersByFilter(server, rootDSE, string.Format("(&(objectClass=user)(objectCategory=person)(uSNChanged>={0})(!(uSNChanged={0})))", prevHighUSN));

                if (prevHighUSN != "0") // only for realtime mode
                    LoadUsersByGroups(server, rootDSE, prevHighUSN);

                return;
            }
        }

        private void LoadUsersByGroups(ADServer server, DirectoryEntry rootDSE, string prevHighUSN)
        {
            //HashSet<string> usersInGroup = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            List<string> usersToUpdate = new List<string>();
            using (DirectorySearcher ds = new DirectorySearcher(rootDSE))
            {
                // if '>=' then we get last update twice
                // Note that the operators "<" and ">" are not supported. See "LDAP syntax filter clause"
                ds.Filter = string.Format("(&(objectClass=group)(uSNChanged>={0})(!(uSNChanged={0})))", prevHighUSN);
                ds.SizeLimit = 0; // unlimited
                ds.PageSize = 1000;
                ds.PropertiesToLoad.Add("distinguishedname");
                ds.PropertiesToLoad.Add("member");
                // looking for changed membership
                using (SearchResultCollection results = ds.FindAll())
                {
                    var cnt = results.Count;
                    if (results != null && cnt > 0)
                        foreach (SearchResult gr in results)
                        {
                            var dn = (string)gr.Properties["distinguishedname"][0];
                            var membersGr = gr.Properties["member"];
                            ISet<string> cachedMembers;
                            if (groupCache.TryGetValue(dn, out cachedMembers))
                            {
                                var members = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
                                foreach (string m in membersGr)
                                    members.Add(m);
                                if (!Utils.CheckEquals(cachedMembers, members))
                                {
                                    var usersDiff = cachedMembers.Except(members).Union(members.Except(cachedMembers)).ToArray();
                                    log.LogInfo("Membership changed in Group '" + dn + "' DiffMembers: " + string.Join(";", usersDiff.Take(5)));
                                    usersToUpdate.AddRange(usersDiff);
                                }
                                groupCache[dn] = members; // update cache
                            }
                        }
                }
            }

            foreach (var u in usersToUpdate.Distinct(StringComparer.OrdinalIgnoreCase))
            {
                LoadUsersByFilter(server, rootDSE, "(&(objectClass=user)(objectCategory=person)(distinguishedName=" + u + "))");
            }
        }

        private void LoadUsersByFilter(ADServer server, DirectoryEntry rootDSE, string filter)
        {
            using (DirectorySearcher ds = new DirectorySearcher(rootDSE))
            {
                // if '>=' then we get last update twice
                // Note that the operators "<" and ">" are not supported. See "LDAP syntax filter clause"
                ds.Filter = filter;
                ds.SizeLimit = 0; // unlimited
                ds.PageSize = 1000;
                IList<string> propsToLoad = server.SourceDest.StartsWith("source", StringComparison.OrdinalIgnoreCase) ? propNamesSource : propNamesDestination;

                foreach (var p in propsToLoad)
                    ds.PropertiesToLoad.Add(p);

                using (SearchResultCollection results = ds.FindAll())
                {
                    var cnt = results.Count;
                    if (results != null && cnt > 0)
                    {
                        log.LogInfo("Reading " + cnt + " account(s) from " + server.ToString() + ". Current USN='" + CurrentHighUSN + "'. InvocationID='" + GetInvocationID + "'");
                        foreach (SearchResult user in results)
                        {
#if DEBUG
                            //foreach (var p in user.Properties)
                            //{
                            //    var prop = (ResultPropertyValueCollection)((System.Collections.DictionaryEntry)p).Value;
                            //    var propVal = (prop.Count > 0) ? Convert.ToString(prop[0]) : null;
                            //    Console.WriteLine(((System.Collections.DictionaryEntry)p).Key + "=" + propVal);
                            //}
#endif
                            //var user = r.GetDirectoryEntry();
                            var objectSID = user.Properties.Contains("objectSID") ? (new SecurityIdentifier(((byte[])user.Properties["objectSID"][0]), 0)).ToString() : string.Empty;
                            var dnProp = user.Properties["distinguishedName"];
                            var dn = (dnProp.Count > 0) ? Convert.ToString(dnProp[0]) : null;

                            // simbols '{}' are special for Format. So replace them in DN.
                            if (cnt <= 20)
                                log.LogInfo(" Read samAccountName='" + user.Properties["samAccountName"][0] + "', objectSID='" + objectSID + "', DN='" + dn.Replace('{', '(').Replace('}', ')') + "'");
                            UserProperties props = new UserProperties(propsToLoad.Count, StringComparer.OrdinalIgnoreCase);

                            //var groups = GetGroups(domainCtx, (string)user.Properties["samAccountName"][0]);

                            foreach (var p in propsToLoad)
                            {
                                var prop = user.Properties[p];
                                if ("objectSID".Equals(p, StringComparison.OrdinalIgnoreCase))
                                    props.Add(p, new string[] { objectSID });
                                else if (prop.Count > 0)// multi-value support  ("memberof".Equals(p, StringComparison.OrdinalIgnoreCase))
                                {
                                    var stringColl = ConvertToStrings(prop);
                                    props.Add(p, stringColl);
                                }
                                else
                                    props.Add(p, null); // property is set to null in AD
                            }

                            usersProperties.Add(props);
                        }
                    }
                }
            }
        }

        private static DirectoryEntry LoadUserByObjectSID(DirectoryEntry searchOU, string objectSID, string samAccountName)
        {
            DirectoryEntry user = null;
            using (DirectorySearcher ds = new DirectorySearcher(searchOU))
            {
                // objectSID is the key stored in "Pager" attribute.
                ds.Filter = string.Format("(&(objectClass=user)(objectCategory=person)(Pager={0}))", objectSID);
                ds.PageSize = 100;
                // what properties we need:
                foreach (var p in propNamesDestination)
                    ds.PropertiesToLoad.Add(p);

                var res = ds.FindOne();
                if (res != null)  // if user is found by ObjectSID
                    user = res.GetDirectoryEntry(); // 
                else // case if ObjectSID is not initialized
                {
                    ds.Filter = string.Format("(&(objectClass=user)(objectCategory=person)(sAMAccountName={0}))", samAccountName);
                    res = ds.FindOne();
                    if (res != null && (res.Properties["Pager"].Count == 0 || string.IsNullOrEmpty((string)res.Properties["Pager"][0])))  // if user is exist and objectSID is not initialized
                        user = res.GetDirectoryEntry(); // found by samAccountName
                }
            }
            return user;
        }


        private static string[] ConvertToStrings(System.Collections.ICollection propCollection)
        {
            if (propCollection.Count > 0)// multi-value support 
            {
                var propVal = new string[propCollection.Count];
                int i = 0;
                foreach (var pV in propCollection)
                    propVal[i++] = Convert.ToString(pV);
                return propVal;
            }
            return null;
        }

        private static bool CheckEquals(PropertyValueCollection properties, string[] values)
        {
            if (values == null)
                return properties.Count == 0;
            if (properties.Count != values.Length)
                return false;
            foreach (var v in values)
                if (!properties.Contains(v))
                    return false;
            return true;
        }

        //public List<string> GetGroups(PrincipalContext domainCtx, string userName)
        //{
        //    List<string> result = new List<string>();

        //    // establish domain context
        //    //PrincipalContext yourDomain = new PrincipalContext(ContextType.Domain);

        //    // find your user
        //    UserPrincipal user = UserPrincipal.FindByIdentity(domainCtx, userName);

        //    // if found - grab its groups
        //    if (user != null)
        //    {
        //        PrincipalSearchResult<Principal> groups = user.GetAuthorizationGroups();

        //        // iterate over all groups
        //        foreach (Principal p in groups)
        //        {
        //            // make sure to add only group principals
        //            if (p is GroupPrincipal)
        //            {
        //                result.Add(p.DistinguishedName);
        //            }
        //        }
        //    }

        //    return result;
        //}

        /// <summary>
        /// function for converting oUser.Properties["USNChanged"].Value
        /// </summary>
        /// <param name="adsLargeInteger"></param>
        /// <returns></returns>
        public static Int64 ConvertADSLargeIntegerToInt64(object adsLargeInteger)
        {
            var highPart = (Int32)adsLargeInteger.GetType().InvokeMember("HighPart", System.Reflection.BindingFlags.GetProperty, null, adsLargeInteger, null);
            var lowPart = (Int32)adsLargeInteger.GetType().InvokeMember("LowPart", System.Reflection.BindingFlags.GetProperty, null, adsLargeInteger, null);
            return highPart * ((Int64)UInt32.MaxValue + 1) + lowPart;
        }

        /// <summary>
        /// Add or Update user in Destination AD
        /// return: 0  - succesfully updated 
        ///         1  - succesfully updated and changedImportantProps
        ///         -1 - error
        /// </summary>
        public static int AddUser(ADServer server, UserProperties newProps, string cprContent, ADServer[] serversToWait)
        {
            // props["userPrincipalName"], props["displayName"], props["givenName"], props["sn"], props["mail"], props["initials"], props["objectSID"]
            string samAccountName = newProps["samAccountName"][0];
            string oldSamAccountName = null;
            bool isNewUser = false;
            //bool isChangedOU = false;

            using (DirectoryEntry searchOU = new DirectoryEntry(server.path, server.ServerUserName, server.ServerPassword, server.authTypes))
            {
                DirectoryEntry oldUser = LoadUserByObjectSID(searchOU, newProps["objectSID"][0], samAccountName);

                UserProperties oldProps = null;
                if (oldUser != null)
                {
                    oldProps = GetUserProperties(oldUser);
                    oldSamAccountName = (string)oldUser.Properties["samAccountName"].Value;
                }
                isNewUser = oldUser == null;

                string transResult;
                var adHint = ADHintsConfigurationSection.GetOUByAttributes(newProps, oldProps, out transResult);
                if (adHint == null)
                {
                    var messageHint = "ADHint is not found for user " + quote(samAccountName) + ". Attributes:" + PrintAttributes(newProps);
                    messageHint += ADHintsConfigurationSection.PrintMemberOfAttributes(newProps.GetPropValue("memberOf"));

                    if (Settings.Default.DataMismatchLogging)
                        log.LogWarn(messageHint);
                    else
                        log.LogDebug(messageHint);
                    return 0;
                }

                var qualityCheck = adHint.QualityCheck(newProps);
                if (qualityCheck.Count > 0)
                {
                    log.LogError("QualityCheck validation fails for user '" + samAccountName + "' with attributes:" + Environment.NewLine + string.Join(Environment.NewLine, qualityCheck.ToArray()));
                    return 15;
                }

                // Process ...
                if (adHint.Type == ADHintElement.AdHintType.Terminate)
                    return CCMApi.Terminate(oldSamAccountName); // return 0 if success

                string destPath = server.path + "/" + adHint.DestOU;  // ="LDAP://myServ.local:636/OU=Office21,OU=Office2,OU=Domain Controllers,DC=myServ,DC=local"

                using (var destOU = new DirectoryEntry(destPath, server.ServerUserName, server.ServerPassword, server.authTypes))
                {
                    string changedImportantProps = transResult;
                    string changedAllProps = "";
                    string msg = " '" + samAccountName + "' . HintNum=" + adHint.Num + ". " + adHint.Type + ". ";
                    if (isNewUser) // new user
                    {
                        //log.LogInfo(" '" + samAccountName + "' . HintNum=" + adHint.Num + ". New user in '" + destOU.Path + "'" + (server.SSL ? "(SSL)" : "(not SSL)") + " ...");
                        msg += "Add user into '" + destOU.Path + "'" + (server.SSL ? "(SSL)" : "(not SSL)");
                        oldUser = destOU.Children.Add("CN=" + samAccountName, "user");
                    }
                    else
                    {
                        // check if OU is changed for user 
                        if (!oldUser.Parent.Path.Equals(destOU.Path, StringComparison.OrdinalIgnoreCase))
                        {
                            //log.LogInfo(" '" + samAccountName + "' move from '" + oldUser.Parent.Path + "' -> '" + destOU.Path + "' " + (server.SSL ? "(SSL)" : "(not SSL)") + ". HintNum=" + adHint.Num + " ...");
                            msg += "Move from '" + oldUser.Parent.Path + "' -> '" + destOU.Path + "' " + (server.SSL ? "(SSL)" : "(not SSL)");
                            oldUser.MoveTo(destOU);
                            changedAllProps += "OU;";
                        }
                        else
                            //log.LogInfo(" '" + samAccountName + "' update in OU '" + destOU.Path + "' " + (server.SSL ? "(SSL)" : "(not SSL)") + ". HintNum=" + adHint.Num + " ...");
                            msg += "Update in OU '" + destOU.Path + "' " + (server.SSL ? "(SSL)" : "(not SSL)");
                    }

                    changedImportantProps += CheckAndSetProperty(oldUser.Properties, "samAccountName", new string[] { samAccountName }); // AD key

                    changedAllProps += SetPropertiesToUser(oldUser, newProps);
                    changedAllProps += CheckAndSetProperty(oldUser.Properties, "Pager", newProps["objectSID"]); // the surrogate key 

                    bool terminate = !isNewUser && changedImportantProps.Length > 0;
                    if (terminate)
                    {
                        log.LogInfo(msg + ". Terminating. Changed attributes: " + Environment.NewLine + changedAllProps + Environment.NewLine + changedImportantProps);
                        CCMApi.Terminate(oldSamAccountName);
                    }
                    else if (!isNewUser && string.IsNullOrEmpty(changedAllProps) && string.IsNullOrEmpty(changedImportantProps))
                    {
                        log.LogDebug("No attributes changed. Skip updating.");
                        return 0; // skip to commit
                    }
                    else
                        log.LogInfo(msg + "Changed attributes: " + Environment.NewLine + changedAllProps + Environment.NewLine + changedImportantProps);

                    if(!terminate && CCMApi.IsActive(samAccountName) != 0)
                        return -1; // card is already active

                    oldUser.Properties["userAccountControl"].Value = Utils.UserAccountControl.NORMAL_ACCOUNT | Utils.UserAccountControl.ACCOUNTDISABLE | Utils.UserAccountControl.PWD_NOTREQD;

                    //Console.WriteLine("  CommitChanges '" + samAccountName + "' ...");
                    oldUser.CommitChanges();

                    if (isNewUser)
                        WaitDestinationADReplication(samAccountName, newProps["objectSID"][0], serversToWait);

                    // Dest AD is commited.
                    // Process CCM ....

                    if (Settings.Default.CCMHost.Trim().Length == 0)
                    {
                        log.LogWarn("CCMHost is not set. Skip CCM processing");
                        return 0;
                    }

                    int ccmResult;
                    try
                    {
                        ccmResult = CCMApi.CreateCPR(samAccountName, cprContent, adHint.CardPolicy); // return 0 if success
                    }
                    catch (Exception ex)
                    {
                        log.LogError(ex, "update user '" + samAccountName + "' in CCM: " + ex.Message);
                        return -1;
                    }

                    if (ccmResult != 0)
                    {
                        // Rollback AD ...
                        if (oldProps == null) // if created then delete
                        {
                            destOU.Children.Remove(oldUser);
                            destOU.CommitChanges();
                        }
                        else
                        {
                            SetPropertiesToUser(oldUser, oldProps); // return to oldProps
                            oldUser.CommitChanges();
                        }
                        log.LogWarn2("Rollback processed in AD for account '" + samAccountName + "' due to CCM error " + ccmResult + ". " + (oldProps == null ? "Account deleted in DestinationAD" : "Account attributes restored in DestinationAD"));
                        return -1;
                    }
                    return ccmResult;
                }
            }
        }

        /// <summary>
        /// Check that the account exists in all secondary destination AD. Only after the account exists in all others Dest AD then send in the CPR to CMS.
        /// </summary>
        /// <param name="samAccountName"></param>
        /// <param name="objectSID"></param>
        /// <param name="serversToWait"></param>
        private static void WaitDestinationADReplication(string samAccountName, string objectSID, ADServer[] serversToWait)
        {
            if (serversToWait.Length == 0)
                return;
            List<string> serversSucces = new List<string>();
            while (true)
            {
                List<string> serversFailed = new List<string>();

                foreach (var server in serversToWait)
                {
                    if (serversSucces.Contains(server.Name))
                        continue;
                    try
                    {
                        using (DirectoryEntry searchOU = new DirectoryEntry(server.path, server.ServerUserName, server.ServerPassword, server.authTypes))
                            if (LoadUserByObjectSID(searchOU, objectSID, samAccountName) == null)
                                serversFailed.Add(server.Name);
                            else
                            {
                                serversSucces.Add(server.Name);
                                serversFailed.Remove(server.Name);
                            }
                    }
                    catch (Exception ex)
                    {
                        if (ex.HResult != -2147016646) // server is not operational
                            serversFailed.Add(server.Name); // don't wait downed servers 
                        log.LogDebug("Error in WaitDestinationADReplication for server '" + server.Name + "': " + ex.Message);
                    }
                }
                if (serversFailed.Count == 0)
                    break; // AD replication seems to be complete

                log.LogInfo(" Waiting for user '" + samAccountName + "' to be created on server(s): " + string.Join("; ", serversFailed.ToArray()));
                Thread.Sleep(3000);
            }

            log.LogDebug(" User '" + samAccountName + "' was found on servers: " + string.Join(";", serversSucces.ToArray()));
        }

        private static string SetPropertiesToUser(DirectoryEntry destUser, UserProperties newProps)
        {
            string changedProps = "";
            foreach (var prop in propNamesDestination)
            {
                string[] newValue;
                if (!propIgnoreDest.Contains(prop))
                    if (newProps.TryGetValue(prop, out newValue))
                        changedProps += CheckAndSetProperty(destUser.Properties, prop, newValue, true);
                //else
                //    changedProps += CheckAndSetProperty(destUser.Properties, prop, null, true);
            }
            return changedProps;
        }

        private static string PrintAttributes(UserProperties props)
        {
            string output = string.Empty;
            foreach (var dict in props)
            {
                if (dict.Key.Equals("memberof", StringComparison.OrdinalIgnoreCase))
                    continue; // exclude printing all user groups. Later we will print only groups from config.
                else if (dict.Key.Equals("userAccountControl", StringComparison.OrdinalIgnoreCase))
                {
                    uint uaControl = 0;
                    UInt32.TryParse(dict.Value[0], out uaControl);
                    List<string> uaControlFlags = new List<string>();
                    foreach (Utils.UserAccountControl enumUa in Enum.GetValues(typeof(Utils.UserAccountControl)))
                        if ((uaControl & Convert.ToUInt32(enumUa)) != 0
                            && enumUa != Utils.UserAccountControl.NORMAL_ACCOUNT && enumUa != Utils.UserAccountControl.PWD_NOTREQD)
                            uaControlFlags.Add(enumUa.ToString());
                    output += Environment.NewLine + dict.Key + "=" + string.Join("|", uaControlFlags) + ";";
                }
                else
                    output += Environment.NewLine + dict.Key + "=" + (dict.Value == null || dict.Value.Length == 0 ? "NULL" : dict.Value[0]) + ";";
            }
            // AD didn't send me a null values. So force to print nulls
            foreach (var missedProp in propNamesSource.Except(props.Keys))
            {
                if (missedProp.Equals("memberof", StringComparison.OrdinalIgnoreCase))
                    continue; // exclude printing all user groups. Later we will print only groups from config.
                output += Environment.NewLine + missedProp + "=NULL;";
            }
            return output;
        }

        private static UserProperties GetUserProperties(DirectoryEntry user)
        {
            var props = new UserProperties(propNamesDestination.Count, StringComparer.OrdinalIgnoreCase);
            foreach (var p in propNamesDestination)
            {
                var prop = user.Properties[p];
                var strings = ConvertToStrings(prop);
                if (strings != null)
                    props.Add(p, strings);
            }
            return props;
        }


        private static string CheckAndSetProperty(PropertyCollection property, string propName, string[] newValue, bool supressLog = false)
        {
            //if (newValue == null || newValue.Length == 0) // (newValue is string && string.IsNullOrEmpty((string)newValue))) // to avoid Constraint Violation for new user. Don't set null to property.
            //    return "";
            try
            {
                var prop = property[propName];
                // check if property is equal to newValue. If not then set property = newValue.
                //if (newValue is string && !((string)newValue).Equals((string)prop.Value, StringComparison.OrdinalIgnoreCase)
                //    || (!(newValue is string) && !Equals(newValue, prop.Value)))
                if (!CheckEquals(prop, newValue))
                {
                    string res = "[" + propName + "]=" + (prop.Value == null ? "null" : quote(prop.Value)) + " -> " + (newValue != null && newValue.Length > 0 ? quote(newValue[0]) : "null") + "; ";
                    //log.LogDebug("   " + res);
                    if (newValue == null || newValue.Length == 0)
                        prop.Clear();
                    else
                        prop.Value = newValue;
                    return res;
                }
            }
            catch (Exception ex)
            {
                log.LogError(ex, "Set attribute name [" + propName + "] with new value = " + quote(newValue));
            }
            return "";
        }

        private static string quote<T>(T txt)
        {
            return "'" + Convert.ToString(txt) + "'";
        }

        public string GetInvocationID
        {
            get { return workInvocationID; }
        }
        public string CurrentHighUSN
        {
            get { return currentHighUSN; }
        }
        public string DnsHostName
        {
            get { return dnsHostName; }
        }

        public List<UserProperties> ChangedUsersProperties
        {
            get { return usersProperties; }
        }


        internal static IDictionary<string, ISet<string>> GetGroupMembers(ADServer server, ISet<string> groupsFilter)
        {
            groupCache = new Dictionary<string, ISet<string>>();
            using (DirectoryEntry rootDSE = new DirectoryEntry(server.path, server.ServerUserName, server.ServerPassword, server.authTypes))
            {
                using (DirectorySearcher ds = new DirectorySearcher(rootDSE))
                {
                    ds.Filter = "(&(objectClass=group))";
                    ds.SizeLimit = 0; // unlimited
                    ds.PageSize = 1000;
                    ds.PropertiesToLoad.Add("distinguishedname");
                    ds.PropertiesToLoad.Add("member");

                    using (SearchResultCollection results = ds.FindAll())
                    {
                        var cnt = results.Count;
                        if (results != null && cnt > 0)
                        {
                            //log.LogInfo("Reading " + cnt + " account(s) from " + server.ToString() + ". Current USN='" + CurrentHighUSN + "'. InvocationID='" + GetInvocationID + "'");
                            foreach (SearchResult gr in results)
                            {
                                var dn = (string)gr.Properties["distinguishedname"][0];
                                var membersGr = gr.Properties["member"];
                                if (groupsFilter.Contains(dn)) // only wanted groups needed
                                {
                                    var members = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
                                    foreach (string m in membersGr)
                                        members.Add(m);
                                    groupCache.Add(dn, members);
                                }
#if DEBUG
                                //foreach (var p in gr.Properties)
                                //{
                                //    var prop = (ResultPropertyValueCollection)((System.Collections.DictionaryEntry)p).Value;
                                //    var propVal = (prop.Count > 0) ? Convert.ToString(prop[0]) : null;
                                //    Console.WriteLine(((System.Collections.DictionaryEntry)p).Key + "=" + propVal);
                                //}
#endif
                            }
                        }
                    }
                }
            }
            return groupCache;
        }
    }
}
