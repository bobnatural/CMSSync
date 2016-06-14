using System;

namespace AdPoolService.Extensions
{
    public static class ByteArrayExtensions {
        public static string ToHexString(this byte[] bytes) {
            var hex = BitConverter.ToString(bytes);
            return hex.Replace("-", "");
        }
    }
}