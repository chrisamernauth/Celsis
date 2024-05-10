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

// CalTypeD.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "CalTypeD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalTypeDlg dialog


CCalTypeDlg::CCalTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalTypeDlg)
	m_bAddCals = TRUE;
	m_bDefaultCals = FALSE;
	//}}AFX_DATA_INIT
}


void CCalTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalTypeDlg)
	DDX_Check(pDX, IDC_ADD_CAL_SAMPS, m_bAddCals);
	DDX_Check(pDX, IDC_DEFAULT_CALS, m_bDefaultCals);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CCalTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
