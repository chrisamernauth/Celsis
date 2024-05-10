// AddDCals.cpp: implementation of the CAddDefCals class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddDCals.h"
#include "Lucept.h"
#include "AssyDef.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CAddDefCals, CConductor)

CAddDefCals::CAddDefCals()
{
	m_iBatch = -1;
	m_pBatch = NULL;
}

CAddDefCals::~CAddDefCals()
{
}

BEGIN_MESSAGE_MAP(CAddDefCals, CConductor)
	//{{AFX_MSG_MAP(CAddDefCals)
    ON_REGISTERED_MESSAGE(WM_CONDUCTOR_BEGIN_FINI, Fini)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWklView diagnostics
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CAddDefCals::AssertValid() const
{
	CView::AssertValid();
}

void CAddDefCals::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWklDoc* CAddDefCals::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWklDoc)));
	return (CWklDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CAddDefCals drawing

void CAddDefCals::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

////////////////////////////////////////////////////////////////////////
// CAddDefCals message handlers
////////////////////////////////////////////////////////////////////////

void CAddDefCals::OnInitialUpdate() 
{
	// Call base class
	CConductor::OnInitialUpdate();
}

long CAddDefCals::Play()
{
	long rv;

	// Get batch pointer
	m_iBatch = GetDocument()->GetCurBatch();
	m_pBatch = GetDocument()->GetBatchPtr(m_iBatch);
	if (m_pBatch == NULL)
		return ErrorReport(KO_LUC_NO_BATCH);

	// Get Assay Pointer
	CAssayDef* pAssayDef;
	rv = m_pBatch->GetAssayPtr(&pAssayDef);
	if (rv != KO_OK)
		return ErrorReport(rv);
	
	// Make calc method specified in AssayDef active in ACC
	long calcMethID = pAssayDef->GetCalcMethID();
	if (calcMethID == LUC_ID_UNDEF)
		return ErrorReport(KO_LUC_ADDCALS_NOMETHOD);
	if (!m_pAcc->SelectMethod(calcMethID))
		return ErrorReport(KO_LUC_BAD_METHOD_ID);

	// Load parameters into ACC
	if (!pAssayDef->InitACC(m_pAcc))
		return ErrorReport(KO_LUC_ACC_ACPARAMS);
	
	// Generate cal samples in ACC
	rv = m_pAcc->CreateCalSamps();
	if (rv != KO_OK)
		return ErrorReport(rv);

	// Add samples to batch
	// !!! Insert samples at beginning of batch
	int numCals = m_pAcc->GetNumSamples();
	ASSERT (numCals >= 0);
	if (numCals == 0)
	{
		AfxMessageBox(IDS_NO_CAL_SAMPS);
		PostMessage(WM_CONDUCTOR_BEGIN_FINI, 0, KO_OK);
		return KO_OK;
	}

	for (int i = 0; i < numCals; i++)
	{
		// Add samples to start of batch
		rv = m_pBatch->InsertSample(i);
		if (rv != KO_OK)
			return ErrorReport(rv);

		CLucSamp* pSamp = m_pBatch->GetSamplePtr(i);

		// Load sample from ACC
		VERIFY(pSamp->ReadACC(m_pAcc, i, pAssayDef) 
			== KO_OK);

		// Change status to Defaul Cal
		pSamp->UpdateStatus(LUC_SSTAT_DEFCAL);
		pSamp->SetField(SF_VIRTUAL, LUC_VSTAT_DEFCAL);
	}

	// Notify views of new data
	GetDocument()->SetModifiedFlag();
	CWklMods mods(CWklMods::NATURE_ADD_SAMP, CWklMods::SCOPE_BATCH, 
		m_iBatch);
	GetDocument()->UpdateAllViews(this, 0, &mods);

	PostMessage(WM_CONDUCTOR_BEGIN_FINI, 0, KO_OK);
	return KO_OK;
}


LRESULT  CAddDefCals::Fini(WPARAM wParam, LPARAM lParam)
{
	UnregisterWithLcc();

	// Notify Patron
	m_pPatron->PostMessage(WM_CONDUCT_FINI, (WPARAM)0, (LPARAM)wParam);

	return (LRESULT)true;
}


long CAddDefCals::ErrorReport(long rv)
{
	UINT msgID = LUC_ID_UNDEF;

	switch (rv)
	{
	case KO_LUC_BATCH_NO_ASSAY:
		msgID = IDS_NO_ASSAY_SEL;
		break;

	case KO_LUC_BATCH_BAD_ASSAY:
		msgID = IDS_ASSAYDEF_NOT_FOUND;
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
	
	case KO_LUC_BATCH_READ_ONLY:
		msgID = IDS_BATCH_READ_ONLY;

	default:
		break;
	}

	if (msgID != LUC_ID_UNDEF)
		AfxMessageBox(msgID);

	PostMessage(WM_CONDUCTOR_BEGIN_FINI, 0, rv);

	return rv;
}

