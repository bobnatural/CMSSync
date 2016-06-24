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
            if (values1.Length != values2.Length)
                return false;
            foreach (var v in values2)
                if (!values1.Contains(v))
                    return false;
            return true;
        }
        public static string PropVal(string[] propValues)
        {
            return propValues != null && propValues.Length > 0 ? propValues[0] : "(null)";
        }
    }
}
