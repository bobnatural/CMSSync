/*
 * (c) Copyright 2006 ActivIdentity
 * All Rights Reserved.
 * 
 * Created on May 5, 2006
 */
#include "aiCCMUtil.h"
#include "aiCCMUtilImpl.h"
#include "aiCCMUser.h"

#ifdef AICCMUSER_EXPORTS
#define AICCMUSER_API __declspec(dllexport)
#define EXPUSER_TEMPLATE
#else
#define AICCMUSER_API __declspec(dllimport)
#define EXPUSER_TEMPLATE extern
#endif

class	userSoapBinding;

namespace AI
{

/**
 * @internal
 * Provides access to CMS user management functionality.
 */
class AICCMUSER_API UserManagerImpl : public DLL_OBJECT(UserManager), public SessionManagerImpl
{
	userSoapBinding	*userProxy;

public:
    /*@internal
     * Constructor.
     * @param URL The URL of the UserManagerImpl web service. The URL should be of the form: 
	 * <pre>
     * http://<host>:<port>/aims/services/user
	 * </pre>.
     */
	UserManagerImpl();
	UserManagerImpl(TString	URL);
	UserManagerImpl(TString URL, ParameterVector* INITPARAM);

    /*@internal
     * Destructor.
     */
	virtual ~UserManagerImpl(void);

	/*@internal
	 * Initializes a UserManagerImpl instance.
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
     * authenticate operator
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

    /*@internal
     * Creates a new user with the information contained in the User object.
     * This method provides individual user creation with strongly typed
     * exceptions.
     * @param user The user&apos;s information.
     * @return The user&apos;s unique identifier.
     * @throws MalformedEnrollmentDataException If the user&apos;s 
     * enrollment data is not validly formatted or the reference is
     * not valid.
     * @throws UserAlreadyExistsException If the user already exists.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserId *createUser(User *user);

    /*@internal
     * Returns a list of user identifiers that meet the specified criteria.
     * <p>
     * This method performs searches where the search result set size is known to be
     * small by the caller. If the result set is large or unknown,  consider using
     * findUserIdsInit/findUserIdsFetch/findUserIdsDestroy.	</p>
     * @param criteria List of search criteria with which to filter.
		* <p>The criteria that can be specified  are:</p>
    * <a name="criteriaTable"></a><TABLE BORDER="1">
<TR>
<TH ROWSPAN="1" COLSPAN="1">
Criterion
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Comparison
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Possible Values
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Description
</TH>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<Code>
CRITERIA_ID_ACTION_STATUS</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
<br><code>
CRITERIA_COMPARISON_NOT_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Accepted values:
<UL>
<li>ACTION_STATUS_REQUESTED</li>
<li>ACTION_STATUS_PENDING</li>
<li>ACTION_STATUS_DENIED</li>
<li>ACTION_STATUS_FAILED</li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions in a particular state.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ID_ACTION_TYPE</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<UL>
<li>
<code>ACTION_TYPE_ISSUANCE</code>
</li>
<li><code>ACTION_TYPE_PRODUCE</code>
</li>
<li><code>
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
ACTION_TYPE_CARDUNLOCK</code></li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions of a particular type.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ID_ACTION_APPLICATION_SET</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">

<code>
CRITERIA_COMPARISON_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">

<code>
String</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects all users with actions that are associated with a particular application set (aka CMS policy ID)
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_WALLET_SECURITYMODULE_COUNT</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
<code>
CRITERIA_COMPARISON_NOT_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
positive integer
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Specifies the number of security modules bound to a wallet.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ID_USERID</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
user id string
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Specifies the identifier of a specific user.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_ENROLLMENT_DATA + attribName</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
user enrollment data value
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Specifies an enrollment data item to search by.
<em>Note</em>: <code>CRITERIA_ID_ENROLLMENT_DATA </code> must be followed by the attribute name.
</TD>
</TR>
</TABLE>
     * @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
	 * <p><b>Note: </b><i>If the number of results that meet the query exceeds the maxItems specified, an 
	 * exception -- SizeLimitPeopleException -- is thrown. (For more information, see the 
	 * <a href="http://java.sun.com/products/jndi/tutorial/basics/directory/countlimit.html">JDK documentation: http://java.sun.com/products/jndi/tutorial/basics/directory/countlimit.html</a>.  
	 * When this occurs, in order receive a result set, either increase the value of maxItems or make your query more specific.  Also note that the CCM API times out after 60 seconds if the query is taking too long.</i></p>
     * @return List of user identifiers that meet the specified criteria.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserIdVector *findUserIds(CriteriaVector *criteria, int maxItems);

	/*@internal
	* Returns a list of user that meet the supplied criteria.
	* This method performs searches where the search result set size is known to be
	* small by the caller. If the result set is large or unknown, then consider using
	* findUsersInit/findUsersFetch/findUsersDestroy.
	* @param criteria List of search criteria with which to filter.
		* <p>The criteria that can be specified  are:</p>
    * <a name="criteriaTable"></a><TABLE BORDER="1">
<TR>
<TH ROWSPAN="1" COLSPAN="1">
Criterion
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Criteria Comparison
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Possible Values
</TH>
<TH ROWSPAN="1" COLSPAN="1">
Description
</TH>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<Code>
CRITERIA_ID_ACTION_STATUS</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
<br><code>
CRITERIA_COMPARISON_NOT_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Accepted values:
<UL>
<li>ACTION_STATUS_REQUESTED</li>
<li>ACTION_STATUS_PENDING</li>
<li>ACTION_STATUS_DENIED</li>
<li>ACTION_STATUS_FAILED</li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions in a particular state.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ID_ACTION_TYPE</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<UL>
<li>
<code>ACTION_TYPE_ISSUANCE</code>
</li>
<li><code>ACTION_TYPE_PRODUCE</code>
</li>
<li><code>
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
ACTION_TYPE_CARDUNLOCK</code></li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions of a particular type.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ID_ACTION_APPLICATION_SET</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">

<code>
CRITERIA_COMPARISON_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">

<code>
String</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects all users with actions that are associated with a particular application set (aka CMS policy ID)
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_WALLET_SECURITYMODULE_COUNT</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>
<code>
CRITERIA_COMPARISON_NOT_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
positive integer
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Specifies the number of security modules bound to a wallet.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_ID_USERID</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
user id string
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Specifies the identifier of a specific user.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_ENROLLMENT_DATA + attribName</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>
CRITERIA_COMPARISON_EQUAL</code>

</TD>
<TD ROWSPAN="1" COLSPAN="1">
user enrollment data value
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Specifies an enrollment data item to search by.
<em>Note</em>: <code>CRITERIA_ID_ENROLLMENT_DATA </code> must be followed by the attribute name.
</TD>
</TR>
</TABLE>
	* @param enrollmentDataIds The enrollment data to retrieve.
	* @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
	 * <p><b>Note: </b><i>If the number of results that meet the query exceeds the maxItems specified, an 
	 * exception -- SizeLimitPeopleException -- is thrown. (For more information, see the 
	 * <a href="http://java.sun.com/products/jndi/tutorial/basics/directory/countlimit.html">JDK documentation: http://java.sun.com/products/jndi/tutorial/basics/directory/countlimit.html</a>.  
	 * When this occurs, in order receive a result set, either increase the value of maxItems or make your query more specific.  Also note that the CCM API times out after 60 seconds if the query is taking too long.</i></p>
	* @return List of user identifiers that meet the supplied criteria.
	* @throws ManagementException If an internal failure occurs.
	* @throws SessionException If a session level error occurs.
	* @throws LocalizedRemoteException If remote connectivity fails.
	*/
	virtual UserVector *findUsers(CriteriaVector *criteria, TStringVector *enrollmentDataIds, int maxItems);

    /*@internal
     * Returns a list of the specified users.
     * <p>
     * This method performs searches where the search result set size is known to be
     * small by the caller. If the result set is large or unknown, then consider using
     * findUsersInit/findUsersFetch/findUsersDestroy.
     * @param userIds List of user identifiers.
     * @param enrollmentDataIds The enrollment data to retrieve.
     * @return List of user identifiers that meet the supplied criteria
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual UserVector *getUsers(UserIdVector *userIds, TStringVector *enrollmentDataIds);

    /*@internal
     * Deletes the specified user including all associated security modules
     * and credentials. 
     * @param userId The identifier of the user to delete.
     * @throws NoSuchUserException If the user does not exist.
     * @throws InvalidStateException If the user is in a state not compatible
     * with deletion.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void deleteUser(UserId *userId);

    /*@internal
     * Adds new or updates existing enrollment data for the specified user.
     * @param userId The unique identifier of the user.
     * @param enrollmentData The enrollment data to add.
     * @throws NoSuchUserException If one or more of the specified users does not exist.
     * * @throws MalformedEnrollmentDataException If the supplied enrollment data
     * is invalidly formatted or the reference is invalid.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void addEnrollmentData(UserId *userId, EnrollmentDataVector *enrollmentData);

    /*@internal
     * Returns a list of specified enrollment data for the specified user.
     * @param userId The unique identifier of the user.
     * @param enrollmentDataIds The enrollment data to retrieve.
     * @return List of specified enrollment data for the specified user.
     * @throws NoSuchUserException If the specified user does not exist.
     * @throws NoSuchEnrollmentDataException If one or more of the identifiers does not
     * specify a valid enrollment data item.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual EnrollmentDataVector *getEnrollmentData(UserId *userId, TStringVector *enrollmentDataIds);

    /*@internal
     * Deletes the specified list of enrollment data from the specified user.
     * @param userId The unique identifier of the user.
     * @param enrollmentDataIds The enrollment data to delete.
     * @throws NoSuchUserException If one or more of the specified users does not exist.
     * @throws NoSuchEnrollmentDataException If one or more of the identifiers does not
     * specify a valid enrollment data item.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void deleteEnrollmentData(UserId *userId, TStringVector *enrollmentDataIds);

    /*@internal
     * Returns the user bound to the specified wallet.
     * @param walletId The unique identifier of the wallet.
     * @return The wallet bound to the specified user.
     * @throws NoSuchWalletException If the wallet identifier 
     * specified does not identify a valid wallet.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserId *getBoundUserFromWallet(WalletId *walletId);

    /*@internal
     * Returns the user bound to the wallet containing the specified security
     * module. <p><b>Note: </b><i>User:Wallet:SecurityModule is 1:1:n.</i></p>
     * @param smId The unique identifier of the security module.
     * @return The user bound (via a wallet) to the specified security module.
     * @throws NoSuchBindingException If the specified security module is not bound
     * to any user.
     * @throws NoSuchSecurityModuleException If the specified security module
     * was not found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserId *getBoundUserFromSM(SecurityModuleId *smId);
};

} // namespace AI
namespace ActivIdentity = AI;
