#pragma once


#ifdef CCMAPI_EXPORTS
#define ICCMAPI_API __declspec(dllexport)
#else
#define ICCMAPI_API  __declspec(dllimport)
#endif

extern "C" ICCMAPI_API int createCPR(
	//char*	cmd, //= args[0]; // CREATE-CPR
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
);

extern "C" ICCMAPI_API int TerminateAll(
	char*	host, //= args[1]; // 192.168.214.134
	char*	port, //= args[2]; // 32456 
	char*	certSerial,
	char*	certIssuer,
	char*	user //= args[6]; // "John G. Doe"
	);

extern "C" ICCMAPI_API int CheckHealth(
	char*	host, //= args[1]; // 192.168.214.134
	char*	port, //= args[2]; // 32456 
	char*	certSerial,
	char*	certIssuer
);

extern "C" ICCMAPI_API int IsDeviceIsActive(
	char*	host, //= args[1]; // 192.168.214.134
	char*	port, //= args[2]; // 32456 
	char*	certSerial,
	char*	certIssuer,
	char*	user);