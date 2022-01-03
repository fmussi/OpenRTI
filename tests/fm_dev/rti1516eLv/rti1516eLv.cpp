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

    EXTERNC void* createRTIambassadorLv()
    {
        auto_ptr<RTIambassador> _rtiAmbassador;

        try {
            auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
            _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();
            
            //return sizeof(_rtiAmbassador);
            return static_cast<void*>(_rtiAmbassador.get());
        }
        catch (RTIinternalError &e) {
            e.what();
        }
    }

    EXTERNC int connectLv(void* _rtiAmbassadorIn, const char address[])
    {
        LvFederate oLvFed;
        string addrStrr(address);
        wstring host;
        host.assign(addrStrr.begin(),addrStrr.end());
        RTIambassador *_rtiAmbassador = static_cast<RTIambassador*>(_rtiAmbassadorIn);

        try {
            wstring localSettingsDesignator(L"rti://" + host);
            _rtiAmbassador->connect(oLvFed,HLA_EVOKED,localSettingsDesignator);

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

    EXTERNC int disconnectLv(void* _rtiAmbassadorIn)
    {
        RTIambassador *_rtiAmbassador = static_cast<RTIambassador*>(_rtiAmbassadorIn);
        _rtiAmbassador->disconnect();
        return 0;
    }

    EXTERNC int destroyRTIambassadorLv(void* _rtiAmbassadorIn)
    {
        RTIambassador *_rtiAmbassador = static_cast<RTIambassador*>(_rtiAmbassadorIn);

        delete _rtiAmbassador;
        return 0;
    }


} // namespace rti1516eLv