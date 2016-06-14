using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using AdPoolService.Logging;

namespace AdPoolService.CryptoApi
{
    public class CryptoHelper {
        public static int GetCRLNumber(string fileName, bool throwExceptions = false)
        {
            Boolean bResult = false;
            IntPtr pvContext = IntPtr.Zero;
            CryptoApiWin32.CRL_CONTEXT CRLContext;
            CryptoApiWin32.CRL_INFO CRLInfo;
            IntPtr rgExtension = IntPtr.Zero;
            CryptoApiWin32.CERT_EXTENSION CRLExtension;
            Int32 cbFormat = 0;
            StringBuilder pbFormat = null;

            try
            {
                // Get CRL context
                //
                bResult = CryptoApiWin32.CryptQueryObject(
                    CryptoApiWin32.CERT_QUERY_OBJECT_FILE,
                    fileName,
                    CryptoApiWin32.CERT_QUERY_CONTENT_FLAG_CRL,
                    CryptoApiWin32.CERT_QUERY_FORMAT_FLAG_BINARY,
                    0,
                    IntPtr.Zero,
                    IntPtr.Zero,
                    IntPtr.Zero,
                    IntPtr.Zero,
                    IntPtr.Zero,
                    ref pvContext
                    );
                if (!bResult)
                {
                    throw new Exception("CryptQueryObject error #" + Marshal.GetLastWin32Error());
                }

                CRLContext =
                    (CryptoApiWin32.CRL_CONTEXT) Marshal.PtrToStructure(pvContext, typeof (CryptoApiWin32.CRL_CONTEXT));

                // Get CRL info
                //
                CRLInfo =
                    (CryptoApiWin32.CRL_INFO)
                        Marshal.PtrToStructure(CRLContext.pCrlInfo, typeof (CryptoApiWin32.CRL_INFO));

                rgExtension = CryptoApiWin32.CertFindExtension(
                    CryptoApiWin32.szOID_CRL_NUMBER_CODE,
                    CRLInfo.cExtension,
                    CRLInfo.rgExtension
                    );
                if (rgExtension.Equals(IntPtr.Zero))
                {
                    throw new Exception("CertFindExtension found no CRL Number");
                }

                CRLExtension =
                    (CryptoApiWin32.CERT_EXTENSION)
                        Marshal.PtrToStructure(rgExtension, typeof (CryptoApiWin32.CERT_EXTENSION));

                var res = new byte[CRLExtension.Value.cbData];
                Marshal.Copy(CRLExtension.Value.pbData, res, 0, res.Length);

                /*if (res.Length == 4)
                {
                    var res2 = new byte[2];
                    res2[0] = res[2];
                    res2[1] = res[3];

                    return res2;
                }*/

                cbFormat = 0;
                pbFormat = null;
                bResult = CryptoApiWin32.CryptFormatObject(
                    CryptoApiWin32.X509_ASN_ENCODING,
                    0,
                    0,
                    IntPtr.Zero,
                    CryptoApiWin32.szOID_CRL_NUMBER_CODE,
                    CRLExtension.Value.pbData,
                    CRLExtension.Value.cbData,
                    null,
                    ref cbFormat
                    );
                if (!bResult)
                {
                    throw new Exception("CryptFormatObject error #" + Marshal.GetLastWin32Error());
                }

                pbFormat = new StringBuilder(cbFormat);

                bResult = CryptoApiWin32.CryptFormatObject(
                    CryptoApiWin32.X509_ASN_ENCODING,
                    0,
                    0,
                    IntPtr.Zero,
                    CryptoApiWin32.szOID_CRL_NUMBER_CODE,
                    CRLExtension.Value.pbData,
                    CRLExtension.Value.cbData,
                    pbFormat,
                    ref cbFormat
                    );
                if (!bResult)
                {
                    throw new Exception("CryptFormatObject error #" + Marshal.GetLastWin32Error());
                }

                LogHelper.GetLogger().WriteEntry("Reading crl number from " + fileName + ": " + pbFormat + ", array length=" + res.Length, EventLogEntryType.Information, 9000);

                //return Convert.ToInt32(pbFormat.ToString().Replace(" ", ""));

                if (res.Length == 3)
                {
                    return res[2];
                } else if (res.Length >= 2)
                {
                    return res[res.Length - 2]*256 + res[res.Length - 1];
                }

                return BitConverter.ToInt32(res, 0);
            } catch(Exception ex)
            {
                if (throwExceptions)
                {
                    throw;
                }

                LogHelper.GetLogger().WriteEntry("Error reading crl number from " + fileName + ": " + ex.Message, EventLogEntryType.Error, 5);
                return 0;
            } finally {
                // Do some clean up
                //
                if (!pvContext.Equals(IntPtr.Zero)) {
                    CryptoApiWin32.CertFreeCRLContext(pvContext);
                }
            }
        }
    }
}
