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
 * \file aiCOEScriptContentMgmt.h
 * \brief A set of constants used by the CCM API Client Operation Executor infrastructure.
 */

// --------------------------------- PKCS11 COE CONSTANTS ---------------------------------

	/**
	 * Constant: Synchronize Action Script Types (Script name).
	 */
	#define	ACTION_SCRIPT_GENERATE_KEYPAIR			_T("generateKeyPair")  
	#define	ACTION_SCRIPT_GET_CHALLENGE				_T("getChallenge")    
	#define	ACTION_SCRIPT_INIT_AUTH					_T("initAuthenticator")    
	#define	ACTION_SCRIPT_INIT_TOKEN				_T("init")    
	#define	ACTION_SCRIPT_READ_DATA					_T("readData")    
	#define	ACTION_SCRIPT_RECYCLE					_T("recycle")    
	#define	ACTION_SCRIPT_REMOVE_DATA				_T("removeData")    
	#define	ACTION_SCRIPT_UNLOCK_AUTH				_T("unlockAuthenticator")    
	#define	ACTION_SCRIPT_WRITE_DATA				_T("writeData")
	
	/**
	 * Constant: Runtime Parameters.
	 */
	#define ACTION_INPUT_PARAM_INIT_KEY				_T("INIT_KEY")
	#define ACTION_INPUT_PARAM_INIT_KEYID			_T("INIT_KEYID")
	#define ACTION_INPUT_PARAM_RECYCLE_KEY			_T("RECYCLE_KEY")
	#define ACTION_INPUT_PARAM_RECYCLE_KEYID		_T("RECYCLE_KEYID")
	#define ACTION_INPUT_PARAM_LABEL				_T("LABEL")
	#define ACTION_INPUT_PARAM_CHALLENGE_LEN		_T("CHALLENGE_LENGTH")
	#define ACTION_INPUT_PARAM_UNLOCK_TYPE			_T("UNLOCK_TYPE")
	#define ACTION_INPUT_PARAM_UNLOCK_KEY			_T("UNLOCK_KEY")
	#define ACTION_INPUT_PARAM_UNLOCK_KEYID			_T("UNLOCK_KEYID")
	#define ACTION_INPUT_PARAM_OBJ_TYPE				_T("OBJ_TYPE")
	#define ACTION_INPUT_PARAM_CERT_TYPE			_T("CERT_TYPE")
	#define ACTION_INPUT_PARAM_CERT_CATEGORY		_T("CERT_CATEGORY")
	#define ACTION_INPUT_PARAM_SUBJECT				_T("SUBJECT")
	#define ACTION_INPUT_PARAM_ID					_T("ID")
	#define ACTION_INPUT_PARAM_ISSUER				_T("ISSUER")
	#define ACTION_INPUT_PARAM_SN					_T("SN")
	#define ACTION_INPUT_PARAM_TOKEN				_T("TOKEN")
	#define ACTION_INPUT_PARAM_PRIVATE				_T("PRIVATE")
	#define ACTION_INPUT_PARAM_PRIVATE_PRK			_T("PRIVATE_PRK")
	#define ACTION_INPUT_PARAM_PRIVATE_PUK			_T("PRIVATE_PUK")
	#define ACTION_INPUT_PARAM_KEYENCODING			_T("KEYENCODING")
	#define ACTION_INPUT_PARAM_ENCRYPTION_ALGOID	_T("ENCRYPTION_ALGOID")
	#define ACTION_INPUT_PARAM_KEYWRAPPING_ID		_T("KEYWRAPPING_ID")
	#define ACTION_INPUT_PARAM_KEYVALUE				_T("KEYVALUE")
	#define ACTION_INPUT_PARAM_MODIFIABLE			_T("MODIFIABLE")
	#define ACTION_INPUT_PARAM_DERIVE				_T("DERIVE")
	#define ACTION_INPUT_PARAM_SENSITIVE			_T("SENSITIVE")
	#define ACTION_INPUT_PARAM_DECRYPT				_T("DECRYPT")
	#define ACTION_INPUT_PARAM_SIGN					_T("SIGN")
	#define ACTION_INPUT_PARAM_SIGN_RECOVER			_T("SIGN_RECOVER")
	#define ACTION_INPUT_PARAM_OVER					_T("OVER")
	#define ACTION_INPUT_PARAM_UNWRAP				_T("UNWRAP")
	#define ACTION_INPUT_PARAM_EXTRACTABLE			_T("EXTRACTABLE")
	#define ACTION_INPUT_PARAM_ENCRYPT				_T("ENCRYPT")
	#define ACTION_INPUT_PARAM_VERIFY				_T("VERIFY")
	#define ACTION_INPUT_PARAM_VERIFY_RECOVER		_T("VERIFY_RECOVER")
	#define ACTION_INPUT_PARAM_WRAP					_T("WRAP")
	#define ACTION_INPUT_PARAM_KEYSIZE				_T("KEYSIZE")
	#define ACTION_INPUT_PARAM_PUBLICEXPONENT		_T("PUBLICEXPONENT")
	#define ACTION_INPUT_PARAM_CONTAINER_NAME		_T("CONTAINER_NAME")
	#define ACTION_INPUT_PARAM_DEFAULT_CONTAINER	_T("DEFAULT_CONTAINER")

	/**
	 * Constant: Output Parameters.
	 */
	#define ACTION_OUTPUT_PARAM_LABEL				ACTION_INPUT_PARAM_LABEL
	#define ACTION_OUTPUT_PARAM_LEFT_UNLOCK_TRY		_T("LEFTUNLOCKTRY")
	#define ACTION_OUTPUT_PARAM_COUNT				_T("COUNT")
	#define ACTION_OUTPUT_PARAM_DATA				ACTION_INPUT_PARAM_DATA
	#define ACTION_OUTPUT_PARAM_MODULUS				_T("MODULUS")
	#define ACTION_OUTPUT_PARAM_PUBLICEXPONENT		_T("PUBLICEXPONENT")
	
	/**
	 * Constant: Pin ID
	 */
	#define ACTION_PINID_VALUE_USER_PIN				_T("USER")
	#define ACTION_PINID_VALUE_SO_PIN				_T("SO")
	/**
	 * Constant: Object Type
	 */
	#define ACTION_OBJ_TYPE_VALUE_ANY_OBJECT			_T("ANY")
	#define ACTION_OBJ_TYPE_VALUE_CERTIFICATE_OBJECT	_T("CERTIFICATE")
	#define ACTION_OBJ_TYPE_VALUE_PRIVATE_KEY_OBJECT	_T("PRIVATE_KEY")
	#define ACTION_OBJ_TYPE_VALUE_PUBLIC_KEY_OBJECT		_T("PUBLIC_KEY")
	#define ACTION_OBJ_TYPE_VALUE_KEY_PAIR_OBJECT		_T("KEY_PAIR")
	#define ACTION_OBJ_TYPE_VALUE_CONTAINER_NAME_OBJECT	_T("CONTAINER_NAME")
	
	/**
	 * Constant: Certificate Type
	 */
	#define ACTION_CERT_TYPE_VALUE_X_509			_T("X_509")
	
	/**
	 * Constant: Certificate Category
	 */
	#define ACTION_CERT_CAT_VALUE_USER_CERTIFICATE	_T("USER")
	#define ACTION_CERT_CAT_VALUE_CA_CERTIFICATE	_T("CA")
	
	
	/**
	 * Constant : Key Type
	 */
	#define ACTION_KEY_TYPE_VALUE_RSA_ALGO_ID		_T("1.2.840.113549.1.1.1")
	
	/**
	 * Constant : Key Size
	 */
	#define ACTION_KEY_SIZE_VALUE_RSA_1024			_T("1024")
	#define ACTION_KEY_SIZE_VALUE_RSA_2048			_T("2048")
	
	/**
	 * Constant: Key encoding
	 */
	#define ACTION_KEY_ENCODING_VALUE_CLEAR			_T("PKCS8.clear")
	#define ACTION_KEY_ENCODING_VALUE_ENCRYPTED		_T("PKCS8.encrypted")
	
	/**
	 *	Constant: Unlock Type
	 */
	#define ACTION_UNLOCK_TYPE_VALUE_STATIC			_T("STATIC")
	#define ACTION_UNLOCK_TYPE_VALUE_CR				_T("C/R")
	
	/**
	 * Constant: authenticate Type
	 */
	#define ACTION_AUTH_TYPE_VALUE_STATIC			_T("STATIC")

// --------------------------------- PKCS11 COE CONSTANTS ---------------------------------	