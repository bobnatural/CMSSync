using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AdPoolService.CryptoApi;
using AdPoolService;

namespace Cmssync
{
    public class ADServerElement : ConfigurationElement
    {

        [ConfigurationProperty("url", IsRequired = true)]
        public string Url
        {
            get
            {
                return this["url"] as string;
            }
            set { this["url"] = value; }
        }
        [ConfigurationProperty("user", IsRequired = true)]
        public string User
        {
            get
            {
                return this["user"] as string;
            }
            set { this["user"] = value; }
        }
        [ConfigurationProperty("type", IsRequired = false)]
        public string Type
        {
            get
            {
                return this["type"] as string;
            }
            set { this["type"] = value; }
        }
        [ConfigurationProperty("password", IsRequired = true)]
        public string Password
        {
            get
            {
                var res = this["password"] as string;
                if (res != null && res.StartsWith(ADCredentialsConfigurationSection.EncodePrefix))
                {
                    res = CryptUtil.Decrypt(res.Substring(ADCredentialsConfigurationSection.EncodePrefix.Length));
                }
                return res;
            }
            set { this["password"] = value; }
        }
        [ConfigurationProperty("allowClearLDAP", IsRequired = false)]
        public string allowClearLDAP
        {
            get
            {
                return (string)this["allowClearLDAP"];
            }
            set { this["allowClearLDAP"] = value; }
        }
        public bool PasswordEncrypted
        {
            get
            {
                var res = this["password"] as string;
                if (res == null)
                {
                    return true;
                }
                if (res.StartsWith(ADCredentialsConfigurationSection.EncodePrefix))
                {
                    return true;
                }
                return false;
            }
        }

        public override string ToString()
        {
            return Type + ":" + User + "@" + Url;
        }
    }

    public class ADServersElement : ConfigurationElementCollection
    {
        public ADServerElement this[int index]
        {
            get
            {
                return base.BaseGet(index) as ADServerElement;
            }
            set
            {
                if (base.Count > index && base.BaseGet(index) != null)
                {
                    base.BaseRemoveAt(index);
                }
                this.BaseAdd(index, value);
            }
        }

        public new ADServerElement this[string responseString]
        {
            get { return (ADServerElement)BaseGet(responseString); }
            set
            {
                if (BaseGet(responseString) != null)
                {
                    BaseRemoveAt(BaseIndexOf(BaseGet(responseString)));
                }
                BaseAdd(value);
            }
        }

        protected override System.Configuration.ConfigurationElement CreateNewElement()
        {
            return new ADServerElement();
        }

        protected override object GetElementKey(System.Configuration.ConfigurationElement element)
        {
            return ((ADServerElement)element).Url.ToString();
        }
    }


    public class ADCredentialsConfigurationSection : ConfigurationSection
    {
        public string SectionName;
        private const string SectionNameSrc = "SourceADCredentials";
        private const string SectionNameDst = "DestinationADCredentials";
        public const string EncodePrefix = "=$enc:";

        private static ADCredentialsConfigurationSection sourceADsettings = null;
        private static ADCredentialsConfigurationSection destADsettings = null;

        public static ADCredentialsConfigurationSection SourceADSettings
        {
            get
            {
                lock (SectionNameSrc)
                {
                    if (sourceADsettings == null)
                    {
                        sourceADsettings = ConfigurationManager.GetSection(SectionNameSrc) as ADCredentialsConfigurationSection;
                        sourceADsettings.SectionName = SectionNameSrc;
                        //settings.Decrypt();
                    }
                }
                return sourceADsettings;
            }
        }
        public static ADCredentialsConfigurationSection DestADSettings
        {
            get
            {
                lock (SectionNameDst)
                {
                    if (destADsettings == null)
                    {
                        destADsettings = ConfigurationManager.GetSection(SectionNameDst) as ADCredentialsConfigurationSection;
                        sourceADsettings.SectionName = SectionNameDst;
                        //settings.Decrypt();
                    }
                }
                return destADsettings;
            }
        }       

        public static void Refresh()
        {
            ConfigurationManager.RefreshSection(SectionNameSrc);
            ConfigurationManager.RefreshSection(SectionNameDst);
            //Settings.Decrypt();
        }

        public void Decrypt()
        {
            foreach (ADServerElement server in ADServers)
            {
                if (server.Password != null)
                {
                    if (server.Password.StartsWith(EncodePrefix))
                    {
                        server.Password = CryptUtil.Decrypt(server.Password.Substring(EncodePrefix.Length));
                    }
                }
            }
        }

        public void Encrypt()
        {
            foreach (ADServerElement server in ADServers)
            {
                if (server.Password != null)
                {
                    if (!server.Password.StartsWith(EncodePrefix))
                    {
                        server.Password = EncodePrefix + CryptUtil.Encrypt(server.Password);
                    }
                }
            }
        }

        [System.Configuration.ConfigurationProperty("ADServers")]
        [ConfigurationCollection(typeof(ADServersElement), AddItemName = "ADServer")]
        public ADServersElement ADServers
        {
            get
            {
                object o = this["ADServers"];
                return o as ADServersElement;
            }
            set { this["ADServers"] = value; }
        }
    }
}
