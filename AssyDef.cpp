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
#include "AssyDef.h"
#include "ACCMgr.h"
#include "WklDef.h"
#include "LuceErr.h"
#include "resource.h"
#include "IniFiles.h"
#include "Lucept.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAssayDef, CObject, ASSAYDEF_SER_SCHEMA)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAssayDef::CAssayDef()
{
	// Default constructor for CFieldProperties initializes to system
	m_fldProps = CFieldProperties();	// ! EGG  

	CString	section = _T("Paths");
	CString item	=_T("AssayDataPath");
	CLuceptApp* app	= (CLuceptApp *)AfxGetApp();


	// Initialize _T("virtual data members")
	// !!! This is not particularly efficient
	mv_fileDir = app->GetLuceptProfileString(section, item, _T(""));
	if (mv_fileDir.IsEmpty())
	{
		AfxMessageBox(IDS_NO_ADF_DIR);						// No Assay Definition Directory Defined.
		app->WriteProfileString(section, item, mv_fileDir);
	}

	VERIFY(mv_filePrefix.LoadString(IDS_AD_FILE_PREFIX));	// AD	
	VERIFY(mv_fileNameFormat.LoadString(IDS_ADFORMAT));		// AD%6.6d
	VERIFY(mv_fileExtActive.LoadString(IDS_ADF_EXTENSION));	// ADF
	m_fileExt = mv_fileExtActive;
	VERIFY(mv_fileExtObs.LoadString(IDS_ADO_EXTENSION));	// ADO
	VERIFY(mv_fileDirErr.LoadString(IDS_RD_ERR_ADF_DIR));	// Error reading from Assay Definition directory:\n

	m_shortAssayName = _T("");
	m_reagentName1 = _T("");	// !!! Get defaults from instrument object
	m_reagentName2 = _T("");	// !!! Get defaults from instrument object
	m_reagentName3 = _T("");	// !!! Get defaults from instrument object
	
	// Read from INI file
	section = _T("Parameter Defaults");
	item = _T("DefSampleRepls");
	m_defRepls = app->GetLuceptProfileInt(section, item, 1);

	item = _T("MaxTubesInBatch");
	m_maxTubesInBatch = app->GetLuceptProfileInt(section, item, gHiTubes);

	// Read from INI file
	item = _T("MeasName");
	m_measName = app->GetLuceptProfileString(section, item, _T(""));

	item = _T("AssayCalcID");
	m_ACalcMethID = app->GetLuceptProfileInt(section, item, -1);

	m_defCalsAllowed = false;
}

CAssayDef::CAssayDef(CAssayDef & src)
{
	*this = src;
}

CAssayDef::~CAssayDef()
{

}

CAssayDef& CAssayDef::operator =(CAssayDef & src)
{
	// Call base class
	CGenDef::operator=(src);
	
	m_shortAssayName	= src.m_shortAssayName;
	m_reagentName1		= src.m_reagentName1;
	m_reagentName2		= src.m_reagentName2;
	m_reagentName3		= src.m_reagentName3;
	m_defRepls			= src.m_defRepls;
	m_maxTubesInBatch	= src.m_maxTubesInBatch;
	m_measName			= src.m_measName;
	m_ACalcMethID		= src.m_ACalcMethID;

	m_sampTypes.Copy(src.m_sampTypes);
	m_sampTypes.FreeExtra();

	m_sampTypeNames.Copy(src.m_sampTypeNames);
	m_sampTypeNames.FreeExtra();

	m_calSampTypeIDs.Copy(src.m_calSampTypeIDs);
	m_calSampTypeIDs.FreeExtra();

	m_calSampUpd.Copy(src.m_calSampUpd);
	m_calSampUpd.FreeExtra();

	m_defCalsAllowed = src.m_defCalsAllowed;
	m_defCalRefList = src.m_defCalRefList;

	for (int i = 0; i < NUM_FLAGS; i++)
	{
		m_aFlags[i] = src.m_aFlags[i];
	}

	m_fldProps = src.m_fldProps; // ! EGG  Rev. 3 Ser Schema

	return *this;
}

//////////////////////////////////////////////////////////////////////
// Serialize
//////////////////////////////////////////////////////////////////////

void CAssayDef::Serialize( CArchive& archive)
{
	// Do base class first
	CGenDef::Serialize( archive );

	if (archive.IsStoring())
	{	// Always saves the latest format
		archive << ASSAYDEF_MAGIC_INT;
		archive << ASSAYDEF_SER_SCHEMA;

		archive << m_shortAssayName;
		archive << m_reagentName1;
		archive << m_reagentName2;
		archive << m_reagentName3;
		archive << m_defRepls;
		archive << m_maxTubesInBatch;
		archive << m_measName;
		archive << m_ACalcMethID;

		m_sampTypes.Serialize(archive);
		m_sampTypeNames.Serialize(archive);
	
		archive << m_defCalsAllowed;

		int nTypes = m_calSampTypeIDs.GetSize();
		archive << nTypes;
		// Cal sample type list
		int iType;

		for ( iType = 0; iType < nTypes; iType++)
		{
			archive << m_calSampTypeIDs[iType];
		}
		// Cal sample auto update enable
		for (iType = 0; iType < nTypes; iType++)
		{
			archive << m_calSampUpd[iType];
		}

		m_defCalRefList.Serialize(archive);

		for (int i = 0; i < NUM_FLAGS; i++)
		{
			m_aFlags[i].Serialize(archive);
		}

		m_fldProps.Serialize(archive); // ! EGG  Rev. 3 Ser Schema
	}
	else	// IsLoading
	{
		int magicInt;
		UINT schemaVer;

		if (m_fileVer == 1)	// from gendef base class
		{	// Kludge based on gendef file version
			schemaVer = 1;
		}
		else
		{	// actually read the schema
			archive >>  magicInt;
			ASSERT(magicInt == ASSAYDEF_MAGIC_INT);
			archive >> schemaVer;
		}

		switch (schemaVer)
		{
		case 3:
		case 2:
			{
				archive >> m_shortAssayName;
				archive >> m_reagentName1;
				archive >> m_reagentName2;
				archive >> m_reagentName3;
				archive >> m_defRepls;
				archive >> m_maxTubesInBatch;
				archive >> m_measName;
				archive >> m_ACalcMethID;

				m_sampTypes.Serialize(archive);
				m_sampTypeNames.Serialize(archive);

				archive >> m_defCalsAllowed;

				int nTypes;
				archive >> nTypes;
				m_calSampTypeIDs.RemoveAll();
				
				int iType;

				for ( iType = 0; iType < nTypes; iType++)
				{
					long ID;
					archive >> ID;
					m_calSampTypeIDs.Add(ID);
				}

				m_calSampUpd.RemoveAll();
				for (iType = 0; iType < nTypes; iType++)
				{
					BOOL bUpdate;
					archive >> bUpdate;
					m_calSampUpd.Add(bUpdate);
				}

				m_defCalRefList.Serialize(archive);

				for (int i = 0; i < NUM_FLAGS; i++)
				{
					m_aFlags[i].Serialize(archive);
				}

				if (schemaVer >= 3 )
				{	// ! EGG  Rev. 3 Ser Schema
					m_fldProps.Serialize(archive); 
				}
				// else use default field properties
			}
			break;

		case 1:
			{
				CParam	jk;

				archive >> m_shortAssayName;
				archive >> m_reagentName1;
				archive >> m_reagentName2;
				archive >> m_reagentName3;
				archive >> m_defRepls;
				archive >> m_maxTubesInBatch;
				archive >> m_measName;
				archive >> m_ACalcMethID;

				m_sampTypes.Serialize(archive);
				m_sampTypeNames.Copy(m_sampTypes);
				m_sampTypeNames.FreeExtra();

				// This was a goofy idea
				// Ignore any unknown params here as CParams
				do
				{
					jk.Serialize(archive);
				} 
				while (jk.m_value.vt != VT_EMPTY);
			}
			break;

		default:
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Load assay def from ACC
//////////////////////////////////////////////////////////////////////

bool CAssayDef::InitParamList(CACCMgr * pACC)
{
	// Select Calc method in ACC
	if (pACC->SelectMethod(m_ACalcMethID) == false)
	{
		CString errMsg;
		errMsg.Format(IDS_BAD_CALC_METH_ID, m_ACalcMethID);
		AfxMessageBox(errMsg);
		return false;
	}

	// Method found
	// Clear existing parameter list
	m_paramList.DeleteAll();

	// Load parameters from ACC
	int nParams = pACC->GetNumParams();
	for (int iParam = 0; iParam < nParams; iParam++)
	{
		CParamEx* pParam;
		long id;

		// Add parameter to list
		pParam = m_paramList.Append();
		// Get parameter ID from iteration
		id = pACC->GetParamID(iParam);
		// Load new parameter
		*pParam = pACC->GetParamEx(id);
	}

	// Clear existing sample type list
	m_sampTypes.RemoveAll();
	m_sampTypeNames.RemoveAll();

	// Load sampTypes from ACC
	int nTypes = pACC->GetNumSampTypes();
	int iType;

	for ( iType = 0; iType < nTypes; iType++)
	{	// Read sample type ID and default name from ACC
		long ID = pACC->GetSampTypeID(iType);
		CString name = pACC->GetSampTypeName(ID);

		// Add sample type to list
		m_sampTypes.Add(CNamedID(ID, name));
	}
	// Initial sample type names use defaults from ACC
	m_sampTypeNames.Copy(m_sampTypes);
	m_sampTypeNames.FreeExtra();

	// Don't allow default cals by default
	m_defCalsAllowed = false;

	// Read list of ID's for calibration sample types
	// For each cal type add an empty SampRef to defCalRefList
	m_calSampTypeIDs.RemoveAll();
	m_calSampUpd.RemoveAll();
	m_defCalRefList.DeleteAll();
	nTypes = pACC->GetNumCalSampTypes();
	for(iType = 0; iType < nTypes; iType++)
	{
		m_calSampTypeIDs.Add(pACC->GetCalSampTypeID(iType));
		m_calSampUpd.Add(TRUE);		// Update by default
		m_defCalRefList.Append();
		m_defCalRefList[iType].SetRefType(CSampRef::REF_DEFCAL);
	}

	return true;
}


//////////////////////////////////////////////////////////////////////
// Verify AssayDef parameter list is consistent with ACC
//////////////////////////////////////////////////////////////////////

bool CAssayDef::VerifyParamList(CACCMgr * pACC)
{
	if (!pACC->SelectMethod(m_ACalcMethID))
	{	// Bad method ID !!! more detailed error info?
		return false;
	}

	if (m_paramList.GetCount() != pACC->GetNumParams())
	{	// Paramerter count mismatch
		return false;
	}
	
	// Check parameter types for all parameters
	// Does not check names
	// Does not check max/min
	int iParam;
	int num;

	for ( iParam = 0, num = m_paramList.GetCount();
		iParam < num; iParam++)
	{
		CParamEx param;

		param = pACC->GetParamEx(m_paramList[iParam].m_ID);
		if (param.m_value.vt != m_paramList[iParam].m_value.vt)
		{	// Parameter type mismatch, or bad param ID
			return false;
		}
			
	}

	if (m_sampTypes.GetSize() != pACC->GetNumSampTypes())
	{	// Sample type count mismatch
		return false;
	}
	
	// Check sample type ID's exists
	// Names don't need to match
	int numSampTypes =  m_sampTypes.GetSize();
	for (int iType = 0; iType < numSampTypes; iType++)
	{
		// Get sample type ID from iteration
		long testID = pACC->GetSampTypeID(iParam);

		// Scan assay def for matching id
		int iDefType;

		for ( iDefType = 0; 	iDefType < numSampTypes; iDefType++)
		{
			if (testID == m_sampTypes[iDefType].m_ID)
			{
				break;
			}
		}

		if (iDefType >= numSampTypes)
		{	// Didn't find matching ID in assay def
			return false;
		}
	}

	// !!! Check cal sample type id's?

	return true;
}


//////////////////////////////////////////////////////////////////////
// Initialize ACC with AssayDef parameter list 
//////////////////////////////////////////////////////////////////////

bool CAssayDef::InitACC(CACCMgr * pACC)
{
	if (m_ACalcMethID == LUC_ID_UNDEF)
	{
		return true;
	}

	if (!pACC->SelectMethod(m_ACalcMethID))
	{	// Bad method ID 
		SetCalcMeth(LUC_ID_UNDEF);
		return false;
	}

	if (pACC->ClearCalc() != KO_OK)
	{
		return false;
	}

	if (m_paramList.GetCount() != pACC->GetNumParams())
	{	// Paramerter count mismatch
		SetCalcMeth(LUC_ID_UNDEF);
		return false;
	}
	
	// Check parameter types for all parameters
	int num = m_paramList.GetCount(); 
	for (int iParam = 0; iParam < num; iParam++)
	{
		CParam param;
		long paramID = m_paramList[iParam].m_ID;

		param = pACC->GetParam(paramID);
		if (param.m_value.vt == m_paramList[iParam].m_value.vt)
		{	// Set parameter in ACC
			if (pACC->SetParamValue(m_paramList[iParam]) != KO_OK)
			{	// Error setting value
				SetCalcMeth(LUC_ID_UNDEF);
				return false;
			};
		}
		else
		{	// Parameter type mismatch, or bad param ID
			SetCalcMeth(LUC_ID_UNDEF);
			return false;
		}
			
	}

	// !!! Check sample types?

	return true;
}

long CAssayDef::IDGetDefCalSampRef(long ID, CSampRef* pSampRef)
{
	// Search the array of cal type ID's
	int numCalTypes = m_calSampTypeIDs.GetSize();
	ASSERT(m_defCalRefList.GetCount() == numCalTypes);
	for (int iCalType = 0; iCalType < numCalTypes; iCalType++)
	{
		if (m_calSampTypeIDs[iCalType] == ID)
		{	// This is it
			*pSampRef = m_defCalRefList[iCalType];
			return KO_OK;
		}
	}

	return KO_BAD_CAL_SAMP_ID;
}

long CAssayDef::NameGetDefCalSampRef(LPCTSTR name, CSampRef * pSampRef)
{	// Get sample type ID
	long sampTypeID = SampNameGetID(name);
	if (sampTypeID == -1)
	{
		return KO_BAD_SAMP_TYPE_NAME;
	}

	// Get sample ref from cal sample type ID
	return IDGetDefCalSampRef(sampTypeID, pSampRef);
}

