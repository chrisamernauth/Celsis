/************************************************************************************
COPYRIGHT © 2001 Celsis Limited
All International Rights Reserved
(This work is unpublished)

CONFIDENTIAL TRADE SECRET
This material is the property of Celsis Limited and may not be 
used, copied, or distributed in any form without written 
permission from Celsis.

     Celsis Limited
     Cambridge Science Park
     Milton Road, Cambridge, CB4 0FX, UK
     Telephone: +44 (0)1223 426008
     Facsimile:   +44 (0)1223 426003
     Web address: http://www.Celsis.com

FILE STATUS
Design Specification:  887/S/02

Modifications:
(WHO)           DD-MMM-YY   		(REASON)
ITL				01-Sept-01			New documentation base line of version 2.1.7

************************************************************************************/

// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "lccproxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ILcc properties

/////////////////////////////////////////////////////////////////////////////
// ILcc operations

long ILcc::Synch()
{
	long result;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long ILcc::Inquire(long InquireType, VARIANT* Status)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		InquireType, Status);
	return result;
}

long ILcc::CSOpen(long ClassID, long ProcID)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID);
	return result;
}

long ILcc::CSStart()
{
	long result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long ILcc::CSStop(long Mode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Mode);
	return result;
}

long ILcc::CSClose()
{
	long result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long ILcc::CSGetStatus()
{
	long result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long ILcc::GetProcLast(long ClassID)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID);
	return result;
}

long ILcc::GetProcName(long ClassID, long ProcID, VARIANT* Name)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, Name);
	return result;
}

long ILcc::GetProcAttr(long ClassID, long ProcID, VARIANT* Attribute)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, Attribute);
	return result;
}

long ILcc::GetParamLast(long ClassID, long ProcID, VARIANT* ParamID)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, ParamID);
	return result;
}

long ILcc::GetParamName(long ClassID, long ProcID, long ParamID, VARIANT* Name)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, ParamID, Name);
	return result;
}

long ILcc::GetParamType(long ClassID, long ProcID, long ParamID, VARIANT* ParamType)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, ParamID, ParamType);
	return result;
}

long ILcc::GetParamMaxVal(long ClassID, long ProcID, long ParamID, VARIANT* Limit)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, ParamID, Limit);
	return result;
}

long ILcc::GetParamMinVal(long ClassID, long ProcID, long ParamID, VARIANT* Limit)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, ParamID, Limit);
	return result;
}

long ILcc::GetParamNomVal(long ClassID, long ProcID, long ParamID, VARIANT* Value)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ClassID, ProcID, ParamID, Value);
	return result;
}

long ILcc::CSGetReagent(long InjectorID, VARIANT* Volume)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		InjectorID, Volume);
	return result;
}

long ILcc::CSGetTimeLeft(VARIANT* Time)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Time);
	return result;
}

long ILcc::CSGetData(long Array, long Index, VARIANT* Value)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Array, Index, Value);
	return result;
}

long ILcc::CSSetParam(long ParamID, VARIANT* Value)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ParamID, Value);
	return result;
}

long ILcc::ShowWindow()
{
	long result;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
