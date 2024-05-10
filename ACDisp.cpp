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
#include "acdisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _DAssyCalc properties

CString _DAssyCalc::GetLibraryName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void _DAssyCalc::SetLibraryName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

long _DAssyCalc::GetLibraryVersion()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void _DAssyCalc::SetLibraryVersion(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long _DAssyCalc::GetMethodCount()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void _DAssyCalc::SetMethodCount(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// _DAssyCalc operations

LPDISPATCH _DAssyCalc::GetMethodPtr(long methodID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		methodID);
	return result;
}

long _DAssyCalc::GetMethodID(long iMethod)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		iMethod);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _DAssyCalcEvents properties

/////////////////////////////////////////////////////////////////////////////
// _DAssyCalcEvents operations


/////////////////////////////////////////////////////////////////////////////
// IACalcMethod properties

long IACalcMethod::GetVersion()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetVersion(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long IACalcMethod::GetId()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetId(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

CString IACalcMethod::GetName()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void IACalcMethod::SetName(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

long IACalcMethod::GetInputFieldCount()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetInputFieldCount(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

long IACalcMethod::GetOutputFieldCount()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetOutputFieldCount(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

long IACalcMethod::GetSampTypeCount()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetSampTypeCount(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

long IACalcMethod::GetNumSamples()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetNumSamples(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

long IACalcMethod::GetNumParams()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetNumParams(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

long IACalcMethod::GetNumCalVals()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetNumCalVals(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

BOOL IACalcMethod::GetIsCalibrated()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}

void IACalcMethod::SetIsCalibrated(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}

long IACalcMethod::GetNumCalSamps()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void IACalcMethod::SetNumCalSamps(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IACalcMethod operations

long IACalcMethod::GetSampleData(long iSamp, long fieldID, VARIANT* pValue)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iSamp, fieldID, pValue);
	return result;
}

long IACalcMethod::Init()
{
	long result;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::ClearCalc()
{
	long result;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::ValidateParameters(LPCTSTR errMsg)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		errMsg);
	return result;
}

LPDISPATCH IACalcMethod::GetParamPtr(long paramID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		paramID);
	return result;
}

long IACalcMethod::GetParamID(long iParam)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		iParam);
	return result;
}

long IACalcMethod::GetInputFieldID(long iInputField)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		iInputField);
	return result;
}

long IACalcMethod::GetOutputFieldID(long iOutputField)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		iOutputField);
	return result;
}

long IACalcMethod::GetSampTypeID(long iSampType)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iSampType);
	return result;
}

CString IACalcMethod::GetSampTypeName(long sampTypeID)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		sampTypeID);
	return result;
}

long IACalcMethod::SetNewSampleData(long fieldID, const VARIANT& value)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		fieldID, &value);
	return result;
}

long IACalcMethod::FinishNewSample()
{
	long result;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::AddCalSamples()
{
	long result;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::StartCalc()
{
	long result;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::StartValidation()
{
	long result;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::CreateCalibs()
{
	long result;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::FinishValidation()
{
	long result;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long IACalcMethod::FinishCalc()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IACalcMethod::GetCalValPtr(long calValID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		calValID);
	return result;
}

long IACalcMethod::GetCalValID(long iCalVal)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		iCalVal);
	return result;
}

long IACalcMethod::GetCalSampTypeID(long iCalSamp)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iCalSamp);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IVParam properties

CString IVParam::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void IVParam::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

long IVParam::GetId()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void IVParam::SetId(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

VARIANT IVParam::GetMax()
{
	VARIANT result;
	GetProperty(0x3, VT_VARIANT, (void*)&result);
	return result;
}

void IVParam::SetMax(const VARIANT& propVal)
{
	SetProperty(0x3, VT_VARIANT, &propVal);
}

VARIANT IVParam::GetMin()
{
	VARIANT result;
	GetProperty(0x4, VT_VARIANT, (void*)&result);
	return result;
}

void IVParam::SetMin(const VARIANT& propVal)
{
	SetProperty(0x4, VT_VARIANT, &propVal);
}

VARIANT IVParam::GetDefault()
{
	VARIANT result;
	GetProperty(0x5, VT_VARIANT, (void*)&result);
	return result;
}

void IVParam::SetDefault(const VARIANT& propVal)
{
	SetProperty(0x5, VT_VARIANT, &propVal);
}

CString IVParam::GetHelpString()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void IVParam::SetHelpString(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IVParam operations

long IVParam::SetValue(const VARIANT& value)
{
	long result;
	static BYTE parms[] = VTS_VARIANT;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, &value);
	return result;
}

VARIANT IVParam::GetValue()
{
	VARIANT result;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}
