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

// BPickDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "BPickDlg.h"
#include "Batch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatPickDlg dialog


CBatPickDlg::CBatPickDlg(int nBatches)
	: CDialog(CBatPickDlg::IDD, NULL)
{ 
	m_nBatches = nBatches;
	m_iBatch = 0;

	//{{AFX_DATA_INIT(CBatPickDlg)
	m_batch = CBatch::GetBatchLetter(m_iBatch);
	//}}AFX_DATA_INIT
}


void CBatPickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatPickDlg)
	DDX_Control(pDX, IDC_SPINBAT, m_spinner);
	DDX_Text(pDX, IDC_EDITBAT, m_batch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatPickDlg, CDialog)
	//{{AFX_MSG_MAP(CBatPickDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINBAT, OnDeltaposSpinbat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatPickDlg message handlers

void CBatPickDlg::OnDeltaposSpinbat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// Down to go through the alphabet
	m_iBatch = m_iBatch - pNMUpDown->iDelta;

	// Range check
	m_iBatch = max(0, m_iBatch);
	m_iBatch = min(m_nBatches - 1, m_iBatch);

	pNMUpDown->iPos = m_iBatch;

	// Update Text
	m_spinner.GetBuddy()->SetWindowText(CBatch::GetBatchLetter(m_iBatch));

	*pResult = 0;
}

BOOL CBatPickDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_nBatches < 1)
	{
		AfxMessageBox(IDS_NO_BATCHES);
		PostMessage(WM_COMMAND, BN_CLICKED << 16 | IDCANCEL,(LPARAM)GetDlgItem(IDCANCEL)->GetSafeHwnd());
	}

	m_spinner.SetRange(0, m_nBatches - 1);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

