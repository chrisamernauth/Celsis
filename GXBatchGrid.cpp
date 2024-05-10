// GXBatchGrid.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "Batch.h"
#include "ultgrid/UGCtrl.h"

#include "GXBatchGrid.h"
#include "BGridPg.h"
#include "WklView.h"
#include "WklDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BLANK_MARKER_STRING	_T("1337 BlAnk Mar|<er Str!ng")

/////////////////////////////////////////////////////////////////////////////
// CGXBatchGrid

CGXBatchGrid::CGXBatchGrid()
	: m_pBatch( NULL), 
		m_bDoingDragDrop( false),
		m_oBoldFont()
{
}

CGXBatchGrid::~CGXBatchGrid()
{
}


BEGIN_MESSAGE_MAP(CGXBatchGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CGXBatchGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND( CUGWKL_CUT, OnMenuCut)
	ON_COMMAND( CUGWKL_COPY, OnMenuCopy)
	ON_COMMAND( CUGWKL_PASTE, OnMenuPaste)
	ON_COMMAND( CUGWKL_TEMPLATE_COPY, OnMenuTemplateCopy)
	ON_COMMAND( CUGWKL_COPY_FOR_RETEST, OnMenuCopyForRetest)
	ON_COMMAND( CUGWKL_COPY_RETEST_PART, OnMenuCopyRetestPart)
	ON_COMMAND( CUGWKL_VIRTUAL_COPY, OnMenuVirtualCopy)

	ON_WM_KEYDOWN( )

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGXBatchGrid message handlers

void CGXBatchGrid::OnMenuCut( void)
{
	EditCut();
}

void CGXBatchGrid::OnMenuCopy( void)
{
	EditCopy();
}

void CGXBatchGrid::OnMenuPaste( void)
{
	EditPaste();
}

void CGXBatchGrid::OnMenuTemplateCopy( void)
{
	EditCopy( TEMPLATE_COPY);
}

void CGXBatchGrid::OnMenuCopyForRetest( void)
{
	EditCopy( RETEST_COPY);
}

void CGXBatchGrid::OnMenuCopyRetestPart( void)
{
	EditCopy( RETEST_PARTS);
}

void CGXBatchGrid::OnMenuVirtualCopy( void)
{
	EditCopy( VIRTUAL_COPY);
}

/**
 * Set the font that is used for bold items.
 * @param the pointer to be used.
 */
void CGXBatchGrid::SetFontSize( int nFontSize)
{
	m_oBoldFont = CGXFont().SetSize( nFontSize).SetBold( TRUE);
}

/*
 * As a hack, nCol = -1 if we just want the low level grid to select a cell without notifying
 * the underlying document. The code has circular problems, UI changes updating doc, doc changes
 * updating ui.
 */
BOOL CGXBatchGrid::SetCurrentCell(ROWCOL nRow, ROWCOL nCol, UINT flags)
{
	if( nCol >= 0 && nCol < 0x80000000 && GetWklView() != NULL) {
		GetWklView()->OnSelectcell((WPARAM)nCol - 1, (LPARAM)nRow - 1);
	}

	if( nCol >= 0x80000000) {
		nCol = 1;
	}

	/*
	 * Setting cell (e.g. by opening sample view) during edit 
	 * causes view to be notifed about edit change and recursively set the current cell.
	 */
	static bool bRecursive = false;

	if( bRecursive == false) {
		bRecursive = true;
	
		BOOL bRet = CGXGridCore::SetCurrentCell( nRow, nCol, flags);

		bRecursive = false;

		return bRet;
	} else {
		return TRUE;
	}
}

BOOL CGXBatchGrid::CanChangeSelection(CGXRange* pRange, BOOL bIsDragging, BOOL)
{
	return TRUE;
}

BOOL CGXBatchGrid::CanChangeAutoFillSelection(CGXRange& rgNew, const CGXRange& rgStart)
{
	/*
	 * Restrict autofill to prevent writing to columns that are not free form.
	 */
	if( IsRangePastable( &rgNew) == false) {
		return FALSE;
	}

	return CGXGridCore::CanChangeAutoFillSelection( rgNew, rgStart);
}

/*
 * Sets text following paste, dnd etc.
 * Only allow free-form values to be pasted unto.
 */
BOOL CGXBatchGrid::SetControlTextRowCol(ROWCOL nRow, ROWCOL nCol, const CString& strDisplay, UINT flags, const CGXStyle* pOldStyle)
{
	if( IsCellPastable( nRow, nCol) == true) {
		return CGXGridCore::SetControlTextRowCol( nRow, nCol, strDisplay, flags, pOldStyle);
	} else {
		/*
		 * Lie, silently ignore problems.
		 */
		return TRUE;
	}
}

/*
 * Determine the number of columns to go in the grid.
 */
ROWCOL CGXBatchGrid::GetRowCount()
{
	return (ROWCOL)m_pBatch->GetNumRows();
}

/*
 * Determine the number of rows to go in the grid.
 */
ROWCOL CGXBatchGrid::GetColCount()
{
	return (ROWCOL )m_pBatch->GetNumCols();
}

/*
 * Override base class to get values for display in the grid.
 */
BOOL CGXBatchGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle &oStyle, GXModifyType mt, int nType)
{
	BOOL ret= CGXGridCore::GetStyleRowCol( nRow,  nCol, oStyle,  mt,  nType);
	CUGCell oCell;

	/*
	 * Disable read-only: or we cannot change anything.
	 */
	GetParam()->SetLockReadOnly( FALSE);

	if(nType >= 0) {
		CString strText;

		if( nCol > 0) {
			if( nRow > 0) {
				m_pBatch->GetCell( nCol - 1, nRow - 1, &oCell);
				oCell.GetText( &strText);

				oStyle.SetValue( strText);
				oStyle.SetTextColor( oCell.GetTextColor());
				oStyle.SetInterior( oCell.GetBackColor());

				switch( oCell.GetAlignment()) {
					case UG_ALIGNRIGHT:
						oStyle.SetHorizontalAlignment( DT_RIGHT);
						break;

					case UG_ALIGNCENTER:
						oStyle.SetHorizontalAlignment( DT_CENTER);
						break;

					case UG_ALIGNLEFT:
						oStyle.SetHorizontalAlignment( DT_LEFT);
						break;
				}

				if( oCell.GetReadOnly()) {
	//				oStyle.SetReadOnly( TRUE);

					/*
					 * Set static control to hide any editing features.
					 */
					oStyle.SetControl(GX_IDS_CTRL_STATIC);
				} else {
					oStyle.SetReadOnly( FALSE);

					switch( oCell.GetCellType()) {
						case UGCT_SPINBUTTON:
/*
 * The old grid used a spin button. The new grid uses a dropdown list. This makes it easier
 * to validate, there is no option for free format data entry, which the grid doesn't handle
 * elegantly.
 */
#ifdef usespinedit
							oStyle.SetControl( GX_IDS_CTRL_SPINEDIT);

							if( nCol > 0 && m_pBatch->GetFldProp()->GetColID( nCol - 1) == SF_REPLS) {
								oStyle.SetUserAttribute( GX_IDS_UA_VALID_MIN, (LONG)1);
								oStyle.SetUserAttribute( GX_IDS_UA_VALID_MAX, (LONG)MAX_REPLICATES);
								oStyle.SetUserAttribute( GX_IDS_UA_SPINBOUND_MIN, (LONG)1);
								oStyle.SetUserAttribute( GX_IDS_UA_SPINBOUND_MAX, (LONG)MAX_REPLICATES);
							}
#else
							oStyle.SetControl( GX_IDS_CTRL_CBS_DROPDOWNLIST);
							oStyle.SetChoiceList( _T("1\n2\n3\n4\n5\n6\n7\n8\n9\n10"));
#endif
							break;

						case UGCT_DROPLIST:
							oStyle.SetControl( GX_IDS_CTRL_CBS_DROPDOWNLIST);

							CString strChoices;

							oCell.GetLabelText( &strChoices);
							oStyle.SetChoiceList( strChoices);
							break;
					}
				}

				/*
				 * Positive results should be shown in bold.
				 * Ug bold font handle is used to flag that the row should be bold.
				 */
				if( oCell.GetFont() == INVALID_HANDLE_VALUE) {
					oStyle.SetFont( m_oBoldFont);
				}

			} else {
				/*
				 * Column headers.
				 */
				CString strText;

				m_pBatch->GetColName( nCol - 1, &strText);

				strText.Replace( _T("_"), _T(" "));

				oStyle.SetValue( strText);
			}
		}
	}

	GetParam()->SetLockReadOnly( TRUE);  // enable read-only cells

	return ret;
}

// Store the changes made in the grid into your 
// external data source
BOOL CGXBatchGrid::StoreStyleRowCol(ROWCOL nRow, ROWCOL nCol, 
                      const CGXStyle * pStyle, GXModifyType mt, 
                      int nType)
{
    if( nCol > 0 && nRow > 0) {
		CUGCell oCell;

		m_pBatch->GetCell( nCol - 1, nRow - 1, &oCell);

		/*
		 * Just because grid holds static controls, does not mean it won't try to paste to them.
		 * Stop ANY writing to read-only cells.
		 */
		if( oCell.GetReadOnly() == FALSE) {
			CString strValue;

			/*
			 * pStyle == NULL if clearing value or style.
			 * This happens when doing drag/drop move.
			 */
			if( pStyle != NULL && nType >= 0 && pStyle->GetIncludeValue()) {
				strValue = pStyle->GetValue();
			}

			strValue.TrimRight();

			/*
			 * As a final precaution, stop any incorrect values ever being stored
			 * in a cell, whether via typing, pasting, dropping.
			 */
			if( nCol > 0 && m_pBatch->GetFldProp()->GetColID( nCol - 1) == SF_REPLS) {
				int nValue = _tstoi( strValue);

				nValue = min( nValue, MAX_REPLICATES);
				nValue = max( nValue, 1);

				strValue.Format( _T("%d"), nValue);
			} else if( oCell.GetCellType() == UGCT_DROPLIST) {
				CString strChoices;
				int nOffset;

				if( strValue == _T("")) {
					return TRUE;
				}

				oCell.GetLabelText( &strChoices);

				nOffset = strChoices.Find( strValue);
				if( nOffset < 0) {
					return TRUE;
				}

			}

			/*
			 * The Mask field is used to store any previous value
			 * for the cell. If the user abandons changes then these old
			 * values are read back from the mask fields before saving.
			 * 
			 * Original values in cell text are copied to the mask fields
			 * when a new value is written into a cell. If a cell is written
			 * to twice the original value in the mask field should not be
			 * overwritten.
			 */
			CString strMaskValue;

			oCell.GetMask( &strMaskValue);
			if( strMaskValue == _T("")) {

				/*
				 * Get original value of cell.
				 */
				CString strOldValue;

				oCell.GetText( &strOldValue);
				if( strOldValue == _T("")) {
					/*
					 * Need to distinguish between blank strings and strings
					 * that have not been specified.
					 */
					strOldValue = BLANK_MARKER_STRING;
				}

				oCell.SetMask( strOldValue);
			}

			oCell.SetText( strValue);
			m_pBatch->SetCell( nCol - 1, nRow - 1, &oCell);

			/*
			 * Tell world something has been modified.
			 */
			if( GetWklView() != NULL) {
				GetWklView()->OnModifycell((WPARAM)(nCol - 1), (LPARAM)(nRow - 1));
			}
		}

		return TRUE;
    } else {
		return CGXGridWnd::StoreStyleRowCol(nRow, nCol, 
		                                       pStyle, mt, nType);
	}
}

BOOL CGXBatchGrid::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	if( nRow > 0 && nCol == 0) {	// Sample is made current by first click
		// Open sample view
		WPARAM wParam = (WPARAM)ID_SAMPLE_VIEW;	// Menu item ID
		LPARAM lParam = (LPARAM)NULL;			// Only used for controls

		AfxGetMainWnd()->SendMessage(WM_COMMAND, wParam, lParam);

		return TRUE;
	} else {
		// Append new row
		return CGXGridWnd::OnLButtonDblClkRowCol( nRow, nCol, nFlags, pt);
	}

	return FALSE;
}

/*
 * Validate values being entered.
 */
BOOL CGXBatchGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	if( nCol > 0 && m_pBatch->GetFldProp()->GetColID( nCol - 1) == SF_REPLS) {
		CString strValue;

		GetCurrentCellControl()->GetCurrentText( strValue);

		strValue.TrimRight();

		int nValue = _tstoi( strValue);

		nValue = min( nValue, MAX_REPLICATES);
		nValue = max( nValue, 1);

		strValue.Format( _T("%d"), nValue);

		GetCurrentCellControl()->SetCurrentText( strValue);

		return TRUE;
	}

	return CGXGridWnd::OnValidateCell( nRow, nCol);
}

/*
 * Another messy one. Look for right clicking in grid, outside rows and columns
 * and show context menu if possible. No nicer hooks to do this.
 */
BOOL CGXBatchGrid::DoRButtonDown(UINT flags, CPoint point)
{
	/*
	 * If grid handles the right click then whoppee, else show paste menu.
	 */
	if( CGXGridCore::DoRButtonDown( flags, point) == TRUE) {
		return TRUE;
	} else {
		if( CanEditPaste()) {
			CMenu oMenu;
			CString strMenuItem;

			oMenu.CreatePopupMenu();

			VERIFY( strMenuItem.LoadString(IDS_PASTE));
			oMenu.AppendMenu( MF_STRING, CUGWKL_PASTE, strMenuItem);

			ClientToScreen( &point);
			
			oMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,
										  point.y, this, NULL);
			return TRUE;
		}
	}

	return FALSE;
}

/*
 * Clicking buttons.
 */
BOOL CGXBatchGrid::OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	if( nRow == 0 && nCol == 0) {
		WPARAM wParam = 0;
		LPARAM lParam = 0;

		if( GetWklView() != NULL) {
			GetWklView()->OnGridproperties(wParam, lParam);
			return TRUE;
		}
	} else {
		CMenu oMenu;
		CString strMenuItem;

		CGXRangeList *pRangeList = GetParam()->GetRangeList();

		if( pRangeList->IsEmpty() == TRUE) {
			/*
			 * No range list, nothing selected. In this case,
			 * automatically select something, rather than do nothing.
			 *
			 */
			ROWCOL nRow;
			ROWCOL nColumn;

			if( GetCurrentCell( nRow, nColumn) == TRUE) {
				/*
				 * There is a current cell so select it.
				 */
				if( nColumn == 0) {
					/*
					 * If column 0 then select the whole row.
					 */
					SelectRange(CGXRange().SetRows( nRow, nRow));
				} else {
					/*
					 * Not column 0 so select just the cell.
					 */
					SelectRange(CGXRange( nRow, nCol));
				}
			}
		}

		oMenu.CreatePopupMenu();

		if( CanEditCut()) {
			VERIFY( strMenuItem.LoadString(IDS_CUT));
			oMenu.AppendMenu( MF_STRING, CUGWKL_CUT, strMenuItem);
		}
		if( CanEditCopy()) {
			VERIFY( strMenuItem.LoadString(IDS_COPY));
			oMenu.AppendMenu( MF_STRING, CUGWKL_COPY, strMenuItem);
		}
		if( CanEditPaste()) {
			VERIFY( strMenuItem.LoadString(IDS_PASTE));
			oMenu.AppendMenu( MF_STRING, CUGWKL_PASTE, strMenuItem);
		}

		/*
		 * Only allow these if whole rows are selected.
		 */
		CGXRange oRange;

		if( GetSelectionMode( &oRange) == CGX_MS_MODE_ROW) {
			// Add menu separator
			oMenu.AppendMenu( MF_SEPARATOR);

			// Side heading special items
			VERIFY( strMenuItem.LoadString( IDS_TEMPLATE_COPY));
			oMenu.AppendMenu( MF_STRING, CUGWKL_TEMPLATE_COPY, strMenuItem);

			VERIFY( strMenuItem.LoadString( IDS_COPY_FOR_RETEST));
			oMenu.AppendMenu( MF_STRING, CUGWKL_COPY_FOR_RETEST, strMenuItem);

			VERIFY( strMenuItem.LoadString( IDS_COPY_RETEST_PART));
			oMenu.AppendMenu( MF_STRING, CUGWKL_COPY_RETEST_PART, strMenuItem);

			VERIFY( strMenuItem.LoadString( IDS_VIRTUAL_COPY));
			oMenu.AppendMenu( MF_STRING, CUGWKL_VIRTUAL_COPY, strMenuItem);
		}

		if( oMenu.GetMenuItemCount() > 0) {
			ClientToScreen( &pt);
		
			oMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x,
									  pt.y, this, NULL);
		}
		return TRUE;
	}

	return CGXGridWnd::OnRButtonClickedRowCol( nRow, nCol, nFlags, pt);
}

/*
 * Get Worklist View object.
 */
CWklView *CGXBatchGrid::GetWklView()
{
	if( GetSafeHwnd() != NULL) {
		CBatchGridPage *pParent = (CBatchGridPage *)GetParent();

		if( pParent != NULL) {
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CBatchGridPage)));
			return pParent->GetWklView();
		}
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////
//CopyFromClipBoard
//	Copies the contents from the clipboard to the
//	specified string
//
//	Replaces base class function.  Does not include additional Tab
//	and Cr/Lf substrings as does base class
//  return
//	UG_SUCCESS	success
//	1			fail
////////////////////////////////////////////////////////////////////////
int CGXBatchGrid::CopyFromClipBoard(CString *string)
{
	HGLOBAL hg;          //memory handle
	LPSTR data;          //memory pointer
//size of clipboard data	unsigned long size;  

	//open clipboard
	OpenClipboard();

	//get a pointer to the text
	hg=GetClipboardData(CF_TEXT);

	//if no text was found then return FALSE
	if(hg==NULL)
	{	//close the clipboard
		CloseClipboard();
		return 1;
	}

	//lock the memory and get a pointer to it
	data=(LPSTR)GlobalLock(hg);
	//get the size of the text

	*string = data;
	// Terminate string after first Tab, Cr, Lf or /0
	TCHAR c;
	int num = string->GetLength();
	for (int i = 0; i < num; i++)
	{
		c = string->GetAt(i);
		if (c == '\t' || c == '\r' || c== '\n')
		{	// End string here
			*string = string->Left(++i);
			break;
		}
	}

	//unlock the global memory
	GlobalUnlock(hg);

	//close the clipboard
	CloseClipboard();

	return UG_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
// CanEditCut and EditCut
////////////////////////////////////////////////////////////////////////

bool CGXBatchGrid::CanEditCut()
{
	if( m_pBatch == NULL) {
		return false;
	}

	CGXRange oRange;

	switch( GetSelectionMode( &oRange)) {
		case CGX_MS_MODE_OFF:
			return false;

		case CGX_MS_MODE_CELLS_IN_COL:
			return true;

			// Only for single cell selected
		case CGX_MS_MODE_CELL:
			if( IsRangePastable( &oRange) == false) {
				return false;
			}

			return true;

		case CGX_MS_MODE_ROW:
			// If single row selected and batch status is NEW
			return (m_pBatch->GetStatus() == LUC_BSTAT_NEW && oRange.GetHeight() == 1);

		case CGX_MS_MODE_CELLS_IN_BLOCK:
			/*
			 * Too error-prone.
			 */
			return false;

		default:
			return false;
	}	
}

void CGXBatchGrid::EditCut()
{
	ASSERT( CanEditCut());

	CGXRange oRange;

	switch( GetSelectionMode( &oRange)) {
		case CGX_MS_MODE_CELL:
		case CGX_MS_MODE_CELLS_IN_COL:
			CutRange( oRange, FALSE);
			break;

		case CGX_MS_MODE_ROW:
			{
				long iSamp;

				// Get sample number
				iSamp = oRange.top;

				if( iSamp == 0) {
					/*
					 * Selected column header.
					 */
					return;
				}

				// Copy to clipboard
				EditCopy();

				// Then delete
				ASSERT(m_pBatch != NULL);
				m_pBatch->DeleteSample(iSamp - 1);
			}
			break;

		default:
			return;
	}	
}


////////////////////////////////////////////////////////////////////////
// CanEditCopy and EditCopy
////////////////////////////////////////////////////////////////////////

bool CGXBatchGrid::CanEditCopy(ECopyType copyType)
{
	if( m_pBatch == NULL) {
		return false;
	}

	CGXRange oRange;

	switch( GetSelectionMode( &oRange)) {
		case CGX_MS_MODE_CELL:
		case CGX_MS_MODE_CELLS_IN_COL:
		case CGX_MS_MODE_CELLS_IN_BLOCK:
			return true;


		case CGX_MS_MODE_ROW:
			// Only for single row selected
			if( oRange.GetHeight() == 1) {
				return true;
			} else {
				return false;
			}
		default:
			return false;
	}	
}

void CGXBatchGrid::EditCopy(ECopyType copyType)
{
	CGXRange oRange;

	switch( GetSelectionMode( &oRange)) {
		case CGX_MS_MODE_CELL:
		case CGX_MS_MODE_CELLS_IN_COL:
		case CGX_MS_MODE_CELLS_IN_BLOCK:
			Copy();
			break;

		case CGX_MS_MODE_ROW:
			{
				long iSamp;

				// Get sample number
				iSamp = oRange.top;

				if( iSamp == 0) {
					/*
					 * Selected column header.
					 */
					return;
				}

				// Create an OLE data source containing sample 
				// and sample ref formats
				COleDataSource* pDataSource = new COleDataSource;
				CopySampleToDataSource( pDataSource, iSamp - 1, copyType);

				// Add data source to clipboard
				// Clipboard owns datasource object and must delete it
				pDataSource->SetClipboard();
			}
			break;

		default:
			break;
	}	
}

////////////////////////////////////////////////////////////////////////
// CanEditPaste and EditPaste
////////////////////////////////////////////////////////////////////////

bool CGXBatchGrid::CanEditPaste()
{
	if( m_pBatch == NULL) {
		return false;
	}

	// Check clipboard for known formats
	OpenClipboard();
	HGLOBAL hgSamp = GetClipboardData(CLucSamp::CF_SAMPLE);
	HGLOBAL hgText = GetClipboardData(CF_TEXT);
	CloseClipboard();

	if( hgSamp != NULL)	{
		// It is sample data.  Ignore selection, just check that
		// batch is writable
		if( m_pBatch->GetStatus() == LUC_BSTAT_NEW) {
			return true; 
		} else {
			return false;
		}
	} else if( hgText != NULL) {
		// It's text but not sample data
		CGXRange oRange;

		switch( GetSelectionMode( &oRange)) {
			case CGX_MS_MODE_CELL:
			case CGX_MS_MODE_CELLS_IN_COL:
			case CGX_MS_MODE_CELLS_IN_BLOCK:
				return IsRangePastable( &oRange);

			default:
				return false;
		}	
	} else {	// It's nothing I expect
		return false;
	}
}

void CGXBatchGrid::EditPaste()
{
	ASSERT(m_pBatch != NULL);

	if( m_pBatch == NULL) {
		return;
	}

	// Check clipboard for known formats
	OpenClipboard();
	HGLOBAL hgSamp = GetClipboardData(CLucSamp::CF_SAMPLE);
	HGLOBAL hgText = GetClipboardData(CF_TEXT);
	CloseClipboard();

	if( hgSamp != NULL)	{	// It is sample data.  
		// Create data object from clipboard
		COleDataObject	dataObject;
		VERIFY(dataObject.AttachClipboard());

		// Append and load sample
		AppendSampleFromDataObject(&dataObject);

		Redraw( GX_INVALIDATE);
	} else if( hgText != NULL) {	// It's text but not sample data

		switch( GetSelectionMode()) {
			case CGX_MS_MODE_CELL:
			case CGX_MS_MODE_CELLS_IN_COL:
			case CGX_MS_MODE_CELLS_IN_BLOCK:
				CGXGridCore::Paste();
				break;

			default:
				break;
		}	
	}
	// else It's nothing I expect
		
	return;
}

////////////////////////////////////////////////////////////////////////
// Utility functions help determine what edit functions are valid
////////////////////////////////////////////////////////////////////////

/*
 * Determine selection mode.
 * This takes a different tack to the ultimate grid, here we see what
 * user has selected and return appropriate code. If the code is inappropriate
 * then do nothing. The ultimate grid method seems overengineered.
 */
enum CGXBatchGrid::CgxMsMode CGXBatchGrid::GetSelectionMode( CGXRange *pRangeResult)
{
	CGXRangeList *pRangeList = GetParam()->GetRangeList();

	if( pRangeList->IsEmpty() == FALSE) {
		/*
		 * Some cells have been selected. Determine whether this is a number
		 * of cells in a row or column.
		 */
		POSITION oPos = pRangeList->GetHeadPosition();

		/*
		 * NOTE: this assumes that multiple selection has been disabled.
		 */
		while( oPos != NULL) {
			CGXRange *pRange = (CGXRange *)pRangeList->GetNext( oPos);

			/*
			 * If whole table is selected then accept that.
			 */
			if( pRange->IsTable()) {
				return CGX_MS_MODE_TABLE;
			}

			/*
			 * Return this range if requested.
			 */
			if( pRangeResult != NULL) {
				*pRangeResult = *pRange;
			}

			/*
			 * If whole rows selected then accept that.
			 */
			if( pRange->IsRows()) {
				return CGX_MS_MODE_ROW;
			}	

			/*
			 * If whole Columns selected then accept that
			 * (although note that column selection should be disabled).
			 */
			if( pRange->IsCols()) {
				return CGX_MS_MODE_COL;
			}

			if( pRange->GetHeight() > 1) {
				/*
				 * More than one row.
				 */
				if( pRange->GetWidth() > 1) {
					/*
					 * More than one column so it's a block.
					 */
					return CGX_MS_MODE_CELLS_IN_BLOCK;
				} else {
					/*
					 * Just one column: selection of cells in a column.
					 */
					return CGX_MS_MODE_CELLS_IN_COL;
				}
			} else {
				/*
				 * One row
				 */
				if( pRange->GetWidth() > 1) {
					/*
					 * More than one column.
					 */
					return CGX_MS_MODE_CELLS_IN_ROW;
				} else {
					/*
					 * Single cell selected.
					 */
					return CGX_MS_MODE_CELL;
				}
			}
		}
	} else {
		/*
		 * No big block selection, if a single cell has been selected then use that.
		 */
		ROWCOL nRow;
		ROWCOL nCol;

		if( GetCurrentCell( &nRow, &nCol) == TRUE) {
			if( nCol > 0 && nRow > 0) {
				if( pRangeResult != NULL) {
					*pRangeResult = CGXRange( nRow, nCol);
				}

				return CGX_MS_MODE_CELL;
			}
		}
	}

	return CGX_MS_MODE_OFF;
}

/*
 * Determine whether any of the cells in a range are pastable.
 */
bool CGXBatchGrid::IsRangePastable( CGXRange *pRange)
{
	/*
	 * Pastable if ANY of the cells is pastable, not ALL.
	 */
	for( ROWCOL nRow = pRange->top; nRow <= pRange->bottom; nRow++) {
		for( ROWCOL nCol = pRange->left; nCol <= pRange->right; nCol++) {
			if( IsCellPastable( nRow, nCol) != false) {
				return true;
			}
		}
	}

	return false;
}

/*
 * Determine whether a cell is pastable.
 */
bool CGXBatchGrid::IsCellPastable( ROWCOL nRow, ROWCOL nCol)
{
	/*
	 * Range includes row header and hence is read only.
	 */
	if( nRow == 0) {
		return false;
	}

	/*
	 * Range includes column header and hence is read only.
	 */
	if( nCol == 0 && nCol >= (ROWCOL)m_pBatch->GetNumCols()) {
		return false;
	}

	/*
	 * If column is not free form then consider it read only.
	 */
	if( !m_pBatch->GetFldProp()->IsColFreeForm( nCol - 1)) {
		return false;
	}

	CUGCell oCell;
	
	m_pBatch->GetCell( nCol - 1, nRow - 1, &oCell);

	if( oCell.GetReadOnly()) {
		return false;
	}

	return true;
}

bool CGXBatchGrid::CopySampleToDataSource(COleDataSource* pDataSource,long iSamp, ECopyType copyType) 
{
	// Get sample pointer
	if( iSamp == LUC_SAMP_UNDEF) {
		CRowColArray oRows;

		GetSelectedRows( oRows, TRUE);

		if( oRows.GetSize() == 0) {
			return false;
		}

		iSamp = (long)oRows[0] - 1;
	}

	ASSERT(m_pBatch != NULL);
	CLucSamp* pSamp = m_pBatch->GetSamplePtr(iSamp);
	ASSERT(pSamp != NULL);

	// Make an appropriate reference
	CSampRef sampRef;
	GetSampRef(&sampRef, iSamp);	
	// may or may not succeed
	// depending on whether workload has been saved.

	return pSamp->CopySampleToDataSource(pDataSource, sampRef, copyType);
}

bool CGXBatchGrid::AppendSampleFromDataObject(COleDataObject* pDataObject)
{
	ASSERT(m_pBatch != NULL);
	if (m_pBatch == NULL)
	{
		return false;
	}

	// Append new sample
	if (m_pBatch->AppendSample() != KO_OK)
	{
		return false;
	}

	// Get sample ptr and index
	CLucSamp* pSamp = m_pBatch->GetSamplePtr();
	ASSERT(pSamp != NULL);

	// If Retest or Retest Part create reference to new sample
	CSampRef selfRef;
	ECopyType copyType = pSamp->GetCopyType(pDataObject);
	if( copyType == RETEST_COPY || copyType == RETEST_PARTS){
		if( CheckWklSaved()) {
			VERIFY(GetSampRef(&selfRef)); 
		} else {
			return false;
		}
	}

	// Sample does the rest
	bool rv = pSamp->LoadSampleFromDataObject(pDataObject, selfRef);

	// Redraw grid
	CWklDoc* pWklDoc = GetWklDoc();
	ASSERT(pWklDoc != NULL);

	/*
	 * Document modified, need to save it.
	 */
	pWklDoc->SetModifiedFlag();

	return rv; 
}

CString	CGXBatchGrid::GetWklFileSpec()
{
	// Get document pointer
	CWklDoc* pWklDoc = GetWklDoc();
	ASSERT(pWklDoc != NULL);

	// Get the workload file name
	return pWklDoc->GetPathName();
}

bool CGXBatchGrid::CheckWklSaved()
{
	// Determine if workload has ever been saved to disk
//	if( !GetWklFileSpec().IsEmpty()) {
//		return true;
//	}

	CWklDoc* pWklDoc = GetWklDoc();
	ASSERT(pWklDoc != NULL);

	/*
	 * We need the document to have a path name for the reference
	 * from the old workload. If it doesn't have a path name then
	 * force user to save it and give it a name.
	 */
	if( pWklDoc->GetPathName() != _T("")) {
		return true;
	}

	// This workload is not yet saved
	if (AfxMessageBox(IDS_MUST_SAVE_WKL, MB_OKCANCEL) == IDOK)
	{	// Get document pointer
		// Initiate file save
//		AfxGetMainWnd()->SendMessage(WM_COMMAND, (WPARAM)ID_FILE_SAVE_AS);

		// Check if the path is now available
		if( pWklDoc->DoFileSave()) {
			return true;
		} else {
			return false;
		}
	}
	else
	{
		return false;
	}
}

CWklDoc* CGXBatchGrid::GetWklDoc()
{
	if (GetSafeHwnd() != NULL)
	{
		CBatchGridPage* pParent = (CBatchGridPage*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CBatchGridPage)));
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

bool CGXBatchGrid::GetSampRef(CSampRef* pSampRef, int iSamp)
{
	bool rv = true;

	// Get the workload file name
	CString wklFileSpec = GetWklFileSpec();
	if (!wklFileSpec.IsEmpty())
	{	// Has a valid file name, so create sample reference
		// Get document pointer
		CWklDoc* pWklDoc = GetWklDoc();
		ASSERT(pWklDoc != NULL);

		int iBatch = GetBatchIndex();
		ASSERT(iBatch != LUC_BATCH_UNDEF);

		// Check sample number
		if (iSamp == LUC_SAMP_UNDEF)
		{	// If none, use current sample
			CBatch* pBatch = pWklDoc->GetBatchPtr(iBatch);
			ASSERT(pBatch != NULL);
			iSamp = pBatch->GetCurSample();
		}

		// Create generic sample referene
		*pSampRef = CSampRef(iSamp, iBatch, wklFileSpec);
	}
	else
	{
		rv = false;
	}

	return rv;
}

int CGXBatchGrid::GetBatchIndex()
{
	if (GetSafeHwnd() != NULL)
	{
		CBatchGridPage* pParent = (CBatchGridPage*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CBatchGridPage)));
			return pParent->GetBatchIndex();
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

/*
 * About to write changes to disk.
 * If bRevoke is true then undo changes user has made.
 * If bRevoke is flase then commit changes.
 */
void CGXBatchGrid::CommitEdits( bool bRevoke)
{
	for( long nRow=0; nRow < m_pBatch->GetNumRows(); nRow++) {
		for( long nCol=0; nCol < m_pBatch->GetNumCols(); nCol++) {
			CUGCell oCell;
			CString strMask;

			m_pBatch->GetCell( nCol, nRow, &oCell);

			oCell.GetMask( &strMask);

			if( bRevoke == true) {
                /*
                 * Revoking: write original version in mask back to data.
                 */
				if( strMask != _T("")) {
					if( strMask == BLANK_MARKER_STRING) {
                        strMask = _T("");
                    }
					oCell.SetText( strMask);
                }
            }

            /*
             * Committing changes: blank mask fields to store new edits.
             */
            oCell.SetMask( _T(""));

			m_pBatch->SetCell( nCol, nRow, &oCell);
        }
    }
}

/*
 * By default, dragging a range of cells into excel does a move, i.e. it leaves the cells blank.
 * This is no good so have to hack the 'drageffect' value to munge it into a copy operation.
 * Also, stuff dragged from excel is moved. The only valid place for a move is when moving
 * to and from the grid itself.
 */
BOOL CGXBatchGrid::DndStartDragDrop(ROWCOL nRow, ROWCOL nCol)
{
	/*
	 * Remember it's me that is the drag and drop source
	 */
	m_bDoingDragDrop = true;

	/*
	 * Do the drag and drop.
	 */
	BOOL bRet = CGXGridCore::DndStartDragDrop( nRow, nCol);

	/*
	 * It's not me any more.
	 */
	m_bDoingDragDrop = false;

	return bRet;
}

/*
 * When DND starts, add extra possible drop formats. In this case we add sample rows for retests.
 */

/* 
 * CopySampleToDataSource below does not get called in the release version unless optimizer (sic) is
 * disabled.
 */
#pragma optimize( "", off )

BOOL CGXBatchGrid::OnDndCacheGlobalData(CGXNoOleDataSourceProxy* pSrcItem, const CGXRangeList& selList, ROWCOL& nDndRowExt, ROWCOL& nDndColExt)
{
	BOOL bRet = CGXGridCore::OnDndCacheGlobalData( pSrcItem, selList, nDndRowExt, nDndColExt);

	COleDataSource* pDataSource = ((CGXOleDataSourceProxy*) pSrcItem)->GetOleDataSource();

	if( !GetWklFileSpec().IsEmpty()) {	// Workload has a filename so a reference can be made
		// Load sample data into OLE Data Source

		CRowColArray oTopRows;
		CRowColArray oBottomRows;

		if( selList.GetRowArray( GetRowCount(), oTopRows, oBottomRows, FALSE)) {
			for( ROWCOL n = 0; n < (ROWCOL)oTopRows.GetSize(); n++) {
				for( ROWCOL nRow = oTopRows[n]; nRow <= oBottomRows[n]; nRow++) {
					if( nRow > 0) {
						CopySampleToDataSource( pDataSource, nRow - 1, RETEST_COPY);
					}
				}
			}
		}

		return TRUE;
	}
	
	return bRet;
}
#pragma optimize( "", on )

/*
 * This function gets called when we are dragging over the grid: if dragging 
 * over another application (e.g. excel) then this is not called.
 */
DROPEFFECT CGXBatchGrid::OnGridDragOver(CGXNoOleDataObjectProxy* pDataObject,	DWORD dwKeyState, CPoint point)
{
	DROPEFFECT nDropEffect = CGXGridCore::OnGridDragOver( pDataObject, dwKeyState, point);

	if( nDropEffect == DROPEFFECT_NONE) {
		return nDropEffect;
	}

	/*
	 * If not writable then do not allow drag.
	 */
	if( m_pBatch->GetStatus() != LUC_BSTAT_NEW) {
		return DROPEFFECT_NONE;
	}

	DROPEFFECT dropEffect;

	if( m_bDoingDragDrop == true) {
		/*
		 * Within this app we move or copy.
		 */
		if(( dwKeyState & MK_CONTROL) == MK_CONTROL) { 
			dropEffect = DROPEFFECT_COPY; // copy data
		} else { 
			dropEffect = DROPEFFECT_MOVE; // move data
		}
	} else {
		/*
		 * Data coming in from outside: default to copying it.
		 */
		if(( dwKeyState & MK_CONTROL) == MK_CONTROL) { 
			dropEffect = DROPEFFECT_MOVE; // move data
		} else { 
			dropEffect = DROPEFFECT_COPY; // copy data
		}
	}

	return dropEffect;
}

BOOL CGXBatchGrid::OnGridDrop(CGXNoOleDataObjectProxy* pProxyDataObject, DROPEFFECT dropEffect, CPoint point)
{
	/*
	 * If not writable then do not allow drag.
	 */
	if( m_pBatch->GetStatus() != LUC_BSTAT_NEW) {
		return FALSE;
	}

	if( dropEffect & DROPEFFECT_MOVE) {
		/*
		 * Attempting a move: is this good?
		 * Only allow move if both the source and the target is this grid.
		 */
		if( m_bDoingDragDrop == true) {
			; // allow the move
		} else {
			dropEffect &= ~DROPEFFECT_MOVE;
			dropEffect |= DROPEFFECT_COPY;
		} 
	} else if( dropEffect & DROPEFFECT_COPY) {
		/*
		 * Attempting a copy: if within the bounds of the grid then munge it into a move.
		 */
		if( m_bDoingDragDrop == true) {
			dropEffect &= ~DROPEFFECT_COPY;
			dropEffect |= DROPEFFECT_MOVE;
		} 
	}

	return CGXGridCore::OnGridDrop( pProxyDataObject, dropEffect, point);
}

BOOL CGXBatchGrid::OnDndDropData(CGXNoOleDataObjectProxy* pOleDataObjectProxy, ROWCOL nRow, ROWCOL nCol)
{
	/*
	 * See if we are dropping in the blank space below the grid
	 */
	if( nRow == GX_INVALID || nRow >= GetRowCount()) {
		/*
		 * See if sample references are being dragged and dropped.
		 */
		COleDataObject* pDataObject = ((CGXOleDataObjectProxy*) pOleDataObjectProxy)->GetOleDataObject();

		FORMATETC etc;

		// Determine how many formats are available on the clipboard.

		pDataObject->BeginEnumFormats();

		while( pDataObject->GetNextFormat( &etc )) {
			if( pDataObject->IsDataAvailable( etc.cfFormat)){
				if( etc.cfFormat == CLucSamp::CF_SAMPLE) {
					// Append and load sample
					AppendSampleFromDataObject( pDataObject);

					Redraw( GX_INVALIDATE);
					return TRUE;
				}
			}
		}
	}

	return CGXGridCore::OnDndDropData( pOleDataObjectProxy, nRow, nCol);
}

/*
 * This is all copied from CGXGridCore and is modified to fix a silly bug where if a range of
 * cells is pasted and the cells cannot be contained in the width or height of the grid then
 * it generates an assertion error. It is hard to believe that it has such a stupid bug or that 
 * fixing it involves copying so much code.
 */
BOOL CGXBatchGrid::SetStyleRange(
	const CGXRange& range,
	const CGXStyle* pStyle,
	GXModifyType mt,
	int nType /*= 0*/,
	const CObArray* pCellsArray /*= NULL*/,
	UINT flags /*= GX_UPDATENOW*/,
	GXCmdType ctCmd /*= gxDo*/)
{
	// Is it called to execute the command
	if (ctCmd == gxDo)
	{
		// ... then pCellsArray must be 0

		if (pStyle == NULL)
		{
			ASSERT(mt == gxRemove || pCellsArray != NULL);
			// ASSERTION-> Do you want to remove the style or is it a bug that pStyle is NULL? ->END
		}
		else
		{
			ASSERT_VALID(pStyle); ASSERT(pStyle->IsKindOf(RUNTIME_CLASS(CGXStyle)));
			// ASSERTION-> style is invalid ->END

			ASSERT(mt != gxRemove);
			// ASSERTION-> If you want to remove the style, pStyle must be NULL! ->END

			ASSERT(pCellsArray == NULL);
		}
	}
	else // undo, redo or rollback the cmd
	{
		// ... pCellsArray contains cells to be restored, pStyle must be 0

		ASSERT(pStyle == NULL);

		ASSERT(pCellsArray != NULL);
		ASSERT_VALID(pCellsArray);
		ASSERT(pCellsArray->IsKindOf(RUNTIME_CLASS(CObArray)));
	}

	if ((range.IsRows() || range.IsCols() || range.IsTable()) && nType == 0)
		nType = -1;
	
	int nUndoType = nType;

	if (nType == 0)
		nUndoType = m_nExpressionValueType;

	// start op, generate undo info
	CGXLongOperation theOp;

	BOOL bIsMouseAction = GetHitState() > 0;

	theOp.SetStatusText(m_pAppData->strmUndoInfo, FALSE);

	theOp.SetCanRollback(ctCmd == gxDo);

	BOOL bDone = FALSE;

	CObArray* pOldCellsArray = NULL;    // will be filled with style setting

	// generate undo info
	if (m_pUndoImp && ctCmd != gxRollback && m_pParam->m_bUndoEnabled)
	{
		DWORD dwSize = range.GetWidth()*range.GetHeight();

		// Uurgh ... too much!
		if (dwSize >= INT_MAX || range.GetWidth() > SHRT_MAX || range.GetHeight() > SHRT_MAX)
		{
			if (AfxMessageBox(GX_IDS_CANNOTUNDO, MB_OKCANCEL) == IDCANCEL)
				return FALSE;
		}
		else
		{
			// Get style info for cells
			TRY
			{
				pOldCellsArray = GetStyleArray(range, nUndoType, bIsMouseAction ? NULL : &theOp);
			}
			CATCH(CMemoryException, e)
			{
				// shall I continue?
				GX_THROW_LAST
				if (AfxMessageBox(GX_IDS_CANNOTUNDO, MB_OKCANCEL) == IDCANCEL)
					return FALSE;
			}
			AND_CATCH(CUserException, e)
			{
				// user aborted, clean up
				GX_THROW_LAST
				return FALSE;
			}
			END_CATCH
		}
	}

	// Now, pOldCellsArray contains the old cell's data

	CGXRange RollbackRange;

	if (ctCmd == gxDo)
		theOp.SetStatusText(m_pAppData->strmStoreCells, FALSE);
	else
		theOp.SetStatusText(m_pAppData->strmRestoreCells, FALSE);

	theOp.SetCanRollback(ctCmd == gxDo && pOldCellsArray != NULL);

	// make sure all cached data are released.
	// BOOL b = m_bLockGDIObjects;
	// m_bLockGDIObjects = TRUE;
	FreeObjectCache();
	// m_bLockGDIObjects = b;

	// bulk add range so that StoreStyleRowCol does not 
	// call AddRecalcRange for each cell.
	if (!m_bLockAddRecalcRange)
		AddRecalcRange(range);

	BOOL bOldLockAddRecalcRange = m_bLockAddRecalcRange;
	m_bLockAddRecalcRange = TRUE;

	// Let engine or CGXData copy cells in memory (and also
	// create undo info for tools and matrix cells)
	BOOL bAdvUndo = SetAdvancedUndo(m_pParam->m_bUndoEnabled && ctCmd != gxRollback);

	DWORD dwSize = range.GetWidth()*range.GetHeight();

	// Latest member in pCellsArray array might contain advanced undo info 
	if (pCellsArray && dwSize < (DWORD) pCellsArray->GetSize())
	{
		CObject* pAdvUndo = (CObject*) pCellsArray->GetAt((int) dwSize);

		// Make sure this is a ObList
		if (pAdvUndo)
		{
			// Let engine handle this advanced undo info and
			// set GX_UPDATE flags for those cells that need
			// repainting
			if (UndoAdvanced(pAdvUndo, ctCmd != gxRollback, FALSE))
				RefreshViews();
		}
	}

	// Store styles
	TRY
	{
		ROWCOL ncRow = GX_MAXROWCOL, ncCol = GX_MAXROWCOL;
		GetCurrentCell(ncRow, ncCol);

		// store styles
		BOOL bAbort = FALSE;
		for (ROWCOL nRow = range.top; nRow <= range.bottom; nRow++)
		{
			/*
			 * pcw: do not process if target row is outside grid.
			 */
			if( nRow > GetRowCount()) {
				continue;
			}

			for (ROWCOL nCol = range.left; nCol <= range.right; nCol++)
			{
				DWORD dwIndex;

				/*
				 * pcw: do not process if target column is outside grid.
				 */
				if( nCol > GetColCount()) {
					continue;
				}

				dwIndex = (nRow-range.top)*range.GetWidth()+(nCol-range.left);

				// process cells array
				if (pCellsArray)
				{
					if (dwIndex >= (DWORD) pCellsArray->GetSize())
						break;

					CGXStyle* pStyle = (CGXStyle*) pCellsArray->GetAt((int) dwIndex);

					if (nType >= 0)
					{
						CGXStyle* pOldStyle = CreateStyle();

						ComposeStyleRowCol(nRow, nCol, pOldStyle);

						// base styles (column, row ?)
						WORD nControlId = pStyle && pStyle->GetIncludeControl()
							? pStyle->GetControl()
							: pOldStyle->GetControl();

						CGXControl* pControl = GetRegisteredControl(nControlId);

						//
						// CGXControl::StoreStyle will call CGXGridCore::StoreStyleRowCol
						// and check if cell is read-only.
						//
						// In OG 1.1, CGXGridCore::StoreStyleRowCol was directly called. This has
						// changed in version 1.2, in order to make the behavior more cell-oriented
						// and allow specific controls to modify the style object before it is stored
						// in the datastructure. The new RTF control will use the passed style object
						// and change the value depending on the new formattings. If the style object
						// is for example _T("Bold") and StoreStyle is called for an active RTF
						// control, the RTF control will make the paragraph bold (by changing
						// the cell's value which contains rich text formattings).
						//
						// NOTE: The check whether a cell is read-only has been moved from
						// CGXGridCore::StoreStyleRowCol to CGXControl::StoreStyle
						//

						if (pStyle)
							bDone |= pControl->StoreStyle(nRow, nCol, pStyle, mt, nType, pOldStyle);
						else
							bDone |= pControl->StoreStyle(nRow, nCol, NULL, gxRemove, nType, pOldStyle);

						RecycleStyle(pOldStyle);
					}
					else
					{
						if (pStyle)
							bDone |= StoreStyleRowCol(nRow, nCol, pStyle, mt, nType);
						else
							bDone |= StoreStyleRowCol(nRow, nCol, NULL, gxRemove, nType);
					}
				}
				else
				{
					if (nType >= 0)
					{
						if (m_pOldStyle)
						{
							WORD nControlId = pStyle && pStyle->GetIncludeControl()
								? pStyle->GetControl()
								: m_pOldStyle->GetControl();

							CGXControl* pControl = GetRegisteredControl(nControlId);

							bDone |= pControl->StoreStyle(nRow, nCol, pStyle, mt, nType, m_pOldStyle);

						}
						else
						{
							CGXStyle* pOldStyle = CreateStyle();

							ComposeStyleRowCol(nRow, nCol, pOldStyle);

							WORD nControlId = pStyle && pStyle->GetIncludeControl()
								? pStyle->GetControl()
								: pOldStyle->GetControl();

							CGXControl* pControl = GetRegisteredControl(nControlId);

							bDone |= pControl->StoreStyle(nRow, nCol, pStyle, mt, nType, pOldStyle);

							RecycleStyle(pOldStyle);
						}

						if (ncRow == nRow && ncCol == nCol)
						{
							// set internal cached current cell dirty
							m_pStyleEdit->Free();
							m_pCurrentCellControl = NULL;
						}
					}
					else
					{
						// base styles (column, row and table style)

						// ... or one style only
						bDone |= StoreStyleRowCol(nRow, nCol, pStyle, mt, nType);

						if (nRow == 0 && nCol == 0 || nRow == ncRow || nCol == ncCol)
						{
							// set internal cached current cell dirty
							m_pStyleEdit->Free();
							m_pCurrentCellControl = NULL;
						}

					}
				}

				// check, if user pressed ESC to cancel
				if (!bIsMouseAction && theOp.NeedMessages())
				{
					theOp.SetPercentDone((int) (dwIndex * 100 / dwSize));

					theOp.DoMessages(bAbort);

					if (bAbort)
					{
						if (nRow == range.top)
							RollbackRange.SetCells(nRow, range.left, nRow, nCol);
						else
							RollbackRange.SetCells(range.top, range.left, nRow, range.right);

						AfxThrowUserException();
					}
				}
			}
		}

		// update
		if (bDone)
		{
			SetAdvancedUndo(bAdvUndo);

			// Delay float cell evaluation of this range
			DelayFloatCells(range);
			DelayMergeCells(range);

			UpdateStyleRange(range, pStyle, mt, nType, flags, TRUE);

			SetModifiedFlag();

			if (pOldCellsArray)
			{
				// Latest member in pOldCellsArray will be assigned advanced undo info (a CObList with CObjects)
				// that we need to initialize here. 
				CObject* pAdvUndo = GetAdvancedUndoInfo();

				if (pAdvUndo)
					pOldCellsArray->SetAtGrow((int) dwSize, pAdvUndo);

				AddCommand(m_pUndoImp->CreateSetStyleRangeCmd(range, nUndoType, pOldCellsArray), ctCmd);
			}

			if (ctCmd == gxRedo || ctCmd == gxUndo)
				OnCellUndone(range.top, range.left, TRUE);
		}
		else if (pOldCellsArray)
		{
			CleanAdvancedUndo();

			// nothing changed, free undo information
			for (int i = 0; i < pOldCellsArray->GetSize(); i++)
				delete pOldCellsArray->GetAt(i);

			delete pOldCellsArray;
		}

	}
	CATCH(CUserException, e)
	{
		// operation aborted
		// is it possible to rollback the command?
		if (pOldCellsArray && theOp.GetRollbackConfirmedState())
		{	

			if (UndoAdvanced(GetAdvancedUndoInfo(), FALSE, FALSE))
				RefreshViews();

			// Undo
			SetStyleRange(RollbackRange, NULL, gxCopy, nUndoType, pOldCellsArray, flags, gxRollback);

			for (int i = 0; i < pOldCellsArray->GetSize(); i++)
				delete pOldCellsArray->GetAt(i);

			delete pOldCellsArray;
		}
		else if (pOldCellsArray)
		{
			// Latest member in pOldCellsArray will be assigned advanced undo info (a CObList with CObjects)
			// that we need to initialize here. 
			CObject* pAdvUndo = GetAdvancedUndoInfo();

			if (pAdvUndo)
				pOldCellsArray->SetAtGrow((int) dwSize, pAdvUndo);

			AddCommand(m_pUndoImp->CreateSetStyleRangeCmd(range, nUndoType, pOldCellsArray), ctCmd);
		}
		else
		{
			// Undo is not possible
			UpdateStyleRange(RollbackRange, pStyle, mt, nUndoType, flags, TRUE);
		}

		bDone = FALSE;

		GX_THROW_LAST
	}
	END_CATCH

#if defined(_DETECTFALSE) && defined(_DEBUG)
	if (!bDone)
	{
		TRACE0("Warning: SetStyleRange(range,pStyle) returned FALSE.\n");
		TRACE(_T("range: (%lu,%lu,%lu,%lu)\n"), range.top, range.left, range.bottom, range.right);
	}
#endif

	SetAdvancedUndo(bAdvUndo);

	// Reset bulk addrange
	m_bLockAddRecalcRange = bOldLockAddRecalcRange;

	return bDone;
}

/*
 * Install own version of CGXOleDataSourceProxy.
 * This is done to prevent DRAGEFFECT_MOVE from clearing the cells in the grid 
 * after dragging contents over to excel.
 */
BOOL CGXBatchGrid::EnableOleDataSource(DWORD dwFlags)
{
	if( m_pOleDataSourceProxy != NULL) {
		delete m_pOleDataSourceProxy;
		m_pOleDataSourceProxy = NULL;
	}

	m_nDndFlags = dwFlags;
	m_nClipboardFlags = dwFlags; // sync clipboard options

	/*
	 * Display annoying paste range confirmation message.
	 */
	m_nDndFlags &= ~GX_DNDDIFFRANGEDLG;
	m_nClipboardFlags &= ~GX_DNDDIFFRANGEDLG;
	
	if ( dwFlags == GX_DNDDISABLED )
		m_bHitTestSelEdge = FALSE;
	else
		m_bHitTestSelEdge = TRUE;

	m_pOleDataSourceProxy = new CGXBatchGridOleDataSourceProxy;
	return m_pOleDataSourceProxy != NULL;
}

DROPEFFECT CGXBatchGridOleDataSourceProxy::DoDragDrop(DWORD dwEffects,
		LPCRECT lpRectStartDrag, COleDropSource* pDropSource)
{
	return CGXOleDataSourceProxy::DoDragDrop( dwEffects & ~DROPEFFECT_MOVE, lpRectStartDrag, pDropSource);
}

/*
 * When column has been resized successfully, remember the new size.
 */
BOOL CGXBatchGrid::OnTrackColWidthEnd(ROWCOL nCol, int nWidth)
{
	BOOL nRet = CGXGridWnd::OnTrackColWidthEnd( nCol, nWidth);

	if( nRet == TRUE) {
		m_pBatch->GetFldProp()->SetColWidth( nCol - 1, nWidth);
	}

	return nRet;
}

/*
 * Save column widths.
 */
void CGXBatchGrid::SaveColumnWidths( void)
{
	for( ROWCOL nCol = 1; nCol < GetColCount(); nCol++) {
		int nWidth = GetColWidth( nCol);

		m_pBatch->GetFldProp()->SetColWidth( nCol - 1, nWidth);
	}
}

BOOL CGXBatchGrid::OnGridKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_RETURN) {
		WORD nAction = GetParam()->GetEnterKeyAction();
		
		if (TransferCurrentCell(TRUE, GX_INVALIDATE) && nAction != 0) {
			BOOL nRet = MoveCurrentCellEx(nAction, 1);

			if( nRet == 0) {
				/*
				 * Move (down) failed: could be because validation failed or because there is no
				 * cell below, grid doesn't tell us.
				 * Try moving cell right, validation will still fail.
				 */
				nRet = MoveCurrentCellEx( GX_RIGHT, 1);

				if( nRet != 0) {
					/*
					 * Managed to move right. Now move up to the top of the column.
					 */
					do {
						nRet = MoveCurrentCellEx( GX_UP, 1);
					} while( nRet != 0);
				}
			}
		}

		return TRUE;
	} else {
		return CGXGridWnd::OnGridKeyDown( nChar, nRepCnt, nFlags);
	}
}

/**
 * Clear the undo history for the grid. This should be done whenever changes are made that cannot be undone or redone
 * by the undo mechanism, such as adding a sample.
 */
void CGXBatchGrid::ClearUndo()
{
	GetParam()->EmptyUndoList();
	GetParam()->EmptyRedoList();
}

/**
 * Copy and paste of grid code to do hit testing. Pixel range for edge detection was hard wired at four, changed to be
 * a fraction of the size of the cell.
 * @param  ncRow  See GX Grid Documentation
 * @param  ncCol  See GX Grid Documentation
 * @param  ptTest See GX Grid Documentation
 * @param  rect   See GX Grid Documentation
 * @return        See GX Grid Documentation
 */
int CGXBatchGrid::OnExtHitTest(ROWCOL ncRow, ROWCOL ncCol, CPoint& ptTest, CRect& rect)
{
	if (!(m_bHitTestSelEdge || GetParam()->m_nExcelLikeFillFlags))
		return GX_NOHIT;

	int nEdgeY = rect.Height() / 3;
	int nEdgeX = rect.Width() / 5;

	BOOL bSelEdge = FALSE;
	POSITION pos = 0; // use pos as a recheck that FindRange did return a valid position
	BOOL bValid = TRUE;
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	CGXRange rg;
	CGXRange* pRange = NULL;

	// check if whole table is selected
	BOOL bTableSel = pSelList->FindRange(0,0) != 0;

	if (bTableSel)
	{
		bSelEdge = ncRow == 0 && ncCol == 0
			// check left border
			&& (ptTest.x >= rect.left && ptTest.x-rect.left <= nEdgeX
			// check top border
			|| ptTest.y >= rect.top && ptTest.y-rect.top <= nEdgeY);
	}
	else
	{
		if (pSelList->IsEmpty())
		{
			rg.SetCells(ncRow, ncCol);
			GetCoveredCellsRowCol(ncRow, ncCol, rg);

			if (IsCurrentCell(rg.top, rg.left))
				pRange = &rg;
		}

		// check left border
		if (bValid && !bSelEdge && ptTest.x >= rect.left && ptTest.x-rect.left <= nEdgeX)
		{
			if (!pRange)
			{
				pos = pSelList->FindRange(ncRow, ncCol);
				if (pos != 0)
					pRange = pSelList->GetAt(pos);
				else
					bValid = FALSE;
			}
			bSelEdge = pRange && !pRange->IsRows() && pRange->left == ncCol;
		}

		// check right border
		if (bValid && !bSelEdge && ptTest.x <= rect.right && rect.right-ptTest.x <= nEdgeX)
		{
			if (!pRange)
			{
				pos = pSelList->FindRange(ncRow, ncCol);
				if (pos != 0)
					pRange = pSelList->GetAt(pos);
				else
					bValid = FALSE;
			}
			bSelEdge = pRange && !pRange->IsRows() && pRange->right == ncCol;
		}

		// check top border
		if (bValid && !bSelEdge && ptTest.y >= rect.top && ptTest.y-rect.top <= nEdgeY)
		{
			if (!pRange)
			{
				pos = pSelList->FindRange(ncRow, ncCol);
				if (pos != 0)
					pRange = pSelList->GetAt(pos);
				else
					bValid = FALSE;
			}
			bSelEdge = pRange && !pRange->IsCols() && pRange->top == ncRow;
		}

		// check bottom border
		if (bValid && !bSelEdge && ptTest.y <= rect.bottom && rect.bottom-ptTest.y <= nEdgeY)
		{
			if (!pRange)
			{
				pos = pSelList->FindRange(ncRow, ncCol);
				if (pos != 0)
					pRange = pSelList->GetAt(pos);
				else
					bValid = FALSE;
			}
			bSelEdge = pRange && !pRange->IsCols() && pRange->bottom == ncRow;
		}
	}

	if(bSelEdge && GetParam()->m_nExcelLikeFillFlags)
	{
		// The hit area 4X4 is hardcoded, you can change this to increase the hit-area...
		// pcw- this has been done...
		if((abs(ptTest.x - rect.right) <= nEdgeX) && (abs(ptTest.y - rect.bottom) <= nEdgeY))
		{
			// Store the intial range...
			return GX_FILLHANDLEHIT;
		}
	}


	if (bSelEdge)
		return GX_SELEDGEHIT;
	else
		return GX_NOHIT;
}
