using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.DirectoryServices;
using System.DirectoryServices.Protocols;

namespace Cmssync
{
    public class ADServer
    {
        public string Name { get; set; }

        public string ServerUserName { get; set; }

        public string ServerPassword { get; set; }

        public bool SSL { get; set; }

        public string SourceDest { get; set; }

        public int port
        {
            get
            {
                return SSL ? 636 : 389;
            }
        }
        public string path
        {
            get
            {
                return @"LDAP://" + Name + (SSL ? ":" + port.ToString() : "");
            }
        }
        public System.DirectoryServices.AuthenticationTypes authTypes
        {
            get
            {
                return AuthenticationTypes.Secure 
                    | (SSL ? AuthenticationTypes.SecureSocketsLayer : 0)
                    | (SourceDest.StartsWith("source", StringComparison.OrdinalIgnoreCase)? AuthenticationTypes.ReadonlyServer : 0);
            }
        }

        public override string ToString()
        {
            return SourceDest + " AD Server '" + Name + "' " + (SSL ? "(SSL)" : "(not SSL)");
        }

        public LdapConnection getLapConnection
        {
            get
            {
                var identifier = new LdapDirectoryIdentifier(this.Name, this.port);
                LdapConnection connection = new LdapConnection(identifier);
                connection.Timeout = new TimeSpan(0, 1, 0);
                connection.Credential = new System.Net.NetworkCredential(ServerUserName, ServerPassword);
                if (SSL)
                {
                    connection.SessionOptions.ProtocolVersion = 3;
                    connection.SessionOptions.SecureSocketLayer = true;
                    connection.SessionOptions.VerifyServerCertificate =
                                    new VerifyServerCertificateCallback((con, cer) => true); // LDAP over SSL 
                }

                connection.Bind();
                return connection;
            }
        }
    }
}
