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
ITL				19-Mar-02			Modified by CAR 887/002

************************************************************************************/

// AssayView.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"

#include "GnDfPrVw.h"
#include "AssyPrVw.h"
#include "AssyPrDc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAssayPrView, CGenDefPrView)

CAssayPrView::CAssayPrView() : CGenDefPrView()
{
}  

CAssayPrView::~CAssayPrView()
{
}

void CAssayPrView::OnInitialUpdate()
{
	CGenDefPrView::OnInitialUpdate();

	if(m_ForHistory)	// ITL: Added by CAR 887/002
	{	// ITL: set header string
		m_lpszHeader  = _T("Celsis Obsolete Assay Report");
	}
	else
	{	// ITL: set header string
		m_lpszHeader  = _T("Celsis Assay Report");
	}
}

