/*
 * (c) Copyright 2006 ActivIdentity
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
#include "aiCCMUtil.h"
#include "aiCCMUtilImpl.h"
#include "aiCCMWallet.h"

#ifdef AICCMWALLET_EXPORTS
#define AICCMWALLET_API __declspec(dllexport)
#define EXPWALLET_TEMPLATE
#else
#define AICCMWALLET_API __declspec(dllimport)
#define EXPWALLET_TEMPLATE extern
#endif

class	walletSoapBinding;

namespace AI
{

/**
 * @internal
 * Provides access to wallet management functionality.
 */
class AICCMWALLET_API WalletManagerImpl : public DLL_OBJECT(WalletManager), public SessionManagerImpl
{
	walletSoapBinding	*walletProxy;

public:
    /*@internal
     * Constructor.
     * @param URL URL of the WalletManagerImpl web service. The URL should be of the form: 
	 * <pre>
     *	http://<host>:<port>/aims/services/wallet.
	 * </pre>.
     */
	WalletManagerImpl();
	WalletManagerImpl(TString URL);
	WalletManagerImpl(TString URL, ParameterVector* INITPARAM);

    /*@internal
     * Destructor.
     */
	virtual ~WalletManagerImpl(void);

	/*@internal
	 * Initializes a WalletManagerImpl instance.
	 */
	virtual Manager* init(ParameterVector* parameters);

    /*@internal
     * Opens a new session for this API. Must be the first call made.  All the subsequent calls must occur in
     * the scope of a valid session.
     * @param parameters Reserved for future use. 
     * @return The identifier of the session opened by the server.
     * @throws SessionException If there is no valid session (session not opened or timed out).
     * @throws LocalizedRemoteException If there is a communication error.
     */
	virtual TString	openSession(ParameterVector *parameters);

    /*@internal
     * Returns true if a session is currently open, otherwise returns false.
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
     * Binds the specified security module to the specified wallet.
     * @param walletId The wallet identifier.
     * @param smId The identifier of a security module to bind.
     * @throws NoSuchWalletException If the wallet identifier specified does not
     * identify a valid wallet.
     * @throws NoSuchSecurityModuleException If the security module identifier 
     * specified does not identify a valid security module.
     * @throws SecurityModuleBoundException If the security module identifier specified
     * is already associated with this module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal implementation level failure occurred.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void bindSecurityModule(WalletId *walletId, SecurityModuleId *smId);

    /*@internal
     * Unbinds the specified security module from the specified wallet.
     * @param walletId The wallet identifier.
     * @param smId identifier of a security module to unbind.
     * @throws NoSuchWalletException If the wallet identifier specified does not identify a valid wallet
     * @throws NoSuchSecurityModuleException If the security module identifier
     * specified does not identify a valid security module.
     * @throws SecurityModuleNotBoundException If the security module identifier specified
     * is not associated with this module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal implementation level failure occurred.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void unbindSecurityModule(WalletId *walletId, SecurityModuleId *smId);

    /*@internal
     * Attempts to submit the specified list of actions for later synchronization.
     * The order of the actions is significant. If an action is rejected during
     * validation <b>no operations will be executed</b>. If a failure occurs
     * executing a valid action, processing will stop at that point.
     * @return List of wallet action identifiers for the actions created.
     * @param walletId identifier of the wallet on which to submit the action.
     * @param actions List of actions to perform.
     * @param owner The owner of the set of actions. This value is used to
     * restrict which operator may later manage these action(s).
     * @throws NoSuchWalletException If the wallet identifier 
     * specified does not identify a valid wallet.
     * @throws NoSuchSecurityModuleException If one or more of the action'(s)
     * specified security module(s) were not found.
     * @throws SecurityModuleNotBoundException If one or more of the action'(s)
     * specified security module(s) not bound to the specified wallet.
     * @throws InvalidActionException If either the enrollment data or
     * parameters of an action was not valid.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual TStringVector	*submitActions(WalletId *walletId, ActionVector *actions, TString owner);

    /*@internal
     * Returns the wallet bound to the specified user.
     * @param userId Unique identifier of the user.
     * @return The wallet bound to the specified user
     * @throws NoSuchUserException If the user identifier specified does not identify a valid user.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual WalletId		*getBoundWalletFromUser(UserId *userId);

    /*@internal
     * Returns the identifier of the wallet that the security module is bound to.
     * Wallet:SecurityModule is 1:n
     * @param smId The security module identifier.
     * @return The identifier of the wallet that contains the security module.
     * @throws NoSuchSecurityModuleException If the security module identifier 
     * specified does not identify a valid security module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual WalletId		*getBoundWalletFromSM(SecurityModuleId *smId);

    /*@internal
     * Triggers the indicated workflow processing for the specified wallet.
     * @param walletId The wallet to process.
     * @param process The workflow process to perform.
     * @param parameters The input parameters required to perform the process,
     * or an empty array.
     * @throws NoSuchWalletException If the wallet does not exist.
     * @throws InvalidStateException If the wallet is in a state not compatible
     * with the requested process.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void			performProcess(WalletId *walletId, TString process, ParameterVector *parameters);

    /*@internal
     * Triggers the indicated workflow processing for the specified wallet
     * action.
     * @param actionId The identifier of the wallet action.
     * @param process The workflow process to perform.
     * @param parameters The input parameters required to perform the process,
     * or an empty array.
     * @throws NoSuchActionException If one or more of the specified actions was
     * not found.
     * @throws InvalidStateException If the status is not valid for
     * one or more of the wallet actions.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void			performActionProcess(TString actionId, TString process, ParameterVector *parameters);

    /*@internal
     * Returns the identifiers of all wallet actions that meet the supplied criteria. For example, this method might be used to  
     * retrieve all pending actions for a specified wallet. 
     * @param criteria List of search criteria.
	  * <p>Criteria may be as follows:</p>
	 *<TABLE BORDER="1">
<TR>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Key</TH>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Comparison</TH>
<TH ROWSPAN="1" COLSPAN="1">
Possible Values and/or Type
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Description 
</TH>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_STATUS</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
<br><code>
CRITERIA_COMPARISON_NOT_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
The action status may be:
<UL>
<li>ACTION_STATUS_REQUESTED</li>
<li>ACTION_STATUS_PENDING</li>
<li>ACTION_STATUS_DENIED</li>
<li>ACTION_STATUS_FAILED</li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects action(s) in a particular state.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_PARENT_WALLET</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Any valid wallet ID
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects actions by the id of the wallet for which the actions were submitted.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_TYPE</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
The following action types are supported:
<UL>
<li>			  <code>
ACTION_TYPE_ISSUANCE</code>
</li>
<li>
<code>
ACTION_TYPE_PRODUCE</code>
</li>
<li>
<code>
ACTION_TYPE_RECYCLE</code>
</li>
<li>
<code>
ACTION_TYPE_POSTISSUANCE</code>
</li>
<li>
<code>
ACTION_TYPE_PINUNLOCK</code>
</li>
<li>
<code>
ACTION_TYPE_CARDLOCK</code>
</li>
<li><code>
ACTION_TYPE_CARDUNLOCK</code>
</li></ul>
For more information, see <code>
Action.</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects actions of a particular type or all types.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_APPLICATION_SET</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Any valid application set name.
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects actions associated with a particular application set (aka CMS card policy ID).
</TD>
</TR>
</TABLE>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The identifiers of all matching wallet actions.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual TStringVector	*findActionIds(CriteriaVector *criteria, int maxItems);

    /*@internal
     * Returns the actions that meet the supplied criteria. For example, this method might be used to  retrieve all 
     * actions that are pending, of type issuance, and with the CMS card policy PIV.
     * @param criteria List of search criteria.
		 * <p>Criteria may be as follows:</p>
	 <TABLE BORDER="1">
<TR>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Key</TH>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Comparison
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Possible Values and/or Type
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Description 
</TH>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ACTION_STATUS</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
<br><code>
CRITERIA_COMPARISON_NOT_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
The action status may be:
<UL>
<li>
ACTION_STATUS_REQUESTED</LI>
<li>
ACTION_STATUS_PENDING</LI>
<li>
ACTION_STATUS_DENIED</LI>
<li>
ACTION_STATUS_FAILED</LI>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects action(s) in a particular state.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_PARENT_WALLET</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Any valid wallet ID
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects actions by the id of the wallet for which the actions were submitted.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_TYPE</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
The following action types are supported:
<UL>
<li>
<code>
ACTION_TYPE_ISSUANCE</code>
</LI>
<li>
<code>
ACTION_TYPE_PRODUCE</code>
</LI>
<li>
<code>
ACTION_TYPE_RECYCLE</code>
</LI>
<li>
<code>
ACTION_TYPE_POSTISSUANCE</code>
</LI>
<li>
<code>
ACTION_TYPE_PINUNLOCK</code>
</LI>
<li>
<code>
ACTION_TYPE_CARDLOCK</code>
</LI>
<li><code>
ACTION_TYPE_CARDUNLOCK</code>
</li>
</ul>
For more information, see <code>
Action.</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects actions of a particular type or all types.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_APPLICATION_SET</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Any valid application set name.
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects actions associated with a particular application set (aka CMS card policy ID).
</TD>
</TR>
</TABLE>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The actions of all matching wallet actions.
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual ActionVector	*findActions(CriteriaVector *criteria, int maxItems);
    
    /*@internal
     * Returns the actions available to be called on the wallet.
     * @param walletId The wallet on which to submit the action.
     * @return List of wallet actions that may be submitted on the specified wallet in its current state.
     * @throws NoSuchWalletException If the wallet identifier specified does not identify a valid wallet.
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual ActionVector	*getAvailableActions(WalletId *walletId);

    /*@internal
     * Returns the application sets that meet the supplied criteria. 
     * For example, this method might be used to  retrieve all application sets allowed for an Action type of PRODUCE.
     * @param criteria List of search criteria.
		 * <p>Criteria may be as follows:</p>
	 *<TABLE BORDER="1">
<TR>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Key
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Comparison
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Possible Values and/or Type
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Description 
</TH>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_APPLICATION_SET</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
<br><code>
CRITERIA_COMPARISON_NOT_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Any application set name.
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Use equality to test the existence of the application set, or inequality to retrieve all possible alternatives.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ACTION_REASON</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
The <code>
Action reason</code> attribute can take the following values:
<UL>
<li>
<code>
ACTION_REASON_NONE </code>
</LI>
</UL>
For initial issuance
<UL>
<li>
<code>
ACTION_REASON_FORGOTTEN</code>
 </LI>
</UL>
For temporary replacement
<UL>
<li>
<code>
ACTION_REASON_LOST</code>
, <code>
ACTION_REASON_STOLEN</code>
, or <code>
ACTION_REASON_DAMAGED</code>
 (Damage replacement)</LI>
</UL>
For permanent replacement
<UL>
<li>
<code>
ACTION_REASON_RENEW </code>
</LI>
<li>Re-issuance</li>
</ul>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Action Reason mapping:
In CMS, there is an &quot;enrollment policy&quot; that is used to map the card policy assignments with an organizational group in the directory. 
In addition, there is another mapping with the card policy issuance types (Initial Issuance, Lost/Stolen, Forgotten, Damaged) for each card policy. 
<p>The action.reason mapping is as follows:</p>
<code>
NONE</code>
 = &quot;Initial Issuance&quot;

<br><code>
FORGOTTEN</code>
 = &quot;Forgotten&quot;

<br><code>
LOST</code>
 = &quot;Lost/Stolen&quot;

<br><code>
STOLEN</code>
 = &quot;Lost/Stolen&quot;

<br><code>
DAMAGED</code>
 = &quot;Damaged&quot; <code>
<br><code>RENEW</code>
 = &quot;Initial Issuance&quot;
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ID_WALLETID</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Any valid wallet ID.
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects application sets with the specified wallet id.
</TD>
</TR>
</TABLE>

     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The application sets that meet the criteria.
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual TStringVector	*findApplicationSets(CriteriaVector *criteria, int maxItems);

	/*@internal
     * Returns the wallet identifiers that meet the supplied criteria.
     * If no criteria are supplied, all wallet identifiers will be returned where the following conditions are met:
	 * <ul>
	 * <li>security module(s) is/are bound or</li>
     * <li>issuance action(s) is/are pending</li>
	 * </ul>
     * @param criteria List of search criteria.
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The wallet identifiers of all matching wallets.
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
	 */
	virtual WalletIdVector	*findWalletIds(CriteriaVector* criteria, int maxItems);

    /*@internal
     * Initializes a paged search of all wallets that meet the specified criteria.
     * @param criteria List of search criteria. 
     * @param fetchSize Specifies  the number of entries to fetch. 
     * @return Handle to the dataset through which to page. 
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual SearchSessionId *findWalletIdsInit(CriteriaVector* criteria, int fetchSize);
    
    /*@internal
     * Fetches/seeks through a previously initialized dataset produced by a wallet search.
     * @param searchHandle Handle to a dataset produced by a wallet search.
     * @param isFetchForward Specifies the fetch direction. Forward is true. 
     * @return The identifiers of matching wallet identifiers in the fetched page of the dataset.
     * @throws SearchEndException If the beginning or the end of the dataset is reached.
     * @throws NoSuchSearchException If the specified search handle is not valid. 
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual WalletIdVector* findWalletIdsFetch(SearchSessionId* searchHandle, bool isFetchForward);    
    
//    /*@internal
//     * Returns the size of the data set found.
//     * @param searchHandle Handle to the dataset produced by a wallet search.
//     * @return The size of the data set found.
//     * @throws NoSuchSearchException If the specified search handle is not valid. 
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual long findWalletIdsSize(SearchSessionId* searchHandle);
//    
//    /*@internal
//     * Sets the current record index to which future fetch will be relative. 
//     * If the specified index is out of range, the exceeded range limit will be 
//     * used. 
//     * If the index is less than 0, the current record index is set to the first record. Greater than the item count 
//     * will be set to the last record. findWalletIdsSize(String) should be used 
//     * to avoid providing an index greater than the last item. 
//     * @param searchHandle Handle to a dataset produced by a wallet search.
//     * @param index The index to which to seek.
//     * @throws NoSuchSearchException If the specified search handle is not valid. 
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws LocalizedRemoteException If remote connectivity fails.
//     */
//    virtual void findWalletIdsSetIndex(SearchSessionId* searchHandle, long index);

    /*@internal
     * Terminates a previously initialized wallet search. 
     * @param searchHandle Handle to a dataset produced by a wallet search.
     * @throws NoSuchSearchException If the specified search handle is not valid. 
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual void findWalletIdsDestroy(SearchSessionId* searchHandle);
};

} // namespace AI
namespace ActivIdentity = AI;
