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
#include <chrono>

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
    bool _disconnected = false;
    bool _processCB = false;

    // temp LVuser event store
    LVUserEventRef tempUserEvStore;
    LVUserEventRef lueObjInsNameResSucceeded;
    LVUserEventRef lueObjInsNameResFailed;

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
        auto_ptr<RTIambassador> _rtiAmbassador;
        InteractionClassHandle _iMessageId;
        ParameterHandle _pTextId;
        ParameterHandle _pSenderId;
        ObjectClassHandle _oParticipantId;
        ObjectInstanceHandle _iParticipantHdl;
        AttributeHandle _aNameId;
        AttributeHandleSet _aHandleSet;
        ParameterHandleValueMap _pHandleValueMap;
        AttributeHandleValueMap _aHandleValueMap;

        //LV user event references to be private for the class
        // TODO        
        wstring _username;
        wstring _message;

        // bool _reservationSucceeded;
        // bool _reservationComplete;
    public:
        LvFederate() {}

        ~LvFederate() throw() {}

        void createRTIambassadorLv()
        {
            try {
                auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
                _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();
            }
            catch (RTIinternalError) {
             wcout << endl << L"Unable to create RTI ambassador" << endl;
            return;
            }
        }

        static void th_cb_consumer(void *data) {
            //ChatCCFederate *ccInstance = static_cast<ChatCCFederate *>(data);
            //ccInstance->ChatUI();
            LvFederate *lvFedInstance = static_cast<LvFederate *>(data);
            _stopped = false;
            // set connected flag
            pthread_mutex_lock(&_mutex);
            _disconnected = false;
            _connected = true;
            pthread_cond_signal(&_threshold_cv);
            pthread_mutex_unlock(&_mutex);
            // start callback event processing
            _processCB = false;
            lvFedInstance->cb_consumer();
            // send disconnect
            pthread_mutex_lock(&_mutex);
            _connected = false;
            _disconnected = true;
            pthread_cond_signal(&_threshold_cv);
            pthread_mutex_unlock(&_mutex);

        }

        void cb_consumer(void)
        {
            while(!_stopped)  {
                if (_processCB) {
                    wcout << "Processing callbacks." << endl;
                    _rtiAmbassador->evokeMultipleCallbacks(1.0,2.0);
                }
                else 
                {
                    this_thread::sleep_for(chrono::milliseconds(100));
                    wcout << "Waiting to start processing." << endl;
                }
            }
            wcout << "RTIdeamon completed." << endl;
            pthread_mutex_lock(&_mutex);
            _disconnected = true;
            pthread_cond_signal(&_threshold_cv);
            pthread_mutex_unlock(&_mutex);
        }
 
        void connectLv(wstring host)
        {
            try {
                wstring localSettingsDesignator(L"rti://" + host);
                wcout << L"connecting to: " << localSettingsDesignator << endl;
                //_rtiAmbassador->connect(*this, HLA_IMMEDIATE,localSettingsDesignator);
                _connected = false;
                _rtiAmbassador->connect(*this, HLA_EVOKED,localSettingsDesignator);
                // spawn thread
                thread thCbProc(th_cb_consumer,this);
                // wait for thread to be launched
                pthread_mutex_lock(&_mutex);
                while (!_connected) {
                    pthread_cond_wait(&_threshold_cv, &_mutex);
                }
                pthread_mutex_unlock(&_mutex);
                thCbProc.detach();

            } catch (RTIinternalError ignored) {}
        }

        void disconnectLv()
            {
                try {
                    _disconnected = false;
                    // set stop flag for cb_consumer
                    _stopped = true;
                        pthread_mutex_lock(&_mutex);
                    while (!_disconnected) {
                        pthread_cond_wait(&_threshold_cv, &_mutex);
                    }
                    pthread_mutex_unlock(&_mutex);
                    _rtiAmbassador->disconnect();

                } catch (RTIinternalError ignored) {}
            }

        void createFederationExecutionWithMIMLv(
            wstring const & federationExecutionName,
            vector<std::wstring> const & fomModules,
            wstring const & mimModule,
            wstring const & logicalTimeImplementationName = L""
        )
        {
            try {
                _rtiAmbassador->createFederationExecutionWithMIM(federationExecutionName,fomModules,mimModule);
            } catch (FederationExecutionAlreadyExists ignored) {
            } catch (RTIinternalError &e){
            wcout << "createFederationExecutionWithMIM: error -> " << e.what() << "returned.\n" << endl;
            }
        }

        void destroyFederationExecutionLv(wstring const & federationExecutionName)
        {
            try {
                _rtiAmbassador->destroyFederationExecution(federationExecutionName);
            } catch (RTIinternalError &e){
            wcout << "destroyFederationExecution: error -> " << e.what() << "returned.\n" << endl;
            }
        }

        FederateHandle joinFederationExecutionLv(
            std::wstring const & federateType,
                std::wstring const & federationExecutionName,
                std::vector<std::wstring> const & additionalFomModules=std::vector<std::wstring>())
        {
            FederateHandle federateHandle;
            try {
                federateHandle = _rtiAmbassador->joinFederationExecution(federateType,federationExecutionName,additionalFomModules);
            } catch (RTIinternalError &e){
            wcout << "joinFederationExecutionLv: error -> " << e.what() << "returned.\n" << endl;
            } 
        }

        void resignFederationExecutionLv(
            ResignAction resignAction)
        {
            // TODO try and catch
            _rtiAmbassador->resignFederationExecution(resignAction);
        }

        void reserveObjectInstanceNameLv(
            std::wstring const & theObjectInstanceName)
        {
            // TODO try and catch
            _rtiAmbassador->reserveObjectInstanceName(theObjectInstanceName);
        }

        ObjectInstanceHandle registerObjectInstanceLv (
         ObjectClassHandle theClass,
         std::wstring const & theObjectInstanceName)
        {
            ObjectInstanceHandle participantHdl;
            try {
                participantHdl = _rtiAmbassador->registerObjectInstance(theClass,theObjectInstanceName);
                return participantHdl;
            } catch (RTIinternalError &e){
            wcout << "registerObjectInstance: error -> " << e.what() << "returned.\n" << endl;
            } 
        }

        // FOM management

        InteractionClassHandle getInteractionClassHandleLv (
         std::wstring const & theName)
        {
            InteractionClassHandle interactionClassHandle;
            try {
                interactionClassHandle = _rtiAmbassador->getInteractionClassHandle(theName);
                return interactionClassHandle;
            } catch (RTIinternalError &e){
            wcout << "getInteractionClassHandle: error -> " << e.what() << "returned.\n" << endl;
            } 
        }

        // Append new lvFederate members right abov
        // Generic guideline: members implement try and catch     

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
    
    LvFederate* oLvFederate; 
    // thread function to be called by connectLv

    EXTERNC int testFunc()
    {
        return 12345;
    }

    EXTERNC MgErr testFireEvent(int value)
    {
        MgErr status;
        status = PostLVUserEvent(tempUserEvStore,&value);
        return status;
    }

    EXTERNC int regObjInstNameResSuccEvent(LVUserEventRef *eventRef)
    {
        tempUserEvStore = *eventRef;
        return 0;
    }

    EXTERNC int regLvUserEvents(
        LVUserEventRef *objInstNameResSucc,
        LVUserEventRef *objInstNameResFail)
    {
        lueObjInsNameResSucceeded = *objInstNameResSucc;
        lueObjInsNameResFailed = *objInstNameResFail;

        return 0;
    }  

    EXTERNC int createRTIambassadorLvEx(RTIambassador **rtiHandle)
    {
        // previous implmentation
        // auto_ptr<RTIambassador> _rtiAmbassador;

        // try {
        //     auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
        //     _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();

        //     *rtiHandle = _rtiAmbassador.release();
        //     _rtiCount++;
        
        // }
        // catch (RTIinternalError &e) {
        //     wcout << "createRTIambassador: error -> " << e.what() << "returned.\n" << endl;
        // }
        oLvFederate = new LvFederate();
        oLvFederate->createRTIambassadorLv();

        return _rtiCount;
    }

    EXTERNC int startRTIambassadorLvEx(RTIambassador *rtiHandle)
    {
        // dummy set _processCB to true;
        bool previous = false;
        previous = _processCB;
        _processCB = true;
        return previous;
    }

    EXTERNC int stopRTIambassadorLvEx(RTIambassador *rtiHandle)
    {
        // dummy set _processCB to false;
        bool previous = false;
        previous = _processCB;
        _processCB = false;
        return previous;
    }

    EXTERNC int connectLvEx(
        RTIambassador *rtiHandle, 
        const char address[])
    {
        wstring host = chararray2wstring(address);
        oLvFederate->connectLv(host);
    }

    EXTERNC int createFederationExecutionWithMIMLvEx(
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
            oLvFederate->createFederationExecutionWithMIMLv(wFedExecName,FOMmoduleUrls,mimModuleUrl,L"");
            //rtiHandle->createFederationExecutionWithMIM(wFedExecName,FOMmoduleUrls,mimModuleUrl,L"");
        } catch (FederationExecutionAlreadyExists ignored) {
        } catch (RTIinternalError &e){
            wcout << "createFederationExecutionWithMIM: error -> " << e.what() << "returned.\n" << endl;
        }
    }

    EXTERNC int joinFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federateType[],
        const char federationExecutionName[],
        const char additionalFomModules[])
    {
        // implementation
        vector<wstring> FOMmoduleUrls;
        wstring wFedType = chararray2wstring(federateType);
        wstring wFedExecName = chararray2wstring(federationExecutionName);

        FOMmoduleUrls.push_back(OpenRTI::localeToUcs(additionalFomModules));

        FederateHandle federateHandle = oLvFederate->joinFederationExecutionLv(
            wFedType,
            wFedExecName,
            FOMmoduleUrls
        );
        // TODO return federate handle if needed by lv code
        return sizeof(federateHandle);
    }

    EXTERNC int getInteractionClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[],
        InteractionClassHandle *intClassHandle)
    {
        // implementation
        wstring wTheName = chararray2wstring(theName);
        intClassHandle = &(oLvFederate->getInteractionClassHandleLv(wTheName));
    }   

    EXTERNC int getParameterHandleLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle whichClass,
        const char theName[])
    {
        // implementation
    } 

    EXTERNC int getObjectClassHandleLvEx(
        RTIambassador *rtiHandle,
        const char theName[])
    {
        // implementation
    }

    EXTERNC int getAttributeHandleLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle whichClass,
        const char theName[])
    {
        // implementation
    } 

    EXTERNC int reserveObjectInstanceNameLvEx(
        RTIambassador *rtiHandle,
        const char theObjectInstanceName[])
    {
        // implementation
        // string sObjInstName(theObjectInstanceName);
        // wstring wObjInstName;
        // wObjInstName.assign(sObjInstName.begin(),sObjInstName.end());
        wstring wObjInstName = chararray2wstring(theObjectInstanceName);

        //rtiHandle->reserveObjectInstanceName(wObjInstName);
        oLvFederate->reserveObjectInstanceNameLv(wObjInstName);

    }  

    EXTERNC int registerObjectInstanceLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle theClass,
        const char theObjectInstanceName[])
    {
        // implementation
    }  

    EXTERNC int updateAttributeValuesLvEx(
        RTIambassador *rtiHandle,
        ObjectInstanceHandle theObject,
        AttributeHandleValueMap const & theAttributeValues,
        VariableLengthData const & theUserSuppliedTag)
    {
        // implementation
    }  

    EXTERNC int sendInteractionLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle theInteraction,
        ParameterHandleValueMap const & theParameterValues,
        VariableLengthData const & theUserSuppliedTag)
    {
        // implementation
    }  

    EXTERNC int subscribeInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle theClass,
        bool active = true)
    {
        // implementation
    }

    EXTERNC int publishInteractionClassLvEx(
        RTIambassador *rtiHandle,
        InteractionClassHandle theInteraction)
    {
        // implementation
    }

    EXTERNC int subscribeObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle theClass,
        AttributeHandleSet const & attributeList,
        bool active,
        const char updateRateDesignator[])
    {
        // implementation
    }

    EXTERNC int publishObjectClassAttributesLvEx(
        RTIambassador *rtiHandle,
        ObjectClassHandle theClass,
        AttributeHandleSet const & attributeList)
    {
        // implementation
    }

    EXTERNC int evokeMultipleCallbacksLvEx(
        RTIambassador *rtiHandle,
        double approximateMinimumTimeInSeconds,
        double approximateMaximumTimeInSeconds)
    {
        // implementation
    }

    EXTERNC int resignFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        ResignAction resignAction)
    {
        // implementation
        oLvFederate->resignFederationExecutionLv(resignAction);
    }

    EXTERNC int destroyFederationExecutionLvEx(
        RTIambassador *rtiHandle,
        const char federationExecutionName[])
    {
        // implementation
        // string sFedExecName(federationExecutionName);
        // wstring wFedExecName;
        // wFedExecName.assign(sFedExecName.begin(),sFedExecName.end());
        wstring wFedExecName = chararray2wstring(federationExecutionName);
    
        try{
            oLvFederate->destroyFederationExecutionLv(wFedExecName);
            //rtiHandle->destroyFederationExecution(wFedExecName);
        } catch (FederatesCurrentlyJoined &ignored) {}
    }  

    EXTERNC int disconnectLvEx(RTIambassador *rtiHandle)
    {

        // //RTIambassador *_rtiAmbassador = static_cast<RTIambassador*>(_rtiAmbassadorIn);
        oLvFederate->disconnectLv();
        return 0;
    }

    EXTERNC int destroyRTIambassadorLvEx(RTIambassador *rtiHandle)
    {
        delete rtiHandle;
    }

} // namespace rti1516eLv