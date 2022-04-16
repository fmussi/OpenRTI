/** @file LabView_EventTest.h
 *
 *  @author L. Sartory
 *  @date   Creation: 18.12.2012
 *
 *  @par LICENCE
 *  @verbatim
 *  Copyright (C) 2012 ADDI-DATA GmbH
 *
 *  ADDI-DATA GmbH
 *  Airpark Business Center
 *  Airport Boulevard B210
 *  77836 Rheinmünster
 *  Germany
 *  Tel: +49(0)7229/1847-0    |  Fax: +49(0)7229/1847-200
 *  http://www.addi-data-com  |  info@addi-data.com
 *  @endverbatim
 **/

BOOL APIENTRY DllMain(HANDLE h_Module, ULONG ul_ReasonForCall, LPVOID lp_Reserved);
void far _stdcall v_SetLabViewInterrupt(LVUserEventRef *p_LabViewEventRef);
void far _stdcall v_ResetLabViewInterrupt();
