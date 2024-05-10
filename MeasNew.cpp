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

//	MeasNew.cpp : implementation file
//
// 

#include "stdafx.h"
#include "MeasNew.h"
#include "Lucept.h"
#include "MeasDef.h"
#include "MDefDlg.h"
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


CMeasNew::CMeasNew(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasNew)
	//}}AFX_DATA_INIT
}


void CMeasNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasNew)
	DDX_Text(pDX, IDC_MD_NEW_NAME, m_newName);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_MD_COPY_FROM, m_oldMeasBox);
	DDX_Control(pDX, IDC_MD_NEW_FROM_OLD, m_fromOldChkBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasNew, CDialog)
	//{{AFX_MSG_MAP(CMeasNew)
	ON_BN_CLICKED(IDC_MD_NEW_FROM_OLD, OnMdNewFromOld)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssayNew message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CMeasNew::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	// Init old meas box
	m_oldMeasBox.Init(&pApp->m_meass);

	// Disable the box.  It will be enabled by check box
	m_oldMeasBox.EnableWindow(FALSE);

	// Check for existing measurement sets
	if (pApp->m_meass.GetCount() == 0)
	{	// None, so disable checkbox
		m_fromOldChkBox.EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// Handle copy from existing assay check box
/////////////////////////////////////////////////////////////////////////////

void CMeasNew::OnMdNewFromOld() 
{
	// If no assays are defined the checkbox is disabled so this
	// function will never be called; so, we needn't worry about
	// enabling the combo box when no assays are defined

	if	(m_fromOldChkBox.GetCheck() == TRUE)
	{
		m_oldMeasBox.EnableWindow(TRUE);
	}
	else
	{
		m_oldMeasBox.EnableWindow(FALSE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// OK 
/////////////////////////////////////////////////////////////////////////////

void CMeasNew::OnOK() 
{
	CLuceptApp* app = (CLuceptApp*)AfxGetApp();
	CMeasures* pMeass = &app->m_meass;

	// Read requested name from edit box
	UpdateData(TRUE);
	
	// Strip white space
	m_newName.TrimLeft();
	m_newName.TrimRight();

	// Check for empty name string
	if (m_newName.IsEmpty())
	{
		CString errMsg;
		errMsg.LoadString(IDS_MD_NEW_NONAME);
		AfxMessageBox(errMsg);
		return;
	}

	// Check if name is already in use
	if (pMeass->GetIDFromName(m_newName) != LUC_ID_UNDEF)
	{	// Name exists.  
		CString errMsg;
		errMsg.LoadString(IDS_MD_NEW_NAME_USED);
		AfxMessageBox(errMsg);
		return;
	}

	// Create new meas definition object and initialize
	CMeasDef newMeasDef;
	
	// Init from old if necessary
	if	(m_fromOldChkBox.GetCheck() == TRUE && m_oldMeasBox.GetCurSel() != -1)
	{	// Get ID of existing assay definition
		long oldID = PUtil::GetLongLBData(m_oldMeasBox);

		CMeasDef* pOldDef = pMeass->GetDefPtr(oldID);
		ASSERT(pOldDef != NULL);

		newMeasDef = *pOldDef;
	}

	// Set name 
	newMeasDef.SetName(m_newName);

	// ITL: Ensure lists are updated from disk
	pMeass->InitFromDisk();

	// Set ID to next available
	// ITL: Check obsolete is not greater added by CAR 887/002
	newMeasDef.SetID(max(pMeass->GetFreeID(),pMeass->GetFreeMHID()));

	// Set creator and create date
	newMeasDef.SetCreator(gGateKeeper.CurrentUserNameGet());
	newMeasDef.SetCreateDate(COleDateTime::GetCurrentTime());

	// Set modifier and mod date (same as creator on new object)
	newMeasDef.SetModifier(gGateKeeper.CurrentUserNameGet());
	newMeasDef.SetModDate(COleDateTime::GetCurrentTime());

	// Open assay editing screen
	CMeasDefDlg	MDDlg(IDD_MEAS_DEF);

	// Pass pointer to assay definition object before starting
	MDDlg.m_pMeasDef = &newMeasDef;
	MDDlg.DoModal();

	// Assay Definition Dialog is responsible for saving 
	// new assay definition if appropriate and modifying 
	// assay list.

	// *** SYSTEM LOG **********************************************************
	CLogEvent	NewEvent;							       // Create a new event   *
	NewEvent.EventSet(_T("Measurement Definition created"),// Event name           *
						m_newName);					   // Event details        *
	CSystemLog::g_oSystemLog.EventAdd( NewEvent);
	// *** SYSTEM LOG **********************************************************

	// Close dialog when done
	CDialog::OnOK();
}

