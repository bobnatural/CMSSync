using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cmssync.Extensions
{
    static class Utils
    {
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
    }
}
