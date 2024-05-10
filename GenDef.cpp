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

// GenDef.cpp: implementation of the CGenDef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h" 
#include "GenDef.h"
#include "Event.h"
#include "SystemLog.h"
#include "UsrAdmin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CGenDef, CObject, GENDEF_SER_SCHEMA) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenDef::CGenDef()
{
	m_ID			= LUC_ID_UNDEF;	
	m_name			= _T("");
	m_fileName		= _T("");	
	m_creatorName	= _T("");
	m_createdOn		= COleDateTime::GetCurrentTime();
	m_modifierName	= _T("");
	m_modifiedOn	= COleDateTime::GetCurrentTime();

	m_fileVer = 0;
	// Descendant constructors must initialize the _T("virtual data members")
	//     which are labelled mv_*

	// Initialize in descendants constructor 
}

CGenDef::CGenDef(CGenDef & src)
{
	*this = src;
}

CGenDef::~CGenDef()
{

}


//////////////////////////////////////////////////////////////////////
// Assignment operator
//////////////////////////////////////////////////////////////////////

CGenDef& CGenDef::operator =(CGenDef & src)
{
	m_ID			= src.m_ID;					
	m_name			= src.m_name;				
	m_fileName		= src.m_fileName;		
// Don't copy extentsion.  
// This allows an obsolete file to be copied to a new one
//	m_fileExt		= src.m_fileExt;			
	m_creatorName	= src.m_creatorName;	
	m_createdOn		= src.m_createdOn;
	m_modifierName	= src.m_modifierName;
	m_modifiedOn	= src.m_modifiedOn;
	m_paramList		= src.m_paramList;

	return *this;
}


//////////////////////////////////////////////////////////////////////
// Serialize
//////////////////////////////////////////////////////////////////////

void CGenDef::Serialize( CArchive& archive)
{
	if (archive.IsStoring())
	{
		// Always saves the latest format
		archive << GENDEF_MAGIC_INT;
		archive << GENDEF_SER_SCHEMA;

		archive << m_ID;
		archive << m_name;
		archive << m_fileName;
		archive << m_fileExt;
		archive << m_creatorName;
		archive << m_createdOn;
		archive << m_modifiedOn;
		archive << m_modifierName; //new in version 3
	}
	else	// IsLoading
	{
		archive >> m_ID;
		if (m_ID == GENDEF_MAGIC_INT)
		{
			// This identifies the file as a version > 1
			// Read the actual file version
			archive >> m_fileVer;

			// Since the first int read was not really m_ID
			archive >> m_ID;
		}
		else
		{
			m_fileVer = 1;
		}
		
		switch (m_fileVer)
		{			
		case GENDEF_SER_SCHEMA:
			archive >> m_name;
			archive >> m_fileName;
			archive >> m_fileExt;
			archive >> m_creatorName;
			archive >> m_createdOn;
			archive >> m_modifiedOn;
			archive >> m_modifierName; //new in version 3
			break;
		case 2:
		case 1:// Version 1 and 2 differ only by inclusion of version info
			archive >> m_name;
			archive >> m_fileName;
			archive >> m_fileExt;
			archive >> m_creatorName;
			archive >> m_createdOn;
			archive >> m_modifiedOn;
			break;

		default:
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
	}

	// Serialize AssayCalc parameters
	m_paramList.Serialize(archive);
}


//////////////////////////////////////////////////////////////////////
// Get Assay Name
//////////////////////////////////////////////////////////////////////

CString CGenDef::GetName()
{
	return m_name;
}

void CGenDef::SetName(CString & name)
{
	m_name = name;
}

//////////////////////////////////////////////////////////////////////
// Get Assay ID
//////////////////////////////////////////////////////////////////////

int CGenDef::GetID()
{
	return m_ID;
}


//////////////////////////////////////////////////////////////////////
// Set Assay ID
//////////////////////////////////////////////////////////////////////
void CGenDef::SetID(int ID)
{
	// Set ID
	m_ID = ID;

	// Set file name from ID
	m_fileName.Format(mv_fileNameFormat, m_ID); 
}

//////////////////////////////////////////////////////////////////////
// Get Assay file path name
//////////////////////////////////////////////////////////////////////

CString CGenDef::GetPath()
{
	return m_fileName+m_fileExt;
}

//////////////////////////////////////////////////////////////////////
// Get Assay file path name
//////////////////////////////////////////////////////////////////////

CString CGenDef::GetFileName()
{
	return m_fileName;
}

//////////////////////////////////////////////////////////////////////
// Write Definition to Disk
//////////////////////////////////////////////////////////////////////

BOOLEAN CGenDef::WriteToDisk()
{
	CString fileSpec;
	CFile	outFile;
	BOOLEAN rv;

	ASSERT (m_ID != LUC_ID_UNDEF);

	VERIFY(GetFileSpec(fileSpec, m_ID));

	rv = outFile.Open(fileSpec, CFile::modeWrite | CFile::modeCreate
				| CFile::shareExclusive | CFile::typeBinary);

	if (rv)
	{
		// Create an archive
		CArchive ar(&outFile, CArchive::store);
		// !!! Catch errors?
		Serialize(ar);

		// Close archive and file
		ar.Close();
		outFile.Close();

		return TRUE;
	}
	else
	{
		// Error opening file
		// !!! Get system error reporting working
		return FALSE;
	}
}


//////////////////////////////////////////////////////////////////////
// Get File Spec from ID
//////////////////////////////////////////////////////////////////////

bool CGenDef::GetFileSpec(CString& fileSpec, int fileID)
{
	CString middleThing;

	// Create file name
	if (!MakeFileName(middleThing, fileID))
	{
		return FALSE;
	}
	middleThing += _T(".");

	// Create complete file specification
	fileSpec = mv_fileDir;
	fileSpec += middleThing;
	if (fileID == LUC_ID_UNDEF)
	{
		// Generic file mask
		fileSpec += mv_fileExtActive;
	}
	else
	{
		fileSpec += m_fileExt;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Create file name (or mask) from ID
//////////////////////////////////////////////////////////////////////

BOOLEAN CGenDef::MakeFileName(CString& name, int ID)
{
	if (ID >= 0)
	{
		// Valid normal ID
		name.Format(mv_fileNameFormat, ID); // File name (less extension)
		return TRUE;
	}
	else if (ID == LUC_ID_ALL)
	{
		name = mv_filePrefix;
		name += _T("*");
		return TRUE;
	}
	else
	{
		// Bad ID
		name = _T("");
		return FALSE;
	}
}


//////////////////////////////////////////////////////////////////////
// Obsolete the specified file
//////////////////////////////////////////////////////////////////////

bool CGenDef::Obsolete()
{
	// Make def obsolete
	m_fileExt = mv_fileExtObs;

	// Write to disk
	if (!WriteToDisk())
	{	// Failed to write 
		return false;
	}

	// Delete original
	m_fileExt = mv_fileExtActive;
	bool rv = DeleteFromDisk();
	m_fileExt = mv_fileExtObs;

	return rv;
}


//////////////////////////////////////////////////////////////////////
// Delete this objects file from disk
//////////////////////////////////////////////////////////////////////

bool CGenDef::DeleteFromDisk()
{
	CString fileSpec;
	CFile	outFile;

	GetFileSpec(fileSpec, m_ID);

	TRY
	{
		CFile::Remove(fileSpec);
	}
	CATCH( CFileException, e )
	{
		// Error deleting
		return false;
	}
	END_CATCH

	return true;
}

int CGenDef::GetNumParams()
{
	return m_paramList.GetCount();
}

CParamEx& CGenDef::GetParamEx(int iParam)	
{
	ASSERT(iParam >= 0 && iParam < m_paramList.GetCount());
	return m_paramList[iParam];
}

CParamEx* CGenDef::GetParamExPtrFromID(long paramID)	
{
	CParamEx* pParam = NULL;
	
	// Silly search
	int num = m_paramList.GetCount();
	for (int iParam = 0; iParam < num; iParam++)
	{
		ASSERT(m_paramList[iParam].IsKindOf(RUNTIME_CLASS(CParamEx)));
		if (m_paramList[iParam].m_ID == paramID)
		{
			pParam = &m_paramList[iParam];
			break;
		}
	}

	return pParam;
}

