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

// BTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BTabCtrl.h"
#include "resource.h"
#include "Batch.h"
#include "WklView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatchTabCtrl

IMPLEMENT_DYNAMIC(CBatchTabCtrl, CTabCtrl)

CBatchTabCtrl::CBatchTabCtrl()
{
}

CBatchTabCtrl::~CBatchTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CBatchTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CBatchTabCtrl)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchTabCtrl message handlers
/////////////////////////////////////////////////////////////////////////////

void CBatchTabCtrl::Append(CBatch* pBatch)
{
	// Create new batch page object and save pointer to it
	CBatchGridPage*	pBatchPage = m_batchPages.Append();

	// Register Document Batch with batch page
	// ( ... BEFORE CREATING WINDOW!)
	pBatchPage ->RegisterBatch(pBatch);

	// Create new batch page window
	pBatchPage ->Create(CBatchGridPage::IDD, this);

	// Add new tab ...
	int iTab = m_batchPages.GetCount() - 1;
	TC_ITEM tabStruct;

	// Store title in batch page
	_tcsncpy(pBatchPage->m_title, CBatch::GetBatchName(iTab), 
		MAX_LEN_BATCH_TITLE);

	// ... load tab info structure
	tabStruct.mask = TCIF_TEXT;
	tabStruct.pszText = pBatchPage->m_title;

	// ... and add tab 
	InsertItem(iTab, &tabStruct);

	// Set page size and make it current page
	SetCurSel(iTab);
	ResizePage(iTab);

	// Redraw tab control
	Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// Change the current sample selection
/////////////////////////////////////////////////////////////////////////////

BOOL CBatchTabCtrl::SetCurSample(long iSamp)
{
	int iPage = GetCurSel();

	if (iPage >= 0 && iPage < m_batchPages.GetCount())
	{
		return m_batchPages[iPage].SetCurSample(iSamp);
	}
	else
	{
		return FALSE;
	}
}

/**
 * Set all grids in tabs to read only.
 * @param bTrue true if read only should be set, false to reset it.
 */
void CBatchTabCtrl::SetReadOnly( bool bTrue)
{
	for( int nPage = 0; nPage < m_batchPages.GetCount(); nPage++) {
		m_batchPages[nPage].SetReadOnly( bTrue);
	}
}


/////////////////////////////////////////////////////////////////////////////
//  Resize Control
/////////////////////////////////////////////////////////////////////////////

void CBatchTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	ResizePage(ALL_PAGES);
}


/////////////////////////////////////////////////////////////////////////////
//  Resize pages
/////////////////////////////////////////////////////////////////////////////

void CBatchTabCtrl::ResizePage(int page)
{

	CRect totalSize, tabSize;

	// Get Property Sheet size
	GetClientRect(totalSize);

	// Calculate Property Page size.  Must allow room for the tabs
	int iCurTab = GetCurSel();
	// Must have a valid tab selected
	if (iCurTab < 0)
	{
		return;
	}
	if(GetItemRect(iCurTab, tabSize))
	{	// Don't cover tabs
		totalSize.top += tabSize.bottom - tabSize.top;

		// Shrink everything from edge a bit
		totalSize.top	 += SHEET_BORDER;
		totalSize.bottom -= SHEET_BORDER;
		totalSize.left	 += SHEET_BORDER;
		totalSize.right	 -= SHEET_BORDER;
	}
	else
	{
		return;
	}

	if (page == ALL_PAGES)
	{	// Resize all pages
		int num = m_batchPages.GetCount(); 
		for (int iPage = 0;	iPage < num; iPage++)
		{
			if( m_batchPages[iPage].GetSafeHwnd() != NULL) {
				m_batchPages[iPage].MoveWindow(totalSize);
			}
		}
	}
	else
	{	// Resize specified page
		if( m_batchPages[page].GetSafeHwnd() != NULL) {
			m_batchPages[page].MoveWindow(totalSize);
		}
	}
};


void CBatchTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int newSelection = GetCurSel();

	if (newSelection != LUC_GRID_UNDEF)
	{	// Make appropriate page visible
		if( m_batchPages[newSelection].GetSafeHwnd() != NULL) {
			m_batchPages[newSelection].ShowWindow(SW_SHOWNOACTIVATE);
		}
	}
	
	// Notify main window of a change in selection
	// Main window routes message to view
	if (GetWklView() != NULL)
	{
		GetWklView()->OnSelectgrid((WPARAM)newSelection);
	}
	*pResult = 0;
}

void CBatchTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int oldSelection = GetCurSel();

	if (oldSelection != -1)
	{	// Make old page invisible
		if( m_batchPages[oldSelection].GetSafeHwnd() != NULL) {
			m_batchPages[oldSelection].ShowWindow(SW_HIDE);
		}
	}
	
	*pResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
//  Get safe tab count
/////////////////////////////////////////////////////////////////////////////

int CBatchTabCtrl::GetItemCount()
{
	// Check that window exists before counting tabs.  Prevent ASSERT's.
	if (GetSafeHwnd() == NULL)
	{
		return 0;
	}
	else
	{
		return CTabCtrl::GetItemCount();
	}
}


/////////////////////////////////////////////////////////////////////////////
//  Set tab selection
/////////////////////////////////////////////////////////////////////////////

int CBatchTabCtrl::SetCurSel( int nItem )
{
	// Calls pre and post processing routines to hide and show grids
	int rv = TRUE;
	NMHDR NMHDR;
	LRESULT Result;

	OnSelchanging(&NMHDR, &Result); 
	rv = CTabCtrl::SetCurSel(nItem);
	OnSelchange(&NMHDR, &Result); 
	
	return rv;
}


/////////////////////////////////////////////////////////////////////////////
//  Re-Initialize grid 
/////////////////////////////////////////////////////////////////////////////

BOOLEAN CBatchTabCtrl::ReInitGrid(int iBatch)
{
	if (iBatch == LUC_GRID_UNDEF)
	{	// Current page only
		m_batchPages[m_batchPages.GetCurIndex()].ReInitGrid();
	}
	else 
	{	// !!! Only current grid implemented for now
		ASSERT(FALSE);
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
//  Get Pointer to Grid
/////////////////////////////////////////////////////////////////////////////

CugWkl* CBatchTabCtrl::GetGridPtr(int iBatch)
{
	if (iBatch == LUC_GRID_UNDEF)
	{	// Current page only
		return m_batchPages[m_batchPages.GetCurIndex()].GetGridPtr();
	}
	else 
	{
		if (iBatch < 0 || iBatch >= GetItemCount())
		{
			return NULL;
		}
		else
		{
			return m_batchPages[iBatch].GetGridPtr();
		}
	}
}

CGXBatchGrid *CBatchTabCtrl::GetCGXGridPtr(int iBatch)
{
	if (iBatch == LUC_GRID_UNDEF)
	{	// Current page only
		int nIndex = m_batchPages.GetCurIndex();

		if( nIndex < 0 || nIndex >= m_batchPages.GetCount()) {
			return NULL;
		} else {
			return m_batchPages[nIndex].GetCGXGridPtr();
		}
	}
	else 
	{
		if (iBatch < 0 || iBatch >= GetItemCount())
		{
			return NULL;
		}
		else
		{
			return m_batchPages[iBatch].GetCGXGridPtr();
		}
	}
}

bool CBatchTabCtrl::RemoveBatch(int iBatch)
{
	if (iBatch == LUC_GRID_UNDEF)
	{
		iBatch = GetCurSel();
	}

	if (iBatch < 0 || iBatch >= GetItemCount())
	{
		return false;
	}
	
	VERIFY(DeleteItem(iBatch));
	// Destroy window hopefully will cause assay def box
	// to unregister with the assay list
	m_batchPages[iBatch].DestroyWindow();
	m_batchPages.DeleteItem(iBatch);
	// Should be OK if the batch is deleted

	RelabelBatches(iBatch);

	return true;
}

void CBatchTabCtrl::RelabelBatches(int startingAt)
{
	TC_ITEM tabStruct;

	int numTabs = m_batchPages.GetCount();
	for (int iTab = startingAt; iTab < numTabs; iTab++)
	{	// ... create batch title 
		_tcsncpy(m_batchPages[iTab].m_title, CBatch::GetBatchName(iTab),MAX_LEN_BATCH_TITLE);

		// Write title text to tab
		tabStruct.mask = TCIF_TEXT;
		tabStruct.pszText = m_batchPages[iTab].m_title;
		VERIFY(SetItem(iTab, &tabStruct));
	}
}

void CBatchTabCtrl::OnPaletteChanged(CWnd * pWnd)
{
	// Notify active grid page
	if (GetCurSel() >= 0)
	{
		m_batchPages[GetCurSel()].OnPaletteChanged(pWnd);
	}
}

void CBatchTabCtrl::UpdateBatchStat(int iBatch)
{
	if (iBatch == ALL_PAGES)
	{
		for (iBatch = 0; iBatch < m_batchPages.GetCount(); iBatch++)
		{
			m_batchPages[iBatch].UpdateStatus();
		}
	}
	else if (iBatch >= 0 && iBatch < m_batchPages.GetCount())
	{
		m_batchPages[iBatch].UpdateStatus();
	}
}

void CBatchTabCtrl::UpdateAssay(CString & assayName, int iBatch)
{
	if (iBatch >= 0 && iBatch < m_batchPages.GetCount())
	{
		m_batchPages[iBatch].UpdateAssay(assayName);
	}
}

void CBatchTabCtrl::UpdateCalVals(int iBatch)
{
	if (iBatch == ALL_PAGES)
	{
		for (iBatch = 0; iBatch < m_batchPages.GetCount(); iBatch++)
		{
			m_batchPages[iBatch].UpdateCalVals();
		}
	}
	else if (iBatch >= 0 && iBatch < m_batchPages.GetCount())
	{
		m_batchPages[iBatch].UpdateCalVals();
	}
}

/**
 * Clear undo history for one or all batches. Not Some or most though.
 * @param iBatch The bacth to clear undo for or all batches.
 */
void CBatchTabCtrl::ClearUndo( int iBatch)
{
	if (iBatch == ALL_PAGES)
	{
		for (iBatch = 0; iBatch < m_batchPages.GetCount(); iBatch++)
		{
			m_batchPages[iBatch].ClearUndo();
		}
	}
	else if (iBatch >= 0 && iBatch < m_batchPages.GetCount())
	{
		m_batchPages[iBatch].ClearUndo();
	}
}


void CBatchTabCtrl::RedrawRow(int iRow)
{
	// Redraws specified row in current batch
	if (GetCurSel() >= 0)
	{
		m_batchPages[GetCurSel()].RedrawRow(iRow);
	}
}

bool CBatchTabCtrl::CanEditCut()
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
		return m_batchPages[GetCurSel()].GetGridPtr()->CanEditCut();
#else
		return m_batchPages[GetCurSel()].GetCGXGridPtr()->CanEditCut();
#endif
	}
	else
	{
		return false;
	}
}

bool CBatchTabCtrl::CanEditCopy()
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
		return m_batchPages[GetCurSel()].GetGridPtr()->CanEditCopy();
#else
		return m_batchPages[GetCurSel()].GetCGXGridPtr()->CanEditCopy();
#endif
	}
	else
	{
		return false;
	}
}

bool CBatchTabCtrl::CanEditPaste()
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
		return m_batchPages[GetCurSel()].GetGridPtr()->CanEditPaste();
#else
		return m_batchPages[GetCurSel()].GetCGXGridPtr()->CanEditPaste();
#endif
	}
	else
	{
		return false;
	}
}

BOOLEAN CBatchTabCtrl::CanEditUndo( CCmdUI* pCmdUI)
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
		return false;
#else
		return m_batchPages[GetCurSel()].GetCGXGridPtr()->CanUndo( pCmdUI);
#endif
	}
	else
	{
		pCmdUI->Enable( false);
		return true;
	}
}

void CBatchTabCtrl::EditCut()
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
		m_batchPages[GetCurSel()].GetGridPtr()->EditCut();
#else
		m_batchPages[GetCurSel()].GetCGXGridPtr()->EditCut();
#endif
	}
}

void CBatchTabCtrl::EditCopy()
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
		m_batchPages[GetCurSel()].GetGridPtr()->EditCopy();
#else
		m_batchPages[GetCurSel()].GetCGXGridPtr()->EditCopy();
#endif
	}
}

void CBatchTabCtrl::EditPaste()
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
		m_batchPages[GetCurSel()].GetGridPtr()->EditPaste();
#else
		m_batchPages[GetCurSel()].GetCGXGridPtr()->EditPaste();
#endif
	}
}

void CBatchTabCtrl::EditUndo()
{
	if (GetCurSel() >= 0)
	{
#ifdef DO_UG
//		m_batchPages[GetCurSel()].GetGridPtr()->EditCopy();
#else
		m_batchPages[GetCurSel()].GetCGXGridPtr()->Undo();
#endif
	}
}


CWklDoc* CBatchTabCtrl::GetWklDoc()
{
	if (GetSafeHwnd() != NULL)
	{
		CWklView* pParent = (CWklView*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CWklView)));
			return pParent->GetDocument();
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

int CBatchTabCtrl::GetBatchIndex(CBatchGridPage * pPage)
{
	int numPages = m_batchPages.GetCount();
	for (int iPage = 0; iPage < numPages; iPage++)
	{
		if (&m_batchPages[iPage] == pPage)
		{
			return iPage;
		}
	}

	return LUC_BATCH_UNDEF;
}

CWklView* CBatchTabCtrl::GetWklView()
{
	if (GetSafeHwnd() != NULL)
	{
		CWklView* pParent = (CWklView*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CWklView)));
			return pParent;
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
