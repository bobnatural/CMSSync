using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Cmssync.Extensions;

namespace Cmssync
{
    class UserProperties : Dictionary<string, string[]>
    {
        public UserProperties(int count, StringComparer stringComparer)
            : base(count, stringComparer)
        {
        }
        public UserProperties()
            : base()
        {
        }
        public string[] GetPropValue(string propName)
        {
            string[] userValue = null;
            string _propName = propName.TrimEnd();
            if (Utils.UserAccountControlFlags.Contains(_propName)) 
                _propName = "userAccountControl";
            ((IDictionary<string, string[]>)this).TryGetValue(_propName, out userValue);
            return userValue;
        }
    }
}
