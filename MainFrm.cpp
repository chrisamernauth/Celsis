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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "LucDef.h"
#include "LuceErr.h"
#include "Lucept.h"
#include "WklFrame.h"
#include "WklView.h"
#include "SampView.h"
#include "AssayNew.h"
#include "LccMgr.h"
#include "AssaySel.h"
#include "MeasSel.h"
#include "MeasNew.h"
#include "MDefDlg.h"
#include "LccDef.h"
#include "MeasDef.h"
#include "UsrAdmin.h"
#include "InjSeq.h"
#include "AddReag.h"
#include "HandCtrl.h"
#include "LucyFrm.h"
#include "LucyDoc.h"
#include "LucyView.h"

#include "LccView.h"
#include "LccVDoc.h"
#include "Lucept.h"

#include "AssyPrDc.h"
#include "AssyPrVw.h"
#include "MeasPrDc.h"
#include "MeasPrVw.h"

#include "IniFiles.h"
#include "PasswordChange.h"
#include "SystemLogDoc.h"
#include "UserDB.h"
#include "User.h"
#include "Event.h"
#include "Specification.h"
#include "SystemLog.h"
#include "SysLogView.h"
#include "RptView.h"
#include "RptDoc.h"
#include "RGridBase.h"

#include <htmlhelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

UINT NEAR WM_PUPDATE_LOG
	= RegisterWindowMessage(_T("LuceptUpdateLog"));


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	//{{AFX_MSG_MAP(CMainFrame)




	// Global help commands
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CONTEXT_HELP, OnHelp)
	ON_COMMAND(ID_DEFAULT_HELP, OnHelp)
	ON_COMMAND(ID_FILE_LOGON, OnFileLogon)
	ON_COMMAND(ID_FILE_OPEN_WORKLOAD, OnFileOpenWorkload)
	ON_COMMAND(ID_FILE_OPEN_REPORT_TEMPLATE, OnFileOpenReportTemplate)
	ON_COMMAND(ID_FILE_OPEN_WORKLOAD_TEMPLATE, OnFileOpenWorkloadTemplate)
	ON_COMMAND(ID_FILE_OPEN_REPORT, OnFileOpenReport)
	ON_COMMAND(ID_GRAPHIC_VIEW, OnGraphicView)
	ON_COMMAND(ID_GRID_VIEW, OnGridView)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HELP_CUSTOMERSERVICE, OnHelpCustomerservice)
	ON_COMMAND(ID_HELP_FINDER, OnHelp)
	ON_COMMAND(ID_LCC_UNVIEW_DIALOG, CloseLccView)
	ON_COMMAND(ID_LCC_VIEW_DIALOG, OnLccView)
	ON_COMMAND(ID_LOCK_APP, OnLockApp)
	ON_COMMAND(ID_MAINTENANCE_INJECTOR_RINSE, OnMaintenanceInjectorRinse)
	ON_COMMAND(ID_MAINTENANCE_INJECTOR_SHUTDOWN, OnMaintenanceInjectorShutdown)
	ON_COMMAND(ID_MAINTENANCE_INJECTOR_STARTUP, OnMaintenanceInjectorStartup)
	ON_COMMAND(ID_MAINTENANCE_INJECTOR_WASH, OnMaintenanceInjectorWash)
	ON_COMMAND(ID_MAINTENANCE_MANUAL, OnMaintenanceManual)
	ON_COMMAND(ID_MAINTENANCE_PRIME, OnMaintenancePrime)
	ON_COMMAND(ID_MAINTENANCE_REFRESH, OnMaintenanceRefresh)
	ON_COMMAND(ID_MEASUREMENT_DATADIALOG, OnMeasurementDatadialog)
	ON_COMMAND(ID_MEASUREMENT_INQUIRE, OnMeasurementInquire)
	ON_COMMAND(ID_NEW_REPORT, OnNewReport)
	ON_COMMAND(ID_NEW_WORKLOAD, OnNewWorkload)
	ON_COMMAND(ID_NEW_WORKLOAD_TEMPLATE, OnNewWorkloadTemplate)
	ON_COMMAND(ID_ROUTINE_REAGENTLEVELS, OnRoutineReagentlevels)
	ON_COMMAND(ID_ROUTINE_RUN, OnRun)
	ON_COMMAND(ID_ROUTINE_SCANCHAIN, OnScanChain)
	ON_COMMAND(ID_ROUTINE_UNLOCK, OnUnlock)
	ON_COMMAND(ID_SAMPLE_VIEW, OnSampleView)
	ON_COMMAND(ID_SETUP_ASSAY_DEL,	OnSetupAssayDel)
	ON_COMMAND(ID_SETUP_ASSAY_EDIT, OnSetupAssayEdit)
	ON_COMMAND(ID_SETUP_ASSAY_HISTORY, OnSetupAssayHistory)
	ON_COMMAND(ID_SETUP_ASSAY_NEW,	OnSetupAssayNew)
	ON_COMMAND(ID_SETUP_ASSAY_PRINT, OnSetupAssayPrint)
	ON_COMMAND(ID_SETUP_CHANGEPASSWORD, OnSetupChangepassword)
	ON_COMMAND(ID_SETUP_MEAS_DEL,	OnSetupMeasDel)
	ON_COMMAND(ID_SETUP_MEAS_EDIT,	OnSetupMeasEdit)
	ON_COMMAND(ID_SETUP_MEAS_HISTORY, OnSetupMeasHistory)
	ON_COMMAND(ID_SETUP_MEAS_NEW,	OnSetupMeasNew)
	ON_COMMAND(ID_SETUP_MEAS_PRINT, OnSetupMeasPrint)
	ON_COMMAND(ID_SETUP_PATHS, OnSetupPaths)
	ON_COMMAND(ID_BUTTON_KICK_LCC, OnKickLcc)
	ON_COMMAND(ID_SETUP_SYSTEM, OnSetupSystem)
	ON_COMMAND(ID_SETUP_SYSTEMLOG, OnSetupSystemlog)
	ON_COMMAND(ID_SETUP_USERADMINISTRATION, OnSetupUseradministration)
	ON_COMMAND(ID_SETUP_USERPREFERENCES, OnSetupUserpreferences)
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND( ID_WINDOW_CASCADE, OnWindowArrange)
	ON_COMMAND( ID_WINDOW_TILE_VERT, OnWindowTileVert)
	ON_COMMAND( ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_VS_2005, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_COMMAND_RANGE(ID_VIEW_FONTSIZE_6, ID_VIEW_FONTSIZE_16, CMainFrame::OnFontSize)

    ON_REGISTERED_MESSAGE(WM_PUPDATE_LOG, OnUpdateLog)
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CMainFrame::OnAfxWmChangingActiveTab)

	
	ON_UPDATE_COMMAND_UI( ID_LCC_VIEW_DIALOG,		 			OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_MAINTENANCE_INJECTOR_RINSE, 		OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_MAINTENANCE_INJECTOR_SHUTDOWN, 	OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_MAINTENANCE_INJECTOR_STARTUP, 		OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_MAINTENANCE_INJECTOR_WASH, 		OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_MAINTENANCE_MANUAL, 				OnUpdateMaintenanceManual)
	ON_UPDATE_COMMAND_UI( ID_MAINTENANCE_PRIME, 				OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_MAINTENANCE_REFRESH, 				OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_ROUTINE_REAGENTLEVELS, 			OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_ROUTINE_RUN,			 			OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_ROUTINE_SCANCHAIN,					OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI( ID_ROUTINE_UNLOCK,					OnUpdateIfLccAvailable)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOGON, OnUpdateFileLogon)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_USER, OnUpdateUserStatusPane)
	ON_UPDATE_COMMAND_UI(ID_SAMPLE_VIEW, OnUpdateSampleView)
	ON_UPDATE_COMMAND_UI(ID_SETUP_ASSAY_DEL,	OnUpdateSetupAssayDel)
	ON_UPDATE_COMMAND_UI(ID_SETUP_ASSAY_EDIT,	OnUpdateSetupAssayEdit)
	ON_UPDATE_COMMAND_UI(ID_SETUP_ASSAY_HISTORY, OnUpdateSetupAssayHistory)
	ON_UPDATE_COMMAND_UI(ID_SETUP_ASSAY_NEW,	OnUpdateSetupAssayNew)
	ON_UPDATE_COMMAND_UI(ID_SETUP_ASSAY_PRINT, OnUpdateSetupAssayPrint)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_DEL,		OnUpdateSetupMeasDel)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_DEL,		OnUpdateSetupMeasDel)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_EDIT,	OnUpdateSetupMeasEdit)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_EDIT,	OnUpdateSetupMeasEdit)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_HISTORY, OnUpdateSetupMeasHistory)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_NEW,		OnUpdateSetupMeasNew)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_NEW,		OnUpdateSetupMeasNew)
	ON_UPDATE_COMMAND_UI(ID_SETUP_MEAS_PRINT, OnUpdateSetupMeasPrint)
	ON_UPDATE_COMMAND_UI(ID_SETUP_SYSTEM, OnUpdateSetupSystem)
	ON_UPDATE_COMMAND_UI(ID_SETUP_SYSTEMLOG, OnUpdateSetupSystemlog)
	ON_UPDATE_COMMAND_UI(ID_SETUP_USERADMINISTRATION, OnUpdateSetupUseradministration)
	ON_UPDATE_COMMAND_UI(ID_SETUP_USERPREFERENCES, OnUpdateSetupUserpreferences)
	ON_UPDATE_COMMAND_UI(IDS_INDICATOR_KO, OnUpdateLumKOPane)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_VS_2005, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_FONTSIZE_6, ID_VIEW_FONTSIZE_16, CMainFrame::OnUpdateFontSize)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_USER,
	ID_INDICATOR_INSTRUMENT,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

bool	m_ForHistory = false;
bool	m_FieldCheck = false;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_nUsersReportBar = 0;
	m_bLocked = false;
	m_hLastFrame = NULL;
	m_nLook = ((CWinAppEx *)AfxGetApp())->GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_bmpBackground.LoadBitmapW( IDB_BACKGROUND_C3);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// Connect to LCC, disconnect is in OnClose handler
	if (!((CLuceptApp*)AfxGetApp())->CreateLcc())
	{	// Terminate application
		
		 // * Do not call AfxMessageBox because logging is not yet viable.
		::MessageBox( NULL, _T("Unable to connect to LCC process"), _T("Advance"), MB_OK);
		return -1;
	}

	// Specification setup requires the lcc which requires the mainframe
	if( Specification::g_oSpecification.Initialise( this) == false) {
		AfxMessageBox( _T( "Could not determine instrument specification"));
		return FALSE;
	}

	Specification::g_oSpecification.SetInstrumentIniDefaults();

	/*
	 * Disable loading of docking state from the registry. This is necessary to avoid annoying problems such
	 * as the user finding some way to hide a window and this hidden state being made permanent via registry
	 * settings.
	 */
	GetDockingManager()->DisableRestoreDockState();

	EnableMDITabs(TRUE,0,CMFCBaseTabCtrl::LOCATION_TOP,0,CMFCTabCtrl::STYLE_3D_SCROLLED,0,1);

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Blank out the logged in user until someone has logged in
	m_wndStatusBar.SetPaneText( m_wndStatusBar.CommandToIndex( ID_INDICATOR_USER), _T(""));

	// Create the ribbon bar
	if (!m_wndRibbonBar.Create(this))
	{
		return -1;   //Failed to create ribbon bar
	}

	if (!m_wndRibbonBar.LoadFromResource(IDR_RIBBON_MAIN))
	{
		return -1;
	}	

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
//	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);

	// Create View toolbar
//	if (!m_wndViewSelect.Create(this) ||!m_wndViewSelect.LoadToolBar(IDR_WORKLOAD))
//	{
//		TRACE0("Failed to create toolbar\n");
		// return -1;      // fail to create
//	}
	// ! Install replaces old comctrl32.dll allowing use of tooltips
//	m_wndViewSelect.SetPaneStyle(m_wndViewSelect.GetPaneStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC );
//	m_wndViewSelect.SetWindowText(_T("Workload View Select"));

//	m_wndViewSelect.EnableDocking(CBRS_ALIGN_ANY); 
//	DockControlBarLeftOf(&m_wndViewSelect, &m_wndToolBar);

	EnableAutoHidePanes( CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY); 

	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	CDockingManager::SetDockingMode(DT_SMART);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	// Connect to LCC, disconnect is in OnClose handler
	if (!((CLuceptApp*)AfxGetApp())->CreateLcc())
	{	// Terminate application
		/*
		 * Do not call AfxMessageBox because logging is not yet viable.
		 */
		::MessageBox( NULL, _T("Unable to connect to LCC process"), _T("Advance"), MB_OK);
		return -1;
	}
  
	// The lcc will change advance 2 tubes back to advance 1 numbers, 164: change it back.
	Sleep( 1000);
	Specification::g_oSpecification.SetInstrumentIniDefaults();
  
	// Create the LucyBar.
	if (!m_LucyBar.Create(this, IDD_LUCY_BAR,CBRS_BOTTOM|CBRS_TOOLTIPS|CBRS_FLYBY|WS_VISIBLE, IDD_LUCY_BAR+56)) {
		TRACE0("Failed to create Lucy Bar\n");
		return -1;      // fail to create
	}

	 m_LucyBar.EnableDocking(CBRS_ALIGN_ANY);  
	 m_LucyBar.EnableGripper( FALSE);
	 EnableDocking(CBRS_ALIGN_ANY); //Surely already there);  
	 AddPane( &m_LucyBar);
	 m_LucyBar.ShowWindow( SW_SHOW);
	 m_LucyBar.InitDialog();

	 /**
	  * Remove the option to create new reports if it is not supported in this version.
	  */
	 if( Specification::g_oSpecification.HasReports() == false) {
		 int nFileIndex = GetCategoryIndexByName( _T("File"));

		 ASSERT( nFileIndex >= 0);

		 CMFCRibbonCategory *pCategory = m_wndRibbonBar.GetCategory( nFileIndex);

 		CMFCRibbonButton *pButton = (CMFCRibbonButton *)(pCategory->FindByID( ID_FILE_NEW));

		pButton->RemoveSubItem( 2); // 2 = index counting from 0-- yeah microsoft

 		pButton = (CMFCRibbonButton *)(pCategory->FindByID( ID_FILE_OPEN));

		pButton->RemoveSubItem( 3); // 3 = index counting from 0-- yeah microsoft
		pButton->RemoveSubItem( 2); // <- index changes after deletion
	 }

	 /**
	  * Remove the debug menu if not enabled in the ini file.
	  */
	 if( ((CLuceptApp *)AfxGetApp())->GetLuceptProfileInt( _T("Features"), _T("DebugMenu"), 0) == 0) {
 		 int nMaintenanceIndex = GetCategoryIndexByName( _T("maintenance"));

		 ASSERT( nMaintenanceIndex >= 0);

		 CMFCRibbonCategory *pCategory = m_wndRibbonBar.GetCategory( nMaintenanceIndex); 

		// Panels were written by a different subdivision of microsoft corp and hence they index from 0.
 		pCategory->RemovePanel( 2, TRUE);
	 }

	 if( Specification::g_oSpecification.HasReports() == false) {
	 	ShowCategory( _T("Report"), false);
	 }

	 /**
	  * Remove the door button if this is not a C3.
	  */
	 if( Specification::g_oSpecification.GetInstrumentType() != INSTRUMENT_C3) {
		 CMFCRibbonCategory *pCategory = m_wndRibbonBar.GetCategory( 1);  // Get Home category- 1 = index counting from 1

		 pCategory->GetPanel(3)->Remove( 2);  // Panels and buttons cound from 0
	 }

	LucEnableTick();
	return 0;
}

BOOL CMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( nCode == CN_UPDATE_COMMAND_UI) {
		if( pExtra != NULL) {
			if( gGateKeeper.IsAppLocked() == true && nID != ID_LOCK_APP) {

				((CCmdUI *)pExtra)->Enable( FALSE);
				return TRUE;
			}
		}
	}

	return CMDIFrameWndEx::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnClose() 
{	
	// Don't close if locked.
	if( gGateKeeper.IsAppLocked()) {
		OnLockApp();   // Unlock and update ui.
		if( gGateKeeper.IsAppLocked()) {
			return;
		}
	}

	// Don't close if workload is running
	if (m_bLocked)
	{
		AfxMessageBox(IDS_WKL_BUSY);
		OnFileLogon();
		return;
	}

	// Disconnect LCC
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	if (pApp->GetLccMgr() != NULL)
	{
		pApp->DestroyLcc();
	}

	// Close graphic 
	((CLucyDoc *)(pApp->m_pLucy->GetDocument()))->m_bCanClose = TRUE;

	CMDIFrameWndEx::OnClose();

	// *** SYSTEM LOG **********************************************************
	CLogEvent	NewEvent;							       // Create a new event   *
	NewEvent.EventSet(_T("Application Terminating       "),// Event name           *
						_T(""));						   // Event details        *
	CSystemLog::g_oSystemLog.EventAdd( NewEvent);
}

void CMainFrame::OnWindowArrange()
{
	if( Specification::g_oSpecification.GetInstrumentType() == INSTRUMENT_ADVANCE_2) {
		if( GetMDITabs().GetTabsNum() > 1) { // If only 1 tab, don't minimize it
			((CLuceptApp *)AfxGetApp())->m_pLucy->GetParentFrame()->ShowWindow( SW_MINIMIZE);
		}
	}
	MDICascade();
}

void CMainFrame::OnWindowTileVert()
{
	if( Specification::g_oSpecification.GetInstrumentType() == INSTRUMENT_ADVANCE_2) {
		if( GetMDITabs().GetTabsNum() > 1) { // If only 1 tab, don't minimize it
			((CLuceptApp *)AfxGetApp())->m_pLucy->GetParentFrame()->ShowWindow( SW_MINIMIZE);
		}
	}
	MDITile( MDITILE_VERTICAL);
}

void CMainFrame::OnWindowTileHorz()
{
	if( Specification::g_oSpecification.GetInstrumentType() == INSTRUMENT_ADVANCE_2) {
		if( GetMDITabs().GetTabsNum() > 1) { // If only 1 tab, don't minimize it
			((CLuceptApp *)AfxGetApp())->m_pLucy->GetParentFrame()->ShowWindow( SW_MINIMIZE);
		}
	}
	MDITile( MDITILE_HORIZONTAL);
}

LRESULT CMainFrame::OnAfxWmChangingActiveTab(WPARAM wParam, LPARAM lParam)
{
	CMFCTabCtrl* pTab = (CMFCTabCtrl*)lParam;

	CWnd *pWnd = pTab->GetActiveWnd();

	if( NULL != pWnd) {
		WINDOWPLACEMENT oPlacement;

		pWnd->GetWindowPlacement( &oPlacement);
		if( oPlacement.showCmd == SW_SHOWMINIMIZED) {
			pWnd->ShowWindow( SW_MAXIMIZE);
		}
	}

	return 0;

}

void CMainFrame::DockControlBarLeftOf(CMFCToolBar* Bar,CMFCToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetPaneStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockPane(Bar,n,&rect);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	// Don't auto-append filename to title
	cs.style &= ~FWS_ADDTOTITLE;

	return CMDIFrameWndEx::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSetupPaths() 
{
	// TODO: Add your command handler code here	
}

void CMainFrame::OnMaintenancePrime() 
{
	CInjSeq dlg(MAINT_SEQ_PRIME);

	dlg.DoModal();
}

void CMainFrame::OnMaintenanceRefresh() 
{
	CInjSeq dlg(MAINT_SEQ_REFRESH);

	dlg.DoModal();
}

void CMainFrame::OnMaintenanceInjectorStartup() 
{
	CInjSeq dlg(MAINT_SEQ_STARTUP);

	dlg.DoModal();
}

void CMainFrame::OnMaintenanceInjectorShutdown() 
{
	CInjSeq dlg(MAINT_SEQ_SHUTDOWN);

	dlg.DoModal();
}

void CMainFrame::OnMaintenanceInjectorWash() 
{
	CInjSeq dlg(MAINT_SEQ_WASH);

	dlg.DoModal();
}

void CMainFrame::OnMaintenanceInjectorRinse() 
{
	CInjSeq dlg(MAINT_SEQ_RINSE);

	dlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// Create TextView 
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnLccView() 
{
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();
	
	if (pApp->m_pLccVDoc != NULL)
	{	// Document (and therefore view) already exist
		ASSERT(pApp->m_pLccView->IsKindOf(RUNTIME_CLASS(CLccView)));
		// Activate and restore parent which is an MDIChildWnd
		WINDOWPLACEMENT wndPlace;
		if (!pApp->m_pLccView->GetOwner()->GetWindowPlacement(&wndPlace))
		{
			return;
		}
		
		wndPlace.showCmd = SW_SHOWNORMAL;
		pApp->m_pLccView->GetOwner()->SetWindowPlacement(&wndPlace);
	
		// ... and exit
		return;
	}

	// Create a document
	// PCW- ripped out old weird way of creating document/view
//	pApp->m_pLccVDoc = new CLccVDoc;
	
	// Create  View frame
//	CFrameWnd* pFrame = pApp->m_pTextViewTemplate->CreateNewFrame(pApp->m_pLccVDoc, NULL);
//	ASSERT(pFrame != NULL);

	// ... and initialize it
//	pApp->m_pTextViewTemplate->InitialUpdateFrame(pFrame, pApp->m_pLccVDoc);

	// Get pointer to view
//	POSITION pos = pApp->m_pLccVDoc->GetFirstViewPosition(); 
//	pApp->m_pLccView = (CLccView*)pApp->m_pLccVDoc->GetNextView(pos);
//	ASSERT(pApp->m_pLccView->IsKindOf(RUNTIME_CLASS(CLccView)));

	pApp->m_pLccVDoc =(CLccVDoc*) pApp->m_pTextViewTemplate->OpenDocumentFile(NULL);
	POSITION pos = pApp->m_pLccVDoc->GetFirstViewPosition(); 
	pApp->m_pLccView = (CLccView*)pApp->m_pLccVDoc->GetNextView(pos);
	// PreCreateWindow is touted as place to set window title but this actually works
	pApp->m_pLccVDoc->SetTitle( _T("Lcc View"));

	// Initialize edit control
	CEdit& editCtrl = pApp->m_pLccView->GetEditCtrl();
	editCtrl.SetReadOnly();

	return;
}


/////////////////////////////////////////////////////////////////////////////
// Close Text view 
/////////////////////////////////////////////////////////////////////////////

void CMainFrame::CloseLccView()
{
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();
	if (pApp->m_pLccView == NULL)
	{
		return;
	}
	
	// Found a text view.  Close it.
	// (MDIChilWnd is parent of view window)
	CMDIChildWndEx* pSVFrame = (CMDIChildWndEx*)pApp->m_pLccView->GetParent();
	ASSERT(pSVFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx)));

	// Destroying frame destroys view
	// Destroying the last (only) view causes the document
	// to self-destruct.  Both the view and the doc will NULL
	// the appropriate pointers in the App on destruction
	pSVFrame->MDIDestroy();
}



/////////////////////////////////////////////////////////////////////////////
// Create sample view of workload
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnSampleView() 
{
	// Verify that we have an active workload view and document
	CDocument* pDocument;

	CMDIChildWndEx* pActiveChild = (CMDIChildWndEx*)MDIGetActive();

	if (pActiveChild == NULL ||(pDocument = pActiveChild->GetActiveDocument()) == NULL)
	{
		AfxMessageBox( _T("No workload is active"));
		return;
	}

	// Check if the active view is a graphic view
	if (pDocument->IsKindOf(RUNTIME_CLASS(CLucyDoc)))
	{	// Reqest is from graphic view substitute WklDoc 
		pDocument = ((CLuceptApp*)AfxGetApp())->m_pLucy->GetWklDoc();
		if (pDocument == NULL)
		{
			AfxMessageBox( _T("No workload is active"));
			return;
		}
		// else the new PDocument puts us back in the flow
	}

	// Check if a Sample View already exists
	POSITION pos = pDocument->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDocument->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CSampleView)))
		{
			pView->GetParentFrame()->ActivateFrame();

			// ... and exit
			return;
		}
	}   

	// Create Sample View frame
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();
	CFrameWnd* pFrame = pApp->m_pSampViewTemplate->CreateNewFrame(pDocument, NULL);
	ASSERT(pFrame != NULL);

	// ... and initialize it
	pApp->m_pSampViewTemplate->InitialUpdateFrame(pFrame, pDocument);

	return;
}


/////////////////////////////////////////////////////////////////////////////
// Close sample view of workload
/////////////////////////////////////////////////////////////////////////////

void CMainFrame::CloseSampView()
{
	// Verify that we have an active workload view 
	CDocument* pDocument;
	CMDIChildWndEx* pActiveChild = (CMDIChildWndEx*)MDIGetActive();
	ASSERT(pActiveChild != NULL);

	// Verify that we have an active document
	pDocument = pActiveChild->GetActiveDocument();
	ASSERT(pDocument != NULL);

	// Check if a Sample View exists
	POSITION pos = pDocument->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDocument->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CSampleView)))
		{	// Found a sample view.  Close it.
			// (MDIChilWnd is parent of view window)
			CMDIChildWndEx* pSVFrame = (CMDIChildWndEx*)pView->GetParent();
			ASSERT(pSVFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx)));
			pSVFrame->MDIDestroy();
		}	
	}   
}


/////////////////////////////////////////////////////////////////////////////
// Timer service
/////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSetupAssayNew() 
{
	CAssayNew newDlg;	
	newDlg.DoModal();
}

void CMainFrame::OnSetupAssayEdit() 
{
	CAssaySel selDlg(FOR_EDIT);	
	selDlg.DoModal();	
}

void CMainFrame::OnSetupAssayDel() 
{
	CAssaySel selDlg(FOR_DELETE);	
	selDlg.DoModal();
}

void CMainFrame::OnSetupAssayPrint() 
{
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	// ITL: If there is one open already, just return to it
	if(pApp->m_pAssayPrDoc)
	{	// ITL: Can't do an assay is open
		// ITL: Added by CAR 887/002
		AfxMessageBox(_T("You must close the open Assay before printing another one"));
		return;
	}

	// Get Assay def pointer
	CAssaySel selDlg(FOR_PRINT);	
	if(selDlg.DoModal() != IDOK)
	{
		return;
	}
	
	CMDIChildWndEx *pFrm	= NULL;	
	
	// Create a document
	pApp->m_pAssayPrDoc = new CAssayPrDoc(*selDlg.m_pAssayDef); 
	ASSERT(pApp->m_pAssayPrDoc);

	// Create frame window
	pFrm = (CMDIChildWndEx*)pApp->m_pAssayPrintTemplate->CreateNewFrame(pApp->m_pAssayPrDoc, NULL);
	ASSERT(pFrm != NULL);

	// ... and initialize it
	pApp->m_pAssayPrintTemplate->InitialUpdateFrame(pFrm, pApp->m_pAssayPrDoc);
	CString tmp;
	tmp = _T("Assay Print:  "); //!!!resource
	tmp += selDlg.m_pAssayDef->GetName();
	pFrm->SetWindowText(tmp);

	// Get pointer to view
	POSITION pos = pApp->m_pAssayPrDoc->GetFirstViewPosition(); 
	pApp->m_pAssayPrView = (CAssayPrView*)pApp->m_pAssayPrDoc->GetNextView(pos);
	ASSERT(pApp->m_pAssayPrView->IsKindOf(RUNTIME_CLASS(CAssayPrView)));
}

void CMainFrame::OnSetupMeasPrint() 
{
	//!!! If there is one open already, just return to it? replace it? open another?
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	if(pApp->m_pMeasPrDoc)
	{	// ITL: Can't do an measurement is open
		// ITL: Added by CAR 887/002
		AfxMessageBox(_T("You must close the open Measurement before printing another one"));
		return;
	}

	CMeasSel selDlg(FOR_PRINT);	
	if(selDlg.DoModal() != IDOK)
	{
		return;
	}

	//get pointer and create document/view/frame here
	CMDIChildWndEx *pFrm	= NULL;	
	
	// Create a document
	pApp->m_pMeasPrDoc = new CMeasPrDoc(*selDlg.m_pMeasDef); 
	ASSERT(pApp->m_pMeasPrDoc);

	// Create frame window
	pFrm = (CMDIChildWndEx*)pApp->m_pMeasPrintTemplate->CreateNewFrame(pApp->m_pMeasPrDoc, NULL);
	ASSERT(pFrm != NULL);

	// ... and initialize it
	pApp->m_pMeasPrintTemplate->InitialUpdateFrame(pFrm, pApp->m_pMeasPrDoc);
	CString tmp;
	tmp = _T("Measurement Print:  "); //!!!resource
	tmp += selDlg.m_pMeasDef->GetName();
	pFrm->SetWindowText(tmp);

	// Get pointer to view
	POSITION pos = pApp->m_pMeasPrDoc->GetFirstViewPosition(); 
	pApp->m_pMeasPrView = (CMeasPrView*)pApp->m_pMeasPrDoc->GetNextView(pos);
	ASSERT(pApp->m_pMeasPrView->IsKindOf(RUNTIME_CLASS(CMeasPrView)));

}

void CMainFrame::OnSetupMeasNew() 
{
	CMeasNew newDlg;	
	newDlg.DoModal();
}

void CMainFrame::OnSetupMeasEdit() 
{
	CMeasSel selDlg(FOR_EDIT);	
	selDlg.DoModal();	
}

void CMainFrame::OnSetupMeasDel() 
{
	CMeasSel selDlg(FOR_DELETE);	
	selDlg.DoModal();
}



void CMainFrame::OnMaintenanceManual() 
{
	CHandCtrl dialog;
	dialog.DoModal();
}


void CMainFrame::OnSetupUserpreferences() 
{
}


void CMainFrame::OnMeasurementDatadialog() 
{
}

void CMainFrame::OnMeasurementInquire() 
{
	if (m_LccInqDlg.GetSafeHwnd() == NULL)
	{
		m_LccInqDlg.Create(CLccInqDlg::IDD, this);
	}
	else
	{
		m_LccInqDlg.SetActiveWindow();
	}
}


/////////////////////////////////////////////////////////////////////////////
// User Administration
/////////////////////////////////////////////////////////////////////////////


void CMainFrame::OnFileLogon() 
{	
	gGateKeeper.UserLogOn();
}

void CMainFrame::OnUpdateFileLogon(CCmdUI* pCmdUI) 
{
	if (gGateKeeper.IsUserLoggedOn()) 
	{
		pCmdUI->SetText(_T("&Logoff"));
	} 
	else 
	{
		pCmdUI->SetText(_T("&Logon"));		
	}
}


void CMainFrame::OnFileOpenWorkload()
{
	CString strFileName;
	CLuceptApp *pApp = (CLuceptApp *)AfxGetApp();

	if( pApp->DoPromptFileName( strFileName, AFX_IDS_OPENFILE,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, pApp->m_pWklViewTemplate)) {
		pApp->OpenDocumentFile( strFileName);
	}
}

void CMainFrame::OnFileOpenReportTemplate()
{
	CString strFileName;
	CLuceptApp *pApp = (CLuceptApp *)AfxGetApp();

	if( pApp->DoPromptFileName( strFileName, AFX_IDS_OPENFILE,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, pApp->m_pRptViewTptTemplate)) {
		pApp->OpenDocumentFile( strFileName);
	}
}

void CMainFrame::OnFileOpenWorkloadTemplate()
{
	CString strFileName;
	CLuceptApp *pApp = (CLuceptApp *)AfxGetApp();

	if( pApp->DoPromptFileName( strFileName, AFX_IDS_OPENFILE,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, pApp->m_pWklViewWktTemplate)) {
		pApp->OpenDocumentFile( strFileName);
	}
}

void CMainFrame::OnFileOpenReport()
{
	CString strFileName;
	CLuceptApp *pApp = (CLuceptApp *)AfxGetApp();

	if( pApp->DoPromptFileName( strFileName, AFX_IDS_OPENFILE,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, pApp->m_pRptViewTemplate)) {
		pApp->OpenDocumentFile( strFileName);
	}
}

void CMainFrame::OnSetupUseradministration() 
{
	gGateKeeper.DoUserAdmin();	
}

void CMainFrame::OnUpdateSetupUseradministration(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_USER_ADMN));	
}

void CMainFrame::OnUpdateMaintenanceManual(CCmdUI* pCmdUI) 
{
	if( IsLccAvailable()) {
		pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_MANUAL_OP));	
	} else {
		pCmdUI->Enable( FALSE);
	}
}

void CMainFrame::OnUpdateUserStatusPane(CCmdUI* pCmdUI)
{	
	if(gGateKeeper.IsUserLoggedOn() == TRUE)
	{		
		pCmdUI->Enable(TRUE);
		pCmdUI->SetText(gGateKeeper.CurrentUserNameGet());
	} 
	else 
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateLumKOPane(CCmdUI* pCmdUI)
{	
	pCmdUI->Enable(TRUE);
	pCmdUI->SetText(m_lumKO);
}

void CMainFrame::OnUpdateSetupAssayNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_ASSAY_DEF) && IsLccAvailable());	
}

void CMainFrame::OnUpdateSetupAssayEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_ASSAY_DEF) && IsLccAvailable());	
}

void CMainFrame::OnUpdateSetupAssayDel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_ASSAY_DEF) && IsLccAvailable());	
}

void CMainFrame::OnUpdateSetupAssayPrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_ASSAY_DEF) && IsLccAvailable());		
}

void CMainFrame::OnUpdateSetupMeasNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_MEASU_DEF) && IsLccAvailable());	
}

void CMainFrame::OnUpdateSetupMeasEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_MEASU_DEF) && IsLccAvailable());	
}

void CMainFrame::OnUpdateSetupMeasDel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_MEASU_DEF) && IsLccAvailable());	
}

void CMainFrame::OnUpdateSetupMeasPrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_MEASU_DEF) && IsLccAvailable());	
}

void CMainFrame::OnUpdateSetupUserpreferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_SYST_PREF));	
}

void CMainFrame::LockWindow(bool lock)
{
	m_bLocked = lock;
}

void CMainFrame::OnUpdateSampleView(CCmdUI* pCmdUI) 
{
	// Only enable if active view is a workload view	
	bool enable = false;

	CMDIChildWndEx* pActiveChild = (CMDIChildWndEx*)MDIGetActive();
	if (pActiveChild != NULL )
	{
		CDocument* pDocument = pActiveChild->GetActiveDocument();
		if (pDocument->IsKindOf(RUNTIME_CLASS(CWklDoc)))
		{
			// Check is current batch and sample are valid
			if (((CWklDoc*)pDocument)->GetBatchPtr() != NULL
				&& ((CWklDoc*)pDocument)->GetBatchPtr()->GetNumSamples() > 0)
			{
				enable = true;
			}
		}
		else if (pDocument->IsKindOf(RUNTIME_CLASS(CLucyDoc)))
		{
			// !!! Check if a registered batch exists with samps
		}
	}

	pCmdUI->Enable(enable);
}

void CMainFrame::DrawLumStatus(LPCTSTR text)
{
	if (text != NULL)
	{
		m_lumKO = text;
	}
}

void CMainFrame::OnRoutineReagentlevels() 
{
	CAddReagents addReagents;
	
	addReagents.DoModal();

}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	LucDisableTick();
	CMDIFrameWndEx::OnSysCommand(nID, lParam);
	LucEnableTick();
}

CWklDoc* CMainFrame::GetActiveWklDoc()
{
	// Verify that we have an active view
	CMDIChildWndEx* pActiveChild = (CMDIChildWndEx*)MDIGetActive();
	if (pActiveChild == NULL)
	{
		return NULL;
	}
	
	// Verify that the active view refers to a CWklDoc
	CWklDoc* pWklDoc = (CWklDoc*)pActiveChild->GetActiveDocument();
	if (pWklDoc == NULL ||
		!pWklDoc->IsKindOf(RUNTIME_CLASS(CWklDoc)))
	{
		return NULL;
	}

	// Return pointer to active workload document
	return pWklDoc;
}

void CMainFrame::OnGridView() 
{
	// If active frame is graphic, de-activate
	CLucyFrame* pActiveFrame = (CLucyFrame*)GetActiveFrame();
	if (pActiveFrame->IsKindOf(RUNTIME_CLASS(CLucyFrame)))
	{
		if (m_hLastFrame != NULL)
		{
			CFrameWnd* pLastFrame = GetLastFrame(m_hLastFrame);

			// Check if last frame is (still) valid
			if (pLastFrame != NULL)
			{	// Re-activate 
				((CMDIChildWndEx*)pLastFrame)->MDIActivate();
				// !!! and restore placement
			}
			else
			{
				m_hLastFrame = NULL;
				MDINext();
			}
		}
		else
		{	// Just activate the next available frame window
			MDINext();
		}
	}

	// Get graphic frame and hide
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	CLucyFrame* pLucyFrame = (CLucyFrame*)pApp->m_pLucy->GetParentFrame();
	ASSERT(pLucyFrame->IsKindOf(RUNTIME_CLASS(CLucyFrame)));
	pLucyFrame->ShowWindow(SW_HIDE);
}

void CMainFrame::OnGraphicView() 
{
	// Save active frame for later restoration
	CFrameWnd* pLastFrame = GetActiveFrame();
	if (pLastFrame != (CFrameWnd*)this)
	{
		m_hLastFrame = pLastFrame->m_hWnd;
	}
	else
	{
		m_hLastFrame = NULL;
	}

	// Point to the _T("current") workload
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	CWklDoc* pWklDoc = GetActiveWklDoc();
	if (pWklDoc != NULL)
	{
		pApp->m_pLucy->SetWklDoc(pWklDoc);
	}

	// Get graphic frame, show it (and MDIActivate it)
	CLucyFrame* pLucyFrame = (CLucyFrame*)pApp->m_pLucy->GetParentFrame();
	ASSERT(pLucyFrame->IsKindOf(RUNTIME_CLASS(CLucyFrame)));

	pLucyFrame->ShowWindow(SW_SHOW);
	pLucyFrame->MDIActivate();
}


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);

	// If lucy view is initialized and visible resize it
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	if(pApp->m_pLucy)
	{
		CLucyFrame* pLucyFrame = (CLucyFrame*)pApp->m_pLucy->GetParentFrame();
		if (pLucyFrame->IsWindowVisible())
		{
			pLucyFrame->UpdateSize();
		}
	}

	Invalidate( TRUE);

	// !!! Can I keep a sample view on top while
	// updating size?
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
	TRACE0("OnPaletteChanged\n");
	CMDIFrameWndEx::OnPaletteChanged(pFocusWnd);
	
	// Redraw with new palette
	// All child windows using custom pallete need to select
	// that palette with forceBackground = TRUE.  The custom pallete
	// will have been realized by mainframe and the mapping will be
	// exact if this program has the focus.
	Invalidate(TRUE);

	// Notify all grid windows so they may _T("realize") their
	// colors and redraw using the new palette

	CWnd* pChild = GetActiveFrame();
	if (pChild == this)
	{	// No child frames
		return;
	}

	while (pChild != NULL)
	{
		if (pChild->IsKindOf(RUNTIME_CLASS(CWklFrame)))
		{	// Pass notification to all workload view windows
			CWklView* pWklView = (CWklView*)
				((CWklFrame*)pChild)->GetActiveView();
			ASSERT(pWklView->IsKindOf(RUNTIME_CLASS(CWklView)));

			pWklView->OnPaletteChanged(pFocusWnd);
		}
		else if (pChild->IsKindOf(RUNTIME_CLASS(CLucyFrame)))
		{	// Notify LucyView
			CLucyView* pLucyView = (CLucyView*)
				((CLucyFrame*)pChild)->GetActiveView();
			ASSERT(pLucyView->IsKindOf(RUNTIME_CLASS(CLucyView)));

			pLucyView->OnPaletteChanged(pFocusWnd);
		}

		// Get next sibling
		pChild = pChild->GetNextWindow();
	}
}


BOOL CMainFrame::OnQueryNewPalette() 
{
		TRACE0("OnQuerryPalette\n");
	CPalette* pPal = ((CLuceptApp*)AfxGetApp())->GetCustPal();
	if (pPal == NULL)
	{
		return false;
	}

	// Get a device context to realize the palette in
	CClientDC tempDC(this);

	// Select the custom palette, MAKE the colors
	CPalette* oldPalette = tempDC.SelectPalette(pPal, false);
	if (oldPalette != NULL)
	{	// Palette selected OK
		UINT nColorsChanged = tempDC.RealizePalette();
		// Restore org palette
		tempDC.SelectPalette(oldPalette, TRUE);

		return CMDIFrameWndEx::OnQueryNewPalette();
	}
	else
	{
		TRACE0("\tSelectPalette failed in CLucyView::OnPaletteChanged\n");

		return false;
	}
}
	
CFrameWnd* CMainFrame::GetLastFrame(HWND hFrame)
{
	bool valid = false;

	// Find MDIClient window, always first child
	CWnd* pMDIClient = GetWindow(GW_CHILD);
	pMDIClient = GetWindow(GW_CHILD);
	ASSERT(pMDIClient != NULL);
#ifdef _DEBUG
	// verify we got the client window
	TCHAR wndClass[31];
	VERIFY(GetClassName(pMDIClient->m_hWnd, wndClass, 63) > 0); 
// PCW- does not work with ribbon bar as that is the last client	ASSERT(CString(wndClass).CompareNoCase(_T("MDICLIENT")) == 0);
#endif

	// search children for frame window
	CWnd* pChild = pMDIClient->GetWindow(GW_CHILD);
	if (pChild == NULL)
	{
		return NULL;
	}
	CWnd* pLastSibling = pChild->GetWindow(GW_HWNDLAST);

	while (!valid && pChild != NULL)
	{
		valid = pChild->m_hWnd == hFrame;
		if (!valid)
		{
			if (pChild->m_hWnd != pLastSibling->m_hWnd)
			{
				pChild = pChild->GetWindow(GW_HWNDNEXT);
			}
			else 
			{
				pChild = NULL;
			}
		}
	}

	if (valid)
	{
		return (CFrameWnd*)pChild;
	}
	else
	{
		return NULL;
	}
}

void CMainFrame::OnSetupSystem() 
{
	CString strMod;
	int PharmaSec;
	CLuceptApp* pApp = NULL;

	pApp	  = (CLuceptApp*)AfxGetApp();
	PharmaSec = pApp->GetLuceptProfileInt(gIniSec_Features,    gIniEnt_PharmaSec, -1 );
	pApp->SetupIniFiles( PharmaSec);	
}

void CMainFrame::OnUpdateSetupSystem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_SYST_PREF));		
}


void CMainFrame::OnHelpCustomerservice() 
{	// ITL: Display Customer Service Help message
	CString msg;
	msg =
	_T("For additional information and assistance please" 
	_T("\r\ntelephone the Celsis Technical Support Helpline:\r\n")
	_T("\r\n\t +32 2 541 15 20 (Europe)")
	_T("\r\n\t +1 (1) 800 222 8260 (USA)")
	_T("\r\n\r\nor your local Celsis representative."));
	AfxMessageBox(msg);
}

void CMainFrame::OnSetupChangepassword() 
{
	CUserAdministrator oAdmin;

	oAdmin.OnSetupChangepassword();
}

void CMainFrame::OnSetupAssayHistory() 
{
	m_ForHistory = true;

	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	// ITL: Ensure lists are updated from disk
	pApp->m_assays.InitFromDisk();
	pApp->m_assaysHistory.Init();
	
	CAssaySel selDlg(FOR_HISTORY);
	if(selDlg.DoModal() != IDOK)
	{
		return;
	}

	CMDIChildWndEx *pFrm	= NULL;	
	
	// Create a document
	pApp->m_pAssayPrDoc = new CAssayPrDoc(*selDlg.m_pAssayDef); 
	ASSERT(pApp->m_pAssayPrDoc);

	// Create frame window
	pFrm = (CMDIChildWndEx*)pApp->m_pAssayPrintTemplate->CreateNewFrame(pApp->m_pAssayPrDoc, NULL);
	ASSERT(pFrm != NULL);

	// ... and initialize it
	pApp->m_pAssayPrintTemplate->InitialUpdateFrame(pFrm, pApp->m_pAssayPrDoc);
	CString tmp;
	tmp = _T("Assay Print:  ");
	tmp += selDlg.m_pAssayDef->GetName();
	pFrm->SetWindowText(tmp);

	// Get pointer to view
	POSITION pos = pApp->m_pAssayPrDoc->GetFirstViewPosition(); 
	pApp->m_pAssayPrView = (CAssayPrView*)pApp->m_pAssayPrDoc->GetNextView(pos);
	ASSERT(pApp->m_pAssayPrView->IsKindOf(RUNTIME_CLASS(CAssayPrView)));	

	m_ForHistory = false;
}

void CMainFrame::OnUpdateSetupAssayHistory(CCmdUI* pCmdUI) 
{	// ITL: This item should only be avaialable if the user has admin rights
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_USER_ADMN));	
}

void CMainFrame::OnSetupMeasHistory() 
{
	m_ForHistory = true;

	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	// ITL: Ensure lists are updated from disk
	pApp->m_meass.InitFromDisk();
	pApp->m_meassHistory.Init();

	CMeasSel selDlg(FOR_HISTORY);
	if(selDlg.DoModal() != IDOK)
	{
		return;
	}

	//get pointer and create document/view/frame here
	CMDIChildWndEx *pFrm	= NULL;	
	
	// Create a document
	pApp->m_pMeasPrDoc = new CMeasPrDoc(*selDlg.m_pMeasDef); 
	ASSERT(pApp->m_pMeasPrDoc);

	// Create frame window
	pFrm = (CMDIChildWndEx*)pApp->m_pMeasPrintTemplate->CreateNewFrame(pApp->m_pMeasPrDoc, NULL);
	ASSERT(pFrm != NULL);

	// ... and initialize it
	pApp->m_pMeasPrintTemplate->InitialUpdateFrame(pFrm, pApp->m_pMeasPrDoc);
	CString tmp;
	tmp = _T("Measurement Print:  "); //!!!resource
	tmp += selDlg.m_pMeasDef->GetName();
	pFrm->SetWindowText(tmp);

	// Get pointer to view
	POSITION pos = pApp->m_pMeasPrDoc->GetFirstViewPosition(); 
	pApp->m_pMeasPrView = (CMeasPrView*)pApp->m_pMeasPrDoc->GetNextView(pos);
	ASSERT(pApp->m_pMeasPrView->IsKindOf(RUNTIME_CLASS(CMeasPrView)));	

	m_ForHistory = false;
}

void CMainFrame::OnUpdateSetupMeasHistory(CCmdUI* pCmdUI) 
{	// ITL: This item should only be avaialable if the user has admin rights
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_USER_ADMN));	
}

void CMainFrame::OnSetupSystemlog() 
{
	CLuceptApp* pApp = (CLuceptApp*) AfxGetApp();

	if(pApp->m_pSystemLog)
	{
		POSITION nView = pApp->m_pSystemLog->GetFirstViewPosition();

		if( nView != NULL) {
			CWnd *pWnd = pApp->m_pSystemLog->GetNextView( nView);

			while( 1) {
				CWnd *pParent = pWnd->GetParent();

				if( pParent == NULL) {
					break;
				}

				if( pParent->IsKindOf( RUNTIME_CLASS( CFrameWnd))) {
					((CFrameWnd *)pParent)->ActivateFrame();
					return;
				}

				pWnd = pParent;
			}
		}

		// ITL: Can't do if a system log is open
		AfxMessageBox(_T("You must close the open system log first\nIf it is not visible click Grid View button"));
		return;
	}

	BOOL maximized = 0;	// ITL: Keep track of window
	CMDIChildWndEx *pFrm	= (CMDIChildWndEx*)MDIGetActive(&maximized);	
	if( pFrm && (!maximized)) {
		// ITL: We need this window maximised
		pFrm->MDIMaximize();    
	}

	// Create a document
	pApp->m_pSystemLog = (CSystemLogDoc *)NewDoc( pApp->m_pSysLogPrintTemplate);
	if( pApp->m_pSystemLog == NULL) {
		return;
	}
}

void CMainFrame::OnUpdateSetupSystemlog(CCmdUI* pCmdUI) 
{	// ITL: This item should only be avaialable if the user has admin rights
	pCmdUI->Enable(gGateKeeper.IsCurrentUserPermitted(PRIV_USER_ADMN));	
}


void CMainFrame::OnUpdateFontSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();		
}

void CMainFrame::OnFontSize( UINT nID)
{
	int nSize;

	switch( nID) {
		case ID_VIEW_FONTSIZE_6:
			nSize = 6;
			break;

		case ID_VIEW_FONTSIZE_7:
			nSize = 7;
			break;

		case ID_VIEW_FONTSIZE_8:
			nSize = 8;
			break;

		case ID_VIEW_FONTSIZE_9:
			nSize = 9;
			break;

		case ID_VIEW_FONTSIZE_10:
			nSize = 10;
			break;

		case ID_VIEW_FONTSIZE_11:
			nSize = 11;
			break;

		case ID_VIEW_FONTSIZE_12:
			nSize = 12;
			break;

		case ID_VIEW_FONTSIZE_13:
			nSize = 13;
			break;

		case ID_VIEW_FONTSIZE_14:
			nSize = 14;
			break;

		case ID_VIEW_FONTSIZE_15:
			nSize = 15;
			break;

		case ID_VIEW_FONTSIZE_16:
			nSize = 16;
			break;
	}

	((CLuceptApp *)AfxGetApp())->WriteLuceptProfileInt( gIniSec_Features, gIniEnt_FontSize, nSize);

	/*
	 * Walk through open windows, telling worklists that the font has changed.
	 * If there is an api to get the first frame window I can't find it. Get
	 * the active frame then walk back through the frames until the first is found.
	 */
	CWnd* pChild = GetActiveFrame();

	while( pChild != NULL) {
		CWnd *pPrev = pChild->GetNextWindow( GW_HWNDPREV);

		if( pPrev == NULL) {
			break;
		}

		pChild = pPrev;
	}

	while( pChild != NULL) {
		if( pChild->IsKindOf(RUNTIME_CLASS(CFrameWnd))) {
			CView *pView = ((CFrameWnd *)pChild)->GetActiveView();

			if( pView->IsKindOf(RUNTIME_CLASS(CWklView))) {

				((CWklView *)pView)->OnFontChanged( nSize);

			} else if( pView->IsKindOf(RUNTIME_CLASS( CSysLogView))) {
				((CSysLogView *)pView)->OnFontChanged( nSize);
			} else if( pView->IsKindOf(RUNTIME_CLASS( CReportGridBase))) {
				((CReportGridBase *)pView)->OnFontChanged( nSize);
			}
		}


		// Get next sibling
		pChild = pChild->GetNextWindow();
	}
}

/*
 * Use htmlhelp as it works in Vista.
 */
void CMainFrame::OnHelp()
{
	CString strHelpPath = ((CLuceptApp *)AfxGetApp())->GetLuceptProfileString( _T("Paths"), _T("Help"), _T("c:\\Advance\\Programs\\")) + Specification::g_oSpecification.GetHelpFileName();

	(void)::HtmlHelp( GetSafeHwnd(), strHelpPath, HH_DISPLAY_TOPIC, NULL);	
}

// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	m_nLook = id;

	switch (m_nLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (m_nLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	((CWinAppEx *)AfxGetApp())->WriteInt(_T("ApplicationLook"), m_nLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_nLook == pCmdUI->m_nID);
}

void CMainFrame::OnRun() 
{
	Run();
}

/**
 * Create a new workload and run it.
 */
void CMainFrame::Run()
{
	// If reached here then create a new workload rather than start some random workload.
	// This involves some fiddling about to create the correct type of document, what with
	// Doc/View being narrow minded in design.
	CWklDoc *pDoc = MakeWorkload();

	if( pDoc == NULL) {
		return;
	}

	POSITION pos = pDoc->GetFirstViewPosition();

	if( pos != NULL) {
		CWklView *pView = (CWklView *)pDoc->GetNextView( pos);

		pView->SetRunAfterScan();  // set run when scan finished

		pView->ScanChain();			// run scan

	}
}

void CMainFrame::OnScanChain() 
{
	CWklDoc *pDoc = MakeWorkload();

	if( pDoc == NULL) {
		return;
	}

	POSITION pos = pDoc->GetFirstViewPosition();

	if( pos != NULL) {
		CWklView *pView = (CWklView *)pDoc->GetNextView( pos);

		pView->ScanChain();
	}
}

/**
 * Create a new workload and return the workload document.
 * @returns pointer to new Workload document.
 */
CWklDoc *CMainFrame::MakeWorkload()
{
	POSITION pos=AfxGetApp()->GetFirstDocTemplatePosition();

	while( pos != NULL) {
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);

		CString strDocName;

		if( pTemplate->GetDocString( strDocName, CDocTemplate::docName) == TRUE) {
			if( strDocName.CompareNoCase( _T( "wkload")) == 0) {
				/*
				 * This looks like a workload so create one.
				 * No need to keep the workload, the framework manages that.
				 */
			    CWklDoc *oDoc = (CWklDoc *)pTemplate->OpenDocumentFile(NULL); 

				return oDoc;
			}
		}
	}

	return NULL;
}

/**
 * Enable button if the LCC is able to service it.
 */
void CMainFrame::OnUpdateIfLccAvailable(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( IsLccAvailable());
}

void CMainFrame::OnUnlock() 
{
	// Lock the door
	AfxGetApp()->DoWaitCursor(1);

	CLumActor actor;
	long rv = actor.Init();
	if( rv == KO_OK) {
		double nResult = 0.0;
		static DWORD nLastDoneTime = 0;
		static bool bOnlyJustOpened;

		/*
		 * If the door button is pressed rapidly there are times when it does not appear to do anything.
		 * This seems to be cause by a delay in the lcc updating the state of the door, i.e. close the door,
		 * and the lcc will only report that the door is closed after a second or so. If during that time you
		 * ask the state of the door, the lcc will say it's open so pressing the button causes it to close it again.
		 * Hence the addition of this little cheat: if the user is banging the door button and the lcc has not had time
		 * to update it's status then use a quick toggle boolean to decide whether the door is open.
		 */
		if( ::GetTickCount() - nLastDoneTime > 2500) {

			// Ask lcc if door is open or closed...
			if( actor.GetLccMgr()->Inquire( LCC_INQ_DOORLOCK, &nResult) == KO_OK) {
				// ..and close or open it, as necessary
				(void)actor.Action(LCC_ACT_DOOR, nResult != 0 ? LCC_DOOR_OPENED : LCC_DOOR_CLOSED);
				bOnlyJustOpened = nResult != 0;
			}
		} else {
			(void)actor.Action(LCC_ACT_DOOR, bOnlyJustOpened ? LCC_DOOR_CLOSED : LCC_DOOR_OPENED);
			bOnlyJustOpened = !bOnlyJustOpened;
		}

		nLastDoneTime = ::GetTickCount();

		actor.Release();
	}

	AfxGetApp()->DoWaitCursor(-1);
}

void CMainFrame::OnNewWorkload()
{
	NewDoc(((CLuceptApp *)AfxGetApp())->m_pWklViewTemplate);
}

void CMainFrame::OnNewWorkloadTemplate()
{
	NewDoc(((CLuceptApp *)AfxGetApp())->m_pWklViewWktTemplate);
}

void CMainFrame::OnNewReport()
{
	NewDoc(((CLuceptApp *)AfxGetApp())->m_pRptViewTemplate);
	ActivateCategory( _T("Report"));   // Auto-bounce to report category
}

/**
 * Given a template, create a new document and frame: very handy, maybe mfc has something to do this
 * hidden away somewhere.
 * @param pDocTemplate The document template to create all kins of stuff from.
 */
CDocument *CMainFrame::NewDoc( CDocTemplate *pDocTemplate)
{
	CDocument *pDoc = pDocTemplate->CreateNewDocument();
	pDocTemplate->SetDefaultTitle( pDoc);
	CFrameWnd *pFrame = pDocTemplate->CreateNewFrame( pDoc, NULL);
	if( !pDoc->OnNewDocument()) {   // Based on standard mfc behaviour
		pFrame->DestroyWindow();
		return NULL;
	}

	pFrame->InitialUpdateFrame( pDoc, TRUE);

	return pDoc;
}

 BOOL CMainFrame::OnEraseMDIClientBackground(CDC* pDC)
 {
	CRect clientRect;
	GetClientRect(clientRect);

	if( clientRect.Width() > 0) {
		CDC oMemDC;
		oMemDC.CreateCompatibleDC( pDC);
		oMemDC.SelectObject( m_bmpBackground);

		BITMAP bm;
		m_bmpBackground.GetObject( sizeof( BITMAP), &bm);

		int nImageWidth = bm.bmWidth;
		int nImageHeight = bm.bmHeight;

		float nScreenAspect = (float)clientRect.Height() / (float)clientRect.Width();
//		float nBitmapAspect = 439.0f/ 689.0f;
		float nBitmapAspect = (float)nImageHeight / nImageWidth;

		if( nScreenAspect > nBitmapAspect) {
			// The screen is taller/thinner than the image: the image should be trimmed along its right edge to fit.
			 int nSrcWidth = (int)(nImageWidth * nBitmapAspect / nScreenAspect);

			pDC->StretchBlt( 0,0, clientRect.Width(), clientRect.Height(), &oMemDC, (nImageWidth - nSrcWidth) / 2, 0, nSrcWidth, nImageHeight, SRCCOPY);
		} else {
			// The screen is shorter/fatter than the image: the image should be trimmed along its bottom edge to fit.
			int nSrcHeight = (int)(nImageHeight * nScreenAspect / nBitmapAspect);

			pDC->StretchBlt( 0,0, clientRect.Width(), clientRect.Height(), &oMemDC, 0, (nImageHeight - nSrcHeight) / 2, nImageWidth, nSrcHeight, SRCCOPY);
		}
	}

	return TRUE;
 }

void CMainFrame::OnLockApp()
{
	CMFCRibbonButton *pButton = (CMFCRibbonButton *)m_wndRibbonBar.FindByID( ID_LOCK_APP);
	bool bReadOnly = false;

	if( !gGateKeeper.IsAppLocked()) {
		gGateKeeper.LockApp();
		pButton->SetImageIndex( 17, TRUE);
		pButton->SetText( _T("Unlock"));
		bReadOnly = true;
	} else {
		gGateKeeper.UnlockApp();
		if( gGateKeeper.IsAppLocked()) {
			return;
		}

		pButton->SetImageIndex( 16, TRUE);
		pButton->SetText( _T("Lock"));
		bReadOnly = false;
	}

	POSITION pos=AfxGetApp()->GetFirstDocTemplatePosition();

	while( pos != NULL) {
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);

		POSITION oDocPos = pTemplate->GetFirstDocPosition();

		while( oDocPos != NULL) {
			CDocument* pDocument = pTemplate->GetNextDoc(oDocPos);

			if( !(pDocument->IsKindOf( RUNTIME_CLASS( CWklDoc)) || pDocument->IsKindOf( RUNTIME_CLASS( CRptDoc)))) {
				continue;
			}

			POSITION oViewPos = pDocument->GetFirstViewPosition();

			while( oViewPos != NULL) {
				CView* pView = pDocument->GetNextView(oViewPos);
				
				if( pView->IsKindOf( RUNTIME_CLASS( CWklView))) {
					((CWklView *)pView)->SetReadOnly( bReadOnly);
				} else if( pView->IsKindOf( RUNTIME_CLASS( CRptView))) { 
					((CRptView *)pView)->SetReadOnly( bReadOnly);
				}
			}
		}
	}
}

/**
 * Determine whether the lcc is available.
 * @returns true if it is.
 */
 BOOL CMainFrame::IsLccAvailable()
 {
	CLuceptApp* pApp = (CLuceptApp*)AfxGetApp();
	CLccMgr *pMgr = pApp->GetLccMgr();

	if( pMgr != NULL){ 
		return pMgr->IsLccAvailable();
	} else {
		return FALSE;
	}
}

/**
 * Display a description of the instrument in the status bar.
 * @param A desctiption of the instrument.
 */
void CMainFrame::DisplayInstrumentDescription( CString strDescription)
{
		// Display instrument description in status bar
	m_wndStatusBar.SetPaneText( m_wndStatusBar.CommandToIndex( ID_INDICATOR_INSTRUMENT), strDescription);
}

/**
 * Post a message such that the system log will be requeried and the views refreshed.
 */
 void CMainFrame::PostSysLogUpdate()
 {
	 if( GetSafeHwnd() != NULL) {
 		PostMessage( WM_PUPDATE_LOG);
	 }
 }

LRESULT CMainFrame::OnUpdateLog( WPARAM wParam, LPARAM lParam)
{
	/*
	 * Update all views.
	 */
	POSITION oDocPos = ((CLuceptApp *)AfxGetApp())->m_pSysLogPrintTemplate->GetFirstDocPosition();

	while( oDocPos != NULL) {
		CDocument* pDocument = ((CLuceptApp *)AfxGetApp())->m_pSysLogPrintTemplate->GetNextDoc(oDocPos);

		if( !(pDocument->IsKindOf( RUNTIME_CLASS( CSystemLogDoc)))) {
			continue;
		}

		((CSystemLogDoc *)pDocument)->Refresh();
	}

	return 0;
}

/**
 * Restart the lcc
 */
void CMainFrame::OnKickLcc()
{
	CLuceptApp *pApp = (CLuceptApp *)AfxGetApp();

	if (pApp->GetLccMgr() != NULL) {
		if( AfxMessageBox( _T( "Restarting the LCC is a dangerous operation: are you sure"), MB_YESNO) == IDYES) {
			pApp->GetLccMgr()->DisconnectLcc();
			pApp->GetLccMgr()->ConnectLcc();
		}
	}
}

/**
 * Given the name of a category in the ribbon, return the index of the category.
 * @param strName the name of the category
 * @return the index of the category or -1 if not found.
 */
int CMainFrame::GetCategoryIndexByName( LPCTSTR strName)
{
	for( int i=0; i < m_wndRibbonBar.GetCategoryCount(); i++) {
		// Unfortunately categorys only have language dependent strings to identify them
		CString strCaption = m_wndRibbonBar.GetCategory( i)->GetName();

		if( strCaption.CompareNoCase( strName) == 0) {
			return i;
		}
	}

	return -1;
}

/**
 * Show of hide a category in the ribbon.
 * @param strCategoryName The name of the category to show or hide.
 * @param bShow true to show the category.
 */
void CMainFrame::ShowCategory( LPCTSTR strCategoryName, bool bShow)
{
	int nIndex = GetCategoryIndexByName( strCategoryName);

	ASSERT( nIndex >= 0);

	m_wndRibbonBar.ShowCategory( nIndex, bShow);
	m_wndRibbonBar.ForceRecalcLayout();
}


/**
 * Activate a category in the ribbon.
 * @param strCategoryName The name of the category to activate.
 */
void CMainFrame::ActivateCategory( LPCTSTR strCategoryName)
{
	int nIndex = GetCategoryIndexByName( strCategoryName);

	ASSERT( nIndex >= 0);

	m_wndRibbonBar.SetActiveCategory( m_wndRibbonBar.GetCategory( nIndex));
	m_wndRibbonBar.ForceRecalcLayout();
}
