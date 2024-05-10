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

// ADPgName.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "ADPgName.h"
#include "ADPrpSh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADPgName property page

IMPLEMENT_DYNCREATE(CADPgName, CPropertyPage)

CADPgName::CADPgName() : CPropertyPage(CADPgName::IDD)
{
	//{{AFX_DATA_INIT(CADPgName)
	//}}AFX_DATA_INIT

	m_pAD		= NULL;
	m_pACalc	= NULL;
}

CADPgName::~CADPgName()
{
}

void CADPgName::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADPgName)
	DDX_Control(pDX, IDC_AD_TYPE_SEL, m_sampGenName);
	DDX_Control(pDX, IDC_AD_TYPE_NAME, m_sampTypeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADPgName, CPropertyPage)
	//{{AFX_MSG_MAP(CADPgName)
	ON_CBN_SELCHANGE(IDC_AD_TYPE_SEL, OnSelchangeSampType)
	ON_EN_KILLFOCUS(IDC_AD_TYPE_NAME, OnKillfocusAdTypeName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADPgName message handlers

////////////////////////////////////////////////////////////////////////
// Save assay def pointer -- called from property sheet's contructor
////////////////////////////////////////////////////////////////////////

long CADPgName::SetAssayDef(CAssayDef * pAssayDef)
{
	ASSERT(pAssayDef);
	m_pAD = pAssayDef;

	return KO_OK;
}

////////////////////////////////////////////////////////////////////////
// Create and initialize controls -- 
// called after property sheet's DoModal (m_pAD is valid)
////////////////////////////////////////////////////////////////////////

BOOL CADPgName::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	/*this is necesary to initialize sample types
	if the Sample Type tab is first selected without
	going through the calculation method tab*/

	if(m_pAD->GetNumSampTypes() < 1)
	{
		// Get pointer to assay calc control
		CADPropSheet* pSheet = (CADPropSheet*)GetParent();
		ASSERT(pSheet != NULL);
		ASSERT(pSheet->IsKindOf(RUNTIME_CLASS(CADPropSheet)));
		m_pACalc = pSheet->m_pACalc;
		ASSERT(m_pACalc);

		//initialize paramlist
		VERIFY(m_pAD->InitParamList(m_pACalc));
	}

	return TRUE;  
}

////////////////////////////////////////////////////////////////////////
// Do anything?
////////////////////////////////////////////////////////////////////////

BOOL CADPgName::OnSetActive() 
{
	OnSelACalcMeth();

	return CPropertyPage::OnSetActive();
}


void CADPgName::OnSelACalcMeth()
{
	if (GetSafeHwnd() != NULL)
	{	// Initialize Sample type droplist
		m_sampGenName.ResetContent();
		// Don't sort the list!
		ASSERT((m_sampGenName.GetStyle() & CBS_SORT) != CBS_SORT);

		int numSampTypes = m_pAD->GetNumSampTypes();
		ASSERT(numSampTypes);
		for (int iType = 0; iType < numSampTypes; iType++)
		{	// Add field name to box
			int iListBox = m_sampGenName.AddString(m_pAD->IndexGetSampDefName(iType));
		}

		m_sampGenName.EnableWindow(numSampTypes > 0);
	}

	// Clear edit box
	OnSelchangeSampType();

	if (GetSafeHwnd() != NULL)
	{
		UpdateData(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////
// Check validity of parameters specified by controls
// Produce error messages if necessary.
////////////////////////////////////////////////////////////////////////

long CADPgName::ChkAssayDef(CAssayDef * pAssayDef)
{
	long rv = KO_OK;

	return rv;
}

////////////////////////////////////////////////////////////////////////
// Check validity of control values
////////////////////////////////////////////////////////////////////////

BOOL CADPgName::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

////////////////////////////////////////////////////////////////////////
// Copy contents of controls back to assay def
// Called regardless of whether page is (or was ever) visible
// It's too late to check validity here
////////////////////////////////////////////////////////////////////////


void CADPgName::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}

void CADPgName::OnSelchangeSampType()
{
	// Init edit box
	int iType = -1;
	
	if (GetSafeHwnd() != NULL)
	{
		iType = m_sampGenName.GetCurSel();

		if (iType != -1)
		{
			ASSERT(m_pAD != NULL);
			m_sampTypeName.SetWindowText(m_pAD->IndexGetSampName(iType));
			m_sampTypeName.EnableWindow(true);
			// Clear edit box modified flag
			m_sampTypeName.SetModify(false);
		}
		else
		{
			m_sampTypeName.SetWindowText(_T(""));
			m_sampTypeName.EnableWindow(false);
			// Clear edit box modified flag
			m_sampTypeName.SetModify(false);
		}

		UpdateData(FALSE);
	}
}

void CADPgName::OnKillfocusAdTypeName() 
{
	// Loss of focus indicates possible change of content
	// requiring update of assay def

	// check if a sample type is selected
	// check if edit box has changed
	int iType = m_sampGenName.GetCurSel();
	if (iType != -1 && m_sampTypeName.GetModify())
	{
		ASSERT(m_pAD != NULL);
		// Contents have changed, update assay def
		CString newName;
		m_sampTypeName.GetWindowText(newName);
		m_pAD->IndexSetSampName(iType, newName);
	}
}
