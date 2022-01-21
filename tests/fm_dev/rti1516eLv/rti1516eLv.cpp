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
    LVUserEventRef lueReceiveInteraction;

    wstring chararray2wstring(const char charArray[])
    {
        wstring wStringOut;
        // allocate strings
        string sStringTemp(charArray);
        wStringOut.assign(sStringTemp.begin(),sStringTemp.end());

        return wStringOut;
    }

    LStrHandle wstring2LvString(wstring const & theObjectInstanceName)
    {
            const wchar_t *input = theObjectInstanceName.c_str();
            size_t size = (wcslen(input)+1)*sizeof(wchar_t);
            char *buffer = new char[size];
            wcstombs(buffer,input,size);
            // char buffer[] = "Ciao Pirla";
            const int headerSize = 4;
            size = strlen(buffer);

	        // Allocate memory for the LV string
            LStrHandle lSh = (LStrHandle)DSNewHandle(headerSize+size);

            // Write the LV string header
            (*lSh)->cnt = size;
            memcpy( (*lSh)->str,buffer,size);
            

        return lSh;
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

        auto_ptr<RTIambassador> getRTIambassador()
        {
            return _rtiAmbassador;
        }

        void createRTIambassadorLv()
        {
            try {
                auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
                _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();
            } catch (Exception &e) { 
                throw;
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
            pthread_mutex_lock(&_mutex);
            _disconnected = true;
            pthread_cond_signal(&_threshold_cv);
            pthread_mutex_unlock(&_mutex);
        }
 
        void connectLv(
            wstring host)
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
            } catch (Exception &e) {
                // wcerr << "RTI1516e thrown " << e.what() << endl;
                // wcerr.flush();
                throw;
            }
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

                }  catch (Exception &e) { 
                    throw;
                }
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
            }  catch (Exception &e) { 
                throw;
            }
        }

        void destroyFederationExecutionLv(
            wstring const & federationExecutionName)
        {
            try {
                _rtiAmbassador->destroyFederationExecution(federationExecutionName);
            }  catch (Exception &e) { 
                throw;
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
            } catch (Exception &e) { 
                throw;
            }
        }

        void resignFederationExecutionLv(
            ResignAction resignAction)
        {
            try {
            _rtiAmbassador->resignFederationExecution(resignAction);               
            } catch (Exception &e) { 
                throw;
            }
        }

        void reserveObjectInstanceNameLv(
            std::wstring const & theObjectInstanceName)
        {
            try {
            _rtiAmbassador->reserveObjectInstanceName(theObjectInstanceName);
            } catch (Exception &e) { 
                throw;
            }
        }

        ObjectInstanceHandle registerObjectInstanceLv (
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

        InteractionClassHandle getInteractionClassHandleLv (
         std::wstring const & theName)
        {
            //InteractionClassHandle interactionClassHandle;
            try {
                return _rtiAmbassador->getInteractionClassHandle(theName);
            } catch (Exception &e) { 
                throw;
            } 
        }

        ParameterHandle getParameterHandleLv (
         InteractionClassHandle whichClass,
         std::wstring const & theName)
        {
            ParameterHandle parameterHande;
            try {
                return _rtiAmbassador->getParameterHandle(whichClass,theName);
            } catch (Exception &e) { 
                throw;
            } 
        }

        ObjectClassHandle getObjectClassHandleLv (
         std::wstring const & theName)
        {
            //ParameterHandle parameterHande;
            try {
                return _rtiAmbassador->getObjectClassHandle(theName);
            } catch (Exception &e) { 
                throw;
            } 
        }

        AttributeHandle getAttributeHandleLv (
         ObjectClassHandle whichClass,
         std::wstring const & theAttributeName)
        {
            try {
                return _rtiAmbassador->getAttributeHandle(whichClass,theAttributeName);
            } catch (Exception &e) { 
                throw;
            }  
        }

        // Pub Sub

        void subscribeInteractionClassLv (
         InteractionClassHandle theClass,
         bool active = true)
        {
            try {
                return _rtiAmbassador->subscribeInteractionClass(theClass);
            } catch (Exception &e) { 
                throw;
            }  
        }

        void publishInteractionClassLv (
         InteractionClassHandle theInteraction)
        {
            try {
                return _rtiAmbassador->publishInteractionClass(theInteraction);
            } catch (Exception &e) { 
                throw;
            }  
        }

        void subscribeObjectClassAttributesLv (
         ObjectClassHandle theClass,
         AttributeHandleSet const & attributeList,
         bool active = true,
         std::wstring const & updateRateDesignator = L"")

        {
            try {
                return _rtiAmbassador->subscribeObjectClassAttributes(theClass,attributeList,active,updateRateDesignator);
            } catch (Exception &e) { 
                throw;
            }  
        }

        void publishObjectClassAttributesLv (
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

        void updateAttributeValuesLv (
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

        virtual void sendInteractionLv (
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

        virtual
        void
        receiveInteraction (
            InteractionClassHandle theInteraction,
            ParameterHandleValueMap const & theParameterValues,
            VariableLengthData const & theUserSuppliedTag,
            OrderType sentOrder,
            TransportationType theType,
            SupplementalReceiveInfo theReceiveInfo)
            throw (FederateInternalError)
        {
            receiveInteractionData dataToSend;
            dataToSend.sentOrder = sentOrder;
            dataToSend.theType = theType;
            PostLVUserEvent(lueReceiveInteraction,&dataToSend);
            
            if (theInteraction == _iMessageId) {
                HLAunicodeString message;
                HLAunicodeString sender;
                for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin(); i != theParameterValues.end(); ++i) {
                    ParameterHandle const & handle = i->first;
                    VariableLengthData const & value = i->second;
                    if (handle == _pTextId) {
                    message.decode(value);
                    } else if (handle == _pSenderId) {
                    sender.decode(value);
                    }
                }
                wcout << wstring(sender) << L": " << wstring(message) << endl;
            }
        }

        virtual
        void
        reflectAttributeValues (
            ObjectInstanceHandle theObject,
            AttributeHandleValueMap const & theAttributeValues,
            VariableLengthData const & theUserSuppliedTag,
            OrderType sentOrder,
            TransportationType theType,
            SupplementalReflectInfo theReflectInfo)
            throw (FederateInternalError)
        {
            HLAunicodeString name;
            name.decode(theAttributeValues.find(_aNameId)->second);

            // if (_knownObjects.count(theObject) == 0) {
            //     Participant member((wstring)name);
            //     wcout << L"[ " << member.toString() << L" has joined the chat ]" << endl;
            //     wcout << L"> ";
            //     _knownObjects[theObject] = member;		
            // }
        }

        virtual void objectInstanceNameReservationSucceeded(
            wstring const & theObjectInstanceName)
            throw (FederateInternalError)
        {
            LStrHandle h = wstring2LvString(theObjectInstanceName);

            PostLVUserEvent(lueObjInsNameResSucceeded,&h);

            DSDisposeHandle(h);
        }

        virtual void objectInstanceNameReservationFailed(
            wstring const & theObjectInstanceName)
            throw (FederateInternalError)
        {            
            LStrHandle h = wstring2LvString(theObjectInstanceName);

            PostLVUserEvent(lueObjInsNameResFailed,&h);

            DSDisposeHandle(h);
        }
        virtual
        void
        removeObjectInstance(
            ObjectInstanceHandle const & theObject,
            VariableLengthData const & theUserSuppliedTag,
            OrderType const & sentOrder)
            throw (FederateInternalError)
        {
            // if (_knownObjects.count(theObject)) {
            //     map<ObjectInstanceHandle,Participant>::iterator iter;
            //     iter = _knownObjects.find(theObject);
            //     Participant member(iter->second);
            //     _knownObjects.erase(theObject);
            //     wcout << L"[ " << member.toString() << L" has left the chat ]" << endl;
            //     wcout << L"> ";
            // }
        }

    };
    
    LvFederate* oLvFederate; 

    // test export functions

    EXTERNC int testFunc()
    {
        return 12345;
    }

    // using struct to test complex data sharing
    EXTERNC MgErr testFireEvent(testEventData *value)
    {
        MgErr status;
        status = PostLVUserEvent(tempUserEvStore,value);
        return status;
    }

    EXTERNC int regObjInstNameResSuccEvent(LVUserEventRef *eventRef)
    {
        tempUserEvStore = *eventRef;
        return 0;
    }

    // helpers 

    EXTERNC  int attrHandleValueMapCreate(AttributeHandleValueMap **attrHandleValueMap)
    {
        *attrHandleValueMap = new AttributeHandleValueMap();
        return 0;
    }

    EXTERNC int attrHandleValueMapAddElementString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[])
    {
        wstring wElem = chararray2wstring(sElem);
        HLAunicodeString uElem(wElem);
        (*attrHandleValueMap)[(*attributeHandle)] = uElem.encode();
        
    }

    EXTERNC int attrHandleValueMapDestroy(
        AttributeHandleValueMap * attrHandleValueMap)
    {
        delete attrHandleValueMap;

        return 0;
    }

    EXTERNC int parHandleValueMapCreate(ParameterHandleValueMap **parHandleValueMap)
    {
        *parHandleValueMap = new ParameterHandleValueMap();
        return 0;
    }

    EXTERNC int parHandleValueMapAddElementString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[])
    {
        wstring wElem = chararray2wstring(sElem);
        HLAunicodeString uElem(wElem);
        (*parHandleValueMap)[(*parameterHandle)] = uElem.encode();
    }

    EXTERNC int parHandleValueMapDestroy(
        ParameterHandleValueMap * parHandleValueMap)
    {
        delete parHandleValueMap;

        return 0;
    } 

    EXTERNC int attrHandleSetCreate(AttributeHandleSet **attrHandleSet)
    {
        *attrHandleSet = new AttributeHandleSet();
        return 0;
    }

    EXTERNC int attrHandleSetInsert(
        AttributeHandleSet *attrHandleSet,
        AttributeHandle *attrHandle
    )
    {
        //AttributeHandle attrHandleVar;
        //attrHandleSet->insert(attrHandleVar);
        //attrHandleSet->insert(*attrHandle);
        (*attrHandleSet).insert(*attrHandle);
        return 0;
    }

    EXTERNC int attrHandleSetDestroy(
        AttributeHandleSet * attrHandleSet
    )
    {
        delete attrHandleSet;
        return 0;
    }

    // OpenRTI wrappers

    EXTERNC int regLvUserEvents(
        LVUserEventRef *objInstNameResSucc,
        LVUserEventRef *objInstNameResFail,
        LVUserEventRef *receiveInteraction
        )
    {
        lueObjInsNameResSucceeded = *objInstNameResSucc;
        lueObjInsNameResFailed = *objInstNameResFail;
        lueReceiveInteraction  = *receiveInteraction;

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
        // dummy set _processCB to true;
        bool previous = false;
        previous = _processCB;
        _processCB = true;
        return previous;
    }

    EXTERNC int stopRTIambassadorLvEx(
        RTIambassador *rtiHandle)
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
        return 0;;
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
    }

} // namespace rti1516eLv