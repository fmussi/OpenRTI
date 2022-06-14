#ifndef RTI1516ELV_H
#define RTI1516ELV_H

// #ifdef __cplusplus
// extern "C" {
// #endif

//#include "rti1516eLv_EXPORTS.h"
#ifdef _OPENRTI
#include <Options.h>
#include <StringUtils.h>
#else
#include "rtiSimpleStringUtil.h"
#endif  

// RTI specific headers
// #include <RTI/RTIambassadorFactory.h>
// #include <RTI/RTIambassador.h>
// #include <RTI/NullFederateAmbassador.h>
// #include <RTI/time/HLAinteger64TimeFactory.h>
// #include <RTI/time/HLAinteger64Time.h>
// #include <RTI/time/HLAinteger64Interval.h>
// #include <RTI/encoding/BasicDataElements.h>

/* Platform specific LabView includes and lib */
#ifdef _WIN32
	#include "extcode.h"
    #define DLLEXPORT __declspec(dllexport)
#else
	#define DLLEXPORT 
    #include "cintools/extcode.h"
#endif


#include "rti1516eLv_defines.h"
#include "rti1516eLv_helpers.h"
#include "rti1516eLv_errors.h"
#include "rti1516eLv_Federate.h"

using namespace rti1516e;

namespace rti1516eLv
{
    EXTERNC DLLEXPORT int regUserEventsLvEx(
        LVUserEventRef *objInstNameResSucc,
        LVUserEventRef *objInstNameResFail,
        LVUserEventRef *receiveInteraction,
        LVUserEventRef *reflectAttributeValues,
        LVUserEventRef *discoverObjectInstance,
        LVUserEventRef *removeObjectInstance,
        LVUserEventRef *reportFederationExecutions
        );
    // LV wrapper functions to OpenRTI

    EXTERNC DLLEXPORT int createRTIambassadorLvEx(
        RTIambassador **rtiHandle);

    // LV specific wrappers to control callback processing
    EXTERNC DLLEXPORT int startRTIambassadorLvEx(
        RTIambassador *rtiHandle);
    EXTERNC DLLEXPORT int stopRTIambassadorLvEx(
        RTIambassador *rtiHandle);

    EXTERNC DLLEXPORT int connectLvEx(
        RTIambassador *rtiHandle, 
        const char localSettingsDesignator[]);

    EXTERNC DLLEXPORT int createFederationExecutionWithMIMLvEx(
    RTIambassador *rtiHandle, 
    const char federationExecutionName[],
    const char fomModules[],
    const char mimModule[],
    const char logicalTimeImplementationName[]);

    EXTERNC DLLEXPORT int listFederationExecutionsLvEx(RTIambassador *rtiHandle);

    EXTERNC DLLEXPORT int joinFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federateType[],
        const char federationExecutionName[],
        const char additionalFomModules[]);

    EXTERNC DLLEXPORT int getInteractionClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[],
        InteractionClassHandle *intClassHandle);

    EXTERNC DLLEXPORT int getInteractionClassNameLvEx(
        RTIambassador* rtiHandle,
        InteractionClassHandle* intClassHandle,
        LStrHandle& name);

    EXTERNC DLLEXPORT int getParameterHandleLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *whichClass,
        const char theName[],
        ParameterHandle *paramHandle);

    EXTERNC DLLEXPORT int getParameterNameLvEx(
        RTIambassador* rtiHandle,
        InteractionClassHandle* whichClass,
        ParameterHandle* theHandle,
        LStrHandle& name);

    EXTERNC DLLEXPORT int getObjectClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[],
        ObjectClassHandle *objectClassHandle);

    EXTERNC DLLEXPORT int getObjectClassNameLvEx(
        RTIambassador* rtiHandle,
        ObjectClassHandle* objectClassHandle,
        LStrHandle& name);
    
    EXTERNC DLLEXPORT int getAttributeHandleLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *whichClass,
        const char theName[],
        AttributeHandle * attributeHandle);

    EXTERNC DLLEXPORT int getAttributeNameLvEx(
        RTIambassador* rtiHandle,
        ObjectClassHandle* whichClass,
        AttributeHandle* theHandle,
        LStrHandle& name);

    EXTERNC DLLEXPORT int reserveObjectInstanceNameLvEx(
        RTIambassador *rtiHandle,
        const char theObjectInstanceName[]);

    EXTERNC DLLEXPORT int registerObjectInstanceLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle & theClass,
        const char theObjectInstanceName[],
        ObjectInstanceHandle *objectInstanceHandle);

    EXTERNC DLLEXPORT int subscribeInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theClass,
        bool active);
    
    EXTERNC DLLEXPORT int publishInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theInteraction);

    EXTERNC DLLEXPORT int subscribeObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *theClass,
        AttributeHandleSet const & attributeList,
        bool active,
        const char updateRateDesignator[]);

    EXTERNC DLLEXPORT int publishObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *theClass,
        AttributeHandleSet const & attributeList);

    EXTERNC DLLEXPORT int updateAttributeValuesLvEx(
        RTIambassador *rtiHandle,
        ObjectInstanceHandle & theObject,
        AttributeHandleValueMap const & theAttributeValues);

    EXTERNC DLLEXPORT int sendInteractionLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theInteraction,
        ParameterHandleValueMap const & theParameterValues);

    EXTERNC DLLEXPORT int resignFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        ResignAction resignAction);

    EXTERNC DLLEXPORT int destroyFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federationExecutionName[]);

    EXTERNC DLLEXPORT int disconnectLvEx(
        RTIambassador *rtiHandle);

    EXTERNC DLLEXPORT int destroyRTIambassadorLvEx(
        RTIambassador *rtiHandle);
}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif