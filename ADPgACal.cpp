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

// ADPgACal.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "ADPgACal.h"
#include "ADPrpSh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADPgACal dialog

CADPgACal::CADPgACal(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CADPgACal::IDD)
{
	//{{AFX_DATA_INIT(CADPgACal)
	m_defParamValTxt = _T("");
	m_maxParamValTxt = _T("");
	m_minParamValTxt = _T("");
	//}}AFX_DATA_INIT
	m_pACalc = NULL;
	m_pParamValCB = NULL;
}

CADPgACal::~CADPgACal()
{
	if (m_pParamValCB != NULL)
	{
		delete m_pParamValCB;
	}
}

void CADPgACal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADPgACal)
	DDX_Control(pDX, IDC_AD_CALC_PARMS, m_calcParamLB);
	DDX_Control(pDX, IDC_AD_CALC_METH, m_calcMethCB);
	DDX_Text(pDX, IDC_AD_DEF_VAL, m_defParamValTxt);
	DDX_Text(pDX, IDC_AD_MAX_VAL, m_maxParamValTxt);
	DDX_Text(pDX, IDC_AD_MIN_VAL, m_minParamValTxt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADPgACal, CPropertyPage)
	//{{AFX_MSG_MAP(CADPgACal)
	ON_CBN_SELCHANGE(IDC_AD_CALC_METH, OnSelchangeAdCalcMeth)
	ON_LBN_SELCHANGE(IDC_AD_CALC_PARMS, OnSelchangeCalcParam)
	ON_CBN_KILLFOCUS(IDC_AD_CPARM_VAL, OnKillfocusAdCparmVal)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADPgACal message handlers

////////////////////////////////////////////////////////////////////////
// Save assay def pointer -- called from property sheet's contructor
////////////////////////////////////////////////////////////////////////

long CADPgACal::SetAssayDef(CAssayDef * pAssayDef)
{
	m_pAD = pAssayDef;

	return KO_OK;
}

////////////////////////////////////////////////////////////////////////
// Create and initialize controls -- 
// called after property sheet's DoModal (m_pAD is valid)
////////////////////////////////////////////////////////////////////////


BOOL CADPgACal::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Get pointer to assay calc control
	CADPropSheet* pSheet = (CADPropSheet*)GetParent();
	ASSERT(pSheet != NULL);
	ASSERT(pSheet->IsKindOf(RUNTIME_CLASS(CADPropSheet)));
	m_pACalc = pSheet->m_pACalc;

	// Create Parameter edit box initially empty
	CParamEx tempParam;
	ASSERT(tempParam.m_value.vt == VT_EMPTY);
	m_pParamValCB = new CParamBoxEx(&tempParam);
	VERIFY(m_pParamValCB->Create(IDC_AD_CPARM_VAL, this));

	// Init Assay Calculation Method Box and set its initial selection
	m_calcMethCB.ResetContent();
	m_calcMethCB.SetCurSel(-1);

	ASSERT(m_pAD != NULL);
	InitACalcMethodBox(m_pAD->GetCalcMethID());

	UpdateData(FALSE);

	return TRUE;  
}

////////////////////////////////////////////////////////////////////////
// Do anything?
////////////////////////////////////////////////////////////////////////

BOOL CADPgACal::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

////////////////////////////////////////////////////////////////////////
// Check validity of parameters specified by controls
// Produce error messages if necessary.
////////////////////////////////////////////////////////////////////////

long CADPgACal::ChkAssayDef(CAssayDef * pAssayDef)
{
	long rv = KO_OK;

	ASSERT(GetSafeHwnd() != NULL);
	VERIFY(UpdateData(TRUE));

	// Check Calculation Method
	if(m_calcMethCB.GetCurSel() == -1)
	{
		// No calculation method selected
		AfxMessageBox(IDS_CALC_METH_INVALID);

		return KO_LUC_BAD_ACALC_METH;
	}

	return rv;
}

////////////////////////////////////////////////////////////////////////
// Check validity of control values
////////////////////////////////////////////////////////////////////////

BOOL CADPgACal::OnKillActive() 
{
	if (ChkAssayDef() != KO_OK)
	{
		return false;
	}

	return CPropertyPage::OnKillActive();
}

////////////////////////////////////////////////////////////////////////
// Copy contents of controls back to assay def
// Called regardless of whether page is (or was ever) visible
// It's too late to check validity here
////////////////////////////////////////////////////////////////////////


void CADPgACal::OnOK() 
{
	// ACalc method saved by OnSelchangeAdCalcMeth();
	
	CPropertyPage::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// Initialize Assay Calculation Method Selection Box
/////////////////////////////////////////////////////////////////////////////

BOOL CADPgACal::InitACalcMethodBox(long selectID)
{
	int numMethods = m_pACalc->GetNumMethods();

	if(numMethods == 0)
	{
		// ACC contains no methods
		CString msg;
		msg.LoadString(IDS_NO_ACMETH_DEF);
		m_calcMethCB.AddString(msg);
		m_calcMethCB.SetCurSel(0);
		m_calcMethCB.EnableWindow(FALSE);
	}
	else
	{
		// Add entry for each calc method returned
		// ... and select the specified entry if found
		for (int iMeth = 0; iMeth < numMethods; iMeth++)
		{		
			long methID = m_pACalc->GetMethodID(iMeth);

#ifndef _EGG_OEM_1
			//Sepsis method should only be added in OEM product
			if(methID == 410)
			{					//This is defined in ACalcDef.h, which should not
				continue;		//be included here in the EXE. This number  
			}					//is probably more stable than the string name
#endif
			// Add method name
			int iBoxSel = m_calcMethCB.AddString(m_pACalc->GetMethodName(methID));

			// Set method id
			VERIFY(PUtil::SetLongLBData(m_calcMethCB, methID, iBoxSel));

			// Select entry if appropriate
			// Parameter listbox will also be initialized
			if (methID == selectID)
			{
				m_calcMethCB.SetCurSel(iBoxSel);
			}
		}

		if (m_calcMethCB.GetCurSel() == LB_ERR && selectID != LUC_ID_UNDEF)
		{	// Calc method requested but not found
			AfxMessageBox(IDS_ACMETH_NOT_SUPPORTED);

			// Clear method ID
			m_pAD->SetCalcMeth(LUC_ID_UNDEF);

			// Clear selection
			m_calcMethCB.SetCurSel(-1);
		}

		// Update dependent controls
		OnSelchangeAdCalcMeth();
		// If selected calc method ID is same as 
		// assay def calc method ID, assay def param values
		// will be retained.
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Changing calc method always deselects any calc params
////////////////////////////////////////////////////////////////////////

void CADPgACal::OnSelchangeAdCalcMeth() 
{
	// Empty parameter list box
	m_calcParamLB.ResetContent();
	m_calcParamLB.SetCurSel(-1);

	// Update dependent controls
	OnSelchangeCalcParam();

	// Check if a Calculation method is selected
	if (m_calcMethCB.GetCurSel() == -1)
	{
		// No method selected
		m_pACalc->DeselectMethod();
		m_calcParamLB.EnableWindow(false);
	}
	else
	{
		// Get Calc Method ID 
		long methodID = PUtil::GetLongLBData(m_calcMethCB);
		if (methodID != LUC_ID_UNDEF)
		{	// Initialize assay definition only if changed
			// since initializing clears parameter values
			if (m_pAD->GetCalcMethID() != methodID)
			{	// Set new calc method
				m_pAD->SetCalcMeth(methodID);
				// calc parameters are initialized from data

				// queried from the ACC
				VERIFY(m_pAD->InitParamList(m_pACalc)); //!*!
			}

			// Update parameter List Box 
			//VERIFY(m_pAD->InitParamList(m_pACalc)); //why was this here?dbm
			int numParams = m_pAD->GetNumParams();
			ASSERT(numParams >= 0);
			for (int iParam = 0; iParam < numParams; iParam++)
			{	// Add param name
				int iBoxSel = m_calcParamLB.AddString(m_pAD->GetParam(iParam).m_name);
				// ... and param id
				VERIFY(PUtil::SetLongLBData(m_calcParamLB, 
					m_pAD->GetParam(iParam).m_ID, iBoxSel));
			}
			m_calcParamLB.EnableWindow(true);
		}
		else
		{	// Bad calc method ID
			AfxMessageBox(IDS_SELECT_CALC_METH);
		}
	}

	// Re-initialize any pages which depend on ACalc method
	ASSERT(((CADPropSheet*)GetParent())->IsKindOf(RUNTIME_CLASS(CADPropSheet)));
		
	// Update calibrator page
	((CADPropSheet*)GetParent())->m_pgCalib.OnSelACalcMeth();

	// Update sample name page
	((CADPropSheet*)GetParent())->m_pgSampNames.OnSelACalcMeth();
}


////////////////////////////////////////////////////////////////////////
// Changing calc param selection changes param value, max, min
////////////////////////////////////////////////////////////////////////

void CADPgACal::OnSelchangeCalcParam() 
{
	if (m_calcParamLB.GetCurSel() == -1)
	{	// No parameter selected
		// Empty parameter value box
		m_pParamValCB->ResetContent();
		m_pParamValCB->EnableWindow(false);

		// Clear Max, Min, default
		m_defParamValTxt = _T("");
		m_maxParamValTxt = _T("");
		m_minParamValTxt = _T("");
	}
	else
	{	// Get selected param ID
		long paramID = PUtil::GetLongLBData(m_calcParamLB);
		
		// Get Extended Param from Assay Def 
		// (includes max, min and default data)
		CParamEx* pParam = m_pAD->GetParamExPtrFromID(paramID);
		ASSERT(pParam != NULL);

		// Set member paramX
		m_curParamX = *pParam;

		// Re-init and enable parameter value control
		VERIFY(m_pParamValCB->InitParamExData(m_curParamX) == KO_OK);
		m_pParamValCB->EnableWindow(true);

		// Set Max, Min and Default values from extended parameter
		COleVariant textVar;
		HRESULT result;
		LCID locale = ::GetUserDefaultLCID();

		m_defParamValTxt = _T("");
		m_maxParamValTxt = _T("");
		m_minParamValTxt = _T("");

		// Generic data type to text translation using OLE facilites
		result = ::VariantChangeTypeEx(&textVar, &m_curParamX.m_default,locale, 0, VT_BSTR);
		if (SUCCEEDED(result))
		{
			m_defParamValTxt = textVar.bstrVal;
		}
		result = ::VariantChangeTypeEx(&textVar, &m_curParamX.m_max, locale, 0, VT_BSTR);
		if (SUCCEEDED(result))
		{
			m_maxParamValTxt = textVar.bstrVal;
		}
		result = ::VariantChangeTypeEx(&textVar, &m_curParamX.m_min, locale, 0, VT_BSTR);
		if (SUCCEEDED(result))
		{
			m_minParamValTxt = textVar.bstrVal;
		}

		// Parameter value box set active
		m_pParamValCB->SetFocus();
	}

	// Update dialog data
	UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// Assay Calculation Method Parameter value changed
/////////////////////////////////////////////////////////////////////////////

void CADPgACal::OnKillfocusAdCparmVal() 
{
	if (m_calcParamLB.GetCurSel() == -1)
	{	// No param selected
		return;
	}

	// Get Parameter ID 
	long paramID = PUtil::GetLongLBData(m_calcParamLB);

	// Get new value from Variant edit box
	CParamEx newParam;
	long rv = m_pParamValCB->GetParamEx(newParam);

	// Check if parameter value was acceptable
	if (rv == KO_OK)
	{
		if (m_pACalc->SetParamValue(newParam) == KO_OK)
		{	// Save setting in assay definition 
			CParamEx* pParam = m_pAD->GetParamExPtrFromID(paramID);
			ASSERT(pParam != NULL);
			*pParam = newParam;
		}
		else
		{	// ACC rejected value
			AfxMessageBox(IDS_BAD_PAR_VAL);

			// Re-init control
			m_pParamValCB->InitParamData(&m_curParamX);
			m_pParamValCB->SetFocus();
		}
	}
	else
	{	// Parameter Box rejected value
		if (rv == KO_PHUT_ABOVE_MAX)
		{
			AfxMessageBox(IDS_PARAM_TOO_BIG);
		}
		else if (rv == KO_PHUT_BELOW_MIN)
		{
			AfxMessageBox(IDS_PARAM_TOO_SMALL);	
		}
		else if (rv == KO_PHUT_INVALID_ENTRY)
		{
			AfxMessageBox(IDS_BAD_PAR_VAL);
		}
		else
		{
			ASSERT(false);
		}

		// Re-init control
		m_pParamValCB->InitParamData(&m_curParamX);
		m_pParamValCB->SetFocus();
	}
}

void CADPgACal::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// Destroy window attached to parameter value box control
	if (m_pParamValCB != NULL && m_pParamValCB->GetSafeHwnd() != NULL)
	{
		VERIFY(m_pParamValCB->DestroyWindow());
	}
}
