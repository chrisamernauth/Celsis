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
#include "AssyDef.h"
#include "GnDfPrDc.h"

#include "ACCMgr.h"
#include "sampref.h"
#include "userflag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CGenDefPrDoc implementations
//
//
IMPLEMENT_DYNCREATE(CGenDefPrDoc, CDocument)

BEGIN_MESSAGE_MAP(CGenDefPrDoc, CDocument)
	//{{AFX_MSG_MAP(CGenDefPrDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGenDefPrDoc::CGenDefPrDoc()
{
	m_indent1  = (_T("    ")); //4 spaces
	m_indent2  = m_indent1 + m_indent1;
	
	m_strTrue  = (_T("True"));  
	m_strFalse = (_T("False"));
	m_strChkd  = (_T("True"));  //box is checked
	m_strnChkd = (_T("False"));//box is not checked
}

BOOL CGenDefPrDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}
	
	return TRUE;
}

#ifdef _DEBUG
void CGenDefPrDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGenDefPrDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGenDefPrDoc Serialization
//   CAssayPrDoc exists only to facilitate the viewing and printing of an assay
//   definition.  It should never be serialized.
//
void CGenDefPrDoc::Serialize(CArchive& ar)
{
	return;
}

void CGenDefPrDoc::OnCloseDocument() 
{
	CDocument::OnCloseDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CGenDefPrDoc commands
void CGenDefPrDoc::AddLine(CPrintLine *pln) 
{
	//update max chars
	if((pln->lbl.GetLength() + pln->txt.GetLength() +1) > m_maxchars)
	{
		m_maxchars = (pln->lbl.GetLength() + pln->txt.GetLength() + 1);
	}

	//add line to list
	m_lines.AddTail(*pln);
}
