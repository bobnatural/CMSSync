using Cmssync.Extensions;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UserProperties = System.Collections.Generic.IDictionary<string, string[]>;

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
                foreach (CheckAttribute attr in hint.QualityCheckAttributes)
                    if (!names.Contains(attr.Name))
                        names.Add(attr.Name);
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

        public static ISet<string> GetAllGroups()
        {
            var hintsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");
            ISet<string> groups = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            foreach (ADHintElement hint in hintsElements.ADHints)
            {
                foreach (HintAttribute hintAttr in hint.ADHintAttributes)
                    if (Utils.Equals(hintAttr.Name, "memberof"))
                        foreach (HintAttributeValue v in hintAttr.HintAttributeValues)
                            if (!groups.Contains(v.Value))
                                groups.Add(v.Value);
                foreach (TransitionAttribute tranAttr in hint.TransitionAttributes)
                    if (Utils.Equals(tranAttr.Name, "memberof"))
                    {
                        foreach (TransitionAttributeValue v in tranAttr.TransitionAttributeValuesFrom)
                            if (!groups.Contains(v.Value))
                                groups.Add(v.Value);
                        foreach (TransitionAttributeValue v in tranAttr.TransitionAttributeValuesTo)
                            if (!groups.Contains(v.Value))
                                groups.Add(v.Value);
                    }
            }
            return groups;
        }
        
        public static ADHintElement GetOUByAttributes(IDictionary<string, string[]> userPropsNew, IDictionary<string, string[]> userPropsOld, out string transResult)
        {
            // ConfigurationManager.RefreshSection("ADHints");
            var hitsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");
            transResult = string.Empty;

            foreach (ADHintElement hint in hitsElements.ADHints)
            {
                bool hintMatched = true;
                foreach (HintAttribute hintAttr in hint.ADHintAttributes)
                {
                    bool attrFound = false;
                    string[] userAttrValue;
                    if (userPropsNew.TryGetValue(hintAttr.Name.Trim(), out userAttrValue))
                        foreach (HintAttributeValue hintAttrVal in hintAttr.HintAttributeValues)
                        {
                            if (userAttrValue != null && userAttrValue.Contains(hintAttrVal.Value.Trim(), StringComparer.OrdinalIgnoreCase))
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
                    if (hint.Type == ADHintElement.AdHintType.Terminate_Create)
                    {
                        transResult = hint.GetTransitionByUserAttributes(userPropsOld, userPropsNew);
                        if (!string.IsNullOrEmpty(transResult))
                            return hint;
                    }
                    else
                        return hint;
                }
            } // foreach ADHints
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

        // depricated by Qualitycheck
        //[ConfigurationProperty("RequireEmail", IsRequired = false)]
        //public bool? RequireEmail
        //{
        //    get { return (bool?)this["RequireEmail"]; }
        //    set { this["RequireEmail"] = value; }
        //}

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

        [System.Configuration.ConfigurationProperty("QualityCheck")]
        [ConfigurationCollection(typeof(CheckAttributesCollection), AddItemName = "CheckAttribute")]
        public CheckAttributesCollection QualityCheckAttributes
        {
            get
            {
                object o = this["QualityCheck"];
                return o as CheckAttributesCollection;
            }
            set { this["QualityCheck"] = value; }
        }

        public string GetTransitionByUserAttributes(UserProperties userAttrFrom, UserProperties userAttrTo)
        {
            string matchMessage = "";
            if (this.Type != AdHintType.Terminate_Create || userAttrFrom == null || userAttrTo == null)
                return ""; // Transition rules defined only for Terminate_Create
            
            foreach (TransitionAttribute hintAttr in TransitionAttributes)
            {
                string matchedAttrValueFrom, matchedAttrValueTo;
                string[] userValueFrom, userValueTo;
                userAttrFrom.TryGetValue(hintAttr.Name.Trim(), out userValueFrom);
                userAttrTo.TryGetValue(hintAttr.Name.Trim(), out userValueTo);
                
                var anyFrom = hintAttr.TransitionAttributeValuesFrom.AnyValue ?? false;
                var anyTo = hintAttr.TransitionAttributeValuesTo.AnyValue ?? false;

                if (anyFrom && userValueFrom != null && !Utils.CheckEquals(userValueFrom, userValueTo))
                    matchedAttrValueFrom = userValueFrom[0] + "*"; // if AnyValueAttribute set and value has changed
                else
                    matchedAttrValueFrom = TryMatchAttributeValues(userValueFrom, hintAttr.TransitionAttributeValuesFrom);
                if (string.IsNullOrEmpty(matchedAttrValueFrom))
                    return null; // values not found. No transition

                if (anyTo && userValueTo != null && !Utils.CheckEquals(userValueFrom, userValueTo))
                    matchedAttrValueTo = userValueTo[0] + "*"; // if AnyValueAttribute set and value has changed
                else
                    matchedAttrValueTo = TryMatchAttributeValues(userValueTo, hintAttr.TransitionAttributeValuesTo);
                
                if (string.IsNullOrEmpty(matchedAttrValueTo))
                    return null; // values not found. No transition
                
                matchMessage += "[" + hintAttr.Name + "]='" + matchedAttrValueFrom + "' -> '" + matchedAttrValueTo + "';";
            }

            return matchMessage;
        }
             

        private string TryMatchAttributeValues(string[] userValue, TransitionAttributeValuesCollection transitionAttributeValuesCollection)
        {
            foreach (TransitionAttributeValue hintAttrVal in transitionAttributeValuesCollection)
                if (userValue.Contains(hintAttrVal.Value.Trim(), StringComparer.OrdinalIgnoreCase))
                    return hintAttrVal.Value;
            return null;
        }

        public IList<string> QualityCheck(UserProperties userAttr)
        {
            IList<string> checkMessage = new List<string>();
            if (userAttr == null)
                return checkMessage;


            foreach (CheckAttribute checkAttr in QualityCheckAttributes)
            {
                bool found = false;

                string[] userValue = null;
                if (userAttr.TryGetValue(checkAttr.Name, out userValue))
                    foreach(CheckAttributeValue chkVal in checkAttr.CheckAttributeValues)
                    {
                        if (found) break;
                        if (chkVal.Value.StartsWith("*") && chkVal.Value.EndsWith("*"))
                        {
                            if(chkVal.Value.Length == 1) // "*"
                            {
                                if (userValue != null && userValue.Any(v => !string.IsNullOrEmpty(v)))
                                    found = true;
                            }
                            else // "*abc*"
                            {
                                var actValue = chkVal.Value.Remove(0,1); // remove first *
                                actValue = actValue.Remove(actValue.Length - 1, 1); // remove last *
                                found = userValue.Any(v => v.IndexOf(actValue, StringComparison.OrdinalIgnoreCase) > 0);
                            }
                        }
                        else if (chkVal.Value.StartsWith("*")) // "*abc"
                        {
                            var actValue = chkVal.Value.Remove(0, 1);
                            found = userValue.Any(v => v.EndsWith(actValue, StringComparison.OrdinalIgnoreCase));
                        }
                        else if (chkVal.Value.EndsWith("*")) // "abc*"
                        {
                            var actValue = chkVal.Value.Remove(chkVal.Value.Length - 1, 1);
                            found = userValue.Any(v => v.StartsWith(actValue, StringComparison.OrdinalIgnoreCase));
                        }
                        else // "abc"
                            found = userValue.Contains(chkVal.Value, StringComparer.OrdinalIgnoreCase);
                    }
                if(!found)
                {
                    checkMessage.Add("[" + checkAttr.Name + "] = " + (userValue != null ? string.Join(";", userValue) : "null"));
                }
            }
            return checkMessage;
        }
    }

    /// <summary>
    /// HINT ATTRIBUTE Collection
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


    /// <summary>
    /// CHECK ATTRIBUTE Collection
    /// </summary>
    [ConfigurationCollection(typeof(CheckAttribute), AddItemName = "CheckAttribute")]
    public class CheckAttributesCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new CheckAttribute();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((CheckAttribute)element).Name;
        }
    }

    /// <summary>
    /// CHECK ATTRUBUTE
    /// </summary>
    public class CheckAttribute : ConfigurationElement
    {
        [ConfigurationProperty("Name", IsRequired = true)]
        public string Name
        {
            get { return this["Name"] as string; }
            set { this["Name"] = value; }
        }


        [ConfigurationProperty("CheckAttributeValues")]
        public CheckAttributeValuesCollection CheckAttributeValues
        {
            get { return ((CheckAttributeValuesCollection)(base["CheckAttributeValues"])); }
        }
    }

    /// <summary>
    /// CHECK ATTRIBUTE VALUES
    /// </summary>
    [ConfigurationCollection(typeof(CheckAttributeValue))]
    public class CheckAttributeValuesCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new CheckAttributeValue();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((CheckAttributeValue)element).Value;
        }
        public CheckAttributeValue this[int idx]
        {
            get { return (CheckAttributeValue)BaseGet(idx); }
        }
    }

    /// <summary>
    /// QUALITYCHECK ATTRIBUTEs
    /// </summary>
    public class CheckAttributeValue : ConfigurationElement
    {
        [ConfigurationProperty("Value", IsRequired = true, IsKey = true)]
        public string Value
        {
            get { return this["Value"] as string; }
            set { this["Value"] = value; }
        }
    }
}
