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

using namespace rti1516e;

namespace rti1516eLv
{
    // Test or debug functions
    EXTERNC int testFunc();
    EXTERNC MgErr testFireEvent(int value);
    EXTERNC int regObjInstNameResSuccEvent(LVUserEventRef *eventRef);

    EXTERNC int regLvUserEvents(
        LVUserEventRef *objInstNameResSucc,
        LVUserEventRef *objInstNameResFail);
    // LV wrapper functions to OpenRTI

    EXTERNC int createRTIambassadorLvEx(RTIambassador **rtiHandle);

    // LV specific wrappers to control callback processing
    EXTERNC int startRTIambassadorLvEx(RTIambassador *rtiHandle);
    EXTERNC int stopRTIambassadorLvEx(RTIambassador *rtiHandle);

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
        InteractionClassHandle whichClass,
        const char theName[],
        ParameterHandle *paramHandle);

    EXTERNC int resignFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        ResignAction resignAction);

    EXTERNC int destroyFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federationExecutionName[]);

    EXTERNC int disconnectLvEx(RTIambassador *rtiHandle);
    EXTERNC int destroyRTIambassadorLvEx(RTIambassador *rtiHandle);
}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif