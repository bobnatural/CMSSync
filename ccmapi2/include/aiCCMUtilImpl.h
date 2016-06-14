/*
 * (c) Copyright 2006-2008 ActivIdentity
 * All Rights Reserved.
 *
 * This program is an unpublished copyrighted work which is proprietary
 * to ActivCard. This computer program includes Confidential,
 * Proprietary Information and is a Trade Secret of ActivCard.
 * Any use, disclosure, modification and/or reproduction is prohibited
 * unless authorized in writing by ActivCard.
 *
 * WARNING:  Unauthorized reproduction of this program as well as
 * unauthorized preparation of derivative works based upon the
 * program or distribution of copies by sale, rental, lease or
 * lending are violations of federal copyright laws and state trade
 * secret laws, punishable by civil and criminal penalties.
 * 
 * Created on May 5, 2006
 */
#pragma once
#include "aiCCMConstants.h"
#include "aiCCMErrorCodes.h"
#include "aiCCMUtil.h"

#ifdef AICCMUTIL_EXPORTS
#define AICCMUTIL_API __declspec(dllexport)
#define EXPUTIL_TEMPLATE
#else
#define AICCMUTIL_API __declspec(dllimport)
#define EXPUTIL_TEMPLATE extern
#endif

BOOL APIENTRY
		DllMain(
				HINSTANCE	hModule,
                DWORD		ul_reason_for_call,
                LPVOID		lpReserved);

struct soap;

namespace AI
{
	// Session Manager Implementation
	class AICCMUTIL_API SessionManagerImpl //: public ManagerInstance, public SessionManager
	{
	public:
		virtual TString getClientVersion() = 0;

	protected:
		virtual TString getConfigURL(ParameterVector* params);
		virtual void initProxy(soap* proxy, ParameterVector* initParms);
		virtual void handleFault(int rc);
		soap* pProxy;
		int nRetries;
	};

} // namespace AI
namespace ActivIdentity = AI;