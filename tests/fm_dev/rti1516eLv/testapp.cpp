#include <iostream>
#include <assert.h>
#include <exception>
#include <map>
//#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <thread>
#include <chrono>

// RTI specific headers
#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
#include <RTI/NullFederateAmbassador.h>
#include <RTI/time/HLAinteger64TimeFactory.h>
#include <RTI/time/HLAinteger64Time.h>
#include <RTI/time/HLAinteger64Interval.h>
#include <RTI/encoding/BasicDataElements.h>

#include <Options.h>
#include <StringUtils.h>

#include "rti1516eLv.h"

using namespace std;
using namespace rti1516e;
using namespace rti1516eLv;

int main()
{
    RTIambassador *rtiHandle;
    const char *connectionString = "192.168.0.8:8990";
    const char *fomModule = "/home/admin/git_repo/OpenRTI/build/bin/Chat-evolved.xml";
    const char *mimModule = "/home/admin/git_repo/OpenRTI/share/rti1516e/HLAstandardMIM.xml";
    
    
   InteractionClassHandle _iMessageId;
   ParameterHandle _pTextId;
   ParameterHandle _pSenderId;
   ObjectClassHandle _oParticipantId;
   ObjectInstanceHandle _iParticipantHdl;
   AttributeHandle _aNameId;
   AttributeHandleSet _aHandleSet;
   ParameterHandleValueMap _pHandleValueMap;
   AttributeHandleValueMap _aHandleValueMap;

    //create RTIambassador
    createRTIambassadorLvEx(&rtiHandle);

    //connect
    connectLvEx(rtiHandle,connectionString);

    destroyFederationExecutionLvEx(rtiHandle,"ChatRoom");

    createFederationExecutionWithMIMLvEx(rtiHandle,"ChatRoom",fomModule,mimModule,"");
    //wait for a certain amoung 

    joinFederationExecutionLvEx(rtiHandle,"Chat","ChatRoom",fomModule);

    getInteractionClassHandleLvEx(rtiHandle,"Communication",&_iMessageId);
    
    getParameterHandleLvEx(rtiHandle,&_iMessageId,"Message",&_pTextId);

    getParameterHandleLvEx(rtiHandle,&_iMessageId,"Sender",&_pSenderId);

    getObjectClassHandleLvEx(rtiHandle,"Participant",&_oParticipantId);

    getAttributeHandleLvEx(rtiHandle,&_oParticipantId,"Name",&_aNameId);

    attrHandleSetCreate(&_aHandleSet);

    attrHandleSetInsert(&_aHandleSet,&_aNameId);

    attrHandleValueMapCreate(&_aHandleValueMap);
    parHandleValueMapCreate(&_pHandleValueMap);

    startRTIambassadorLvEx(rtiHandle);
    
    subscribeInteractionClassLvEx(rtiHandle,&_iMessageId,true);
    publishInteractionClassLvEx(rtiHandle,&_iMessageId);
    subscribeObjectClassAttributesLvEx(rtiHandle,&_oParticipantId,_aHandleSet,true,"");
    publishObjectClassAttributesLvEx(rtiHandle,&_oParticipantId,_aHandleSet);

    this_thread::sleep_for(chrono::seconds(5));

    reserveObjectInstanceNameLvEx(rtiHandle,"ciccione1");

    this_thread::sleep_for(chrono::seconds(5));

    registerObjectInstanceLvEx(rtiHandle,_oParticipantId,"ciccione1",&_iParticipantHdl);

    attrHandleValueMapAddElementString(&_aHandleValueMap,&_aNameId,"ciccione1");

    updateAttributeValuesLvEx(rtiHandle,_iParticipantHdl,_aHandleValueMap);

    parHandleValueMapAddElementString(&_pHandleValueMap,&_pTextId,"Ciao a tutti");

    parHandleValueMapAddElementString(&_pHandleValueMap,&_pSenderId,"ciccione1");

    sendInteractionLvEx(rtiHandle,&_iMessageId,_pHandleValueMap);

    stopRTIambassadorLvEx(rtiHandle);
    this_thread::sleep_for(chrono::seconds(2));

    resignFederationExecutionLvEx(rtiHandle,CANCEL_THEN_DELETE_THEN_DIVEST);

    destroyFederationExecutionLvEx(rtiHandle,"ChatRoom");
    
    //disconnect
    disconnectLvEx(rtiHandle);

    //destroy RTIambassador
    //destroyRTIambassadorLv(rtiHandle);

    return 0;

}