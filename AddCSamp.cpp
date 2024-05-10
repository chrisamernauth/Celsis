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

// AddCSamp.cpp: implementation of the CAddCalSamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lucept.h"
#include "AddCSamp.h"
#include "Batch.h"
#include "ACCMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddCalSamp::CAddCalSamp()
{
	m_nAdded = -1;
	m_msgEnable = true;
}

CAddCalSamp::~CAddCalSamp()
{

}
	
long CAddCalSamp::Add(CBatch* pBatch, bool defaultCal)
{
	CACCMgr acc;
	long rv = KO_OK;
	ASSERT(pBatch != NULL);

	// Get Assay Pointer
	CAssayDef* pAssayDef;
	rv = pBatch->GetAssayPtr(&pAssayDef);
	if (rv != KO_OK)
	{
		return rv;
	}
	
	// Make calc method specified in AssayDef active in ACC
	long calcMethID = pAssayDef->GetCalcMethID();
	if (calcMethID == LUC_ID_UNDEF)
	{
		return KO_LUC_ADDCALS_NOMETHOD;
	}
	if (!acc.SelectMethod(calcMethID))
	{
		return KO_LUC_BAD_METHOD_ID;
	}

	// Load parameters into ACC
	if (!pAssayDef->InitACC(&acc))
	{
		return KO_LUC_ACC_ACPARAMS;
	}
	
	// Generate cal samples in ACC
	rv = acc.CreateCalSamps();
	if (rv != KO_OK)
	{
		return rv;
	}

	// Add samples to batch
	m_nAdded = acc.GetNumSamples();
	ASSERT (m_nAdded >= 0);
	if (m_nAdded == 0 && m_msgEnable)
	{
		AfxMessageBox(IDS_NO_CAL_SAMPS);
		return KO_OK;
	}

	for (int i = 0; i < m_nAdded; i++)
	{
		// Add samples to start of batch
		rv = pBatch->InsertSample(i);
		if (rv != KO_OK)
		{
			return rv;
		}

		CLucSamp* pSamp = pBatch->GetSamplePtr(i);
		ASSERT(pSamp != NULL);

		// Load sample from ACC
		VERIFY(pSamp->ReadACC(&acc, i, pAssayDef) == KO_OK);
	
		if (defaultCal)
		{
			// Change status to Defaul Cal
			pSamp->UpdateStatus(LUC_SSTAT_DEFCAL);
			pSamp->SetField(SF_VIRTUAL, LUC_VSTAT_DEFCAL);
		}
	}

	return rv;
};

int CAddCalSamp::GetNumSampsAdded()
{
	return m_nAdded;
}

void CAddCalSamp::EnableUserMessages(bool enable)
{
	m_msgEnable = enable;
}
