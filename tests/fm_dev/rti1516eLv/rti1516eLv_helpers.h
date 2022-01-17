#ifndef RTI1516ELV_HELPERS_H
#define RTI1516ELV_HELPERS_H

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
    // Helper functions

    // EXTERNC  int attrHandleValueMapCreate(
    //     AttributeHandleValueMap **attrHandleValueMap);
    EXTERNC  int attrHandleValueMapCreate(AttributeHandleValueMap **attrHandleValueMap);   
    EXTERNC int attrHandleValueMapDestroy(
        AttributeHandleValueMap *attrHandleValueMap);
    EXTERNC int attrHandleValueMapAddElementString(
        AttributeHandleValueMap *attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[]);

    // EXTERNC int parHandleValueMapCreate(
    //     ParameterHandleValueMap **parHandleValueMap);
    EXTERNC int parHandleValueMapCreate(ParameterHandleValueMap **parHandleValueMap);
    EXTERNC int attrHandleValueMapDestroy(
        AttributeHandleValueMap * attrHandleValueMap);
    EXTERNC int parHandleValueMapAddElementString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[]);

    // EXTERNC int attrHandleSetCreate(
    //     AttributeHandleSet **attrHandleSet);
    EXTERNC int attrHandleSetCreate(AttributeHandleSet **attrHandleSet);

    EXTERNC int attrHandleSetInsert(
        AttributeHandleSet * attrHandleSet,
        AttributeHandle * attrHandle
    );
    EXTERNC int attrHandleSetDestroy(
        AttributeHandleSet * attrHandleSet
    );


}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif