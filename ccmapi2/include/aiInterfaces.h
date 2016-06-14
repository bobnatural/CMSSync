 /*
  * (c) Copyright 2006-2008 ActivIdentity
  * All Rights Reserved.
  *
  * This program is an unpublished copyrighted work which is proprietary
  * to ActivIdentity. This computer program includes Confidential,
  * Proprietary Information and is a Trade Secret of ActivIdentity.
  * Any use, disclosure, modification and/or reproduction is prohibited
  * unless authorized in writing by ActivIdentity.
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
#include <windows.h>
#include "aiDllUtil.h"
#include "aiTString.h"
#include "aiVector.h"
#include <time.h>

#ifdef AIINTERFACES_EXPORTS
#define AIINTERFACES_API __declspec(dllexport)
#define EXPINTERFACES_TEMPLATE
#else
#define AIINTERFACES_API __declspec(dllimport)
#define EXPINTERFACES_TEMPLATE extern
#endif

namespace AI
{

//====================================================================================================
// BEANS DEFINITIONS
//====================================================================================================

	/**
	 * Represents a single search criterion for an object search. 
	 * <i>(For example: users, security modules, actions, Credentials, etc)</i>.
	 * <p>The Criteria object contains:
	 * <ul>
	 * 		<li>An attribute identifier,</li>
	 * 		<li>A value, and			</li>
	 * 		<li>A comparison type - for int and date, supported types are:
	 *			<ul>
	 *				<li>&quot;&lt;&quot;</li>
	 *				<li>&quot;&lt;=&quot;</li>
	 *				<li>&quot;=&quot;</li>
	 *				<li>&quot;&gt;=&quot;</li>
	 *				<li>&quot;&gt;&quot;</li>
	 *			</ul>
	 *		</li>
	 * </ul>
	 * </p>
	 * For string values, the '*' character represents a pattern:<br>
	 * e.g. "*sam*" matches "asama", "bsamb", "1sam2".
	 * <p>
	 * The Criteria class is a mechanism for specifying intra- and inter-manager search criteria.
	 * A criteria key is relative to its context. For UserManager.findUserIds, the base of all keys is
	 * user. For WalletManager.findActionIds, the base of all keys is action.
	 * The criteria keys are also hierarchical in nature and, to some extent, object-oriented. For example,
	 * a User has zero or one Wallet and that Wallet has zero or more SecurityModules. Thus, in the
	 * context of UserManager.findUserIds, <code>user.wallet.securitymodule.xyz</code> indicates the
	 * attribute <code>xyz</code> of any security module that is bound to a wallet which is bound to a user. Similarly,
	 * <code>user.wallet.action.abc</code> indicates the attribute <code>abc</code> of any existing action for any wallet that is
	 * bound to a user.</p>
	 * The hierarchy of managed objects and their prefixes is:
	 * <ul>
	 * <li>	user -- &quot;user.&quot;
	 * 		<ul>
	 *			<li>wallet -- &quot;wallet.&quot;
	 *			<ul>
	 *				<li>security module -- &quot;securitymodule.&quot;</li>
	 *				<li>action -- &quot;action.&quot;</li>
	 *			</ul>
	 *			</li>
	 *		</ul>
	 *	</li>
	 *	</ul>
	 * The starting point of the hierarchy is determined by the context of the finder method. Refer to
	 * specific finder methods for details about which criteria are supported.
	 */
	class AIINTERFACES_API Criteria : public DLL_OBJECT_BASE
	{
		TString	id;
		TString	comparison;
		TString	value;

	public:
		/**
		 * Constructor.
		 */
		Criteria();

		/**
		 * Constructor.
		 * @param id The identifier of the Criteria (specific to object types
		 * being searched).
		 * @param comparison Type of comparison (specific to value type).
		 * @param value Value to use in comparison.
		 */
		Criteria(TString id , TString comparison, TString value);

		/**
		 * Copy Constructor.
		 * @param src Source Criteria object.  A copy of the content of this object is used for new content.
		 */
		Criteria(const Criteria &src);

		/**
		 * Assignment Operator overload.
		 * @param src Source Criteria object.  A copy of the content of this object is used for new content.
		 */
		virtual Criteria& operator=(const Criteria& src);

		/**
		 * Destructor.
		 */
		virtual ~Criteria(void);

		/**
		 * Virtual Copy Constructor.
		 */
		virtual Criteria * clone() const	{ return new Criteria(*this); }

		/**
		 * Returns the Criteria identifier.
		 * @return The Criteria identifier.
		 */
		virtual	TString	getId(void);

		/**
		 * Returns the comparison type.
		 * @return The comparison type.
		 */
		virtual	TString	getComparison(void);

		/**
		 * Returns the value to compare against. 
		 * @return The value to compare against.
		 */
		virtual	TString	getValue(void);

		/**
		 * Sets Sets the the Criteria identifier.
		 * @param id The Criteria identifier.
		 */
		virtual	void			setId(TString id);

		/**
		 * Sets the comparison type.
		 * @param comparison The comparison type.
		 */
		virtual	void			setComparison(TString comparison);

		/**
		 * Sets the value to compare against.
		 * @param value The value to compare against.
		 */
		virtual	void			setValue(TString value);
	};

	/**
	 * Vector of Criteria objects.
	 */
	EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<Criteria>;
	typedef Vector<Criteria>			CriteriaVector;

	/**
	 * A base abstract identifier class. This class is intended to be subclassed to
	 * enforce strong typing in the various APIs/SPIs.
	 */
	class AIINTERFACES_API Identifier : public DLL_OBJECT_BASE
	{
		TString	id;
		TString	type;

	public:
		/**
		 * Constructor.
		 * @param id Uniquely identifies an entity.
		 * @param type The type of the entity being identified.
		 */
		Identifier(TString id, TString type);

		/**
		 * Constructor.
		 */
		Identifier(void);

		/**
		 * Copy Constructor.
		 * @param src Source Identifier object.  A copy of the content of this object is used for new content.
		 */
		Identifier(const Identifier &src);

		/**
		 * Assignment Operator overload.
		 * @param src Source Identifier object.  A copy of the content of this object is used for new content.
		 */
		virtual Identifier& operator=(const Identifier& src);

		/**
		 * Destructor.
		 */
		virtual ~Identifier(void);

		/**
		 * Virtual Copy Constructor.
		 */
		virtual Identifier * clone() const	{ return new Identifier(*this); }

		/**
		 * Returns the unique identifier of an entity.
		 * @return The unique identifier of an entity.
		 */
		virtual	TString	getId(void);

		/**
		 * Returns the type of the entity being identified.
		 * @return The type of the entity being identified.
		 */
		virtual	TString	getType(void);

		/**
		 * Sets the unique identifier of an entity.
		 * @param id The unique identifier of an entity.
		 */
		virtual	void			setId(TString id);

		/**
		 * Sets the type of the entity being identified.
		 * @param type The type of the entity being identified.
		 */
		virtual	void			setType(TString type);
	};

//	 * @author John Babbidge
	/**
	 * A base class that contains the basic common attributes of all managed object
	 * classes.
	 * <p>A managed object contains:
	 * <ul>
	 * <li>A unique managed object identifier containing:
	 * 	<ul>
	 * 		<li>A unique identifier.</li>
	 * 		<li>An object type.</li>
	 *  </ul>
	 * </li>
	 * <li>The state of the object (based on its life cycle).</li>
	 * <li>The date the object was created.</li>
	 * <li>The date the object was last modified.</li>
	 * </ul></p>
	 * The term "managed object" specifies  the above-described class base.
	 */
	class AIINTERFACES_API Managed : public DLL_OBJECT_BASE
	{
		TString		state;
		__time64_t	created;
		__time64_t	modified;

	public:
		/**
		 * Constructor.
		 */
		Managed(void);

		/**
		 * Copy Constructor.
		 * @param src Source Managed object.  A copy of the content of this object is used for new content.
		 */
		Managed(const Managed &src);

		/**
		 * Assignment Operator overload.
		 * @param src Source Managed object.  A copy of the content of this object is used for new content.
		 */
		virtual Managed& operator=(const Managed& src);

		/**
		 * Destructor.
		 */
		virtual ~Managed(void);

		/**
		 * Virtual Copy Constructor.
		 */
		virtual Managed * clone() const	{ return new Managed(*this); }

		/**
		 * Returns the managed object's life cycle state.
		 * @return The managed object's life cycle state.
		 */
		virtual TString getState();

		/**
		 * Returns the date of last modification.
		 * @return The date of last modification.
		 */
		virtual __time64_t *getModified();

		/**
		 * Returns the managed object's creation time. 
		 * @return The managed object's creation time.
		 */
		virtual __time64_t *getCreated();
		
		/**
		 * Sets the managed object's life cycle state.
		 * @param state The managed object's life cycle state.
		 */
		virtual void setState(TString state);

		/**
		 * Sets the managed object's last modification time.
		 * @param modified The managed object's last modification time.
		 */
		virtual void setModified(__time64_t *modified);

		/**
		 * Sets the managed object's creation date.
		 * @param created The managed object's creation date.
		 */
		virtual void setCreated(__time64_t *created);
	};

	/**
	 * Represents a parameter or option where the id and value
	 * are enforced to be of type String.
	 * <p>
	 * A type and encoding attribute are also available to represent more complex
	 * data is to be sent or received. These might be useful, for example, when
	 * transporting a complex binary object.</p>
	 * <b>Example: </b><i> For an ASN.1-encoded RSA private key, the Parameter may 
	 * be:</i>
	 * <ul>
	 * <li><code>type = "CRMF.EncryptedValue"</code> (or an OID if available) </li>
	 * <li><code>encoding = "base64Binary"</code></li>
	 * </ul>
	 */
	class AIINTERFACES_API Parameter : public DLL_OBJECT_BASE
	{
	public:
		/**
		  Parameter encoding values: ENCODING_NONE, ENCODING_BASE64_BYTE_ARRAY, ENCODING_HEX_BYTE_ARRAY, ENCODING_DECIMAL, ENCODING_UTF8, ENCODING_DEFAULT.
		*/
		enum ParameterEncoding	{ ENCODING_NONE, ENCODING_BASE64_BYTE_ARRAY, ENCODING_HEX_BYTE_ARRAY, ENCODING_DECIMAL, ENCODING_UTF8, ENCODING_DEFAULT=0 };

		/**
		  Parameter type values: TYPE_STRING, TYPE_INTEGER, TYPE_BYTEARRAY, TYPE_LONG, TYPE_DEFAULT.
		*/
		enum ParameterType		{ TYPE_STRING, TYPE_INTEGER, TYPE_BYTEARRAY, TYPE_LONG, TYPE_DEFAULT=0 };

	private:
		TString	id;
		TString	value;
		ParameterType type;
		ParameterEncoding encoding;

	public:
		/**
		 * Constructor.
		 * @param id The Parameter's identifier.
		 * @param value The Parameter's value.
		 */
		Parameter(TString id, TString value);

		/**
		 * Constructor.
		 * @param encoding The Parameter's encoding.
		 * @param id The Parameter's identifier.
		 * @param type The Parameter's type.
		 * @param value The Parameter's value.
		 */
		Parameter(TString encoding, TString id, TString type, TString value);

		/**
		 * Constructor.
		 * @param encoding The Parameter's encoding.
		 * @param id The Parameter's identifier.
		 * @param type The Parameter's type.
		 * @param value The Parameter's value.
		 */
		Parameter(ParameterEncoding encoding, TString id, ParameterType type, TString value);

		/**
		 * Constructor.
		 */
		Parameter(void);

		/**
		 * Copy Constructor.
		 * @param src Source Parameter object.  A copy of the content of this object is used for new content.
		 */
		Parameter(const Parameter &src);

		/**
		 * Assignment Operator overload.
		 * @param src Source Parameter object.  A copy of the content of this object is used for new content.
		 */
		virtual Parameter& operator=(const Parameter& src);

		/**
		 * Destructor.
		 */
		virtual ~Parameter(void);

		/**
		 * Virtual Copy Constructor.
		 */
		virtual Parameter * clone() const	{ return new Parameter(*this); }

		/**
		 * Returns this Parameter's encoding or null if not set.
		 * @return This Parameter's encoding or null if not set.
		 */
		virtual	TString	getEncoding(void);

		/**
		 * Returns this Parameter's encoding or ENCODING_DEFAULT if not set.
		 * @return This Parameter's encoding or ENCODING_DEFAULT if not set.
		 */
		virtual	ParameterEncoding getEncodingEnum(void);

		/**
		 * Returns this Parameter's identifier or null if not set.
		 * @return This Parameter's identifier or null if not set.
		 */
		virtual	TString	getId(void);

		/**
		 * Returns this Parameter's type or null if not set.
		 * @return This Parameter's type or null if not set.
		 */
		virtual	TString	getType(void);

		/**
		 * Returns this Parameter's type or TYPE_DEFAULT if not set.
		 * @return This Parameter's type or TYPE_DEFAULT if not set.
		 */
		virtual	ParameterType getTypeEnum(void);

		/**
		 * Returns this Parameter's value or null if not set.
		 * @return This Parameter's type or null if not set.
		 */
		virtual	TString	getValue(void);

		/**
		 * Sets the Parameter's encoding.
		 * @param encoding The Parameter's encoding.
		 */
		virtual	void	setEncoding(TString encoding);

		/**
		 * Sets the Parameter's encoding.
		 * @param encoding The Parameter's encoding.
		 */
		virtual	void	setEncoding(ParameterEncoding encoding);

		/**
		 * Sets the Parameter's identifier.
		 * @param id The Parameter's identifier.
		 */
		virtual	void	setId(TString id);

		/**
		 * Sets the Parameter's type.
		 * @param type The Parameter's type.
		 */
		virtual	void	setType(TString type);

		/**
		 * Sets the Parameter's type.
		 * @param type The Parameter's type.
		 */
		virtual	void	setType(ParameterType type);

		/**
		 * Sets the Parameter's value.
		 * @param value The Parameter's value.
		 */
		virtual	void	setValue(TString value);

public:
		/**
		 * Returns TString for enumeration.
		 * @param type ParameterType enumeration.
		 * @return String representation of type.
		 */
		static TString	getTypeAsString(ParameterType type);

		/**
		 * Returns TString for enumeration.
		 * @param encoding ParameterEncoding enumeration.
		 * @return String representation of encoding.
		 */
		static TString	getEncodingAsString(ParameterEncoding encoding);

		/**
		 * Returns enumeration for TString
		 * @param type String representation of type.
		 * @return ParameterType enumeration.
		 */
		static ParameterType getTypeAsEnum(const TString &type);

		/**
		 * Returns enumeration for TString
		 * @param encoding String representation of encoding type.
		 * @return ParameterEncoding enumeration.
		 */
		static ParameterEncoding getEncodingAsEnum(const TString &encoding);
	};

	/**
	 * Vector of Parameter objects.
	 */
	EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<Parameter>;
	typedef Vector<Parameter>			ParameterVector;

//====================================================================================================
// CONFIG DEFINITIONS
//====================================================================================================

   /**
	* A unique identifier of a CredentialProfileId in getProfileSyntax(), getProfileDynamicEntries(), and 
	* findCredentialProfileIds(), and also in an Application object.
	* <p>
	* <b>Note: </b><i>This class is provided for type safety.</i></p>
	*/
	class AIINTERFACES_API ConfigurationId: public Identifier 
	{
	public:
		/**
		* Constructor.
		*/
		ConfigurationId(void);

		/**
		* Constructor.
		* @param id Unique identifier of the configuration.
		*/
		ConfigurationId(TString id);

		/**
		* Constructor.
		* @param id The unique identifier of the configuration.
		* @param type The type of configuration.
		*/
		ConfigurationId(TString id, TString type);

		/**
		* Copy Constructor.
		* @param src Source ConfigurationId object.  A copy of the content of this object is used for new content.
		*/
		ConfigurationId(const ConfigurationId &src);

		/**
		* Assignment Operator overload.
		* @param src Source ConfigurationId object.  A copy of the content of this object is used for new content.
		*/
		virtual ConfigurationId& operator=(const ConfigurationId& src);

		/**
		* Virtual Copy Constructor.
		*/
		virtual ConfigurationId* clone() const	{ return new ConfigurationId(*this); }
	};

	/**
	 * Vector of ConfigurationId objects.
	 */
	EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<ConfigurationId>;
	typedef Vector<ConfigurationId>		ConfigurationIdVector;

   /**
	* An Entry is a sub-element of a Configuration or Profile.
	* An Entry represents a concrete configuration value with the labeling and sensitivity
	* information required to display the value in a presentation layer where
	* appropriate.
	* <p>
	* Each Entry contains:
	* <ul>
	* <li>A unique identifier.</li>
	* <li>A human-readable (or localizable) label for display on the user interface.</li>
	* <li>A boolean that specifies  whether the entry's value is sensitive data. If true, any external
	* components are requested to protect it (especially where displayed or persisted).</li>
	* <li>The entry value.</li>
	* </ul></p>
	*/
	class AIINTERFACES_API Entry : public DLL_OBJECT_BASE
	{
		TString key;
		TString value;
		TString label;
		bool sensitiveFlag;
		bool dynamicFlag;

	public:
		/**
		* Constructor.
		*/
		Entry(void);

		/**
		* Copy Constructor.
		* @param src Source Entry object.  A copy of the content of this object is used for new content.
		*/
		Entry(const Entry &src);

		/**
		* Assignment Operator overload.
		* @param src Source Entry object.  A copy of the content of this object is used for new content.
		*/
		virtual Entry& operator=(const Entry& src);

		/**
		* Destructor.
		*/
		virtual ~Entry(void);

		/**
		* Virtual Copy Constructor.
		*/
		virtual Entry* clone() const	{ return new Entry(*this); }

		/**
		* Returns true if the entry point represents a dynamic piece of data that
		* must be provisioned prior to use by the credential provider context.
		* <p><b>Note: </b><i>This attribute is primarily for use in a Profile but may
		* be for use in a Configuration in future.</i></p>
		* <b>Note: </b><i>This attribute is for use above the CredentialProviderContext
		* level only. A CredentialProviderContext should assume, regardless of
		* whether this value is true or false, that the value supplied in the Entry
		* is concrete.</i>
		* @return isDynamic true If the entry point represents a dynamic piece of data.
		*/
		virtual bool isDynamic();

		/**
		* Sets the isDynamic attribute.
		* @param isDynamic The isDynamic.
		*/
		virtual void setDynamic(bool isDynamic);

		/**
		* Returns the key.
		* @return Returns the key.
		*/
		virtual TString getKey();
	    
		/**
		* Sets the key.
		* @param key The key.
		*/
		virtual void setKey(TString key);
	    
		/**
		* Returns the label.
		* @return Returns the label.
		*/
		virtual TString getLabel();
	    
		/**
		* Sets the label.
		* @param label The label.
		*/
		virtual void setLabel(TString label);
	    
		/**
		* Returns the value.
		* @return Returns the value.
		*/
		virtual TString getValue();
	    
		/**
		* Sets the value.
		* @param value Sets the value.
		*/
		virtual void setValue(TString value);

		/**
		* Returns true if the entry's value is sensitive data such that any
		* external components are requested to protect it (especially where
		* persisted).
		* @return true If the entry's value is sensitive data.
		*/
		virtual bool isSensitive();

		/**
		* Sets whether the entry's value is sensitive data. If true, any
		* external components are requested to protect it (especially where
		* persisted).
		* @param isSensitive Whether the entry's value is sensitive data.
		*/
		virtual void setSensitive(bool isSensitive);
	};

	/**
	 * Vector of Entry objects.
	 */
	EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<Entry>;
	typedef Vector<Entry>	EntryVector;

//	* @author John Babbidge
	/**
	* An EntryTemplate is a sub-element of a ConfigurationTemplate
	* or ProfileTemplate. An EntryTemplate represents the requirements, and 
	* labeling required to gather a value from some external source such as 
	* an operator via a UI and, optionally, a default value.
	* <p>
	* Each EntryTemplate contains:
	* <ul>
	* 	<li>A unique identifier</li>
	* 	<li>A label that provides a user interface-friendly description</li>
	* 	<li>A boolean that specifies  whether the entry is read only or not</li>
	* 	<li>A boolean that specifies  whether the entry's value is sensitive data
	* 	If true, any external components are requested to protect it (especially
	* 	where displayed and/or persisted).</li>
	* 	<li>(Optional) A boolean that specifies whether to logically AND or OR the
	* 		Criteria together when more than one exists.</li>
	* 	<li>(Optional) The default entry value. This value specifies  the default
	* 		configuration value.</li>
	* 	<li>(Optional) One or more Criteria that indicate whether this entry is
	* 		required or not based on another entry's value along with an logic indicator
	* 		to determine how to group the multiple criteria.</li>
	* </ul></p>
	* <b>Note: </b><i>Each Criteria in the EntryTemplate's set has an identifier.
	* These identifiers are the keys of other EntryTemplates. If no
	* Criteria are specified, the EntryTemplate is mandatory. If more than one
	* Criteria is specified, the result of {@link #isCriteriaLogicalAND} specifies  whether to logically AND or OR
	* the Criteria together</i> 
	*/
	class AIINTERFACES_API EntryTemplate: public Entry
	{
		bool readOnlyFlag;
		CriteriaVector *entryCriteria;
		bool criteriaLogicalAND;

	public:
		/**
		* Constructor.
		*/
		EntryTemplate(void);

		/**
		* Copy Constructor.
		* @param src Source EntryTemplate object.  A copy of the content of this object is used for new content.
		*/
		EntryTemplate(const EntryTemplate &src);

		/**
		* Assignment Operator overload.
		* @param src Source EntryTemplate object.  A copy of the content of this object is used for new content.
		*/
		virtual EntryTemplate& operator=(const EntryTemplate& src);

		/**
		* Destructor.
		*/
		virtual ~EntryTemplate(void);

		/**
		* Virtual Copy Constructor.
		*/
		virtual EntryTemplate* clone() const	{ return new EntryTemplate(*this); }

		/**
		* Returns the Criteria that must be met in order to enable this Entry.
		* <br><b>Note: </b><i>Only required if Entry is optional based on a certain Criteria.</i>
		* @return The required Criteria  that must be met in order to enable this Entry.
		*/
		virtual CriteriaVector *getEntryCriteria();

		/**
		* Sets the list of Criteria that must be met in order to enable this entry.
		* <p>
		* <b>Note: </b><i>Only required if entry is optional based on a certain criteria.
		* Furthermore, the relationship between the Criteria is determined by boolean
		* passed to {@link #setCriteriaLogicalAND(bool)}.</i></p>
		* @param entryCriteria The Criteria that must be met in order to enable this entry.
		*/
		virtual void setEntryCriteria(CriteriaVector *entryCriteria);

		/**
		* Returns true if the entry criteria should be logically ANDed together;
		* if logically OR'ed, returns false.
		* <br><b>Note: </b><i>Required only if entry Criteria are specified</i>
		* @return true If the entry criteria should be logically ANDed;
		* if should be logically ORed, returns false.
		*/
		virtual bool isCriteriaLogicalAND();

		/**
		* Sets whether the entry criteria should be logically ANDed together or
		* logically ORed.
		* <br><b>Note: </b><i>Only required if entry criteria are specified.</i>
		* @param criteriaLogicalAND true If the entry criteria should be logically ANDed.
		*/
		virtual void setCriteriaLogicalAND(bool criteriaLogicalAND);

		/**
		* Returns true if the element's value is read-only.
		* @return true If the element's value is read-only.
		*/
		virtual bool isReadOnly();

		/**
		* Sets whether the element's value is read-only or not.
		* @param isReadOnly Boolean indicating whether the element's value is read-only or not.
		*/
		virtual void setReadOnly(bool isReadOnly);
	};

	/**
	 * Vector of EntryTemplate objects.
	 */
	EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<EntryTemplate>;
	typedef Vector<EntryTemplate>		EntryTemplateVector;

//====================================================================================================
// CRED DEFINITIONS
//====================================================================================================

	/**
	  * A unique credential identifier consisting of:
 	  * <ul>
 	  * <li>A type</li>
 	  * <li>An identifier</li>
 	  * </ul>
 	  * The combination of a type and an identifier must represent a unique
 	  * Credential.
	 */
class AIINTERFACES_API CredentialId: public Identifier
{
public:
    /**
     * Constructor.
     */
	CredentialId(void);

	/**
     * Constructor.
     * @param id The unique identifier of the credential.
     * @param type The type of the credential.
     */
	CredentialId(TString id, TString type);

	/**
	 * Copy Constructor.
 	 * @param src Source CredentialId object.  A copy of the content of this object is used for new content.
	 */
	CredentialId(const CredentialId &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source CredentialId object.  A copy of the content of this object is used for new content.
	 */
	virtual CredentialId& operator=(const CredentialId& src);

    /**
     * Virtual Copy Constructor.
     */
	virtual CredentialId* clone() const	{ return new CredentialId(*this); }
};

EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<CredentialId>;
typedef Vector<CredentialId>			CredentialIdVector;

// * @author John Babbidge
/**
 * A CredentialElement is a fragment of a Credential.  A Credential contains
 * zero or more CredentialElements.
 * <p>Each CredentialElement consists of:
 * <ul>
 * 	<li>The owning / parent Credential's unique identifier</li>
 * 	<li>The type of element</li>
 * 	<li>A boolean that specifies whether this is a reference to an element or a value</li>
 * 	<li>The value or the reference information</li>
 * 	<li>The type of encoding for the value</li>
 * </ul> </p>
 */
class AIINTERFACES_API CredentialElement : public DLL_OBJECT_BASE
{
    CredentialId *parentCredId;
	TString id;
    TString type;
    TString value;
    TString encoding;
    bool isReferenceFlag;
    bool isEncryptedFlag;

public:
	/**
     * Constructor.
     */
	CredentialElement(void);

    /**
     * Copy Constructor.
 	 * @param src Source CredentialElement object.  A copy of the content of this object is used for new content.
     */
	CredentialElement(const CredentialElement &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source CredentialElement object.  A copy of the content of this object is used for new content.
	 */
	virtual CredentialElement& operator=(const CredentialElement& src);

    /**
     * Destructor.
     */
	virtual ~CredentialElement(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual CredentialElement* clone() const	{ return new CredentialElement(*this); }

    /**
     * Returns the CredentialElement unique identifier. The identifer is
     * unique within the scope of the Credential that contains it.
     * @return The unique identifier of the CredentialElement.
     */
	virtual TString getId();
	
    /**
     * Returns the CredentialElement's parent Credential.
     * @return The CredentialElement's parent Credential.
     */
	virtual CredentialId *getParentCredentialId();
	
	/**
     * Sets the CredentialElement's parent Credential.
	 * @param parentCredId The CredentialElement's parent Credential.
	 */
	virtual void setParentCredentialId(CredentialId *parentCredId);
	
	/**
     * Sets the CredentialElement unique identifier. The identifer is
     * unique within the scope of the Credential that contains it.
	 * @param id The CredentialElement unique identifier.
	 */
	virtual void setId(TString id);
	
    /**
     * Returns true if the CredentialElement is a reference.
     * @return true If the CredentialElement is a reference.
     */
	virtual bool isReference();

	/**
     * Sets whether the CredentialElement is a reference to a value or the
     * actual value.
     * @param isReference Whether the CredentialElement is a reference to a value.
     */
    virtual void setIsReference(bool isReference);

    /**
     * Returns the type of the CredentialElement.
     * @return The type of the CredentialElement.
     */
	virtual TString getType();

	/**
     * Sets type of the CredentialElement. 
	 * @param type Type of the CredentialElement.
	 */
	virtual void setType(TString type);

    /**
     * Returns the value's encoding.
     * @return The value's encoding.
     */
	virtual TString getEncoding();

    /**
     * Returns the value or reference to a protected or external value. 
     * Depending upon its encoding, the value may be a String (either through 
     * encoding or natively so) or other supported object type. See documentation 
     * for valid encoding types and the supported object classes for each encoding.
     * @return The value or reference to a protected or external value.
     */
	virtual TString getValue();

    /**
     * Sets the value's encoding.
     * @param encoding The value encoding style.
     */
	virtual void setEncoding(TString encoding);

    /**
     * Sets the value or reference to a protected or external value.
     * @param value The value or reference to a protected or external value.
     */
	virtual void setValue(TString value);

    /**
     * Returns true if the CredentialElement is encrypted.
     * <p>
     * This attribute specifies  <a href="http://www.w3.org/TR/xmlenc-core/">http://www.w3.org/TR/xmlenc-core/</a>
     * based encryption encoding, and therefore it must be assumed that the
     * value is a String. The encoding specifies  the encoded structure of
     * the decrypted value.</p>
     * @return true If the CredentialElement is encrypted.
     */
    virtual bool isEncrypted();

    /**
     * Sets whether the CredentialElement is encrypted.
     * <p>
     * This attribute specifies  <a href="http://www.w3.org/TR/xmlenc-core/">http://www.w3.org/TR/xmlenc-core/</a>
     * based encryption encoding, and therefore it must be assumed that the
     * value is a String. The encoding specifies the encoded structure of
     * the decrypted value.</p>
     * @param isEncrypted true If the CredentialElement is encrypted.
     */
    virtual void setEncrypted(bool isEncrypted);
};

EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<CredentialElement>;
typedef Vector<CredentialElement> CredentialElementVector;

// * @author John Babbidge
/**
 * A Credential is a collection of {@link AI::CredentialElement}s which, in combination,
 * provide identity proofing services to its owner.
 * <p>
 * A Credential contains:
 * <ul>
 * <li>A unique identifier of the Credential</li>
 * <li>Zero or more {@link CredentialElement}s</li>
 * </ul></p>
 */
class AIINTERFACES_API Credential: public Managed
{
    CredentialId *id;
    CredentialElementVector *credentialElements;
    ConfigurationId *profileId;
	TString description;

public:
	/**
     * Constructor.
     */
	Credential(void);

    /**
     * Copy Constructor.
 	 * @param src Source Credential object.  A copy of the content of this object is used for new content.
     */
	Credential(const Credential &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source Credential object.  A copy of the content of this object is used for new content.
	 */
	virtual Credential& operator=(const Credential& src);

    /**
     * Destructor.
     */
	virtual ~Credential(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual Credential* clone() const	{ return new Credential(*this); }

    /**
     * Sets a provider-formed string representation of the Credential.
     * @param description A provider-formed string representation of the
     * Credential.
     */
	virtual void setDescription(TString description);

    /**
     * Returns a provider-formed string representation of the Credential.
     * @return A provider-formed string representation of the Credential.
     */
	virtual TString getDescription();

    /**
     * Returns the list of {@link CredentialElement}s that comprise the Credential.
     * @return The list of CredentialElements that comprise the Credential.
     */
	virtual CredentialElementVector *getCredentialElements();

    /**
     * Returns the Credential identifier.
     * @return The Credential identifier.
     */
    virtual CredentialId *getId();

    /**
     * Sets the list of CredentialElements that comprise the Credential.
     * @param elements The CredentialElements that comprise the Credential.
     */
	virtual void setCredentialElements(CredentialElementVector* elements);

    /**
     * Sets the Credential identifier.
     * @param id The Credential identifier.
     */
    virtual void setId(CredentialId *id);

    /**
     * Returns the unique identifier of the credential profile used to create
     * this Credential.
     * @return The unique identifier of the credential profile used to create
     * this Credential.
     */
    virtual ConfigurationId *getProfileId();

    /**
     * Sets the unique identifier of the credential profile used to create
     * this credential.
     * @param profileId The unique identifier of the credential profile used to create
     * this Credential.
     */
    virtual void setProfileId(ConfigurationId *profileId);
};

EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<Credential>;
typedef Vector<Credential>		CredentialVector;

//====================================================================================================
// MANAGER DEFINITIONS
//====================================================================================================

	/**
	 * Managed object API base.
	 * Abstract Class, Pure-Virtual Interface
	 */
	class AIINTERFACES_API Manager : public DLL_INTERFACE
	{
	public:
		/**
		 * Returns the API version.
		 * @return The version returned by the implementation.
		 * @throws LocalizedRemoteException If there is a communication error.
		 */
		virtual TString getVersion() = 0;
	};

	/*@internal
	 * A base for producing initializable Manager instances without exposing the
	 * init(...) as part of the Manager's interface.
	 * Abstract Class, Pure-Virtual Interface.
	 */
	class AIINTERFACES_API ManagerInstance //: public virtual DLL_INTERFACE
	{
	public:
	/* @internal
		 * Initializes the manager. 
		 * <p>
		 * The implementer of this interface is typically the very same class and
		 * instance that implements this interface and is being called.</p>
		 * @param parameters The parameters with which to initialize the manager.
		 * @return The initialized Manager.
		 * @throws FactoryException If the initialization fails.
		 */
		virtual Manager* init(ParameterVector* paramaters) = 0;
	};

	/**
	 * Base manager factory.
	 */
	class AIINTERFACES_API ManagerFactory : public DLL_OBJECT_BASE
	{
	public:

	   /**
	    * @internal
		* Retrieves a new manager instance.
		* Defaults to "PROVIDER.properties" for FactoryDefinition.
		* @param provider The provider type.
		* @param parameters The parameters with which to initialize the Manager.
		* @return The initialized Manager.
		* @throws FactoryException If an error occurs during initialization.
		*/
		static HINSTANCE newManagerInstance(TString provider, ParameterVector* parameters);

	   /**
	    * @internal
		* Retrieves a new manager instance.
		* @param factoryDefinition Property file containing provider information 
		* (if not given, defaults to PROVIDER.properties).
		* @param provider Provider identifier.
		* @param parameters Configuration parameters used at initialization.
		* @return Manager the created manager instance.
		* @throws FactoryException If initialization fails.
		*/
		static HINSTANCE newManagerInstance(TString factoryDefinition, TString provider, ParameterVector* parameters);
	};

	class AIINTERFACES_API ManagerFactoryLookupService : public DLL_OBJECT_BASE
	{
	public:
		/* @internal
		 * Retrieves a new Manager API instance factory.
		 * @param factoryDefinition The properties file name to search for. The
		 *            properties file contains provider information.
		 * @param provider The provider to use.
		 * @return The implementation library to be used and definition values added to parameters.
		 * @throws ManagerInstanceNotFoundException 
		 * @throws FactoryException <span class="qqq">qqq--Is this in the right place?</span>If ManagerInstance is not found.
		 */
		HINSTANCE getManagerInstanceImpl(TString factoryDefinition, TString provider, ParameterVector* parameters);

	private:
		TString getFullPath(HINSTANCE hLib, TString filename);
	};

//====================================================================================================
// TX DEFINITIONS
//====================================================================================================

/**
 * The unique identifier of a Transaction. 
 */
class AIINTERFACES_API TransactionId: public Identifier
{
public:
    /**
     * Constructor.
     */
	TransactionId(void);

	/**
     * Constructor.
     * @param id The unique identifier of the transaction. 
     */
	TransactionId(TString id);

	/**
	 * Copy Constructor.
 	 * @param src Source TransactionId object.  A copy of the content of this object is used for new content.
	 */
	TransactionId(const TransactionId &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source TransactionId object.  A copy of the content of this object is used for new content.
	 */
	virtual TransactionId& operator=(const TransactionId& src);

    /**
     * Virtual Copy Constructor.
     */
	virtual TransactionId* clone() const	{ return new TransactionId(*this); }
};

EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<TransactionId>;
typedef Vector<TransactionId>		TransactionIdVector;

// * @author John Babbidge
/**
 * A Credential management construct that communicates a request from the
 * Provider to an external entity to perform an operation required by the
 * Provider to complete an step in a Credential's internal workflow.
 * <p>
 * An example of an ExternalOperation would be the Provider requiring proof
 * of authorization to carry out the requested functionality such as an
 * end-entity self-signing a certificate renewal where the key pair is on-card.</p>
 * An ExternalOperation has the following attributes:
 * <ul>
 * 	<li>The Credential transaction identifier with which it is associated.</li>
 * 	<li>The identifier of the Provider that requested this ExternalOperation.</li>
 * 	<li>The type of the ExternalOperation. This information is used to determine
 * 		how to interpret the script.</li>
 * 	<li>The script the external entity is to execute.</li>
 * 	<li>The parameters required as input by  this ExternalOperation.</li>
 * 	<li>The CredentialElements resulting from the processing of this external
 * 		operation.</li>
 * 	<li>An optional credential profile identifier for ExternalOperations
 * 		generated via batch credential creation. </li>
 * </ul>
 */
class AIINTERFACES_API ExternalOperation : public DLL_OBJECT_BASE
{
	TString externalOperationId;
    TString providerId;
    TransactionId *transactionId;
    TString type;
    TString script;
    ParameterVector *parameters;
    ParameterVector *responseParameters;
    ConfigurationId *profileId;

public:
	/**
     * Constructor.
     */
	ExternalOperation(void);

    /**
     * Copy Constructor.
 	 * @param src Source ExternalOperation object.  A copy of the content of this object is used for new content.
     */
	ExternalOperation(const ExternalOperation &src);

	/**
	 * Assignment Operator overload.
 	 * @param src Source ExternalOperation object.  A copy of the content of this object is used for new content.
	 */
	virtual ExternalOperation& operator=(const ExternalOperation& src);

    /**
     * Destructor.
     */
	virtual ~ExternalOperation(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual ExternalOperation* clone() const	{ return new ExternalOperation(*this); }

   /**
     * Returns the profile identifier of the object being managed.
     * <p>
     * This attribute is set when using batch operations. </p>
     * @return The profile identifier of the object being managed.
     */
    virtual ConfigurationId *getProfileId();

    /**
     * Sets the profile identifier of the object being managed.
     * <p>
     * This attribute is set when using batch operations. </p>
     * @param profileId The profile identifier of the object being managed.
     */
    virtual void setProfileId(ConfigurationId *profileId);

    /**
     * Returns the Parameter(s) resulting from the processing of this
     * ExternalOperation.
     * @return The Parameter(s) resulting from the processing of this
     * ExternalOperation.
     */
    virtual ParameterVector *getResponseParameters();
    
    /**
     * Sets the Parameter(s) resulting from the processing of this
     * ExternalOperation.
     * @param responseParameters The Parameter(s) resulting from the
     * processing of this ExternalOperation.
     */
    virtual void setResponseParameters(ParameterVector *responseParameters);
    
    /**
     * Returns the transaction identifier.
     * @return The transaction identifier.
     */
    virtual TransactionId *getTransactionId();
    
    /**
     * Sets the transaction identifier.
     * @param txId The transaction identifier.
     */
    virtual void setTransactionId(TransactionId *txId);
    
    /**
     * Returns the script for an external entity to execute.
     * @return The script for an external entity to execute.
     */
    virtual TString getScript();
    
    /**
     * Sets the script for an external entity to execute.
     * @param script The directive for an external entity to execute.
     */
    virtual void setScript(TString script);
    
    /**
     * Returns the parameters required as input by  this ExternalOperation.
     * @return The parameters required as input by  this ExternalOperation.
     */
    virtual ParameterVector *getParameters();
    
    /**
     * Sets the parameters required as input by  this ExternalOperation.
     * @param parameters The parameters required as input by  this external
     * operation.
     */
    virtual void setParameters(ParameterVector *parameters);

    /**
     * Returns the identifier of the Provider that requested this 
     * ExternalOperation.
     * @return The ProviderContext identifier.
     */
    virtual TString getProviderContextId();

    /**
     * Sets the identifier of the ProviderContext that requested this
     * ExternalOperation.
     * @param providerId The ProviderContext identifier.
     */
    virtual void setProviderContextId(TString providerId);

    /**
     * Returns the type of the ExternalOperation.
     * @return The type of the ExternalOperation.
     */
    virtual TString getType();

    /**
     * Sets the type of the ExternalOperation.
     * @param type The type of the ExternalOperation.
     */
    virtual void setType(TString type);

    /**
     * Returns the unique identifier of the ExternalOperation.
     * @return The unique identifier of the ExternalOperation.
     */
    virtual TString getExternalOperationId();

    /**
     * Sets the unique identifier of the ExternalOperation.
     * @param externalOperationId The unique identifier of the 
     * ExternalOperation.
     */
    virtual void setExternalOperationId(TString externalOperationId);
};

EXPINTERFACES_TEMPLATE template class AIINTERFACES_API Vector<ExternalOperation>;
typedef Vector<ExternalOperation>	ExternalOperationVector;

} // namespace AI
namespace ActivIdentity = AI;
