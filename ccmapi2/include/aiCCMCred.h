/*.
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
// COMMON CLASS DEFINITIONS
//====================================================================================================

//====================================================================================================
// CREDENTIAL MANAGER DEFINITION
//====================================================================================================

	/**
	 * Provides access to the CMS credential management functionality.
	 * <p>The CredentialManager component makes it possible to:
 	 * <ul>
 	 * 	<li>Manage credentials</li>
 	 * 	<li>Suspend, revoke, or resume an application</li>
 	 * 	<li>Compute an unlock response</li>
 	 * 	<li>Get the initial PIN of a smart card</li>
	 * 	<li>Set the initial password for a smart card self-issuance</li>
	 * 	<li>Manage security questions and answers / Set the security answers</li>
	 * </ul></p>
	 * 
	 * <p><b>Note:</b><i>This API, when used in a remote client, requires appropriate authentication and
	 * authorization in order to be authorized to exercise any functionality.</i></p>
 	 * <p>Abstract Class, Pure-Virtual Interface.</p>
	 */
	class AICCMAPI_API CredentialManager : public SessionManager
	{
	public:

//    /*
//     * Deletes the specified Credential(s), completing/terminating their
//     * life cycles in batch.
//     * <p><b>Note: </b><i> Due to this method&apos;s batch nature, it assumes an all or
//     * nothing (transactional) approach to Credential management. One failure
//     * means the entire transaction is failed, and rolled back. It is the
//     * provider's responsibility to enforce the contract.</i></p>
//     * @param credIds List of Credential identifiers.
//     * @throws NoSuchCredentialException If one or more of the specified
//     * Credentials is not found.
//     * @throws InvalidStateException If Credential deletion violates the one or
//     * more of the Credentials' life cycles.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void deleteCredentials(CredentialIdVector *credIds) = 0;

    /**
     * Retrieves pending ExternalOperation(s) for the specified Credential
     * transaction where the resulting data from external processing will be
     * subsequently used to advance the Credential transaction. The pending
     * ExternalOperation(s) are formed in the process of credential usage
     * where processing involves multiple steps that require interaction with an
     * entity external to the CredentialManager.
     * <p>
     * Initial pending ExternalOperations can be queued/requested as a result
     * of calling one of the following entry points:
     * <ul>
     * <li>{@link AI::CredentialManager::operateCredential(CredentialId*, TString, ParameterVector*) operateCredential}</li>
     * </ul></p>
     * Once external processing is complete, the updated, processed external
     * operation(s) is/are returned to the CredentialManager to advance or
     * complete the workflow via this same entry point. The updated external
     * operation results may themselves cause additional pending external
     * operations to be queued/requested. The passed in completed External
     * Operation(s) are the results of processing ExternalOperation(s) returned
     * from a previous call to
     * {@link AI::CredentialManager::getNextExternalOperations(TransactionId*, ExternalOperationVector *)}.
     * @see AI::CredentialManager::operateCredential(CredentialId*, TString, ParameterVector*)    
     * @param txId Pointer to the Credential transaction identifier.
     * @param completedExternalOps Pointer to the previously completed ExternalOperations
     * populated with Credential Elements from each ExternalOperation's
     * execution.
     * @return Zero or more generated ExternalOperation(s) for the specified
     * transaction.
     * @throws NoSuchTransactionException If the specified transaction is not
     * found.
     * @throws MalformedDataException If the input data was malformed.
     * @throws InvalidStateException If Credential deletion violates the one or
     * more of the Credential's life cycles.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal provider failure occurs or if
     * the external processing was completed with an unrecoverable error.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual ExternalOperationVector *getNextExternalOperations(TransactionId *txId, ExternalOperationVector *completedExternalOps) = 0;

    /**
     * Commits a credential transaction to complete the creation/update of
     * the proto-credential/credential associated with the specified
     * transaction. The transaction is expected to be valid and all internal
     * workflow steps be completed, prior to calling this method.
     * @param txId Pointer to the identifier of the transaction within which the Credential
     * was created/updated.
     * @return List of one or more credential identifiers depending upon how the
     * transaction was initiated.
     * @throws NoSuchTransactionException If the transaction specified was
     * not found.
     * @throws TransactionIncompleteException If the transaction was not
     * completed.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual CredentialIdVector *commitCredentialTransaction(TransactionId *txId) = 0;

    /**
     * Returns the list of runtime input requirement aliases that must be resolved
     * for the specified CredentialProfile.
     * <p><b>Note: </b>This method supports the importing of security Q&A and updating of initial passwords and security Q&A credentials; it only supports 
     * profiles for credentials of types:
     * <ul>
     * 		<li><code>CRED_ELEMENT_TYPE_PASSWORD</code></li>
     *		<li><code>CRED_ELEMENT_TYPE_SQ</code></li>
     * </ul>
     * If the ConfigurationId passed in is not a legacy profile identifier, a ManagementException is thrown. This protects you 
     * from getting entries that cannot possibly be used and ensures that the workflow is terminated as promptly as possible.</p>
     * <p>For <code>CRED_ELEMENT_TYPE_PASSWORD</code> credential profile identifiers, a single EntryTemplate instance is fabricated from the card policy's PIN application. 
     * The profileId attribute of the Credential retrieved for the purposes of updating the initial password should be: <br>
     * <code>&lt;applicationSet&gt; || ".PIN"</code></p>
     * @param credProfileId The CredentialProfile to be examined.
     * @return The list of runtime input requirement aliases to be resolved.
     * @throws NoSuchConfigException If the credential profile addressed by the
     * profileId attribute any of passed Credential instances is not found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException Exception If remote connectivity fails.
     */
	virtual EntryTemplateVector *getProfileDynamicEntries(ConfigurationId *credProfileId) = 0;

    /**
	 * Returns the complete data dictionary of all the possible EntryTemplates
	 * that may appear in a CredentialProfileTemplate from the specified
	 * CredentialProvider. The data dictionary is returned as a
	 * CredentialProfileTemplate syntax which is a limited scope XML schema.
	 * <p>This method supports the importation of security Q&A and updating of initial passwords and security Q&A credentials. 
	 * It supports profiles for credentials of types:
	 * <ul>
	 * 		<li><code>CRED_ELEMENT_TYPE_PASSWORD</code><br>
	 * 		For <code>CRED_ELEMENT_TYPE_PASSWORD</code> credential profile identifiers, a single EntryTemplate instance is fabricated from the card policy's PIN application. 
	 * 		The profileId attribute of the Credential retrieved for the purposes of updating the initial password should be:<br>
	 *			<code>&lt;applicationSet&gt; || ".PIN"</code>
	 *		</li>
	 * 		<li><code>CRED_ELEMENT_TYPE_SQ</code></li>
	 * </ul></p>
     * @param credProfileId The CredentialProfile to be examined.
	 * @return The complete data dictionary, as a syntax, of all the possible
	 *         EntryTemplates for a CredentialProfileTemplate.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException Exception If remote connectivity fails.
     */
	virtual TString getProfileSyntax(ConfigurationId *credProfileId) = 0;
    
    /**
     * Performs a life cycle process for the specified Credential. The types of
     * processes are associated with the Credential's lifecycle rather than its
     * content.
     * <p><b>Example: </b><i>suspend, resume, or revoke.</i>
     * @param credId The Credential identifier.
     * @param process The process to perform.
     * @param parameters Pointer to the input parameters required to perform the process, or
     * an empty array.
     * @throws NoSuchCredentialException If the specified Credential is not found.
     * @throws InvalidStateException If the process requested is invalid for the
     * current state of the Credential.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual void performProcess(CredentialId *credId, TString process, ParameterVector *parameters) = 0;

//    /*
//     * Performs a life cycle process or initiates a workflow for the process for
//     * each of the specified Credentials in batch. The types of processes are associated
//     * with the Credentials' life cycle rather than its content.
//     * <p><b>Example: </b><i>suspend, resume, or revoke.</i></p>
//     * <b>Note: </b><i> Due to this method&apos;s batch nature, it assumes an all or
//     * nothing (transactional) approach to Credential management. One failure
//     * means the entire transaction is failed, and rolled back. It is the
//     * provider's responsibility to enforce the contract.</i>
//     * @param credIds List of Credential identifiers.
//     * @param process The process to perform.
//     * @param parameters Pointer to the input parameters required to perform the process, or
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

    /**
     * Deletes the specified Credential, completing/terminating its life cycle.
     * @param credId Pointer to the Credential identifier.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws InvalidStateException If Credential deletion violates the
     * Credential&apos;s life cycle.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual void deleteCredential(CredentialId *credId) = 0;

    /**
     * Updates an existing Credential with the supplied CredentialElements,
     * according to the requested action.
     * @param credId Pointer to the Credential identifier.
     * @param action The update process requested for the specified credential.
     * @param inputRequirementData Pointer to the list of data collected based on the profile
     * input requirements to use during the Credential update.
     * @throws NoSuchCredentialException If the specified credential is not
     * found or valid.
     * @throws MalformedDataException If the input data was malformed.
     * @throws InvalidStateException If Credential update violates the
     * Credential's life cycle.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual void updateCredential(CredentialId *credId, TString action, EntryTemplateVector *inputRequirementData) = 0;

//    /*
//     * Updates existing Credentials in batch with the supplied CredentialUpdateRequests,
//     * according to the requested action. 
//     * <p>
//     * An CredentialUpdateRequest is a comprised of:
//     * <ul>
//     * 	<li>A Credential identifier</li>
//     * 	<li>A list of data collected based on the profile input requirements to use during the Credential update</li>
//     * </ul></p>
//     * <b>Note: </b><i> Due to this method&apos;s batch nature, it assumes an all or
//     * nothing (transactional) approach to Credential management. One failure
//     * means the entire transaction is failed, and rolled back. It is the
//     * provider's responsibility to enforce the contract.</i>
//     * @param action The update process requested for the specified Credential.
//     * @param requests List of CredentialUpdateRequests identifying the
//     * Credentials and input requirement data to be used in the update.
//     * @throws NoSuchCredentialException If the specified Credential is not
//     * found or valid.
//     * @throws MalformedDataException If the input data was malformed.
//     * @throws InvalidStateException If Credential update violates the
//     * Credential's life cycle.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void updateCredentials(TString action, CredentialUpdateRequest[] requests) throws NoSuchCredentialException, MalformedDataException, InvalidStateException, SessionException, ManagementException, LocalizedRemoteException;

    /** 
     * Returns the identifiers of all credentials that meet the supplied criteria. 
     * This could be used, for example, to retrieve all credentials in a certain state. 
	 * @param criteria Pointer to the list of search criteria with which to filter.
	 * <p>The criteria that can be specified  are:</p>
     * <a name="criteriaTable"></a><TABLE BORDER="1">
     * <TR>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Criterion
     * </TH>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Criteria Comparison
     * </TH>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Possible Values
     * </TH>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Description
     * </TH>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_CRED_PARENT_WALLET</code>*
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * Any valid wallet ID
     * <TD ROWSPAN="1" COLSPAN="1">
     * The wallet to which the credential is bound. <b>(Required)</b>
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_CRED_APPLICATION_ID</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Any valid application set name
     * </TD>
     *  <TD ROWSPAN="1" COLSPAN="1">
     * Selects all credentials that are associated with a particular application set (aka CMS policy ID)
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_CRED_TYPE</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * The following credential types are supported:
     * <UL>
     * <li><code>CRED_ID_TYPE_PIN</code></li>
     * <li><code>CRED_ID_TYPE_SQ</code></li>
     * <li><code>CRED_ID_TYPE_PASSWORD</code></li>
     * <li><code>CRED_ID_TYPE_PKI</code></li>
     * </UL>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * The credential type.
     * </TD>
     * </TR>
	 * </TABLE>
	 * @param maxItems The maximum number of items to fetch if the search finds more than this number. 
	 * Set this parameter to 0 if there is no limit. 
     * @return The identifiers of all matching credentials.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException Exception If remote connectivity fails.
     */
	virtual CredentialIdVector *findCredentialIds(CriteriaVector *criteria, int maxItems) = 0;

//    /*
//     * Initializes a paged search of all credentials that meet the specified
//     * criteria.
//     * @param criteria A list of search criteria.
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
//     * @param searchHandle Handle to the dataset produced by a credential
//     * search
//     * @param isFetchForward  Specifies the fetch direction. Forward is true
//     * @param fetchSize   the number of entries to fetch.
//     * @return The identifiers of matching credentials in the fetched page of the dataset.
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
//     * @param searchHandle Handle to the dataset produced by a credential
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
//     * @param searchHandle Handle to the dataset produced by a credential
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
//     * to avoid providing an index greater than the last item. 
//     * @param searchHandle Handle to the dataset produced by a credential
//     * search
//     * @param index Index to which to seek.
//     * @throws NoSuchSearchException If the specified search handle is not valid.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void findCredentialIdsSetIndex(TString searchHandle, long index) throws NoSuchSearchException, SessionException, ManagementException, LocalizedRemoteException;
    
    /**
     * Returns the lifecycle status of the specified Credential.
     * @param credId Pointer to the Credential identifier.
     * @return The lifecycle status of the specified Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual TString getLifecycleStatus(CredentialId *credId) = 0;

    /**
     * Returns the list of credential update action types supported for the
     * specified Credential using the updateCredential() method.
     * @param credId The Credential for which to assess the available update actions.
	 * <p>If the credId attribute starts with legacy and the credential identifier type attribute equals password, this method returns 
	 * the identifier of the Credential whose available update action types were assessed.</p>
     * @return  The list of update action types supported for the specified
     * Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException Exception If remote connectivity fails.
     * @see #updateCredential(CredentialId, String, Entry[])
     */
    virtual TStringVector *getActionTypes(CredentialId *credId) = 0;
    
    /**
     * Returns the list of credential lifecycle process types supported for the
     * specified Credential.
     * @param credId Pointer to the identifier of the Credential for which to assess the available lifecycle
     * processes.
     * @return The list of process types supported for the specified Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     * @see #performProcess(CredentialId, TString, Parameter[])
     */
    virtual TStringVector *getProcessTypes(CredentialId *credId) = 0;

    /**
     * Returns the list of credential operation types supported for the
     * specified Credential.
     * @param credId Pointer to the identifier of the Credential for which to assess the available credential
     * operations.
     * @return The list of credential operation types supported for the specified
     * Credential.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     * @see #operateCredential(CredentialId, TString, Parameter[])
     */
    virtual TStringVector *getOperationTypes(CredentialId *credId) = 0;

    /**
     * Imports a set of externally formed credentials.
     * <p><b>Note: </b>The following prerequisites must be fulfilled in order to provision an initial password:
     * <ul>
     * <li>The device must be bound.</li>
     * <li>A pending request with the specified card policy must exist.</li> 
     * <li>The initial password must comply with the PIN policy specified.</li>
     * </ul></p>
     * @param walletId Pointer to the identifier of the wallet into which to import the credential.
     * @param credentials Pointer to the externally formed credentials.
     * @return Pointer to the array of credential identifiers of the successfully imported
     * credentials.
     * @throws MalformedDataException If the credential data was malformed.
     * @throws NoSuchConfigException If the credential profile addressed by the
     * profileId attribute any of passed Credential instances is not found.
     * @throws NoSuchWalletException If the specified wallet is not found. 
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual CredentialIdVector *importCredentials(WalletId *walletId, CredentialVector *credentials) = 0;
    
    /**
     * Returns the set of specified credentials.
     * @param credentialIds Pointer to the list of credentials to retrieve.
     * @return Pointer to the list of retrieved credentials.
     * @throws NoSuchCredentialException If one or more of the specified credentials is not found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual CredentialVector *getCredentials(CredentialIdVector *credentialIds) = 0;
    
    /** 
     * Returns the identifiers of all CredentialProfiles that meet the supplied
     * criteria. For example, this method can be used to find all credential profiles for a certain credential type. 
     * This method supports all types of credentials (both legacy and credential provider-based).
	 * <p><b>Notes: </b>
	 * <ul>
	 *	<li>Returns the identifiers of all credential profiles that meet the supplied criteria. For example, this method can be used to find all 
	 *   credential profiles for a certain credential type. This method supports all types of credentials (both legacy and credential provider based).</li>
	 *  <li>Make sure to set the criteria such that only profiles suitable for use with importCredentials and updateCredentials are returned. </li>
	 * </ul> </p>
	 * @param criteria Pointer to the list of search criteria with which to filter.
	 * <p>The criteria that can be specified  are:</p>
     * <a name="criteriaTable"></a><TABLE BORDER="1">
     * <TR>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Criterion
     * </TH>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Criteria Comparison
     * </TH>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Possible Values
     * </TH>
     * <TH ROWSPAN="1" COLSPAN="1">
     * Description
     * </TH>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_CRED_PARENT_WALLET</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * Any valid wallet ID
     * <TD ROWSPAN="1" COLSPAN="1">
     * The wallet to which the credential is bound.
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_CRED_TYPE</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * The following credential types are supported:
     * <UL>
     * <li><code>CRED_ID_TYPE_SQ</code></li>
     * <li><code>CRED_ID_TYPE_PASSWORD</code></li>
     * </UL>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * The credential type.
     * </TD>
     * </TR>
	 * </TABLE>
	 * @param maxItems The maximum number of items to fetch if the search finds more than this number. 
	 * Set this parameter to 0 if there is no limit. 
     * @return The matching CredentialProfile identifiers.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException Exception If remote connectivity fails.
     */
	virtual ConfigurationIdVector *findCredentialProfileIds(CriteriaVector *criteria, int maxItems) = 0;

    /**
     * Requests the operation of a credential with the supplied parameters.
     * <p><b>Note: </b><i> This entry point may result in the need to retrieve a
     * ExternalOperation using
     * {@link AI::CredentialManager::getNextExternalOperations(TransactionId*, ExternalOperationVector *)}, externally
     * process it, and return it to the CredentialManager using
     * the same entry point.</i></p>
     * @param credId Pointer to the Credential identifier.
     * @param operation The credential operation requested for the specified
     * Credential.
     * @param parameters Pointer to the input parameters required to perform the operation,
     * or an empty array.
     * @return Transaction identifier to facilitate <i>potential</i>
     * further steps required for Credential update.
     * @throws NoSuchCredentialException If the specified Credential is not
     * found or is not valid.
     * @throws MalformedDataException If the input data was malformed.
     * @throws InvalidStateException If Credential update violates the
     * Credential's life cycle.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     * @see AI::CredentialManager::getNextExternalOperations(TransactionId*, ExternalOperationVector *)
     * @see AI::CredentialManager::commitCredentialTransaction(TransactionId*)
     */
    virtual TransactionId *operateCredential(CredentialId *credId, TString operation, ParameterVector *parameters) = 0;
};

//====================================================================================================
// FACTORY CLASS DEFINITION
//====================================================================================================

	/**
	* Factory that provides a handle to a CredentialManager instance.
	* <p>
	* The CredentialManager factory configuration parameters are either of the following values:
	* <ul>
	*	<li>CONFIG_URL -- The complete URL of the CredentialManager service in CMS. </li>
	* </ul>
	* or both of the following values:
	* <ul>
	*	<li>CONFIG_HOST_NAME -- The hostname of the CredentialManager service in CMS. </li>
	*	<li>CONFIG_HOST_PORT -- The port number of the CredentialManager service in CMS.   </li>
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
	class AICCMAPI_API CredentialManagerFactory : public ManagerFactory
	{
	public:
		/**
		 * Returns the CredentialManager based on the underlying provider.
		 * Uses the default factoryDefinition and default ProviderName.
		 * @param parameters Pointer to the configuration parameters used at initialization.
		 * <p>
		 * The CredentialManager factory configuration parameters are either of the following values:
		 * <ul>
		 *	<li>CONFIG_URL -- The complete URL of the CredentialManager service in CMS. </li>
		 * </ul>
		 * or both of the following values:
		 * <ul>
		 *	<li>CONFIG_HOST_NAME -- The hostname of the CredentialManager service in CMS. </li>
		 *	<li>CONFIG_HOST_PORT -- The port number of the CredentialManager service in CMS.   </li>
		 * </ul>
		 * ...and in addition, all of the following values:
		 *  <ul>
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
		 * @return CredentialManager Pointer to the created manager instance.
		 * @throws FactoryException If initialization fails.
		 */
		static CredentialManager* newInstance(ParameterVector* parameters);

		/**
		 * Returns the CredentialManager based on the underlying provider.
		 * Uses the default factoryDefinition.
		 * @param providerName Provider identifier.
		 * @param parameters Pointer to the configuration parameters used at initialization.
		 * <p>
		 * The CredentialManager factory configuration parameters are either of the following values:
		 * <ul>
		 *	<li>CONFIG_URL -- The complete URL of the CredentialManager service in CMS. </li>
		 * </ul>
		 * or both of the following values:
		 * <ul>
		 *	<li>CONFIG_HOST_NAME -- The hostname of the CredentialManager service in CMS. </li>
		 *	<li>CONFIG_HOST_PORT -- The port number of the CredentialManager service in CMS.   </li>
		 * </ul>
		 * ...and in addition, all of the following values:
		 *  <ul>
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
		 * @return CredentialManager Pointer to the created manager instance.
		 * @throws FactoryException If initialization fails.
		 */
		static CredentialManager* newInstance(TString providerName, ParameterVector* parameters);

		/**
		 * Returns the CredentialManager based on the underlying provider.
		 * @param factoryDefinition Property file that contains provider information.
		 * @param providerName Provider identifier.
		 * @param parameters Pointer to the configuration parameters used at initialization.
		 * <p>
		 * The CredentialManager factory configuration parameters are either of the following values:
		 * <ul>
		 *	<li>CONFIG_URL -- The complete URL of the CredentialManager service in CMS. </li>
		 * </ul>
		 * or both of the following values:
		 * <ul>
		 *	<li>CONFIG_HOST_NAME -- The hostname of the CredentialManager service in CMS. </li>
		 *	<li>CONFIG_HOST_PORT -- The port number of the CredentialManager service in CMS.   </li>
		 * </ul>
		 * ...and in addition, all of the following values:
		 *  <ul>
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
		 * @return CredentialManager Pointer to the created manager instance.
		 * @throws FactoryException If initialization fails.
		 */
		static CredentialManager* newInstance(TString factoryDefinition, TString providerName, ParameterVector* parameters);

	protected:
		static CredentialManager* init(TString factoryDefinition, TString providerName, ParameterVector* parameters);
	};

//====================================================================================================
// EXCEPTION CLASS DEFINITIONS
//====================================================================================================

/**
 * This exception is thrown when the requested entity is not found. 
 */
class AICCMAPI_API NoSuchCredentialException: public LocalizedException
{
	CredentialId	*credId;

public:
	/**
     * Default Constructor.
     */
	NoSuchCredentialException(void){ credId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchCredentialException(const LocalizedException &ex): LocalizedException(ex){ credId=NULL; };

	/**
     * Destructor.
     */
	virtual ~NoSuchCredentialException(void);

	/**
     * Returns the unique identifier of the Credential.
     * @return The unique of the Credential.
     */
	virtual	CredentialId *getCredentialId(void);

	/**
     * Sets the unique identifier of the Credential.
     * @param credId The unique identifier of the Credential.
	 */
	virtual	void setCredentialId(CredentialId *credId);
};

}





