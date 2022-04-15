/* Windows includes */
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "extcode.h"

#include "LabView_EventTest.h"

/* Platform specific LabView includes and lib
#ifdef _WIN64
	#include "C:\\Program Files (x86)\\National Instruments\\LabVIEW 2020\\cintools\\extcode.h"
	#pragma comment(lib, "C:\\Program Files (x86)\\National Instruments\\LabVIEW 2020\\cintools\\labview.lib")
#else
	#include "cintools/32b/extcode.h"
	#pragma comment(lib, "cintools/32b/labview.lib")
#endif
*/

/* Global variables */
LVUserEventRef dw_LabViewEventRef;
unsigned __int8 b_ThreadState;

#pragma pack (push,1)
typedef struct
{
	unsigned __int8  b_FifoOverflow;
	unsigned __int32 dw_InterruptSource;
	unsigned __int32 dw_InterruptMask;
} interruptStruct_t;
#pragma pack(pop)

BOOL APIENTRY DllMain(HANDLE h_Module, ULONG ul_ReasonForCall, LPVOID lp_Reserved)
{
	UNREFERENCED_PARAMETER(h_Module);
	UNREFERENCED_PARAMETER(ul_ReasonForCall);
	UNREFERENCED_PARAMETER(lp_Reserved);
	return TRUE;
}

/**
*  Specific interrupt handler for LabView
**/
void far _stdcall LabViewEventHandler(unsigned __int8   b_FifoOverflow,
                                      unsigned __int32  dw_InterruptSource,
                                      unsigned __int32  dw_InterruptMask)
{
	/* Declare & initialize the LabView structure */
	interruptStruct_t interruptStruct;
	memset(&interruptStruct, 0, sizeof(interruptStruct));

	/* Copy the values in the structure */
	interruptStruct.b_FifoOverflow     = b_FifoOverflow;
	interruptStruct.dw_InterruptSource = dw_InterruptSource;
	interruptStruct.dw_InterruptMask   = dw_InterruptMask;

	/* Post the event to LabView */
	PostLVUserEvent(dw_LabViewEventRef, (void *)&interruptStruct);
}

void EventThread(void *dummy)
{
	unsigned __int8  b_FifoOverflow = 0;
	unsigned __int32 dw_InterruptSource = 0;
	unsigned __int32 dw_InterruptMask = 0;

	UNREFERENCED_PARAMETER(dummy);

	while (b_ThreadState != 0)
	{
		
		b_FifoOverflow++;
		dw_InterruptSource += 2;
		dw_InterruptMask = dw_InterruptSource * b_FifoOverflow;

		LabViewEventHandler(b_FifoOverflow, dw_InterruptSource, dw_InterruptMask);
		
	}

	_endthread();
}

void far _stdcall v_SetLabViewInterrupt(LVUserEventRef *p_LabViewEventRef)
{
	dw_LabViewEventRef = *p_LabViewEventRef;

	b_ThreadState = 1;
	_beginthread(EventThread, 0, NULL);
}

void far _stdcall v_ResetLabViewInterrupt()
{
	b_ThreadState = 0;
}
