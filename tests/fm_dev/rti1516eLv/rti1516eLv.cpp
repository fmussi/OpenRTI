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
    // thread signaling variables
    // TODO 
    pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t _threshold_cv = PTHREAD_COND_INITIALIZER;
    bool _connected = false;
    bool _stopped = false;

    // temp LVuser event store
    LVUserEventRef tempUserEvStore;

    wstring chararray2wstring(const char charArray[])
    {
        wstring wStringOut;
        // allocate strings
        string sStringTemp(charArray);
        wStringOut.assign(sStringTemp.begin(),sStringTemp.end());

        return wStringOut;
    }

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

        //LV user event references
        LVUserEventRef lueObjInsNameResSucceeded;
        LVUserEventRef lueObjInsNameResFailed;
        
        wstring _username;
        wstring _message;

        // bool _reservationSucceeded;
        // bool _reservationComplete;
    public:
        LvFederate() {}

        ~LvFederate() throw() {}

        void RTIdaemon(RTIambassador *rtiHandle,const char address[])
        {
            // connect to RTI and process callbacks
            wstring host = chararray2wstring(address);
        
            try {
                wstring localSettingsDesignator(L"rti://" + host);
                rtiHandle->connect(*this,HLA_EVOKED,localSettingsDesignator);
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
            // set connected flag
            pthread_mutex_lock(&_mutex);
            _connected = true;
            pthread_cond_signal(&_threshold_cv);
            pthread_mutex_unlock(&_mutex);
            // process events
            // TODO: manage how to exit function and how to manage errors
            // e.g. rtiHandle invalid
            while(!_stopped) {
                rtiHandle->evokeMultipleCallbacks(2.0,5.0);
            }
        }

        // map<ObjectInstanceHandle, Participant> _knownObjects;	
        static void th_cb_consumer(void *data) {
            //ChatCCFederate *ccInstance = static_cast<ChatCCFederate *>(data);
            //ccInstance->ChatUI();
        }
        
        virtual void objectInstanceNameReservationSucceeded(
            wstring const & theObjectInstanceName)
            throw (FederateInternalError)
        {
            double eventData = 12345.0;
            // send LV event
            PostLVUserEvent(lueObjInsNameResSucceeded,&eventData);
        }

        virtual void objectInstanceNameReservationFailed(
            wstring const & theObjectInstanceName)
            throw (FederateInternalError)
        {            
            double eventData = 54321.0;
            // send LV event
            PostLVUserEvent(lueObjInsNameResFailed,&eventData);
        }
    };
    
    // thread function to be called by connectLv

    void th_connect(RTIambassador *rtiHandle, const char address[]) 
    {
        LvFederate* lvFederate = new LvFederate();
        lvFederate->RTIdaemon(rtiHandle,address);
        // TODO: exit thread function
        // signal varible to be sent to caller
    }

    EXTERNC int testFunc()
    {
        return 12345;
    }

    EXTERNC int regObjInstNameResSuccEvent(LVUserEventRef *eventRef)
    {
        tempUserEvStore = *eventRef;
        return 0;
    }

    EXTERNC MgErr testFireEvent(int value)
    {
        MgErr status;
        status = PostLVUserEvent(tempUserEvStore,&value);
        return status;
    }

    EXTERNC int createRTIambassadorLv(RTIambassador **rtiHandle)
    {
        auto_ptr<RTIambassador> _rtiAmbassador;

        try {
            auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
            _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();

            *rtiHandle = _rtiAmbassador.release();
            _rtiCount++;
        
        }
        catch (RTIinternalError &e) {
            e.what();
        }

        return _rtiCount;
    }

    EXTERNC int connectLv(
        RTIambassador *rtiHandle, 
        const char address[])
    {
        // LvFederate oLvFed;
        // spawn thread
        _connected = false;
        thread thRtiHandle(th_connect,rtiHandle,address);
        pthread_mutex_lock(&_mutex);
        while (!_connected) {
            pthread_cond_wait(&_threshold_cv, &_mutex);
        }
        pthread_mutex_unlock(&_mutex);
        // check that connection has been succesful

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
        wstring wFedExecName = chararray2wstring(federationExecutionName);

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
        // string sFedType(federateType);
        // string sFedExecName(federationExecutionName);
        // wstring wFedType,wFedExecName;
        // wFedType.assign(sFedType.begin(),sFedType.end());
        // wFedExecName.assign(sFedExecName.begin(),sFedExecName.end());
        wstring wFedType = chararray2wstring(federateType);
        wstring wFedExecName = chararray2wstring(federationExecutionName);

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
        // string sObjInstName(theObjectInstanceName);
        // wstring wObjInstName;
        // wObjInstName.assign(sObjInstName.begin(),sObjInstName.end());
        wstring wObjInstName = chararray2wstring(theObjectInstanceName);

        rtiHandle->reserveObjectInstanceName(wObjInstName);

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
        // string sFedExecName(federationExecutionName);
        // wstring wFedExecName;
        // wFedExecName.assign(sFedExecName.begin(),sFedExecName.end());
        wstring wFedExecName = chararray2wstring(federationExecutionName);
    
        try{
            rtiHandle->destroyFederationExecution(wFedExecName);
        } catch (FederatesCurrentlyJoined &ignored) {}
    }  

    EXTERNC int disconnectLv(RTIambassador *rtiHandle)
    {
        auto_ptr<RTIambassador> _rtiAmbassador;
        _stopped = true;
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