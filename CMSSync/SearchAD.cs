using AdPoolService.Logging;
using Cmssync;
using System;
using System.Collections.Generic;
using System.DirectoryServices;
using System.DirectoryServices.Protocols;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace AdPoolService
{
    class PollAD
    {
        private List<IDictionary<string, string>> usersProperties; // changed users

        private string workInvocationID; // database ID that was used in search
        private string currentHighUSN;
        private string dnsHostName;
        private string dsServiceName;

        private static ILog log = new NullLog();

        // what properties we need from SourceAD (Pager is needed in initialization DestAD to compare with ObjectSID)
        static readonly ISet<string> propLoadHard = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "samAccountName", "displayName", "givenName", "sn", "cn", "distinguishedName", "userPrincipalName", "initials", "mail", "uSNChanged", "objectSID", "Pager" };
        // ignore to update Destination:
        public static readonly ISet<string> propIgnoreDest = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "objectSID", "userAccountControl", "password", "pager", "uSNChanged", "distinguishedName", "cn" };
        public static List<string> propNamesAll; // hard + hint + transition props for SourceAD
        public static List<string> propNamesDestination; // hard + transition props for DestAD

        static readonly int NORMAL_ACCOUNT = 0x200; // normal account
        static readonly int PWD_NOTREQD = 0x20; // password not required
        static readonly int DISABLED = 0x002; //account disabled


        static public void AddSourcePropNames(string[] addProps)
        {
            if (propNamesAll == null)
                propNamesAll = new List<string>(propLoadHard);
            propNamesAll.AddRange(addProps);
            propNamesAll = propNamesAll.Distinct(StringComparer.OrdinalIgnoreCase).ToList();
        }
        static public void AddDestinationPropNames(string[] addProps)
        {
            if (propNamesDestination == null)
                propNamesDestination = new List<string>(propLoadHard);
            propNamesDestination.AddRange(addProps);
            propNamesDestination = propNamesDestination.Distinct(StringComparer.OrdinalIgnoreCase).ToList();
        }

        public static ILog Log
        {
            set { log = value; }
        }

        public PollAD(ADServer server, IDictionary<string, string> prevHighUSNs, bool supressLog = false)
        {
            usersProperties = new List<IDictionary<string, string>>();
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
                //Console.WriteLine(" dsServiceName: " + dsServiceName);                
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
                    if (prevHighUSNs == null)
                        prevHighUSN = "0"; // initialize mode. Load All accounts.
                    else if (!prevHighUSNs.TryGetValue(workInvocationID, out prevHighUSN))
                        return;
#if DEBUG
                    //foreach (var prop in dcService.SchemaEntry.Properties)
                    //{
                    //    string propName = prop.ToString();
                    //    var propValue = dcService.SchemaEntry.Properties[propName].Value;
                    //    Console.WriteLine(propName + "=" + propValue);
                    //}
#endif
                }
                
                log.LogDebug(server.SourceDest + " AD '" + server.Name + "' " + (server.SSL ? "(SSL)" : "(not SSL)") + " currentUSN=" + currentHighUSN);

                if (String.IsNullOrEmpty(prevHighUSN))
                {
                    log.LogWarn("FIRST launch. Skip polling. Set currentUSN=" + currentHighUSN);
                    return; // first launch. Just set currentHighUSN and return.
                }

                if (Equals(prevHighUSN, currentHighUSN))
                    return; // no changes since last update

                if (prevHighUSN == "0")
                    log.LogWarn("Load all users from " + server.Name + " ...");

                using (DirectorySearcher ds = new DirectorySearcher(rootDSE))
                {
                    // if '>=' then we get last update twice
                    // Note that the operators "<" and ">" are not supported. See "LDAP syntax filter clause"
                    ds.Filter = string.Format("(&(objectClass=user)(objectCategory=person)(uSNChanged>={0})(!(uSNChanged={0})))", prevHighUSN);
                    ds.SizeLimit = 0; // unlimited
                    ds.PageSize = 1000;
                    if (server.SourceDest.StartsWith("source", StringComparison.OrdinalIgnoreCase))
                        foreach (var p in propNamesAll)
                            ds.PropertiesToLoad.Add(p);
                    else
                        foreach (var p in propNamesDestination)
                            ds.PropertiesToLoad.Add(p);

                    using (SearchResultCollection results = ds.FindAll())
                    {
                        var cnt = results.Count;
                        if (results != null && cnt > 0)
                        {
                            log.LogInfo("Reading " + cnt + " account(s) from " + server.SourceDest + " AD '" + server.Name + "' " + (server.SSL ? "(SSL)" : "(not SSL)") + ". Current USN='" + CurrentHighUSN + "'. InvocationID='" + GetInvocationID + "'");
                            foreach (SearchResult user in results)
                            {
                                //var user = r.GetDirectoryEntry();
                                var objectSID = (new SecurityIdentifier(((byte[])user.Properties["objectSID"][0]), 0)).ToString();
                                var dnProp = user.Properties["distinguishedName"];
                                var dn = (dnProp.Count > 0) ? Convert.ToString(dnProp[0]) : null;

                                // simbols '{}' are special for Format. So replace them in DN.
                                if (cnt <= 20)
                                    log.LogInfo(" Read samAccountName='" + user.Properties["samAccountName"][0] + "', objectSID='" + objectSID + "', DN='" + dn.Replace('{', '(').Replace('}', ')') + "'");
                                var props = new Dictionary<string, string>(propNamesAll.Count, StringComparer.OrdinalIgnoreCase);

                                //foreach (var p in user.Properties)
                                //{
                                //    var prop = (ResultPropertyValueCollection)((System.Collections.DictionaryEntry)p).Value;
                                //    var propVal = (prop.Count > 0) ? Convert.ToString(prop[0]) : null;
                                //    Console.WriteLine(((System.Collections.DictionaryEntry)p).Key + "=" + propVal);
                                //}

                                foreach (var p in propNamesAll)
                                {
                                    var prop = user.Properties[p];
                                    var propVal = (prop.Count > 0) ? Convert.ToString(prop[0]) : null;
                                    if ("objectSID".Equals(p, StringComparison.OrdinalIgnoreCase))
                                        propVal = objectSID;
                                    props.Add(p, propVal);
                                }

                                usersProperties.Add(props);
                            }
                        }
                    }
                }
                return;
            }
        }

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
        /// </summary>
        public static bool AddUser(ADServer server, string OU, IDictionary<string, string> newProps, //string samAccountName, string userPrincipalName, string displayName, string givenName, string sn, string email, string initials, string objectSID, 
            Func<IDictionary<string, string>, IDictionary<string, string>, string> CheckTransition, string hintNum)
        {
            // props["userPrincipalName"], props["displayName"], props["givenName"], props["sn"], props["mail"], props["initials"], props["objectSID"]
            string samAccountName = newProps["samAccountName"];
                       
            string destPath = server.path + "/" + OU;
            // DestPath =LDAP://myServ.local:636/CN=Users,DC=myServ,DC=local
            // or       =LDAP://myServ.local:636/OU=Office21,OU=Office2,OU=Domain Controllers,DC=myServ,DC=local

            using (DirectoryEntry searchOU = new DirectoryEntry(server.path, server.ServerUserName, server.ServerPassword, server.authTypes))
            {
                DirectoryEntry user = null;
                using (DirectorySearcher ds = new DirectorySearcher(searchOU))
                {
                    // objectSID is the key stored in "Pager" attribute.
                    ds.Filter = string.Format("(&(objectClass=user)(objectCategory=person)(Pager={0}))", newProps["objectSID"]);
                    ds.PageSize = 100;
                    // what properties we need:
                    foreach (var p in propNamesDestination)
                        ds.PropertiesToLoad.Add(p);

                    var res = ds.FindOne();
                    if (res != null)  // if user is exist
                        user = res.GetDirectoryEntry(); // then update
                    else // case if ObjectSID is not initialized
                    {
                        ds.Filter = string.Format("(&(objectClass=user)(objectCategory=person)(sAMAccountName={0}))", samAccountName);
                        res = ds.FindOne();
                        if (res != null && (res.Properties["Pager"].Count == 0 || string.IsNullOrEmpty((string)res.Properties["Pager"][0])))  // if user is exist and objectSID is not initialized
                            user = res.GetDirectoryEntry(); // then update
                    }
                }

                using (var destOU = new DirectoryEntry(destPath, server.ServerUserName, server.ServerPassword, server.authTypes))
                {
                    bool newUser = false;
                    if (user == null) // new user
                    {
                        newUser = true;
                        log.LogInfo(" '" + samAccountName + "' add into OU '" + destOU.Path + "'" + (server.SSL ? "(SSL)" : "(not SSL)") + ". HintNum=" + hintNum + " ...");
                        user = destOU.Children.Add("CN=" + samAccountName, "user");
                    }
                    else
                    {
                        // check if OU is changed for user 
                        if (!user.Parent.Path.Equals(destOU.Path, StringComparison.OrdinalIgnoreCase))
                        {
                            log.LogInfo(" '" + samAccountName + "' move from '" + user.Parent.Path + "' -> '" + destOU.Path + "' " + (server.SSL ? "(SSL)" : "(not SSL)") + ". HintNum=" + hintNum + " ...");
                            user.MoveTo(destOU);
                        }
                        else
                            log.LogInfo(" '" + samAccountName + "' update in OU '" + destOU.Path + "' " + (server.SSL ? "(SSL)" : "(not SSL)") + ". HintNum=" + hintNum + " ...");
                    }

                    string changedImportantProps = "";
                    if (!newUser)
                    {
                        var OldProps = new Dictionary<string, string>(propNamesDestination.Count, StringComparer.OrdinalIgnoreCase);
                        foreach (var p in propNamesDestination)
                        {
                            var prop = user.Properties[p];
                            var propVal = (prop.Count > 0) ? Convert.ToString(prop[0]) : null;
                            OldProps.Add(p, propVal);
                        }
                        changedImportantProps += CheckTransition(OldProps, newProps);
                    }

                    string prevSamAccountName = (string)user.Properties["samAccountName"].Value;
                    changedImportantProps += CheckAndSetProperty(user.Properties, "samAccountName", samAccountName); // AD key
                    if (!newUser && changedImportantProps.Length > 0)
                    {
                        log.LogInfo("Changed account attributes: " + changedImportantProps + "Terminating '" + prevSamAccountName + "' in CCM ...");
                        CCMApi.Terminate(prevSamAccountName);
                    }

                    foreach (var prop in propNamesDestination)
                    {
                        string newValue;
                        if (!propIgnoreDest.Contains(prop)
                            && newProps.TryGetValue(prop, out newValue))
                            CheckAndSetProperty(user.Properties, prop, newValue, newUser);
                    }

                    //CheckAndSetProperty(user.Properties, "givenName", newProps["givenName"]); // FirstName
                    //CheckAndSetProperty(user.Properties, "sn", newProps["sn"]); // LastName
                    //CheckAndSetProperty(user.Properties, "initials", newProps["initials"]); // midname
                    //CheckAndSetProperty(user.Properties, "mail", newProps["mail"]);
                    //CheckAndSetProperty(user.Properties, "displayName", newProps["displayName"]);
                    //CheckAndSetProperty(user.Properties, "userPrincipalName", newProps["userPrincipalName"]);
                    CheckAndSetProperty(user.Properties, "Pager", newProps["objectSID"]); // the surrogate key 
                    user.Properties["userAccountControl"].Value = NORMAL_ACCOUNT | DISABLED | PWD_NOTREQD;
                    //Console.WriteLine("  CommitChanges '" + samAccountName + "' ...");
                    user.CommitChanges();

                    return changedImportantProps.Length > 0 || newUser; // signal for notify CMS or skip
                }
            }
        }

        private static string CheckAndSetProperty(PropertyCollection property, string propName, object newValue, bool supressLog = false)
        {
            if (newValue == null || (newValue is string && string.IsNullOrEmpty((string)newValue))) // to avoid Constraint Violation for new user. Don't set null to property.
                return "";
            try
            {
                var prop = property[propName];
                if (newValue is string && !((string)newValue).Equals((string)prop.Value, StringComparison.OrdinalIgnoreCase)
                    || (!(newValue is string) && !Equals(newValue, prop.Value)))
                {
                    string res = "[" + propName + "]='" + prop.Value + "' -> '" + newValue + "'; ";
                    if (!supressLog) log.LogDebug("   " + res); 
                    prop.Value = newValue;
                    return res;
                }
            }
            catch(Exception ex)
            {
                log.LogError(ex, "Set attribute name [" + propName + "] with new value = '" + newValue + "'");
            }
            return "";
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

        public List<IDictionary<string, string>> ChangedUsersProperties
        {
            get { return usersProperties; }
        }

        internal void InitializeDestination(ADServer server)
        {

        }
    }
}
