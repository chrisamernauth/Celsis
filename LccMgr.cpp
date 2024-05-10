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

// LccMgr.cpp : implementation of the CLccMgr class
//

#include "stdafx.h"
#include "Lucept.h"
#include "LccMgr.h"
#include "MainFrm.h"
#include "LuceErr.h"
#include "LucDef.h"
#include "LccDef.h"
#include "LucOle.h"
#include "resource.h"
#include "LccView.h"
#include "LccVDoc.h"
#include "IniFiles.h"
#include "Event.h"
#include "SystemLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int BUSY_NOT				= 0;
const int BUSY_INITIAL			= 1;
const int BUSY_DISCONNECT		= 2;
const int BUSY_DONE				= 3;
const int BUSY_OPEN				= 4;
const int BUSY_SYNC				= 5;
const int BUSY_START			= 6;
const int BUSY_CLOSE			= 7;
const int BUSY_SET_PAR			= 8;
const int BUSY_GET_LAST_DATA	= 9;
const int BUSY_STATUS			= 10;
const int BUSY_GET_NUM_READ_PROC= 11;
const int BUSY_GET_NUM_PAR		= 12;
const int BUSY_PAR_NAME			= 13;
const int BUSY_PAR_DEFAULT		= 14;
const int BUSY_PAR_TYPE			= 15;
const int BUSY_PAR_MAX			= 16;
const int BUSY_PAR_MIN			= 17;
const int BUSY_INQUIRE			= 18;
const int BUSY_GET_NUM_ACT		= 19;
const int BUSY_GET_NUM_CLASS	= 20;
const int BUSY_GET_CLASS_NAME	= 21;
const int BUSY_SHOW				= 22;
const int BUSY_REAGENT			= 23;
const int BUSY_TIME				= 24;
const int BUSY_GET_DATA			= 25;

#undef LCC_CONN_DEBUG

bool CLccMgr::g_bSysLog = false;


// Select null or active macro depending on whether messages are
// to be enabled
#define DIAG_OUT

#ifndef DIAG_OUT
	#define SHOW_DIAG(string)
#else
void SHOW_DIAG(LPCTSTR string)									
{		
	static int	lumMsgNum = 0;
	CString		lumMsg;


	// Empty string is interpretted as a carriage return
	if (_tcslen(string) == 0)	
	{
		lumMsg = _T("\r\n");										
	}
	else													
	{														
		lumMsg.Format(_T("%04d:  "), lumMsgNum++);			
		lumMsg += string;									
		lumMsg += _T("\r\n");									
	}
	
#ifdef _DEBUG			
	TRACE(lumMsg);									
#endif 
	if( CLccMgr::g_bSysLog) {
		CSystemLog::g_oSystemLog.EventAdd( _T( "LCC"), lumMsg);
	}

	// If an LCC dialog window exits...
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();			
	if (pApp->m_pLccView != NULL)							
	{
		CLccView* pView = pApp->m_pLccView;
		// Append message to existing text
		CEdit& editCtrl = pView->GetEditCtrl();	
		CString currentText;								
		editCtrl.GetWindowText(currentText);
		currentText += lumMsg;		
		editCtrl.SetWindowText(currentText);	
		
		// Scroll window to end
		editCtrl.LineScroll(editCtrl.GetLineCount());
	}														

	return;
}
#endif

	
#ifndef DIAG_OUT
	#define SHOW_INQUIRE(inquireID, pResult)
#else

static TCHAR *g_strInquires[] = {
	_T( "LCC_INQ_UNDEFINED"),
	_T( "LCC_INQ_LCCSTAT"),
	_T( "LCC_INQ_LUMSTAT"),
	_T( "LCC_INQ_FIRMREV"),
	_T( "LCC_INQ_TEMP"),
	_T( "LCC_INQ_CHAINPOS"),
	_T( "LCC_INQ_DOORLOCK"),
	_T( "LCC_INQ_PROTOCOL_CT"),
	_T( "LCC_INQ_PROTOCOL_ID"),
	_T( "LCC_INQ_PROTOCOL_NM"),
	_T( "LCC_INQ_INSTRUMENT_ID"),
};

void ShowInquire( long inquireID, double *pResult = NULL)
{
	CString jk;

	if( inquireID >= 0 && inquireID < sizeof( g_strInquires)/ sizeof( g_strInquires[0])) {
		if( pResult != NULL) {
			if( inquireID != LCC_INQ_LUMSTAT) {
				jk.Format(_T("Inquire( %s) = %g"), g_strInquires[inquireID], *pResult);
			} else {
				jk.Format(_T("Inquire( %s) = %08x"), g_strInquires[inquireID], (long)*pResult);
			}
		} else {
			jk.Format(_T("Inquire( %s)"), g_strInquires[inquireID]);
		}

	} else {
		jk.Format(_T("Inquire( %d)"), inquireID);
	}

	SHOW_DIAG(jk);
}

	#define SHOW_INQUIRE(inquireID, pResult) ShowInquire( inquireID, pResult)

#endif

#ifndef DIAG_OUT
	#define SHOW_INQ_TEMP()
#else
void SHOW_INQ_TEMP()
{
	// If an LCC dialog window exits...
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();			
	if (pApp->m_pLccView != NULL)							
	{
		CLccView* pView = pApp->m_pLccView;

		// Show inquire status is alive
		// This bodge seems to avoid filling the screen with 'inquire temperature' messages and flashes an indicator instead
		CEdit& editCtrl = pView->GetEditCtrl();	
		CString currentText;								
		editCtrl.GetWindowText(currentText);
		if (currentText.GetLength() < 3)
		{
			currentText += _T("Inquire Temperature:  ?\r\n");
		}
		switch(currentText[currentText.GetLength() - 3])
		{
		case '?':
			currentText.SetAt(currentText.GetLength() - 3, '*');
			break;
		case '*':
			currentText.SetAt(currentText.GetLength() - 3, '?');
			break;
		case '\n':
		default:
			currentText += _T("Inquire Temperature:  ?\r\n");
		}
		editCtrl.SetWindowText(currentText);	
		
		// Scroll window to end
		editCtrl.LineScroll(editCtrl.GetLineCount());

	}

	if( CLccMgr::g_bSysLog) {
		CSystemLog::g_oSystemLog.EventAdd( _T( "LCC"), _T(" Inquire Temperature"));
	}
	return;
}
#endif

#ifndef DIAG_OUT
	#define SHOW_GET_STATUS()
#else
	#define SHOW_GET_STATUS() SHOW_DIAG(_T("CSGetStatus()"))
#endif

#ifndef DIAG_OUT
	#define SHOW_STATUS(status)
#else
void SHOW_STATUS(int status)
{									
	switch (status)
	{
	case LCC_CS_OPENED:
		SHOW_DIAG(_T("Control session is OPENED"));
		break;
	case LCC_CS_RUNNING:
		SHOW_DIAG(_T("Control session is RUNNING"));
		break;
	case LCC_CS_ENDED:
		SHOW_DIAG(_T("Control session is ENDED"));
		break;
	case LCC_CS_STOPPING:
		SHOW_DIAG(_T("Control session is STOPPING"));
		break;
	case LCC_CS_STOPPED:
		SHOW_DIAG(_T("Control session is STOPPED"));
		break;
	case LCC_CS_FAULTED:
		SHOW_DIAG(_T("Control session is FAULTED"));
		break;
	case LCC_CS_CLOSED:
		SHOW_DIAG(_T("Control session is CLOSED"));
		break;
	default:
		SHOW_DIAG(_T("Control session status is invalid!"));
	}							
}		
#endif



#ifndef DIAG_OUT
	#define SHOW_EXCEPT(fmtstring)
#else
	#define SHOW_EXCEPT(exception)								\
	{															\
		CString jk;												\
		jk.Format(IDS_LCC_EXCEPTION, exception, CLccMgr::LookupError( exception));				\
		SHOW_DIAG(jk);											\
	}
#endif
	
#ifndef DIAG_OUT
	#define SHOW_DONE_EVENT()
#else
	#define SHOW_DONE_EVENT()	\
		SHOW_DIAG(_T("Received Control Session Done Event"));
#endif

#ifndef DIAG_OUT
	#define SHOW_DATA_EVENT()
#else
	#define SHOW_DATA_EVENT()								\
	{														\
		SHOW_DIAG(_T(""));										\
		SHOW_DIAG(_T("Received New Data Event"));				\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_EVENT_MESSAGE(string)
#else
	#define SHOW_EVENT_MESSAGE(string)
#endif

#ifndef DIAG_OUT
	#define SHOW_OPEN(classID, procedureID)
#else

static const TCHAR *g_strProcedures[] = {
	_T("LCC_UNDERFINED_PROCEDURE_0"),
	_T("LCC_ACT_SCAN_CHAIN"),
	_T("LCC_ACT_MOVE_CHAIN"),
	_T("LCC_ACT_INJECT"),
	_T("LCC_ACT_LIFT_MOVE"),
	_T("LCC_ACT_GET_LUM"),
	_T("LCC_ACT_DOOR_LOCK"),
	_T("LCC_ACT_PRIME"),
	_T("LCC_ACT_FINDFIRST"),
	_T("LCC_ACT_DOOR"),
	_T("LCC_ACT_MOVE_CAROUSEL")
};

static void ShowOpen( long classID, long procedureID)						\
{
	CString jk;
	SHOW_DIAG(_T(""));
	if( procedureID >= 0 && procedureID < sizeof( g_strProcedures)/ sizeof( g_strProcedures[0])) {
		jk.Format(_T("CSOpen(%d, %s)"), classID, g_strProcedures[procedureID]);
		SHOW_DIAG(jk);
	} else {
		jk.Format(_T("CSOpen(%d, %d)"), classID, procedureID);
		SHOW_DIAG(jk);
	}
}
	#define SHOW_OPEN(classID, procedureID) ShowOpen( classID, procedureID)

#endif

#ifndef DIAG_OUT
	#define SHOW_SYNC()
#else
	#define SHOW_SYNC() SHOW_DIAG(_T("CSSync()"));
#endif

#ifndef DIAG_OUT
	#define SHOW_START()
#else
	#define SHOW_START() SHOW_DIAG(_T("CSStart()"));
#endif

#ifndef DIAG_OUT
	#define SHOW_STOP(mode) 
#else
	#define SHOW_STOP(mode)										\
	{															\
		CString jk;												\
		jk.Format(_T("CSStop(%d)"), mode);							\
		SHOW_DIAG(jk);											\
	}				
#endif

#ifndef DIAG_OUT
	#define SHOW_CLOSE()
#else
	#define SHOW_CLOSE() SHOW_DIAG(_T("CSClose()"));				\
			SHOW_DIAG(_T(""));
#endif
		
#ifndef DIAG_OUT
	#define SHOW_GET_PROC_LAST(classID)
#else
	#define SHOW_GET_PROC_LAST(classID)							\
	{															\
		CString jk;												\
		jk.Format(_T("GetProcLast(%d)"), classID);					\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_GET_PROC_NAME(classID, procID)
#else
	#define SHOW_GET_PROC_NAME(classID, procID)					\
	{															\
		CString jk;												\
		jk.Format(_T("GetProcName(%d, %d)"), classID, procID);		\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_PROC_NAME(name)
#else
	#define SHOW_PROC_NAME(name)								\
	{															\
		CString jk;												\
		jk = _T("Procedure name is:  ") + name;						\
		SHOW_DIAG(jk);											\
	}
#endif


#ifndef DIAG_OUT
	#define SHOW_GET_PARAM_LAST(classID, procID)
#else
	#define SHOW_GET_PARAM_LAST(classID, procID)				\
	{															\
		CString jk;												\
		jk.Format(_T("GetParamLast(%d, %d)"), classID, procID);		\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_GET_PARAM_NAME(classID, procID, paramID)
#else
	#define SHOW_GET_PARAM_NAME(classID, procID, paramID)		\
	{															\
		CString jk;												\
		jk.Format(_T("GetParamName(%d, %d, %d)"), classID, procID, paramID);	\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_PARAM_NAME(name)
#else
	#define SHOW_PARAM_NAME(name)								\
	{															\
		CString jk;												\
		jk = _T("Parameter name is:  ") + name;						\
		SHOW_DIAG(jk);											\
	}
#endif


#ifndef DIAG_OUT
	#define SHOW_GET_PARAM_NOM(classID, procID, paramID)
#else
	#define SHOW_GET_PARAM_NOM(classID, procID, paramID)		\
	{															\
		CString jk;												\
		jk.Format(_T("GetParamNomVal(%d, %d, %d)"), classID, procID, paramID);	\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_GET_PARAM_MAX(classID, procID, paramID)
#else
	#define SHOW_GET_PARAM_MAX(classID, procID, paramID)		\
	{															\
		CString jk;												\
		jk.Format(_T("GetParamMaxVal(%d, %d, %d)"), classID, procID, paramID);	\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_GET_PARAM_MIN(classID, procID, paramID)
#else
	#define SHOW_GET_PARAM_MIN(classID, procID, paramID)		\
	{															\
		CString jk;												\
		jk.Format(_T("GetParamMinVal(%d, %d, %d)"), classID, procID, paramID);	\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_GET_PARAM_TYPE(classID, procID, paramID)
#else
	#define SHOW_GET_PARAM_TYPE(classID, procID, paramID)		\
	{															\
		CString jk;												\
		jk.Format(_T("GetParamType(%d, %d, %d)"), classID, procID, paramID);	\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_SET_PAR(paramID, val)
#else
	#define SHOW_SET_PAR(paramID, val)							\
	{															\
		CString jk;												\
		jk.Format(_T("CSSetParam(%d, %g)"), paramID, val);			\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_GET_DATA(array, index)
#else
	#define SHOW_GET_DATA(array, index)							\
	{															\
		CString jk;												\
		jk.Format(_T("CSGetData(%d, %d)"), array, index);			\
		SHOW_DIAG(jk);											\
	}
#endif
					
#ifndef DIAG_OUT
	#define SHOW_LAST_TUBE(index)
#else
	#define SHOW_LAST_TUBE(index)								\
	{															\
		CString jk;												\
		jk.Format(_T("Data is available for %d tubes"), index);		\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_TUBE_RLU(iTube, RLU)
#else
	#define SHOW_TUBE_RLU(iTube, RLU)							\
	{															\
		CString jk;												\
		jk.Format(_T("Tube #%d's RLU = %g"), iTube, RLU);		\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_TUBE_BKG(iTube, BKG)
#else
	#define SHOW_TUBE_BKG(iTube, BKG)							\
	{															\
		CString jk;												\
		jk.Format(_T("Tube #%d's background = %g"), iTube, BKG);	\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_TUBE_FLAGS(iTube, flags)
#else
	#define SHOW_TUBE_FLAGS(iTube, flags)						\
	{															\
		CString jk;												\
		jk.Format(_T("Tube #%d's flags = 0x%08X"), iTube, flags);	\
		SHOW_DIAG(jk);											\
	}
#endif

									
#ifndef DIAG_OUT
	#define SHOW_GET_TIME( nMilliSeconds)
#else
	#define SHOW_GET_TIME( nMilliSeconds)						\
	{															\
		CString jk;												\
		jk.Format(_T("CSGetTimeLeft() = %dms"), nMilliSeconds);	\
		SHOW_DIAG(jk);											\
	}
#endif

#ifndef DIAG_OUT
	#define SHOW_SHOW()
#else
	#define SHOW_SHOW() SHOW_DIAG(_T("ShowWindow()"));
#endif

#ifndef DIAG_OUT
	#define CALL_OUT_MESSAGE(fmtstring)
#else
	#define CALL_OUT_MESSAGE(fmtstring) SHOW_DIAG(fmtstring);
#endif

// No _T("Calling Out").  Check code to be included in all routines
// which call LCC.
long CLccMgr::LccBusyCheck()
{
	if( m_busyStatus != BUSY_NOT)
	{
		CString jk;
		jk.Format(_T("You called out while busy %d!"), m_busyStatus);
		SHOW_DIAG(jk);
		ASSERT(m_busyStatus == BUSY_NOT);
		return KO_LUC_LCC_IN_CALL;
	}

	return KO_OK;
}

// #define LCC_SOUND_ON
#undef LCC_SOUND_ON

const int LCCMGR_RETRY_CONNECT = 10;		// Zero value disables

////////////////////////////////////////////////////////////////////////
// CLccMgr construction/destruction
////////////////////////////////////////////////////////////////////////

CLccMgr::CLccMgr()
{
	m_sink.RegisterMgr(this);
	m_cookie = 0;
	
	m_pController = NULL;
	m_classID = LUC_ID_UNDEF;
	m_procedureID = LUC_ID_UNDEF;
	m_lastStatus = LCC_CS_CLOSED;
	m_busyStatus = BUSY_INITIAL;				// busy until connected
	m_bLumConnected = false;
	m_cReadsPending = 0;
	m_IsRelinquishing = false;
	m_IsClosing = false;
	m_IsDonePending = false;
	m_changeOfStatusPending = false;
	m_IsStoppingMode = LCC_STOP_UNDEF;
	m_lastAsyncErr = KO_OK;

	// Determine whether lcc comms goes into system log
	g_bSysLog = ((CLuceptApp*)AfxGetApp())->GetLuceptProfileInt( _T("Features"), _T("LCCSyslog"), 0) ? true : false;
}

CLccMgr::~CLccMgr()
{
}


////////////////////////////////////////////////////////////////////////
// CLccMgr OLE connection management
////////////////////////////////////////////////////////////////////////

bool CLccMgr::ConnectLcc()
{
	// Flush any lingering LCC's from the system
	if (!BuryDeadLcc())
	{
		return false;
	}

	// Start Lcc server and connect 
	if (m_Lcc.CreateDispatch(IID_ILcc))
	{
		// Connect events to Lcc
		ConnectEvents();
		if (m_cookie != 0)
		{
#ifdef LCC_SOUND_ON
			::MessageBeep(MB_OK);
#endif
			m_busyStatus = BUSY_NOT;			// Ready to take requests

			// Check if luminometer is online
			double LCCStat;
			Inquire(LCC_INQ_LCCSTAT, &LCCStat);
			m_bLumConnected = LCCStat > 0.0;

			return true;
		}
		else
		{
			AfxMessageBox(_T("Can't connect events to Lcc"));
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool CLccMgr::DisconnectLcc()
{
	m_busyStatus = BUSY_DISCONNECT;

	// End control session
	// OnMeasurementStop();  
	TRY {
		m_Lcc.CSStop(LCC_STOP_IMMEDIATE);  // Suppress error reporting
		m_Lcc.CSClose();
	} CATCH_ALL(e) {
		TRACE( "Swallowing exception when closing lcc");
	}
	END_CATCH_ALL;

	// Unadvise connection point
	DisconnectEvents();

#ifdef timedkill
	DWORD nThreadId;
	HANDLE hThread = CreateThread( NULL, // pointer to security attributes
									0,		// initial thread stack size
									(LPTHREAD_START_ROUTINE) CLccMgr::StartTimedRelease,     
											// pointer to thread function
									(LPVOID)this,				                        
											// argument for new thread
									0,      // creation flags
									&nThreadId); 
											// pointer to receive thread ID

	if( WaitForSingleObject( hThread, 10000) == WAIT_TIMEOUT) {
		TheTerminator();
		m_Lcc.m_lpDispatch = NULL;
	}

	CloseHandle( hThread);

#else
	m_Lcc.ReleaseDispatch();
#endif

	m_busyStatus = BUSY_NOT;

	return true;
}

void CLccMgr::StartTimedRelease( LPVOID pClass)
{
	((CLccMgr *)pClass)->TimedRelease();
}

void CLccMgr::TimedRelease( void)
{
	m_Lcc.ReleaseDispatch();
}

///////////////////////////////////////////////////////////////////////
// Flush any lingering LCC's from memory
///////////////////////////////////////////////////////////////////////

bool CLccMgr::BuryDeadLcc()
{
	bool rv = true;
	int bodyCount = 0;

	// Get interface pointer to Running Object Table
	LPRUNNINGOBJECTTABLE ipRot;

	if (SUCCEEDED(GetRunningObjectTable(NULL, &ipRot)))
	{
		LPENUMMONIKER ipEnumMoniker;

		// Get Moniker Enumerator interface from ROT to iterate all the
		// running objects
		if (SUCCEEDED(ipRot->EnumRunning(&ipEnumMoniker)))
		{
			LPMONIKER ipRunningMoniker;
			
			// Check all running objects
			while(ipEnumMoniker->Next(1, &ipRunningMoniker, NULL) == S_OK)
			{
				LPUNKNOWN ipUnk;
				
				// Bind to object named
				if (SUCCEEDED(ipRot->GetObject(ipRunningMoniker, &ipUnk)))
				{
					LPCONNECTIONPOINT ipConnPt;

					// Check for appropriate connection point
					if (GetLccConnPt(&ipConnPt, ipUnk))
					{	//if we find it, let Gerhard kill it
						//if he fails, squeal
						if(!(rv = TheTerminator()))
						{
							AfxMessageBox(IDS_DEAD_LCC);
						}												
						ipConnPt->Release();
					}
					ipUnk->Release();
				}
				ipRunningMoniker->Release();
			}
			ipEnumMoniker->Release();
		}
		ipRot->Release();
	}

	return rv;
}


///////////////////////////////////////////////////////////////////////
// Pass event sink pointers to Lcc
///////////////////////////////////////////////////////////////////////

void CLccMgr::ConnectEvents() 
{
	LPCONNECTIONPOINT ipConnPt;

	if (m_cookie != 0)
	{
		AfxMessageBox(_T("Already connected."));
		return;
	}

	if(GetLccConnPt(&ipConnPt))
	{	// Connect 
		if (FAILED(ipConnPt->Advise(m_sink.GetInterface(&IID_IUnknown), &m_cookie)))
		{
			m_cookie = 0;
		}
		// When done, connection point must be Unadvised with cookie
		ipConnPt->Release();
	}

	if (m_cookie != 0)
	{	// Don't end program while interface pointer is held
		AfxOleLockApp();
	}
}


///////////////////////////////////////////////////////////////////////
// Disconnect Lcc's event sources
///////////////////////////////////////////////////////////////////////

void CLccMgr::DisconnectEvents()
{
	LPCONNECTIONPOINT ipConnPt;

	if (m_cookie == 0)
	{	// Not connected
		return;
	}

	if(GetLccConnPt(&ipConnPt))
	{
		if (FAILED(ipConnPt->Unadvise(m_cookie)))
		{
			AfxMessageBox(_T("Failure to Unadvise connection point"));
		}

		ipConnPt->Release();
	}
		
	// Unlock App either way
	m_cookie = 0;
	AfxOleUnlockApp();
}

///////////////////////////////////////////////////////////////////////
// Gets pointer to the LCC's connection point interface
///////////////////////////////////////////////////////////////////////

bool CLccMgr::GetLccConnPt(LPCONNECTIONPOINT* pipConnPt, LPUNKNOWN ipUnk)
{
	bool rv = false;
	HRESULT result;
	LPCONNECTIONPOINTCONTAINER ipConnPtCont;

	// Get Connection Point Container Interface
	if (ipUnk == NULL)
	{	// Get interface from m_Lcc
		result = m_Lcc.m_lpDispatch->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&ipConnPtCont);
	}
	else
	{	// Get interface from passed IUnknown
		result = ipUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&ipConnPtCont);
	}

	if (SUCCEEDED(result))
	{	// Get connection point for IID_ILccEventSink. 
		if (SUCCEEDED(ipConnPtCont->FindConnectionPoint(IID_ILCCEventSink, pipConnPt)))
		{
			rv = true;
		}
		ipConnPtCont->Release();
	}

	return rv;
}


////////////////////////////////////////////////////////////////////////
// CLccMgr Events
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Asynchronous Exception Event
////////////////////////////////////////////////////////////////////////

void CLccMgr::OnLccException(long exception) 
{
	// Inform users
	m_changeOfStatusPending = true;

	if( exception != KO_LUM_ERR_DOOR_NOT_OPENED) {
		m_lastAsyncErr = exception;
	}

	SHOW_EXCEPT(exception);

	// Inform users of exception
	POSITION pos = m_userList.GetHeadPosition();
	while (pos != NULL)
	{
		m_userList.GetNext(pos)->AsyncLccExceptionHandler(exception);
	}
}



///////////////////////////////////////////////////////////////////
// End of control session event
////////////////////////////////////////////////////////////////////////

void CLccMgr::OnLccCSDone() 
{
#ifdef LCC_SOUND_ON
	::MessageBeep(MB_ICONASTERISK);
#endif
	
	SHOW_DONE_EVENT();

	if (m_classID == LCC_CS_CLASS_ACTION)
	{
		m_IsDonePending = true;

		// The controlling user will get notified immediately
		// Make sure the LCC is returning reasonable statuses
		if(m_busyStatus != BUSY_NOT)		
		{									
			CString jk;						
			jk.Format(_T("You called out while busy %d!"), m_busyStatus);	
			SHOW_DIAG(jk);		
		}

	}
	else if (m_classID == LCC_CS_CLASS_READ)
	{
		// The controlling user will get a notice of the end of 
		// control session after all pending reads have been cleared.
		// This is handled by OnTick().
		m_IsDonePending = true;
	}
}


////////////////////////////////////////////////////////////////////////
// Data available Event
////////////////////////////////////////////////////////////////////////

void CLccMgr::OnLccNewData() 
{
#ifdef LCC_SOUND_ON
	::MessageBeep(MB_OK);
#endif

	SHOW_DATA_EVENT();

	m_cReadsPending++;
}


////////////////////////////////////////////////////////////////////////
// Control Session message Event
////////////////////////////////////////////////////////////////////////

void CLccMgr::OnLccMsg(LPCTSTR pText) 
{
	SHOW_EVENT_MESSAGE( pText);

	m_lccMsgPending = pText;
}

////////////////////////////////////////////////////////////////////////
// FindVial has finished
////////////////////////////////////////////////////////////////////////

void CLccMgr::OnFindVialDone() 
{
	SHOW_EVENT_MESSAGE( pText);

	if( m_pController != NULL) {
		m_pController->FindVialEndHandler();
	}
}

////////////////////////////////////////////////////////////////////////
// User Registration
////////////////////////////////////////////////////////////////////////

long CLccMgr::RegisterUser(CLccUser * pUser)
{
	// Check if already registered
	POSITION pos = m_userList.GetHeadPosition();

	while (pos != NULL)
	{
		if	(m_userList.GetNext(pos) == pUser)
		{
			return KO_OK;
		}
	}

	// Else add the pointer to the list
	m_userList.AddTail(pUser);

	return KO_OK;
}

long CLccMgr::UnregisterUser(CLccUser * pUser)
{
	POSITION pos = m_userList.GetHeadPosition();
	POSITION nextPos;

	while (pos != NULL)
	{
		nextPos = pos;
		if	(m_userList.GetNext(nextPos) == pUser)
		{
			m_userList.RemoveAt(pos);
		}
		pos = nextPos;
	}

	return KO_OK;
}

long CLccMgr::RequestControl(CLccUser * pRequestor)
{
	long rv = KO_OK;
	
	// Must be registered user
	bool isUser = false;
	POSITION pos = m_userList.GetHeadPosition();
	while (pos != NULL && !isUser)
	{
		if	(m_userList.GetNext(pos) == pRequestor)
		{
			isUser = true;
		}
	}
	if (!isUser)
	{
		return KO_LUC_NOT_LCC_USER;
	}

	if (m_pController != NULL)
	{	// Ask current controller to release
		rv = m_pController->ReqReleaseControl();
		if (rv != KO_OK)
		{
			return rv;
		}
	}

	// Check if Luminometer is online.  
	if (!m_bLumConnected)
	{
		rv = Sync(pRequestor);
		if (rv != KO_OK)
		{
			return rv;
		}

		// Check again
		double LccStat;
		rv = Inquire(LCC_INQ_LCCSTAT, &LccStat);
		if (rv != KO_OK)
		{
			return rv;
		}
		if (LccStat == 0.0)
		{
			return KO_LCC_LUM_DISCONNECTED;
		}
	}

	// Control session status must be Closed 
	if (m_lastStatus != LCC_CS_CLOSED)
	{
		return KO_LCC_CS_NOT_CLOSED;
	}
	
	// Make it so
	m_pController = pRequestor;

	return rv;
}


long CLccMgr::RelinquishControl(CLccUser* pUser)
{
	if (pUser != m_pController)
	{
		return KO_LUC_NOT_CONTROLLER2;
	}

	m_IsRelinquishing = true;

	// Control session must be shut down
	long rv = ReportCSStatus();
	if (rv != KO_OK)
	{
		return rv;
	}

	if (m_lastStatus != LCC_CS_CLOSED)
	{
		if (!m_IsClosing)
		{
			return KO_LUC_CS_STILL_ACTIVE;
		}
		else
		{
			return KO_OK;
		}
	}

	// Control session is closed
	CLccUser* pRelinquisher = m_pController;
	m_pController = NULL;
	ReportAvailable(pRelinquisher);
	m_IsRelinquishing = false;
	
	return KO_OK;
}

bool CLccMgr::IsController(CLccUser * pUser)
{
	return (m_pController != NULL && m_pController == pUser);
}

bool CLccMgr::IsLccAvailable()
{
	return m_pController == NULL;
}


////////////////////////////////////////////////////////////////////////
// Controller Actions
////////////////////////////////////////////////////////////////////////

long CLccMgr::CSOpen(CLccUser* pUser, long classID, long procedureID)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	if (!IsController(pUser))
	{
		return KO_LUC_NOT_CONTROLLER;
	}

	SHOW_OPEN(classID, procedureID);

	m_busyStatus = BUSY_OPEN;
	long rv = m_Lcc.CSOpen(classID, procedureID);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK)
	{
		m_classID = classID;
		m_procedureID = procedureID;
	}

	m_changeOfStatusPending = true;

	return rv;
}


long CLccMgr::Sync(CLccUser* pUser)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	long rv;

	// If there is an active controller, only it may sync.
	// Otherwise, anyone can 
	if (!IsController(pUser) && !IsLccAvailable())
	{
		return KO_LUC_NOT_CONTROLLER;
	}

	// Clear any previous errors
	m_lastAsyncErr = KO_OK;

	SHOW_SYNC();

	m_busyStatus = BUSY_SYNC;
	rv = m_Lcc.Synch();
	m_busyStatus = BUSY_NOT;
	if (rv != KO_OK)
	{	// Should not return any errors (?)
		CString jk; 
		jk.Format(_T("LCC Synch() returns error 0x%X"), rv);
		AfxMessageBox(jk);
	}

	// Check for any change in CSStatus
	m_changeOfStatusPending = true;

	return rv;
};

long  CLccMgr::CSStart(CLccUser* pUser)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	if (!IsController(pUser))
	{
		return KO_LUC_NOT_CONTROLLER;
	}

	SHOW_START();

	m_busyStatus = BUSY_START;
	long rv = m_Lcc.CSStart();
	m_busyStatus = BUSY_NOT;

	m_changeOfStatusPending = true;

	return rv;
}

long CLccMgr::CSStop(CLccUser * pUser, long mode)
{
	// Log and honor even badly timed calls to CSStop!
	if (!IsController(pUser))
	{
		return KO_LUC_NOT_CONTROLLER;
	}
	m_IsStoppingMode = mode;	

	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	SHOW_STOP(mode);

	m_busyStatus = BUSY_START;
	long rv = m_Lcc.CSStop(mode);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK || rv == KO_LCC_CS_NOT_STARTED)
	{
		m_IsStoppingMode = LCC_STOP_UNDEF;
	}

	m_changeOfStatusPending = true;

	return rv;
}

long CLccMgr::CSClose(CLccUser * pUser)
{
	// Log and honor even badly timed calls to CSClose!
	if (!IsController(pUser))
	{
		return KO_LUC_NOT_CONTROLLER;
	}
	m_IsClosing = true;
	
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	if (m_cReadsPending > 0)
	{
		return KO_LUC_DATA_PENDING;
	}

	SHOW_CLOSE();

	m_busyStatus = BUSY_CLOSE;
	long rv = m_Lcc.CSClose();
	m_busyStatus = BUSY_NOT;

	if (rv == KO_OK || rv == KO_LCC_CS_NOT_OPEN)
	{
		m_classID = LUC_ID_UNDEF;
		m_procedureID = LUC_ID_UNDEF;
		m_IsClosing = false;
		m_IsStoppingMode = LCC_STOP_UNDEF;
	}

	m_changeOfStatusPending = true;

	return rv;
}

long CLccMgr::CSSetParam(CLccUser* pUser, long paramID, COleVariant& val)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	if (!IsController(pUser))
	{
		return KO_LUC_NOT_CONTROLLER;
	}

	SHOW_SET_PAR(paramID, val.dblVal);

	m_busyStatus = BUSY_SET_PAR;
	long rv = m_Lcc.CSSetParam(paramID, LPVARIANT(val));
	m_busyStatus = BUSY_NOT;

	return rv;
}

////////////////////////////////////////////////////////////////////////
// Inquiries
////////////////////////////////////////////////////////////////////////

long CLccMgr::CSGetStatus()
{
	ReportCSStatus();

	return m_lastStatus;
};


long CLccMgr::GetIndexLastData(int* index)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	long rv;
    COleVariant var;

	// Get index of last available tube data
	SHOW_GET_DATA(LCC_DARRAY_GEN, LCC_IDX_MAXDATA);

	m_busyStatus = BUSY_GET_LAST_DATA;
	rv = m_Lcc.CSGetData(LCC_DARRAY_GEN, LCC_IDX_MAXDATA, var);
	m_busyStatus = BUSY_NOT;
	if (rv != KO_OK)
	{
		return rv;
	}

	ASSERT(var.vt == VT_R8);
	*index = (int)var.dblVal;

	SHOW_LAST_TUBE(*index);

	return KO_OK;
}


long CLccMgr::GetTubeData(int iTube, double* RLU, double* bkgnd, long* flags)
{
	long rv;

	if (iTube <= 0)
	{
		return KO_LUC_BAD_TUBE_INDEX;
	}

	// Get check index of last available tube data
	int maxIndex;
	rv = GetIndexLastData(&maxIndex);
	if (rv != KO_OK)
	{
		return rv;
	}
	if (iTube > maxIndex)
	{
		return KO_LUC_TUBE_DATA_UNAVAIL;
	}

	// Get RLU Value
	rv = CSGetData(LCC_DARRAY_LUM, iTube, RLU);
	if (rv != KO_OK)
	{
		return rv;
	}

	SHOW_TUBE_RLU(iTube, *RLU);

	// Get Background Value
	rv = CSGetData(LCC_DARRAY_BKG, iTube, bkgnd);
	if (rv != KO_OK)
	{
		return rv;
	}

	SHOW_TUBE_BKG(iTube, *bkgnd);

	return rv;
}


////////////////////////////////////////////////////////////////////////
// Support functions
////////////////////////////////////////////////////////////////////////

long CLccMgr::ReportCSStatus()
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	// Check if luminometer is actually online
	double LCCStat;
	VERIFY(Inquire(LCC_INQ_LCCSTAT, &LCCStat) == KO_OK);
	if (m_bLumConnected != (LCCStat > 0.0))
	{	// Lum connection status has changed
		m_bLumConnected = LCCStat > 0.0;
		// Inform the users
		POSITION pos = m_userList.GetHeadPosition();
		while (pos != NULL)
		{
			m_userList.GetNext(pos)->LumConnectHandler(m_bLumConnected);
		}
	}
	
	long CSstatus;

	SHOW_GET_STATUS();

	m_busyStatus = BUSY_STATUS;
	CSstatus = m_Lcc.CSGetStatus();
	m_busyStatus = BUSY_NOT;

	SHOW_STATUS(CSstatus);

	// Only reports if status has changed since last report
	if (CSstatus == m_lastStatus)
	{
		return KO_OK;
	}
	else
	{
		m_lastStatus = CSstatus;
	}

	// Attempt to close control session with unread data
	if (m_lastStatus == LCC_CS_CLOSED && m_cReadsPending > 0)
	{
		CString errMsg;

		errMsg.Format(_T("LCC closed with %d tubes of unread data."),m_cReadsPending);
		AfxMessageBox(errMsg);
	}

	// Send new status to all users
	POSITION pos = m_userList.GetHeadPosition();
	while (pos != NULL)
	{
		m_userList.GetNext(pos)->CSStatusHandler(m_lastStatus, m_classID, m_procedureID);
	}

	return KO_OK;
}

void CLccMgr::ReportAvailable(CLccUser* pRelinquisher)
{
	POSITION pos = m_userList.GetHeadPosition();

	while (pos != NULL)
	{
		CLccUser* pUser = m_userList.GetNext(pos);

		if (pUser != pRelinquisher)
		{
			pUser->LccAvailableHandler();
		}
	}
}

void CLccMgr::ErrorReport(long rv)
{
	if (rv != KO_OK)
	{
		CString errMsg;
		errMsg.Format(_T("LCC returned exception 0x%X"), rv);
		AfxMessageBox(errMsg);
	}
}


////////////////////////////////////////////////////////////////////////
// Procedure and Parameter Inquiries
////////////////////////////////////////////////////////////////////////

long CLccMgr::GetNumReadProcs()
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	SHOW_GET_PROC_LAST(LCC_CS_CLASS_READ);

	m_busyStatus = BUSY_GET_NUM_READ_PROC;
	long rv = m_Lcc.GetProcLast(LCC_CS_CLASS_READ);
	m_busyStatus = BUSY_NOT;

	return rv;
}

long CLccMgr::GetReadProcName(long procID, CString& name)
{
	return GetClassProcName(LCC_CS_CLASS_READ, procID, name);
}

long CLccMgr::GetNumParams(long classID, long procID, int* num)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	COleVariant var;

	SHOW_GET_PARAM_LAST(classID, procID);

	m_busyStatus = BUSY_GET_NUM_PAR;
	long rv = m_Lcc.GetParamLast(classID, procID, &var);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK)
	{
		ASSERT(var.vt == VT_I4);
		*num = var.lVal;
	}
	else
	{
		*num = 0;
	}

	return rv;
}

long CLccMgr::GetParamName(long classID, long procID, long paramID, 
	CString& name)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	COleVariant var;

	SHOW_GET_PARAM_NAME(classID, procID, paramID);

	m_busyStatus = BUSY_PAR_NAME;
	long rv = m_Lcc.GetParamName(classID, procID, paramID, &var);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK)
	{
		ASSERT(var.vt == VT_BSTR);
		name = var.bstrVal;
	}
	else
	{
		name.Empty();
	}

	SHOW_PARAM_NAME(name);

	return rv;
}

long CLccMgr::GetParamNomVal(long classID, long procID, long paramID, 
	COleVariant & val)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	COleVariant var;

	SHOW_GET_PARAM_NOM(classID, procID, paramID);

	m_busyStatus = BUSY_PAR_DEFAULT;
	long rv = m_Lcc.GetParamNomVal(classID, procID, paramID, &var);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK)
	{
		// Verify data type
		COleVariant parType;

		SHOW_GET_PARAM_TYPE(classID, procID, paramID);

		m_busyStatus = BUSY_PAR_TYPE;
		rv = m_Lcc.GetParamType(classID, procID, paramID, &parType);
		m_busyStatus = BUSY_NOT;
		ASSERT(parType.vt == VT_I4);
		ASSERT( rv == KO_OK && long(var.vt) == parType.lVal);

		// Assign the value
		val.vt = var.vt;
		val = var;
	}
	else
	{
		val.vt = VT_EMPTY;
	}

	return rv;
}

long CLccMgr::GetParamEx(long classID, long procID, long paramID, 
	CParamEx& paramx)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	long rv;
	COleVariant var;

	// Parameter ID
	paramx.m_ID = paramID;

	// Parameter name
	rv = GetParamName(classID, procID, paramID, paramx.m_name); 
	if (rv != KO_OK )
	{
		return rv;
	}

	// Parameter Default Value
	rv = GetParamNomVal(classID, procID, paramID, paramx.m_default);
	if (rv != KO_OK )
	{
		return rv;
	}

	// Parameter Max Value
	SHOW_GET_PARAM_MAX(classID, procID, paramID);

	m_busyStatus = BUSY_PAR_MAX;
	rv = m_Lcc.GetParamMaxVal(classID, procID, paramID,	&paramx.m_max);
	m_busyStatus = BUSY_NOT;
	if (rv != KO_OK)
	{
		return rv;
	}

	// Parameter Min Value
	SHOW_GET_PARAM_MIN(classID, procID, paramID);

	m_busyStatus = BUSY_PAR_MIN;
	rv = m_Lcc.GetParamMinVal(classID, procID, paramID,	&paramx.m_min);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK)
	{
		return rv;
	}

	return rv;
}

long CLccMgr::Inquire(long inquireID, double* value)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	long rv;
	COleVariant val;

	// If Luminometer is not connected, can only check LCC status
	// and measurement protocol data
	if(!m_bLumConnected)
	{
		if (inquireID != LCC_INQ_LCCSTAT &&
			inquireID != LCC_INQ_PROTOCOL_CT &&
			inquireID != LCC_INQ_PROTOCOL_ID &&
			inquireID != LCC_INQ_PROTOCOL_NM)
		{
			return KO_LCC_LUM_DISCONNECTED;
		}
	}

	m_busyStatus = BUSY_INQUIRE;
	val.vt = VT_R8;
	val.dblVal = *value;
	rv = m_Lcc.Inquire(inquireID, &val);
	m_busyStatus = BUSY_NOT;
	if (rv != KO_OK)
	{
		return rv;
	}

	ASSERT(val.vt == VT_R8);
	*value = val.dblVal;

	if (inquireID != LCC_INQ_TEMP)
	{
		SHOW_INQUIRE(inquireID, value);
	}
	else
	{
		SHOW_INQ_TEMP();
	}
		
	// !!! Show results of inquire

	// Luminometer connected status is checked 
	if (inquireID == LCC_INQ_LCCSTAT && m_bLumConnected != (*value > 0.0))
	{	// Lum connection status has changed
		m_bLumConnected = *value > 0.0;
		// Inform the users
		POSITION pos = m_userList.GetHeadPosition();
		while (pos != NULL)
		{
			m_userList.GetNext(pos)->LumConnectHandler(m_bLumConnected);
		}
	}

	return rv;
}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
long CLccMgr::Inquire(long inquireID, double *inVal, CString &outStr)
{
	// This routine should only be used for protocol name inquiry,
	// since this is the only inquiry that returns a string
	ASSERT(inquireID > 0);
	ASSERT(inquireID == LCC_INQ_PROTOCOL_NM);
	
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}


	// If Luminometer is not connected, can only check LCC status and measurement 
	// protocol data
	if(!m_bLumConnected)
	{
		if (inquireID != LCC_INQ_LCCSTAT &&
			inquireID != LCC_INQ_PROTOCOL_CT &&
			inquireID != LCC_INQ_PROTOCOL_ID &&
			inquireID != LCC_INQ_PROTOCOL_NM)
		{
			return KO_LCC_LUM_DISCONNECTED;
		}
	}

	SHOW_INQUIRE(inquireID, NULL);

	m_busyStatus = BUSY_INQUIRE;
	long rv;
	COleVariant val;
	val.vt = VT_R8;
	val.dblVal = *inVal;
	rv = m_Lcc.Inquire(inquireID, &val);
	m_busyStatus = BUSY_NOT;
	if (rv != KO_OK)
	{
		return rv;
	}

	// The variant going in is a numeric, 1-based index.
	// On return, its type is changed to a BSTR
	if(val.vt == VT_BSTR)
	{
		outStr = V_BSTR(&val);
	}
	else
	{
		outStr.Format(_T("Error: CLccMgr::Inquire(long inquireID, double *inVal, CString &outStr) %s %ld"), 
			__FILE__, __LINE__);
	}

	return rv;
}

int CLccMgr::GetNumActProcs()
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	SHOW_GET_PROC_LAST(LCC_CS_CLASS_ACTION);

	m_busyStatus = BUSY_GET_NUM_ACT;
	long rv = m_Lcc.GetProcLast(LCC_CS_CLASS_ACTION);
	m_busyStatus = BUSY_NOT;

	return rv;
}

long CLccMgr::GetActProcName(long procID, CString& name)
{
	long rv = GetClassProcName(LCC_CS_CLASS_ACTION, procID, name);

	return rv;
}

long CLccMgr::GetNumClassProcs(long classID)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	SHOW_GET_PROC_LAST(classID);

	m_busyStatus = BUSY_GET_NUM_CLASS;
	long rv = m_Lcc.GetProcLast(classID);
	m_busyStatus = BUSY_NOT;

	return rv;
}

long CLccMgr::GetClassProcName(long classID, long procID, CString & name)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	COleVariant	var;

	SHOW_GET_PROC_NAME(classID, procID);
	
	m_busyStatus = BUSY_GET_CLASS_NAME;
	long rv = m_Lcc.GetProcName(classID, procID, &var);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK)
	{
		ASSERT(var.vt == VT_BSTR);
		name = var.bstrVal;
	}
	else
	{
		name.Empty();
	}

	SHOW_PROC_NAME(name);
	
	return rv;
}

long CLccMgr::ShowLccNow()
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	SHOW_SHOW();

	m_busyStatus = BUSY_SHOW;
	m_Lcc.ShowWindow();
	m_busyStatus = BUSY_NOT;

	return KO_OK;
}

long CLccMgr::CSGetReagent(long injectorID, long * pVolume)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	COleVariant	var;

	m_busyStatus = BUSY_REAGENT;
	long rv = m_Lcc.CSGetReagent(injectorID, &var);
	m_busyStatus = BUSY_NOT;

	if (rv == KO_OK)
	{
		ASSERT(var.vt == VT_I4);
		*pVolume = var.lVal;
	}

#ifdef DIAG_OUT
	CString jk;

	jk.Format(_T("CSGetReagent(%d) = %ld"), injectorID, *pVolume);
	SHOW_DIAG(jk);
#endif

	return rv;
}

long CLccMgr::CSGetTimeLeft(long * pMilliseconds)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	COleVariant	var;

	m_busyStatus = BUSY_TIME;
	long rv = m_Lcc.CSGetTimeLeft(&var);
	m_busyStatus = BUSY_NOT;

	if (rv == KO_OK)
{
		ASSERT(var.vt == VT_I4);
		*pMilliseconds = var.lVal;

		SHOW_GET_TIME( *pMilliseconds);
	}

	return rv;
}

long CLccMgr::CSGetData(long iArray, long iElement, double * pVal)
{
	// No _T("Calling Out")
	long nBusyErr = LccBusyCheck();

	if( nBusyErr != KO_OK) {
		return nBusyErr;
	}

	long rv;
    COleVariant var;

	SHOW_GET_DATA(iArray, iElement)

	m_busyStatus = BUSY_GET_DATA;
	rv = m_Lcc.CSGetData(iArray, iElement, var);
	m_busyStatus = BUSY_NOT;
	if (rv == KO_OK) 
	{
		ASSERT(var.vt == VT_R8);
		*pVal = var.dblVal;
	}

	return rv;
}

void CLccMgr::OnTick()
{
	static int tickCount;
	static bool isServicing = false;

	// Update count
	tickCount++;

	// Handle any one of the pending requests in order of priority
	if (m_busyStatus == BUSY_NOT && !isServicing)
	{
		long rv;
		CString errMsg;
	
		isServicing = true;

		if (m_lastAsyncErr != KO_OK)
		{
			if (m_lastAsyncErr == KO_LUM_ERR_COMM_PARITY)
			{	// Typical of power-up
				// !!! set a status indicator or something
				// AfxMessageBox(IDS_LUM_COM_ERR);
			}
			else if (m_lastAsyncErr == KO_LUM_ERR_BAD_COMMAND)
			{	// Sometimes get these during scan chain batch
				// reagent usage check.  Every SetParam is relayed 
				// to the luminometer.  Too many SetParams sent to 
				// quickly (as checking reagent usage for many batches)
				// will overflow the luminometers comm buffer.
				// Since the LCC calculates reagent usage, it doesn't
				// matter if the luminometer gets correct data.

				// Ignore only during Scan chain reagent usage test.
			}
			else
			{	// Generic error message
				
				// *** SYSTEM LOG **********************************************************
				CLogEvent	NewEvent;							       // Create a new event   *
				CString strDetails;

				strDetails.Format( _T("Exception code 0x%08x (%s)"), CLccMgr::LookupError( m_lastAsyncErr));
				NewEvent.EventSet(_T("LCC returned exception        "),// Event name           *
									strDetails);				   // Event details        *
				CSystemLog::g_oSystemLog.EventAdd( NewEvent);
				// *** SYSTEM LOG **********************************************************
				

				errMsg.Format(IDS_LCC_EXCEPTION, m_lastAsyncErr, CLccMgr::LookupError( m_lastAsyncErr));
				AfxMessageBox(errMsg);


			}
			m_lastAsyncErr = KO_OK;
		}
		else if (m_cReadsPending > 0)
		{
			rv = FeedUsersNextRead();
			if (rv != KO_OK)
			{
				errMsg.Format(_T("Feed Users Read returns exception 0x%X"), rv);
				AfxMessageBox(errMsg);
			}
		}
		else if (!m_lccMsgPending.IsEmpty())
		{	// Pass message event on to users
			POSITION pos = m_userList.GetHeadPosition();
			while (pos != NULL)
			{
				m_userList.GetNext(pos)->LccMsgHandler(m_lccMsgPending);
			}
			m_lccMsgPending.Empty();
		}
		else if (m_IsDonePending)
		{
			m_IsDonePending = false;
			// We have received the end of run event, but it has been 
			// held until all pending reads have been completed
			ReportCSStatus();
			if (m_pController != NULL)
			{
				m_pController->CSDoneHandler(m_lastStatus);
			}
		}
		else if (m_changeOfStatusPending)
		{
			m_changeOfStatusPending = false;
			ReportCSStatus();
		}
		else if (m_IsStoppingMode != LCC_STOP_UNDEF)
		{	// We have logged a request to stop
			rv = CSStop(m_pController, m_IsStoppingMode);
			if (rv != KO_OK)
			{
				errMsg.Format(_T("Delayed CSStop returns exception 0x%X"), rv);
				AfxMessageBox(errMsg);
			}
		}
		else if (m_IsClosing)
		{	// We have logged a request to close
			rv = CSClose(m_pController);
			if (rv != KO_OK)
			{
				errMsg.Format(_T("Delayed CSClose returns exception 0x%X"), rv);
				AfxMessageBox(errMsg);
			}
		}
		else if (m_IsRelinquishing)
		{
			rv = RelinquishControl(m_pController);
			if (rv != KO_OK)
			{
				errMsg.Format(_T("Delayed Relinquish control returns exception 0x%X"), rv);
				AfxMessageBox(errMsg);
			}
		}
		else if (LCCMGR_RETRY_CONNECT != 0
			&& m_bLumConnected == false 
			&& m_pController == NULL 
			&& tickCount % LCCMGR_RETRY_CONNECT == 0)
		{	// Attempt to connect every so often if LCC is disconnected
			// and not in use
			Sync(NULL);
		}

		isServicing = false;
	}

	if (m_busyStatus == BUSY_NOT)
	{
		// Send tick first to controller ...
		if (m_pController != NULL)
		{
			m_pController->TickHandler();
		}
	}

	// ...then to remaining users
	POSITION pos = m_userList.GetHeadPosition();
	while (pos != NULL && m_busyStatus == BUSY_NOT)
	{
		CLccUser* pUser = m_userList.GetNext(pos);
		if (pUser != m_pController)
		{
			pUser->TickHandler();
		}
	}
}


long CLccMgr::FeedUsersNextRead()
{
	long rv = KO_OK;

	switch (m_classID)
	{
	case LCC_CS_CLASS_ACTION:
		if (m_procedureID != LCC_ACT_GET_LUM)
		{
			break;
		}
		// else fall into read
	case LCC_CS_CLASS_READ:
		{
			double RLU, bkgnd;
			long flags;
			int  tubeIndex;
			COleVariant var;

			// Get index of last available tube data
			rv = GetIndexLastData(&tubeIndex);
			if (rv != KO_OK)
			{
				return rv;
			}
			// If more than one read is are pending, the next data is not
			// the last data.  (The last read  is always pending.)
			tubeIndex -= (m_cReadsPending - 1);

			// Get Tube data
			rv = GetTubeData(tubeIndex, &RLU, &bkgnd, &flags);
			if (rv != KO_OK)
			{
				return rv;
			}

			// Report tube data
			POSITION pos = m_userList.GetHeadPosition();
			while (pos != NULL)
			{
				m_userList.GetNext(pos)->NewReadDataHandler(tubeIndex, RLU, bkgnd, flags);
			}
		}
		break;

	default:
		break;
	}
				
	if (rv == KO_OK)
	{
		m_cReadsPending--;
	}

	return rv;
}


bool CLccMgr::TheTerminator()
{
	//////////////////////////////////////////////////////////////// 
    // Code to remove Lcc from memory after a crash 
    PROCESS_INFORMATION pi; 
    memset(&pi, 0, sizeof(pi)); 

    // Registered window class of LCC 
    CString ServerWindowClassName = _T("Luminometer Control Component"); 

    // Try to find existing server window 
    HWND hWndServer = ::FindWindow(ServerWindowClassName, NULL); 
    if(hWndServer != NULL)
	{   // Is a _T("Lcc.Document") open? 
        CString sDoc; 
        LPTSTR p = sDoc.GetBuffer(128); 
        ::GetWindowText(hWndServer, p, 128); 
        sDoc.ReleaseBuffer(-1); 

        // LCC is started and _T("Lcc.Document") is closed if Window text 
        // is only _T("Lcc") 
        // For debugging LCC it is necessary that LCC can be open without 
        // a _T("Lcc.Document"). 
        // After a crash LCC is still in memory with an open _T("Lcc.Document"). 
        // With an open _T("Lcc.Document"), Window text is _T("Lcc - Lcc1") etc. 
		int DebugOn = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt(gIniSec_Features,gIniEnt_DbgApps, 0);
        if (!DebugOn || sDoc.Find(_T("-")) != -1)
		{	//!!!doesn't work this way on NT; always _T("LCC") only
            // We do not have a valid thread handle 
            pi.hThread = NULL; 

            // Get server process handle 
            DWORD dwProcessID; 
            GetWindowThreadProcessId(hWndServer, &dwProcessID); 
            pi.hProcess = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, dwProcessID); 

            // Process handle is valid, terminate LCC process 
            if(pi.hProcess!= NULL) 
			{
				if(TerminateProcess(pi.hProcess, -1))
				{ 				 
					/*
					 * Seem to need a bodgy delay before lcc can be made to rise like a phoenix from the ashes.
					 */
					Sleep( 500);

					return true;
				}
				else
				{
					return false;
				}
			}
		}
    }
	return false;
}

struct LCCError {
	TCHAR *m_strError;
	long m_nValue;
} g_oLccErrorLookup[] = {
    { _T("KO_LUC_GENERIC"), 0x01000},	// Lucept returned exceptions

// Exceptions due to valid commands issued to Lucept when it is in an
// inappropriate state will begin with 0x01100
    { _T("KO_LUC_SYNC_GEN"), 0x01100},	// Command incompatible with state
    { _T("KO_LUC_NOT_CONTROLLER"), 0x01101},	// LccUser is not controller
    { _T("KO_LUC_NOT_CONTROLLER2"), 0x01102},	// LccUser is not controller
    { _T("KO_LUC_NOT_LCC_USER"), 0x01103},	
    { _T("KO_LUC_REFUSE_RELEASE"), 0x01104},	
    { _T("KO_LUC_RELEASE_PENDING"), 0x01105},
    { _T("KO_LUC_CS_STILL_ACTIVE"), 0x01106},
    { _T("KO_LUC_LCC_UNCONNECTED"), 0x01107},
    { _T("KO_LUC_CS_NOT_OPEN"), 0x01108},
    { _T("KO_LUC_NO_BATCH"), 0x01109},
    { _T("KO_LUC_LCC_IN_CALL"), 0x0110A},
    { _T("KO_LUC_DATA_PENDING"), 0x0110B},
    { _T("KO_LUC_WKL_NOT_READING"), 0x0110C},
    { _T("KO_LUC_BSTAT_BAD_CHANGE"), 0x0110D},
    { _T("KO_LUC_FINI_NOT_CONTROL"), 0x0110E},
    { _T("KO_LUC_BATCH_READ_ONLY"), 0x0110F},
    { _T("KO_LUC_WKL_READ_ONLY"), 0x01110},
    { _T("KO_LUC_WSTAT_BAD_CHANGE"), 0x01111},
    { _T("KO_LUC_READ_STAT_NEW"), 0x01112},
    { _T("KO_LUC_READ_NOT_CHECKED"), 0x01113},
    { _T("KO_LUC_ACT_NOT_CONTROL"), 0x01114},
    { _T("KO_LUC_ACTION_TIMEOUT"), 0x01115},
    { _T("KO_LUC_REQ_LCC_STOP"), 0x01116},
    { _T("KO_LUC_RGRID_NO_RECSET"), 0x01117},
    { _T("KO_LUC_RGRID_NO_TBLDEF"), 0x01118},
    { _T("KO_LUC_WKLVIEW_REGD"), 0x01119},
    { _T("KO_LUC_WKLVIEW_NOT_REGD"), 0x0111A},
    { _T("KO_LUC_NOT_YET_WINDOW"), 0x0111B},
    { _T("KO_LUC_BAD_BATCALC"), 0x0111C},
    { _T("KO_LUC_USER_ABORT_SC"), 0x0111D},

// Commands issued with unrecognized identifiers will begin with 0x01200
    { _T("KO_LUC_BAD_ID"), 0x01200},
    { _T("KO_LUC_BAD_TUBE_INDEX"), 0x01201},
    { _T("KO_LUC_BAD_ASSAY_ID"), 0x01202},
    { _T("KO_LUC_BAD_METHOD_ID"), 0x01203},
    { _T("KO_LUC_BAD_PROC_ID"), 0x01204},
    { _T("KO_LUC_BATCH_BAD_ASSAY"), 0x01205},
    { _T("KO_LUC_BATCH_BAD_MEAS"), 0x01206},
    { _T("KO_LUC_BAD_BATCH_INDEX"), 0x01207},
    { _T("KO_LUC_BAD_SAMP_INDEX"), 0x01208},
    { _T("KO_LUC_BAD_SAMP_STAT"), 0x01209},
    { _T("KO_LUC_ACT_ID_UNKNOWN"), 0x0120A},
    { _T("KO_LUC_BAD_INJECTOR_ID"), 0x0120B},
    { _T("KO_BAD_CAL_SAMP_ID"), 0x0120C},
    { _T("KO_BAD_SAMP_TYPE_NAME"), 0x0120D},

// Valid requests for data or information which is simply unavailable
// will begin with 0x1300
    { _T("KO_LUC_DATA_UNAVAIL"), 0x01300},
    { _T("KO_LUC_TUBE_DATA_UNAVAIL"), 0x01301},
    { _T("KO_LUC_ADDCALS_NOASSAY"), 0x01302},
    { _T("KO_LUC_ADDCALS_NOMETHOD"), 0x01303},
    { _T("KO_LUC_BATCH_NO_ASSAY"), 0x01304},
    { _T("KO_LUC_LOAD_WKL"), 0x01305},
    { _T("KO_LUC_NO_SAMP_REF_EXIST"), 0x01306},

// Attempts to set parameters or properties to values not within
// their acceptable range begin with 0x01400
    { _T("KO_LUC_BAD_VAL"), 0x01400},
    { _T("KO_LUC_CELL_TO_VARIANT"), 0x01401}, 
    { _T("KO_LUC_NULL_BATCH_PTR"), 0x01402},
    { _T("KO_LUC_TOO_FEW_CAL_TUBES"), 0x01403},
    { _T("KO_LUC_BAD_START_POS"), 0x01404},
    { _T("KO_LUC_TOO_FEW_TUBES"), 0x01405},
    { _T("KO_LUC_BAD_SAMPLE_INDEX"), 0x01406},
    { _T("KO_LUC_BAD_WKL_STAT"), 0x01407},
    { _T("KO_LUC_EXCEED_MAX_TUBES"), 0x01408},
    { _T("KO_LUC_NO_WKL_SPECIFIED"), 0x01409},
    { _T("KO_LUC_BAD_WKL_NAME"), 0x0140A},
    { _T("KO_LUC_BAD_RPT_DBNAME"), 0x0140B},
    { _T("KO_LUC_VOL_NOT_IN_RANGE"), 0x0140C},
    { _T("KO_LUC_INSUFFICIENT_REAG"), 0x0140D},
    { _T("KO_LUC_BAD_WKLVIEW_PTR"), 0x0140E},
    { _T("KO_LUC_BAD_MEAS_DEF"), 0x0140F},
    { _T("KO_LUC_BAD_ACALC_METH"), 0x01410},
    { _T("KO_LUC_SAMP_NOT_VITUAL"), 0x01411},
    { _T("KO_NO_REAL_SAMPLES"), 0x01412},
    { _T("KO_NO_VISIBLE_FIELDS"), 0x01413},

// Data inconsistencies between components
    { _T("KO_LUC_DATA_INCONSOSTENT"), 0x01500},
    { _T("KO_LUC_ACC_ACPARAMS"), 0x01501},
    { _T("KO_LUC_NUM_READ_PARAMS"), 0x01502},
    { _T("KO_LUC_BAD_PARAM_NAME_VT"), 0x01503},
    { _T("KO_BAD_LCC_DONE_STAT"), 0x01504},
    { _T("KO_BATCH_MISCOUNT"), 0x01505},
    { _T("KO_TUBE_MISCOUNT"), 0x01506},
    { _T("KO_LINK_IS_SPACER"), 0x01507},
    { _T("KO_LINK_PAST_END"), 0x01508},
    { _T("KO_REFSTR_MISSING_DELIM"), 0x01509},
    { _T("KO_REFSTR_MISSING_OPEN"), 0x0150A},
    { _T("KO_REFSTR_BAD_REFTYPE"), 0x0150B},
    { _T("KO_REFSTR_BAD_FILESPEC"), 0x0150C},

// Fatal Lucept software errors begin with 0x1800
    { _T("KO_LUC_SW_FATAL"), 0x01800},	
    { _T("KO_LUC_PLAY_VIRTUAL"), 0x01801},
    { _T("KO_LUC_READ_ACC_VIRTUAL"), 0x01802},
    { _T("KO_LUC_WRITE_ACC_VIRTUAL"), 0x01803},
    { _T("KO_LUC_CELL_DATA_TYPE"), 0x01804},
    { _T("KO_LUC_GET_SAMP_FIELD"), 0x01805},
    { _T("KO_LUC_SET_SAMP_FIELD"), 0x01806},
    { _T("KO_LUC_LUMACTOR_CREATE"), 0x01807},
    { _T("KO_LUC_LUMACTOR_EXISTS"), 0x01808},

// File I/O errors
    { _T("KO_LUC_FILE_GEN"), 0x01900},
    { _T("KO_LUC_FILE_OPEN"), 0x01910},
    { _T("KO_LUC_RPT_DB_CREATE"), 0x01911},
    { _T("KO_LUC_FILE_READ"), 0x01920},
    { _T("KO_LUC_WKL_READ"), 0x01921},
    { _T("KO_LUC_FILE_WRITE"), 0x01930},
    { _T("KO_LUC_RPT_REC_CREATE"), 0x01931},
    { _T("KO_LUCY_DOC_SAVE"), 0x01941},


////////////////////////////////////////////////////////////////////////
// Luminometer Control Component Exceptions
////////////////////////////////////////////////////////////////////////

// All exceptions returned by the LCC will begin with 0x02000
    { _T("KO_LCC_GENERIC"), 0x02000},	// LCC returned exceptions

// Exceptions due to valid commands issued to LCC when it is in an
// inappropriate state will begin with 0x02100
    { _T("KO_LCC_SYNC_GEN"), 0x02100},	// Command incompatible with state
    { _T("KO_LCC_CS_ALREADY_OPEN"), 0x02101},
    { _T("KO_LCC_CS_NOT_OPEN"), 0x02102},
	{ _T("KO_LCC_CS_ALREADY_STARTED"), 0x02103},
    { _T("KO_LCC_CS_NOT_STARTED"), 0x02104},
    { _T("KO_LCC_CS_RUNNING"), 0x02105},


// Commands issued with unrecognized identifiers will begin with 0x02200
    { _T("KO_LCC_BAD_ID"), 0x02200},
    { _T("KO_LCC_BAD_CLASS"), 0x02201},	
    { _T("KO_LCC_BAD_PROC"), 0x02202},	
    { _T("KO_LCC_BAD_PARAM"), 0x02203},

// Valid requests for data or information which is simply unavailable
// will begin with 0x2300
    { _T("KO_LCC_DATA_UNAVAIL"), 0x02300},
    { _T("KO_LCC_VALUE_NOT_DEFINED"), 0x02301},	// Parameter data unavailable

// Attempts to set parameters or properties to values not within
// their acceptable range begin with 0x02400
    { _T("KO_LCC_BAD_VAL"), 0x02400},
    { _T("KO_LCC_BAD_PARAM_VAL"), 0x02401},	// Bad CS parameter value

// Control session statuses may be translated into exceptions beginning
// with 0x02500 (translation done by UIC)
    { _T("KO_LCC_CS_STAT"), 0x02500},	
    { _T("KO_LCC_CS_FAULTED"), 0x02501},
    { _T("KO_LCC_CS_STOPPED"), 0x02502},
    { _T("KO_LCC_CS_CLOSED"), 0x02503},
    { _T("KO_LCC_CS_NOT_CLOSED"), 0x02504},
    { _T("KO_LCC_CS_NO_TUBES"), 0x02505},
    { _T("KO_LCC_CS_NO_TUBE_WELL_1"), 0x02506},

// LCC statuses may be translated into exceptions beginning 
// with 0x02600 (translation done by UIC)
    { _T("KO_LCC_STAT"), 0x02600},
    { _T("KO_LCC_LUM_DISCONNECTED"), 0x02606}, // See 0x0270B

// Luminometer statuses may be translated into exceptions beginning
// with 0x02700 (translation done by UIC)
    { _T("KO_LUM_STAT_GEN"), 0x02700},
    { _T("KO_LUM_ERR_CHAIN_SLOW"), 0x02701},	// Lum Err A
    { _T("KO_LUM_ERR_CHAIN_BLOCKED"), 0x02702},	// Lum Err B
    { _T("KO_LUM_ERR_LIFT_TIMEOUT"), 0x02703},	// Lum Err C
    { _T("KO_LUM_ERR_TUBE_DET_FAIL"), 0x02704},	// Lum Err D
    { _T("KO_LUM_ERR_TEMP_"), 0x02705},	// Lum Err E
    { _T("KO_LUM_ERR_DOOR_OPEN"), 0x02706},	// Lum Err H
    { _T("KO_LUM_ERR_WASH_BLANKS"), 0x02707},	// Lum Err I
    { _T("KO_LUM_ERR_WASH_TUBES"), 0x02708},	// Lum Err J
    { _T("KO_LUM_ERR_INJ_NO_TUBE"), 0x02709},	// Lum Err K
    { _T("KO_LUM_ERR_MEAS_NO_TUBE"), 0x0270A},	// Lum Err L
    { _T("KO_LUM_ERR_COMM_PARITY"), 0x0270B},	// Lum Err M
    { _T("KO_LUM_ERR_BAD_COMMAND"), 0x0270C},	// Lum Err N
    { _T("KO_LUM_ERR_END_CHAIN"), 0x0270D},	// Lum Err P
    { _T("KO_LUM_ERR_NONE"), 0x0270E},	// Lum Err Z
    { _T("KO_LUM_ERR_DOOR_NOT_OPEN"), 0x0270F},	// Lcc is of the opinion that the user should open the door

// Fatal LCC software errors are persistant and will be returned
// from every command until cleared.  They begin with 0x2800
    { _T("KO_LCC_SW_FATAL"), 0x02800},	
// G.P. --	You may want to define specific instances of this class
//			of exceptions to aid in error analysis and correction.
//			- P.M.



////////////////////////////////////////////////////////////////////////
// Assay Calculation Component Exceptions
////////////////////////////////////////////////////////////////////////

// All exceptions returned by the ACC will begin with 0x03000
    { _T("KO_ACC_GENERIC"), 0x03000},	// LCC returned exceptions

// Exceptions due to valid commands issued to ACC when it is in an
// inappropriate state will begin with 0x03100
    { _T("KO_ACC_SYNC_GEN"), 0x03100},	// Command incompatible with state
    { _T("KO_ACC_CALC_NOT_READY"), 0x03101},	
    { _T("KO_ACC_VALID_NOT_READY"), 0x03102},	
    { _T("KO_ACC_CRCAL_NOT_READY"), 0x03103},	
    { _T("KO_ACC_PARAM_FROZEN"), 0x03104},
    { _T("KO_ACC_NOT_CALIBRATED"), 0x03105},
    { _T("KO_ACC_VALIDATE_STATE"), 0x03106},

// Commands issued with unrecognized identifiers will begin with 0x03200
    { _T("KO_ACC_BAD_ID"), 0x03200},	
    { _T("KO_ACC_BAD_METH"), 0x03201},	
    { _T("KO_ACC_BAD_PARAM"), 0x03202},	
    { _T("KO_ACC_BAD_FIELD"), 0x03203},
    { _T("KO_ACC_BAD_OUTPUT_FIELD"), 0x03204},
    { _T("KO_ACC_BAD_INPUT_FIELD"), 0x03205},
    { _T("KO_ACC_BAD_FIELD_ID"), 0x03206},
    { _T("KO_ACC_WRONG_VARTYPE"), 0x03207},
    { _T("KO_ACC_BAD_SAMP_TYPE"), 0x03208},

// Valid requests for data or information which is simply unavailable
// will begin with 0x3300
    { _T("KO_ACC_DATA_UNAVAIL"), 0x03300},
    { _T("KO_ACC_VALUE_NOT_DEFINED"), 0x03301},	// Parameter data unavailable
    { _T("KO_ACC_BAD_SAMP_NUMBER"), 0x03302},
    { _T("KO_ACC_NO_SUCH_SAMP"), 0x03303},

// Attempts to set parameters or properties to values not within
// their acceptable range begin with 0x03400
    { _T("KO_ACC_BAD_VAL"), 0x03400},
    { _T("KO_ACC_BAD_PARAM_VAL"), 0x03401},	
    { _T("KO_ACC_PARAM_MAXXED"), 0x03402},	
    { _T("KO_ACC_PARAM_MINNED"), 0x03403},	
    { _T("KO_ACC_WRONG_PARAM_TYPE"), 0x03404},
    { _T("KO_ACC_REPLS_TOO_LARGE"), 0x03405},
    { _T("KO_ACC_REPLS_TOO_SMALL"), 0x03406},
    { _T("KO_ACC_MISSING_CAL"), 0x03407},
    { _T("KO_ACC_EXTRA_CAL"), 0x03408},
    { _T("KO_ACC_TOO_FEW_SAMPLES"), 0x03409},
    { _T("KO_ACC_PMT_SATURATION"), 0x0340A},

// Measurement method statuses may be translated into exceptions beginning
// with 0x03500 (translation done by UIC)
    { _T("KO_ACC_STAT"), 0x03500},


// Fatal ACC software errors are persistant and will be returned
// from every command until cleared.  They begin with 0x3800
    { _T("KO_ACC_SW_FATAL"), 0x03800},	
    { _T("KO_ACC_VAR_COPY_FAIL"), 0x03801},
    { _T("KO_ACC_SAMP_UNFORMATTED"), 0x03802},
    { _T("KO_ACC_METH_UNINIT"), 0x03803},
    { _T("KO_ACC_BAD_PARAM_TYPE"), 0x03804},
    { _T("KO_ACC_ORIGINAL_SIN"), 0x03805},
    { _T("KO_ACC_ADD_FIELD_REPEAT"), 0x03806},
    { _T("KO_ACC_ADD_FIELD_FROZEN"), 0x03807},

// ACC encountered variant data which is of an inappropriate
// data type for the given context
    { _T("KO_ACC_GEN_WRONG_VT"), 0x03900},
    { _T("KO_ACC_STYPE_WRONG_VT"), 0x03901},
    { _T("KO_ACC_REPLS_WRONG_VT"), 0x03902},
    { _T("KO_ACC_RLU_WRONG_VT"), 0x03902},
    { _T("KO_ACC_BKG_WRONG_VT"), 0x03903},

////////////////////////////////////////////////////////////////////////
// Phil Util Exceptions
////////////////////////////////////////////////////////////////////////

// All exceptions returned by the Lucept will begin with 0x01000
    { _T("KO_PHUT_GENERIC"), 0x04000},	// Lucept returned exceptions

// Exceptions due to valid commands issued in an
// inappropriate state will begin with 0x01100
    { _T("KO_PHUT_SYNC_GEN"), 0x04100},	// Command incompatible with state

// Commands issued with unrecognized identifiers will begin with 0x04200
    { _T("KO_PHUT_BAD_ID"), 0x04200},

// Valid requests for data or information which is unavailable
// will begin with 0x4300
    { _T("KO_PHUT_DATA_UNAVAIL"), 0x04300},

// Attempts to set parameters or properties to values not within
// their acceptable range begin with 0x04400
    { _T("KO_PHUT_BAD_VAL"), 0x04400},
    { _T("KO_PHUT_INVALID_ENTRY"), 0x04401},
    { _T("KO_PHUT_ABOVE_MAX"), 0x04402},
    { _T("KO_PHUT_BELOW_MIN"), 0x04403},

// Data inconsistencies between components
	{ _T("KO_PHUT_DATA_INCONSISTENT"), 0x04500},

// Fatal software errors begin with 0x4800
    { _T("KO_PHUT_SW_FATAL"), 0x04800},	


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Report specific constants
    { _T("KO_RPT_GENERIC"), 0x05000},

// Report 
    { _T("KO_RPT_APPEND_RETEST_SMP"), 0x05101},

    { NULL, -1},
};

/**
 * Do something radical: convert mysterious error code into something vaguely (and I mean vaguely) informative.
 */
CString CLccMgr::LookupError( long nErrorCode)
{
	for( int i=0; g_oLccErrorLookup[i].m_nValue != -1; i++) {
		if( g_oLccErrorLookup[i].m_nValue == nErrorCode) {
			return g_oLccErrorLookup[i].m_strError;
		}
	}

	return _T(" Unknown error code");
}

/**
 * Dump measurement definitions from the lcc.
 */
void CLccMgr::DumpMeasures()
{
	/*
	 * No point in calling
     *	          int nReadProcs = GetNumReadProcs();
	 * as the count does not include procs that are marked as 'unused' and yet these are included in protcol id
	 * indexes.
	 */
	int nReadProcs = 100;

	for( int nProc=0; nProc < nReadProcs; nProc++) {
		CString strName;
		CString strFmt;

		if( GetReadProcName( nProc + 1, strName) != KO_OK) {
			// If the proc name could not be read then it is probably not defined.
			continue;
		}

		int nParams;

		if( GetNumParams( LCC_CS_CLASS_READ, nProc + 1, &nParams) != KO_OK) {
			SHOW_DIAG( _T( "Could not get number of params"));
			continue;
		}

		for( int nParam = 0; nParam < nParams; nParam++) {
			CString strParamName;

			if( GetParamName( LCC_CS_CLASS_READ, nProc + 1, nParam, strParamName) != KO_OK) {
				SHOW_DIAG( _T( "Could not get paramater name"));
			}
		}
	}
}
