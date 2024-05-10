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
ITL				03-Apr-02			Amended by CAR 887/003
************************************************************************************/

// ADPrpSh.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "ADPrpSh.h"
#include "Event.h"
#include "SystemLog.h"
#include "UsrAdmin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADPropSheet

IMPLEMENT_DYNAMIC(CADPropSheet, CPropertySheet)

CADPropSheet::CADPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_pACalc = NULL;

	AddPages();
	InitAssayDef();
}

CADPropSheet::CADPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pACalc = NULL;

	AddPages();
	InitAssayDef();
}

CADPropSheet::~CADPropSheet()
{	// Release Assay Calc Interface Manager
	if (m_pACalc != NULL)
	{
		delete m_pACalc;
	}
}


BEGIN_MESSAGE_MAP(CADPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CADPropSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADPropSheet message handlers

bool CADPropSheet::AddPages()
{
	AddPage(&m_pgMain);
	AddPage(&m_pgACalc);
	AddPage(&m_pgCalib);
	AddPage(&m_pgFld);		// ! EGG
	AddPage(&m_pgSampNames);
	AddPage(&m_pgFlags);

	return true;
}

long CADPropSheet::InitAssayDef(CAssayDef* pAssayDef)
{
	long rv = KO_OK;

	if (pAssayDef != NULL)
	{
		m_pAD = pAssayDef;
	}
	else
	{
		m_pAD = &m_defaultAssayDef;
	}
	m_pgMain.SetAssayDef(m_pAD);
	m_pgACalc.SetAssayDef(m_pAD);
	m_pgCalib.SetAssayDef(m_pAD);
	m_pgFld.SetAssayDef(m_pAD);		// ! EGG
	m_pgSampNames.SetAssayDef(m_pAD);
	m_pgFlags.SetAssayDef(m_pAD);

	return KO_OK;
}

BOOL CADPropSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// Add assay name to dialog box title
	CString dlgTitle;

	GetWindowText(dlgTitle);
	ASSERT(m_pAD != NULL);
	dlgTitle += _T(" - ");
	dlgTitle += m_pAD->GetName();
	SetWindowText(dlgTitle);
	
	// Create the Assay Calculation Library Interface Manager!
	m_pACalc = new CACCMgr;
	ASSERT(m_pACalc != NULL);
	// !!! What about error opening control?
	// Can we detect one?

	return bResult;
}


BOOL CADPropSheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	// OK button sends wParam = 0x00000001 and lParam = hWnd of button
	// Cancel sends wParam = 0x00000002 and lParam = hWnd of button


	// Property Pages' OnOK() is not called till later
	// We'll do a premptive data check and save if necessary
	if (wParam == 0x00000001)
	{
		// OnOk Handler
		VERIFY(SetActivePage(&m_pgMain));
		if (m_pgMain.ChkAssayDef() == KO_OK)
		{
			m_pgMain.OnOK();
		}
		else
		{
			return true;
		}

		VERIFY(SetActivePage(&m_pgACalc));
		if (m_pgACalc.ChkAssayDef() == KO_OK)
		{
			m_pgACalc.OnOK();
		}
		else
		{
			return true;
		}

		VERIFY(SetActivePage(&m_pgCalib));
		if (m_pgCalib.ChkAssayDef() == KO_OK)
		{
			m_pgCalib.OnOK();
		}
		else
		{
			return true;
		}

		VERIFY(SetActivePage(&m_pgFld));
		if (m_pgFld.ChkAssayDef() == KO_OK)
		{
			m_pgFld.OnOK();
		}
		else
		{
			return true;
		}

		VERIFY(SetActivePage(&m_pgSampNames));
		if (m_pgSampNames.ChkAssayDef() == KO_OK)
		{
			m_pgSampNames.OnOK();
		}
		else
		{
			return true;
		}

		VERIFY(SetActivePage(&m_pgFlags));
		if (m_pgFlags.ChkAssayDef() == KO_OK)
		{
			m_pgFlags.OnOK();
		}
		else
		{
			return true;
		}

		// If we got this far, the assay def should be updated and
		// correct.  So save it.

		// Write assay def to disk
		m_pAD->WriteToDisk();

		// *** SYSTEM LOG **********************************************************
		CLogEvent	NewEvent;							       // Create a new event   *
		NewEvent.EventSet(_T("Assay Definition edited       "),// Event name           *
		m_pAD->GetFileName()+_T(" : ")+m_pAD->GetName());	   // Event details		   *
		CSystemLog::g_oSystemLog.EventAdd( NewEvent);
		// *** SYSTEM LOG **********************************************************

		// Obsolete any existing assays
		// Ignore error if none exist
		((CLuceptApp*)AfxGetApp())->m_assays.Obsolete(m_pAD->GetName());

		// Read from disk into assays object 
		((CLuceptApp*)AfxGetApp())->m_assays.InitFromDisk(m_pAD->GetID());
	}

	return CPropertySheet::OnCommand(wParam, lParam);
}
