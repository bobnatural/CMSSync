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
 * An action to be performed on a wallet. High level actions can be
 * performed against a wallet and indirectly against the security modules
 * bound to it.
 * <p>CCM API Actions make it possible to programmatically perform CMS tasks (actions)
 * that are otherwise performed using the Operator Portal. </p>
 * <b>Note:</b> <i>Actions are not synchronous; they are not executed immediately when
 * submitted but are instead scheduled to be executed when a device synchronization
 * operation occurs after the action is scheduled.</i>
 */
class AICCMAPI_API Action : public DLL_OBJECT_BASE
{
	TString					id;
	TString					type;
	TString					description;
	TString					owner;
	TString					status;
	__time64_t				createDate;
	TString					lastModifier;
	__time64_t				lastModifiedDate;
	ParameterVector			*runtimeParameters;
	SecurityModuleIdVector	*securityModuleIds;
	int						expirationPeriod;
	TString					applicationSet;
	WalletId				*walletId;

public:
    /**
     * Constructor.
     * @param applicationSet The application set identifier.
     * @param type The type of the new action.
     * @param description The description of the new action.
     */
	Action(TString applicationSet, TString type, TString description = _T(""));

    /**
     * Constructor.
     */
	Action(void);

    /**
     * Copy Constructor.
	 * @param src Source Action object.  A copy of the content of this object is used for new content.
     */
	Action(const Action &src);

	/**
	 * Assignment Operator overload.
	 * @param src Source Action object.  A copy of the content of this object is used for new content.
	 */
	virtual Action& operator=(const Action& src);

    /**
     * Destructor.
     */
	virtual ~Action(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual Action* clone() const	{ return new Action(*this); }

    /**
     * Returns the unique identifier of the action.
     * @return The unique identifier of the action.
     */
	virtual	TString			getId(void);

    /**
     * Returns a pointer to the identifier of the wallet with which the action is associated.
     * @return A pointer to the identifier of the wallet with which the action is associated.
     */
	virtual	WalletId*		getWalletId(void);

    /**
     * Returns the type of the action.
     * @return The type of the action.
     */
	virtual	TString			getType(void);

    /**
     * Returns the description of the action.
     * @return The description of the action.
     */
	virtual	TString			getDescription(void);

    /**
     * Returns the identifier of the owner/creator of this action.
     * @return The identifier of the owner/creator of this action.
     */
	virtual	TString			getOwner(void);

    /**
     * Returns the current status of the action.
     * @return The current status of the action.
     */
	virtual	TString			getStatus(void);

	/**
     * Returns the date of creation.
     * @return The date of creation.
     */
	virtual	__time64_t					*getCreateDate(void);

    /**
     * Returns the last operator to modify the action.
     * @return The last operator to modify the action.
     */
	virtual	TString			getLastModifier(void);

	/**
     * Returns the last date of modification.
     * @return The last date of modification.
     */
	virtual	__time64_t					*getLastModifiedDate(void);

    /**
     * Returns current runtime parameters.
     * @return current runtime parameters.
     */
	virtual	ParameterVector			*getRuntimeParameters(void);

    /**
     * Returns the security modules that the wallet action is to be applied
     * against. Otherwise returns null.
     * @return The security modules against which the wallet action is to be applied.
     */
	virtual	SecurityModuleIdVector	*getSecurityModuleIds(void);

    /**
     * Returns the expiration period for an action request.
     * @return The expiration period in hours.
     */
	virtual	int						getExpirationPeriod(void);

    /**
     * <b>Deprecated</b> Returns the application identifier for the action.
     * @return The application identifier for the action.
	 * @deprecated Use {@link AI::Action::getApplicationSet()}
     */
	virtual	TString			getApplicationId(void);

    /**
     * Returns the application set identifer associated with this action.
     * An application set identifer specifies  the group of applications to be
     * affected by this action. This identifier is rendered in the CMS Portal
     * as the card policy.
     * @return The application set identifer for the action.
     */
	virtual	TString			getApplicationSet(void);

    /**
     * Sets the unique identifier of the action.
     * @param id The unique identifier of the action.
     */
	virtual	void					setId(TString id);

    /**
     * Sets the wallet identifier that the action is associated with.
     * @param walletId Pointer to the identifier of the wallet with which the action is associated.
     */
	virtual	void					setWalletId(WalletId *walletId);

    /**
     * Sets the type of the action.
     * @param type The type of the action.
     */
	virtual	void					setType(TString type);

	/**
     * Sets the description of the action.
     * @param description Description of the action.
     */
	virtual	void					setDescription(TString description);

    /**
     * Sets the owner/creator of this action.
     * @param owner Owner/creator of this action.
     */
	virtual	void					setOwner(TString owner);

	/**
     * Sets the current status of the action.
     * @param status The current status of the action.
     */
	virtual	void					setStatus(TString status);

	/**
     * Sets the date of creation.
     * @param createDate Pointer to the date of creation.
     */
	virtual	void					setCreateDate(__time64_t *createDate);

    /**
     * Sets the last operator to modify the action.
     * @param lastModifier The last operator to modify the action.
     */
	virtual	void					setLastModifier(TString lastModifier);

    /**
     * Sets the date of last modification.
     * @param lastModifiedDate Pointer to the date of last modification.
     */
	virtual	void					setLastModifiedDate(__time64_t *lastModifiedDate);

    /**
     * Sets the runtime parameters for the action.
     * @param runtimeParameters Pointer to the runtime parameters for the action.
     */
	virtual	void					setRuntimeParameters(ParameterVector *runtimeParameters);

    /**
     * Allows runtime parameters to be added to the action for execution. If
     * there are already runtime parameters set, additional runtime parameters
     * may still be added. If runtime parameter keys overlap, previous values
     * will be overwritten.
     * @param runtimeParameters Pointer to the runtime parameters to add.
     */
	virtual	void					addRuntimeParameters(ParameterVector *runtimeParameters);

    /**
     * Sets the security modules that the wallet action is to be applied
     * against. If this method is not called then the wallet action is
     * applied by default against one or more of the wallet's security modules
     * as determined internally.
     * @param smIds Pointer to the set of security module identifiers.
     */
	virtual	void					setSecurityModuleIds(SecurityModuleIdVector *smIds);

    /**
     * Sets the expiration period for an action request.
     * @param expirationPeriod Period in hours.
     */
	virtual	void					setExpirationPeriod(int expirationPeriod);

    /**
     * <b>Deprecated</b> Sets the application identifier for the action.
     * @param applicationId The application identifier for the action.
	 * @deprecated Use {@link AI::Action::setApplicationSet(TString)}
     */
	virtual	void					setApplicationId(TString applicationId);

    /**
     * Sets the application set identifer associated with this action.
     * An application set identifer specifies  the group of applications to be
     * affected by this action. This identifier is rendered in the CMS Portal
     * as the card policy. Returns the application set identifer for the action.
     * @param applicationSet The application set name associated with this action.
     */
	virtual	void					setApplicationSet(TString applicationSet);
};

/**
 * A list of actions.
 */
EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<Action>;
typedef Vector<Action>	ActionVector;

//====================================================================================================
// WALLET MANAGER DEFINITION
//====================================================================================================

	/**
	 * The WalletManager makes it possible to assign SecurityModules to card holders (Users) and
	 * to submit actions to be executed against a user&apos;s security modules. The wallet management
	 * process is asynchronous; first, a set of actions is scheduled for execution on a security module;
	 * second, the SyncManager is used to update the security module according to the actions
	 * previously submitted.
	 *
	 * <p><b>Note:</b><i>This API, when used in a remote client, requires appropriate authentication and
	 * authorization in order to be authorized to exercise any functionality.</i></p>
 	 * <p>Abstract Class, Pure-Virtual Interface.</p>
	 */
	class AICCMAPI_API WalletManager : public SessionManager
	{
	public:

    /**
     * Binds the specified security module to the specified wallet.
     * @param walletId Pointer to the wallet identifier.
     * @param smId Pointer to the identifier a security module to bind.
     * @throws NoSuchWalletException If the wallet identifier specified does not
     * identify a valid wallet.
     * @throws NoSuchSecurityModuleException If the security module identifier
     * specified does not identify a valid security module.
     * @throws SecurityModuleBoundException If the security module identifier specified
     * is already associated with this module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal implementation level failure occurred.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual void			bindSecurityModule(WalletId *walletId, SecurityModuleId *smId) = 0;

    /**
     * Unbinds the specified security module from the specified wallet.
     * @param walletId Pointer to the wallet identifier.
     * @param smId Pointer to the identifier of a security module to unbind.
     * @throws NoSuchWalletException If the wallet identifier specified does not identify a valid wallet.
     * @throws NoSuchSecurityModuleException If the security module identifier
     * specified does not identify a valid security module.
     * @throws SecurityModuleNotBoundException If the security module identifier specified
     * is not associated to this module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal implementation level failure occurred.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual void			unbindSecurityModule(WalletId *walletId, SecurityModuleId *smId) = 0;

    /**
     * Attempts to submit the specified list of actions for later synchronization.
     * The order of the actions is significant. If an action is rejected during
     * validation <b>no operations will be executed</b>. If a failure occurs
     * executing a valid action, processing will stop at that point.
     * @return List of wallet action identifiers for the actions created.
     * @param walletId Pointer to the wallet on which to submit the action.
     * @param actions Pointer to the list of actions to perform.
     * @param owner The owner of the set of actions. This value is used to
     * restrict which operator may later manage these action(s).
     * @throws NoSuchWalletException If the wallet identifier
     * specified does not identify a valid wallet.
     * @throws NoSuchSecurityModuleException If one or more of the action(s)
     * specified security module(s) that were not found.
     * @throws SecurityModuleNotBoundException If one or more of the action(s)
     * specified security module(s) not bound to the specified wallet.
     * @throws InvalidActionException If either the enrollment data or
     * parameters of an action was not valid.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual TStringVector	*submitActions(WalletId *walletId, ActionVector *actions, TString owner) = 0;

    /**
     * Returns the wallet bound to the specified user.
     * @param userId Pointer to the unique identifier of the user.
     * @return Pointer to the identifier of the wallet bound to the specified user.
     * @throws NoSuchUserException If the user identifier specified does not identify a valid user.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual WalletId		*getBoundWalletFromUser(UserId *userId) = 0;

    /**
     * Returns the identifier of the wallet that the security module is bound.
     * Wallet:SecurityModule is 1:n
     * @param smId Pointer to the security module identifier.
     * @return Pointer to the identifier of the wallet that contains the security module.
     * @throws NoSuchSecurityModuleException If the security module identifier
     * specified does not identify a valid security module.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual WalletId		*getBoundWalletFromSM(SecurityModuleId *smId) = 0;

    /**
     * Triggers the indicated workflow processing for the specified wallet.
     * @param walletId Pointer to the wallet to process.
     * @param process The workflow process to perform.
     * @param parameters Pointer to the input parameters required to perform the process
     * or an empty array.
     * @throws NoSuchWalletException If the wallet does not exist.
     * @throws InvalidStateException If the wallet is in a state not compatible
     * with the requested process.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual void			performProcess(WalletId *walletId, TString process, ParameterVector *parameters) = 0;

    /**
     * Triggers the indicated workflow processing for the specified wallet
     * action.
     * <p>
     * <b>Notes:</b>
     * <ul>
	 *		<li>Parameter <code>ACTION_RUNTIME_PARAM_POLICY</code> may be given with process <code>ACTION_PROCESS_APPROVE</code> to define applicationSet for the given actionId.</li>
     * </ul>
     * </p> 
     * @param actionId The identifier of the wallet action.
     * @param process The workflow process to be performed.
     * <p>Process may be as follows:</p>
     * <ul>
     * <li><code>ACTION_PROCESS_CANCEL</code><br>Cancels a pending action and results in the action being remove from the system.</LI>
     * <li><code>ACTION_PROCESS_APPROVE</code><br>Approves a pending action and results in the action being in an approved lifecycle state.</LI>
     * <li><code>ACTION_PROCESS_DENY</code><br>Denies a pending action and results in the action being in a denied lifecycle state.</LI>
     * <li><code>ACTION_PROCESS_RESET</code><br>Resets a failed request back to a pending lifecycle state.</LI>
     * </ul>
     * @param parameters Pointer to the input parameters required to perform the process,
     * or an empty array.
     * @throws NoSuchActionException If one or more of the specified actions was
     * not found.
     * @throws InvalidStateException If the status is not valid for
     * one or more of the wallet actions.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual void			performActionProcess(TString actionId, TString process, ParameterVector *parameters) = 0;

    /**
     * Returns the actions that meet the supplied criteria. For example, retrieve all
     * actions that are pending, of type issuance, and with the CMS card policy PIV.
     * @param criteria Pointer to the list of search criteria.
	 * <p>Criteria may be as follows:</p>
	 * <TABLE BORDER="1">
	 * <TR>
	 * <TH ROWSPAN="1" COLSPAN="1">Criteria Key</TH>
	 * <TH ROWSPAN="1" COLSPAN="1">Criteria Comparison</TH>
	 * <TH ROWSPAN="1" COLSPAN="1">Possible Values and/or Type</TH>
	 * <TH ROWSPAN="1" COLSPAN="1">Description</TH>
	 * </TR>
	 * <TR>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_STATUS</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code><br>
	 * 	 <code>CRITERIA_COMPARISON_NOT_EQUAL</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1">The action status may be:
	 * <UL>
	 * <li><code>ACTION_STATUS_REQUESTED</code></li>
	 * <li><code>ACTION_STATUS_PENDING</code></li>
	 * <li><code>ACTION_STATUS_DENIED</code></li>
	 * <li><code>ACTION_STATUS_FAILED</code></li>
	 * </UL>
	 * </TD>
	 * <TD ROWSPAN="1" COLSPAN="1">Selects action(s) in a particular state.</TD>
	 * </TR>
	 * <TR>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_PARENT_WALLET</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1">Any valid wallet ID</TD>
	 * <TD ROWSPAN="1" COLSPAN="1">Selects actions by the id of the wallet for which the actions were submitted.</TD>
	 * </TR>
	 * <TR>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_TYPE</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1">The following action types are supported:
	 * <UL>
	 * <li><code>ACTION_TYPE_ISSUANCE</code></li>
	 * <li><code>ACTION_TYPE_PRODUCE</code></li>
	 * <li><code>ACTION_TYPE_RECYCLE</code></li>
	 * <li><code>ACTION_TYPE_POSTISSUANCE</code></li>
	 * <li><code>ACTION_TYPE_PINUNLOCK</code></li>
	 * <li><code>ACTION_TYPE_CARDLOCK</code></li>
	 * <li><code>ACTION_TYPE_CARDUNLOCK</code></li>
	 * </UL>
	 * For more information, see <code>Action.</code>
	 * </TD>
	 * <TD ROWSPAN="1" COLSPAN="1">Selects actions of a particular type or all types.</TD>
	 * </TR>
	 * <TR>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_APPLICATION_SET</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
	 * <TD ROWSPAN="1" COLSPAN="1">Any valid application set name.</TD>
	 * <TD ROWSPAN="1" COLSPAN="1">Selects actions associated with a particular application set (aka CMS card policy ID).</TD>
	 * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_REASON</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * <br><code>CRITERIA_COMPARISON_NOT_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * The action reason may be:<BR>
     * <BR>
     * For initial issuance:
     * <UL>
     * <li><code>ACTION_REASON_NONE</code></li>
     * </UL>
     * For temporary replacement:
     * <UL>
     * <li><code>ACTION_REASON_FORGOTTEN</code></li>
     * </UL>
     * For permanent replacement:
     * <UL>
     * <li><code>ACTION_REASON_LOST</code></li>
     * <li><code>ACTION_REASON_STOLEN</code></li>
     * <li><code>ACTION_REASON_DAMAGED</code></li>
     * <li><code>ACTION_REASON_EXPIRED</code></li>
     * </UL>
     * For re-issuance:
     * <UL>
     * <li><code>ACTION_REASON_RENEW</code></li>
     * </UL>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects actions(s) of a particular reason.</TD>
     * </TR>
	 * </TABLE>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The identifiers of all matching wallet actions
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
	virtual TStringVector	*findActionIds(CriteriaVector *criteria, int maxItems) = 0;

    /**
     * Returns the actions that meet the supplied criteria. For example, this method might be used to  retrieve all
     * actions that are pending, of type issuance, and with the CMS card policy PIV.
     * @param criteria Pointer to the list of search criteria.
     * <p>Criteria may be as follows:</p>
     * <TABLE BORDER="1">
     * <TR>
     * <TH ROWSPAN="1" COLSPAN="1">Criteria Key</TH>
     * <TH ROWSPAN="1" COLSPAN="1">Criteria Comparison</TH>
     * <TH ROWSPAN="1" COLSPAN="1">Possible Values and/or Type</TH>
     * <TH ROWSPAN="1" COLSPAN="1">Description</TH>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_STATUS</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * <br><code>CRITERIA_COMPARISON_NOT_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * The action status may be:
     * <UL>
     * <li><code>ACTION_STATUS_REQUESTED</code></LI>
     * <li><code>ACTION_STATUS_PENDING</code></LI>
     * <li><code>ACTION_STATUS_DENIED</code></LI>
     * <li><code>ACTION_STATUS_FAILED</code></LI>
     * </UL>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects action(s) in a particular state.</TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_PARENT_WALLET</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">Any valid wallet ID</TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects actions by the id of the wallet for which the actions were submitted.</TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_TYPE</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * The following action types are supported:
     * <UL>
     * <li><code>ACTION_TYPE_ISSUANCE</code></LI>
     * <li><code>ACTION_TYPE_PRODUCE</code></LI>
     * <li><code>ACTION_TYPE_RECYCLE</code></LI>
     * <li><code>ACTION_TYPE_POSTISSUANCE</code></LI>
     * <li><code>ACTION_TYPE_PINUNLOCK</code></LI>
     * <li><code>ACTION_TYPE_CARDLOCK</code></LI>
     * <li><code>ACTION_TYPE_CARDUNLOCK</code></li>
     * </ul>
     * For more information, see <code>Action.</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects actions of a particular type or all types.</TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_APPLICATION_SET</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">Any valid application set name.</TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects actions associated with a particular application set (aka CMS card policy ID).</TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_REASON</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * <br><code>CRITERIA_COMPARISON_NOT_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * The action reason may be:<BR>
     * <BR>
     * For initial issuance:
     * <UL>
     * <li><code>ACTION_REASON_NONE</code></li>
     * </UL>
     * For temporary replacement:
     * <UL>
     * <li><code>ACTION_REASON_FORGOTTEN</code></li>
     * </UL>
     * For permanent replacement:
     * <UL>
     * <li><code>ACTION_REASON_LOST</code></li>
     * <li><code>ACTION_REASON_STOLEN</code></li>
     * <li><code>ACTION_REASON_DAMAGED</code></li>
     * <li><code>ACTION_REASON_EXPIRED</code></li>
     * </UL>
     * For re-issuance:
     * <UL>
     * <li><code>ACTION_REASON_RENEW</code></li>
     * </UL>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects actions(s) of a particular reason.</TD>
     * </TR>
     * </TABLE>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return Pointer to the actions of all matching wallet actions
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws RemoteException If remote connectivity fails.
     */
    virtual ActionVector	*findActions(CriteriaVector *criteria, int maxItems) = 0;

    /**
     * <b>Deprecated</b> Returns the actions available to be called on the wallet.
     * @param walletId Pointer to the wallet on which to submit the action.
     * @return List of wallet actions that may be submitted on the specified wallet in its current state
     * @throws NoSuchWalletException If the wallet identifier specified does not identify a valid wallet
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws RemoteException If remote connectivity fails.
	 * @deprecated Use {@link AI::WalletManager::findApplicationSets(CriteriaVector*,int)}
     */
    virtual ActionVector	*getAvailableActions(WalletId *walletId) = 0;

    /**
     * Returns the application sets that meet the supplied criteria.
     * For example, this method might be used to retrieve all application sets allowed for the Action type PRODUCE.
     * <p>An application set (called a card policy in the CMS Operator Portal) is a list of applications that are to
     * be personalized during card issuance. The application set specifies what credentials and data are
     * to be loaded on the card during card issuance. For instance, the application set based on the
     * Windows Smart Card Logon Certificate Template might specify that only one PIN application and
     * two PKI applications are to be loaded onto the smart card.</p>
     * CMS supports the following card polices:
     * <ul>
     * <li>PIN</li>
     * <li>PKI (This policy will use X509 certificates)</li>
     * <li>SKI</li>
     * <li>Entrust profile (Entrust profile certificates)</li>
     * <li>Data storage (static data)</li>
     * </ul>
     * <b>Notes:</b>
     * <ul>
	 *		<li>All criteria are AND-ed together.</li>
     *		<li>A card policy is dependent on the following parameters:
     *			<ul>
     *				<li>Card profile</li>
     *				<li>Card type</li>
     *				<li>Set of credentials to be generated and loaded on the card</li>
     * 			</ul>
     * 		</li>
     *		<li>CMS uses an enrollment policy to map the card policy assignments to an organizational group
     * 		in the directory. It uses an additional mapping of the card policy issuance types (Re-issuance,
     *		Lost/Stolen, Forgotten, Damaged) to each card policy. The action-to-reason mapping is:
     *			<ul>
     *				<li><code>NONE</code> = "Initial Issuance"</li>
     *				<li><code>FORGOTTEN</code> = "Forgotten"</li>
     *				<li><code>LOST</code> = "Lost/Stolen"</li>
     *				<li><code>STOLEN</code> = "Lost/Stolen"</li>
     *				<li><code>DAMAGED</code> = "Damaged/Expired"</li>
     *				<li><code>EXPIRED</code> = "Damaged/Expired"</li>
     *				<li><code>RENEW</code> = "Re-Issuance" (one step card recycle and re-issuance of new applets and/or credentials</li>
     *			</ul>
     *		</li>
     *	</ul>
     * @param criteria Pointer to the list of search criteria.
     * <p>Criteria may be as follows:</p>
     * <TABLE BORDER="1">
     * <TR>
     * <TH ROWSPAN="1" COLSPAN="1">Criteria Key</TH>
     * <TH ROWSPAN="1" COLSPAN="1">Criteria Comparison</TH>
     * <TH ROWSPAN="1" COLSPAN="1">Possible Values and/or Type</TH>
     * <TH ROWSPAN="1" COLSPAN="1">Description</TH>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ACTION_APPLICATION_SET</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">Any application set name.</TD>
     * <TD ROWSPAN="1" COLSPAN="1">Use equality to test the existence of the application set, or inequality to retrieve all possible alternatives.</TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_ACTION_REASON</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code>
     * <br><code>CRITERIA_COMPARISON_NOT_EQUAL</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * The <code>Action reason</code> attribute can take the following values:<BR>
     * <BR>
     * For initial issuance:
     * <UL>
     * <li><code>ACTION_REASON_NONE</code></li>
     * </UL>
     * For temporary replacement:
     * <UL>
     * <li><code>ACTION_REASON_FORGOTTEN</code></li>
     * </UL>
     * For permanent replacement:
     * <UL>
     * <li><code>ACTION_REASON_LOST</code></li>
     * <li><code>ACTION_REASON_STOLEN</code></li>
     * <li><code>ACTION_REASON_DAMAGED</code></li>
     * <li><code>ACTION_REASON_EXPIRED</code></li>
     * </UL>
     * For re-issuance:
     * <UL>
     * <li><code>ACTION_REASON_RENEW</code></li>
     * </UL>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Action Reason mapping:
     * In CMS, there is an &quot;enrollment policy&quot; that is used to map the card policy assignments with an organizational group in the directory.
     * In addition, there is another mapping with the card policy issuance types (Initial Issuance, Lost/Stolen, Forgotten, Damaged) for each card policy.
     * <p>The action.reason mapping is as follows:</p>
     * <br><code>NONE</code> = &quot;Initial Issuance&quot;
     * <br><code>FORGOTTEN</code> = &quot;Forgotten&quot;
     * <br><code>LOST</code> = &quot;Lost/Stolen&quot;
     * <br><code>STOLEN</code> = &quot;Lost/Stolen&quot;
     * <br><code>DAMAGED</code> = &quot;Damaged&quot;
     * <br><code>RENEW</code> = &quot;Initial Issuance&quot;
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ID_SMID</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">Any valid security module ID.</TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects application sets with the specified security module id.</TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_ID_WALLETID</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1"><code>CRITERIA_COMPARISON_EQUAL</code></TD>
     * <TD ROWSPAN="1" COLSPAN="1">Any valid wallet ID.</TD>
     * <TD ROWSPAN="1" COLSPAN="1">Selects application sets with the specified wallet id.</TD>
     * </TR>
     * </TABLE>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The application sets that meet the criteria
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws RemoteException If remote connectivity fails.
     */
    virtual TStringVector	*findApplicationSets(CriteriaVector *criteria, int maxItems) = 0;

	/**
     * Returns the wallet identifiers that meet the supplied criteria.
     * If no criteria are supplied, all wallet identifiers will be returned where the following conditions are met:
     * <ul><li>security module(s) is/are bound or</li>
     * <li>issuance action(s) is/are pending	 </li></ul>
     * @param criteria Pointer to the list of search criteria.
	 * <p>All criteria are AND'ed together. None of the criteria listed below are required to call this method.</p>
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
     * <Code>CRITERIA_WALLET_ACTION_STATUS</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Accepted values:
	 * <UL>
	 * <li><code>ACTION_STATUS_REQUESTED</code></li>
	 * <li><code>ACTION_STATUS_PENDING</code></li>
	 * <li><code>ACTION_STATUS_DENIED</code></li>
	 * <li><code>ACTION_STATUS_FAILED</code></li>
 	 * </UL>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Selects wallet ids with actions in a particular state.
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_WALLET_ACTION_TYPE</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Accepted values:
     * <UL>
     * <li><code>ACTION_TYPE_ISSUANCE</code></li>
     * <li><code>ACTION_TYPE_PRODUCE</code></li>
     * <li><code>ACTION_TYPE_RECYCLE</code></li>
     * <li><code>ACTION_TYPE_POSTISSUANCE</code></li>
     * <li><code>ACTION_TYPE_PINUNLOCK</code></li>
     * <li><code>ACTION_TYPE_CARDLOCK</code></li>
     * <li><code>ACTION_TYPE_CARDUNLOCK</code></li>
     * </UL>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Selects wallet ids with actions of a particular type.
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_WALLET_ACTION_APPLICATION_SET</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
	 * <TD ROWSPAN="1" COLSPAN="1">
	 * Any valid application set name.
	 * </TD>
	 * <TD ROWSPAN="1" COLSPAN="1">
	 * Selects actions associated with a particular application set (aka CMS card policy ID).
	 * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_WALLET_SECURITYMODULE_COUNT</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * positive integer
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * Specifies the number of security modules bound to a wallet.
	 * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_ID_WALLETID</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * Any valid wallet ID.
     * <TD ROWSPAN="1" COLSPAN="1">
     * Selects specified wallet id.
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_ID_WALLET_TIMESTAMP</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Accepted value formats:
     * <UL>
     * <li>Unix epoch timestamp (positive long)</li>
     * <li>RFC 2282/822-compliant timestamp</li>
     * <li>ISO 8601-compiant timestamp</li>
     * </UL>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * <p>The Unix epoch timestamp coresponds to number of
	 * milliseconds since January 1, 1970 00:00:00 GMT.</p>
     * <p>The RFC 2282 format is:<p>
	 * <code>dd mmm yyyy HH:MM[:SS] (+|-)HHMM</code></p>
	 * <p>The ISO 8601 format is:
	 * <code>yyyy-mm-ddThh:MM:ssZ</code></p>
     * </TD>
     * </TR>
	 * </TABLE>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
     * @return The wallet identifiers of all matching wallets.
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws RemoteException If remote connectivity fails.
	 */
	virtual WalletIdVector	*findWalletIds(CriteriaVector* criteria, int maxItems) = 0;

    /**
     * Initializes a paged search of all wallets that meet the specified criteria.
     * @param criteria Pointer to the list of search criteria.
	 * <p>All criteria are AND'ed together. None of the criteria listed below are required to call this method.</p>
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
     * <Code>CRITERIA_WALLET_ACTION_STATUS</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Accepted values:
	 * <UL>
	 * <li><code>ACTION_STATUS_REQUESTED</code></li>
	 * <li><code>ACTION_STATUS_PENDING</code></li>
	 * <li><code>ACTION_STATUS_DENIED</code></li>
	 * <li><code>ACTION_STATUS_FAILED</code></li>
 	 * </UL>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Selects wallet ids with actions in a particular state.
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_WALLET_ACTION_TYPE</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Accepted values:
     * <UL>
     * <li><code>ACTION_TYPE_ISSUANCE</code></li>
     * <li><code>ACTION_TYPE_PRODUCE</code></li>
     * <li><code>ACTION_TYPE_RECYCLE</code></li>
     * <li><code>ACTION_TYPE_POSTISSUANCE</code></li>
     * <li><code>ACTION_TYPE_PINUNLOCK</code></li>
     * <li><code>ACTION_TYPE_CARDLOCK</code></li>
     * <li><code>ACTION_TYPE_CARDUNLOCK</code></li>
     * </UL>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Selects wallet ids with actions of a particular type.
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_WALLET_ACTION_APPLICATION_SET</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
	 * <TD ROWSPAN="1" COLSPAN="1">
	 * Any valid application set name.
	 * </TD>
	 * <TD ROWSPAN="1" COLSPAN="1">
	 * Selects actions associated with a particular application set (aka CMS card policy ID).
	 * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_WALLET_SECURITYMODULE_COUNT</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * positive integer
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * Specifies the number of security modules bound to a wallet.
	 * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_ID_WALLETID</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
	 * Any valid wallet ID.
     * <TD ROWSPAN="1" COLSPAN="1">
     * Selects specified wallet id.
     * </TD>
     * </TR>
     * <TR>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <Code>CRITERIA_ID_WALLET_TIMESTAMP</code>
     * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <code>CRITERIA_COMPARISON_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN</code><br>
     * <code>CRITERIA_COMPARISON_GREATER_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN</code><br>
     * <code>CRITERIA_COMPARISON_LESS_THAN_EQUAL</code><br>
     * <code>CRITERIA_COMPARISON_NOT_EQUAL</code>
	 * </TD>
     * <TD ROWSPAN="1" COLSPAN="1">
     * Accepted value formats:
     * <UL>
     * <li>RFC 2282/822-compliant timestamp</li>
     * <li>ISO 8601-compiant timestamp</li>
     * <li>Unix epoch timestamp (positive long)</li>
     * </UL>
     * <TD ROWSPAN="1" COLSPAN="1">
     * <p>The RFC 2282 format is:<p>
	 * <code>dd mmm yyyy HH:MM[:SS] (+|-)HHMM</code></p>
	 * <p>The ISO 8601 format is:
	 * <code>yyyy-mm-ddThh:MM:ssZ</code></p>
	 * <p>The Unix epoch timestamp coresponds to number of
	 * milliseconds since January 1, 1970 00:00:00 GMT.</p>
     * </TD>
     * </TR>
	 * </TABLE>
	 * @param fetchSize  Specifies  the number of entries to fetch.
     * @return Handle to the dataset to page through.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
    virtual SearchSessionId *findWalletIdsInit(CriteriaVector* criteria, int fetchSize) = 0;

    /**
     * Fetches/seeks through a previously initialized dataset produced by a wallet search.
     * @param searchHandle Handle to a dataset produced by a wallet search.
     * @param isFetchForward  Specifies the fetch direction. Forward is true.
     * @return The identifiers of matching wallet identifiers in the fetched page of the dataset.
     * @throws SearchEndException If the beginning or the end of the dataset is reached.
     * @throws NoSuchSearchException If the specified search handle is not valid.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
    virtual WalletIdVector* findWalletIdsFetch(SearchSessionId* searchHandle, bool isFetchForward) = 0;

//    /*
//     * Returns the size of the data set found.
//     * @param searchHandle Handle to a dataset produced by a wallet search.
//     * @return The size of the data set found.
//     * @throws NoSuchSearchException If the specified search handle is not valid.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws RemoteException If remote connectivity fails.
//     */
//    virtual long findWalletIdsSize(SearchSessionId* searchHandle) = 0;
//
//    /*
//     * Sets the current record index to which future fetch will be relative.
//     * If the specified index is out of range, the exceeded range limit will be
//     * used.
//     * Less than 0 will be set to the first record. Greater than the item count
//     * will be set to the last record. findWalletIdsSize(String) should be used
//     * to avoid providing an index greater than the last item.
//     * @param searchHandle Handle to a dataset produced by a wallet search.
//     * @param index Index to which to seek.
//     * @throws NoSuchSearchException If the specified search handle is not valid.
//     * @throws SessionException If a session level error occurs.
//     * @throws ManagementException If an internal failure occurs.
//     * @throws RemoteException If remote connectivity fails.
//     */
//    virtual void findWalletIdsSetIndex(SearchSessionId* searchHandle, long index) = 0;

    /**
     * Terminates a previously initialized wallet search.
     * @param searchHandle Handle to a dataset produced by a wallet search.
     * @throws NoSuchSearchException If the specified search handle is not valid.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws RemoteException If remote connectivity fails.
     */
    virtual void findWalletIdsDestroy(SearchSessionId* searchHandle) = 0;

	};


//====================================================================================================
// FACTORY CLASS DEFINITION
//====================================================================================================

	/**
	* Factory that provides a handle to a WalletManager instance.
	* <p>
	* The WalletManager factory configuration parameters are either of the following values:
	* <ul>
	*	<li>CONFIG_URL -- The complete URL of the WalletManager service in CMS. </li>
	* </ul>
	* or both of the following values:
	* <ul>
	*	<li>CONFIG_HOST_NAME -- The hostname of the WalletManager service in CMS. </li>
	*	<li>CONFIG_HOST_PORT -- The port number of the WalletManager service in CMS.</li>
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
	*/
	class AICCMAPI_API WalletManagerFactory : public ManagerFactory
	{
	public:
		/**
		* Returns a pointer to the WalletManager based on the underlying provider.
		* Uses the default factoryDefinition and default ProviderName.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The WalletManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the WalletManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the WalletManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the WalletManager service in CMS.</li>
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
		* @return WalletManager The created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static WalletManager* newInstance(ParameterVector* parameters);

		/**
		* Returns the WalletManager based on the underlying provider.
		* Uses the default factoryDefinition.
		* @param providerName Provider identifier.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The WalletManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the WalletManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the WalletManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the WalletManager service in CMS.</li>
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
		* @return WalletManager The created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static WalletManager* newInstance(TString providerName, ParameterVector* parameters);

		/**
		* Returns the WalletManager based on the underlying provider.
		* @param factoryDefinition Property file containing provider information.
		* @param providerName Provider identifier.
		* @param parameters Pointer to the configuration parameters used at initialization.
		* <p>
		* The WalletManager factory configuration parameters are either of the following values:
		* <ul>
		*	<li>CONFIG_URL -- The complete URL of the WalletManager service in CMS. </li>
		* </ul>
		* or both of the following values:
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the WalletManager service in CMS. </li>
		*	<li>CONFIG_HOST_PORT -- The port number of the WalletManager service in CMS.</li>
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
		* @return WalletManager The created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static WalletManager* newInstance(TString factoryDefinition, TString providerName, ParameterVector* parameters);

	protected:
		static WalletManager* init(TString factoryDefinition, TString providerName, ParameterVector* parameters);
	};

//====================================================================================================
// EXCEPTION CLASS DEFINITIONS
//====================================================================================================

// * @author John Babbidge
/**
 * Thrown when invalid or disallowed enrollment data or parameters, action types,
 * or action sequences are detected.
 */
class AICCMAPI_API InvalidActionException: public LocalizedException
{
    Action* action;
    TStringVector* enrollmentDataIds;
    TStringVector* parameterIds;

public:

	/**
     * Default Constructor.
     */
	InvalidActionException(void){ action=NULL;enrollmentDataIds=NULL;parameterIds=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	InvalidActionException(const LocalizedException &ex): LocalizedException(ex){ action=NULL;enrollmentDataIds=NULL;parameterIds=NULL; };

	/**
     * Destructor.
     */
	virtual ~InvalidActionException(void);

    /**
     * Returns The list of zero or more invalid enrollment data identifiers.
     * @return The list of zero or more invalid enrollment data identifiers.
     */
    virtual TStringVector* getEnrollmentDataIds();

    /**
     * Returns the list of zero or more invalid parameter identifiers.
     * @return The list of zero or more invalid parameter identifiers.
     */
    virtual TStringVector* getParameterIds();

    /**
     * Sets the list of zero or more invalid enrollment data identifiers.
     * @param enrollmentDataIds The list of zero or more invalid enrollment data identifiers.
     */
    virtual void setEnrollmentDataIds(TStringVector* enrollmentDataIds);

    /**
     * Sets the list of zero or more invalid parameter identifiers.
     * @param parameterIds the list of zero or more invalid parameter identifiers.
     */
    virtual void setParameterIds(TStringVector* parameterIds);

    /**
     * Returns the causing user action.
     * @return The causing user action.
     */
    virtual Action* getAction();

    /**
     * Sets the causing user action.
     * @param action Pointer to the causing user action.
     */
    virtual void setAction(Action* action);
};

/**
 * This exception is thrown when an attempt is made to bind a security module
 * already bound to a wallet.
 */
class AICCMAPI_API SecurityModuleBoundException: public LocalizedException
{
	SecurityModuleId	*smId;
	WalletId			*walletId;

public:

	/**
     * Default Constructor.
     */
	SecurityModuleBoundException(void){ smId=NULL; walletId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	SecurityModuleBoundException(const LocalizedException &ex): LocalizedException(ex){ smId=NULL; walletId=NULL; };

	/**
     * Destructor.
     */
	virtual ~SecurityModuleBoundException(void);

	/**
     * Returns the security module identifier.
     * @return The security module identifier.
     */
	virtual	SecurityModuleId	*getSecurityModuleId(void);

	/**
     * Sets the security module identifier.
     * @param smId Pointer to the security module identifier.
     */
	virtual	void				setSecurityModuleId(SecurityModuleId *smId);

	/**
     * Returns a pointer to the wallet identifier.
     * @return Pointer to the wallet identifier.
     */
	virtual	WalletId			*getWalletId(void);

	/**
     * Sets the wallet identifier.
     * @param walletId Pointer to the wallet identifier.
     */
	virtual	void				setWalletId(WalletId *walletId);
};

/**
 * This exception is thrown if a security module is not bound when it was
 * expected to be.
 */
class AICCMAPI_API SecurityModuleNotBoundException: public LocalizedException
{
	SecurityModuleId	*smId;
	WalletId			*walletId;

public:

	/**
     * Default Constructor.
     */
	SecurityModuleNotBoundException(void){ smId=NULL; walletId=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	SecurityModuleNotBoundException(const LocalizedException &ex): LocalizedException(ex){ smId=NULL; walletId=NULL; };

	/**
     * Destructor.
     */
	virtual ~SecurityModuleNotBoundException(void);

	/**
     * Returns the security module identifier.
     * @return The security module identifier.
     */
	virtual	SecurityModuleId	*getSecurityModuleId(void);

	/**
     * Sets the security module identifier.
     * @param smId Pointer to the security module identifier.
     */
	virtual	void				setSecurityModuleId(SecurityModuleId *smId);

	/**
     * Returns the wallet identifier.
     * @return The wallet identifier.
     */
	virtual	WalletId			*getWalletId(void);

	/**
     * Sets the wallet identifier.
     * @param walletId Pointer to the wallet identifier.
     */
	virtual	void				setWalletId(WalletId *walletId);
};



// * @author John Babbidge
/**
 * This exception is thrown when an attempt is made to retrieve or operate on
 * an action that does not exist.
 */
class AICCMAPI_API NoSuchActionException: public LocalizedException
{
	TStringVector* actionIds;

public:

	/**
     * Default Constructor.
     */
	NoSuchActionException(){ actionIds=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchActionException(const LocalizedException &ex): LocalizedException(ex){ actionIds=NULL; };

	/**
	* Destructor.
	*/
	virtual ~NoSuchActionException();

    /**
     * Returns a pointer to the list of one or more action identifiers associated with this exception.
     * @return Pointer to the list of one or more action identifiers associated with this exception.
     */
    virtual TStringVector* getActionIds();

	/**
     * Sets a list of one or more action identifiers associated with this exception.
     * @param actionIds List of one or more action identifiers associated with this
     * exception.
     */
    virtual void setActionIds(TStringVector* actionIds);
};


}


