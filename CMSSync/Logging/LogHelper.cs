using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace AdPoolService.Logging
{
    public class LogHelper
    {
        private static EventLog _logger;

        public static int EVT_SOURCE_FOLDER_UNAVAILABLE = 101;
        public static int EVT_BITS_UNAVAILABLE = 102;
        public static int EVT_BITS_ACCESS = 103;
        public static int EVT_LOCAL_UNAVAILABLE = 104;
        public static int EVT_NEW_CRL = 105;
        public static int EVT_SUCCESS = 106;
        public static int EVT_FAILED_UPLOAD = 107;
        public static int EVT_ERROR_CONFIG = 108;
        public static int EVT_FLUSH_NEW = 109;
        public static int EVT_STARTUP_DETECT = 110;
        public static int EVT_FAILSAFE_DETECT = 111;
        public static int EVT_FAILSAFE_UNAVAILABLE = 112;
        public static int EVT_FAILED_HB = 120;

        public static int EVT_VERBOSE = 9000;

        public static EventLog GetLogger()
        {
            if (_logger == null)
            {
                _logger = new EventLog { Source = "CMSSync", Log = "Application", EnableRaisingEvents = true };

                if (!EventLog.SourceExists("CMSSync"))
                {
                    EventLog.CreateEventSource("CMSSync", "Application");
                }
            }

            return _logger;
        }
    }
}
