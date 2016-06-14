using System;
using AdPoolService.Extensions;

namespace AdPoolService.HealthCheck {
    public sealed class CheckResult {
        private readonly string failureMessage;
        private string warningMessage;

        private CheckResult(string failureMessage, bool logsWritten = false) {
            this.failureMessage = failureMessage;
            this.LogsWritten = true;
        }

        public static CheckResult Success {
            get { return new CheckResult(string.Empty); }
        }

        public bool IsSuccess {
            get { return string.IsNullOrEmpty(failureMessage); }
        }

        public bool LogsWritten { get; set; }

        public string FailureMessage {
            get {
                if (IsSuccess) {
                    throw new InvalidOperationException("Cannot get failure message for success result");
                }

                return failureMessage;
            }
        }
        public string Message
        {
            get
            {
                return IsSuccess ? warningMessage: failureMessage;
            }
        }

        public static CheckResult FailWith(string message, params object[] args)
        {
            return FailWith(false, message, args);
        }

        public static CheckResult FailWith(bool logsWritten, string message, params object[] args) {
            if (string.IsNullOrEmpty(message)) {
                throw new ArgumentNullException("message");
            }
            return new CheckResult(message.Fmt(args), logsWritten);
        }
        public static CheckResult WarningWith(string message, params object[] args)
        {
            if (string.IsNullOrEmpty(message))
            {
                throw new ArgumentNullException("message");
            }
            var res = new CheckResult("", false);
            res.warningMessage = message.Fmt(args);
            return res;
        }
    }
}