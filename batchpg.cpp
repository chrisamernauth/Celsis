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

// BatchPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "BatchPg.h"
#include "Lucept.h"
#include "LuceErr.h"
#include "ScnChain.h"
#include "AssyDef.h"
#include "LucSamp.h"
#include "LccMgr.h"
#include "Specification.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CBatchPropPage, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CBatchPropPage constructor/destructor
/////////////////////////////////////////////////////////////////////////////

CBatchPropPage::CBatchPropPage() : CPropertyPage(CBatchPropPage::IDD)
{
	//{{AFX_DATA_INIT(CBatchPropPage)
	m_nTubes = 0;
	m_nRealCalTubes = 0;
	m_nAddedSampTubes = 0;
	m_nLeftoverTubes = 0;
	m_reagPstr = _T("0");
	m_reag1str = _T("0");
	m_reag2str = _T("0");
	m_reag3str = _T("0");
	m_timeStr = _T("0:00");
	m_bAddCalSamps = FALSE;
	m_bUseDefCals = FALSE;
	m_bUseExistingCals = FALSE;
	m_allowMerge = TRUE;
	//}}AFX_DATA_INIT

	m_mSec = 0;
	m_reagentP = 0;
	m_reagent1 = 0;
	m_reagent2 = 0;
	m_reagent3 = 0;
	m_pScanChain = NULL;
	m_pAssay = NULL;
	m_pBatch = NULL;
	m_pageInitialized = false;
	m_assayDefined = false;

	m_defCalsAllowed = true;
	m_existCalsAllowed = true;
	m_addCalsAllowed = true;

	m_runHidden = false;
	m_sampRepl = -1;
}


CBatchPropPage::~CBatchPropPage()
{
}


void CBatchPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchPropPage)
	DDX_Control(pDX, IDC_ASSAYBOX_BPP, m_assayBox);
	DDX_Control(pDX, IDC_SPIN_SAMP_REPL, m_spinSampRepl);
	DDX_Text(pDX, IDC_BATCH_TUBES, m_nTubes);
	DDX_Text(pDX, IDC_LEFTOVER_TUBES, m_nLeftoverTubes);
	DDX_Text(pDX, IDC_REAGENT_P_REQB, m_reagPstr);
	DDX_Text(pDX, IDC_REAGENT_1_REQB, m_reag1str);
	DDX_Text(pDX, IDC_REAGENT_2_REQB, m_reag2str);
	DDX_Text(pDX, IDC_REAGENT_3_REQB, m_reag3str);
	DDX_Text(pDX, IDC_CAL_TUBES, m_nRealCalTubes);
	DDX_Text(pDX, IDC_PROC_TIMEB, m_timeStr);
	DDX_Check(pDX, IDC_ADD_CAL_SAMPS, m_bAddCalSamps);
	DDX_Check(pDX, IDC_USE_DEF_CALS, m_bUseDefCals);
	DDX_Check(pDX, IDC_USE_EXIST_CALS, m_bUseExistingCals);
	DDX_Check(pDX, IDC_ALLOW_MERGE, m_allowMerge);
	DDX_Text(pDX, IDC_ADDED_TUBES, m_nAddedSampTubes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBatchPropPage)
	ON_EN_CHANGE(IDC_SAMP_REPL, OnChangeSampRepl)
	ON_CBN_SELENDOK(IDC_ASSAYBOX_BPP, OnSelectassay)
	ON_BN_CLICKED(IDC_ADD_CAL_SAMPS, OnAddCalSamps)
	ON_BN_CLICKED(IDC_USE_DEF_CALS, OnUseDefaultCal)
	ON_BN_CLICKED(IDC_USE_EXIST_CALS, OnUseExistingCal)
	ON_BN_CLICKED(IDC_ALLOW_MERGE, OnAllowMerge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Initilization
/////////////////////////////////////////////////////////////////////////////

BOOL CBatchPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	if( Specification::g_oSpecification.Has4thInjector() == false) {
		/*
		 * if there is no fourth injector then hide associated controls and shift other
		 * controls accordingly.
		 */
		CRect oRectP;
		CRect oRect1;
		CRect oRect2;
		CRect oRect3;

		GetDlgItem( IDC_LABEL_REAGENT_P)->GetWindowRect( &oRectP);
		GetDlgItem( IDC_LABEL_REAGENT_1)->GetWindowRect( &oRect1);
		GetDlgItem( IDC_LABEL_REAGENT_2)->GetWindowRect( &oRect2);
		GetDlgItem( IDC_LABEL_REAGENT_3)->GetWindowRect( &oRect3);

		long nShiftReag1 = oRect1.top - oRectP.top;
		long nShiftReag2 = oRect2.top - (oRect3.top + oRectP.top) / 2;

		Specification::MoveWindowUp( GetDlgItem( IDC_LABEL_REAGENT_1), nShiftReag1);
		Specification::MoveWindowUp( GetDlgItem( IDC_REAGENT_1_REQB), nShiftReag1);

		Specification::MoveWindowUp( GetDlgItem( IDC_LABEL_REAGENT_2), nShiftReag2);
		Specification::MoveWindowUp( GetDlgItem( IDC_REAGENT_2_REQB), nShiftReag2);

		GetDlgItem( IDC_LABEL_REAGENT_P)->ShowWindow( SW_HIDE);
		GetDlgItem( IDC_REAGENT_P_REQB)->ShowWindow( SW_HIDE);
	}


	// Default is add cals no default cals and no choice
	AllowDefCals(false);

	// Initialize assay box
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();
	m_assayBox.Init(&pApp->m_assays);

	m_spinSampRepl.SetRange(1,MAX_REPLICATES);
	m_spinSampRepl.SetPos(1);
	m_sampRepl = 1; // In lieu of calling UpdataData
	
	// If batch is defined set assay
	if (m_pBatch != NULL)
	{	// Look for assay name in list box
		ASSERT(m_pBatch->IsKindOf(RUNTIME_CLASS(CBatch)));
		m_assayName = m_pBatch->GetAssayName();
		int iSel = m_assayBox.FindStringExact(0, m_assayName);
		if (iSel != CB_ERR)
		{	// Found a match so make selection
			m_assayBox.SetCurSel(iSel);
		}

		// Also set merge state
		m_allowMerge = m_pBatch->IsMergeAllowed();
	}
	// for Uni
	else if (m_runHidden)
	{
		int iSel = m_assayBox.FindStringExact(0, m_assayName);
		ASSERT (iSel != CB_ERR);
		// Found a match so make selection
		m_assayBox.SetCurSel(iSel);

		// Also set merge state
		m_allowMerge = true;
	}
	else
	{
		m_assayBox.SetCurSel(-1);
	}

	GetLeftoverTubes();

	m_pageInitialized = true;

	// Set Next or Finish button in Property Sheet, as appropriate

	if (m_runHidden) 
	{
		OnSelectassay();
		CDialog::OnOK();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// Set page title
/////////////////////////////////////////////////////////////////////////////

void CBatchPropPage::SetTitle(CString& name)
{
	m_title = name;
	m_psp.pszTitle = LPCTSTR(m_title);
	m_psp.dwFlags |= PSP_USETITLE;
}


/////////////////////////////////////////////////////////////////////////////
// Set proper buttons on page enable
/////////////////////////////////////////////////////////////////////////////

BOOL CBatchPropPage::OnSetActive() 
{
	OnSelectassay();
	if (m_assayDefined == TRUE)
	{
		((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	}
	else
	{
		((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK);
	}

	return CPropertyPage::OnSetActive();
}


/////////////////////////////////////////////////////////////////////////////
// Calculate number of leftover tubes
/////////////////////////////////////////////////////////////////////////////

int CBatchPropPage::GetLeftoverTubes()
{
	m_nAddedSampTubes = m_nTubes - m_nRealCalTubes;
	if (m_pBatch != NULL)
	{	// Also subtract existing tubes
		m_nAddedSampTubes -= m_pBatch->GetNumTubes();
	}

	// Count out sample replicates
	m_nLeftoverTubes = m_nAddedSampTubes % m_spinSampRepl.GetPos();

	UpdateData(FALSE);

	return m_nLeftoverTubes;
}


/////////////////////////////////////////////////////////////////////////////
// Changed number of sample replicates
/////////////////////////////////////////////////////////////////////////////

void CBatchPropPage::OnChangeSampRepl() 
{	// Must do initialized test because the first call to this handler
	// occurs before the other spin controls are created.
	if (m_pageInitialized)
	{
		m_sampRepl = m_spinSampRepl.GetPos();
		GetLeftoverTubes();
	}

}


/////////////////////////////////////////////////////////////////////////////
// Process notification of change of assay selection
/////////////////////////////////////////////////////////////////////////////

void CBatchPropPage::OnSelectassay()
{
	long rv = KO_OK;

	// Get assay name (if valid)
	int iSelect = m_assayBox.GetCurSel();

	if (iSelect != -1)
	{
		m_assayBox.GetLBText(iSelect, m_assayName);

		// Set assay def pointer
		m_pAssay = GetAssayPtr(m_assayName);
		if (m_pAssay != NULL)
		{	// Assay dependent settings
			m_spinSampRepl.SetPos(m_pAssay->GetDefaultRepls());
			AllowDefCals(m_pAssay->DefCalsAllowed());

			// Get number of tubes required for calibration
			rv = GetNumCalTubes();
			if (rv == KO_OK)
			{
				if (m_nTotalCalTubes > 0)
				{
					if (m_pBatch == NULL)
					{	// New batch, No pre-existing cals
						AllowExistingCals(false);
						m_bUseExistingCals = false;							
						AllowDefCals(m_pAssay->DefCalsAllowed());
						m_bUseDefCals = false;
						AllowAddCals(true);
						m_bAddCalSamps = true;				
						UpdateData(false);	// Update dialog
					}
					else
					{	// Existing full or partial batch
						rv = CheckBatchFormat();
						if (rv == KO_ACC_NOT_CALIBRATED ||
							rv == KO_ACC_MISSING_CAL)
						{	// No pre-existing cals
							AllowExistingCals(false);
							m_bUseExistingCals = false;							
							AllowAddCals(m_nTotalCalTubes <=	
								m_nTubes - m_pBatch->GetNumTubes());
							m_bAddCalSamps = m_nTotalCalTubes <=	
								m_nTubes - m_pBatch->GetNumTubes();				
							AllowDefCals(m_pAssay->DefCalsAllowed());
							m_bUseDefCals = m_pAssay->DefCalsAllowed()
								&& !m_bAddCalSamps;
							UpdateData(false);	// Update dialog
						}
						else if (rv != KO_OK)
						{	// Bad format in defined samples
							ErrorReport(rv);
							// Deselcet assay
							iSelect = -1;
						}
						else
						{	// Allow use of existing and enable them
							// by default
							AllowExistingCals(true);
							m_bUseExistingCals = true;							
							AllowDefCals(m_pAssay->DefCalsAllowed());
							m_bUseDefCals = false;
							AllowAddCals(m_nTotalCalTubes <=	
								m_nTubes - m_pBatch->GetNumTubes());
							m_bAddCalSamps = false;				
							UpdateData(false);	// Update dialog
						}
					}
				}
				else
				{
					if (m_pBatch != NULL)
					{	// Check existing samples
						rv = CheckBatchFormat();
					}
					else
					{	// No pre-existing tubes
						rv = KO_OK;
					}

					if (rv == KO_OK)
					{	// No pre-existing conditions
						AllowExistingCals(false);
						m_bUseExistingCals = false;							
						AllowDefCals(false);
						m_bUseDefCals = false;
						AllowAddCals(false);
						m_bAddCalSamps = false;				
						UpdateData(false);	// Update dialog
					}
					else
					{	// Bad format in defined samples
						ErrorReport(rv);
						// Deselcet assay
						iSelect = -1;
					}
				}
			}
			else
			{	// Error getting number of cal tubes
				iSelect = -1;
			}
		}
		else
		{	// Error getting assay pointer
			iSelect = -1;
		}
	}

	if (iSelect == -1)
	{
		m_assayName	= _T("");
		m_pAssay = NULL;

		AllowExistingCals(false);
		m_bUseExistingCals = false;							
		AllowDefCals(false);
		m_bUseDefCals = false;
		AllowAddCals(false);
		m_bAddCalSamps = false;				
		
		UpdateData(false);	// Update dialog
	}

	ErrorReport(ProcessSettings());
}

long CBatchPropPage::GetNumCalTubes()
{
	long rv = KO_OK;
	CACCMgr acc;		// Assay Calc Component

	// Initialize ACC calc method and parameters
	rv = PrepareAcc(&acc);
	if (rv != KO_OK)
	{
		return rv;
	}

	// Generate cal samples in ACC
	rv = acc.CreateCalSamps();
	if (rv != KO_OK)
	{
		return ErrorReport(rv);
	}

	// Save cal samples
	int numCalSamps = acc.GetNumSamples();
	ASSERT (numCalSamps >= 0);

	// Total cal tubes 
	m_nTotalCalTubes = 0;
	for (int i = 0; i < numCalSamps; i++)
	{
		int sampTubes;
		CLucSamp samp(&m_fldProp);	

		// Load sample from ACC
		VERIFY(samp.ReadACC(&acc, i, m_pAssay) == KO_OK);

		// Add num replicates to total
		samp.GetField(SF_REPLS, &sampTubes);
		m_nTotalCalTubes += sampTubes;
	}

	return rv;
}


long CBatchPropPage::ErrorReport(long rv)
{
	UINT msgID = LUC_ID_UNDEF;

	if (rv == KO_OK || rv == KO_LUC_BAD_ASSAY_ID)
	{	// no error reported
		return rv;
	}

	switch (rv)
	{
	case KO_LUC_ADDCALS_NOASSAY:
		msgID = IDS_NO_ASSAY_SEL;
		break;

	case KO_LUC_BAD_ASSAY_ID:
		msgID = IDS_ASSAYDEF_NOT_FOUND;
		break;

	case KO_LUC_EXCEED_MAX_TUBES:
		msgID = IDS_EXCEED_MAX_TUBES;
		break;

	case KO_LUC_ADDCALS_NOMETHOD:
		msgID = IDS_CALC_METH_INVALID;
		break;

	case KO_LUC_BAD_METHOD_ID:
		msgID = IDS_ACMETH_NOT_SUPPORTED;
		break;

	case KO_LUC_ACC_ACPARAMS:
		msgID = IDS_ACPARAMS_INVALID;
		break;

	case KO_LUC_NO_BATCH:
		msgID = IDS_NO_BATCH;
		break;

	case KO_LUC_TOO_FEW_TUBES:
		msgID =  IDS_CANT_ADD_CALS;
		break;

	case KO_LUC_TOO_FEW_CAL_TUBES:
		msgID = IDS_NO_CALS_SPECD;
		break;

	case KO_ACC_BAD_SAMP_TYPE:
		msgID = IDS_BAD_SAMP_TYPE;
		break;
	}

	if (msgID != LUC_ID_UNDEF)
	{
		AfxMessageBox(msgID);
	}
	else
	{
		CString errMsg;
		errMsg.Format(IDS_ACC_SAMPLE_WRITE_ERR, rv, CLccMgr::LookupError( rv));
		AfxMessageBox(errMsg);
	}

	return rv;
}


long CBatchPropPage::GetRunSpecs()
{
	long rv = KO_OK;

	// Set by GetNumCalTubes
	ASSERT(m_pAssay->IsKindOf(RUNTIME_CLASS(CAssayDef)));
	ASSERT(m_pScanChain->IsKindOf(RUNTIME_CLASS(CScanChain)));

	// Initialize LCC 
	rv = PrepareLccRead(m_pAssay, 1, m_nTubes);
	if (rv != KO_OK)
	{
		return rv;
	}

	// Read time and reagent usage
	rv = m_pScanChain->CSGetTimeLeft(&m_mSec);
	if (rv != KO_OK)
	{
		ASSERT(false);
	}
	m_timeStr.Format(IDS_MIN_SEC, m_mSec / 60000, (m_mSec / 1000) % 60);

	rv = m_pScanChain->CSGetReagent(0, &m_reagentP);
	if (rv != KO_OK)
	{
		ASSERT(false);
	}
	m_reagPstr.Format(IDS_REAG_ML, (float)m_reagentP / 1000.0);

	rv = m_pScanChain->CSGetReagent(1, &m_reagent1);
	if (rv != KO_OK)
	{
		ASSERT(false);
	}
	m_reag1str.Format(IDS_REAG_ML, (float)m_reagent1 / 1000.0);

	rv = m_pScanChain->CSGetReagent(2, &m_reagent2);
	if (rv != KO_OK)
	{
		ASSERT(false);
	}
	m_reag2str.Format(IDS_REAG_ML, (float)m_reagent2 / 1000.0);

	rv = m_pScanChain->CSGetReagent(3, &m_reagent3);
	if (rv != KO_OK)
	{
		ASSERT(false);
	}
	m_reag3str.Format(IDS_REAG_ML, (float)m_reagent3 / 1000.0);

	// Close Read procedure
	rv = m_pScanChain->CSClose();

	return rv;
}


void CBatchPropPage::AllowDefCals(bool allowed)
{
	if (m_defCalsAllowed != allowed)
	{	// State has changed
		m_defCalsAllowed = allowed;

		CButton* pCtrl = (CButton*)GetDlgItem(IDC_USE_DEF_CALS);
		ASSERT(pCtrl != NULL);
		pCtrl->EnableWindow(allowed);

		if (!allowed && m_bUseDefCals)
		{
			m_bUseDefCals = false;
			UpdateData(false);
			ProcessSettings();
		}
	}
}

void CBatchPropPage::AllowExistingCals(bool allowed)
{
	if (m_existCalsAllowed != allowed)
	{	// State has changed
		m_existCalsAllowed = allowed;

		CButton* pCtrl = (CButton*)GetDlgItem(IDC_USE_EXIST_CALS);
		ASSERT(pCtrl != NULL);
		pCtrl->EnableWindow(allowed);

		if (!allowed && m_bUseExistingCals)
		{
			m_bUseExistingCals = false;
			UpdateData(false);
			ProcessSettings();
		}
	}
}


void CBatchPropPage::AllowAddCals(bool allowed)
{
	if (m_addCalsAllowed != allowed)
	{	// State has changed
		m_addCalsAllowed = allowed;

		CButton* pCtrl = (CButton*)GetDlgItem(IDC_ADD_CAL_SAMPS);
		ASSERT(pCtrl != NULL);
		pCtrl->EnableWindow(allowed);

		if (!allowed && m_bAddCalSamps)
		{
			m_bAddCalSamps = false;
			UpdateData(false);
			ProcessSettings();
		}
	}
}

void CBatchPropPage::OnAllowMerge()
{
	UpdateData(true);
}

void CBatchPropPage::OnAddCalSamps()
{
	UpdateData(true);
	ErrorReport(ProcessSettings());
}


void CBatchPropPage::OnUseDefaultCal()
{
	UpdateData(true);
	ErrorReport(ProcessSettings());
}

void CBatchPropPage::OnUseExistingCal()
{
	UpdateData(true);
	ErrorReport(ProcessSettings());
}

long CBatchPropPage::ProcessSettings()
{
	long rv = KO_OK;

	if (m_pAssay != NULL)
	{	// m_nTotalCalTubes is set by procedure which gets m_pAssay
		if (m_nTotalCalTubes == 0)
		{
			m_bUseDefCals = false;
			m_bAddCalSamps = false;
			m_bUseExistingCals = false;
		}
		else
		{	// assay requires calibrators
			if (m_bUseDefCals)
			{
				ASSERT(!m_bAddCalSamps && !m_bUseExistingCals);
				m_nRealCalTubes = 0;
			}
			else if (m_bAddCalSamps)
			{
				ASSERT(!m_bUseDefCals && !m_bUseExistingCals);
				m_nRealCalTubes = m_nTotalCalTubes;
				if (m_nRealCalTubes > m_nTubes)
				{
					AllowAddCals(false);
					rv = KO_LUC_TOO_FEW_TUBES;
				}
			}
			else if (m_bUseExistingCals)
			{
				ASSERT(!m_bUseDefCals && !m_bAddCalSamps);
				m_nRealCalTubes = 0;	// cal tubes ADDED
			}
			else
			{	// No calibrators specified
				rv = KO_LUC_TOO_FEW_CAL_TUBES;
			}
		}
		
		// Check max assay tubes
		if (rv == KO_OK && m_nTubes > m_pAssay->GetMaxTubes())
			rv = KO_LUC_EXCEED_MAX_TUBES;

		// Update run time specs
		if (rv == KO_OK && m_nTubes > 0)
		{
			rv = GetRunSpecs();
		}
	}
	else
	{	// No assay selected
		AllowDefCals(false);
		m_nRealCalTubes = 0;
		m_nTotalCalTubes = 0;
		m_spinSampRepl.SetPos(1);

		rv = KO_LUC_BAD_ASSAY_ID;
	}

	GetLeftoverTubes();

	if (rv != KO_OK)
	{	// GetRunSpecs failed or wasn't run
		m_mSec = 0;
		m_reagentP = 0;
		m_reagent1 = 0;
		m_reagent2 = 0;
		m_reagent3 = 0;
	}

	UpdateData(false);	// update dialog

	if (rv == KO_OK)
	{	// Set next button enabled
		ASSERT(m_nAddedSampTubes >= 0);
		m_assayDefined = true;
		((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	}
	else
	{	// Set next button disabled
		m_assayDefined = false;
		((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_BACK);
	}

	return rv;
}

CAssayDef* CBatchPropPage::GetAssayPtr(LPCTSTR assayName)
{
	// Get assay ptr (and return it)
	CAssayDef* pAssay = ((CLuceptApp*)AfxGetApp())->m_assays.GetPtrFromName(assayName);
	if (pAssay == NULL)
	{
		ErrorReport(KO_LUC_BAD_ASSAY_ID);
	}

	return pAssay;
}

////////////////////////////////////////////////////////////////////////
// Set Calculation method and parameters
////////////////////////////////////////////////////////////////////////

long CBatchPropPage::PrepareAcc(CACCMgr* pAcc)
{
	long rv = KO_OK;

	// Make calc method specified in AssayDef active in ACC
	ASSERT(m_pAssay != NULL);
	long calcMethID = m_pAssay->GetCalcMethID();
	if (calcMethID == LUC_ID_UNDEF)
	{
		return ErrorReport(KO_LUC_ADDCALS_NOMETHOD);
	}
	if (!pAcc->SelectMethod(calcMethID))
	{
		return ErrorReport(KO_LUC_BAD_METHOD_ID);
	}

	// Load parameters into ACC
	if (!m_pAssay->InitACC(pAcc))
	{
		return ErrorReport(KO_LUC_ACC_ACPARAMS);
	}

	return rv;
};


////////////////////////////////////////////////////////////////////////
// Set measurement method and parameters
////////////////////////////////////////////////////////////////////////


long CBatchPropPage::PrepareLccRead(CAssayDef* pAssay, int startPos, 
	int nTubes)
{
	long rv;

	// Get Measurement definition ID
	CString measName;
	measName = pAssay->GetMeasName();
	long measDefID = 
		((CLuceptApp*)AfxGetApp())->m_meass.GetIDFromName(measName);
	if (measDefID == LUC_ID_UNDEF)
	{
		AfxMessageBox(IDS_BAD_MEAS_ID);
		return KO_LUC_BAD_METHOD_ID;
	}

	// Get Meas def pointer
	CMeasDef* pMeas = 
		((CLuceptApp*)AfxGetApp())->m_meass.GetDefPtr(measDefID);
	ASSERT(pMeas != NULL);

	// Open Read Procedure
	ASSERT(m_pScanChain != NULL);
	rv= m_pScanChain->CSOpen(LCC_CS_CLASS_READ, pMeas->GetReadProcID());
	if (rv != KO_OK)
	{
		AfxMessageBox(IDS_READ_OPEN_ERR);
		return rv;
	}

	// Set read parameters
	rv = pMeas->InitLccParams(m_pScanChain);
	if (rv != KO_OK)
	{
		AfxMessageBox(IDS_LCC_PARAM_ERR);
		return rv;
	}

	// Set start position
	COleVariant var;
	var.vt = VT_R8;
	var.dblVal = (double)startPos;
	CParam param(var);
	param.m_ID = LCC_PARAM_START_POS;
	VERIFY(m_pScanChain->CSSetParam(param) == KO_OK);

	// Set number of tubes
	var.dblVal = (double)nTubes;
	param.m_value = var;
	param.m_ID = LCC_PARAM_NUM_TUBES;
	VERIFY(m_pScanChain->CSSetParam(param) == KO_OK);

	return KO_OK;
}

long CBatchPropPage::CheckBatchFormat()
{
	CACCMgr acc;
	bool addCals = false;
	long rv =KO_OK;

	ASSERT(m_pAssay != NULL);
	ASSERT(m_pBatch != NULL);

	// Make calc method specified in AssayDef active in ACC
	long calcMethID = m_pAssay->GetCalcMethID();
	if (calcMethID == LUC_ID_UNDEF)
	{
		return ErrorReport(KO_LUC_ADDCALS_NOMETHOD);
	}

	if (!acc.SelectMethod(calcMethID))
	{
		return ErrorReport(KO_LUC_BAD_METHOD_ID);
	}

	// Load parameters into ACC
	if (!m_pAssay->InitACC(&acc))
	{
		return ErrorReport(KO_LUC_ACC_ACPARAMS);
	}

	// Start Validating samples
	rv = acc.StartValidation();
	if (rv != KO_OK)
	{
		return rv;
	}

	// Cycle the samples through the ACC
	int nSamps = m_pBatch->GetNumSamples();
	for (int iSamp = 0; iSamp < nSamps; iSamp++)
	{
		CLucSamp* pSamp = m_pBatch->GetSamplePtr(iSamp);
		ASSERT(pSamp != NULL);

		// Send the samples to the ACC and wait for the validation
		// to fail
		rv = pSamp->WriteACC(&acc, m_pAssay);
		if (rv == KO_ACC_NOT_CALIBRATED)
		{	// Require calibrators
			addCals = true;
		}
		else if (rv != KO_OK)
		{
			return rv;
		}
	}

	// Survived validation
	// Well formatted and properly calibrated

	if (addCals)
	{
		return KO_ACC_NOT_CALIBRATED;
	}
	else
	{
		return KO_OK;
	}
}

int CBatchPropPage::SetupAssay(LPCTSTR assyName)
{
	int rv;

	m_assayName = assyName;
	m_runHidden = true;
	rv = DoModal();
	m_runHidden = false;

	return rv;
}

