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

// LucyBar.cpp : implementation file
//

#include "stdafx.h"
#include "Lucept.h"
#include "LucyBar.h"
#include "LuceErr.h"
#include "LccMgr.h"
#include "Specification.h"
#include "Systemlog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLucyBar dialog


CLucyBar::CLucyBar(CWnd* pParent /*=NULL*/)
	: CPaneDialog()
{
	//{{AFX_DATA_INIT(CLucyBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLucyDoc = NULL;
	m_pConductor = NULL;
	// ASSERT(m_pLucyDoc != NULL);
	// ASSERT(m_pLucyDoc->IsKindOf(RUNTIME_CLASS(CLucyDoc)));
}

void CLucyBar::DoDataExchange(CDataExchange* pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLucyBar)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STOP, m_butStopRun);
}


BEGIN_MESSAGE_MAP(CLucyBar, CPaneDialog)
	//{{AFX_MSG_MAP(CLucyBar)
	//}}AFX_MSG_MAP
	ON_COMMAND( IDC_STOP,			 	OnStop)
	ON_UPDATE_COMMAND_UI( IDC_STOP,		OnUpdateStop)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLucyBar message handlers

bool CLucyBar::InitDialog()
{
	m_butStopRun.EnableWindowsTheming( true);
	m_butStopRun.SetImage( IDB_STOP_PNG, 0, 0);

	if( true) {  // Fourth injector fitted?
		VERIFY(ReplaceProgress(	IDC_REAGENT_A, m_ProgCtrlP));
		VERIFY(ReplaceProgress(	IDC_REAGENT_B, m_ProgCtrl1));
		VERIFY(ReplaceProgress(	IDC_REAGENT_C, m_ProgCtrl2));
		VERIFY(ReplaceProgress(	IDC_REAGENT_D, m_ProgCtrl3));
	} else {
		VERIFY(ReplaceProgress(	IDC_REAGENT_A, m_ProgCtrl1));
		VERIFY(ReplaceProgress(	IDC_REAGENT_B, m_ProgCtrl2));
		VERIFY(ReplaceProgress(	IDC_REAGENT_C, m_ProgCtrl3));
		VERIFY(ReplaceProgress(	IDC_REAGENT_D, m_ProgCtrlP));
		m_ProgCtrlP.ShowWindow( SW_HIDE);
	}

	m_ProgCtrl1.SetLabel( _T("1"));
	m_ProgCtrl2.SetLabel( _T("2"));
	m_ProgCtrl3.SetLabel( _T("3"));
	m_ProgCtrlP.SetLabel( _T("P"));

	m_ProgCtrl1.SetBarBrush( 0xe0, 0, 0);       // Red
	m_ProgCtrl2.SetBarBrush( 0x0, 0x80, 0x80);  // Greeny blue
	m_ProgCtrl3.SetBarBrush( 0xff, 0xff, 0x0); // Yellow
	m_ProgCtrlP.SetBarBrush( 0, 44, 119);  // Dark Blue

	if( Specification::g_oSpecification.Has4thInjector() == false) {
		// There is no P injector so hide the P control and space out the
		// remaining controls to fill the gap
		m_ProgCtrlP.ShowWindow( SW_HIDE);

		CRect oRectP;
		CRect oRect1;
		CRect oRect2;
		CRect oRect3;
		CRect oRect;

		m_ProgCtrlP.GetWindowRect( &oRectP);
		m_ProgCtrl1.GetWindowRect( &oRect1);
		m_ProgCtrl2.GetWindowRect( &oRect2);
		m_ProgCtrl3.GetWindowRect( &oRect3);

		long nGap = oRect1.left - oRectP.right;   // gap between controls
		long nSpaceAvailable = oRect3.right - oRectP.left;  // Space available
		long nControlWidth = (nSpaceAvailable - (nGap * 2)) / 3;

		oRect = oRectP;

	    ScreenToClient( (LPPOINT) &((LPPOINT)&oRect)[0]);
	    ScreenToClient( (LPPOINT) &((LPPOINT)&oRect)[1]);

	    oRect.right = oRect.left + nControlWidth;
	    m_ProgCtrl1.MoveWindow( &oRect, FALSE);

	    oRect.left = oRect.right + nGap;
	    oRect.right = oRect.left + nControlWidth;
	    m_ProgCtrl2.MoveWindow( &oRect, FALSE);

	    oRect.left = oRect.right + nGap;
	    oRect.right = oRect.left + nControlWidth;
	    m_ProgCtrl3.MoveWindow( &oRect, FALSE);
	}

	// Add time and temp display
	// Find placeholder 
	CWnd* pPlaceHolder = GetDlgItem(IDC_TIME_DISP);
	if(pPlaceHolder == NULL)
	{
		return false;
	}

	// Locate display to create
	CRect dispRect, barRect;
	GetWindowRect(barRect);
	pPlaceHolder->GetWindowRect(dispRect);
	dispRect.OffsetRect(-barRect.left, -barRect.top);

	// Kill placeholder
	pPlaceHolder->DestroyWindow();

	// Create time display
	if (!m_timeDisp.Create(dispRect, this, IDC_TIME_DISP))
	{
		return false;
	}

	// Find placeholder 
	pPlaceHolder = GetDlgItem(IDC_TEMP_DISP);
	if(pPlaceHolder == NULL)
	{
		return false;
	}

	// Locate display to create
	pPlaceHolder->GetWindowRect(dispRect);
	dispRect.OffsetRect(-barRect.left, -barRect.top);

	// Kill placeholder
	pPlaceHolder->DestroyWindow();

	// Create temp display
	if (!m_tempDisp.Create(dispRect, this, IDC_TEMP_DISP))	
	{
		return false;
	}

	// Hide temperature on C3: TODO- show it if temperature is something other than zero
	if( Specification::g_oSpecification.GetInstrumentType() == INSTRUMENT_C3) {
		GetDlgItem( IDC_TEMP_DISP)->ShowWindow( SW_HIDE);
		GetDlgItem( IDC_TEMPERATURE_FRAME)->ShowWindow( SW_HIDE);
	}

	EnableToolTips(TRUE);

	if (m_pLucyDoc != NULL)
	{
		IndicatorsUpdate();
	}

	RegisterWithLcc();

	return true;
}

void CLucyBar::OnClose() 
{
	UnregisterWithLcc();
	CPaneDialog::OnClose();	
}

bool CLucyBar::ReplaceButton(UINT ID, CBitmapButton& bitBut)
{
	// Find placeholder button
	CWnd* pPlaceHolder = GetDlgItem(ID);
	if(pPlaceHolder == NULL)
	{
		return false;
	}

	// Locate button to create
	CRect btnRect, barRect;
	GetWindowRect(barRect);
	pPlaceHolder->GetWindowRect(btnRect);
	btnRect.OffsetRect(-barRect.left, -barRect.top);

	// Kill placeholder
	pPlaceHolder->DestroyWindow();

	// Create bitmapped button
	if(!bitBut.Create(_T(""),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP,btnRect, this, ID))
	{
		return false;
	}

	return true;
}


bool CLucyBar::ReplaceProgress(UINT ID, CReagentGauge& progCtrl)
{	// Find placeholder control
	CWnd* pPlaceHolder = GetDlgItem(ID);
	if(pPlaceHolder == NULL)
	{
		return false;
	}

	// Locate button to create
	CRect ctrlRect, barRect;
	GetWindowRect(barRect);
	pPlaceHolder->GetWindowRect(ctrlRect);
	ctrlRect.OffsetRect(-barRect.left, -barRect.top);

	// Kill placeholder
	pPlaceHolder->DestroyWindow();

	// Create progress control
	if (!progCtrl.Create(WS_CHILD | WS_VISIBLE, ctrlRect, this, ID))
	{
		return false;
	}

	return true;
}


////////////////////////////////////////////////////////////////////////
//  Reagent Stuff
////////////////////////////////////////////////////////////////////////

long CLucyBar::GetReagentLevel(long injectID, long* level)
{
	int rv = KO_OK;

	ASSERT(m_pLucyDoc != NULL);
	if (injectID < 1 || injectID > NUM_REAGENTS)
	{
		*level =  LUCYDOC_VOL_UNDEF;
		return KO_LUC_BAD_INJECTOR_ID;
	}

	*level = m_pLucyDoc->GetReagentLevel(injectID);

	return rv;
}

long CLucyBar::SetReagent(long injectID, long volume)
{
	int rv = KO_OK;

	ASSERT(m_pLucyDoc != NULL);
	rv = m_pLucyDoc->SetReagentLevel(injectID, volume);
	
	IndicatorsUpdate();

	return rv;
}

long CLucyBar::DeductReagent(long injectID, long volume)
{
	int rv = KO_OK;

	ASSERT(m_pLucyDoc != NULL);
	rv = m_pLucyDoc->DeductReagent(injectID, volume);
	
	IndicatorsUpdate();

	return rv;
}

void CLucyBar::IndicatorsUpdate()
{
	CString temp;

	//progress controls
	ASSERT(m_pLucyDoc != NULL);

	long nCapacity1;
	long nCapacity2;
	long nCapacity3;
	long nCapacityP;

	nCapacity1 = m_pLucyDoc->GetReagentLevel(1) * 100 / m_pLucyDoc->GetReagentCapacity(1);
	nCapacity2 = m_pLucyDoc->GetReagentLevel(2) * 100 / m_pLucyDoc->GetReagentCapacity(2);
	nCapacity3 = m_pLucyDoc->GetReagentLevel(3) * 100 / m_pLucyDoc->GetReagentCapacity(3);
	nCapacityP = m_pLucyDoc->GetReagentLevel(4) * 100 / m_pLucyDoc->GetReagentCapacity(4);

	// Set position to nCapacity + 1 and then nCapacity as a workaround to the 'filling up'
	// animation that appears when the software boots.
	m_ProgCtrl1.SetPos( nCapacity1+1);
	m_ProgCtrl2.SetPos( nCapacity2+1);
	m_ProgCtrl3.SetPos( nCapacity3+1);
	m_ProgCtrlP.SetPos( nCapacityP+1);
	m_ProgCtrl1.SetPos( nCapacity1);
	m_ProgCtrl2.SetPos( nCapacity2);
	m_ProgCtrl3.SetPos( nCapacity3);
	m_ProgCtrlP.SetPos( nCapacityP);

	//edit controls
	temp.Format(_T("%04.1f ml"), (float)m_pLucyDoc->GetReagentLevel(1) / 1000);
	m_ProgCtrl1.SetQuantity(temp);
	
	temp.Format(_T("%04.1f ml"), (float)m_pLucyDoc->GetReagentLevel(2) / 1000);
	m_ProgCtrl2.SetQuantity(temp);

	temp.Format(_T("%04.1f ml"), (float)m_pLucyDoc->GetReagentLevel(3) / 1000);
	m_ProgCtrl3.SetQuantity(temp);

	temp.Format(_T("%04.1f ml"), (float)m_pLucyDoc->GetReagentLevel(4) / 1000);
	m_ProgCtrlP.SetQuantity(temp);

	Invalidate();
}

bool CLucyBar::SetDocument(CLucyDoc* pDoc)
{
	m_pLucyDoc = pDoc;

	return true;
}

/**
 * Fix the vertical position of a text control to align it with another control. This is intended
 * to fix vertical alignment issues caused by the visual studio dialog editor.
 */
void CLucyBar::FixTextCtrl( int nID, int nReference)
{
	CWnd *pControl = GetDlgItem( nID);
	CWnd *pReference = GetDlgItem( nReference);

	ASSERT( pControl);
	ASSERT( pReference);

	CRect oClientRect;
	CRect oReferenceRect;

	pControl->GetWindowRect( &oClientRect);
	pReference->GetWindowRect( &oReferenceRect);

	int nWidth = oClientRect.Width();
	int nHeight = oClientRect.Height();

	oClientRect.top = oReferenceRect.top + (oReferenceRect.Height() - nHeight) / 2;
	oClientRect.bottom = oClientRect.top + nHeight;

	ScreenToClient( &oClientRect);

	pControl->SetWindowPos( NULL, oClientRect.left, oClientRect.top, oClientRect.Width(), oClientRect.Height(), SWP_NOZORDER | SWP_NOSIZE);
}

void CLucyBar::RegisterConductor( CConductor *pConductor)
{
	if( m_pConductor == NULL) {
		m_pConductor = pConductor;
	}
}

void CLucyBar::UnregisterConductor( CConductor *pConductor)
{
	m_pConductor = NULL;
}

void CLucyBar::OnUpdateStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pConductor != NULL);
}

void CLucyBar::OnStop() 
{
	if( m_pConductor != NULL) {
		long rv = m_pConductor->Stop();

		if (rv != KO_OK)
		{
			CString errMsg;

			errMsg.Format(IDS_STOP_RETURNS, rv);
			AfxMessageBox(errMsg);
		}
	}
}

/**
 * This is the main UI handler for asynchronous lcc exceptions.
 */
void CLucyBar::AsyncLccExceptionHandler(long exception)
{
#ifdef LCC_SOUND_ON
 	::MessageBeep(MB_ICONEXCLAMATION);
#endif

	switch( exception) {
		/* Historically, these were ignored. */
		case KO_LUM_ERR_COMM_PARITY:
		case KO_LUM_ERR_BAD_COMMAND:
			break;


		// This error is treated specially
		case KO_LUM_ERR_DOOR_NOT_OPENED:
			break;

		default:
			// *** SYSTEM LOG **********************************************************
			CLogEvent	NewEvent;							       // Create a new event   *
			CString strDetails;

			strDetails.Format( _T("Exception code 0x%08x (%s)"), exception, CLccMgr::LookupError( exception));
			NewEvent.EventSet(_T("LCC returned exception        "),// Event name           *
								strDetails);				   // Event details        *
			CSystemLog::g_oSystemLog.EventAdd( NewEvent);
			// *** SYSTEM LOG **********************************************************
			
			AfxMessageBox( strDetails);
			break;
	}
}
