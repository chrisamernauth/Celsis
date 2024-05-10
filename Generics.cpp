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
ITL				19-Mar-02			Modified by CAR 887/002
ITL				03-Apr-02			Amended by CAR 887/003

************************************************************************************/

//////////////////////////////////////////////////////////////////////
// Generics.cpp: implementation of the CGenerics class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Generics.h"
#include "GenBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenerics::CGenerics()
{
	// Descendants must initialize m_errMsg... strings
}

CGenerics::~CGenerics()
{
}


//////////////////////////////////////////////////////////////////////
// Add box pointer to registration list
//////////////////////////////////////////////////////////////////////
void CGenerics::RegGenBox (CGenBox* genBox)
{	// Update list of boxes
	m_pBoxList.AddTail(genBox);
};

//////////////////////////////////////////////////////////////////////
// Fill the provided Combo box with active names and associated ID's
//////////////////////////////////////////////////////////////////////

void CGenerics::LoadBox(CGenBox * genBox)
{	// Clear any pre-existing items
	genBox->ResetContent();

	// Make sure sorting is off so items and data are aligned
	if (m_pGenList.GetCount() == 0)
	{	// If none defined
		VERIFY(genBox->AddString(m_errMsgNoneDefined) == 0);
		// Show none defined string
		genBox->SetCurSel(0);
		// Disallow input
		genBox->EnableWindow(false);
	}
	else
	{	// Load names and associated ID's
		POSITION pos = m_pGenList.GetHeadPosition();

		while (pos != NULL)
		{
			int iBoxSel;
			iBoxSel= genBox->AddString(m_pGenList.GetAt(pos)->GetName());
			ASSERT(iBoxSel >=0);
			PUtil::SetLongLBData(*genBox, m_pGenList.GetAt(pos)->GetID(),
				iBoxSel);
			// Next in list
			m_pGenList.GetNext(pos);
		}

		// Enable selection, if appropriate
		if (genBox->IsEditable())
		{
			genBox->EnableWindow(true);
		}
	}
}

void CGenerics::LoadBoxAH(CGenBox * genBox)
{	// Clear any pre-existing assays
	genBox->ResetContent();

	// Make sure sorting is off so items and data are aligned
	if (m_pAHList.GetCount() == 0)
	{	// If none defined
		VERIFY(genBox->AddString(m_errMsgNoneDefined) == 0);
		// Show none defined string
		genBox->SetCurSel(0);
		// Disallow input
		genBox->EnableWindow(false);
	}
	else
	{	// Load names and associated ID's
		POSITION pos = m_pAHList.GetHeadPosition();

		CString	tempFileName = _T("");
		while (pos != NULL)
		{
			int iBoxSel;
			CString	tempItem;	// ITL: combine filename with assay name
			tempItem  = m_pAHList.GetAt(pos)->GetFileName();
			tempFileName = tempItem;
			tempItem += _T(" : ");
			tempItem += m_pAHList.GetAt(pos)->GetName();
			iBoxSel= genBox->AddString(tempItem);
			ASSERT(iBoxSel >=0);
			PUtil::SetLongLBData(*genBox, m_pAHList.GetAt(pos)->GetID(),
				iBoxSel);
			// Next in list
			m_pAHList.GetNext(pos);
		}

		// Enable selection, if appropriate
		if (genBox->IsEditable())
		{
			genBox->EnableWindow(true);
		}
	}
}

void CGenerics::LoadBoxMH(CGenBox * genBox)
{	// Clear any pre-existing obsolete measurement definitions
	genBox->ResetContent();

	// Make sure sorting is off so items and data are aligned
	if (m_pMHList.GetCount() == 0)
	{	// If none defined
		VERIFY(genBox->AddString(m_errMsgNoneDefined) == 0);
		// Show none defined string
		genBox->SetCurSel(0);
		// Disallow input
		genBox->EnableWindow(false);
	}
	else
	{	// Load names and associated ID's
		POSITION pos = m_pMHList.GetHeadPosition();

		while (pos != NULL)
		{
			int iBoxSel;
			CString	tempItem;	// ITL: combine filename with measurement name
			tempItem =  m_pMHList.GetAt(pos)->GetFileName();
			tempItem += _T(" : ");
			tempItem += m_pMHList.GetAt(pos)->GetName();
			iBoxSel= genBox->AddString(tempItem);
			ASSERT(iBoxSel >=0);
			PUtil::SetLongLBData(*genBox, m_pMHList.GetAt(pos)->GetID(),
				iBoxSel);
			// Next in list
			m_pMHList.GetNext(pos);
		}

		// Enable selection, if appropriate
		if (genBox->IsEditable())
		{
			genBox->EnableWindow(true);
		}
	}
}

void CGenerics::RelGenBox(CGenBox * genBox)
{	// Remove specified pointer from box list
	POSITION pos = m_pBoxList.GetHeadPosition();
	POSITION nextPos;

	// (Remove as many times as it occurs)
	while (pos != NULL)
	{
		nextPos = pos;
		if (m_pBoxList.GetNext(nextPos) == genBox)
		{	// pos contains the specified pointer
			m_pBoxList.RemoveAt(pos);
		}
		pos = nextPos;
	}
}


bool CGenerics::SetBoxSelection(CGenBox * pBox, long selectID)
{	// If no definitions exist, ignore selection
	if (m_pGenList.GetCount() == 0)
	{
		pBox->EnableWindow(false);

		if (selectID == LUC_ID_UNDEF)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (selectID == LUC_ID_UNDEF)
	{	// Deselect all
		pBox->SetCurSel(-1);
		return true;
	}

	// Find combobox index of selection with specified ID
	int num = pBox->GetCount();
	int iSelect = -1;
	for (int iGeneric = 0; iGeneric < num; iGeneric++)
	{
		long iID = PUtil::GetLongLBData(*pBox, iGeneric);
		if (iID == selectID)
		{	// ID's match.  
			ASSERT(iSelect == -1);	
			iSelect = iGeneric;
		}
	}

	if (iSelect != -1)
	{	// ID was found
		return (pBox->SetCurSel(iSelect) == iSelect);
	}
	else
	{	// ID not found, deselect all
		VERIFY(pBox->SetCurSel(iSelect) == CB_ERR);
		return false;
	}
}

bool CGenerics::SetBoxSelection(CGenBox * pBox, LPCTSTR selectName)
{
	CString name(selectName);

	// If no definitions exist, ignore selection
	if (m_pGenList.GetCount() == 0)
	{
		pBox->EnableWindow(false);

		if (name.IsEmpty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (name.IsEmpty())
	{	// Deselect all
		pBox->SetCurSel(-1);
		return true;
	}

	// Find combobox index of selection with specified name
	int iSelect = pBox->FindStringExact(-1, name);
	if (iSelect != -1)
	{	// Name was found
		return (pBox->SetCurSel(iSelect) == iSelect);
	}
	else
	{	// Name was not found, deselect all
		pBox->SetCurSel(-1);
		return false;
	}
}

void CGenerics::UpdateGenBoxes()
{	// Re-initialize all registered boxes
	POSITION pos = m_pBoxList.GetHeadPosition();

	while (pos != NULL)
	{	// Get selected items ID
		CGenBox* pBox = m_pBoxList.GetNext(pos);
		long selectID = LUC_ID_UNDEF;
		if (pBox->GetCurSel() != -1)
		{
			selectID = PUtil::GetLongLBData(*pBox);
		}

		// Reload item list
		LoadBox(pBox);

		// Re-select specified item
		SetBoxSelection(pBox, selectID);
	}
}

void CGenerics::UpdateGenBoxesAH()
{	// Re-initialize all registered boxes
	POSITION pos = m_pBoxList.GetHeadPosition();

	while (pos != NULL)
	{	// Get selected items ID
		CGenBox* pBox = m_pBoxList.GetNext(pos);
		long selectID = LUC_ID_UNDEF;
		if (pBox->GetCurSel() != -1)
		{
			selectID = PUtil::GetLongLBData(*pBox);
		}

		// Reload item list
		LoadBoxAH(pBox);

		// Re-select specified item
		SetBoxSelection(pBox, selectID);
	}
}

void CGenerics::UpdateGenBoxesMH()
{	// Re-initialize all registered boxes
	POSITION pos = m_pBoxList.GetHeadPosition();

	while (pos != NULL)
	{	// Get selected items ID
		CGenBox* pBox = m_pBoxList.GetNext(pos);
		long selectID = LUC_ID_UNDEF;
		if (pBox->GetCurSel() != -1)
		{
			selectID = PUtil::GetLongLBData(*pBox);
		}

		// Reload item list
		LoadBoxMH(pBox);

		// Re-select specified item
		SetBoxSelection(pBox, selectID);
	}
}

//////////////////////////////////////////////////////////////////////
// Return number of active generics
//////////////////////////////////////////////////////////////////////

int CGenerics::GetCount()
{
	return m_pGenList.GetCount();
}


//////////////////////////////////////////////////////////////////////
// Load available information by reading from disk
//////////////////////////////////////////////////////////////////////

BOOLEAN CGenerics::InitFromDisk(long ID)
{
	POSITION pos = NULL;	
	CString errMsg;
	CString fileSpec, fileSpecPattern;
	CFileFind finder;
	BOOLEAN rv, rv2;
	CFile InpFile;
	DWORD sysErr;

	CString fileSpecPatternAH;	// ITL: History
	CString fileSpecPatternMH;	// ITL: History
	int		fileLegnth;			// ITL: History
	bool	assayNotMeas;		// ITL: History
	
	// Create dummy definition 
	CGenDef* pDummyDef = MakeNewDef();
	if (pDummyDef == NULL)
	{
		ASSERT(FALSE);
	}

	// Remove items to initialize from existing list
	if (ID == LUC_ID_ALL)
	{	// Re-read all.  Clear entire list
		DeleteAll();
		DeleteAll_AH();
		DeleteAll_MH();
	}
	else
	{	// If specified definition exists, re-read object
		// ITL: Generic
		// pos != NULL will be tested later
		pos = m_pGenList.GetHeadPosition();

		while (pos != NULL)
		{
			if (m_pGenList.GetAt(pos)->GetID() == ID)
			{
				break;
			}
			m_pGenList.GetNext(pos);// Try next
		}
		// ITL: Assay History
		pos = m_pAHList.GetHeadPosition();

		while (pos != NULL)
		{
			if (m_pAHList.GetAt(pos)->GetID() == ID)
			{
				break;
			}
			m_pAHList.GetNext(pos);
		}
		// ITL: Measurement History
		pos = m_pMHList.GetHeadPosition();

		while (pos != NULL)
		{
			if (m_pMHList.GetAt(pos)->GetID() == ID)
			{
				break;
			}
			m_pMHList.GetNext(pos);
		}
	}

	// Get file specification for given ID
	// Use dummy instance to get file spec
	pDummyDef->GetFileSpec(fileSpecPattern, ID);
	fileLegnth = fileSpecPattern.GetLength();

	if(fileSpecPattern.Right(3) == _T("ADF"))
	{	// ITL: Set the flag to assay and change the file extension
		assayNotMeas = true;
		fileSpecPatternAH = fileSpecPattern.Left(fileLegnth-1)+_T("O");	
	}
	else
	{	// ITL: Set the flag to measurement and change the file extension
		assayNotMeas = false;
		fileSpecPatternMH = fileSpecPattern.Left(fileLegnth-1)+_T("O");	
	}

	// ITL: Generics original search
	rv = finder.FindFile(fileSpecPattern);		
	if (!rv)
	{	// Start file search
		sysErr = ::GetLastError();
		if (sysErr == ERROR_PATH_NOT_FOUND)
		{	// Use dummy instance to get file spec
			errMsg = pDummyDef->mv_fileDirErr;
			errMsg += fileSpecPattern;
			goto error;
		}
		else if (sysErr != ERROR_FILE_NOT_FOUND)
		{
			errMsg.Format(_T("System Error:  %d"), sysErr);
			AfxMessageBox(errMsg);
			goto error;
		}
	}
	// Either the find was OK or the specified file(s) were not found

	// Find all files
	while (rv)
    {
        rv = finder.FindNextFile();
		if (rv || ::GetLastError() == ERROR_NO_MORE_FILES )
		{	// ERROR_NO_MORE_FILES is returned upon finding the last
			// file, so we really got a file this time

			// Found a file, so connect it to an archive
			fileSpec = finder.GetFilePath();
			rv2 = InpFile.Open(fileSpec, CFile::modeRead 
				| CFile::shareDenyWrite | CFile::typeBinary);
			if (rv2)
			{
				CGenDef* pGenDef;
				// Create an archive
				CArchive ar(&InpFile, CArchive::load);
				// Get definition object
				if (pos != NULL)
				{	// Point to existing object
					pGenDef = m_pGenList.GetAt(pos);
				}
				else
				{	// Create new object added to tail
					pGenDef = MakeNewDef();
					ASSERT(pGenDef != NULL);

					m_pGenList.AddTail(pGenDef);
				}
				// Load object from disk
				pGenDef->Serialize(ar);
				// Close archive and file
				ar.Close();
				InpFile.Close();
			}
			else
			{	// Use dummy instance to get file spec
				errMsg = pDummyDef->mv_fileDirErr;
				errMsg += fileSpec;
				goto error;
			}
		}
		else
		{
			sysErr = ::GetLastError();
			errMsg.Format(_T("System Error:  %d"), sysErr);
			AfxMessageBox(errMsg);
		}
    }	// ITL: End Generics original search

	// ITL: Assay / Measurement Definitions History search
	if(assayNotMeas)
	{
		rv = finder.FindFile(fileSpecPatternAH);
	}
	else
	{
		rv = finder.FindFile(fileSpecPatternMH);
	}
	if (!rv)
	{	// ITL: Start file search
		sysErr = ::GetLastError();
		if (sysErr == ERROR_PATH_NOT_FOUND)
		{	// ITL: Use dummy instance to get file spec
			errMsg = pDummyDef->mv_fileDirErr;
			if(assayNotMeas)
			{
				errMsg += fileSpecPatternAH;
			}
			else
			{
				errMsg += fileSpecPatternMH;
			}
			errMsg += fileSpecPatternAH;
			goto error;
		}
		else if (sysErr != ERROR_FILE_NOT_FOUND)
		{
			errMsg.Format(_T("System Error:  %d"), sysErr);
			AfxMessageBox(errMsg);
			goto error;
		}
	}	// ITL: Either the find was OK or the specified file(s) were not found

	while (rv)
	{	// ITL: Find all files
     rv = finder.FindNextFile();
		if (rv || ::GetLastError() == ERROR_NO_MORE_FILES )
		{	// ERROR_NO_MORE_FILES is returned upon finding the last
			// file, so we really got a file this time
			// Found a file, so connect it to an archive
			fileSpec = finder.GetFilePath();
			rv2 = InpFile.Open(fileSpec, CFile::modeRead 
				| CFile::shareDenyWrite | CFile::typeBinary);
			if (rv2)
			{
				CGenDef* pGenDef;
				// Create an archive
				CArchive ar(&InpFile, CArchive::load);
				// Get definition object
				if (pos != NULL)
				{	// Point to existing object
					if(assayNotMeas)
					{	// ITL: Assay Definitions
						pGenDef = m_pAHList.GetAt(pos);
					}
					else
					{	// ITL: Measurement Definitions
						pGenDef = m_pMHList.GetAt(pos);
					}
				}
				else
				{	// Create new object added to tail
					pGenDef = MakeNewDef();
					ASSERT(pGenDef != NULL);
					if(assayNotMeas)
					{	// ITL: Assay Definitions
						m_pAHList.AddTail(pGenDef);
					}
					else
					{	// ITL: Measurement Definitions
						m_pMHList.AddTail(pGenDef);
					}
				}
				// Load object from disk
				pGenDef->Serialize(ar);
				// Close archive and file
				ar.Close();
				InpFile.Close();
			}
			else
			{	// Use dummy instance to get file spec
				errMsg = pDummyDef->mv_fileDirErr;
				errMsg += fileSpec;
				goto error;
			}
		}
		else
		{
			sysErr = ::GetLastError();
			errMsg.Format(_T("System Error:  %d"), sysErr);
			AfxMessageBox(errMsg);
		}
	}	// ITL: End History search

	// De-allocate dummy definition
	VERIFY(DeleteDef(pDummyDef));

	// Update any attached assay list boxes
	UpdateGenBoxes();
	if(assayNotMeas)
	{	// Update any attached assay list boxes
		UpdateGenBoxesAH();
	}
	else
	{	// Update any attached measurement list boxes
		UpdateGenBoxesMH();
	}

	return TRUE;

error:
	// De-allocate dummy definition
	VERIFY(DeleteDef(pDummyDef));
	AfxMessageBox(errMsg);
	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Return ID given name 
//////////////////////////////////////////////////////////////////////

int CGenerics::GetIDFromName(CString& name)
{
	POSITION pos = m_pGenList.GetHeadPosition();

	// For entire list
	while (pos != NULL)
	{	// Check for matching name
		if (m_pGenList.GetAt(pos)->GetName() == name)
		{	// First name found is used
			return m_pGenList.GetAt(pos)->GetID();
		}
		m_pGenList.GetNext(pos);
	}
	// No match was found
	return LUC_ID_UNDEF;
};

//////////////////////////////////////////////////////////////////////
// Return name given ID
//////////////////////////////////////////////////////////////////////

BOOLEAN CGenerics::GetNameFromID(long ID, CString & name)
{
	POSITION pos = m_pGenList.GetHeadPosition();

	// For entire list
	while (pos != NULL)
	{	// Check for matching ID
		if (m_pGenList.GetAt(pos)->GetID() == ID)
		{	// First ID found is used
			name =  m_pGenList.GetAt(pos)->GetName();
			return TRUE;
		}
		m_pGenList.GetNext(pos);
	}
	name = m_errMsgBadID;
	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Get next available ID
//////////////////////////////////////////////////////////////////////

long CGenerics::GetFreeID()
{
	POSITION pos = m_pGenList.GetHeadPosition();

	int freeID = 0;
	while (pos != NULL)
	{
		int nextID = m_pGenList.GetAt(pos)->GetID();
		freeID = max(freeID, nextID);
		m_pGenList.GetNext(pos);
	}
	// One bigger than the existing biggest, or starts at 1
	freeID++;
	return freeID;
}

long CGenerics::GetFreeAHID()
{	// Added by CAR 887/002
	POSITION pos = m_pAHList.GetHeadPosition();

	int freeID = 0;
	while (pos != NULL)
	{
		int nextID = m_pAHList.GetAt(pos)->GetID();
		freeID = max(freeID, nextID);
		m_pAHList.GetNext(pos);
	}
	// One bigger than the existing biggest, or starts at 1
	freeID++;
	return freeID;
}

long CGenerics::GetFreeMHID()
{	// Added by CAR 887/002
	POSITION pos = m_pMHList.GetHeadPosition();

	int freeID = 0;
	while (pos != NULL)
	{
		int nextID = m_pMHList.GetAt(pos)->GetID();
		freeID = max(freeID, nextID);
		m_pMHList.GetNext(pos);
	}
	// One bigger than the existing biggest, or starts at 1
	freeID++;
	return freeID;
}

//////////////////////////////////////////////////////////////////////
// Delete entire list of objects
//////////////////////////////////////////////////////////////////////

void CGenerics::DeleteAll()
{
	POSITION pos = m_pGenList.GetTailPosition();

	while (pos != NULL)
	{	// Delete object
		VERIFY(DeleteDef(m_pGenList.GetAt(pos)));
		// Delete Pointer
		m_pGenList.RemoveTail();
		// Next tail
		pos = m_pGenList.GetTailPosition();
	}
	UpdateGenBoxes();
}

void CGenerics::DeleteAll_AH()
{	// ITL: Added by CAR 887/002
	POSITION pos = m_pAHList.GetTailPosition();

	while (pos != NULL)
	{	// ITL: Delete object
		VERIFY(DeleteDef(m_pAHList.GetAt(pos)));
		// ITL: Delete Pointer
		m_pAHList.RemoveTail();
		// ITL: Next tail
		pos = m_pAHList.GetTailPosition();
	}
	UpdateGenBoxesAH();
}

void CGenerics::DeleteAll_MH()
{	// ITL: Added by CAR 887/002
	POSITION pos = m_pMHList.GetTailPosition();

	while (pos != NULL)
	{	// ITL: Delete object
		VERIFY(DeleteDef(m_pMHList.GetAt(pos)));
		// ITL: Delete Pointer
		m_pMHList.RemoveTail();
		// ITL: Next tail
		pos = m_pMHList.GetTailPosition();
	}
	UpdateGenBoxesMH();
}


//////////////////////////////////////////////////////////////////////
// Find definition in list and return its pointer
//////////////////////////////////////////////////////////////////////

CGenDef* CGenerics::GetGenDefPtr(long ID)
{
	POSITION pos = m_pGenList.GetHeadPosition();

	while (pos != NULL)
	{
		CGenDef* ptr = 	m_pGenList.GetNext(pos);
		if (ptr->GetID() == ID)
		{	// Found the right definition
			return ptr;
		}
	}
	return NULL;
}

CGenDef* CGenerics::GetGenDefPtrAH(long ID)
{
	POSITION pos = m_pAHList.GetHeadPosition();

	while (pos != NULL)
	{
		CGenDef* ptr = 	m_pAHList.GetNext(pos);
		if (ptr->GetID() == ID)
		{	// Found the right definition
			return ptr;
		}
	}
	return NULL;
}

CGenDef* CGenerics::GetGenDefPtrMH(long ID)
{
	POSITION pos = m_pMHList.GetHeadPosition();

	while (pos != NULL)
	{
		CGenDef* ptr = 	m_pMHList.GetNext(pos);
		if (ptr->GetID() == ID)
		{	// Found the right definition
			return ptr;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// Obsolete all defs with the specified name
//////////////////////////////////////////////////////////////////////

bool CGenerics::Obsolete(CString & name)
{
	bool rv = false;
	POSITION posThis, posNext;

	CGenDef* pDef;
	posThis = posNext = m_pGenList.GetHeadPosition();

	while (posNext != NULL)
	{
		pDef = m_pGenList.GetNext(posNext);
		if (pDef->GetName() == name)
		{
			// Remove from list
			m_pGenList.RemoveAt(posThis);
			// Delete from disk and re-write as obsolete
			rv = pDef->Obsolete();
			if (rv == false)
			{
				ASSERT(false);
			}
			// Destroy object
			delete pDef;
		}
		posThis = posNext;
	}
	UpdateGenBoxes();

	return rv;
}


CGenDef* CGenerics::GetPtrFromName(LPCTSTR name)
{
	POSITION pos = m_pGenList.GetHeadPosition();

	while (pos != NULL)
	{
		CGenDef* ptr = 	m_pGenList.GetNext(pos);
		if (ptr->GetName() == name)
		{	// Found the right definition
			return ptr;
		}
	}
	return NULL;
}
