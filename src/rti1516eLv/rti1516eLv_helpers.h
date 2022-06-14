#ifndef RTI1516ELV_HELPERS_H
#define RTI1516ELV_HELPERS_H

// #ifdef __cplusplus
// extern "C" {
// #endif

#include "rti1516eLv_EXPORTS.h"
#include <string.h>
#ifdef _OPENRTI
#include <Options.h>
#include <StringUtils.h>
#endif

// RTI specific headers
// #include <RTI/RTIambassadorFactory.h>
// #include <RTI/RTIambassador.h>
// #include <RTI/NullFederateAmbassador.h>
// #include <RTI/time/HLAinteger64TimeFactory.h>
// #include <RTI/time/HLAinteger64Time.h>
// #include <RTI/time/HLAinteger64Interval.h>
// #include <RTI/encoding/BasicDataElements.h>
// #include <RTI/encoding/HLAvariantRecord.h>
// #include <RTI/encoding/HLAfixedRecord.h>

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
    /* data types */ 
    struct testEventData {
        int count1;
        int count2;
    };
    
    /* Test or debug functions */
    EXTERNC DLLEXPORT int testFunc();
    // do not export these function
    EXTERNC MgErr testFireEvent(
        testEventData *value);
    EXTERNC int regObjInstNameResSuccEvent(
        LVUserEventRef *eventRef);

    /* Exception handling to error code */
    int lvErrorCodeFromException(Exception& e);

    /* Data manipulation */
    wstring chararray2wstring(const char charArray[]);
    LStrHandle wstring2LvString(wstring const & theObjectInstanceName);

    /* Attribute Handle Value Map - Helpers*/
    EXTERNC DLLEXPORT  int attrHandleValueMapCreate(AttributeHandleValueMap **attrHandleValueMap);   
    
    EXTERNC DLLEXPORT int attrHandleValueMapDestroy(
        AttributeHandleValueMap *attrHandleValueMap);

    EXTERNC DLLEXPORT int attrHandleValueMapGetHandleByIndex(
        AttributeHandleValueMap* attrHandleValueMap,
        const int index,
        AttributeHandle* attributeHandleOut);

    EXTERNC DLLEXPORT int attrHandleValueMapSetHLAunicodeString(
        AttributeHandleValueMap *attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[]);
    
    EXTERNC DLLEXPORT int attrHandleValueMapGetHLAunicodeString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        LStrHandle &lSh);

    EXTERNC DLLEXPORT int attrHandleValueMapSetHLAASCIIString(
        AttributeHandleValueMap *attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[]);
    
    EXTERNC DLLEXPORT int attrHandleValueMapGetHLAASCIIString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        LStrHandle &lSh);

    EXTERNC DLLEXPORT int attrHandleValueMapSetRaw(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        Octet* data, size_t size);

    EXTERNC DLLEXPORT size_t attrHandleValueMapGetRawSize(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle);

    EXTERNC DLLEXPORT int attrHandleValueMapGetRaw(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        Octet* data);
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
    
    // helpers for complex types
    EXTERNC DLLEXPORT int attrHandleValueMapGetElement_Coords(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        CoordsPtr coords);

    EXTERNC DLLEXPORT int attrHandleValueMapNumElements(
        AttributeHandleValueMap * parHandleValueMap); 

    /* Parameter Handle Value Map - Helpers*/
    EXTERNC DLLEXPORT int parHandleValueMapCreate(
        ParameterHandleValueMap **parHandleValueMap);
    
    EXTERNC DLLEXPORT int parHandleValueMapDestroy(
        ParameterHandleValueMap * parHandleValueMap);

    EXTERNC DLLEXPORT int parHandleValueMapGetHandleByIndex(
        ParameterHandleValueMap* parHandleValueMap,
        const int index,
        ParameterHandle* parameterHandleOut);

    EXTERNC DLLEXPORT int parHandleValueMapSetHLAunicodeString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[]);

    EXTERNC DLLEXPORT int parHandleValueMapGetHLAunicodeString(
        ParameterHandleValueMap const * parHandleValueMap,
        ParameterHandle *parameterHandle,
        LStrHandle &lSh);

    EXTERNC DLLEXPORT int parHandleValueMapSetHLAASCIIString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[]);

    EXTERNC DLLEXPORT int parHandleValueMapGetHLAASCIIString(
        ParameterHandleValueMap* parHandleValueMap,
        ParameterHandle* parameterHandle,
        LStrHandle& lSh);

    EXTERNC DLLEXPORT size_t parHandleValueMapGetRawSize(
        ParameterHandleValueMap const* parHandleValueMap,
        ParameterHandle* parameterHandle);

    EXTERNC DLLEXPORT int parHandleValueMapGetRaw(
        ParameterHandleValueMap const * parHandleValueMap,
        ParameterHandle *parameterHandle,
        Octet *data);

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
    
    /* Attribute handle set - Helpers*/
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