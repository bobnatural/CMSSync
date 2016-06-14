using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdPoolService.Logging
{
    class ConsoleLog: LogBase {
    
        public ConsoleLog()
        {
        }

        protected override void WriteMessage(Severity severity, string message) {
            switch (severity) {
                case Severity.Error:
                case Severity.Fatal:
                    Console.Error.WriteLine(message);
                    break;
                default:
                    Console.Out.WriteLine(message);
                    break;
            }
        }
    }
}
