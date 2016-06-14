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
 * Created on May 5, 2006
 */

/**
 * \file aiCCMConstants.h
 * \brief A set of constants used by the CCM API infrastructure.
 */

// --------------------------------- CCM API CONSTANTS ---------------------------------

    /**
     * Constant: The default manager factory provider.
     */
	#define	DEFAULT_MANAGER_FACTORY_PROVIDERNAME	_T("SOAP")
	
    /**
     * Constant: Configuration parameter -- SSL host port.
     */
    #define	CONFIG_HOST_PORT						_T("HOST_PORT")
    /**
     * Constant: Configuration parameter -- SSL host name.
     */
    #define	CONFIG_HOST_NAME						_T("HOST_NAME")
    /**
     * Constant: Configuration parameter -- SSL host service.
     */    
    #define	CONFIG_HOST_SERVICE						_T("provider.service")
    /**
     * Constant: Configuration parameter for the URL.
     */
    #define	CONFIG_URL								_T("URL")
    /**
     * Constant: Configuration parameter -- certificate issuer subject dn.
     */
    #define	CONFIG_CERTSERIAL 						_T("CertSerial")
    /**
     * Constant: Configuration parameter -- certificate serial number.
     */
    #define	CONFIG_CERTISSUER						_T("CertIssuer")
    /**
     * Constant: Configuration parameter -- cookie.
     */
	#define	CONFIG_COOKIE							_T("Cookie")
    /**
     * Constant: Configuration parameter -- connection timeout.
     */
	#define CONFIG_TIMEOUT						_T("CONNECT_TIMEOUT")
    /**
     * Constant: Configuration parameter -- connection retry.
     */
	#define CONFIG_RETRY						_T("CONNECT_RETRY")
    /**
     * Constant: Reader class -- smart cards.
     */
    #define READER_CLASS_SMARTCARD				_T("reader.smartcard")
    /**
     * Constant: Platform class -- smart cards.
     */
    #define	PLATFORM_CLASS_SMARTCARD				_T("smartcard")
    /**
     * Constant: Platform class -- smart card ATR.
     */
    #define	PLATFORM_ID_SMARTCARD_SMOTYPE			_T("securitymodule.smartcard.SMOTYPE")
    /**
     * Constant: Platform class -- smart card CUID.
     */
    #define	PLATFORM_ID_SMARTCARD_SMOID				_T("securitymodule.smartcard.SMOID")    
    /**
     * Constant: Platform class -- smart card LABEL.
     */
    #define	PLATFORM_ID_SMARTCARD_LABEL				_T("securitymodule.smartcard.LABEL")    
    
    /**
     * Constant: Credential identifier parameter --  PIN.
     */
    #define CRED_ID_TYPE_PIN						_T("PIN")
    /**
     * Constant: Credential identifier parameter --  security question/answer.
     */    
    #define CRED_ID_TYPE_SQ 						_T("QUESTIONANSWER")
    /**
     * Constant: Credential identifier parameter --  initial password
     */    
    #define CRED_ID_TYPE_PASSWORD					_T("PASSWORD")
    /**
     * Constant: Credential identifier parameter --  PKI
     */    
    #define CRED_ID_TYPE_PKI						_T("PKI")
	
    // Credential Status values

    /**
     * Constant: Credential status -- active.
     */
    #define CRED_STATUS_ACTIVE						_T("ACTIVE")
    /**
     * Constant: Credential status -- suspended.
     */
    #define CRED_STATUS_SUSPENDED					_T("HOLD")
    /**
     * Constant: Credential status -- revoked.
     */
    #define CRED_STATUS_REVOKED						_T("REVOKED")
    
    // Credential elements for base types

    /**
     * Constant: Credential element identifier -- PIN.
     */
    #define CRED_ELEMENT_ID_PIN          	   		_T("pin")
    /**
     * Constant: Credential element identifier -- password.
     */
    #define CRED_ELEMENT_ID_PASSWORD        	    _T("password")
    /**
     * Constant: Credential element identifier -- certificate.
     */
    #define CRED_ELEMENT_ID_CERTIFICATE     	    _T("com.activcard.provider.cred.pki.certificate")

    /**
     * Constant: Credential element type -- PIN.
     */
    #define CRED_ELEMENT_TYPE_PIN          	    	_T("pin")
    /**
     * Constant: Credential element type -- initial password.
     */
    #define CRED_ELEMENT_TYPE_PASSWORD        		_T("password")
    /**
     * Constant: Credential element type -- certificate.
     */
    #define CRED_ELEMENT_TYPE_CERTIFICATE     		_T("pki.x509")
    /**
     * Constant: Credential element type -- security question answer.
     */
    #define CRED_ELEMENT_TYPE_SQ		     		_T("questionanswer")
    /**
     * Constant: Credential element encoding -- none.
     */    
	#define CRED_ELEMENT_ENCODING_NONE				_T("NONE")
    /**
     * Constant: Credential element encoding -- certificate.
     */    
	#define CRED_ELEMENT_ENCODING_CERTIFICATE		_T("1.2.840.113549.1.9.22")
    /**
     * Constant: Credential element encoding -- sha1.
     */    
	#define CRED_ELEMENT_ENCODING_SHA1				_T("1.3.14.3.2.26")

    /**
     * Constant: Credential entry key -- minimum answers.
     */    
	#define CRED_ENTRY_KEY_SQ_MIN 					_T("minAnswers")
    /**
     * Constant: Credential entry key -- password.
     */    
	#define CRED_ENTRY_KEY_PASSWORD					_T("password")

    // Credential ExternalOperations

    /**
     * Constant: Credential ExternalOperation -- get challenge.
     */    
    #define CRED_EXOP_GETCHALLENGE              	_T("getChallenge")
    /**
     * Constant: Credential ExternalOperation -- unlock.
     */    
    #define CRED_EXOP_UNLOCK                     	_T("unlock")

    // ExternalOperation response parameters

    /**
     * Constant: Credential ExternalOperation -- get challenge response.
     */    
    #define CRED_EXOP_PARAM_GETCHALLENGE_RESPONSE	_T("challenge")

    /**
     * Constant: Credential ExternalOperation -- unlock code response.
     */    
    #define CRED_EXOP_PARAM_UNLOCKCODE_RESPONSE  	_T("response")
    
    // Session Info constants
      
    /**
     * Constant: Session principal type -- DN (if available - otherwise user identifier).
     */
    #define	SESSION_PRINCIPAL_TYPE_DN				_T("DN")
    /**
     * Constant: Session principal type -- the roles configured for the subject.
     */
    #define	SESSION_PRINCIPAL_TYPE_ROLE				_T("role")
    /**
     * Constant: Session principal type -- the subject's user identifier (if available - otherwise DN).
     */
    #define	SESSION_PRINCIPAL_TYPE_USERID			_T("userId")
    
    // access parameters
    
    /**
     * Constant: Access parameter smart card -- reader name.
     */
    #define	ACCESS_PARAM_SMARTCARD_READER			_T("readerName")
  	/**
  	 * Constant: Access parameter smart card -- PIN.
  	 */
    #define	ACCESS_PARAM_SMARTCARD_PIN				_T("PIN")
    
    // runtime parameters

	/**
	 * Constant: Action runtime parameter -- PIN.
	 */
    #define	ACTION_RUNTIME_PARAM_PIN				_T("PIN")
	/**
	 * Constant: Action runtime parameter -- replacement reason.
	 * <p>
	 * The reason parameter may take the following values:
	 *	<ul>
	 *		<li>ACTION_REASON_NONE</li>
	 *		<li>ACTION_REASON_FORGOTTEN</li>
	 *		<li>ACTION_REASON_LOST</li>
	 *		<li>ACTION_REASON_STOLEN</li>
	 *		<li>ACTION_REASON_DAMAGED</li>
	 *		<li>ACTION_REASON_EXPIRED</li>
	 *		<li>ACTION_REASON_RENEW</li>
	 *	</ul>
	 * </p>
	 */
    #define	ACTION_RUNTIME_PARAM_REASON				_T("ReplacementReason")
	/**
	 * Constant: Action runtime parameter -- GlobalPlatform card locking.
	 *<p>
	 *	The synchronize method accepts this optional runtime parameter, which can accepts the values
	 *	ACTION_RUNTIME_PARAM_VALUE_TRUE and ACTION_RUNTIME_PARAM_VALUE_FALSE. 
	 *	If this parameter is set to ACTION_RUNTIME_PARAM_VALUE_TRUE, GlobalPlatform locking will occur at the end of the synchronization. 
	 *</p>
	 *<p>
	 *		In the Wallet Manager, this parameter is used with the PRODUCE action type. 
	 *		The card PRODUCE action type can perform GlobalPlatform card locking at the end of a security module production. 
	 *		The parameter name value pair should be 
	 *		&quot;ACTION_RUNTIME_PARAM_LOCK = ACTION_RUNTIME_PARAM_VALUE_TRUE&quot; or &quot;ACTION_RUNTIME_PARAM_LOCK = ACTION_RUNTIME_PARAM_VALUE_FALSE&quot;. 
	 *		If no ACTION_RUNTIME_PARAM_LOCK parameter is supplied at action submission time, the default behavior is to not 
	 *		lock the security module.
	 *</p>
	 *<p>
	 *		In the Synchronization Manager, if the ACTION_RUNTIME_PARAM_LOCK parameter is set to ACTION_RUNTIME_PARAM_VALUE_TRUE, 
	 *		GlobalPlatform locking will occur at the end of the synchronization. If a synchronize is performed with no pending 
	 *		actions and &quot;ACTION_RUNTIME_PARAM_LOCK = ACTION_RUNTIME_PARAM_VALUE_TRUE&quot; is supplied to the synchronization, the security module is locked with no other effect 
	 *		on the card or its lifecycle state. If no actions are pending and &quot;ACTION_RUNTIME_PARAM_LOCK = ACTION_RUNTIME_PARAM_VALUE_FALSE&quot;, no security module content action occurs.
	 *</p>
	 */
	#define	ACTION_RUNTIME_PARAM_LOCK				_T("lock")
    /**	
     * Constant: Action runtime parameter -- SyncListener events subscription.
     * <p>
     * The SyncListener implementation is passed only events that you explicitly subscribe it to. 
     * The subscription is achieved by passing a comma-delimited list of event identifiers using the ACTION_RUNTIME_PARAM_EVENTS runtime parameter.
     * </p>
     */
	#define ACTION_RUNTIME_PARAM_EVENTS				_T("com.activcard.cms.service.tcd.subscribedevents")
    
    /**
     * Constant: Action runtime parameter value -- true.
     */
	#define	ACTION_RUNTIME_PARAM_VALUE_TRUE			_T("true")
  	/**
  	 * Constant: Action runtime parameter value -- false.
  	 */
    #define	ACTION_RUNTIME_PARAM_VALUE_FALSE		_T("false")
    /**
     * Constant: Action runtime parameter value -- event status.
     */
	#define	ACTION_RUNTIME_PARAM_VALUE_EVENT_STATUS	_T("90001")
    /**
     * Constant: Action runtime parameter value -- event application.
     */
	#define	ACTION_RUNTIME_PARAM_VALUE_EVENT_APPLICATION _T("90002")
	
    // Synchronization event parameter identifiers.

    /**
     * Constant: Sync event parameter -- message.
     */
    #define	SYNC_EVENT_PARAM_MESSAGE 				_T("message")
	/**
	 * Constant: Sync event parameter -- percent.
	 */
    #define	SYNC_EVENT_PARAM_PERCENT 				_T("percent")
    /**
     * Constant: Sync event parameter -- level.
     */
    #define	SYNC_EVENT_PARAM_LEVEL 					_T("level")
    /**
     * Constant: Sync event parameter -- app name.
     */
    #define	SYNC_EVENT_PARAM_APP_NAME 				_T("appName")
    /**
     * Constant: Sync event parameter -- app description.
     */
    #define	SYNC_EVENT_PARAM_APP_DESC 				_T("appDesc")
    /**
     * Constant: Sync event parameter -- credential type.
     */
    #define	SYNC_EVENT_PARAM_CRED_TYPE 				_T("credType")
    /**
     * Constant: Sync event parameter -- status.
     */
    #define	SYNC_EVENT_PARAM_STATUS 				_T("status")
    
    // Criteria comparison operators

    /**
     * Constant: Criteria comparison -- equal.
     */
    #define	CRITERIA_COMPARISON_EQUAL				_T("=")
	/**
	 * Constant: Criteria comparison -- not equal.
     */
    #define	CRITERIA_COMPARISON_NOT_EQUAL			_T("!=")
    /**
     * Constant: Criteria comparison -- greater than.
     */
    #define	CRITERIA_COMPARISON_GREATER_THAN		_T(">")
    /**
     * Constant: Criteria comparison -- less than.
     */
    #define	CRITERIA_COMPARISON_LESS_THAN			_T("<")
    /**
     * Constant: Criteria comparison -- greater than or equal.
     */
    #define	CRITERIA_COMPARISON_GREATER_THAN_EQUAL	_T(">=")
    /**
     * Constant: Criteria comparison -- less than or equal.
     */
    #define	CRITERIA_COMPARISON_LESS_THAN_EQUAL		_T("<=")
    
    // The criteria identifiers

    /**
     * Constant: Criteria id -- user identifier.
     */
    #define	CRITERIA_ID_USERID						_T("user.id")
    /**
     * Constant: Criteria id -- wallet identifier.
     */
    #define	CRITERIA_ID_WALLETID					_T("wallet.id")
    /**
     * Constant: Criteria id -- security module identifier.
     */
	#define CRITERIA_ID_SMID						_T("securitymodule.id")
    /**
     * Constant: Criteria id -- enrollment identifier data.
     */
    #define	CRITERIA_ID_ENROLLMENT_DATA				_T("user.enrollmentdata.")
    /**
     * Constant: Criteria id -- action application set.
     */
    #define CRITERIA_ID_ACTION_APPLICATION_SET		_T("user.wallet.action.applicationSet")
    /**
     * Constant: Criteria id -- action type.
     */
    #define CRITERIA_ID_ACTION_TYPE					_T("user.wallet.action.type")
    /**
     * Constant: Criteria id -- action status.
     */
    #define CRITERIA_ID_ACTION_STATUS				_T("user.wallet.action.status")
    /**
     * Constant: Criteria id -- security module count.
     */
    #define CRITERIA_ID_SECURITYMODULE_COUNT		_T("user.wallet.securitymodulecount")
    
    // Action criteria keys

    /**
     * Constant: Criteria action -- status.
     */
    #define	CRITERIA_ACTION_STATUS					_T("action.status")
    /**
     * Constant: Criteria action -- parent wallet.
     */
    #define	CRITERIA_ACTION_PARENT_WALLET			_T("action.parentWallet")
    /**
     * Constant: Criteria action -- type.
     */
    #define	CRITERIA_ACTION_TYPE					_T("action.type")
    /**
     * Constant: Criteria action -- reason.
     */
    #define	CRITERIA_ACTION_REASON					_T("action.reason")
    /**
     * Constant: Criteria action -- application set.
     */
    #define	CRITERIA_ACTION_APPLICATION_SET			_T("action.applicationSet")
    
    // Credential criteria keys

    /**
     * Constant: Credential criteria keys -- parent wallet.
     */
	#define	CRITERIA_CRED_PARENT_WALLET				_T("credential.parentWallet")
    /**
     * Constant: Credential criteria keys -- type.
     */
	#define	CRITERIA_CRED_TYPE						_T("credential.type")
    /**
     * Constant: Credential criteria keys -- application identifier.
     */
	#define	CRITERIA_CRED_APPLICATION_ID			_T("application.id")
    
    // The action types

    /**
     * Constant: Action type -- issuance.
     */
    #define	ACTION_TYPE_ISSUANCE					_T("ISSUANCE")
    /**
     * Constant: Action type -- produce.
     */
    #define	ACTION_TYPE_PRODUCE						_T("PRODUCE")
    /**
     * Constant: Action type -- post issuance.
     */
    #define	ACTION_TYPE_POSTISSUANCE				_T("POST-ISSUANCE")
    /**
     * Constant: Action type -- PIN unlock.
     */
    #define	ACTION_TYPE_PINUNLOCK					_T("PINUNLOCK")
    /**
     * Constant: Action type -- card lock.
     */
    #define	ACTION_TYPE_CARDLOCK					_T("CARDLOCK")		// =CARDLOCK w/ STATUS=deactivate
    /**
     * Constant: Action type -- card unlock.
     */
	#define	ACTION_TYPE_CARDUNLOCK					_T("CARDUNLOCK")	// =CARDLOCK w/ STATUS=activate
	/**
	 * Constant: Action type -- recycle.
	 */
    #define	ACTION_TYPE_RECYCLE						_T("RECYCLE")

    // The action processes

    /**
     * Constant: Action process -- cancel.
     */
    #define	ACTION_PROCESS_CANCEL					_T("action.cancel")
    /**
     * Constant: Action process -- approve.
     */
    #define	ACTION_PROCESS_APPROVE					_T("action.approve")
    /**
     * Constant: Action process -- deny.
     */
    #define	ACTION_PROCESS_DENY						_T("action.deny")
    /**
     * Constant: Action process -- reset.
     */
    #define	ACTION_PROCESS_RESET					_T("action.reset")

    // The action statuses
    /**
     * Constant: Action status -- requested.
     */
    #define	ACTION_STATUS_REQUESTED					_T("REQUESTED")
    /**
     * Constant: Action status -- pending.
     */
    #define	ACTION_STATUS_PENDING					_T("PENDING")
    /**
     * Constant: Action status -- denied.
     */
    #define	ACTION_STATUS_DENIED					_T("DENIED")
    /**
     * Constant: Action status -- failed.
     */
    #define	ACTION_STATUS_FAILED					_T("FAILED")
    /**
     * Constant: Action status -- forbidden.
     */
	#define	ACTION_STATUS_FORBIDDEN					_T("FORBIDDEN")
       
    // The action reasons

    /**
     * Constant: Action reason -- none.
     */
    #define ACTION_REASON_NONE      				_T("NONE")		// ISSUANCE
    /**
     * Constant: Action reason -- forgotten.
     */
	#define ACTION_REASON_FORGOTTEN 				_T("FORGOTTEN")	// TEMPORARY_REPLACEMENT
	/**
	 * Constant: Action reason -- lost.
	 */
	#define ACTION_REASON_LOST      				_T("LOST")		// PERMANENT_REPLACEMENT
	/**
	 * Constant: Action reason -- stolen.
	 */
	#define ACTION_REASON_STOLEN    				_T("STOLEN")	// PERMANENT_REPLACEMENT
	/**
	 * Constant: Action reason -- damaged.
	 */
	#define ACTION_REASON_DAMAGED	   				_T("DAMAGED")	// DAMAGE_REPLACEMENT
	/**
	 * Constant: Action reason -- expired.
	 */
	#define ACTION_REASON_EXPIRED	   				_T("EXPIRED")	// EXPIRED_REPLACEMENT
	/**
	 * Constant: Action reason -- renew.
	 */
	#define ACTION_REASON_RENEW     				_T("RENEW")		// ISSUANCE
    
    // SecurityModule statuses

	/**
	 * Constant: Security module status -- available.
	 */
	#define  SECURITY_MODULE_STATUS_AVAILABLE		_T("AVAILABLE")
	/**
	 * Constant: Security module status -- assigned.
	 */
	#define  SECURITY_MODULE_STATUS_ASSIGNED		_T("ASSIGNED")
	/**
	 * Constant: Security module status -- issued.
	 */
	#define  SECURITY_MODULE_STATUS_ISSUED			_T("ISSUED")
	/**
	 * Constant: Security module status -- in production.
	 */
	#define  SECURITY_MODULE_STATUS_INPRODUCTION	_T("INPRODUCTION")
	/**
	 * Constant: Security module status -- produced.
	 */
	#define  SECURITY_MODULE_STATUS_PRODUCED		_T("PRODUCED")
	/**
	 * Constant: Security module status -- invalid.
	 */
	#define  SECURITY_MODULE_STATUS_INVALID			_T("INVALID")
	/**
	 * Constant: Security module status -- terminated.
	 */
	#define  SECURITY_MODULE_STATUS_TERMINATED		_T("TERMINATED")

    // SecurityModule status reasons

	/**
	 * Constant: Security module status reason -- none.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_NONE			_T("NONE")
	/**
	 * Constant: Security module status reason -- assigned.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_ASSIGNED		_T("ASSIGNED")
	/**
	 * Constant: Security module status reason -- unassigned.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_UNASSIGNED	_T("UNASSIGNED")
	/**
	 * Constant: Security module status reason -- failed.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_FAILED		_T("FAILED")
	/**
	 * Constant: Security module status reason -- stolen.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_STOLEN		_T("STOLEN")
	/**
	 * Constant: Security module status reason -- terminate.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_TERMINATE	_T("TERMINATE")
	/**
	 * Constant: Security module status reason -- forgotten.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_FORGOTTEN	_T("FORGOTTEN")
	/**
	 * Constant: Security module status reason -- recycle.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_RECYCLE		_T("RECYCLE")
	/**
	 * Constant: Security module status reason -- lost.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_LOST			_T("LOST")
	/**
	 * Constant: Security module status reason -- end of life.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_END_OF_LIFE	_T("END_OF_LIFE")
	/**
	 * Constant: Security module status reason -- damaged.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_DAMAGED		_T("DAMAGED")
	/**
	 * Constant: Security module status reason -- expired.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_EXPIRED		_T("EXPIRED")
	/**
	 * Constant: Security module status reason -- on hold.
	 */
	#define  SECURITY_MODULE_STATUS_REASON_ONHOLD		_T("ON_HOLD")

	// SecurityModule Processes

	/**
	 * Constant: Security module process -- suspend.
	 */
    #define	PROCESS_ID_SM_SUSPEND						_T("securityModule.suspend")
    /**
     * Constant: Security module process -- resume.
     */
    #define	PROCESS_ID_SM_RESUME						_T("securityModule.resume")
    /**
     * Constant: Security module process -- revoke.
     */
    #define	PROCESS_ID_SM_REVOKE						_T("securityModule.revoke")
    /**
     * Constant: Security module process -- activate.
     */
    #define	PROCESS_ID_SM_ACTIVATE						_T("securityModule.activate")

    // Wallet criteria keys

    /**
     * Constant: Wallet criterion -- action status.
     */
    #define	CRITERIA_WALLET_ACTION_STATUS				_T("wallet.action.status")
    /**
     * Constant: Wallet criterion -- action type.
     */
    #define	CRITERIA_WALLET_ACTION_TYPE					_T("wallet.action.type")
    /**
     * Constant: Wallet criterion -- action application set.
     */
    #define	CRITERIA_WALLET_ACTION_APPLICATION_SET		_T("wallet.action.applicationSet")
    /**
     * Constant: Wallet criterion -- security module count.
     */
    #define	CRITERIA_WALLET_SECURITYMODULE_COUNT		_T("wallet.securityModuleCount")
    /**
     * Constant: Wallet criterion -- timestamp.
     */
    #define CRITERIA_WALLET_TIMESTAMP					_T("wallet.timestamp")
    
    // Credential Processes

    /**
     * Constant: Credential process -- suspend.
     */
    #define	PROCESS_ID_CRED_SUSPEND						_T("suspend")
    /**
     * Constant: Credential process -- resume.
     */
    #define	PROCESS_ID_CRED_RESUME						_T("resume")
    /**
     * Constant: Credential process -- revoke.
     */
    #define	PROCESS_ID_CRED_REVOKE						_T("revoke")
    
    // Credential Actions

    /**
     * Constant: Credential action -- replace.
     */
    #define	ACTION_ID_CRED_REPLACE						_T("replace")

    // Credential Operations

    /**
     * Constant: Credential operation -- unlock.
     */
    #define	OPER_ID_CRED_UNLOCK							_T("operation.unlock")
    
    // Enrollment Data identifiers
    
    /**
     * Constant: Enrollment data -- parent DN.
     */
    #define ENROLLMENT_DATA_ID_PARENT_DN				_T("parentDN") 	
	
    /*
     * Enrollment Data Types
     */
/**
	 Constant: Enrollment data type -- string.
*/
    #define	ENROLLMENT_DATA_TYPE_STRING 				_T("String")
/**
	 Constant: Enrollment data type -- date.
*/
    #define	ENROLLMENT_DATA_TYPE_DATE 					_T("Date")
/**
	 Constant: Enrollment data type -- int.
*/
    #define	ENROLLMENT_DATA_TYPE_INT 					_T("Integer")
/**
	 Constant: Enrollment data type -- float.
*/
    #define	ENROLLMENT_DATA_TYPE_FLOAT 					_T("Float")
/**
	 Constant: Enrollment data type -- long.
*/
    #define	ENROLLMENT_DATA_TYPE_LONG 					_T("Long")
/**
	 Constant: Enrollment data type -- double.
*/
    #define	ENROLLMENT_DATA_TYPE_DOUBLE 				_T("Double")
/**
	 Constant: Enrollment data type -- boolean.
*/
    #define	ENROLLMENT_DATA_TYPE_BOOLEAN 				_T("Boolean")
/**
	 Constant: Enrollment data type -- byte.
*/
    #define	ENROLLMENT_DATA_TYPE_BYTE 					_T("byte[]")
/**
	 Constant: Enrollment data encoding -- none.
*/
    #define	ENROLLMENT_DATA_ENCODING_NONE 				_T("None")
/**
	 Constant: Enrollment data encoding -- base64.
*/
    #define	ENROLLMENT_DATA_ENCODING_BASE64 			_T("Base64")    

    // --------------------------------- DEPRECATED ---------------------------------
	/*
	* @deprecated
	* The following constants have been marked as "@deprecated", to indicate 
	* discouragement of their usage.  They are only included in order to provide 
	* backward compatibility for existing legacy implementations.
	*/

    /**
     * <b>Deprecated</b> 
	 * @deprecated Use {@link ::PLATFORM_ID_SMARTCARD_SMOTYPE}
     */
    #define	PLATFORM_ID_SMARTCARD_ATR					_T("securitymodule.smartcard.ATR")
    /**
     * <b>Deprecated</b> 
	 * @deprecated Use {@link ::PLATFORM_ID_SMARTCARD_SMOID}
     */
    #define	PLATFORM_ID_SMARTCARD_CUID					_T("securitymodule.smartcard.gp20.CUID")    

    /**
     * <b>Deprecated</b>
	 * @deprecated Use {@link AI::Action::setApplicationSet(TString)}
     */
	#define	ACTION_RUNTIME_PARAM_POLICY					_T("Policy")
    /**
     * <b>Deprecated</b> 
	 * @deprecated Use {@link ::ACTION_TYPE_CARDUNLOCK} or {@link ::ACTION_TYPE_CARDLOCK}
     */
	#define	ACTION_RUNTIME_PARAM_STATUS					_T("status")
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ACTION_TYPE_CARDUNLOCK}
     */
	#define	ACTION_RUNTIME_PARAM_VALUE_ACTIVATE			_T("activate")
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ACTION_TYPE_CARDLOCK}
     */
	#define	ACTION_RUNTIME_PARAM_VALUE_DEACTIVATE 		_T("deactivate")
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ACTION_TYPE_PINUNLOCK}
     */
    #define	ACTION_TYPE_UNLOCK							_T("UNLOCK")		// =PINUNLOCK
    /**
     * <b>Deprecated</b> 
     * @deprecated
     */
	#define	CRITERIA_ID_PASSWORD						_T("unicodePwd")
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::PROCESS_ID_SM_ACTIVATE}
     */
    #define	SECURITYMODULE_PROCESS_ACTIVATE				ACTION_RUNTIME_PARAM_VALUE_ACTIVATE
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ACTION_TYPE_PRODUCE}
     */
    #define	ACTION_TYPE_PRODUCTION						ACTION_TYPE_PRODUCE
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ACTION_PROCESS_CANCEL}
     */
	#define ACTION_TYPE_ACTION_CANCEL					ACTION_PROCESS_CANCEL
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ENROLLMENT_DATA_ID_PARENT_DN}
     */
	#define	CRITERIA_ID_PARENT_DN						ENROLLMENT_DATA_ID_PARENT_DN
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::CRITERIA_ACTION_PARENT_WALLET}
     */
	#define CRITERIA_ID_ACTION_PARENTWALLET				CRITERIA_ACTION_PARENT_WALLET
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::CRITERIA_ACTION_PARENT_WALLET}
     */
    #define	CRITERIA_ACTION_PARENTWALLET				CRITERIA_ACTION_PARENT_WALLET
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ENROLLMENT_DATA_TYPE_STRING}
     */
	#define PARAM_TYPE_STRING							ENROLLMENT_DATA_TYPE_STRING
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ENROLLMENT_DATA_TYPE_STRING}
     */
    #define PARAM_TYPE_DEFAULT 							ENROLLMENT_DATA_TYPE_STRING
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ENROLLMENT_DATA_ENCODING_NONE}
     */
	#define PARAM_ENCODING_NONE							ENROLLMENT_DATA_ENCODING_NONE
    /**
     * <b>Deprecated</b> 
     * @deprecated Use {@link ::ENROLLMENT_DATA_ENCODING_NONE}
     */
    #define PARAM_ENCODING_DEFAULT 						ENROLLMENT_DATA_ENCODING_NONE

// --------------------------------- CCM API CONSTANTS ---------------------------------
