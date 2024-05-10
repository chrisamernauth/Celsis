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


// ACCtnr.cpp : implementation file
//

#include "stdafx.h"
#include "ACCtnr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACalcCtnr dialog


CACalcCtnr::CACalcCtnr(CWnd* pParent /*=NULL*/)
	: CDialog(CACalcCtnr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACalcCtnr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CACalcCtnr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACalcCtnr)
	DDX_Control(pDX, IDC_ACALC_CTRL, m_Acc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACalcCtnr, CDialog)
	//{{AFX_MSG_MAP(CACalcCtnr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACalcCtnr message handlers
