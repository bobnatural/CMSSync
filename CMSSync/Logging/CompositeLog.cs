using System;
using System.Collections.Generic;

namespace AdPoolService.Logging
{
    public sealed class CompositeLog : ILog {
        private readonly IEnumerable<ILog> children;

        public CompositeLog(params Func<ILog>[] logFactories) {
            var c = new List<ILog>();

            foreach (var factory in logFactories) {
                try {
                    c.Add(factory());
                } catch (Exception e) {
                    foreach (var log in c) {
                        log.LogFatal(e, e.Message);
                    }
                }
            }

            children = c;
        }

        public void AddLog(ILog log)
        {
            ((List<ILog>)children).Add(log);
        }

        private void DoForAllChildren(Action<ILog> action) {
            foreach (var log in children) {
                try {
                    action(log);
// ReSharper disable once EmptyGeneralCatchClause
                } catch { }
            }
        }
        public void LogFatal(Exception exception, string message, params object[] args) {
            DoForAllChildren(l => l.LogFatal(exception, message, args));
        }

        public void LogError(Exception exception, string message, params object[] args) {
            DoForAllChildren(l => l.LogError(exception, message, args));
        }

        public void LogFatal(string message, params object[] args) {
            DoForAllChildren(l => l.LogFatal(message, args));
        }

        public void LogError(string message, params object[] args) {
            DoForAllChildren(l => l.LogError(message, args));
        }

        public void LogWarn(string message, params object[] args) {
            DoForAllChildren(l => l.LogWarn(message, args));
        }
        public void LogWarn2(string message, params object[] args)
        {
            DoForAllChildren(l => l.LogWarn2(message, args));
        }

        public void LogInfo(string message, params object[] args) {
            DoForAllChildren(l => l.LogInfo(message, args));
        }

        public void LogDebug(string message, params object[] args) {
            DoForAllChildren(l => l.LogDebug(message, args));
        }
    }
}
