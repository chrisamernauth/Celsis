// CGXGraphCell.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "GXGraphCell.h"

#include "2dconst.h"

#include "DerivedDouble.h"
#include "Axis.h"
#include "AxisCollection.h"
#include "ChartArea.h"
#include "Chart2DData.h"
#include "ChartGroup.h"
#include "ChartGroupCollection.h"
#include "SymbolStyle.h"
#include "ChartStyle.h"
#include "ChartStyleCollection.h"
#include "Label.h"
#include "LabelCollection.h"
#include "Border.h"
#include "Interior.h"
#include "LineStyle.h"
#include "Legend.h"
#include "Font.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGXGraphCell

CGXGraphCell::CGXGraphCell(CGXGridCore* pGrid, CReportGridGraphCollection *pGraphs)
	: CGXStatic( pGrid),
		m_pGraphs( pGraphs)
{
}

CGXGraphCell::~CGXGraphCell()
{
}

#define CWnd CGXGraphCell
BEGIN_MESSAGE_MAP(CGXGraphCell, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CGXGraphCell)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGXGraphCell message handlers

void CGXGraphCell::Create( CWnd* pParentWnd)
{
}

/*
	Draw cell: get the graph pane to do the hard work.
*/
void CGXGraphCell::Draw( CDC* pDC, CRect rect, ROWCOL nRow, ROWCOL nCol, const CGXStyle& style, const CGXStyle *pStandardStyle)
{
//		ASSERTX( m_pGraph != NULL);

	ASSERT(pDC != NULL && pDC->IsKindOf(RUNTIME_CLASS(CDC)));
	// ASSERTION-> Invalid Device Context ->END
	ASSERT(nRow <= Grid()->GetRowCount() && nCol <= Grid()->GetColCount());

	// ASSERTION-> Cell coordinates out of range ->END

	ASSERT_VALID(pDC);

	//Print the chart using the DrawToDC function.
	//The chart is printed at the same size as it appears on the screen.

	//BLOCK: Set up scale mode
	// {
	// 	CClientDC dcScreen(NULL);

	// 	pDC->SetMapMode( MM_ANISOTROPIC);

	// 	// map 1 screen logical inch to 1 printer (/output) logical inch
	// 	pDC->SetWindowExt( dcScreen.GetDeviceCaps(LOGPIXELSX),
	// 			dcScreen.GetDeviceCaps(LOGPIXELSX));
	// 	pDC->SetViewportExt( pDC->GetDeviceCaps(LOGPIXELSX),
	// 			pDC->GetDeviceCaps(LOGPIXELSX));
	// }

//	DrawFrame(pDC, rect, style);

	ASSERT( m_pGraphs != NULL);

	long nGraph = style.GetUserAttribute( IDS_RESULT_GRAPH_INDEX).GetLongValue();

	ASSERT( nGraph >= 0 && nGraph < m_pGraphs->GetNoGraphs());

	CReportGridGraph *pGraph = m_pGraphs->GetGraph( nGraph);

	::LPtoDP( pDC->GetSafeHdc(), (POINT*)(&rect), 2);
	
	int nGraphWidth = rect.Height() * 3 / 2;

	int nGraphOffset = 0;

	if( nGraphWidth < rect.Width()) {
		nGraphOffset = (rect.Width() - nGraphWidth) / 2;
	}

	if( nGraphOffset > 0) {
		CBrush oWhiteBrush;
		CPen oNullPen;
		
		oWhiteBrush.CreateStockObject( WHITE_BRUSH);
//		oWhiteBrush.CreateSolidBrush( RGB( 212, 0, 0));
		oNullPen.CreateStockObject( NULL_BRUSH);

		CGdiObject *pOldBrush = pDC->SelectObject( &oWhiteBrush);
		CGdiObject *pOldPen = pDC->SelectObject( &oNullPen);

		pDC->FillRect( CRect( rect.left, rect.top, rect.left + nGraphOffset, rect.bottom), &oWhiteBrush);
		pDC->FillRect( CRect( rect.left + nGraphOffset + nGraphWidth, rect.top, rect.right, rect.bottom), &oWhiteBrush);

		pDC->SelectObject( pOldBrush);
		pDC->SelectObject( pOldPen);
	}

	/*
	 * Scale the graph such that the height is correct: otherwise it will probably be too tall to fit in the rect given.
	 */
	pGraph->DrawToDC( (long)pDC->GetSafeHdc(), 1, oc2dScaleToHeight, rect.left + nGraphOffset, rect.top, nGraphWidth, rect.Height());
}
