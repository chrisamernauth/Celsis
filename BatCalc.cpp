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

// BatCalc.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "BatCalc.h"
#include "WklDoc.h"
#include "ACCMgr.h"
#include "DaoUtils.h"
#include "WklRead.h"
#include "LccMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const LPCTSTR tableName = _T("FLG_TBL");

/////////////////////////////////////////////////////////////////////////////
// CBatCalc

IMPLEMENT_DYNCREATE(CBatCalc, CView)

CBatCalc::CBatCalc()
{	TRACE0("CBatCalc::CBatCalc()\n");

	m_iBatch = LUC_BATCH_UNDEF;
	m_pBatch = NULL;
	m_pAssay = NULL;
	m_iNextSamp = LUC_SAMP_UNDEF;
	m_batchCalibrated = false;
	m_missingDefCals = false;

	m_pAcc = NULL;
	m_pReader = NULL;
	m_action = CUserFlag::DO_NOTHING;
	m_pRecSet = NULL;
	m_pTableDef = NULL;
	
	InitDefFInfo();	
}

CBatCalc::~CBatCalc()
{	TRACE(_T("CBatCalc::~CBatCalc()\n"));

	DaoObjectsClose();
}


BEGIN_MESSAGE_MAP(CBatCalc, CView)
	//{{AFX_MSG_MAP(CBatCalc)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatCalc drawing

void CBatCalc::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CBatCalc diagnostics

#ifdef _DEBUG
void CBatCalc::AssertValid() const
{
	CView::AssertValid();
}

void CBatCalc::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWklDoc* CBatCalc::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWklDoc)));
	return (CWklDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBatCalc message handlers

void CBatCalc::OnInitialUpdate() 
{	TRACE(_T("CBatCalc::OnInitialUpdate() \n"));

	ASSERT(GetSafeHwnd() != NULL);
	CView::OnInitialUpdate();

	// Create ACC
	m_pAcc = new CACCMgr(this);
	if (m_pAcc == NULL)
	{
		AfxMessageBox(IDS_NO_ACC);
		ASSERT(false);
	}
}

void CBatCalc::OnDestroy() 
{
	ASSERT(m_pAcc);

	delete m_pAcc;
	m_pAcc = NULL;

	CView::OnDestroy();
}

void CBatCalc::RegReader(CWklRead* pReader)
{
	ASSERT(pReader);

	m_pReader = pReader;
}

void CBatCalc::SetBatch(int iBatch)
{	TRACE1("CBatCalc::SetBatch, iBatch == %d\n", iBatch);

	m_iBatch = iBatch;
	m_pBatch = GetDocument()->GetBatchPtr(iBatch);
	ASSERT(m_pBatch != NULL);
	
	DaoObjectsClose();
	DaoObjectsOpen();
}

long CBatCalc::StartCalc()
{	TRACE0("CBatCalc::StartCalc()\n");

	long rv;

	if (m_pBatch == NULL)
	{
		return KO_LUC_NO_BATCH;
	}

	// Get Assay Pointer
	rv = m_pBatch->GetAssayPtr(&m_pAssay);
	if (rv != KO_OK)
	{
		AfxMessageBox(IDS_BAD_ASSAY_ID);
		return KO_LUC_BAD_ASSAY_ID;
	}
	ASSERT(m_pAssay != NULL);

	rv = ResolveDefCals();

	// Set calculation parameters
	if (!m_pAssay->InitACC(m_pAcc))
	{
		AfxMessageBox(IDS_ACPARAMS_INVALID);
		return KO_LUC_ACC_ACPARAMS;
	};
	
	// Start calculation
	rv = m_pAcc->StartCalc();
	if (rv != KO_OK)
	{
		return rv;
	}

	// Note calibration values have not been read
	m_batchCalibrated = false;
	
	m_iNextSamp = 0;
	
	return rv;
};

long CBatCalc::CalcNextSamp()
{	TRACE0("CBatCalc::CalcNextSamp()\n");

	long rv = KO_OK;

	// Get sample pointer
	CLucSamp* pSamp = m_pBatch->GetSamplePtr(m_iNextSamp);
	if (pSamp == NULL)
	{
		return KO_LUC_BAD_SAMP_INDEX;
	}

	// Send sample data through ACC for calculation
	ASSERT(m_pAcc);
	ASSERT(m_pAssay);
	rv = pSamp->WriteACC(m_pAcc, m_pAssay);
	if (rv != KO_OK)
	{	// !!! Supress non-fatal errors
		CString errMsg;
		errMsg.Format(IDS_ACC_SAMPLE_WRITE_ERR, rv, CLccMgr::LookupError( rv));
		AfxMessageBox(errMsg);		
	}

	// Read processed sample back from ACC
	rv = pSamp->ReadACC(m_pAcc, m_iNextSamp, m_pAssay);
	if (rv != KO_OK)
	{
		// !!! Supress non-fatal errors
		CString errMsg;
		errMsg.Format(IDS_ACC_SAMPLE_READ_ERR, rv, CLccMgr::LookupError( rv));
		AfxMessageBox(errMsg);		
	}

	CheckFlags(m_iNextSamp);

	if (m_action > CUserFlag::DO_NOTHING)
	{
		// Set flagged status
		VERIFY(GetDocument()->SetSampStatus(LUC_SSTAT_FSTOP, 
			m_iBatch, m_iNextSamp, this) == KO_OK);
	}
	else if (m_missingDefCals)
	{
		// Set uncalibrated status
		VERIFY(GetDocument()->SetSampStatus(LUC_SSTAT_UNCAL, 
			m_iBatch, m_iNextSamp, this) == KO_OK);
	}

	// Allow this sample to become the next def cal if appropriate
	if (m_action == CUserFlag::DO_NOTHING)
	{
		CheckDefCal(m_iNextSamp);
	}

	// Notify views of new data
	GetDocument()->SetModifiedFlag();
	CWklMods mods(CWklMods::NATURE_MOD, CWklMods::SCOPE_SAMPLE, 
		m_iBatch, m_iNextSamp);
	GetDocument()->UpdateAllViews(this, 0, &mods);

	// Check if cal values are done
	if (!m_batchCalibrated && m_pAcc->IsCalibrated())
	{
		m_batchCalibrated = true;

		int numCals = m_pAcc->GetNumCalVals();
		for (int iVal = 0; iVal < numCals; iVal++)
		{
			// Add calibration values to batch
			long calValID = m_pAcc->GetCalValID(iVal);
			m_pBatch->AppendCalVal(m_pAcc->GetCalVal(calValID));
		}

		// Notify views
		if (numCals > 0)
		{
			CWklMods mods(CWklMods::NATURE_ADD_CALVAL, CWklMods::SCOPE_BATCH, m_iBatch);
			GetDocument()->UpdateAllViews(this, 0, &mods);
		}
	}

	// If that is the last sample in the batch 
	if (++m_iNextSamp == m_pBatch->GetNumSamples())
	{
		long batStat;

		// Set batch status
		if (!m_missingDefCals)
		{
			batStat = LUC_BSTAT_DONE;
		}
		else
		{
			batStat = LUC_BSTAT_UNCAL;
		}

		VERIFY(GetDocument()->SetBatchStatus(batStat, m_iBatch, this) == KO_OK);
	}

	return rv;
}


long CBatCalc::SyncCalcSamp(int iSamp)
{	TRACE1("CBatCalc::SyncCalcSamp iSamp == %d\n", iSamp);	

	long rv = KO_OK;

	if (iSamp == LUC_SAMP_UNDEF)
	{
		iSamp = m_iNextSamp;
	}

	if (iSamp < m_iNextSamp)
	{	// Already done
		AfxMessageBox(_T("Already calc'd that sample"));
		return KO_OK;
	}

	while (iSamp > m_iNextSamp)
	{	// Process intervening virtual samples

		// Check if next sample is virtual
		CLucSamp* pSamp = m_pBatch->GetSamplePtr(m_iNextSamp);
		if (pSamp == NULL)
		{
			return KO_LUC_BAD_SAMP_INDEX;
		}
		long stat = pSamp->GetStatus();

		// Error if non-virtual unprocessed sample precedes
		if (stat != LUC_SSTAT_VIRTUAL)
		{
			return KO_LUC_SAMP_NOT_VITUAL;
		}

		// Calc skipped virtual sample
		rv = CalcNextSamp();
		if (rv != KO_OK)
		{
			return rv;
		}
	}

	// Calc the specified sample
	ASSERT(iSamp == m_iNextSamp);
	rv = CalcNextSamp();
	if (rv != KO_OK)
	{
		return rv;
	}

	while (!IsDone() && m_pBatch->GetSamplePtr(m_iNextSamp)->
		GetStatus() == LUC_SSTAT_VIRTUAL)	
	{	// Process immediately following virtual samples
		rv = CalcNextSamp();
		if (rv != KO_OK)
		{
			return rv;
		}
	}

	return rv;
}

////////////////////////////////////////////////////////////////////////
// Replace Default calibrator samples with virtual samples
////////////////////////////////////////////////////////////////////////
long CBatCalc::ResolveDefCals()
{
	long rv = KO_OK;

	// Get application assay def (not batch private copy)
	CAssayDef* pAppAssay = ((CLuceptApp*)AfxGetApp())->
		m_assays.GetPtrFromName(m_pBatch->GetAssayName());
	ASSERT(pAppAssay != NULL);

	// Get default cal references if appropriate
	if (pAppAssay->DefCalsAllowed())
	{	// Check for any DefCal samples
		int numSamps = m_pBatch->GetNumSamples();
		CLucSamp* pSamp;
		for (int iSamp = 0; iSamp < numSamps; iSamp++)
		{
			pSamp = m_pBatch->GetSamplePtr(iSamp);
			ASSERT(pSamp != NULL);

			int virtStat;
			pSamp->GetField(SF_VIRTUAL, &virtStat);
			if (virtStat == LUC_VSTAT_DEFCAL)	
			{	// We have a compelling need for a default calibrator
				// Get calibrator sample type name
				CString calTypeName;
				VERIFY(pSamp->GetField(SF_TYPE_XFI, &calTypeName) == KO_OK);

				// Get sample reference from assay
				CSampRef calRef;
				long tempRV = pAppAssay->
					NameGetDefCalSampRef(calTypeName, &calRef);
				if (tempRV == KO_OK)
				{	// Fill the DefCal sample with data
					tempRV = calRef.MakeVirtualSamp(pSamp);
					pSamp->SetField(SF_VIRTUAL, LUC_VSTAT_DEFCAL);
					if (tempRV != KO_OK)
					{	// can't get the default cal data
						// Save first error and continue
						m_missingDefCals = true;
						if (rv == KO_OK)
						{
							rv = tempRV;
						}
					}
				}
				else // No such cal type
				{
					// Samples don't match assay def
					// Save first error and continue
					m_missingDefCals = true;
					if (rv == KO_OK)
					{
						rv = tempRV;
					}
				}
			} // end of this sample
		} // end of this batch
	} // end of DefCalsAllowed

	return rv;
}


void CBatCalc::Dismiss()
{	TRACE0("CBatCalc::Dismiss()\n");

	// Remove yourself 
	((CMDIChildWndEx*)GetParent())->MDIDestroy();
}

bool CBatCalc::IsDone()
{	TRACE0("CBatCalc::IsDone()\n");

	return m_pBatch != NULL && 
	m_iNextSamp >= m_pBatch->GetNumSamples(); 
}

long CBatCalc::CheckDefCal(int iSamp)
{
	// Get app's assay def 
	// Don't use batch's internal assay def
	CAssayDef* pAppAssay = ((CLuceptApp*)AfxGetApp())->
		m_assays.GetPtrFromName(m_pBatch->GetAssayName());
	ASSERT(pAppAssay != NULL);

	// Check if assay allows default calibrators
	if (!m_pAssay->DefCalsAllowed())
	{
		return KO_OK;
	}
	
	CLucSamp* pSamp = m_pBatch->GetSamplePtr(iSamp);

	// Get sample type
	CString txt;
	pSamp->GetField(SF_TYPE_XFI, &txt);	
	long typeID = pAppAssay->SampNameGetID(txt);

	// Check if this is a virtual sample
	if(pSamp->IsVirtual())
	{
		return KO_OK;
	}
	
	// Check if this is a calibrator sample
	int numCalTypes = pAppAssay->GetNumCalSampTypes();
	for (int iCalType = 0; iCalType < numCalTypes; iCalType++)
	{
		if (pAppAssay->IndexGetCalSampTypeID(iCalType) == typeID)
		{	// Found a matching calibrator
			// Get automatic update flag
			if (pAppAssay->IndexGetCalUpdate(iCalType))
			{	// This cal type uses automatic updates
				// Verify sample result 
				// Result strings are not currently meaningful to Lucept
				// !!! Kludge !!! assume any notification string is bad
				CString txt;
				pSamp->GetField(SF_NOTIF, &txt);
				if (txt.IsEmpty())
				{	// Assume cal is OK
					UpdateDefCal(pAppAssay, iCalType, iSamp);
				}
			}

			// There can only be one
			break;
		}
	}

	return KO_OK;
}

long CBatCalc::UpdateDefCal(CAssayDef* pAppAssay, int calIndex, int iSamp)
{	// Create def cal reference to this sample
	CString path = GetDocument()->GetPathName();
	ASSERT(!path.IsEmpty());

	CSampRef newRef(iSamp, m_iBatch, path, CSampRef::REF_DEFCAL); 

	// Copy new cal ref to assay
	pAppAssay->m_defCalRefList[calIndex] = newRef;

	// And save revised assay def
	pAppAssay->WriteToDisk();

	return KO_OK;
}


////////////////////////////////////////////////////////////////////////
// Flag stuff
////////////////////////////////////////////////////////////////////////

long CBatCalc::CheckFlags(int iSamp)
{	TRACE1("CBatCalc::CheckFlags: iSamp==%d\n", iSamp);

	CLucSamp* pSamp = m_pBatch->GetSamplePtr(iSamp);
	ASSERT(pSamp != NULL);

	// Do each query
	int iFlag;

	for ( iFlag = 0; iFlag < NUM_FLAGS; iFlag++)
	{
		bool thisFlag;

		ASSERT(m_pAssay != NULL);
		CUserFlag* pFlag = &m_pAssay->m_aFlags[iFlag];
		ASSERT(pFlag != NULL);

		// Make table containing only current sample
		// Reloading for each flag updates prefious flag fields
		LoadSampleRec(iSamp);

		int numCond = pFlag->m_condList.GetSize();
		if (numCond == 0)
		{
			// No conditions means the flag is always false
			thisFlag = false;
		}
		else
		{
			// Gonna build me a query string
			CString qryStr(_T("SELECT * FROM "));
			qryStr += tableName;
			qryStr += _T(" WHERE ");

			for (int iCond = 0; iCond < numCond; iCond++)
			{
				qryStr += pFlag->m_condList[iCond];
				if (iCond < numCond - 1)
				{
					qryStr += _T(" AND ");
				}
				else
				{
					qryStr += _T(";");
				}
			}

			TRACE2("Flag %d query is %s", iFlag, qryStr);
			try
			{	// Create a temporary query def
				ASSERT(m_db.IsOpen());
				CDaoQueryDef qryDef(&m_db);
				qryDef.Create(NULL, qryStr);

				// And use it to create a temporary record set
				CDaoRecordset qryRec(&m_db);
				qryRec.Open(&qryDef, dbOpenSnapshot);

				// If the set is empty the flag if false
				if(qryRec.IsBOF() && qryRec.IsEOF())
				{
					thisFlag = false;
				}
				else
				{
					thisFlag = true;
				}

				qryRec.Close();
				qryDef.Close();
			}
			catch(CDaoException* e)
			{
				DaoUtils::DaoExceptionBox(e, __FILE__,__LINE__,
					_T("CBatCalc::CheckFlags()"));
				DaoObjectsClose();
			} 

		}
		// Value of thisFlag is determined

		// Get flag fieldID
		long fieldID = pSamp->GetFlagFieldID(iFlag);
		ASSERT(fieldID != LUC_ID_UNDEF);
		// Set the field value
		pSamp->SetField(fieldID, thisFlag);

	}
	// All flag values are set

	// Determine most severe action required
	for (iFlag = 0; iFlag < NUM_FLAGS; iFlag++)
	{
		long fieldID =  pSamp->GetFlagFieldID(iFlag);
		bool thisFlag;
		VERIFY(pSamp->GetField(fieldID, &thisFlag) == KO_OK);
		if (m_pAssay->m_aFlags[iFlag].m_polarity && thisFlag ||
			!m_pAssay->m_aFlags[iFlag].m_polarity && !thisFlag)
		{
			// this flag is active
			if (m_pAssay->m_aFlags[iFlag].m_action > m_action)
			{	// action becomes most severe action called for
				m_action = m_pAssay->m_aFlags[iFlag].m_action;
			}
		}
	}

	if (m_action > CUserFlag::DO_NOTHING && m_pReader != NULL)
	{	// take action
		if(m_pReader->IsKindOf(RUNTIME_CLASS(CWklRead)))
		{
			m_pReader->FlagAction(m_iBatch, iSamp, m_action);
		}
	}

	return KO_OK;
}


long CBatCalc::DaoObjectsOpen()
{	TRACE(_T("CBatCalc::DaoObjectsOpen()\n"));

	//get unique temp filename
	m_dbFileName = DBTempFilenameGet();		
	ASSERT(!m_dbFileName.IsEmpty());
	_wremove(m_dbFileName);

	// Create the DAO database
	try
	{
		m_db.Create(m_dbFileName);
		ASSERT(m_db.IsOpen());
	}
	catch(CDaoException* e)
	{
		DaoUtils::DaoExceptionBox(e, __FILE__,__LINE__,
			_T("CBatCalc::DaoObjectsOpen()"));
		DaoObjectsClose();
	} 

	// Create the table definition
	try
	{
		m_pTableDef = new CDaoTableDef(&m_db);
		ASSERT(m_pTableDef);
		m_pTableDef->Create(tableName);

		// Add fields to table definition
		for (int iFld = 0; iFld < SF_FIELD_COUNT; iFld++)
		{
			CDaoFieldInfo tmpInfo;

			// Fill with defaults
			tmpInfo = m_defFieldInfo;
			
			// Load field data from field properties
			tmpInfo.m_strName =	CFieldProperties::GetIndexTitle(iFld); 
			tmpInfo.m_nType = CFieldProperties::GetIndexDAOType(iFld);
			if (tmpInfo.m_nType == dbText)
			{
				tmpInfo.m_lSize = CFieldProperties::GetIndexDaoSize(iFld);
			}
			else
			{	// This keeps DAO happy
				tmpInfo.m_bAllowZeroLength	= FALSE;
				tmpInfo.m_lAttributes &= ~dbVariableField;
			}

			// Add the new field definition
			m_pTableDef->CreateField(tmpInfo);
		}
		
		// Save the table def in the database
		m_pTableDef->Append();
		ASSERT(m_pTableDef->IsOpen());
	}
	catch(CDaoException* e)
	{
		DaoUtils::DaoExceptionBox(e, __FILE__,__LINE__,_T("CBatCalc::DaoObjectsOpen()"));
		DaoObjectsClose();
	} 
	
	// Open Record Set 
	try
	{	
		m_pRecSet = new CDaoRecordset(&m_db);
		ASSERT(m_pRecSet);
		m_pRecSet->Open(m_pTableDef, dbOpenDynaset);
		ASSERT(m_pRecSet->IsOpen());
	}
	catch(CDaoException* e)
	{
		DaoUtils::DaoExceptionBox(e, __FILE__,__LINE__,_T("CBatCalc::DaoObjectsOpen()"));
		DaoObjectsClose();
	} 

	return 0;
}

long CBatCalc::DaoObjectsClose()
{	TRACE(_T("CBatCalc::DaoObjectsClose()\n"));

	if(m_pRecSet != NULL)
	{
		if(m_pRecSet->IsOpen())
		{
			m_pRecSet->Close();		
		}

		delete m_pRecSet;
		m_pRecSet = NULL;
	}

	if(m_pTableDef != NULL)
	{
		if(m_pTableDef->IsOpen())
		{
			m_pTableDef->Close();
		}

		delete m_pTableDef;
		m_pTableDef = NULL;
	}

	if(m_db.IsOpen())
	{
		m_db.Close();
	}

	_wremove(m_dbFileName);
	
	return false;
}

long CBatCalc::LoadSampleRec(int iSamp)
{
	long rv = KO_OK;

	ASSERT (m_pTableDef != NULL && m_pTableDef->IsOpen());
	ASSERT (m_pRecSet != NULL && m_pRecSet->IsOpen());
	//purge existing records
	DaoUtils::RecordsRemoveAll(*m_pRecSet);

	// Get sample pointer
	CSample* pSamp = m_pBatch->GetSamplePtr(iSamp);
	ASSERT(pSamp != NULL);
			
	// Add this sample as a new record
	try
	{
		COleVariant fldVal;

		// Create new record
		m_pRecSet->AddNew();
			
		// For each workload field
		CUGCell	cell;
		for (int iFld = 0; iFld < SF_FIELD_COUNT; iFld++)
		{	// Get cell
			VERIFY(pSamp->GetDatum(iFld, &cell) == UG_SUCCESS);
			// Convert cell to variant (with text lenght checking)
			VERIFY(cell.CopyInfoTo(fldVal, 
				m_pBatch->GetFldProp()->GetIndexDaoSize(iFld)) == UG_SUCCESS);
			// Write variant to db record field
			m_pRecSet->SetFieldValue(iFld, fldVal);
		}

		// Save record
		m_pRecSet->Update();
	}
	catch(CDaoException* e)
	{
		DaoUtils::DaoExceptionBox(e, __FILE__,__LINE__,_T("CBatCalc::LoadSampleRec()"));
		return KO_LUC_RPT_REC_CREATE;
	} 

	return rv;
}


CString CBatCalc::DBTempFilenameGet()
{
	LPTSTR  localtemp = NULL;
	LPTSTR  envtmpvar = NULL;
	CString strFilename(_T(""));

	//get a unique temporary filename in temp directory
	envtmpvar = _tgetenv(_T("TMP"));
	ASSERT(envtmpvar != NULL);
	if((localtemp = _ttempnam(envtmpvar, _T("flg"))) != NULL) 
	{
		strFilename = localtemp;
		free(localtemp);
		strFilename += _T(".mdb");
	}
	else
	{
		strFilename.Empty();
	}
	
	return strFilename;
}


void CBatCalc::InitDefFInfo()
{
	// Initialize default field info structure

	// The following are used in field creation
	m_defFieldInfo.m_strName = _T("");		
 	m_defFieldInfo.m_nType = dbText;
	m_defFieldInfo.m_lSize = 0;				// Only used for text
	m_defFieldInfo.m_lAttributes = dbVariableField | dbUpdatableField;
	m_defFieldInfo.m_bAllowZeroLength = TRUE; // Only allowed for text

	// The following are ignored for field creation
	m_defFieldInfo.m_bRequired			= FALSE;
	m_defFieldInfo.m_nOrdinalPosition	= 0;	
	m_defFieldInfo.m_lCollatingOrder	= dbSortGeneral;
	m_defFieldInfo.m_strForeignName		= _T("");
 	m_defFieldInfo.m_strSourceField		= _T("");
 	m_defFieldInfo.m_strSourceTable		= _T("");
	m_defFieldInfo.m_strValidationRule	= _T("");
	m_defFieldInfo.m_strValidationText	= _T("");
	m_defFieldInfo.m_strDefaultValue	= _T("");	

	return;
}

