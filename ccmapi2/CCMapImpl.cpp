
#include "stdafx.h"
#include "ICCMapi.h"
#include "TestCCM.h"

ICCMAPI_API int createCPR(
//	char*	cmd, //= args[0]; // CREATE-CPR
	char*	host, //= args[1]; // 192.168.214.134
	char*	port, //= args[2]; // 32456 
	char*	certSerial,
	char*	certIssuer,
	//char*	client, //= args[3]; // client.pfx
	//char*	pwd, //= args[4]; // actividentity
	//char*	CA, //= args[5]; // root.cer
	char*	user, //= args[6]; // "John G. Doe"
	char*	reader, //= args[7]; // "CPR 2.1.8.sample.signed.xml"
	char*	pin, //= args[8]; // ""
	char*	policy, //= args[9]; // "F2F" or "test6"
	char*	reason //= args[10]; // ""	
	)
{
	TestCCM ccm; 
	return ccm.Process("CREATE-CPR", host, port, certSerial, certIssuer, user, reader, pin, policy, reason);
}

ICCMAPI_API int TerminateAll(
	char*	host, //= args[1]; // 192.168.214.134
	char*	port, //= args[2]; // 32456 
	char*	certSerial,
	char*	certIssuer,
	char*	user //= args[6]; // "John G. Doe"
	)
{
	TestCCM ccm;
	return ccm.Process("TERMINATE-ALL", host, port, certSerial, certIssuer, user, NULL, NULL, NULL, NULL);
}

ICCMAPI_API int CheckHealth(
	char*	host, //= args[1]; // 192.168.214.134
	char*	port, //= args[2]; // 32456 
	char*	certSerial,
	char*	certIssuer
	)
{
	TestCCM ccm;
	return ccm.Process("CHECK-HEALTH", host, port, certSerial, certIssuer, NULL, NULL, NULL, NULL, NULL);
}


ICCMAPI_API int IsDeviceIsActive(
	char*	host, //= args[1]; // 192.168.214.134
	char*	port, //= args[2]; // 32456 
	char*	certSerial,
	char*	certIssuer,
	char*	user)
{
	TestCCM ccm;
	return ccm.Process("IsDeviceIsActive", host, port, certSerial, certIssuer, user, NULL, NULL, NULL, NULL);
}
