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

************************************************************************************/

// Batch.cpp: implementation of the CBatch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Batch.h"
#include "Lucept.h"
#include "resource.h"
#include "LuceErr.h"
#include "CugWkl.h"
#include "ultgrid/Ugdao.h"
#include "WklDoc.h"
#include "DaoUtils.h"

#include "GXBatchGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CBatch, CObject, BATCH_SER_SCHEMA)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBatch::CBatch()
{
	// Initialize variables
	m_curSample = LUC_SAMP_UNDEF;
	m_status = LUC_BSTAT_NEW;
	m_iDataSource = LUC_ID_UNDEF;
	m_chainPos = LUC_ID_UNDEF;
	m_pGrid = NULL;
	m_pBatchGrid = NULL;
	m_iMultiBatch = -1;
	m_mSec = 0;
	m_reagentP = 0;
	m_reagent1 = 0;
	m_reagent2 = 0;
	m_reagent3 = 0;
	m_allowMerge = TRUE;
}

CBatch::~CBatch()
{
}

//////////////////////////////////////////////////////////////////////
// Serialization
//////////////////////////////////////////////////////////////////////

void CBatch::Serialize( CArchive& archive)
{
	CObject::Serialize( archive );

	if (archive.IsStoring())
	{	// Always stores the latest version
		archive << BATCH_MAGIC_INT;
		archive << BATCH_SER_SCHEMA;

		// The following is common to version 1
		archive << m_assayName;

		// Version 2 is the same as version 3 but with Reagent P added.

		// Don't save a status of checked, revert to new
		if (m_status == LUC_BSTAT_CHECKED)
		{
			archive << LUC_BSTAT_NEW;
		}
		else
		{
			archive << m_status;
		}

		archive << m_curSample;
		archive << m_chainPos;
		archive << m_allowMerge;
		archive << m_iMultiBatch;
		archive << m_mSec;
		archive << m_reagentP;
		archive << m_reagent1;
		archive << m_reagent2;
		archive << m_reagent3;

		if (m_status > LUC_BSTAT_CHECKED) 
		{
			m_assayDef.Serialize(archive);
			m_measDef.Serialize(archive);
			m_calValList.Serialize(archive);
		}

		m_fldProp.Serialize(archive);
		m_sampList.Serialize(archive);
		m_sideHeaderList.Serialize(archive);
	}
	else	// IsLoading
	{
		UINT fileVersion;
		UINT batchVersion;

		// Get file version from parent document
		CWklDoc* pDoc = (CWklDoc*)archive.m_pDocument;
		ASSERT_VALID(pDoc);
		fileVersion = pDoc->GetFileVersion();
		ASSERT(fileVersion != (UINT)-1);

		if (fileVersion > 1)
		{
			int magic;

			archive >> magic;
			if (magic != BATCH_MAGIC_INT)
			{
				AfxThrowArchiveException(CArchiveException::badClass);
			}
			archive >> batchVersion;
		}
		else
		{
			batchVersion = 1;
		}


		switch (batchVersion)
		{
		case BATCH_SER_SCHEMA:
			{
				archive >> m_assayName;
				archive >> m_status;	
				archive >> m_curSample;
				archive >> m_chainPos;
				archive >> m_allowMerge;
				archive >> m_iMultiBatch;
				archive >> m_mSec;
				archive >> m_reagentP;
				archive >> m_reagent1;
				archive >> m_reagent2;
				archive >> m_reagent3;

				// Changed order to read assay before fldProp before
				// sample list so sample type list could be properly
				// initialized
				// Serialize Assay and Measure Defs if appropriate
				// Also calibration values
				if (m_status > LUC_BSTAT_CHECKED)
				{
					m_assayDef.Serialize(archive);
					m_measDef.Serialize(archive);
					m_calValList.Serialize(archive);
				}
				m_fldProp.Serialize(archive);
				CAssayDef* pAssay;
				GetAssayPtr(&pAssay);
				m_fldProp.SetSampTypeList(pAssay);

				// Set properties for samples to be created from serialization
				CLucSamp::RegDefFieldProp(&m_fldProp);

				m_sampList.Serialize(archive);
				m_sideHeaderList.Serialize(archive);
			}
			break;
		case 2:
			{
				archive >> m_assayName;
				archive >> m_status;	
				archive >> m_curSample;
				archive >> m_chainPos;
				archive >> m_allowMerge;
				archive >> m_iMultiBatch;
				archive >> m_mSec;
				archive >> m_reagent1;
				archive >> m_reagent2;
				archive >> m_reagent3;

				// Changed order to read assay before fldProp before
				// sample list so sample type list could be properly
				// initialized
				// Serialize Assay and Measure Defs if appropriate
				// Also calibration values
				if (m_status > LUC_BSTAT_CHECKED)
				{
					m_assayDef.Serialize(archive);
					m_measDef.Serialize(archive);
					m_calValList.Serialize(archive);
				}
				m_fldProp.Serialize(archive);
				CAssayDef* pAssay;
				GetAssayPtr(&pAssay);
				m_fldProp.SetSampTypeList(pAssay);

				// Set properties for samples to be created from serialization
				CLucSamp::RegDefFieldProp(&m_fldProp);

				m_sampList.Serialize(archive);
				m_sideHeaderList.Serialize(archive);
			}
			break;
		case 1:
			{
				archive >> m_assayName;
				archive >> m_status;	
				archive >> m_curSample;
				archive >> m_chainPos;
				archive >> m_iMultiBatch;
				archive >> m_mSec;
				archive >> m_reagent1;
				archive >> m_reagent2;
				archive >> m_reagent3;
				m_fldProp.Serialize(archive);

				// Set properties for samples to be created from serialization
				if(archive.IsLoading())
				{
					CLucSamp::RegDefFieldProp(&m_fldProp);
				}

				m_sampList.Serialize(archive);
				m_sideHeaderList.Serialize(archive);

				// Serialize Assay and Measure Defs if appropriate
				// Also calibration values
				if (m_status >= LUC_BSTAT_CHECKED)
				{
					m_assayDef.Serialize(archive);
					m_measDef.Serialize(archive);
					m_calValList.Serialize(archive);
				}
			}
			break;

		default:
			AfxThrowArchiveException(CArchiveException::badSchema);
		}

		// Modify batch based if loaded as a template
		if (pDoc->GetCopyType() == TEMPLATE_COPY)
		{
			m_status = LUC_BSTAT_NEW;	
			m_chainPos = LUC_ID_UNDEF;
			m_iMultiBatch = -1;
			m_mSec = 0;
			m_reagentP = 0;
			m_reagent1 = 0;
			m_reagent2 = 0;
			m_reagent3 = 0;

			m_calValList.DeleteAll();

			// Init samp type list from APPLICATIONS's assay def
			CAssayDef* pAssay;
			GetAssayPtr(&pAssay);
			m_fldProp.SetSampTypeList(pAssay);

			// Set properties for samples to be created from serialization

		} // end of template copy

	}// end of Loading
}

//////////////////////////////////////////////////////////////////////
// GetCell
//////////////////////////////////////////////////////////////////////

#define UG_NO_SUCH_CELL	1

int CBatch::GetCell(int iCol, long sample, CUGCell* cell)
{
	// Check for a valid cell
	if (sample >= GetNumSamples() || sample < 0)
	{
		ASSERT(FALSE);
		return UG_NO_SUCH_CELL;
	}
	// Check for valid column
	if (iCol >= m_fldProp.GetColumnCount())
	{
		ASSERT(FALSE);
		return UG_NO_SUCH_CELL;
	}
	
	// Load return cell
	if (iCol != GRID_SIDEHEADING)
	{	
		// Get field index from column
		int iField = m_fldProp.GetColIndex(iCol);
		if (iField >= GetNumFields() || iField < 0)
		{
			ASSERT(FALSE);
			return UG_NO_SUCH_CELL;
		}
		return m_sampList[sample].GetDatum(iField, cell);
	}
	else
	{	// Is a side header
		return m_sideHeaderList[sample].CopyInfoTo(cell);
	}
}

//////////////////////////////////////////////////////////////////////
// SetCell
//////////////////////////////////////////////////////////////////////

int CBatch::SetCell(int iCol, long sample, CUGCell* cell)
{
	// Check for valid column
	if (iCol >= m_fldProp.GetColumnCount())
	{
		ASSERT(FALSE);
		return UG_NO_SUCH_CELL;
	}
	
	// Check for valid sample
	if (sample < 0)
	{
		ASSERT(FALSE);
		return UG_NO_SUCH_CELL;
	}

	// Create new samples if necessary
	while (sample >= GetNumSamples())
	{
		m_sampList.Append();
		m_sideHeaderList.Append();
	}

	// Put data into cell
	if (iCol != GRID_SIDEHEADING)
	{	// Not a side header cell
		// Get field index from column
		int iField = m_fldProp.GetColIndex(iCol);
		if (iField >= GetNumFields() || iField < 0)
		{
			ASSERT(FALSE);
			return UG_NO_SUCH_CELL;
		}
		return m_sampList[sample].SetDatum(iField, *cell);
	}
	else
	{	// Is a side header
		return m_sideHeaderList[sample].CopyInfoFrom(cell);
	}
}

//////////////////////////////////////////////////////////////////////
// Append Sample
//////////////////////////////////////////////////////////////////////

long CBatch::AppendSample()
{
	if (m_status != LUC_BSTAT_NEW)
	{
		return KO_LUC_BATCH_READ_ONLY;
	}

	// Append Sample with proper field properties
	CLucSamp::RegDefFieldProp(&m_fldProp);
	m_sampList.Append();
	m_sampList[m_sampList.GetCount() - 1].UpdateProperties();

	int nRet;
#ifdef DO_UG

	// Add new side header with index 
	CUGCell	cell;
	CUGCell *pCell;
	TCHAR string[20];

	m_pGrid->GetHeadingDefault(&cell);
	_stprintf(string, _T("%d"), GetNumSamples());
	cell.SetText(string);
	pCell = m_sideHeaderList.Append();
	ASSERT(pCell != NULL);

	m_curSample = GetNumSamples() - 1;
	if (m_pGrid != NULL)
	{
		m_pGrid->SetNumberRows(GetNumSamples());
	}

	nRet = cell.CopyInfoTo(pCell);
#else
	m_curSample = GetNumSamples() - 1;
#endif

#ifdef DO_CGX
	nRet = KO_OK;
#endif
	return nRet;
};


//////////////////////////////////////////////////////////////////////
// Insert new Sample at specified index
//////////////////////////////////////////////////////////////////////

long CBatch::InsertSample(int index)
{
	if (m_status != LUC_BSTAT_NEW)
	{
		return KO_LUC_BATCH_READ_ONLY;
	}

	if (index < 0 || index > m_sampList.GetCount())
	{
		return KO_LUC_BAD_SAMP_INDEX;
	}

	// Create Sample with proper field properties
	CLucSamp::RegDefFieldProp(&m_fldProp);
	m_sampList.CreateAt(index);
	m_sampList[index].UpdateProperties();

	int nRet;
#ifdef DO_UG

	// Add new side header with index 
	CUGCell	cell;
	CUGCell *pCell;
	TCHAR string[20];

	m_pGrid->GetHeadingDefault(&cell);
	_stprintf(string, _T("%d"), GetNumSamples());
	cell.SetText(string);
	pCell = m_sideHeaderList.Append();
	ASSERT(pCell != NULL);

	m_curSample = index;
	if (m_pGrid != NULL)
	{
		m_pGrid->SetNumberRows(GetNumSamples());
	}

	nRet = cell.CopyInfoTo(pCell);
#else
	m_curSample = index;
#endif

#ifdef DO_CGX
	nRet = KO_OK;
#endif
	return nRet;
}

//////////////////////////////////////////////////////////////////////
// Save newly selected assay
//////////////////////////////////////////////////////////////////////

bool CBatch::SetAssayName(CString& name)
{ 
	CAssayDef* pAssayDef;

	if (!name.IsEmpty())
	{
		// Get pointer to assay definition
		 pAssayDef = ((CLuceptApp*)AfxGetApp())->m_assays.GetPtrFromName(name);
		if (pAssayDef == NULL)
		{	// Not a valid name
			m_assayName.Empty();
			m_fldProp.SetSampTypeList(NULL);
			return false;
		}
		else
		{	//batch takes default props f. its assay
			ASSERT(pAssayDef->GetFldPropPtr());
			SetFldProp(*pAssayDef->GetFldPropPtr());
		}
	}
	else
	{	// Purposely clearing assay name
		pAssayDef = NULL;
	}

	// Save new name
	m_assayName = name;                  //!!!could be null

	// Change sample type drop list 
	m_fldProp.SetSampTypeList(pAssayDef);//!!!could be null

	// And update all sample cells with new list
	int num = m_sampList.GetCount();
	for (int i = 0; i < num; i++)
	{
		m_sampList[i].UpdateSampType();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// Initialize grid from batch 
//////////////////////////////////////////////////////////////////////
	
int CBatch::RegisterGrid(CugWkl* pGrid)
{
	CUGCell	cell;

	if (pGrid != NULL)
	{	// New grid to register
		m_iDataSource = pGrid->AddDataSource(this);
		pGrid->SetDefDataSource(m_iDataSource);
		m_pGrid = pGrid;
	}
	// else just re-initialize the same grid

	if (m_pGrid == NULL)
	{
		return LUC_ID_UNDEF;
	}

#ifdef DO_UG
	m_pGrid->SetGridUsingDataSource(m_iDataSource);

	long iRow;
	int iCol;

	// Get handle for Bold font from Grid
	m_fldProp.SetBoldFont(m_pGrid->m_boldFont);

	// Add sample numbers to side headers
	// And Set colors/fonts 
	m_pGrid->GetHeadingDefault(&cell);
	int num = m_pGrid->GetNumberRows();
	for (iRow = 0; iRow < num; iRow++)
	{
		TCHAR string[20];

		_stprintf(string, _T("%d"), iRow + 1);
		cell.SetText(string);
		m_pGrid->SetCell(GRID_SIDEHEADING, iRow, &cell);

		// Colors/Fonts based on sample status
		m_sampList[iRow].UpdateStatus();
	}

	// Set corner button text
	CString text;
	text.LoadString(IDS_SF_SAMPNUM);
	m_pGrid->GetHeadingDefault(&cell);
	cell.SetText(text);
	m_pGrid->SetCell(-1, -1, &cell);
	
	// Set column widths
	num = m_fldProp.GetColumnCount();
	for (iCol = 0; iCol < num; iCol++)
	{
		m_pGrid->SetColWidth(iCol, m_fldProp.GetColWidth(iCol));
	}

	// Set current row 
	if (m_curSample >= 0)
	{
		m_pGrid->GotoRow(m_curSample);
	}
#endif

#if DO_CGX	
	// UG cell will have this value if it should be shown in bold: use it
	// as a flag.
	m_fldProp.SetBoldFont( (HFONT)INVALID_HANDLE_VALUE);
#endif

	return m_iDataSource;
}

int CBatch::RegisterCGXGrid(CGXBatchGrid* pBatchGrid)
{
	long iRow;
	int iCol;
	CUGCell	cell;

	if( pBatchGrid != NULL) {
		m_pBatchGrid = pBatchGrid;
	}

	if( m_pBatchGrid == NULL)
	{
		return LUC_ID_UNDEF;
	}

	// Get handle for Bold font from Grid

	// Add sample numbers to side headers
	// And Set colors/fonts 

	int num = m_pBatchGrid->GetRowCount();
	
	for (iRow = 0; iRow < num; iRow++)
	{
		TCHAR string[20];

		_sntprintf(string, sizeof(string), _T("%d"), iRow + 1);

		// Colors/Fonts based on sample status
		m_sampList[iRow].UpdateStatus();
	}

	// Set corner button text
	CString text;
	text.LoadString(IDS_SF_SAMPNUM);
	m_pBatchGrid->SetValueRange( CGXRange( 0, 0), text);
	
	// Set column widths
	num = m_fldProp.GetColumnCount();
	for (iCol = 0; iCol < num; iCol++)
	{
		GetColName( iCol, &text);
		m_pBatchGrid->SetColWidth( iCol + 1, iCol + 2, m_fldProp.GetColWidth(iCol));
	}

	/*
	 * Set Sample column to fixed 80.
	 * Old grid didn't store this columns width.
	 */
	m_pBatchGrid->SetColWidth( 0, 1, 80);

	// Set current row 
	if (m_curSample >= 0)
	{
//?		m_pGrid->GotoRow(m_curSample);
	}

	return KO_OK;
}


//////////////////////////////////////////////////////////////////////
// Set Current sample returns TRUE for valid sample
//////////////////////////////////////////////////////////////////////
	
BOOL CBatch::SetCurSample(long samp)
{ 
	if (samp >= 0  && samp < m_sampList.GetCount()) 
	{
		m_curSample = samp;
		return TRUE;
	}
	else
	{
		m_curSample = LUC_ROW_UNDEF;
		return FALSE;
	}
}


CString CBatch::GetBatchName(int iBatch)
{
	CString name;
	CString prefix;

	ASSERT(iBatch >= 0);
	VERIFY(prefix.LoadString(IDS_BATCH_NAME_PREFIX));

	name = prefix + GetBatchLetter(iBatch);

	return name;
}


CString CBatch::GetBatchLetter(int iBatch)
{
	CString letter;
	CString symbols;

	ASSERT(iBatch >= 0);
	VERIFY(symbols.LoadString(IDS_BATCH_NAME_SYMBOLS));

	int	radix = symbols.GetLength();
	ASSERT(radix > 0);

	do
	{
		int remainder = iBatch % radix;

		iBatch /= radix;
		letter += symbols[remainder];
	}
	while (iBatch-- > 0);
	// The -- addresses the fact that A is not exactly
	// 0 and not exactly 1

	letter.MakeReverse();

	return letter;
}

int CBatch::LetterGetBatchIndex(LPCTSTR letters)
{
	CString lets(letters);
	CString symbols;

	VERIFY(symbols.LoadString(IDS_BATCH_NAME_SYMBOLS));

	int	radix = symbols.GetLength();
	ASSERT(radix > 0);

	// trim any white space
	lets.TrimLeft();
	lets.TrimRight();

	int letsLen = lets.GetLength();
	if (letsLen == 0)
	{
		return LUC_BATCH_UNDEF;
	}

	// Get rightmost char
	int index = symbols.Find(lets[--letsLen]);
	while (letsLen > 0)
	{
		int next = symbols.Find(lets[--letsLen]);
		if (next == -1)
		{
			return LUC_BATCH_UNDEF;
		}

		index += (next + 1) * radix;
	}

	return index;
}

////////////////////////////////////////////////////////////////////////
// all qunatities except link are 0 based
////////////////////////////////////////////////////////////////////////

int CBatch::GetTubeNumber(int iSamp, int iRepl)
{
	ASSERT(m_chainPos != LUC_ID_UNDEF);
	ASSERT(iSamp >= 0 && iSamp < m_sampList.GetCount());

	int link;
	VERIFY(m_sampList[iSamp].GetField(SF_POSITION, &link) == KO_OK);

	return link - m_chainPos + iRepl;
}


long CBatch::GetSampRepl(int iTube, int* piSamp, int* piRepl)
{
	// All indices are zero based
	if (iTube < 0)
		return KO_LUC_BAD_TUBE_INDEX;

	int numSamp = m_sampList.GetCount();
	for (*piSamp = 0; *piSamp < numSamp; (*piSamp)++)
	{	// Check if this sample is real
		if (!m_sampList[*piSamp].IsVirtual())
		{	// Get replicates for this sample
			int sampRepls;
			m_sampList[*piSamp].GetField(SF_REPLS, &sampRepls);
			ASSERT(sampRepls >= 1 && sampRepls <= 10);

			if (iTube < sampRepls)
			{	// In this sample
				*piRepl = iTube;
				break;
			}
			else
			{	// Deduct this sample's tubes
				iTube -= sampRepls;
			}
		}
	}

	if (*piSamp >= numSamp)
	{	// No such tube
		*piSamp = -1;
		*piRepl = -1;
		return KO_LUC_BAD_TUBE_INDEX;
	}

	return KO_OK;
}

long CBatch::DeleteSample(long iSamp)
{
	if (m_status != LUC_BSTAT_NEW)
	{
		return KO_LUC_BATCH_READ_ONLY;
	}

	if (iSamp == LUC_ROW_UNDEF)
	{
		iSamp = m_curSample;
	}

	if (iSamp < 0 || iSamp >= m_sampList.GetCount())
	{
		return KO_LUC_BAD_SAMPLE_INDEX;
	}
	
	m_sampList.DeleteItem(iSamp);
	if( m_sideHeaderList.GetCount() > 0) {
		m_sideHeaderList.DeleteItem(m_sideHeaderList.GetCount() - 1);
	}

	// Notify grid
#ifdef DO_UG
	if (m_pGrid != NULL)
	{
		m_pGrid->SetNumberRows(GetNumSamples());
	}
#endif

#ifdef DO_CGX
#endif

	if (m_sampList.GetCount() == 0)
	{
		m_curSample = LUC_ROW_UNDEF;
	}
	else if	(m_curSample >= m_sampList.GetCount())
	{
		m_curSample = m_sampList.GetCount() - 1;
	}
	
	return KO_OK;
}

bool CBatch::SetFldProp(CFieldProperties & source)
{
	// Don't let the current column become invalid!
#ifdef DO_UG
	if (m_pGrid != NULL)
	{
		m_pGrid->GotoCol(0);
	}
#endif	
	// Load new properties
	m_fldProp = source;

	// Update all samples.  Since the samples hold a pointer
	// to the field properties we don't need to pass one
	int num = m_sampList.GetCount();
	for (int i = 0; i < num; i++)
	{
		m_sampList[i].UpdateProperties();
	}

	// Re-initialize the grid
#ifdef DO_UG
	RegisterGrid(NULL);
#endif
#ifdef DO_CGX
	RegisterCGXGrid(NULL);
#endif	
	return true;
}

int CBatch::GetNumTubes()
{
	// Sum of replicates for all samples which are not virtual
	// or default cal types
	int nTubes = 0;
	int num = GetNumSamples();
	for (int i = 0; i < num; i++) 
	{
		int sampVStat;
		m_sampList[i].GetField(SF_VIRTUAL, &sampVStat);
		if (sampVStat == LUC_VSTAT_REAL)
		{
			CUGCell cell;
			m_sampList[i].GetField(SF_REPLS, &cell);
			nTubes += (int)cell.GetNumber();
		}
	}

	return nTubes;
}

long CBatch::GetAssayID() 
{ 
	if (m_assayName.IsEmpty())
	{
		return LUC_ID_UNDEF;
	}

	return ((CLuceptApp*)AfxGetApp())->	
		m_assays.GetIDFromName(m_assayName);
}


long CBatch::GetAssayPtr(CAssayDef** ppAssay)
{
	if (m_status >= LUC_BSTAT_CHECKED)
	{	// Use embedded assay definition
		*ppAssay = &m_assayDef;
		return KO_OK;
	}

	if (m_assayName.IsEmpty())
	{
		*ppAssay = NULL;
		return KO_LUC_BATCH_NO_ASSAY;
	}

	*ppAssay = ((CLuceptApp*)AfxGetApp())->
		m_assays.GetPtrFromName(m_assayName);
	if (*ppAssay == NULL)
	{
		return KO_LUC_BATCH_BAD_ASSAY;
	}

	return KO_OK;
}


long CBatch::GetMeasPtr(CMeasDef** ppMeas)
{
	CAssayDef* pAssay;
	return GetAssayMeasPtr(&pAssay, ppMeas);
}

CString CBatch::GetMeasName()
{
	CString	measName;
	CMeasDef*	pMeas;
	if (GetMeasPtr(&pMeas) == KO_OK)
	{
		ASSERT(pMeas != NULL);
		measName = pMeas->GetName();
	}
	
	// Returns blank if error
	return measName;
}

long CBatch::GetAssayMeasPtr(CAssayDef** ppAssay, CMeasDef** ppMeas)
{
	// Get assay pointer
	long rv = GetAssayPtr(ppAssay);
	if (rv != KO_OK)
	{
		*ppMeas = NULL;
		return rv;
	}
	ASSERT(*ppAssay != NULL);

	if (m_status >= LUC_BSTAT_CHECKED)
	{	// Use embedded measurement definition
		*ppMeas = &m_measDef;
		return KO_OK;
	}

	// Get measurement pointer
	*ppMeas = ((CLuceptApp*)AfxGetApp())->
		m_meass.GetPtrFromName((*ppAssay)->GetMeasName());
	if (*ppMeas == NULL)
	{
		return KO_LUC_BATCH_BAD_MEAS;
	}

	return KO_OK;
}


long CBatch::SetStatus(long status)
{ 
	switch (m_status)
	{
	case LUC_BSTAT_NEW:
		if (status == LUC_BSTAT_CHECKED)
		{	// Save Assay parameters in batch
			CAssayDef* pAssay;
			CMeasDef* pMeas;
			long rv = GetAssayMeasPtr(&pAssay, &pMeas);
			if (rv != KO_OK)
			{
				return rv;
			}
			m_assayDef = *pAssay;
			m_measDef = *pMeas;

			// Update status
			m_status = LUC_BSTAT_CHECKED;
			return KO_OK;
		}
		else
		{
			return KO_LUC_BSTAT_BAD_CHANGE;
		}

	case LUC_BSTAT_CHECKED:
		switch (status)
		{
		case LUC_BSTAT_NEW:
		case LUC_BSTAT_COMMITTED:
		case LUC_BSTAT_READING:
			m_status = status;
			return KO_OK;

		// To re-check, change status to new and make recursive call
		case LUC_BSTAT_CHECKED:
			m_status = LUC_BSTAT_NEW;
			return SetStatus(LUC_BSTAT_CHECKED);

		default:
			return KO_LUC_BSTAT_BAD_CHANGE;
		}


	case LUC_BSTAT_COMMITTED:
		switch (status)
		{
		case LUC_BSTAT_READING:
		case LUC_BSTAT_STOPPED:
		case LUC_BSTAT_ABORTED:
		case LUC_BSTAT_UNCAL:
		case LUC_BSTAT_FSTOP:
			m_status = status;
			return KO_OK;
		case LUC_BSTAT_NEW:
		case LUC_BSTAT_CHECKED:
		case LUC_BSTAT_COMMITTED:
		case LUC_BSTAT_READ:
		case LUC_BSTAT_DONE:
		default:
			return KO_LUC_BSTAT_BAD_CHANGE;
		}
		break;
	case LUC_BSTAT_READING:
		switch (status)
		{
		case LUC_BSTAT_READ:
		case LUC_BSTAT_DONE:
		case LUC_BSTAT_STOPPED:
		case LUC_BSTAT_ABORTED:
		case LUC_BSTAT_UNCAL:
		case LUC_BSTAT_FSTOP:
			m_status = status;
			return KO_OK;
		case LUC_BSTAT_NEW:
		case LUC_BSTAT_CHECKED:
		case LUC_BSTAT_COMMITTED:
		case LUC_BSTAT_READING:
		default:
			return KO_LUC_BSTAT_BAD_CHANGE;
		}
		break;

	case LUC_BSTAT_READ:
		if (status == LUC_BSTAT_DONE ||
			status == LUC_BSTAT_UNCAL ||
			status == LUC_BSTAT_FSTOP	)
		{	// A read batch can be calculated and become done
			m_status = status;
			return KO_OK;
		}
		else
		{
			return KO_LUC_BSTAT_BAD_CHANGE;
		}

	case LUC_BSTAT_DONE:
		if (status == LUC_BSTAT_UNCAL ||
			status == LUC_BSTAT_FSTOP)
		{
			m_status = status;
			return KO_OK;
		}
		return KO_LUC_BSTAT_BAD_CHANGE;

	case LUC_BSTAT_STOPPED:
		if (status == LUC_BSTAT_UNCAL)
		{
			m_status = status;
			return KO_OK;
		}
		return KO_LUC_BSTAT_BAD_CHANGE;

	case LUC_BSTAT_ABORTED:
		if (status == LUC_BSTAT_UNCAL)
		{
			m_status = status;
			return KO_OK;
		}
		return KO_LUC_BSTAT_BAD_CHANGE;

	default:
		return KO_OK;
	}

}

long CBatch::AppendCalVal(CParam & calVal)
{
	m_calValList.Append();
	m_calValList[m_calValList.GetCount() - 1] = calVal;

	return KO_OK;
}

int CBatch::GetNumCalVals()
{
	return m_calValList.GetCount();
}

CParam CBatch::GetCalVal(int iCalVal)
{
	ASSERT(iCalVal >= 0 && iCalVal < m_calValList.GetCount());
	return m_calValList[iCalVal];
}


void WriteString( CFile &oFile, LPCTSTR str)
{
	CT2CA outputString( str, CP_UTF8);
	oFile.Write( outputString, ::strlen(outputString));
}

bool CBatch::TextPortBatch(LPCTSTR fileSpec, LPCTSTR wklName, int iBatch, CWklDoc* pDoc)
{
	bool bSuccess = false;

	try
	{
		// Create file
		CFile outFile(fileSpec, CFile::modeWrite 
			| CFile::modeCreate | CFile::modeNoTruncate);

		bool bSuccess = TextPortBatch( outFile, wklName, iBatch, pDoc);

		outFile.Close();
	}
	catch( CFileException exception)
	{
		AfxMessageBox(_T("Error writing to output file!"));

		exception.Delete();
	}

	return bSuccess;
}

bool CBatch::TextPortBatch( CFile &outFile, LPCTSTR wklName, int iBatch, CWklDoc* pDoc)
{
		
	CString scratch;
	COleVariant var;
	int numCols = GetNumCols();
	int	numRows = GetNumRows();
	int iCol, iRow;
	CUGCell cell;

	try
	{
		// Write header info...

		// ...workload
		WriteString( outFile, _T("Workload:  "));
		WriteString( outFile, wklName);
		WriteString( outFile, _T("\r\n"));

		// ... batch
		WriteString( outFile, _T("Batch:  "));
		WriteString( outFile, CBatch::GetBatchLetter(iBatch));
		WriteString( outFile, _T("\r\n"));

		// ... Operator
		WriteString( outFile, _T("Operator:  "));
		WriteString( outFile, pDoc->GetCreator());
		WriteString( outFile, _T("\r\n"));

		// ... Date
		WriteString( outFile, _T("Date:  "));
		// Ole Date format Bug fix!
		// This code prevents errors when Regional date settings are
		// not English
		{
			CSetLocale l(LC_TIME);
			scratch = pDoc->GetModDate().Format(_T("%x"));
		}
		WriteString( outFile, scratch);
		WriteString( outFile, _T("\r\n"));

		// ... Assay
		WriteString( outFile, _T("Assay:  "));
		WriteString( outFile, GetAssayName());
		WriteString( outFile, _T("\r\n"));

		// ... Calibration values
		int num = m_calValList.GetCount();
		for (int i = 0; i < num; i++)
		{
			WriteString( outFile, m_calValList[i].m_name);
			var = m_calValList[i].m_value;
			var.ChangeType(VT_R8);
			scratch.Format(_T(":  %9.1f"), var.dblVal);	
			WriteString( outFile, scratch);
			WriteString( outFile, _T("\r\n"));
		}

		// Column titles
		for (iCol = 0; iCol < numCols; iCol++)
		{
			scratch = m_fldProp.GetColName(iCol);

			WriteString( outFile, scratch);
			if (iCol == numCols - 1)
			{
				WriteString( outFile, _T("\r\n"));
			}
			else
			{
				WriteString( outFile, _T(", "));
			}
		}

		// Sample values
		for (iRow = 0; iRow < numRows; iRow++)
		{
			for (iCol = 0; iCol < numCols; iCol++)
			{
				VERIFY(GetCell(iCol, iRow, &cell) == UG_SUCCESS);
				
				// This is easier and imports better
				VERIFY(cell.GetText(&scratch) == UG_SUCCESS);
				WriteString( outFile, scratch);

				if (iCol == numCols - 1)
				{
					WriteString( outFile, _T("\r\n"));
				}
				else
				{
					WriteString( outFile, _T(", "));
				}
			}
		}
	
		// End of data
		WriteString( outFile, _T("\r\n--- End of Batch ---\r\n"));
		return true;
	}
	catch( CFileException exception)
	{
		AfxMessageBox(_T("Error writing to output file!"));

		exception.Delete();
		return false;
	}
}

/**
 * Export batch info in csv format.
 */
bool CBatch::CsvPortBatch(LPCTSTR fileSpec, LPCTSTR wklName, int iBatch, CWklDoc* pDoc)
{
	bool bSuccess = false;

	try
	{
		// Create file
		CFile outFile(fileSpec, CFile::modeWrite 
			| CFile::modeCreate | CFile::modeNoTruncate);

		bool bSuccess = CsvPortBatch( outFile, wklName, iBatch, pDoc);

		outFile.Close();
	}
	catch( CFileException exception)
	{
		AfxMessageBox(_T("Error writing to output file!"));

		exception.Delete();
	}

	return bSuccess;
}

/**
 * Export batch info in csv format.
 */
bool CBatch::CsvPortBatch( CFile &outFile, LPCTSTR wklName, int iBatch, CWklDoc* pDoc)
{
		
	CString scratch;
	COleVariant var;
	int numCols = GetNumCols();
	int	numRows = GetNumRows();
	int iCol, iRow;
	CUGCell cell;

	try
	{
		// Write header info...

		// ...workload
		WriteString( outFile, _T("Workload,"));
		WriteString( outFile, wklName);
		WriteString( outFile, _T("\r\n"));

		// ... batch
		WriteString( outFile, _T("Batch,"));
		WriteString( outFile, CBatch::GetBatchLetter(iBatch));
		WriteString( outFile, _T("\r\n"));

		// ... Operator
		WriteString( outFile, _T("Operator,"));
		WriteString( outFile, pDoc->GetCreator());
		WriteString( outFile, _T("\r\n"));

		// ... Date
		WriteString( outFile, _T("Date,"));
		// Ole Date format Bug fix!
		// This code prevents errors when Regional date settings are
		// not English
		{
			CSetLocale l(LC_TIME);
			scratch = pDoc->GetModDate().Format(_T("%x"));
		}
		WriteString( outFile, scratch);
		WriteString( outFile, _T("\r\n"));

		// ... Assay
		WriteString( outFile, _T("Assay,"));
		WriteString( outFile, GetAssayName());
		WriteString( outFile, _T("\r\n"));

		// ... Calibration values
		int num = m_calValList.GetCount();
		for (int i = 0; i < num; i++)
		{
			WriteString( outFile, m_calValList[i].m_name);
			var = m_calValList[i].m_value;
			var.ChangeType(VT_R8);
			scratch.Format(_T(",%9.1f"), var.dblVal);	
			WriteString( outFile, scratch);
			WriteString( outFile, _T("\r\n"));
		}

		// Column titles
		for (iCol = 0; iCol < numCols; iCol++)
		{
			scratch = m_fldProp.GetColName(iCol);

			WriteString( outFile, scratch);
			if (iCol == numCols - 1)
			{
				WriteString( outFile, _T("\r\n"));
			}
			else
			{
				WriteString( outFile, _T(","));
			}
		}

		// Sample values
		for (iRow = 0; iRow < numRows; iRow++)
		{
			for (iCol = 0; iCol < numCols; iCol++)
			{
				VERIFY(GetCell(iCol, iRow, &cell) == UG_SUCCESS);
				
				// This is easier and imports better
				VERIFY(cell.GetText(&scratch) == UG_SUCCESS);
				WriteString( outFile, scratch);

				if (iCol == numCols - 1)
				{
					WriteString( outFile, _T("\r\n"));
				}
				else
				{
					WriteString( outFile, _T(","));
				}
			}
		}
	
		// End of data
		WriteString( outFile, _T("\r\n\r\n"));
		return true;
	}
	catch( CFileException exception)
	{
		AfxMessageBox(_T("Error writing to output file!"));

		exception.Delete();
		return false;
	}
}

int CBatch::ExportBatch(LPCTSTR fileSpec, LPCTSTR tableName, LPCTSTR strWorkloadName, int nBatch, CWklDoc *pDoc)
{
#ifdef DO_UG
	if (m_pGrid == NULL)
	{
		return 1;
	}
#endif

#ifdef DO_CGX
	if (m_pBatchGrid == NULL)
	{
		return 1;
	}
#endif

	CDaoDatabase	defDB;
	CString strTempFileName;

	if( InitDatabase( fileSpec, defDB, strTempFileName) != 0) {
		return 1;
	}

	if(DefineTable(&defDB, tableName, defDB.GetConnect()) != 0) 
	{
		AfxMessageBox(_T("CBatch::ExportBatch: error defining table"));
		return 1;
	}
	if(CreateRecordset(&defDB, tableName) != 0) 
	{
		AfxMessageBox(_T("CBatch::ExportBatch: error creating recordset"));
		return 1;
	}

	if( DefinePropertiesTable( &defDB, _T("Batch Properties"), defDB.GetConnect()) != 0) {
		AfxMessageBox(_T("CBatch::ExportBatch: error defining properties table"));
		return 1;
	}

	if( AddProperties( &defDB, _T("Batch Properties"), strWorkloadName, nBatch, pDoc) != 0) {
		AfxMessageBox(_T("CBatch::ExportBatch: error creating properties recordset"));
		return 1;
	}


	CloseDatabase( fileSpec, defDB, strTempFileName);
	
	return 0;
}

/**
 * Initialise DAO database prior to export.
 * @param fileSpec The path to the file where the data is exported.
 * @param defDB The database that is to be initialised (in/out parameter)
 * @param strTempFileName The database is created in this temporary file that is later copied to the target.
 * @returns 1 on FAILURE , 0 on success (legacy...!).
 */
int CBatch::InitDatabase( LPCTSTR fileSpec, CDaoDatabase &defDB, CString &strTempFileName)
{
	CString			ConnectStr;
	CString			temp;
	TCHAR strTempDir[MAX_PATH];

	/*
	 * Due to dao bugs with long file names, create file in temporary directory
	 * (hoping file path is not too long) and copy to target.
	 */
	GetTempPath( MAX_PATH, strTempDir);

	strTempFileName = _ttempnam( strTempDir, _T("AIM"));
	
	//find type of conversion and create connect string
	temp = fileSpec;
	if((temp.Right(4)).CompareNoCase(_T(".mdb")) == 0) 
	{	//Access format

		/*
		 * The _ttmpnam function is dumb and you cannot specify a file extension.
		 * We have to add one to get dao to as DAO will do that anyway.
		 * It is possible for a later invocation of _ttmpnam to return an existing
		 * file name since it is not looking for the file extension.
		 * Hence delete the temporary file if it exists for any reason, otherwise
		 * we get errors from trying to append to it.
		 */
		strTempFileName += _T(".mdb");
		_wremove( strTempFileName);

		try 
		{
			defDB.Create( strTempFileName); //only .mdb files can be 'Created'
		} 
		catch(CDaoException* e) 
		{
			DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::ExportBatch: Database create failed."));
			return 1;
		} 
		catch(...)
		{
			throw;
		}		
	} 
	else if ((temp.Right(4)).CompareNoCase(_T(".xls")) == 0) 
	{	//Excel format
		strTempFileName += _T(".xls");
		_wremove( strTempFileName);

		ConnectStr.Format(_T("%s;DATABASE=%s"),_T("Excel 5.0"), fileSpec); // OK ITL: Changed from 3.0
		try 
		{
			defDB.Open( strTempFileName, TRUE, FALSE, ConnectStr);
		} 
		catch(CDaoException* e) 
		{
			DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::ExportBatch: Database open failed."));
			return 1;
		} 
		catch(...)
		{
			throw;
		}	
	} 
	else 
	{
		temp.Format(_T("Export Error:  %s\r\n\r\nExport to specified file type not supported"), fileSpec); 
		AfxMessageBox(temp);
		return 1;
	}

	return 0;
}

/**
 * Close and tidy up after database export.
 * @param fileSpec The path to the file where the data is exported.
 * @param defDB The database that is to be closed (in/out parameter)
 * @param strTempFileName The database was created in this temporary file that is now copied to the target.
 * @returns 1 on FAILURE , 0 on success (legacy...!).
 */
int CBatch::CloseDatabase( LPCTSTR fileSpec, CDaoDatabase &defDB, CString &strTempFileName)
{
	try 
	{
		defDB.Close();
	} 
	catch(CDaoException *) 
	{
		return 1;
	} 

	/*
	 * Copy file to destination (movefile won't work if target is on a different volume).
	 */
	CopyFile( strTempFileName, fileSpec, FALSE);
	_wremove( strTempFileName);
	
	return 0;
}

int CBatch::DefineTable(CDaoDatabase *pDB, LPCTSTR TableName, const CString& ConnectStr)
{
	ASSERT(pDB->IsOpen());
	ASSERT(pDB->CanUpdate());

#ifdef DO_UG
	if (m_pGrid == NULL)
	{
		return 1;
	}
#endif

#ifdef DO_CGX
	if (m_pBatchGrid == NULL)
	{
		return 1;
	}
#endif

	CString temp;
	CDaoTableDef	tblDef(pDB);
	CDaoFieldInfo	fldInfo;

	//create table definition object
	try 
	{	// Fixed by CAR 887/002 (Last two arguments were swaped!)
		tblDef.Create(TableName, dbAttachExclusive, _T(""),ConnectStr);
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::DefineTable: tblDef.Create"));
		return 1;
	} 
	catch (...) 
	{
		AfxMessageBox(_T("tblDef default catch"));
		throw;
	}

	//set default field property values
	fldInfo.m_strName				= _T(""); 
 	fldInfo.m_nType					= 0;
	fldInfo.m_lSize					= 50;		// ITL: Increased from 25 by CAR 887/002
 	fldInfo.m_lAttributes			= dbFixedField;
	fldInfo.m_nOrdinalPosition		= 0;		//Excel chokes on ordinal positions
	fldInfo.m_bRequired				= FALSE;
	fldInfo.m_bAllowZeroLength		= FALSE;
	fldInfo.m_lCollatingOrder		= 0;
	fldInfo.m_strForeignName		= _T("");
 	fldInfo.m_strSourceField		= _T("");
 	fldInfo.m_strSourceTable		= _T("");
	fldInfo.m_strValidationRule		= _T("");
	fldInfo.m_strValidationText		= _T("");
	fldInfo.m_strDefaultValue		= _T("");	//Excel chokes on default values

	//loop through grid adding fields to table def
	int i, k = m_fldProp.GetColumnCount();
	ASSERT(k > 0);
	for (i = 0; i < k; i++ )
	{

		//get field properties
		fldInfo.m_strName	= (LPCTSTR)m_fldProp.GetColName(i);	
		fldInfo.m_nType		= UGTypeToDaoType(m_fldProp.GetColDataType(i));
		
		if (fldInfo.m_strName == _T("Notification") 
			|| fldInfo.m_strName == _T("References")	// ITL: Added by CAR 887/002
			|| fldInfo.m_strName == _T("Workload") 		// ITL: Added by CAR 887/002
			|| fldInfo.m_strName == _T("Comment")) 
		{	//adjust for longer text fields
			fldInfo.m_lSize = 255;		
		}

		if (fldInfo.m_nType == dbDouble )  
		{
			fldInfo.m_bAllowZeroLength		= FALSE;
		}
		if (fldInfo.m_nType == dbDate) 
		{
			fldInfo.m_bAllowZeroLength		= FALSE;
		}
		if (fldInfo.m_nType == dbText ) 
		{
			fldInfo.m_bAllowZeroLength		= TRUE;
		}

		//add field to table def
		try 
		{
			tblDef.CreateField(fldInfo);
		} 
		catch(CDaoException* e) 
		{
			temp.Format(_T("tbDef.CreateField exception\r\ni = %d\r\nk = %d\r\nfield name = %s\r\nfield type = %d\r\nfield width = %ld\r\n"), 
			i, k, fldInfo.m_strName,fldInfo.m_nType,fldInfo.m_lSize);
			DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, (LPCTSTR)temp);
		} 
		catch(...)
		{
			throw;
		}

		//reinitialize field properties
		fldInfo.m_lSize		= 50;		// ITL: Increased from 25 by CAR 887/002
	}

	//append the table to database.
	try 
	{	
		tblDef.Append();
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("tbDef.Append exception"));
		return 1;
	} 
	catch(...)
	{
		throw;
	}

#ifdef _DEBUG
//until tabeldef is appended, can't get any info
	DaoUtils::TableDefTrace(pDB, TableName);
#endif
	
	//close table def and return
	try 
	{	
		tblDef.Close();
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("tbDef.Append exception"));
	} 
	catch (...) 
	{
		throw;
	}	
	return 0;
}

int CBatch::CreateRecordset(CDaoDatabase *pDB, LPCTSTR tableName)
{
#ifdef DO_UG
	if (m_pGrid == NULL)
	{
		return 1;
	}
#endif

#ifdef DO_CGX
	if (m_pBatchGrid == NULL)
	{
		return 1;
	}
#endif

	CDaoTableDef  TblDef(pDB);	
	CDaoRecordset oRecordset(pDB);
	
	//open table definition
	try 
	{
		TblDef.Open(tableName);
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::CreateRecordset: exception on TblDef.Open"));
	} 
	catch(...)
	{
		throw;
	}
	
	//open recordset
	try 
	{
		oRecordset.Open(&TblDef, dbOpenDynaset, dbDenyWrite);
	} 
	catch (CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::CreateRecordset: exception on oRecordset.Open"));
	} 
	catch(...)
	{
		throw;
	}
	
	//write records
	int			nCols;	
	int			nRows;
	int			iRow;
	int			iCol;
	CUGCell		cell;
	COleVariant fldVal;
	CString		temp;

	nRows = GetNumRows();
	nCols = TblDef.GetFieldCount();  //Only export columns actually in table def vice
	for (iRow = 0; iRow < nRows; iRow++)
	{		
		
		//add an empty record
		try 
		{
			oRecordset.AddNew(); 
		} 
		catch(CDaoException* e) 
		{
			DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::CreateRecordset: oRecordset.AddNew"));
		} 
		catch(...)
		{
			throw;
		}

		//write columns/fields in the new record 
		for (iCol = 0 ; iCol < nCols; iCol++)
		{
			
			//get and convert grid cell value to Ole value
			if (GetCell(iCol , iRow, &cell) == UG_SUCCESS)
			{
				if(CellValToCOleVal(&cell, fldVal) != 0) 
				{
					temp.Format(_T("CBatch::CreateRecordset: CellValToCOleVal error\r\niRow = %d\r\biCol = %d"), iRow, iCol);
					AfxMessageBox(temp);
					return 1;				
				}
			} 
			else 
			{
				temp.Format(_T("CBatch::CreateRecordset: GetCell error\r\niRow = %d\r\biCol = %d"), iRow, iCol);
				AfxMessageBox(temp);
				return 1;
			}

			//set database field value
			try 
			{
				//can't set field values by ordinals, since the fields 
				//don't get added in order--apparently a Dao bug
				oRecordset.SetFieldValue(m_fldProp.GetColName(iCol), fldVal); 
			} 
			catch(CDaoException* e) 
			{
				temp.Format(_T("oRecordset.SetFieldValue exception:\r\niRow = %d\r\niCol = %d"), iRow, iCol);
				DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, (LPCTSTR)temp);
			} 
			catch(...)
			{
				throw;
			}
		}
		
		//save changes to the record
		try 
		{
			oRecordset.Update();
		} 
		catch(CDaoException* e) 
		{
			DaoUtils::DaoExceptionBox(e,  __FILE__ , __LINE__, _T("oRecordset.Update() exception"));
		} 
		catch(...)
		{
			throw;
		}
	}
	
	//close recordset and return
	try
	{
		oRecordset.Close();
	} 
	catch (CDaoException* e)
	{
		DaoUtils::DaoExceptionBox(e,  __FILE__ , __LINE__, _T("oRecordset.Close() exception"));
	} 
	catch(...)
	{
		throw;
	}
	return 0;
}

int CBatch::UGTypeToDaoType(const int& UGtype)
{	
	switch(UGtype) 
	{
	case UG_TEXTDATA:
		return dbText;
	case UG_BOOLDATA:	
		return  dbBoolean;
	case UG_NUMBERDATA:
		return dbDouble;
	case UG_DATEDATA:
	case UG_TIMEDATA:
		return dbDate;
	default:
		return dbText;
	}
}

int CBatch::CellValToCOleVal(CUGCell* pCell, COleVariant& OleVal)
{
	CString strTemp;
	int		intTemp		= 0;
	double  dblTemp		= 0;
	short	shtDay		= 0;
	short	shtMon		= 0;
	short	shtYear		= 0;
	short	hour		= 0;
	short	minute		= 0;
	short	second		= 0;

	switch(pCell->GetDataType()) 
	{
	case UG_TEXTDATA:
		pCell->GetText(&strTemp);
		OleVal = COleVariant(strTemp, VT_BSTRT);
		break;

	case UG_BOOLDATA:
		intTemp = pCell->GetBool();
		OleVal = COleVariant((BYTE)(intTemp ? true : false));
		break;

	case UG_NUMBERDATA:
		pCell->GetNumber(&dblTemp);	
		OleVal = COleVariant(dblTemp);
		break;

	case UG_DATEDATA:
		pCell->GetDate(&shtDay, &shtMon, &shtYear);
		if( shtDay != -1 && shtMon != -1 && shtYear != -1) {
			OleVal = COleDateTime(shtYear, shtMon, shtDay, 0, 0, 0);
		} else {
			OleVal = COleDateTime( 1900, 1, 1, 0, 0, 0);
		}
		break;

	case UG_TIMEDATA:
		pCell->GetTime(&second, &minute, &hour);
		// The dawning of the age of aquarius = 30 Dec 1899
		if( hour >= 0 && minute >= 0 && second >= 0) {
			OleVal = COleDateTime(1899, 12, 30, hour, minute, second);
		} else {
			OleVal = COleDateTime(1899, 12, 30, 0, 0, 0);
		}
		break;

	default:
		pCell->GetText(&strTemp);
		OleVal = COleVariant(strTemp, VT_BSTRT);
		break;
	}

	return KO_OK;
}


CString CBatch::GetStatusString()
{
	return ((CLuceptApp*)AfxGetApp())->
		m_sampStats.GetNameFromID(m_status);

}

double CBatch::GetNegCutoff()
{
	if (GetNumCalVals() >= 1)
	{
		CParam calVal = GetCalVal(0);
		if (calVal.m_value.vt == VT_R8)
		{
			return calVal.m_value.dblVal;
		}
	}
	
	// Default calibration value
	return 0.0;
}

double CBatch::GetPosCutoff()
{
	if (GetNumCalVals() >= 2)
	{
		CParam calVal = GetCalVal(1);
		if (calVal.m_value.vt == VT_R8)
		{
			return calVal.m_value.dblVal;
		}
	}
	
	// Default calibration value
	return 0.0;
}


int CBatch::GetRepl(int iSamp)
{
	if (iSamp < 0 || iSamp >= GetNumSamples())
	{
		return -1;
	}

	CUGCell cell;
	m_sampList[iSamp].GetField(SF_REPLS, &cell);
	int sampRepls;
	cell.GetNumber(&sampRepls);

	return sampRepls;
}



bool CBatch::IsVirtual()
{
	bool rv = true;

	int nSamp = m_sampList.GetCount();
	if (nSamp < 1)
	{	// Define zero sample batch to be real not virtual
		return false;
	}
	for (int i = 0; i < nSamp && rv == true; i++)
	{
		if (!m_sampList[i].IsVirtual())
		{
			rv = false;
		}
	}
	return rv;
}

/**
 * Define a table for storing the properties of a run.
 */
int CBatch::DefinePropertiesTable(CDaoDatabase *pDB, LPCTSTR TableName, const CString& ConnectStr)
{
	ASSERT(pDB->IsOpen());
	ASSERT(pDB->CanUpdate());

#ifdef DO_UG
	if (m_pGrid == NULL)
	{
		return 1;
	}
#endif

#ifdef DO_CGX
	if (m_pBatchGrid == NULL)
	{
		return 1;
	}
#endif

	CString temp;
	CDaoTableDef	tblDef(pDB);
	CDaoFieldInfo	fldInfo;

	//create table definition object
	try 
	{	// Fixed by CAR 887/002 (Last two arguments were swaped!)
		tblDef.Create(TableName, dbAttachExclusive, _T(""),ConnectStr);
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::DefineTable: tblDef.Create"));
		return 1;
	} 
	catch (...) 
	{
		AfxMessageBox(_T("tblDef default catch"));
		throw;
	}

	//set default field property values
	fldInfo.m_strName				= _T(""); 
 	fldInfo.m_nType					= 0;
	fldInfo.m_lSize					= 50;		// ITL: Increased from 25 by CAR 887/002
 	fldInfo.m_lAttributes			= dbFixedField;
	fldInfo.m_nOrdinalPosition		= 0;		//Excel chokes on ordinal positions
	fldInfo.m_bRequired				= FALSE;
	fldInfo.m_bAllowZeroLength		= TRUE;
	fldInfo.m_lCollatingOrder		= 0;
	fldInfo.m_strForeignName		= _T("");
 	fldInfo.m_strSourceField		= _T("");
 	fldInfo.m_strSourceTable		= _T("");
	fldInfo.m_strValidationRule		= _T("");
	fldInfo.m_strValidationText		= _T("");
	fldInfo.m_strDefaultValue		= _T("");	//Excel chokes on default values

	//get field properties
	fldInfo.m_strName	= _T("Property");
	fldInfo.m_nType		= dbText;
		
	try 
	{
		tblDef.CreateField(fldInfo);
	} 
	catch(CDaoException* e) 
	{
		temp.Format(_T("tbDef.CreateField exception on Property field"));
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, (LPCTSTR)temp);
	} 
	catch(...)
	{
		throw;
	}

	//get field properties
	fldInfo.m_strName	= _T("Value");
	fldInfo.m_nType		= dbText;
		
	try 
	{
		tblDef.CreateField(fldInfo);
	} 
	catch(CDaoException* e) 
	{
		temp.Format(_T("tbDef.CreateField exception on Value field"));
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, (LPCTSTR)temp);
	} 
	catch(...)
	{
		throw;
	}

	//append the table to database.
	try 
	{	
		tblDef.Append();
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("tbDef.Append exception"));
		return 1;
	} 
	catch(...)
	{
		throw;
	}

#ifdef _DEBUG
//until tabeldef is appended, can't get any info
	DaoUtils::TableDefTrace(pDB, TableName);
#endif
	
	//close table def and return
	try 
	{	
		tblDef.Close();
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("tbDef.Append exception"));
	} 
	catch (...) 
	{
		throw;
	}	
	return 0;
}

/**
 * Add the properties to the results.
 */
int CBatch::AddProperties(CDaoDatabase *pDB, LPCTSTR tableName, LPCTSTR strWklName, int nBatch, CWklDoc* pDoc)
{
#ifdef DO_UG
	if (m_pGrid == NULL)
	{
		return 1;
	}
#endif

#ifdef DO_CGX
	if (m_pBatchGrid == NULL)
	{
		return 1;
	}
#endif

	CDaoTableDef  TblDef(pDB);	
	CDaoRecordset oRecordset(pDB);
	CString strFmt;
	
	//open table definition
	try 
	{
		TblDef.Open(tableName);
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::CreateRecordset: exception on TblDef.Open"));
	} 
	catch(...)
	{
		throw;
	}
	
	//open recordset
	try 
	{
		oRecordset.Open(&TblDef, dbOpenDynaset, dbDenyWrite);
	} 
	catch (CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::CreateRecordset: exception on oRecordset.Open"));
	} 
	catch(...)
	{
		throw;
	}

	// Write header info...

	// ...workload
	AddProperty( oRecordset, _T("Workload"), strWklName);

	// ... batch
	AddProperty( oRecordset, _T("Batch"), CBatch::GetBatchLetter(nBatch));

	// ... Operator
	AddProperty( oRecordset, _T("Operator"), pDoc->GetCreator());

	// ... Date
	// Ole Date format Bug fix!
	// This code prevents errors when Regional date settings are
	// not English
	{
		CSetLocale l(LC_TIME);
	
		strFmt = pDoc->GetModDate().Format(_T("%x"));
		AddProperty( oRecordset, _T("Date"), strFmt);
	}

	// ... Assay
	AddProperty( oRecordset, _T("Assay"), GetAssayName());

	// ... Calibration values
	int num = m_calValList.GetCount();
	for (int i = 0; i < num; i++)
	{
		COleVariant var;

		var = m_calValList[i].m_value;
		var.ChangeType(VT_R8);
		strFmt.Format(_T("%9.1f"), var.dblVal);	

		AddProperty( oRecordset, m_calValList[i].m_name, strFmt);
	}

	//close recordset and return
	try
	{
		oRecordset.Close();
	} 
	catch (CDaoException* e)
	{
		DaoUtils::DaoExceptionBox(e,  __FILE__ , __LINE__, _T("oRecordset.Close() exception"));
	} 
	catch(...)
	{
		throw;
	}
	return 0;
}

/**
 * Add a property to the properties table.
 */
void CBatch::AddProperty( CDaoRecordset &oRecordset, LPCTSTR strName, LPCTSTR strValue) 
{
	//add an empty record
	try 
	{
		oRecordset.AddNew(); 
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e, __FILE__ , __LINE__, _T("CBatch::CreateRecordset: oRecordset.AddNew"));
	} 
	catch(...)
	{
		throw;
	}

	oRecordset.SetFieldValue( _T("Property"), strName);
	oRecordset.SetFieldValue( _T("Value"), strValue);

	//save changes to the record
	try 
	{
		oRecordset.Update();
	} 
	catch(CDaoException* e) 
	{
		DaoUtils::DaoExceptionBox(e,  __FILE__ , __LINE__, _T("oRecordset.Update() exception"));
	} 
	catch(...)
	{
		throw;
	}
}
