using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdPoolService
{
    public sealed class ServiceStartupException : Exception
    {
        public ServiceStartupException() { }
        public ServiceStartupException(string message) : base(message) { }
        public ServiceStartupException(string message, Exception innerException) : base(message, innerException) { }
    }
}
