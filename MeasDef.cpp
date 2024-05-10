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

// AssyDef.cpp: implementation of the CAssyDef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeasDef.h"
#include "LccMgr.h"
#include "LccUser.h"
#include "LuceErr.h"
#include "Lucept.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define JUST_FOR_TEST

IMPLEMENT_SERIAL(CMeasDef, CObject, MEASDEF_SER_SCHEMA) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeasDef::CMeasDef()
{
	CString	section = _T("Paths");
	CString item	=_T("MeasDataPath");
	CLuceptApp* app	= (CLuceptApp *)AfxGetApp();

	// Initialize _T("virtual data members")
	mv_fileDir = app->GetLuceptProfileString(section, item, _T(""));
	if (mv_fileDir.IsEmpty())
	{
		AfxMessageBox(IDS_NO_MSR_DIR);
		app->WriteProfileString(section, item, mv_fileDir);
	}

	VERIFY(mv_filePrefix.LoadString(IDS_MS_FILE_PREFIX));
	VERIFY(mv_fileNameFormat.LoadString(IDS_MSFORMAT));
	VERIFY(mv_fileExtActive.LoadString(IDS_MSR_EXTENSION));
	m_fileExt = mv_fileExtActive;
	VERIFY(mv_fileExtObs.LoadString(IDS_MSO_EXTENSION));
	VERIFY(mv_fileDirErr.LoadString(IDS_RD_ERR_MSR_DIR));

	m_procClass		= LCC_CS_CLASS_READ;
	m_readProcID	= LUC_ID_UNDEF;
	m_readProcName	= _T(""); //new in Adv 2.1
}

CMeasDef::~CMeasDef()
{

}


//////////////////////////////////////////////////////////////////////
// Serialize
//////////////////////////////////////////////////////////////////////

void CMeasDef::Serialize( CArchive& archive)
{
	// Do base class first
	CGenDef::Serialize( archive );

	if (archive.IsStoring())
	{	// Always saves the latest format
		archive << MEASDEF_MAGIC_INT;
		archive << MEASDEF_SER_SCHEMA;

		archive << m_readProcID;
		archive << m_readProcName;
	}
	else
	{
		int magicInt;
		UINT schemaVer;

		if (m_fileVer == 1) // from gendef base class
		{	// Kludge based on gendef file version
			schemaVer = 1;
		}
		else
		{	// actually read the schema
			archive >>  magicInt;
			ASSERT(magicInt == MEASDEF_MAGIC_INT);
			archive >> schemaVer;
		}

		switch (schemaVer)
		{
		case MEASDEF_SER_SCHEMA:
			archive >> m_readProcID;
			archive >> m_readProcName;
			break;

		case 2:// differ only in version information
		case 1:
			archive >> m_readProcID;
			m_readProcName = _T("undefined"); //default intitialization
			break;

		default:
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
	}

	// Nothing else to save
}

long CMeasDef::GetReadProcID()
{
	return m_readProcID;
}

CString CMeasDef::GetReadProcName()
{
	return m_readProcName;
}

//////////////////////////////////////////////////////////////////////
// Load measurement parameters from LCC
//////////////////////////////////////////////////////////////////////

bool CMeasDef::InitParamList(CLccMgr * pLcc)
{
	long rv;

//
//  The following sanity check is now removed as if any protocols are marked as 'used=0' in lcc.ini they
//  are not included in the count and yet they still cause a protocol id to be allocated.
//
//	if (m_readProcID < 1 ||	m_readProcID > pLcc->GetNumClassProcs(m_procClass))
//
	if( m_readProcID < 1) {
		CString errMsg;
		errMsg.Format(IDS_BAD_READ_PROC_ID, m_readProcID);
		AfxMessageBox(errMsg);
		return false;
	}

	// Procedure found
	// Clear existing parameter list
	m_paramList.DeleteAll();

	// Load parameters from LCC
	int nParams;
	rv = pLcc->GetNumParams(m_procClass, m_readProcID, &nParams);
	if (rv != KO_OK) 
	{
		return false;
	}

	// For each parameter
	for (int iParam = 0; iParam < nParams; iParam++)
	{	// Add parameter to list
		long paramID = iParam + 1;	// Offset 1 (ugh.)
		CParamEx* pParam = m_paramList.Append();

		// Load parameter from Lcc
		rv = pLcc->GetParamEx(m_procClass, m_readProcID,
			paramID, *pParam);

		// Set param value to nominal
		pParam->m_value = pParam->m_default;
	}
	return true;
}

void CMeasDef::SetReadProc(long readProcID, CString readProcName, long procClass)
{
	m_readProcID	= readProcID;
	m_readProcName	= readProcName;
	m_procClass		= procClass;

	// Clear any parameters
	m_paramList.DeleteAll();
}

long CMeasDef::InitLccParams(CLccUser* pUser)
{
	long rv;

	if (m_readProcID == LUC_ID_UNDEF)
	{
		return KO_LUC_BAD_PROC_ID;
	}
	// Method must already be opened!
	ASSERT(pUser->GetLccMgr()->CSGetStatus() == LCC_CS_OPENED);

	// Check number of parameters
	int numParams;

	rv = pUser->GetLccMgr()->GetNumParams(m_procClass,m_readProcID, &numParams);
	if (rv != KO_OK)
	{
		return rv;
	}
	if (m_paramList.GetCount() != numParams)
	{
		return KO_LUC_NUM_READ_PARAMS;
	}
	// Set parameters
	for (int iParam = 0; iParam < numParams; iParam++)
	{
		rv = pUser->CSSetParam(m_paramList[iParam]);
		if (rv != KO_OK)
		{
			return rv;
		}
	}
	return rv;
}
