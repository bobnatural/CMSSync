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
#pragma warning(disable:4231)	// disable nonstandard extern warning
#include "aiCCMConstants.h"
#include "aiCCMErrorCodes.h"
#include "aiInterfaces.h"
#include "aiLocalization.h"


#ifdef AICCMAPI_EXPORTS
#define AICCMAPI_API __declspec(dllexport)
#define EXPUTIL_TEMPLATE
#else
#define AICCMAPI_API __declspec(dllimport)
#define EXPUTIL_TEMPLATE extern
#endif // AICCMAPI_EXPORTS

namespace AI
{

/**
 * @internal
 * Retrieves localized message for given errorCode
 */
AICCMAPI_API TString localize(UINT errorCode);

//====================================================================================================
// IDENTIFIER CLASS DEFINITIONS
//====================================================================================================

#define USER_TYPE							_T("USER")
#define WALLET_TYPE							_T("WALLET")
#define APP_TYPE							_T("APPLICATION")

/**
 * Represents a unique security module identifier. It consists of:
 * <ul>
 * 	<li>A type.</li>
 * 	<li>An identifier.</li>
 * </ul>
 * The combination of a type and an identifier must represent a unique
 * security module identifier.
 */
class AICCMAPI_API SecurityModuleId: public Identifier
{
public:
    /**
     * Creates an empty security module identifier.
     */
	SecurityModuleId(void);

    /**
     * Creates a security module identifier.
     * @param type The security module type.
     * @param id The security module identifier.
     */
	SecurityModuleId(TString type, TString id);

	/**
	 * Copy Constructor.
 	 * @param src Source SecurityModuleId object.  A copy of the content of this object is used for new content.
	 */
	SecurityModuleId(const SecurityModuleId &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source SecurityModuleId object.  A copy of the content of this object is used for new content.
	 */
	virtual SecurityModuleId& operator=(const SecurityModuleId& src);

    /**
     * Virtual Copy Constructor.
     */
	virtual SecurityModuleId* clone() const	{ return new SecurityModuleId(*this); }

};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<SecurityModuleId>;
typedef Vector<SecurityModuleId>		SecurityModuleIdVector;

/**
 * Identifier of a user. 
 */
class AICCMAPI_API UserId: public Identifier 
{
public:
    /**
     * Constructor.
     */
	UserId(void);

	/**
     * Constructor.
     * @param id The unique identifier of the user.
     */
	UserId(TString id);

	/**
	 * Copy Constructor.
 	 * @param src Source UserId object.  A copy of the content of this object is used for new content.
	 */
	UserId(const UserId &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source UserId object.  A copy of the content of this object is used for new content.
	 */
	virtual UserId& operator=(const UserId& src);

    /**
     * Virtual Copy Constructor.
     */
	virtual UserId* clone() const	{ return new UserId(*this); }
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<UserId>;
typedef Vector<UserId>				UserIdVector;

/**
 * Identifier of a wallet.
 * <p>A wallet is a set of security modules and credentials assigned to a card holder (User). 
 * Usually, there is only one SecurityModule per User. The only case in which more than one bound card can be 
 * issued to a User is when the cardholder has been issued a temporary replacement card.</p>
 * <p>A user may have one and only one wallet and the wallet is optionally bound
 * to a user.</p>
 * <p>A wallet contains zero or more applications and zero or more security
 * modules where each application is contained within only one wallet.</p>
 * <p>An application owns zero or more credentials and exists within one or more
 * security modules where each credential belongs to only one application.</p>
 * <p>A credential contains zero or more credential elements where each
 * credential element exists within only one security module (either physical or
 * virtual/soft).</p>
 */
class AICCMAPI_API WalletId: public Identifier 
{
public:
    /**
	 * Constructor.
	 */
	WalletId(void);

    /**
	 * Constructor.
     * @param id The identifier of the wallet.
	 */
	WalletId(TString id);

	/**
	 * Copy Constructor.
 	 * @param src Source WalletId object.  A copy of the content of this object is used for new content.
	 */
	WalletId(const WalletId &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source WalletId object.  A copy of the content of this object is used for new content.
	 */
	virtual WalletId& operator=(const WalletId& src);

    /**
     * Virtual Copy Constructor.
     */
	virtual WalletId* clone() const	{ return new WalletId(*this); }
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<WalletId>;
typedef Vector<WalletId>				WalletIdVector;

/**
 * Application identifier.
 */
class AICCMAPI_API ApplicationId: public Identifier 
{
public:
    /**
     * Constructor.
     */
	ApplicationId(void);

	/**
     * Constructor.
     * @param id The unique identifier of the application.
     */
	ApplicationId(TString id);

	/**
	 * Copy Constructor.
 	 * @param src Source ApplicationId object.  A copy of the content of this object is used for new content.
	 */
	ApplicationId(const ApplicationId &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source ApplicationId object.  A copy of the content of this object is used for new content.
	 */
	virtual ApplicationId& operator=(const ApplicationId& src);

    /**
     * Virtual Copy Constructor.
     */
	virtual ApplicationId* clone() const	{ return new ApplicationId(*this); }
};

/**
 * Identifier of a search session handle. 
 */
class AICCMAPI_API SearchSessionId: public Identifier 
{
public:
    /**
     * Constructor.
     */
	SearchSessionId(void);

	/**
     * Constructor.
     * @param id The unique identifier of the user. <span class="qqq">qqq--user whose session this is?</span>
     */
	SearchSessionId(TString id);

	/**
	 * Copy Constructor.
 	 * @param src Source SearchSessionId object.  A copy of the content of this object is used for new content.
	 */
	SearchSessionId(const SearchSessionId &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source SearchSessionId object.  A copy of the content of this object is used for new content.
	 */
	virtual SearchSessionId& operator=(const SearchSessionId& src);

    /**
     * Virtual Copy Constructor.
     */
	virtual SearchSessionId* clone() const	{ return new SearchSessionId(*this); }
};

//====================================================================================================
// COMMON CLASS DEFINITIONS
//====================================================================================================

// * @author John Babbidge
/**
 * Simplified permission object for client side use.
 */
class AICCMAPI_API Permission : public DLL_OBJECT_BASE
{
	TString resource;
	TString type;
    TStringVector *actions;

public:
	/**
     * Constructor.
     */
	Permission(void);

    /**
     * Copy Constructor.
 	 * @param src Source Permission object.  A copy of the content of this object is used for new content.
     */
	Permission(const Permission &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source Permission object.  A copy of the content of this object is used for new content.
	 */
	virtual Permission& operator=(const Permission& src);

    /**
     * Destructor.
     */
	virtual ~Permission(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual Permission* clone() const	{ return new Permission(*this); }

    /**
     * Returns the resource of the permission.
     * @return The resource of the permission.
     */
	virtual TString getResource();

    /**
     * Returns the type of the permission.
     * @return The type of the permission.
     */
    virtual TString getType();

    /**
     * Sets the resource of the permission.
     * @param resource The resource of the permission.
     */
    virtual void setResource(TString resource);

    /**
     * Sets the type of the permission.
     * @param type The type of the permission.
     */
    virtual void setType(TString type);

    /**
     * Returns the actions allowed on the resource.
     * @return The actions allowed on the resource.
     */
    virtual TStringVector *getActions();

    /**
     * Sets the actions allowed on the resource.
     * @param actions The actions allowed on the resource.
     */
    virtual void setActions(TStringVector *actions);
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<Permission>;
typedef Vector<Permission>	PermissionVector;

// * @author John Babbidge
/**
 * Represents a login event for the operator of the current session.
 */
class AICCMAPI_API LoginInformation : public DLL_OBJECT_BASE
{
	bool		successfulFlag;
	__time64_t  loginTimestamp;
	
public:
	/**
     * Constructor.
     */
	LoginInformation(void);

    /**
     * Copy Constructor.
 	 * @param src Source LoginInformation object.  A copy of the content of this object is used for new content.
     */
	LoginInformation(const LoginInformation &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source LoginInformation object.  A copy of the content of this object is used for new content.
	 */
	virtual LoginInformation& operator=(const LoginInformation& src);

    /**
     * Destructor.
     */
	virtual ~LoginInformation(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual LoginInformation* clone() const	{ return new LoginInformation(*this); }

    /**
     * Returns whether the login was successful or not.
     * @return Whether the login was successful or not.
     */
    virtual bool isSuccessful();
    
    /**
     * Sets whether the login was successful or not.
     * @param isSuccessful Whether the login was successful or not.
     */
    virtual void setSuccessful(bool isSuccessful);
    
    /**
     * Returns the timestamp of this login entry.
     * @return The timestamp of this login entry.
     */
    virtual __time64_t *getLoginTimestamp();
    
    /**
     * Sets the timestamp of this login entry.
     * @param loginTimestamp The timestamp of this login entry.
     */
    virtual void setLoginTimestamp(__time64_t *loginTimestamp);
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<LoginInformation>;
typedef Vector<LoginInformation>		LoginInformationVector;

// * @author John Babbidge
// * @see java.security.Principal
/**
 * A Principal that provides semantic typing.
 */
class AICCMAPI_API Principal : public DLL_OBJECT_BASE
{
	TString name;
	TString type;

public:
	/**
     * Constructor.
     */
	Principal(void);

    /**
     * Copy Constructor.
 	 * @param src Source Principal object.  A copy of the content of this object is used for new content.
     */
	Principal(const Principal &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source Principal object.  A copy of the content of this object is used for new content.
	 */
	virtual Principal& operator=(const Principal& src);

    /**
     * Destructor.
     */
	virtual ~Principal(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual Principal* clone() const	{ return new Principal(*this); }

    /**
     * Returns the principal type.
     * @return The principal type.
     */
	virtual TString getType();

    /**
     * Sets the principal type.
     * @param type The principal type.
     */
	virtual void setType(TString type);

    /**
     * Returns the name of the principal.
     * @return The name of the principal.
     */
    // @see java.security.Principal#getName()
	virtual TString getName();

    /**
     * Sets the name of the principal.
     * @param name The name of the principal.
     */
	virtual void setName(TString name);
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<Principal>;
typedef Vector<Principal>		PrincipalVector;

// * @author John Babbidge
/**
 * An authenticated identity.
 */
class AICCMAPI_API Subject : public DLL_OBJECT_BASE
{
	PrincipalVector *principals;

public:
    /**
     * Default Constructor.
     */
    Subject();

    /**
     * Copy Constructor.
 	 * @param src Source Subject object.  A copy of the content of this object is used for new content.
     */
	Subject(const Subject &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source Subject object.  A copy of the content of this object is used for new content.
	 */
	virtual Subject& operator=(const Subject& src);

    /**
     * Destructor.
     */
	virtual ~Subject(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual Subject* clone() const	{ return new Subject(*this); }    
    /**
     * Returns the principals.
     * @return The principals.
     */
    virtual PrincipalVector* getPrincipals();
    
    /**
     * Sets the principals.
     * @param principals The principals.
     */
    virtual void setPrincipals(PrincipalVector *principals);
};

// * @author John Babbidge
/**
 * Contains information about and related to the current session.
 */
class AICCMAPI_API SessionInfo : public DLL_OBJECT_BASE
{
	LoginInformationVector*	loginInformation;
	Subject* subject;

public:
	/**
     * Constructor.
     */
	SessionInfo(void);

    /**
     * Copy Constructor.
 	 * @param src Source SessionInfo object.  A copy of the content of this object is used for new content.
     */
	SessionInfo(const SessionInfo &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source SessionInfo object.  A copy of the content of this object is used for new content.
	 */
	virtual SessionInfo& operator=(const SessionInfo& src);

    /**
     * Destructor.
     */
	virtual ~SessionInfo(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual SessionInfo* clone() const	{ return new SessionInfo(*this); }

    /**
     * Returns the authenticated identity and all associated Principals.
     * @return The authenticated identity and all associated Principals.
     */
    virtual Subject *getSubject();
    
    /**
     * Sets the authenticated identity and all associated Principals.
     * @param subject The authenticated identity and all associated Principals.
     */
    virtual void setSubject(Subject *subject);

    /**
     * Returns the available login records of the operator who established the
     * current session.
     * @return The available login records of the operator.
     */
    virtual LoginInformationVector *getLoginInformation();

    /**
     * Sets the available login records of the operator who established the
     * current session.
     * @param loginInformation The available login records of the operator who established the
     * current session.
     */
    virtual void setLoginInformation(LoginInformationVector *loginInformation);
};
// * @author John Babbidge
/**
 * An abstract application.
 */
class AICCMAPI_API Application: public Managed
{
	ApplicationId* id;
    ConfigurationId* profileId;
	TString description;

public:
	/**
     * Constructor.
     */
	Application(void);

    /**
     * Copy Constructor.
 	 * @param src Source Application object.  A copy of the content of this object is used for new content.
     */
	Application(const Application &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source Application object.  A copy of the content of this object is used for new content.
	 */
	virtual Application& operator=(const Application& src);

    /**
     * Destructor.
     */
	virtual ~Application(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual Application* clone() const	{ return new Application(*this); }


    /**
     * Returns the identifier of the abstract application.
     * @return The identifier of the abstract application.
     */
    virtual ApplicationId* getId();

    /**
     * Returns the description of the abstract application.
     * @return The description of the abstract application.
     */
	virtual TString getDescription();

//    /*
//     * Returns the version of the abstract application.
//     * @return The version of the abstract application.
//     */
//    virtual TString getVersion();

    /**
     * Sets the description of the abstract application.
     * @param description Description of the abstract application.
     */
	virtual void setDescription(TString description);

    /**
     * Sets the identifier of the abstract application.
     * @param id The identifier of the abstract application.
     */
    virtual void setId(ApplicationId* id);
    
//    /*
//     * Sets the version of the abstract application.
//     * @param version The version of the abstract application.
//     */
//    virtual void setVersion(TString version);

    /**
     * Returns the unique identifier of the application profile used to manage
     * this application.
     * @return The unique identifier of the application profile.
     */
    virtual ConfigurationId* getProfileId();

    /**
     * Sets the unique identifier of the application profile used to manage
     * this application.
     * @param profileId The unique identifier of the application profile.
     */
    virtual void setProfileId(ConfigurationId* profileId);

    //
    //  TODO: for potential later use
    //    
    //    /*
    //     * Returns the array of credentials associated with the abstract application.
    //     * @return Array of credentials associated with the application.
    //     */
    //    virtual CredentialVector getCredentials();
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<Application>;
typedef Vector<Application>				ApplicationVector;

//====================================================================================================
// EXCEPTION CLASS DEFINITIONS
//====================================================================================================

// * @author John Babbidge
/**
 * A tuple of an entity identifier and the error reason code.
 */
class AICCMAPI_API Error : public DLL_OBJECT_BASE
{
	TString	id;
	TString	reasonCode;

public:
    /**
	 * Constructor.
     * @param id The entity identifier associated with the error.
     * @param reasonCode The reason code associated with the error.
     */
	Error(TString id , TString reasonCode);

	/**
	 * Constructor.
	 */
	Error(void);

    /**
     * Copy Constructor.
 	 * @param src Source Error object.  A copy of the content of this object is used for new content.
     */
	Error(const Error &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source Error object.  A copy of the content of this object is used for new content.
	 */
	virtual Error& operator=(const Error& src);

	/**
	 * Destructor.
	 */
	virtual ~Error(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual Error* clone() const	{ return new Error(*this); }
    
    /**
     * Returns the entity identifier associated with the error.
     * @return The entity identifier associated with the error.
     */
	virtual TString getId();

    /**
     * Returns the reason code associated with the error.
     * @return The reason code associated with the error.
     */
    virtual TString getReasonCode();

    /**
     * Sets the entity identifier associated with the error.
     * @param id The entity identifier associated with the error.
     */
    virtual void setId(TString id);

    /**
     * Sets the reason code associated with the error.
     * @param reasonCode The reason code associated with the error.
     */
    virtual void setReasonCode(TString reasonCode);
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<Error>;
typedef Vector<Error>					ErrorVector;

/**
 * This exception is thrown when an error occurs in the course of a session.
 */
class AICCMAPI_API SessionException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	SessionException() {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	SessionException(const LocalizedException &ex):LocalizedException(ex){};
};

/**
 * This exception is thrown upon remote connectivity failure.
 */
class AICCMAPI_API LocalizedRemoteException: public SessionException
{
public:
	/**
     * Default Constructor.
     */
	LocalizedRemoteException() {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	LocalizedRemoteException(const LocalizedException &ex):SessionException(ex){};
};

/**
 * This exception is thrown when an unexpected failure has occurred in the
 * implementation layer. These failures are not normal business level failures.
 * For example, connectivity interruption, database failure, etc. 
 */
class AICCMAPI_API ManagementException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	ManagementException() {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	ManagementException(const LocalizedException &ex):LocalizedException(ex){};
};

// * @author John Babbidge
/**
 * A base exception that contains multiple failures and the associated id of
 * the entity causing the error.
 */
class AICCMAPI_API MultiErrorException: public LocalizedException
{
	ErrorVector* errors;

public:
	/**
     * Default Constructor.
     */
	MultiErrorException(){ errors=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	MultiErrorException(const LocalizedException &ex):LocalizedException(ex){ errors=NULL; };

    /**
     * Returns the list of errors associated with the exception.
     * @return The list of errors associated with the exception.
     */
    virtual ErrorVector* getErrors();

    /**
     * Sets the list of errors associated with the exception.
     * @param errors The list of errors associated with the exception.
     */
    virtual void setErrors(ErrorVector* errors);
};

/**
 * Thrown when invalid data is passed to a manager API call.
 */
class AICCMAPI_API MalformedDataException: public MultiErrorException
{
public:
	/**
     * Default Constructor.
     */
	MalformedDataException() {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	MalformedDataException(const LocalizedException &ex):MultiErrorException(ex){};
};

/**
 * This exception is thrown when an attempt is made to retrieve an attribute on
 * a profile that does not exist.
 */
class AICCMAPI_API NoSuchProfileAttributeException: public LocalizedException
{
	TString attribute;

public:

	/**
     * Default Constructor.
     */
	NoSuchProfileAttributeException() {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchProfileAttributeException(const LocalizedException &ex): LocalizedException(ex){};

    /**
     * Returns the invalid attribute.
     * @return The invalid attribute.
     */
	virtual	TString	getAttribute(void);

	/**
     * Sets the invalid attribute.
	 * @param attribute The invalid attribute.
	 */
	virtual	void			setAttribute(TString attribute);
};

/**
 * This exception is thrown when an attempt is made to set a life cycle
 * state that is not a valid state for the referenced object(s).
 */
class AICCMAPI_API InvalidStateException: public LocalizedException
{
	TString status;

public:

	/**
     * Default Constructor.
     */
	InvalidStateException(){};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	InvalidStateException(const LocalizedException &ex): LocalizedException(ex){};

    /**
     * Returns the invalid state.
     * @return The invalid state.
     */
	virtual	TString	getStatus(void);

	/**
     * Sets the invalid state.
	 * @param status The invalid state.
	 */
	virtual	void			setStatus(TString status);
};

/**
 * Thrown when a security module identifier does not correspond to a valid
 * security module.
 */
class AICCMAPI_API NoSuchSecurityModuleException: public LocalizedException
{
	SecurityModuleId	*smId;

public:

	/**
     * Default Constructor.
     */
	NoSuchSecurityModuleException(void){ smId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchSecurityModuleException(const LocalizedException &ex): LocalizedException(ex){ smId=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchSecurityModuleException(void);

	/**
     * Returns the security module identifier.
     * @return The security module identifier.
     */
	virtual	SecurityModuleId	*getSecurityModuleId(void);

	/**
     * Sets the security module identifier.
     * @param smId The security module identifier.
     */
	virtual	void				setSecurityModuleId(SecurityModuleId *smId);
};

/**
 * Thrown when a specified user could not be found.
 */
class AICCMAPI_API NoSuchUserException: public LocalizedException
{
	UserId	*userId;

public:
	/**
     * Default Constructor.
     */
	NoSuchUserException(void){ userId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchUserException(const LocalizedException &ex): LocalizedException(ex){ userId=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchUserException(void);

	/**
     * Returns the user identifier.
     * @return The user identifier.
     */
	virtual	UserId	*getUserId(void);

    /**
     * Sets the user identifier.
     * @param userId The user identifier.
     */
	virtual	void	setUserId(UserId *userId);
};

/**
 * This exception is thrown when an attempt is made to retrieve or operate on
 * a wallet that does not exist.
 */
class AICCMAPI_API NoSuchWalletException: public LocalizedException
{
	WalletId	*walletId;

public:
	/**
     * Default Constructor.
     */
	NoSuchWalletException(void){ walletId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchWalletException(const LocalizedException &ex): LocalizedException(ex){ walletId=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchWalletException(void);

	/**
     * Returns the identifier of the wallet associated with this exception.
     * @return The identifier of the wallet associated with this exception.
     */
	virtual	WalletId	*getWalletId(void);

	/**
     * Sets the identifier of the wallet associated with this exception.
	 * @param walletId The identifier of the wallet associated with this exception.
	 */
	virtual	void		setWalletId(WalletId *walletId);
};

/**
 * This exception is thrown when the specified transaction is not found.
 */
class AICCMAPI_API NoSuchTransactionException: public LocalizedException
{
	TransactionId	*txId;

public:
	/**
     * Default Constructor.
     */
	NoSuchTransactionException(void){ txId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchTransactionException(const LocalizedException &ex): LocalizedException(ex){ txId=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchTransactionException(void);

	/**
     * Returns the transaction identifier.
     * @return The transaction identifier.
     */
	virtual	TransactionId *getTransactionId(void);

	/**
     * Sets the transaction identifier.
     * @param txId The transaction identifier.
	 */
	virtual	void setTransactionId(TransactionId *txId);
};

/**
 * This exception is thrown when the specified configuration is not found.
 */
class AICCMAPI_API NoSuchConfigException: public LocalizedException
{
	ConfigurationId	*configId;

public:
	/**
     * Default Constructor.
     */
	NoSuchConfigException(void){ configId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchConfigException(const LocalizedException &ex): LocalizedException(ex){ configId=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchConfigException(void);

	/**
     * Returns the configuration identifier that was not found.
     * @return The configuration identifier that was not found.
     */
	virtual	ConfigurationId *getConfigurationId(void);

	/**
     * Sets the configuration identifier that was not found.
     * @param configId The configuration identifier that was not found.
	 */
	virtual	void setConfigurationId(ConfigurationId* configId);
};

/**
 * This exception is thrown when an attempt to retrieve a binding was made but no
 * binding was found.
 */
class AICCMAPI_API NoSuchBindingException: public LocalizedException
{
	Identifier	*id;

public:
	/**
     * Default Constructor.
     */
	NoSuchBindingException(void){ id=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchBindingException(const LocalizedException &ex): LocalizedException(ex){ id=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchBindingException(void);

	/**
     * Returns the identifier associated with this exception.
     * @return The identifier associated with this exception.
     */
	virtual	Identifier	*getId(void);

	/**
     * Sets the identifier associated with this exception.
	 * @param id The identifier associated with this exception.
	 */
	virtual	void		setId(Identifier *id);
};

/**
 * Thrown when premature retrieval is attempted for an object based on a
 * transaction. This usually occurs when the referenced transaction 
 * is in an intermediate state and not able to provide some end result data. 
 */
class AICCMAPI_API TransactionIncompleteException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	TransactionIncompleteException(void) {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	TransactionIncompleteException(const LocalizedException &ex): LocalizedException(ex){};
};

/**
 * If the discovered security module is not in valid state for synchronization.
 */
class AICCMAPI_API NotSyncableStateException: public LocalizedException
{
	TString value;
	TString reason;

public:

	static const TString REASON_SECURITYMODULE_STATUS;
	static const TString REASON_ACTION_STATUS;

	/**
     * Default Constructor.
     */
	NotSyncableStateException(){};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NotSyncableStateException(const LocalizedException &ex): LocalizedException(ex){};

    /**
     * Returns the invalid state value.
     * @return The invalid state value.
     */
	virtual	TString	getValue(void);

	/**
     * Sets the invalid state.
	 * @param value The invalid state value.
	 */
	virtual	void setValue(TString value);

    /**
     * Returns the invalid state reason.
     * @return The invalid state reason.
     */
	virtual	TString	getReason(void);

	/**
     * Sets the invalid state reason.
	 * @param status The invalid state reason.
	 */
	virtual	void setReason(TString reason);
};

/**
 * This exception is thrown when an unexpected failure has occurred in the
 * implementation layer. These failures are not normal business level failures.
 * For example, connectivity interruption, database failure, etc. 
 */
class AICCMAPI_API SyncException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	SyncException(void) {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	SyncException(const LocalizedException &ex): LocalizedException(ex){};
};

/**
 * This exception is thrown if an attempt to cancel a synchronization is rejected.
 */
class AICCMAPI_API SyncCancelledException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	SyncCancelledException(void) {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	SyncCancelledException(const LocalizedException &ex): LocalizedException(ex){};
};

/**
 * This exception is thrown if an attempt to cancel a synchronization is rejected.
 */
class AICCMAPI_API SyncNotCancelledException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	SyncNotCancelledException(void) {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	SyncNotCancelledException(const LocalizedException &ex): LocalizedException(ex){};

};

/**
 * Thrown when a limit of a search dataset is reached by either paging forward to
 * the end or paging back to the beginning of the dataset.
 */
class AICCMAPI_API SearchEndException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	SearchEndException(void) {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	SearchEndException(const LocalizedException &ex): LocalizedException(ex){};

};

/**
 * Thrown when an attempt is made to page through a search using a
 * search handle that is not valid for the current session.
 */
class AICCMAPI_API NoSuchSearchException: public LocalizedException
{
	SearchSessionId	*handle;

public:
	/**
     * Default Constructor.
     */
	NoSuchSearchException(void) { handle=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchSearchException(const LocalizedException &ex): LocalizedException(ex) { handle=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchSearchException(void);

	/**
     * Returns the invalid handle identifier.
     * @return The invalid handle identifier.
     */
	virtual	SearchSessionId	*getHandle(void);

	/**
     * Sets the invalid handle identifier.
     * @param searchId The invalid handle identifier.
     */
	virtual	void setHandle(SearchSessionId *searchId);
};


//====================================================================================================
// SESSION MANAGER DEFINITION
//====================================================================================================

	/**
	 * Common session management functionality base.
	 * Abstract Class, Pure-Virtual Interface.
	 */
	class AICCMAPI_API SessionManager : public Manager, public ManagerInstance
	{
	public:

		/**
		 * Opens a new session for this API. Must be the first call made.  All the subsequent calls must occur in
		 * the scope of a valid session.
		 * @param parameters Reserved for future use. 
		 * @return The identifier of the session opened by the server.
		 * @throws SessionException If there is no valid session (session not opened or timed out).
		 * @throws LocalizedRemoteException If there is a communication error.
		 */
		virtual TString openSession(ParameterVector* parameters) = 0;

		/**
		 * Closes the current session. 
		 * @throws SessionException If there is no valid session (session not opened or timed out).
		 * @throws LocalizedRemoteException If there is a communication error.
		 */
		virtual void closeSession() = 0;
    
		/**
		 * Returns status information about the current session and the current
		 * authenticate operator. 
		 * <p>This method exposes the following information via nested objects:
		 * <ul>
		 *		<li>A list of login information. Each entry represents a login attempt and includes:
		 * 			<ul>
		 * 				<li>Whether it was successful or not.</li>
		 *  			<li>The date/time at which it occurred.</li>
 		 *			</ul>
 		 * 		</li>	
		 * 		<li>The authenticated subject, which contains List of principals representing the identities of the
		 * 			authenticated subject. These identities are:
		 * 			<ul>
		 * 				<li>The subject's DN (if available - otherwise user identifier).</li>
		 * 				<li>The subject's user identifier (if available - otherwise DN).</li>
		 * 				<li>The roles configured for the subject.</li>
		 * 			</ul>
		 *	 	</li>
		 * 	</ul></p>
		 * @return status Information about the current session and the current
		 * authenticate operator.
		 * @throws SessionException If there is no valid session (session not opened or timed out).
		 * @throws LocalizedRemoteException If there is a communication error.
		 */
		virtual SessionInfo* getSessionInfo() = 0;
    
		/**
		 * Returns true if a session is currently open, otherwise returns false.
		 * @return true If a session is currently open.
		 * @throws SessionException If there is no valid session (session not opened or timed out).
		 * @throws LocalizedRemoteException If there is a communication error.
		 */
		virtual bool isSessionOpen() = 0;
    
		/**
   		 * Returns the permissions allowed in the context of the currently
		 * authenticated identity.
		 * <p>An optional resource name can be specified to provide
		 * resource-specific permission information for the identity.</p>
		 * @param resource Resource name.
		 * @return The permissions allowed for the specified resource and identity.
		 * @throws SessionException If there is no valid session (session not opened or timed out).
		 * @throws LocalizedRemoteException If there is a communication error.
		 */
		virtual PermissionVector* getPermissions(TString resource) = 0;

		/**
		 * Returns configuration information stored on the CMS server.
		 * @param configurationIds An array of configuration information to be retrieved from the server. 
		 * @return An array of parameter with one parameter for each configuration elements requested.
		 * @throws NoSuchConfigException If the specified configuration is not found.
		 * @throws LocalizedRemoteException If there is a communication error.
		 */
		virtual ParameterVector* getConfiguration(TStringVector* configurationIds) = 0;

// -----------------------------------------------------------------------------------------
//		TODO: the methods below will likely not be published
//  
//		TODO: future implementation
//      /*
//       * Initiates/continues a open session conversation where more than one
//       * request/response interaction is required. If an empty array of parameters
//       * is returned, the conversation is complete from the server-side and should
//       * be completed from the client side with a final call to
//       * openSesssion(Parameter[]).
//       * @see #openSession(Parameter[])
//       * @param parameters Reserved for future use. 
//       * @return Response parameters to be used on the client side for the specific
//       * authentication mechanism in use.
//       * @throws SessionException If there is no valid session (session not opened or timed out).
//       * @throws LocalizedRemoteException If there is a communication error.
//       */
//		virtual ParameterVector* openSessionConv(ParameterVector* parameters) = 0;
//
//		TODO: future implementation
//		/*
//		 * Returns true if a session is currently open and the a user is
//		 * authenticated, otherwise false.
//		 * @return true If a session is currently open and the a user is
//		 * authenticated.
//		 * @throws SessionException If there is no valid session (session not opened or timed out).
//		 * @throws LocalizedRemoteException If there is a communication error.
//		 */
//		virtual bool isAuthenticated() = 0;
//
//		TODO: future implementation
//		/*
//		 * Gets the active role identifier.
//		 * @return The active role identifier.
//		 * @throws SessionException If there is no valid session (session not opened or timed out).
//		 * @throws LocalizedRemoteException If there is a communication error.
//		 */
//		virtual TString getRole() = 0;
//  
//		TODO: future implementation
//		/*
//		 * Activates the role given as parameter.
//		 * @param roleID The role to activate.
//		 * @throws NoSuchRoleException If the roles does not exist.
//		 * @throws RoleNotAuthorizedException If the user is not authorized to use
//		 * the specified role.
//		 * @throws RoleException 
//		 * @throws SessionException If there is no valid session (session not opened or timed out).
//		 * @throws LocalizedRemoteException If there is a communication error.
//		 */
//		virtual void activateRole(TString roleID) = 0;

	};

} // namespace AI
namespace ActivIdentity = AI;
