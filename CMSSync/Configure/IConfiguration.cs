using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cmssync
{
    /// <summary>
    ///     A wrapper around app configuration, for decoupling and testing. Can be implemented through Settings.Default etc.
    /// </summary>
    public interface IConfiguration
    {
        int PoolsInterval { get; }
        int ServiceHealthCheckInterval { get; }

        ADServer[] SourceADServers { get; }
        ADServer[] DestADServers { get; }

        bool LogVerbose { get; }

        string CCMCertificateSerial { get; }

        string CCMCertificateIssuer { get; }

        string CCMCPRFile { get; }
        string CCMHost { get; }
        string CCMPort { get; }

        ISet<string> OUsDNToMonitor { get; }
    }
    
}
