// rti1516eLv.cpp : shared library to call OpenRTI function from LV.
// [FM-03/01/2022]: first release
#include <iostream>
#include <assert.h>
#include <exception>
#include <map>
//#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <thread>

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

// shared lib header
#include "rti1516eLv.h"

// namespaces
using namespace std;
using namespace rti1516e;


namespace rti1516eLv
{   
    int _rtiCount = 0;
    //map<int,auto_ptr<RTIambassador>> _rtiMap;

    class LvFederate : public NullFederateAmbassador {
    private:
        InteractionClassHandle _iMessageId;
        ParameterHandle _pTextId;
        ParameterHandle _pSenderId;
        ObjectClassHandle _oParticipantId;
        ObjectInstanceHandle _iParticipantHdl;
        AttributeHandle _aNameId;
        AttributeHandleSet _aHandleSet;
        ParameterHandleValueMap _pHandleValueMap;
        AttributeHandleValueMap _aHandleValueMap;

        wstring _username;
        wstring _message;

        // bool _reservationSucceeded;
        // bool _reservationComplete;
    public:
        LvFederate() {}

        ~LvFederate() throw() {}

        void core_cb_consumer() {

        }
        // map<ObjectInstanceHandle, Participant> _knownObjects;	
        static void th_cb_consumer(void *data) {
            //ChatCCFederate *ccInstance = static_cast<ChatCCFederate *>(data);
            //ccInstance->ChatUI();
        }

    };
    
    EXTERNC int testFunc()
    {
        return 12345;
    }

    EXTERNC int createRTIambassadorLv(RTIambassador **rtiHandle)
    {
        auto_ptr<RTIambassador> _rtiAmbassador;

        try {
            auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
            _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();
            
            //return sizeof(_rtiAmbassador);
            //return static_cast<void*>(_rtiAmbassador.get());

            *rtiHandle = _rtiAmbassador.release();
            _rtiCount++;
            return _rtiCount;
            

        }
        catch (RTIinternalError &e) {
            e.what();
        }
    }

    EXTERNC int connectLv(
        RTIambassador *rtiHandle, 
        const char address[])
    {
        LvFederate oLvFed;
        auto_ptr<RTIambassador> _rtiAmbassador;
        string addrStr(address);
        wstring host;
        host.assign(addrStr.begin(),addrStr.end());
        //RTIambassador *_rtiAmbassador = static_cast<RTIambassador*>(_rtiAmbassadorIn);

        try {
            wstring localSettingsDesignator(L"rti://" + host);
            rtiHandle->connect(oLvFed,HLA_EVOKED,localSettingsDesignator);
            //_rtiAmbassador->connect(oLvFed,HLA_EVOKED,localSettingsDesignator);

        }
        catch (CouldNotOpenFDD &fdde) {
         wcerr << fdde.what() << endl;
         wcerr.flush();
        } catch (Exception &e) {
         wcerr << e.what() << endl;
         wcerr.flush();
        } catch (exception &e2) {
         wcerr << e2.what() << endl;
         wcerr.flush();
        }
    }

    EXTERNC int createFederationExecutionWithMIMLv(
        RTIambassador *rtiHandle, 
        const char federationExecutionName[],
        const char fomModules[],
        const char mimModule[],
        const char logicalTimeImplementationName[])
    {
        // implementation
        vector<wstring> FOMmoduleUrls;
        wstring mimModuleUrl;
        // allocate strings
        string sFedExecName(federationExecutionName);
        wstring wFedExecName;
        wFedExecName.assign(sFedExecName.begin(),sFedExecName.end());
        
        FOMmoduleUrls.push_back(OpenRTI::localeToUcs(fomModules));
        mimModuleUrl = OpenRTI::localeToUcs(mimModule);

        try {
            rtiHandle->createFederationExecutionWithMIM(
            wFedExecName,FOMmoduleUrls,mimModuleUrl,L""
        );
        } catch (FederationExecutionAlreadyExists ignored) {
        }
    }

    EXTERNC int joinFederationExecutionLv(
        RTIambassador *rtiHandle,
        const char federateType[],
        const char federationExecutionName[],
        const char additionalFomModules[])
    {
        // implementation
        vector<wstring> FOMmoduleUrls;
        string sFedType(federateType);
        string sFedExecName(federationExecutionName);
        wstring wFedType,wFedExecName;
        wFedType.assign(sFedType.begin(),sFedType.end());
        wFedExecName.assign(sFedExecName.begin(),sFedExecName.end());

        FOMmoduleUrls.push_back(OpenRTI::localeToUcs(additionalFomModules));

        FederateHandle federateHandle = rtiHandle->joinFederationExecution(
            wFedType,
            wFedExecName,
            FOMmoduleUrls
        );
    }

    EXTERNC int getInteractionClassHandleLv(
        RTIambassador *rtiHandle,
        const char federateType[],
        const char federationExecutionName[],
        const char additionalFomModules[])
    {
        // implementation
    }   

    EXTERNC int getParameterHandleLv(
        RTIambassador *rtiHandle,
        InteractionClassHandle whichClass,
        const char theName[])
    {
        // implementation
    } 

    EXTERNC int getObjectClassHandleLv(
        RTIambassador *rtiHandle,
        const char theName[])
    {
        // implementation
    }

    EXTERNC int getAttributeHandleLv(
        RTIambassador *rtiHandle,
        InteractionClassHandle whichClass,
        const char theName[])
    {
        // implementation
    } 

    EXTERNC int reserveObjectInstanceNameLv(
        RTIambassador *rtiHandle,
        const char theObjectInstanceName[])
    {
        // implementation
    }  

    EXTERNC int registerObjectInstanceLv(
        RTIambassador *rtiHandle,
        ObjectClassHandle theClass,
        const char theObjectInstanceName[])
    {
        // implementation
    }  

    EXTERNC int updateAttributeValuesLv(
        RTIambassador *rtiHandle,
        ObjectInstanceHandle theObject,
        AttributeHandleValueMap const & theAttributeValues,
        VariableLengthData const & theUserSuppliedTag)
    {
        // implementation
    }  

    EXTERNC int sendInteractionLv(
        RTIambassador *rtiHandle,
        InteractionClassHandle theInteraction,
        ParameterHandleValueMap const & theParameterValues,
        VariableLengthData const & theUserSuppliedTag)
    {
        // implementation
    }  

    EXTERNC int subscribeInteractionClassLv(
        RTIambassador *rtiHandle,
        InteractionClassHandle theClass,
        bool active = true)
    {
        // implementation
    }

    EXTERNC int publishInteractionClassLv(
        RTIambassador *rtiHandle,
        InteractionClassHandle theInteraction)
    {
        // implementation
    }

    EXTERNC int subscribeObjectClassAttributesLv(
        RTIambassador *rtiHandle,
        ObjectClassHandle theClass,
        AttributeHandleSet const & attributeList,
        bool active,
        const char updateRateDesignator[])
    {
        // implementation
    }

    EXTERNC int publishObjectClassAttributesLv(
        RTIambassador *rtiHandle,
        ObjectClassHandle theClass,
        AttributeHandleSet const & attributeList)
    {
        // implementation
    }

    EXTERNC int evokeMultipleCallbacksLv(
        RTIambassador *rtiHandle,
        double approximateMinimumTimeInSeconds,
        double approximateMaximumTimeInSeconds)
    {
        // implementation
    }

    EXTERNC int resignFederationExecutionLv(
        RTIambassador *rtiHandle,
        ResignAction resignAction)
    {
        // implementation
        rtiHandle->resignFederationExecution(CANCEL_THEN_DELETE_THEN_DIVEST);
    }

    EXTERNC int destroyFederationExecutionLv(
        RTIambassador *rtiHandle,
        const char federationExecutionName[])
    {
        // implementation
        string sFedExecName(federationExecutionName);
        wstring wFedExecName;
        wFedExecName.assign(sFedExecName.begin(),sFedExecName.end());
    
        try{
            rtiHandle->destroyFederationExecution(wFedExecName);
        } catch (FederatesCurrentlyJoined &ignored) {}
    }  

    EXTERNC int disconnectLv(RTIambassador *rtiHandle)
    {
        auto_ptr<RTIambassador> _rtiAmbassador;
        rtiHandle->disconnect();
        //RTIambassador *_rtiAmbassador = static_cast<RTIambassador*>(_rtiAmbassadorIn);
        //_rtiAmbassador->disconnect();
        return 0;
    }

    EXTERNC int destroyRTIambassadorLv(RTIambassador *rtiHandle)
    {
        delete rtiHandle;
    }



} // namespace rti1516eLv