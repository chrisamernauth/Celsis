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

////////////////////////////////////////////////////////////////////////////
//
// LogonDlg.cpp : implementation file 
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogonDlg.h"
#include "UsrAdmin.h"
#include "MainFrm.h"
#include "..\PhilUtil\PhilUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog


CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogonDlg)
	m_UserID = _T("");
	m_password = _T("");
	//}}AFX_DATA_INIT
}


void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogonDlg)
	DDX_Text(pDX, IDC_USER_ID, m_UserID);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	//{{AFX_MSG_MAP(CLogonDlg)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg message handlers
//
//	traps the ESC key
//
void CLogonDlg::OnCancel() 
{
}

void CLogonDlg::OnExitButton() 
{
	if(gGateKeeper.IsSafeToExit()) 
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->OnClose();	
	} 
	else 
	{
		AfxMessageBox(IDS_WKL_BUSY);
	}	
}
