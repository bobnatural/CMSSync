using System.Globalization;

namespace AdPoolService.Extensions
{
    public static class StringExtensions {
        public static string Fmt(this string template, params object[] args) {
            return string.Format(CultureInfo.CurrentCulture, template, args);
        }

        public static string FmtInv(this string template, params object[] args) {
            return string.Format(CultureInfo.InvariantCulture, template, args);
        }
    }
}