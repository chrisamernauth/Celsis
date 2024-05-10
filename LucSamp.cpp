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

// LucSamp.cpp: implementation of the CLucSamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LucSamp.h"
#include "FldProp.h"
#include "Lucept.h"
#include "ACCMgr.h"
#include "LuceErr.h"
#include "LucColor.h"
#include "AssyDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLucSamp, CSample, SAMP_SER_SCHEMA);

CLucSamp::CLucSamp(CFieldProperties* pFldProp) 
	: CSample(pFldProp)
{
	UpdateStatus();
}

CLucSamp::~CLucSamp()
{

}

void CLucSamp::Serialize(CArchive& ar)
{	// Just call base class
	CSample::Serialize(ar);
}


//////////////////////////////////////////////////////////////////////
// Update sample's field properties based on document's property info
//////////////////////////////////////////////////////////////////////

bool CLucSamp::UpdateProperties()
{	// Call base class
	bool rv = CSample::UpdateProperties();

	if (rv == true)
	{	// Do specialized initialization
		CUGCell tempCell;
		CString text;

		// If undefined, Replicates defaults to 1
		GetDatum(m_pFldProp->GetIDIndex(SF_REPLS), &tempCell);
		tempCell.GetText(&text);
		if (text.IsEmpty())
		{
			tempCell.SetNumber(1);
		}
		SetDatum(m_pFldProp->GetIDIndex(SF_REPLS), tempCell);

		// If undefined, Attitude defaults to coma
		GetDatum(m_pFldProp->GetIDIndex(SF_ATTITUDE), &tempCell);
		tempCell.GetText(&text);
		if (text.IsEmpty())
		{
			tempCell.SetNumber(LUC_TUDE_COMA);
		}
		SetDatum(m_pFldProp->GetIDIndex(SF_ATTITUDE), tempCell);

		// If undefined, status defaults to new
		GetDatum(m_pFldProp->GetIDIndex(SF_STATUS), &tempCell);
		tempCell.GetText(&text);
		if (text.IsEmpty())
		{
			tempCell.SetText(((CLuceptApp*)AfxGetApp())->m_sampStats.GetNameFromID(LUC_SSTAT_NEW));
		}
		SetDatum(m_pFldProp->GetIDIndex(SF_STATUS), tempCell);

		// If undefined, Virtual defaults to real
		GetDatum(m_pFldProp->GetIDIndex(SF_VIRTUAL), &tempCell);
		tempCell.GetText(&text);
		if (text.IsEmpty())
		{
			tempCell.SetNumber(LUC_VSTAT_REAL);
		}
		SetDatum(m_pFldProp->GetIDIndex(SF_VIRTUAL), tempCell);

		// Initialize sample types 
		UpdateSampType();

		// Set cell colors based on status and properties
		VERIFY(UpdateStatus());
	}

	return rv;
}


void CLucSamp::UpdateSampType()
{
		CUGCell tempCell;
		CString text;

		// Sample type droplist string must be set
		GetDatum(m_pFldProp->GetIDIndex(SF_TYPE_XFI), &tempCell);
		tempCell.SetLabelText(m_pFldProp->m_sampTypeList);

		// If undefined, initialize to first selection in list
		tempCell.GetText(&text);
		if (text.IsEmpty())
		{
			tempCell.SetText(m_pFldProp->m_sampTypeList.SpanExcluding(_T("\n")));
		}

		SetDatum(m_pFldProp->GetIDIndex(SF_TYPE_XFI), tempCell);
};


//////////////////////////////////////////////////////////////////////
// Load sample from the ACC 
//////////////////////////////////////////////////////////////////////

long CLucSamp::ReadACC(CACCMgr * pACC, int iSample, CAssayDef* pAssayDef)
{	TRACE3("CLucSamp::ReadACC: pACC==%u iSample==%d pAssayDef==%u\n", pACC, iSample, pAssayDef);

    int num = m_pFldProp->GetFieldCount();
	for (int iField = 0; iField < num; iField++)
	{
		long fieldID = m_pFldProp->GetIndexID(iField);
		long fieldIDDataType = UG_NODATA;
		COleVariant val;
		CUGCell cell;

		// Load cell from existing data
		GetDatum(iField, &cell);

		// For each field, attempt to read
		long rv = pACC->GetSampleData(iSample, fieldID, &val);
TRACE1("pACC->GetSampleData returned %ld\n", rv);
		if (rv == KO_OK)
		{
			CString txt;

			// Good data
			switch (fieldID)
			{
			case SF_TYPE_XFI:
TRACE1("CLucSamp::ReadACC: case SF_TYPE_XFI : fieldID == %ld\n", fieldID);
				// Sample Type field must be converted from an ID to
				// text using the NamedIDArray provided			
				val.ChangeType(VT_I4); // Coerce to long if necessary
				ASSERT(pAssayDef != NULL);
				txt = pAssayDef->IDGetSampName(val.lVal);
				cell.SetText(txt);
				break;

			case SF_STATUS:
TRACE1("CLucSamp::ReadACC: case SF_STATUS: fieldID == %ld\n", fieldID);
				// Sample status field must be converted from an ID to
				// text using the NamedIDArray provided			
				val.ChangeType(VT_I4); // Coerce to long if necessary
				txt = ((CLuceptApp*)AfxGetApp())->m_sampStats.GetNameFromID(val.lVal);
				cell.SetText(txt);
				break;

			default:
TRACE1("CLucSamp::ReadACC: case default : fieldID==%d\n", fieldID);
				// Use standard conversion
				fieldIDDataType = m_pFldProp->GetIDDataType(fieldID);
TRACE1("CLucSamp::ReadACC: case default : fieldIDDataType==%ld\n", fieldIDDataType);
				rv = VariantToCell(cell, val, fieldIDDataType);
TRACE1("CLucSamp::ReadACC: VariantToCell returned %ld\n", rv);
			}
		}
		else if (rv == KO_ACC_BAD_FIELD_ID || rv == KO_ACC_VALUE_NOT_DEFINED)
		{	// Not an output field for the ACC or
			// Field data hasn't been set
			// Ignore either
			rv = KO_OK;
		}

		if (rv != KO_OK)
		{
TRACE1("CLucSamp::ReadACC: returning %ld\n", rv);
			return rv;
		}

		// Write updated cell data back to sample
		SetDatum(iField, cell);
	}//end for

	VERIFY(UpdateStatus());

	return KO_OK;
}


//////////////////////////////////////////////////////////////////////
// Write sample to the ACC 
//////////////////////////////////////////////////////////////////////

long CLucSamp::WriteACC(CACCMgr* pACC, CAssayDef* pAssayDef)
{	TRACE0("CLucSamp::WriteACC\n");

    int num = m_pFldProp->GetFieldCount();
	for (int iField = 0; iField < num; iField++)
	{
		long rv = KO_OK;
		long fieldID = m_pFldProp->GetIndexID(iField);
		COleVariant val;
		CUGCell cell;

		// Load cell from existing data
		GetDatum(iField, &cell);

		// Get LCC data type
		VARTYPE vt = sampFieldVT[fieldID];	
		val.vt = vt;

		// Translate as necessary per sample field
		switch (fieldID)
		{
		case SF_TYPE_XFI:
			{	// Sample Type field must be converted from text
				// to an ID using the NamedIDArray provided
				CString txt;
				cell.GetText(&txt);
				ASSERT(pAssayDef != NULL);
				val = pAssayDef->SampNameGetID(txt);
				// Assignment sets type, but should set to proper type
				ASSERT(val.vt == vt);
			}
			break;

		case SF_STATUS:
			{
				// Sample Type field must be converted from text
				// to an ID using the NamedIDArray provided
				CString txt;
				cell.GetText(&txt);
				val = ((CLuceptApp*)AfxGetApp())->m_sampStats.GetIDFromName(txt);
				// Assignment sets type, but should set to proper type
				ASSERT(val.vt == vt);
			}
			break;

		default:
			rv = CellToVariant(val, cell, m_pFldProp->GetIDDataType(fieldID));
		} 
			
		// Check for errors formatting data
		if (rv != KO_OK)
		{
			AfxMessageBox(_T("Error in data translation"));
			return rv;
		}

		// Write data to LCC
		rv = pACC->SetNewSampleData(fieldID, val);
		if (rv == KO_ACC_BAD_INPUT_FIELD)
		{	// Ignore rejection of unwanted fields
			rv = KO_OK;
		}

		if (rv != KO_OK)
		{
			return rv;
		}
	}

	// All acceptable fields have been written
	return pACC->FinishNewSample();
}

long CLucSamp::GetSStatID()
{
	CUGCell cell;
	CString text;

	// Get status string
	GetDatum(m_pFldProp->GetIDIndex(SF_STATUS), &cell);
	cell.GetText(&text);

	// Translate to an ID
	return ((CLuceptApp*)AfxGetApp())->m_sampStats.GetIDFromName(text);
}

bool CLucSamp::UpdateStatus(long sstatID)
{	TRACE0("CLucSamp::UpdateStatus()\n");

	// Color cells based on status and result 
	CUGCell cell;	
	CString result;
	CString type;
	long	attitude;
	HFONT	hFont = NULL;

	if (sstatID != LUC_ID_UNDEF)
	{	// Set new status first
		GetDatum(m_pFldProp->GetIDIndex(SF_STATUS), &cell);
		cell.SetText(((CLuceptApp*)AfxGetApp())->m_sampStats.GetNameFromID(sstatID));
		SetDatum(m_pFldProp->GetIDIndex(SF_STATUS), cell);
	}

	// Get status ID of sample
	sstatID = GetSStatID();
	if (sstatID == LUC_ID_UNDEF)
	{
		return false;
	}

	// Get attitude of sample
	GetDatum(m_pFldProp->GetIDIndex(SF_ATTITUDE), &cell);
	cell.GetNumber(&attitude);

	// Set Bold font for sample if positive or error
	if (attitude == LUC_TUDE_DISGUSTED || attitude == LUC_TUDE_ENRAGED)
	{
		hFont = m_pFldProp->GetBoldFont();
	}
	// else hFont == NULL yields default font

	// Update cells
	int num = m_pFldProp->GetFieldCount();
	for (int iField = 0; iField < num; iField++)
	{
		COLORREF color;

		// Read existing cell contents
		GetDatum(iField, &cell);

		if (m_pFldProp->GetIndexReadOnly(iField, sstatID))
		{
			cell.SetReadOnly(true);

			// Read only cells
			switch(sstatID)
			{
			case LUC_SSTAT_NEW:
			case LUC_SSTAT_CHECKED:
			case LUC_SSTAT_COMMITTED:
				color = LUC_COLOR_RO_COMA;
				break;

			case LUC_SSTAT_READING:
			case LUC_SSTAT_READ:
				color = LUC_COLOR_RO_DELIGHTED;
				break;

			case LUC_SSTAT_DEFCAL:		
			case LUC_SSTAT_VIRTUAL:		
				color = LUC_COLOR_RO_COMA;
				break;

			case LUC_SSTAT_DONE:
				switch (attitude)
				{
				case LUC_TUDE_DELIGHTED:
					color = LUC_COLOR_RO_DELIGHTED;
					break;
				case LUC_TUDE_SATIFIED:
					color = LUC_COLOR_RO_SATIFIED;
					break;
				case LUC_TUDE_APATHETIC:
					color = LUC_COLOR_RO_APATHETIC;
					break;
				case LUC_TUDE_DISGUSTED:
					color = LUC_COLOR_RO_DISGUSTED;
					break;
				case LUC_TUDE_ENRAGED:
					color = LUC_COLOR_RO_ENRAGED;
					break;
				case LUC_TUDE_COMA:
				default:
					color = LUC_COLOR_RO_COMA;
				}
				break;

			case LUC_SSTAT_STOPPED:
			case LUC_SSTAT_ABORTED:
			case LUC_SSTAT_FSTOP:
				color = LUC_COLOR_RO_ENRAGED;
				break;

			case LUC_SSTAT_UNCAL:
				color = LUC_COLOR_RO_APATHETIC;
				break;

			default:
				color = LUC_COLOR_RO_COMA;
			}
		}
		else
		{
			cell.SetReadOnly(false);

			// Read only cells
			switch(sstatID)
			{
			case LUC_SSTAT_NEW:
			case LUC_SSTAT_CHECKED:
			case LUC_SSTAT_COMMITTED:
				color = LUC_COLOR_RW_COMA;
				break;

			case LUC_SSTAT_READING:
			case LUC_SSTAT_READ:
				color = LUC_COLOR_RW_DELIGHTED;
				break;

			case LUC_SSTAT_DEFCAL:		
			case LUC_SSTAT_VIRTUAL:		
				color = LUC_COLOR_RW_COMA;
				break;

			case LUC_SSTAT_DONE:
				switch (attitude)
				{
				case LUC_TUDE_DELIGHTED:
					color = LUC_COLOR_RW_DELIGHTED;
					break;
				case LUC_TUDE_SATIFIED:	
					color = LUC_COLOR_RW_SATIFIED;
					break;
				case LUC_TUDE_APATHETIC:	
					color = LUC_COLOR_RW_APATHETIC;
					break;
				case LUC_TUDE_DISGUSTED:	
					color = LUC_COLOR_RW_DISGUSTED;
					break;
				case LUC_TUDE_ENRAGED:	
					color = LUC_COLOR_RW_ENRAGED;
					break;
				case LUC_TUDE_COMA:
				default:
					color = LUC_COLOR_RW_COMA;
				}
				break;

			case LUC_SSTAT_STOPPED:
			case LUC_SSTAT_ABORTED:
			case LUC_SSTAT_FSTOP:
				color = LUC_COLOR_RW_ENRAGED;
				break;

			case LUC_SSTAT_UNCAL:
				color = LUC_COLOR_RW_APATHETIC;
				break;


			default:
				color = LUC_COLOR_RO_COMA;
			}
		}
		
		// Update cell
		cell.SetBackColor(color);
		cell.SetFont(hFont);
		SetDatum(iField, cell);

	} // End of for (...)

	return true;
}

