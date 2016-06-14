using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cmssync.Configure
{
    class OUsDNToMonitorConfigurationSection : ConfigurationSection
    {
        [System.Configuration.ConfigurationProperty("OUsDNs")]
        [ConfigurationCollection(typeof(OUsDNsCollection))] // , AddItemName = "OU"
        private OUsDNsCollection OUsDNs
        {
            get
            {
                object o = this["OUsDNs"];
                return o as OUsDNsCollection;
            }
            set { this["OUsDNs"] = value; }
        }

        public static ISet<string> GetOUsDNsToMonitor()
        {
            var settings = (OUsDNToMonitorConfigurationSection)ConfigurationManager.GetSection("OUsDNToMonitorSettings");
            ISet<string> names = new HashSet<string>();
            foreach (OUsDNElement hint in settings.OUsDNs)
                names.Add(hint.Value);
            return names;
        }
    }
    [ConfigurationCollection(typeof(OUsDNElement))]
    class OUsDNsCollection : ConfigurationElementCollection
    {
        protected override ConfigurationElement CreateNewElement()
        {
            return new OUsDNElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((OUsDNElement)element).Value.ToString();
        }
        public OUsDNElement this[int idx]
        {
            get { return (OUsDNElement)BaseGet(idx); }
        }
    }
    class OUsDNElement: ConfigurationElement
    {
        [ConfigurationProperty("Value", IsRequired = true, IsKey = true)]
        public string Value
        {
            get { return this["Value"] as string; }
            set { this["Value"] = value; }
        }
    }
}
