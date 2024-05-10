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

// LccVDoc.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "LccVDoc.h"
#include "LccView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLccVDoc

IMPLEMENT_DYNCREATE(CLccVDoc, CDocument)

CLccVDoc::CLccVDoc()
{
}

BOOL CLccVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}
	return TRUE;
}

CLccVDoc::~CLccVDoc()
{
}


BEGIN_MESSAGE_MAP(CLccVDoc, CDocument)
	//{{AFX_MSG_MAP(CLccVDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLccVDoc diagnostics

#ifdef _DEBUG
void CLccVDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLccVDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLccVDoc serialization

void CLccVDoc::Serialize(CArchive& ar)
{

	((CLccView*)m_viewList.GetHead())->SerializeRaw(ar);
}


/////////////////////////////////////////////////////////////////////////////
// CLccVDoc commands

void CLccVDoc::OnCloseDocument() 
{
	CDocument::OnCloseDocument();

	// NULL document pointer
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();			
	pApp->m_pLccVDoc = NULL;
}
