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
    const char *connectionString = "172.30.157.155:8990";
    const char *fomModule = "/home/admin/git_repo/OpenRTI/build/bin/Chat-evolved.xml";
    const char *mimModule = "/home/admin/git_repo/OpenRTI/share/rti1516e/HLAstandardMIM.xml";
      
    //create RTIambassador
    createRTIambassadorLv(&rtiHandle);

    //connect
    connectLv(rtiHandle,connectionString);

    createFederationExecutionWithMIMLv(rtiHandle,"ChatRoom",fomModule,mimModule,"");
    //wait for a certain amoung 
    //this_thread::sleep_for(chrono::seconds(5));

    //disconnect
    disconnectLv(rtiHandle);

    //destroy RTIambassador
    destroyRTIambassadorLv(rtiHandle);

    return 0;

}