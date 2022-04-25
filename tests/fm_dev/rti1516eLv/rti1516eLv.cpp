// rti1516eLv.cpp : shared library to call OpenRTI function from LV.
// [FM-03/01/2022]: first release
#include <iostream>
#include <assert.h>
#include <exception>
#include <map>
#include <string.h>
#include <thread>
#include <chrono>

// shared lib header
#include "rti1516eLv.h"

// namespaces
using namespace std;
using namespace rti1516e;

namespace rti1516eLv
{   

    LvFederate* oLvFederate; 

    // OpenRTI wrappers

    EXTERNC int regUserEventsLvEx(
        LVUserEventRef *objInstNameResSucc,
        LVUserEventRef *objInstNameResFail,
        LVUserEventRef *receiveInteraction,
        LVUserEventRef *reflectAttributeValues,
        LVUserEventRef *discoverObjectInstance
        )
    {
        
        oLvFederate->regUserEventsLv(
            objInstNameResSucc,
            objInstNameResFail,
            receiveInteraction,
            reflectAttributeValues,
            discoverObjectInstance
        );
        return 0;
    }  

    EXTERNC int createRTIambassadorLvEx(
        RTIambassador **rtiHandle)
    {
        // create LV Federate object
        oLvFederate = new LvFederate();

        try {
        oLvFederate->createRTIambassadorLv();
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int startRTIambassadorLvEx(
        RTIambassador *rtiHandle)
    {
        oLvFederate->startRTIambassadorLvCallback(0);
        return 0;
    }

    EXTERNC int stopRTIambassadorLvEx(
        RTIambassador *rtiHandle)
    {
        oLvFederate->stopRTIambassadorLvCallback(0);
        return 0;
    }

    EXTERNC int connectLvEx(
        RTIambassador *rtiHandle, 
        const char address[])
    {
        wstring host = chararray2wstring(address);
        try {
            oLvFederate->connectLv(host);
            } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int createFederationExecutionWithMIMLvEx(
        RTIambassador *rtiHandle, 
        const char federationExecutionName[],
        const char fomModules[],
        const char mimModule[],
        const char logicalTimeImplementationName[])
    {
        vector<wstring> FOMmoduleUrls;
        wstring mimModuleUrl;
        wstring wFedExecName = chararray2wstring(federationExecutionName);

        FOMmoduleUrls.push_back(OpenRTI::localeToUcs(fomModules));
        mimModuleUrl = OpenRTI::localeToUcs(mimModule);

        try {
            oLvFederate->createFederationExecutionWithMIMLv(wFedExecName,FOMmoduleUrls,mimModuleUrl,L"");
            } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0; 
    }

    EXTERNC int joinFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federateType[],
        const char federationExecutionName[],
        const char additionalFomModules[])
    {
        vector<wstring> FOMmoduleUrls;
        wstring wFedType = chararray2wstring(federateType);
        wstring wFedExecName = chararray2wstring(federationExecutionName);

        FOMmoduleUrls.push_back(OpenRTI::localeToUcs(additionalFomModules));
        try {
            FederateHandle federateHandle = oLvFederate->joinFederationExecutionLv(
            wFedType,
            wFedExecName,
            FOMmoduleUrls
        );
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    // FOM handling 
    
    EXTERNC int getInteractionClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[],
        InteractionClassHandle *intClassHandle)
    {
        wstring wTheName = chararray2wstring(theName);
        try {
            (*intClassHandle) = oLvFederate->getInteractionClassHandleLv(wTheName);        
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int getInteractionClassNameLvEx(
        RTIambassador* rtiHandle,
        InteractionClassHandle* intClassHandle,
        LStrHandle& name)
    {
        try {
            name = wstring2LvString(oLvFederate->getInteractionClassNameLv((*intClassHandle)));
        }
        catch (Exception& e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int getParameterHandleLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle * whichClass,
        const char theName[],
        ParameterHandle *paramHandle)
    {
        wstring wTheName = chararray2wstring(theName);
        try {
            (*paramHandle) = oLvFederate->getParameterHandleLv((*whichClass),wTheName);        
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0; 
    } 

    EXTERNC int getParameterNameLvEx(
        RTIambassador* rtiHandle,
        InteractionClassHandle* whichClass,
        ParameterHandle* theHandle,
        LStrHandle& name)
    {
        try {
            name = wstring2LvString(oLvFederate->getParameterNameLv((*whichClass), (*theHandle)));
        } catch (Exception& e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int getObjectClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[],
        ObjectClassHandle *objectClassHandle)
    {
        wstring wTheName = chararray2wstring(theName);
        try {
            (*objectClassHandle) = oLvFederate->getObjectClassHandleLv(wTheName);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int getObjectClassNameLvEx(
        RTIambassador* rtiHandle,
        ObjectClassHandle* objectClassHandle,
        LStrHandle& name)
    {
        try {
            name = wstring2LvString(oLvFederate->getObjectClassNameLv((*objectClassHandle)));
        }
        catch (Exception& e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int getAttributeHandleLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *whichClass,
        const char theName[],
        AttributeHandle * attributeHandle)
    {
        wstring wTheName = chararray2wstring(theName);
        try {
            (*attributeHandle) = oLvFederate->getAttributeHandleLv(*whichClass,wTheName);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC DLLEXPORT int getAttributeNameLvEx(
        RTIambassador* rtiHandle,
        ObjectClassHandle* whichClass,
        AttributeHandle* theHandle,
        LStrHandle& name)
    {
        try {
            name = wstring2LvString(oLvFederate->getAttributeNameLv((*whichClass), (*theHandle)));
        }
        catch (Exception& e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }


    EXTERNC int reserveObjectInstanceNameLvEx(
        RTIambassador *rtiHandle,
        const char theObjectInstanceName[])
    {
        wstring wObjInstName = chararray2wstring(theObjectInstanceName);
        try {
            oLvFederate->reserveObjectInstanceNameLv(wObjInstName);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }  

    EXTERNC int registerObjectInstanceLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle & theClass,
        const char theObjectInstanceName[],
        ObjectInstanceHandle  *objectInstanceHandle)
    {
        wstring wObjInstName = chararray2wstring(theObjectInstanceName);
        try {
            (*objectInstanceHandle) = oLvFederate->registerObjectInstanceLv(theClass,wObjInstName);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }  

    EXTERNC int updateAttributeValuesLvEx(
        RTIambassador *rtiHandle,
        ObjectInstanceHandle & theObject,
        AttributeHandleValueMap const & theAttributeValues)
    {
        try {
            oLvFederate->updateAttributeValuesLv(theObject,theAttributeValues,VariableLengthData());
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }  

    EXTERNC int sendInteractionLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle * theInteraction,
        ParameterHandleValueMap const & theParameterValues)
    {
        try {
            oLvFederate->sendInteractionLv(*theInteraction,theParameterValues,VariableLengthData());
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;  
    }  

    EXTERNC int subscribeInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theClass,
        bool active)
    {
        try {
            oLvFederate->subscribeInteractionClassLv(*theClass,active);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int publishInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle *theInteraction)
    {
        try {
            oLvFederate->publishInteractionClassLv(*theInteraction);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;

    }

    EXTERNC int subscribeObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *theClass,
        AttributeHandleSet const & attributeList,
        bool active,
        const char updateRateDesignator[])
    {
        try {
            oLvFederate->subscribeObjectClassAttributesLv(*theClass,attributeList);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int publishObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle *theClass,
        AttributeHandleSet const & attributeList)
    {
        try {
            oLvFederate->publishObjectClassAttributesLv(*theClass,attributeList);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0; 
    }

    EXTERNC int resignFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        ResignAction resignAction)
    {
        try {
            oLvFederate->resignFederationExecutionLv(resignAction);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int destroyFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federationExecutionName[])
    {
        wstring wFedExecName = chararray2wstring(federationExecutionName);
    
        try{
            oLvFederate->destroyFederationExecutionLv(wFedExecName);
            //rtiHandle->destroyFederationExecution(wFedExecName);
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }  

    EXTERNC int disconnectLvEx(
        RTIambassador *rtiHandle)
    {
        try {
            oLvFederate->disconnectLv();
        } catch (Exception &e) {
            return lvErrorCodeFromException(e);
        }
        return 0;
    }

    EXTERNC int destroyRTIambassadorLvEx(
        RTIambassador *rtiHandle)
    {
        delete rtiHandle;
        return 0;
    }

} // namespace rti1516eLv