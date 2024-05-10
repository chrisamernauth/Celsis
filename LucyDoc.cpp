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

// LucyDoc.cpp : implementation of the CLucyDoc class
//

#include "stdafx.h"
#include "LucyDoc.h"

#include "Lucept.h"
#include "LuceErr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLucyDoc

// IMPLEMENT_DYNCREATE(CLucyDoc, CDocument)
IMPLEMENT_SERIAL(CLucyDoc, CDocument, LUCYDOC_SER_SCHEMA)


BEGIN_MESSAGE_MAP(CLucyDoc, CDocument)
	//{{AFX_MSG_MAP(CLucyDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLucyDoc construction/destruction

CLucyDoc::CLucyDoc()
{
	m_bCanClose = false;

	// Initialize the reagent capacity and volumes
	for (int i = 0;  i < NUM_REAGENTS;  i++) 
	{
		m_ReagentLevels[i] = 0;
		m_ReagentCapacity[i] = MAX_REAGENT_VOL;
	}
}

CLucyDoc::~CLucyDoc()
{
}

BOOL CLucyDoc::CanCloseFrame( CFrameWnd* pFrame)
{
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CLucyDoc serialization

void CLucyDoc::Serialize(CArchive& ar)
{
	// does nothing CDocument::Serialize(ar);

	if (ar.IsStoring()) 
	{
		// Save doc type ID
		ar << LUCYDOC_MAGIC_INT_V2;
		// Save doc type version
		ar << LUCYDOC_SER_SCHEMA;
		for (int i = 0;  i < NUM_REAGENTS;  i++) 
		{
			ar << m_ReagentCapacity[i];
			ar << m_ReagentLevels[i];
		}
	} 
	else 
	{
		int magic;
		UINT schema;

		// Verify document ID
		ar >> magic;
		if (magic != LUCYDOC_MAGIC_INT && magic != LUCYDOC_MAGIC_INT_V2)
		{
			AfxThrowArchiveException(CArchiveException::badClass);
		}

		// Check document schema
		ar >> schema;
		if (schema == LUCYDOC_SER_SCHEMA)
		{
			for (int i = 0;  i < NUM_REAGENTS;  i++) 
			{
				// Old advance had only three volumes on the lucydoc file
				if( magic == LUCYDOC_MAGIC_INT_V2 || i <= 2) {
					ar >> m_ReagentCapacity[i];
					ar >> m_ReagentLevels[i];
				}
			}
		}
		else
		{
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLucyDoc diagnostics

#ifdef _DEBUG
void CLucyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLucyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLucyDoc commands

void CLucyDoc::ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault) 
{
	if (!bSaving && e->IsKindOf(RUNTIME_CLASS(CFileException)) 
		&& ((CFileException*)e)->m_cause == CFileException::fileNotFound)
	{	// If loading file and it doesn't exist, create a new one
		AfxMessageBox(_T("Creating new luminometer reagent file."));

		// File doesn't exist so create it the first time
		CFile file;
		CFileException e;
		if(!file.Open(lpszPathName, 
			CFile::modeCreate | 
			CFile::modeWrite | 
			CFile::shareExclusive, &e))
		{	
			AfxMessageBox(_T("Can't create luminometer reagent file."));
			CDocument::ReportSaveLoadException(lpszPathName, &e, bSaving, 
				AFX_IDP_FAILED_TO_OPEN_DOC);		
		}
		else
		{	// file opened so initialize it
			CArchive ar(&file, CArchive::store);
			Serialize(ar);
			ar.Close();
			file.Close();
		}
	}
	else
	{	// Default handler
		CDocument::ReportSaveLoadException(lpszPathName, e, bSaving, nIDPDefault);
	}
}

void CLucyDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(false);
}

void CLucyDoc::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(false);
}

void CLucyDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(false);
}

void CLucyDoc::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(false);
}

void CLucyDoc::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(false);
}

long CLucyDoc::GetReagentLevel(long injectID)
{
	ASSERT(injectID >= 1 && injectID <= NUM_REAGENTS);
	if (injectID >= 1 && injectID <= NUM_REAGENTS)
	{
		return m_ReagentLevels[injectID - 1];
	}
	else
	{
		return LUCYDOC_VOL_UNDEF;
	}
}

long CLucyDoc::GetReagentCapacity(long injectID)
{
	ASSERT(injectID >= 1 && injectID <= NUM_REAGENTS);
	if (injectID >= 1 && injectID <= NUM_REAGENTS)
	{
		return m_ReagentCapacity[injectID - 1];
	}
	else
	{
		return LUCYDOC_VOL_UNDEF;
	}
}


long CLucyDoc::SetReagentLevel(long injectID, long volume)
{
	long rv = KO_OK;

	if (injectID < 1 || injectID > NUM_REAGENTS)
	{
		return KO_LUC_BAD_INJECTOR_ID;
	}

	if (volume < 0 || volume > m_ReagentCapacity[injectID - 1])
	{
		return KO_LUC_VOL_NOT_IN_RANGE;
	}

	m_ReagentLevels[injectID - 1] = volume;

	if (Save())
	{
		return KO_OK;
	}
	else
	{
		return KO_LUCY_DOC_SAVE;
	}
}

long CLucyDoc::DeductReagent(long injectID, long volume)
{
	if (injectID < 1 || injectID > NUM_REAGENTS)
	{
		return KO_LUC_BAD_INJECTOR_ID;
	}

	if (volume < 0)
	{
		return KO_LUC_VOL_NOT_IN_RANGE;
	}

	if(volume > m_ReagentLevels[injectID - 1])
	{
		return KO_LUC_INSUFFICIENT_REAG;
	}

	m_ReagentLevels[injectID - 1] -= volume;

	if (Save())
	{
		return KO_OK;
	}
	else
	{
		return KO_LUCY_DOC_SAVE;
	}
}


bool CLucyDoc::Save()
{
#pragma warning(disable : 4800)
	return (bool)OnSaveDocument(GetPathName());
#pragma warning(default: 4800)
}
