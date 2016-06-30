using System;
using System.Diagnostics;

namespace AdPoolService.Logging
{
    public class WinEventLog : LogBase {
        private readonly string sourceName;

        public WinEventLog(string sourceName) {
            this.sourceName = sourceName;
            if (sourceName == null) {
                throw new ArgumentNullException("sourceName");
            }

            if (!EventLog.SourceExists(sourceName)) {
                EventLog.CreateEventSource(sourceName, "Application");
            }
        }

        protected override void WriteMessage(Severity severity, string message) {
            switch (severity) {
                case Severity.Fatal:
                    EventLog.WriteEntry(sourceName, message, EventLogEntryType.Error, 9);
                    break;
                case Severity.Error:
                    EventLog.WriteEntry(sourceName, message, EventLogEntryType.Error, 15);
                    break;
                case Severity.Warn:
                    EventLog.WriteEntry(sourceName, message, EventLogEntryType.Warning, 150);
                    break;
                case Severity.Warn2:
                    EventLog.WriteEntry(sourceName, message, EventLogEntryType.Warning, 151);
                    break;
                case Severity.Info:
                    EventLog.WriteEntry(sourceName, message, EventLogEntryType.Information, 1500);
                    break;
            }
        }
    }
}