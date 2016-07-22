using Cmssync.Extensions;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cmssync
{
    internal class ADHintsConfigurationSection : ConfigurationSection
    {
        private static ISet<string> allGroupsFromConfig = null; // cache of configured groups

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
            return names.Where(a => !Utils.UserAccountControlFlags.Contains(a)).ToArray();
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

        public static ISet<string> GetAllGroupsFromConfig()
        {
            if (allGroupsFromConfig != null)
                return allGroupsFromConfig;

            var hintsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");
            ISet<string> groups = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            foreach (ADHintElement hint in hintsElements.ADHints)
            {
                foreach (HintAttribute hintAttr in hint.ADHintAttributes)
                    if (Utils.Equals(hintAttr.Name, "memberof"))
                        foreach (AttributeValue v in hintAttr.HintAttributeValues)
                            if (!groups.Contains(v.Value))
                                groups.Add(v.Value);
                foreach (TransitionAttribute tranAttr in hint.TransitionAttributes)
                    if (Utils.Equals(tranAttr.Name, "memberof"))
                    {
                        foreach (AttributeValue v in tranAttr.TransitionAttributeValuesFrom)
                            if (!groups.Contains(v.Value))
                                groups.Add(v.Value);
                        foreach (AttributeValue v in tranAttr.TransitionAttributeValuesTo)
                            if (!groups.Contains(v.Value))
                                groups.Add(v.Value);
                    }
            }
            return (allGroupsFromConfig = groups);
        }
        
        public static ADHintElement GetOUByAttributes(UserProperties userPropsNew, UserProperties userPropsOld, out string transResult)
        {
            // ConfigurationManager.RefreshSection("ADHints");
            var hitsElements = (ADHintsConfigurationSection)ConfigurationManager.GetSection("ADHintSettings");
            transResult = string.Empty;

            foreach (ADHintElement hint in hitsElements.ADHints)
            {
                bool hintMatched = true;
                foreach (HintAttribute hintAttr in hint.ADHintAttributes)
                {
                    string[] userAttrValue = userPropsNew.GetPropValue(hintAttr.Name);
                    if ( //userAttrValue == null
                        string.IsNullOrEmpty(GetFirstMatchedAttribute(userAttrValue, hintAttr.Name, hintAttr.HintAttributeValues)))
                    {
                        hintMatched = false; // if any hintvalue is not matched 
                        break; // Then Hint is not matched
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

        /// <summary>
        /// if userAttributeValues equal at least one of HintAttribute Value
        /// </summary>
        /// <param name="userPropsNew"></param>
        /// <param name="hintAttr"></param>
        /// <returns></returns>
        internal static string GetFirstMatchedAttribute(string[] userAttrValue, string attributeName, IEnumerable configAttrValues)
        {
            if (Utils.UserAccountControlFlags.Contains(attributeName.Trim()))
            {
                Utils.UserAccountControl uaEnum; // flag
                UInt32 userAccountControl = 0;
                if (userAttrValue != null && userAttrValue.Length > 0)
                    UInt32.TryParse(userAttrValue[0], out userAccountControl);
                if (Enum.TryParse(attributeName.Trim(), out uaEnum)) // 
                {
                    bool userValue = (userAccountControl & (uint)uaEnum) != 0; // if userValue has given flag
                    foreach (AttributeValue attrVal in configAttrValues)
                    {
                        var hintVal = attrVal.Value.Trim();
                        bool bHintVal = false;
                        Boolean.TryParse(hintVal, out bHintVal);
                        if (hintVal == "*" || userValue == bHintVal)
                            return hintVal; // userValue.ToString(); // +"(" + hintVal + ")";
                    }
                }
            }
            else 
                foreach (AttributeValue attrVal in configAttrValues)
                {
                    if (Utils.MatchValueInArray(attrVal.Value.Trim(), userAttrValue))
                        return attrVal.Value.Trim(); // at least one value is equal
                }
            return string.Empty;
        }

        internal static string PrintMemberOfAttributes(string[] userGroups)
        {
            string res = Environment.NewLine + "MemberOf from config:";
            foreach(var confGr in GetAllGroupsFromConfig())
                res += Environment.NewLine + (userGroups != null && userGroups.Contains(confGr, StringComparer.OrdinalIgnoreCase) ? " Yes:" : " No: ") + confGr;
            return res;
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

    class ADHintElement : ConfigurationElement
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
                var userValueFrom = userAttrFrom.GetPropValue(hintAttr.Name);
                var userValueTo = userAttrTo.GetPropValue(hintAttr.Name);
                
                var anyFrom = hintAttr.TransitionAttributeValuesFrom.AnyValue ?? false;
                var anyTo = hintAttr.TransitionAttributeValuesTo.AnyValue ?? false;

                if (!Utils.CheckEquals(userValueFrom, userValueTo))
                {
                    if (anyFrom)
                        matchedAttrValueFrom = (userValueFrom == null || userValueFrom.Length == 0 || string.IsNullOrEmpty(userValueFrom[0]) ? "NULL" : userValueFrom[0]) + "(*)"; // if AnyValueAttribute set and value has changed
                    else
                        matchedAttrValueFrom = ADHintsConfigurationSection.GetFirstMatchedAttribute(userValueFrom, hintAttr.Name, hintAttr.TransitionAttributeValuesFrom);

                    if (string.IsNullOrEmpty(matchedAttrValueFrom))
                        return null; // values not found. No transition

                    if (anyTo)
                        matchedAttrValueTo = (userValueTo == null || userValueTo.Length == 0 || string.IsNullOrEmpty(userValueTo[0]) ? "NULL" : userValueTo[0]) + "(*)"; // if AnyValueAttribute set and value has changed
                    else
                        matchedAttrValueTo = ADHintsConfigurationSection.GetFirstMatchedAttribute(userValueTo, hintAttr.Name, hintAttr.TransitionAttributeValuesTo);

                    if (string.IsNullOrEmpty(matchedAttrValueTo))
                        return null; // values not found. No transition

                    matchMessage += "[" + hintAttr.Name + "]='" + matchedAttrValueFrom + "' -> '" + matchedAttrValueTo + "';";
                }
                else
                    return null; // values not changed
            }

            return matchMessage;
        }


        //private string GetFirstMatchedAttribute(string[] userValue, TransitionAttributeValuesCollection transitionAttributeValuesCollection)
        //{
        //    foreach (AttributeValue hintAttrVal in transitionAttributeValuesCollection)
        //        if (userValue.Contains(hintAttrVal.Value.Trim(), StringComparer.OrdinalIgnoreCase))
        //            return hintAttrVal.Value;
        //    return null;
        //}

        public IList<string> QualityCheck(UserProperties userAttr)
        {
            IList<string> checkMessage = new List<string>();
            if (userAttr == null)
                return checkMessage;


            foreach (CheckAttribute checkAttr in QualityCheckAttributes)
            {
                bool found = false;

                string[] userValue = userAttr.GetPropValue(checkAttr.Name);
                if (userAttr != null)
                    foreach (AttributeValue chkVal in checkAttr.CheckAttributeValues)
                    {
                        if (found) break;
                        found = Utils.MatchValueInArray(chkVal.Value, userValue);
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
    [ConfigurationCollection(typeof(AttributeValue))] // , AddItemName = "HintAttributeValue"
    public class HintAttributeValuesCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new AttributeValue();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((AttributeValue)element).Value;
        }
        public AttributeValue this[int idx]
        {
            get { return (AttributeValue)BaseGet(idx); }
        }
    }

    public class AttributeValue : ConfigurationElement
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
    [ConfigurationCollection(typeof(AttributeValue))] // , AddItemName = "HintAttributeValue"
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
            return new AttributeValue();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((AttributeValue)element).Value;
        }
        public AttributeValue this[int idx]
        {
            get { return (AttributeValue)BaseGet(idx); }
        }
    }

    //public class TransitionAttributeValue : ConfigurationElement
    //{
    //    [ConfigurationProperty("Value", IsRequired = false, IsKey = true)]
    //    public string Value
    //    {
    //        get { return this["Value"] as string; }
    //        set { this["Value"] = value; }
    //    }       
    //}

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
    [ConfigurationCollection(typeof(AttributeValue))]
    public class CheckAttributeValuesCollection : ConfigurationElementCollection
    {

        protected override ConfigurationElement CreateNewElement()
        {
            return new AttributeValue();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((AttributeValue)element).Value;
        }
        public AttributeValue this[int idx]
        {
            get { return (AttributeValue)BaseGet(idx); }
        }
    }

    ///// <summary>
    ///// QUALITYCHECK ATTRIBUTEs
    ///// </summary>
    //public class CheckAttributeValue : ConfigurationElement
    //{
    //    [ConfigurationProperty("Value", IsRequired = true, IsKey = true)]
    //    public string Value
    //    {
    //        get { return this["Value"] as string; }
    //        set { this["Value"] = value; }
    //    }
    //}
}
