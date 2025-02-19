#ifndef RTI1516ELV_ERRORS_H
#define RTI1516ELV_ERRORS_H

// #ifdef __cplusplus
// extern "C" {
// #endif

// using namespace rti1516e;
#define LV_ERROR_RANGE_OFFSET   -500000
#define LV_WARNING_RANGE_OFFSET 500000

//Ambassador specific error codes
#define LV_ERROR_UKNOWNERROR                            LV_ERROR_RANGE_OFFSET
#define	LV_ERROR_ALREADYCONNECTED	                    LV_ERROR_RANGE_OFFSET-1
#define	LV_ERROR_ASYNCHRONOUSDELIVERYALREADYDISABLED	LV_ERROR_RANGE_OFFSET-2
#define	LV_ERROR_ASYNCHRONOUSDELIVERYALREADYENABLED	        LV_ERROR_RANGE_OFFSET-3
#define	LV_ERROR_ATTRIBUTEACQUISITIONWASNOTCANCELED	LV_ERROR_RANGE_OFFSET-4
#define	LV_ERROR_ATTRIBUTEACQUISITIONWASNOTREQUESTED	LV_ERROR_RANGE_OFFSET-5
#define	LV_ERROR_ATTRIBUTEALREADYBEINGACQUIRED	LV_ERROR_RANGE_OFFSET-6
#define	LV_ERROR_ATTRIBUTEALREADYBEINGCHANGED	LV_ERROR_RANGE_OFFSET-7
#define	LV_ERROR_ATTRIBUTEALREADYBEINGDIVESTED	LV_ERROR_RANGE_OFFSET-8
#define	LV_ERROR_ATTRIBUTEALREADYOWNED	LV_ERROR_RANGE_OFFSET-9
#define	LV_ERROR_ATTRIBUTEDIVESTITUREWASNOTREQUESTED	LV_ERROR_RANGE_OFFSET-10
#define	LV_ERROR_ATTRIBUTENOTDEFINED	LV_ERROR_RANGE_OFFSET-11
#define	LV_ERROR_ATTRIBUTENOTOWNED	LV_ERROR_RANGE_OFFSET-12
#define	LV_ERROR_ATTRIBUTENOTPUBLISHED	LV_ERROR_RANGE_OFFSET-13
#define	LV_ERROR_ATTRIBUTENOTRECOGNIZED	LV_ERROR_RANGE_OFFSET-14
#define	LV_ERROR_ATTRIBUTENOTSUBSCRIBED	LV_ERROR_RANGE_OFFSET-15
#define	LV_ERROR_ATTRIBUTERELEVANCEADVISORYSWITCHISOFF	LV_ERROR_RANGE_OFFSET-16
#define	LV_ERROR_ATTRIBUTERELEVANCEADVISORYSWITCHISON	LV_ERROR_RANGE_OFFSET-17
#define	LV_ERROR_ATTRIBUTESCOPEADVISORYSWITCHISOFF	LV_ERROR_RANGE_OFFSET-18
#define	LV_ERROR_ATTRIBUTESCOPEADVISORYSWITCHISON	LV_ERROR_RANGE_OFFSET-19
#define	LV_ERROR_BADINITIALIZATIONPARAMETER	LV_ERROR_RANGE_OFFSET-20
#define	LV_ERROR_CALLNOTALLOWEDFROMWITHINCALLBACK	LV_ERROR_RANGE_OFFSET-21
#define	LV_ERROR_CONNECTIONFAILED	LV_ERROR_RANGE_OFFSET-22
#define	LV_ERROR_COULDNOTCREATELOGICALTIMEFACTORY	LV_ERROR_RANGE_OFFSET-23
#define	LV_ERROR_COULDNOTDECODE	LV_ERROR_RANGE_OFFSET-24
#define	LV_ERROR_COULDNOTDISCOVER	LV_ERROR_RANGE_OFFSET-25
#define	LV_ERROR_COULDNOTENCODE	LV_ERROR_RANGE_OFFSET-26
#define	LV_ERROR_COULDNOTOPENFDD	LV_ERROR_RANGE_OFFSET-27
#define	LV_ERROR_COULDNOTOPENMIM	LV_ERROR_RANGE_OFFSET-28
#define	LV_ERROR_COULDNOTINITIATERESTORE	LV_ERROR_RANGE_OFFSET-29
#define	LV_ERROR_DELETEPRIVILEGENOTHELD	LV_ERROR_RANGE_OFFSET-30
#define	LV_ERROR_DESIGNATORISHLASTANDARDMIM	LV_ERROR_RANGE_OFFSET-31
#define	LV_ERROR_REQUESTFORTIMECONSTRAINEDPENDING	LV_ERROR_RANGE_OFFSET-32
#define	LV_ERROR_NOREQUESTTOENABLETIMECONSTRAINEDWASPENDING	LV_ERROR_RANGE_OFFSET-33
#define	LV_ERROR_REQUESTFORTIMEREGULATIONPENDING	LV_ERROR_RANGE_OFFSET-34
#define	LV_ERROR_NOREQUESTTOENABLETIMEREGULATIONWASPENDING	LV_ERROR_RANGE_OFFSET-35
#define	LV_ERROR_NOFEDERATEWILLINGTOACQUIREATTRIBUTE	LV_ERROR_RANGE_OFFSET-36
#define	LV_ERROR_ERRORREADINGFDD	LV_ERROR_RANGE_OFFSET-37
#define	LV_ERROR_ERRORREADINGMIM	LV_ERROR_RANGE_OFFSET-38
#define	LV_ERROR_FEDERATEALREADYEXECUTIONMEMBER	LV_ERROR_RANGE_OFFSET-39
#define	LV_ERROR_FEDERATEHANDLENOTKNOWN	LV_ERROR_RANGE_OFFSET-40
#define	LV_ERROR_FEDERATEHASNOTBEGUNSAVE	LV_ERROR_RANGE_OFFSET-41
#define	LV_ERROR_FEDERATEINTERNALERROR	LV_ERROR_RANGE_OFFSET-42
#define	LV_ERROR_FEDERATEISEXECUTIONMEMBER	LV_ERROR_RANGE_OFFSET-43
#define	LV_ERROR_FEDERATENAMEALREADYINUSE	LV_ERROR_RANGE_OFFSET-44
#define	LV_ERROR_FEDERATENOTEXECUTIONMEMBER	LV_ERROR_RANGE_OFFSET-45
#define	LV_ERROR_FEDERATEOWNSATTRIBUTES	LV_ERROR_RANGE_OFFSET-46
#define	LV_ERROR_FEDERATESERVICEINVOCATIONSAREBEINGREPORTEDVIAMOM	LV_ERROR_RANGE_OFFSET-47
#define	LV_ERROR_FEDERATEUNABLETOUSETIME	LV_ERROR_RANGE_OFFSET-48
#define	LV_ERROR_FEDERATESCURRENTLYJOINED	LV_ERROR_RANGE_OFFSET-49
#define	LV_ERROR_FEDERATIONEXECUTIONALREADYEXISTS	LV_ERROR_RANGE_OFFSET-50
#define	LV_ERROR_FEDERATIONEXECUTIONDOESNOTEXIST	LV_ERROR_RANGE_OFFSET-51
#define	LV_ERROR_ILLEGALNAME	LV_ERROR_RANGE_OFFSET-52
#define	LV_ERROR_ILLEGALTIMEARITHMETIC	LV_ERROR_RANGE_OFFSET-53
#define	LV_ERROR_INCONSISTENTFDD	LV_ERROR_RANGE_OFFSET-54
#define	LV_ERROR_INTERACTIONCLASSALREADYBEINGCHANGED	LV_ERROR_RANGE_OFFSET-55
#define	LV_ERROR_INTERACTIONCLASSNOTDEFINED	LV_ERROR_RANGE_OFFSET-56
#define	LV_ERROR_INTERACTIONCLASSNOTPUBLISHED	LV_ERROR_RANGE_OFFSET-57
#define	LV_ERROR_INTERACTIONCLASSNOTRECOGNIZED	LV_ERROR_RANGE_OFFSET-58
#define	LV_ERROR_INTERACTIONCLASSNOTSUBSCRIBED	LV_ERROR_RANGE_OFFSET-59
#define	LV_ERROR_INTERACTIONPARAMETERNOTDEFINED	LV_ERROR_RANGE_OFFSET-60
#define	LV_ERROR_INTERACTIONPARAMETERNOTRECOGNIZED	LV_ERROR_RANGE_OFFSET-61
#define	LV_ERROR_INTERACTIONRELEVANCEADVISORYSWITCHISOFF	LV_ERROR_RANGE_OFFSET-62
#define	LV_ERROR_INTERACTIONRELEVANCEADVISORYSWITCHISON	LV_ERROR_RANGE_OFFSET-63
#define	LV_ERROR_INTIMEADVANCINGSTATE	LV_ERROR_RANGE_OFFSET-64
#define	LV_ERROR_INVALIDATTRIBUTEHANDLE	LV_ERROR_RANGE_OFFSET-65
#define	LV_ERROR_INVALIDDIMENSIONHANDLE	LV_ERROR_RANGE_OFFSET-66
#define	LV_ERROR_INVALIDFEDERATEHANDLE	LV_ERROR_RANGE_OFFSET-67
#define	LV_ERROR_INVALIDINTERACTIONCLASSHANDLE	LV_ERROR_RANGE_OFFSET-68
#define	LV_ERROR_INVALIDLOCALSETTINGSDESIGNATOR	LV_ERROR_RANGE_OFFSET-69
#define	LV_ERROR_INVALIDLOGICALTIME	LV_ERROR_RANGE_OFFSET-70
#define	LV_ERROR_INVALIDLOGICALTIMEINTERVAL	LV_ERROR_RANGE_OFFSET-71
#define	LV_ERROR_INVALIDLOOKAHEAD	LV_ERROR_RANGE_OFFSET-72
#define	LV_ERROR_INVALIDOBJECTCLASSHANDLE	LV_ERROR_RANGE_OFFSET-73
#define	LV_ERROR_INVALIDORDERNAME	LV_ERROR_RANGE_OFFSET-74
#define	LV_ERROR_INVALIDORDERTYPE	LV_ERROR_RANGE_OFFSET-75
#define	LV_ERROR_INVALIDPARAMETERHANDLE	LV_ERROR_RANGE_OFFSET-76
#define	LV_ERROR_INVALIDRANGEBOUND	LV_ERROR_RANGE_OFFSET-77
#define	LV_ERROR_INVALIDREGION	LV_ERROR_RANGE_OFFSET-78
#define	LV_ERROR_INVALIDRESIGNACTION	LV_ERROR_RANGE_OFFSET-79
#define	LV_ERROR_INVALIDREGIONCONTEXT	LV_ERROR_RANGE_OFFSET-80
#define	LV_ERROR_INVALIDMESSAGERETRACTIONHANDLE	LV_ERROR_RANGE_OFFSET-81
#define	LV_ERROR_INVALIDSERVICEGROUP	LV_ERROR_RANGE_OFFSET-82
#define	LV_ERROR_INVALIDTRANSPORTATIONNAME	LV_ERROR_RANGE_OFFSET-83
#define	LV_ERROR_INVALIDTRANSPORTATIONTYPE	LV_ERROR_RANGE_OFFSET-84
#define	LV_ERROR_INVALIDUPDATERATEDESIGNATOR	LV_ERROR_RANGE_OFFSET-85
#define	LV_ERROR_JOINEDFEDERATEISNOTINTIMEADVANCINGSTATE	LV_ERROR_RANGE_OFFSET-86
#define	LV_ERROR_LOGICALTIMEALREADYPASSED	LV_ERROR_RANGE_OFFSET-87
#define	LV_ERROR_MESSAGECANNOLONGERBERETRACTED	LV_ERROR_RANGE_OFFSET-88
#define	LV_ERROR_NAMENOTFOUND	LV_ERROR_RANGE_OFFSET-89
#define	LV_ERROR_NAMESETWASEMPTY	LV_ERROR_RANGE_OFFSET-90
#define	LV_ERROR_NOACQUISITIONPENDING	LV_ERROR_RANGE_OFFSET-91
#define	LV_ERROR_NOTCONNECTED	LV_ERROR_RANGE_OFFSET-92
#define	LV_ERROR_OBJECTCLASSNOTDEFINED	LV_ERROR_RANGE_OFFSET-93
#define	LV_ERROR_OBJECTCLASSNOTKNOWN	LV_ERROR_RANGE_OFFSET-94
#define	LV_ERROR_OBJECTCLASSNOTPUBLISHED	LV_ERROR_RANGE_OFFSET-95
#define	LV_ERROR_OBJECTCLASSRELEVANCEADVISORYSWITCHISOFF	LV_ERROR_RANGE_OFFSET-96
#define	LV_ERROR_OBJECTCLASSRELEVANCEADVISORYSWITCHISON	LV_ERROR_RANGE_OFFSET-97
#define	LV_ERROR_OBJECTINSTANCENAMEINUSE	LV_ERROR_RANGE_OFFSET-98
#define	LV_ERROR_OBJECTINSTANCENAMENOTRESERVED	LV_ERROR_RANGE_OFFSET-99
#define	LV_ERROR_OBJECTINSTANCENOTKNOWN	LV_ERROR_RANGE_OFFSET-100
#define	LV_ERROR_OWNERSHIPACQUISITIONPENDING	LV_ERROR_RANGE_OFFSET-101
#define	LV_ERROR_RTIINTERNALERROR	LV_ERROR_RANGE_OFFSET-102
#define	LV_ERROR_REGIONDOESNOTCONTAINSPECIFIEDDIMENSION	LV_ERROR_RANGE_OFFSET-103
#define	LV_ERROR_REGIONINUSEFORUPDATEORSUBSCRIPTION	LV_ERROR_RANGE_OFFSET-104
#define	LV_ERROR_REGIONNOTCREATEDBYTHISFEDERATE	LV_ERROR_RANGE_OFFSET-105
#define	LV_ERROR_RESTOREINPROGRESS	LV_ERROR_RANGE_OFFSET-106
#define	LV_ERROR_RESTORENOTINPROGRESS	LV_ERROR_RANGE_OFFSET-107
#define	LV_ERROR_RESTORENOTREQUESTED	LV_ERROR_RANGE_OFFSET-108
#define	LV_ERROR_SAVEINPROGRESS	LV_ERROR_RANGE_OFFSET-109
#define	LV_ERROR_SAVENOTINPROGRESS	LV_ERROR_RANGE_OFFSET-110
#define	LV_ERROR_SAVENOTINITIATED	LV_ERROR_RANGE_OFFSET-111
#define	LV_ERROR_SPECIFIEDSAVELABELDOESNOTEXIST	LV_ERROR_RANGE_OFFSET-112
#define	LV_ERROR_SYNCHRONIZATIONPOINTLABELNOTANNOUNCED	LV_ERROR_RANGE_OFFSET-113
#define	LV_ERROR_TIMECONSTRAINEDALREADYENABLED	LV_ERROR_RANGE_OFFSET-114
#define	LV_ERROR_TIMECONSTRAINEDISNOTENABLED	LV_ERROR_RANGE_OFFSET-115
#define	LV_ERROR_TIMEREGULATIONALREADYENABLED	LV_ERROR_RANGE_OFFSET-116
#define	LV_ERROR_TIMEREGULATIONISNOTENABLED	LV_ERROR_RANGE_OFFSET-117
#define	LV_ERROR_UNABLETOPERFORMSAVE	LV_ERROR_RANGE_OFFSET-118
#define	LV_ERROR_UNKNOWNNAME	LV_ERROR_RANGE_OFFSET-119
#define	LV_ERROR_UNSUPPORTEDCALLBACKMODEL	LV_ERROR_RANGE_OFFSET-120
#define	LV_ERROR_INTERNALERROR	LV_ERROR_RANGE_OFFSET-121

// Generic Error codes
#define LV_ERROR_MAPOUTOFRANGE	LV_ERROR_RANGE_OFFSET-122


//Ambassador specific warning codes
#define	LV_WARNING_FEDERATESCURRENTLYJOINED	LV_WARNING_RANGE_OFFSET+49
#define	LV_WARNING_FEDERATIONEXECUTIONALREADYEXISTS LV_WARNING_RANGE_OFFSET+50

//#define LV_ERROR_INVALID_LOCAL_SETTINGS_DESIGNATOR  LV_ERROR_RANGE_OFFSET-2

#endif

// #ifdef __cplusplus
// } // extern "C"
// #endif