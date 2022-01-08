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
    const char *connectionString = "172.26.104.77:8990";
    const char *fomModule = "/home/admin/git_repo/OpenRTI/build/bin/Chat-evolved.xml";
    const char *mimModule = "/home/admin/git_repo/OpenRTI/share/rti1516e/HLAstandardMIM.xml";
    InteractionClassHandle iMessageId;
    ParameterHandle pTextId;

    //create RTIambassador
    createRTIambassadorLvEx(&rtiHandle);

    //connect
    connectLvEx(rtiHandle,connectionString);

    createFederationExecutionWithMIMLvEx(rtiHandle,"ChatRoom",fomModule,mimModule,"");
    //wait for a certain amoung 

    joinFederationExecutionLvEx(rtiHandle,"Chat","ChatRoom",fomModule);

    getInteractionClassHandleLvEx(rtiHandle,"Communication",&iMessageId);

    getParameterHandleLvEx(rtiHandle,&iMessageId,"Message",&pTextId);

    startRTIambassadorLvEx(rtiHandle);

    this_thread::sleep_for(chrono::seconds(5));

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