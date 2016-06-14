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
#include "aiCCMUtil.h"
#include "aiInterfaces.h"
#include "aiLocalization.h"

#ifdef AICCMAPI_EXPORTS
#define AICCMAPI_API __declspec(dllexport)
#define EXPUTIL_TEMPLATE
#else
#define AICCMAPI_API __declspec(dllimport)
#define EXPUTIL_TEMPLATE extern
#endif // AICCMAPI_EXPORTS

namespace ActivIdentity = AI;
namespace AI
{

//====================================================================================================
// COMMON CLASS DEFINITIONS
//====================================================================================================
/**
* An interface that describes a particular security module. Each SecurityModuleInterface instance contain 
* an interface type (contact or contact-less) and status (active or not).
*/
class AICCMAPI_API SecurityModuleInterface : public DLL_OBJECT_BASE
{
	TString type;
	TString status;

public:
	/**
     * Constructor.
     */
	SecurityModuleInterface(void);

    /**
     * Copy Constructor.
 	 * @param src Source SecurityModuleInterface object.  A copy of the content of this object is used for new content.
     */
	SecurityModuleInterface(const SecurityModuleInterface &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source SecurityModuleInterface object.  A copy of the content of this object is used for new content.
	 */
	virtual SecurityModuleInterface& operator=(const SecurityModuleInterface& src);

    /**
     * Destructor.
     */
	virtual ~SecurityModuleInterface(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual SecurityModuleInterface* clone() const	{ return new SecurityModuleInterface(*this); }

    /**
     * Returns the type of this interface of the security module.
     * @return A pointer to the type of this interface of the security module.
     */
	virtual TString getType();
    
    /**
     * Sets the type of this interface of the security module.
     * @param type The type of this interface of the security module.
     */
	virtual void setType(TString type);
	    
    /**
     * Returns the status of this interface of the security module.
     * @return The status of this interface of the security module.
     */
	virtual TString getStatus();
    
    /**
     * Sets the status of this interface of the security module.
     * @param status The status of this interface of the security module.
     */
	virtual void setStatus(TString status);
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<SecurityModuleInterface>;
typedef Vector<SecurityModuleInterface>				SecurityModuleInterfaceVector;

// * @author John Babbidge
/**
 * The security module, its state, and application/credential contents.
 */
class AICCMAPI_API SecurityModule: public Managed
{
	SecurityModuleId* id;
	ConfigurationId* profileId;
	SecurityModuleInterfaceVector* interfaces;
	ApplicationVector* applications;
	CredentialVector* credentials;
	TString applicationSet;

public:
	/**
     * Constructor.
     */
	SecurityModule(void);

    /**
     * Copy Constructor.
 	 * @param src Source SecurityModule object.  A copy of the content of this object is used for new content.
     */
	SecurityModule(const SecurityModule &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source SecurityModule object.  A copy of the content of this object is used for new content.
	 */
	virtual SecurityModule& operator=(const SecurityModule& src);

    /**
     * Destructor.
     */
	virtual ~SecurityModule(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual SecurityModule* clone() const	{ return new SecurityModule(*this); }

    /**
     * Returns a pointer to the unique identifier of the security module.
     * @return A pointer to the unique identifier of the security module.
     */
    virtual SecurityModuleId* getId();

    /**
     * Sets the unique identifier of the security module.
     * @param id Pointer to the unique identifier of the security module.
     */
    virtual void setId(SecurityModuleId* id);

    /**
     * Returns the unique identifier of the security module profile used to
     * manage this security module.
     * @return Pointer to the unique identifier of the security module profile.
     */
    virtual ConfigurationId* getProfileId();

    /**
     * Sets the unique identifier of the security module profile used to manage
     * this security module.
     * @param profileId Pointer to the unique identifier of the security module profile.
     */
    virtual void setProfileId(ConfigurationId* profileId);

    /**
     * Returns the supported interfaces for a security module.
     * @return A pointer to the supported interfaces for a security module.
     */
    virtual SecurityModuleInterfaceVector* getInterfaces();

    /**
     * Sets the supported interfaces for a security module.
     * @param interfaces Pointer to the supported interfaces for a security module.
     */
    virtual void setInterfaces(SecurityModuleInterfaceVector* interfaces);

//    /*
//     * Returns the type of the security module.
//     * @return The type of the security module.
//     */
//    virtual TString getType();
//
//    /*
//     * Sets the type of the security module.
//     * @param type The type of the security module.
//     */
//    virtual void setType(TString type);

    /**
     * Returns the set of applications in the security module.
     * @return Pointer to the set of applications in the security module.
     */
    virtual ApplicationVector* getApplications();

    /**
     * Sets the set of applications in the security module.
     * @param applications Pointer to the set of applications in the security module.
     */
    virtual void setApplications(ApplicationVector* applications);

    /**
     * Returns the application set name.
     * @return The application set name.
     */
	virtual TString getApplicationSet();

    /**
     * Sets the application set name.
     * @param applicationSet the application set name.
     */
	virtual void setApplicationSet(TString applicationSet);

    /**
     * Returns the set of credentials in the security module.
     * @return A pointer to the set of credentials in the security module.
     */
    virtual CredentialVector* getCredentials();

    /**
     * Sets the set of credentials in the security module.
     * @param credentials Pointer to the set of credentials in the security module.
     */
    virtual void setCredentials(CredentialVector* credentials);
};

//====================================================================================================
// SECURITY MODULE MANAGER DEFINITION
//====================================================================================================

   /**
	* The SecurityModuleManager component makes it possible to retrieve information about and
	* perform lifecycle processes (such as suspend, resume, revoke, activate) on devices
	* (SecurityModules).
	* <p>This API represents an abstract business-level management view of the
	* security module contents and logical state.</p>
	* 
	* <p><b>Note:</b><i>This API, when used in a remote client, requires appropriate authentication and
	* authorization in order to be authorized to exercise any functionality.</i></p>
 	* <p>Abstract Class, Pure-Virtual Interface.</p>
	*/
	class AICCMAPI_API SecurityModuleManager : public SessionManager
	{
	public:

	/**
	 * Returns the life cycle status of this security module.
	 * @param smId Pointer to the security module.
	 * @return The life cycle status of this security module.
	 * @throws NoSuchSecurityModuleException If the security module identifier 
	 * specified does not identify a valid security module.
	 * @throws ManagementException If an internal implementation level failure
	 * occurs.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual TString			getLifecycleStatus(SecurityModuleId *smId) = 0;

    /**
     * Returns a list of security module identifiers that are bound to the specified
     * wallet. Wallet:SecurityModule is 1:n.
     * @param walletId Pointer to the unique identifier of the wallet.
     * @return List of identifiers identifying security modules.
     * @throws NoSuchWalletException If the specified wallet identifier does not
     * identify a valid wallet.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual SecurityModuleIdVector	*getBoundSMFromWallet(WalletId *walletId) = 0;

    /**
     * Returns a list of security module identifiers that are bound to the specified
     * user. <p><b>Note: </b><i>User:Wallet:SecurityModule is 1:1:n.</i></p>
     * @param userId Pointer to the unique identifier of the user.
     * @return List of identifiers identifying security modules.
     * @throws NoSuchUserException If the specified user identifier does not identify a
     * valid user.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual SecurityModuleIdVector	*getBoundSMFromUser(UserId *userId) = 0;

    /**
     * Triggers the indicated lifecycle workflow processing for the specified
     * security module.
     * <p>Processing invoked through this method will not impact the card's content.
     * For card content modification use {@link WalletManager#submitActions(WalletId *, ActionVector *, TString)}</p>
     * @param smId Pointer to the security module to process.
     * @param process The workflow process to perform.
     * @param parameters Pointer to the input parameters required to perform the process or
     * an empty array.
     * @throws NoSuchSecurityModuleException If the security module identifier specified
     * does not identify a valid security module.
     * @throws InvalidStateException If the security modules is in a state not
     * compatible with the requested process.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void					performProcess(SecurityModuleId *smId, TString process, ParameterVector *parameters) = 0;

    /**
     * Identifies a security module using platform-specific security module
     * identification information and returns a security module identifier based
     * on this information.
     * <p>
     * If CMS is configured to automatically register security modules, this
     * method will register any unregistered security module presented. If
     * already recognized, no action will be taken.</p>
     * Use {@link AI::SyncManager#getPlatformIdentifiers(TString, ParameterVector *) SyncManager.getPlatformIdentifiers(TString, ParameterVector *)}
     * to retrieve platform identifiers from the security module.
     * @param platformClass The class of the security module platform
     * (smartcard, TPM, USB, etc.). Supported options are:
     * <ul>
     * <li>PLATFORM_CLASS_SMARTCARD
     * </ul>
     * @param platformIds List of platform-specific security module
     * identification name/value pairs as reported by
     * {@link AI::SyncManager.getPlatformIdentifiers(TString, ParameterVector *) SyncManager.getPlatformIdentifiers(TString, ParameterVector *)}
     * @return A unique security module identifier.
     * @throws NoSuchSecurityModuleException If the security module is
     * unidentifiable or not known where automatic security module registration
     * is disabled.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual SecurityModuleId		*identifySecurityModule(TString platformClass, ParameterVector *platformIds) = 0;

    /**
     * Returns the security module with information representing its
     * internal encoded state.  
     * @see SecurityModule
     * @param smId The security module identifier.
     * @return The security module.
     * @throws NoSuchSecurityModuleException If the security module identifier 
     * specified does not identify a valid security module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual SecurityModule			*getSecurityModule(SecurityModuleId *smId) = 0;

	};

//====================================================================================================
// FACTORY CLASS DEFINITION
//====================================================================================================

	/**
	* Factory that provides a handle to a SecurityModuleManager instance.
	* <p>
	* The SecurityModuleManager factory configuration parameters are either of the following values:
	* <ul>
	*	<li>CONFIG_URL -- The complete URL of the SecurityModuleManager service in CMS. </li>
	* </ul>
	* or both of the following values:
	* <ul>
	*	<li>CONFIG_HOST_NAME -- The hostname of the SecurityModuleManager service in CMS. </li>
	*	<li>CONFIG_HOST_PORT -- The port number of the SecurityModuleManager service in CMS.   </li>
	* </ul>
	* ...and in addition, all of the following values:
	* <ul>
	*	<li>CONFIG_COOKIE -- The semicolon separated list of cookies to append to request header.</li>
	*	<li>CONFIG_RETRY -- The number of attempts to connect to the CMS host (default 1).</li>
	*	<li>CONFIG_TIMEOUT -- The timeout for each connection attempt (default 0; no timeout).</li>
	* </ul>
	* If the certificates have been imported into the store, there will be a prompt to pick a certificate.  
	* To avoid the prompt, pass the following two parameters:
	* <ul>
	*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.					 </li>
	*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.	  </li>
	* </ul>
	* </p>
	*/
	class AICCMAPI_API SecurityModuleManagerFactory : public ManagerFactory
	{
	public:
		/**
		* Returns the SecurityModuleManager based on the underlying provider.
		* Uses the default factoryDefinition and default ProviderName.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The SecurityModuleManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the SecurityModuleManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the SecurityModuleManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the SecurityModuleManager service in CMS.   </li>
		* </ul>
		* ...and in addition, all of the following values:
		* <ul>
		*	<li>CONFIG_COOKIE -- The semicolon separated list of cookies to append to request header.</li>
		*	<li>CONFIG_RETRY -- The number of attempts to connect to the CMS host (default 1).</li>
		*	<li>CONFIG_TIMEOUT -- The timeout for each connection attempt (default 0; no timeout).</li>
		* </ul>
		* If the certificates have been imported into the store, there will be a prompt to pick a certificate.  
		* To avoid the prompt, pass the following two parameters:
		* <ul>
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.					 </li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.	  </li>
		* </ul>
		* </p>
		* @return SecurityModuleManager Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static SecurityModuleManager* newInstance(ParameterVector* parameters);

		/**
		* Returns the SecurityModuleManager based on the underlying provider.
		* Uses the default factoryDefinition.
		* @param providerName Provider identifier.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The SecurityModuleManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the SecurityModuleManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the SecurityModuleManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the SecurityModuleManager service in CMS.   </li>
		* </ul>
		* ...and in addition, all of the following values:
		* <ul>
		*	<li>CONFIG_COOKIE -- The semicolon separated list of cookies to append to request header.</li>
		*	<li>CONFIG_RETRY -- The number of attempts to connect to the CMS host (default 1).</li>
		*	<li>CONFIG_TIMEOUT -- The timeout for each connection attempt (default 0; no timeout).</li>
		* </ul>
		* If the certificates have been imported into the store, there will be a prompt to pick a certificate.  
		* To avoid the prompt, pass the following two parameters:
		* <ul>
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.					 </li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.	  </li>
		* </ul>
		* </p>
		* @return SecurityModuleManager Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static SecurityModuleManager* newInstance(TString providerName, ParameterVector* parameters);

		/**
		* Returns the SecurityModuleManager based on the underlying provider.
		* @param factoryDefinition Property file containing provider information.
		* @param providerName Provider identifier.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The SecurityModuleManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the SecurityModuleManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the SecurityModuleManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the SecurityModuleManager service in CMS.   </li>
		* </ul>
		* ...and in addition, all of the following values:
		* <ul>
		*	<li>CONFIG_COOKIE -- The semicolon separated list of cookies to append to request header.</li>
		*	<li>CONFIG_RETRY -- The number of attempts to connect to the CMS host (default 1).</li>
		*	<li>CONFIG_TIMEOUT -- The timeout for each connection attempt (default 0; no timeout).</li>
		* </ul>
		* If the certificates have been imported into the store, there will be a prompt to pick a certificate.  
		* To avoid the prompt, pass the following two parameters:
		* <ul>
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.					 </li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.	  </li>
		* </ul>
		* </p>
		* @return SecurityModuleManager Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static SecurityModuleManager* newInstance(TString factoryDefinition, TString providerName, ParameterVector* parameters);

	protected:
		static SecurityModuleManager* init(TString factoryDefinition, TString providerName, ParameterVector* parameters);
	};

//====================================================================================================
// EXCEPTION CLASS DEFINITIONS
//====================================================================================================


}


