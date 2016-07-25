#pragma once

#include <sstream>
#include <iostream>

#include "Utils.h"
#include "WinLogger.h"

// ActivIdentity CCM API includes
#include "aiCCMUtil.h"
#include "aiCCMWallet.h"
#include "aiCCMUser.h"
#include "aiCCMCred.h"
#include "aiCCMSmo.h"
#include "aiCCMTcd.h"


using namespace ActivIdentity;

class TestCCM
{
public:
	TestCCM();
	~TestCCM();

	/**
	* Instance variables
	*/
private:
	SyncManager				*syncMgr = NULL;
	SecurityModuleManager	*smoMgr = NULL;
	UserManager				*userMgr = NULL;
	WalletManager			*walletMgr = NULL;
	CredentialManager       *credMgr = NULL;
	TString					syncSessID;
	TString					credSessID;
	TString					smoSessID;
	TString					userSessID;
	TString					walletSessID;

	WinLogger				logger;
	std::stringstream		dbgout;
	
	// commands:
	int createCPR(const TString& user, const TString& cprFile, const TString& policy, const TString& reason);
	int TerminateAll(const TString &user);
	int IsDeviceIsActive(const TString &user);
	void cancelActions(WalletId* pWalletId);
	TString GetLifecycleStatus(WalletId* pWalletId);

	UserId getUserId(TString user);
	WalletId getWalletId(UserId* userId);

// Helpers:	
	//static TString WCharT(const wchar_t*);
	static TString readFile(const TString& fileName);
	void printException(LocalizedException* pExc);
	void logError();

public:
	// All commands need Connect-Disconnect:
	void Connect(const TString& host, const TString& port, const TString& certSerial, const TString& certIssuer);
	void DisConnect();

public:
	int Process(
		const  char*	cmd, //= args[0]; // CREATE-CPR
		const  char*	host, //= args[1]; // 192.168.214.134
		const  char*	port, //= args[2]; // 32456 
		const  char*	certSerial,
		const  char*	certIssuer,
		//const  char*	client, //= args[3]; // client.pfx
		//const  char*	pwd, //= args[4]; // actividentity
		//const  char*	CA, //= args[5]; // root.cer
		const  char*	user, //= args[6]; // "John G. Doe"
		const  char*	reader, //= args[7]; // "CPR 2.1.8.sample.signed.xml"
		const  char*	pin, //= args[8]; // ""
		const  char*	policy, //= args[9]; // "F2F" or "test6"
		const  char*	reason //= args[10]; // ""
		); 
};

