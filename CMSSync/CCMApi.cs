using AdPoolService.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Cmssync
{
    static class CCMApi
    {
        public static ILog log { set; private get; } 


        [DllImport("ccmapi2.dll", EntryPoint = "createCPR", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int createCPR(
            //String	cmd, //= args[0]; // CREATE-CPR
            String host, //= args[1]; // 192.168.214.134
            String port, //= args[2]; // 32456 
            String sertSerial,
            String sertIssuer,
            //String	client, //= args[3]; // client.pfx
            //String	pwd, //= args[4]; // actividentity
            //String	CA, //= args[5]; // root.cer
            String user, //= args[6]; // "John G. Doe"
            String reader, //= args[7]; // "CPR 2.1.8.sample.signed.xml"
            String pin, //= args[8]; // ""
            String policy, //= args[9]; // "F2F" or "test6"
            String reason //= args[10]; // ""		    
        );

        [DllImport("ccmapi2.dll", EntryPoint = "TerminateAll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int TerminateAll(
            String host,        // 192.168.214.134
            String port,        // 32456 
            String sertSerial,
            String sertIssuer,
            String user
        );
        [DllImport("ccmapi2.dll", EntryPoint = "CheckHealth", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CheckHealth(
            String host,        // 192.168.214.134
            String port,        // 32456 
            String sertSerial,
            String sertIssuer
        );

        public static int Terminate(string samAccount)
        {
            log.LogInfo("Terminate user in CCM '{0}': samAccountName='{1}' ...", Settings.Default.CCMHost, samAccount);
            int res = 0;
            if (Settings.Default.CCMHost.Trim().Length != 0)
            {
                res = CCMApi.TerminateAll(Settings.Default.CCMHost, Settings.Default.CCMPort,
                        Settings.Default.CCMCertificateSerial.Trim().ToLower(), // CMS required lowcase letters !
                        Settings.Default.CCMCertificateIssuer.Trim(),
                        samAccount);
                if (res == 0)
                    log.LogInfo("Terminate user '" + samAccount + "' in CCM complete successfully");
                else
                    log.LogError("Terminate user '" + samAccount + "' in CCM complete with error code = " + res);
            }
            else
                log.LogWarn("CCMHost is not set. Skip CCM processing");
            return res;
        }
        public static int CreateCPR(
            String user, //= args[6]; // "John G. Doe"
            String cprContent, //= args[7]; // "CPR 2.1.8.sample.signed.xml"
            String cardPolicy //= args[9]; // "F2F" or "test6"
        )
        {
            try
            {
                log.LogInfo("update user in CCM '{0}': samAccountName='{1}', CardPolicy='{2}' ...", Settings.Default.CCMHost, user, cardPolicy);

                var res = CCMApi.createCPR(Settings.Default.CCMHost, Settings.Default.CCMPort,
                    Settings.Default.CCMCertificateSerial.Trim().ToLower(), // CMS required lowcase letters !
                    Settings.Default.CCMCertificateIssuer.Trim(),
                    user,
                    cprContent, // Settings.Default.CCMCPRFile, // "CPR 2.1.8.sample.signed.xml",
                    "",
                    cardPolicy, // "test6", // "F2F"
                    ""
                    );
                if (res == 0)
                    log.LogInfo("update user '" + user + "' in CCM complete successfully");
                else
                    log.LogError("update user '" + user + "' in CCM complete with error code = " + res);
                return res;
            }
            catch (Exception ex)
            {
                log.LogError(ex, "update user '" + user + "' in CCM: " + ex.Message);
            }
            return -9;
        }
    }
}
