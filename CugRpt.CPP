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
Skeleton Class for a Derived CugRpt
****************************************************
****************************************************/

#include "stdafx.h"
#include "CugRpt.h"
#include "ultgrid/UGDao.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
****************************************************/
CugRpt::CugRpt()
{
}
/***************************************************
****************************************************/
CugRpt::~CugRpt()
{
}

/***************************************************
OnSetup
	This function is called just after the grid window 
	is created in CUGCtrl::OnCreate. 
****************************************************/
void CugRpt::OnSetup()
{
	// Create standard and bold fonts (10 point)
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
int CugRpt::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
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
int CugRpt::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	return TRUE;
}
/***************************************************
****************************************************/
void CugRpt::OnHitBottom(long numrows,long rowspast,long rowsfound)
{	// Dao override to ensure all records are read
	if (rowsfound > 0)
	{
		SetNumberRows(numrows + rowsfound);
	}
}
/***************************************************
****************************************************/
void CugRpt::OnHitTop(long numrows,long rowspast)
{

}
/***************************************************
OnCanSizeCol
	Sent when the user is over a separation line on
	the top heading
	A return value of TRUE allows the possibiliy of
	a resize
****************************************************/
int CugRpt::OnCanSizeCol(int col)
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
void CugRpt::OnColSizing(int col,int *width)
{
}
/***************************************************
OnColSized
	This is sent when the user finished sizing the 
	given column (see above for more details)
****************************************************/
void CugRpt::OnColSized(int col,int *width)
{
}
/***************************************************
OnCanSizeRow
	Sent when the user is over a separation line on
	the side heading
	A return value of TRUE allows the possibiliy of
	a resize
****************************************************/
int  CugRpt::OnCanSizeRow(long row)
{
	return TRUE;
}
/***************************************************
OnRowSizing
    Sent when the user is sizing a row
	The row that is being sized is given as 
	well as the height. Plus the height can be modified 
	at this point. This makes it easy to set min and
	max heights
****************************************************/
void CugRpt::OnRowSizing(long row,int *height)
{
}
/***************************************************
OnRowSized
	This is sent when the user is finished sizing hte
	given row ( see above for more details)
****************************************************/
void CugRpt::OnRowSized(long row,int *height)
{
}
/***************************************************
OnCanSizeSideHdg
	This is sent when the user moves into position 
	for sizing the width of the side heading
	return TRUE to allow the sizing
	or FALSE to not allow it
****************************************************/
int CugRpt::OnCanSizeSideHdg()
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
int CugRpt::OnCanSizeTopHdg()
{
	return TRUE;
}
/***************************************************
OnSideHdgSizing
****************************************************/
int CugRpt::OnSideHdgSizing(int *width)
{
	return TRUE;

}
/***************************************************
OnTopHdgSizing
****************************************************/
int CugRpt::OnTopHdgSizing(int *height)
{
	return TRUE;

}
/***************************************************
OnColChange
	Sent whenever the current column changes
	The old and the new columns are given
****************************************************/
void CugRpt::OnColChange(int oldcol,int newcol)
{
}
/***************************************************
OnRowChange
	Sent whenever the current row changes
	The old and the new rows are given
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
void CugRpt::OnRowChange(long oldrow,long newrow)
{
}
/***************************************************
OnCellChange
	Sent whenever the current cell changes rows or
	columns
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
void CugRpt::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
}
/***************************************************
OnLeftColChange
	Sent whenever the left visible column in the grid 
	changes
****************************************************/
void CugRpt::OnLeftColChange(int oldcol,int newcol)
{
}
/***************************************************
OnTopRowChange
	Sent whenever the top visible row in the grid changes
****************************************************/
// !Modification! 06-Jun-97 PM - To fix bug in source
void CugRpt::OnTopRowChange(long oldrow, long newrow)
{
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
void CugRpt::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
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
void CugRpt::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int procssed)
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
void CugRpt::OnDClicked(int col,long row,RECT *rect,POINT *point,int processed)
{
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
void CugRpt::OnTH_LClicked(int col,int updn)
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
void CugRpt::OnTH_RClicked(int col,int updn)
{
}
/***************************************************
OnTH_LClicked
	Sent whenever the user double clicks the left mouse
	button within the top heading

	'col' is negitive if the area clicked in is not valid
****************************************************/
void CugRpt::OnTH_DClicked(int col)
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
void CugRpt::OnSH_LClicked(long row,int updn)
{
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
void CugRpt::OnSH_RClicked(long row,int updn)
{
}
/***************************************************
OnSH_DClicked
	Sent whenever the user double clicks the left mouse
	button within the side heading

	'row' is negitive if the area clicked in is not valid
****************************************************/
void CugRpt::OnSH_DClicked(long row)
{
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
void CugRpt::OnCB_LClicked(int updn)
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
void CugRpt::OnCB_RClicked(int updn)
{
}
/***************************************************
OnCB_DClicked
	Sent whenever the user double clicks the left mouse
	button within the top corner button
****************************************************/
void CugRpt::OnCB_DClicked()
{
}
/***************************************************
OnKeyDown
	Sent whenever the user types when the grid has
	focus. The keystroke can be modified here as well.
	(See WM_KEYDOWN for more information)
****************************************************/
void CugRpt::OnKeyDown(UINT *vcKey,int processed)
{
}
/***************************************************
OnCharDown
	Sent whenever the user types when the grid has
	focus. The keystroke can be modified here as well.
	(See WM_CHAR for more information)
****************************************************/
void CugRpt::OnCharDown(UINT *vcKey,int processed)
{
}
	
/***************************************************
OnGetCell
	This message is sent everytime the grid needs to
	draw a cell in the grid. At this point the cell
	class has been filled with the information to be
	used to draw the cell. The information can now be
	changed before it is used for drawing
****************************************************/
void CugRpt::OnGetCell(int col,long row,CUGCell *cell)
{
}
/***************************************************
OnSetCell
	This message is sent everytime the a cell is about 
	to change.
****************************************************/
void CugRpt::OnSetCell(int col,long row,CUGCell *cell)
{
}
/***************************************************
OnDataSourceNotify
	This message is sent from a data source , message
	depends on the data source - check the information
	on the data source(s) being used
	- The ID of the Data source is also returned
****************************************************/
void CugRpt::OnDataSourceNotify(int ID,long msg,long param)
{
}
/***************************************************
ReadDataSource
	This routine loads a memory sourced batch grid with
	text from a DAO data source.
****************************************************/
void CugRpt::ReadDataSource(LPCTSTR fileName, LPCTSTR tableName)
{
	// Load working grid from data sourced grid 
	CUG_DAO m_dao;
	CugRpt	ds_grid;
	int		ds_index;
	RECT	ds_gridSize = {0,0,0,0};
	CUGCell cell;
	int		col;
	long	row;
	CString text;

	// Open data access object to Table1 in data base file db1
	ds_grid.CreateGrid(WS_CHILD|WS_VISIBLE, ds_gridSize, this, 1234);
	if (m_dao.Open(fileName, tableName) == UG_SUCCESS)
	{	// Found data source
		ds_index = ds_grid.AddDataSource(&m_dao);
		ds_grid.SetDefDataSource(ds_index);
		ds_grid.SetGridUsingDataSource(ds_index);
	}

	// Set grid dimensions
	SetNumberCols(ds_grid.GetNumberCols());
	SetNumberRows(ds_grid.GetNumberRows());

	// Set column headers
	int num = GetNumberCols();
	for (col = 0; col < num; col++)
	{
		ds_grid.GetCell(col, -1, &cell);
		SetCell(col, -1, &cell);
	}

	// Hide side headings
	SetSH_Width(0);

	// Set default data type to Text
	GetGridDefault(&cell);
	cell.SetDataType(UG_TEXTDATA);
	SetGridDefault(&cell);

	// Load cells with data from data source grid
	num = GetNumberCols();
	int numr = GetNumberRows();
	for (col = 0; col < num; col++)
	{
		for (row = 0; row < numr; row++)
		{
			ds_grid.GetCell(col, row, &cell);
			cell.GetText(&text);
			QuickSetText(col, row, text);
		}
	}

	EnableMenu(1);
}

/***************************************************
OnCellTypeNotify
	This message is sent from a cell type , message
	depends on the cell type - check the information
	on the cell type classes
	- The ID of the cell type is given
****************************************************/
void CugRpt::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
}
/***************************************************
OnEditStart
	This message is sent whenever the grid is ready 
	to start editing a cell
	A return of TRUE allows the editing a return of
	FALSE stops editing
	Plus the properties of the CEdit class can be modified
****************************************************/
int CugRpt::OnEditStart(int col, long row,CEdit *edit)
{
	return TRUE;
}
/***************************************************
OnEditVerify
	This is send when the editing is about to end
****************************************************/
int CugRpt::OnEditVerify(int col, long row,CEdit *edit,UINT *vcKey)
{
	return TRUE;
}
/***************************************************
OnEditFinish this is send when editing is finished
****************************************************/
int CugRpt::OnEditFinish(int col, long row,CEdit *edit)
{
	return TRUE;
}
/***************************************************
OnEditFinish this is send when editing is finished
****************************************************/
int CugRpt::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	return TRUE;
}
/***************************************************
sections - UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
			UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
****************************************************/
void CugRpt::OnMenuCommand(int col,long row,int section,int item)
{

	CUGCell cell;

	if (section == UG_TOPHEADING)
	{
		GetCell(col,row,&cell);
		switch(item)
		{
		case 1010:
			break;

		case 1011:
		break;
		}	
	}
}
/***************************************************
return UG_SUCCESS to allow the menu to appear
return 1 to not allow the menu to appear
****************************************************/
int CugRpt::OnMenuStart(int col,long row,int section)
{
	EmptyMenu();
	CUGCell cell;
	CString string;
	GetCell(col,row,&cell);
	cell.GetText(&string);

	if(section == UG_TOPHEADING)
	{
			AddMenuItem(1010,_T("Hide Column"));
			AddMenuItem(1011,_T("Sort by Column"));
			return 1;
	}

	return FALSE;
}
/***************************************************
****************************************************/
int CugRpt::OnHint(int col,long row,CUGHint *hint)
{
	return TRUE;
}
/***************************************************
****************************************************/
int CugRpt::OnVScrollHint(long row,int percent,CUGHint *hint)
{
	return TRUE;
}
/***************************************************
****************************************************/
int CugRpt::OnHScrollHint(int col,int percent,CUGHint *hint)
{
	return TRUE;
}

/*********************************************
OLE
**********************************************/
#ifdef __AFXOLE_H__

/***************************************************
****************************************************/
DROPEFFECT  CugRpt::OnDragEnter(COleDataObject* pDataObject)
{
	return DROPEFFECT_NONE;
}
/***************************************************
****************************************************/
DROPEFFECT  CugRpt::OnDragOver(COleDataObject* pDataObject,int col,long row)
{
	return DROPEFFECT_NONE;
}
/***************************************************
****************************************************/
DROPEFFECT  CugRpt::OnDragDrop(COleDataObject* pDataObject,int col,long row)
{
	return DROPEFFECT_NONE;
}
#endif


