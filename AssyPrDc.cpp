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

// AssayDoc.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "AssyDef.h"

#include "GnDfPrDc.h"
#include "AssyPrDc.h"
#include "ACCMgr.h"
#include "sampref.h"
#include "userflag.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssayPrDoc

IMPLEMENT_DYNCREATE(CAssayPrDoc, CGenDefPrDoc)

CAssayPrDoc::CAssayPrDoc()
{
	m_maxchars = 0;
}

CAssayPrDoc::CAssayPrDoc(CAssayDef& AssayDef)
{
	m_AssayDef = AssayDef;
	m_maxchars = 0;
}


CAssayPrDoc::~CAssayPrDoc()
{
}


BEGIN_MESSAGE_MAP(CAssayPrDoc, CGenDefPrDoc)
	//{{AFX_MSG_MAP(CAssayPrDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CAssayPrDoc::OnCloseDocument() 
{
	CGenDefPrDoc::OnCloseDocument();

	// NULL out the document pointer
	((CLuceptApp*)AfxGetApp())->m_pAssayPrDoc = NULL;
}

void CAssayPrDoc::InitPrintData()
{

	CACCMgr		lclACCMgr;
	CPrintLine	tmp;
	CSampRef	sampref;

	int i		= 0;
	long count	= 0;
	
	struct FLAG_ACTION 
	{
		UINT		action_k;
		LPCTSTR action_s;
	};
	FLAG_ACTION lclFlagActions[4]= 
	{ //cf, 
		{CUserFlag::DO_NOTHING,	_T("Do Nothing")	},
		{CUserFlag::SKIP_CONT,	_T("Skip Batch / Continue")	},
		{CUserFlag::SKIP_PAUSE, _T("Skip Batch / Pause")	},
		{CUserFlag::STOP,		_T("Stop Workload")			}
	};

	// Reset 
	m_lines.RemoveAll();
	m_maxchars = 0;

	//Initialize doc content from assay object
	//start with a blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	tmp.lbl = _T("Assay Name:");
	tmp.txt = m_AssayDef.GetName();
	if(m_ForHistory)
	{
		tmp.txt = m_AssayDef.m_fileName + _T(" : ") + m_AssayDef.GetName();
	}
	AddLine(&tmp);

	tmp.lbl = _T("Creator:");
	tmp.txt = m_AssayDef.GetCreator();
	AddLine(&tmp);

	tmp.lbl = _T("Created On:");
	tmp.txt = m_AssayDef.GetCreateDate().Format(_T("%x %X"));
	AddLine(&tmp);

	tmp.lbl = _T("Modifier:");
	tmp.txt = m_AssayDef.GetModifier();
	AddLine(&tmp);

	tmp.lbl = _T("Modified On:");
	tmp.txt = m_AssayDef.GetModDate().Format(_T("%x %X"));
	AddLine(&tmp);

	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	tmp.lbl = _T("Default Replicates:");
	tmp.txt.Format(_T("%d"),m_AssayDef.GetDefaultRepls());
	AddLine(&tmp);

	tmp.lbl = _T("Maximum Tubes in Batch:");
	tmp.txt.Format(_T("%d"),m_AssayDef.GetMaxTubes());
	AddLine(&tmp);

	tmp.lbl = _T("Measurement Method Name:");
	tmp.txt = m_AssayDef.GetMeasName();
	AddLine(&tmp);

	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	tmp.lbl = _T("Calculation Method:");
	tmp.txt = lclACCMgr.GetMethodName(m_AssayDef.GetCalcMethID());
	AddLine(&tmp);

	//calculation parameters
	count = m_AssayDef.GetNumParams();
	CParam param;
	ASSERT(count >= 0);
	for(i = 0; i < count;i++)
	{
		param = m_AssayDef.GetParamEx(i);
		tmp.lbl = m_indent1;
		tmp.lbl += param.m_name;
		//convert param value based on type
		switch(param.m_value.vt)
		{
		case VT_BOOL:
			tmp.txt = (param.m_value.boolVal ? _T("Yes") : _T("No"));
			break;

		case VT_UI1:
		case VT_I2:
		case VT_I4:
			tmp.txt.Format(_T("%ld"), param.m_value.lVal);
			break;

		case VT_R4:
		case VT_R8:
			tmp.txt.Format(_T("%.2f"), param.m_value.dblVal);
			break;

		default:
			tmp.txt.Format(_T("%f"), param.m_value.dblVal);
			break;
		}
		AddLine(&tmp);
	}

	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	tmp.lbl = _T("Use Default Calibrators:");
	tmp.txt = m_AssayDef.DefCalsAllowed() ? m_strChkd : m_strnChkd;
	AddLine(&tmp);

	tmp.lbl = _T("Calibrator Types Defined");
	tmp.txt = _T("");
	AddLine(&tmp);

	//loop through Calibrator Types
	count = m_AssayDef.GetNumCalSampTypes();
	for(i = 0; i < count; i++)
	{
		long id = m_AssayDef.IndexGetCalSampTypeID(i);

		//Calibrator Type name; no data
		tmp.lbl = m_indent1;
		tmp.lbl += m_AssayDef.IDGetSampDefName(id);
		tmp.txt = _T("");
		AddLine(&tmp);		
		
		// Properties:  sample reference 
		tmp.lbl = m_indent2;
		tmp.lbl += _T("Default Calibrator Sample Reference:"); 
		tmp.txt = _T("");
		AddLine(&tmp);
		//split this logical line to avoid potential path length problems
		tmp.lbl = _T("");
		VERIFY(!m_AssayDef.IDGetDefCalSampRef(id, &sampref));
		tmp.txt = sampref.GetRefString();
		AddLine(&tmp);

		//  Properties:  auto update
		tmp.lbl = m_indent2;
		tmp.lbl += _T("Automatically Update Reference:");
		tmp.txt = m_AssayDef.IndexGetCalUpdate(i) ? m_strChkd : m_strnChkd;
		AddLine(&tmp);
	}

	/* ...ignoring column settings ...*/
	
	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);
	
	tmp.lbl = _T("Sample Types:");
	tmp.txt = _T("");
	AddLine(&tmp);

	count = m_AssayDef.GetNumSampTypes();
	for(i = 0; i < count; i++)
	{	// Sample Type
		tmp.lbl = m_indent1;
		tmp.lbl += _T("Sample Type");
		tmp.txt = m_AssayDef.IndexGetSampDefName(i);
		AddLine(&tmp);		

		// Display Name
		tmp.lbl = m_indent2;
		tmp.lbl += _T("Display Name");
		tmp.txt = m_AssayDef.IDGetSampName(m_AssayDef.IndexGetSampID(i));
		AddLine(&tmp);			
	}

	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	//Flags
	tmp.txt = _T(""); //null str throughout flags loop
	CUserFlag fl;
	for(i = 0; i < NUM_FLAGS; i++)
	{	//get flag object
		fl = m_AssayDef.m_aFlags[i];
		tmp.lbl.Format(_T("Flag %d:"), i + 1);
		AddLine(&tmp);

		//if
		tmp.lbl = m_indent1;
		tmp.lbl += _T("If condition(s):");
		AddLine(&tmp);

		//conditions
		for(int j = 0; j < fl.m_condList.GetSize(); j++)
		{
			tmp.lbl = m_indent2;
			tmp.lbl += fl.m_condList[j];
			AddLine(&tmp);
		}

		//are
		tmp.lbl = m_indent1;
		tmp.lbl += _T("are:");
		AddLine(&tmp);

		//polarity
		tmp.lbl = m_indent2;
		tmp.lbl += fl.m_polarity ? m_strTrue : m_strFalse;
		AddLine(&tmp);

		//then
		tmp.lbl = m_indent1;
		tmp.lbl += _T("then:");
		AddLine(&tmp);

		//action
		tmp.lbl = m_indent2;
		tmp.lbl += lclFlagActions[fl.m_action].action_s;
		AddLine(&tmp);

		//blank line
		tmp.lbl = _T("");
		AddLine(&tmp);
	}
}

