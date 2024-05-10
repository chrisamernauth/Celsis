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

// BatRef.cpp: implementation of the CBatRef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lucept.h"
#include "BatRef.h"
#include "WklDoc.h"
#include "BPickDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CBatRef, CWklRef, BATREF_SER_SCHEMA)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBatRef::CBatRef(int iBatch, LPCTSTR fileSpec, RefType refType, 
				 PathBase base, LPCTSTR peerPath) 
	: CWklRef(fileSpec, refType, base, peerPath)
{
	m_iBatch = iBatch;
}

CBatRef::~CBatRef()
{
}

const CBatRef& CBatRef::operator =(const CBatRef & source)
{
	// Call base class assignment operator
	*((CWklRef*)this) = *((CWklRef*)&source);

	m_iBatch = source.m_iBatch;

	return *this;
}

void CBatRef::Serialize(CArchive & ar)
{
	// Base class
	CWklRef::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << BATREF_MAGIC_INT;
		ar << BATREF_SER_SCHEMA;
		ar << m_iBatch;
	}
	else
	{
		int magic;
		UINT ver;

		ar >> magic;
		if (magic != BATREF_MAGIC_INT)
		{
			AfxThrowArchiveException(CArchiveException::badClass);
		}
		ar >> ver;
		if (ver != BATREF_SER_SCHEMA)
		{
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
		ar >> m_iBatch;
	}
}

CString CBatRef::GetRefString()
{
	CString refStr;
	if (!IsValid())
	{
		refStr.Empty();
	}
	else
	{
		refStr = CWklRef::GetRefString();

		// Trim trailing >
		ASSERT(refStr.Right(1) == REF_STRING_CLOSE);
		refStr = refStr.Left(refStr.GetLength() - 1);

		// Add batch letter
		refStr += REF_COMPONENT_DELIMITER;
		refStr += CBatch::GetBatchLetter(m_iBatch);
		refStr += REF_STRING_CLOSE;
	}

	return refStr;
}

long CBatRef::InitFromRefString(LPCTSTR refString)
{
	CString wklRefStr(refString);
	CString batStr(refString);
	
	// Find last occurrence of delimiter string
	int delim;
	int lastDelim = -1;
	int delimLen = CString(REF_COMPONENT_DELIMITER).GetLength();
	do
	{
		delim = batStr.Find(REF_COMPONENT_DELIMITER);
		if (delim != -1)
		{
			// strip from left up to and including delimiter
			batStr = batStr.Mid(delim + delimLen);
			lastDelim = delim;
		}
	}
	while (delim != -1);

	if (lastDelim == -1)
	{
		return KO_REFSTR_MISSING_DELIM;
	}

	// Trim end of wkl string and append close
	wklRefStr = wklRefStr.Left(wklRefStr.GetLength() - 
		CString(REF_COMPONENT_DELIMITER).GetLength() -
		batStr.GetLength());
	wklRefStr += REF_STRING_CLOSE ;

	// Trim closing string from right of bat string
	ASSERT(batStr.Right(CString(REF_STRING_CLOSE).GetLength()) ==
		REF_STRING_CLOSE);
	batStr = batStr.Left(batStr.GetLength() - 
		CString(REF_STRING_CLOSE).GetLength());

	// Convert letters to index
	m_iBatch = CBatch::LetterGetBatchIndex(batStr);
	if (m_iBatch == LUC_BATCH_UNDEF)
	{	// should just be one int left
		return -1;
	}

	// Call base class on what's left of string
	return CWklRef::InitFromRefString(wklRefStr);
}

bool CBatRef::PickRef()
{
	if (!CWklRef::PickRef())
	{
		return false;
	}

	// Pick Batch
	CBatPickDlg dlg(GetNumBatches());

	if (dlg.DoModal() == IDOK)
	{
		m_iBatch = dlg.m_iBatch;
		return true;
	}
	else
	{
		return false;
	}
}

bool CBatRef::IsValid()
{
	if (m_iBatch == LUC_BATCH_UNDEF || m_iBatch < 0)
	{
		return false;
	}

	return CWklRef::IsValid();
}

bool CBatRef::RefExists()
{
	if (!IsValid())
	{
		return false;
	}

	// Check base class
	if (!CWklRef::RefExists())
	{
		return false;
	}
	
	// Get number of batches in workload ...
	int nBatches = GetNumBatches();
	
	if (nBatches == LUC_BATCH_UNDEF)
	{
		return false;
	}
	else
	{
		return m_iBatch < nBatches;	
	}
}

int CBatRef::GetNumBatches()
{
	CWklDoc* pWkl;
	CWklDoc	tempWkl;

	// Get a pointer to the workload ...
	pWkl = GetOpenDoc();
	if (pWkl == NULL)
	{
		// Read workload temporarily
		int rv = LoadWorkload(tempWkl);
		if (rv != KO_OK)
		{
			return LUC_BATCH_UNDEF;
		}
		pWkl = &tempWkl;
	}
	ASSERT_VALID(pWkl);
	
	return pWkl->GetNumBatches();	
}

int CBatRef::GetBatIndex()
{
	return m_iBatch;
}
