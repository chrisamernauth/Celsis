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

// LccInq.cpp : implementation file
//

#include "stdafx.h"
#include "LccInq.h"
#include "LuceErr.h"
#include "LccMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Declare messages
UINT NEAR WM_AUTO_PRESS
	= RegisterWindowMessage(_T("LuceptInquireAutopress"));


/////////////////////////////////////////////////////////////////////////////
// CLccInqDlg dialog


CLccInqDlg::CLccInqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLccInqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLccInqDlg)
	m_csStat = _T("");
	//}}AFX_DATA_INIT
}


void CLccInqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLccInqDlg)
	DDX_Text(pDX, IDC_LUM_DOOR_LOCK, m_doorLock);
	DDX_Text(pDX, IDC_LUM_FW_REV, m_FWRev);
	DDX_Text(pDX, IDC_LUM_STAT, m_lumStat);
	DDX_Text(pDX, IDC_LCC_STAT, m_lccStat);
	DDX_Text(pDX, IDC_LUM_CHAIN_POS, m_chainPos);
	DDX_Text(pDX, IDC_LUM_TMPR, m_lumTmpr);
	DDX_Text(pDX, IDC_CS_STAT, m_csStat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLccInqDlg, CDialog)
	//{{AFX_MSG_MAP(CLccInqDlg)
	ON_BN_CLICKED(IDC_LCC_INQUIRE, OnLccInquire)
	ON_WM_CLOSE()
    ON_REGISTERED_MESSAGE(WM_AUTO_PRESS, OnAutopress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLccInqDlg message handlers

void CLccInqDlg::OnLccInquire() 
{
	long val;

	// Control session status
	m_csStat = _T("Inquiring...");
	UpdateData(false);
	val = GetLccMgr()->CSGetStatus();
	m_csStat.Format(_T("%d"), val);
	UpdateData(false);

	// Misc inquires
	GetInquire(LCC_INQ_LCCSTAT,  m_lccStat);
	GetInquire(LCC_INQ_LUMSTAT,  m_lumStat);
	GetInquire(LCC_INQ_FIRMREV,  m_FWRev);
	GetInquire(LCC_INQ_TEMP,	 m_lumTmpr);
	GetInquire(LCC_INQ_CHAINPOS, m_chainPos);
	GetInquire(LCC_INQ_DOORLOCK, m_doorLock);
}

BOOL CLccInqDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	VERIFY(RegisterWithLcc() == KO_OK);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLccInqDlg::OnOK() 
{
	UnregisterWithLcc();
	DestroyWindow();	
}

void CLccInqDlg::OnClose() 
{
	UnregisterWithLcc();
	DestroyWindow();	
}

void CLccInqDlg::GetInquire(long inqID, CString& result)
{
	long rv;
	double val;

	result = _T("Inquiring...");
	UpdateData(false);

	rv = GetLccMgr()->Inquire(inqID, &val);
	if (rv != KO_OK)
	{
		result.Format(_T("Error 0x%X"), rv);
	}
	else if (inqID == LCC_INQ_LUMSTAT)
	{
		result.Format(_T("0x%X"), (int)val);
	}
	else
	{
		result.Format(_T("%g"), val);
	}

	UpdateData(false);
}
/////////////////////////////////////////////////////////////////////////////
// Process notification of a periodic update update
/////////////////////////////////////////////////////////////////////////////
LRESULT CLccInqDlg::OnAutopress(WPARAM wParam, LPARAM lParam)
{
	OnLccInquire();
	return (LRESULT)TRUE;
}


void CLccInqDlg::TickHandler()
{
	static int period = 3;
	static int second = 0;

	// Every period seconds
	if (++second % period == 0)
	{
		PostMessage(WM_AUTO_PRESS);
	}
}
