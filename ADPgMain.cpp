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

// ADPgMain.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "MainFrm.h"
#include "ADPgMain.h"
#include "ADPrpSh.h"
#include "UsrAdmin.h"
#include "IniFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADPgMain dialog


CADPgMain::CADPgMain(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CADPgMain::IDD)
{
	//{{AFX_DATA_INIT(CADPgMain)
	m_creator		= _T("");
	m_createDateTxt = _T("");
	m_modDateTxt	= _T("");
	m_maxTubes		= gHiTubes; 
	m_defaultRepl	= 1;
	//}}AFX_DATA_INIT

	m_pAD = NULL;
}


void CADPgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADPgMain)
	DDX_Control(pDX, IDC_AD_SPIN_REPL,		m_defReplSpin);
	DDX_Control(pDX, IDC_AD_SPIN_MAXTUBE,	m_maxTubeSpin);
	DDX_Control(pDX, IDC_AD_MEAS_METH,		m_measMethCB);
	DDX_Control(pDX, IDC_AD_ASSAY_GROUP,	m_assayNameGB);
	DDX_Text(pDX, IDC_AD_CREATOR,			m_creator);
	DDX_Text(pDX, IDC_AD_CREATED_ON,		m_createDateTxt);
	DDX_Text(pDX, IDC_AD_MODIFIER,			m_modifier);
	DDX_Text(pDX, IDC_AD_MODIFIED_ON,		m_modDateTxt);
	DDX_Text(pDX, IDC_AD_EDIT_MAXTUBES,		m_maxTubes);
	DDV_MinMaxUInt(pDX, m_maxTubes, 1, gHiTubes);
	DDX_Text(pDX, IDC_AD_EDIT_REPL,			m_defaultRepl);
	DDV_MinMaxUInt(pDX, m_defaultRepl, 1, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADPgMain, CPropertyPage)
	//{{AFX_MSG_MAP(CADPgMain)
	ON_BN_CLICKED(IDC_AD_NEW_MEASMETH, OnAdNewMeasmeth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADPgMain message handlers

////////////////////////////////////////////////////////////////////////
// Save assay def pointer -- called from property sheet's contructor
////////////////////////////////////////////////////////////////////////

long CADPgMain::SetAssayDef(CAssayDef * pAssayDef)
{
	m_pAD = pAssayDef;

	return KO_OK;
}

////////////////////////////////////////////////////////////////////////
// Create and initialize controls -- 
// called after property sheet's DoModal (m_pAD is valid)
////////////////////////////////////////////////////////////////////////

BOOL CADPgMain::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Setup range for default replicates and max tubes
	m_defReplSpin.SetRange(1, MAX_REPLICATES);
	m_maxTubeSpin.SetRange(1, (short)gHiTubes);

	ASSERT(m_pAD != NULL);

	m_assayNameGB.SetWindowText(m_pAD->GetName());
	m_creator	= m_pAD->GetCreator();
	m_modifier	= m_pAD->GetModifier();

	// Ole Date format Bug fix!
	// This code prevents errors when Regional date settings are
	// not English
	{
		CSetLocale l(LC_TIME);
		m_createDateTxt = m_pAD->GetCreateDate().Format(_T("%x %X"));
		m_modDateTxt	= m_pAD->GetModDate().Format(_T("%x %X"));
	}

	// Get measuremnt ID from measurement name
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	long measID = pApp->m_meass.GetIDFromName(m_pAD->GetMeasName());
	if (measID == LUC_ID_UNDEF && !m_pAD->GetMeasName().IsEmpty())
	{	// A measurement name was specified but is invalid
		AfxMessageBox(IDS_MEAS_METH_INVALID);
		// Clear Assay Definition
		m_pAD->SetMeasName(_T(""));
	}

	// Initialize measurement method list and initial selection
	// in the measurement method drop box
	VERIFY(m_measMethCB.Init(&pApp->m_meass, measID));

	// Setup initial value for default replicates and max tubes
	m_defaultRepl = m_pAD->GetDefaultRepls();
	m_maxTubes = m_pAD->GetMaxTubes();

	// This call is particularly important for the static strings
	UpdateData(FALSE);

	return TRUE;  
}

////////////////////////////////////////////////////////////////////////
// Do anything?
////////////////////////////////////////////////////////////////////////

BOOL CADPgMain::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

////////////////////////////////////////////////////////////////////////
// Check validity of parameters specified by controls
// Produce error messages if necessary.
////////////////////////////////////////////////////////////////////////

long CADPgMain::ChkAssayDef(CAssayDef * pAssayDef)
{
	long rv = KO_OK;

	ASSERT(GetSafeHwnd() != NULL);

	// Check Measurement Method selection
	int iMeasSel = m_measMethCB.GetCurSel();
	if(iMeasSel == -1)
	{	// No measurement selected
		AfxMessageBox(IDS_MEAS_METH_INVALID);
		return KO_LUC_BAD_MEAS_DEF;
	}

	return rv;
}

////////////////////////////////////////////////////////////////////////
// Check validity of control values
////////////////////////////////////////////////////////////////////////

BOOL CADPgMain::OnKillActive() 
{
	if(!UpdateData(TRUE))
	{	//this function will show the error
		return FALSE;
	}

	if (ChkAssayDef() != KO_OK)
	{
		return FALSE;
	}

	return CPropertyPage::OnKillActive();
}

////////////////////////////////////////////////////////////////////////
// Copy contents of controls back to assay def
// Called regardless of whether page is (or was ever) visible
// It's too late to check validity here
////////////////////////////////////////////////////////////////////////

void CADPgMain::OnOK() 
{
	if(!UpdateData(TRUE))
	{	//this function will show the error
		return;
	}

	//if OK was pressed, we assume a change was made
	m_pAD->SetModifier(gGateKeeper.CurrentUserNameGet());
	m_pAD->SetModDate(COleDateTime::GetCurrentTime()); //i.e., now

	// Save Replicates and max tubes to Assay Def
	m_pAD->SetDefaultRepls(m_defaultRepl); 
	m_pAD->SetMaxTubes(m_maxTubes); 

	// Update assay def with measurement method
	int iMeasSel = m_measMethCB.GetCurSel();
	ASSERT(iMeasSel != -1);
	CString tempStr;
	m_measMethCB.GetLBText(iMeasSel, tempStr);
	m_pAD->SetMeasName(tempStr);
	
	CPropertyPage::OnOK();
}

void CADPgMain::OnAdNewMeasmeth() 
{
	// Just another way of launching the usual stuff
	((CMainFrame*)AfxGetMainWnd())->OnSetupMeasNew();
	CADPgMain::OnInitDialog();
}
