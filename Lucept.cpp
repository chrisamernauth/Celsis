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

// Lucept.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <WinVer.h>
#include "Lucept.h"
#include "resource.h"

#include "WklDoc.h"
#include "WklView.h"
#include "WklFrame.h"

#include "SampView.h"
#include "SampFrm.h"

#include "RptDoc.h"
#include "RptRibbonView.h"
#include "RptFrame.h"

#include "LucyDoc.h"
#include "LucyView.h"
#include "LucyFrm.h"
#include "BCFrame.h"
#include "BatCalc.h"
#include "LccView.h"
#include "LccVDoc.h"

#include "LccDef.h"
#include "LucOle.h"
#include "LCCUser.h"

#include "LccMgr.h"

#include "LuceErr.h"

#include "UsrAdmin.h"

#include "SetupDlg.h"
#include "IniFiles.h"

#include "AssyPrDc.h"
#include "AssyPrVw.h"
#include "MeasPrDc.h"
#include "MeasPrVw.h"
#include "SystemLog.h"
#include "SysLogView.h"
#include "SystemLogDoc.h"

#include "Setupapi.h"

#include "Specification.h"

#include <io.h>
#include <sys\stat.h> 

#include <Winreg.h>     // for registry functions
#include <Direct.h>		// for change directory functions

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT hTimer;
static MSG tickMsg;
///////////////////////////////////////////////////////////////////////////////
// Some application scope externals 
///////////////////////////////////////////////////////////////////////////////
//extern long				 gMaxRgntVol			= 0;
//extern LUMINOMETER_MODEL gLuminometerModel		= MODEL_SEDAN;
//
///////////////////////////////////////////////////////////////////////////////

void CALLBACK EXPORT TimerProc(HWND hWnd, UINT wm_timer, UINT nIDEvent, DWORD dwTime)
{
	// Feed Lcc
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	if (pApp->GetLccMgr() != NULL)
	{
			pApp->GetLccMgr()->OnTick();
	}
}

void LucEnableTick()
{
	// Add application's tick
	void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT, DWORD) = NULL;
	hTimer = ::SetTimer(NULL, 0, LUC_TICK_MSEC, TimerProc);
}

void LucDisableTick()
{
	::KillTimer(NULL, hTimer);
	
	// Also need to flush pending timer messages from queue
	while(::PeekMessage(&tickMsg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
		;
}



/////////////////////////////////////////////////////////////////////////////
// CLuceptApp

BEGIN_MESSAGE_MAP(CLuceptApp, CWinAppEx)
	//{{AFX_MSG_MAP(CLuceptApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MEASUREMENT_CLOSE, OnMeasurementClose)
	ON_COMMAND(ID_MEASUREMENT_STANDARD1, OnMeasurementStandard1)
	ON_COMMAND(ID_MEASUREMENT_STANDARD2, OnMeasurementStandard2)
	ON_COMMAND(ID_MEASUREMENT_LCCISVISIBLE, OnMeasurementLccisvisible)
	ON_COMMAND(ID_MEASUREMENT_START, OnMeasurementStart)
	ON_COMMAND(ID_MEASUREMENT_STOP, OnMeasurementStop)
	ON_COMMAND(ID_MEASUREMENT_DISCONNECTLCC, OnMeasurementDisconnectlcc)
	ON_COMMAND(ID_MEASUREMENT_CONNECTLCC, OnMeasurementConnectlcc)
	ON_COMMAND(ID_MEASUREMENT_SYNCLCC, OnMeasurementSynclcc)
	ON_COMMAND(ID_MEASUREMENT_GETSTATUS, OnMeasurementGetstatus)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_DISCONNECTLCC, OnUpdateMeasurementDisconnectlcc)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_CONNECTLCC, OnUpdateMeasurementConnectlcc)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_SYNCLCC, OnUpdateMeasurementSynclcc)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_LCCISVISIBLE, OnUpdateMeasurementLccisvisible)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_GETSTATUS, OnUpdateMeasurementGetstatus)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_STANDARD1, OnUpdateMeasurementStandard1)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_STANDARD2, OnUpdateMeasurementStandard2)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_START, OnUpdateMeasurementStart)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_STOP, OnUpdateMeasurementStop)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_CLOSE, OnUpdateMeasurementClose)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_DATADIALOG, OnUpdateMeasurementDatadialog)
	ON_UPDATE_COMMAND_UI(ID_MEASUREMENT_INQUIRE, OnUpdateMeasurementInquire)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CLuceptApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

void CLuceptApp::OnFileNew()
{
	CWinApp::OnFileNew();
}

/////////////////////////////////////////////////////////////////////////////
// CLuceptApp construction

CLuceptApp::CLuceptApp()
{
	m_pLucy = NULL;

	m_pWklViewTemplate		= 
	m_pRptViewTemplate		= 
	m_pRptViewTptTemplate	= 
	m_pAssayPrintTemplate	= 
	m_pMeasPrintTemplate	= 
	m_pSysLogPrintTemplate	= NULL;	// ITL: Added by CAR 887/002

	m_pLccVDoc				= NULL;
	m_pLccView				= NULL;
	m_pAssayPrDoc			= NULL;
	m_pAssayPrView			= NULL;
	m_pMeasPrDoc			= NULL;
	m_pMeasPrView			= NULL;
	m_pSystemLog			= NULL;
	m_pSysLogView			= NULL;

	m_bOfflineMode = FALSE;

	m_nCurrentTemp = 0.0;
}

CLuceptApp::~CLuceptApp()
{
	delete m_pSampViewTemplate;
	delete m_pTextViewTemplate;
	delete m_pBatCalcTemplate;
	delete m_pAssayPrintTemplate;
	delete m_pMeasPrintTemplate;
	delete m_pSysLogPrintTemplate;	// ITL: Added by CAR 887/002
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CLuceptApp object

CLuceptApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLuceptApp initialization

BOOL CLuceptApp::InitInstance()
{
	//sets date and other formats to local operating system defaults
	VERIFY(*setlocale(LC_ALL, ""));

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles. Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;

	InitCommonControlsEx(&InitCtrls);	

	CWinAppEx::InitInstance();

	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	
	if(!RegisterOleCOM())
	{
		return FALSE;
	}
	AfxEnableControlContainer();// Enable use of Active X controls
	RegisterAppPaths();

 	AfxOleGetMessageFilter()->SetMessagePendingDelay( (68000 * 105) / 100);

	// This call will initialize the grid library
    GXInit();

    switch( CSystemLog::g_oSystemLog.FileOpenOrCreate()) {
    	case CSystemLog::FILE_FAILED:
    		::MessageBox( NULL, _T("Failed to create log file"), _T("Celsis"), MB_OK);
    		return FALSE;

    	case CSystemLog::FILE_CREATED:
    		::MessageBox( NULL, _T("Initialized Log File"), _T("Celsis"), MB_OK);
    		break;

    	default:
    		break;
    }

#ifdef _STUFFIT
	for( int i=0; i < 100000; i++) {
		CLogEvent	NewEvent;							       // Create a new event   *
		CString strFmt;

		strFmt.Format( _T("Loop %d"), i + 1);
	
		NewEvent.EventSet(_T("Thrash Test"), strFmt);
		CSystemLog::g_oSystemLog.EventAdd( NewEvent);
	}
#endif

	if( GuessLCCPort() == FALSE) {
		m_bOfflineMode = TRUE;
	}

	///////////////////////////////////////////////////////
	// New MFC LNF insists on use of registry.. so use app registry for that stuff and use .ini
	// file for advance settings.
	SetRegistryKey(_T("Celsis\\Advance"));
	SetRegistryBase(_T("Settings"));

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	RegisterDocTemplates();

	// Call setup dialog on initial installation or if certain INI
	// corruptions are found
	if( IsInitialOrCorruptInstallation() == TRUE) {
		return FALSE;
	}

	int PharmaSec = GetLuceptProfileInt(gIniSec_Features,    gIniEnt_PharmaSec, -1 );	

	if( PharmaSec == -1) {
		//show window so the dialog does not hang in space
		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();
		
		if(SetupIniFiles(PharmaSec) == FALSE)
		{
			AfxMessageBox(IDS_SETUP_CANCELLED);
			return FALSE;
		}
	}

	// Display instrument type in the status bar
	pMainFrame->DisplayInstrumentDescription( Specification::g_oSpecification.GetDescription());

	// Create luminometer document path (reagent information)
	m_lumDocPath = GetAppPath() + _T("\\Reagents.lum");

	// 
	IniFilesRead();

	// And create LucyView
	LoadStdProfileSettings(0);  //turn MRU off so reagent.lum doesn't show
	CDocument* pLucyDoc = OpenDocumentFile(m_lumDocPath);
	if(pLucyDoc == NULL)
	{
		// Failed to open file
		// Try one more time it may have been created anew 
		// on the first attempt to read it
		pLucyDoc = OpenDocumentFile(m_lumDocPath);
		if (pLucyDoc == NULL)
		{
			CString errMsg;

			errMsg.Format(_T("Cannot open luminometer reagent file:\n %s"),m_lumDocPath);
			AfxMessageBox(errMsg, MB_ICONSTOP | MB_RETRYCANCEL);
			return ExitInstance();
		}
	}

	LoadStdProfileSettings(8);  //turn MRU on with filelist == 8

	pMainFrame->GetLucyBar().SetDocument( (CLucyDoc *)pLucyDoc);
	pMainFrame->GetLucyBar().IndicatorsUpdate();

	// Save pointer to LucyView
	POSITION pos = pLucyDoc->GetFirstViewPosition();
	m_pLucy = (CLucyView*)pLucyDoc->GetNextView(pos);
	ASSERT(m_pLucy->IsKindOf(RUNTIME_CLASS(CLucyView)));

	// Replacement for Reagent Levels Modeless Dialog
	m_pRgntDlg = m_pLucy->GetReagentDlgPtr();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);//!!! is this important?

	// Don't create new document by default
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
 		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}

	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

	// Show the main window
//	pMainFrame->ShowWindow(SW_MAXIMIZE);
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// Load default field properties
	CFieldProperties::InitDefaults(GetAppPath() + _T("\\FldProp.std"));

	// Initialize user-defined sample view fields
	// (Must be done AFTER loading default field properties
	CSampleView::InitUserFields();

	if( m_bOfflineMode == FALSE) {

		// Read measurement definitions
		if (!m_meass.Init())
		{
			AfxMessageBox(IDS_ERR_READING_MEASS);
		}

		// Read measurement history definitions
		if (!m_meassHistory.Init())
		{
			AfxMessageBox(IDS_ERR_READING_MEASS);
		}
	}

	// Read assay definitions
	if (!m_assays.Init())
	{
		AfxMessageBox(IDS_ERR_READING_ASSAYS);
	}

	// Read assay history definitions
	if (!m_assaysHistory.Init())
	{
		AfxMessageBox(IDS_ERR_READING_ASSAYS);
	}

	// Load sample status names
	if (!InitSampStats())
	{
		AfxMessageBox(IDS_SSTAT_LOAD_ERR);
	}

	// Initialize user-defined sample view fields
	// (Must be done AFTER loading default field properties
	CSampleView::InitUserFields();

#ifndef NO_USER_ADMIN
	gGateKeeper.UserLogOn();
#endif

	//Population stats are the default 99Dec20.DBM
	gUseSampleStatistics = GetLuceptProfileInt(gIniSec_Features, gIniEnt_SampStats, FALSE);

	//ordinary return
	return TRUE;	
}

BOOL CLuceptApp::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( nCode == CN_UPDATE_COMMAND_UI) {
		if( pExtra != NULL) {
			if( gGateKeeper.IsAppLocked() == true && nID != ID_LOCK_APP) {

				((CCmdUI *)pExtra)->Enable( FALSE);
				return TRUE;
			}
		}
	}

	return CWinApp::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo);
}

/**
 * Does a crude sanity check on .init files.
 */
BOOL CLuceptApp::IsInitialOrCorruptInstallation()
{
	//no model entry
	CString strMod;
	strMod = GetLuceptProfileString(gIniSec_Luminometer, gIniEnt_Model, _T(""));
	if(strMod.IsEmpty())
	{
		AfxMessageBox( _T( "Model parameter not specified in Advance.ini"));
		return TRUE;
	}

	long MaxTubesInBatch;
	long ChainLength;
	long HiTubes;
	long HiPosition;

	// Maximum samples from Advance.ini
	MaxTubesInBatch = GetLuceptProfileInt(gIniSec_ParmDefs, gIniEnt_MaxTubesInBatch, -2);

	//Get some values from LCC.INI
	TCHAR destBuf[256];
	memset(destBuf, 0, 256);

	// Chain length
	GetPrivateProfileString(gLccIniSec_Settings,gLccIniEnt_ChainLength, _T("0"), destBuf, 256, m_strLccProfileName);
	ChainLength = _tstol(destBuf);
	
	// HiTubes
	GetPrivateProfileString(gLccIniSec_Settings,gLccIniEnt_HiTubes,     _T("0"), destBuf, 256, m_strLccProfileName);
	HiTubes = _tstol(destBuf);

	// HiPosition
	GetPrivateProfileString(gLccIniSec_Settings,gLccIniEnt_HiPosition,  _T("0"), destBuf, 256, m_strLccProfileName);
	HiPosition = _tstol(destBuf);

	if(ChainLength == 180) {
		if(!(HiTubes == 164 || HiTubes == 162)) {
			AfxMessageBox( _T( "HiTubes parameter in Lcc.ini should be 164 or 162"));
			return TRUE;
		}

		if(!(HiPosition == 164 || HiPosition == 162)) {
			AfxMessageBox( _T( "HiPosition parameter in Lcc.ini should be 164 or 162"));
			return TRUE;
		}
	} else if( ChainLength == 46 || ChainLength == 30) {
		if(HiTubes != 30) {
			AfxMessageBox( _T( "HiTubes parameter in Lcc.ini should be 30"));
			return TRUE;
		}
		if(HiPosition != 30) {
			AfxMessageBox( _T( "HiPosition parameter in Lcc.ini should be 30"));
			return TRUE;
		}
	} else {
		AfxMessageBox( _T( "ChainLength parameter in Lcc.ini should be 180, 46 or 30"));
		return TRUE;
	}

	//These must be consistent
	if(MaxTubesInBatch != HiPosition || MaxTubesInBatch != HiTubes)
	{
		AfxMessageBox( _T( "MaxTubesInBatch parameter in Advance.ini should be equal to HiPosition or HiTubes in Lcc.ini"));
		return TRUE;
	}

	return FALSE;	
}

BOOL CLuceptApp::SetupIniFiles(const int PharmaSec)
{
	CSetupDlg dlg(PharmaSec);

	if(dlg.DoModal() == IDOK)
	{
		// CString LCCIni;
		// LCCIni = GetAppPath();
		// LCCIni += _T("\\LCC.INI");
		long rc = 0;

		//make sure the INI files are writeable
// pcw- mfc upgrade		VERIFY(!_chmod( m_pszProfileName,   _S_IREAD|_S_IWRITE));
		VERIFY(!_tchmod( m_strLccProfileName,				_S_IREAD|_S_IWRITE));

		//Pharma security
		WriteLuceptProfileInt(gIniSec_Features, gIniEnt_PharmaSec, dlg.m_PharmaSec);
		
		//make LCC.INI read only (Advance.ini must stay writeable for MRU list)
		//this prevents data from being written by previous calls ...
		//_flushall(); //...and this doesn't help
		//VERIFY(!_chmod( LCCIni,	_S_IREAD));
		return TRUE;
	}
	else
	{ //i.e., on Cancel
		return FALSE;
	}
}

void CLuceptApp::IniFilesRead()
{
	// Maximum samples
	gMaxTubesInBatch = GetLuceptProfileInt(gIniSec_ParmDefs, gIniEnt_MaxTubesInBatch, 0);
	ASSERT(gMaxTubesInBatch);

	//from LCC.INI
	TCHAR destBuf[256];
	memset(destBuf, 0, 256);

	// Chain length
	GetPrivateProfileString(gLccIniSec_Settings,gLccIniEnt_ChainLength, _T("180"), destBuf, 256, m_strLccProfileName);
	gChainLength = _tstol(destBuf);
	ASSERT(gChainLength == 180 || gChainLength == 46 || gChainLength == 30);
	
	// HiTubes
	GetPrivateProfileString(gLccIniSec_Settings,gLccIniEnt_HiTubes,     _T("164"), destBuf, 256, m_strLccProfileName);
	gHiTubes = _tstol(destBuf);
	ASSERT(gHiTubes == 164 || gHiTubes == 162 || gHiTubes == 30);
	// PCW: A failure here probably means lcc is set up for a different instrument to advance.
	ASSERT(gHiTubes == gMaxTubesInBatch);

	// These are the absolute theoretical maximums -- 
	// assume 1 replicate samples and 1 sample batches
	gMaxBatches = gHiTubes/2;
	gMaxSamples = gHiTubes;

}

/////////////////////////////////////////////////////////////////////////////
// CLuceptApp clean-up

int CLuceptApp::ExitInstance() 
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem( IDC_STATIC_PRODUCT)->SetWindowText( CLuceptApp::GetExecutableResource( _T("ProductName")));
	GetDlgItem( IDC_STATIC_VERSION)->SetWindowText( _T("Version ") + CLuceptApp::GetExecutableResource( _T("ProductVersion")));
	GetDlgItem( IDC_STATIC_COPYRIGHT)->SetWindowText( CLuceptApp::GetExecutableResource( _T("LegalCopyright")));
	return TRUE;
}

// App command to run the dialog
void CLuceptApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



void CLuceptApp::OnMeasurementClose() 
{
	if (m_pLccMgr == NULL)
	{
		AfxMessageBox(_T("LCC not connected"));
	}
	else
	{
		ErrorReport(m_pLccMgr->CSClose(this));	
	}
}

void CLuceptApp::OnMeasurementStandard1() 
{
	if (m_pLccMgr == NULL)
	{
		AfxMessageBox(_T("LCC not connected"));
	}
	else
	{
		ErrorReport(m_pLccMgr->CSOpen(this,1,1));	
	}
}

void CLuceptApp::OnMeasurementStandard2() 
{
	if (m_pLccMgr == NULL)
	{
		AfxMessageBox(_T("LCC not connected"));
	}
	else
	{
		ErrorReport(m_pLccMgr->CSOpen(this,1,2));	
	}
}

void CLuceptApp::OnMeasurementStart() 
{
	if (m_pLccMgr == NULL)
	{
		AfxMessageBox(_T("LCC not connected"));
	}
	else
	{
		ErrorReport(m_pLccMgr->CSStart(this));	
	}
}

void CLuceptApp::OnMeasurementStop() 
{
	if (m_pLccMgr == NULL)
	{
		AfxMessageBox(_T("LCC not connected"));
	}
	else
	{
		ErrorReport(m_pLccMgr->CSStop(this, LCC_STOP_IMMEDIATE));	
	}
}

void CLuceptApp::OnMeasurementDisconnectlcc() 
{
	if (m_pLccMgr != NULL)
	{
		if (IsController())
		{
			ErrorReport(RelinquishControl());
		}
		// Remain a user
	}
}


void CLuceptApp::OnMeasurementConnectlcc() 
{
	long rv;

	if (m_pLccMgr != NULL)
	{
		ErrorReport(rv = RegisterWithLcc());
		if (rv == KO_OK)
		{
			ErrorReport(RequestControl());
		}
	}
}

void CLuceptApp::OnMeasurementSynclcc()
{
	if (m_pLccMgr != NULL)
	{
		ConnectLum();
	}
}

void CLuceptApp::OnMeasurementLccisvisible()
{
	if (m_pLccMgr != NULL)
	{
		m_pLccMgr->ShowLccNow();
	}
}

void CLuceptApp::OnMeasurementGetstatus() 
{
	if (m_pLccMgr == NULL)
	{
		AfxMessageBox(_T("LCC not connected"));
	}
	else
	{
		long rv = m_pLccMgr->CSGetStatus();
		CString status;
		status.Format(_T("Control session status = 0x%X"), rv);
		AfxMessageBox(status);
	}
}

void CLuceptApp::ErrorReport(long rv)
{
	if (rv != KO_OK)
	{
		CString errMsg;
		errMsg.Format(_T("LCC returned exception 0x%X"), rv);
		AfxMessageBox(errMsg);
	}
}


bool CLuceptApp::CreateLcc()
{
	if (m_pLccMgr != NULL)
	{
		DestroyLcc();
	}

//#ifdef _EMULATE
//	m_pLccMgr = new CLccMgrEmulator;
//#else
	if( m_bOfflineMode == FALSE) {
		m_pLccMgr = new CLccMgr;
//#endif
		ASSERT(m_pLccMgr != NULL);
		if (!m_pLccMgr->ConnectLcc())
		{
			DestroyLcc();
			AfxMessageBox( _T("Instrument not available, running in offline mode"));
//		return false;
		}

#ifdef _DEBUG
		// In debug mode show lcc
		m_pLccMgr->ShowLccNow();  // pcw
#endif
	}

	return true;
}

void CLuceptApp::DestroyLcc()
{
	if (m_pLccMgr != NULL)
	{
		m_pLccMgr->DisconnectLcc();
		delete m_pLccMgr;
		m_pLccMgr = NULL;
	}
}

bool CLuceptApp::InitSampStats()
{
	CString name;

	// Load sample status array
	if (!name.LoadString(IDS_SSTAT_NEW))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_NEW, name));

	if (!name.LoadString(IDS_SSTAT_CHECKED))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_CHECKED, name));

	if (!name.LoadString(IDS_SSTAT_COMMITTED))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_COMMITTED, name));

	if (!name.LoadString(IDS_SSTAT_READING))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_READING, name));

	if (!name.LoadString(IDS_SSTAT_READ))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_READ, name));

	if (!name.LoadString(IDS_SSTAT_DONE))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_DONE, name));

	if (!name.LoadString(IDS_SSTAT_STOPPED))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_STOPPED, name));

	if (!name.LoadString(IDS_SSTAT_ABORTED))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_ABORTED, name));

	if (!name.LoadString(IDS_SSTAT_DEFCAL))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_DEFCAL, name));

	if (!name.LoadString(IDS_SSTAT_VIRTUAL))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_VIRTUAL, name));

	if (!name.LoadString(IDS_SSTAT_UNCAL))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_UNCAL, name));

	if (!name.LoadString(IDS_SSTAT_FSTOP))
	{
		return false;
	}
	m_sampStats.Add(CNamedID(LUC_SSTAT_FSTOP, name));

	return true;
}

void CLuceptApp::OnUpdateMeasurementDisconnectlcc(CCmdUI* pCmdUI) 
{
	long rv = m_pLccMgr->CSGetStatus();
	if (IsController() && rv == LCC_CS_CLOSED)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementConnectlcc(CCmdUI* pCmdUI) 
{
	if (!IsController())
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementSynclcc(CCmdUI* pCmdUI) 
{
	if (IsController())
	{
		pCmdUI->Enable(true);
	}
	else if (IsLccAvailable())
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementLccisvisible(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}


void CLuceptApp::OnUpdateMeasurementGetstatus(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementStandard1(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL && IsController())
	{
		long rv = m_pLccMgr->CSGetStatus();
		if (rv == LCC_CS_CLOSED)
		{
			pCmdUI->Enable(true);
		}
		else
		{
			pCmdUI->Enable(false);
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementStandard2(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL && IsController())
	{
		long rv = m_pLccMgr->CSGetStatus();
		if (rv == LCC_CS_CLOSED)
		{
			pCmdUI->Enable(true);
		}
		else
		{
			pCmdUI->Enable(false);
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementStart(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL && IsController())
	{
		long rv = m_pLccMgr->CSGetStatus();
		if (rv == LCC_CS_OPENED)
		{
			pCmdUI->Enable(true);
		}
		else
		{
			pCmdUI->Enable(false);
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementStop(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL && IsController())
	{
		long rv = m_pLccMgr->CSGetStatus();
		if (rv == LCC_CS_RUNNING)
		{
			pCmdUI->Enable(true);
		}
		else
		{
			pCmdUI->Enable(false);
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementClose(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL && IsController())
	{
		long rv = m_pLccMgr->CSGetStatus();
		if (rv == LCC_CS_ENDED	 ||
			rv == LCC_CS_STOPPED ||
			rv == LCC_CS_FAULTED ||
			rv == LCC_CS_OPENED)
		{
			pCmdUI->Enable(true);
		}
		else
		{
			pCmdUI->Enable(false);
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}
	
}

void CLuceptApp::OnUpdateMeasurementDatadialog(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CLuceptApp::OnUpdateMeasurementInquire(CCmdUI* pCmdUI) 
{
	if (m_pLccMgr != NULL)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

const CString& CLuceptApp::GetAppPath()
{
	return m_appPath;
}

const CString& CLuceptApp::GetDataPath()
{
	return m_dataPath;
}

const CString& CLuceptApp::GetLumDocPath()
{
	return m_lumDocPath;
}


CCustPal* CLuceptApp::GetCustPal()
{
	return &m_custPal;
}


BOOL CLuceptApp::RegisterOleCOM()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	HKEY key;

	// Get handle to Interface Classes Key in registry
	RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("Interface"), 0, KEY_ALL_ACCESS, &key);

	// !!! Use RegSetValueEx
	// Define ProgramID for UIC Event Sink Interface
	RegSetValue( key, IID_SNK,  REG_SZ, _T("IID_ILccEventSink"), 0);

	// Associate Interface with Proxy Stub to handle marshalling
	// 16 and 32 bit version
	RegSetValue( key, PROXYID_SNK,   REG_SZ, PROXYID_DISP, 0);
	RegSetValue( key, PROXYID32_SNK, REG_SZ, PROXYID_DISP, 0);

	//  4 Dispatch Methods - Invoke, GetIDsOfNames, GetTypeInfo, GetTypeInfoCount
	RegSetValue( key, NUMMETH_SNK, REG_SZ, _T("8"), 0);

	// Based on IDispatch interface
	RegSetValue( key, BASEI_SNK, REG_SZ, IID_IDISPATCH, 0);

	RegCloseKey(key);

	return TRUE;
}

BOOL CLuceptApp::RegisterAppPaths()
{
	// First see if we are running on XP so lets bang some rocks together

	// Determine where the executable is. This used to be done by reading the registry but on >= vista that bangs into
	// UAC so lets just get the path to the executable.
	TCHAR appPath[MAX_PATH];

	GetModuleFileName( NULL, appPath, MAX_PATH);
	CString strAppExePath( appPath);

	int iLastSlash = strAppExePath.ReverseFind('\\');
	CString strProgramPath = strAppExePath.Left(iLastSlash);

    // So is advance.exe under a Programs subdirectory or is it in the root?
    // Determine the name of the directory that it is in.
	iLastSlash = strProgramPath.ReverseFind('\\');

	CString strProgramDir;

	if( iLastSlash > -1) {
		strProgramDir = strProgramPath.Mid(iLastSlash + 1);
	} else {
		strProgramDir = "";  // In C:\ ??? 
	}

	// Save application path for later
	m_appPath = strProgramPath;

	// If advance is in a programs directory then strip it off to find the other directories.
	if( strProgramDir.CompareNoCase( _T("programs")) == 0) {
		m_dataPath = strProgramPath.Left(iLastSlash) + _T("\\Results");
	} else {
		m_dataPath = strProgramPath + _T("\\Results");
	}
	// Set working directory
	_tchdir(m_dataPath);

	// For backwards compatibility with old installations, assume Advance.ini is in the same place as the advance executable.
	m_strLuceptProfileName = strProgramPath + _T("\\Advance.ini");
	m_strLccProfileName = strProgramPath + _T("\\lcc.ini");
	m_strSettingsProfileName = strProgramPath + _T("\\Settings.ini");

	// See where advance.ini really is
	CFileStatus oStatus;

	if( !CFile::GetStatus( m_strLuceptProfileName, oStatus)) {
		TCHAR strLocalAppPath[MAX_PATH];

		// If it is not in the same directory as advance.exe then assume it has been put in the correct place for
		// data files shared between applications.

		HRESULT oPathRv = SHGetFolderPath( 
  			NULL,					// HWND hwndOwner,
  			CSIDL_COMMON_APPDATA,	// int nFolder,
  			NULL,					// HANDLE hToken,
			SHGFP_TYPE_CURRENT,		// DWORD dwFlags,
  			strLocalAppPath			// LPTSTR pszPath
		);

		if( oPathRv == S_OK) {
			// We are using the localapp folger for data. Build correct paths to data directories.
			CString strLocalPath( strLocalAppPath);

			CString strRoot = strLocalPath + _T( "\\Celsis\\Advance");

			m_appPath = strRoot + _T( "\\Programs");
			m_dataPath = strRoot + _T( "\\Results");

			_tchdir(m_dataPath);

			// Build path to ini file.
			m_strLuceptProfileName = strRoot + _T( "\\Programs\\Advance.ini");
			m_strLccProfileName = strRoot + _T( "\\Programs\\lcc.ini");
			m_strSettingsProfileName = strRoot + _T("\\Programs\\Settings.ini");
		}
	}

	return TRUE;
}

void CLuceptApp::RegisterDocTemplates()
{
	// Workload Template
	m_pWklViewTemplate = new CWklMultiDocTemplate(
		IDR_WORKLOAD,
		RUNTIME_CLASS(CWklDoc),
		RUNTIME_CLASS(CWklFrame),
		RUNTIME_CLASS(CWklView));
	AddDocTemplate(m_pWklViewTemplate);

	// Workload Template Template
	m_pWklViewWktTemplate = new CWklMultiDocTemplate(
		IDR_WORKLOAD_WKT,
		RUNTIME_CLASS(CWklDoc),
		RUNTIME_CLASS(CWklFrame),
		RUNTIME_CLASS(CWklView));
	AddDocTemplate(m_pWklViewWktTemplate);

	if( Specification::g_oSpecification.HasReports()) {
		// Report Template
		m_pRptViewTemplate = new CWklMultiDocTemplate(
			IDR_REPORT,
			RUNTIME_CLASS(CRptDoc),
			RUNTIME_CLASS(CRptFrame), 
			RUNTIME_CLASS(CRptRibbonView));
		ASSERT(m_pRptViewTemplate);
		AddDocTemplate(m_pRptViewTemplate);

		// Report Template Template
		m_pRptViewTptTemplate = new CWklMultiDocTemplate(
			IDR_REPORT_TPT,	//<-a null string in here prevents template from showing in File|New box
			RUNTIME_CLASS(CRptDoc),
			RUNTIME_CLASS(CRptFrame), 
			RUNTIME_CLASS(CRptRibbonView));
		ASSERT(m_pRptViewTptTemplate);
		AddDocTemplate(m_pRptViewTptTemplate);
	}

	AddDocTemplate(new CMultiDocTemplate(
		IDR_LUCYTYPE,
		RUNTIME_CLASS(CLucyDoc),
		RUNTIME_CLASS(CLucyFrame), // custom MDI child frame
		RUNTIME_CLASS(CLucyView)));

	m_pSampViewTemplate = new CMultiDocTemplate(
		IDR_WORKLOAD,
		RUNTIME_CLASS(CWklDoc),	
		RUNTIME_CLASS(CSampFrm),
		RUNTIME_CLASS(CSampleView));

	// Create Lcc View Template, but don't register it
	m_pTextViewTemplate = new CMultiDocTemplate(
		IDR_LCCTYPE,
		RUNTIME_CLASS(CLccVDoc), 
		RUNTIME_CLASS(CMDIChildWndEx),
		RUNTIME_CLASS(CLccView));

	// Create BatCalc View Template, but don't register it
	m_pBatCalcTemplate = new CMultiDocTemplate(
		IDR_WORKLOAD,
		RUNTIME_CLASS(CWklDoc), 
		RUNTIME_CLASS(CBatCalcFrame),
		RUNTIME_CLASS(CBatCalc));

	// Assay print template
	m_pAssayPrintTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAssayPrDoc),
		RUNTIME_CLASS(CMDIChildWndEx),
		RUNTIME_CLASS(CAssayPrView));
	ASSERT(m_pAssayPrintTemplate);

	// Measurement print template
	m_pMeasPrintTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMeasPrDoc),
		RUNTIME_CLASS(CMDIChildWndEx),
		RUNTIME_CLASS(CMeasPrView));
	ASSERT(m_pMeasPrintTemplate);

	// System Log print template
	m_pSysLogPrintTemplate = new CSystemLogDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSystemLogDoc),
		RUNTIME_CLASS(CMDIChildWndEx),
		RUNTIME_CLASS(CSysLogView));
	ASSERT(m_pSysLogPrintTemplate);
}

int CLuceptApp::GetLuceptProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault, m_strLuceptProfileName);
}

CString CLuceptApp::GetLuceptProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (lpszDefault == NULL)
		lpszDefault = _T("");	// don't pass in NULL
	TCHAR szT[4096];
	DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry,
		lpszDefault, szT, _countof(szT), m_strLuceptProfileName);
	ASSERT(dw < 4095);
	return szT;
}

void CLuceptApp::WriteLuceptProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue, m_strLuceptProfileName);
}

void CLuceptApp::WriteLuceptProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, long nValue)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	CString strValue;
	
	strValue.Format( _T("%d"), nValue);

	::WritePrivateProfileString(lpszSection, lpszEntry, strValue, m_strLuceptProfileName);
}

/**
 * Read a section from the advance.ini file.
 * @param lpAppName The name of the section to read
 * @param pReturnedString A buffer to read the section into.
 * @param nSize The size of the receiving buffer.
 */
void CLuceptApp::GetLuceptProfileSection( LPCTSTR lpAppName, LPTSTR pReturnedString, DWORD nSize)
{
	::GetPrivateProfileSection( lpAppName, pReturnedString, nSize, m_strLuceptProfileName);
}

/**
 * Write a whole section to the advance.ini file.
 * @param lpAppName The name of the section to written
 * @param pReturnedString A buffer to read the section from.
 */
void CLuceptApp::WriteLuceptProfileSection( LPCTSTR lpAppName, LPCTSTR pString)
{
	if( ::WritePrivateProfileSection( lpAppName, pString, m_strLuceptProfileName) == 0) {
		TRACE1( "Failed to write Advance.ini section: %d", GetLastError());
	}
}

/**
 * Read a section from the lcc.ini file.
 * @param lpAppName The name of the section to read
 * @param pReturnedString A buffer to read the section into.
 * @param nSize The size of the receiving buffer.
 */
void CLuceptApp::GetLccProfileSection( LPCTSTR lpAppName, LPTSTR pReturnedString, DWORD nSize)
{
	if( ::GetPrivateProfileSection( lpAppName, pReturnedString, nSize, m_strLccProfileName) == 0) {
		TRACE1( "Failed to write Lcc.ini section: %d", GetLastError());
	}
}

/**
 * Read a section from the Settings.ini file.
 * @param lpAppName The name of the section to read
 * @param pReturnedString A buffer to read the section into.
 * @param nSize The size of the receiving buffer.
 */
void CLuceptApp::GetSettingsProfileSection( LPCTSTR lpAppName, LPTSTR pReturnedString, DWORD nSize)
{
	if( ::GetPrivateProfileSection( lpAppName, pReturnedString, nSize, m_strSettingsProfileName) == 0) {
		TRACE1( "Failed to read Settings.ini section: %d", GetLastError());
	}
}

/**
 * Write a whole section to the lcc.ini file.
 * @param lpAppName The name of the section to written
 * @param pReturnedString A buffer to read the section from.
 */
void CLuceptApp::WriteLccProfileSection( LPCTSTR lpAppName, LPCTSTR pString)
{
	::WritePrivateProfileSection( lpAppName, pString, m_strLccProfileName);
}

/**
 * Make a guess as to which port the luminometer is connected to.
 * @returns FALSE if the port could not be determined or otherwise does not look right.
 */
BOOL CLuceptApp::GuessLCCPort()
{
//     TCHAR lpTargetPath[5000]; // buffer to store the path of the COMPORTS
//     DWORD test;
//     bool gotPort=0; // in case the port is not found

//     for(int i=0; i<255; i++) // checking ports from COM0 to COM255
//     {
//         CString str;
//         str.Format(_T("%d"),i);
//         CString ComName=CString("COM") + CString(str); // converting to COM0, COM1, COM2

//         test = QueryDosDevice(ComName, lpTargetPath, 5000);

//             // Test the return value and error if any
//         if(test!=0) //QueryDosDevice returns zero if it didn't find an object
//         {
// //            m_MyPort.AddString((CString)ComName); // add to the ComboBox
//             gotPort=1; // found port
//         }

//         if(::GetLastError()==ERROR_INSUFFICIENT_BUFFER)
//         {
//             lpTargetPath[10000]; // in case the buffer got filled, increase size of the buffer.
//             continue;
//         }

//     }

	CString strAdvancePort = GetLuceptProfileString( _T( "CommunicationSetup"), _T( "Port"), _T( "auto"));

	// If the instruction is to let lcc handle the comms port then don't do anything here.
	if( strAdvancePort.CompareNoCase( _T( "lcc")) == 0) {
		return TRUE;
	}

	CStringArray strPorts;
	CStringArray strNames;

	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;

	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs(NULL,
										0, // Enumerator
										0,
										DIGCF_PRESENT | DIGCF_ALLCLASSES);

	if( hDevInfo == INVALID_HANDLE_VALUE) {
		// Insert error handling here.
		DWORD nFuggit = GetLastError();
		return 1;
	}

	// Enumerate through all devices in Set.

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for( i=0; SetupDiEnumDeviceInfo( hDevInfo, i, &DeviceInfoData); i++)	{
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;

		//Get the registry key which stores the ports settings
		HKEY hDeviceKey = SetupDiOpenDevRegKey( hDevInfo, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
		if( hDeviceKey != INVALID_HANDLE_VALUE) {
			int nPort = 0;
			LPTSTR pszPortName = NULL;

			if (RegQueryValueString(hDeviceKey, _T("PortName"), pszPortName)) {
				TRACE1( "Port %s\n", pszPortName);

				TCHAR szFriendlyName[1024];
				szFriendlyName[0] = _T('\0');
				DWORD dwSize = sizeof(szFriendlyName);
				DWORD dwType = 0;

				if( SetupDiGetDeviceRegistryProperty( hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC, &dwType, reinterpret_cast<PBYTE>(szFriendlyName), dwSize, &dwSize) && (dwType == REG_SZ)){
					// Succeeded in getting 'nice' name for device.
					// Note the port name and the friendly name.
					TRACE1( "Port Name %s\n", szFriendlyName);

					strPorts.Add( pszPortName);
					strNames.Add( szFriendlyName);
				}

				//Close the key now that we are finished with it
				RegCloseKey(hDeviceKey);
				LocalFree(pszPortName);
			}
		}

		if( GetLastError() != NO_ERROR && GetLastError() == ERROR_NO_MORE_ITEMS) {
			break;
		}
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);

	if( strAdvancePort.CompareNoCase( _T("auto")) == 0) {

		if( strPorts.GetCount() == 0) {
			AfxMessageBox( _T("The Luminometer is not connected or is not switched on"));
			return FALSE;
		}

		bool bFound = false;

		for( int i=0; i<strPorts.GetCount(); i++) {
			if( strNames[i].CompareNoCase( _T("usb serial port")) == 0) {
				CLogEvent	NewEvent;							       // Create a new event   *
	
				NewEvent.EventSet(_T("Comms"), _T("Automatically setting port to ") + strPorts[i]);
				CSystemLog::g_oSystemLog.EventAdd( NewEvent);

				WritePrivateProfileString( _T("CommunicationSetup"), _T("Port"), strPorts[i], m_strLccProfileName);

				bFound = true;
				break;
			}
		}

		if( bFound == false) {
			AfxMessageBox( _T("Could not locate the luminometer. Please ensure it is connected and switched on."));
			return FALSE;
		}
	} else {
		WritePrivateProfileString( _T( "CommunicationSetup"), _T( "Port"), strAdvancePort, m_strLccProfileName);
	}

	return TRUE;
}

BOOL CLuceptApp::RegQueryValueString(HKEY kKey, LPCTSTR lpValueName, LPTSTR& pszValue)
{
  //Initialize the output parameter
  pszValue = NULL;

  //First query for the size of the registry value 
  DWORD dwType = 0;
  DWORD dwDataSize = 0;
  LONG nError = RegQueryValueEx(kKey, lpValueName, NULL, &dwType, NULL, &dwDataSize);
  if (nError != ERROR_SUCCESS)
  {
    SetLastError(nError);
    return FALSE;
  }

  //Ensure the value is a string
  if (dwType != REG_SZ)
  {
    SetLastError(ERROR_INVALID_DATA);
    return FALSE;
  }

  //Allocate enough bytes for the return value
  DWORD dwAllocatedSize = dwDataSize + sizeof(TCHAR); //+sizeof(TCHAR) is to allow us to NULL terminate the data if it is not null terminated in the registry
  pszValue = reinterpret_cast<LPTSTR>(LocalAlloc(LMEM_FIXED, dwAllocatedSize)); 
  if (pszValue == NULL)
    return FALSE;

  //Recall RegQueryValueEx to return the data
  pszValue[0] = _T('\0');
  DWORD dwReturnedSize = dwAllocatedSize;
  nError = RegQueryValueEx(kKey, lpValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pszValue), &dwReturnedSize);
  if (nError != ERROR_SUCCESS)
  {
    LocalFree(pszValue);
    pszValue = NULL;
    SetLastError(nError);
    return FALSE;
  }

  //Handle the case where the data just returned is the same size as the allocated size. This could occur where the data
  //has been updated in the registry with a non null terminator between the two calls to ReqQueryValueEx above. Rather than
  //return a potentially non-null terminated block of data, just fail the method call
  if (dwReturnedSize >= dwAllocatedSize)
  {
    SetLastError(ERROR_INVALID_DATA);
    return FALSE;
  }

  //NULL terminate the data if it was not returned NULL terminated because it is not stored null terminated in the registry
  if (pszValue[dwReturnedSize/sizeof(TCHAR) - 1] != _T('\0'))
    pszValue[dwReturnedSize/sizeof(TCHAR)] = _T('\0');

  return TRUE;
}

/**
 * Read the value of one of the version info properties from the executable.
 * @param  strResourceName    The name of the resource to read.
 * @return                    The value of the property.
 */
CString CLuceptApp::GetExecutableResource( LPCTSTR strResourceName)
{
    // get the filename of the executable containing the version resource
    TCHAR szFilename[MAX_PATH + 1] = {0};
    if (GetModuleFileName(NULL, szFilename, MAX_PATH) == 0)
    {
        TRACE("GetModuleFileName failed with error %d\n", GetLastError());
        return _T("Unknown");
    }

    // allocate a block of memory for the version info
    DWORD dummy;
    DWORD dwSize = GetFileVersionInfoSize(szFilename, &dummy);
    if (dwSize == 0)
    {
        TRACE("GetFileVersionInfoSize failed with error %d\n", GetLastError());
        return _T("Unknown");
    }
    std::vector<BYTE> data(dwSize);

    // load the version info
    if (!GetFileVersionInfo(szFilename, NULL, dwSize, &data[0]))
    {
        TRACE("GetFileVersionInfo failed with error %d\n", GetLastError());
        return _T("Unknown");
    }

    // get the name and version strings
    LPVOID pResource = NULL;
    unsigned int nResourceLen = 0;

    CString strPath = _T("\\StringFileInfo\\040904b0\\");
    strPath += strResourceName;

    // replace "040904b0" with the language ID of your resources
    if (!VerQueryValue( &data[0], strPath, &pResource, &nResourceLen)) {
        TRACE("VerQueryValue failed with error %d\n", GetLastError());
    	return _T("Unknown");
    }

    CString strResource;

    strResource.SetString( (LPCTSTR)pResource, nResourceLen - 1);

    return strResource;
}
