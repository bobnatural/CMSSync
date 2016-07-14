#include "stdafx.h"
#include "TestCCM.h"
#include <memory>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace ActivIdentity;

#define tcout dbgout

TestCCM::TestCCM() : logger("CMSSync")
{
}


TestCCM::~TestCCM()
{
}

int TestCCM::Process(
	const char*	cmd, //= args[0]; // CREATE-CPR
	const char*	host, //= args[1]; // 192.168.214.134
	const char*	port, //= args[2]; // 32456 
	const char *certSerial,
	const char *certIssuer,
	//const char*	client, //= args[3]; // client.pfx
	//const char*	pwd, //= args[4]; // actividentity
	//const char*	CA, //= args[5]; // root.cer
	const char*	user, //= args[6]; // "John G. Doe"
	const char*	cprContent, //= args[7]; // "CPR 2.1.8.sample.signed.xml"
	const char*	pin, //= args[8]; // ""
	const char*	policy, //= args[9]; // "F2F" or "test6"
	const char*	reason //= args[10]; // ""
	)
{
	dbgout.clear();
	int exitCode = 0;
	if (strnlen_s(host, 500) == 0 || strnlen_s(port, 500) == 0)
	{
		dbgout << _T("Host or Port is not set.");
		logError();
		return -1;
	}

	try{
		Connect(host, port, certSerial, certIssuer);
	}
	catch (LocalizedException* e)
	{
		dbgout << "Error in Connect to CCM ([" << host << "][" << port << "][" << certSerial << "][" << certIssuer << "])" << endl;
		if (credSessID.length() == 0)
			dbgout << "Error to open CredentialManager. Credential may be invalid.";
		else if (smoSessID.length() == 0)
			dbgout << "Error to open SecurityModuleManager.";
		else if (userSessID.length() == 0)
			dbgout << "Error to open UserManager.";
		else if (walletSessID.length() == 0)
			dbgout << "Error to open WalletManager.";
		
		printException(e);
		exitCode = -1;
	}
	catch (...)
	{
		dbgout << _T("Unknown error in Connect to CCM.");
		logError();
		exitCode = -2;
	}

	if (exitCode == 0)
	{
		try{
			if (0 == strcmp(cmd, _T("CREATE-CPR")))
				createCPR(TString(user), TString(cprContent), TString(policy), TString(reason));
			else if (0 == strcmp(cmd, _T("TERMINATE-ALL")))
				TerminateAll(user);
		}
		catch (LocalizedException* e)
		{
			//logger.ErrorFormat("Error in processing user '%s' in CCM: %s, %s", user, e->getMessage().c_str(), e->getErrorCode().c_str());
			dbgout << _T("Error in processing CCM operation '") << cmd << "'. User=" << user;
			printException(e);
			logError();
			exitCode = -11;
		}
		catch (...)
		{
			dbgout << _T("Unknown error in processing CCM operation '") << cmd << "'. User=" << user;
			logError();
			exitCode = -12;
		}
	}

	try{
		DisConnect();
	}
	catch (...)
	{
	}
	return exitCode;
}


////////////////////////////////////////////////
//Open connection with service managers
////////////////////////////////////////////////
void TestCCM::Connect(const TString& host, const TString& port, const TString& certSerial, const TString& certIssuer)
{
	// Assemble initial parameters
	ParameterVector config;
	config.push_back(Parameter(CONFIG_HOST_NAME, host));
	config.push_back(Parameter(CONFIG_HOST_PORT, port));
	config.push_back(Parameter(CONFIG_TIMEOUT, _T("120000")));		// 2min = 1000*60*2
	config.push_back(Parameter(CONFIG_RETRY, _T("2")));

	ParameterVector myidConfig;
	myidConfig.push_back(Parameter(CONFIG_HOST_NAME, host));
	myidConfig.push_back(Parameter(CONFIG_HOST_PORT, port));
	myidConfig.push_back(Parameter(CONFIG_TIMEOUT, _T("120000")));		// 2min = 1000*60*2
	myidConfig.push_back(Parameter(CONFIG_RETRY, _T("2")));

	if (certSerial.length() > 0)
	{
		config.push_back(Parameter(CONFIG_CERTSERIAL, certSerial));
		config.push_back(Parameter(CONFIG_CERTISSUER, certIssuer));
		myidConfig.push_back(Parameter(CONFIG_CERTSERIAL, certSerial));
		myidConfig.push_back(Parameter(CONFIG_CERTISSUER, certIssuer));
	}
	//logger.SuccessFormat("Connect to [%s] port [%s] with certSerial=[%s] certIssuer=[%s]", host.c_str(), port.c_str(), certSerial.c_str(), certIssuer.c_str());

	// Synchronization Manager
	if (syncMgr == NULL || !syncMgr->isSessionOpen())
	{
		// NOTE: For Sun Solaris CMS server or SSL Termination, must use MDIDC Port 49152
		if (syncMgr == NULL)
			syncMgr = SyncManagerFactory::newInstance(&myidConfig);
		else
			dbgout << _T("Reconnecting...") << endl;

		syncSessID = syncMgr->openSession(NULL);
		if (syncMgr->isSessionOpen())
		{
			//dbgout << _T("Opened SyncManager Session.") << endl;
			dbgout << _T("SyncManager Server Version: ") << syncMgr->getVersion().c_str() << _T(".") << endl;
			//logger.SuccessFormat("SyncManager Server Version: %s. SessionID=%s", syncMgr->getVersion().c_str(), syncSessID.c_str());
		}
	}

	// Credential Manager
	if (credMgr == NULL || !credMgr->isSessionOpen())
	{
		if (credMgr == NULL)
			credMgr = CredentialManagerFactory::newInstance(&config);
		else
			dbgout << _T("Reconnecting...") << endl;

		credSessID = credMgr->openSession(NULL);
		if (credMgr->isSessionOpen())
		{
			//dbgout << _T("Opened CredentialManager Session.") << endl;
			dbgout << _T("CredentialManager Server Version: ") << credMgr->getVersion().c_str() << _T(".") << endl;
			//logger.SuccessFormat("CredentialManager Server Version: %s. SessionID=%s", credMgr->getVersion().c_str(), credSessID.c_str());
		}
	}

	// Security Module Manager
	if (smoMgr == NULL || !smoMgr->isSessionOpen())
	{
		if (smoMgr == NULL)
			smoMgr = SecurityModuleManagerFactory::newInstance(&config);
		else
			dbgout << _T("Reconnecting...") << endl;

		smoSessID = smoMgr->openSession(NULL);
		if (smoMgr->isSessionOpen())
		{
			//dbgout << _T("Opened SecurityModuleManager Session.") << endl;
			dbgout << _T("SecurityModuleManager Server Version: ") << smoMgr->getVersion().c_str() << _T(".") << endl;
			//logger.SuccessFormat("SecurityModuleManager Server Version: %s. SessionID=%s", smoMgr->getVersion().c_str(), smoSessID.c_str());
		}
	}

	// User Manager
	if (userMgr == NULL || !userMgr->isSessionOpen())
	{
		if (userMgr == NULL)
			userMgr = UserManagerFactory::newInstance(&config);
		else
			dbgout << _T("Reconnecting...") << endl;

		userSessID = userMgr->openSession(NULL);
		if (userMgr->isSessionOpen())
		{
			//dbgout << _T("Opened UserManager Session.") << endl;
			dbgout << _T("UserManager Server Version: ") << userMgr->getVersion().c_str() << _T(".") << endl;
			//logger.SuccessFormat("UserManager Server Version: %s. SessionID=%s", userMgr->getVersion().c_str(), userSessID.c_str());
		}
	}

	// Wallet Manager
	if (walletMgr == NULL || !walletMgr->isSessionOpen())
	{
		if (walletMgr == NULL)
			walletMgr = WalletManagerFactory::newInstance(&config);
		else
			dbgout << _T("Reconnecting...") << endl;

		walletSessID = walletMgr->openSession(NULL);
		if (walletMgr->isSessionOpen())
		{
			dbgout << _T("WalletManager Server Version: ") << walletMgr->getVersion().c_str() << _T(".") << endl;
			//logger.SuccessFormat("WalletManager Server Version: %s. SessionID=%s", walletMgr->getVersion().c_str(), walletSessID.c_str());
		}
	}
}


/**
* Close connection with service managers
*/
void TestCCM::DisConnect()
{
	// Close Cred Manager
	if (credMgr != NULL)
	{
		try
		{
			if (credMgr->isSessionOpen())
			{
				credMgr->closeSession();

				if (!credMgr->isSessionOpen())
					tcout << _T("Closed CredentialManager Session.") << endl;
				else
					tcout << _T("CredentialManager Session NOT Closed.") << endl;
			}
		}
		catch (LocalizedException*)
		{
			tcout << _T("Unexpected Failure:") << endl;
			//printException(locEx);
		}
		catch (std::exception* stdEx)	// should never happen
		{
			tcout << _T("Unexpected Failure: ") << stdEx->what() << endl;
		}
		catch (...)
		{
			tcout << _T("Unexpected Failure.") << endl;
		}
		delete credMgr;
	}

	// Close Sync Manager
	if (syncMgr != NULL)
	{
		try
		{
			if (syncMgr->isSessionOpen())
			{
				syncMgr->closeSession();

				if (!syncMgr->isSessionOpen())
					tcout << _T("Closed SyncManager Session.") << endl;
				else
					tcout << _T("SyncManager Session NOT Closed.") << endl;
			}
		}
		catch (LocalizedException*)
		{
			tcout << _T("Unexpected Failure:") << endl;
			//printException(locEx);
		}
		catch (std::exception* stdEx)	// should never happen
		{
			tcout << _T("Unexpected Failure: ") << stdEx->what() << endl;
		}
		catch (...)
		{
			tcout << _T("Unexpected Failure.") << endl;
		}
		delete syncMgr;
	}

	// Close Security Manager
	if (smoMgr != NULL)
	{
		try
		{
			if (smoMgr->isSessionOpen())
			{
				smoMgr->closeSession();

				if (!smoMgr->isSessionOpen())
					tcout << _T("Closed SecurityModuleManager Session.") << endl;
				else
					tcout << _T("SecurityModuleManager Session NOT Closed.") << endl;
			}
		}
		catch (LocalizedException*)
		{
			tcout << _T("Unexpected Failure:") << endl;
			//printException(locEx);
		}
		catch (std::exception* stdEx)	// should never happen
		{
			tcout << _T("Unexpected Failure: ") << stdEx->what() << endl;
		}
		catch (...)
		{
			tcout << _T("Unexpected Failure.") << endl;
		}
		delete smoMgr;
	}

	// Close User Manager
	if (userMgr != NULL)
	{
		try
		{
			if (userMgr->isSessionOpen())
			{
				userMgr->closeSession();

				if (!userMgr->isSessionOpen())
					tcout << _T("Closed UserManager Session.") << endl;
				else
					tcout << _T("UserManager Session NOT Closed.") << endl;
			}
		}
		catch (LocalizedException*)
		{
			tcout << _T("Unexpected Failure:") << endl;
			//printException(locEx);
		}
		catch (std::exception* stdEx)	// should never happen
		{
			tcout << _T("Unexpected Failure: ") << stdEx->what() << endl;
		}
		catch (...)
		{
			tcout << _T("Unexpected Failure.") << endl;
		}
		delete userMgr;
	}

	// Close Wallet Manager
	if (walletMgr != NULL)
	{
		try
		{
			if (walletMgr->isSessionOpen())
			{
				walletMgr->closeSession();

				if (!walletMgr->isSessionOpen())
					tcout << _T("Closed WalletManager Session.") << endl;
				else
					tcout << _T("WalletManager Session NOT Closed.") << endl;
			}
		}
		catch (LocalizedException*)
		{
			tcout << _T("Unexpected Failure:") << endl;
			//printException(locEx);
		}
		catch (std::exception* stdEx)	// should never happen
		{
			tcout << _T("Unexpected Failure: ") << stdEx->what() << endl;
		}
		catch (...)
		{
			tcout << _T("Unexpected Failure.") << endl;
		}
		delete walletMgr;
	}
}


/**
* Display Exception details
*/
void TestCCM::printException(LocalizedException* pExc)
{
	tcout << endl << _T("---") << endl;
	tcout << _T("EXCEPTION Detail:") << endl;
	tcout << _T("message:            ") << pExc->getMessage().c_str() << endl;
	tcout << _T("additionalMessage:  ") << pExc->getAdditionalMessage().c_str() << endl;
	tcout << _T("localizedMessage:   ") << pExc->getLocalizedMessage().c_str() << endl;
	tcout << _T("unlocalizedMessage: ") << pExc->getUnlocalizedMessage().c_str() << endl;
	tcout << _T("errorCode:          ") << pExc->getErrorCode().c_str() << endl;

	TStringVector* params = pExc->getParameters();
	for (unsigned int ix = 0; params != NULL && ix<params->size(); ix++)
	{
		tcout << _T("parameter:          ") << params->at(ix).c_str() << endl;
	}
	if (params != NULL) delete params;

	//tcout << _T(" ") << endl;
	logError();
}

void TestCCM::logError()
{
	std::string msg = dbgout.str();
	std::replace(msg.begin(), msg.end(), '%', '_'); // If the string that you log contains %n, where n is an integer value (for example, %1), the event viewer treats it as an insertion string.
	DWORD eventID = 14;
	if (msg.find("AIMS_NO_SUCH_USER") != string::npos)
		eventID = 12;
	logger.Error(msg.c_str(), eventID);
}



//
//// Convert from .NET unicode to mb ccm
//TString TestCCM::WCharT(const wchar_t* wstr)
//{
//	int result_c = WideCharToMultiByte(CP_ACP, 0, wstr, -1, 0, 0, 0, 0);
//	if (!result_c)
//		throw new std::exception("Failed to convert string");
//
//	return TString(result_c);
//}

TString TestCCM::readFile(const TString& fileName)
{
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	std::string filePath = string(buffer).substr(0, pos);
	filePath.append("\\").append(fileName.c_str());
	std::ifstream ifs(filePath.c_str());
	if (!ifs)
		throw new std::exception((std::string("file not found:").append(filePath)).c_str());

	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	return TString(content.c_str());
}

UserId TestCCM::getUserId(TString user)
{
	// Get user handle
	//CriteriaVector criterias;
	//criterias.push_back(Criteria(CRITERIA_ID_USERID, CRITERIA_COMPARISON_EQUAL, user));
	//UserIdVector* userIds = userMgr->findUserIds(&criterias, 1);

	UserIdVector userIds; // = new  UserId[]{new UserId(user)};
	userIds.push_back(user);
	TStringVector atttoReturn;
	atttoReturn.push_back("cn");
	UserVector* users = userMgr->getUsers(&userIds, &atttoReturn);

	if (users == NULL || users->size() == 0)
	{
		NoSuchUserException* nsue = new NoSuchUserException();
		TString errorMsg = _T("The user ") + TString(user) + _T(" was not found.");
		nsue->setMessage(errorMsg);
		nsue->setErrorCode(_T("AIMS_NO_SUCH_USER"));
		UserId userId = UserId(user);
		nsue->setUserId(&userId);
		throw nsue;
	}
	logger.SuccessFormat("User found: %s", user.c_str());

	// Copy objects to stack and free memory
	UserId userId(*(users->at(0).getId()));
	delete users;
	return userId;
}

int TestCCM::createCPR(const TString& user, const TString& cprData, const TString& policy, const TString& reason)
{
	UserId userId = getUserId(user);

	EnrollmentDataVector enrollmentVector;
	EnrollmentDataValue enrollDataDn;
	enrollDataDn.setId("piv:PIVData");
	enrollDataDn.setValue(cprData);
	enrollDataDn.setType("String");
	enrollDataDn.setEncoding("None");
	enrollmentVector.push_back(enrollDataDn);

	dbgout << "addEnrollmentData cprFileLength=" << cprData.length() << " bytes. userMgrSessionID=" << userSessID.c_str()<<" ... ";
	//logger.SuccessFormat("addEnrollmentData cprFileLength=%d bytes. userMgrSessionID=%s. SessionIsOpen=%s ... ", cprData.length(), userSessID.c_str(), userMgr->isSessionOpen() ? "true" : "false");
	userMgr->addEnrollmentData(&userId, &enrollmentVector);

	if (policy.length() > 0)
	{
		dbgout << "getBoundWalletFromUser Policy=" << policy.c_str();
		//logger.SuccessFormat("getBoundWalletFromUser Policy=%s. walletMgrSessionID=%s. SessionIsOpen=%s. ...", policy.c_str(), walletSessID.c_str(), walletMgr->isSessionOpen() ? "true" : "false");

		WalletId*	walletId = walletMgr->getBoundWalletFromUser(&userId);

		auto devStatus = GetLifecycleStatus(walletId);
		if (devStatus.length() > 0)
		{
			logger.WarnFormat("Skip submitActions in CCM. Device: %s", devStatus.c_str());
			return 1;
		}

		cancelActions(walletId); // clear all pending actions ...

		// Create Action
		Action action;
		action.setType(ACTION_TYPE_PRODUCTION);

		//if (!policy.empty())
		//	action.setApplicationSet(policy);

		// Action Runtime parameters
		ParameterVector actionRuntimeParams;
		actionRuntimeParams.push_back(Parameter(ACTION_RUNTIME_PARAM_POLICY, policy));
		if (!reason.empty())
			actionRuntimeParams.push_back(Parameter(ACTION_RUNTIME_PARAM_REASON, reason));
		action.setRuntimeParameters(&actionRuntimeParams);

		// Place action into Vector
		ActionVector actions;
		actions.push_back(action);

		//logger.SuccessFormat("submitActions SessionIsOpen=%s. ...", walletMgr->isSessionOpen() ? "true" : "false");
		// Submit Production Action
		TStringVector *actionIds = walletMgr->submitActions(walletId, &actions, _T(""));

		logger.SuccessFormat("submitActions done. SessionIsOpen=%s. Returned ActionIDs count=%d ...", walletMgr->isSessionOpen() ? "true" : "false", actionIds != NULL ? actionIds->size() : 0);

		// cleanup
		if (actionIds != NULL) delete actionIds;
	}
	return 0;
}

void TestCCM::cancelActions(WalletId* pWalletId)
{
	tcout << _T("Clearing all pending requests for ") << pWalletId->getId().c_str() << "...." << endl;

	// Create search criteria
	CriteriaVector SearchCriteria;
	SearchCriteria.push_back(
		Criteria(CRITERIA_ACTION_PARENT_WALLET, CRITERIA_COMPARISON_EQUAL, pWalletId->getId()));

	// Return list of associated requests
	TStringVector* pActionIds = walletMgr->findActionIds(&SearchCriteria, 0);

	if (pActionIds && pActionIds->size() <= 0)
		tcout << _T("User has no pending actions") << endl;

	// Cancel each request found
	for (int i = 0; pActionIds && i < (int)pActionIds->size(); i++)
	{
		tcout << _T("Canceling action id: ") << pActionIds->at(i).c_str() << endl;

		ParameterVector params;
		walletMgr->performActionProcess(pActionIds->at(i), ACTION_PROCESS_CANCEL, &params);
	}

	// Free action id vector
	delete pActionIds;
}

/**
* @param userId
* @return WalletId
*/
WalletId TestCCM::getWalletId(UserId* userId)
{
	// Get Wallet handle
	WalletId* pWalletId = walletMgr->getBoundWalletFromUser(userId);

	// Copy objects to stack and free memory
	WalletId walletId(pWalletId->getId());
	delete pWalletId;

	tcout << _T("Found wallet Id: ") << walletId.getId().c_str() << _T(".") << endl;
	return walletId;
}

int TestCCM::TerminateAll(const TString &user)
{
	UserId userId = getUserId(user);
	WalletId walletId = getWalletId(&userId);

	// Obtain the security module ids
	SecurityModuleIdVector* smIds = smoMgr->getBoundSMFromWallet(&walletId);

	// Trigger process for all security modules
	for (int i = 0; smIds != NULL && i < (int)smIds->size(); i++)
	{
		//unbind(&smIds->at(i), walletId);
		SecurityModuleId* const smId = &(smIds->at(i));

		tcout << _T("Unbinding device ") << smId->getId().c_str() << _T("....") << endl;

		try
		{
			// Unbind/Terminate the card
			walletMgr->unbindSecurityModule(&walletId, smId);
			//tcout << _T("Unbound Security Module: ") << smId->getType().c_str() << _T(", ") << smId->getId().c_str() << _T(" from Wallet:") << walletId->getId().c_str() << _T(".") << endl;
		}
		catch (SecurityModuleNotBoundException* smnbe)
		{
			//tcout << _T("Warning: ") << smnbe->getMessage().c_str() << endl;
			logger.ErrorFormat("Error unbinding device %s: SecurityModuleNotBoundException. %s ", smId->getId().c_str(), smnbe->getMessage().c_str());
			delete smnbe;
		}
	}

	// cleanup
	delete smIds;
	return 0;
}

TString TestCCM::GetLifecycleStatus(WalletId* walletId)
{
	// Obtain the security module ids
	SecurityModuleIdVector* smIds = smoMgr->getBoundSMFromWallet(walletId);

	// Trigger process for all security modules
	for (int i = 0; smIds != NULL && i < (int)smIds->size(); i++)
	{
		SecurityModuleId* const smId = &(smIds->at(i));
		try
		{
			TString status = smoMgr->getLifecycleStatus(smId);
			if (status.trim().length() > 0)
				return smId->getId() + _T(" Status: ") + status;
		}
		catch (SecurityModuleNotBoundException* smnbe)
		{
			logger.ErrorFormat("Error getLifecycleStatus %s: SecurityModuleNotBoundException. %s ", smId->getId().c_str(), smnbe->getMessage().c_str());
			delete smnbe;
		}
	}
	
	delete smIds; // cleanup
	return "";
}