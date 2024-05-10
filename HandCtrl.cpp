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

// HandCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "HandCtrl.h"
#include "LuceErr.h"
#include "LccDef.h"
#include "LccMgr.h"
#include "Specification.h"
// #include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHandCtrl dialog

CHandCtrl::CHandCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CHandCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHandCtrl)
	m_nChainEdit = 1;
	m_nInjEdit2 = 25;
	m_nInjEdit1 = 25;
	m_nInjEdit3 = 25;
	m_nInjEditP = 25;
	m_sBkg = _T("");
	m_sRLU = _T("");
	//}}AFX_DATA_INIT

	m_bTubes = 0;

    m_nIntegrationTime = 1;

	m_nLiftState = LIFT_UNKNOWN;

	bLockControls = false;
	m_state = STATE_NULL;
}


void CHandCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHandCtrl)
	DDX_Text(pDX, IDC_EDIT_INTEGRATION_TIME, m_nIntegrationTime);
	DDX_Control(pDX, IDC_INJECT_1_EDIT, m_injEdit1);
	DDX_Control(pDX, IDC_INJECT_2_EDIT, m_injEdit2);
	DDX_Control(pDX, IDC_INJECT_3_EDIT, m_injEdit3);
	DDX_Control(pDX, IDC_INJECT_P_EDIT, m_injEditP);

	DDX_Control(pDX, IDC_INJECT_1_SPIN, m_injSpin1);
	DDX_Control(pDX, IDC_INJECT_2_SPIN, m_injSpin2);
	DDX_Control(pDX, IDC_INJECT_3_SPIN, m_injSpin3);
	DDX_Control(pDX, IDC_INJECT_P_SPIN, m_injSpinP);

	DDX_Control(pDX, IDC_CHAIN_EDIT, m_chainEdit);
	DDX_Control(pDX, IDC_CHAIN_SPIN, m_chainSpin);
	DDX_Text(pDX, IDC_CHAIN_EDIT, m_nChainEdit);
	DDV_MinMaxInt(pDX, m_nChainEdit, 1, 179);

	DDX_Text(pDX, IDC_INJECT_1_EDIT, m_nInjEdit1);
	DDV_MinMaxInt(pDX, m_nInjEdit1, 25, 300);
	DDX_Text(pDX, IDC_INJECT_2_EDIT, m_nInjEdit2);
	DDV_MinMaxInt(pDX, m_nInjEdit2, 25, 300);
	DDX_Text(pDX, IDC_INJECT_3_EDIT, m_nInjEdit3);
	DDV_MinMaxInt(pDX, m_nInjEdit3, 25, 300);
	DDX_Text(pDX, IDC_INJECT_P_EDIT, m_nInjEditP);
	DDV_MinMaxInt(pDX, m_nInjEditP, 25, 300);
	
	DDX_Text(pDX, IDC_TXT_BACKGROUND, m_sBkg);
	DDX_Text(pDX, IDC_TXT_RLU, m_sRLU);
	//}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_TXT_STEPS, m_sSteps);
    DDX_Control(pDX, IDC_COMBO_FINDVIAL, m_cbFindVial);
}


BEGIN_MESSAGE_MAP(CHandCtrl, CDialog)
	//{{AFX_MSG_MAP(CHandCtrl)
	ON_MESSAGE(WM_USER, OnUser)
	ON_BN_CLICKED(IDCANCEL, OnClose)
	ON_BN_CLICKED(IDC_CHAIN_B, OnChainBack)
	ON_BN_CLICKED(IDC_CHAIN_F, OnChainFwd)
	ON_BN_CLICKED(IDC_BUTTON_INJ_1, OnInject1)
	ON_BN_CLICKED(IDC_BUTTON_INJ_2, OnInject2)
	ON_BN_CLICKED(IDC_BUTTON_INJ_3, OnInject3)
	ON_BN_CLICKED(IDC_BUTTON_INJ_P, OnInjectP)
	ON_BN_CLICKED(IDC_LIFT_DOWN, OnLiftDown)
	ON_BN_CLICKED(IDC_LIFT_HALF, OnLiftHalf)
	ON_BN_CLICKED(IDC_LIFT_UP, OnLiftUp)
	ON_BN_CLICKED(IDC_LOCK_CLOSED, OnLockClosed)
	ON_BN_CLICKED(IDC_LOCK_OPEN, OnLockOpen)
	ON_BN_CLICKED(IDC_DOOR_CLOSED, OnDoorClose)
	ON_BN_CLICKED(IDC_DOOR_OPEN, OnDoorOpen)
	ON_BN_CLICKED(IDC_LUMIN_START, OnLuminStart)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FINDVIAL, OnFindVial)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_FIRST_SEG, OnClickedButtonFirstSeg)
    ON_BN_CLICKED(IDC_BUTTON_NEXT_SEG, OnClickedButtonNextSeg)
    ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandCtrl message handlers

BOOL CHandCtrl::OnInitDialog() 
{
	UDACCEL injAccel = {0, 25};
	CDialog::OnInitDialog();

	m_chainSpin.SetRange(1, 179);
	m_injSpin1.SetRange(25, 300);
	m_injSpin1.SetAccel(1, &injAccel);
	m_injSpin2.SetRange(25, 300);
	m_injSpin2.SetAccel(1, &injAccel);
	m_injSpin3.SetRange(25, 300);
	m_injSpin3.SetAccel(1, &injAccel);
	m_injSpinP.SetRange(25, 300);
	m_injSpinP.SetAccel(1, &injAccel);

	// Init bitmapped buttons 
	m_offline.AutoLoad(IDC_HC_OFFLINE,this);
	m_offline.EnableWindow(false);
	m_ready.AutoLoad(IDC_HC_READY,this);
	m_ready.EnableWindow(false);
	m_busy.AutoLoad(IDC_HC_BUSY,this);
	m_busy.EnableWindow(false);
	m_inj1.AutoLoad(IDC_BUTTON_INJ_1,this);
	m_inj2.AutoLoad(IDC_BUTTON_INJ_2,this);
	m_inj3.AutoLoad(IDC_BUTTON_INJ_3,this);
	m_injP.AutoLoad(IDC_BUTTON_INJ_P,this);

	if( Specification::g_oSpecification.Has4thInjector() == false) {
		/*
		 * if there is no fourth injector then hide associated controls and shift other
		 * controls accordingly.
		 */
		CRect oRectP;
		CRect oRect1;
		CRect oRect2;
		CRect oRect3;

		m_injEditP.GetWindowRect( &oRectP);
		m_injEdit1.GetWindowRect( &oRect1);
		m_injEdit2.GetWindowRect( &oRect2);
		m_injEdit3.GetWindowRect( &oRect3);

		long nShiftInj1 = oRect1.left - oRectP.left;
		long nShiftInj2 = oRect2.left - (oRect3.left + oRectP.left) / 2;

		Specification::MoveWindowLeft( &m_injEdit1, nShiftInj1);
		Specification::MoveWindowLeft( &m_injEdit2, nShiftInj2);
		Specification::MoveWindowLeft( &m_injSpin1, nShiftInj1);
		Specification::MoveWindowLeft( &m_injSpin2, nShiftInj2);
		Specification::MoveWindowLeft( &m_inj1, nShiftInj1);
		Specification::MoveWindowLeft( &m_inj2, nShiftInj2);
		Specification::MoveWindowLeft( GetDlgItem( IDC_LABEL_INJECT_1), nShiftInj1);
		Specification::MoveWindowLeft( GetDlgItem( IDC_LABEL_INJECT_2), nShiftInj2);

		m_injEditP.ShowWindow( SW_HIDE);
		m_injSpinP.ShowWindow( SW_HIDE);
		m_injP.ShowWindow( SW_HIDE);
		GetDlgItem( IDC_LABEL_INJECT_P)->ShowWindow( SW_HIDE);
	}

    m_cbFindVial.SetCurSel(1);

	((CButton*)GetDlgItem(IDC_LOCK_OPEN))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_DOOR_CLOSED))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_LIFT_UP))->SetCheck(1);

	m_cbFindVial.ResetContent();
	if( Specification::g_oSpecification.Has4thInjector()) {
		m_cbFindVial.AddString( _T("Inj P"));
		m_cbFindVial.SetItemData( m_cbFindVial.GetCount() - 1, 0);
	}
	m_cbFindVial.AddString( _T("Inj 1"));
	m_cbFindVial.SetItemData( m_cbFindVial.GetCount() - 1, 1);
	m_cbFindVial.AddString( _T("Inj 2"));
	m_cbFindVial.SetItemData( m_cbFindVial.GetCount() - 1, 2);
	m_cbFindVial.AddString( _T("Inj 3"));
	m_cbFindVial.SetItemData( m_cbFindVial.GetCount() - 1, 3);

	switch( Specification::g_oSpecification.GetInstrumentType()) {
		case INSTRUMENT_C3:
		    GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_LOCK_BMP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_DOOR_BMP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CHAIN_BMP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CAROUSEL_BMP)->ShowWindow(SW_SHOW);
			break;

		default:
			GetDlgItem(IDC_BUTTON_FIRST_SEG)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_NEXT_SEG)->ShowWindow(SW_HIDE);
//				GetDlgItem(IDC_BUTTON_FINDVIAL)->ShowWindow(SW_HIDE);
//				GetDlgItem(IDC_COMBO_FINDVIAL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DOOR_OPEN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DOOR_CLOSED)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LOCK_OPEN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_LOCK_CLOSED)->ShowWindow(SW_SHOW);
//				GetDlgItem(IDC_STATIC_POSITION)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_LOCK_BMP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_DOOR_BMP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHAIN_BMP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CAROUSEL_BMP)->ShowWindow(SW_HIDE);

			CRect oDoorRect;
			CRect oUnlockRect;
				
			GetDlgItem( IDC_DOOR_OPEN)->GetWindowRect( &oDoorRect);
			GetDlgItem( IDC_LOCK_OPEN)->GetWindowRect( &oUnlockRect);

			Specification::MoveWindowUp( GetDlgItem( IDC_LOCK_OPEN), oUnlockRect.top - oDoorRect.top);
			Specification::MoveWindowUp( GetDlgItem( IDC_LOCK_CLOSED), oUnlockRect.top - oDoorRect.top);

			break;
	}

	/*
	 * Controls disabled until lcc established.
	 */
	ControlsEnable( STATE_BUSY);

	/*
	 * Delay initialisation of dialog until it has been drawn: sometime it
	 * can not appear for a few seconds.
	 */
	m_nInitTimer = SetTimer( 1, 250, 0);

	return TRUE;
}

void CHandCtrl::OnTimer( UINT_PTR nEvent)
{
	long rv;

	KillTimer( m_nInitTimer);

	RedrawWindow();
	UpdateWindow();	

	// Create Lumactor
	m_actor.EnableUserMessages(false);
	rv = m_actor.Init(this);
	if (rv == KO_OK)
	{
		m_state = STATE_READY;

		// Make it paint
		RedrawWindow();

		UpdateDoorState();

		// This is done in ClearForLaunch
		// DoLockOpen(true);

		if( m_actor.ClearForLaunch() == false) {
			OnClose();
			return;
		}

		DoLiftUp();

		LumStatusUpdate();
	}
	else if(rv == KO_LCC_LUM_DISCONNECTED)
	{
		ControlsEnable(m_state = STATE_OFFLINE);
	}
	else if(rv == KO_LUC_REFUSE_RELEASE)
	{
		AfxMessageBox(IDS_NO_CONTROL_LCC);
		OnClose();
	}
	else
	{
		CString errMsg;
		errMsg.Format(_T("Unexpected error 0x%04x (%s)"), rv, CLccMgr::LookupError( rv));
		AfxMessageBox(errMsg);
		OnClose();
	}
}

LRESULT CHandCtrl::OnUser(WPARAM wParam, LPARAM lParam)
{

	return (LRESULT)TRUE;
}	

/////////////////////////////////////////////////////////////////////////////
// Message handlers
/////////////////////////////////////////////////////////////////////////////

void CHandCtrl::OnChainBack()	{ DoChainBack();	}
void CHandCtrl::OnChainFwd()	{ DoChainFwd();		}
void CHandCtrl::OnInject1()		{ DoInject1();		}
void CHandCtrl::OnInject2()		{ DoInject2();		}
void CHandCtrl::OnInject3()		{ DoInject3();		}
void CHandCtrl::OnInjectP()		{ DoInjectP();		}
void CHandCtrl::OnLiftDown()	{ DoLiftDown();		}
void CHandCtrl::OnLiftHalf()	{ DoLiftHalf();		}
void CHandCtrl::OnLiftUp()		{ DoLiftUp();		}
void CHandCtrl::OnLockClosed()	{ DoLockClosed();	}
void CHandCtrl::OnLockOpen()	{ DoLockOpen();		}
void CHandCtrl::OnLuminStart()	{ DoLuminStart();	} 
void CHandCtrl::OnDoorClose()	{ DoDoorClose();	}
void CHandCtrl::OnDoorOpen()	{ DoDoorOpen();		}
void CHandCtrl::OnFindVial()	{ DoFindVial();		}
void CHandCtrl::OnClickedButtonStop() { DoStop(); }
void CHandCtrl::OnClickedButtonFirstSeg() { DoFirstSeg(); }
void CHandCtrl::OnClickedButtonNextSeg() { DoNextSeg(); }

/////////////////////////////////////////////////////////////////////////////
// Go
/////////////////////////////////////////////////////////////////////////////

long CHandCtrl::Action(long actionID, long par1, long par2, long timeout)
{
	CString errMsg;

//	LumStatusUpdate();

	if (m_state != STATE_READY)
	{
		return KO_LCC_LUM_DISCONNECTED;
	}

	// disable buttons
	ControlsEnable(m_state = STATE_BUSY);

	// do what needs to be done
	long rv = m_actor.Action(actionID, par1, par2, timeout);
	if (rv != KO_OK)
	{
		// !!! human readable error messages
		errMsg.Format(IDS_LCC_EXCEPTION, rv, CLccMgr::LookupError( rv));
		AfxMessageBox(errMsg);
	}

	// Re-enable buttons 
	m_state = STATE_READY;

	return rv;
}

/**
 * Reenable buttons after doing something.
 */
void CHandCtrl::EnableButtons( bool toBeContinued)
{
	// Re-enable buttons 
	m_state = STATE_READY;
	LumStatusUpdate();
	UpdateData( FALSE);
	if (!toBeContinued)
	{
		ControlsEnable(m_state);
	}
}

void CHandCtrl::DoChainBack(bool toBeContinued) 
{
	UpdateData();

	// Set timeout based on number of steps
	long timeOut = LCC_ACT_MCHAIN_TIMEOUT * m_nChainEdit / 180;
	timeOut += timeOut / 2;		// It's slower backwards
	// Plus 5 seconds
	timeOut += 5000;

	DoLiftUp(true);
	Action(LCC_ACT_MOVE_CHAIN, LCC_CHAIN_REV, m_nChainEdit, timeOut);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoChainFwd(bool toBeContinued) 
{
	UpdateData();

	// Set timeout based on number of steps
	long timeOut = LCC_ACT_MCHAIN_TIMEOUT * m_nChainEdit / 180;
	// Plus 5 seconds
	timeOut += 5000;

	DoLiftUp(true);
	Action(LCC_ACT_MOVE_CHAIN, LCC_CHAIN_FWD, m_nChainEdit, timeOut);

	EnableButtons( toBeContinued);
}

void CHandCtrl::OnAbort() 
{
//	m_actor.Stop();
}

void CHandCtrl::DoInject1(bool toBeContinued) 
{
	UpdateData();

	Action(LCC_ACT_INJECT, LCC_INJ1, m_nInjEdit1);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoInject2(bool toBeContinued) 
{
	UpdateData();

	Action(LCC_ACT_INJECT, LCC_INJ2, m_nInjEdit2);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoInject3(bool toBeContinued) 
{
	UpdateData();

	Action(LCC_ACT_INJECT, LCC_INJ3, m_nInjEdit3);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoInjectP(bool toBeContinued) 
{
	UpdateData();

	Action(LCC_ACT_INJECT, LCC_INJP, m_nInjEditP);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoLiftDown(bool toBeContinued) 
{
	// Lift won't go down from half position
	if (m_nLiftState == LIFT_UNKNOWN || m_nLiftState == LIFT_HALF) {
		// Send it up first
		Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_UP, 0, 2000);	
	}

	Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_DOWN, 0, 2000);	
	m_nLiftState = LIFT_DOWN;

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoLiftHalf(bool toBeContinued) 
{
	// Lift won't go to half from down, lcc goes tits up
	if (m_nLiftState == LIFT_UNKNOWN || m_nLiftState == LIFT_DOWN) {
		// Send it up first
		Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_UP, 0, 2000);	
	}

	Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_HALF_DOWN, 0, 2000);	
	m_nLiftState = LIFT_HALF;

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoLiftUp(bool toBeContinued) 
{
	Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_UP, 0);	
	m_nLiftState = LIFT_UP;
	
	// Since move chain raises lift, I have to manage the lift buttons
	((CButton*)GetDlgItem(IDC_LIFT_UP))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_LIFT_DOWN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_LIFT_HALF))->SetCheck(0);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoLockClosed(bool toBeContinued) 
{
	Action(LCC_ACT_DOOR_LOCK, LCC_DOOR_LOCK, 0);	

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoLockOpen(bool toBeContinued) 
{
	Action(LCC_ACT_DOOR_LOCK, LCC_DOOR_UNLOCK, 0);	

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoDoorOpen(bool toBeContinued)
{
	Action(LCC_ACT_DOOR, LCC_DOOR_OPENED);	

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoDoorClose(bool toBeContinued)
{
	Action(LCC_ACT_DOOR, LCC_DOOR_CLOSED);	

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoFirstSeg(bool toBeContinued)
{
	Action(LCC_ACT_MOVE_CAROUSEL, LCC_TFLTT_FIRST_SEG);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoNextSeg(bool toBeContinued)
{
	Action(LCC_ACT_MOVE_CAROUSEL, LCC_TFLTT_NEXT_SEG, 0);	

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoFindVial(bool toBeContinued)
{
    int nSel = m_cbFindVial.GetCurSel();

	if( nSel < 0) {
		nSel = 0;
	}

    int nIndex = m_cbFindVial.GetItemData( nSel);

	DoLiftUp(true);

	long timeOut = LCC_ACT_MCHAIN_TIMEOUT;

	// Regualr chain move wisely lifts the measure head/injectors/whatever it is
	Action( LCC_ACT_FINDFIRST, nIndex, 0, timeOut);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoStop(bool toBeContinued)
{
	Action( LCC_ACT_FINDFIRST, toBeContinued);

	// LCC code does this... why?
    double var1 = 0.0;
    m_actor.Inquire(LCC_INQ_LUMSTAT, &var1);

	EnableButtons( toBeContinued);
}

void CHandCtrl::DoLuminStart(bool toBeContinued) 
{
	double lum, dark;
	long flags;

	UpdateData( true); // get time entered
	Action(LCC_ACT_GET_LUM, m_nIntegrationTime, 0);	
	if( m_actor.GetLumData(&lum, &dark, &flags)) {
		m_sBkg.Format(_T("%g"), dark);
		m_sRLU.Format(_T("%g"), lum);
		EnableButtons( toBeContinued);
	}
}

void CHandCtrl::OnClose() 
{
	m_actor.Release();
	CDialog::OnCancel();
}


////////////////////////////////////////////////////////////////////////
// Enable/Disable buttons 
////////////////////////////////////////////////////////////////////////
void CHandCtrl::ControlsEnable(EState state)
{
	CWnd* control = GetWindow(GW_CHILD);
	while (control != NULL)
	{	// disable child window (control)
		switch(control->GetDlgCtrlID())
		{
		case IDC_CHAIN_B:
		case IDC_CHAIN_F:
		case IDC_LIFT_DOWN:
		case IDC_LIFT_HALF:
		case IDC_LIFT_UP:
		case IDC_LOCK_CLOSED:
		case IDC_LOCK_OPEN:
		case IDC_BUTTON_FIRST_SEG:
		case IDC_BUTTON_NEXT_SEG:
		case IDC_BUTTON_FINDVIAL:
		case IDC_INJECT_1_EDIT:
		case IDC_INJECT_2_EDIT:
		case IDC_INJECT_3_EDIT:
		case IDC_INJECT_P_EDIT:
		case IDC_INJECT_1_SPIN:
		case IDC_INJECT_2_SPIN:
		case IDC_INJECT_3_SPIN:
		case IDC_INJECT_P_SPIN:
		case IDC_DOOR_OPEN:
		case IDC_DOOR_CLOSED:
		case IDC_CHAIN_EDIT:
		case IDC_CHAIN_SPIN:
		case IDC_LUMIN_COMBO:
			control->EnableWindow(state == STATE_READY);
			break;


		case IDC_BUTTON_INJ_1:
			control->EnableWindow(state == STATE_READY && ((m_bTubes & Specification::INJECTOR_1) != 0));
			break;

		case IDC_BUTTON_INJ_2:
			control->EnableWindow(state == STATE_READY && ((m_bTubes & Specification::INJECTOR_2) != 0));
			break;

		case IDC_LUMIN_START:
		case IDC_BUTTON_INJ_3:
			control->EnableWindow(state == STATE_READY && ((m_bTubes & Specification::INJECTOR_3) != 0));
			break;

		case IDC_BUTTON_INJ_P:
			control->EnableWindow(state == STATE_READY && ((m_bTubes & Specification::INJECTOR_P) != 0));
			break;

		case IDC_HAND_ABORT:
			control->EnableWindow(state == STATE_BUSY);
			break;

		case IDCANCEL:
			control->EnableWindow(state != STATE_BUSY);
			break;

		case IDC_HC_OFFLINE:
			((CButton*)control)->SetState(state == STATE_OFFLINE);
			break;

		case IDC_HC_READY:
			((CButton*)control)->SetState(state == STATE_READY);
			break;

		case IDC_HC_BUSY:
			((CButton*)control)->SetState(state == STATE_BUSY);
			break;

		case 0:
		default:
			break;
		}

		// Check if this is last child (control)
		if (control->GetSafeHwnd() != control->GetWindow(GW_HWNDLAST)->GetSafeHwnd())
		{	// Not last control, get next control
			control = control->GetWindow(GW_HWNDNEXT);
		}
		else
		{
			control = NULL;
		}
	}

	return;
}

void CHandCtrl::LumStatusUpdate()
{
	CButton* pButton = NULL;
	double dStatus = 0.0;

	if (!m_actor.IsLumOnline())
	{
		ControlsEnable(m_state = STATE_OFFLINE);
		return;
	}

	// Check for tubes under injectors
	m_actor.Inquire(LCC_INQ_LUMSTAT,&dStatus);
	m_bTubes = Specification::g_oSpecification.DecodeTubeStatus( (long)dStatus);
}

/**
 * Update the state of the door.
 */
void CHandCtrl::UpdateDoorState()
{
    double var1 = 0.0;
	m_actor.Inquire(LCC_INQ_LUMSTAT, &var1);

    STATUS status;
    status.u = (BYTE)var1;

    CheckDlgButton(IDC_DOOR_OPEN, status.st.uDoor == 0 ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_DOOR_CLOSED, status.st.uDoor == 1 ? BST_CHECKED : BST_UNCHECKED);
}

void CHandCtrl::SetSteps(CString sText)
{
    m_sSteps = sText;
    UpdateData(FALSE);
}
