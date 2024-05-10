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

// AssayNew.cpp : implementation file
//

#include "stdafx.h"
#include "AssayNew.h"
#include "Lucept.h"
#include "AssyDef.h"
#include "ADPrpSh.h"
#include "UsrAdmin.h"
#include "Event.h"
#include "SystemLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssayNew dialog

IMPLEMENT_DYNAMIC(CAssayNew, CDialog);

CAssayNew::CAssayNew(CWnd* pParent /*=NULL*/)
	: CDialog(CAssayNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssayNew)
	m_newName = _T("");
	//}}AFX_DATA_INIT
}


void CAssayNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssayNew)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_AD_COPY_FROM, m_oldAssayBox);
	DDX_Control(pDX, IDC_AD_NEW_FROM_OLD, m_fromOldChkBox);
	DDX_Text(pDX, IDC_AD_NEW_NAME, m_newName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssayNew, CDialog)
	//{{AFX_MSG_MAP(CAssayNew)
	ON_BN_CLICKED(IDC_AD_NEW_FROM_OLD, OnAdNewFromOld)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssayNew message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CAssayNew::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	// Init old assay box
	m_oldAssayBox.Init(&pApp->m_assays);
	// Disable the box.  It will be enabled by check box
	m_oldAssayBox.EnableWindow(FALSE);

	// Check for existing assays
	if (pApp->m_assays.GetCount() == 0)
	{
		// No assays defined, so disable checkbox
		m_fromOldChkBox.EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// Handle copy from existing assay check box
/////////////////////////////////////////////////////////////////////////////

void CAssayNew::OnAdNewFromOld() 
{	// If no assays are defined the checkbox is disabled so this
	// function will never be called; so, we needn't worry about
	// enabling the combo box when no assays are defined

	if	(m_fromOldChkBox.GetCheck() == TRUE)
	{
		m_oldAssayBox.EnableWindow(TRUE);
	}
	else
	{
		m_oldAssayBox.EnableWindow(FALSE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// OK 
/////////////////////////////////////////////////////////////////////////////

void CAssayNew::OnOK() 
{
	CLuceptApp* app = (CLuceptApp*)AfxGetApp();
	CAssays* pAssays = &app->m_assays;

	// Read requested name from edit box
	UpdateData(TRUE);

	// Strip white space
	m_newName.TrimLeft();
	m_newName.TrimRight();

	// Check for empty name string
	if (m_newName.IsEmpty())
	{
		CString errMsg;

		errMsg.LoadString(IDS_AD_NEW_NONAME);
		AfxMessageBox(errMsg);
		return;
	}

	// Check if name is already in use
	if (pAssays->GetIDFromName(m_newName) != LUC_ID_UNDEF)
	{	// Name exists.  
		CString errMsg;

		errMsg.LoadString(IDS_AD_NEW_NAME_USED);
		AfxMessageBox(errMsg);
		return;
	}

	// Create new assay definition object and initialize
	CAssayDef newAssayDef;
	
	// Init from old if necessary
	if	(m_fromOldChkBox.GetCheck() == TRUE && 
		m_oldAssayBox.GetCurSel() != -1)
	{	// Get ID of existing assay definition
		long oldID = PUtil::GetLongLBData(m_oldAssayBox);

		CAssayDef* pOldDef = pAssays->GetDefPtr(oldID);
		ASSERT(pOldDef != NULL);

		newAssayDef = *pOldDef;
	}

	// Set name 
	newAssayDef.SetName(m_newName);

	// ITL: Ensure lists are updated from disk
	pAssays->InitFromDisk();

	// Set ID to next available
	// ITL: Check obsolete is not greater added by CAR 887/002
	newAssayDef.SetID(max(pAssays->GetFreeID(),pAssays->GetFreeAHID()));

	// Set creator and create date
	newAssayDef.SetCreator(gGateKeeper.CurrentUserNameGet());
	newAssayDef.SetCreateDate(COleDateTime::GetCurrentTime());

	// Set modifier and modification date (same as cretor on new)
	newAssayDef.SetModifier(gGateKeeper.CurrentUserNameGet());
	newAssayDef.SetModDate(COleDateTime::GetCurrentTime());

	// Open assay editing screen
	CADPropSheet ADSheet(IDS_ASSAY_DEF_TITLE); 
	
	// Pass pointer to assay definition object before starting
	ADSheet.InitAssayDef(&newAssayDef);
	ADSheet.DoModal();

	// ADSheet is responsible for saving 
	// new assay definition if appropriate and modifying 
	// assay list.

	// *** SYSTEM LOG **********************************************************
	CLogEvent	NewEvent;							       // Create a new event   *
	NewEvent.EventSet(_T("Assay Definition created      "),// Event name           *
						m_newName);					   // Event details        *
	CSystemLog::g_oSystemLog.EventAdd( NewEvent);
	// *** SYSTEM LOG **********************************************************

	// Close dialog when done
	CDialog::OnOK();
}
