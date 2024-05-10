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

// Conduct.cpp : implementation file
//

#include "stdafx.h"
#include "Conduct.h"
#include "LccDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Declare messages
extern UINT NEAR WM_CONDUCTOR_BEGIN_FINI
	= RegisterWindowMessage(_T("LuceptConductorBeginFini"));

/////////////////////////////////////////////////////////////////////////////
// CConductor

IMPLEMENT_DYNCREATE(CConductor, CView)

CConductor::CConductor()
{
	m_pPatron = NULL;
	m_IsFinale = false;
	m_lastRv = KO_OK;
	// The default OnDone message handler
	m_wmActDone = WM_CONDUCTOR_BEGIN_FINI;
	m_pWndRecipient = NULL;
}

CConductor::~CConductor()
{
}


BEGIN_MESSAGE_MAP(CConductor, CView)
	//{{AFX_MSG_MAP(CConductor)
	ON_WM_DESTROY()
    ON_REGISTERED_MESSAGE(WM_CONDUCTOR_BEGIN_FINI, Fini)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConductor drawing

void CConductor::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CConductor diagnostics

#ifdef _DEBUG
void CConductor::AssertValid() const
{
	CView::AssertValid();
}

void CConductor::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWklDoc* CConductor::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWklDoc)));
	return (CWklDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConductor message handlers

void CConductor::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	// Create ACC
	m_pAcc = new CACCMgr(this);
	if (m_pAcc == NULL)
	{
		AfxMessageBox(IDS_NO_ACC);
		ASSERT(false);
	}
}

void CConductor::RegisterPatron(CWnd* pPatron) 
{
	m_pPatron = pPatron;
}

LRESULT  CConductor::Fini(WPARAM wParam, LPARAM lParam)
{
	long rv;
	// Under any reasonable circumstances this code should
	// not be executed if this LccUser is not the controller
	if (!IsController())
	{
		UnregisterWithLcc();
		// Notify Patron
		m_pPatron->PostMessage(WM_CONDUCT_FINI, WPARAM(0), 
			KO_LUC_FINI_NOT_CONTROL);
		return (LRESULT)true;
	}
		
	if (!m_IsFinale)
	{
		// This is the first call of Fini
		// Save lparam as exception code to return to Patron
		m_lastRv = lParam;
		m_IsFinale = true;
	}

	// Shut down the workload if necessary
	long csStatus = CSGetStatus();
	switch (csStatus)
	{
	case LCC_CS_RUNNING:
		// Stop the workload
		rv = Stop();
		if (rv != KO_OK)
		{
			#ifdef _DEBUG
			CString errMsg;
			errMsg.Format(_T("CSStop returns 0x%X"), rv); 
			AfxMessageBox(errMsg);
			#endif
		}
		// Fall into stopping...
	case LCC_CS_STOPPING: 
		{
			// Wait for workload to stop
			CString msg;
			msg.LoadString(IDS_STOPPING___);
			GetParent()->SetWindowText(msg); 
		}
		// Will be called back later by OnTick handler
		return (LRESULT)true;

	case LCC_CS_OPENED: 
	case LCC_CS_ENDED: 
	case LCC_CS_STOPPED: 
	case LCC_CS_FAULTED:
		// Close the workload
		rv = CSClose();
		if (rv != KO_OK)
		{
			#ifdef _DEBUG
			CString errMsg;
			errMsg.Format(_T("CSClose returns 0x%X"), rv);
			AfxMessageBox(errMsg);
			#endif
		}
		// Will be called back later by OnTick handler
		return (LRESULT)true;

	case LCC_CS_CLOSED: 
		// Don't need to do anything
		break;

	default:
		ASSERT(false);
	}

	// Release control of LCC
	rv = RelinquishControl();
	if (rv != KO_OK)
	{
		#ifdef _DEBUG
		CString errMsg;
		errMsg.Format(_T("RelinquishControl returns 0x%X"), rv); 
		AfxMessageBox(errMsg);
		#endif
	}
	UnregisterWithLcc();

	// Notify Patron that we are done
	m_pPatron->PostMessage(WM_CONDUCT_FINI, WPARAM(0), m_lastRv);

	return (LRESULT)true;
}

long CConductor::Play()
{
	return KO_LUC_PLAY_VIRTUAL;
}


long CConductor::Stop()
{
	return CSStop(LCC_STOP_IMMEDIATE);
}

void CConductor::OnDestroy() 
{
	delete m_pAcc;
	CView::OnDestroy();
}

void CConductor::TickHandler()
{
	// We are in the process of closing
	if (m_IsFinale)
	{
		PostMessage(WM_CONDUCTOR_BEGIN_FINI, 0, 0);
	}
}


void CConductor::CSDoneHandler(long CSStatus)
{
	// If a message has been armed, fire it, else some other custom
	// method of shutting down is required
	if (m_wmActDone != 0)
	{
		long rv;

		// Patch bad status returned by LCC
		long correctStatus = CSStatus;
		if (m_lastAsyncError != KO_OK)
		{
			correctStatus = LCC_CS_FAULTED;
		}

		// Convert CSStatus to an exception code
		// which is passes as the message LPARAM
		switch (correctStatus)
		{
		case LCC_CS_ENDED:				// Normal termination
			rv = KO_OK;
			break;
		case LCC_CS_STOPPED:			// Aborted by Lucept
			rv = KO_LCC_CS_STOPPED;
			break;
		case LCC_CS_FAULTED:			// Aborted by LCC
			rv =  KO_LCC_CS_FAULTED;
			break;

		case LCC_CS_OPENED:			
		case LCC_CS_RUNNING: 
		case LCC_CS_STOPPING: 
		case LCC_CS_CLOSED:				
		default:
			rv =  KO_BAD_LCC_DONE_STAT;
		}

		// Post loaded message.
		if (m_pWndRecipient	== NULL)
		{	// Send to myself
			PostMessage(m_wmActDone, 0, rv);
		}
		else
		{	// Send elsewhere
			ASSERT(m_pWndRecipient->IsKindOf(RUNTIME_CLASS(CWnd)));
			m_pWndRecipient->PostMessage(m_wmActDone, 0, rv);
		}
		// Reset the message.  It's a one shot and must be reloaded
		// to fire again.
		m_pWndRecipient = NULL;
		m_wmActDone = 0;
	}

	// Call base class to post error message
	CLccUser::CSDoneHandler(CSStatus);
}

bool CConductor::LoadDoneMsg(UINT msg, CWnd* pWndRecipient)
{
	m_wmActDone = msg;
	m_pWndRecipient = pWndRecipient;
	if (m_pWndRecipient != NULL	&& 
		!m_pWndRecipient->IsKindOf(RUNTIME_CLASS(CWnd)))
	{
		return false;
	}
	else
	{
		return true;
	}
}


////////////////////////////////////////////////////////////////////////
// Get control of LCC
////////////////////////////////////////////////////////////////////////

long CConductor::GetLccControl()
{
	long rv;

	rv = RegisterWithLcc();
	if (rv != KO_OK)
	{
		AfxMessageBox(IDS_NO_CONNECT_LCC);
		// Notify Patron
		m_pPatron->PostMessage(WM_CONDUCT_FINI, WPARAM(0), rv);
		return rv;
	};

	rv = RequestControl();
	if (rv != KO_OK)
	{
		if (rv == KO_LCC_LUM_DISCONNECTED)
		{
			AfxMessageBox(IDS_LUM_OFFLINE);
		}
		else if( rv == KO_LUC_REFUSE_RELEASE)
		{
			AfxMessageBox(IDS_NO_CONTROL_LCC);
		}
		
		UnregisterWithLcc();
		// Notify Patron
		m_pPatron->PostMessage(WM_CONDUCT_FINI, WPARAM(0), rv);
		return rv;
	}

	return rv;
}
