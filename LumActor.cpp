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

// LumActor.cpp : implementation file
//

#include "stdafx.h"
#include "LumActor.h"
#include "LccDef.h"
#include "LccMgr.h"

#include "..\PhilUtil\PhilUtil.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const long mSecPerDay  = 1000 * 60 * 60 * 24;
const int  mSecPerHour = 1000 * 60 * 60;
const int  mSecPerMin  = 1000 * 60;
const int  mSecPerSec  = 1000;

const long NAP_TICKS = 500;	// milliseconds

/////////////////////////////////////////////////////////////////////////////
// CLumActor

IMPLEMENT_DYNCREATE(CLumActor, CWnd)

CLumActor::CLumActor()
{
	m_endTime = CTime::GetCurrentTime();
	m_pLumDialog = NULL;
	m_bMessages = true;
	m_bRegistered = false;
	m_bFindVialEndeth = false;
	m_gotData = false;
	m_actionID = LCC_ACT_UNDEF; 
}

CLumActor::~CLumActor()
{
	Release();
}

BEGIN_MESSAGE_MAP(CLumActor, CWnd)
	//{{AFX_MSG_MAP(CWnd)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

long CLumActor::Init(CWnd* pParent)
{
	if (GetSafeHwnd() != NULL)
	{
		return KO_LUC_LUMACTOR_EXISTS;
	}

	m_pLumDialog = pParent;

	if (pParent == NULL)
	{
		pParent = AfxGetMainWnd();
	}

	CRect zRect(0,0,0,0);
	if (Create(NULL,_T("WndNameLumActor"),WS_CHILD & ~WS_VISIBLE,zRect,pParent,IDD_LUMACTOR))
	{
		long rv = GetLccControl();
		return rv;
	}
	else
	{
		return KO_LUC_LUMACTOR_CREATE;
	}

};

long CLumActor::Release()
{
	long rv = KO_OK;

	if (m_bRegistered)
	{
		rv = RelinquishControl();
	}
	if (rv != KO_OK)
	{
		// Try to reset LCC
		ConnectLum();
		RelinquishControl();
	}

	if (GetSafeHwnd() != NULL)
	{
		VERIFY(DestroyWindow());
	}
	m_pLumDialog = NULL;

	return rv;
};


/////////////////////////////////////////////////////////////////////////////


long CLumActor::Action(long actionID, long par1, long par2, long timeout)
{
	long rv;
	long maxTime;
	CParam param1, param2;

	m_bDoorOpenException = false;

	m_actionID = actionID;
	
	if (!IsController())
	{
		return KO_LUC_ACT_NOT_CONTROL;
	}

//#ifdef DEBUG
	// pcw: this is surprisingly time consuming so make it a debug assertion
	long LCCStat = CSGetStatus();
	if (LCCStat != LCC_CS_CLOSED)
	{
		return KO_LCC_CS_NOT_CLOSED;
	}
//#endif	
	// Try anything more?

	// Open Action Control Session
	rv = CSOpen(LCC_CS_CLASS_ACTION, actionID);
	if (rv != KO_OK)
	{
		return rv;
	}

	// Set parameters.  
	// ! No range checking on values
	param1.m_ID = 1;
	param2.m_ID = 2;
	param1.m_value = (double)par1;
	param2.m_value = (double)par2;

	switch (actionID)
	{
	case LCC_ACT_MOVE_CHAIN:
		maxTime = LCC_ACT_MCHAIN_TIMEOUT;
		rv = CSSetParam(param1);
		if (rv != KO_OK)
		{
			break;
		}
		rv = CSSetParam(param2);
		break;

	case LCC_ACT_INJECT:		
		maxTime = LCC_ACT_INJECT_TIMEOUT;
		rv = CSSetParam(param1);
		if (rv != KO_OK)
		{
			break;
		}
		rv = CSSetParam(param2);
		break;

	case LCC_ACT_LIFT_MOVE:		
		maxTime = LCC_ACT_LIFT_TIMEOUT;
		rv = CSSetParam(param1);
		break;

	case LCC_ACT_GET_LUM:				
		maxTime = LCC_ACT_GET_LUM_MIN_TIMEOUT + par1 * 1000;
		m_gotData = false;
		rv = CSSetParam(param1);
		break;

	case LCC_ACT_DOOR_LOCK:
		maxTime = LCC_ACT_LOCK_TIMEOUT;
		rv = CSSetParam(param1);
		break;

	case LCC_ACT_SCAN_CHAIN:
		maxTime = LCC_ACT_SCHAIN_TIMEOUT;
		break;

	case LCC_ACT_PRIME:
		maxTime = LCC_ACT_PRIME_TIMEOUT;
		rv = CSSetParam(param1);
		break;

	case LCC_ACT_FINDFIRST:
		maxTime = LCC_ACT_FINDFIRST_TIMEOUT;
		m_bFindVialEndeth = false;
		rv = CSSetParam(param1);
		break;

	case LCC_ACT_DOOR:
		maxTime = LCC_ACT_DOOR_TIMEOUT;
		rv = CSSetParam(param1);
		break;

	case LCC_ACT_MOVE_CAROUSEL:
		maxTime = LCC_ACT_MOVE_CAROUSEL_TIMEOUT;
		rv = CSSetParam(param1);
		break;

	default:
		rv = KO_LUC_ACT_ID_UNKNOWN;
	}

	// If parameter setting caused errors, close session and return
	if (rv != KO_OK)
	{
		CSClose();
		return rv;
	}

	// If passed a custom timeout
	if (timeout != 0)
	{
		maxTime = timeout;
	}

	// Go
	return DoIt(maxTime);
}

///////////////////////////////////////////////////////////////////////
// Executes a control session action that has been
// opened and parameterized
///////////////////////////////////////////////////////////////////////

long CLumActor::DoIt(long timeout)
{
	long rv;

	m_sessionRV = KO_LUC_ACTION_TIMEOUT;
	m_stopPending = false;

	// Create a CTimeSpan equal to timeout
	// CTimeSpan( LONG lDays, int nHours, int nMins, int nSecs );
	long days = timeout / mSecPerDay;
	timeout %= mSecPerDay;
	int hours = timeout / mSecPerHour;
	timeout %= mSecPerHour;
	int min = timeout / mSecPerMin;
	timeout %= mSecPerMin;
	int sec = timeout / mSecPerSec + 1;
	CTimeSpan duration(days, hours, min, sec); 

	m_endTime = CTime::GetCurrentTime() + duration;
	m_sessionRV = KO_LUC_ACTION_TIMEOUT;
	rv = CSStart();
	if (rv != KO_OK)
	{
		m_sessionRV = rv;
	}

	// Disable user dialog
	if (m_pLumDialog != NULL)
	{
		m_pLumDialog->EnableWindow(false);
	}

	UINT_PTR nTimer = SetTimer( 123, 100, NULL);

	// Wait for completion, or for timeout
	RunModalLoop(MLF_NOIDLEMSG);	

	KillTimer( nTimer);

	// Re-enable user dialog
	if (m_pLumDialog != NULL && m_pLumDialog->GetSafeHwnd() != NULL)
	{
		m_pLumDialog->EnableWindow(true);
	}

	// Save the timeout, error code or success code
	rv = m_sessionRV;

	if (rv == KO_LUC_ACTION_TIMEOUT)
	{
		// Try to stop and resync
		long stopRV = CSStop(LCC_STOP_IMMEDIATE);
		CSClose();
		ConnectLum();
	}
	else
	{
		CSClose();
	}

	return rv;
}

void CLumActor::StopChain()
{
	CSStop( 0);
}


void CLumActor::OnTimer( UINT_PTR nIdEvent) 
{
	if( !ContinueModal()) {
		return;
	}

	if (m_stopPending)
	{
		CSStop(LCC_STOP_IMMEDIATE);
		m_stopPending = false;
	}

	// If seeking first vial and it has been found then shouts of joy and happiness
	if( m_bDoorOpenException == true) {
		m_sessionRV = KO_OK;  // All is well
		TRACE( "PCW: Door exception");
		EndModalLoop(0);
	} else if( m_actionID == LCC_ACT_FINDFIRST && m_bFindVialEndeth == true) {
		m_sessionRV = KO_OK;  // All is well
		TRACE( "PCW: found vial");
		EndModalLoop(0);
	} else if (CTime::GetCurrentTime() > m_endTime)
	{	// timed out.  quit modaling
		TRACE( "PCW: timeout");
		m_sessionRV = KO_LUC_ACTION_TIMEOUT;
		EndModalLoop(0);
	} else if (m_sessionRV == KO_LUC_ACTION_TIMEOUT)
	{	// AGE OLD HACK: on setup 
		//     m_sessionRV == KO_LUC_ACTION_TIMEOUT
		// so stay in the loop until it changes
		return;
	}
		
	if( m_sessionRV == KO_OK && m_actionID == LCC_ACT_GET_LUM && !m_gotData) {
		// Read lum is done but we must wait for the data to be read
	} else {	// quit modaling
		TRACE( "PCW: no reason to stay");
		EndModalLoop(0);
	}
};

void CLumActor::NewReadDataHandler(int tubeIndex, double RLU, 
		double bkgnd, long flags)
{
	// Is this is our data?
	if (IsController() && m_actionID == LCC_ACT_GET_LUM)
	{
		ASSERT(tubeIndex == 1);
		m_luminosity = RLU;
		m_dark = bkgnd;
		m_lumFlags = flags;
		m_gotData = true;
	}

};

bool CLumActor::GetLumData(double* RLU, double* bkgnd, long* flags)
{
	if (m_gotData)
	{
		*RLU = m_luminosity;
		*bkgnd = m_dark;
		*flags = m_lumFlags;
		return true;
	}
	else
	{
		return false;
	}
};

void CLumActor::CSDoneHandler(long CSStatus)
{
	// Patch bad status returned by LCC
	long correctStatus = CSStatus;
	if (m_lastAsyncError != KO_OK)
	{
		correctStatus = LCC_CS_FAULTED;
	}

	// Convert CSStatus to an exception code
	switch (correctStatus)
	{
	case LCC_CS_ENDED:				// Normal termination
		m_sessionRV = KO_OK;
		break;
	case LCC_CS_STOPPED:			// Aborted by Lucept
		m_sessionRV = KO_LCC_CS_STOPPED;
		break;
	case LCC_CS_FAULTED:			// Aborted by LCC
		m_sessionRV =  KO_LCC_CS_FAULTED;
		break;

	case LCC_CS_RUNNING: 
	case LCC_CS_STOPPING: 
		m_sessionRV =  KO_LUC_ACTION_TIMEOUT;
		break;

	case LCC_CS_OPENED:			
	case LCC_CS_CLOSED:				
	default:
		m_sessionRV =  KO_BAD_LCC_DONE_STAT;
	}

	// Call base class to post error message
	CLccUser::CSDoneHandler(CSStatus);
}

void CLumActor::FindVialEndHandler()
{
	m_bFindVialEndeth = true;
}

long CLumActor::Stop()
{
	m_stopPending = true;

	return KO_OK;
}


////////////////////////////////////////////////////////////////////////
// Get control of LCC
////////////////////////////////////////////////////////////////////////

long CLumActor::GetLccControl()
{
	long rv = KO_OK;

	// Register with LCC if necessary
	if (!m_bRegistered)
	{
		rv = RegisterWithLcc();
	}
	if (rv != KO_OK)
	{
		AfxMessageBox(IDS_NO_CONNECT_LCC);
		return rv;
	};
	m_bRegistered = true;

	// Request control if you ain't got it
	if (!IsController())
	{
		rv = RequestControl();
	}
	if (rv == KO_LCC_LUM_DISCONNECTED)
	{
		if (m_bMessages)
		{
			AfxMessageBox(IDS_LUM_OFFLINE);
		}
	}
	else if( rv == KO_LUC_REFUSE_RELEASE)
	{
		if (m_bMessages)
		{
			AfxMessageBox(IDS_NO_CONTROL_LCC);
		}
	}
	else if (rv != KO_OK)
	{
		UnregisterWithLcc();
	}

	return rv;
}

/**
 * In the special case of the error that occurs when the lcc/firmware demands the user
 * should open the door, this exception should indicate that a control sequence should end.
 * @param exception The exception code.
 */
void CLumActor::AsyncLccExceptionHandler(long exception)
{
	switch( exception) {
		// This error is treated specially
		case KO_LUM_ERR_DOOR_NOT_OPENED:
			m_bDoorOpenException = true;
			exception = KO_OK;
			break;

		default:
			// Other exceptions traditionally just get thrown up with no proper handling.
			break;
	}

	CLccUser::AsyncLccExceptionHandler( exception);
}

/**
 * Clear for launch: the C3 and advance 2 will raise exception 9999 if the door has not been opened
 * before asking for the injectors to move. The user is expected to open the door. This method implements
 * this mix of low level and user interface logic. It should be done before any set of actions.
 * @returns true if ok to run, false if user is incapable of opening the door and has cancelled.
 */
bool CLumActor::ClearForLaunch()
{
	while( true) {
		long rv;

		rv = Action(LCC_ACT_DOOR_LOCK, LCC_DOOR_UNLOCK, 0, 1000);
//		rv = Action(LCC_ACT_LIFT_MOVE, LCC_LIFT_UP, 0, 1000);

TRACE1( "CSGetStatus() = %d\n", CSGetStatus());

		switch( rv) {
			case KO_OK:
				if( m_bDoorOpenException == false) {
					return true;
				} else {
					CSStop(LCC_STOP_IMMEDIATE);
					CSClose();
					ConnectLum();

					TRACE1( "CSGetStatus() = %d\n", CSGetStatus());

					if( ::AfxMessageBox( _T("Please open the door"), MB_RETRYCANCEL) == IDCANCEL) {
						return false;
					}
				}
				break;
				
			case KO_LUC_ACTION_TIMEOUT:
				CSStop(LCC_STOP_IMMEDIATE);
				CSClose();
				ConnectLum();
				// Try to stop and resync
				if( m_bDoorOpenException == false) {
					return true;
				} else {

					if( ::AfxMessageBox( _T("Please open the door"), MB_RETRYCANCEL) == IDCANCEL) {
						return false;
					}
				}
				break;

			default:
				CString errMsg;
				errMsg.Format(_T("Unexpected error 0x%04x (%s)"), rv, CLccMgr::LookupError( rv));
				AfxMessageBox(errMsg);
				return false;
		}

// 		// You know you are up against it if the command to unlock the door can return
// 		// an error that complains that the door is not open.
// 		rv = Action(LCC_ACT_DOOR_LOCK, LCC_DOOR_UNLOCK, 0, 1000);

// TRACE1( "CSGetStatus() = %d\n", CSGetStatus());

// 		switch( rv) {
// 			case KO_OK:
// 				if( m_bDoorOpenException == false) {
// 					return true;
// 				}
// 				CSStop(LCC_STOP_IMMEDIATE);
// 				CSClose();
// 				ConnectLum();
// 				break;

// 			case KO_LUC_ACTION_TIMEOUT:
// 				// Try to stop and resync
// 				CSStop(LCC_STOP_IMMEDIATE);
// 				CSClose();
// 				ConnectLum();
// 				break;

// 			default:
// 				CString errMsg;
// 				errMsg.Format(_T("Unexpected error 0x%04x (%s)"), rv, CLccMgr::LookupError( rv));
// 				AfxMessageBox(errMsg);
// 				return false;
// 		}
	}
}
