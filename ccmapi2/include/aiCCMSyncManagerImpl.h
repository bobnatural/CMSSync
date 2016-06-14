/*
 * (c) Copyright 2006 ActivIdentity
 * All Rights Reserved.
 * 
 * Created on May 5, 2006
 */
#include "aiCCMUtil.h"
#include "aiCCMUtilImpl.h"
#include "aiCCMTcd.h"
#include "aiClientOperationExecutor.h"

#ifdef AICCMSYNC_EXPORTS
#define AICCMSYNC_API __declspec(dllexport)
#define EXPSYNC_TEMPLATE
#else
#define AICCMSYNC_API __declspec(dllimport)
#define EXPSYNC_TEMPLATE extern
#endif

class	pipeSoapBinding;

namespace AI
{

// forward declaration
class COEPluginHandler;

/**
 * This API provides a security module-centric content synchronization
 * functionality. The security module management request is realized as security module
 * personalization operations through the synchronization management
 * functionality. SecurityModule management requests are action in a security
 * module-centric manner. The security module itself is synchronized with all
 * requests pending for that security module.
 */
class AICCMSYNC_API SyncManagerImpl : public DLL_OBJECT(SyncManager), public SessionManagerImpl
{
private:
	pipeSoapBinding	*pipeProxy;
	COEPluginHandler *coePlugins;
	ParameterVector *getCOEConfiguration();

protected:
	bool synch(
			TString	platformClass, 
			ParameterVector	*accessParams, 
			ParameterVector	*runtimeParams, 
			SyncListener	*listener);

	TString	openSync(
			TString	platformClass, 
			ParameterVector	*platformIds, 
			ParameterVector	*runtimeParams);

	void closeSync(TString	syncId);

public:
    /**
     * Constructor.
     * @param URL The URL of the SyncManagerImpl web service. The URL should be of the form: 
	 * <pre>
     * http://<host>:<port>/aims/services/pipe
	 * </pre>.
     */
	SyncManagerImpl();
	SyncManagerImpl(TString	URL);  // SSL V2
	SyncManagerImpl(TString	URL, ParameterVector* INITPARAM);

    /**
     * Destructor.
     */
	virtual ~SyncManagerImpl(void);

    /**
	 * Initializes the SyncManagerImpl instance.
	 */
	virtual Manager* init(ParameterVector* parameters);

    /**
     * Opens a new session for this API. Must be the first call made. All the subsequent calls must occur in
     * the scope of a valid session.
     * @param parameters Reserved for future use. 
     * @return The identifier of the session opened by the server.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual TString	openSession(ParameterVector *parameters);

    /**
     * Returns true if a session is currently open, otherwise returns false.
     * @return true If a session is currently open.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual bool isSessionOpen(void);

    /**
     * Closes the current session. 
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual void closeSession(void);

    /**
     * Returns status information about the current session and the current
     * authenticate operator. 
     * @return Status information about the current session and the current
     * authenticate operator.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
    virtual SessionInfo	*getSessionInfo(void);
    
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
	virtual PermissionVector *getPermissions(TString resource);

	/**
	 * Returns configuration information stored on the CMS server.
	 * @param configurationIds An array of configuration information to be retrieved from the server. 
	 * @return An array of parameter with one parameter for each configuration elements requested.
	 * @throws NoSuchConfigException If the specified configuration is not found.
	 * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual ParameterVector* getConfiguration(TStringVector* configurationIds);

    /**
	 * Returns the API version.
	 * @return The version returned by the implementation.
	 * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual TString	getVersion(void);

    /**
	 * Returns the DLL version.
	 * @return The version returned by the implementation.
	 */
	virtual TString	getClientVersion(void);

        /**
     * Sends the synchronize command for the specified security module.
     * param smId identifier of the security module on which to attempt to cancel synchronization.
     * throws NoSuchSecurityModuleException If the specified security module does not exist.
     * throws SyncNotCancelledException If unable to cancel the synchronization.
     * throws SyncException If an internal implementation level failure occurred.
     * throws SessionException If a problem occurs when communicating with the server.
     * throws LocalizedRemoteException If there is a communication error.
     */
	virtual void cancelSynchronize(SecurityModuleId *smId);

    /**
     * Retrieves and returns the platform identifiers from the security module
     * in the supplied reader.
     * @param platformClass The class of the security module platform
     * (smartcard, TPM, USB, etc.). Supported options are:
     * <ul>
     * <li>PLATFORM_CLASS_SMARTCARD
     * </ul>
     * @param accessParameters The parameters required to access the security
     * module. Supported options are:
     * <ul>
     * 	<li>For the PLATFORM_CLASS_SMARTCARD platform class:
     * 		<ul>
     * 			<li>"securitymodule.smartcard.readerName": the name of the reader in which the security module is inserted</li>
     * 		</ul>
	 *	</li>
     * </ul>
     * @return platformIds Platform-specific data that identifies the security
     * module's platform and uniquely identifies the security module.
     * @throws SecurityModuleIdentException If the platformClass is not recognized.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual ParameterVector* getPlatformIdentifiers(TString platformClass, ParameterVector *accessParameters);

    /**
     * Retrieves and returns the platform profile attributes from the security 
     * module in the supplied reader.
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.).
     * @param accessParameters The parameters required to access the security
     * module such as the smartcard reader name. Supported options are:
     * <ul>
     * 	<li>For the "smartcard" platform class:
     * 		<ul>
     * 			<li><pre>readerName
     * </pre>: the name of the reader in which the security module is inserted</li>
     * 		</ul>
	 *  </li>
     * </ul>
     * @param profileAttributeIds The platform profile attributes to return.
     * @return platformIds Data that describes the security module's profile 
     * attributes.
     * @throws NoSuchSecurityModuleException If the specified security module does not exist.
     * @throws NoSuchProfileAttributeException If the specified profile attribtue does not exist.
     * @throws SessionException If a problem occurs when communicating with the server.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual ParameterVector* getProfileAttributes( TString platformClass, ParameterVector *accessParameters, TStringVector *profileAttributeIds );

    /**
     * Executes all pending requests for the specified security module.
     * @param platformClass the class of the security module platform
     * (smartcard, TPM, USB, etc.). Supported options are:
     * <ul>
     * <li>PLATFORM_CLASS_SMARTCARD
     * </ul>
     * @param accessParameters The parameters required to access the security
     * module. Supported options are:
     * <ul>
     * <li>For the PLATFORM_CLASS_SMARTCARD platform class:
     * <ul>
     * <li>"securitymodule.smartcard.readerName": the name of the reader in which the security module is inserted</li>
     * </ul></li>
     * </ul>
     * @param runtimeParameters Pointer to the runtime parameters for the synchronization.
     * @return true If synchronization was performed and false if nothing was found to synchronize.
     * @throws NoSuchSecurityModuleException If the specified security module does not exist
     * @throws SyncCancelledException If the synchronization is cancelled by another thread.
     * @throws SyncException If an internal implementation level failure occurred.
     * @throws SessionException If a problem occurs when communicating with the server.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual bool synchronize(TString platformClass, ParameterVector *accessParameters, ParameterVector *runtimeParameters);

    /**
     * Executes all pending requests for the specified security module and provides an event
     * listener to which to provide status data.
     * @param platformClass The class of the security module platform.
     * (smartcard, TPM, USB, etc.). Supported options are:
     * <ul>
     * <li>PLATFORM_CLASS_SMARTCARD
     * </ul>
     * @param accessParameters The parameters required to access the security
     * module. Supported options are:
     * <ul>
     * <li>For the PLATFORM_CLASS_SMARTCARD platform class:
     * <ul>
     * <li>"securitymodule.smartcard.readerName": the name of the reader in which the security module is inserted</li>
     * </ul></li>
     * </ul>
     * @param runtimeParameters Pointer to the runtime parameters for the synchronization.
     * @param listener A SyncListener implementation to which to report status information.
     * @return true If synchronization was performed and false if nothing was found to synchronize.
     * @throws NoSuchSecurityModuleException If the specified security module does not exist.
     * @throws SyncCancelledException If the synchronization is cancelled by another thread.
     * @throws SyncException If an internal implementation level failure occurred.
     * @throws SessionException If a problem occurs when communicating with the server.
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual bool synchronize(TString platformClass, ParameterVector *accessParameters, ParameterVector *runtimeParameters, SyncListener *listener);

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
	virtual bool isSyncRequired(TString platformClass, ParameterVector* platformIds);

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
	virtual ParameterVector* synchronizeAction(TString platformClass, ParameterVector* accessParameters, ParameterVector* runtimeParameters, TString actionType, SyncListener* listener);

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
	virtual ParameterVector* getReaderList(TString readerClass);

	/**
	 * Retrieves and returns client plug-ins information.
	 * @return An array of client plug-in information for all registered plug-ins.
	 * @throws SyncException If an internal implementation level failure.
	 */
	virtual ClientPluginInfoVector* getClientPluginsInfo();

};

} // namespace AI
namespace ActivIdentity = AI;