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

// ADPgFlag.cpp : implementation file
//

#include "stdafx.h"
#include "lucept.h"
#include "ADPgFlag.h"
#include "ADPrpSh.h"
#include "ultgrid/UGDefine.h"
#include "DaoUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADPgFlag property page

IMPLEMENT_DYNCREATE(CADPgFlag, CPropertyPage)

CADPgFlag::CADPgFlag() : CPropertyPage(CADPgFlag::IDD)
{
	//{{AFX_DATA_INIT(CADPgFlag)
	//}}AFX_DATA_INIT

	m_iFlag = -1;
	m_action = -1;
	m_polarity = TRUE;
	m_condIndex = -1;
	m_newFieldID = -1;
	m_DaoOpsType = -1;
	m_DAOType = -1;
}

CADPgFlag::~CADPgFlag()
{
}

void CADPgFlag::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADPgFlag)
	DDX_Control(pDX, IDC_FLAG_ACTION, m_actionCB);
	DDX_Control(pDX, IDC_FLAG_SEL, m_flagSelCB);
	DDX_Control(pDX, IDC_FLAG_POLARITY, m_polarityCB);
	DDX_Control(pDX, IDC_FIELD_TEST, m_newCondTestDL);
	DDX_Control(pDX, IDC_FIELD_VALUE, m_newCondValEdit);
	DDX_Control(pDX, IDC_FLAG_FIELD, m_newCondFieldDL);
	DDX_Control(pDX, IDC_FLAG_COND_LIST, m_condListLB);
	DDX_Control(pDX, IDC_RST_CONDITION, m_rstCondBtn);
	DDX_Control(pDX, IDC_DEL_CONDITION, m_delCondBtn);
	DDX_Control(pDX, IDC_ADD_CONDITION, m_addCondBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADPgFlag, CPropertyPage)
	//{{AFX_MSG_MAP(CADPgFlag)
	ON_CBN_SELCHANGE(IDC_FLAG_SEL, OnSelchangeFlagSel)
	ON_LBN_SELCHANGE(IDC_FLAG_COND_LIST, OnSelchangeFlagCondList)
	ON_CBN_SELCHANGE(IDC_FLAG_FIELD, OnSelchangeFlagField)
	ON_EN_KILLFOCUS(IDC_FIELD_VALUE, OnKillfocusFieldValue)
	ON_EN_UPDATE(IDC_FIELD_VALUE, OnUpdateFieldValue)
	ON_CBN_SELCHANGE(IDC_FIELD_TEST, OnSelchangeFieldTest)
	ON_BN_CLICKED(IDC_ADD_CONDITION, OnAddCondition)
	ON_BN_CLICKED(IDC_RST_CONDITION, OnRstCondition)
	ON_BN_CLICKED(IDC_DEL_CONDITION, OnDelCondition)
	ON_CBN_SELCHANGE(IDC_FLAG_ACTION, OnSelchangeFlagAction)
	ON_CBN_SELCHANGE(IDC_FLAG_POLARITY, OnSelchangeFlagPolarity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADPgFlag message handlers
////////////////////////////////////////////////////////////////////////
// Save assay def pointer -- called from property sheet's contructor
////////////////////////////////////////////////////////////////////////

long CADPgFlag::SetAssayDef(CAssayDef * pAssayDef)
{
	m_pAD = pAssayDef;

	return KO_OK;
}

////////////////////////////////////////////////////////////////////////
// Create and initialize controls -- 
// called after property sheet's DoModal (m_pAD is valid)
////////////////////////////////////////////////////////////////////////

BOOL CADPgFlag::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CString txt;

	// Initialize flag select combo box
	m_flagSelCB.ResetContent();
	// Don't sort the list!
	ASSERT((m_flagSelCB.GetStyle() & CBS_SORT) != CBS_SORT);
	m_flagSelCB.AddString(m_fldProp.GetIDTitle(SF_FLAG1));
	m_flagSelCB.AddString(m_fldProp.GetIDTitle(SF_FLAG2));
	m_flagSelCB.AddString(m_fldProp.GetIDTitle(SF_FLAG3));
	
	// Initialize polarity list
	m_polarityCB.ResetContent();
	// Don't sort the list!
	ASSERT((m_polarityCB.GetStyle() & CBS_SORT) != CBS_SORT);
	VERIFY(txt.LoadString(IDS_FALSE));
	m_polarityCB.AddString(txt);
	VERIFY(txt.LoadString(IDS_TRUE));
	m_polarityCB.AddString(txt);

	// Initialize action list
	m_actionCB.ResetContent();
	// Don't sort the list!
	ASSERT((m_actionCB.GetStyle() & CBS_SORT) != CBS_SORT);
	VERIFY(txt.LoadString(IDS_FLAG_ACT_NOTHING));
	m_actionCB.AddString(txt);
	VERIFY(txt.LoadString(IDS_FLAG_ACT_CONT));
	m_actionCB.AddString(txt);
	VERIFY(txt.LoadString(IDS_FLAG_ACT_PAUSE));
	m_actionCB.AddString(txt);
	VERIFY(txt.LoadString(IDS_FLAG_ACT_STOP));
	m_actionCB.AddString(txt);

	// Initialize field list
	m_newCondFieldDL.ResetContent();
	for (int iFld = 0; iFld < m_fldProp.GetFieldCount(); iFld++)
	{	// Add field name to box
		int iListBox = m_newCondFieldDL.AddString(m_fldProp.GetIndexName(iFld));
		// Save field ID with item
		m_newCondFieldDL.SetItemData(iListBox, m_fldProp.GetIndexID(iFld));
	}

	// Set current flag
	m_flagSelCB.SetCurSel(0);	
	OnSelchangeFlagSel();

	return TRUE;  
}

////////////////////////////////////////////////////////////////////////
// Do anything?
////////////////////////////////////////////////////////////////////////

BOOL CADPgFlag::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

////////////////////////////////////////////////////////////////////////
// Check validity of parameters specified by controls
// Produce error messages if necessary.
////////////////////////////////////////////////////////////////////////

long CADPgFlag::ChkAssayDef(CAssayDef * pAssayDef)
{
	long rv = KO_OK;

	return rv;
}
////////////////////////////////////////////////////////////////////////
// Check validity of control values
////////////////////////////////////////////////////////////////////////

BOOL CADPgFlag::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

////////////////////////////////////////////////////////////////////////
// Copy contents of controls back to assay def
// Called regardless of whether page is (or was ever) visible
// It's too late to check validity here
////////////////////////////////////////////////////////////////////////

void CADPgFlag::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}


void CADPgFlag::OnSelchangeFlagSel() 
{	// Read flag params from assay def and init controls
	ASSERT(m_pAD != NULL);

	m_iFlag = m_flagSelCB.GetCurSel();
	ASSERT(m_iFlag != -1);

	// Set polarity and action
	m_polarity = m_pAD->m_aFlags[m_iFlag].m_polarity;
	if (m_polarity == FALSE)
	{
		m_polarityCB.SetCurSel(0);
	}
	else
	{
		m_polarityCB.SetCurSel(1);
	}

	m_action = m_pAD->m_aFlags[m_iFlag].m_action;
	m_actionCB.SetCurSel(m_action);

	// Add current list of conditions
	m_condListLB.ResetContent();
	int nCond = m_pAD->m_aFlags[m_iFlag].m_condList.GetSize();
	for (int iCond = 0; iCond < nCond; iCond++)
	{
		m_condListLB.AddString(m_pAD->m_aFlags[m_iFlag].m_condList[iCond]);
	}
	OnSelchangeFlagCondList();

	// Reset new condition
	m_newCondFieldDL.SetCurSel(-1);
	OnSelchangeFlagField();
}

void CADPgFlag::OnSelchangeFlagCondList() 
{
	if (m_condListLB.GetCurSel() != LB_ERR)
	{
		m_delCondBtn.EnableWindow(true);
	}
	else
	{
		m_delCondBtn.EnableWindow(false);
	}
}

void CADPgFlag::OnSelchangeFlagField() 
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	int sel = m_newCondFieldDL.GetCurSel();
	
	if (sel == -1)
	{	// No selection
		m_newCondTestDL.ResetContent();
		m_newCondTestDL.EnableWindow(false);
		
		m_newFieldID = -1;
		m_newFieldName.Empty();
		m_DAOType = -1;
	}
	else
	{	// Set content of test based on data type
		m_newFieldID = m_newCondFieldDL.GetItemData(sel);
		m_newFieldName = m_fldProp.GetIDTitle(m_newFieldID);
		m_DAOType = m_fldProp.GetIDDAOType(m_newFieldID);
		m_DaoOpsType = DaoUtils::SQLOperatorsTypeGet(m_DAOType);

		m_newCondTestDL.ResetContent();
		int i = 0;
		while(*DaoUtils::SQLOperators[m_DaoOpsType][i]) 
		{	// while operator string not empty
			m_newCondTestDL.AddString(DaoUtils::SQLOperators[m_DaoOpsType][i++]);
		}
		
		m_newCondTestDL.EnableWindow(true);
	}

	m_newCondTestDL.SetCurSel(-1);
	OnSelchangeFieldTest();


	// Disable add condition button
	m_addCondBtn.EnableWindow(false);
}


void CADPgFlag::OnKillfocusFieldValue() 
{	// Get edit box contents
	m_newCondValEdit.GetWindowText(m_newFieldVal);

	if 	(IsWellFormatted(m_newFieldVal, m_DAOType))
	{	// Allow adding new condition
		m_addCondBtn.EnableWindow(true);
	}
	else
	{
		m_addCondBtn.EnableWindow(false);
	}
}

void CADPgFlag::OnUpdateFieldValue() 
{
	m_addCondBtn.EnableWindow(true);
}


void CADPgFlag::OnSelchangeFieldTest() 
{
	int selCondIndex = m_newCondTestDL.GetCurSel();
	if (selCondIndex != -1)
	{
		m_newCondTestDL.GetWindowText(m_newFieldTest);
		m_newCondValEdit.EnableWindow(true);
	}
	else
	{
		m_newFieldTest.Empty();
		m_newFieldVal.Empty();
		m_newCondValEdit.SetWindowText(m_newFieldVal);
		m_newCondValEdit.EnableWindow(false);
	}
}

void CADPgFlag::OnAddCondition() 
{
	if 	(IsWellFormatted(m_newFieldVal, m_DAOType))
	{	// Create condition string
		CString condition;

		condition = m_newFieldName;
		condition += _T(" ");
		condition += m_newFieldTest;
		condition += _T(" ");
		condition += Delimited(m_newFieldVal, m_DAOType);

		// Add to condition list
		ASSERT(m_pAD != NULL);
		m_pAD->m_aFlags[m_iFlag].m_condList.Add(condition);
	
		// Reset the page
		OnSelchangeFlagSel();
	}
	else
	{
		m_addCondBtn.EnableWindow(false);
	}
}

void CADPgFlag::OnRstCondition() 
{
	// Clear condition list
	ASSERT(m_pAD != NULL);
	m_pAD->m_aFlags[m_iFlag].m_condList.RemoveAll();

	// Reset condition list box
	m_condListLB.ResetContent();
	OnSelchangeFlagCondList();
}

bool CADPgFlag::IsWellFormatted(LPCTSTR str, short DAOType)
{
	VARTYPE vt;	

	switch (DAOType)
	{
	// Only types supported
    case dbText:
		vt = VT_BSTR;
		break;

	case dbBoolean:
		vt = VT_BOOL;
		break;

    case dbDouble:
		vt = VT_R8 ;
		break;

    case dbDate:
		vt = VT_DATE ;
		break;

    case dbByte:
    case dbInteger:
    case dbLong:
    case dbCurrency:
    case dbSingle:
    case dbBinary:
    case dbLongBinary:
    case dbMemo:
    case dbGUID:
    case dbBigInt:
    case dbVarBinary:
    case dbChar:
    case dbNumeric:
    case dbDecimal:
    case dbFloat:
    case dbTime:
    case dbTimeStamp:
	default:
		vt = VT_EMPTY;
    }

	// Create a variant from the string
	//! must be UNICODE for VarianChangeTypeEx
	COleVariant var(str, VT_BSTR);	
	
	HRESULT result = ::VariantChangeTypeEx(var, var, ::GetUserDefaultLCID(), 0, vt);

	if (SUCCEEDED(result))
	{
		return true;
	}
	else
	{
		if (result & DISP_E_TYPEMISMATCH)
		{	// Bad string
			switch(vt)
			{
			case VT_DATE:	// and time
				AfxMessageBox(_T("Requires date/time value!"));
				break;

			case VT_R8:
				AfxMessageBox(_T("Requires numeric value!"));
				break;

			case VT_BOOL:
				AfxMessageBox(_T("Requires boolean value!"));
				break;

			case VT_BSTR:
				ASSERT(vt != VT_BSTR);
				break;

			default:
				ASSERT(vt == VT_BSTR);
				return false;
			}

			return false;
		}
		else
		{
			return false;
			ASSERT(result == S_OK);
		}
	}
}

CString CADPgFlag::Delimited(LPCTSTR content, short DAOType)
{
	CString deltdText;
	CString delimiter;

	switch(DAOType) 
	{
	case dbText:
		delimiter = _T("'");
		break;

	case dbDate:
	case dbTime:
		delimiter = _T("#");
		break;

	case dbBoolean:
	case dbDouble:
	case dbFloat:
	case dbInteger:
	case dbLong:
	case dbNumeric:
	case dbSingle:
	default:
		delimiter.Empty();
		break;
	}

	deltdText = delimiter;
	deltdText += content;
	deltdText += delimiter;

	return deltdText;
}

void CADPgFlag::OnDelCondition() 
{
	int iCond = m_condListLB.GetCurSel();

	if (iCond != LB_ERR)
	{
		ASSERT(m_pAD != NULL);
		ASSERT(m_pAD->m_aFlags[m_iFlag].m_condList.GetSize() > iCond);
		m_pAD->m_aFlags[m_iFlag].m_condList.RemoveAt(iCond);
		
		// Update page
		OnSelchangeFlagSel();
	}
	else
	{
		m_delCondBtn.EnableWindow(false);
	}
}

void CADPgFlag::OnSelchangeFlagAction() 
{
	int selActionIndex = m_actionCB.GetCurSel();
	if (selActionIndex != -1)
	{
		if (m_iFlag != -1)
		{
			ASSERT(m_pAD != NULL);
			m_pAD->m_aFlags[m_iFlag].m_action = (CUserFlag::EFlagAct)selActionIndex;
		}
	}
}

void CADPgFlag::OnSelchangeFlagPolarity() 
{
	int selPolarity = m_polarityCB.GetCurSel();
	if (selPolarity != -1)
	{
		if (m_iFlag != -1)
		{
			ASSERT(m_pAD != NULL);
			m_pAD->m_aFlags[m_iFlag].m_polarity = selPolarity != FALSE;
		}
	}
}
