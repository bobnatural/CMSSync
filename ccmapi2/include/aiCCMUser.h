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
 * Represents an abstract enrollment data instance.
 * <p> The EnrollmentData and EnrollmentDataValue classes are used to convey all enrollment data
 *	(User attributes) to CMS.</p>
 */
class AICCMAPI_API EnrollmentData : public DLL_INTERFACE
{
public:

    /**
     * Returns the identifier of the enrollment data.
     * @return The identifier of the enrollment data.
     */
	virtual	TString	getId(void)=0;

    /**
     * Returns the type of the enrollment data
     * @return The type of the enrollment data.
     */
	virtual	TString	getType(void)=0;

    /**
     * Sets the identifier of the enrollment data.
     * @param id The identifier of the enrollment data.
     */
	virtual	void setId(TString id)=0;

    /**
     * Sets the type of the action.
     * @param type The type of the action.
     */
	virtual	void setType(TString type)=0;

	/**
	 * Returns true if the enrollment data instance is an external reference.
	 * @return true If the enrollment data instance is an external reference.
	 */
	virtual	bool getIsReference(void)=0;

	/**
     * Sets whether the enrollment data instance is an external reference.
	 * @param isReference Whether the enrollment data instance is an external
     * reference.
	 */
	virtual	void setIsReference(bool isReference)=0;
};

/**
 * Represents a physical enrollment data value.
 * <p> The EnrollmentData and EnrollmentDataValue classes are used to convey all enrollment data
 *	(User attributes) to CMS.</p>
 */
class AICCMAPI_API EnrollmentDataValue: public DLL_OBJECT(EnrollmentData)
{
	TString	id;
	TString	type;
	TString	encoding;
	TString	value;
	bool isReference;

public:
    /**
     * Constructor used to construct a enrollment data value.
     * @param id The identifier of the enrollment data.
     * @param value The enrollment data value.
     */
	EnrollmentDataValue(TString id, TString value);

    /**
     * Constructor used to construct a enrollment data value.
     * @param id The identifier of the enrollment data.
     * @param type The type of the enrollment data.
     * @param encoding The encoding of the enrollment data.
     * @param value The enrollment data value.
 	 * <p>The type attribute of the EnrollmentDataValue class can accept the following values:
 	 * <ul>
  	 * 		<li>ENROLLMENT_DATA_TYPE_STRING</li>             
     * </ul>
     */
	EnrollmentDataValue(TString id , TString type, TString encoding, TString value);


	/**
	 * Constructor.
	 */
	EnrollmentDataValue(void);

    /**
     * Copy Constructor.
 	 * @param src Source EnrollmentDataValue object.  A copy of the content of this object is used for new content.
     */
	EnrollmentDataValue(const EnrollmentDataValue &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source EnrollmentDataValue object.  A copy of the content of this object is used for new content.
	 */
	virtual EnrollmentDataValue& operator=(const EnrollmentDataValue& src);

	/**
	 * Destructor.
	 */
	virtual ~EnrollmentDataValue(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual EnrollmentDataValue* clone() const	{ return new EnrollmentDataValue(*this); }

    /**
     * Returns the identifier of the enrollment data.
     * @return The identifier of the enrollment data.
     */
	virtual	TString	getId(void);

    /**
     * Returns the type of the enrollment data
     * @return The type of the enrollment data.
     */
	virtual	TString	getType(void);

    /**
     * Sets the identifier of the enrollment data.
     * @param id The identifier of the enrollment data.
     */
	virtual	void setId(TString id);

    /**
     * Sets the type of the action.
     * @param type The type of the action.
     */
	virtual	void setType(TString type);

	/**
	 * Returns true if the enrollment data instance is an external reference.
	 * @return true If the enrollment data instance is an external reference.
	 */
	virtual	bool getIsReference(void);

	/**
     * Sets whether the enrollment data instance is an external reference.
	 * @param isReference Whether the enrollment data instance is an external
     * reference.
	 */
	virtual	void setIsReference(bool isReference);

    /**
     * Returns the encoding of the enrollment data or null if none used.
     * @return The encoding of the enrollment data.
     */
	virtual	TString	getEncoding(void);

    /**
     * Returns the enrollment data value. The enrollment data value will be
     * encoded according to the encoding type.
     * @see #getEncoding()
     * @return The enrollment data value.
     */
	virtual	TString	getValue(void);

	/**
     * Sets the encoding of the enrollment data or null if none used.
	 * @param encoding The encoding of the enrollment data or null if none used.
	 */
	virtual	void			setEncoding(TString encoding);

	/**
     * Sets the enrollment data value. The enrollment data value will be
     * encoded according to the encoding type.
	 * @param value The enrollment data value.
	 */
	virtual	void			setValue(TString value);
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<EnrollmentDataValue>;
typedef Vector<EnrollmentDataValue>	EnrollmentDataVector;

/**
  *	In CMS, a User object represents a card holder. A User object is identified by a unique UserId instance. 
  *	The User object contains a list of EnrollmentData objects which describe the user information. 
  * Operations such as PRODUCE or POST-ISSUANCE are based on the configuration values applied to the CMS group to which the User.
  * <p>A user object has a wallet when one or more SecurityModules are bound to it and/or pending Actions are submitted to that user.</p>
  * A Security Module contains one or more Applications.
  *	<p>An Application owns zero or more credentials where each credential belongs to only one application.</p>
  * A Credential contains zero or more credential elements where each CredentialElement exists within only one security module.
 */
class AICCMAPI_API User : public DLL_OBJECT_BASE
{
	UserId*					id;
	EnrollmentDataVector*	enrollDatas;

public:
    /**
     * Constructor.
     * @param userId Pointer to the unique identifier of the user.
     * @param enrollmentData The initial enrollment data to associate with this
     * new user.
     */
	User(UserId* userId, EnrollmentDataVector* enrollmentData);

    /**
     * Constructor.
     */
	User(void);

    /**
     * Copy Constructor.
 	 * @param src Source User object.  A copy of the content of this object is used for new content.
     */
	User(const User &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source User object.  A copy of the content of this object is used for new content.
	 */
	virtual User& operator=(const User& src);

    /**
     * Destructor.
     */
	virtual ~User(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual User* clone() const	{ return new User(*this); }

    /**
     * Returns the user&apos;s unique identifier.
     * @return The user&apos;s unique identifier
     */
	virtual	UserId*	getId(void);

    /**
     * Returns the enrollment data for the user.
     * @return The enrollment data for the user.
     */
	virtual	EnrollmentDataVector* getEnrollmentData(void);

    /**
     * Sets the user&apos;s unique identifier.
     * @param id The user&apos;s unique identifier.
     */
	virtual	void setId(UserId* id);

    /**
     * Sets the enrollment data for the user.
     * @param enrollmentData The enrollment data for the user.
     */
	virtual	void setEnrollmentData(EnrollmentDataVector* enrollmentData);
};

EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<User>;
typedef Vector<User>				UserVector;

//====================================================================================================
// USER MANAGER DEFINITION
//====================================================================================================

	/**
	 * The UserManager component makes it possible to create, delete, enroll data attributes, and perform searches 
	 * for card holders (Users). 
	 * <p><b>Note: </b><i>All user data must be enrolled before executing wallet actions that update physical security modules.</i></p>
	 * <p><i>The CCM API UserManager methods now handles multivalued attributes.</i></p>
	 * <p><i>This API, when used in a remote client, requires appropriate authentication and
	 * authorization in order to be authorized to exercise any functionality.</i></p>
 	 * <p>Abstract Class, Pure-Virtual Interface.</p>
	 */
	class AICCMAPI_API UserManager : public SessionManager
	{
	public:

    /**
     * Creates a new user with the information contained in the User object.
     * This method provides individual user creation with strongly typed
     * exceptions.
     * @param user A pointer to the user information.
     * @return The user&apos;s unique identifier.
     * @throws MalformedEnrollmentDataException If the supplied 
     * enrollment data is not validly formatted or the reference is
     * invalid.
     * @throws UserAlreadyExistsException If the user already exists.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserId					*createUser(User *user) = 0;
    /**
     * Returns a list of user identifiers of users who meet the supplied criteria.
     * <p>This method performs searches with the specified size of a result set.</p>
     * @param criteria The list of search criteria with which to filter.
	* <p>The criteria that can be specified  are:</p>
    * <a name="criteriaTable"></a><TABLE BORDER="1">
<TR>
<TH ROWSPAN="1" COLSPAN="1">Criterion</TH>
<TH ROWSPAN="1" COLSPAN="1">Criteria Comparison</TH>
<TH ROWSPAN="1" COLSPAN="1">Possible Values</TH>
<TH ROWSPAN="1" COLSPAN="1">Description</TH>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<Code>CRITERIA_ID_ACTION_STATUS</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
<br><code>CRITERIA_COMPARISON_NOT_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Accepted values:
<UL>
<li><code>ACTION_STATUS_REQUESTED</code></li>
<li><code>ACTION_STATUS_PENDING</code></li>
<li><code>ACTION_STATUS_DENIED</code></li>
<li><code>ACTION_STATUS_FAILED</code></li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions in a particular state.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_ACTION_TYPE</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<UL>
<li><code>ACTION_TYPE_ISSUANCE</code></li>
<li><code>ACTION_TYPE_PRODUCE</code></li>
<li><code>ACTION_TYPE_RECYCLE</code></li>
<li><code>ACTION_TYPE_POSTISSUANCE</code></li>
<li><code>ACTION_TYPE_PINUNLOCK</code></li>
<li><code>ACTION_TYPE_CARDLOCK</code></li>
<li><code>ACTION_TYPE_CARDUNLOCK</code></li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions of a particular type.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_ACTION_APPLICATION_SET</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>String</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects all users with actions that are associated with a particular application set (aka CMS policy ID)
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_SECURITYMODULE_COUNT</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
<code>CRITERIA_COMPARISON_NOT_EQUAL</code>
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
<code>CRITERIA_ID_USERID</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
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
<code>CRITERIA_COMPARISON_EQUAL</code>
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
     * @param maxItems The maximum number of items to fetch if the search finds more than this number. 
	 * Set this parameter to 0 if there is no limit. 
	 * <p><b>Note: </b><i>If the number of results that meet the query exceeds the maxItems specified, an 
	 * exception -- SizeLimitPeopleException -- is thrown. (For more information, even though it is not specifically
	 * C++ related, see the 
	 * <a href="http://java.sun.com/products/jndi/tutorial/basics/directory/countlimit.html">JDK documentation: http://java.sun.com/products/jndi/tutorial/basics/directory/countlimit.html</a>.  
	 * When this occurs, in order receive a result set, either increase the value of maxItems or make your query more specific.  Also note that the CCM API times out after 60 seconds if the query is taking too long.</i></p>
     * @return List of identifiers of users that meet the specified criteria.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserIdVector			*findUserIds(CriteriaVector *criteria, int maxItems) = 0;

	/**
	* Returns a list of user that meet the supplied criteria.
    * <p>This method performs searches where the search result set size is known to be
    * small by the caller.</p>
	* @param criteria Pointer to the list of search criteria with which to filter.
	* <p>The criteria that can be specified  are:</p>
    * <a name="criteriaTable"></a><TABLE BORDER="1">
<TR>
<TH ROWSPAN="1" COLSPAN="1">Criterion</TH>
<TH ROWSPAN="1" COLSPAN="1">Criteria Comparison</TH>
<TH ROWSPAN="1" COLSPAN="1">Possible Values</TH>
<TH ROWSPAN="1" COLSPAN="1">Description</TH>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<Code>CRITERIA_ID_ACTION_STATUS</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
<br><code>CRITERIA_COMPARISON_NOT_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Accepted values:
<UL>
<li><code>ACTION_STATUS_REQUESTED</code></li>
<li><code>ACTION_STATUS_PENDING</code></li>
<li><code>ACTION_STATUS_DENIED</code></li>
<li><code>ACTION_STATUS_FAILED</code></li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions in a particular state.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_ACTION_TYPE</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<UL>
<li><code>ACTION_TYPE_ISSUANCE</code></li>
<li><code>ACTION_TYPE_PRODUCE</code></li>
<li><code>ACTION_TYPE_RECYCLE</code></li>
<li><code>ACTION_TYPE_POSTISSUANCE</code></li>
<li><code>ACTION_TYPE_PINUNLOCK</code></li>
<li><code>ACTION_TYPE_CARDLOCK</code></li>
<li><code>ACTION_TYPE_CARDUNLOCK</code></li>
</UL>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects users with actions of a particular type.
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_ACTION_APPLICATION_SET</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>String</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
Selects all users with actions that are associated with a particular application set (aka CMS policy ID)
</TD>
</TR>
<TR>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_ID_SECURITYMODULE_COUNT</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
<code>CRITERIA_COMPARISON_NOT_EQUAL</code>
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
<code>CRITERIA_ID_USERID</code>
</TD>
<TD ROWSPAN="1" COLSPAN="1">
<code>CRITERIA_COMPARISON_EQUAL</code>
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
<code>CRITERIA_COMPARISON_EQUAL</code>
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
	* @param enrollmentDataIds Pointer to the identifiers of enrollment data to retrieve.
	* @param maxItems  Specifies  the maximum number of items to fetch should the search find more than this number. Set to 0 if no limit.
	* @return List of identifiers of users that meet the specified criteria.
	* @throws ManagementException If an internal failure occurs.
	* @throws SessionException If a session level error occurs.
	* @throws LocalizedRemoteException If remote connectivity fails.
	*/
	virtual UserVector* findUsers(CriteriaVector *criteria, TStringVector *enrollmentDataIds, int maxItems) = 0;

    /**
     * Returns a list of the specified users and optionally some of their enrollment data (depending on
	 * the list of UserIds and enrollmentDataIds provided as parameters).
     * <p>This method performs searches where the search result set size is known to be
     * small by the caller.</p>
     * @param userIds Pointer to the list of user identifiers.
     * @param enrollmentDataIds Pointer to the identifiers of enrollment data to retrieve.
     * @return List of identifiers of users that meet the specified criteria.
     * @throws ManagementException If an internal failure occurs.
     * @throws SessionException If a session level error occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
    virtual UserVector				*getUsers(UserIdVector *userIds, TStringVector *enrollmentDataIds) = 0;

    /**
     * Deletes the specified user including all associated security modules
     * and credentials. 
     * @param userId Pointer to the identifier of the user to delete.
     * @throws NoSuchUserException If the user does not exist.
     * @throws InvalidStateException If the user is in a state not compatible
     * with deletion.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void					deleteUser(UserId *userId) = 0;

    /**
     * Adds new or updates existing enrollment data for the specified user.
     * @param userId Pointer to the unique identifier of the user.
     * @param enrollmentData Pointer to the enrollment data to add.
     * @throws NoSuchUserException If one or more of the specified users does not exist.
     * * @throws MalformedEnrollmentDataException If the supplied enrollment data
     * is not validly formatted or the reference is not valid.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void					addEnrollmentData(UserId *userId, EnrollmentDataVector *enrollmentData) = 0;

    /**
     * Returns a list of specified enrollment data for the specified user.
     * @param userId Pointer to the unique identifier of the user.
     * @param enrollmentDataIds Pointer to the enrollment data to retrieve.
     * @return List of specified enrollment data for the specified user.
     * @throws NoSuchUserException If the specified user does not exist.
     * @throws NoSuchEnrollmentDataException If one or more of the identifiers does not
     * specify a valid enrollment data item.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual EnrollmentDataVector	*getEnrollmentData(UserId *userId, TStringVector *enrollmentDataIds) = 0;

    /**
     * Deletes the specified list of enrollment data from the specified user.
     * @param userId Pointer to the unique identifier of the user.
     * @param enrollmentDataIds Pointer to the enrollment data to delete.
     * @throws NoSuchUserException If one or more of the specified users does not exist.
     * @throws NoSuchEnrollmentDataException If one or more of the identifiers does not
     * specify a valid enrollment data item.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual void					deleteEnrollmentData(UserId *userId, TStringVector *enrollmentDataIds) = 0;

    /**
     * Returns the user bound to the specified wallet.
     * @param walletId Pointer to the unique identifier of the wallet.
     * @return The wallet bound to the specified user.
     * @throws NoSuchWalletException If the wallet identifier 
     * specified does not identify a valid wallet.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserId					*getBoundUserFromWallet(WalletId *walletId) = 0;

    /**
     * Returns the user bound to the wallet containing the specified security
     * module. <p><b>Note: </b><i>User:Wallet:SecurityModule is 1:1:n.</i></p>
     * @param smId Pointer to the unique identifier of the security module.
     * @return The user bound (via a wallet) to the specified security module.
     * @throws NoSuchBindingException If the specified security module is not bound
     * to any user.
     * @throws NoSuchSecurityModuleException If the specified security module
     * was not found.
     * @throws SessionException If a session level error occurs.
     * @throws ManagementException If an internal failure occurs.
     * @throws LocalizedRemoteException If remote connectivity fails.
     */
	virtual UserId					*getBoundUserFromSM(SecurityModuleId *smId) = 0;

	};
	
//====================================================================================================
// FACTORY CLASS DEFINITION
//====================================================================================================

	/**
	* Factory that provides a handle to a UserManager instance.
	* <p>
	* The UserManager factory configuration parameters are either of the following values:
	* <ul>
	*	<li>CONFIG_URL -- The complete URL of the UserManager service in CMS. </li>
	* </ul>
	* or both of the following values:
	* <ul>
	*	<li>CONFIG_HOST_NAME -- The hostname of the UserManager service in CMS. </li>
	*	<li>CONFIG_HOST_PORT -- The port number of the UserManager service in CMS.</li>
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
	class AICCMAPI_API UserManagerFactory : public ManagerFactory
	{
	public:
		/**
		* Returns the UserManager based on the underlying provider.
		* Uses the default factoryDefinition and default ProviderName.
		* @param parameters Pointer to the configuration parameters used at initialization.
 	 	* The factory configuration parameters are:
		* <br>Either:
		* <ul>
		* 	<li>CONFIG_URL -- The URL of the UserManager service in CMS.</li>
		* </ul>
		* or both
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the UserManager service in CMS.</li>
 		*	<li>CONFIG_HOST_PORT -- The port number of the UserManager service in CMS.</li>
		* </ul>
		* If the certificates have been imported into the store, there will be a prompt to pick a certificate.  
		* To avoid the prompt, pass the following two parameters:
		* <ul>
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.</li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.</li>
		* </ul>
		* @return UserManager* Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static UserManager* newInstance(ParameterVector* parameters);

		/**
		* Returns the UserManager based on the underlying provider.
		* Uses the default factoryDefinition.
		* @param providerName Provider identifier.
		* @param parameters Pointer to the configuration parameters used at initialization.
 	 	* The factory configuration parameters are:
		* <br>Either:
		* <ul>
		* 	<li>CONFIG_URL -- The URL of the UserManager service in CMS.</li>
		* </ul>
		* or both
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the UserManager service in CMS.</li>
 		*	<li>CONFIG_HOST_PORT -- The port number of the UserManager service in CMS.</li>
		* </ul>
		* If the certificates have been imported into the store, there will be a prompt to pick a certificate.  
		* To avoid the prompt, pass the following two parameters:
		* <ul>
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.</li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.</li>
		* </ul>
		* @return UserManager* Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static UserManager* newInstance(TString providerName, ParameterVector* parameters);

		/**
		* Returns the UserManager based on the underlying provider.
		* @param factoryDefinition Property file containing provider information.
		* @param providerName Provider identifier.
		* @param *parameters Pointer to the configuration parameters used at initialization.
 	 	* The factory configuration parameters are:
		* <br>Either:
		* <ul>
		* 	<li>CONFIG_URL -- The URL of the UserManager service in CMS.</li>
		* </ul>
		* or both
		* <ul>
		*	<li>CONFIG_HOST_NAME -- The hostname of the UserManager service in CMS.</li>
 		*	<li>CONFIG_HOST_PORT -- The port number of the UserManager service in CMS.</li>
		* </ul>
		* If the certificates have been imported into the store, there will be a prompt to pick a certificate.  
		* To avoid the prompt, pass the following two parameters:
		* <ul>
		*	<li>CONFIG_CERTSERIAL -- The serial number of certificate for SSL V3.</li>
		*	<li>CONFIG_CERTISSUER -- The subject DN string of CA that issued certificate for SSL V3.</li>
		* </ul>
		* @return UserManager* Pointer to the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static UserManager* newInstance(TString factoryDefinition, TString providerName, ParameterVector* parameters);

	protected:
		static UserManager* init(TString factoryDefinition, TString providerName, ParameterVector* parameters);
	};

//====================================================================================================
// EXCEPTION CLASS DEFINITIONS
//====================================================================================================

/**
 * Thrown if the user attempted to be added already exists.
 */
class AICCMAPI_API UserAlreadyExistsException: public LocalizedException
{
	UserIdVector		*userIds;

public:

	/**
     * Default Constructor.
     */
	UserAlreadyExistsException(void){ userIds=NULL; };

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	UserAlreadyExistsException(const LocalizedException &ex): LocalizedException(ex){ userIds=NULL; };

	/**
     * Destructor.
     */
	virtual ~UserAlreadyExistsException(void);

    /**
     * Returns the list of users that already exist.
     * @return The list of users that already exist.
     */
	virtual	UserIdVector	*getUserIds(void);

    /**
     * Sets the list of users that already exist.
     * @param userIds The list of users that already exist.
     */
	virtual	void				setUserIds(UserIdVector *userIds);
};

/**
 * Thrown when invalid enrollment data is passed.
 */
class AICCMAPI_API MalformedEnrollmentDataException: public MultiErrorException
{
public:
	/**
     * Default Constructor.
     */
	MalformedEnrollmentDataException() {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	MalformedEnrollmentDataException(const LocalizedException &ex):MultiErrorException(ex){};
};

/**
 * Thrown if the specified enrollment data does not exist.
 */
class AICCMAPI_API NoSuchEnrollmentDataException: public MultiErrorException
{
public:
	/**
     * Default Constructor.
     */
	NoSuchEnrollmentDataException() {};

	/**
     * Copy Constructor.
 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
     */
	NoSuchEnrollmentDataException(const LocalizedException &ex):MultiErrorException(ex){};
};

}


