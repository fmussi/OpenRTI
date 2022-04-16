#ifndef RTI1516ELV_DEFINES_H
#define RTI1516ELV_DEFINES_H

// #ifdef __cplusplus
// extern "C" {
// #endif

#include "rti1516eLv_EXPORTS.h"
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


/* Platform specific LabView includes and lib */
#ifdef _WIN32
	#include "extcode.h"
#else
	#include "cintools/extcode.h"
#endif

using namespace rti1516e;

namespace rti1516eLv
{
    // struct receiveInteractionData {
    //     InteractionClassHandle interactionClassHandle;
    //     ParameterHandleValueMap const *parHandleValueMap;
    //     OrderType sentOrder;
    //     TransportationType theType;
    //     size_t numOfElements;
    // };


#pragma pack(1)

    // variant with Uptr
    // struct receiveInteractionData {
    //     size_t numOfElements;
    //     OrderType sentOrder;
    //     TransportationType theType;
    //     UPtr parHandleValueMap;
    //     InteractionClassHandle interactionClassHandle;
    // };
    // variant with handle
    struct receiveInteractionData {
        size_t numOfElements;
        OrderType sentOrder;
        TransportationType theType;
        ParameterHandleValueMap *parHandleValueMap;
        InteractionClassHandle interactionClassHandle;
    };

#pragma pack()

#pragma pack(1)

    struct reflectAttributeValuesData {
        size_t numOfElements;
        OrderType sentOrder;
        TransportationType theType;
        AttributeHandleValueMap *attrHandleValueMap;
        ObjectInstanceHandle objectClassHandle;
    };

#pragma pack()

}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif