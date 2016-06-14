/*
 * (c) Copyright 2006 ActivIdentity
 * All Rights Reserved.
 * 
 * Created on May 5, 2006
 */
#include "aiCCMUtil.h"
#include "aiCCMUtilImpl.h"
#include "aiCCMSmo.h"

#ifdef AICCMSMO_EXPORTS
#define AICCMSMO_API __declspec(dllexport)
#define EXPSMO_TEMPLATE
#else
#define AICCMSMO_API __declspec(dllimport)
#define EXPSMO_TEMPLATE extern
#endif

class	securitymoduleSoapBinding;

namespace AI
{

/*@internal
 * This API provides an security module-centric content synchronization
 * functionality. The security module management request is realized as security module
 * personalization operations through the synchronization management
 * functionality. SecurityModule management requests are action in a security
 * module-centric manner. The security module itself is synchronized with all
 * requests pending for that security module.
 */
class AICCMSMO_API SecurityModuleManagerImpl : public DLL_OBJECT(SecurityModuleManager), public SessionManagerImpl
{
	securitymoduleSoapBinding	*smoProxy;

public:
/*@internal
     * Constructor.
     * @param URL URL of the SecurityModuleManager web service. The URL should be of the form: 
     * <pre>http://<host>:<port>/aims/services/securitymodule
     * </pre>.
     */
	SecurityModuleManagerImpl();
	SecurityModuleManagerImpl(TString URL);
	SecurityModuleManagerImpl(TString URL, ParameterVector* INITPARAM);

/*@internal
     * Destructor.
     */
	virtual ~SecurityModuleManagerImpl(void);

/*@internal
	 * Initializes a SecurityModuleManagerImpl instance.
	 */
	virtual Manager* init(ParameterVector* parameters);

/*@internal
     * Opens a new session for this API. Must be the first call made. All the subsequent calls must occur in
     * the scope of a valid session.
     * @param parameters Reserved for future use. 
     * @return The identifier of the session opened by the server.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual TString	openSession(ParameterVector *parameters);

/*@internal
     * Returns true if a session is currently open, otherwise false.
     * @return true If a session is currently open.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual bool			isSessionOpen(void);

/*@internal
     * Closes the current session. 
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual void			closeSession(void);

/*@internal
     * Returns status information about the current session and the current
     * authenticate operator. 
     * @return Status information about the current session and the current
     * authenticate operator.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
    virtual SessionInfo				*getSessionInfo(void);
    
/*@internal
     * Returns the permissions allowed in the context of the currently
     * authenticated identity.
     * <p>An optional resource name can be specified to provide
     * resource-specific permission information for the identity.</p>
     * @param resource Resource name.
     * @return The permissions allowed for the specified resource and identity.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual PermissionVector		*getPermissions(TString resource);

/*@internal
	 * Returns configuration information stored on the CMS server.
	 * @param configurationIds An array of configuration information to be retrieved from the server. 
	 * @return An array of parameter with one parameter for each configuration elements requested.
	 * @throws NoSuchConfigException If the specified configuration is not found.
	 * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual ParameterVector* getConfiguration(TStringVector* configurationIds);

/*@internal
	 * Returns the API version.
	 * @return The version returned by the implementation.
	 * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual TString	getVersion(void);

/*@internal
	 * Returns the DLL version.
	 * @return The version returned by the implementation.
	 * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual TString	getClientVersion(void);

/*@internal
	 * Returns the life cycle status of this security module.
	 * @param smId The security module.
	 * @return The life cycle status of this security module.
	 * @throws NoSuchSecurityModuleException If the security module identifier 
	 * specified does not identify a valid security module.
	 * @throws ManagementException If an internal implementation level failure
	 * occurs.
     * @throws SessionException  If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
	 */
	virtual TString			getLifecycleStatus(SecurityModuleId *smId);

/*@internal
     * Returns a list of security module identifiers that are bound to the specified
     * wallet. Wallet:SecurityModule is 1:n.
     * @param walletId The unique identifier of the wallet.
     * @return List of identifiers identifying security modules.
     * @throws NoSuchWalletException If the specified wallet identifier does not
     * identify a valid wallet.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual SecurityModuleIdVector	*getBoundSMFromWallet(WalletId *walletId);

/*@internal
     * Returns a list of security module identifiers that are bound to the specified
     * user. <p><b>Note: </b><i>User:Wallet:SecurityModule is 1:1:n.</i></p>
     * @param userId The unique identifier of the user.
     * @return List of identifiers identifying security modules.
     * @throws NoSuchUserException If the specified user identifier does not identify a
     * valid user.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual SecurityModuleIdVector	*getBoundSMFromUser(UserId *userId);

/*@internal
     * Triggers the indicated lifecycle workflow processing for the specified
     * security module.
     * <p>Processing invoked through this method will not impact the card's content.
     * For card content modification use {@link WalletManager#submitActions(WalletId *, ActionVector *, TString)}</p>
     * @param smId The security module to process.
     * @param process The workflow process to perform.
     * @param parameters The input parameters required to perform the process, or
     * an empty array.
     * @throws NoSuchSecurityModuleException If the security module identifier specified
     * does not identify a valid security module.
     * @throws InvalidStateException If the security modules is in a state not
     * compatible with the requested process.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void					performProcess(SecurityModuleId *smId, TString process, ParameterVector *parameters);

/*@internal
     * Identifies a security module using platform-specific security module
     * identification information and returns a security module identifier based
     * on this information.
     * <p>
     * If CMS is configured to automatically register security modules, this
     * method will register any unregistered security module presented. If
     * already recognized, no action will be taken.				 </p>
     * <p>
     * Use {@link SyncManager#getPlatformIdentifiers(TString, ParameterVector *) SyncManager#getPlatformIdentifiers(TString, ParameterVector *)}
     * for retrieval of the platform identifiers from the security module.</p>
     * @param platformClass The class of the security module platform
     * (smartcard, TPM, USB, etc.). Supported options are:
     * <ul>
     * <li>PLATFORM_CLASS_SMARTCARD
     * </ul>
     * @param platformIds List of platform-specific security module
     * identification name/value pairs as reported by
     * {@link SyncManager#getPlatformIdentifiers(TString, ParameterVector *) SyncManager.getPlatformIdentifiers(TString, ParameterVector *)}.
     * @return Unique security module identifier.
     * @throws NoSuchSecurityModuleException If the security module is
     * unidentifiable or not known where automatic security module registration
     * is disabled.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual SecurityModuleId		*identifySecurityModule(TString platformClass, ParameterVector *platformIds);

/*@internal
     * Returns the security module with information representing its
     * internal encoded state.  
     * @see SecurityModule
     * @param securityModuleId The security module identifier.
     * @return The security module.
     * @throws NoSuchSecurityModuleException If the security module identifier 
     * specified does not identify a valid security module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual SecurityModule			*getSecurityModule(SecurityModuleId *smId);
};

} // namespace AI
namespace ActivIdentity = AI;
