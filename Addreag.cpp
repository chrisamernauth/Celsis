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

// AddReagents.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "AddReag.h"
#include "LucyDef.h"
#include "LucyDoc.h"
#include "LucyView.h"
#include "LucyBar.h"
#include "Specification.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLuceptApp theApp;


#define REAGENT_LEVEL_SPIN_MAX  MAX_REAGENT_VOL/100
#define REAGENT_LEVEL_SPIN_MIN -MAX_REAGENT_VOL/100

/////////////////////////////////////////////////////////////////////////////
// CAddReagents dialog


CAddReagents::CAddReagents(CWnd* pParent /*=NULL*/)
	: CDialog(CAddReagents::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddReagents)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddReagents::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddReagents)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddReagents, CDialog)
	//{{AFX_MSG_MAP(CAddReagents)
	ON_BN_CLICKED(IDC_ADD_REAGENT_1, OnAddReagent1)
	ON_BN_CLICKED(IDC_ADD_REAGENT_2, OnAddReagent2)
	ON_BN_CLICKED(IDC_ADD_REAGENT_3, OnAddReagent3)
	ON_BN_CLICKED(IDC_ADD_REAGENT_P, OnAddReagentP)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ADDRGNT_1_SPIN, OnDeltaposAddrgnt1Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ADDRGNT_2_SPIN, OnDeltaposAddrgnt2Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ADDRGNT_3_SPIN, OnDeltaposAddrgnt3Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ADDRGNT_P_SPIN, OnDeltaposAddrgntPSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddReagents message handlers

/////////////////////////////////////////////////////////////////////////////
//
//
BOOL CAddReagents::OnInitDialog() 
{
	CDialog::OnInitDialog();

	pVol1 = (CEdit*) GetDlgItem(IDC_VOLUME_1);
	pVol2 = (CEdit*) GetDlgItem(IDC_VOLUME_2);
	pVol3 = (CEdit*) GetDlgItem(IDC_VOLUME_3);
	pVolP = (CEdit*) GetDlgItem(IDC_VOLUME_P);

	pAmt1 = (CEdit*) GetDlgItem(IDC_AMOUNT_1);
	pAmt2 = (CEdit*) GetDlgItem(IDC_AMOUNT_2);
	pAmt3 = (CEdit*) GetDlgItem(IDC_AMOUNT_3);
	pAmtP = (CEdit*) GetDlgItem(IDC_AMOUNT_P);

	pAddBtn1 = (CButton*) GetDlgItem(IDC_ADD_REAGENT_1);
	pAddBtn2 = (CButton*) GetDlgItem(IDC_ADD_REAGENT_2);
	pAddBtn3 = (CButton*) GetDlgItem(IDC_ADD_REAGENT_3);
	pAddBtnP = (CButton*) GetDlgItem(IDC_ADD_REAGENT_P);

	pSpin1 = (CSpinButtonCtrl*) GetDlgItem(IDC_ADDRGNT_1_SPIN);
	pSpin2 = (CSpinButtonCtrl*) GetDlgItem(IDC_ADDRGNT_2_SPIN);
	pSpin3 = (CSpinButtonCtrl*) GetDlgItem(IDC_ADDRGNT_3_SPIN);	
	pSpinP = (CSpinButtonCtrl*) GetDlgItem(IDC_ADDRGNT_P_SPIN);	

	if( Specification::g_oSpecification.Has4thInjector() == false) {
		/*
		 * if there is no fourth injector then hide associated controls and shift other
		 * controls accordingly.
		 */
		CRect oRectP;
		CRect oRect1;
		CRect oRect2;
		CRect oRect3;

		pVolP->GetWindowRect( &oRectP);
		pVol1->GetWindowRect( &oRect1);
		pVol2->GetWindowRect( &oRect2);
		pVol3->GetWindowRect( &oRect3);

		long nShiftReag1 = oRect1.top - oRectP.top;
		long nShiftReag2 = oRect2.top - (oRect3.top + oRectP.top) / 2;

		Specification::MoveWindowUp( GetDlgItem( IDC_LABEL_1), nShiftReag1);
		Specification::MoveWindowUp( pVol1, nShiftReag1);
		Specification::MoveWindowUp( pAmt1, nShiftReag1);
		Specification::MoveWindowUp( pAddBtn1, nShiftReag1);
		Specification::MoveWindowUp( pSpin1, nShiftReag1);

		Specification::MoveWindowUp( GetDlgItem( IDC_LABEL_2), nShiftReag2);
		Specification::MoveWindowUp( pVol2, nShiftReag2);
		Specification::MoveWindowUp( pAmt2, nShiftReag2);
		Specification::MoveWindowUp( pAddBtn2, nShiftReag2);
		Specification::MoveWindowUp( pSpin2, nShiftReag2);

		GetDlgItem( IDC_LABEL_P)->ShowWindow( SW_HIDE);
		pVolP->ShowWindow( SW_HIDE);
		pAmtP->ShowWindow( SW_HIDE);
		pAddBtnP->ShowWindow( SW_HIDE);
		pSpinP->ShowWindow( SW_HIDE);
	}

	ReagentLevelsGet();
	pAmt1->SetWindowText(_T("0.0"));
	pAmt2->SetWindowText(_T("0.0"));
	pAmt3->SetWindowText(_T("0.0"));
	pAmtP->SetWindowText(_T("0.0"));
	SpinControlsInit();

	return TRUE;  
}


void CAddReagents::OnAddReagent1() 
{
	OnAddReagent( pVol1, pAmt1, 1);
}

void CAddReagents::OnAddReagent2() 
{
	OnAddReagent( pVol2, pAmt2, 2);
}

void CAddReagents::OnAddReagent3() 
{
	OnAddReagent( pVol3, pAmt3, 3);
}

void CAddReagents::OnAddReagentP() 
{
	OnAddReagent( pVolP, pAmtP, 4);
}

void CAddReagents::OnAddReagent( CEdit *pVol, CEdit *pAmt, int nReagent) 
{
	long tempCurrent	= ReagentInputBoxFltTxtToInt(pVol);
	long tempIncrement	= ReagentInputBoxFltTxtToInt(pAmt);

	if (tempCurrent + tempIncrement > REAGENT_LEVEL_SPIN_MAX)
	{
		CString fmt, msg;

		fmt.LoadString(IDS_RGT_MAX_WARNING_FMT);
		msg.Format(fmt, (float) REAGENT_LEVEL_SPIN_MAX/10);
		if(AfxMessageBox(msg, MB_OKCANCEL) == IDOK)
		{
			theApp.m_pRgntDlg->SetReagent( nReagent, REAGENT_LEVEL_SPIN_MAX * 100);
		}
	} 
	else if (tempCurrent + tempIncrement < 0) 
	{
		CString fmt, msg;
		fmt.LoadString(IDS_RGT_MIN_WARNING_FMT);
		msg.Format(fmt, (float) 0);
		if(AfxMessageBox(msg, MB_OKCANCEL) == IDOK)
		{
			theApp.m_pRgntDlg->SetReagent( nReagent, 0);
		}
	} 
	else 
	{
		tempCurrent += tempIncrement;
		theApp.m_pRgntDlg->SetReagent( nReagent, tempCurrent * 100); //convert float 0.0 ml to mml
	}

	//refresh boxes
	pAmt->SetWindowText(_T("0.0"));
	SpinControlsInit();
	ReagentLevelsGet();
}

void CAddReagents::OnDeltaposAddrgnt1Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposAddrgntSpin( pNMHDR, pResult, pAmt1);
}

void CAddReagents::OnDeltaposAddrgnt2Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposAddrgntSpin( pNMHDR, pResult, pAmt2);
}

void CAddReagents::OnDeltaposAddrgnt3Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposAddrgntSpin( pNMHDR, pResult, pAmt3);
}

void CAddReagents::OnDeltaposAddrgntPSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDeltaposAddrgntSpin( pNMHDR, pResult, pAmtP);
}

void CAddReagents::OnDeltaposAddrgntSpin(NMHDR* pNMHDR, LRESULT* pResult, CEdit *pAmt) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//synch spin control iPos with text control current value
	//in case text has been manually entered
	pNMUpDown->iPos = ReagentInputBoxFltTxtToInt(pAmt);

	//increment number	
	if ( pNMUpDown->iPos + pNMUpDown->iDelta > REAGENT_LEVEL_SPIN_MAX) 
	{
		pNMUpDown->iPos = REAGENT_LEVEL_SPIN_MAX;
	} 
	else if (pNMUpDown->iPos + pNMUpDown->iDelta < REAGENT_LEVEL_SPIN_MIN) 
	{
		pNMUpDown->iPos = REAGENT_LEVEL_SPIN_MIN;
	} 
	else 
	{
		pNMUpDown->iPos += pNMUpDown->iDelta;
	}

	//write text to control
	pAmt->SetWindowText(ReagentInputBoxIntToFltTxt(pNMUpDown->iPos));
	
	*pResult = 0;
}

void CAddReagents::ReagentLevelsGet()
{
	CString tempstr;
	long temp;

	//first reagent level
	theApp.m_pRgntDlg->GetReagentLevel(1, &temp); 
	tempstr.Format(_T("%6.1f"), float(temp)/1000.0);		//convert mml to ml
	pVol1->SetWindowText(tempstr);
	
	//second reagent level
	theApp.m_pRgntDlg->GetReagentLevel(2, &temp); 
	tempstr.Format(_T("%6.1f"), float(temp)/1000.0);		//convert mml to ml
	pVol2->SetWindowText(tempstr);

	//third reagent level
	theApp.m_pRgntDlg->GetReagentLevel(3, &temp); 
	tempstr.Format(_T("%6.1f"), float(temp)/1000.0);		//convert mml to ml
	pVol3->SetWindowText(tempstr);

	//third reagent level
	theApp.m_pRgntDlg->GetReagentLevel(4, &temp); 
	tempstr.Format(_T("%6.1f"), float(temp)/1000.0);		//convert mml to ml
	pVolP->SetWindowText(tempstr);

}

void CAddReagents::SpinControlsInit()
{
	//!!! all this might go in OnInit
	UDACCEL accelarray[3];
	accelarray[0].nSec	= 1;
	accelarray[0].nInc	= 1;
	accelarray[1].nSec	= 2;
	accelarray[1].nInc	= 10;
	accelarray[2].nSec	= 3;
	accelarray[2].nInc	= 50;

	pSpin1->SetAccel(3, accelarray);
	pSpin2->SetAccel(3, accelarray);
	pSpin3->SetAccel(3, accelarray);
	pSpinP->SetAccel(3, accelarray);
	
	pSpin1->SetRange( REAGENT_LEVEL_SPIN_MIN,  REAGENT_LEVEL_SPIN_MAX);
	pSpin2->SetRange( REAGENT_LEVEL_SPIN_MIN,  REAGENT_LEVEL_SPIN_MAX);
	pSpin3->SetRange( REAGENT_LEVEL_SPIN_MIN,  REAGENT_LEVEL_SPIN_MAX);
	pSpinP->SetRange( REAGENT_LEVEL_SPIN_MIN,  REAGENT_LEVEL_SPIN_MAX);

	pSpin1->SetBuddy(pAmt1);
	pSpin2->SetBuddy(pAmt2);
	pSpin3->SetBuddy(pAmt3);
	pSpinP->SetBuddy(pAmtP);

	pSpin1->SetPos(0);
	pSpin2->SetPos(0);
	pSpin3->SetPos(0);
	pSpinP->SetPos(0);
}

int CAddReagents::ReagentInputBoxFltTxtToInt(CEdit * pBox)
{
	double f;
	CString strTemp;

	pBox->GetWindowText(strTemp);
	f = _tstof((LPCTSTR)strTemp);
	f *= 10;

	return (int)f;
}

CString CAddReagents::ReagentInputBoxIntToFltTxt(const int & n)
{
	CString text;
	float fTemp;

	//format text as single decimal place float
	fTemp = (float)n;
	fTemp /= 10;
	text.Format(_T("%.1f"), fTemp);

	return text;
}

