// rti1516eLv.cpp : shared library to call OpenRTI function from LV.
// [FM-03/01/2022]: first release
#include <iostream>
#include <assert.h>
#include <exception>
#include <map>
#include <string.h>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <mutex>
#include <condition_variable>
#include <io.h>
#include <fcntl.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif

// shared lib header
#include "rti1516eLv.h"

// namespaces
using namespace std;
using namespace rti1516e;

namespace rti1516eLv
{   
    // thread signaling variables
#ifdef _WIN32
    mutex _mutex;
    condition_variable _threshold_cv;
#else
    pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t _threshold_cv = PTHREAD_COND_INITIALIZER;
#endif
    bool _connected = false;
    bool _stopped = false;
    bool _disconnected = false;
    bool _processCB = false;

    LVUserEventRef lueObjInsNameResSucceeded;
    LVUserEventRef lueObjInsNameResFailed;
    LVUserEventRef lueReceiveInteraction;
    LVUserEventRef lueReflectAttributeValues;
    LVUserEventRef lueDiscoverObjectInstance;
    LVUserEventRef lueRemoveObjectInstance;
    LVUserEventRef lueReportFederationExecutions;

    //LvFederate() {}
    LvFederate::LvFederate(){}

    //~LvFederate() throw() {}
    LvFederate::~LvFederate() throw() {}

    //Event registration
    void LvFederate::regUserEventsLv(
        LVUserEventRef *objInstNameResSucc,
        LVUserEventRef *objInstNameResFail,
        LVUserEventRef *receiveInteraction,
        LVUserEventRef *reflectAttributeValues,
        LVUserEventRef *discoverObjectInstance,
        LVUserEventRef *removeObjectInstance,
        LVUserEventRef *reportFederationExecutions
    )
    {
        lueObjInsNameResSucceeded = *objInstNameResSucc;
        lueObjInsNameResFailed = *objInstNameResFail;
        lueReceiveInteraction  = *receiveInteraction;
        lueReflectAttributeValues = *reflectAttributeValues;
        lueDiscoverObjectInstance = *discoverObjectInstance;
        lueRemoveObjectInstance = *removeObjectInstance;
        lueReportFederationExecutions = *reportFederationExecutions;
    }

    auto_ptr<RTIambassador> LvFederate::getRTIambassador()
    {
        return _rtiAmbassador;
    }

    void LvFederate::createRTIambassadorLv()
    {
        try {
            auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
            _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();
        } catch (Exception &e) { 
            throw;
        }
    }

    int LvFederate::startRTIambassadorLvCallback(int timeout)
    {
        // dummy set _processCB to true;
        bool previous = false;
        previous = _processCB;
        _processCB = true;
        return previous;              
    }

    int LvFederate::stopRTIambassadorLvCallback(int timeout)
    {
        // dummy set _processCB to false;
        bool previous = false;
        previous = _processCB;
        _processCB = false;
        return previous;          
    }

    void LvFederate::th_cb_consumer(void *data) {
        LvFederate *lvFedInstance = static_cast<LvFederate *>(data);
        _stopped = false;
        // set connected flag
#ifdef _WIN32
        unique_lock<mutex> lk(_mutex);
#else     
        pthread_mutex_lock(&_mutex);
#endif
        _disconnected = false;
        _connected = true;
#ifdef _WIN32
        _threshold_cv.notify_one();
        lk.unlock();
#else  
      pthread_cond_signal(&_threshold_cv);
      pthread_mutex_unlock(&_mutex);
#endif
        // start callback event processing
        _processCB = false;
        lvFedInstance->cb_consumer();

#ifdef _WIN32
        lk.lock();
        //unique_lock<mutex> lk(_mutex);
#else     
        pthread_mutex_lock(&_mutex);
#endif
        // send disconnect
        _connected = false;
        _disconnected = true;

#ifdef _WIN32
        _threshold_cv.notify_one();
        lk.unlock();
#else  
        pthread_cond_signal(&_threshold_cv);
        pthread_mutex_unlock(&_mutex);
#endif

    }

    void LvFederate::cb_consumer(void)
    {
        while(!_stopped)  {
            if (_processCB) {
                // TODO: pass parameters function members
                // wcout << "Processing callbacks." << endl;
                // TODO: manage exception better
                try {
                    _rtiAmbassador->evokeMultipleCallbacks(1.0,2.0);
                } catch (Exception &e) { 
                    throw;
                }   
            }
            else 
            {
                this_thread::sleep_for(chrono::milliseconds(100));
                wcout << "Waiting to start processing." << endl;
            }
        }
        wcout << "RTIdeamon completed." << endl;
#ifdef _WIN32
        unique_lock<mutex> lk(_mutex);
#else     
        pthread_mutex_lock(&_mutex);
#endif
        _disconnected = true;
#ifdef _WIN32
        _threshold_cv.notify_one();
        lk.unlock();
#else  
        pthread_cond_signal(&_threshold_cv);
         pthread_mutex_unlock(&_mutex);
#endif
    }

    void LvFederate::connectLv(
        wstring host)
        RTI_THROW ((Exception))
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
#ifdef _WIN32
            unique_lock<mutex> lk(_mutex);
            while (!_connected) {
                _threshold_cv.wait(lk);
            }
            lk.unlock();
#else
            pthread_mutex_lock(&_mutex);
            while (!_connected) {
                pthread_cond_wait(&_threshold_cv, &_mutex);
            }
            pthread_mutex_unlock(&_mutex);
#endif
            thCbProc.detach();
        } catch (Exception &e) {
            // wcerr << "RTI1516e thrown " << e.what() << endl;
            // wcerr.flush();
            throw;
        }
    }

    void LvFederate::disconnectLv()
        {
            try {
                _disconnected = false;
                // set stop flag for cb_consumer
                _stopped = true;
#ifdef _WIN32
                unique_lock<mutex> lk(_mutex);
                while (!_disconnected) {
                    _threshold_cv.wait(lk);
                }
                lk.unlock();
#else
                    pthread_mutex_lock(&_mutex);
                while (!_disconnected) {
                    pthread_cond_wait(&_threshold_cv, &_mutex);
                }
                pthread_mutex_unlock(&_mutex);
#endif
                _rtiAmbassador->disconnect();

            }  catch (Exception &e) { 
                throw;
            }
        }

    void LvFederate::createFederationExecutionWithMIMLv(
        wstring const & federationExecutionName,
        vector<std::wstring> const & fomModules,
        wstring const & mimModule,
        wstring const & logicalTimeImplementationName)
    {
        try {
            _rtiAmbassador->createFederationExecutionWithMIM(federationExecutionName,fomModules,mimModule);
        }  catch (Exception &e) { 
            throw;
        }
    }

    void LvFederate::destroyFederationExecutionLv(
        wstring const & federationExecutionName)
    {
        try {
            _rtiAmbassador->destroyFederationExecution(federationExecutionName);
        }  catch (Exception &e) { 
            throw;
        }
    }

    void LvFederate::listFederationExecutionsLv()
    {
        try {
            _rtiAmbassador->listFederationExecutions();
        } catch (Exception &e) { 
            throw;
        }        
    }

    FederateHandle LvFederate::joinFederationExecutionLv(
        std::wstring const & federateType,
            std::wstring const & federationExecutionName,
            std::vector<std::wstring> const & additionalFomModules)
    {
        FederateHandle federateHandle;
        try {
            federateHandle = _rtiAmbassador->joinFederationExecution(federateType,federationExecutionName,additionalFomModules);
        } catch (Exception &e) { 
            throw;
        }
    }

    void LvFederate::resignFederationExecutionLv(
        ResignAction resignAction)
    {
        try {
        _rtiAmbassador->resignFederationExecution(resignAction);               
        } catch (Exception &e) { 
            throw;
        }
    }

    void LvFederate::reserveObjectInstanceNameLv(
        std::wstring const & theObjectInstanceName)
    {
        try {
        _rtiAmbassador->reserveObjectInstanceName(theObjectInstanceName);
        } catch (Exception &e) { 
            throw;
        }
    }

    ObjectInstanceHandle LvFederate::registerObjectInstanceLv (
        ObjectClassHandle theClass,
        std::wstring const & theObjectInstanceName)
    {
        ObjectInstanceHandle participantHdl;
        try {
            participantHdl = _rtiAmbassador->registerObjectInstance(theClass,theObjectInstanceName);
            return participantHdl;
        } catch (Exception &e) { 
            throw;
        }
    }

    // FOM management

    InteractionClassHandle LvFederate::getInteractionClassHandleLv (
        std::wstring const & theName)
    {
        //InteractionClassHandle interactionClassHandle;
        try {
            return _rtiAmbassador->getInteractionClassHandle(theName);
        } catch (Exception &e) { 
            throw;
        } 
    }

    std::wstring LvFederate::getInteractionClassNameLv(
        InteractionClassHandle theHandle)
        try {
        return _rtiAmbassador->getInteractionClassName(theHandle);
    }
    catch (Exception& e) {
        throw;
    }

    ParameterHandle LvFederate::getParameterHandleLv (
        InteractionClassHandle whichClass,
        std::wstring const & theName)
    {
        //ParameterHandle parameterHande;
        try {
            return _rtiAmbassador->getParameterHandle(whichClass,theName);
        } catch (Exception &e) { 
            throw;
        } 
    }

    std::wstring LvFederate::getParameterNameLv(
        InteractionClassHandle whichClass,
        ParameterHandle theHandle)
    {
        try {
            return _rtiAmbassador->getParameterName(whichClass, theHandle);
        }
        catch (Exception& e) {
            throw;
        }
    }

    ObjectClassHandle LvFederate::getObjectClassHandleLv (
        std::wstring const & theName)
    {
        //ParameterHandle parameterHande;
        try {
            return _rtiAmbassador->getObjectClassHandle(theName);
        } catch (Exception &e) { 
            throw;
        } 
    }

    std::wstring LvFederate::getObjectClassNameLv(
        ObjectClassHandle theHandle)
    {
        try {
            return _rtiAmbassador->getObjectClassName(theHandle);
        }
        catch (Exception& e) {
            throw;
        }
    }

    AttributeHandle LvFederate::getAttributeHandleLv (
        ObjectClassHandle whichClass,
        std::wstring const & theAttributeName)
    {
        try {
            return _rtiAmbassador->getAttributeHandle(whichClass,theAttributeName);
        } catch (Exception &e) { 
            throw;
        }  
    }

    std::wstring LvFederate::getAttributeNameLv(
        ObjectClassHandle whichClass,
        AttributeHandle theHandle)
    {
        try {
            return _rtiAmbassador->getAttributeName(whichClass, theHandle);
        }
        catch (Exception& e) {
            throw;
        }
    }

    // Pub Sub

    void LvFederate::subscribeInteractionClassLv (
        InteractionClassHandle theClass,
        bool active)
    {
        try {
            return _rtiAmbassador->subscribeInteractionClass(theClass);
        } catch (Exception &e) { 
            throw;
        }  
    }

    void LvFederate::publishInteractionClassLv (
        InteractionClassHandle theInteraction)
    {
        try {
            return _rtiAmbassador->publishInteractionClass(theInteraction);
        } catch (Exception &e) { 
            throw;
        }  
    }

    void LvFederate::subscribeObjectClassAttributesLv (
        ObjectClassHandle theClass,
        AttributeHandleSet const & attributeList,
        bool active,
        std::wstring const & updateRateDesignator)

    {
        try {
            return _rtiAmbassador->subscribeObjectClassAttributes(theClass,attributeList,active,updateRateDesignator);
        } catch (Exception &e) { 
            throw;
        }  
    }

    void LvFederate::publishObjectClassAttributesLv (
        ObjectClassHandle theClass,
        AttributeHandleSet const & attributeList)
    {
        try {
            return _rtiAmbassador->publishObjectClassAttributes(theClass,attributeList);
        } catch (Exception &e) { 
            throw;
        }  
    }      

    // data update

    void LvFederate::updateAttributeValuesLv (
        ObjectInstanceHandle theObject,
        AttributeHandleValueMap const & theAttributeValues,
        VariableLengthData const & theUserSuppliedTag)
    {
        try {
            _rtiAmbassador->updateAttributeValues(theObject,theAttributeValues,theUserSuppliedTag);
        } catch (Exception &e) { 
            throw;
        }  
    }

    void LvFederate::sendInteractionLv (
        InteractionClassHandle theInteraction,
        ParameterHandleValueMap const & theParameterValues,
        VariableLengthData const & theUserSuppliedTag)
    {
        try {
            _rtiAmbassador->sendInteraction(theInteraction,theParameterValues,theUserSuppliedTag);
        } catch (Exception &e) { 
            throw;
        }  
    }
    // Append new lvFederate members right abov
    // Generic guideline: members implement try and catch     

    void
        LvFederate::receiveInteraction(
            InteractionClassHandle theInteraction,
            ParameterHandleValueMap const& theParameterValues,
            VariableLengthData const& theUserSuppliedTag,
            OrderType sentOrder,
            TransportationType theType,
            SupplementalReceiveInfo theReceiveInfo)
        throw (FederateInternalError)
    {
        // Declare variables and initialize
        receiveInteractionData dataToSend;
        ParameterHandleValueMap *dsParam;
        
        // Init struct
        dsParam = new ParameterHandleValueMap();
        memset(&dataToSend, 0, sizeof(dataToSend));
        //dataToSend.parHandleValueMap = (ParameterHandleValueMap*)DSNewPtr(sizeof(theParameterValues));
        //dsParam = (ParameterHandleValueMap *)DSNewPtr(sizeof(theParameterValues));
        *dsParam = theParameterValues;
        
        // initialize event data
        dataToSend.interactionClassHandle = theInteraction;
        dataToSend.parHandleValueMap = dsParam;
        dataToSend.sentOrder = sentOrder;
        dataToSend.theType = theType;
        dataToSend.numOfElements = (uint32_t)(*dsParam).size();

        PostLVUserEvent(lueReceiveInteraction,&dataToSend);

    }

    void
    LvFederate::reflectAttributeValues (
        ObjectInstanceHandle theObject,
        AttributeHandleValueMap const & theAttributeValues,
        VariableLengthData const & theUserSuppliedTag,
        OrderType sentOrder,
        TransportationType theType,
        SupplementalReflectInfo theReflectInfo)
        throw (FederateInternalError)
    {
        reflectAttributeValuesData dataToSend;
        AttributeHandleValueMap *dsParam;

        // Init struct
        dsParam = new AttributeHandleValueMap();
        memset(&dataToSend, 0, sizeof(dataToSend));
        //dsParam = (AttributeHandleValueMap *)DSNewPtr(sizeof(theAttributeValues));
        *dsParam = theAttributeValues;

        dataToSend.objectInstanceHandle = theObject;
        dataToSend.attrHandleValueMap = dsParam;
        dataToSend.sentOrder = sentOrder;
        dataToSend.theType = theType;
        dataToSend.numOfElements = (uint32_t)(*dsParam).size();

        PostLVUserEvent(lueReflectAttributeValues,&dataToSend);
    }

    void LvFederate::objectInstanceNameReservationSucceeded(
        wstring const & theObjectInstanceName)
        throw (FederateInternalError)
    {
        LStrHandle h = wstring2LvString(theObjectInstanceName);

        PostLVUserEvent(lueObjInsNameResSucceeded,&h);

        DSDisposeHandle(h);
    }

    void LvFederate::objectInstanceNameReservationFailed(
        wstring const & theObjectInstanceName)
        throw (FederateInternalError)
    {            
        LStrHandle h = wstring2LvString(theObjectInstanceName);

        PostLVUserEvent(lueObjInsNameResFailed,&h);

        DSDisposeHandle(h);
    }
    
    void
    LvFederate::removeObjectInstance(
        ObjectInstanceHandle const & theObject,
        VariableLengthData const & theUserSuppliedTag,
        OrderType const & sentOrder)
        throw (FederateInternalError)
    {
        removeObjectInstanceData dataToSend;

        dataToSend.objectInstanceHandle = theObject;
        dataToSend.sentOrder = sentOrder;

        PostLVUserEvent(lueRemoveObjectInstance, &dataToSend);
    }
    
    void 
    LvFederate::discoverObjectInstance(
        ObjectInstanceHandle theObject,
        ObjectClassHandle theObjectClass,
        std::wstring const& theObjectInstanceName)
        throw (
            FederateInternalError)
    {
        // Declare variables and initialize
        discoverObjectInstanceData dataToSend;
        memset(&dataToSend, 0, sizeof(dataToSend));
        const wchar_t* input = theObjectInstanceName.c_str();
        size_t size = (wcslen(input) + 1) * sizeof(wchar_t);
        dataToSend.size = (size < (MAXSTRLVCLUSTER + 1) ? size : MAXSTRLVCLUSTER);
        
        // initialize event data
        wcstombs(dataToSend.objectInstanceName, input, dataToSend.size);
        dataToSend.objectInstanceHandle = theObject;
        dataToSend.objectClassHandle = theObjectClass; 

        PostLVUserEvent(lueDiscoverObjectInstance, &dataToSend);
    }

    void
    LvFederate::reportFederationExecutions (
         FederationExecutionInformationVector const &
         theFederationExecutionInformationList)
        RTI_THROW((
            FederateInternalError))
    {
        LStrArrHdl hArr;
        //LStrHandle h;
        int i = 0;

        auto arraySize = theFederationExecutionInformationList.size();
        hArr = arraySize == 0 ? NULL : reinterpret_cast<LStrArr**>(DSNewHClr(sizeof(LStrArr) + sizeof(LStrHandle) * (arraySize - 1)));
        (*hArr)->len = arraySize == 0 ? 0 : arraySize;

        for (auto it = begin(theFederationExecutionInformationList); it != end(theFederationExecutionInformationList); ++it) {
            (*hArr)->elm[i] = wstring2LvString(it->federationExecutionName);
            ++i;
        }
        PostLVUserEvent(lueReportFederationExecutions, &hArr);
        DSDisposeHandle(hArr);
    }

    
} // namespace rti1516eLv