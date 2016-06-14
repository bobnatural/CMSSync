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

#include "aiInterfaces.h"

#ifdef AILOCALIZATION_EXPORTS
#define AILOCALIZATION_API __declspec(dllexport)
#define EXPLOCALIZATION_TEMPLATE
#else
#define AILOCALIZATION_API __declspec(dllimport)
#define EXPLOCALIZATION_TEMPLATE extern
#endif

namespace AI
{
	/**
	 * This exception should form the base of all <b>local</b> exceptions that need to produce
	 * localized error messages. No human-readable messages are passed into the
	 * exception constructors, just error codes. The {@link #getLocalizedMessage()}
	 * method then provides the fully localized and human-readable error message.
	 */
	class AILOCALIZATION_API LocalizedException : public DLL_OBJECT_BASE //: std::exception
	{
		TString message;
		TString additionalMessage;
		TString localizedMessage;
		TString unlocalizedMessage;
		TString errorCode;
		TStringVector * parameters;

	public:
		/**
		 * Constructor.
		 */
		LocalizedException(void);

		/**
		 * Copy Constructor.
	 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
		 */
		LocalizedException(const LocalizedException &ex);

		/**
		 * Assignment Operator overload.
	 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
		 */
		virtual LocalizedException& operator=(const LocalizedException& ex);

		/**
		 * Destructor.
		 */
		virtual ~LocalizedException(void);

		/**
		 * Returns the message with localization.
		 * <p><b>Note: </b><i>By default, an attempt is made to localize all content.</i></p>
	     * @return The localized message.
		 */
		virtual	TString	getMessage(void);

		/**
		 * Returns any additional information included in the exception. This
		 * content is not localized.
		 * @return Any additional information included in the exception.
		 */
		virtual	TString	getAdditionalMessage(void);

		/**
		 * Returns the message with localization.
		 * @return The localized message.
		 */
		virtual	TString	getLocalizedMessage(void);

		/**
		 * Returns the message with localization explicitly turned off.  By default, it is turned on.
		 * @return The unlocalized message.
		 */
		virtual	TString	getUnlocalizedMessage(void);

		/**
		 * Returns the error code for this exception.
		 * @return The error code for this exception.
		 */
		virtual	TString	getErrorCode(void);

		/**
		 * Returns the parameters to be used during localization.
		 * @return The parameters to be used during localization.
		 */
		virtual	TStringVector*	getParameters(void);

		/**
		 * Sets the error code for this exception.
		 * @param message Error code for this exception.
		 */
		virtual	void	setMessage(TString message);

		/**
		 * Sets any additional information included in the exception. This
		 * content is not localized.
	 	 * @param additionalMessage An unlocalized message that provides additional
	 	 * detail about the error.
		 */
		virtual	void	setAdditionalMessage(TString additionalMessage);

		/**
		 * Sets information included in the exception.  This content is localized.
		 * @param localizedMessage A localized message that provides detail about the error.
		 */
		virtual	void	setLocalizedMessage(TString localizedMessage);

		/**
		 * Sets the message with localization explicitly turned off.  By default, it is turned on.
		 * @param message The unlocalized message.
		 */
		virtual	void	setUnlocalizedMessage(TString message);

		/**
		 * Sets the error code for this exception.
		 * @param errorCode The error code for this exception.
		 */
		virtual	void	setErrorCode(TString errorCode);

		/**
		 * Sets the parameters to be used during localization.
		 * @param parameters The parameters to be used during localization.
		 */
		virtual	void	setParameters(TStringVector * parameters);
	};

	/**
	 * Thrown when there is an error in fetching a manager instance.
	 * <p>
	 * A sub-class is thrown that corresponds with the level at which the error occurs.</p>
	 */
	class AILOCALIZATION_API FactoryException : public LocalizedException
	{
	public:
		/**
		 * Default Constructor.
		 */
		FactoryException(void) {};

		/**
		 * Copy Constructor.
	 	 * @param ex Source LocalizedException object.  A copy of the content of this object is used for new content.
		 */
		FactoryException(const LocalizedException &ex): LocalizedException(ex){};
	};

	/**
	 * Thrown when there are errors during instantiation of the Class returned by the LookupService.
	 * Some possible reasons for instantiation errors 
	 * <ul>
	 *   <li>InstantiationException</li>
	 *   <li>IllegalAccessException</li>
	 *   <li>The Class returned did not represent an a class that implemented the ManagerInstance</li>
	 * </ul>
	 */
	class AILOCALIZATION_API ManagerInstanceInstantiationException : public FactoryException
	{
	public:
		/**
		 * Default Constructor.
		 */
		ManagerInstanceInstantiationException(void) {};

		/**
		 * Copy Constructor.
	 	 * @param ex Source ManagerInstanceInstantiationException object.  A copy of the content of this object is used for new content.
		 */
		ManagerInstanceInstantiationException(const FactoryException &ex): FactoryException(ex){};
	};

	/**
	 * Thrown when a specific implementation of the ManagerFactory does not receive 
	 * a specific implementation of the Manager interface.
	 */
	class AILOCALIZATION_API InvalidManagerSpecificImplException : public FactoryException
	{
	public:
		/**
		 * Default Constructor.
		 */
		InvalidManagerSpecificImplException(void) {};

		/**
		 * Copy Constructor.
	 	 * @param ex Source InvalidManagerSpecificImplException object.  A copy of the content of this object is used for new content.
		 */
		InvalidManagerSpecificImplException(const FactoryException &ex): FactoryException(ex){};
	};

	/**
	* Thrown when the class defining a ManagerInstance is not found.
	* A ManagerInstance might not be found for the following reasons:
	* <ul>
	*   <li>No properties files were found corresponding to factoryDefinition</li>
	*   <li>None of the properties files that meeted the factoryDefinition had the provider as a key</li>
	* </ul>
	*/
	class AILOCALIZATION_API ManagerInstanceNotFoundException : public FactoryException
	{
	public:
		/**
		 * Default Constructor.
		 */
		ManagerInstanceNotFoundException(void) {};

		/**
		 * Copy Constructor.
	 	 * @param ex Source ManagerInstanceNotFoundException object.  A copy of the content of this object is used for new content.
		 */
		ManagerInstanceNotFoundException(const FactoryException &ex): FactoryException(ex){};
	};

}	// namespace AI
