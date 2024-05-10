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

// ADPgCal.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "ADPgCal.h"
#include "ADPrpSh.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADPgCal dialog


CADPgCal::CADPgCal(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CADPgCal::IDD)
{
	//{{AFX_DATA_INIT(CADPgCal)
	m_calRefString = _T("");
	m_updateCalRef = FALSE;
	m_allowExtCals = FALSE;
	//}}AFX_DATA_INIT

	m_pAD = NULL;
}


void CADPgCal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADPgCal)
	DDX_Control(pDX, IDC_CLEAR_DEFAULT_CAL, m_clearRef);
	DDX_Control(pDX, IDC_AD_CAL_SEL, m_calType);
	DDX_Text(pDX, IDC_DEF_CAL_REF, m_calRefString);
	DDX_Check(pDX, IDC_UPDATE_DEFAULT_CAL, m_updateCalRef);
	DDX_Check(pDX, IDC_USE_DEFAULT_CAL, m_allowExtCals);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADPgCal, CPropertyPage)
	//{{AFX_MSG_MAP(CADPgCal)
	ON_BN_CLICKED(IDC_USE_DEFAULT_CAL, OnUseDefaultCal)
	ON_CBN_SELCHANGE(IDC_AD_CAL_SEL, OnSelchangeAdCalSel)
	ON_BN_CLICKED(IDC_CLEAR_DEFAULT_CAL, OnClearCalRef)
	ON_BN_CLICKED(IDC_UPDATE_DEFAULT_CAL, OnUpdateDefaultCal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADPgCal message handlers

long CADPgCal::SetAssayDef(CAssayDef * pAssayDef)
{
	m_pAD = pAssayDef;

	return KO_OK;
}

////////////////////////////////////////////////////////////////////////
// Create and initialize controls -- 
// called after property sheet's DoModal (m_pAD is valid)
////////////////////////////////////////////////////////////////////////

BOOL CADPgCal::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Don't sort the list!
	ASSERT((m_calType.GetStyle() & CBS_SORT) != CBS_SORT);

	UpdateData(FALSE);

	return TRUE;  
}

////////////////////////////////////////////////////////////////////////
// Update to current assay calc method
// Also called directly by ACalc page
////////////////////////////////////////////////////////////////////////

BOOL CADPgCal::OnSetActive() 
{
	OnSelACalcMeth();

	UpdateData(FALSE);

	return CPropertyPage::OnSetActive();
}

void CADPgCal::OnSelACalcMeth()
{
	// Dis/Enable allow default calibrators
	ASSERT(m_pAD != NULL);

	m_allowExtCals = m_pAD->DefCalsAllowed();
	if (GetSafeHwnd() != NULL)
	{
		UpdateData(FALSE);
	}
	// Update dependencies
	OnUseDefaultCal();
	
	// disable use default cals if no cal sample types
	if (GetSafeHwnd() != NULL)
	{
		GetDlgItem(IDC_USE_DEFAULT_CAL)->
			EnableWindow(m_pAD->GetNumCalSampTypes() > 0);
	}

	// Reset cal samp sel box
	if (GetSafeHwnd() != NULL)
	{
		int nTypes = m_pAD->GetNumCalSampTypes();
		m_calType.ResetContent();
		for (int iType = 0; iType < nTypes; iType++)
		{
			// Add cal samp type name to box
			long sampTypeID = m_pAD->IndexGetCalSampTypeID(iType);
			m_calType.AddString(m_pAD->IDGetSampDefName(sampTypeID));
		}
	}
	// Process dependencies
	OnSelchangeAdCalSel();
	
	if (GetSafeHwnd() != NULL)
	{
		UpdateData(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////
// Check validity of parameters specified by controls
// Produce error messages if necessary.
////////////////////////////////////////////////////////////////////////

long CADPgCal::ChkAssayDef(CAssayDef * pAssayDef)
{
	long rv = KO_OK;

	ASSERT(GetSafeHwnd() != NULL);
	VERIFY(UpdateData(TRUE));

	return rv;
}

////////////////////////////////////////////////////////////////////////
// Check validity of control values
////////////////////////////////////////////////////////////////////////

BOOL CADPgCal::OnKillActive() 
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

void CADPgCal::OnOK() 
{
	UpdateData(TRUE);

	// default cal references updated when entered
	
	CPropertyPage::OnOK();
}

void CADPgCal::OnUseDefaultCal() 
{
	if (GetSafeHwnd() != NULL)
	{
		UpdateData(TRUE);

		m_pAD->AllowDefCals(m_allowExtCals != FALSE);

		if (m_allowExtCals == FALSE)
		{	// Disable cal selection window and fill it
			m_calType.EnableWindow(false);
		}
		else
		{	// Enable cal selection window
			m_calType.EnableWindow(true);
		}
	}
}

void CADPgCal::OnSelchangeAdCalSel() 
{
	// Set default cal reference based on 
	// cal sample type selected
	int iCal = -1;
	if (GetSafeHwnd() != NULL)
	{
		iCal = m_calType.GetCurSel();
	}
	if (iCal != -1)
	{	// Check if the cal sample exists
 		if (!m_pAD->m_defCalRefList[iCal].RefExists())
		{	// Clear the reference
			OnClearCalRef();
		}
		else
		{	// Set the reference string
			m_calRefString = m_pAD->m_defCalRefList[iCal].GetRefString();
		}
		// !!! Do me m_pickCalRef.EnableWindow(true);
		if (!m_calRefString.IsEmpty())
		{
			m_clearRef.EnableWindow(true);
		}
		// set the update check and enable it
		m_updateCalRef = m_pAD->IndexGetCalUpdate(iCal);
		GetDlgItem(IDC_UPDATE_DEFAULT_CAL)->EnableWindow(true);
	}
	else
	{	// No cal sample type selected

		// Clear reference box
		m_calRefString = _T("");
		m_updateCalRef = FALSE;

		if (GetSafeHwnd() != NULL)
		{
			m_clearRef.EnableWindow(false);
			GetDlgItem(IDC_UPDATE_DEFAULT_CAL)->EnableWindow(false);
		}
	}
	if (GetSafeHwnd() != NULL)
	{
		UpdateData(FALSE);
	}
}

void CADPgCal::OnClearCalRef() 
{
	// Create empty ref
	CSampRef newCalRef;
	newCalRef.SetRefType(CSampRef::REF_DEFCAL);

	// Update assay def
	if (m_calType.GetCurSel() != -1)
	{
		m_pAD->m_defCalRefList[m_calType.GetCurSel()] = newCalRef;
	}

	m_calRefString.Empty();

	UpdateData(FALSE);
}

void CADPgCal::OnUpdateDefaultCal() 
{
	// Update assay def with new setting
	UpdateData(TRUE);
	m_pAD->IndexSetCalUpdate(m_calType.GetCurSel(), m_updateCalRef);
}


