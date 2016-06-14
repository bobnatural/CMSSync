/*
 * (c) Copyright 2006 ActivIdentity
 * All Rights Reserved.
 * 
 * Created on May 5, 2006
 */
#include "aiCCMUtil.h"
#include "aiCCMUtilImpl.h"
#include "aiCCMCred.h"

#ifdef AICCMCRED_EXPORTS
#define AICCMCRED_API __declspec(dllexport)
#define EXPCRED_TEMPLATE
#else
#define AICCMCRED_API __declspec(dllimport)
#define EXPCRED_TEMPLATE extern
#endif

class	credSoapBinding;

namespace AI
{

/*@internal
 * Provides access to CMS credential management functionality.
 */
class AICCMCRED_API CredentialManagerImpl : public DLL_OBJECT(CredentialManager), public SessionManagerImpl
{
	credSoapBinding	*credProxy;

public:
/*@internal
     * Constructor.
     * @param URL URL of the CredentialManager web service. The URL should be of the form: 
     * <pre>http://<host>:<port>/aims/services/securitymodule
     * </pre>.
     */
	CredentialManagerImpl();
	CredentialManagerImpl(TString URL);
	CredentialManagerImpl(TString URL, ParameterVector* INITPARAM);

/*@internal
     * Destructor.
     */
	virtual ~CredentialManagerImpl(void);

/*@internal
	 * Initializes a CredentialManager instance.
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
	virtual bool isSessionOpen(void);

/*@internal
     * Closes the current session. 
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual void closeSession(void);

/*@internal
     * Returns status information about the current session and the current
     * authenticate operator. 
     * @return Status information about the current session and the current
     * authenticate operator.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
    virtual SessionInfo	*getSessionInfo(void);
    
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
	virtual PermissionVector *getPermissions(TString resource);

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

//    /*
//     * Deletes the specified Credential(s) completing/terminating their
//     * life cycles in batch.
//     * <p>
//     * <br><b>Note: </b><i> Due to this method's batch nature, it assumes an all or
//     * nothing (transactional) approach to Credential management. One failure
//     * means the entire transaction is failed, and rolled back. It is the
//     * provider's responsibility to enforce the contract.</i></p>
//     * @param credIds a list of Credential identifiers
//     * @throws NoSuchCredentialException If one or more of the specified
//     * Credentials is not found.
//     * @throws InvalidStateException If Credential deletion violates the one or
//     * more of the Credentials' life cycles.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void deleteCredentials(CredentialIdVector *credIds) throws NoSuchCredentialException, InvalidStateException, SessionException, ManagementException, LocalizedRemoteException;

/*@internal
     * Retrieves pending ExternalOperation(s) for the specified Credential
     * transaction where the resulting data from external processing will be
     * subsequently used to advance the Credential transaction. The pending
     * ExternalOperation(s) are formed from in the process of credential usage
     * where processing involves multiple steps that require interaction with an
     * entity external to the CredentialManager.
     * <p>
     * Initial pending ExternalOperations can be queued/requested as a result
     * of calling one of the following entry points:
     * <ul>
     * <li><b>{@link #operateCredential(CredentialId, TString, Parameter[]) operateCredential}</b>
     * </ul></p>
     * Once external processing is complete, the updated, processed external
     * operation(s) is/are returned to the CredentialManager to advance or
     * complete the workflow via this same entry point. This updated external
     * operation results  may themselves cause further pending external
     * operations to be queued/requested. The passed in completed External
     * Operation(s) are the results of processing ExternalOperation(s) returned
     * from a previous call to
     * {@link #getNextExternalOperations(TransactionId, ExternalOperationVector *)}.
     * @see #operateCredential(CredentialId, TString, Parameter[])    
     * @param txId The Credential transaction identifier.
     * @param completedExternalOps The previously completed ExternalOperations
     * populated with Credential Elements from each ExternalOperation's
     * execution.
     * @return Zero or more generated ExternalOperation(s) for the specified
     * transaction.
     * @throws NoSuchTransactionException If the specified transaction is not
     * found.
     * @throws MalformedDataException If the input data was malformed.
     * @throws InvalidStateException If Credential deletion violates the one or
     * more of the Credentials' life cycles.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal provider failure occurs or if
     * the external processing was completed with an unrecoverable error.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual ExternalOperationVector *getNextExternalOperations(TransactionId *txId, ExternalOperationVector *completedExternalOps);

/*@internal
     * Commits a Credential transaction to complete the creation/update of
     * the proto-credential/credential associated with the specified
     * transaction. The transaction is expected to be valid and all internal
     * workflow steps be completed, prior to calling this method.
     * @param txId The transaction identifier within which the Credential
     * was created/updated.
     * @return A list of one or more credential identifiers depending upon how the
     * transaction was initiated.
     * @throws NoSuchTransactionException If the transaction identifier was
     * not found.
     * @throws TransactionIncompleteException If the transaction was not
     * completed.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual CredentialIdVector *commitCredentialTransaction(TransactionId *txId);

/*@internal
     * Returns a list of runtime input requirement aliases that must be resolved
     * for the specified credential profile.
     * @param credProfileId The credential profile to examine.
     * @return List of runtime input requirement aliases that must be resolved.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual EntryTemplateVector *getProfileDynamicEntries(ConfigurationId *credProfileId);

/*@internal
     * Returns the complete data dictionary of all the possible EntryTemplates
     * that may appear in a CredentialProfileTemplate from the specified
     * CredentialProvider. The data dictionary will be returned as a
     * CredentialProfileTemplate syntax which is a limited scope XML schema.
     * @param credProfileId The credential profile to examine.
     * @return The complete data dictionary, as a syntax, of all the possible
     *         EntryTemplates for a CredentialProfileTemplate.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual TString getProfileSyntax(ConfigurationId *credProfileId);
    
/*@internal
     * Performs a life cycle process for the specified Credential. The types of
     * processes are associated with the Credential's lifecycle rather than its
     * content.
     * <p><b>Example: </b><i>suspend, resume, or revoke</i></p>
     * @param credId The Credential identifier.
     * @param process The process to perform.
     * @param parameters The input parameters required to perform the process, or
     * an empty array.
     * @throws NoSuchCredentialException If the specified Credential is not found.
     * @throws InvalidStateException If the process requested is invalid for the
     * current state of the Credential.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual void performProcess(CredentialId *credId, TString process, ParameterVector *parameters);

//    /*
//     * Performs a life cycle process or initiates a workflow for the process, for
//     * each of the specified Credentials in batch. The types of processes are associated
//     * with the Credentials' life cycle rather than its content.
//     * <p><b>Example: </b><i>suspend, resume, or revoke</i>
//     * <br><b>Note: </b><i> Due to this method's batch nature, it assumes an all or
//     * nothing (transactional) approach to Credential management. One failure
//     * means the entire transaction is failed, and rolled back. It is the
//     * provider's responsibility to enforce the contract.</i></p>
//     * @param credIds a list of Credential identifiers
//     * @param process The process to perform.
//     * @param parameters The input parameters required to perform the process, or
//     * an empty array.
//     * @throws NoSuchCredentialException If one or more of the specified
//     * Credentials is not found.
//     * @throws InvalidStateException If the process requested is invalid for the
//     * current state of one or more of the Credentials.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void performProcesses(CredentialIdVector *credIds, TString process, ParameterVector *parameters) throws NoSuchCredentialException, InvalidStateException, SessionException, ManagementException, LocalizedRemoteException;

/*@internal
     * Deletes the specified Credential completing/terminating its life cycle.
     * @param credId The Credential identifier.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws InvalidStateException If Credential deletion violates the
     * Credential's life cycle.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual void deleteCredential(CredentialId *credId);

/*@internal
     * Updates an existing credential with the supplied CredentialElements,
     * according to the requested action.
     * @param credId The Credential identifier.
     * @param action The update process requested for the specified Credential.
     * @param inputRequirementData List of data collected based on the profile
     * input requirements to use during the Credential update.
     * @throws NoSuchCredentialException If the specified credential is not
     * found or not valid.
     * @throws MalformedDataException If the input data was malformed.
     * @throws InvalidStateException If Credential update violates the
     * Credential's life cycle.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual void updateCredential(CredentialId *credId, TString action, EntryTemplateVector *inputRequirementData);

//    /*
//     * Updates existing Credentials in batch with the supplied CredentialUpdateRequests,
//     * according to the requested action. 
//     * <p>
//     * An CredentialUpdateRequest is a comprised of:
//     * <ul>
//     * <li>A Credential identifier
//     * <li>A list of data collected based on the profile input requirements to use during the Credential update
//     * </ul>		</p>
//     * <p>
//     * <br><b>Note: </b><i> Due to this method's batch nature, it assumes an all or
//     * nothing (transactional) approach to Credential management. One failure
//     * means the entire transaction is failed, and rolled back. It is the
//     * provider's responsibility to enforce the contract.</i></p>
//     * @param action the update process requested for the specified Credential
//     * @param requests a list of CredentialUpdateRequests identifying the
//     * Credentials and input requirement data to use in the update
//     * @throws NoSuchCredentialException If the specified Credential is not
//     * found or valid
//     * @throws MalformedDataException If the input data was malformed.
//     * @throws InvalidStateException If Credential update violates the
//     * Credential's life cycle.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void updateCredentials(TString action, CredentialUpdateRequest[] requests) throws NoSuchCredentialException, MalformedDataException, InvalidStateException, SessionException, ManagementException, LocalizedRemoteException;

/*@internal
     * Returns the identifiers of all credentials that meet the supplied criteria. For example, this method might be used to  
     * retrieve all credentials in a certain state. 
     * @param criteria List of search criteria.
	 * <p>The CRITERIA_CRED_PARENT_WALLET criterion is mandatory.  </p>
	 * <ul>
	 * 		<li>For all credential provider-based, legacy PKI, and PIN credentials, the CRITERIA_CRED_APPLICATION_ID criterion is not mandatory.</li>
	 * 		<li>The CRITERIA_CRED_TYPE criterion is optional.</li>
	 * </ul>
	 * <em>Note:</em> <i>Weak or poor criteria selection in searches may result in inefficient searches and may not
	 * necessarily (in some cases not) mitigated by supplying a maxItems to the finder method.</i>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The identifiers of all matching credentials.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual CredentialIdVector *findCredentialIds(CriteriaVector *criteria, int maxItems);

//    /*
//     * Initializes a paged search of all credentials that meet the specified
//     * criteria.
//     * @param criteria List of search criteria.
//     * @return Handle to the dataset through which to page.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual TString findCredentialIdsInit(CriteriaVector *criteria) throws SessionException, ManagementException, LocalizedRemoteException;
//
//    /*
//     * Fetches/seeks through a previously initialized dataset produced by a
//     * credential search.
//     * @param searchHandle Handle to a dataset produced by a credential
//     * search.
//     * @param isFetchForward Specifies the fetch direction. Forward is true
//     * @param fetchSize Specifies  the number of entries to fetch.
//     * @return The identifiers of matching credential identifiers in the fetched page of the dataset
//     * @throws SearchEndException If the beginning or the end of the dataset is
//     * reached
//     * @throws NoSuchSearchException If the specified search handle is not valid.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual CredentialIdVector *findCredentialIdsFetch(TString searchHandle, boolean isFetchForward, int fetchSize) throws SearchEndException, NoSuchSearchException, SessionException, ManagementException, LocalizedRemoteException;
//
//    /*
//     * Terminates a previously initialized credential search.
//     * @param searchHandle Handle to a dataset produced by a credential
//     * search.
//     * @throws NoSuchSearchException If the specified search handle is not valid.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void findCredentialIdsDestroy(TString searchHandle) throws NoSuchSearchException, SessionException, ManagementException, LocalizedRemoteException;
//  
//    /*
//     * Returns the size of the data set found.
//     * @param searchHandle Handle to a dataset produced by a credential
//     * search
//     * @return The size of the data set found
//     * @throws NoSuchSearchException If the specified search handle is not valid.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual long findCredentialIdsSize(TString searchHandle) throws NoSuchSearchException, SessionException, ManagementException, LocalizedRemoteException;
//
//    /*
//     * Sets the current record index to which future fetch will be relative. If
//     * the specified index is out of range, the exceeded range limit will be used.
//     * <p>
//     * Less than 0 will be set to the first record. Greater than the item count
//     * will be set to the last record. {@link #findCredentialIdsSize(TString)} should be used
//     * to avoid providing an index greater than the last item.</p> 
//     * @param searchHandle Handle to a dataset produced by a credential
//     * search.
//     * @param index The index to which to seek.
//     * @throws NoSuchSearchException If the specified search handle is not valid.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void findCredentialIdsSetIndex(TString searchHandle, long index) throws NoSuchSearchException, SessionException, ManagementException, LocalizedRemoteException;
    
/*@internal
     * Returns the lifecycle status of the specified Credential.
     * @param credId The Credential identifier.
     * @return The lifecycle status of the specified Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual TString getLifecycleStatus(CredentialId *credId);

/*@internal
     * Returns the list of credential update action types supported for the
     * specified Credential.
     * @param credId The Credential for which to assess the available update actions.
     * @return  The list of update action types supported for the specified
     * Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     * @see #updateCredential(CredentialId, TString, Entry[])
     * @see #updateCredentials(TString, CredentialUpdateRequest[])
     */
    virtual TStringVector *getActionTypes(CredentialId *credId);
    
/*@internal
     * Returns the list of credential lifecycle process types supported for the
     * specified Credential.
     * @param credId The Credential for which to assess the available lifecycle
     * processes.
     * @return The list of process types supported for the specified Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     * @see #performProcess(CredentialId, TString, Parameter[])
     * @see #performProcesses(CredentialId[], TString, Parameter[])
     */
    virtual TStringVector *getProcessTypes(CredentialId *credId);

/*@internal
     * Returns the list of credential operation types supported for the
     * specified Credential.
     * @param credId The Credential for which to assess the available credential
     * operations
     * @return  The list of credential operation types supported for the specified
     * Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     * @see #operateCredential(CredentialId, TString, Parameter[])
     */
    virtual TStringVector *getOperationTypes(CredentialId *credId);

/*@internal
     * Imports a set of externally formed credentials.
     * @param walletId The wallet into which to import the Credential.
     * @param credentials The externally formed credentials.
     * @return Array of credential identifiers of the successfully imported
     * credentials.
     * @throws MalformedDataException If the credential data was malformed.
     * @throws NoSuchConfigException If the credential profile addressed by the
     * profileId attribute any of passed Credential instances is not found.
     * @throws NoSuchWalletException If the specified wallet is not found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual CredentialIdVector *importCredentials(WalletId *walletId, CredentialVector *credentials);
    
/*@internal
     * Returns the set of specified credentials.
     * @param credentialIds The credentials to retrieve.
     * @return The retrieved credentials.
     * @throws NoSuchCredentialException If one or more of the specified credentials is not found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual CredentialVector *getCredentials(CredentialIdVector *credentialIds);
    
/*@internal
     * Returns the identifiers of all credential profiles matching the supplied
     * criteria. For example, this method might be used to  find all credential profiles for a certain credential
     * type. 
     * @param criteria List of search criteria.
		 * <p>The following criteria are optional in all searches:
	 * <ul>
	 * 		<li>wallet.id</li>
	 * 		<li>credential.type</li>
	 * </ul>
	 * The valid values for the credential.type criterion are:
	 * <ul>
	 *		<li>CRED_ID_TYPE_SQ (security questions and answers)</li>
	 *		<li>CRED_ID_TYPE_PASSWORD (password)</li>
	 * </ul>
	 * If the credential.type criterion specifies any other value, a ManagementException is thrown.
	 * This protects you from getting profile IDs that cannot possibly be used for an import or update and
	 * ensures that the workflow is terminated as promptly as possible.</p>
	 * <em>Note: </em><i>Weak or poor criteria selection in searches may result in inefficient searches and may not
	 * necessarily (in some cases not) mitigated by supplying a maxItems to the finder method.</i>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The matching credential profile identifiers.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual ConfigurationIdVector *findCredentialProfileIds(CriteriaVector *criteria, int maxItems);

/*@internal
     * Requests the operation of a credential with the supplied parameters.
     * <p><b>Note: </b><i> This entry point may result in the need to retrieve a
     * ExternalOperation via
     * <b>{@link #getNextExternalOperations(TransactionId, ExternalOperationVector *)}</b>, externally
     * process it, and return it to the CredentialManager via
     * the same entry point.</i></p>
     * @param credId The Credential identifier.
     * @param operation The credential operation requested for the specified
     * credential.
     * @param parameters The input parameters required to perform the operation,
     * or an empty array.
     * @return The transaction identifier to facilitate <i>potential</i>
     * further steps required for Credential update.
     * @throws NoSuchCredentialException If the specified credential is not
     * found or valid.
     * @throws MalformedDataException If the input data was malformed.
     * @throws InvalidStateException If Credential update violates the
     * Credential's life cycle.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     * @see #getNextExternalOperations(TransactionId, ExternalOperationVector *)
     * @see #commitCredentialTransaction(TransactionId)
     */
    virtual TransactionId *operateCredential(CredentialId *credId, TString operation, ParameterVector *parameters);

};

} // namespace AI
namespace ActivIdentity = AI;
