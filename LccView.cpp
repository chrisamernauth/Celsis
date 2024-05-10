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

// LccView.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "LccView.h"
#include "LccMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLccView

IMPLEMENT_DYNCREATE(CLccView, CEditView)

CLccView::CLccView()
{
}

CLccView::~CLccView()
{
}


BEGIN_MESSAGE_MAP(CLccView, CEditView)
	//{{AFX_MSG_MAP(CLccView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( ID_BUTTON_DUMP_PROCS, OnDumpProcs)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLccView drawing

void CLccView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLccView diagnostics


#ifdef _DEBUG
void CLccView::AssertValid() const
{
	CEditView::AssertValid();
}

void CLccView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLccView message handlers

void CLccView::OnDestroy() 
{
	CEditView::OnDestroy();
	
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();			
	pApp->m_pLccView = NULL;
}

void CLccView::OnDumpProcs()
{
	((CLuceptApp *)AfxGetApp())->GetLccMgr()->DumpMeasures();
}
