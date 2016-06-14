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
 * Created on  10/28, 2008 - FEG
 * v 1.0 interface definition
 */
#pragma once

#include "aiCCMUtil.h"
#include "aiCCMTcd.h"
#include "aiInterfaces.h"
#include "aiLocalization.h"
#include "aiCOEConstants.h"

#ifdef AICCMAPI_EXPORTS
#define AICCMAPI_API __declspec(dllexport)
#define EXPUTIL_TEMPLATE
#else
#define AICCMAPI_API __declspec(dllimport)
#define EXPUTIL_TEMPLATE extern
#endif // AICCMAPI_EXPORTS

namespace AI
{

/**
 * This exception is thrown when an error occurs in the course of invoking a client plugin.
 */
class AICCMAPI_API ClientOperationException: public LocalizedException
{
public:
	/**
     * Default Constructor.
     */
	ClientOperationException() {};

	/**
     * Copy Constructor.
     */
	ClientOperationException(const LocalizedException &ex):LocalizedException(ex){};
};

/**
 * A client-side operation in the form of a script. The script is executed by
 * examining the script type, selecting the appropriate execution module, and
 * executing the script with its parameters as input.
 */
class AICCMAPI_API ClientOperation : public DLL_OBJECT_BASE {

private:
	TString scriptType;
	TString script;
	ParameterVector *parameters;
	TString id;

public:

	/**
     * Constructor.
     */
	ClientOperation(void);

	/**
     * Constructor.
     */
	ClientOperation(TString id, TString script, TString type, ParameterVector* params);

	/**
     * Copy Constructor.
     */
	ClientOperation(const ClientOperation &src);

	/**
	 * Assignment Operator overload.
	 */
	virtual ClientOperation& operator=(const ClientOperation& src);

    /**
     * Destructor.
     */
	virtual ~ClientOperation(void);

    /**
     * Copy Constructor.
     */
	virtual ClientOperation* clone() const	{ return new ClientOperation(*this); }


	 /**
     * Returns the type of script indicating the appropriate module with which to execute
     * the script.
     * @return The type of script.
     */
	virtual TString							getScriptType(void);

	/**
     * Sets the type of script indicating the appropriate module to execute the
     * script with.
     * @param scriptType The type of script.
     */
	virtual void							setScriptType(TString scriptType);

	/**
     * Returns the script content suitable for execution by the appropriate
     * module indicated by {@link #getScriptType()}.
     * @return The script content.
     */
	virtual TString							getScript(void);

	/**
     * Sets the content of the script.
     * @param script The content of the script.
     */
	virtual void							setScript(TString script);

	/**
     * Returns the parameters for use with the script during processing.
     * @return List of parameters for client consumption.
     */
	virtual ParameterVector*				getParameters(void);

	/**
     * Sets the parameters to be used with the script during processing.
     * @param parameters The parameters to be used with the script.
     */
	virtual void							setParameters(ParameterVector *parameters);

	/**
     * Sets Unique identifier of the client operation instance.
     * @param id  Unique identifier of the client operation instance.
     */
	virtual void							setId(TString id);

	/**
     * Returns Unique identifier of the client operation instance.
     * @return Unique identifier of the client operation instance.
     */
	virtual TString							getId(void);
};

/**
 * Vector of ClientOperation objects.
 */
EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<ClientOperation>;
typedef Vector<ClientOperation>	ClientOperationVector;

/**
 * A client-side response that represent the results of executing a client operation
 * script.
 */
class AICCMAPI_API ClientOperationResponse : public DLL_OBJECT_BASE {

private:
	ParameterVector* parameters;
	TString id;

public:

	/**
     * Constructor.
     */
	ClientOperationResponse(void);

	/**
     * Constructor.
     */
	ClientOperationResponse(TString id, ParameterVector* params);

	/**
     * Copy Constructor.
     */
	ClientOperationResponse(const ClientOperationResponse &src);

	/**
	 * Assignment Operator overload.
	 */
	virtual ClientOperationResponse& operator=(const ClientOperationResponse& src);

    /**
     * Destructor.
     */
	virtual ~ClientOperationResponse(void);

    /**
     * Virtual Copy Constructor.
     */
	virtual ClientOperationResponse* clone() const	{ return new ClientOperationResponse(*this); }

	/**
     * Returns the response parameters from the execution of the client
     * operation script.
     * @return The response parameters from the execution of the client
     * operation script.
     */
	virtual ParameterVector*		getResponseParameters(void);

	/**
     * Sets the response parameters from the execution of the client operation
     * script.
     * @param responseParameters The response parameters from the execution of
     * the client operation script.
     */
	virtual void					setResponseParameters(ParameterVector *responseParameters);

	/**
     * Sets the unique identifier of the client operation instance.
     * @param id  The unique identifier of the client operation instance.
     */
	virtual void					setId(TString id);

	/**
     * Returns the unique identifier of the client operation instance.
     * @return The unique identifier of the client operation instance.
     */
	virtual TString					getId(void);
};

/**
 * Vector of ClientOperationResponse objects.
 */
EXPUTIL_TEMPLATE template class AICCMAPI_API Vector<ClientOperationResponse>;
typedef Vector<ClientOperationResponse>	ClientOperationResponseVector;

/**
 * Client operation execution module interface. This interface must be
 * implemented in order to support embedding the functionality in the
 * SyncManager client to handle client operation scripting.
 */
class AICCMAPI_API ClientOperationExecutor : public DLL_INTERFACE {

public:

	/**
     * Constructor.
     */
	ClientOperationExecutor ()	{}

	/**
     * Destructor.
     */
	virtual ~ClientOperationExecutor(void) {}

/**
 * Initializes the script interpreter plug-in.
 * @param platformClass The class of the security module platform as passed
 * to synchronize.
 * @param accessParameters The parameters required to access the security
 * module as passed to synchronize.
 * @param runtimeParameters The runtime parameters for the synchronization
 * as passed to synchronize.
 * @param listener A SyncListener implementation to which script
 * interpreter plug-in status information is to be reported.
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual void							initialize(TString platformClass, ParameterVector *accessParameters, ParameterVector *runtimeParameters, SyncListener *listener) = 0;

/**
 * Executes the supplied client operation and returns a response in
 * corresponding client operation response.
	 * <p><b>Note: </b><i>Failures that occur during execution of client
 * operations should be returned as errors inside of the relevant
 * ClientOperationResponse. Errors must be returned using the fixed
 * Parameter key: "error".</i></p>
 * @param clientOperation The client operation to be executed.
 * @return The responses for each of the supplied client operations.
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual ClientOperationResponse			*execute(ClientOperation *clientOperation) = 0;

/**
 * Breaks down the plug-in.
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual void							terminate(void) = 0;

/**
	 * Return the plugin version
	 * @return The script interpreter version.
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual TString							getVersion(void) = 0;

/**
	* Return the type of the plugin interpreter
	* @return The script interpreter supported type.
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual TString							getType(void) = 0;

/**
	* Return the name of the plugin interpreter
	* @return The script interpreter name.
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual TString							getName(void) = 0;

/**
 * Return free-form information description of the plugin interpreter
 * @return the description of plugin
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual TString							getDescription (void) = 0;

/**
 * Return the list of scripts suported by this interpreter
 * @return An array of supported scripts
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual TStringVector*					getSupportedScripts(void) = 0;

/**
 * Identifies whether the plugin is ready for execution. 
 * Performs a validation check on the plugin to ensure it is functioning correctly.
 * @return TRUE if the plugin has all necessary context for executing else FALSE
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual bool							validate(void) = 0;

/**
 * Identifies whether a script can be executed locally by this plugin or the script requires a call to the server.
 * @param script. The action type to execute as passed by the syncManager synchronizeAction method.
 * @return TRUE if the script can be executed locally by this plugin else FALSE
 * @throws ClientOperationException If an internal failure occurs.
 */
	virtual bool							isLocal(TString script) = 0;
};

} // namespace AI
namespace ActivIdentity = AI;

/**
 * The factory method implemented by the client plugin libraries.
 * @param parameters The client operation executor parameters.
 * @return the ClientOperationExecutor object to be used by the synchronization manager
 */
//extern "C" AI::ClientOperationExecutor *newInstance(AI::ParameterVector *parameters);
