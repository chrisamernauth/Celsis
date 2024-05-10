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
ITL				03-Apr-02			Amended by CAR 887/003

************************************************************************************/

// MDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDefDlg.h"
#include "LuceErr.h"
#include "Lucept.h"
#include "MainFrm.h"
#include "LccMgr.h"
#include "LumActor.h"
#include "UsrAdmin.h"
#include "Event.h"
#include "SystemLog.h"
#include "Specification.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasDefDlg dialog


CMeasDefDlg::CMeasDefDlg(UINT dialogID, CWnd* pParent /*=NULL*/)
	: CDialog(dialogID, pParent)
{
	//{{AFX_DATA_INIT(CMeasDefDlg)
	m_createDate		= _T("");
	m_modDate			= _T("");
	m_creator			= _T("");
	m_maxParamVal		= _T("");
	m_minParamVal		= _T("");
	m_defaultParamVal	= _T("");
	//}}AFX_DATA_INIT

	if (dialogID == IDD_MANUAL_MODE)
	{
		m_procClass = LCC_CS_CLASS_ACTION;
	}
	else
	{
		m_procClass = LCC_CS_CLASS_READ;
	}
}

void CMeasDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasDefDlg)
	DDX_Control(pDX, IDC_MD_PROTO_PARMS,m_procParams);
	DDX_Control(pDX, IDC_MD_PROTOCOL,	m_procedures);
	DDX_Text(pDX, IDC_MD_CREATED_ON,	m_createDate);
	DDX_Text(pDX, IDC_MD_MODIFIED,		m_modDate);
	DDX_Text(pDX, IDC_MD_CREATOR,		m_creator);
	DDX_Text(pDX, IDC_MD_MODIFIER,		m_modifier);
	DDX_Text(pDX, IDC_MD_MAX_VAL,		m_maxParamVal);
	DDX_Text(pDX, IDC_MD_MIN_VAL,		m_minParamVal);
	DDX_Text(pDX, IDC_MD_DEF_VAL,		m_defaultParamVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasDefDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasDefDlg)
	ON_CBN_SELCHANGE(IDC_MD_PROTOCOL,	 OnSelchangeMdProc)
	ON_LBN_SELCHANGE(IDC_MD_PROTO_PARMS, OnSelchangeMdProcParms)
	ON_CBN_KILLFOCUS(IDC_MD_PPARM_VAL,	 OnKillfocusMdPparmVal)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_GO, OnGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasDefDlg message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CMeasDefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add measurement name to dialog box title
	CString dlgTitle;
	GetWindowText(dlgTitle);
	ASSERT(m_pMeasDef != NULL);
	dlgTitle += m_pMeasDef->GetName();
	SetWindowText(dlgTitle);

	// Add creator, and creation and modification dates
	m_creator	= m_pMeasDef->GetCreator();
	m_modifier	= m_pMeasDef->GetModifier();

	// Ole Date format Bug fix!
	// This code prevents errors when Regional date settings are
	// not English
	{
		CSetLocale l(LC_TIME);
		m_createDate = m_pMeasDef->GetCreateDate().Format(_T("%x %X"));
		m_modDate	 = m_pMeasDef->GetModDate().Format(_T("%x %X"));
	}

	// Create Parameter edit box initially empty
	CParamEx tempParam;
	ASSERT(tempParam.m_value.vt == VT_EMPTY);
	m_pParamBox = new CParamBoxEx(&tempParam);
	VERIFY(m_pParamBox->Create(IDC_MD_PPARM_VAL, this));
	
	// Connect to the LCC
	if (m_lcc.RegisterWithLcc() == KO_OK)
	{	// Init Measurement Procedure Droplist 
		InitMeasProcBox(m_pMeasDef->GetReadProcID());

		// This call is particularly important for the static strings
		UpdateData(false);
	}
	else
	{
		OnCancel();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// OK
/////////////////////////////////////////////////////////////////////////////

void CMeasDefDlg::OnOK() 
{
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();

	// Do data validation and store results in assay definition object
	VERIFY(UpdateData(true));

	//if OK was pressed, we assume a change was made
	m_pMeasDef->SetModifier(gGateKeeper.CurrentUserNameGet());
	m_pMeasDef->SetModDate(COleDateTime::GetCurrentTime()); //i.e., now

	// Check Measurement Procedure
	if(m_procedures.GetCurSel() == -1)
	{	// No measurement selected
		AfxMessageBox(IDS_READ_PROC_INVALID);
		return;
	}

	// Write meas def to disk
	m_pMeasDef->WriteToDisk();

	// *** SYSTEM LOG **********************************************************
	CLogEvent	NewEvent;							       // Create a new event   *
	CString temp;
	temp = m_pMeasDef->GetFileName()+_T(" : ")+m_pMeasDef->GetName();
	NewEvent.EventSet(_T("Measurement Definition edited "),// Event name           *
						temp);						   // Event details	       *
	CSystemLog::g_oSystemLog.EventAdd( NewEvent);
	// *** SYSTEM LOG **********************************************************

	// Obsolete any existing assays
	// Ignore error if none exist
	pApp->m_meass.Obsolete(m_pMeasDef->GetName());

	// Read from disk into meass object 
	pApp->m_meass.InitFromDisk(m_pMeasDef->GetID());

	// Free objects on the heap
	CleanUp();	

	CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// Go
/////////////////////////////////////////////////////////////////////////////

void CMeasDefDlg::OnGo() 
{
	CString errMsg;
	CLumActor actor;

	// Check for Action Procedure
	if(m_procedures.GetCurSel() == -1)
	{	// No action selected
		AfxMessageBox(IDS_NO_ACTION);
		return;
	}

	long rv = actor.Init(this);
	if (rv != KO_OK)
	{
		return;
	}

	// Disable buttons
	CWnd* control = GetWindow(GW_CHILD);
	while (control != NULL)
	{	// disable child window (control)
		control->EnableWindow(false);

		// Check if this is last child (control)
		if (control->GetSafeHwnd() != control->GetWindow(GW_HWNDLAST)->GetSafeHwnd())
		{	// Not last control, get next control
			control = control->GetWindow(GW_HWNDNEXT);
		}
		else
		{
			control = NULL;
		}
	}

	rv = actor.CSOpen(m_procClass, m_pMeasDef->GetReadProcID());
	if (rv == KO_OK)
	{
		rv = m_pMeasDef->InitLccParams(&actor);
		if (rv == KO_OK)
		{
			rv = actor.DoIt();
		}
	}

	if (rv != KO_OK)
	{
		errMsg.Format(IDS_LCC_EXCEPTION, rv, CLccMgr::LookupError( rv));
		AfxMessageBox(errMsg);
	}

	actor.Release();

	// Re-enable buttons 
	control = GetWindow(GW_CHILD);
	while (control != NULL)
	{	// Enable child window (control)
		control->EnableWindow(true);

		// Check if this is last child (control)
		if (control->GetSafeHwnd() != control->GetWindow(GW_HWNDLAST)->GetSafeHwnd())
		{	// Not last control, get next control
			control = control->GetWindow(GW_HWNDNEXT);
		}
		else
		{
			control = NULL;
		}
	}
	return;
}


/////////////////////////////////////////////////////////////////////////////
// Init Measurement Procedure Droplist
/////////////////////////////////////////////////////////////////////////////

BOOL CMeasDefDlg::InitMeasProcBox(long selectID)
{
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();

	int numProcs = pApp->m_meass.GetProtocolCount();

	if(numProcs == 0)
	{	// Display error condition in box and disable
		CString msg;
		msg.LoadString(IDS_NO_ACMETH_DEF);
		m_procedures.AddString(msg);
		m_procedures.SetCurSel(0);
		m_procedures.EnableWindow(FALSE);
	}
	else
	{
		IDNAME *idnm_pair; //ID/Name pair
		for (int i = 0; i < numProcs; i++)
		{	
			idnm_pair = NULL;
			idnm_pair = pApp->m_meass.GetProtocol(i);
			if(idnm_pair)
			{
				int iBoxSel = m_procedures.AddString(idnm_pair->protocolName);
				VERIFY(PUtil::SetLongLBData(m_procedures, idnm_pair->protocolID, iBoxSel));
				if (idnm_pair->protocolID == selectID)
				{
					m_procedures.SetCurSel(iBoxSel);
				}
			}
		}

		if (m_procedures.GetCurSel() == LB_ERR && selectID != LUC_ID_UNDEF)
		{	// Read procedure requested but not found
			CString errMsg;
			errMsg.Format(IDS_READ_PROC_INVALID, selectID);
			AfxMessageBox(errMsg);

			// Clear Procedure ID
			m_pMeasDef->SetReadProc(LUC_ID_UNDEF, _T(""));

			// Clear selection
			m_procedures.SetCurSel(-1);
		}

		if (m_procedures.GetCurSel() != LB_ERR)
		{	// A procedure was already defined so initialize
			// its protocol ID window and parameter list.
			// Note that OnSelchangeMdProc will not retain
			// the original values

			//clear protocol window during transition stage
			GetDlgItem(IDC_PROTOCOL_ID_BOX)->SetWindowText(_T(""));
			//m_procedures.SetWindowText(_T(""));

			//write parameter list: takes several seconds
			BeginWaitCursor();
			m_procParams.ResetContent();
			int numParams = m_pMeasDef->GetNumParams();
			for (int iParam = 0; iParam < numParams; iParam++)
			{	// Add param name
				// LCC reads the names of the parameters from the lcc.ini file. UTF is not treated correctly and
				// the escape character 0xc2 is interpreted as a captial A with a hat on. This is a quick
				// fix to sort out the mu and degree symbol.
				CString strName = m_pMeasDef->GetParam(iParam).m_name;

				if( !Specification::g_oSpecification.HasTemperatureControl()) {
					if( strName.Find( _T("Temperature")) != -1) {
						continue;
					}
				}

				strName.Remove( _T('\xC2'));

				int iBoxSel = m_procParams.AddString( strName);

				// Set param id
				VERIFY(PUtil::SetLongLBData(m_procParams,m_pMeasDef->GetParam(iParam).m_ID, iBoxSel));
			}

			// Deselect all parameters
			m_procParams.SetCurSel(-1);

			//update protocol ID window
			CString tmp;
			tmp.Format(_T("%ld"), m_pMeasDef->GetReadProcID());
			GetDlgItem(IDC_PROTOCOL_ID_BOX)->SetWindowText(tmp);
			GetDlgItem(IDCANCEL)->SetFocus();
			EndWaitCursor();
		}
	}
	return TRUE;
}

void CMeasDefDlg::OnSelchangeMdProc() 
{
	// No method selected
	if (m_procedures.GetCurSel() == -1)
	{	// clear proc ID window
		GetDlgItem(IDC_PROTOCOL_ID_BOX)->SetWindowText(_T(""));
		m_procedures.SetWindowText(_T(""));

		// Empty parameter list box
		m_procParams.ResetContent();

		// Empty parameter value edit box
		m_pParamBox->ResetContent();
		return;
	}

	BeginWaitCursor();

	// Get read procedure ID and procName
	long cur_sel = m_procedures.GetCurSel();
	ASSERT(cur_sel >= 0);
	long	procID	 = PUtil::GetLongLBData(m_procedures, cur_sel);
	CString procName;
	m_procedures.GetLBText(cur_sel, procName);
	//m_procedures.GetWindowText(procName);
	if (procID != LUC_ID_UNDEF)
	{	// clear some windows during transition
		GetDlgItem(IDC_PROTOCOL_ID_BOX)->SetWindowText(_T(""));
		m_procedures.SetWindowText(_T(""));
		m_procParams.ResetContent();

		// Initialize measurement definition if changed
		if (m_pMeasDef->GetReadProcID() != procID)
		{
			m_pMeasDef->SetReadProc(procID, procName, m_procClass);
			VERIFY(m_pMeasDef->InitParamList(m_lcc.GetLccMgr()));
		}

		// Load the parameter list box with the new parameter list
		int numParams = m_pMeasDef->GetNumParams();
		ASSERT(numParams >= 0);
		for (int iParam = 0; iParam < numParams; iParam++)
		{	// Add param name
			// LCC reads the names of the parameters from the lcc.ini file. UTF is not treated correctly and
			// the escape character 0xc2 is interpreted as a captial A with a hat on. This is a quick
			// fix to sort out the mu and degree symbol.
			CString strName = m_pMeasDef->GetParam(iParam).m_name;

			if( !Specification::g_oSpecification.HasTemperatureControl()) {
				if( strName.Find( _T("Temperature")) != -1) {
					continue;
				}
			}

			strName.Remove( _T('\xC2'));

			int iBoxSel = m_procParams.AddString( strName);

			// Set param id
			VERIFY(PUtil::SetLongLBData(m_procParams,m_pMeasDef->GetParam(iParam).m_ID, iBoxSel));
		}

		//update protocol ID window with new protocol ID
		CString tmp;
		tmp.Format(_T("%ld"), procID);
		GetDlgItem(IDC_PROTOCOL_ID_BOX)->SetWindowText(tmp);

		//unselect the new protocol name so it doesn't invite editing
		//m_procedures.SetEditSel(-1, 0); //<-doesn't work. why?
		GetDlgItem(IDOK)->SetFocus();
	}
	else
	{
		AfxMessageBox(IDS_SELECT_MEAS_METH);
	}

	// Deselect all parameters
	m_procParams.SetCurSel(-1);

	EndWaitCursor();	
}

void CMeasDefDlg::OnSelchangeMdProcParms() 
{	// Get param ID
	long paramID = PUtil::GetLongLBData(m_procParams);
	
	// Get Extended parameter from Meas Def
	CParamEx* pParam = m_pMeasDef->GetParamExPtrFromID(paramID);
	ASSERT(pParam != NULL);
	m_curParamX = *pParam;

	// Re-init control
	VERIFY(m_pParamBox->InitParamExData(m_curParamX) == KO_OK);

	// Set Max, Min and Default values from extended parameter
	COleVariant textVar;
	HRESULT result;
	LCID locale = ::GetUserDefaultLCID();
	
	m_maxParamVal = _T("");
	m_minParamVal = _T("");
	m_defaultParamVal = _T("");

	result = ::VariantChangeTypeEx(&textVar, &m_curParamX.m_max,locale, 0, VT_BSTR);
	if (SUCCEEDED(result))
	{
		m_maxParamVal = textVar.bstrVal;
	}

	result = ::VariantChangeTypeEx(&textVar, &m_curParamX.m_min,locale, 0, VT_BSTR);
	if (SUCCEEDED(result))
	{
		m_minParamVal = textVar.bstrVal;
	}

	result = ::VariantChangeTypeEx(&textVar, &m_curParamX.m_default,locale, 0, VT_BSTR);
	if (SUCCEEDED(result))
	{
		m_defaultParamVal = textVar.bstrVal;
	}

	// Parameter box set active
	m_pParamBox->SetFocus();

	// Update dialog data
	UpdateData(false);
}

void CMeasDefDlg::CleanUp()
{
	m_lcc.UnregisterWithLcc();

	// Release Variant Edit control
	delete m_pParamBox;
}

void CMeasDefDlg::OnCancel() 
{
	CleanUp();
	CDialog::OnCancel();
}

void CMeasDefDlg::OnKillfocusMdPparmVal() 
{
	if (m_procParams.GetCurSel() == -1)
	{	// No param selected
		return;
	}

	// Get Parameter ID 
	long paramID = m_procParams.GetItemData(m_procParams.GetCurSel());
	// Get new parameter value from Variant edit box
	CParamEx newParam;
	long rv = m_pParamBox->GetParamEx(newParam);

	// Check if parameter value was acceptable
	if (rv == KO_OK)
	{	// Save setting in measurement definition 
		CParamEx* pParam = m_pMeasDef->GetParamExPtrFromID(paramID);
		ASSERT(pParam != NULL);

		*pParam = newParam;
	}
	else
	{
		if (rv == KO_PHUT_ABOVE_MAX)
		{
			AfxMessageBox(IDS_PARAM_TOO_BIG);
		}
		else if (rv == KO_PHUT_BELOW_MIN)
		{
			AfxMessageBox(IDS_PARAM_TOO_SMALL);	
		}
		else if (rv == KO_PHUT_INVALID_ENTRY)
		{
			AfxMessageBox(IDS_BAD_PAR_VAL);
		}
		else
		{
			ASSERT(false);
		}
		// Re-init control
		m_pParamBox->InitParamData(&m_curParamX);
		m_pParamBox->SetFocus();
	}
}

void CMeasDefDlg::PostNcDestroy() 
{	
	CDialog::PostNcDestroy();
}
