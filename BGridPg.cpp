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

// BGridPg.cpp : implementation file
//

#include "stdafx.h"
#include "Lucept.h"
#include "WklView.h"
#include "Batch.h"
#include "BGridPg.h"
#include "BTabCtrl.h"
#include "IniFiles.h"
#include "UsrAdmin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatchGridPage dialog

IMPLEMENT_DYNAMIC(CBatchGridPage, CDialog)

CBatchGridPage::CBatchGridPage(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchGridPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBatchGridPage)
	m_batStat = _T("");
	m_calValName1 = _T("");
	m_calValName2 = _T("");
	m_calValVal1 = _T("");
	m_calValVal2 = _T("");
	m_allowMerge = TRUE;
	//}}AFX_DATA_INIT

	m_pBatch = NULL;
	m_iDataSource = UNDEF_DATA_SOURCE;
}


void CBatchGridPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchGridPage)
	DDX_Control(pDX, IDC_ASSAYBOX_BGP, m_assayBox);
	DDX_Text(pDX, IDC_BAT_STAT, m_batStat);
	DDX_Text(pDX, IDC_CALVAL1_NAME, m_calValName1);
	DDX_Text(pDX, IDC_CALVAL2_NAME, m_calValName2);
	DDX_Text(pDX, IDC_CALVAL1_VAL, m_calValVal1);
	DDX_Text(pDX, IDC_CALVAL2_VAL, m_calValVal2);
	DDX_Check(pDX, IDC_ALLOW_MERGE, m_allowMerge);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchGridPage, CDialog)
	//{{AFX_MSG_MAP(CBatchGridPage)
	ON_WM_SIZE()
	ON_CBN_SELENDOK(IDC_ASSAYBOX_BGP, OnSelectassay)
	ON_WM_DESTROY()
//	ON_WM_KEYUP()
//	ON_WM_KEYDOWN()
//	ON_WM_VKEYTOITEM()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ALLOW_MERGE, OnAllowMerge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBatchGridPage message handlers

BOOL CBatchGridPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add batch grid
#ifdef DO_UG
	RECT	gridRect;

	GetDlgItem(IDC_GRID_AREA)->GetWindowRect(&gridRect);
	ScreenToClient(&gridRect);
	m_grid.CreateGrid(WS_CHILD | WS_VISIBLE 
		| WS_TABSTOP | WS_BORDER, gridRect, this, IDC_GRID_AREA);
#else
	GetDlgItem(IDC_GRID_AREA)->ShowWindow( SW_HIDE);
#endif

#ifdef DO_CGX
	m_wndCGXGrid.SubclassDlgItem(IDC_CUSTOM_CGX_GRID, this);

	// Initialize the grid.
	m_wndCGXGrid.Initialize();

	// Undo is disabled by default, only enabled when history is cleared which is an implicit goahead to start recording.
	m_wndCGXGrid.GetParam()->EnableUndo(FALSE);

	/*
	 * Read default font size from settings.
	 */
	int nFontSize = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt( gIniSec_Features, gIniEnt_FontSize, 0);

	if( nFontSize > 0) {
		m_wndCGXGrid.ChangeStandardStyle( CGXStyle().SetFont( CGXFont().SetSize( nFontSize)));

		m_wndCGXGrid.SetFontSize( nFontSize);
	} else {
		m_wndCGXGrid.SetFontSize( (int)m_wndCGXGrid.StandardStyle().GetFont().GetSize());
	}

	m_wndCGXGrid.SetStyleRange(CGXRange().SetRows( 0), CGXStyle().SetHorizontalAlignment(DT_CENTER));



	m_wndCGXGrid.EnableIntelliMouse();

	m_wndCGXGrid.GetParam()->EnableSelection( GX_SELROW |			// specifies selecting rows.
												GX_SELCELL |		// specifies selecting of cells in the grid.
												GX_SELTABLE |		// specifies selecting the whole table.
												GX_SELSHIFT |		// specifies extending selections by pressing SHIFT. 
												GX_SELKEYBOARD);	// specifies selecting with SHIFT+Arrow 

	m_wndCGXGrid.GetParam()->SetMinTrackColWidth( 10);		// Stop users hiding columns
	m_wndCGXGrid.GetParam()->EnableTrackRowHeight(FALSE);   // Disable altering row heights
	m_wndCGXGrid.GetParam()->EnableMoveCols(FALSE);			// Disable moving columns
	m_wndCGXGrid.GetParam()->EnableMoveRows(FALSE);			// Disable moving rows
	m_wndCGXGrid.GetParam()->SetEnterKeyAction( GX_DOWN);	// Go down on me

	/*
	 * Excel like auto fill.
	 */
	 m_wndCGXGrid.GetParam()->SetExcelLikeCurrentCell( TRUE);
	 m_wndCGXGrid.GetParam()->SetExcelLikeSelectionFrame( TRUE);
	 m_wndCGXGrid.GetParam()->SetExcelLikeFillFlags( GX_XLFILLTEXT);
	 	
	/*
	 * This would enable drag and drop moving of data, especially to excel which would
	 * be useful if not for the following bugs:
	 *   - when dragging block within grid is copies rather than moves, original remains
	 *   - when dragging block to excel it moves rather than copies: clears cells in grid
	 *   - It will happily allow you to paste anything into drop down list cells, 
	 *     i.e. there is no validation when pasting, even a silent ignore
	 * I'm not inclined to spend hours single stepping through source code to implement
	 * features that have not been specifically requested.
	 */
	m_wndCGXGrid.EnableOleDataSource();
	m_wndCGXGrid.EnableOleDropTarget();

#endif

	// Initialize batch grid from data source
	ASSERT(m_pBatch != NULL);
	m_iDataSource = m_pBatch->RegisterGrid(&m_grid);
	m_pBatch->RegisterCGXGrid( &m_wndCGXGrid);

	// merging
	if (m_pBatch->IsMergeAllowed() == FALSE)
	{
		m_allowMerge = FALSE;
	}
	else
	{
		m_allowMerge = TRUE;
	}

	// Initialize assay box
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	if (!m_assayBox.Init(&pApp->m_assays, m_pBatch->GetAssayName()))
	{	// Assay name may no longer exist for previously run workloads
		if (m_pBatch->GetStatus() != LUC_BSTAT_NEW)
		{
			m_assayBox.SetCurSel(m_assayBox.AddString(m_pBatch->GetAssayName()));
		}
	}

	UpdateStatus();	// Also does UpdateData(false)

	// Enable Drag and Drop
#ifdef DO_UG
	m_grid.DragDropTarget(TRUE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 * Override the default handlers for OnOK and OnCancel because they cause the
 * embedded dialog to be closed, leaving a blank tab.
 */
void CBatchGridPage::OnOK()
{
}

void CBatchGridPage::OnCancel()
{
}

BOOL CBatchGridPage::PreTranslateMessage( MSG *msg)
{
	/*
	 * This dialog is embedded and is not really a dialog.
	 * Disable any dialog-like tendancies (including OnOK and OnCancel).
	 */
//    if( IsDialogMessage( msg ) )
//        return TRUE;
//    else
	/*
	 * If message in Return Key Down then don't allow dialog processing to grab it
	 * dispatch the message to the grid instead.
	 */
	if( msg->message==WM_KEYDOWN && msg->wParam==VK_RETURN) {
		::TranslateMessage( msg);
		::DispatchMessage( msg);
		return TRUE;    // DO NOT process further
    }

    return CWnd::PreTranslateMessage( msg);
}


/////////////////////////////////////////////////////////////////////////////
// Register batch with page
/////////////////////////////////////////////////////////////////////////////

void CBatchGridPage::RegisterBatch(CBatch* pBatch)
{
	// Initialize batch pointer
	m_pBatch = pBatch;

	// Set batch status
	UpdateStatus();

	// Give one to the grid also
#ifdef DO_UG
	m_grid.RegisterBatch(pBatch);
#endif

#ifdef DO_CGX
	m_wndCGXGrid.RegisterBatch(pBatch);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// Select current sample
/////////////////////////////////////////////////////////////////////////////

BOOL CBatchGridPage::SetCurSample(long iSamp)
{
	BOOL rv = FALSE;
	
	/*
	 * The current sample is now only controlled by the user, it is not
	 * moved as a side effect of other things happening (e.g. results being
	 * added to the grid) as it confuses and annoys the user.
	 */
	return rv;
}

/////////////////////////////////////////////////////////////////////////////
// Resize page
/////////////////////////////////////////////////////////////////////////////
void CBatchGridPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect pageRect, gridRect;

	// Get page size
	GetClientRect(pageRect);
	// Get original grid area
#ifdef DO_UG
	if( m_grid.GetSafeHwnd() == NULL) {
		return;
	}

	GetDlgItem(IDC_GRID_AREA)->GetWindowRect(gridRect);
	ScreenToClient(gridRect);

#ifdef DO_CGX
	/*
	 * Showing both old UG grid and new CGX grid.
	 */
	int nHalfWay = (pageRect.bottom - pageRect.top) / 2;

	gridRect.bottom = pageRect.top + nHalfWay - GRID_BORDER;
	gridRect.right = pageRect.right - GRID_BORDER;
	
	// Resize the grid
	m_grid.MoveWindow(gridRect);

	gridRect.top = pageRect.top + nHalfWay;
	gridRect.bottom = pageRect.bottom - GRID_BORDER;

	if( m_wndCGXGrid.GetSafeHwnd() != NULL) {
		m_wndCGXGrid.MoveWindow(gridRect);
	}
#else
	/*
	 * Just UG Grid
	 */

	// Leave top-left alone, but expand bottom-right of grid
	gridRect.bottom = pageRect.bottom - GRID_BORDER;
	gridRect.right = pageRect.right - GRID_BORDER;
	
	// Resize the grid
	m_grid.MoveWindow(gridRect);
#endif
#else
	/*
	 * Just CGX Grid
	 */
	// Resize the grid
	if( m_wndCGXGrid.GetSafeHwnd() != NULL) {
		m_wndCGXGrid.GetWindowRect(gridRect);
		ScreenToClient(gridRect);
		
		// Leave top-left alone, but expand bottom-right of grid
		gridRect.bottom = pageRect.bottom - GRID_BORDER;
		gridRect.right = pageRect.right - GRID_BORDER;
	
		m_wndCGXGrid.MoveWindow(gridRect);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Process notification of change of assay selection
/////////////////////////////////////////////////////////////////////////////

void CBatchGridPage::OnSelectassay()
{
	// Pass Assay Name to view
	int iSelect = m_assayBox.GetCurSel();
	if (iSelect == -1)
	{
		m_assayName.Empty();
	}
	else
	{
		m_assayBox.GetLBText(iSelect, m_assayName);
	}

	LPARAM lParam = (LPARAM)&m_assayName;

	WPARAM wParam = 0;

	if (GetWklView() != NULL)
	{
		GetWklView()->OnSelectassay(wParam, lParam);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Re-initialize grid after restructuring
/////////////////////////////////////////////////////////////////////////////

void CBatchGridPage::ReInitGrid()
{
	ASSERT(m_pBatch != NULL);

	// Remove previous data source
	ASSERT(m_iDataSource != UNDEF_DATA_SOURCE);
	VERIFY(m_grid.RemoveDataSource(m_iDataSource) == UG_SUCCESS);

	// Initialize batch grid from data source
	m_pBatch->RegisterGrid(&m_grid);
	m_pBatch->RegisterCGXGrid(&m_wndCGXGrid);

	// Set batch status
	UpdateStatus();
}

CugWkl* CBatchGridPage::GetGridPtr()
{
	return &m_grid;
}

CGXBatchGrid *CBatchGridPage::GetCGXGridPtr()
{
	return &m_wndCGXGrid;
}


void CBatchGridPage::OnPaletteChanged(CWnd * pWnd)
{
	// If the grid is the source of the palette change
	// pWnd holds a pointer to the _T("CugGrid") window.
	m_grid.OnPaletteChanged(pWnd);
}

void CBatchGridPage::UpdateStatus()
{
	if (GetSafeHwnd() == NULL) {
		return;
	}

	CWnd* pChkbox = GetDlgItem(IDC_ALLOW_MERGE);

	if( gGateKeeper.IsAppLocked()) {
		pChkbox ->EnableWindow( false);
		m_assayBox.EnableWindow( false);
		return;
	}

	if (m_pBatch != NULL)
	{	// Set status string
		m_batStat = ((CLuceptApp*)AfxGetApp())->
			m_sampStats.GetNameFromID(m_pBatch->GetStatus());
		
		// merging
		if (m_pBatch->IsMergeAllowed() == FALSE)
		{
			m_allowMerge = FALSE;
		}
		else
		{
			m_allowMerge = TRUE;
		}

		// Enable/Disable changes to assay selection 
		if(m_pBatch->GetStatus() == LUC_BSTAT_NEW)
		{
			m_assayBox.EnableEditing(true);
			if (GetSafeHwnd() != NULL)
			{
				if (pChkbox != NULL)
				{
					pChkbox ->EnableWindow(true);
				}
			}
		}
		else
		{
			m_assayBox.EnableEditing(false);
			if (GetSafeHwnd() != NULL)
			{
				if (pChkbox != NULL)
				{
					pChkbox ->EnableWindow(false);
				}
			}
		}

		if (GetSafeHwnd() != NULL)
		{
			UpdateData(false);
		}
	}
}

void CBatchGridPage::SetReadOnly( bool bTrue)
{
	m_wndCGXGrid.SetReadOnly( bTrue);
	UpdateStatus();
}

void CBatchGridPage::OnDestroy()
{
	m_assayBox.DestroyWindow();
}

void CBatchGridPage::UpdateAssay(CString& assayName)
{
	m_assayBox.SetCurSel(m_assayBox.FindStringExact(-1,assayName));
}

// Display the first two calibration values
void CBatchGridPage::UpdateCalVals()
{
	COleVariant var;

	if (m_pBatch == NULL)
	{
		return;
	}

	int nCalVals = m_pBatch->GetNumCalVals();
	if (nCalVals <= 0)
	{
		return;
	}

	m_calValName1 = m_pBatch->GetCalVal(0).m_name;
	m_calValName1 += _T(":");
	var = m_pBatch->GetCalVal(0).m_value;
	var.ChangeType(VT_R8);
	m_calValVal1.Format(_T("%9.1f"), var.dblVal);
	if (nCalVals == 1)
	{
		return;
	}

	m_calValName2 = m_pBatch->GetCalVal(1).m_name;
	m_calValName2 += _T(":");
	var = m_pBatch->GetCalVal(1).m_value;
	var.ChangeType(VT_R8);
	m_calValVal2.Format(_T("%9.1f"), var.dblVal);

	UpdateData(false);
}

void CBatchGridPage::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
}

void CBatchGridPage::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
}

int CBatchGridPage::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	return CDialog::OnVKeyToItem(nKey, pListBox, nIndex);
}

void CBatchGridPage::RedrawRow(int iRow)
{
	if( m_grid.GetSafeHwnd() != NULL) {
		m_grid.RedrawRow((long)iRow);
	}
	if( m_wndCGXGrid.GetSafeHwnd() != NULL) {
		m_wndCGXGrid.Redraw( GX_INVALIDATE | GX_SMART);
	}
}


CWklDoc* CBatchGridPage::GetWklDoc()
{
	if (GetSafeHwnd() != NULL)
	{
		CBatchTabCtrl* pParent = (CBatchTabCtrl*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CBatchTabCtrl)));
			return pParent->GetWklDoc();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

int CBatchGridPage::GetBatchIndex()
{
	if (GetSafeHwnd() != NULL)
	{
		CBatchTabCtrl* pParent = (CBatchTabCtrl*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CBatchTabCtrl)));
			return pParent->GetBatchIndex(this);
		}
		else
		{
			return LUC_BATCH_UNDEF;
		}
	}
	else
	{
		return LUC_BATCH_UNDEF;
	}
}

void CBatchGridPage::OnAllowMerge() 
{
	UpdateData(true);	
	if (m_pBatch != NULL)
	{
		m_pBatch->AllowMerge(m_allowMerge);
	}
}

CWklView* CBatchGridPage::GetWklView()
{
	if (GetSafeHwnd() != NULL)
	{
		CBatchTabCtrl* pParent = (CBatchTabCtrl*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CBatchTabCtrl)));
			return pParent->GetWklView();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

/**
 * The request to clear undo history implies that recording of undo should commence.
 */
void CBatchGridPage::ClearUndo()
{ 
	m_wndCGXGrid.GetParam()->EnableUndo(TRUE);
	m_wndCGXGrid.ClearUndo();
}
