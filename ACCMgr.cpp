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


// ACCMgr.cpp : implementation file
//

#include "stdafx.h"
#include "ACCMgr.h"
#include "ACDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CACCMgr::m_instances = 0;

/////////////////////////////////////////////////////////////////////////////
// CACCMgr

IMPLEMENT_DYNCREATE(CACCMgr, CCmdTarget)

CACCMgr::CACCMgr(CWnd* pParent)
{
	m_instances++;

	// Create dialog containing assay calc control
	m_calcDlg.Create(IDD_ACALC_CTNR, pParent);

	if (m_calcDlg.GetSafeHwnd() != NULL)
	{	// Get pointer to calc control proxy
		m_pCalcCtrl = &m_calcDlg.m_Acc;
	}
	else
	{
		AfxMessageBox(IDS_ASSY_CALC_CREATE);
		m_pCalcCtrl = NULL;
	}

	m_methodID = LUC_ID_UNDEF;
	m_pMethod = NULL;
}

CACCMgr::~CACCMgr()
{
	DeselectMethod();
	m_calcDlg.DestroyWindow();

	m_instances--;
}


BEGIN_MESSAGE_MAP(CACCMgr, CCmdTarget)
	//{{AFX_MSG_MAP(CACCMgr)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Assay Calc Library General Method Info
/////////////////////////////////////////////////////////////////////////////

CString CACCMgr::GetLibraryName()
{
	if (m_pCalcCtrl == NULL)
	{
		CString empty;
		return empty;
	}
	else
	{
		return m_pCalcCtrl->GetLibraryName();
	}
}

int CACCMgr::GetNumMethods()
{
	if (m_pCalcCtrl == NULL)
	{
		return 0;
	}
	else
	{
		return m_pCalcCtrl->GetMethodCount();
	}
}

long CACCMgr::GetMethodID(long iMethod)
{
	if (m_pCalcCtrl == NULL)
	{
		return LUC_ID_UNDEF;
	}
	else
	{
		return m_pCalcCtrl->GetMethodID(iMethod);
	}
}

CString CACCMgr::GetMethodName(long methodID)
{
	if (m_pCalcCtrl == NULL)
	{
		CString empty;
		return empty;
	}

	LPDISPATCH pMeth = m_pCalcCtrl->GetMethodPtr(methodID);
	ASSERT(pMeth != NULL);

	// Create tempory method proxy object just to read name
	IACalcMethod meth(pMeth);

	return meth.GetName();
}


/////////////////////////////////////////////////////////////////////////////
// Select Method
/////////////////////////////////////////////////////////////////////////////

bool CACCMgr::SelectMethod(long methodID)
{
	DeselectMethod();
	
	if (methodID == LUC_ID_UNDEF)
	{
		return true;
	}

	if (m_pCalcCtrl == NULL)
	{
		return false;
	}

	LPDISPATCH pMeth = m_pCalcCtrl->GetMethodPtr(methodID);
	if(pMeth == NULL)
	{
		return false;
	}

	m_pMethod = new IACalcMethod(pMeth);
	m_methodID = methodID;

	return true;
}

bool CACCMgr::DeselectMethod()
{
	if (m_pMethod != NULL)
	{
		delete m_pMethod;
	}

	m_methodID = LUC_ID_UNDEF;
	m_pMethod = NULL;

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// Get Method's Parameter info
/////////////////////////////////////////////////////////////////////////////

int CACCMgr::GetNumParams()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->GetNumParams();
}


long CACCMgr::GetParamID(int iParam)
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->GetParamID(iParam);
}


CParam CACCMgr::GetParam(int paramID)
{
	CParam param;
	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL && paramID != LUC_ID_UNDEF);

	// Get parameter pointer
	LPDISPATCH pParam = m_pMethod->GetParamPtr(paramID);
	if (pParam == NULL)
	{	// Bad paramerter id, return empty param 
		return param;
	}

	// Create proxy object
	IVParam IDispParam(pParam);

	// Create a CParam and load it with the parameter info
	param.m_ID = paramID;
	param.m_name = IDispParam.GetName();
	param.m_value = IDispParam.GetValue();

	return param;
}

CParamEx CACCMgr::GetParamEx(long paramID)
{
	CParamEx paramx;
	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL && paramID != LUC_ID_UNDEF);

	// Get parameter pointer
	LPDISPATCH pParam = m_pMethod->GetParamPtr(paramID);
	if (pParam == NULL)
	{	// Bad paramerter id, return empty param 
		return paramx;
	}

	// Create proxy object
	IVParam IDispParam(pParam);

	// Load parameter info
	paramx.m_ID			= paramID;
	paramx.m_name		= IDispParam.GetName();
	paramx.m_value		= IDispParam.GetValue();
	paramx.m_max		= IDispParam.GetMax();
	paramx.m_min		= IDispParam.GetMin();
	paramx.m_default	= IDispParam.GetDefault();

	return paramx;
}

CString CACCMgr::GetParamName(long paramID)
{
	return GetParam(paramID).m_name;
}

// Disable warning about converting int to bool
#pragma warning(disable : 4800)

long CACCMgr::SetParamValue(CParam& param)
{	// Assumes a method has been selected
	if (m_pMethod == NULL || param.m_ID == LUC_ID_UNDEF)
	{
		return false;
	}

	// Get parameter pointer
	LPDISPATCH pParam = m_pMethod->GetParamPtr(param.m_ID);
	if (pParam == NULL)
	{
		return true;
	}

	// Create proxy object
	IVParam IDispParam(pParam);

	return IDispParam.SetValue(param.m_value);
}
// Re-enable warning
#pragma warning(default : 4800)


/////////////////////////////////////////////////////////////////////////////
// Get Method's Sample Type info
/////////////////////////////////////////////////////////////////////////////

long CACCMgr::GetNumSampTypes()
{
	if (m_pMethod != NULL)
	{
		return m_pMethod->GetSampTypeCount();
	}
	else
	{
		return -1;
	}
}

long CACCMgr::GetNumCalSampTypes()
{
	if (m_pMethod != NULL)
	{
		return m_pMethod->GetNumCalSamps();
	}
	else
	{
		return -1;
	}
}

long CACCMgr::GetSampTypeID(int iSampType)
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->GetSampTypeID(iSampType);
}

long CACCMgr::GetCalSampTypeID(int iCalSamp)
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->GetCalSampTypeID(iCalSamp);
}

CString CACCMgr::GetSampTypeName(long sampTypeID)
{
	CParam param;
	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL && sampTypeID != LUC_ID_UNDEF);

	return m_pMethod->GetSampTypeName(sampTypeID);
}


/////////////////////////////////////////////////////////////////////////////
// Get Method's required calibration samples
/////////////////////////////////////////////////////////////////////////////

long CACCMgr::CreateCalSamps()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->CreateCalibs();
}


/////////////////////////////////////////////////////////////////////////////
// Get number of samples in ACC
/////////////////////////////////////////////////////////////////////////////

int CACCMgr::GetNumSamples()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->GetNumSamples();
}


/////////////////////////////////////////////////////////////////////////////
// GetSampleData
/////////////////////////////////////////////////////////////////////////////

long CACCMgr::GetSampleData(long iSamp, long fieldID, VARIANT* pValue)
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->GetSampleData(iSamp, fieldID, pValue);
}


/////////////////////////////////////////////////////////////////////////////
// SetNewSampleData
/////////////////////////////////////////////////////////////////////////////

long CACCMgr::SetNewSampleData(long fieldID, COleVariant& value)
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->SetNewSampleData(fieldID, value);
}


/////////////////////////////////////////////////////////////////////////////
// FinishNewSample
/////////////////////////////////////////////////////////////////////////////

long CACCMgr::FinishNewSample()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->FinishNewSample();
}

long CACCMgr::StartCalc()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->StartCalc();
}


long CACCMgr::ClearCalc()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->ClearCalc();
}

long CACCMgr::StartValidation()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->StartValidation();
}

long CACCMgr::GetCalValID(int iCalVal)
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

	return m_pMethod->GetParamID(iCalVal);
}

int CACCMgr::GetNumCalVals()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);
	
	return m_pMethod->GetNumCalVals();
}

CParam CACCMgr::GetCalVal(long calValID)
{
	CParam param;
	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL && calValID != LUC_ID_UNDEF);

	// Get parameter pointer
	LPDISPATCH pCalVal = m_pMethod->GetCalValPtr(calValID);
	if (pCalVal == NULL)
	{	// Bad paramerter id, return empty param 
		return param;
	}

	// Create proxy object
	IVParam IDispParam(pCalVal);

	// Create a CParam and load it with the parameter info
	param.m_ID = calValID;
	param.m_name = IDispParam.GetName();
	param.m_value = IDispParam.GetValue();

	return param;
}

bool CACCMgr::IsCalibrated()
{	// Assumes a method has been selected
	ASSERT(m_pMethod != NULL);

#pragma warning(disable:4800)
	return m_pMethod->GetIsCalibrated();
#pragma warning(default:4800)
}


