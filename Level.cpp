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

// Level.cpp: implementation of the CLevel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lucy.h"
#include "Level.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CLevel, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLevel::CLevel()
{
	// Create the brushes
	m_brushEmpty = new CBrush(RGB(255, 255, 255));
	m_brushFull = new CBrush(RGB(0, 0, 0));
	m_levelType = rect;
	m_ratioFull = 0.0;
	m_caption = _T("");
}


CLevel::~CLevel()
{
	delete m_brushEmpty;
	delete m_brushFull;
}

//***********************************************************
//
//  CLevel :: Serialize()
//
//  Purpose:
//              Writes Block to disk
//
//
//  Parameters:
//      CArchive& archive - The archive to write into
//      
//
//  Return: (void)
//
//
//  Comments:
//
//
//
//*************************************************************

void CLevel :: Serialize(CArchive& archive)
{
	WORD writeme;
   
	// call baseclass function
	CObject::Serialize(archive);
   
	// dump data for this class
	if(archive.IsStoring())
	{
		archive << m_levelRect;
	}
	else
    {
		archive >> (WORD)writeme >> m_levelRect;
    }
}

//***********************************************************
//
//  CRoundBlock :: Draw()
//
//  Purpose:
//              Draws a Eliptical block
//
//
//  Parameters:
//      CDC* pDC     - The device context that the Round Rectangle 
//                         block is drawn into.
//      
//
//  Return: (void)
//
//
//  Comments:
//
//
//
//*************************************************************
void CLevel::Draw(CDC* pDC)
{
    CBrush *oldbrush;
	double pi = 3.1415926535;

	// Switch to an invisible pen
	CPen *newpen = new CPen(PS_NULL, 0, RGB(0, 0, 0));
    CPen *oldpen = pDC->SelectObject(newpen);

	if (m_levelType == rect) 
	{	// Draw the empty portion
		oldbrush = pDC->SelectObject(m_brushEmpty);
		RECT rTmp = m_levelRect;
		rTmp.bottom -= (long) ((rTmp.bottom - rTmp.top) * (m_ratioFull)); 
		pDC->Rectangle(&rTmp);

		// Draw the full portion
		pDC->SelectObject(m_brushFull);
		rTmp = m_levelRect;
		rTmp.top += (long)((rTmp.bottom - rTmp.top) * (1 - m_ratioFull));
		pDC->Rectangle(&rTmp);
	} 
	else 
	{	// Draw the empty portion
		oldbrush = pDC->SelectObject(m_brushFull);
		pDC->Pie(&m_levelRect, 
				CPoint((m_levelRect.right + m_levelRect.left) / 2 + int(100.0*cos(2.0 * pi * m_ratioFull - pi/2.0)) + 1,
					(m_levelRect.bottom + m_levelRect.top) / 2 + int(100.0*sin(2.0 * pi * m_ratioFull - pi/2.0)) + 1),
				CPoint((m_levelRect.left+m_levelRect.right)/2, m_levelRect.top));
	}

	// create a new font
	CFont boxFont;
	if (!boxFont.CreatePointFont(80, _T("OCR-A"), pDC))
	{
			AfxMessageBox(_T("Can not create font"));
	}
	// Save the old font, and change to new font
	CFont* pOldFont = pDC->SelectObject( &boxFont);
	// Write the text
	//	TCHAR box[81]; 
	pDC->DrawText(m_caption, -1, &m_captionRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	// switch back to original font
	pDC->SelectObject( pOldFont);

	// Restore the original pen and brush
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);
	delete newpen;
}

void CLevel :: SetType (LEVELTYPES level_type)
{
	m_levelType = level_type;
}

void CLevel::SetLevel(double ratio)
{
	// tha ratio must be between 0 and 1
	if (ratio < 0.0) ratio = 0.0;
	if (ratio > 1.0) ratio = 1.0;
	m_ratioFull = ratio;
}

void CLevel::SetColor(COLORREF fullColor, COLORREF emptyColor)
{
	delete m_brushFull;
	delete m_brushEmpty;
	m_brushFull = new CBrush(fullColor);
	m_brushEmpty = new CBrush(emptyColor);
}