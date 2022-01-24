#ifndef RTI1516ELV_H
#define RTI1516ELV_H

// #ifdef __cplusplus
// extern "C" {
// #endif

//#include "rti1516eLv_EXPORTS.h"
#include <Options.h>
#include <StringUtils.h>

// RTI specific headers
#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
#include <RTI/NullFederateAmbassador.h>
#include <RTI/time/HLAinteger64TimeFactory.h>
#include <RTI/time/HLAinteger64Time.h>
#include <RTI/time/HLAinteger64Interval.h>
#include <RTI/encoding/BasicDataElements.h>

#include "cintools/extcode.h"

#include "rti1516eLv_defines.h"
#include "rti1516eLv_helpers.h"
#include "rti1516eLv_errors.h"
#include "rti1516eLv_Federate.h"

using namespace rti1516e;

namespace rti1516eLv
{
    EXTERNC int regUserEventsLvEx(
        LVUserEventRef *objInstNameResSucc,
        LVUserEventRef *objInstNameResFail,
        LVUserEventRef *receiveInteraction,
        LVUserEventRef *reflectAttributeValues
        );
    // LV wrapper functions to OpenRTI

    EXTERNC int createRTIambassadorLvEx(
        RTIambassador **rtiHandle);

    // LV specific wrappers to control callback processing
    EXTERNC int startRTIambassadorLvEx(
        RTIambassador *rtiHandle);
    EXTERNC int stopRTIambassadorLvEx(
        RTIambassador *rtiHandle);

    EXTERNC int connectLvEx(
        RTIambassador *rtiHandle, 
        const char address[]);

    EXTERNC int createFederationExecutionWithMIMLvEx(
    RTIambassador *rtiHandle, 
    const char federationExecutionName[],
    const char fomModules[],
    const char mimModule[],
    const char logicalTimeImplementationName[]);

    EXTERNC int joinFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federateType[],
        const char federationExecutionName[],
        const char additionalFomModules[]);

    EXTERNC int getInteractionClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[],
        InteractionClassHandle *intClassHandle);

    EXTERNC int getParameterHandleLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *whichClass,
        const char theName[],
        ParameterHandle *paramHandle);

    EXTERNC int getObjectClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[],
        ObjectClassHandle *objectClassHandle);
    
    EXTERNC int getAttributeHandleLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *whichClass,
        const char theName[],
        AttributeHandle * attributeHandle);

    EXTERNC int reserveObjectInstanceNameLvEx(
        RTIambassador *rtiHandle,
        const char theObjectInstanceName[]);

    EXTERNC int registerObjectInstanceLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle & theClass,
        const char theObjectInstanceName[],
        ObjectInstanceHandle *objectInstanceHandle);

    EXTERNC int updateAttributeValuesLvEx(
        RTIambassador *rtiHandle,
        ObjectInstanceHandle & theObject,
        AttributeHandleValueMap const & theAttributeValues);

    EXTERNC int sendInteractionLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theInteraction,
        ParameterHandleValueMap const & theParameterValues);

    EXTERNC int subscribeInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theClass,
        bool active);
    
    EXTERNC int publishInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theInteraction);

    EXTERNC int subscribeObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *theClass,
        AttributeHandleSet const & attributeList,
        bool active,
        const char updateRateDesignator[]);

    EXTERNC int publishObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *theClass,
        AttributeHandleSet const & attributeList);

    EXTERNC int resignFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        ResignAction resignAction);

    EXTERNC int destroyFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federationExecutionName[]);

    EXTERNC int disconnectLvEx(
        RTIambassador *rtiHandle);

    EXTERNC int destroyRTIambassadorLvEx(
        RTIambassador *rtiHandle);
}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif