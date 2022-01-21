// rti1516eLv.cpp : shared library to call OpenRTI function from LV.
// [FM-03/01/2022]: first release
#include <iostream>
#include <assert.h>
#include <exception>
#include <typeindex>

// shared lib header
#include "rti1516eLv.h"

// namespaces
using namespace std;
using namespace rti1516e;

namespace rti1516eLv
{   
    int lvErrorCodeFromException(Exception &e)
    {
        if (typeid(e) == typeid(AlreadyConnected))
        {
            return LV_ERROR_ALREADYCONNECTED;
        }
        else if (typeid(e) == typeid(AlreadyConnected))  return LV_ERROR_ALREADYCONNECTED;
        else if (typeid(e) == typeid(AsynchronousDeliveryAlreadyDisabled))  return LV_ERROR_ASYNCHRONOUSDELIVERYALREADYDISABLED;
        else if (typeid(e) == typeid(AsynchronousDeliveryAlreadyEnabled))  return LV_ERROR_ASYNCHRONOUSDELIVERYALREADYENABLED;
        else if (typeid(e) == typeid(AttributeAcquisitionWasNotCanceled))  return LV_ERROR_ATTRIBUTEACQUISITIONWASNOTCANCELED;
        else if (typeid(e) == typeid(AttributeAcquisitionWasNotRequested))  return LV_ERROR_ATTRIBUTEACQUISITIONWASNOTREQUESTED;
        else if (typeid(e) == typeid(AttributeAlreadyBeingAcquired))  return LV_ERROR_ATTRIBUTEALREADYBEINGACQUIRED;
        else if (typeid(e) == typeid(AttributeAlreadyBeingChanged))  return LV_ERROR_ATTRIBUTEALREADYBEINGCHANGED;
        else if (typeid(e) == typeid(AttributeAlreadyBeingDivested))  return LV_ERROR_ATTRIBUTEALREADYBEINGDIVESTED;
        else if (typeid(e) == typeid(AttributeAlreadyOwned))  return LV_ERROR_ATTRIBUTEALREADYOWNED;
        else if (typeid(e) == typeid(AttributeDivestitureWasNotRequested))  return LV_ERROR_ATTRIBUTEDIVESTITUREWASNOTREQUESTED;
        else if (typeid(e) == typeid(AttributeNotDefined))  return LV_ERROR_ATTRIBUTENOTDEFINED;
        else if (typeid(e) == typeid(AttributeNotOwned))  return LV_ERROR_ATTRIBUTENOTOWNED;
        else if (typeid(e) == typeid(AttributeNotPublished))  return LV_ERROR_ATTRIBUTENOTPUBLISHED;
        else if (typeid(e) == typeid(AttributeNotRecognized))  return LV_ERROR_ATTRIBUTENOTRECOGNIZED;
        else if (typeid(e) == typeid(AttributeNotSubscribed))  return LV_ERROR_ATTRIBUTENOTSUBSCRIBED;
        else if (typeid(e) == typeid(AttributeRelevanceAdvisorySwitchIsOff))  return LV_ERROR_ATTRIBUTERELEVANCEADVISORYSWITCHISOFF;
        else if (typeid(e) == typeid(AttributeRelevanceAdvisorySwitchIsOn))  return LV_ERROR_ATTRIBUTERELEVANCEADVISORYSWITCHISON;
        else if (typeid(e) == typeid(AttributeScopeAdvisorySwitchIsOff))  return LV_ERROR_ATTRIBUTESCOPEADVISORYSWITCHISOFF;
        else if (typeid(e) == typeid(AttributeScopeAdvisorySwitchIsOn))  return LV_ERROR_ATTRIBUTESCOPEADVISORYSWITCHISON;
        else if (typeid(e) == typeid(BadInitializationParameter))  return LV_ERROR_BADINITIALIZATIONPARAMETER;
        else if (typeid(e) == typeid(CallNotAllowedFromWithinCallback))  return LV_ERROR_CALLNOTALLOWEDFROMWITHINCALLBACK;
        else if (typeid(e) == typeid(ConnectionFailed))  return LV_ERROR_CONNECTIONFAILED;
        else if (typeid(e) == typeid(CouldNotCreateLogicalTimeFactory))  return LV_ERROR_COULDNOTCREATELOGICALTIMEFACTORY;
        else if (typeid(e) == typeid(CouldNotDecode))  return LV_ERROR_COULDNOTDECODE;
        else if (typeid(e) == typeid(CouldNotDiscover))  return LV_ERROR_COULDNOTDISCOVER;
        else if (typeid(e) == typeid(CouldNotEncode))  return LV_ERROR_COULDNOTENCODE;
        else if (typeid(e) == typeid(CouldNotOpenFDD))  return LV_ERROR_COULDNOTOPENFDD;
        else if (typeid(e) == typeid(CouldNotOpenMIM))  return LV_ERROR_COULDNOTOPENMIM;
        else if (typeid(e) == typeid(CouldNotInitiateRestore))  return LV_ERROR_COULDNOTINITIATERESTORE;
        else if (typeid(e) == typeid(DeletePrivilegeNotHeld))  return LV_ERROR_DELETEPRIVILEGENOTHELD;
        else if (typeid(e) == typeid(DesignatorIsHLAstandardMIM))  return LV_ERROR_DESIGNATORISHLASTANDARDMIM;
        else if (typeid(e) == typeid(RequestForTimeConstrainedPending))  return LV_ERROR_REQUESTFORTIMECONSTRAINEDPENDING;
        else if (typeid(e) == typeid(NoRequestToEnableTimeConstrainedWasPending))  return LV_ERROR_NOREQUESTTOENABLETIMECONSTRAINEDWASPENDING;
        else if (typeid(e) == typeid(RequestForTimeRegulationPending))  return LV_ERROR_REQUESTFORTIMEREGULATIONPENDING;
        else if (typeid(e) == typeid(NoRequestToEnableTimeRegulationWasPending))  return LV_ERROR_NOREQUESTTOENABLETIMEREGULATIONWASPENDING;
        else if (typeid(e) == typeid(NoFederateWillingToAcquireAttribute))  return LV_ERROR_NOFEDERATEWILLINGTOACQUIREATTRIBUTE;
        else if (typeid(e) == typeid(ErrorReadingFDD))  return LV_ERROR_ERRORREADINGFDD;
        else if (typeid(e) == typeid(ErrorReadingMIM))  return LV_ERROR_ERRORREADINGMIM;
        else if (typeid(e) == typeid(FederateAlreadyExecutionMember))  return LV_ERROR_FEDERATEALREADYEXECUTIONMEMBER;
        else if (typeid(e) == typeid(FederateHandleNotKnown))  return LV_ERROR_FEDERATEHANDLENOTKNOWN;
        else if (typeid(e) == typeid(FederateHasNotBegunSave))  return LV_ERROR_FEDERATEHASNOTBEGUNSAVE;
        else if (typeid(e) == typeid(FederateInternalError))  return LV_ERROR_FEDERATEINTERNALERROR;
        else if (typeid(e) == typeid(FederateIsExecutionMember))  return LV_ERROR_FEDERATEISEXECUTIONMEMBER;
        else if (typeid(e) == typeid(FederateNameAlreadyInUse))  return LV_ERROR_FEDERATENAMEALREADYINUSE;
        else if (typeid(e) == typeid(FederateNotExecutionMember))  return LV_ERROR_FEDERATENOTEXECUTIONMEMBER;
        else if (typeid(e) == typeid(FederateOwnsAttributes))  return LV_ERROR_FEDERATEOWNSATTRIBUTES;
        else if (typeid(e) == typeid(FederateServiceInvocationsAreBeingReportedViaMOM))  return LV_ERROR_FEDERATESERVICEINVOCATIONSAREBEINGREPORTEDVIAMOM;
        else if (typeid(e) == typeid(FederateUnableToUseTime))  return LV_ERROR_FEDERATEUNABLETOUSETIME;
        else if (typeid(e) == typeid(FederatesCurrentlyJoined))  return LV_ERROR_FEDERATESCURRENTLYJOINED;
        else if (typeid(e) == typeid(FederationExecutionAlreadyExists))  return LV_WARNING_FEDERATIONEXECUTIONALREADYEXISTS;
        else if (typeid(e) == typeid(FederationExecutionDoesNotExist))  return LV_ERROR_FEDERATIONEXECUTIONDOESNOTEXIST;
        else if (typeid(e) == typeid(IllegalName))  return LV_ERROR_ILLEGALNAME;
        else if (typeid(e) == typeid(IllegalTimeArithmetic))  return LV_ERROR_ILLEGALTIMEARITHMETIC;
        else if (typeid(e) == typeid(InconsistentFDD))  return LV_ERROR_INCONSISTENTFDD;
        else if (typeid(e) == typeid(InteractionClassAlreadyBeingChanged))  return LV_ERROR_INTERACTIONCLASSALREADYBEINGCHANGED;
        else if (typeid(e) == typeid(InteractionClassNotDefined))  return LV_ERROR_INTERACTIONCLASSNOTDEFINED;
        else if (typeid(e) == typeid(InteractionClassNotPublished))  return LV_ERROR_INTERACTIONCLASSNOTPUBLISHED;
        else if (typeid(e) == typeid(InteractionClassNotRecognized))  return LV_ERROR_INTERACTIONCLASSNOTRECOGNIZED;
        else if (typeid(e) == typeid(InteractionClassNotSubscribed))  return LV_ERROR_INTERACTIONCLASSNOTSUBSCRIBED;
        else if (typeid(e) == typeid(InteractionParameterNotDefined))  return LV_ERROR_INTERACTIONPARAMETERNOTDEFINED;
        else if (typeid(e) == typeid(InteractionParameterNotRecognized))  return LV_ERROR_INTERACTIONPARAMETERNOTRECOGNIZED;
        else if (typeid(e) == typeid(InteractionRelevanceAdvisorySwitchIsOff))  return LV_ERROR_INTERACTIONRELEVANCEADVISORYSWITCHISOFF;
        else if (typeid(e) == typeid(InteractionRelevanceAdvisorySwitchIsOn))  return LV_ERROR_INTERACTIONRELEVANCEADVISORYSWITCHISON;
        else if (typeid(e) == typeid(InTimeAdvancingState))  return LV_ERROR_INTIMEADVANCINGSTATE;
        else if (typeid(e) == typeid(InvalidAttributeHandle))  return LV_ERROR_INVALIDATTRIBUTEHANDLE;
        else if (typeid(e) == typeid(InvalidDimensionHandle))  return LV_ERROR_INVALIDDIMENSIONHANDLE;
        else if (typeid(e) == typeid(InvalidFederateHandle))  return LV_ERROR_INVALIDFEDERATEHANDLE;
        else if (typeid(e) == typeid(InvalidInteractionClassHandle))  return LV_ERROR_INVALIDINTERACTIONCLASSHANDLE;
        else if (typeid(e) == typeid(InvalidLocalSettingsDesignator))  return LV_ERROR_INVALIDLOCALSETTINGSDESIGNATOR;
        else if (typeid(e) == typeid(InvalidLogicalTime))  return LV_ERROR_INVALIDLOGICALTIME;
        else if (typeid(e) == typeid(InvalidLogicalTimeInterval))  return LV_ERROR_INVALIDLOGICALTIMEINTERVAL;
        else if (typeid(e) == typeid(InvalidLookahead))  return LV_ERROR_INVALIDLOOKAHEAD;
        else if (typeid(e) == typeid(InvalidObjectClassHandle))  return LV_ERROR_INVALIDOBJECTCLASSHANDLE;
        else if (typeid(e) == typeid(InvalidOrderName))  return LV_ERROR_INVALIDORDERNAME;
        else if (typeid(e) == typeid(InvalidOrderType))  return LV_ERROR_INVALIDORDERTYPE;
        else if (typeid(e) == typeid(InvalidParameterHandle))  return LV_ERROR_INVALIDPARAMETERHANDLE;
        else if (typeid(e) == typeid(InvalidRangeBound))  return LV_ERROR_INVALIDRANGEBOUND;
        else if (typeid(e) == typeid(InvalidRegion))  return LV_ERROR_INVALIDREGION;
        else if (typeid(e) == typeid(InvalidResignAction))  return LV_ERROR_INVALIDRESIGNACTION;
        else if (typeid(e) == typeid(InvalidRegionContext))  return LV_ERROR_INVALIDREGIONCONTEXT;
        else if (typeid(e) == typeid(InvalidMessageRetractionHandle))  return LV_ERROR_INVALIDMESSAGERETRACTIONHANDLE;
        else if (typeid(e) == typeid(InvalidServiceGroup))  return LV_ERROR_INVALIDSERVICEGROUP;
        else if (typeid(e) == typeid(InvalidTransportationName))  return LV_ERROR_INVALIDTRANSPORTATIONNAME;
        else if (typeid(e) == typeid(InvalidTransportationType))  return LV_ERROR_INVALIDTRANSPORTATIONTYPE;
        else if (typeid(e) == typeid(InvalidUpdateRateDesignator))  return LV_ERROR_INVALIDUPDATERATEDESIGNATOR;
        else if (typeid(e) == typeid(JoinedFederateIsNotInTimeAdvancingState))  return LV_ERROR_JOINEDFEDERATEISNOTINTIMEADVANCINGSTATE;
        else if (typeid(e) == typeid(LogicalTimeAlreadyPassed))  return LV_ERROR_LOGICALTIMEALREADYPASSED;
        else if (typeid(e) == typeid(MessageCanNoLongerBeRetracted))  return LV_ERROR_MESSAGECANNOLONGERBERETRACTED;
        else if (typeid(e) == typeid(NameNotFound))  return LV_ERROR_NAMENOTFOUND;
        else if (typeid(e) == typeid(NameSetWasEmpty))  return LV_ERROR_NAMESETWASEMPTY;
        else if (typeid(e) == typeid(NoAcquisitionPending))  return LV_ERROR_NOACQUISITIONPENDING;
        else if (typeid(e) == typeid(NotConnected))  return LV_ERROR_NOTCONNECTED;
        else if (typeid(e) == typeid(ObjectClassNotDefined))  return LV_ERROR_OBJECTCLASSNOTDEFINED;
        else if (typeid(e) == typeid(ObjectClassNotKnown))  return LV_ERROR_OBJECTCLASSNOTKNOWN;
        else if (typeid(e) == typeid(ObjectClassNotPublished))  return LV_ERROR_OBJECTCLASSNOTPUBLISHED;
        else if (typeid(e) == typeid(ObjectClassRelevanceAdvisorySwitchIsOff))  return LV_ERROR_OBJECTCLASSRELEVANCEADVISORYSWITCHISOFF;
        else if (typeid(e) == typeid(ObjectClassRelevanceAdvisorySwitchIsOn))  return LV_ERROR_OBJECTCLASSRELEVANCEADVISORYSWITCHISON;
        else if (typeid(e) == typeid(ObjectInstanceNameInUse))  return LV_ERROR_OBJECTINSTANCENAMEINUSE;
        else if (typeid(e) == typeid(ObjectInstanceNameNotReserved))  return LV_ERROR_OBJECTINSTANCENAMENOTRESERVED;
        else if (typeid(e) == typeid(ObjectInstanceNotKnown))  return LV_ERROR_OBJECTINSTANCENOTKNOWN;
        else if (typeid(e) == typeid(OwnershipAcquisitionPending))  return LV_ERROR_OWNERSHIPACQUISITIONPENDING;
        else if (typeid(e) == typeid(RTIinternalError))  return LV_ERROR_RTIINTERNALERROR;
        else if (typeid(e) == typeid(RegionDoesNotContainSpecifiedDimension))  return LV_ERROR_REGIONDOESNOTCONTAINSPECIFIEDDIMENSION;
        else if (typeid(e) == typeid(RegionInUseForUpdateOrSubscription))  return LV_ERROR_REGIONINUSEFORUPDATEORSUBSCRIPTION;
        else if (typeid(e) == typeid(RegionNotCreatedByThisFederate))  return LV_ERROR_REGIONNOTCREATEDBYTHISFEDERATE;
        else if (typeid(e) == typeid(RestoreInProgress))  return LV_ERROR_RESTOREINPROGRESS;
        else if (typeid(e) == typeid(RestoreNotInProgress))  return LV_ERROR_RESTORENOTINPROGRESS;
        else if (typeid(e) == typeid(RestoreNotRequested))  return LV_ERROR_RESTORENOTREQUESTED;
        else if (typeid(e) == typeid(SaveInProgress))  return LV_ERROR_SAVEINPROGRESS;
        else if (typeid(e) == typeid(SaveNotInProgress))  return LV_ERROR_SAVENOTINPROGRESS;
        else if (typeid(e) == typeid(SaveNotInitiated))  return LV_ERROR_SAVENOTINITIATED;
        else if (typeid(e) == typeid(SpecifiedSaveLabelDoesNotExist))  return LV_ERROR_SPECIFIEDSAVELABELDOESNOTEXIST;
        else if (typeid(e) == typeid(SynchronizationPointLabelNotAnnounced))  return LV_ERROR_SYNCHRONIZATIONPOINTLABELNOTANNOUNCED;
        else if (typeid(e) == typeid(TimeConstrainedAlreadyEnabled))  return LV_ERROR_TIMECONSTRAINEDALREADYENABLED;
        else if (typeid(e) == typeid(TimeConstrainedIsNotEnabled))  return LV_ERROR_TIMECONSTRAINEDISNOTENABLED;
        else if (typeid(e) == typeid(TimeRegulationAlreadyEnabled))  return LV_ERROR_TIMEREGULATIONALREADYENABLED;
        else if (typeid(e) == typeid(TimeRegulationIsNotEnabled))  return LV_ERROR_TIMEREGULATIONISNOTENABLED;
        else if (typeid(e) == typeid(UnableToPerformSave))  return LV_ERROR_UNABLETOPERFORMSAVE;
        else if (typeid(e) == typeid(UnknownName))  return LV_ERROR_UNKNOWNNAME;
        else if (typeid(e) == typeid(UnsupportedCallbackModel))  return LV_ERROR_UNSUPPORTEDCALLBACKMODEL;
        else if (typeid(e) == typeid(InternalError))  return LV_ERROR_INTERNALERROR;
        else {return LV_ERROR_UKNOWNERROR;}
    }

} // namespace rti1516eLv