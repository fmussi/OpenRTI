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

    // LV wrapper functions to OpenRTI

    EXTERNC int createRTIambassadorLv(RTIambassador **rtiHandle);

    EXTERNC int connectLv(
        RTIambassador *rtiHandle, 
        const char address[]);

    EXTERNC int createFederationExecutionWithMIMLv(
    RTIambassador *rtiHandle, 
    const char federationExecutionName[],
    const char fomModules[],
    const char mimModule[],
    const char logicalTimeImplementationName[]);

    EXTERNC int disconnectLv(RTIambassador *rtiHandle);
    EXTERNC int destroyRTIambassadorLv(RTIambassador *rtiHandle);
}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif