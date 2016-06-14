using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cmssync
{
    class ADHintsConfigurationSection : ConfigurationSection
    {

        [ConfigurationProperty("ADHints")]
        [ConfigurationCollection(typeof(ADHintsCollection), AddItemName = "ADHint")]
        private ADHintsCollection ADHints
        {
            get
            {
                object o = this["ADHints"];
                return o as ADHintsCollection;
            }
            set { this["ADHints"] = value; }
        }

        public static string[] GetAllAttributeNames()
        {
            var hintsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");
            ISet<string> names = new HashSet<string>(GetTransitionAttributeNames());
            foreach (ADHintElement hint in hintsElements.ADHints)
            {
                foreach (HintAttribute hintAttr in hint.ADHintAttributes)
                    if (!names.Contains(hintAttr.Name))
                        names.Add(hintAttr.Name);
            }
            return names.ToArray();
        }

        public static string[] GetTransitionAttributeNames()
        {
            var hintsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");
            ISet<string> names = new HashSet<string>();
            foreach (ADHintElement hint in hintsElements.ADHints)
            {
                foreach (TransitionAttribute tranAttr in hint.TransitionAttributes)
                    if (!names.Contains(tranAttr.Name))
                        names.Add(tranAttr.Name);
            }
            return names.ToArray();
        }
        public static string[] GetAllOUs()
        {
            var hintsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");
            ISet<string> names = new HashSet<string>();
            foreach (ADHintElement hint in hintsElements.ADHints)
                if (!string.IsNullOrEmpty(hint.DestOU) && !names.Contains(hint.DestOU))
                    names.Add(hint.DestOU);
            return names.ToArray();
        }

        public static ADHintElement GetOUByAttributes(IDictionary<string, string> attributes)
        {
            // ConfigurationManager.RefreshSection("ADHints");
            var hitsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");

            foreach (ADHintElement hint in hitsElements.ADHints)
            {
                bool hintMatched = true;
                foreach (HintAttribute hintAttr in hint.ADHintAttributes)
                {
                    bool attrFound = false;
                    string userAttrValue;
                    if (attributes.TryGetValue(hintAttr.Name.Trim(), out userAttrValue))
                        foreach (HintAttributeValue hintAttrVal in hintAttr.HintAttributeValues)
                        {
                            if (userAttrValue != null && hintAttrVal.Value.Trim().Equals(userAttrValue.Trim(), StringComparison.OrdinalIgnoreCase))
                            {
                                attrFound = true;
                                break;
                            }
                        }
                    if (!attrFound)
                    {
                        hintMatched = false;
                        break;
                    }
                }
                if (hintMatched) // all attributes are found and equal
                {
                    return hint;
                }
            }
            return null;
        }

    }

    public class ADHintsCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new ADHintElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((ADHintElement)element).Num.ToString();
        }
    }

    public class ADHintElement : ConfigurationElement
    {
        [ConfigurationProperty("Num", IsRequired = true)]
        public string Num
        {
            get { return this["Num"] as string; }
            set { this["Num"] = value; }
        }

        [ConfigurationProperty("DestOU", IsRequired = false)]
        public string DestOU
        {
            get { return this["DestOU"] as string; }
            set { this["DestOU"] = value; }
        }
        [ConfigurationProperty("CardPolicy", IsRequired = false)]
        public string CardPolicy
        {
            get { return this["CardPolicy"] as string; }
            set { this["CardPolicy"] = value; }
        }
        [ConfigurationProperty("Type", IsRequired = true)]
        public AdHintType Type
        {
            get
            {
                return (AdHintType)this["Type"];
            }
            set { this["Type"] = value; }
        }

        [ConfigurationProperty("RequireEmail", IsRequired = false)]
        public bool? RequireEmail
        {
            get { return (bool?)this["RequireEmail"]; }
            set { this["RequireEmail"] = value; }
        }

        public enum AdHintType
        {
            Create = 1,
            Terminate = 2,
            Terminate_Create = 3,
        }

        [System.Configuration.ConfigurationProperty("HintAttributes")]
        [ConfigurationCollection(typeof(HintAttributesCollection), AddItemName = "HintAttribute")]
        public HintAttributesCollection ADHintAttributes
        {
            get
            {
                object o = this["HintAttributes"];
                return o as HintAttributesCollection;
            }
            set { this["HintAttributes"] = value; }
        }

        [System.Configuration.ConfigurationProperty("TransitionAttributes")]
        [ConfigurationCollection(typeof(TransitionAttributesCollection), AddItemName = "TransitionAttribute")]
        public TransitionAttributesCollection TransitionAttributes
        {
            get
            {
                object o = this["TransitionAttributes"];
                return o as TransitionAttributesCollection;
            }
            set { this["TransitionAttributes"] = value; }
        }

        public string GetTransitionByUserAttributes(IDictionary<string, string> userAttrFrom, IDictionary<string, string> userAttrTo)
        {
            string matchMessage = "";
            if (this.Type != AdHintType.Terminate_Create)
                return ""; // Transition rules defined only for Terminate_Create
            
            foreach (TransitionAttribute hintAttr in TransitionAttributes)
            {
                string matchedAttrValueFrom, matchedAttrValueTo;
                string userValueFrom, userValueTo;
                userAttrFrom.TryGetValue(hintAttr.Name.Trim(), out userValueFrom);
                userAttrTo.TryGetValue(hintAttr.Name.Trim(), out userValueTo);
                
                var anyFrom = hintAttr.TransitionAttributeValuesFrom.AnyValue ?? false;
                var anyTo = hintAttr.TransitionAttributeValuesTo.AnyValue ?? false;
                
                if (anyFrom && userValueFrom != null && !userValueFrom.Equals(userValueTo))
                    matchedAttrValueFrom = userValueFrom + "*"; // if AnyValueAttribute set and value has changed
                else
                    matchedAttrValueFrom = TryMatchAttributeValues(userValueFrom, hintAttr.TransitionAttributeValuesFrom);
                if (string.IsNullOrEmpty(matchedAttrValueFrom))
                    return null; // values not found. No transition

                if (anyTo && userValueTo != null && !userValueTo.Equals(userValueFrom))
                    matchedAttrValueTo = userValueTo + "*"; // if AnyValueAttribute set and value has changed
                else
                    matchedAttrValueTo = TryMatchAttributeValues(userValueTo, hintAttr.TransitionAttributeValuesTo);
                
                if (string.IsNullOrEmpty(matchedAttrValueTo))
                    return null; // values not found. No transition
                
                matchMessage += "[" + hintAttr.Name + "]='" + matchedAttrValueFrom + "' -> '" + matchedAttrValueTo + "';";
            }

            return matchMessage;
        }

        private string TryMatchAttributeValues(string userValue, TransitionAttributeValuesCollection transitionAttributeValuesCollection)
        {
            foreach (TransitionAttributeValue hintAttrVal in transitionAttributeValuesCollection)
                if (hintAttrVal.Value.Trim().Equals(userValue.Trim(), StringComparison.OrdinalIgnoreCase))
                    return hintAttrVal.Value;
            return null;
        }
    }

    /// <summary>
    /// HINT ATTRIBUTE
    /// </summary>
    [ConfigurationCollection(typeof(HintAttribute), AddItemName = "HintAttribute")]
    public class HintAttributesCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new HintAttribute();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((HintAttribute)element).Name;
        }
    }

    public class HintAttribute : ConfigurationElement
    {
        [ConfigurationProperty("Name", IsRequired = true)]
        public string Name
        {
            get { return this["Name"] as string; }
            set { this["Name"] = value; }
        }


        [ConfigurationProperty("HintAttributeValues")]
        public HintAttributeValuesCollection HintAttributeValues
        {
            get { return ((HintAttributeValuesCollection)(base["HintAttributeValues"])); }
        }
    }

    /// <summary>
    /// HINT ATTRIBUTE VALUES
    /// </summary>
    [ConfigurationCollection(typeof(HintAttributeValue))] // , AddItemName = "HintAttributeValue"
    public class HintAttributeValuesCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new HintAttributeValue();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((HintAttributeValue)element).Value;
        }
        public HintAttributeValue this[int idx]
        {
            get { return (HintAttributeValue)BaseGet(idx); }
        }
    }

    public class HintAttributeValue : ConfigurationElement
    {
        [ConfigurationProperty("Value", IsRequired = true, IsKey = true)]
        public string Value
        {
            get { return this["Value"] as string; }
            set { this["Value"] = value; }
        }
    }

    /// <summary>
    /// TRANSITION ATTRIBUTEs
    /// </summary>
    [ConfigurationCollection(typeof(TransitionAttribute), AddItemName = "TransitionAttribute")]
    public class TransitionAttributesCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new TransitionAttribute();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((TransitionAttribute)element).Name;
        }
    }

    public class TransitionAttribute : ConfigurationElement
    {
        [ConfigurationProperty("Name", IsRequired = true)]
        public string Name
        {
            get { return this["Name"] as string; }
            set { this["Name"] = value; }
        }

        [ConfigurationProperty("TranAttributeValuesFrom")]
        public TransitionAttributeValuesCollection TransitionAttributeValuesFrom
        {
            get { return ((TransitionAttributeValuesCollection)(base["TranAttributeValuesFrom"])); }
        }
        [ConfigurationProperty("TranAttributeValuesTo")]
        public TransitionAttributeValuesCollection TransitionAttributeValuesTo
        {
            get { return ((TransitionAttributeValuesCollection)(base["TranAttributeValuesTo"])); }
        }
    }
    /// <summary>
    /// TRANSITION ATTRIBUTE VALUES
    /// </summary>
    [ConfigurationCollection(typeof(TransitionAttributeValue))] // , AddItemName = "HintAttributeValue"
    public class TransitionAttributeValuesCollection : ConfigurationElementCollection
    {
        [ConfigurationProperty("AnyValue", IsRequired = false, DefaultValue = false)]
        public bool? AnyValue
        {
            get { return base["AnyValue"] as bool?; }
            set { base["AnyValue"] = value; }
        }
               
        protected override ConfigurationElement CreateNewElement()
        {
            return new TransitionAttributeValue();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((TransitionAttributeValue)element).Value;
        }
        public TransitionAttributeValue this[int idx]
        {
            get { return (TransitionAttributeValue)BaseGet(idx); }
        }
    }

    public class TransitionAttributeValue : ConfigurationElement
    {
        [ConfigurationProperty("Value", IsRequired = false, IsKey = true)]
        public string Value
        {
            get { return this["Value"] as string; }
            set { this["Value"] = value; }
        }       
    }

    public class AnyValue : ConfigurationElement
    {        
        [ConfigurationProperty("Allow", IsRequired = false)]
        public bool? Allow
        {
            get { return this["Allow"] as bool?; }
            set { this["Allow"] = value; }
        }
    }
}
