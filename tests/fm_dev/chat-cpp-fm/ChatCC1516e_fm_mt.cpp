// ChatCC1516e.cpp : Defines the entry point for the console application.
// [FM-24/12/2021]: coded from pRTI example

//TODO: #include "StdAfx.h" 

#include <iostream>
#include <assert.h>
#include <exception>
#include <map>
//#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <thread>

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

using namespace std;
using namespace rti1516e;

pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t _threshold_cv = PTHREAD_COND_INITIALIZER;

//thread handshake vars
bool _reservationSucceeded;
bool _reservationComplete;

void string2wstring(wstring &dest, const string &src);

// main class
class ChatCCFederate : public NullFederateAmbassador {
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

   wstring _username;
   wstring _message;

   // bool _reservationSucceeded;
   // bool _reservationComplete;

public:

   ChatCCFederate() {
   }

  ~ChatCCFederate() throw()
   {
   }

   class Participant{
   private:
      wstring _name;
   public:
      Participant(wstring name)
      {
         _name = name;
      }

      Participant()
      {
      }

      wstring toString()
      {
         return _name;
      }
   };	

   map<ObjectInstanceHandle, Participant> _knownObjects;	


   void ChatUI() {

      wchar_t tmpUsername[128];

      //auto_ptr<RTIambassador> _rtiAmbassador(reinterpret_cast<RTIambassador*>(threadarg));
      //auto_ptr<RTIambassador> _rtiAmbassador(static_cast<RTIambassador*>(threadarg));

      do {
         wcout << L"Enter your name: ";
         wcout.flush();
         wcin.getline(tmpUsername, 128, '\n');
         if (wcin.fail()) {
            wcin.clear();
            wcin.ignore(128, '\n');
         }

         _username = tmpUsername;

         try {
            _reservationComplete = false;
            _rtiAmbassador->reserveObjectInstanceName(_username);
            pthread_mutex_lock(&_mutex);
            while (!_reservationComplete) {
               pthread_cond_wait(&_threshold_cv, &_mutex);
            }
            pthread_mutex_unlock(&_mutex);
            if (!_reservationSucceeded) {
               wcout << L"Name already taken, try again.\n";
            }
         }
         catch (IllegalName& e) {
            wcout << L"Illegal name. Try again.\n";
         }
         catch (Exception&) {
            wcout << L"RTI exception when reserving name: \n";
            pthread_exit(NULL);
         }
      } while (!_reservationSucceeded);

      HLAunicodeString unicodeUserName(_username);
      _iParticipantHdl = _rtiAmbassador->registerObjectInstance(_oParticipantId, _username);
      _aHandleValueMap[_aNameId] = unicodeUserName.encode();
      _rtiAmbassador->updateAttributeValues(_iParticipantHdl, _aHandleValueMap, VariableLengthData());
      
      wcout << L"Type messages you want to send. To exit, type . <ENTER>" << endl;
      while (true) {
         wchar_t msg[256];
         wstring wmsg;
         wcout << L"> ";
         wcout.flush();
         wcin.getline(msg, sizeof(msg));
         wmsg = msg;

         if (wmsg == L".") {
            break;
         }

         HLAunicodeString unicodeMessage(wmsg);
         _pHandleValueMap[_pTextId] = unicodeMessage.encode();
         _pHandleValueMap[_pSenderId] = unicodeUserName.encode();
         _rtiAmbassador->sendInteraction(_iMessageId, _pHandleValueMap, VariableLengthData());
      }

   }
   // static class to be used as a thread arg
   static void thread_ChatUI(void* data) {
      ChatCCFederate *ccInstance = static_cast<ChatCCFederate *>(data);
      ccInstance->ChatUI();
   }

   void run(int argc, char* argv[]){
      wstring host = L"localhost";
      wstring port = L"8989";
      wchar_t tmpCrcHost[128];			
      wchar_t tmpUsername[128];
      vector<wstring> FOMmoduleUrls;
      std::wstring mimModule;

      int rc;
      pthread_t threadId;

      FOMmoduleUrls.push_back(OpenRTI::localeToUcs("/home/admin/git_repo/OpenRTI/build/bin/Chat-evolved.xml"));
      mimModule = OpenRTI::localeToUcs("/home/admin/git_repo/OpenRTI/share/rti1516e/HLAstandardMIM.xml");
      
      try {
         if (argc > 1) {
            string2wstring(host, argv[1]);
         } else {
            //wcout << L"Using RTI version " << rtiVersion() << endl << endl; 
            wcout << L"Enter the CRC address, such as" << endl;
            wcout << L"'localhost', 'localhost:8989', '192.168.1.62'" << endl;
            wcout << L"or when using Pitch Booster on the form" << endl;
            wcout << L"<CRC name>@<booster address>:<booster port>" << endl;
            wcout << L"such as 'MyCRCname@192.168.1.70:8688'" << endl;
            wcout << endl;
            wcout << L"[localhost]: ";
            wcout.flush();
            wcin.getline(tmpCrcHost, 128, '\n');
            if (wcin.fail()) {
               wcin.clear();
               wcin.ignore(128,'\n');
            } else {
               host = tmpCrcHost;
            }

            if (host == L"") {
               host = L"localhost";
            }
         }
         auto_ptr<RTIambassadorFactory> rtiAmbassadorFactory(new RTIambassadorFactory());
         _rtiAmbassador = rtiAmbassadorFactory->createRTIambassador();
      }
      catch (RTIinternalError) {
         wcout << endl << L"Unable to create RTI ambassador" << endl;
         return;
      }

      try {
         //wstring localSettingsDesignator(L"crcAddress=" + host);
         wstring localSettingsDesignator(L"rti://" + host);
         wcout << L"connecting to: " << localSettingsDesignator << endl;
         //_rtiAmbassador->connect(*this, HLA_IMMEDIATE,localSettingsDesignator);
         _rtiAmbassador->connect(*this, HLA_EVOKED,localSettingsDesignator);

         try {
         _rtiAmbassador->destroyFederationExecution(L"ChatRoom");
         } catch (FederatesCurrentlyJoined ignored) {
         } catch (FederationExecutionDoesNotExist ignored) {
         } catch (RTIinternalError ignored) {
         } catch (NotConnected ignored) {
         } 

         try {
            //_rtiAmbassador->createFederationExecution(L"ChatRoom",FOMmoduleUrls,L"");
            _rtiAmbassador->createFederationExecutionWithMIM(L"ChatRoom", FOMmoduleUrls, mimModule, L"");
         } catch (FederationExecutionAlreadyExists ignored) {
         }

         FederateHandle federateHandle = _rtiAmbassador->joinFederationExecution(
            L"Chat",
            L"ChatRoom",
            FOMmoduleUrls
            );

         _iMessageId = _rtiAmbassador->getInteractionClassHandle(L"Communication");
         _pTextId = _rtiAmbassador->getParameterHandle(_iMessageId, L"Message");
         _pSenderId = _rtiAmbassador->getParameterHandle(_iMessageId, L"Sender");
         _oParticipantId = _rtiAmbassador->getObjectClassHandle(L"Participant");
         _aNameId = _rtiAmbassador->getAttributeHandle(_oParticipantId, L"Name");
         _aHandleSet.insert(_aNameId);

         _rtiAmbassador->subscribeInteractionClass(_iMessageId);
         _rtiAmbassador->publishInteractionClass(_iMessageId);
         _rtiAmbassador->subscribeObjectClassAttributes(_oParticipantId , _aHandleSet);
         _rtiAmbassador->publishObjectClassAttributes(_oParticipantId , _aHandleSet);

#ifdef _WIN32
         // Make sure that stdin and stdout is in text mode. Initialization of the
         // Java Virtual Machine (done by pRTI) has a side-effect which
         // is to set stdin and stdout to binary mode.
         _setmode(_fileno(stdout), _O_TEXT);
         _setmode(_fileno(stderr), _O_TEXT);
         _setmode(_fileno(stdin), _O_TEXT);
#endif
         // spawn to a new thread
         thread th1(thread_ChatUI,this);
         // rc = pthread_create(&threadId,NULL,ChatUI,reinterpret_cast<void *>(_rtiAmbassador.get()));
         //rc = pthread_create(&threadId,NULL,&ChatUI,NULL);
         //    if (rc) {
         //       cout << "Error: unable to create thread, " << rc << endl;
         //       exit (-1);
         //   }
         // TODO - add logic to monitor thread
         while(true) {
            _rtiAmbassador->evokeMultipleCallbacks(2.0,5.0);
         }
         //pthread_join(threadId,NULL);
         th1.join();

         _rtiAmbassador->resignFederationExecution(CANCEL_THEN_DELETE_THEN_DIVEST );
         try {
            _rtiAmbassador->destroyFederationExecution(L"ChatRoom");
         } catch (FederatesCurrentlyJoined &ignored) {
         }
         _rtiAmbassador->disconnect();
      } catch (CouldNotOpenFDD &fdde) {
         wcerr << fdde.what() << endl;
         wcerr.flush();
      } catch (Exception &e) {
         wcerr << e.what() << endl;
         wcerr.flush();
      } catch (exception &e2) {
         wcerr << e2.what() << endl;
         wcerr.flush();
      }
      wcout << endl << L"Press <ENTER> to shutdown" << endl;
      wchar_t emptybuffer[256];
      wcin.getline(emptybuffer, sizeof(emptybuffer));
   }


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

      if (_knownObjects.count(theObject) == 0) {
         Participant member((wstring)name);
         wcout << L"[ " << member.toString() << L" has joined the chat ]" << endl;
         wcout << L"> ";
         _knownObjects[theObject] = member;		
      }
   }

   virtual
   void
   objectInstanceNameReservationSucceeded(
      wstring const &
      theObjectInstanceName)
      throw (FederateInternalError)
   {
      pthread_mutex_lock(&_mutex);
      _reservationComplete = true;
      _reservationSucceeded = true;
      pthread_cond_signal(&_threshold_cv);
      pthread_mutex_unlock(&_mutex);
   }

   virtual
   void
   objectInstanceNameReservationFailed(
      wstring const &
      theObjectInstanceName)
      throw (FederateInternalError)
   {
      pthread_mutex_lock(&_mutex);
      _reservationComplete = true;
      _reservationSucceeded = false;
      pthread_cond_signal(&_threshold_cv);
      pthread_mutex_unlock(&_mutex);
   }

   virtual
   void
   removeObjectInstance(
      ObjectInstanceHandle const & theObject,
      VariableLengthData const & theUserSuppliedTag,
      OrderType const & sentOrder)
      throw (FederateInternalError)
   {
      if (_knownObjects.count(theObject)) {
         map<ObjectInstanceHandle,Participant>::iterator iter;
         iter = _knownObjects.find(theObject);
         Participant member(iter->second);
         _knownObjects.erase(theObject);
         wcout << L"[ " << member.toString() << L" has left the chat ]" << endl;
         wcout << L"> ";
      }
   }
        
   virtual
   void 
   provideAttributeValueUpdate (
      ObjectInstanceHandle theObject,
      AttributeHandleSet const & theAttributes,
      VariableLengthData const & theUserSuppliedTag)
      throw (FederateInternalError)
   {
      if (theObject == _iParticipantHdl && theAttributes.count(_aNameId) != 0) {
         HLAunicodeString unicodeUserName(_username);
         AttributeHandleValueMap attributeValues;
         attributeValues[_aNameId] = unicodeUserName.encode();
         _rtiAmbassador->updateAttributeValues(_iParticipantHdl, attributeValues, VariableLengthData());
      }
   }
};

int main(int argc, char* argv[])
{
   ChatCCFederate* chatCCFederate = new ChatCCFederate();
   chatCCFederate->run(argc, argv);
   delete chatCCFederate;
   return 0;
}

void string2wstring (wstring &dest, const string &src)
{
    dest.resize(src.size());
    for (string::size_type i = 0; i < src.size(); i++) {
        dest[i] = static_cast<unsigned char>(src[i]);
    }
}