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

/***************************************************
****************************************************
Skeleton Class for a Derived CugWkl
****************************************************
****************************************************/

#include "stdafx.h"
#include "Lucept.h"
#include "CugWkl.h"
#include "WklDoc.h"
#include "WklView.h"
#include "MainFrm.h"
#include "SampRef.h"
#include "BGridPg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////
// Register batch with grid
///////////////////////////////////////////////////////////////////////
void CugWkl::RegisterBatch(CBatch* pBatch)
{
	m_pBatch = pBatch;
	m_CUGGrid.SetPalette(((CLuceptApp*)AfxGetApp())->GetCustPal());
};

/***************************************************
****************************************************/
CugWkl::CugWkl()
{
	m_pBatch = NULL;
	SetHighlightRow(FALSE);
	Set3DHeight(2);

	SetCurrentCellMode(2);	// highlight only
	// Grid drawing with focus is very buggy and inconsistent
	
	EnableMenu(TRUE);
	SetMultiSelectMode(MS_MODE_CELLS_IN_COL);

	//drag and drop setup for side header
	#ifdef __AFXOLE_H__  //OLE must be included
		m_SHDropTarget.m_Ctrl = this;
	#endif

}
/***************************************************
****************************************************/
CugWkl::~CugWkl()
{
}

/***************************************************
OnSetup
	This function is called just after the grid window 
	is created or attached to a dialog item.
	It can be used to initially setup the grid
****************************************************/
void CugWkl::OnSetup()
{
	// Create standard and bold fonts (8 point)
	// Must be valid for both screen and printer
	VERIFY(m_stdFont.CreatePointFont(100, _T("Arial")));
	m_GI.DefaultFont = HFONT(m_stdFont);
	VERIFY(m_boldFont.CreatePointFont(100, _T("Arial Bold")));

}

/***************************************************
OnCanMove
	Sent when the current cell in the grid is about
	to move
	A return of TRUE allows the move, a return of
	FALSE stops the move
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
int CugWkl::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/***************************************************
OnCanMove
	Sent when the top row or left column in the grid is about
	to move
	A return of TRUE allows the move, a return of
	FALSE stops the move
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
int CugWkl::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/***************************************************
****************************************************/
void CugWkl::OnHitBottom(long numrows,long rowspast,long rowsfound)
{	// DAO override to ensure all records are read
	if (rowsfound > 0)
	{
		SetNumberRows(numrows + rowsfound);
	}
}
/***************************************************
****************************************************/
void CugWkl::OnHitTop(long numrows,long rowspast)
{

}
/***************************************************
OnCanSizeCol
	Sent when the user is over a separation line on
	the top heading
	A return value of TRUE allows the possibiliy of
	a resize
****************************************************/
int CugWkl::OnCanSizeCol(int col)
{
	return TRUE;
}
/***************************************************
OnColSizing
	Sent when the user is sizing a column
	The column that is being sized is given as 
	well as the width. Plus the width can be modified 
	at this point. This makes it easy to set min and
	max widths
****************************************************/
void CugWkl::OnColSizing(int col,int *width)
{
	*width = max(*width, LUC_MIN_COL_WIDTH);
}

/***************************************************
OnColSized
	This is sent when the user finished sizing the 
	given column (see above for more details)
****************************************************/
void CugWkl::OnColSized(int col,int *width)
{
	ASSERT(m_pBatch != NULL);

	// Save the new width in the field properties
	VERIFY(m_pBatch->GetFldProp()->SetColWidth(col, *width));

	// This is message is sent to set _T("dirty bit") in Doc
	if (GetWklView() != NULL)
	{
		GetWklView()->OnModifycell((WPARAM)col, (LPARAM)LUC_ROW_UNDEF);
	}
}

/***************************************************
OnCanSizeRow
	Sent when the user is over a separation line on
	the side heading
	A return value of TRUE allows the possibiliy of
	a resize
****************************************************/
int  CugWkl::OnCanSizeRow(long row)
{
	return FALSE;
}
/***************************************************
OnRowSizing
    Sent when the user is sizing a row
	The row that is being sized is given as 
	well as the height. Plus the height can be modified 
	at this point. This makes it easy to set min and
	max heights
****************************************************/
void CugWkl::OnRowSizing(long row,int *height)
{
}
/***************************************************
OnRowSized
	This is sent when the user is finished sizing hte
	given row ( see above for more details)
****************************************************/
void CugWkl::OnRowSized(long row,int *height)
{
}
/***************************************************
OnCanSizeSideHdg
	This is sent when the user moves into position 
	for sizing the width of the side heading
	return TRUE to allow the sizing
	or FALSE to not allow it
****************************************************/
int CugWkl::OnCanSizeSideHdg()
{
	return TRUE;
}
/***************************************************
OnCanSizeTopHdg
	This is sent when the user moves into position 
	for sizing the height of the top heading
	return TRUE to allow the sizing
	or FALSE to not allow it
****************************************************/
int CugWkl::OnCanSizeTopHdg()
{
	return TRUE;
}
/***************************************************
OnSideHdgSizing
****************************************************/
int CugWkl::OnSideHdgSizing(int *width)
{
	return TRUE;
}
/***************************************************
OnTopHdgSizing
****************************************************/
int CugWkl::OnTopHdgSizing(int *height)
{
	return TRUE;
}
/***************************************************
OnColChange
	Sent whenever the current column changes
	The old and the new columns are given
****************************************************/
void CugWkl::OnColChange(int oldcol,int newcol)
{
	// Don't allow column to change if doing a multi-select

	// Paraphrased from CUGCtrl::Moved()
	if( m_GI.MovedBy == 1 && GetKeyState(VK_SHIFT) < 0 ||
		// Keyboard selection in progress
		m_GI.MovedBy == 2 && GetKeyState(VK_SHIFT) < 0 ||
		// Mouse shift click selection in progress
		m_GI.MovedBy == 3)
		// Mouse drag selection in progress
	{
		m_GI.CurrentCol = oldcol;
	}
}
/***************************************************
OnRowChange
	Sent whenever the current row changes
	The old and the new rows are given
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
void CugWkl::OnRowChange(long oldrow,long newrow)
{
}

/***************************************************
OnCellChange
	Sent whenever the current cell changes rows or
	columns
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
void CugWkl::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
	// Raise old row and column headers
	QuickSetBorder(-1, oldrow, UG_BDR_RAISED);
	QuickSetBorder(oldcol, -1, UG_BDR_RAISED);

	// Recess new row and column headers
	QuickSetBorder(-1, newrow, UG_BDR_RECESSED);
	QuickSetBorder(newcol, -1, UG_BDR_RECESSED);

	// Redraw side headers
	RedrawCell(-1, newrow);
	if (newrow != oldrow)
	{
		RedrawCell(-1, oldrow);
	}

	// Redraw top headers
	RedrawTopHdg();		// +++ 18SEP97 P.A.M. Function added to UGCtrl

	// Notify main app window of cell select change
	// Main window is responsible for routing message to view
	if (GetWklView() != NULL)
	{
		GetWklView()->OnSelectcell((WPARAM)newcol, (LPARAM)newrow);
	}
}

/***************************************************
OnLeftColChange
	Sent whenever the left visible column in the grid 
	changes
****************************************************/
void CugWkl::OnLeftColChange(int oldcol,int newcol)
{
	// Redraw top headers
	RedrawTopHdg();		// +++ 21NOV97 P.A.M. 
	// Redraw whole stinkin grid
	RedrawAll();	// +++ 21NOV97 P.A.M. 
}
/***************************************************
OnTopRowChange
	Sent whenever the top visible row in the grid changes
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
void CugWkl::OnTopRowChange(long oldrow, long newrow)
{
	// Redraw top headers
	RedrawTopHdg();		// +++ 08APR98 P.A.M. 
	// Redraw whole stinkin grid
	RedrawAll();	// +++ 08APR98 P.A.M. 
}
/***************************************************
OnLClicked
	Sent whenever the user clicks the left mouse
	button within the grid
	this message is sent when the button goes down
	then again when the button goes up

	'col' and 'row' are negitive if the area clicked
	in is not a valid cell
	'rect' the rectangle of the cell that was clicked in
	'point' the point where the mouse was clicked
	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	if (updn && col >= 0 && row >= 0)
	{	// Reset multiselect selections and mode
		SetMultiSelectMode(MS_MODE_CELLS_IN_COL);

		// Redraw whole stinkin grid
		RedrawTopHdg();	
		RedrawAll();
	}
}
/***************************************************
OnRClicked
	Sent whenever the user clicks the right mouse
	button within the grid
	this message is sent when the button goes down
	then again when the button goes up

	'col' and 'row' are negitive if the area clicked
	in is not a valid cell
	'rect' the rectangle of the cell that was clicked in
	'point' the point where the mouse was clicked
	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
}
/***************************************************
OnDClicked
	Sent whenever the user double clicks the left mouse
	button within the grid

	'col' and 'row' are negitive if the area clicked
	in is not a valid cell
	'rect' the rectangle of the cell that was clicked in
	'point' the point where the mouse was clicked
****************************************************/
void CugWkl::OnDClicked(int col,long row,RECT *rect,POINT *point,int processed)
{
	CUGCell cell;

	// from activating edit on double click

	ASSERT(m_pBatch != NULL);

	GetCell(col, row, &cell);
	if (cell.GetReadOnly() == FALSE && 
		m_pBatch->GetFldProp()->GetColID(col) != SF_REPLS &&
		m_pBatch->GetFldProp()->GetColID(col) != SF_TYPE_XFI)
	{
		StartEdit();
	}
}
/***************************************************
OnTH_LClicked
	Sent whenever the user clicks the left mouse
	button within the top heading
	this message is sent when the button goes down
	then again when the button goes up

	'col' is negitive if the area clicked in is not valid
	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnTH_LClicked(int col,int updn)
{
}
/***************************************************
OnTH_RClicked
	Sent whenever the user clicks the right mouse
	button within the top heading
	this message is sent when the button goes down
	then again when the button goes up

	'col' is negitive if the area clicked in is not valid
	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnTH_RClicked(int col,int updn)
{
}
/***************************************************
OnTH_LClicked
	Sent whenever the user double clicks the left mouse
	button within the top heading

	'col' is negitive if the area clicked in is not valid
****************************************************/
void CugWkl::OnTH_DClicked(int col)
{
}
/***************************************************
OnSH_LClicked
	Sent whenever the user clicks the left mouse
	button within the side heading
	this message is sent when the button goes down
	then again when the button goes up

	'row' is negitive if the area clicked in is not valid
	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnSH_LClicked(long row,int updn)
{
	// For a valid sample, make sample current
	if (updn && row >= 0)
	{
		GotoCell(0, row);

		// And select entire sample
		VERIFY(SetMultiSelectMode(MS_MODE_ROW) == UG_SUCCESS);

		// Single row selection for now
		// SetMultiSelectMode defines current row as StartBlock
		m_MS->EndBlock(0, row);
		
		// Redraw whole stinkin grid
		RedrawAll(); 

		// Allow sample dragging
		StartDragDrop();
	}
}
/***************************************************
OnSH_RClicked
	Sent whenever the user clicks the right mouse
	button within the side heading
	this message is sent when the button goes down
	then again when the button goes up

	'row' is negitive if the area clicked in is not valid
	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnSH_RClicked(long row,int updn)
{
	// For a valid sample, make sample current
	if (updn && row >= 0)
	{
		GotoCell(0, row);

		// And select entire sample
		VERIFY(SetMultiSelectMode(MS_MODE_ROW) == UG_SUCCESS);

		// Single row selection for now
		// SetMultiSelectMode defines current row as StartBlock
		m_MS->EndBlock(0, row);

		// Redraw whole stinkin grid
		RedrawAll(); 
	
		// Pop-up context menu
		OnMenuStart(0, row, UG_SIDEHEADING);
	}

}
/***************************************************
OnSH_DClicked
	Sent whenever the user double clicks the left mouse
	button within the side heading

	'row' is negitive if the area clicked in is not valid
****************************************************/
void CugWkl::OnSH_DClicked(long row)
{
	if (row >= 0)
	{	// Sample is made current by first click

		// Open sample view
		WPARAM wParam = (WPARAM)ID_SAMPLE_VIEW;	// Menu item ID
		LPARAM lParam = (LPARAM)NULL;			// Only used for controls
		AfxGetMainWnd()->
			SendMessage(WM_COMMAND, wParam, lParam);
	}
	else
	{	// Append new row
		if (AppendRow() == UG_SUCCESS)
		{
			GotoRow(GetNumberRows() -1);
		}
	}
}
/***************************************************
OnCB_LClicked
	Sent whenever the user clicks the left mouse
	button within the top corner button
	this message is sent when the button goes down
	then again when the button goes up

	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnCB_LClicked(int updn)
{
}
/***************************************************
OnCB_RClicked
	Sent whenever the user clicks the right mouse
	button within the top corner button
	this message is sent when the button goes down
	then again when the button goes up

	'updn'  TRUE if the button is down FALSE if the
	        button just when up
****************************************************/
void CugWkl::OnCB_RClicked(int updn)
{	
	WPARAM wParam = 0;
	LPARAM lParam = 0;

	if (GetWklView() != NULL)
	{
		GetWklView()->OnGridproperties(wParam, lParam);
	}
}
/***************************************************
OnCB_DClicked
	Sent whenever the user double clicks the left mouse
	button within the top corner button
****************************************************/
void CugWkl::OnCB_DClicked()
{
}
/***************************************************
OnKeyDown
	Sent whenever the user types when the grid has
	focus. The keystroke can be modified here as well.
	(See WM_KEYDOWN for more information)
****************************************************/
void CugWkl::OnKeyDown(UINT *vcKey,int processed)
{
}
/***************************************************
OnCharDown
	Sent whenever the user types when the grid has
	focus. The keystroke can be modified here as well.
	(See WM_CHAR for more information)
****************************************************/
void CugWkl::OnCharDown(UINT *vcKey,int processed)
{
	if(GetKeyState(VK_CONTROL) <0)
	{
		if(*vcKey == 24 && CanEditCut())
		{
			SetMultiSelectMode(MS_MODE_OFF); // Cut only single cells for now
			CutSelected();
			SetMultiSelectMode(MS_MODE_CELLS_IN_COL);
		}
		if(*vcKey == 3 && CanEditCopy())
		{
			SetMultiSelectMode(MS_MODE_OFF); // Cut only single cells for now
			CopySelected();
			SetMultiSelectMode(MS_MODE_CELLS_IN_COL);
		}
		if(*vcKey == 22 && CanEditPaste())
		{
				Paste();
		}
	}
}
	
/***************************************************
OnGetCell
	This message is sent everytime the grid needs to
	draw a cell in the grid. At this point the cell
	class has been filled with the information to be
	used to draw the cell. The information can now be
	changed before it is used for drawing
****************************************************/
void CugWkl::OnGetCell(int col,long row,CUGCell *cell)
{
}

/***************************************************
OnSetCell
	This message is sent everytime the a cell is about 
	to change.
****************************************************/
void CugWkl::OnSetCell(int col,long row,CUGCell *cell)
{
	// Notify main app window of cell contents change
	// Main window is responsible for routing message to view
	if (GetWklView() != NULL)
	{
		GetWklView()->OnModifycell((WPARAM)col, (LPARAM)row);
	}
}

/***************************************************
OnDataSourceNotify
	This message is sent from a data source , message
	depends on the data source - check the information
	on the data source(s) being used
	- The ID of the Data source is also returned
****************************************************/
void CugWkl::OnDataSourceNotify(int ID,long msg,long param)
{
}

/***************************************************
OnCellTypeNotify
	This message is sent from a cell type , message
	depends on the cell type - check the information
	on the cell type classes
	- The ID of the cell type is given
****************************************************/
void CugWkl::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if (ID==UGCT_SPINBUTTON)
	{
		CUGCell cell;

		ASSERT(m_pBatch != NULL);
		// Replicates spin button
		if(m_pBatch->GetFldProp()->GetColID(col) == SF_REPLS)
		{
			GetCell(col,row,&cell);
			if (cell.GetReadOnly() == FALSE)
			{
				double d = cell.GetNumber();

				if (msg == UGCT_SPINBUTTONUP)
				{
					d += 1.0;
				}
				else if (msg == UGCT_SPINBUTTONDOWN)
				{
					d -= 1.0;
				}
				d = max(d, 1);
				d = min(d, MAX_REPLICATES);
				cell.SetNumDecimals(0);	
				cell.SetNumber(d);
				SetCell(col,row,&cell);
			}
		}
	}
}
/***************************************************
OnEditStart
	This message is sent whenever the grid is ready 
	to start editing a cell
	A return of TRUE allows the editing a return of
	FALSE stops editing
	Plus the properties of the CEdit class can be modified
****************************************************/
int CugWkl::OnEditStart(int col, long row,CEdit *edit)
{
	CUGCell cell;
	if (GetCell(col, row, &cell) == UG_SUCCESS &&
		cell.GetCellType() == UGCT_NORMAL &&
		cell.GetReadOnly() == FALSE)
	{
		if (GetWklView() != NULL)
		{
			GetWklView()->OnUserEditing((WPARAM)true, 0);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/***************************************************
OnEditVerify
	This is send when the editing is about to end
****************************************************/
int CugWkl::OnEditVerify(int col, long row,CEdit *edit,UINT *vcKey)
{
	return TRUE;
}
/***************************************************
OnEditFinish this is send when editing is finished
****************************************************/
int CugWkl::OnEditFinish(int col, long row,CEdit *edit)
{
	if (GetWklView() != NULL)
	{
		GetWklView()->OnUserEditing((WPARAM)false, 0);
	}
	return TRUE;
}
/***************************************************
OnEditFinish this is send when editing is finished
****************************************************/
int CugWkl::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	int rv;

	// Check if new cell allows editing; if not, pick next col until 
	// it does or we run out of cols.  Doesn't do anything with rows.
	CUGCell cell;

	// Check if we are past last editable cell in grid
	if (*newrow == oldrow && *newrow == GetNumberRows() - 1 && *newcol == 0)
	{
		// Stay in last cell
		*newcol = oldcol;
		return FALSE;
	}

	while ((rv = GetCell(*newcol,*newrow,&cell)) == UG_SUCCESS &&
			(cell.GetCellType() != UGCT_NORMAL ||
			cell.GetReadOnly() != FALSE))
	{
		(*newcol)++;
	}

	if (rv != UG_SUCCESS)
	{	// No more editable cells in this row.  End edit
		return FALSE;
	}
	else
	{
		// Notify Document we are still editing
		if (GetWklView() != NULL)
		{
			GetWklView()->OnUserEditing((WPARAM)true, 0);
		}
		return TRUE;
	}
}
/***************************************************
sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
			UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
****************************************************/
void CugWkl::OnMenuCommand(int col,long row,int section,int item)
{
	if(section == UG_GRID)
	{
		switch(item)
		{
		case CUGWKL_CUT:
			SetMultiSelectMode(MS_MODE_OFF); // Cut only single cells for now
			CutSelected();
			SetMultiSelectMode(MS_MODE_CELLS_IN_COL);
			break;
		case CUGWKL_COPY:
			SetMultiSelectMode(MS_MODE_OFF);	// Copy only single cells for now
			CopySelected();
			SetMultiSelectMode(MS_MODE_CELLS_IN_COL);
			break;
		case CUGWKL_PASTE:
			Paste();
			break;
		default:
			break;
		}
	}
	else if(section == UG_SIDEHEADING)
	{
		switch(item)
		{
		case CUGWKL_CUT:
			EditCut();
			// Restore default select mode
			SetMultiSelectMode(MS_MODE_CELLS_IN_COL);
			break;

		case CUGWKL_COPY:
			EditCopy(TEMPLATE_COPY);
			break;

		case CUGWKL_PASTE:
			EditPaste();
			break;

		case CUGWKL_TEMPLATE_COPY:
			EditCopy(TEMPLATE_COPY);
			break;

		case CUGWKL_COPY_FOR_RETEST:
			if(CheckWklSaved())
				EditCopy(RETEST_COPY);
			break;

		case CUGWKL_COPY_RETEST_PART:
			if(CheckWklSaved())
				EditCopy(RETEST_PARTS);
			break;

		case CUGWKL_VIRTUAL_COPY:
			if(VirtualCopyCheck() && CheckWklSaved())
				EditCopy(VIRTUAL_COPY);
			break;

		default:
			break;
		}
	}
	
}
/***************************************************
return UG_SUCCESS to allow the menu to appear
return 1 to not allow the menu to appear
****************************************************/
int CugWkl::OnMenuStart(int col,long row,int section)
{
	EmptyMenu();

	// Enable context menu for any valid cell in grid
	if(section == UG_GRID && m_pBatch != NULL && col >= 0 && row >= 0
		|| section == UG_SIDEHEADING  && m_pBatch != NULL  && row >= 0)
	{
		CString menuItem;

		if (CanEditCut())
		{
			VERIFY(menuItem.LoadString(IDS_CUT));
			AddMenuItem(CUGWKL_CUT, menuItem);
		}
		if (CanEditCopy())
		{
			VERIFY(menuItem.LoadString(IDS_COPY));
			AddMenuItem(CUGWKL_COPY, menuItem);
		}
		if (CanEditPaste())
		{
			VERIFY(menuItem.LoadString(IDS_PASTE));
			AddMenuItem(CUGWKL_PASTE, menuItem);
		}

		if (section == UG_SIDEHEADING)
		{	// Add menu separator
			menuItem.Empty();
			AddMenuItem(0, menuItem);

			// Side heading special items
			menuItem.LoadString(IDS_TEMPLATE_COPY);
			AddMenuItem(CUGWKL_TEMPLATE_COPY, menuItem);

			menuItem.LoadString(IDS_COPY_FOR_RETEST);
			AddMenuItem(CUGWKL_COPY_FOR_RETEST, menuItem);

			menuItem.LoadString(IDS_COPY_RETEST_PART);
			AddMenuItem(CUGWKL_COPY_RETEST_PART, menuItem);

			menuItem.LoadString(IDS_VIRTUAL_COPY);
			AddMenuItem(CUGWKL_VIRTUAL_COPY, menuItem);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/***************************************************
****************************************************/
int CugWkl::OnHint(int col,long row,CUGHint *hint)
{
	return TRUE; 
}
/***************************************************
****************************************************/
int CugWkl::OnVScrollHint(long row,int percent,CUGHint *hint)
{
	return TRUE;
}
/***************************************************
****************************************************/
int CugWkl::OnHScrollHint(int col,int percent,CUGHint *hint)
{
	return TRUE;
}

//***************************************************
//****************************************************/

void CugWkl::OnPaletteChanged(CWnd * pWnd)
{	// repaint grid
	RedrawAll();
}


/***************************************************
SetMultiSelectMode
	Turns multiple selection on and off
	off by default

	- Replaces base class function.  Allows extended modes
***************************************************/
int	CugWkl::SetMultiSelectMode(int mode)
{
	switch(mode)
	{
	case MS_MODE_OFF:
		m_GI.MultiSelectFlag = FALSE;
		m_GI.HighlightRowFlag = FALSE;
		ClearSelections();
		m_MS->SelectMode(MS_MODE_OFF);
		break;

	case MS_MODE_ROW:
		m_GI.MultiSelectFlag = TRUE;
		m_GI.HighlightRowFlag = TRUE;
		m_MS->SelectMode(MS_MODE_ROW);
		m_MS->StartBlock(0, m_GI.CurrentRow);
		break;

	case MS_MODE_CELL:
	case MS_MODE_CELLS_IN_COL:
		m_GI.MultiSelectFlag = TRUE;
		m_GI.HighlightRowFlag = FALSE;
		m_MS->SelectMode(mode);
		m_MS->StartBlock(m_GI.CurrentCol,m_GI.CurrentRow);
		break;

	case MS_MODE_COL:
	case MS_MODE_CELLS_IN_ROW:
	default:
		m_GI.MultiSelectFlag = FALSE;
		m_GI.HighlightRowFlag = FALSE;
		ClearSelections();
		m_MS->SelectMode(MS_MODE_OFF);
		return 1;

	}

	return UG_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
// Paste
//	Pastes the string in the clipboard into all selected cells
//  Replaces base class function which pastes an array of strings once
//  return
//	UG_SUCCESS	success
//	1			fail
////////////////////////////////////////////////////////////////////////
int CugWkl::Paste()
{
	return CUGCtrl::Paste();
}

int CugWkl::Paste(CString &string)
{
	int		startcol, endcol;
	long	startrow, endrow;
	CUGCell cell;

	// Get selected range
	VERIFY(m_MS->GetCurrentBlock(&startcol, &startrow, &endcol, &endrow) 
		== UG_SUCCESS);

	ASSERT(m_pBatch != NULL);

	// Fill cell range with string
	for (int col = startcol; col <= endcol; col++)
	{
		if (m_pBatch->GetFldProp()->IsColFreeForm(col))
		{	// Column is _T("FreeForm") and hence pastable
			for (long row = startrow; row <= endrow; row++)
			{
				VERIFY(GetCell(col, row, &cell) == UG_SUCCESS);
				if (!cell.GetReadOnly())
				{
					cell.SetText(string);
					SetCell(col, row, &cell);
				}
			}
		}
	}

	m_CUGGrid.m_DrawHint.AddHint(startcol, startrow, endcol, endrow);

	RedrawAll();

	return UG_SUCCESS;
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
int CugWkl::CopyFromClipBoard(CString* string)
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
	UCHAR c;
	int num = string->GetLength();
	for (int i = 0; i < num; i++)
	{
		c = (UCHAR)string->GetAt(i);
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
// Replace base class function.  Only clear cell text
////////////////////////////////////////////////////////////////////////

void CugWkl::CreateSelectedString(CString& string,int cutFlag)
{
	
	int rt; 
	int col;
	long row,lastrow;
	CUGCell cell;
	
	string = _T("");

	//enum selected items and add them to the string
	rt = m_MS->EnumFirstSelected(&col,&row);
	lastrow = row;
	while(rt == UG_SUCCESS)
	{	//get the selected cell then copy the string
		GetCell(col,row,&cell);
		string += cell.m_ci.String;

		//check the cut flag
		if(cutFlag)
		{
			cell.SetText(_T(""));
			SetCell(col,row,&cell);
		}
		
		//update the last row flag
		lastrow = row;

		//find the next selected item
		rt = m_MS->EnumNextSelected(&col,&row);

		//add line feeds between rows
		if(rt == UG_SUCCESS)
		{
			if(row != lastrow)
			{
				string += _T("\n");
			}
			else
			{	//otherwise add tabs between cols
				string += _T("\t");
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////
// CanEditCut and EditCut
////////////////////////////////////////////////////////////////////////

bool CugWkl::CanEditCut()
{
	if (m_pBatch == NULL)
	{
		return false;
	}

	switch (m_MS->GetMode())
	{
	case MS_MODE_OFF:
	case MS_MODE_CELL:
	case MS_MODE_CELLS_IN_COL:
		// Only for single cell selected
		if (SingleCellSelected())
		{
			int col;
			long row;
			CUGCell cell;

			if(m_MS->EnumFirstSelected(&col, &row) != UG_SUCCESS)
			{
				return false;
			}
			if(GetCell(col, row, &cell) != UG_SUCCESS)
			{
				return false;
			}
			if (cell.GetReadOnly())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}

	case MS_MODE_ROW:
		// If single row selected and batch status is NEW
		return (m_pBatch->GetStatus() == LUC_BSTAT_NEW &&
			SingleRowSelected());

 	case MS_MODE_COL:
	case MS_MODE_CELLS_IN_ROW:
	default:
		return false;
	}	
}

void CugWkl::EditCut()
{
	switch (m_MS->GetMode())
	{
	case MS_MODE_OFF:
	case MS_MODE_CELL:
	case MS_MODE_CELLS_IN_COL:
		{
			// Only for single cell selected
			ASSERT(SingleCellSelected());
			CutSelected();
			// Clear selection and set default multiselect mode
			SetMultiSelectMode(MS_MODE_CELLS_IN_COL);
		}
		break;

	case MS_MODE_ROW:
		{
			int col, rv;
			long iSamp;

			// Get sample number
			rv = m_MS->EnumFirstSelected(&col, &iSamp);
			if (rv != UG_SUCCESS || col != 0)
			{	// Couldn't find the selected row 
				return;
			}

			// Copy to clipboard
			EditCopy();

			// Then delete
			ASSERT(m_pBatch != NULL);
			m_pBatch->DeleteSample(iSamp);
		}
		break;

 	case MS_MODE_COL:
	case MS_MODE_CELLS_IN_ROW:
	default:
		return;
	}	
}


////////////////////////////////////////////////////////////////////////
// CanEditCopy and EditCopy
////////////////////////////////////////////////////////////////////////

bool CugWkl::CanEditCopy(ECopyType copyType)
{
	if (m_pBatch == NULL)
	{
		return false;
	}

	switch (m_MS->GetMode())
	{
	case MS_MODE_OFF:
	case MS_MODE_CELL:
	case MS_MODE_CELLS_IN_COL:
		// Only for single cell selected
		return SingleCellSelected();

	case MS_MODE_ROW:
	{		
		// Only for single row selected
		return SingleRowSelected();
	}

	case MS_MODE_COL:
	case MS_MODE_CELLS_IN_ROW:
	default:
		return false;
	}	
}

void CugWkl::EditCopy(ECopyType copyType)
{
	switch (m_MS->GetMode())
	{
	case MS_MODE_OFF:
	case MS_MODE_CELL:
	case MS_MODE_CELLS_IN_COL:
		{
			// Copy only single cells for now
			SetMultiSelectMode(MS_MODE_OFF);	
			CopySelected();
			SetMultiSelectMode(MS_MODE_CELLS_IN_COL);
		}
		break;

	case MS_MODE_ROW:
		{
			int col, rv;
			long iSamp;

			// Get sample number
			rv = m_MS->EnumFirstSelected(&col, &iSamp);
			if (rv != UG_SUCCESS || col != 0)
			{	// Couldn't find the selected row 
				return;
			}

			// Create an OLE data source containing sample 
			// and sample ref formats
			COleDataSource* pDataSource = new COleDataSource;
			CopySampleToDataSource(pDataSource, iSamp, copyType);

			// Add data source to clipboard
			// Clipboard owns datasource object and must delete it
			pDataSource->SetClipboard();
		}
		break;

	case MS_MODE_COL:
	case MS_MODE_CELLS_IN_ROW:
	default:
		// Don't do nuttin
		break;
	}	
}

////////////////////////////////////////////////////////////////////////
// CanEditPaste and EditPaste
////////////////////////////////////////////////////////////////////////

bool CugWkl::CanEditPaste()
{
	if (m_pBatch == NULL)
	{
		return false;
	}

	// Check clipboard for known formats
	OpenClipboard();
	HGLOBAL hgSamp = GetClipboardData(CLucSamp::CF_SAMPLE);
	HGLOBAL hgText = GetClipboardData(CF_TEXT);
	CloseClipboard();

	if (hgSamp != NULL)
	{
		// It is sample data.  Ignore selection, just check that
		// batch is writable
		if (m_pBatch->GetStatus() == LUC_BSTAT_NEW)
		{
			return true; 
		}
		else
		{
			return false;
		}
	}
	else if (hgText != NULL)
	{
		// It's text but not sample data
		switch (m_MS->GetMode())
		{
		case MS_MODE_OFF:
		case MS_MODE_CELL:
		case MS_MODE_CELLS_IN_COL:
			return PastableCellsSelected();
			break;

		case MS_MODE_ROW:
		case MS_MODE_COL:
		case MS_MODE_CELLS_IN_ROW:
		default:
			return false;
		}	
	}
	else
	{	// It's nothing I expect
		return false;
	}
}

void CugWkl::EditPaste()
{
	ASSERT(m_pBatch != NULL);
	if (m_pBatch == NULL)
	{
		return;
	}

	// Check clipboard for known formats
	OpenClipboard();
	HGLOBAL hgSamp = GetClipboardData(CLucSamp::CF_SAMPLE);
	HGLOBAL hgText = GetClipboardData(CF_TEXT);
	CloseClipboard();

	if (hgSamp != NULL)
	{	// It is sample data.  
		// Create data object from clipboard
		COleDataObject	dataObject;
		VERIFY(dataObject.AttachClipboard());

		// Append and load sample
		AppendSampleFromDataObject(&dataObject);
	}
	else if (hgText != NULL)
	{	// It's text but not sample data
		switch (m_MS->GetMode())
		{
		case MS_MODE_OFF:
		case MS_MODE_CELL:
		case MS_MODE_CELLS_IN_COL:
			// Use grid's default multicell paste capability
			Paste();
			break;

		case MS_MODE_ROW:
		case MS_MODE_COL:
		case MS_MODE_CELLS_IN_ROW:
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

bool CugWkl::SingleCellSelected()
{
	int col;
	long row;
	int rv;


	int mode = m_MS->GetMode();
	if (mode != MS_MODE_OFF && 
		mode != MS_MODE_CELL &&
		mode != MS_MODE_CELLS_IN_COL)
	{
		return false;
	}

	rv = m_MS->EnumFirstSelected(&col, &row);
	if (rv != UG_SUCCESS)
	{	// Couldn't find the first selected cell
		return false;
	}

	rv = m_MS->EnumNextSelected(&col, &row);
	if (rv == UG_SUCCESS)
	{	// Found a second selected cell
		return false;
	}

	return true;
}

bool CugWkl::SingleRowSelected()
{
	int col;
	long row, firstRow;
	int rv;

	// Check that selection mode is rows
	if (m_MS->GetMode() != MS_MODE_ROW)
	{
		return false;
	}

	rv = m_MS->EnumFirstSelected(&col, &firstRow);
	if (rv != UG_SUCCESS || col != 0)
	{	// Couldn't find the first row 
		return false;
	}

	do
	{
		rv = m_MS->EnumNextSelected(&col, &row);
		if (row != firstRow && rv == UG_SUCCESS)
		{	// More than one row is selected
			return false;
		}

	} 
	while (rv == UG_SUCCESS);

	// Doesn't check for full row selected!
	return true;
}

bool CugWkl::PastableCellsSelected()
{
	int col;
	long row;
	int rv;
	CUGCell cell;

	rv = m_MS->EnumFirstSelected(&col, &row);
	if (rv != UG_SUCCESS)
	{	// Couldn't find the first selected cell
		return false;
	}

	do
	{	// Check if column is FreeForm
		if (!m_pBatch->GetFldProp()->IsColFreeForm(col))
		{	// Selected cell not free form
			return false;
		}

		// Check if cell is writable
		if (GetCell(col, row, &cell) != UG_SUCCESS)
		{	// Couldn't get cell
			return false;
		}
		if (cell.GetReadOnly())
		{	// Not writable
			return false;
		}

		// Check for more cells
		rv = m_MS->EnumNextSelected(&col, &row);
	} 
	while (rv == UG_SUCCESS);

	return true;
}


////////////////////////////////////////////////////////////////////////
// OLE Drag and Drop Support
////////////////////////////////////////////////////////////////////////


/***************************************************
****************************************************/
DROPEFFECT  CugWkl::OnDragEnter(COleDataObject* pDataObject)
{
	if (m_pBatch == NULL)
	{
		return DROPEFFECT_NONE;
	}

	VERIFY(Activate());

	// If sample data is available and batch status is new
	// and workload has a name
	if (pDataObject->IsDataAvailable(CLucSamp::CF_SAMPLE) &&
		m_pBatch->GetStatus() == LUC_BSTAT_NEW)
	{
		return DROPEFFECT_COPY;
	}
	else 
	{
		return DROPEFFECT_NONE;
	}
}

/***************************************************
****************************************************/
DROPEFFECT  CugWkl::OnDragOver(COleDataObject* pDataObject,int col,long row)
{
	return OnDragEnter(pDataObject);
}

/***************************************************
****************************************************/
DROPEFFECT  CugWkl::OnDragDrop(COleDataObject* pDataObject,int col,long row)
{
	// It has been dropped
	if (OnDragEnter(pDataObject) == DROPEFFECT_COPY)
	{	// Good to drop if 
		if(CheckWklSaved())
		{
			AppendSampleFromDataObject(pDataObject);
		}
	}

	// This prevents default CUGDropTarget from trying to handle drop	
	return DROPEFFECT_NONE;
}


int CugWkl::StartDragDrop()
{

	if (!GetWklFileSpec().IsEmpty())
	{	// Workload has a filename so a reference can be made
		m_DragStartFlag = TRUE;	//set the DragStart Flag
		
		// Load sample data into OLE Data Source
		CopySampleToDataSource(&m_DataSource, LUC_SAMP_UNDEF, 
				RETEST_COPY);

		// Start drag ooperation	
		m_DataSource.DoDragDrop(DROPEFFECT_COPY);
		// Finish drag
		m_DataSource.Empty();
	}

	return UG_SUCCESS;
}


int CugWkl::DragDropTarget(BOOL state)
{
	int rv; 

	// Call base class implementation
	rv = CUGCtrl::DragDropTarget(state);
	if (rv == UG_SUCCESS)
	{	// Also make side header a drop target
		if(state == FALSE)
		{
			m_SHDropTarget.Revoke();
		}
		else
		{
			m_SHDropTarget.Register(&m_CUGSideHdg);
		}
	}

	return rv;
}


bool CugWkl::CopySampleToDataSource(COleDataSource* pDataSource,long iSamp, ECopyType copyType) 
{
	// Get sample pointer
	if (iSamp == LUC_SAMP_UNDEF)
	{
		iSamp = GetCurrentRow();
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


bool CugWkl::AppendSampleFromDataObject(COleDataObject* pDataObject)
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
	if (copyType == RETEST_COPY || copyType == RETEST_PARTS)
	{
		if (CheckWklSaved())
		{
			VERIFY(GetSampRef(&selfRef)); 
		}
		else
		{
			return false;
		}
	}

	// Sample does the rest
	bool rv = pSamp->LoadSampleFromDataObject(pDataObject, selfRef);

	// Redraw grid
	RedrawAll();

	return rv; 
}


CString	CugWkl::GetWklFileSpec()
{
	// Get document pointer
	CWklDoc* pWklDoc = GetWklDoc();
	ASSERT(pWklDoc != NULL);

	// Get the workload file name
	return pWklDoc->GetPathName();
}

bool CugWkl::VirtualCopyCheck()
{
	// Sample status must be SSTAT_READ or LUC_SSTAT_DONE
	int col, rv;
	long iSamp;

	// Get sample number
	rv = m_MS->EnumFirstSelected(&col, &iSamp);
	if (rv != UG_SUCCESS)
	{	// Couldn't find the selected row 
		return false;
	}

	// Get sample status
	ASSERT(m_pBatch != NULL);
	CLucSamp* pSamp = m_pBatch->GetSamplePtr(iSamp);
	ASSERT(pSamp != NULL);
	long iStat = pSamp->GetStatus();
	ASSERT(iStat != -1);

	if (iStat != LUC_SSTAT_READ && iStat != LUC_SSTAT_DONE)
	{
		CString errMsg;
		VERIFY(errMsg.LoadString(IDS_BAD_VIRTUAL_SAMP));
		AfxMessageBox(errMsg);
		return false;
	}
	else
	{
		return true;
	}
}


bool CugWkl::CheckWklSaved()
{
	// Determine if workload has ever been saved to disk
	if (!GetWklFileSpec().IsEmpty())
	{
		return true;
	}

	// This workload is not yet saved
	if (AfxMessageBox(IDS_MUST_SAVE_WKL, MB_OKCANCEL) == IDOK)
	{	// Get document pointer
		CWklDoc* pWklDoc = GetWklDoc();
		ASSERT(pWklDoc != NULL);

		// Initiate file save
		AfxGetMainWnd()->
			SendMessage(WM_COMMAND, (WPARAM)ID_FILE_SAVE_AS);

		// Check if the path is now available
		if (!GetWklFileSpec().IsEmpty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CugWkl::GetSampRef(CSampRef* pSampRef, int iSamp)
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

CWklDoc* CugWkl::GetWklDoc()
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

int CugWkl::GetBatchIndex()
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

bool CugWkl::Activate()
{
	CWklDoc* pDoc = GetWklDoc();
	if (pDoc == NULL)
	{
		return false;
	}

	// Look for CWklView views
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CWklView)))
		{
			((CMDIFrameWndEx*)AfxGetMainWnd())->MDIActivate((CMDIChildWndEx*)pView->GetParent());
			return true;
		}
	}

	return false;
}

CWklView* CugWkl::GetWklView()
{
	if (GetSafeHwnd() != NULL)
	{
		CBatchGridPage* pParent = (CBatchGridPage*)GetParent();
		if (pParent != NULL)
		{
			ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CBatchGridPage)));
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
