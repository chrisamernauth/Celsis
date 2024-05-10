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
ITL				19-Mar-02			Modified by CAR 887/002

************************************************************************************/

// AssaySel.cpp : implementation file
//

#include "stdafx.h"
#include "AssaySel.h"
#include "Lucept.h"
#include "AssyDef.h"
#include "ADPrpSh.h"
#include "AssyPrDc.h"
#include "AssyPrVw.h"
#include "Event.h"
#include "SystemLog.h"
#include "UsrAdmin.h"
#include "GenDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssaySel dialog


CAssaySel::CAssaySel(SEL_PURPOSE Purpose, CWnd* pParent)
	: CDialog(CAssaySel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssaySel)
	//}}AFX_DATA_INIT

	m_Purpose = Purpose;
	m_pAssayDef = NULL;
}


void CAssaySel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssaySel)
	DDX_Control(pDX, IDC_ASSAY_SEL_CB, m_assaySelCB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssaySel, CDialog)
	//{{AFX_MSG_MAP(CAssaySel)
	ON_CBN_DBLCLK(IDC_ASSAY_SEL_CB, OnDblclkAssaySelCb)
	ON_CBN_SELCHANGE(IDC_ASSAY_SEL_CB, OnSelchangeAssaySelCb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssaySel message handlers

BOOL CAssaySel::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set proper title
	CString title;
	if(m_Purpose == FOR_EDIT)
	{	// Select Assay to Edit
		title.LoadString(IDS_SEL_ASSAY_EDIT);	
	}
	else if(m_Purpose == FOR_DELETE)
	{	// Select Assay to Delete
		title.LoadString(IDS_SEL_ASSAY_DEL);
	}
	else if(m_Purpose == FOR_PRINT)
	{	// Select Assay to Print
		title.LoadString(IDS_SEL_ASSAY_PRINT);	
	}
	else if(m_Purpose == FOR_HISTORY)
	{	// ITL: Select Assay History to Print Added by CAR 887/002
		title.LoadString(IDS_SEL_ASSAY_PRINT);	
	}
	else
	{	// Nothing chosen
		ASSERT(0);
		title = _T("error");
	}
	SetWindowText(title);

	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	if(m_Purpose != FOR_HISTORY)	// ITL: Added by CAR 887/002
	{	// ITL: Initialize Selection Combobox for current assays
		m_assaySelCB.Init(&pApp->m_assays);
	}
	else
	{	// ITL: Initialize Selection Combobox for obsolete assays
		m_assaySelCB.InitAH(&pApp->m_assaysHistory);
	}

	// Handle OK button
	if (m_Purpose == FOR_DELETE)
	{	// For delete only, change button text
		title.LoadString(IDS_DELETE);
		GetDlgItem(IDOK)->SetWindowText(title);
	}

	// For both, disable (until an assay is selected)
	GetDlgItem(IDOK)->EnableWindow(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAssaySel::OnOK() 
{
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	if(m_Purpose == FOR_DELETE)
	{	// Deleting						
		CString name;
		m_assaySelCB.GetWindowText(name);
		VERIFY(pApp->m_assays.Obsolete(name));
		// *** SYSTEM LOG **********************************************************
		CLogEvent	NewEvent;							       // Create a new event   *
		NewEvent.EventSet(_T("Assay Definition deleted      "),// Event name           *
							name);						   // Event details        *
		CSystemLog::g_oSystemLog.EventAdd( NewEvent);
		// *** SYSTEM LOG **********************************************************
	}
	else if(m_Purpose == FOR_EDIT)
	{	// Editing
		
		// Create new assay definition object and initialize
		CAssayDef newAssayDef;
		
		// Init from old
		// Get ID of existing assay definition
		long oldID = PUtil::GetLongLBData(m_assaySelCB);
		CAssayDef* pOldDef = pApp->m_assays.GetDefPtr(oldID);
		ASSERT(pOldDef != NULL);
		newAssayDef = *pOldDef;

		// ITL: Ensure lists are updated from disk
		pApp->m_assays.InitFromDisk();

		// Set ID to next available
		// ITL: Check obsolete is not greater added by CAR 887/002
		newAssayDef.SetID(max(pApp->m_assays.GetFreeID(),pApp->m_assays.GetFreeAHID()));

		// Open assay editing screen
		CADPropSheet	ADSheet(IDS_ASSAY_DEF_TITLE); 
		
		// Pass pointer to assay definition object before starting
		ADSheet.InitAssayDef(&newAssayDef);
		ADSheet.DoModal();

		// ADSheet is responsible for saving 
		// new assay definition if appropriate and modifying 
		// assay list.
	}
	else if(m_Purpose == FOR_PRINT)
	{	// Printing current assay
		// Get pointer to selected assay definition
		long oldID = PUtil::GetLongLBData(m_assaySelCB);
		m_pAssayDef = (CAssayDef*)((CLuceptApp*)AfxGetApp())->m_assays.GetDefPtr(oldID);
		ASSERT(m_pAssayDef != NULL);
	}
	else if(m_Purpose == FOR_HISTORY)
	{	// Printing obsolete assay
		// Get pointer to selected assay definition
		long oldID = PUtil::GetLongLBData(m_assaySelCB);
		m_pAssayDef = (CAssayDef*)((CLuceptApp*)AfxGetApp())->m_assaysHistory.GetDefPtrAH(oldID);
		ASSERT(m_pAssayDef != NULL);
	}
	else
	{
		ASSERT(0);
	}

	CDialog::OnOK();
}

void CAssaySel::OnSelchangeAssaySelCb() 
{
	if (m_assaySelCB.GetCurSel() != -1)
	{
		// Something selected ...
		GetDlgItem(IDOK)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(false);
	}
}

void CAssaySel::OnDblclkAssaySelCb() 
{
	// Initiate editing if appropriate
	if (m_Purpose != FOR_DELETE)
	{
		OnOK();
	}
}

