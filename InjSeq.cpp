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

// InjSeq.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "InjSeq.h"
#include "LuceErr.h"
#include "LucyDef.h"
#include "LucyDoc.h"
#include "LucyView.h"
#include <stdlib.h>
#include "LucyBar.h"
#include "IniFiles.h"
#include "Specification.h"

extern CLuceptApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SPIN_MAX	MAX_REAGENT_VOL/100		//window data is in terms of mls, 
#define SPIN_MIN	0		

/////////////////////////////////////////////////////////////////////////////
// CInjSeq dialog
//

CInjSeq::CInjSeq(MAINT_SEQ Script, CWnd* pParent /*=NULL*/)
: CDialog(CInjSeq::IDD, pParent),
m_kCurrentScript(Script)
{
	//{{AFX_DATA_INIT(CInjSeq)
	m_ScriptTitle = _T("");
	m_PromptBox = _T("");
	m_Rgnt1Box = _T("");
	m_Rgnt2Box = _T("");
	m_Rgnt3Box = _T("");
	m_RgntPBox = _T("");
	m_Chk1 = FALSE;
	m_Chk2 = FALSE;
	m_Chk3 = FALSE;
	m_ChkP = FALSE;
	//}}AFX_DATA_INIT
	m_CurrentState		= 0;
	m_index				= 0;
	m_nWashRinseCount	= 0;
	m_nPrimeCount		= 0;
	m_InjectionVolume	= 0;
	m_InjectorMask		= 0;
	m_nRequiredTubes	= 0;
	m_bTubeDetected		= false;
	m_requestCancel = PROCESS_CANCEL;

	m_ScriptTitles[0].LoadString(IDS_INJ_SEQ_STARTUP); 
	m_ScriptTitles[1].LoadString(IDS_INJ_SEQ_SHUTDOWN);
	m_ScriptTitles[2].LoadString(IDS_INJ_SEQ_WASH); 
	m_ScriptTitles[3].LoadString(IDS_INJ_SEQ_RINSE); 
	m_ScriptTitles[4].LoadString(IDS_INJ_SEQ_PRIME); 
	m_ScriptTitles[5].LoadString(IDS_INJ_SEQ_REFRESH);
}


void CInjSeq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInjSeq)
	DDX_Control(pDX, IDCANCEL, m_BtnCcl);
	DDX_Control(pDX, ID_NEXT, m_BtnNext);
	DDX_Control(pDX, IDC_REAGENT3_SPIN, m_Spin3);
	DDX_Control(pDX, IDC_REAGENT2_SPIN, m_Spin2);
	DDX_Control(pDX, IDC_REAGENT1_SPIN, m_Spin1);
	DDX_Control(pDX, IDC_REAGENTP_SPIN, m_SpinP);
	DDX_Text(pDX, IDC_SCRIPT_TITLE_BOX, m_ScriptTitle);
	DDX_Text(pDX, IDC_PROMPT, m_PromptBox);
	DDX_Text(pDX, IDC_RGNT_LEV_1, m_Rgnt1Box);
	DDX_Text(pDX, IDC_RGNT_LEV_2, m_Rgnt2Box);
	DDX_Text(pDX, IDC_RGNT_LEV_3, m_Rgnt3Box);
	DDX_Text(pDX, IDC_RGNT_LEV_P, m_RgntPBox);
	DDX_Check(pDX, IDC_INJ_1, m_Chk1);
	DDX_Check(pDX, IDC_INJ_2, m_Chk2);
	DDX_Check(pDX, IDC_INJ_3, m_Chk3);
	DDX_Check(pDX, IDC_INJ_P, m_ChkP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInjSeq, CDialog)
	//{{AFX_MSG_MAP(CInjSeq)
	ON_BN_CLICKED(ID_NEXT, OnNext)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_23, OnRadio23)
	ON_BN_CLICKED(IDC_RADIO_SELECT, OnRadioSelect)
	ON_WM_CTLCOLOR()
	ON_WM_CANCELMODE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_REAGENT1_SPIN, OnDeltaposReagent1Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_REAGENT2_SPIN, OnDeltaposReagent2Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_REAGENT3_SPIN, OnDeltaposReagent3Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_REAGENTP_SPIN, OnDeltaposReagentPSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInjSeq message handlers
//
BOOL CInjSeq::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pScriptTitle=	(CEdit*)GetDlgItem(IDC_SCRIPT_TITLE_BOX);
	pPromptBox	=	(CEdit*)GetDlgItem(IDC_PROMPT);
	pRgnt1Box	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_1);	
	pRgnt2Box	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_2);	
	pRgnt3Box	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_3);	
	pRgntPBox	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_P);	

	pRdoAll		=	(CButton*)GetDlgItem(IDC_RADIO_ALL);
	pRdo2_3		=	(CButton*)GetDlgItem(IDC_RADIO_23);
	pRdoSel		=	(CButton*)GetDlgItem(IDC_RADIO_SELECT);
	
	pChk1		=	(CButton*)GetDlgItem(IDC_INJ_1);
	pChk2		=	(CButton*)GetDlgItem(IDC_INJ_2);
	pChk3		=	(CButton*)GetDlgItem(IDC_INJ_3);
	pChkP		=	(CButton*)GetDlgItem(IDC_INJ_P);

	pBtnNxt		=	(CButton*)GetDlgItem(ID_NEXT);
	pBtnCcl		=	(CButton*)GetDlgItem(IDCANCEL);
	
	pSpin1		=	(CSpinButtonCtrl*)GetDlgItem(IDC_REAGENT1_SPIN);
	pSpin2		=	(CSpinButtonCtrl*)GetDlgItem(IDC_REAGENT2_SPIN);
	pSpin3		=	(CSpinButtonCtrl*)GetDlgItem(IDC_REAGENT3_SPIN);
	pSpinP		=	(CSpinButtonCtrl*)GetDlgItem(IDC_REAGENTP_SPIN);

	if( Specification::g_oSpecification.Has4thInjector() == false) {
		/*
		 * if there is no fourth injector then hide associated controls and shift other
		 * controls accordingly.
		 */
		CRect oRectP;
		CRect oRect1;
		CRect oRect2;
		CRect oRect3;

		pRgntPBox->GetWindowRect( &oRectP);
		pRgnt1Box->GetWindowRect( &oRect1);
		pRgnt2Box->GetWindowRect( &oRect2);
		pRgnt3Box->GetWindowRect( &oRect3);

		long nShiftInj1 = oRect1.left - oRectP.left;
		long nShiftInj2 = oRect2.left - (oRect3.left + oRectP.left) / 2;

		Specification::MoveWindowLeft( pRgnt1Box, nShiftInj1);
		Specification::MoveWindowLeft( pRgnt2Box, nShiftInj2);
		Specification::MoveWindowLeft( pSpin1, nShiftInj1);
		Specification::MoveWindowLeft( pSpin2, nShiftInj2);
		Specification::MoveWindowLeft( pChk1, nShiftInj1);
		Specification::MoveWindowLeft( pChk2, nShiftInj2);

		pRgntPBox->ShowWindow( SW_HIDE);
		pSpinP->ShowWindow( SW_HIDE);
		pChkP->ShowWindow( SW_HIDE);
	}


	CFont Font;
	Font.CreateFont(
		0,						//height
		0,						//width
		0,						//escapement
		0,						//orientation
		FW_ULTRABOLD,			//weight
		0,						//is italic
		0,						//is underlined
		0,						//is strikeout
		DEFAULT_CHARSET,		//charset
		OUT_DEFAULT_PRECIS,		//output precision
		CLIP_DEFAULT_PRECIS,	//clipping precision
		DEFAULT_QUALITY,		//font quality
		DEFAULT_PITCH,			//pitch and family
		_T("System")			//face name
		);
	
	//Set script title and font
	pScriptTitle->SetFont(&Font, TRUE);	
	pScriptTitle->SetWindowText(m_ScriptTitles[m_kCurrentScript]);

	//set new font in prompt edit box
	pPromptBox->SetFont(&Font, TRUE);

	//get current reagent levels from Lucy	
	ReagentLevelsGetCurrent();
	SpinControlsInit();
	ReagentInputBoxesDisable();

	//start sequence
	OnNext();

	return TRUE;  
}

void CInjSeq::OnNext() 
{
	switch(SwitchOnScript())
	{
	case SCRIPT_FINISHED:
		OnOK();
		break;
	case SCRIPT_ERROR:
		OnCancel();
		break;
	case SCRIPT_STOP:
		AfxMessageBox(IDS_INJ_SEQ_CANCELLED);
		OnCancel();
		break;
	case SCRIPT_UNDEFINED:
		AfxMessageBox(IDS_INJ_SCRIPT_UNDEFINED);
	default:
		break;
	}
	return;
}

void CInjSeq::OnRadioAll() 
{
	pChk1->EnableWindow(FALSE);
	pChk2->EnableWindow(FALSE);
	pChk3->EnableWindow(FALSE);
	pChkP->EnableWindow(FALSE);

	pChk1->SetCheck(1);
	pChk2->SetCheck(1);
	pChk3->SetCheck(1);
	pChkP->SetCheck(1);
}

void CInjSeq::OnRadio23() 
{
	pChk1->EnableWindow(FALSE);
	pChk2->EnableWindow(FALSE);
	pChk3->EnableWindow(FALSE);
	pChkP->EnableWindow(FALSE);
	
	pChk1->SetCheck(0);
	pChk2->SetCheck(1);
	pChk3->SetCheck(1);	
	pChkP->SetCheck(0);	
}

void CInjSeq::OnRadioSelect() 
{
	pChk1->EnableWindow(TRUE);
	pChk2->EnableWindow(TRUE);
	pChk3->EnableWindow(TRUE);
	pChkP->EnableWindow(TRUE);
	
	pChk1->SetCheck(0);
	pChk2->SetCheck(0);
	pChk3->SetCheck(0);	
	pChkP->SetCheck(0);	
}

void CInjSeq::InjectorSelectionDisable()
{
	pRdoAll->EnableWindow(FALSE);
	pRdo2_3->EnableWindow(FALSE);
	pRdoSel->EnableWindow(FALSE);

	pChk1->EnableWindow(FALSE);
	pChk2->EnableWindow(FALSE);
	pChk3->EnableWindow(FALSE);
	pChkP->EnableWindow(FALSE);
}

void CInjSeq::ReagentInputBoxesEnable()
{
	pRgnt1Box->EnableWindow(pChk1->GetState());
	pRgnt2Box->EnableWindow(pChk2->GetState());
	pRgnt3Box->EnableWindow(pChk3->GetState());
	pRgntPBox->EnableWindow(pChkP->GetState());

	//Set focus to leftmost enabled box
	pRgntPBox->SetFocus();
	pRgnt3Box->SetFocus();
	pRgnt2Box->SetFocus();
	pRgnt1Box->SetFocus();

	//spin controls
	SpinControlsEnable();
}

void CInjSeq::ReagentInputBoxesDisable()
{
	pRgnt1Box->EnableWindow(FALSE);
	pRgnt2Box->EnableWindow(FALSE);
	pRgnt3Box->EnableWindow(FALSE);
	pRgntPBox->EnableWindow(FALSE);

	SpinControlsDisable();
}

////////////////////////////////////////////////////////////////////////
// Used to load simple string resources
//
void CInjSeq::LoadPrompt(const UINT StrResourceID)
{
	static CString temp;

	temp.LoadString(StrResourceID);
	pPromptBox->SetWindowText((LPCTSTR)temp);
}

////////////////////////////////////////////////////////////////////////
// Used to load compound strings created in the calling procedure
//
void CInjSeq::LoadPrompt(const CString& s)
{
	pPromptBox->SetWindowText((LPCTSTR)s);
}

void CInjSeq::InjectorSelectionEnable()
{
	//enable selection
	pRdoAll->SetCheck(1);
	pRdo2_3->SetCheck(0);
	pRdoSel->SetCheck(0);

	pRdoAll->SetFocus();
	OnRadioAll();
}

HBRUSH CInjSeq::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;	
}

void CInjSeq::OnCancel() 
{
	// Disable cancel and next buttons
	pBtnCcl->EnableWindow(FALSE);
	pBtnNxt->EnableWindow(FALSE);

	if (m_requestCancel == PROCESS_CANCEL) 
	{
		LCCClearChamber();
		LCCReleaseDoor();
		
		CDialog::OnCancel();
	} 
	else if (m_requestCancel == FLAG_CANCEL) 
	{
		m_requestCancel = CANCEL_FLAGGED;
	}
}

SCRIPT_PROGRESS CInjSeq::SwitchOnScript()
{
	switch(m_kCurrentScript)
	{
	case MAINT_SEQ_STARTUP:
		return SequenceStartup(m_CurrentState);
	case MAINT_SEQ_SHUTDOWN:
		return SequenceShutdown(m_CurrentState);
	case MAINT_SEQ_WASH:
		return SequenceWash(m_CurrentState);
	case MAINT_SEQ_RINSE:
		return SequenceRinse(m_CurrentState);
	case MAINT_SEQ_PRIME:
		return SequencePrime(m_CurrentState);
	case MAINT_SEQ_REFRESH:
		return SequenceRefresh(m_CurrentState);
	default:
		AfxMessageBox(IDS_INJ_INVALID_SCRIPT);
	}
	return SCRIPT_UNDEFINED;
}

/////////////////////////////////////////////////////////////////////////
// Define _NO_BOX to test without hardware attached.
// _NO_BOX must be undefined in the release version.
//
// #define _NO_BOX
#ifdef _NO_BOX
#pragma message(_T("*** Danger! Danger! ***"))
#pragma message(_T("Instrument Control Disabled in Injector Sequences!"))
#endif  
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Find the first tube
/////////////////////////////////////////////////////////////////////////
long CInjSeq::LCCFindFirstTube()
{
	if( Specification::g_oSpecification.HasCarousel() == false) {
		return LCCMoveTubes( MAX_LEADING_EMPTYS);
	} else {
		// With carousels (i.e. C3) use the find first command
	 	long rv; 

		// Initialize the action procedure
		if ((rv = m_actor.Init())!= KO_OK) 
		{
			return rv;
		}

		// Use the find first command
		m_actor.Action( LCC_ACT_FINDFIRST, 3);
		// Release and return
		m_actor.Release();
		m_requestCancel = PROCESS_CANCEL;
		return KO_OK;
	}
}

/////////////////////////////////////////////////////////////////////////
//	LCCMoveTubes -- Move chain forward until there is a tube under 
//		injector3, or a maximum of 16 steps
/////////////////////////////////////////////////////////////////////////
long CInjSeq::LCCMoveTubes(int maxLead)
{
#ifdef _NO_BOX
	if(AfxMessageBox(_T("Moving tubes to position. Cancel simulates an error."), MB_OKCANCEL) == IDCANCEL)
	{
		return !KO_OK;
	} 
	else 
	{
		return KO_OK;
	}
#else

 	long rv; 
	double status;

	// Initialize the action procedure
	if ((rv = m_actor.Init())!= KO_OK) 
	{
		return rv;
	}

	int nTubes;

	if( Specification::g_oSpecification.Has4thInjector()) {
		nTubes = 4;
	} else {
		nTubes = 3;
	}

	// Move the chain three steps
TRACE( "Moving %d steps\n", nTubes);
	if ((rv = m_actor.Action(LCC_ACT_MOVE_CHAIN, LCC_CHAIN_FWD, nTubes)) != KO_OK) 
	{
		m_actor.Release();
		return rv;
	}
	m_index += nTubes;

	// Traverse chain looking for the first tube
	m_requestCancel = FLAG_CANCEL;
	while (true) {	// Is there a tube in the measurement position (injector 3)	
		
		if( TubeAtInjector3()) {
			m_bTubeDetected = true;		// set flag for clear chamber
			break;
		} 
		else if (++m_index > maxLead) 
		{	// Have maxLead positions been searched?
			AfxMessageBox(IDS_INJ_TUBE_SEQ_GAP);
			m_actor.Release();
			m_requestCancel = PROCESS_CANCEL;
			return KO_TUBE_MISCOUNT;
		} 
		else 
		{								// Move one tube 				
			if (m_requestCancel != CANCEL_FLAGGED)
			{
TRACE( "Seeking tube at inj 3, Moving %d steps\n", 1);
				if ((rv = m_actor.Action(LCC_ACT_MOVE_CHAIN, LCC_CHAIN_FWD, 1)) != KO_OK) 
				{
					m_actor.Release();
					m_requestCancel = FLAG_CANCEL;  // Don't try to process cancel actions
					return rv;
				}
			} 
			else 
			{	// m_actor.Action background processing detected Cancel
				m_actor.Release();
				m_requestCancel = PROCESS_CANCEL;
				return KO_LUC_REQ_LCC_STOP;	
			}
		}
	}

	// Release and return
	m_actor.Release();
	m_requestCancel = PROCESS_CANCEL;
	return KO_OK;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
//	LCCInject
//
//
long CInjSeq::LCCInject(const long InjMask, const long& InjVol, const int& InjCount)
{
#ifdef _NO_BOX
	if(AfxMessageBox(_T("Injecting. Cancel simulates an error."), MB_OKCANCEL) == IDCANCEL)
	{
		return !KO_OK;
	} 
	else 
	{
		return KO_OK;
	}
#else

 	long   rv; 
	double status;

	// Initialize LCC
	if ((rv = m_actor.Init()) != KO_OK) 
	{
		return rv;
	}

	// Make sure a tube is under each injector
	if( !TubeAtAll()) {
		AfxMessageBox(IDS_INJ_TUBE_SEQ_GAP);
		m_actor.Release();
		return KO_TUBE_MISCOUNT;
	}

	// Lower the boom
	if ((rv = m_actor.Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_HALF_DOWN)) != KO_OK) 
	{
		m_actor.Release();
		return rv;
	}

	// Inject
	m_requestCancel = FLAG_CANCEL;
	for( int injID = 0;  injID < 4;  injID++) {
		static const int nMasks[] = {
			// Desired sequence is P-1-2-3
			0x08,   // LCC thinks inj 0 is P
			0x01,   // LCC thinks inj 1 is 1
			0x02,   // LCC thinks inj 2 is 3
			0x04
		};

		if( InjMask & nMasks[injID]) {
			for (int count = 0;  count < InjCount;  count++) 
			{
				rv = m_actor.Action(LCC_ACT_INJECT, injID, InjVol);
				if (m_requestCancel != CANCEL_FLAGGED)
				{
					if (rv != KO_OK) 
					{
						m_actor.Release();
						m_requestCancel = FLAG_CANCEL;  // Don't try to process cancel actions
						return rv;
					}
				}
				else 
				{	// User pressed cancel
					m_actor.Release();
					m_requestCancel = PROCESS_CANCEL;
					return KO_LUC_REQ_LCC_STOP;	
				}
			}
		}
	}

	m_requestCancel = PROCESS_CANCEL;
	
	// Raise the boom
	rv = m_actor.Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_UP);
	if (rv != KO_OK) 
	{
		m_actor.Release();
		return rv;
	}

	// Release and return
	m_actor.Release();
	return KO_OK;
#endif
}

long CInjSeq::LCCLockDoor()
{
#ifdef _NO_BOX
	if(AfxMessageBox(_T("Locking door. Cancel simulates an error."), MB_OKCANCEL) == IDCANCEL)
	{
		return !KO_OK;
	} 
	else 
	{
		return KO_OK;
	}
#else 

	long rv; 
	if ((rv = m_actor.Init()) != KO_OK) 
	{
		return rv;
	}

	if( Specification::g_oSpecification.HasDoorLock() == true) {
		rv = m_actor.Action(LCC_ACT_DOOR_LOCK, LCC_DOOR_LOCK);
	} else {
		rv = m_actor.Action(LCC_ACT_DOOR, LCC_DOOR_LOCK);
	}

	m_actor.Release();
	return rv;
#endif
}

/**
 * Unload/open door at the end of a run.
 */
long CInjSeq::LCCReleaseDoor()
{
#ifdef _NO_BOX
	if(AfxMessageBox(_T("Unlocking door. Cancel simulates an error."), MB_OKCANCEL) == IDCANCEL)
	{
		return !KO_OK;
	} 
	else 
	{
		return KO_OK;
	}
#else

 	long rv; 
	if ((rv = m_actor.Init()) != KO_OK) 
	{
		return rv;
	}

	if( Specification::g_oSpecification.HasElectricDoor()) {
		rv = m_actor.Action(LCC_ACT_DOOR, LCC_DOOR_OPENED);
	} else {
		rv = m_actor.Action(LCC_ACT_DOOR_LOCK, LCC_DOOR_UNLOCK);
	}
	m_actor.Release();
	return rv;
#endif
}

/**
 * Open door for when prompting user to load tubes.
 */
long CInjSeq::LCCOpenDoor()
{
#ifdef _NO_BOX
	if(AfxMessageBox(_T("Opening. Cancel simulates an error."), MB_OKCANCEL) == IDCANCEL)
	{
		return !KO_OK;
	} 
	else 
	{
		return KO_OK;
	}
#else

 	long rv; 
	if ((rv = m_actor.Init()) != KO_OK) 
	{
		return rv;
	}

	rv = m_actor.Action(LCC_ACT_DOOR, LCC_DOOR_OPENED);
	m_actor.Release();
	return rv;
#endif
}

/**
 * Shut door for when prompting user to load tubes.
 */
long CInjSeq::LCCCloseDoor()
{
#ifdef _NO_BOX
	if(AfxMessageBox(_T("Closing. Cancel simulates an error."), MB_OKCANCEL) == IDCANCEL)
	{
		return !KO_OK;
	} 
	else 
	{
		return KO_OK;
	}
#else

 	long rv; 
	if ((rv = m_actor.Init()) != KO_OK) 
	{
		return rv;
	}

	rv = m_actor.Action(LCC_ACT_DOOR, LCC_DOOR_CLOSED);
	m_actor.Release();
	return rv;
#endif
}

long CInjSeq::LCCClearChamber()
{
#ifdef _NO_BOX
	if(AfxMessageBox(_T("Moving tubes from chamber. Cancel simulates an error."), MB_OKCANCEL) == IDCANCEL)
	{
		return !KO_OK;
	} 
	else 
	{
		return KO_OK;
	}
#else
	// Initialize 
 	long rv; 

	if ((rv = m_actor.Init()) != KO_OK) 
	{
		return rv;
	}

	if( Specification::g_oSpecification.HasCarousel()) {
		// For C3, move to the first segment
		rv = m_actor.Action( LCC_ACT_MOVE_CAROUSEL, LCC_TFLTT_FIRST_SEG);
	} else {

		if (!m_bTubeDetected)
		{	// Never saw a tube. Do we see any now?
			double status;

			rv = m_actor.Inquire(LCC_INQ_LUMSTAT, &status);
			if (rv != KO_OK)
			{
				return rv;
			}

			// Return if we still don't see any tubes
			if (!Specification::g_oSpecification.TubeAtAny( (long)status)) {
				m_actor.Release();
				return KO_OK;
			}
		}

		// Move the chain 16 steps to clear the chamber
TRACE( "Moving %d steps\n", 16);
		rv = m_actor.Action(LCC_ACT_MOVE_CHAIN, LCC_CHAIN_FWD, 16);
	}

	m_actor.Release();
	return rv;
#endif
}
////////////////////////////////////////////////////////////////////////////////////////
//	LCCNewPrime
//
//  Uses Berthold/Celsis' new firmware primimg algorithm
long CInjSeq::LCCEconoPrime(const long InjMask)
{	
	long rv		= KO_OK;
 	double status;

	// Initialize LCC
	if ((rv = m_actor.Init()) != KO_OK) 
	{
		return rv;
	}

	// Make sure a tube is under each injector
	if( !TubeAtAll()) {
		AfxMessageBox(IDS_INJ_TUBE_SEQ_GAP);
		m_actor.Release();
		return KO_TUBE_MISCOUNT;
	} else {
		m_actor.Release();
		return rv;   //inquiry error
	}

	// Lower the boom
	if ((rv = m_actor.Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_HALF_DOWN)) != KO_OK) 
	{
		m_actor.Release();
		return rv;
	}

	//cf. LCCInject
	m_requestCancel = FLAG_CANCEL;
	for( int injID = 0;  injID < 4;  injID++) {
		static const int nMasks[] = {
			// Desired sequence is P-1-2-3
			0x08,   // LCC thinks inj 0 is P
			0x01,   // LCC thinks inj 1 is 1
			0x02,   // LCC thinks inj 2 is 3
			0x04
		};

		if( InjMask & nMasks[injID]) {
			rv = m_actor.Action(LCC_ACT_PRIME, injID < 4 ? injID : 0);
			if(m_requestCancel != CANCEL_FLAGGED)
			{
				if(rv != KO_OK)
				{
					m_actor.Release();
					m_requestCancel = FLAG_CANCEL;  // Don't try to process cancel actions
					return rv;
				}
			}
			else
			{
				// User pressed cancel
				m_actor.Release();
				m_requestCancel = PROCESS_CANCEL;
				return KO_LUC_REQ_LCC_STOP;	
			}
	
		}
	}
	m_requestCancel = PROCESS_CANCEL;
	
	// Raise the boom
	rv = m_actor.Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_UP);
	if(rv != KO_OK)
	{
		m_actor.Release();
		return rv;
	}

	// Release and return
	m_actor.Release();
	return KO_OK;
}

#ifdef _NO_BOX
#undef _NO_BOX
#endif

void CInjSeq::ReagentLevelsGetCurrent()
{
	long temp;

	//first injector
	theApp.m_pRgntDlg->GetReagentLevel(1, &temp); 
	pRgnt1Box->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));

	//second injector
	theApp.m_pRgntDlg->GetReagentLevel(2, &temp);
	pRgnt2Box->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));

	//third injector
	theApp.m_pRgntDlg->GetReagentLevel(3, &temp);
	pRgnt3Box->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));

	//fourth injector
	theApp.m_pRgntDlg->GetReagentLevel(4, &temp);
	pRgntPBox->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));
}

/*
 * Remember current reagent levels as defaults to be used later on.
 */
void CInjSeq::ReagentLevelsRemember()
{
	long temp;

	//first injector
	theApp.m_pRgntDlg->GetReagentLevel(1, &temp); 
	((CLuceptApp *)AfxGetApp())->WriteLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagent1Level, temp);

	//second injector
	theApp.m_pRgntDlg->GetReagentLevel(2, &temp);
	((CLuceptApp *)AfxGetApp())->WriteLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagent2Level, temp);

	//third injector
	theApp.m_pRgntDlg->GetReagentLevel(3, &temp);
	((CLuceptApp *)AfxGetApp())->WriteLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagent3Level, temp);

	//fourth injector
	theApp.m_pRgntDlg->GetReagentLevel(4, &temp);
	((CLuceptApp *)AfxGetApp())->WriteLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagentPLevel, temp);
}

/*
 * Recall remembered reagent levels.
 */
void CInjSeq::ReagentLevelsRecall()
{
	long temp;

	//first injector
	temp = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagent1Level, 0);
	pRgnt1Box->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));

	//second injector
	temp = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagent2Level, 0);
	pRgnt2Box->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));

	//third injector
	// PCW- should last parameter be 0??
	temp = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagent3Level, 3);
	pRgnt3Box->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));

	//fourth injector
	temp = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt( gIniSec_ReagentLevels, gIniEnt_LastReagentPLevel, 0);
	pRgntPBox->SetWindowText(ReagentInputBoxIntToFltTxt(temp/100));
}

void CInjSeq::ReagentLevelsSetCurrent()
{
	bool warning = false;
	long temp;

	//refresh pointers
	pRgnt1Box	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_1);	
	pRgnt2Box	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_2);	
	pRgnt3Box	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_3);	
	pRgntPBox	=	(CEdit*)GetDlgItem(IDC_RGNT_LEV_P);	

	//first reagent level
	temp = ReagentInputBoxFltTxtToInt(pRgnt1Box);
	if(temp > MAX_REAGENT_VOL/100)
	{
		temp = MAX_REAGENT_VOL/100;
		warning = true;
	}
	theApp.m_pRgntDlg->SetReagent(1, temp * 100); //convert float 0.0 ml to mml

	//second reagent level
	temp = ReagentInputBoxFltTxtToInt(pRgnt2Box);
	if(temp > MAX_REAGENT_VOL/100)
	{
		temp = MAX_REAGENT_VOL/100;
		warning = true;
	}
	theApp.m_pRgntDlg->SetReagent(2, temp * 100); 

	//third reagent level
	temp = ReagentInputBoxFltTxtToInt(pRgnt3Box);
	if(temp > MAX_REAGENT_VOL/100)
	{
		temp = MAX_REAGENT_VOL/100;
		warning = true;
	}
	theApp.m_pRgntDlg->SetReagent(3, temp * 100); 

	//fourth reagent level
	temp = ReagentInputBoxFltTxtToInt(pRgntPBox);
	if(temp > MAX_REAGENT_VOL/100)
	{
		temp = MAX_REAGENT_VOL/100;
		warning = true;
	}
	theApp.m_pRgntDlg->SetReagent(4, temp * 100); 

	//show info warning and correct the input boxes
	if(warning == true) 
	{ 
		AfxMessageBox(IDS_INJ_REAG_OVERFLOW);
		ReagentLevelsGetCurrent();  
	}
}

int CInjSeq::TubeCountGet()
{
	m_nRequiredTubes =  0;

	//GetCheck() returns 1 if selected, else 0
	m_nRequiredTubes =  pChk1->GetCheck();	
	m_nRequiredTubes += pChk2->GetCheck();
	m_nRequiredTubes += pChk3->GetCheck();
	m_nRequiredTubes += pChkP->GetCheck();

	//startup and shutdown require multiple tubes per injector
	//other scripts require only one tube
	if (m_kCurrentScript == MAINT_SEQ_STARTUP) 
	{
		if( Specification::g_oSpecification.Has4thInjector()) {
			m_nRequiredTubes = 12;	//FindTubes() substitute
		} else {
			m_nRequiredTubes = 9;
		}
	}
	else if (m_kCurrentScript == MAINT_SEQ_SHUTDOWN) 
	{
		if( Specification::g_oSpecification.Has4thInjector()) {
			m_nRequiredTubes = 8;
		} else {
			m_nRequiredTubes = 6;
		}
	} 
	else 
	{	//!!! for the time being, tubes for all injectors must be present
		// pcw 19-11-2013 'time being' turned out at 12 years and counting.
		if( Specification::g_oSpecification.Has4thInjector()) {
			m_nRequiredTubes = 4;
		} else {
			m_nRequiredTubes = 3;
		}
	}
	
	return m_nRequiredTubes;
}

void CInjSeq::InjectionVolumeAndCountGet()
{
	//per Sal's instructions:
	if (m_kCurrentScript == MAINT_SEQ_REFRESH) 
	{
		m_InjectionVolume	= 100; //micro liters		//!!!magic number
		m_nPrimeCount		= 1;
		m_nWashRinseCount	= 1;
	} 
	else if( m_kCurrentScript == MAINT_SEQ_PRIME || m_kCurrentScript == MAINT_SEQ_STARTUP) {
 		m_InjectionVolume	= 300; //micro liters		//!!!magic number
		/*
		 * Read prime cycles count from advance.ini
		 */
		m_nPrimeCount	= ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt( gIniSec_ParmDefs, gIniEnt_PrimeCycles, 6);
		m_nWashRinseCount = 6;
	} else {
 		m_InjectionVolume	= 300; //micro liters		//!!!magic number
		m_nPrimeCount	= 6;
		m_nWashRinseCount = 6;
	}
}

long CInjSeq::InjectorMaskGet()
{
	m_InjectorMask = 0;

	//check box, returns 1 if selected, else 0
	if(pChk1->GetCheck())
	{  
		m_InjectorMask |= 0x1;	
	}
	if(pChk2->GetCheck()) 
	{
		m_InjectorMask |= 0x2;
	}
	if(pChk3->GetCheck()) 
	{
		m_InjectorMask |= 0x4;	
	}
	if( Specification::g_oSpecification.Has4thInjector() && pChkP->GetCheck()) 
	{
		m_InjectorMask |= 0x8;	
	}
	return m_InjectorMask;
}

void CInjSeq::OnDeltaposReagent1Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposReagentSpin( pNMHDR, pResult, pRgnt1Box);
}

void CInjSeq::OnDeltaposReagent2Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposReagentSpin( pNMHDR, pResult, pRgnt2Box);
}

void CInjSeq::OnDeltaposReagent3Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposReagentSpin( pNMHDR, pResult, pRgnt3Box);
}

void CInjSeq::OnDeltaposReagentPSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposReagentSpin( pNMHDR, pResult, pRgntPBox);
}

/**
 * Common handler for all spin controls.
 */
void CInjSeq::OnDeltaposReagentSpin(NMHDR* pNMHDR, LRESULT* pResult, CEdit *pReagentBox) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//synch spin control iPos with text control current value
	//in case text has been manually entered
	pNMUpDown->iPos = ReagentInputBoxFltTxtToInt(pReagentBox);

	//increment number	
	if (pNMUpDown->iPos + pNMUpDown->iDelta > SPIN_MAX) 
	{
		pNMUpDown->iPos = SPIN_MAX;
	} 
	else if (pNMUpDown->iPos + pNMUpDown->iDelta < SPIN_MIN) 
	{
		pNMUpDown->iPos = SPIN_MIN;
	} 
	else 
	{
		pNMUpDown->iPos += pNMUpDown->iDelta;
	}

	//write text to control
	pReagentBox->SetWindowText(ReagentInputBoxIntToFltTxt(pNMUpDown->iPos));

	*pResult = 0;
}

void CInjSeq::SpinControlsEnable()
{
	pSpin1->EnableWindow(TRUE);
	pSpin2->EnableWindow(TRUE);
	pSpin3->EnableWindow(TRUE);
	pSpinP->EnableWindow(TRUE);
}

void CInjSeq::SpinControlsDisable()
{
	pSpin1->EnableWindow(FALSE);
	pSpin2->EnableWindow(FALSE);
	pSpin3->EnableWindow(FALSE);
	pSpinP->EnableWindow(FALSE);
}

int CInjSeq::ReagentInputBoxFltTxtToInt(CEdit* pBox)
{
	double f;
	CString strTemp;

	pBox->GetWindowText(strTemp);
	f = _tstof((LPCTSTR)strTemp);
	f *= 10;
	return (int)f;
}

CString CInjSeq::ReagentInputBoxIntToFltTxt(const int& n)
{
	CString text;
	float fTemp;

	//format text as single decimal place float
	fTemp = (float)n;
	fTemp /= 10;
	text.Format(_T("%.1f"), fTemp);
	return text;
}

void CInjSeq::DialogLock(const CString& s)
{
	CString temp;
	
	temp.Format(IDS_INJ_FMT_WAIT_FOR_CYCLE, s);
	LoadPrompt(temp);

	pBtnNxt->EnableWindow(FALSE);
	//pBtnCcl->EnableWindow(FALSE);
	//this->EnableWindow(FALSE);
}

void CInjSeq::DialogUnlock()
{
	pBtnNxt->EnableWindow(TRUE);
	pBtnCcl->EnableWindow(TRUE);
	this->EnableWindow(TRUE);
	
	//if system can't play the sound, the message box pops up
	CString sfile = theApp.GetAppPath();
	sfile += _T("\\Advanceim.wav");
	if(PlaySound(sfile, NULL, SND_FILENAME|SND_ASYNC ) == FALSE)
	{
		Beep(800, 500);			// Notice that the parameters are ignored in Win95
	}
}

SCRIPT_PROGRESS CInjSeq::SequenceStartup(const int& Step)
{
	static CString temp;		//static to prevent reconstruction on each call
	static CString temp1;
	long EconoPrime;
	long rv;

	TRACE( "###############\n");
	TRACE( "Running Startup\n");
	TRACE( "###############\n");

	switch(Step) 
	{
	case 0:
		InjectorSelectionEnable();
		LoadPrompt(IDS_INJ_SELECT_INJS);

		if( ClearForLaunch() == false) {
			return SCRIPT_STOP;
		}

		m_CurrentState++;
		break;

	case 1:
		//make sure at least 1 injector is selected
		if (InjectorMaskGet() < 1) 
		{
			AfxMessageBox(IDS_INJ_NO_INJS_SELECTED);
			break;
		} 
		else 
		{
			InjectorSelectionDisable();
		}

		InjectionVolumeAndCountGet();
		TubeCountGet();

		LCCOpenDoor();

		//build prompt string
		temp1.LoadString(IDS_INJ_FMT_LOAD_N_TUBES); 
		temp.Format(temp1, m_nRequiredTubes);
		LoadPrompt(temp);

		m_CurrentState++;
		break;

	case 2:
		LCCCloseDoor();
		LoadPrompt(IDS_INJ_LOAD_WASH_SOL);
		m_CurrentState++;
		break;

	case 3:
		temp.LoadString(IDS_INJ_INJECTING_WASH);
		DialogLock(temp);
		if(LCCLockDoor() != KO_OK)
		{
			AfxMessageBox(IDS_INJ_ERR_LOCKING_DOOR);
			return SCRIPT_ERROR;
		}
		rv = LCCFindFirstTube();
		if (rv == KO_TUBE_MISCOUNT)	
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}

		rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nWashRinseCount);
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		DialogUnlock();
		m_CurrentState++;//!!! why does this fall through?
	case 4:
		LoadPrompt(IDS_INJ_LOAD_RINSE_SOL);
		m_CurrentState++;
		break;

	case 5:
		temp.LoadString(IDS_INJ_INJECTING_RINSE);
		DialogLock(temp);
		rv = LCCMoveTubes();
		if (rv == KO_TUBE_MISCOUNT)
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}
		rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nWashRinseCount);
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		LoadPrompt(IDS_INJ_LOAD_REAGENT_SOL);
		m_CurrentState++;
		break;

	case 6:
		LoadPrompt(IDS_INJ_ENTER_REAGENT_VOL);
		ReagentLevelsRecall();

		ReagentInputBoxesEnable();
		m_CurrentState++;
		break;

	case 7:
		ReagentLevelsSetCurrent();

		//use EGG's new firmware prime, or not; 1999 Aug 11. DBM
		EconoPrime = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt(gIniSec_Features, gIniEnt_EconoPrime, 0);
		if(EconoPrime)
		{	//keep reagent levels synchronized for decrement
			m_nPrimeCount	= 7;		 
			m_InjectionVolume	= 1200/m_nPrimeCount;//force a total of 1.2 mls
		}
		if (!ReagentLevelsAreAdequate()) 
		{
			temp.LoadString(IDS_INJ_INSUFF_REAGENT);
			temp1.Format(temp, (float)(m_InjectionVolume * m_nPrimeCount)/1000);
			AfxMessageBox(temp1);
			break;
		}
		ReagentInputBoxesDisable();

		temp.LoadString(IDS_INJ_INJECTING_REAGENT);//
		DialogLock(temp);
		rv = LCCMoveTubes();
		if (rv == KO_TUBE_MISCOUNT)
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}

		if(EconoPrime)
		{	
			rv = LCCEconoPrime(m_InjectorMask);
		}
		else
		{
			rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nPrimeCount);
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		if(LCCClearChamber() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_CLEAR_CHAMBER); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		ReagentLevelsDecrement(m_InjectorMask); //decrement reagent levels in selected injectors		
		ReagentLevelsGetCurrent();				//conform controls to decremented state
		
		temp = m_ScriptTitles[m_kCurrentScript];
		temp1.LoadString(IDS_INJ_SCRIPT_FINISHED); 
		temp += temp1;
		LoadPrompt(temp);
		
		temp.LoadString(IDS_INJ_CLOSE_CAPTION);
		pBtnNxt->SetWindowText((LPCTSTR)temp);
		pBtnCcl->EnableWindow(FALSE);
		m_CurrentState++;
		break;
	case 8:
		temp.LoadString(IDS_INJ_UNLOCKING_DOOR);
		DialogLock(temp);
		if(LCCReleaseDoor() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_UNLOCK_DOOR); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		return SCRIPT_FINISHED;
	default:
		AfxMessageBox(IDS_INJ_SCRIPT_CANCELLED);
		return SCRIPT_ERROR;
	}
	return SCRIPT_CONTINUING;
}

SCRIPT_PROGRESS CInjSeq::SequenceShutdown(const int & Step)
{
	static CString temp;		//static to prevent reconstruction on each call
	static CString temp1;
	long rv;

	switch(Step) 
	{
	case 0:
		if( ClearForLaunch() == false) {
			return SCRIPT_STOP;
		}
		
		InjectorSelectionEnable();
		LoadPrompt(IDS_INJ_SELECT_INJS);

		m_CurrentState++;
		break;

	case 1:
		//make sure at least 1 injector is selected
		if (InjectorMaskGet() < 1) 
		{
			AfxMessageBox(IDS_INJ_NO_INJS_SELECTED);
			break;
		} 
		else 
		{
			InjectorSelectionDisable();
		}

		InjectionVolumeAndCountGet();
		TubeCountGet();
		LCCOpenDoor();

		//build prompt string
		temp1.LoadString(IDS_INJ_FMT_LOAD_N_TUBES);
		temp.Format(temp1, m_nRequiredTubes);
		LoadPrompt(temp);

		m_CurrentState++;
		break;

	case 2:
		LCCCloseDoor();
		LoadPrompt(IDS_INJ_LOAD_RINSE_SOL);
		m_CurrentState++;
		break;

	case 3:
		temp.LoadString(IDS_INJ_INJECTING_RINSE);
		DialogLock(temp);
		if(LCCLockDoor() != KO_OK){
			AfxMessageBox(IDS_INJ_ERR_LOCKING_DOOR); 
			return SCRIPT_ERROR;
		}
		rv = LCCFindFirstTube();
		if (rv == KO_TUBE_MISCOUNT)	
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}
		rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nWashRinseCount);
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		LoadPrompt(IDS_INJ_LOAD_WASH_SOL);
		m_CurrentState++;
		break;

	case 4:
		temp.LoadString(IDS_INJ_INJECTING_WASH);
		DialogLock(temp);
		rv = LCCMoveTubes();
		if (rv == KO_TUBE_MISCOUNT)	
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}
		rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nWashRinseCount);
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		if(LCCClearChamber() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_CLEAR_CHAMBER); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		ReagentLevelsRemember();
		ReagentLevelsZero();
		ReagentLevelsGetCurrent();

		//script finished string
		temp = m_ScriptTitles[m_kCurrentScript];
		temp1.LoadString(IDS_INJ_SCRIPT_FINISHED);
		temp += temp1;
		LoadPrompt(temp);
		
		temp.LoadString(IDS_INJ_CLOSE_CAPTION);
		pBtnNxt->SetWindowText((LPCTSTR)temp);
		pBtnCcl->EnableWindow(FALSE);
		m_CurrentState++;
		break;	

	case 5:
		temp.LoadString(IDS_INJ_UNLOCKING_DOOR);
		DialogLock(temp);
		if(LCCReleaseDoor() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_UNLOCK_DOOR); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		return SCRIPT_FINISHED;
	default:
		AfxMessageBox(IDS_INJ_SCRIPT_CANCELLED);
		return SCRIPT_ERROR;
	}
	return SCRIPT_CONTINUING;
}

SCRIPT_PROGRESS CInjSeq::SequenceWash(const int & Step)
{
	static CString temp;		//static to prevent reconstruction on each call
	static CString temp1;
	long rv;

	switch(Step) 
	{
	case 0:
		if( ClearForLaunch() == false) {
			return SCRIPT_STOP;
		}
		
		InjectorSelectionEnable();
		LoadPrompt(IDS_INJ_SELECT_INJS);
		m_CurrentState++;
		break;
	
	case 1:
		//make sure at least 1 injector is selected
		if (InjectorMaskGet() < 1) 
		{
			AfxMessageBox(IDS_INJ_NO_INJS_SELECTED);
			break;
		} 
		else 
		{
			InjectorSelectionDisable();
		}

		InjectionVolumeAndCountGet();
		TubeCountGet();
		LCCOpenDoor();

		//build prompt string
		temp1.LoadString(IDS_INJ_FMT_LOAD_N_TUBES);
		temp.Format(temp1, m_nRequiredTubes);
		LoadPrompt(temp);

		m_CurrentState++;
		break;
	
	case 2:
		LCCCloseDoor();
		LoadPrompt(IDS_INJ_LOAD_WASH_SOL);
		m_CurrentState++;
		break;

	case 3:
		temp.LoadString(IDS_INJ_INJECTING_WASH);
		DialogLock(temp);
		if(LCCLockDoor() != KO_OK)
		{
			AfxMessageBox(IDS_INJ_ERR_LOCKING_DOOR); 
			return SCRIPT_ERROR;
		}
		rv = LCCFindFirstTube();
		if (rv == KO_TUBE_MISCOUNT)	
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}
		rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nWashRinseCount);
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		if(LCCClearChamber() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_CLEAR_CHAMBER); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		//finished string
		temp = m_ScriptTitles[m_kCurrentScript];
		temp1.LoadString(IDS_INJ_SCRIPT_FINISHED); 
		temp += temp1;
		LoadPrompt(temp);
		
		temp.LoadString(IDS_INJ_CLOSE_CAPTION);
		pBtnNxt->SetWindowText((LPCTSTR)temp);
		pBtnCcl->EnableWindow(FALSE);

		m_CurrentState++;
		break;

	case 4:
		temp.LoadString(IDS_INJ_UNLOCKING_DOOR);
		DialogLock(temp);
		if(LCCReleaseDoor() != KO_OK) {
			AfxMessageBox(IDS_INJ_ERR_UNLOCK_DOOR); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();
		return SCRIPT_FINISHED;

	default:
		AfxMessageBox(IDS_INJ_SCRIPT_CANCELLED);
		return SCRIPT_ERROR;
	}
	return SCRIPT_CONTINUING;

}

SCRIPT_PROGRESS CInjSeq::SequenceRinse(const int & Step)
{
	static CString temp;		//static to prevent reconstruction on each call
	static CString temp1;
	long rv;

	switch(Step) 
	{
	case 0:
		if( ClearForLaunch() == false) {
			return SCRIPT_STOP;
		}
		
		InjectorSelectionEnable();
		LoadPrompt(IDS_INJ_SELECT_INJS);
		m_CurrentState++;
		break;

	case 1:
		//make sure at least 1 injector is selected
		if (InjectorMaskGet() < 1) 
		{
			AfxMessageBox(IDS_INJ_NO_INJS_SELECTED);
			break;
		} 
		else 
		{
			InjectorSelectionDisable();
		}

		InjectionVolumeAndCountGet();
		TubeCountGet();
		LCCOpenDoor();

		//build prompt string
		temp1.LoadString(IDS_INJ_FMT_LOAD_N_TUBES);
		temp.Format(temp1, m_nRequiredTubes);
		LoadPrompt(temp);

		m_CurrentState++;
		break;

	case 2:
		LoadPrompt(IDS_INJ_LOAD_RINSE_SOL);
		m_CurrentState++;
		break;

	case 3:
		temp.LoadString(IDS_INJ_INJECTING_RINSE);
		DialogLock(temp);
		if(LCCLockDoor() != KO_OK)
		{
			AfxMessageBox(IDS_INJ_ERR_LOCKING_DOOR); 
			return SCRIPT_ERROR;
		}
		rv = LCCFindFirstTube();
		if (rv == KO_TUBE_MISCOUNT)	
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}
		rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nWashRinseCount);
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		if(LCCClearChamber() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_CLEAR_CHAMBER); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		//finished string
		temp = m_ScriptTitles[m_kCurrentScript];
		temp1.LoadString(IDS_INJ_SCRIPT_FINISHED); 
		temp += temp1;
		LoadPrompt(temp);
		
		temp.LoadString(IDS_INJ_CLOSE_CAPTION);
		pBtnNxt->SetWindowText((LPCTSTR)temp);
		pBtnCcl->EnableWindow(FALSE);

		m_CurrentState++;
		break;

	case 4:
		temp.LoadString(IDS_INJ_UNLOCKING_DOOR);
		DialogLock(temp);
		if(LCCReleaseDoor() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_UNLOCK_DOOR); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();
		return SCRIPT_FINISHED;

	default:
		AfxMessageBox(IDS_INJ_SCRIPT_CANCELLED);
		return SCRIPT_ERROR;
	}
	return SCRIPT_CONTINUING;

}

SCRIPT_PROGRESS CInjSeq::SequencePrime(const int & Step)
{
	static CString temp;		//static to prevent reconstruction on each call
	static CString temp1;
	static long EconoPrime;
	long rv;

	switch(Step) 
	{
	case 0:
		if( ClearForLaunch() == false) {
			return SCRIPT_STOP;
		}
		
		InjectorSelectionEnable();
		LoadPrompt(IDS_INJ_SELECT_INJS);
		m_CurrentState++;
		break;
	
	case 1:
		//make sure at least 1 injector is selected
		if (InjectorMaskGet() < 1) 
		{
			AfxMessageBox(IDS_INJ_NO_INJS_SELECTED);
			break;
		} 
		else 
		{
			InjectorSelectionDisable();
		}

		InjectionVolumeAndCountGet();
		TubeCountGet();
		LCCOpenDoor();

		//build prompt string
		temp1.LoadString(IDS_INJ_FMT_LOAD_N_TUBES);
		temp.Format(temp1, m_nRequiredTubes);
		LoadPrompt(temp);

		m_CurrentState++;
		break;

	case 2:
		LoadPrompt(IDS_INJ_LOAD_REAGENT_SOL);
		if(LCCLockDoor() != KO_OK)
		{
			AfxMessageBox(IDS_INJ_ERR_LOCKING_DOOR); 
			return SCRIPT_ERROR;
		}

		m_CurrentState++;
		break;
	
	case 3:
		LoadPrompt(IDS_INJ_ENTER_REAGENT_VOL);
		ReagentInputBoxesEnable();
		m_CurrentState++;
		break;
	
	case 4:
		ReagentLevelsSetCurrent();
		
		//use EGG's new firmware prime, or not; 1999 Aug 11. DBM
		EconoPrime = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt(gIniSec_Features, gIniEnt_EconoPrime, 0);
		if(EconoPrime)
		{
			//keep reagent levels synchronized for decrement
			m_nPrimeCount	= 7;		 
			m_InjectionVolume	= 1200/m_nPrimeCount;//force a total of 1.2 mls
		}
		if (!ReagentLevelsAreAdequate()) 
		{
			temp.LoadString(IDS_INJ_INSUFF_REAGENT);
			temp1.Format(temp, (float)(m_InjectionVolume * m_nPrimeCount)/1000);
			AfxMessageBox(temp1);
			break;
		}
		ReagentInputBoxesDisable();
		
		temp.LoadString(IDS_INJ_INJECTING_REAGENT);
		DialogLock(temp);
		rv = LCCFindFirstTube();
		if (rv == KO_TUBE_MISCOUNT)	
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}

		if(EconoPrime)
		{	
			rv = LCCEconoPrime(m_InjectorMask);
		}
		else
		{
			rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nPrimeCount);
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		if(LCCClearChamber() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_CLEAR_CHAMBER); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		ReagentLevelsDecrement(m_InjectorMask);//decrement reagent levels in selected injectors		
		ReagentLevelsGetCurrent();			   //conform controls to decremented state
		
		temp = m_ScriptTitles[m_kCurrentScript];
		temp1.LoadString(IDS_INJ_SCRIPT_FINISHED); 
		temp += temp1;
		LoadPrompt(temp);
		
		temp.LoadString(IDS_INJ_CLOSE_CAPTION);
		pBtnNxt->SetWindowText((LPCTSTR)temp);
		pBtnCcl->EnableWindow(FALSE);
		m_CurrentState++;
		break;

	case 5:
		temp.LoadString(IDS_INJ_UNLOCKING_DOOR);
		DialogLock(temp);
		if(LCCReleaseDoor() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_UNLOCK_DOOR); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();		
		return SCRIPT_FINISHED;

	default:
		AfxMessageBox(IDS_INJ_SCRIPT_CANCELLED);
		return SCRIPT_ERROR;
	}
	return SCRIPT_CONTINUING;
}

SCRIPT_PROGRESS CInjSeq::SequenceRefresh(const int & Step)
{
	static CString temp;		//static to prevent reconstruction on each call
	static CString temp1;
	long rv;

	switch(Step) 
	{
	case 0:
		if( ClearForLaunch() == false) {
			return SCRIPT_STOP;
		}
		
		InjectorSelectionEnable();
		LoadPrompt(IDS_INJ_SELECT_INJS);
		m_CurrentState++;
		break;

	case 1:
		//make sure at least 1 injector is selected
		if (InjectorMaskGet() < 1) 
		{
			AfxMessageBox(IDS_INJ_NO_INJS_SELECTED);
			break;
		}
		else 
		{
			InjectorSelectionDisable();
		}

		InjectionVolumeAndCountGet();
		TubeCountGet();
		LCCOpenDoor();

		//build prompt string
		temp1.LoadString(IDS_INJ_FMT_LOAD_N_TUBES);
		temp.Format(temp1, m_nRequiredTubes);
		LoadPrompt(temp);

		m_CurrentState++;
		break;

	case 2:
		LCCCloseDoor();
		ReagentLevelsSetCurrent(); //necessary to get input on successive attempts
		if (!ReagentLevelsAreAdequate()) 
		{
			temp.LoadString(IDS_INJ_INSUFF_REAGENT);
			temp1.Format(temp, (float)(m_InjectionVolume * m_nPrimeCount)/1000);
			AfxMessageBox(temp1);
			ReagentInputBoxesEnable();
			break;
		}
		ReagentInputBoxesDisable();

		temp.LoadString(IDS_INJ_INJECTING_REAGENT);
		DialogLock(temp);
		if(LCCLockDoor() != KO_OK)
		{
			AfxMessageBox(IDS_INJ_ERR_LOCKING_DOOR); 
			return SCRIPT_ERROR;
		}
		rv = LCCFindFirstTube();
		if (rv == KO_TUBE_MISCOUNT)	
		{
			return SCRIPT_ERROR;
		}
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_MOVING_TUBES); 
			return SCRIPT_ERROR;
		}
		rv = LCCInject(m_InjectorMask, m_InjectionVolume, m_nPrimeCount);
		if (rv == KO_LUC_REQ_LCC_STOP)
		{
			return SCRIPT_STOP;
		}
		if(rv != KO_OK) 
		{
			if (rv != KO_TUBE_MISCOUNT)
			{
				AfxMessageBox(IDS_INJ_ERR_INJECTING); 
			}
			return SCRIPT_ERROR;
		}
		if(LCCClearChamber() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_CLEAR_CHAMBER); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();

		//decrement reagent levels in selected injectors	
		ReagentLevelsDecrement(m_InjectorMask);	
		ReagentLevelsGetCurrent();				//conform controls to decremented state

		//finished string
		temp = m_ScriptTitles[m_kCurrentScript];
		temp1.LoadString(IDS_INJ_SCRIPT_FINISHED); 
		temp += temp1;
		LoadPrompt(temp);
		
		temp.LoadString(IDS_INJ_CLOSE_CAPTION);
		pBtnNxt->SetWindowText((LPCTSTR)temp);
		pBtnCcl->EnableWindow(FALSE);
		m_CurrentState++;
		break;

	case 3:
		temp.LoadString(IDS_INJ_UNLOCKING_DOOR);
		DialogLock(temp);
		if(LCCReleaseDoor() != KO_OK) 
		{
			AfxMessageBox(IDS_INJ_ERR_UNLOCK_DOOR); 
			return SCRIPT_ERROR;
		}
		DialogUnlock();
		return SCRIPT_FINISHED;

	default:
		AfxMessageBox(IDS_INJ_SCRIPT_CANCELLED);
		return SCRIPT_ERROR;
	}
	return SCRIPT_CONTINUING;

}

void CInjSeq::ReagentLevelsDecrement(const long &mask)
{
	long temp;

	if(mask & 0x1)
	{	//first bit
		theApp.m_pRgntDlg->GetReagentLevel(1, &temp); 
		temp -= (m_nPrimeCount * m_InjectionVolume);
		theApp.m_pRgntDlg->SetReagent(1, temp);
	}
	
	if(mask & 0x2)
	{	//second bit
		theApp.m_pRgntDlg->GetReagentLevel(2, &temp); 
		temp -= (m_nPrimeCount * m_InjectionVolume);
		theApp.m_pRgntDlg->SetReagent(2, temp);
	}

	if(mask & 0x4)
	{	//third bit
		theApp.m_pRgntDlg->GetReagentLevel(3, &temp); 
		temp -= (m_nPrimeCount * m_InjectionVolume);
		theApp.m_pRgntDlg->SetReagent(3, temp);
	}

	if(mask & 0x8)
	{	//fourth bit
		theApp.m_pRgntDlg->GetReagentLevel(4, &temp); 
		temp -= (m_nPrimeCount * m_InjectionVolume);
		theApp.m_pRgntDlg->SetReagent(4, temp);
	}
}


void CInjSeq::ReagentLevelsZero()
{
	theApp.m_pRgntDlg->SetReagent(1, 0);
	theApp.m_pRgntDlg->SetReagent(2, 0);
	theApp.m_pRgntDlg->SetReagent(3, 0);
	theApp.m_pRgntDlg->SetReagent(4, 0);
}


void CInjSeq::SpinControlsInit()
{
	UDACCEL accelarray[3];
	accelarray[0].nSec	= 1;
	accelarray[0].nInc	= 1;
	accelarray[1].nSec	= 2;
	accelarray[1].nInc	= 10;
	accelarray[2].nSec	= 3;
	accelarray[2].nInc	= 30;

	pSpin1->SetAccel(3, accelarray);
	pSpin2->SetAccel(3, accelarray);
	pSpin3->SetAccel(3, accelarray);
	pSpinP->SetAccel(3, accelarray);
	
	pSpin1->SetRange(SPIN_MIN, SPIN_MAX);
	pSpin2->SetRange(SPIN_MIN, SPIN_MAX);
	pSpin3->SetRange(SPIN_MIN, SPIN_MAX);
	pSpinP->SetRange(SPIN_MIN, SPIN_MAX);

	pSpin1->SetBuddy(pRgnt1Box);
	pSpin2->SetBuddy(pRgnt2Box);
	pSpin3->SetBuddy(pRgnt3Box);
	pSpinP->SetBuddy(pRgntPBox);

	//NOTE: this must be called after reagent boxes are initialized:
	pSpin1->SetPos(ReagentInputBoxFltTxtToInt(pRgnt1Box));
	pSpin2->SetPos(ReagentInputBoxFltTxtToInt(pRgnt2Box));
	pSpin3->SetPos(ReagentInputBoxFltTxtToInt(pRgnt3Box));
	pSpinP->SetPos(ReagentInputBoxFltTxtToInt(pRgntPBox));
}

bool CInjSeq::ReagentLevelsAreAdequate()
{	
	long temp;

	if (m_InjectorMask & 0x1) 
	{
		theApp.m_pRgntDlg->GetReagentLevel(1, &temp); 
		if(temp < m_InjectionVolume * m_nPrimeCount) 
		{
			return false;
		}
	}
	
	if (m_InjectorMask & 0x2) 
	{
		theApp.m_pRgntDlg->GetReagentLevel(2, &temp); 
		if(temp < m_InjectionVolume * m_nPrimeCount) 
		{
			return false;
		}
	}

	if (m_InjectorMask & 0x4) 
	{
		theApp.m_pRgntDlg->GetReagentLevel(3, &temp); 
		if(temp < m_InjectionVolume * m_nPrimeCount) 
		{
			return false;
		}
	}

	if (m_InjectorMask & 0x8) 
	{
		theApp.m_pRgntDlg->GetReagentLevel(4, &temp); 
		if(temp < m_InjectionVolume * m_nPrimeCount) 
		{
			return false;
		}
	}

	return true;
}

/**
 * Determine if clear to run and assay or scan the chain.
 * @returns FALSE if not clear to proceed.
 */
bool CInjSeq::ClearForLaunch()
{
	long rv = m_actor.Init();
	if( rv != KO_OK) { 
		return false;
	}

	bool bClear = m_actor.CLumActor::ClearForLaunch();

	m_actor.Release();

	return bClear;
}

/**
 * Determine whether there is a tube at injector 3
 */
bool CInjSeq::TubeAtInjector3()
{
	double status;

	for( int i=0; i<3; i++) {
		long rv;

		if((rv = m_actor.Inquire(LCC_INQ_LUMSTAT, &status)) == KO_OK) {
			int nStatus = (int)status;
			TRACE( "Seeking tube at inj 3- Status: 0x%02x\n", nStatus);
			if (Specification::g_oSpecification.TubeAtInj3( (long)status)) 
			{
				if( i > 0) {
					TRACE( "Took %d reads to see opto\n", i);
				}
				return true;
			}
		}
		Sleep( 100);
	}

	return false;
}

/**
 * Determine whether there is a tube at all injectors.
 * @returns true if there is a tube at all injectors.
 */
bool CInjSeq::TubeAtAll()
{
	double status;

	for( int i=0; i<3; i++) {
		long rv;

		if((rv = m_actor.Inquire(LCC_INQ_LUMSTAT, &status)) == KO_OK) {
			int nStatus = (int)status;
			TRACE( "Seeking tube at all- Status: 0x%02x\n", nStatus);
			if (Specification::g_oSpecification.TubeAtAll( (long)status)) 
			{
				if( i > 0) {
					TRACE( "Took %d reads to see opto\n", i);
				}
				return true;
			}
		}
		Sleep( 100);
	}

	return false;
}

//make sure these constants form InjSeq.cpp do not interfere
//in other modules
#ifdef SPIN_MAX
#undef SPIN_MAX
#undef SPIN_MIN
#endif

