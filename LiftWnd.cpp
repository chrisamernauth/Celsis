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

// LiftWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Lucy.h"
#include "LiftWnd.h"
#include "Luceerr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Make application object accessible in this module since 
// we are storing the timer variable in CListDragApp (m_Timer)

/////////////////////////////////////////////////////////////////////////////
// CLiftWnd

CLiftWnd::CLiftWnd()
{
    // Load bitmap that contains all of the animated lift motion
    // can 'cells'. All images are contained in one bitmap in order
    // to speed up redraws and conserve system resources
    m_LiftBmp.LoadBitmap(IDB_LIFT);
	
	// Calculate the number of frames in the bitmap
	BITMAP bm;
    m_LiftBmp.GetBitmap(&bm);
	m_Frames = bm.bmWidth / LIFT_WIN_WIDE;

	// Start the lift up
	m_LiftPosition = up;

    // Load two Cursor handles. m_NullCursor is an empty cursor to use 
    // during the animation so there is no flicker if the cursor is over 
    // the animation window. m_ArrowCursor is the standard arrow cursor.
    m_NullCursor  = AfxGetApp()->LoadCursor(IDC_NULL);
    m_ArrowCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

    // Initalize Timer Counter to zero
    m_Timer = 0;
	m_TimerCnt=0;
}

CLiftWnd::~CLiftWnd()
{
	m_LiftBmp.DeleteObject();
}


BEGIN_MESSAGE_MAP(CLiftWnd, CWnd)
	//{{AFX_MSG_MAP(CLiftWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLiftWnd message handlers

void CLiftWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
    CDC MemDC;         // Memory DC 
    CBitmap* pOldBmp;  // Save old bitmap selected out of DC
    
    // Create a memory DC compatible with window DC
    MemDC.CreateCompatibleDC(&dc);
    
    // Select bitmap into memory DC
    pOldBmp=MemDC.SelectObject(&m_LiftBmp);
    
    // copy the proper frame into the window
	if (m_Timer) 
	{
		dc.BitBlt(0, 0, LIFT_WIN_WIDE, LIFT_WIN_HIGH, &MemDC,
			(m_LiftPosition == down? m_TimerCnt : m_Frames - 1 - m_TimerCnt)*LIFT_WIN_WIDE,
			0, SRCCOPY);
	} 
	else 
	{
		dc.BitBlt(0, 0, LIFT_WIN_WIDE, LIFT_WIN_HIGH, &MemDC,
			(m_LiftPosition==down? m_Frames-1:0)*LIFT_WIN_WIDE,0,SRCCOPY);
	}
	if (m_Timer && m_TimerCnt == (m_Frames - 1)) 
	{	// kill the animation timer
		if(!KillTimer(m_Timer))
		{
			TRACE(_T("Error killing Timer"));                
		}
        m_Timer = 0;    
        m_TimerCnt=0;   
            
        // Set Class Cursor to the normal arrow cursor
		::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,(LONG)m_ArrowCursor);
            
        // If cursor is not moved by user it does not get redrawn
        // after SetClassWord - need to force redraw
        POINT FAR CursorPoint;
        ::GetCursorPos(&CursorPoint);     
        ::SetCursorPos(CursorPoint.x,CursorPoint.y);
	}   
    
    MemDC.SelectObject(pOldBmp);    
	
	// Do not call CWnd::OnPaint() for painting messages
}


BOOL CLiftWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
    CDC MemDC;          
    CBitmap* pOldBmp;
    
    // Create a memory DC compatible with window DC
    MemDC.CreateCompatibleDC(pDC);
    
    // Select bitmap into memory DC
    pOldBmp=MemDC.SelectObject(&m_LiftBmp);
    
    // BitBlt the first or the last frame into background
	if (m_LiftPosition == down) 
	{
		pDC->BitBlt(0, 0, LIFT_WIN_WIDE, LIFT_WIN_HIGH, &MemDC, m_Frames * LIFT_WIN_WIDE, 0, SRCCOPY);
	}
	else 
	{
		pDC->BitBlt(0, 0, LIFT_WIN_WIDE, LIFT_WIN_HIGH, &MemDC, 0, 0, SRCCOPY);
	}
   
    MemDC.SelectObject(pOldBmp);    
    
    return TRUE;
}

void CLiftWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    // Increment the timer event counter
    m_TimerCnt++;      
    
    // Invalidate the lift window
	Invalidate();    
	
	CWnd::OnTimer(nIDEvent);
}


// Start moving the lift up
int CLiftWnd::StartLiftMove (DIRECTION dir)
{
	if ((dir==up && m_LiftPosition==down) || (dir==down && m_LiftPosition==up)) 
	{
		m_LiftPosition = dir;
		StartAnimation();
	} 
	else 
	{	// Just repaint the screen
		Invalidate();
	}

	return KO_OK;
}	


// Start the move animation
void CLiftWnd::StartAnimation()
{
	if (m_Timer) 
	{	// if the timer is running, kill it
		KillTimer(m_Timer);
	}

	// Set a timer for 100 msec
	m_TimerCnt = 1;
	m_Timer = SetTimer(1, 100, NULL);
	ASSERT(m_Timer != 0);     
	Invalidate();    

    // Set class cursor to empty cursor to eliminate
    // flicker when we bitblt
	::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,(LONG)m_NullCursor);
}

DIRECTION CLiftWnd::LiftPosition()
{
	return m_LiftPosition;
}

void CLiftWnd::ForcePosition(DIRECTION pos)
{
	m_LiftPosition = pos;
	Invalidate();
}

BOOL CLiftWnd::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (m_Timer)
	{	// if the timer is running, kill it
		KillTimer(m_Timer);
	}
	
	return CWnd::DestroyWindow();
}
