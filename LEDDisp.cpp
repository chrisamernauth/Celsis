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

// LEDDisp.cpp : implementation file
//

#include "stdafx.h"
#include "Lucept.h"
#include "LEDDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LPCTSTR fontName = _T("LcdD");

/////////////////////////////////////////////////////////////////////////////
// CLEDDisp

CLEDDisp::CLEDDisp(int nDigits, LPCTSTR text)
{
	if (nDigits < 1 || nDigits > 80)
	{
		m_nDigits = 5;
	}
	else
	{
		m_nDigits = nDigits;
	}

	m_color = RGB(255, 31, 31);		// orange it up a bit

	SetDispText(text);
}

CLEDDisp::~CLEDDisp()
{
}


BEGIN_MESSAGE_MAP(CLEDDisp, CButton)
	//{{AFX_MSG_MAP(CLEDDisp)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLEDDisp message handlers

int CLEDDisp::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// Font creation structure
	LOGFONT	logFont;

	// Set desired height (in Pixels)
	logFont.lfHeight = lpCreateStruct->cy * 8 / 10;
	// Set desired width (in Pixels)
	logFont.lfWidth= 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_NORMAL;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
 	logFont.lfQuality = DEFAULT_QUALITY   ;
	logFont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	_tcsncpy(logFont.lfFaceName, fontName, 32);
	
	// Create LED font based on height
	VERIFY(m_LEDFont.CreateFontIndirect(&logFont));

	return 0;
}

BOOL CLEDDisp::Create(const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	return CButton::Create(_T(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		rect, pParentWnd, nID);
}


void CLEDDisp::OnDraw(CDC * pDC)
{
}
	
void CLEDDisp::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HDC hDC = lpDrawItemStruct->hDC;
	RECT* pRect = &lpDrawItemStruct->rcItem;
	HBRUSH oldBrush;
	HANDLE oldPen, newPen;
	HFONT oldFont;
	COLORREF oldTextColor, oldBkColor;

	// Fill background and draw bezel
	oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(BLACK_BRUSH)); 
	newPen = CreatePen(PS_INSIDEFRAME, 1, 0x808080);	// !!! system color? 
//	newPen = ::GetStockObject( NULL_PEN);
 	oldPen = (HPEN)SelectObject(hDC, newPen);
	if( Rectangle(hDC, pRect->left, pRect->top, pRect->right, 
		pRect->bottom ) == FALSE) {

		long nError = GetLastError();

		TRACE1( "Rectangle returned error %08x", nError);
	}
//	SelectObject(hDC, GetStockObject(WHITE_PEN));
//	MoveToEx(hDC, pRect->left + 1, pRect->bottom - 1 , NULL);
//	LineTo(hDC, pRect->right - 1, pRect->bottom - 1);
//	LineTo(hDC, pRect->right - 1, pRect->top + 1);


	// Draw text
	oldFont = (HFONT)SelectObject(hDC, m_LEDFont);
	oldTextColor = SetTextColor(hDC, m_color); 
	oldBkColor = SetBkColor(hDC, 0);			
	DrawText(hDC, m_dispText, m_nDigits, pRect, 
		DT_CENTER | DT_VCENTER | DT_SINGLELINE );
 
	// Restore DC
	SetBkColor(hDC, oldBkColor);
	SetTextColor(hDC, oldTextColor); 
	SelectObject(hDC, oldFont);
	SelectObject(hDC, oldPen);
	SelectObject(hDC, oldBrush);

	DeleteObject(newPen);
}

bool CLEDDisp::SetDispText(LPCTSTR text)
{
	bool rv = true;

	m_dispText = text;

	if (m_dispText.GetLength() > m_nDigits)
	{	// Truncate string to specified length
		m_dispText = m_dispText.Left(m_nDigits);
		rv = false;
	}
	else while (m_dispText.GetLength() < m_nDigits)
	{	// Pad with spaces on the left
		m_dispText = CString(_T(" ")) + m_dispText;
	}

	if (GetSafeHwnd() != NULL)
	{
		Invalidate();
	}

	return rv;
}

