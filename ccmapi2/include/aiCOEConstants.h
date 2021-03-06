#pragma once
/*
 * (c) Copyright 2006-2009 ActivIdentity
 * All Rights Reserved.
 *
 * This program is an unpublished copyrighted work which is proprietary
 * to ActivIdentity. This computer program includes Confidential,
 * Proprietary Information and is a Trade Secret of ActivIdentity.
 * Any use, disclosure, modification and/or reproduction is prohibited
 * unless authorized in writing by ActivIdentity.
 *
 * WARNING:  Unauthorized reproduction of this program as well as
 * unauthorized preparation of derivative works based upon the
 * program or distribution of copies by sale, rental, lease or
 * lending are violations of federal copyright laws and state trade
 * secret laws, punishable by civil and criminal penalties.
 * 
 * Created on January 27, 2009
 */

/**
 * \file aiCOEConstants.h
 * \brief A set of constants used by the CCM API Client Operation Executor infrastructure.
 */

// --------------------------------- GLOBAL COE CONSTANTS ---------------------------------

	// *** Synchronize Action Script Types (Script name) ***

	/**
	 * Constant: Synchronize Action Script Type -- login using an authenticator
	 */
	#define	ACTION_SCRIPT_AUTHENTICATE				_T("authenticate")

	/**
	 * Constant: Synchronize Action Script Type -- notify start of synchronization operation
	 */
	#define	ACTION_SCRIPT_BEGIN_SYNCHRO				_T("beginSynchronization")

	/**
	 * Constant: Synchronize Action Script Type -- replaces old authenticator by a new authenticator
	 */
	#define	ACTION_SCRIPT_CHANGE_AUTH				_T("changeAuthenticator")
	
	/**
	 * Constant: Synchronize Action Script Type -- logout an authenticator
	 */
	#define	ACTION_SCRIPT_DEAUTHENTICATE			_T("deauthenticate")

	/**
	 * Constant: Synchronize Action Script Type -- notify end of synchronization operation
	 */
	#define	ACTION_SCRIPT_END_SYNCHRO				_T("endSynchronization")

	/**
	 * Constant: Synchronize Action Script Type -- query authenticator state
	 */
	#define	ACTION_SCRIPT_GET_AUTH_STATE			_T("getAuthenticatorState")

	/**
	 * Constant: Synchronize Action Script Type -- query card state
	 */
	#define	ACTION_SCRIPT_GET_CARD_STATE			_T("getCardState")

	/**
	 * Constant: Synchronize Action Script Type -- retrieve platform identifiers
	 */
	#define	ACTION_SCRIPT_GET_PLATFORM_IDS			_T("getPlatformIdentifiers")

	/**
	 * Constant: Synchronize Action Script Type -- retrieve reader list
	 */
	#define	ACTION_SCRIPT_GET_READER_LIST 			_T("getReaderList")

	/**
	 * Constant: Synchronize Action Script Type -- retrieve SKI information
	 */
	#define	ACTION_SCRIPT_GET_SKI_INFO				_T("getSKIInformation")

	/**
	 * Constant: Synchronize Action Script Type -- perform signature of data
	 */
	#define	ACTION_SCRIPT_SIGN_DATA					_T("signData")


	// *** Input Access Parameters ***

	/**
	 * Constant: Access Parameter -- previous PIN
	 */
	#define	ACCESS_PARAM_SMARTCARD_OLD_PIN			_T("OLD_PIN")

	/**
	 * Constant: Access Parameter -- replacement PIN
	 */
	#define	ACCESS_PARAM_SMARTCARD_NEW_PIN			_T("NEW_PIN")
	
	
	// *** Input Runtime Parameters ***

	/**
	 * Constant: Runtime Parameter -- indicates punishable tion result to ACTION_SCRIPT_END_SYNCHRO notification
	 */
	#define ACTION_INPUT_PARAM_PIN					ACCESS_PARAM_SMARTCARD_PIN

	/**
	 * Constant: Runtime Parameter -- indicates synchronization result to ACTION_SCRIPT_END_SYNCHRO notification
	 */
	#define ACTION_INPUT_PARAM_SYNCSTATUS			_T("SYNCSTATUS")
	
	/**
	 * Constant: Runtime Parameter -- indicates failure details to ACTION_SCRIPT_END_SYNCHRO notification
	 */
	#define ACTION_INPUT_PARAM_SYNCEXCEPTION		_T("SYNCEXCEPTION")
	
	/**
	 * Constant: Runtime Parameter -- indicates readername
	 */
	#define ACTION_INPUT_PARAM_READERNAME			_T("READERNAME")
	
	/**
	 * Constant: Runtime Parameter -- indicates type of security module
	 */
	#define ACTION_INPUT_PARAM_SMOTYPE				_T("SMOTYPE")

	/**
	 * Constant: Runtime Parameter -- indicates unique identifier of security module
	 */
	#define ACTION_INPUT_PARAM_SMOID				_T("SMOID")

	/**
	 * Constant: Runtime Parameter -- indicates PIN identifier value
	 */
	#define ACTION_INPUT_PARAM_PINID				_T("PINID")
	
	/**
	 * Constant: Runtime Parameter -- indicates SKI identifier value
	 */
	#define ACTION_INPUT_PARAM_SKIID				_T("SKIID")
	
	/**
	 * Constant: Runtime Parameter -- indicates authenticate operation
	 */
	#define ACTION_INPUT_PARAM_AUTH_TYPE			_T("AUTH_TYPE")
	
	/**
	 * Constant: Runtime Parameter -- indicates PKI identifier value
	 */
	#define ACTION_INPUT_PARAM_KEYID				_T("KEYID")
	
	/**
	 * Constant: Runtime Parameter -- indicates data value
	 */
	#define ACTION_INPUT_PARAM_DATA					_T("DATA")
	
	/**
	 * Constant: Runtime Parameter -- indicates algorithm identifier value
	 */
	#define ACTION_INPUT_PARAM_ALGOID				_T("ALGOID")

	
	// *** Output Parameters ***
	
	/**
	 * Constant: Output Parameter -- indicates script execution result
	 */
	#define ACTION_OUTPUT_PARAM_ERROR				_T("ERROR")
	
	/**
	 * Constant: Output Parameter -- indicates script failure details
	 */
	#define ACTION_OUTPUT_PARAM_MESSAGE				_T("MESSAGE")
	
	/**
	 * Constant: Output Parameter -- indicates type of security module
	 */
	#define ACTION_OUTPUT_PARAM_SMOTYPE				ACTION_INPUT_PARAM_SMOTYPE
	
	/**
	 * Constant: Output Parameter -- indicates unique identifier of security module
	 */
	#define ACTION_OUTPUT_PARAM_SMOID				ACTION_INPUT_PARAM_SMOID
	
	/**
	 * Constant: Output Parameter -- indicates reader name list
	 */
	#define ACTION_OUTPUT_PARAM_READERS				_T("READERS")
	
	/**
	 * Constant: Output Parameter -- indicates card state
	 */
	#define ACTION_OUTPUT_PARAM_CARDSTATE			_T("CARDSTATE")
	
	/**
	 * Constant: Output Parameter -- indicates authenticator state
	 */
	#define ACTION_OUTPUT_PARAM_AUTHSTATE			_T("AUTHSTATE")

	/**
	 * Constant: Output Parameter -- indicates number of remaining PIN tries
	 */
	#define ACTION_OUTPUT_PARAM_LEFT_PIN_TRY		_T("LEFTPINTRY")
	
	/**
	 * Constant: Output Parameter -- indicates signed data value
	 */
	#define ACTION_OUTPUT_PARAM_SIGNEDDATA			_T("SIGNEDDATA")
	
	/**
	 * Constant: Output Parameter -- indicates SKI counter value
	 */
	#define ACTION_OUTPUT_PARAM_SKICOUNTER			_T("SKICOUNTER")
	
	/**
	 * Constant: Output Parameter -- indicates SKI clock value
	 */
	#define ACTION_OUTPUT_PARAM_SKICLOCK			_T("SKICLOCK")


	// *** Card State ***

	/**
	 * Constant: Card State -- card manager not locked
	 */
	#define GCP_CARD_STATE_VALID					1

	/**
	 * Constant: Card State -- card manager locked
	 */
	#define GCP_CARD_STATE_LOCKED					2
	
	/**
	 * Constant: Authenticator State -- authenticated
	 */
	#define	GCP_AUTH_AUTHENTICATED					1 
	
	/**
	 * Constant: Authenticator State -- not authenticated
	 */
	#define	GCP_AUTH_NOT_AUTHENTICATED				2 
	
	/**
	 * Constant: Authenticator State -- locked
	 */
	#define	GCP_AUTH_LOCKED							3 

	/**
	 * Constant : static authenticator type
	 */
	#define GCP_STATIC_AUTH_TYPE					1


	// *** Error Codes ***

	#define GCP_ERRORBASE							0x80E8 				//33000
	
	/**
	 * Constant : Error code -- OK
	 */
	#define GCP_OK									0x0					//OK
	
	/**
	 * Constant : Error code -- Reader (slot id) invalid
	 */
	#define GCP_UNKNOWN_READER						(GCP_ERRORBASE+0x1)	//reader (slot id) invalid
	
	/**
	 * Constant : Error code -- Card (token) invalid
	 */
	#define GCP_UNKNOWN_CARD						(GCP_ERRORBASE+0x2)	//card (token) invalid
	
	/**
	 * Constant : Error code -- Card has been removed
	 */
	#define GCP_CARD_REMOVED						(GCP_ERRORBASE+0x3)	//card has been removed
	
	/**
	 * Constant : Error code -- Bad Parameter
	 */
	#define GCP_BAD_PARAMETER						(GCP_ERRORBASE+0x4)	//bad parameter
	
	/**
	 * Constant : Error code -- Unknown State
	 */
	#define GCP_UNKNOWN_STATE						(GCP_ERRORBASE+0x5)	//unknown state
	
	/**
	 * Constant : Error code -- Access Denied
	 */
	#define GCP_ACCESS_DENIED						(GCP_ERRORBASE+0x6)	//access denied
	
	/**
	 * Constant : Error code -- PIN is already locked
	 */
	#define GCP_PIN_LOCKED							(GCP_ERRORBASE+0x7)	//Pin is already locked
	
	/**
	 * Constant : Error code -- User already logged in
	 */
	#define	GCP_USER_NOT_LOGGED_IN					(GCP_ERRORBASE+0x8)	//User already logged in
	
	/**
	 * Constant : Error code -- Another User already logged in
	 */
	#define	GCP_USER_ANOTHER_ALREADY_LOGGED_IN		(GCP_ERRORBASE+0x9)	//Another User already logged in
	
	/**
	 * Constant : Error code -- List reader failed
	 */
	#define	GCP_LIST_READER_FAILED					(GCP_ERRORBASE+0xA)	//List Reader Failed
	
	/**
	 * Constant : Error code -- Data not found
	 */
	#define	GCP_DATA_NOT_FOUND						(GCP_ERRORBASE+0xB)	//Data not found
	
	/**
	 * Constant : Error code -- Not supported
	 */
	#define GCP_FCT_NOT_SUPPORTED					(GCP_ERRORBASE+0xC)	//not supported
	
	/**
	 * Constant : Error code -- Unknown Error
	 */
	#define GCP_UNKNOWN_ERROR						(GCP_ERRORBASE+0xD)	//unknown error

	/**
	 * Constant : Error code -- Memory Error
	 */
	#define GCP_MEMORY_ERROR						(GCP_ERRORBASE+0xE)	//memory error
	
	/**
	 * Constant : Error code -- General Error
	 */
	#define GCP_GENERAL_ERROR						(GCP_ERRORBASE+0xF)	//general error
	
	/**
	 * Constant : Error code -- function failed Error
	 */
	#define GCP_FUNCTION_FAILED						(GCP_ERRORBASE+0x10) //function failed error
	
	/**
	 * Constant : Error code -- Bad arguments Error
	 */
	#define GCP_BAD_ARGUMENTS						(GCP_ERRORBASE+0x11) //Bad arguments error

	/**
	 * Constant : Error code -- Device Error
	 */
	#define GCP_DEVICE_ERROR						(GCP_ERRORBASE+0x12) //Device error

	/**
	 * Constant : Error code -- Bad Key arguments Error
	 */
	#define GCP_BAD_KEY_ARGUMENTS					(GCP_ERRORBASE+0x13) //Bad Key arguments error

	/**
	 * Constant : Error code -- Pin Len Range Error
	 */
	#define GCP_PIN_LEN_RANGE						(GCP_ERRORBASE+0x14) //Pin Len Range error

	/**
	 * Constant : Error code -- Session closed Error
	 */
	#define GCP_SESSION_CLOSED						(GCP_ERRORBASE+0x15) //Session closed Error

	/**
	 * Constant : Error code -- Session Count Error
	 */
	#define GCP_SESSION_COUNT						(GCP_ERRORBASE+0x16) //Session Count Error

	/**
	 * Constant : Error code -- Session Handle Invalid Error
	 */
	#define GCP_SESSION_HANDLE_INVALID				(GCP_ERRORBASE+0x17) //Session Handle Invalid Error

	/**
	 * Constant : Error code -- Session Parallel not supported Error
	 */
	#define GCP_SESSION_PARALLEL_NOT_SUPPORTED		(GCP_ERRORBASE+0x18) //Session Parallel not supported Error

	/**
	 * Constant : Error code -- Read Only Error
	 */
	#define GCP_SESSION_READ_ONLY					(GCP_ERRORBASE+0x19) //Session Read Only Error

	/**
	 * Constant : Error code -- Session Exist Error
	 */
	#define GCP_SESSION_EXISTS						(GCP_ERRORBASE+0x1A) //Session Exist Error

	/**
	 * Constant : Error code -- Session Read Only Exist Error
	 */
	#define GCP_SESSION_READ_ONLY_EXISTS			(GCP_ERRORBASE+0x1B) //Session Read Only Exist Error

	/**
	 * Constant : Error code -- Session read write SO Error
	 */
	#define GCP_SESSION_READ_WRITE_SO_EXISTS		(GCP_ERRORBASE+0x1C) //Session read write SO Error

	/**
	 * Constant : Error code -- Template Incomplete Error
	 */
	#define GCP_TEMPLATE_INCOMPLETE					(GCP_ERRORBASE+0x1D) //Template Incomplete Error

	/**
	 * Constant : Error code -- Template Inconsistent Error
	 */
	#define GCP_TEMPLATE_INCONSISTENT				(GCP_ERRORBASE+0x1E) //Template Inconsistent Error
// --------------------------------- GLOBAL COE CONSTANTS ---------------------------------	