using System;

namespace AdPoolService.Logging {
    public interface ILog {
        void LogFatal(Exception exception, string message, params object[] args);
        void LogError(Exception exception, string message, params object[] args);

        void LogFatal(string message, params object[] args);
        void LogError(string message, params object[] args);

        void LogWarn(string message, params object[] args);
        void LogInfo(string message, params object[] args);
        void LogDebug(string message, params object[] args);
    }
}