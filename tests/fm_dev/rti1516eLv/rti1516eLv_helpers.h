#ifndef RTI1516ELV_HELPERS_H
#define RTI1516ELV_HELPERS_H

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

using namespace std;
using namespace rti1516e;

namespace rti1516eLv
{
        // data types
    struct testEventData {
        int count1;
        int count2;
    };
    
    // internal functions
    int lvErrorCodeFromException(Exception &e);
    //wstring chararray2wstring(const char charArray[]);

    // Test or debug functions
    EXTERNC DLLEXPORT int testFunc();
    // do not export these function
    EXTERNC MgErr testFireEvent(
        testEventData *value);
    EXTERNC int regObjInstNameResSuccEvent(
        LVUserEventRef *eventRef);
    
    
    // Helper functions
    wstring chararray2wstring(const char charArray[]);

    LStrHandle wstring2LvString(wstring const & theObjectInstanceName);

    // EXTERNC  int attrHandleValueMapCreate(
    //     AttributeHandleValueMap **attrHandleValueMap);
    EXTERNC DLLEXPORT  int attrHandleValueMapCreate(AttributeHandleValueMap **attrHandleValueMap);   
    
    EXTERNC DLLEXPORT int attrHandleValueMapDestroy(
        AttributeHandleValueMap *attrHandleValueMap);

    EXTERNC DLLEXPORT int attrHandleValueMapSetElementString(
        AttributeHandleValueMap *attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[]);
    
    EXTERNC DLLEXPORT int attrHandleValueMapGetElementString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        LStrHandle &lSh);

    EXTERNC DLLEXPORT int attrHandleValueMapSetElementInt32(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        const int iElem);

    EXTERNC DLLEXPORT int attrHandleValueMapGetElementInt32(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        int& iElem);

    EXTERNC DLLEXPORT int attrHandleValueMapSetElementDouble(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        const double dElem);

    EXTERNC DLLEXPORT int attrHandleValueMapGetElementDouble(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        double& dElem);

    EXTERNC DLLEXPORT int attrHandleValueMapNumElements(
        AttributeHandleValueMap * parHandleValueMap); 

    // EXTERNC int parHandleValueMapCreate(
    //     ParameterHandleValueMap **parHandleValueMap);
    EXTERNC DLLEXPORT int parHandleValueMapCreate(
        ParameterHandleValueMap **parHandleValueMap);
    
    EXTERNC DLLEXPORT int parHandleValueMapDestroy(
        ParameterHandleValueMap * parHandleValueMap);
    
    EXTERNC DLLEXPORT int parHandleValueMapSetElementString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[]);

    EXTERNC DLLEXPORT int parHandleValueMapGetElementString(
        ParameterHandleValueMap const * parHandleValueMap,
        ParameterHandle *parameterHandle,
        LStrHandle &lSh);

    EXTERNC DLLEXPORT int parHandleValueMapSetElementInt32(
        ParameterHandleValueMap* attrHandleValueMap,
        ParameterHandle* attributeHandle,
        const int iElem);

    EXTERNC DLLEXPORT int parHandleValueMapGetElementInt32(
        ParameterHandleValueMap* attrHandleValueMap,
        ParameterHandle* attributeHandle,
        int& iElem);

    EXTERNC DLLEXPORT int parHandleValueMapSetElementDouble(
        ParameterHandleValueMap* attrHandleValueMap,
        ParameterHandle* attributeHandle,
        double const dElem);

    EXTERNC DLLEXPORT int parHandleValueMapGetElementDouble(
        ParameterHandleValueMap* attrHandleValueMap,
        ParameterHandle* attributeHandle,
        double& dElem);

    EXTERNC DLLEXPORT int parHandleValueMapNumElements(
        ParameterHandleValueMap * parHandleValueMap);   
     
    // atrribute handle set
    
    EXTERNC DLLEXPORT int attrHandleSetCreate(
        AttributeHandleSet **attrHandleSet);

    EXTERNC DLLEXPORT int attrHandleSetInsert(
        AttributeHandleSet * attrHandleSet,
        AttributeHandle * attrHandle
    );

    EXTERNC DLLEXPORT int attrHandleSetNumElements(
        AttributeHandleSet * attrHandleSet);

    EXTERNC DLLEXPORT int attrHandleSetDestroy(
        AttributeHandleSet * attrHandleSet
    );

}

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif