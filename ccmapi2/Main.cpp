
#include "stdafx.h"

// C++ includes
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <time.h>
using namespace std;

#ifdef UNICODE
#define tcout wcout
#define tcin wcin
#else
#define tcout cout
#define tcin cin
#endif

#include "TestCCM.h"

// ActivIdentity CCM API includes
#include "aiCCMUtil.h"
#include "aiCCMWallet.h"
#include "aiCCMUser.h"
#include "aiCCMCred.h"
#include "aiCCMSmo.h"
#include "aiCCMTcd.h"
using namespace ActivIdentity;


/**
* Instance variables
*/
SyncManager				*syncMgr = NULL;
SecurityModuleManager	*smoMgr = NULL;
UserManager				*userMgr = NULL;
WalletManager			*walletMgr = NULL;
CredentialManager       *credMgr = NULL;


TString host = _T("localhost");
TString adminPort = _T("32456");
TString myidPort;
TString certIssuer;
TString certSerial;
TString user;
TString reader;
TString pin;
TString policy;
TString reason = ACTION_REASON_NONE;
TString userPwd;
TString userPath;
TString appId;
TString initialPass;
TString walletPg = _T("2");
TString unlockCode;

bool bForceDefaults = false;
bool bForceQuit = false;
bool bResult = false;


/**
* Prompts user and retrieves text from standard input
*/
TString prompt(const TString &text, const TString &defaultValue, bool optional = false)
{
	// prompt for input
	if (!optional && defaultValue.empty())
	{
		tcout << _T("Please enter ") << text.c_str() << _T(" --> ");
	}
	else
	{
		if (defaultValue.empty())
			tcout << _T("Please enter ") << text.c_str() << _T(" (ENTER to skip) --> ");
		else
			tcout << _T("Please enter ") << text.c_str() << _T(" (ENTER for default [") << defaultValue.c_str() << _T("]) --> ");

		if (bForceDefaults)
		{
			tcout << defaultValue.c_str() << endl;
			return defaultValue;
		}
	}

	// read from standard in
	TCHAR buffer[1024] = { 0 };
	tcin.getline(buffer, sizeof(buffer) / sizeof(TCHAR));
	TString value = TString(buffer);

	// use default if none given
	if (!defaultValue.empty() && value.empty())
	{
		tcout << defaultValue.c_str() << endl;
		return defaultValue;
	}

	return value;
}

/**
* Prompts user and retrieves text from standard input
*/
TString prompt(const TString &text)
{
	return prompt(text, _T(""), false);
}

/**
* Prompt user before continuing
*/
void pause()
{
	// skip pause if "forceQuit" enabled
	if (bForceQuit) return;

	// display pause
	tcout << endl << _T("Press any key to continue...") << endl;
	getch();
}

int _tmain(int argc, _TCHAR* argv[])
{

	// Gather configuration info if not already given
	host = prompt(_T("host"), host);
	adminPort = prompt(_T("Admin port"), adminPort);
	if (myidPort.empty()) myidPort = adminPort;
	myidPort = prompt(_T("MDIDC port"), myidPort);
	certIssuer = prompt(_T("certificate issuer"), certIssuer, true);
	if (!certIssuer.empty())
		certSerial = prompt(_T("certificate serial"), certSerial, true);

	tcout << _T("Opening service connections to ") << host.c_str() << _T(" port ") << adminPort.c_str();
	if (adminPort.compareIgnoreCase(myidPort) != 0)
		tcout << _T("/") << myidPort.c_str();
	tcout << _T("...") << endl;

	TestCCM ccm;
	
	ccm.Connect(host, adminPort, certSerial, certIssuer);
	
	tcout << "Connected !!";

	ccm.DisConnect();
		
	pause();
}

