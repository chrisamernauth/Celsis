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

// BCFrame.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "BCFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatCalcFrame

IMPLEMENT_DYNCREATE(CBatCalcFrame, CMDIChildWndEx)

CBatCalcFrame::CBatCalcFrame()
{
}

CBatCalcFrame::~CBatCalcFrame()
{
}


BEGIN_MESSAGE_MAP(CBatCalcFrame, CMDIChildWndEx)
	//{{AFX_MSG_MAP(CBatCalcFrame)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatCalcFrame message handlers

BOOL CBatCalcFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~WS_VISIBLE;	

	return CMDIChildWndEx::PreCreateWindow(cs);
}

void CBatCalcFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	if (bActivate)
	{	// Refuse
		return;
	}

	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}
