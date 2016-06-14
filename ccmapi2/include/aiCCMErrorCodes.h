#pragma once
/*
 * (c) Copyright 2006-2008 ActivIdentity
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
 * Created on May 5, 2006
 */

/**
 * \file aiCCMErrorCodes.h
 * \brief This constants class contains all error codes for the <b>CMS APIs</b>.
 * \description Use these codes to produce exceptions with localized errors
 * messages potentially suitable for user consumption or reporting.
 */

	/**
     * CMS / AIMS-Enterprise error codes
     */
	#define PRODUCT_AIMS 							_T("AIMS")

    /**
     * Manager layer error codes
     */
	#define LAYER_MANAGER 							_T("MGR")

    /**
     * Provider layer error codes
     */
	#define LAYER_PROVIDER							_T("PROV")

// --------------------------------- CCM API ERROR CODES ---------------------------------

    /**
     * Constant: The attempt to audit CMS process event failed.
     */
    #define AIMS_AUDIT_EVENT_FAILURE 				PRODUCT_AIMS _T("_AUDIT_EVENT_FAILURE")

    /**
     * Constant: The attempt to notify CMS process event failed.
     */
    #define AIMS_AUDIT_NOTIFY_FAILURE 				PRODUCT_AIMS _T("_NOTIFY_EVENT_FAILURE")

    /**
     * Constant: The attempt to bind a security module that was already bound.
     */
    #define AIMS_BOUND_SM 							PRODUCT_AIMS _T("_BOUND_SM")

    /**
     * Constant: The given criteria comparison was invalid.
     */
    #define AIMS_CRITERIA_COMPARISON_INVALID 		PRODUCT_AIMS _T("_CRITERIA_COMPARISON_INVALID")

    /**
     * Constant: The given Criteria was not valid.
     */
    #define AIMS_CRITERIA_INVALID 					PRODUCT_AIMS _T("_CRITERIA_INVALID")

    /**
     * Constant: Credential deletion failed.
     */
    #define AIMS_CREDENTIAL_DELETION_FAILED 		PRODUCT_AIMS _T("_CREDENTIAL_DELETION_FAILED")

    /**
     * Constant: Credential update failed.
     */
    #define AIMS_CREDENTIAL_UPDATE_FAILED 			PRODUCT_AIMS _T("_CREDENTIAL_UPDATE_FAILED")

    /**
     * Constant: Credential creation failed.
     */
    #define AIMS_CREDENTIAL_CREATION_FAILED 		PRODUCT_AIMS _T("_CREDENTIAL_CREATION_FAILED")

    /**
     * Constant: Device creation failed.
     */
    #define AIMS_DEVICE_CREATION_FAILED 			PRODUCT_AIMS _T("_DEVICE_CREATION_FAILED")

    /**
     * Constant: Device operation failed.
     */
    #define AIMS_DEVICE_OPERATION_FAILED 			PRODUCT_AIMS _T("_DEVICE_OPERATION_FAILED")

    /**
     * Constant: Device ATR is unrecognized by the card content server.
     */
    #define AIMS_DEVICE_UNKNOWN_ATR 				PRODUCT_AIMS _T("_DEVICE_UNKNOWN_ATR")

    /**
     * Constant: Failure occurred while trying to synchronize device with the card content server.
     */
    #define AIMS_DEVICE_SYNC_FAILED 				PRODUCT_AIMS _T("_DEVICE_SYNC_FAILED")

    /**
     * Constant: Failure occurred while trying to retrieve devices list.
     */
    #define AIMS_FIND_DEVICES_FAILED 				PRODUCT_AIMS _T("_FIND_DEVICES_FAILED")

    /**
     * Constant: A failure occurred while trying to retrieve the device policies list.
     */
    #define AIMS_FIND_APPL_SETS_FAILED				PRODUCT_AIMS _T("_FIND_APPL_SET_FAILED")

    /**
     * Constant: A failure occurred while trying to retrieve privileges list.
     */
    #define AIMS_FIND_PRIVILEGES_FAILED 			PRODUCT_AIMS _T("_FIND_PRIVILEGES_FAILED")

    /**
     * Constant: A failure occurred while trying to retrieve credential profiles list.
     */
    #define AIMS_FIND_PROFILES_FAILED 				PRODUCT_AIMS _T("_FIND_PROFILES_FAILED")

    /**
     * Constant: Failure occurred while trying to retrieve actions list.
     */
    #define AIMS_FIND_ACTIONS_FAILED 				PRODUCT_AIMS _T("_FIND_ACTIONS_FAILED")

    /**
     * Constant: Failure occurred while trying to retrieve credentials list.
     */
    #define AIMS_FIND_CREDENTIALS_FAILED 			PRODUCT_AIMS _T("_FIND_CREDENTIALS_FAILED")

    /**
     * Constant: Failure occurred while trying to retrieve transaction(s).
     */
    #define AIMS_FIND_TRANSACTIONS_FAILED 			PRODUCT_AIMS _T("_FIND_TRANSACTIONS_FAILED")

    /**
     * Constant: Failure occurred while trying to retrieve the list of users.
     */
    #define AIMS_FIND_USERS_FAILED 					PRODUCT_AIMS _T("_FIND_USERS_FAILED")

    /**
     * Constant: Failure occurred while trying to retrieve the list of wallets.
     */
    #define AIMS_FIND_WALLETS_FAILED 				PRODUCT_AIMS _T("_FIND_WALLETS_FAILED")

    /**
     * Constant: The size limit (maxItems) has been exceeded.
     */
    #define AIMS_FIND_USERS_SIZE_LIMIT 				PRODUCT_AIMS _T("_FIND_USERS_SIZE_LIMIT")

    /**
     * Constant: Either enrollment data and/or parameters were not valid for
     * the committed action.
     */
    #define AIMS_INVALID_ACTION_PARAMS 				PRODUCT_AIMS _T("_INVALID_ACTION_PARAMS")

    /**
     * Constant: Data value / ref in enrollment data was invalid.
     */
    #define AIMS_INVALID_ENROLL_DATA 				PRODUCT_AIMS _T("_INVALID_ENROLL_DATA")

    /**
     * Constant: An invalid parameter was given.
     */
    #define AIMS_INVALID_PARAM 						PRODUCT_AIMS _T("_INVALID_PARAM")

    /**
     * Constant: An invalid state was attempted to be set for an action.
     */
    #define AIMS_INVALID_STATE 						PRODUCT_AIMS _T("_INVALID_STATE")

    /**
     * Constant: An invalid state was attempted to be set for an action.
     */
    #define AIMS_INVALID_SYNC_STATE                PRODUCT_AIMS _T("_INVALID_SYNC_STATE");
    
    /**
     * Constant: An invalid credential was given for an action or operation.
     */
    #define AIMS_INVALID_CREDENTIAL 				PRODUCT_AIMS _T("_INVALID_CREDENTIAL")

    /**
     * Constant: An operation that does not conform to application set definition was requested.
     */
    #define AIMS_INVALID_APPL_SET 					PRODUCT_AIMS _T("_INVALID_APPL_SET")

    /**
     * Constant: Specified action does not exist.
     */
    #define AIMS_NO_SUCH_ACTION 					PRODUCT_AIMS _T("_NO_SUCH_ACTION")

    /**
     * Constant: No binding exists for the supplied subject and the relevant object
     * type implied by the method's context.
     */
    #define AIMS_NO_SUCH_BINDING 					PRODUCT_AIMS _T("_NO_SUCH_BINDING")

    /**
     * Constant: The specified credential was not found.
     */
    #define AIMS_NO_SUCH_CREDENTIAL 				PRODUCT_AIMS _T("_NO_SUCH_CREDENTIAL")

    /**
     * Constant: The specified enrollment data does not exist.
     */
    #define AIMS_NO_SUCH_ENROLL_DATA 				PRODUCT_AIMS _T("_NO_SUCH_ENROLL_DATA")

    /**
     * Constant: The specified application set does not exist.
     */
    #define AIMS_NO_SUCH_APPL_SET 					PRODUCT_AIMS _T("_NO_SUCH_APPL_SET")

    /**
     * Constant: An invalid process was supplied.
     */
    #define AIMS_NO_SUCH_PROCESS 					PRODUCT_AIMS _T("_NO_SUCH_PROCESS")

    /**
     * Constant: Specified profile attribute does not exist.
     */
    #define AIMS_NO_SUCH_PROFILE_ATTRIB 			PRODUCT_AIMS _T("_NO_SUCH_PROFILE_ATTRIB")

    /**
     * Constant: The specified security module was not found.
     */
    #define AIMS_NO_SUCH_SM 						PRODUCT_AIMS _T("_NO_SUCH_SM")

    /**
     * Constant: Specified user does not exist.
     */
    #define AIMS_NO_SUCH_USER 						PRODUCT_AIMS _T("_NO_SUCH_USER")

    /**
     * Constant: Specified wallet was not found.
     */
    #define AIMS_NO_SUCH_WALLET 					PRODUCT_AIMS _T("_NO_SUCH_WALLET")

    /**
     * Constant: Specified attribute was not found.
     */
    #define AIMS_NO_SUCH_ATTRIB 					PRODUCT_AIMS _T("_NO_SUCH_ATTRIB")

    /**
     * Constant: Specified parameter is missing or was not found.
     */
    #define AIMS_NO_SUCH_PARAM 						PRODUCT_AIMS _T("_NO_SUCH_PARAM")

    /**
     * Constant: Specified transaction is missing or was not found.
     */
    #define AIMS_NO_SUCH_TX 						PRODUCT_AIMS _T("_NO_SUCH_TX")

    /**
     * Constant: Specified transaction is incomplete
     */
    #define AIMS_TX_INCOMPLETE 						PRODUCT_AIMS _T("_TX_INCOMPLETE")

    /**
     * Constant: Attempted task that requires the security module to be bound to a user
     * when it was not. e.g submitActions, unbindSecurityModule, getBoundUserBySM.
     */
    #define AIMS_NOT_BOUND_SM 						PRODUCT_AIMS _T("_NOT_BOUND_SM")

    /**
     * Constant: The operator is already logged in.
     */
    #define AIMS_OPERATOR_ALREADY_LOGGED_IN 		PRODUCT_AIMS _T("_OPERATOR_ALREADY_LOGGED_IN")

    /**
     * Constant: The operator is not active.
     */
    #define AIMS_OPERATOR_NOT_ACTIVE 				PRODUCT_AIMS _T("_OPERATOR_NOT_ACTIVE")

    /**
     * Constant: The operator is not authorized to perform this operation.
     */
    #define AIMS_OPERATOR_PERMISSION_DENIED 		PRODUCT_AIMS _T("_OPERATOR_PERMISSION_DENIED")

    /**
     * Constant: Action creation failed.
     */
    #define AIMS_ACTION_CREATION_FAILED 			PRODUCT_AIMS _T("_ACTION_CREATION_FAILED")

    /**
     * Constant: Action deletion failed.
     */
    #define AIMS_ACTION_DELETION_FAILED 			PRODUCT_AIMS _T("_ACTION_DELETION_FAILED")

    /**
     * Constant: Action update failed.
     */
    #define AIMS_ACTION_UPDATE_FAILED 				PRODUCT_AIMS _T("_ACTION_UPDATE_FAILED")

    /**
     * Constant: Action is still in REQUESTED state and not approved.
     */
	#define	AIMS_ACTION_NOT_APPROVED				PRODUCT_AIMS _T("_ACTION_NOT_APPROVED")

    /**
     * Constant: Action has been denied.
     */
	#define	AIMS_ACTION_DENIED						PRODUCT_AIMS _T("_ACTION_DENIED")

    /**
     * Constant: Action has failed.
     */
	#define	AIMS_ACTION_FAILED						PRODUCT_AIMS _T("_ACTION_FAILED")

    /**
     * Constant: There are other associated pending actions.
     */
    #define AIMS_ACTION_PENDING 					PRODUCT_AIMS _T("_ACTION_PENDING")

    /**
     * Constant: This API or feature is not implemented but reserved for future use.
     */
    #define AIMS_RFU_NOT_IMPLEMENTED 				PRODUCT_AIMS _T("_RFU_NOT_IMPLEMENTED")

    /**
     * Constant: This API or feature is deprecated or no longer supported.
     */
    #define AIMS_DEPRECATED 						PRODUCT_AIMS _T("_DEPRECATED")

    /**
     * Constant: The CMS session has not been opened.
     */
    #define AIMS_SESSION_NOT_OPENED 				PRODUCT_AIMS _T("_SESSION_NOT_OPENED")

    /**
     * Constant: Maximum number of available sessions reached.
     */
    #define AIMS_MAX_SESSION_REACHED                PRODUCT_AIMS _T("_MAX_SESSION_REACHED")    
    
    /**
     * Constant: Timeout occurred with connection. 
     */
    #define AIMS_CONNECTION_TIMEOUT                 PRODUCT_AIMS _T("_CONNECTION_TIMEOUT")
    
    /**
     * Constant: HTTP session error.
     */
	#define AIMS_HTTP_SESSION_ERROR					PRODUCT_AIMS _T("_HTTP_SESSION_ERROR")

	/**
     * Constant: SSL session error.
     */
	#define AIMS_SSL_SESSION_ERROR					PRODUCT_AIMS _T("_SSL_SESSION_ERROR")

	/**
     * Constant: Method not supported by server.
     */
    #define AIMS_UNSUPPORTED_METHOD					PRODUCT_AIMS _T("_UNSUPPORTED_METHOD")

    /**
     * Constant: Unexpected clients error occurred.
     */
    #define AIMS_UNEXPECTED_CLIENT_ERROR			PRODUCT_AIMS _T("_CLIENTOPERATION_ERROR")

    /**
     * Constant: Error occurred with client operation executor.
     */
    #define AIMS_CLIENTOPERATION_ERROR				PRODUCT_AIMS _T("_CLIENTOPERATION_ERROR")

    /**
     * Constant: User creation failed.
     */
    #define AIMS_USER_CREATION_FAILED 				PRODUCT_AIMS _T("_USER_CREATION_FAILED")

    /**
     * Constant: User deletion failed.
     */
    #define AIMS_USER_DELETION_FAILED 				PRODUCT_AIMS _T("_USER_DELETION_FAILED")

    /**
     * Constant: User already exists.
     */
    #define AIMS_USER_EXISTS 						PRODUCT_AIMS _T("_USER_EXISTS")

    /**
     * Constant: User update failed.
     */
    #define AIMS_USER_UPDATE_FAILED 				PRODUCT_AIMS _T("_USER_UPDATE_FAILED")

    /**
     * Constant: An attempt was made to page through a search using an invalid
     * search handle.
     */
    #define AIMS_NO_SUCH_SEARCH 					PRODUCT_AIMS _T("_NO_SUCH_SEARCH")

    /**
     * Constant: The limit of a search dataset was reached.
     */
    #define AIMS_SEARCH_END 						PRODUCT_AIMS _T("_SEARCH_END")

    /**
     * Constant: A configuration, profile, or policy was not found.
     */
    #define AIMS_NO_SUCH_CONFIG 					PRODUCT_AIMS _T("_NO_SUCH_CONFIG")

    /**
     * Constant: Thrown when invalid data is passed to a manager API call.
     */
    #define AIMS_MALFORMED_DATA 					PRODUCT_AIMS _T("_MALFORMED_DATA")

// --------------------------------- CCM API ERROR CODES ---------------------------------

	// Interfaces Error-Codes

	/**
     * Constant: Thrown when an unexpected internal error is thrown from a Provider.
     * This type of error should be accompanied by a vendor code / or mapped to a
     * more appropriate exception.
     */
	#define MGR_INTERFACE  							LAYER_MANAGER _T("_INTERFACE")

    /**
     * Constant: Thrown when there is an error in fetching a manager instance.
     */
	#define MGR_INTERFACE_FACTORY 					LAYER_MANAGER _T("_FACTORY")

    /**
     * Constant: Thrown when a specific implementation of the ManagerFactory does not
     * receive a specific implementation of the Manager interface.
     */
	#define MGR_INTERFACE_IMPLEMENTATION 			LAYER_MANAGER _T("_IMPLEMENTATION")

    /**
     * Constant: Thrown when there are errors during instantiation of the Class returned by the LookupService.
     */
	#define MGR_INTERFACE_INVALID_INSTANTIATION 	LAYER_MANAGER _T("_INVALID_INSTANTIATION")

    /**
     * Constant: Thrown when the class defining a ManagerInstance is not found.
     */
	#define MGR_INTERFACE_NO_SUCH_INSTANCE  		LAYER_MANAGER _T("_NO_SUCH_INSTANCE")

