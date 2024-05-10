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

// LCDWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Lucy.h"
#include "LCDWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLCDWnd

CLCDWnd::CLCDWnd()
{
}

CLCDWnd::~CLCDWnd()
{
}


BEGIN_MESSAGE_MAP(CLCDWnd, CWnd)
	//{{AFX_MSG_MAP(CLCDWnd)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLCDWnd message handlers


void CLCDWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Get the client area dimensions
	RECT lcdRect;
	GetClientRect(&lcdRect);

	// Get a pointer to the document
	CLucyDoc *pDoc = ((CLucyView *)GetParent())->GetDocument();

	// create a test font to get the size
	CFont testFont;
	if (!testFont.CreatePointFont(100, _T("OCR-A"), &dc))
	{
			AfxMessageBox(_T("Can not create font"));
	}
	// Save the old font, and change to test font
	CFont* pOldFont = dc.SelectObject( &testFont);
	// Get the pixel height of the test size
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	// On that basis, calculate the points required to fit fout lines in the box
	int points = (lcdRect.bottom - lcdRect.top) * 100 / tm.tmHeight / 4;

	// Create the actual new font
	CFont boxFont;
	if (!boxFont.CreatePointFont(points, _T("OCR-A"), &dc))
	{
			AfxMessageBox(_T("Cannot create font"));
	}
	// Change to the new font
	dc.SelectObject( &boxFont);
	dc.DrawText(m_msg, -1, &lcdRect, DT_LEFT);

	// switch back to original font
	dc.SelectObject( pOldFont);

	// Do not call CWnd::OnPaint() for painting messages
}

void CLCDWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
}
