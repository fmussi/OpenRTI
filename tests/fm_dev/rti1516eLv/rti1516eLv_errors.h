#ifndef RTI1516ELV_ERRORS_H
#define RTI1516ELV_ERRORS_H

// #ifdef __cplusplus
// extern "C" {
// #endif

//#include "rti1516eLv_EXPORTS.h"
// #include <Options.h>
// #include <StringUtils.h>

// RTI specific headers
// #include <RTI/RTIambassadorFactory.h>
// #include <RTI/RTIambassador.h>
// #include <RTI/NullFederateAmbassador.h>
// #include <RTI/time/HLAinteger64TimeFactory.h>
// #include <RTI/time/HLAinteger64Time.h>
// #include <RTI/time/HLAinteger64Interval.h>
// #include <RTI/encoding/BasicDataElements.h>

// #include "cintools/extcode.h"

// using namespace rti1516e;
#define LV_ERROR_RANGE_OFFSET   -500000
#define LV_WARNING_RANGE_OFFSET 500000

//Ambassador specific error codes
#define LV_ERROR_RTI_INTERNAL_ERROR   LV_ERROR_RANGE_OFFSET
#define LV_ERROR_CONNECTION_FAILED  LV_ERROR_RANGE_OFFSET-1
#define LV_ERROR_INVALID_LOCAL_SETTINGS_DESIGNATOR  LV_ERROR_RANGE_OFFSET-2
#define LV_ERROR_UNSUPPORTED_CALLBACK_MODEL  LV_ERROR_RANGE_OFFSET-3
#define LV_ERROR_ALREADY_CONNECTED  LV_ERROR_RANGE_OFFSET-4
#define LV_ERROR_CALL_NOT_ALLOWED_FROM_WITHIN_CALLBACK  LV_ERROR_RANGE_OFFSET-5
//#define LV_ERROR_INVALID_LOCAL_SETTINGS_DESIGNATOR  LV_ERROR_RANGE_OFFSET-2

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif