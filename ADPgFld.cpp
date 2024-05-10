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

// ADPgFld.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "ADPgFld.h"
#include "ADPrpSh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADPgFld property page

IMPLEMENT_DYNCREATE(CADPgFld, CPropertyPage)

CADPgFld::CADPgFld() : CPropertyPage(CADPgFld::IDD)
{
	//{{AFX_DATA_INIT(CADPgFld)
	//}}AFX_DATA_INIT

	m_pFieldProp = NULL;
}

CADPgFld::~CADPgFld()
{
}

void CADPgFld::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADPgFld)
	DDX_Control(pDX, IDC_HIDELIST, m_hideList);
	DDX_Control(pDX, IDC_SHOWLIST, m_showList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADPgFld, CPropertyPage)
	//{{AFX_MSG_MAP(CADPgFld)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADPgFld message handlers

////////////////////////////////////////////////////////////////////////
// Save assay def pointer -- called from property sheet's contructor
////////////////////////////////////////////////////////////////////////

long CADPgFld::SetAssayDef(CAssayDef * pAssayDef)
{
	m_pAD = pAssayDef;
	ASSERT(m_pAD != NULL);

	m_pFieldProp = m_pAD->GetFldPropPtr();

	return KO_OK;
}

////////////////////////////////////////////////////////////////////////
// Create and initialize controls -- 
// called after property sheet's DoModal (m_pAD is valid)
////////////////////////////////////////////////////////////////////////

BOOL CADPgFld::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
		
	// Initialize list box contents here
	InitLists(m_pFieldProp);

	// Update button enable?

	return TRUE;  
}

////////////////////////////////////////////////////////////////////////
// Do anything?
////////////////////////////////////////////////////////////////////////

BOOL CADPgFld::OnSetActive() 
{
	// Update controls with data
	UpdateData(FALSE);

	return CPropertyPage::OnSetActive();
}


////////////////////////////////////////////////////////////////////////
// Check validity of parameters specified by controls
// Produce error messages if necessary.
////////////////////////////////////////////////////////////////////////

long CADPgFld::ChkAssayDef(CAssayDef * pAssayDef)
{
	long rv = KO_OK;

	// Require one column showing.
	if (m_showList.GetCount() < 1) 
	{
		AfxMessageBox(IDS_NO_VISIBLE_FIELDS);
		rv = KO_NO_VISIBLE_FIELDS;
	}

	return rv;
}

////////////////////////////////////////////////////////////////////////
// Check validity of control values
////////////////////////////////////////////////////////////////////////

BOOL CADPgFld::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

////////////////////////////////////////////////////////////////////////
// Copy contents of controls back to assay def
// Called regardless of whether page is (or was ever) visible
// It's too late to check validity here
////////////////////////////////////////////////////////////////////////


void CADPgFld::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateProperties();
	
	CPropertyPage::OnOK();
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CADPgFld::InitLists(CFieldProperties *pFldProp) 
{
	ASSERT(pFldProp != NULL);

	// First fill hide box
	m_hideList.ResetContent();
	int num = pFldProp->GetFieldCount();  
	for (int iField = 0; iField < num; iField++)
	{
		if (pFldProp->IsIndexHidden(iField))
		{
			// Add field name to box
			int iListBox = m_hideList.AddString(pFldProp->GetIndexName(iField));
			// Save field ID with item
			m_hideList.SetItemData(iListBox, pFldProp->GetIndexID(iField));
		}
	}
	
	// Now fill show box in existing column order
	m_showList.ResetContent();
	for (int iCol = 0; iCol < pFldProp->GetColumnCount(); iCol++)
	{
		// Add field name to box
		int iListBox = m_showList.AddString(pFldProp->GetColName(iCol));
		// Save field ID with item
		m_showList.SetItemData(iListBox, pFldProp->GetColID(iCol));
	}

	return;
}

void CADPgFld::OnHide() 
{
	int seldex	= LB_ERR;
	int ct		= 0;
	if((seldex = m_showList.GetCurSel()) != LB_ERR)
	{
		CString cs;
		m_showList.GetText(seldex, cs);		//get selected string					
		m_hideList.SetItemData(				//add string and data to hide box
			m_hideList.AddString(cs), 
			m_showList.GetItemData(seldex));
		m_showList.DeleteString(seldex);	//delete string from show box
	}
	
	// Can't exit dialog without at least one column showing
	ct = m_showList.GetCount();
	if (ct == 0)
	{
		AfxMessageBox(_T("At least one column must be shown."));
	}
	else
	{	//this is handy for multiple hides:
		m_showList.SetCurSel(seldex <= ct - 1 ? seldex : ct - 1);
	}
}

void CADPgFld::OnShow() 
{	// Indices of selected items
	int* selected = new int[m_pFieldProp->GetFieldCount()];

	// Get the selections
	int nToShow = 
		m_hideList.GetSelItems(m_pFieldProp->GetFieldCount(), selected);
	
	// Copy to show box
	int i;

	for ( i = 0; i < nToShow; i++)
	{
		int iSel = selected[i];
		CString text;

		// Add item to show list
		m_hideList.GetText(iSel, text);
		int iShowList =	m_showList.AddString(text);
		m_showList.SetItemData(iShowList, m_hideList.GetItemData(iSel));
	}

	// Delete from show box
	for (i = nToShow; --i >= 0; )
	{
		// !!! Hoping indices are in order, removing in reverse order
		// should not shuffle remaining
		VERIFY(m_hideList.DeleteString(selected[i]) != LB_ERR);
	}

	delete selected;
}

void CADPgFld::UpdateProperties()
{
	// Hide all fields
	m_pFieldProp->HideAllFields();

	int nShow = m_showList.GetCount();
	for (int i = 0; i < nShow; i++)
	{	// Add fields to show, in order, to field property object
		VERIFY(m_pFieldProp->ShowIDNext(m_showList.GetItemData(i)));
	}

	return;
}
