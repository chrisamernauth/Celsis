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

// ColHideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HideDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColHideDlg dialog


CColHideDlg::CColHideDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColHideDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColHideDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pFieldProp = NULL;
}


void CColHideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColHideDlg)
	DDX_Control(pDX, IDC_HIDELIST, m_hideList);
	DDX_Control(pDX, IDC_SHOWLIST, m_showList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColHideDlg, CDialog)
	//{{AFX_MSG_MAP(CColHideDlg)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	ON_BN_CLICKED(IDC_SH_GET_DEFAULT, OnShGetDefault)
	ON_BN_CLICKED(IDC_SH_SET_DEFAULT, OnShSetDefault)
	ON_BN_CLICKED(IDC_SH_RESET, OnShReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColHideDlg message handlers

CFieldProperties* CColHideDlg::RegFieldProp(CFieldProperties* pFieldProp)
{
	// ! It is the callers responsibility to delete this !
	m_pFieldProp = new CFieldProperties(*pFieldProp);

	return m_pFieldProp;
}

BOOL CColHideDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pFieldProp != NULL);

	VERIFY(InitLists());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColHideDlg::OnHide() 
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

	// Can't exit without at least one column showing
	ct = m_showList.GetCount();
	if(ct == 0)
	{
		GetDlgItem(IDOK)->EnableWindow(false);
		AfxMessageBox(_T("At least one column must be shown."));
	}
	else
	{	//this is handy for multiple hides:
		m_showList.SetCurSel(seldex <= ct - 1 ? seldex : ct - 1);
	}
}

void CColHideDlg::OnShow() 
{
	// Indices of selected items
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
	{	//!!! Hoping indices are in order, removing in reverse order
		//    should not shuffle remaining
		VERIFY(m_hideList.DeleteString(selected[i]) != LB_ERR);
	}

	delete selected;

	// Enable OK button if at least one column is showing
	if (m_showList.GetCount() > 0)
	{
		GetDlgItem(IDOK)->EnableWindow(true);
	}
}

void CColHideDlg::OnOK() 
{
	UpdateProperties();
	CDialog::OnOK();
}

void CColHideDlg::OnShGetDefault() 
{
	ASSERT(m_pFieldProp != NULL);
	VERIFY(m_pFieldProp->GetDefaults());
	VERIFY(InitLists());
}

void CColHideDlg::OnShSetDefault() 
{
	ASSERT(m_pFieldProp != NULL);
	UpdateProperties();
	VERIFY(m_pFieldProp->SetDefaults());
}

/////////////////////////////////////////////////////////////
// void CColHideDlg::OnShReset() 
// This is necessary because if defaults are set
// from a saved  Pharma security workload, the SampleID
// field comes up read only and FldProp.std would
// otherwise have to be deleted manually
void CColHideDlg::OnShReset() 
{
	ASSERT(m_pFieldProp != NULL);
	if(m_pFieldProp)
	{
		delete m_pFieldProp;
	}
	VERIFY(m_pFieldProp = new CFieldProperties());
	m_pFieldProp->StaticDefaultsRead();
	m_pFieldProp->GetDefaults();
	VERIFY(InitLists());	
}

bool CColHideDlg::InitLists()
{
	// First fill hide box
	m_hideList.ResetContent();
	int num = m_pFieldProp->GetFieldCount();  
	for (int iField = 0; iField < num; iField++)
	{
		if (m_pFieldProp->IsIndexHidden(iField))
		{
			// Add field name to box
			int iListBox = m_hideList.AddString(m_pFieldProp->GetIndexName(iField));
			// Save field ID with item
			m_hideList.SetItemData(iListBox,m_pFieldProp->GetIndexID(iField));
		}
	}
	
	// Now fill show box in existing column order
	m_showList.ResetContent();
	for (int iCol = 0; iCol < m_pFieldProp->GetColumnCount(); iCol++)
	{	// Add field name to box
		int iListBox = 	m_showList.AddString(m_pFieldProp->GetColName(iCol));
		// Save field ID with item
		m_showList.SetItemData(iListBox, m_pFieldProp->GetColID(iCol));
	}

	return true;
}

bool CColHideDlg::UpdateProperties()
{
	// Hide all fields
	m_pFieldProp->HideAllFields();

	int nShow = m_showList.GetCount();
	for (int i = 0; i < nShow; i++)
	{	// Add fields to show, in order, to field property object
		VERIFY(m_pFieldProp->ShowIDNext(m_showList.GetItemData(i)));
	}

	return true;
}

