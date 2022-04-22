#ifndef RTI1516ELV_FEDERATE_H
#define RTI1516ELV_FEDERATE_H

// #ifdef __cplusplus
// extern "C" {
// #endif

#include "rti1516eLv_EXPORTS.h"
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

/* Platform specific LabView includes and lib */
#ifdef _WIN32
	#include "extcode.h"
#else
	#include "cintools/extcode.h"
#endif

// #include "rti1516eLv_helpers.h"
// #include "rti1516eLv_errors.h"

using namespace rti1516e;

namespace rti1516eLv
{   
    //class LvFederate : public NullFederateAmbassador {
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

    public:
        LvFederate();

        ~LvFederate() throw();

        //Event registration
        virtual void regUserEventsLv(
            LVUserEventRef *objInstNameResSucc,
            LVUserEventRef *objInstNameResFail,
            LVUserEventRef *receiveInteraction,
            LVUserEventRef *reflectAttributeValues
        );

        virtual auto_ptr<RTIambassador> getRTIambassador();

        virtual void createRTIambassadorLv();

        static void th_cb_consumer(void *data);

        virtual void cb_consumer();
        
        virtual void connectLv(
            wstring host)
            RTI_THROW ((Exception));

        virtual void disconnectLv();

        // Start and Stop ambassador calback processing
        virtual int startRTIambassadorLvCallback(
            int timeout);
        virtual int stopRTIambassadorLvCallback(
            int timeout);

        virtual void createFederationExecutionWithMIMLv(
            wstring const & federationExecutionName,
            vector<std::wstring> const & fomModules,
            wstring const & mimModule,
            wstring const & logicalTimeImplementationName = L""
        );

        virtual void destroyFederationExecutionLv(
            wstring const & federationExecutionName);

        virtual FederateHandle joinFederationExecutionLv(
            std::wstring const & federateType,
                std::wstring const & federationExecutionName,
                std::vector<std::wstring> const & additionalFomModules=std::vector<std::wstring>());

        virtual void resignFederationExecutionLv(
            ResignAction resignAction);


        virtual void reserveObjectInstanceNameLv(
            std::wstring const & theObjectInstanceName);

        virtual ObjectInstanceHandle registerObjectInstanceLv (
            ObjectClassHandle theClass,
            std::wstring const & theObjectInstanceName);

        // FOM management

        virtual InteractionClassHandle getInteractionClassHandleLv (
            std::wstring const & theName);

        virtual std::wstring getInteractionClassNameLv(
            InteractionClassHandle theHandle);

        virtual ParameterHandle getParameterHandleLv (
            InteractionClassHandle whichClass,
            std::wstring const & theName);

        virtual std::wstring getParameterNameLv(
            InteractionClassHandle whichClass,
            ParameterHandle theHandle);

        virtual ObjectClassHandle getObjectClassHandleLv (
            std::wstring const & theName);

        virtual std::wstring getObjectClassNameLv(
            ObjectClassHandle theHandle);

        virtual AttributeHandle getAttributeHandleLv (
            ObjectClassHandle whichClass,
            std::wstring const & theAttributeName);

        virtual std::wstring getAttributeNameLv(
            ObjectClassHandle whichClass,
            AttributeHandle theHandle);

        // Pub Sub

        virtual void subscribeInteractionClassLv (
            InteractionClassHandle theClass,
            bool active = true);

        virtual void publishInteractionClassLv (
            InteractionClassHandle theInteraction);

        virtual void subscribeObjectClassAttributesLv (
            ObjectClassHandle theClass,
            AttributeHandleSet const & attributeList,
            bool active = true,
            std::wstring const & updateRateDesignator = L"");
        

        virtual void publishObjectClassAttributesLv (
            ObjectClassHandle theClass,
            AttributeHandleSet const & attributeList);  

        // data update

        virtual void updateAttributeValuesLv (
            ObjectInstanceHandle theObject,
            AttributeHandleValueMap const & theAttributeValues,
            VariableLengthData const & theUserSuppliedTag);

        virtual void sendInteractionLv (
            InteractionClassHandle theInteraction,
            ParameterHandleValueMap const & theParameterValues,
            VariableLengthData const & theUserSuppliedTag);
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
            throw (FederateInternalError);

        virtual
        void
        reflectAttributeValues (
            ObjectInstanceHandle theObject,
            AttributeHandleValueMap const & theAttributeValues,
            VariableLengthData const & theUserSuppliedTag,
            OrderType sentOrder,
            TransportationType theType,
            SupplementalReflectInfo theReflectInfo)
            throw (FederateInternalError);

        virtual void objectInstanceNameReservationSucceeded(
            wstring const & theObjectInstanceName)
            throw (FederateInternalError);

        virtual 
        void 
        objectInstanceNameReservationFailed (
            wstring const & theObjectInstanceName)
            throw (FederateInternalError);

        virtual
        void
        removeObjectInstance(
            ObjectInstanceHandle const & theObject,
            VariableLengthData const & theUserSuppliedTag,
            OrderType const & sentOrder)
            throw (FederateInternalError);

    };
}

#endif // RTI1516ELV_FEDERATE_H

// #ifdef __cplusplus
// } // extern "C"
// #endif