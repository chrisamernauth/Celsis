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

#include "stdafx.h"
#include "lucept.h"

#include "GnDfPrVw.h"
#include "GnDfPrDc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenDefPrView
//
IMPLEMENT_DYNCREATE(CGenDefPrView, CScrollView)

CGenDefPrView::CGenDefPrView() 
{
	m_bottomMarginLU	=
	m_topMarginLU		= 
	m_rightMarginLU		= 
	m_leftMarginLU		= 1270; // == 1/2 inch

	m_bodyYLU			= 
	m_lines_per_page	= 0;

	m_lpszHeader  = _T("GenDef Report");
	m_lpszFooterL = _T("Page %d of %d");
}

CGenDefPrView::~CGenDefPrView()
{
}

BEGIN_MESSAGE_MAP(CGenDefPrView, CScrollView)
	//{{AFX_MSG_MAP(CGenDefPrView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT,		  CGenDefPrView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT,  CGenDefPrView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CGenDefPrView::OnFilePrintPreview)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGenDefPrView drawing
/////////////////////////////////////////////////////////////////////////////
// CGenDefPrView message handlers

void CGenDefPrView::OnInitialUpdate() 
{
	// Initialize document data
	CGenDefPrDoc* pDoc = (CGenDefPrDoc*)GetDocument();
	pDoc->InitPrintData();
	int chars = pDoc->m_maxchars;
	int lines = pDoc->m_lines.GetCount();

	// Get text metrics
	CDC *pDC;
	TEXTMETRIC tm;
	VERIFY(pDC = GetDC());
	VERIFY(pDC->GetTextMetrics(&tm));
	VERIFY(ReleaseDC(pDC));

	// Calculate the total size of the onscreen view
	CSize sizeTotal(70 * tm.tmAveCharWidth, lines * tm.tmHeight);  //70 == 1/2 margins
	CSize sizeLine(sizeTotal.cx, tm.tmHeight);
	CSize sizePage(sizeTotal.cx, tm.tmHeight * 25);	//an onscreen page
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
}

void CGenDefPrView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
}

void CGenDefPrView::OnDraw(CDC* pDC)
{
	CGenDefPrDoc* pDoc = (CGenDefPrDoc*)GetDocument();
	
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);		
	long  left_margin = 0, right_margin = 0, y = 0;
	left_margin  =  5 * tm.tmAveCharWidth; //1/2" margins
	right_margin = 70 * tm.tmAveCharWidth; //80 columns dispay
		
	//loop 
	POSITION pos = pDoc->m_lines.GetHeadPosition();
	ASSERT(pos);
	CGenDefPrDoc::CPrintLine pl;
	while(pos)
	{
		pl = pDoc->m_lines.GetNext(pos);

		//align label left
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->TextOut(left_margin, y, pl.lbl);

		//align value right
		pDC->SetTextAlign(TA_RIGHT|TA_TOP);
		pDC->TextOut(right_margin, y, pl.txt);
			
		//increment line
		y += tm.tmHeight;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGenDefPrView diagnostics

#ifdef _DEBUG
void CGenDefPrView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGenDefPrView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

void CGenDefPrView::OnDestroy() 
{
	CScrollView::OnDestroy();

	//NULL out the view pointer
	((CLuceptApp*)AfxGetApp())->m_pAssayPrView = NULL;
}

void CGenDefPrView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	pInfo->SetMaxPage(PrintPageSetup(pDC, pInfo));
}

long CGenDefPrView::PrintPageSetup(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(pDC->GetMapMode() == MM_TEXT);
	
	int availYLU	   = 0;
	int lines_per_page = 0;
	int required_pages = 0;
	long old_map_mode  = 0;
	TEXTMETRIC tm;

	CGenDefPrDoc *pDoc = (CGenDefPrDoc*)GetDocument();
	ASSERT(pDoc);

	//get selected paper size
	CSize cSize;
	cSize.cx = (pDC->GetDeviceCaps(HORZSIZE) * 100);//in mm, so * 100 = HIMETRIC
	cSize.cy = (pDC->GetDeviceCaps(VERTSIZE) * 100); 
	//pDC->HIMETRICtoLP(&cSize);

	m_TotalPageSizeXLU = cSize.cx;
	m_TotalPageSizeYLU = cSize.cy;

	//get text size, in himetrics
	VERIFY(pDC->GetTextMetrics(&tm));
	
	//convert text metrics (in pixels) to HIMETRICS
	cSize.cx = tm.tmAveCharWidth;
	cSize.cy = tm.tmHeight;
	pDC->DPtoHIMETRIC(&cSize);	
	availYLU = m_TotalPageSizeYLU - 
		((m_topMarginLU   + (2 * cSize.cy))   //top margin + header + space
		+ 
		(m_bottomMarginLU + (2 * cSize.cy))); //bottom margin + header + space

	//set body start point
	m_bodyYLU = m_topMarginLU   + (2 * cSize.cy);

	//get required pages
	m_lines_per_page = availYLU/cSize.cy;
	required_pages = pDoc->m_lines.GetCount()/m_lines_per_page;
	if(pDoc->m_lines.GetCount() % m_lines_per_page)
	{ //get the fractional page
		required_pages++;
	}
	
	return required_pages;
}

void CGenDefPrView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
}

void CGenDefPrView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

void CGenDefPrView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);
}

BOOL CGenDefPrView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return 	DoPreparePrinting(pInfo);
}

void CGenDefPrView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{		
	PrintHeader(pDC, pInfo);
	PrintBody(pDC, pInfo);
	PrintFooter(pDC, pInfo);
}

LRESULT CGenDefPrView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CScrollView::WindowProc(message, wParam, lParam);
}

void CGenDefPrView::PrintHeader(CDC *pDC, CPrintInfo* pInfo)
{
	int leftX;
	int hdrY;
	CSize tmp;

	//get xpoint in device units
	tmp.cx = m_leftMarginLU;
	pDC->HIMETRICtoLP(&tmp);
	leftX = tmp.cx;

	//get ypoint in device units
	tmp.cy = m_topMarginLU;
	pDC->HIMETRICtoLP(&tmp);
	hdrY = tmp.cy;
	
	// Print the header
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOut(leftX,	 hdrY, m_lpszHeader);
}

void CGenDefPrView::PrintFooter(CDC *pDC, CPrintInfo* pInfo)
{
	int ftrY, leftX, rightX;
	CSize tmp;

	//get left xpoint 
	tmp.cx = m_leftMarginLU;
	pDC->HIMETRICtoLP(&tmp);
	leftX = tmp.cx;

	//get ypoint in device units
	tmp.cy = m_TotalPageSizeYLU - m_bottomMarginLU;
	pDC->HIMETRICtoLP(&tmp);
	ftrY = tmp.cy;

	//left xpoint
	tmp.cx = m_TotalPageSizeXLU - m_rightMarginLU;
	pDC->HIMETRICtoLP(&tmp);
	rightX = tmp.cx;
	
	CString S;
	S.Format(m_lpszFooterL, pInfo->m_nCurPage, pInfo->GetMaxPage());

	// Print the left footer
	pDC->SetTextAlign(TA_LEFT|TA_BOTTOM);
	pDC->TextOut(leftX,	 ftrY, S);

	S.Format(_T("Printed %s"), COleDateTime::GetCurrentTime().Format(_T("%x %X")));

	pDC->SetTextAlign(TA_RIGHT|TA_BOTTOM);
	pDC->TextOut(rightX, ftrY, S);

	//restore
	pDC->SetTextAlign(TA_LEFT|TA_TOP); 
}

void CGenDefPrView::PrintBody(CDC *pDC, CPrintInfo *pInfo)
{
	int  index   = 0;
	int ln_count = 0;
	long y       = 0;
	long xLeft   = 0;
	long xRight  = 0;
	
	CGenDefPrDoc *pDoc = (CGenDefPrDoc*)GetDocument();
	
	//convert from HIMETRICS
	CSize cs;
	cs.cx = m_leftMarginLU;
	cs.cy = m_bodyYLU;
	pDC->HIMETRICtoLP(&cs);	
	y	  = cs.cy;
	xLeft = cs.cx;

	// get right margin
	cs.cx = m_TotalPageSizeXLU - m_rightMarginLU;
	pDC->HIMETRICtoLP(&cs);
	xRight = cs.cx;

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	
	//find start point for the current page
	index = (pInfo->m_nCurPage - 1) * m_lines_per_page; 
	POSITION pos = pDoc->m_lines.FindIndex(index);
	ASSERT(pos);
	CGenDefPrDoc::CPrintLine pl;

	//loop for one page
	while(pos && (ln_count++ < m_lines_per_page))
	{
		pl = pDoc->m_lines.GetNext(pos);

		//align label left
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->TextOut(xLeft,  y, pl.lbl);

		//align value right
		pDC->SetTextAlign(TA_RIGHT|TA_TOP);
		pDC->TextOut(xRight, y, pl.txt);
			
		//increment line
		y += tm.tmHeight;
	}
}
