// dummy.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "dummy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cdummy

IMPLEMENT_DYNCREATE(Cdummy, CScrollView)

Cdummy::Cdummy()
{
}

Cdummy::~Cdummy()
{
}


BEGIN_MESSAGE_MAP(Cdummy, CScrollView)
	//{{AFX_MSG_MAP(Cdummy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cdummy drawing

void Cdummy::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void Cdummy::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// Cdummy diagnostics

#ifdef _DEBUG
void Cdummy::AssertValid() const
{
	CScrollView::AssertValid();
}

void Cdummy::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Cdummy message handlers
