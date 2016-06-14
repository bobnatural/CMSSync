using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security.AccessControl;
using System.Security.Cryptography;
using System.Security.Principal;
using System.Text;
using AdPoolService.Extensions;
using AdPoolService.Logging;

namespace AdPoolService
{
    public class CryptUtil {
        public const string KeyContainerName = "CMSSyncConfigContainer";

        private class SecurityKey
        {
            public byte[] IV {get; private set; }
            public byte[] Key { get; private set; }

            public static SecurityKey Generate()
            {
                var sk = new SecurityKey();
                using (var aes = new AesCryptoServiceProvider())
                {
                    aes.BlockSize = 128;
                    aes.KeySize = 256;

                    aes.GenerateIV();
                    aes.GenerateKey();

                    sk.IV = aes.IV;
                    sk.Key = aes.Key;
                }
                return sk;
            }

            public void Save(string file)
            {
                try
                {
                    using (var fs = new FileStream(file, FileMode.Create))
                    {
                        fs.Write(Encoding.ASCII.GetBytes("CRLP"), 0, 4);
                        var bytes = BitConverter.GetBytes(IV.Length);
                        fs.Write(bytes, 0, bytes.Length);
                        bytes = BitConverter.GetBytes(Key.Length);
                        fs.Write(bytes, 0, bytes.Length);

                        fs.Write(IV, 0, IV.Length);
                        fs.Write(Key, 0, Key.Length);
                    }
                }
                catch (Exception ex)
                {
                    throw new CryptographicException("Error saving security key: " + ex.Message, ex);
                }
            }

            public static SecurityKey Read(string file)
            {
                try
                {
                    using (var fs = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                    {
                        var buf = new byte[8];
                        fs.Seek(4, SeekOrigin.Begin); //Skip signature

                        fs.Read(buf, 0, 8);
                        int ivLen = BitConverter.ToInt32(buf, 0);
                        int keyLen = BitConverter.ToInt32(buf, 4);

                        var sk = new SecurityKey();
                        sk.IV = new byte[ivLen];
                        sk.Key = new byte[keyLen];

                        fs.Read(sk.IV, 0, ivLen);
                        fs.Read(sk.Key, 0, keyLen);

                        return sk;
                    }
                }
                catch (Exception ex)
                {
                    throw new CryptographicException("Error reading security key: " + ex.Message, ex);
                }
            }
        }

        private static void UnlockFileAccessForRsaKey(ILog log) {
            FileInfo containerFile;
            try {
                containerFile = GetRsaContainerFile();
                if (!containerFile.Exists) {
                    return;
                }
            } catch (CryptographicException) {
                return;
            }

            var security = containerFile.GetAccessControl();
            security.SetAccessRuleProtection(false, false);
            var oldRules = security.GetAccessRules(true, false, typeof(NTAccount));
            foreach (var oldRule in oldRules.OfType<FileSystemAccessRule>()) {
                security.RemoveAccessRule(oldRule);
            }
            containerFile.SetAccessControl(security);

            log.LogInfo("Ensured unlocked access on RSA file: {0}", containerFile.FullName);
        }

        private static void LockFileAccessForRsaKey(ILog log) {
            var containerFile = GetRsaContainerFile();

            var security = containerFile.GetAccessControl();
            security.SetAccessRuleProtection(true, false);
            var oldRules = security.GetAccessRules(true, false, typeof(NTAccount));
            foreach (var oldRule in oldRules.OfType<FileSystemAccessRule>()) {
                security.RemoveAccessRule(oldRule);
            }

            var newRule1 = new FileSystemAccessRule(new NTAccount(@"NT AUTHORITY\SYSTEM"),
                                                   FileSystemRights.Read | FileSystemRights.Synchronize,
                                                   AccessControlType.Allow);
            security.SetAccessRule(newRule1);
            var newRule2 = new FileSystemAccessRule(new NTAccount(@"BUILTIN\Administrators"),
                                                   FileSystemRights.FullControl,
                                                   AccessControlType.Allow);
            security.SetAccessRule(newRule2);
            containerFile.SetAccessControl(security);

            log.LogInfo("Ensured tight access on RSA file: {0}", containerFile.FullName);
        }

        private static FileInfo GetRsaContainerFile() {
            var csp = new CspParameters {
                KeyContainerName = KeyContainerName,
                KeyNumber = 1,
                ProviderType = 1,
                Flags = CspProviderFlags.UseExistingKey | CspProviderFlags.UseMachineKeyStore
            };
            var containerInfo = new CspKeyContainerInfo(csp);
            var fileName = containerInfo.UniqueKeyContainerName;
            var filePath = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData),
                @"Microsoft\Crypto\RSA\MachineKeys\{0}".FmtInv(fileName));
            var containerFile = new FileInfo(filePath);
            return containerFile;
        }

        private static void PrecreateRsaKeyIfNeeded() {
            var csp = new CspParameters { KeyContainerName = KeyContainerName, KeyNumber = 1, ProviderType = 1, Flags = CspProviderFlags.UseExistingKey | CspProviderFlags.UseMachineKeyStore };
            try {
                // ReSharper disable once UnusedVariable
                var _ = new RSACryptoServiceProvider(csp);
            } catch (Exception e) {
                // exception means the key does not exist yet, so create it
                var cspNew = new CspParameters { KeyContainerName = KeyContainerName, KeyNumber = 1, ProviderType = 1, Flags = CspProviderFlags.UseMachineKeyStore };
                try {
                    var rsa = new RSACryptoServiceProvider(2048, cspNew) { PersistKeyInCsp = true };
                    rsa.Clear();
                } catch (Exception e1) {
                    var hrProp = typeof(Exception).GetProperty("HResult", BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance);
                    if (hrProp != null
                        && (int)hrProp.GetValue(e, new object[0]) == -2146893802
                        && (int)hrProp.GetValue(e1, new object[0]) == -2146893809) {
                        throw new CryptographicException("Keyset {0} is not accessible", KeyContainerName);
                    }
                    throw;
                }
            }
        }

        private static SecurityKey ReadOrCreateAesKey()
        {
            var filePath = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData),
                @"Microsoft\Crypto\Aes\MachineKeys\{0}".FmtInv(KeyContainerName));

            if (File.Exists(filePath))
            {
                return SecurityKey.Read(filePath);
            }
            else
            {
                var dir = Path.GetDirectoryName(filePath);
                if (!Directory.Exists(dir))
                {
                    Directory.CreateDirectory(dir);
                }

                var sk = SecurityKey.Generate();
                sk.Save(filePath);

                return sk;
            }
        }

        public static string Encrypt(string text)
        {
            var sk = ReadOrCreateAesKey();

            byte[] encBytes;
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            {
                aes.BlockSize = 128;
                aes.KeySize = 256;

                aes.Key = sk.Key;
                aes.IV = sk.IV;

                var encryptor = aes.CreateEncryptor();
                
                var bytes = Encoding.UTF8.GetBytes(text);

                using (MemoryStream msEncrypt = new MemoryStream()) {
                    using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write)) {
                        csEncrypt.Write(bytes, 0, bytes.Length);
                    }

                    encBytes = msEncrypt.ToArray();
                }
            }

            return Convert.ToBase64String(encBytes);
        }

        private static void CopyStream(Stream input, Stream output) {
            byte[] b = new byte[32768];
            int r;
            while ((r = input.Read(b, 0, b.Length)) > 0)
                output.Write(b, 0, r);
        }

        public static string Decrypt(string text)
        {
            var encBytes = Convert.FromBase64String(text);

            var sk = ReadOrCreateAesKey();

            byte[] decBytes;
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider()) {
                aes.BlockSize = 128;
                aes.KeySize = 256;

                aes.Key = sk.Key;
                aes.IV = sk.IV;

                var decryptor = aes.CreateDecryptor();

                using (MemoryStream msEncrypt = new MemoryStream(encBytes)) {
                    using (CryptoStream csDecrypt = new CryptoStream(msEncrypt, decryptor, CryptoStreamMode.Read)) {
                        using (var msDecrypt = new MemoryStream())
                        {
                            CopyStream(csDecrypt, msDecrypt);

                            decBytes = msDecrypt.ToArray();
                        }
                    }
                }
            }

            return Encoding.UTF8.GetString(decBytes);
        }
    }
}
