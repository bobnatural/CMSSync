using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cmssync.Extensions
{
    static class Utils
    {
        public enum UserAccountControl
        {
            NORMAL_ACCOUNT = 0x200, // normal account
            PWD_NOTREQD = 0x20, // password not required
            ACCOUNTDISABLE = 0x002, //account disabled
            //DONT_EXPIRE_PASSWORD = 	0x10000,
            SMARTCARD_REQUIRED = 0x40000,
        }

        public static readonly ISet<string> UserAccountControlFlags = new HashSet<string>(((IEnumerable<UserAccountControl>)Enum.GetValues(typeof(UserAccountControl))).Select(f => f.ToString()), StringComparer.OrdinalIgnoreCase);

        public static bool CheckEquals(string[] values1, string[] values2)
        {
            if (values1 == null && values2 == null)
                return true;
            if (values1==null && values2!=null
                || values2==null && values1!=null
                || values1.Length != values2.Length)
                return false;
            foreach (var v in values2)
                if (!values1.Contains(v, StringComparer.OrdinalIgnoreCase))
                    return false;
            return true;
        }
        public static bool CheckEquals(ISet<string> values1, ISet<string> values2)
        {
            if (values1.Count != values2.Count)
                return false;
            foreach (var v in values2)
                if (!values1.Contains(v, StringComparer.OrdinalIgnoreCase))
                    return false;
            return true;
        }
        public static string PropVal(string[] propValues)
        {
            return propValues != null && propValues.Length > 0 ? propValues[0] : "(null)";
        }

        public static bool Equals(string s1, string s2)
        {
            return s1!=null && s1.Equals(s2, StringComparison.OrdinalIgnoreCase);
        }

        internal static bool MatchValueInArray(string val, string[] userValue)
        {
            bool found = false;
            if (val.StartsWith("*") && val.EndsWith("*") && userValue != null)
            {
                if (val.Length == 1) // "*" = any NotNull value
                {
                    if (userValue != null && userValue.Any(v => !string.IsNullOrEmpty(v)))
                        found = true;
                }
                else // "*abc*"
                {
                    var actValue = val.Remove(0, 1); // remove first *
                    actValue = actValue.Remove(actValue.Length - 1, 1); // remove last *
                    found = userValue.Any(v => v.IndexOf(actValue, StringComparison.OrdinalIgnoreCase) > 0);
                }
            }
            else if (val.StartsWith("*") && userValue != null) // "*abc" = EndsWith abc
            {
                var actValue = val.Remove(0, 1);
                found = userValue.Any(v => v.EndsWith(actValue, StringComparison.OrdinalIgnoreCase));
            }
            else if (val.EndsWith("*") && userValue != null) // "abc*" = StartsWith abc
            {
                var actValue = val.Remove(val.Length - 1, 1);
                found = userValue.Any(v => v.StartsWith(actValue, StringComparison.OrdinalIgnoreCase));
            }
            else if (userValue != null) // "abc" = Equal abc
                found = userValue.Contains(val, StringComparer.OrdinalIgnoreCase);
            else if (userValue == null && "null".Equals(val, StringComparison.OrdinalIgnoreCase))
                found = true;
            return found;
        }
    }
}
