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

// LucyView.cpp : implementation of the CLucyView class
//

#include "stdafx.h"
#include "resource.h"
#include "Lucept.h"
#include "MainFrm.h"
#include "WklDoc.h"
#include "WklView.h"

#include "LucyDef.h"
#include "LCCUser.h"
#include "resource.h"       // main symbols

#include "LucyDoc.h"
#include "LucyView.h"
#include "HandCtrl.h"
#include "LuceErr.h"
#include "UsrAdmin.h"
#include "MProcDlg.h"

#include "Specification.h"

#include "WklMods.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Declare messages
UINT NEAR WM_PUPDATE_LUCY
	= RegisterWindowMessage(_T("LuceptUpdateLucy"));

#include "TubeMap.h"

/////////////////////////////////////////////////////////////////////////////
// CLucyView

IMPLEMENT_DYNCREATE(CLucyView, CView)

BEGIN_MESSAGE_MAP(CLucyView, CView)
	//{{AFX_MSG_MAP(CLucyView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_NEWWORKLOAD, OnNewWorkload)
	ON_BN_CLICKED(IDC_MAINTENANCE, OnMaintenance)
	ON_BN_CLICKED(IDC_MANUALMODE, OnManualMode)
	ON_BN_CLICKED(IDC_LOGOUT, OnLogOut)
	ON_BN_CLICKED(IDC_GRIDVIEW, OnGridView)
	ON_BN_CLICKED(IDC_HELPBUTTON, OnHelp)
	ON_BN_CLICKED(IDC_SCANCHAIN, OnScanChain)
	ON_BN_CLICKED(IDC_STOP, OnStopRun)
    ON_REGISTERED_MESSAGE(WM_PUPDATE_LUCY, OnPeriodicUpdateLucy)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_USER, OnUpdateUserStatusPane)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SHOWWINDOW()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLucyView construction/destruction

CLucyView::CLucyView()
{
	m_pWklDoc = NULL;
	m_pRunningWklView = NULL;
	m_bTicking = false;

	switch( Specification::g_oSpecification.GetInstrumentType()) {
		default:
			{
				m_bmpDest = CRect(0,0,0,0);
			//	m_bmpBackground.LoadBitmapW( IDB_BACKGROUND_BMP);
				m_bmpBackground.LoadBitmapW( Specification::g_oSpecification.GetBackgroundBitmap());
				ASSERT( m_bmpBackground.GetSafeHandle() != NULL);

				long nTubesBitmapID = Specification::g_oSpecification.GetTubesBitmap();

				if( nTubesBitmapID != 0) {
					m_bmpBigIcon.LoadBitmapW( nTubesBitmapID);
				}
			}
			break;

		case INSTRUMENT_ADVANCE_2:
			m_oTubeDisplay.LoadImages( this);
			break;
	}
}

CLucyView::~CLucyView()
{
	if( m_bmpBigIcon.GetSafeHandle() != NULL) {
		m_bmpBigIcon.DeleteObject();
	}

	m_bmpBackground.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CLucyView drawing

BOOL CLucyView::OnEraseBkgnd( CDC* pDC)
{
	// Don't erase the background as OnDraw will fill it all
	return TRUE;
}

void CLucyView::OnDraw(CDC* pDC)
{
	switch( Specification::g_oSpecification.GetInstrumentType()) {
		case INSTRUMENT_UNKNOWN:
		case INSTRUMENT_C3:
		default:
			return;

		case INSTRUMENT_ADVANCE_SEDAN:
		case INSTRUMENT_ADVANCE_COUPE:
			DrawSedan( pDC);
			break;

		case INSTRUMENT_ADVANCE_2:
			DrawAdvance2( pDC);
			break;
	}
}

/**
 * Draw sedan and coupe old style tube display.
 * @param pDC Device Context to draw on.
 */
void CLucyView::DrawSedan( CDC *pDC)
{
	CBrush NewBrush;
		
	CLucyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect clientRect;
	GetClientRect(clientRect);

	if( clientRect.Width() > 0) {
		CDC oMemDC;
		oMemDC.CreateCompatibleDC( pDC);
		oMemDC.SelectObject( m_bmpBackground);

		BITMAP bm;
		m_bmpBackground.GetObject( sizeof( BITMAP), &bm);

		// nb the GetDimensions member of m_bmpBackground returns the highly informative value (0,0)
		CSize oDimensions = CSize( bm.bmWidth, bm.bmHeight);

		float nScreenAspect = (float)clientRect.Height() / (float)clientRect.Width();
		float nBitmapAspect = (float)oDimensions.cy / oDimensions.cx;

		if( nScreenAspect > nBitmapAspect) {
			// The screen is taller/thinner than the image: the image should be trimmed along its right edge to fit.
			 int nSrcWidth = (int)(oDimensions.cx * nBitmapAspect / nScreenAspect);

			pDC->StretchBlt( 0,0, clientRect.Width(), clientRect.Height(), &oMemDC, (oDimensions.cx - nSrcWidth) / 2, 0, nSrcWidth, oDimensions.cy, SRCCOPY);
		} else {
			// The screen is shorter/fatter than the image: the image should be trimmed along its bottom edge to fit.
			int nSrcHeight = (int)(oDimensions.cy * nScreenAspect / nBitmapAspect);

			pDC->StretchBlt( 0,0, clientRect.Width(), clientRect.Height(), &oMemDC, 0, (oDimensions.cy - nSrcHeight) / 2, oDimensions.cx, nSrcHeight, SRCCOPY);
		}
	}

	if( m_bmpBigIcon.GetSafeHandle() != NULL) { // not C3 which doesn't display tubes
		CDC oMemDC;
		oMemDC.CreateCompatibleDC( pDC);
		oMemDC.SelectObject( m_bmpBigIcon);

		BITMAP bm;
		m_bmpBigIcon.GetObject( sizeof( BITMAP), &bm);

		// nb the GetDimensions member of m_bmpBackground returns the highly informative value (0,0)
		CSize oDimensions = CSize( bm.bmWidth, bm.bmHeight);

		m_bmpDest.top = max(0, (clientRect.Height() - oDimensions.cy) / 2);
		m_bmpDest.left = max(0, (clientRect.Width() - oDimensions.cx) / 2);
		m_bmpDest.right = m_bmpDest.left + oDimensions.cx;
		m_bmpDest.bottom = m_bmpDest.top + oDimensions.cy;

		pDC->BitBlt( m_bmpDest.left, m_bmpDest.top, m_bmpDest.Width(), m_bmpDest.Height(), &oMemDC, 0, 0, SRCCOPY);

		// Draw all the tubes
		// Offset the same as BMP
		pDC->OffsetViewportOrg(m_bmpDest.left, m_bmpDest.top);
		int nTubes = m_tubeList.GetCount();
		for (int iTube = 0; iTube < nTubes; iTube++)
		{
			m_tubeList[iTube].Draw(pDC);
		}
		// Restore origin
		pDC->OffsetViewportOrg(-m_bmpDest.left, -m_bmpDest.top);
	}
}

/**
 * Draw Sdvance 2 style tube display.
 * @param pDC Device Context to draw on.
 */
void CLucyView::DrawAdvance2( CDC *pDC)
{
	m_oTubeDisplay.Draw( this, pDC, m_tubeList);
}

/////////////////////////////////////////////////////////////////////////////
// CLucyView Initial Update

void CLucyView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	long nLCCState = RegisterWithLcc();

	VERIFY(nLCCState == KO_OK || nLCCState == KO_LUC_LCC_UNCONNECTED);

	GetLucyBar()->SetDocument((CLucyDoc*)GetDocument());

	// Set the window size
    GetParentFrame()->RecalcLayout();

	m_bTicking = true;

	// Start with no running workloads
	m_TotalTime = 0;
	m_EstSecsLeft = 0;
	m_pRunningWklView = NULL;

	// Register with tooltip control
	VERIFY(EnableToolTips(true));

	// Select palette into the foreground
	OnPaletteChanged(this);

	InitTubes();
}


void CLucyView::OnDestroy()
{
	UnregisterWithLcc();

   CView::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CLucyView diagnostics

#ifdef _DEBUG
void CLucyView::AssertValid() const
{
	CView::AssertValid();
}

void CLucyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLucyDoc* CLucyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLucyDoc)));
	return (CLucyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLucyView message handlers

// This function tests to see if the mouse is clicked on an item 
// and if so selects the item, and performs any user action
// Uses the CRectTracker to determine the item and the resize/move the item.

void CLucyView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// Button New Wrokload Handler
/////////////////////////////////////////////////////////////////////////////
void CLucyView::OnNewWorkload() 
{	// Create a new workload document
	(((CLuceptApp*)AfxGetApp())->m_pWklViewTemplate)->OpenDocumentFile(NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Button Manual Mode Handler
/////////////////////////////////////////////////////////////////////////////
void CLucyView::OnManualMode() 
{
	CHandCtrl	dialog;
	
	dialog.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// Button Log Off Handler
/////////////////////////////////////////////////////////////////////////////
void CLucyView::OnLogOut() 
{
	gGateKeeper.UserLogOn();
}

/////////////////////////////////////////////////////////////////////////////
// Button Gridview Handler
/////////////////////////////////////////////////////////////////////////////
void CLucyView::OnGridView() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_GRID_VIEW);
}

/////////////////////////////////////////////////////////////////////////////
// Button Help Handler
/////////////////////////////////////////////////////////////////////////////

void CLucyView::OnHelp() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_HELP);
}

/////////////////////////////////////////////////////////////////////////////
// Button Scan Chain Handler
/////////////////////////////////////////////////////////////////////////////
void CLucyView::OnScanChain() 
{
	// Create a new workload document
	CDocument *theDoc = (((CLuceptApp*)AfxGetApp())->m_pWklViewTemplate)->OpenDocumentFile(NULL);
	if (theDoc != NULL) 
	{	// Get the view pointer
		POSITION pos = theDoc->GetFirstViewPosition();
		CWklView * theView = (CWklView *)theDoc->GetNextView(pos);
		ASSERT(theView->IsKindOf(RUNTIME_CLASS(CWklView)));
		theView->ScanChain();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Button Stop Run Handler
/////////////////////////////////////////////////////////////////////////////
void CLucyView::OnStopRun() 
{
	if (m_pRunningWklView != NULL) 
	{
		ASSERT(m_pRunningWklView->IsKindOf(RUNTIME_CLASS(CWklView)));
		m_pRunningWklView->StopPause();
	}
}

////////////////////////////////////////////////////////////////////////
// Add a new tube to the list
////////////////////////////////////////////////////////////////////////
void CLucyView::InitTubes() 
{
	int nTubeVectors;
	STubeVector* tubeMap;

	// Clear list
	m_tubeList.DeleteAll();
	
	// Read luminometer model from INI file
	CString model = 
		((CLuceptApp *)AfxGetApp())->GetLuceptProfileString(_T("Luminometer"), _T("Model"), _T(""));
	if (model.CompareNoCase(_T("Coupe")) == 0)
	{	// It's a coupe
		tubeMap = coupeTubeMap;
		nTubeVectors = sizeof(coupeTubeMap) / sizeof(STubeVector);
	}
	else
	{	// Default luminometer is Sedan
		tubeMap = sedanTubeMap;
		nTubeVectors = sizeof(sedanTubeMap) / sizeof(STubeVector);
	}

	// Process each vector
 	for (int iVect = 0; iVect < nTubeVectors; iVect++)
	{	// Calculate placement range
		int rangeX = tubeMap[iVect].endPoint.x - 
					 tubeMap[iVect].startPoint.x;
		int rangeY = tubeMap[iVect].endPoint.y - 
					 tubeMap[iVect].startPoint.y;

		// Calculate size range
		int rangeWidth = tubeMap[iVect].lastSize.cx - 
					 tubeMap[iVect].firstSize.cx;
		int rangeHeight = tubeMap[iVect].lastSize.cy - 
					 tubeMap[iVect].firstSize.cy;

		// Process each tube
		for (int iTube = 0; iTube < tubeMap[iVect].nTubes; iTube++)
		{
			int x, y, width, height;

			// Create tube
			CTube* pTube = m_tubeList.Append();

			// Do  Placement
			if (tubeMap[iVect].nTubes != 1)
			{
				x = iTube * rangeX / (tubeMap[iVect].nTubes - 1) + 
					tubeMap[iVect].startPoint.x;
				y = iTube * rangeY / (tubeMap[iVect].nTubes - 1) + 
					tubeMap[iVect].startPoint.y;
				pTube->SetPos(x, y);

				// Do Sizing
				width = iTube  * rangeWidth / (tubeMap[iVect].nTubes - 1) + 
					tubeMap[iVect].firstSize.cx;
				height = iTube * rangeHeight / (tubeMap[iVect].nTubes - 1) + 
					tubeMap[iVect].firstSize.cy;
				pTube->SetSize(width, height);
			}
			else
			{
				x = tubeMap[iVect].startPoint.x;
				y = tubeMap[iVect].startPoint.y;
				pTube->SetPos(x, y);

				// Do Sizing
				width = tubeMap[iVect].firstSize.cx;
				height = tubeMap[iVect].firstSize.cy;
				pTube->SetSize(width, height);
			}
			
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Window Destroy
/////////////////////////////////////////////////////////////////////////////
BOOL CLucyView::DestroyWindow() 
{
	return CView::DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////
// The timer tick
/////////////////////////////////////////////////////////////////////////////
void CLucyView::TickHandler()
{
	static int timer=4;

	if (!m_bTicking)
	{
		return;
	}

	if (m_EstSecsLeft > 0) 
	{
		CString text;
		m_EstSecsLeft--;
		text.Format(_T("%01ld:%02ld:%02ld"), m_EstSecsLeft/3600, 
			(m_EstSecsLeft/60)%60, m_EstSecsLeft%60);
		GetLucyBar()->m_timeDisp.SetDispText(text);
	}

	// Only process this every five seconds
	timer = (timer + 1) % 5;
	if (timer) 
	{
		return;
	}
	
	// Send a message to the updates
	PostMessage(WM_PUPDATE_LUCY);
}


/////////////////////////////////////////////////////////////////////////////
// Process notification of a periodic update update
/////////////////////////////////////////////////////////////////////////////
LRESULT CLucyView::OnPeriodicUpdateLucy (WPARAM wParam, LPARAM lParam)
{
	double temp;
	CString text;
	
	// Set Temperature Display
	if (Inquire(LCC_INQ_TEMP, &temp) == KO_OK) 
	{
		text.Format(_T("%3.1fC"), temp);
		GetLucyBar()->m_tempDisp.SetDispText(text);
		((CLuceptApp*)AfxGetApp())->SetCurrentTemp( temp);
	}

	// Resynch and set time display
	if (m_TotalTime > 0) 
	{
		long base_time, fract_time;

		// Read time remaining on current batch
		if (CSGetTimeLeft(&fract_time) == KO_OK) 
		{	// Read estimated time for following batches
			if (m_pRunningWklView->GetWklTimeLeft(&base_time) == KO_OK) 
			{
				m_EstSecsLeft = (base_time + fract_time) / 1000;

				text.Format(_T("%01ld:%02ld:%02ld"), m_EstSecsLeft/3600, 
					(m_EstSecsLeft/60)%60, m_EstSecsLeft%60);
				GetLucyBar()->m_timeDisp.SetDispText(text);
			}
		}
	}
	else 
	{
		GetLucyBar()->m_timeDisp.SetDispText(_T("0:00:00"));
	}

	return (LRESULT)TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// A workload within a control session will register itself here
/////////////////////////////////////////////////////////////////////////////
long CLucyView::RegisterWorkload(CWklView* Workload)
{
	int rv = KO_OK;
	long fract_time;

	if (Workload == NULL)
	{
		rv = KO_LUC_BAD_WKLVIEW_PTR;
	}
	else if(m_pRunningWklView != NULL) 
	{
		rv = KO_LUC_WKLVIEW_REGD;
	}
	else 
	{
		SetWklDoc(Workload->GetDocument());
		m_pRunningWklView = Workload;

		// Disable utility buttons
		GetLucyBar()->m_butStopRun.EnableWindow(TRUE);

		rv = m_pRunningWklView->GetWklTimeLeft(&m_TotalTime);
		if (rv != KO_OK) 
		{
			m_TotalTime = 0;
		}
		else if ((rv = CSGetTimeLeft(&fract_time)) == KO_OK) 
		{
			m_TotalTime += fract_time;
		}

		m_TotalTime += 
			m_pRunningWklView->GetDocument()->GetBatchPtr(0)->m_mSec;
		m_EstSecsLeft = m_TotalTime / 1000;
	}

	return rv;
}

/////////////////////////////////////////////////////////////////////////////
// Unregister the public procedure to update the reagent levels
/////////////////////////////////////////////////////////////////////////////
long CLucyView::UnregisterWorkload(CWklView* Workload)
{
	int rv=KO_OK;

	if (Workload == NULL) 
	{
		rv = KO_LUC_BAD_WKLVIEW_PTR;
	}
	else if (Workload != m_pRunningWklView) 
	{
		rv = KO_LUC_WKLVIEW_NOT_REGD;
	}
	else 
	{
		m_pRunningWklView = NULL;
		m_TotalTime = 0;
		m_EstSecsLeft = 0;

		// Enable utility buttons
		GetLucyBar()->m_butStopRun.EnableWindow(FALSE);

		LccMsgHandler(CString(_T(" Run Complete")));
	}

	return rv;
}


////////////////////////////////////////////////////////////////////////
// Required to show current user in status bar
////////////////////////////////////////////////////////////////////////
void CLucyView::OnUpdateUserStatusPane(CCmdUI* pCmdUI)
{	
	//calls CMainFrame member function
	((CMainFrame*) AfxGetApp()->m_pMainWnd)->
		OnUpdateUserStatusPane(pCmdUI);
}

/////////////////////////////////////////////////////////////////////////////
// Button Maintenance Handler
/////////////////////////////////////////////////////////////////////////////
void CLucyView::OnMaintenance() 
{
	CMaintProcDlg	dlg;

	dlg.DoModal();
}

bool CLucyView::SetWklDoc(CWklDoc * pDoc)
{
	// If no registered workload use specified
	if(m_pRunningWklView == NULL)
	{
		if (pDoc != m_pWklDoc)
		{	// Unregister with old workload
			if (m_pWklDoc != NULL)
			{
				m_pWklDoc->RegisterLucy(false);
			}

			// Save new doc pointer
			m_pWklDoc = pDoc;
			if (m_pWklDoc != NULL)
			{	// register if it is not null
				m_pWklDoc->RegisterLucy();
			}
	
			UpdateTubes();
		}
		// else already registered
		return true;
	}
	else
	{
		return false;
	}
}

void CLucyView::OnWklUpdate(CWklMods* pHint)
{
	// Update only if visible
	WINDOWPLACEMENT place;
	GetWindowPlacement(&place);
	if (place.showCmd == SW_HIDE)
	{
		return;
	}

	if (m_pWklDoc == NULL)
	{
		return;
	}

	if (pHint != NULL)
	{
		ASSERT(pHint->IsKindOf(RUNTIME_CLASS(CWklMods)));

		switch(pHint->GetNature())
		{
		case CWklMods::NATURE_WKL_STAT:
		case CWklMods::NATURE_DEL_BATCH:
		case CWklMods::NATURE_ASSAY:
		case CWklMods::NATURE_ADD_CALVAL:
			UpdateTubes();
			break;

		case CWklMods::NATURE_WRITE:
			break;

		case CWklMods::NATURE_ADD_BATCH:
			// Batch deletion
			if (pHint->GetBatch() != LUC_BATCH_UNDEF)
			{
				UpdateTubes();
			}
			else
				UpdateTubes();
			break;

		case CWklMods::NATURE_BATCH_STAT:
			if (pHint->GetScope() == CWklMods::SCOPE_BATCH)
			{
				UpdateTubes();
			}
			else
			{	// Scope is whole workload
				UpdateTubes();
			}
			break;

		case CWklMods::NATURE_MOD:
			if (pHint->GetScope() == CWklMods::SCOPE_WORKLOAD)
			{	// Whole workload
				UpdateTubes();
			}
			else if (pHint->GetScope() == CWklMods::SCOPE_BATCH)
			{	// Single batch
				UpdateTubes();
			}
			else if(pHint->GetScope() == CWklMods::SCOPE_SAMPLE)
			{
				UpdateTubes();
			}
			break;

		default:
			UpdateTubes();
		}
	}
	else
	{	// No hint, do everything
		UpdateTubes();
	}
}


void CLucyView::OnWklDocClose(CWklDoc* pDoc)
{
	if (pDoc == m_pWklDoc)
	{
		m_pWklDoc = NULL;
		UpdateTubes();
	}
}

void CLucyView::UpdateTubes(int iLink)
{
	
	if (iLink == LUC_LINK_UNDEF)
	{
		iLink = 0;
		int iBatch, iSamp, iRepl;
		long rv = KO_OK;

		if (m_pWklDoc != NULL)
		{	// first tube
			rv = m_pWklDoc->LocateFirstLink(iLink, &iBatch, &iSamp, &iRepl);
			if (rv == KO_OK)
			{
				m_tubeList[iLink].SetColor(m_pWklDoc->GetTubeColor(iBatch, iSamp, iRepl));
			}
			else
			{
				m_tubeList[iLink].SetColor(LUC_COLOR_RO_ABSENT);
			}
			iLink++;

			// remaining tubes
			for (; iLink < m_tubeList.GetCount(); iLink++)
			{
				rv = m_pWklDoc->LocateNextLink(iLink, &iBatch, &iSamp, &iRepl);
				if (rv == KO_OK)
				{
					m_tubeList[iLink].SetColor(m_pWklDoc->GetTubeColor(iBatch, iSamp, iRepl));
				}
				else
				{
					m_tubeList[iLink].SetColor(LUC_COLOR_RO_ABSENT);
				}
			}
		}
		else
		{	// No workload attached
			for (; iLink < m_tubeList.GetCount(); iLink++)
			{
				m_tubeList[iLink].SetColor(LUC_COLOR_RO_ABSENT);
			}
		}

		// Redraw window
		Invalidate(false);
	}
	else
	{
		if (m_pWklDoc == NULL)
		{
			m_tubeList[iLink].SetColor(LUC_COLOR_RO_ABSENT);
			m_tubeList[iLink].Invalidate(this, m_bmpDest);
		}
		else
		{
			m_tubeList[iLink].SetColor(m_pWklDoc->GetLinkColor(iLink));
			m_tubeList[iLink].Invalidate(this, m_bmpDest);
		}
	}
}

CWklDoc* CLucyView::GetWklDoc()
{
	return m_pWklDoc;
}


void CLucyView::OnPaletteChanged(CWnd * pFocusWnd)
{	// Repaint
	Invalidate();
}


void CLucyView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  	CLucyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
	int nTube = -1;

	switch( Specification::g_oSpecification.GetInstrumentType()) {
		case INSTRUMENT_UNKNOWN:
		case INSTRUMENT_C3:
		default:
			return;

		case INSTRUMENT_ADVANCE_SEDAN:
		case INSTRUMENT_ADVANCE_COUPE:
			{
				int nTubes = m_tubeList.GetCount();
				point.Offset(-m_bmpDest.left, -m_bmpDest.top);
				for (int iTube = 0; iTube < nTubes; iTube++) {
					if (m_tubeList[iTube].PtInRect(point)) {
						nTube = iTube;
						break;
					}
				}
			}
			break;

		case INSTRUMENT_ADVANCE_2:
			nTube = m_oTubeDisplay.FindNearestTube( point);
			break;
	}

#ifdef _DEBUG
	if( GetKeyState( VK_SHIFT) & 0x8000) {
		CString strMsg;
		strMsg.Format( _T( "Tube %d"), nTube);
		AfxMessageBox( strMsg);
	}
#endif

	if( nTube != -1) {

		if (m_pWklDoc != NULL) {
			long rv;
			int iBatch, iSamp, iRepl;
			rv = m_pWklDoc->LocateLink( nTube, &iBatch, &iSamp, &iRepl);
			if (rv == KO_OK) {
				m_pWklDoc->SetCurBatch( iBatch, this);
				m_pWklDoc->SetCurSample( iSamp, this);
				((CMainFrame*)AfxGetMainWnd())->OnSampleView();
			}
		}
	}
	
	CView::OnLButtonDblClk(nFlags, point);
}

void CLucyView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CView::OnShowWindow(bShow, nStatus);
	
	// Update when becoming visible
	if (bShow)
	{
		UpdateTubes();	
	}
}

