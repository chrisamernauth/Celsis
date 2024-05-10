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

// MeasPrDc.cpp: implementation of the CMeasPrDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lucept.h"
#include "MeasPrDc.h"
#include "LuceErr.h"
#include "lccmgr.h"
#include "GnDfPrDc.h"
#include "MainFrm.h"
#include "Specification.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMeasPrDoc, CGenDefPrDoc)
BEGIN_MESSAGE_MAP(CMeasPrDoc, CGenDefPrDoc)
	//{{AFX_MSG_MAP(CMeasPrDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeasPrDoc::CMeasPrDoc()
{
}

CMeasPrDoc::CMeasPrDoc(CMeasDef& MeasDef)
{
	m_MeasDef = MeasDef;
}

CMeasPrDoc::~CMeasPrDoc()
{

}

void CMeasPrDoc::OnCloseDocument() 
{
	CGenDefPrDoc::OnCloseDocument();

	// NULL out the document pointer
	if(((CLuceptApp*)AfxGetApp())->m_pMeasPrDoc)
	{
		((CLuceptApp*)AfxGetApp())->m_pMeasPrDoc = NULL;
	}
}

void CMeasPrDoc::InitPrintData()
{
	CPrintLine	tmp;

	int i		= 0;
	long count	= 0;

	// Reset 
	m_lines.RemoveAll();
	m_maxchars = 0;

	//Add text lines
	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	tmp.lbl = _T("Measurement Name:");
	tmp.txt = m_MeasDef.GetName();
	if(m_ForHistory)
	{
		tmp.txt = m_MeasDef.m_fileName + _T(" : ") + m_MeasDef.GetName();
	}
	AddLine(&tmp);

	tmp.lbl = _T("Creator:");
	tmp.txt = m_MeasDef.GetCreator();
	AddLine(&tmp);

	tmp.lbl = _T("Created On:");
	tmp.txt = m_MeasDef.GetCreateDate().Format(_T("%x %X"));
	AddLine(&tmp);

	tmp.lbl = _T("Modifier:");
	tmp.txt = m_MeasDef.GetModifier();
	AddLine(&tmp);

	tmp.lbl = _T("Modified On:");
	tmp.txt = m_MeasDef.GetModDate().Format(_T("%x %X"));
	AddLine(&tmp);

	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	//get protocol filename
	tmp.lbl = _T("Luminometer Measurement Protocol:");
	CLccUser lclLcc;
	long rc = KO_OK;
	rc = lclLcc.RegisterWithLcc();
	if (rc == KO_OK)
	{
		rc = lclLcc.GetLccMgr()->GetClassProcName(
			LCC_CS_CLASS_READ, //read procedure
			m_MeasDef.GetReadProcID(),
			tmp.txt);
		ASSERT(rc == KO_OK);
		rc = lclLcc.UnregisterWithLcc();
		ASSERT(rc == KO_OK);
	}
	else
	{
		tmp.txt = _T("Error Registering LCC");
	}
	AddLine(&tmp);
	
	//blank line
	tmp.lbl = _T("");
	tmp.txt = _T("");
	AddLine(&tmp);

	//calculation parameters
	count = m_MeasDef.GetNumParams();
	CParam param;
	ASSERT(count >= 0);
	for(i = 0; i < count;i++)
	{
		param = m_MeasDef.GetParamEx(i);

		CString strName = param.m_name;

		if( strName.Find( _T("Temp")) >= 0 && !Specification::g_oSpecification.HasTemperatureControl()) {
			continue;
		}

		// Crud unicode translation.
		strName.Remove( _T('\xC2'));
		
		tmp.lbl = strName;

		//convert param value based on type
		switch(param.m_value.vt)
		{
		case VT_BOOL:
			tmp.txt = (param.m_value.boolVal ? _T("Yes") : _T("No"));
			break;
		case VT_UI1:
		case VT_I2:
		case VT_I4:
			tmp.txt.Format(_T("%ld"),  param.m_value.lVal);
			break;
		case VT_R4:
		case VT_R8:
			tmp.txt.Format(_T("%.0f"), param.m_value.dblVal);
			break;
		default:
			tmp.txt.Format(_T("%f"),   param.m_value.dblVal);
			break;
		}
		AddLine(&tmp);
	}
}