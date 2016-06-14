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
// CLIENT CLASS IMPLEMENTATIONS
//====================================================================================================

// * @author John Babbidge
/**
 * A synchronization event sent from the server. The event identifier may be used to
 * determine the expected event data.
 */
class AICCMAPI_API SyncEvent
{
	TString		id;
	ParameterVector		*parameters;

public:
    /**
     * Constructor.
     * @param id The type identifier of this event.
     * @param parameters Pointer to the event&apos;s metadata parameters.
     */
	SyncEvent(TString id, ParameterVector *parameters = NULL);

    /**
     * Copy Constructor.
 	 * @param src Source SyncEvent object.  A copy of the content of this object is used for new content.
     */
	SyncEvent(const SyncEvent &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source SyncEvent object.  A copy of the content of this object is used for new content.
	 */
	virtual SyncEvent& operator=(const SyncEvent& src);

    /**
     * Destructor.
     */
	virtual ~SyncEvent(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual SyncEvent* clone() const	{ return new SyncEvent(*this); }

    /**
     * Returns the type identifier of this event.
     * @return The type identifier of this event.
     */
	virtual	TString			getId(void);

    /**
     * Returns the event&apos;s metadata parameters. 
     * @return The event&apos;s metadata parameters.
     */
	virtual	ParameterVector			*getParameters(void);

    /**
     * Sets the event&apos;s metadata parameters. 
     * @param parameters Pointer to the event&apos;s metadata parameters.
     */
	virtual	void					setParameters(ParameterVector *parameters);
};

// * @author John Babbidge
/**
 * Interface that provides an entry point that receives events sent from the server during synchronization. 
 * <p>This interface should be implemented by the integration layer
 * to process synchronization events in an implementation-specific manner.</p>
 * Any class that implements the SyncListener interface should also implement the
 * onEventReceived(SyncEvent e) method.
 * <p><b>Note: </b><i>SyncListener does not generate credential events on the Cybertrust PKI certificate authority
 * platform and the Verisign PKI connector.</i>
 */
class AICCMAPI_API SyncListener
{
public:
    /**
     * If a listener implementation is passed to a synchronization, this
     * method is invoked when events are dispatched from the
     * server.
     * @param event A synchronization event that was received.
     * @throws SyncListenerException If an error occurs in the SyncListener
     * implementation.
     */
    virtual	void onEventReceived(SyncEvent event) = 0;
};

/**
 * Represents all client plug-in information.
 */
#define CLIENT_PLUGIN_STATUS_READY				_T("READY")
#define CLIENT_PLUGIN_STATUS_INVALID			_T("INVALID")
class AICCMAPI_API ClientPluginInfo : public DLL_OBJECT_BASE {

private:
	TString name;
	TString type;
	TString version;
	TString description;
	TString status;
	TStringVector* actionTypeList;

public:

	/**
     * Constructor.
     */
	ClientPluginInfo(void);

	/**
     * Copy Constructor.
     */
	ClientPluginInfo(const ClientPluginInfo &src);

	/**
	 * Assignment Operator overload.
	 */
	virtual ClientPluginInfo& operator=(const ClientPluginInfo& src);

    /**
     * Destructor.
     */
	virtual ~ClientPluginInfo(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual ClientPluginInfo* clone() const	{ return new ClientPluginInfo(*this); }

	/**
	 * @return the name
	 */
	virtual TString getName();

	/**
	 * @param name the name to set
	 */
	virtual void setName(TString name);

	/**
	 * @return the type
	 */
	virtual TString getType();

	/**
	 * @param type the type to set
	 */
	virtual void setType(TString type);

	/**
	 * @return the version
	 */
	virtual TString getVersion();

	/**
	 * @param version the version to set
	 */
	virtual void setVersion(TString version);

	/**
	 * @return the description
	 */
	virtual TString getDescription();

	/**
	 * @param description the description to set
	 */
	virtual void setDescription(TString description);

	/**
	 * @return the status
	 */
	virtual TString getStatus();

	/**
	 * @param status the status to set
	 */
	virtual void setStatus(TString status);

	/**
	 * @return the actionTypeList
	 */
	virtual TStringVector* getActionTypeList();

	/**
	 * @param actionTypeList the actionTypeList to set
	 */
	virtual void setActionTypeList(TStringVector* actionTypeList);
};

/**
 * Vector of ClientPluginInfo objects.
 */
EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<ClientPluginInfo>;
typedef Vector<ClientPluginInfo>	ClientPluginInfoVector;

//====================================================================================================
// COMMON CLASS DEFINITIONS
//====================================================================================================

//====================================================================================================
// SYNC MANAGER DEFINITION
//====================================================================================================

/**
 * The SyncManager component performs the submitted actions 
 * (issue, post-issue, recycle, unlock, etc) on a particular SecurityModule in a User&apos;s Wallet. 
 * The operations are scheduled against a User&apos;s Wallet.
 * <p>This API provides a security module-centric content synchronization functionality. The security module 
 * management request is a security module personalization operation submitting through synchronization management 
 * functionality. The security module itself is synchronized with all requests pending for that security module. </p>
 */
class AICCMAPI_API SyncManager : public SessionManager
{
public:

    /**
     * @internal
     * Sends the synchronize command for the specified security module.
     * param smId identifier of the security module to attempt synchronization cancellation.
     * throws NoSuchSecurityModuleException If the specified security module does not exist.
     * throws SyncNotCancelledException If unable to cancel the synchronization.
     * throws SyncException If an internal implementation level failure occurred.
     * throws SessionException If a problem communication with the server occurs.
     * throws LocalizedRemoteException If there is a communication error.
     */
	virtual void cancelSynchronize(SecurityModuleId *smId) = 0;

    /**
     * Retrieves and returns the platform identifiers from the security module
     * in the supplied reader.
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.).
     * @param accessParameters The parameters required to access the security
     * module such as the smartcard reader name. Supported options are:
     * <ul>
     * 	<li>For the PLATFORM_CLASS_SMARTCARD platform class:
     * 		<ul>
     * 			<li>ACCESS_PARAM_SMARTCARD_READER: the name of the reader in which the security module is inserted</li>
     * 		</ul>
	 *  </li>
     * </ul>
     * @return platformIds Platform-specific data that identifies the security
     * module&apos;s platform and uniquely identifies the security module.
     * @throws SecurityModuleIdentException If the platformClass is not recognized.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual ParameterVector* getPlatformIdentifiers(TString platformClass, ParameterVector *accessParameters) = 0;

    /**
     * Retrieves and returns the platform profile attributes from the security 
     * module in the supplied reader.
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.).
     * @param accessParameters The parameters required to access the security
     * module such as the smartcard reader name. Supported options are:
     * <ul>
     * 	<li>For the PLATFORM_CLASS_SMARTCARD platform class:
     * 		<ul>
     * 			<li>ACCESS_PARAM_SMARTCARD_READER: the name of the reader in which the security module is inserted</li>
     * 		</ul>
	 *  </li>
     * </ul>
     * @param profileAttributeIds Pointer to the platform profile attributes to return.
     * @return platformIds Pointer to the requested profile attributes.
     * @throws NoSuchSecurityModuleException If the specified security module does not exist.
     * @throws NoSuchProfileAttributeException If the specified profile attribtue does not exist.
     * @throws SessionException If a problem communication with the server occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual ParameterVector* getProfileAttributes( TString platformClass, ParameterVector *accessParameters, TStringVector *profileAttributeIds ) = 0;

	/**
     * Executes all pending requests for the specified security module.
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.).
     * @param accessParameters The parameters required to access the security
     * module such as the smartcard reader name. Supported options are:
     * <ul>
     * 	<li>For the PLATFORM_CLASS_SMARTCARD platform class:
     * 		<ul>
     * 			<li>ACCESS_PARAM_SMARTCARD_READER: the name of the reader in which the security module is inserted</li>
     * 		</ul>
	 *  </li>
     * </ul>
     * @param runtimeParameters Pointer to the runtime parameters for the synchronization.
     * @return true If synchronization was performed and false if nothing was found to synchronize.
     * @throws NoSuchSecurityModuleException If the specified security module does not exist.
     * @throws SyncCancelledException If the synchronization is cancelled by another thread.
     * @throws SyncException If an internal implementation level failure occurred.
     * @throws SessionException If a problem communication with the server occurs.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual bool			synchronize(TString platformClass, ParameterVector *accessParameters, ParameterVector *runtimeParameters) = 0;

    /**
     * Executes all pending requests for the specified security module and provides an event
     * listener to provide status data to.
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.).
     * @param accessParameters The parameters required to access the security
     * module such as the smartcard reader name. Supported options are:
     * <ul>
     * 	<li>For the PLATFORM_CLASS_SMARTCARD platform class:
     * 		<ul>
     * 			<li>ACCESS_PARAM_SMARTCARD_READER: the name of the reader in which the security module is inserted</li>
     * 		</ul>
	 *  </li>
     * </ul>
     * @param runtimeParameters Pointer to the runtime parameters for the synchronization.
     * @param listener Pointer to a SyncListener implementation to which status information is to be reported.
     * @return true If synchronization was performed and false if nothing was found to synchronize.
     * @throws NoSuchSecurityModuleException If the specified security module does not exist.
     * @throws SyncCancelledException If the synchronization is cancelled by another thread.
     * @throws SyncException If an internal implementation level failure occurred. 
     * @throws SessionException If a problem communication with the server occurs.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual bool synchronize(TString platformClass, ParameterVector *accessParameters, ParameterVector *runtimeParameters, SyncListener *listener) = 0;

	/**
	 * Returns <code>true</code> if there is pending activity for the discovered security module, 
	 * otherwise <code>false</code>. 
	 * <br>If the security module is not in a state valid for synchronization an exception will be thrown. 
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.).
     * <br>Supported classes:
     * <ul>
     * <li><code>PLATFORM_CLASS_SMARTCARD</code></li>
     * </ul>
     * @param platformIds A list of platform-specific security module identification name/value pairs.
     * @return <code>true</code> if there are pending tasks for the discovered security module 
     * @throws NoSuchSecurityModuleException If the specified security module does not exist.
     * @throws NotSyncableStateException If the discovered security module is not in a valid state for synchronization.
     * @throws SessionException If a problem communication with the server occurs. 
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual bool isSyncRequired(TString platformClass, ParameterVector* platformIds) = 0;
    
	/**
	 * Synchronizes the supplied action type for the security module inserted into a given reader.
	 * <p><b>Note: </b>If the desired action requires an application set then it must be 
	 * submitted via WalletManager.submitActions(WalletId, Action[], String)<br>
	 * Supported action types can:
	 * <ul>
	 * <li>Not modify the managed state of the security module or its applications. Failure of an action will not invalidate the security module.</li> 
	 * <li>Only require low privilege (client authentication to security module. e.g. PIN authentication) or no privilege to be considered valid.</li>
	 * </ul>
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.).
     * <br>Supported classes:
     * <ul>
     * <li><code>PLATFORM_CLASS_SMARTCARD</code></li>
     * </ul>
     * @param accessParameters The parameters required to access the security
     * module such as the smartcard reader name. Supported options are:
     * <ul>
     * 	<li>For the <code>PLATFORM_CLASS_SMARTCARD</code> platform class:
     * 		<ul>
     * 			<li><code>ACCESS_PARAM_SMARTCARD_READER</code>: the name of the reader in which the security module is inserted</li>
     * 		</ul>
	 *  </li>
     * </ul>
     * @param runtimeParameters The runtime parameters for the synchronization.
	 * @param actionType The action type to perform.
     * @param listener A SyncListener implementation to which to report status information..
	 * @return Returned data from the executed action type in form of a parameter array.
	 * @throws InvalidActionException If either the access or runtime parameters for the action type were invalid.
	 * @throws NoSuchSecurityModuleException If the specified security module does not exist.
	 * @throws SyncCancelledException If the synchronization is canceled by another thread.
	 * @throws SyncException If an internal implementation level failure.
	 * @throws SessionException If a problem communication with the server occurs occurs. 
	 * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual ParameterVector* synchronizeAction(TString platformClass, ParameterVector* accessParameters, ParameterVector* runtimeParameters, TString actionType, SyncListener* listener) = 0;

	/**
	 * Retrieves and returns the list of readers of the specified reader class 
	 * accessible via the registered client plug-ins.
     * @param readerClass the class of the reader (smartcard, TPM, USB, etc.).
     * <br>Supported classes:
     * <ul>
     * <li><code>READER_CLASS_SMARTCARD</code></li>
     * </ul>
	 * @return The list of readers. The resulting concatenated list of readers is 
	 * returned as an array parameter with one reader per parameter identified by 
	 * incrementing id.
     * @throws ManagementException If an internal failure occurs.
	 */
	virtual ParameterVector* getReaderList(TString readerClass) = 0;

	/**
	 * Retrieves and returns client plug-ins information.
	 * @return An array of client plug-in information for all registered plug-ins.
	 * @throws SyncException If an internal implementation level failure.
	 */
	virtual ClientPluginInfoVector* getClientPluginsInfo() = 0;

};


//====================================================================================================
// FACTORY CLASS DEFINITION
//====================================================================================================

	/**
	* Factory that provides a handle to a SyncManagerFactory instance.
	* <p>
	* The SyncManagerFactory factory configuration parameters are either of the following values:
	* <ul>
	*	<li>CONFIG_URL -- The complete URL of the SyncManagerFactory service in CMS. </li>
	* </ul>
	* or both of the following values:
	* <ul>
	*	<li>CONFIG_HOST_NAME -- The hostname of the SyncManagerFactory service in CMS. </li>
	*	<li>CONFIG_HOST_PORT -- The port number of the SyncManagerFactory service in CMS.   </li>
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
	class AICCMAPI_API SyncManagerFactory : public ManagerFactory
	{
	public:
		/**
		* Returns the SyncManager based on the underlying provider.
		* Uses the default factoryDefinition and default ProviderName.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The SyncManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the SyncManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the SyncManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the SyncManager service in CMS.   </li>
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
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.</li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.</li>
		* </ul>
		* </p>
		* @return SyncManager Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static SyncManager* newInstance(ParameterVector* parameters);

		/**
		* Returns the SyncManager based on the underlying provider.
		* Uses the default factoryDefinition.
		* @param providerName Provider identifier.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The SyncManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the SyncManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the SyncManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the SyncManager service in CMS.   </li>
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
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.</li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.</li>
		* </ul>
		* </p>
		* @return SyncManager Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static SyncManager* newInstance(TString providerName, ParameterVector* parameters);

		/**
		* Returns the SyncManager based on the underlying provider.
		* @param factoryDefinition Property file containing provider information.
		* @param providerName Provider identifier.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The SyncManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the SyncManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the SyncManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the SyncManager service in CMS.   </li>
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
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.</li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.</li>
		* </ul>
		* </p>
		* @return SyncManager Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static SyncManager* newInstance(TString factoryDefinition, TString providerName, ParameterVector* parameters);

	protected:
		static SyncManager* init(TString factoryDefinition, TString providerName, ParameterVector* parameters);
	};

//====================================================================================================
// EXCEPTION CLASS DEFINITIONS
//====================================================================================================


}


