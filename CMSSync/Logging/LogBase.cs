using System;
using AdPoolService.Extensions;

namespace AdPoolService.Logging
{
    public abstract class LogBase : ILog {
        public enum Severity {
            Debug,
            Info,
            Warn,
            Warn2, 
            Error,
            Fatal
        }

        public virtual void LogFatal(Exception exception, string message, params object[] args) {
            LogFatal("{0}\r\n{1}".Fmt(message.Fmt(args), exception.ToString()));
            if (exception.InnerException != null)
                LogFatal("\r\nInner Exception: {0}", exception.InnerException.Message);
        }

        public virtual void LogError(Exception exception, string message, params object[] args) {
            LogError("{0}\r\n{1}".Fmt(message.Fmt(args), exception.ToString()));
            if (exception.InnerException != null)
                LogError("\r\nInner Exception: {0}", exception.InnerException.Message);
        }

        public virtual void LogFatal(string message, params object[] args) {
            WriteMessage(Severity.Fatal, message.Fmt(args));
        }

        public virtual void LogError(string message, params object[] args) {
            WriteMessage(Severity.Error, message.Fmt(args));
        }

        public virtual void LogWarn(string message, params object[] args) {
            WriteMessage(Severity.Warn, message.Fmt(args));
        }
        public virtual void LogWarn2(string message, params object[] args)
        {
            WriteMessage(Severity.Warn2, message.Fmt(args));
        }

        public virtual void LogInfo(string message, params object[] args) {
            WriteMessage(Severity.Info, message.Fmt(args));
        }

        public virtual void LogDebug(string message, params object[] args) {
            WriteMessage(Severity.Debug, message.Fmt(args));
        }

        protected abstract void WriteMessage(Severity severity, string message);
    }
}