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

// ChildFrm.cpp : implementation of the CLucyFrame class
//

#include "stdafx.h"
#include "LucyFrm.h"
#include "LucyView.h"
#include "resource.h"
#include "BCFrame.h" // !!! kill me later
#include "SampFrm.h"
#include "Specification.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLucyFrame

IMPLEMENT_DYNCREATE(CLucyFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CLucyFrame, CMDIChildWndEx)
	//{{AFX_MSG_MAP(CLucyFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLucyFrame construction/destruction

CLucyFrame::CLucyFrame() 
{
	m_pSampViewFrame = NULL;	
}

CLucyFrame::~CLucyFrame()
{
}

BOOL CLucyFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Remove thick frame (resizable) style
//	cs.style &= ~WS_THICKFRAME;
//	cs.style &= ~WS_BORDER;
//	cs.style &= ~WS_CAPTION;
// 	cs.style &= ~WS_SYSMENU; 
	cs.style &= ~FWS_ADDTOTITLE;
	cs.style |= WS_MAXIMIZE;

	cs.lpszName = _T("Luminometer");

	return CMDIChildWndEx::PreCreateWindow(cs);
}

/**
 * This is provided to make the Lucy Frame initially maximized but not always so (doing this
 * with WS_MAXIMIZE causes it to be always maximized).
 */
void CLucyFrame::ActivateFrame(int nCmdShow)
{
    if (nCmdShow == -1)
        nCmdShow = SW_SHOWMAXIMIZED;

	if( Specification::g_oSpecification.GetInstrumentType() == INSTRUMENT_C3) {
		nCmdShow = SW_HIDE;
	}

    CMDIChildWndEx::ActivateFrame(nCmdShow);
}

/////////////////////////////////////////////////////////////////////////////
// CLucyFrame diagnostics

#ifdef _DEBUG
void CLucyFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CLucyFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLucyFrame message handlers

void CLucyFrame::UpdateSize()
{
	// Get MDI Client window
	CWnd* pMDIClient = GetParent();
	ASSERT(pMDIClient != NULL);

	// Get size of client rect
	RECT clientRect;
	pMDIClient->GetClientRect(&clientRect);

// pcw killing toolbar?	VERIFY(SetWindowPos(&wndTop, clientRect.left, clientRect.top, 
//		clientRect.right, clientRect.bottom, SWP_SHOWWINDOW));
}

int CLucyFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

void CLucyFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIChildWndEx::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	RecalcLayout();
}


