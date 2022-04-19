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
    // replaces size_t with uint32_t for multi bitness compatibility
    // replaced ParameterHandleValueMap and InteractionClassHandle with uint64
    struct receiveInteractionData {
        uint32_t numOfElements;
        OrderType sentOrder;
        TransportationType theType;
#ifdef _WIN32
#ifdef _WIN64
        ParameterHandleValueMap*parHandleValueMap;
        InteractionClassHandle interactionClassHandle;
#else
        // pad with 32bit
        ParameterHandleValueMap* parHandleValueMap;
        uint32_t _pad1;
        InteractionClassHandle interactionClassHandle;
        uint32_t _pad2;
#endif
#endif
    };

#pragma pack()

#pragma pack(1)

// replaces size_t with uint32_t for multi bitness compatibility
// replaced AttributeHandleValueMap and ObjectInstanceHandle with uint64
    struct reflectAttributeValuesData {
        uint32_t numOfElements;
        OrderType sentOrder;
        TransportationType theType;
        uInt64 *attrHandleValueMap;
        uInt64 objectClassHandle;
    };

#pragma pack()

}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif