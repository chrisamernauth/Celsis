# Microsoft Developer Studio Generated NMAKE File, Based on Lucept.dsp
!IF "$(CFG)" == ""
CFG=LUCEPT - WIN32 DEBUG
!MESSAGE No configuration specified. Defaulting to LUCEPT - WIN32 DEBUG.
!ENDIF 

!IF "$(CFG)" != "Lucept - Win32 Release" && "$(CFG)" != "Lucept - Win32 Debug" && "$(CFG)" != "Lucept - Win32 MacroDebug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Lucept.mak" CFG="LUCEPT - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Lucept - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Lucept - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Lucept - Win32 MacroDebug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Lucept - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\.\Release\Advance.hlp" "$(OUTDIR)\Advance.exe" "$(OUTDIR)\EventSnk.tlb" "$(OUTDIR)\Lucept.bsc"


CLEAN :
	-@erase "$(INTDIR)\ACCMgr.obj"
	-@erase "$(INTDIR)\ACCMgr.sbr"
	-@erase "$(INTDIR)\ACCtnr.obj"
	-@erase "$(INTDIR)\ACCtnr.sbr"
	-@erase "$(INTDIR)\ACCtrl.obj"
	-@erase "$(INTDIR)\ACCtrl.sbr"
	-@erase "$(INTDIR)\acdisp.obj"
	-@erase "$(INTDIR)\acdisp.sbr"
	-@erase "$(INTDIR)\AddCSamp.obj"
	-@erase "$(INTDIR)\AddCSamp.sbr"
	-@erase "$(INTDIR)\Addreag.obj"
	-@erase "$(INTDIR)\Addreag.sbr"
	-@erase "$(INTDIR)\ADefDlg.obj"
	-@erase "$(INTDIR)\ADefDlg.sbr"
	-@erase "$(INTDIR)\ADPgACal.obj"
	-@erase "$(INTDIR)\ADPgACal.sbr"
	-@erase "$(INTDIR)\ADPgCal.obj"
	-@erase "$(INTDIR)\ADPgCal.sbr"
	-@erase "$(INTDIR)\ADPgFlag.obj"
	-@erase "$(INTDIR)\ADPgFlag.sbr"
	-@erase "$(INTDIR)\ADPgFld.obj"
	-@erase "$(INTDIR)\ADPgFld.sbr"
	-@erase "$(INTDIR)\ADPgMain.obj"
	-@erase "$(INTDIR)\ADPgMain.sbr"
	-@erase "$(INTDIR)\ADPgName.obj"
	-@erase "$(INTDIR)\ADPgName.sbr"
	-@erase "$(INTDIR)\ADPrpSh.obj"
	-@erase "$(INTDIR)\ADPrpSh.sbr"
	-@erase "$(INTDIR)\AssayNew.obj"
	-@erase "$(INTDIR)\AssayNew.sbr"
	-@erase "$(INTDIR)\Assays.obj"
	-@erase "$(INTDIR)\Assays.sbr"
	-@erase "$(INTDIR)\AssaySel.obj"
	-@erase "$(INTDIR)\AssaySel.sbr"
	-@erase "$(INTDIR)\AssyDef.obj"
	-@erase "$(INTDIR)\AssyDef.sbr"
	-@erase "$(INTDIR)\AssyPrDc.obj"
	-@erase "$(INTDIR)\AssyPrDc.sbr"
	-@erase "$(INTDIR)\AssyPrVw.obj"
	-@erase "$(INTDIR)\AssyPrVw.sbr"
	-@erase "$(INTDIR)\BatCalc.obj"
	-@erase "$(INTDIR)\BatCalc.sbr"
	-@erase "$(INTDIR)\Batch.obj"
	-@erase "$(INTDIR)\Batch.sbr"
	-@erase "$(INTDIR)\BatchPg.obj"
	-@erase "$(INTDIR)\BatchPg.sbr"
	-@erase "$(INTDIR)\BatRef.obj"
	-@erase "$(INTDIR)\BatRef.sbr"
	-@erase "$(INTDIR)\BCFrame.obj"
	-@erase "$(INTDIR)\BCFrame.sbr"
	-@erase "$(INTDIR)\BGridPg.obj"
	-@erase "$(INTDIR)\BGridPg.sbr"
	-@erase "$(INTDIR)\BPickDlg.obj"
	-@erase "$(INTDIR)\BPickDlg.sbr"
	-@erase "$(INTDIR)\BTabCtrl.obj"
	-@erase "$(INTDIR)\BTabCtrl.sbr"
	-@erase "$(INTDIR)\CalTypeD.obj"
	-@erase "$(INTDIR)\CalTypeD.sbr"
	-@erase "$(INTDIR)\CondFrm.obj"
	-@erase "$(INTDIR)\CondFrm.sbr"
	-@erase "$(INTDIR)\Conduct.obj"
	-@erase "$(INTDIR)\Conduct.sbr"
	-@erase "$(INTDIR)\CugRpt.obj"
	-@erase "$(INTDIR)\CugRpt.sbr"
	-@erase "$(INTDIR)\CugWkl.obj"
	-@erase "$(INTDIR)\CugWkl.sbr"
	-@erase "$(INTDIR)\CustPal.obj"
	-@erase "$(INTDIR)\CustPal.sbr"
	-@erase "$(INTDIR)\DaoUtils.obj"
	-@erase "$(INTDIR)\DaoUtils.sbr"
	-@erase "$(INTDIR)\dibapi.obj"
	-@erase "$(INTDIR)\dibapi.sbr"
	-@erase "$(INTDIR)\DIBUtil.obj"
	-@erase "$(INTDIR)\DIBUtil.sbr"
	-@erase "$(INTDIR)\Eventsnk.obj"
	-@erase "$(INTDIR)\Eventsnk.sbr"
	-@erase "$(INTDIR)\EventSnk.tlb"
	-@erase "$(INTDIR)\FldProp.obj"
	-@erase "$(INTDIR)\FldProp.sbr"
	-@erase "$(INTDIR)\GenBox.obj"
	-@erase "$(INTDIR)\GenBox.sbr"
	-@erase "$(INTDIR)\GenDef.obj"
	-@erase "$(INTDIR)\GenDef.sbr"
	-@erase "$(INTDIR)\Generics.obj"
	-@erase "$(INTDIR)\Generics.sbr"
	-@erase "$(INTDIR)\GnDfPrDc.obj"
	-@erase "$(INTDIR)\GnDfPrDc.sbr"
	-@erase "$(INTDIR)\GnDfPrVw.obj"
	-@erase "$(INTDIR)\GnDfPrVw.sbr"
	-@erase "$(INTDIR)\HandCtrl.obj"
	-@erase "$(INTDIR)\HandCtrl.sbr"
	-@erase "$(INTDIR)\hidedlg.obj"
	-@erase "$(INTDIR)\hidedlg.sbr"
	-@erase "$(INTDIR)\IniFiles.obj"
	-@erase "$(INTDIR)\IniFiles.sbr"
	-@erase "$(INTDIR)\InjSeq.obj"
	-@erase "$(INTDIR)\InjSeq.sbr"
	-@erase "$(INTDIR)\LccInq.obj"
	-@erase "$(INTDIR)\LccInq.sbr"
	-@erase "$(INTDIR)\LccMgr.obj"
	-@erase "$(INTDIR)\LccMgr.sbr"
	-@erase "$(INTDIR)\LCCProxy.obj"
	-@erase "$(INTDIR)\LCCProxy.sbr"
	-@erase "$(INTDIR)\LCCUser.obj"
	-@erase "$(INTDIR)\LCCUser.sbr"
	-@erase "$(INTDIR)\LccVDoc.obj"
	-@erase "$(INTDIR)\LccVDoc.sbr"
	-@erase "$(INTDIR)\LccView.obj"
	-@erase "$(INTDIR)\LccView.sbr"
	-@erase "$(INTDIR)\LCDWnd.obj"
	-@erase "$(INTDIR)\LCDWnd.sbr"
	-@erase "$(INTDIR)\LEDDisp.obj"
	-@erase "$(INTDIR)\LEDDisp.sbr"
	-@erase "$(INTDIR)\Level.obj"
	-@erase "$(INTDIR)\Level.sbr"
	-@erase "$(INTDIR)\LiftWnd.obj"
	-@erase "$(INTDIR)\LiftWnd.sbr"
	-@erase "$(INTDIR)\LogonDlg.obj"
	-@erase "$(INTDIR)\LogonDlg.sbr"
	-@erase "$(INTDIR)\Lucept.obj"
	-@erase "$(INTDIR)\Lucept.pch"
	-@erase "$(INTDIR)\Lucept.res"
	-@erase "$(INTDIR)\Lucept.sbr"
	-@erase "$(INTDIR)\LucSamp.obj"
	-@erase "$(INTDIR)\LucSamp.sbr"
	-@erase "$(INTDIR)\LucyBar.obj"
	-@erase "$(INTDIR)\LucyBar.sbr"
	-@erase "$(INTDIR)\LucyDoc.obj"
	-@erase "$(INTDIR)\LucyDoc.sbr"
	-@erase "$(INTDIR)\LucyFrm.obj"
	-@erase "$(INTDIR)\LucyFrm.sbr"
	-@erase "$(INTDIR)\LucyView.obj"
	-@erase "$(INTDIR)\LucyView.sbr"
	-@erase "$(INTDIR)\LumActor.obj"
	-@erase "$(INTDIR)\LumActor.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MDefDlg.obj"
	-@erase "$(INTDIR)\MDefDlg.sbr"
	-@erase "$(INTDIR)\MeasDef.obj"
	-@erase "$(INTDIR)\MeasDef.sbr"
	-@erase "$(INTDIR)\MeasNew.obj"
	-@erase "$(INTDIR)\MeasNew.sbr"
	-@erase "$(INTDIR)\MeasPrDc.obj"
	-@erase "$(INTDIR)\MeasPrDc.sbr"
	-@erase "$(INTDIR)\MeasPrVw.obj"
	-@erase "$(INTDIR)\MeasPrVw.sbr"
	-@erase "$(INTDIR)\MeasSel.obj"
	-@erase "$(INTDIR)\MeasSel.sbr"
	-@erase "$(INTDIR)\Measures.obj"
	-@erase "$(INTDIR)\Measures.sbr"
	-@erase "$(INTDIR)\ModulVer.obj"
	-@erase "$(INTDIR)\ModulVer.sbr"
	-@erase "$(INTDIR)\MProcDlg.obj"
	-@erase "$(INTDIR)\MProcDlg.sbr"
	-@erase "$(INTDIR)\PPUAdd.obj"
	-@erase "$(INTDIR)\PPUAdd.sbr"
	-@erase "$(INTDIR)\PPUDel.obj"
	-@erase "$(INTDIR)\PPUDel.sbr"
	-@erase "$(INTDIR)\PPUEdit.obj"
	-@erase "$(INTDIR)\PPUEdit.sbr"
	-@erase "$(INTDIR)\RefList.obj"
	-@erase "$(INTDIR)\RefList.sbr"
	-@erase "$(INTDIR)\RGridFrm.obj"
	-@erase "$(INTDIR)\RGridFrm.sbr"
	-@erase "$(INTDIR)\RptDoc.obj"
	-@erase "$(INTDIR)\RptDoc.sbr"
	-@erase "$(INTDIR)\RptFInfo.obj"
	-@erase "$(INTDIR)\RptFInfo.sbr"
	-@erase "$(INTDIR)\RptFrame.obj"
	-@erase "$(INTDIR)\RptFrame.sbr"
	-@erase "$(INTDIR)\RptPgCol.obj"
	-@erase "$(INTDIR)\RptPgCol.sbr"
	-@erase "$(INTDIR)\RptPgFmt.obj"
	-@erase "$(INTDIR)\RptPgFmt.sbr"
	-@erase "$(INTDIR)\RptPgFrm.obj"
	-@erase "$(INTDIR)\RptPgFrm.sbr"
	-@erase "$(INTDIR)\RptPgGph.obj"
	-@erase "$(INTDIR)\RptPgGph.sbr"
	-@erase "$(INTDIR)\RptPgQry.obj"
	-@erase "$(INTDIR)\RptPgQry.sbr"
	-@erase "$(INTDIR)\RptPgSrt.obj"
	-@erase "$(INTDIR)\RptPgSrt.sbr"
	-@erase "$(INTDIR)\RptPgWkl.obj"
	-@erase "$(INTDIR)\RptPgWkl.sbr"
	-@erase "$(INTDIR)\RptUgDao.obj"
	-@erase "$(INTDIR)\RptUgDao.sbr"
	-@erase "$(INTDIR)\RptView.obj"
	-@erase "$(INTDIR)\RptView.sbr"
	-@erase "$(INTDIR)\SampFrm.obj"
	-@erase "$(INTDIR)\SampFrm.sbr"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\Sample.sbr"
	-@erase "$(INTDIR)\SampRef.obj"
	-@erase "$(INTDIR)\SampRef.sbr"
	-@erase "$(INTDIR)\SampView.obj"
	-@erase "$(INTDIR)\SampView.sbr"
	-@erase "$(INTDIR)\ScanPage.obj"
	-@erase "$(INTDIR)\ScanPage.sbr"
	-@erase "$(INTDIR)\ScanSht.obj"
	-@erase "$(INTDIR)\ScanSht.sbr"
	-@erase "$(INTDIR)\ScnChain.obj"
	-@erase "$(INTDIR)\ScnChain.sbr"
	-@erase "$(INTDIR)\ScnSumPg.obj"
	-@erase "$(INTDIR)\ScnSumPg.sbr"
	-@erase "$(INTDIR)\SetupDlg.obj"
	-@erase "$(INTDIR)\SetupDlg.sbr"
	-@erase "$(INTDIR)\SPickDlg.obj"
	-@erase "$(INTDIR)\SPickDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TempDisp.obj"
	-@erase "$(INTDIR)\TempDisp.sbr"
	-@erase "$(INTDIR)\TimeDisp.obj"
	-@erase "$(INTDIR)\TimeDisp.sbr"
	-@erase "$(INTDIR)\Tube.obj"
	-@erase "$(INTDIR)\Tube.sbr"
	-@erase "$(INTDIR)\Ugcbtype.obj"
	-@erase "$(INTDIR)\Ugcbtype.sbr"
	-@erase "$(INTDIR)\Ugcell.obj"
	-@erase "$(INTDIR)\Ugcell.sbr"
	-@erase "$(INTDIR)\Ugceltyp.obj"
	-@erase "$(INTDIR)\Ugceltyp.sbr"
	-@erase "$(INTDIR)\Ugcnrbtn.obj"
	-@erase "$(INTDIR)\Ugcnrbtn.sbr"
	-@erase "$(INTDIR)\Ugctafnt.obj"
	-@erase "$(INTDIR)\Ugctafnt.sbr"
	-@erase "$(INTDIR)\Ugctarrw.obj"
	-@erase "$(INTDIR)\Ugctarrw.sbr"
	-@erase "$(INTDIR)\Ugctbutn.obj"
	-@erase "$(INTDIR)\Ugctbutn.sbr"
	-@erase "$(INTDIR)\Ugctelps.obj"
	-@erase "$(INTDIR)\Ugctelps.sbr"
	-@erase "$(INTDIR)\Ugctpie.obj"
	-@erase "$(INTDIR)\Ugctpie.sbr"
	-@erase "$(INTDIR)\Ugctpro1.obj"
	-@erase "$(INTDIR)\Ugctpro1.sbr"
	-@erase "$(INTDIR)\Ugctprog.obj"
	-@erase "$(INTDIR)\Ugctprog.sbr"
	-@erase "$(INTDIR)\Ugctrado.obj"
	-@erase "$(INTDIR)\Ugctrado.sbr"
	-@erase "$(INTDIR)\Ugctrl.obj"
	-@erase "$(INTDIR)\Ugctrl.sbr"
	-@erase "$(INTDIR)\Ugctsarw.obj"
	-@erase "$(INTDIR)\Ugctsarw.sbr"
	-@erase "$(INTDIR)\Ugctspin.obj"
	-@erase "$(INTDIR)\Ugctspin.sbr"
	-@erase "$(INTDIR)\Ugdao.obj"
	-@erase "$(INTDIR)\Ugdao.sbr"
	-@erase "$(INTDIR)\Ugdltype.obj"
	-@erase "$(INTDIR)\Ugdltype.sbr"
	-@erase "$(INTDIR)\Ugdrgdrp.obj"
	-@erase "$(INTDIR)\Ugdrgdrp.sbr"
	-@erase "$(INTDIR)\Ugdrwhnt.obj"
	-@erase "$(INTDIR)\Ugdrwhnt.sbr"
	-@erase "$(INTDIR)\Ugdtasrc.obj"
	-@erase "$(INTDIR)\Ugdtasrc.sbr"
	-@erase "$(INTDIR)\Ugedit.obj"
	-@erase "$(INTDIR)\Ugedit.sbr"
	-@erase "$(INTDIR)\Uggrid.obj"
	-@erase "$(INTDIR)\Uggrid.sbr"
	-@erase "$(INTDIR)\Ughint.obj"
	-@erase "$(INTDIR)\Ughint.sbr"
	-@erase "$(INTDIR)\Ughscrol.obj"
	-@erase "$(INTDIR)\Ughscrol.sbr"
	-@erase "$(INTDIR)\Uglstbox.obj"
	-@erase "$(INTDIR)\Uglstbox.sbr"
	-@erase "$(INTDIR)\Ugmemman.obj"
	-@erase "$(INTDIR)\Ugmemman.sbr"
	-@erase "$(INTDIR)\Ugmultis.obj"
	-@erase "$(INTDIR)\Ugmultis.sbr"
	-@erase "$(INTDIR)\Ugprint.obj"
	-@erase "$(INTDIR)\Ugprint.sbr"
	-@erase "$(INTDIR)\Ugroutin.obj"
	-@erase "$(INTDIR)\Ugroutin.sbr"
	-@erase "$(INTDIR)\Ugsidehd.obj"
	-@erase "$(INTDIR)\Ugsidehd.sbr"
	-@erase "$(INTDIR)\UGTab.obj"
	-@erase "$(INTDIR)\UGTab.sbr"
	-@erase "$(INTDIR)\Ugtophdg.obj"
	-@erase "$(INTDIR)\Ugtophdg.sbr"
	-@erase "$(INTDIR)\Ugvscrol.obj"
	-@erase "$(INTDIR)\Ugvscrol.sbr"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\User.sbr"
	-@erase "$(INTDIR)\UserDB.obj"
	-@erase "$(INTDIR)\UserDB.sbr"
	-@erase "$(INTDIR)\UserFlag.obj"
	-@erase "$(INTDIR)\UserFlag.sbr"
	-@erase "$(INTDIR)\UsrAdmin.obj"
	-@erase "$(INTDIR)\UsrAdmin.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WklDoc.obj"
	-@erase "$(INTDIR)\WklDoc.sbr"
	-@erase "$(INTDIR)\WklFrame.obj"
	-@erase "$(INTDIR)\WklFrame.sbr"
	-@erase "$(INTDIR)\WklMods.obj"
	-@erase "$(INTDIR)\WklMods.sbr"
	-@erase "$(INTDIR)\WklRead.obj"
	-@erase "$(INTDIR)\WklRead.sbr"
	-@erase "$(INTDIR)\WklRef.obj"
	-@erase "$(INTDIR)\WklRef.sbr"
	-@erase "$(INTDIR)\WklView.obj"
	-@erase "$(INTDIR)\WklView.sbr"
	-@erase "$(OUTDIR)\Advance.exe"
	-@erase "$(OUTDIR)\Advance.map"
	-@erase "$(OUTDIR)\Advance.pdb"
	-@erase "$(OUTDIR)\Lucept.bsc"
	-@erase ".\.\Release\Advance.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Lucept.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Lucept.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Lucept.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ACCMgr.sbr" \
	"$(INTDIR)\ACCtnr.sbr" \
	"$(INTDIR)\ACCtrl.sbr" \
	"$(INTDIR)\acdisp.sbr" \
	"$(INTDIR)\AddCSamp.sbr" \
	"$(INTDIR)\Addreag.sbr" \
	"$(INTDIR)\ADefDlg.sbr" \
	"$(INTDIR)\ADPgACal.sbr" \
	"$(INTDIR)\ADPgCal.sbr" \
	"$(INTDIR)\ADPgFlag.sbr" \
	"$(INTDIR)\ADPgFld.sbr" \
	"$(INTDIR)\ADPgMain.sbr" \
	"$(INTDIR)\ADPgName.sbr" \
	"$(INTDIR)\ADPrpSh.sbr" \
	"$(INTDIR)\AssayNew.sbr" \
	"$(INTDIR)\Assays.sbr" \
	"$(INTDIR)\AssaySel.sbr" \
	"$(INTDIR)\AssyDef.sbr" \
	"$(INTDIR)\AssyPrDc.sbr" \
	"$(INTDIR)\AssyPrVw.sbr" \
	"$(INTDIR)\BatCalc.sbr" \
	"$(INTDIR)\Batch.sbr" \
	"$(INTDIR)\BatchPg.sbr" \
	"$(INTDIR)\BatRef.sbr" \
	"$(INTDIR)\BCFrame.sbr" \
	"$(INTDIR)\BGridPg.sbr" \
	"$(INTDIR)\BPickDlg.sbr" \
	"$(INTDIR)\BTabCtrl.sbr" \
	"$(INTDIR)\CalTypeD.sbr" \
	"$(INTDIR)\CondFrm.sbr" \
	"$(INTDIR)\Conduct.sbr" \
	"$(INTDIR)\CugRpt.sbr" \
	"$(INTDIR)\CugWkl.sbr" \
	"$(INTDIR)\CustPal.sbr" \
	"$(INTDIR)\DaoUtils.sbr" \
	"$(INTDIR)\dibapi.sbr" \
	"$(INTDIR)\DIBUtil.sbr" \
	"$(INTDIR)\Eventsnk.sbr" \
	"$(INTDIR)\FldProp.sbr" \
	"$(INTDIR)\GenBox.sbr" \
	"$(INTDIR)\GenDef.sbr" \
	"$(INTDIR)\Generics.sbr" \
	"$(INTDIR)\GnDfPrDc.sbr" \
	"$(INTDIR)\GnDfPrVw.sbr" \
	"$(INTDIR)\HandCtrl.sbr" \
	"$(INTDIR)\hidedlg.sbr" \
	"$(INTDIR)\IniFiles.sbr" \
	"$(INTDIR)\InjSeq.sbr" \
	"$(INTDIR)\LccInq.sbr" \
	"$(INTDIR)\LccMgr.sbr" \
	"$(INTDIR)\LCCProxy.sbr" \
	"$(INTDIR)\LCCUser.sbr" \
	"$(INTDIR)\LccVDoc.sbr" \
	"$(INTDIR)\LccView.sbr" \
	"$(INTDIR)\LCDWnd.sbr" \
	"$(INTDIR)\LEDDisp.sbr" \
	"$(INTDIR)\Level.sbr" \
	"$(INTDIR)\LiftWnd.sbr" \
	"$(INTDIR)\LogonDlg.sbr" \
	"$(INTDIR)\Lucept.sbr" \
	"$(INTDIR)\LucSamp.sbr" \
	"$(INTDIR)\LucyBar.sbr" \
	"$(INTDIR)\LucyDoc.sbr" \
	"$(INTDIR)\LucyFrm.sbr" \
	"$(INTDIR)\LucyView.sbr" \
	"$(INTDIR)\LumActor.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MDefDlg.sbr" \
	"$(INTDIR)\MeasDef.sbr" \
	"$(INTDIR)\MeasNew.sbr" \
	"$(INTDIR)\MeasPrDc.sbr" \
	"$(INTDIR)\MeasPrVw.sbr" \
	"$(INTDIR)\MeasSel.sbr" \
	"$(INTDIR)\Measures.sbr" \
	"$(INTDIR)\ModulVer.sbr" \
	"$(INTDIR)\MProcDlg.sbr" \
	"$(INTDIR)\PPUAdd.sbr" \
	"$(INTDIR)\PPUDel.sbr" \
	"$(INTDIR)\PPUEdit.sbr" \
	"$(INTDIR)\RefList.sbr" \
	"$(INTDIR)\RGridFrm.sbr" \
	"$(INTDIR)\RptDoc.sbr" \
	"$(INTDIR)\RptFInfo.sbr" \
	"$(INTDIR)\RptFrame.sbr" \
	"$(INTDIR)\RptPgCol.sbr" \
	"$(INTDIR)\RptPgFmt.sbr" \
	"$(INTDIR)\RptPgFrm.sbr" \
	"$(INTDIR)\RptPgGph.sbr" \
	"$(INTDIR)\RptPgQry.sbr" \
	"$(INTDIR)\RptPgSrt.sbr" \
	"$(INTDIR)\RptPgWkl.sbr" \
	"$(INTDIR)\RptUgDao.sbr" \
	"$(INTDIR)\RptView.sbr" \
	"$(INTDIR)\SampFrm.sbr" \
	"$(INTDIR)\Sample.sbr" \
	"$(INTDIR)\SampRef.sbr" \
	"$(INTDIR)\SampView.sbr" \
	"$(INTDIR)\ScanPage.sbr" \
	"$(INTDIR)\ScanSht.sbr" \
	"$(INTDIR)\ScnChain.sbr" \
	"$(INTDIR)\ScnSumPg.sbr" \
	"$(INTDIR)\SetupDlg.sbr" \
	"$(INTDIR)\SPickDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TempDisp.sbr" \
	"$(INTDIR)\TimeDisp.sbr" \
	"$(INTDIR)\Tube.sbr" \
	"$(INTDIR)\Ugcbtype.sbr" \
	"$(INTDIR)\Ugcell.sbr" \
	"$(INTDIR)\Ugceltyp.sbr" \
	"$(INTDIR)\Ugcnrbtn.sbr" \
	"$(INTDIR)\Ugctafnt.sbr" \
	"$(INTDIR)\Ugctarrw.sbr" \
	"$(INTDIR)\Ugctbutn.sbr" \
	"$(INTDIR)\Ugctelps.sbr" \
	"$(INTDIR)\Ugctpie.sbr" \
	"$(INTDIR)\Ugctpro1.sbr" \
	"$(INTDIR)\Ugctprog.sbr" \
	"$(INTDIR)\Ugctrado.sbr" \
	"$(INTDIR)\Ugctrl.sbr" \
	"$(INTDIR)\Ugctsarw.sbr" \
	"$(INTDIR)\Ugctspin.sbr" \
	"$(INTDIR)\Ugdao.sbr" \
	"$(INTDIR)\Ugdltype.sbr" \
	"$(INTDIR)\Ugdrgdrp.sbr" \
	"$(INTDIR)\Ugdrwhnt.sbr" \
	"$(INTDIR)\Ugdtasrc.sbr" \
	"$(INTDIR)\Ugedit.sbr" \
	"$(INTDIR)\Uggrid.sbr" \
	"$(INTDIR)\Ughint.sbr" \
	"$(INTDIR)\Ughscrol.sbr" \
	"$(INTDIR)\Uglstbox.sbr" \
	"$(INTDIR)\Ugmemman.sbr" \
	"$(INTDIR)\Ugmultis.sbr" \
	"$(INTDIR)\Ugprint.sbr" \
	"$(INTDIR)\Ugroutin.sbr" \
	"$(INTDIR)\Ugsidehd.sbr" \
	"$(INTDIR)\UGTab.sbr" \
	"$(INTDIR)\Ugtophdg.sbr" \
	"$(INTDIR)\Ugvscrol.sbr" \
	"$(INTDIR)\User.sbr" \
	"$(INTDIR)\UserDB.sbr" \
	"$(INTDIR)\UserFlag.sbr" \
	"$(INTDIR)\UsrAdmin.sbr" \
	"$(INTDIR)\WklDoc.sbr" \
	"$(INTDIR)\WklFrame.sbr" \
	"$(INTDIR)\WklMods.sbr" \
	"$(INTDIR)\WklRead.sbr" \
	"$(INTDIR)\WklRef.sbr" \
	"$(INTDIR)\WklView.sbr"

"$(OUTDIR)\Lucept.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\PhilUtil\Release\PhilUtil.lib ..\Olectra\Release\Olectra.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Advance.pdb" /map:"$(INTDIR)\Advance.map" /debug /machine:I386 /out:"$(OUTDIR)\Advance.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ACCMgr.obj" \
	"$(INTDIR)\ACCtnr.obj" \
	"$(INTDIR)\ACCtrl.obj" \
	"$(INTDIR)\acdisp.obj" \
	"$(INTDIR)\AddCSamp.obj" \
	"$(INTDIR)\Addreag.obj" \
	"$(INTDIR)\ADefDlg.obj" \
	"$(INTDIR)\ADPgACal.obj" \
	"$(INTDIR)\ADPgCal.obj" \
	"$(INTDIR)\ADPgFlag.obj" \
	"$(INTDIR)\ADPgFld.obj" \
	"$(INTDIR)\ADPgMain.obj" \
	"$(INTDIR)\ADPgName.obj" \
	"$(INTDIR)\ADPrpSh.obj" \
	"$(INTDIR)\AssayNew.obj" \
	"$(INTDIR)\Assays.obj" \
	"$(INTDIR)\AssaySel.obj" \
	"$(INTDIR)\AssyDef.obj" \
	"$(INTDIR)\AssyPrDc.obj" \
	"$(INTDIR)\AssyPrVw.obj" \
	"$(INTDIR)\BatCalc.obj" \
	"$(INTDIR)\Batch.obj" \
	"$(INTDIR)\BatchPg.obj" \
	"$(INTDIR)\BatRef.obj" \
	"$(INTDIR)\BCFrame.obj" \
	"$(INTDIR)\BGridPg.obj" \
	"$(INTDIR)\BPickDlg.obj" \
	"$(INTDIR)\BTabCtrl.obj" \
	"$(INTDIR)\CalTypeD.obj" \
	"$(INTDIR)\CondFrm.obj" \
	"$(INTDIR)\Conduct.obj" \
	"$(INTDIR)\CugRpt.obj" \
	"$(INTDIR)\CugWkl.obj" \
	"$(INTDIR)\CustPal.obj" \
	"$(INTDIR)\DaoUtils.obj" \
	"$(INTDIR)\dibapi.obj" \
	"$(INTDIR)\DIBUtil.obj" \
	"$(INTDIR)\Eventsnk.obj" \
	"$(INTDIR)\FldProp.obj" \
	"$(INTDIR)\GenBox.obj" \
	"$(INTDIR)\GenDef.obj" \
	"$(INTDIR)\Generics.obj" \
	"$(INTDIR)\GnDfPrDc.obj" \
	"$(INTDIR)\GnDfPrVw.obj" \
	"$(INTDIR)\HandCtrl.obj" \
	"$(INTDIR)\hidedlg.obj" \
	"$(INTDIR)\IniFiles.obj" \
	"$(INTDIR)\InjSeq.obj" \
	"$(INTDIR)\LccInq.obj" \
	"$(INTDIR)\LccMgr.obj" \
	"$(INTDIR)\LCCProxy.obj" \
	"$(INTDIR)\LCCUser.obj" \
	"$(INTDIR)\LccVDoc.obj" \
	"$(INTDIR)\LccView.obj" \
	"$(INTDIR)\LCDWnd.obj" \
	"$(INTDIR)\LEDDisp.obj" \
	"$(INTDIR)\Level.obj" \
	"$(INTDIR)\LiftWnd.obj" \
	"$(INTDIR)\LogonDlg.obj" \
	"$(INTDIR)\Lucept.obj" \
	"$(INTDIR)\LucSamp.obj" \
	"$(INTDIR)\LucyBar.obj" \
	"$(INTDIR)\LucyDoc.obj" \
	"$(INTDIR)\LucyFrm.obj" \
	"$(INTDIR)\LucyView.obj" \
	"$(INTDIR)\LumActor.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDefDlg.obj" \
	"$(INTDIR)\MeasDef.obj" \
	"$(INTDIR)\MeasNew.obj" \
	"$(INTDIR)\MeasPrDc.obj" \
	"$(INTDIR)\MeasPrVw.obj" \
	"$(INTDIR)\MeasSel.obj" \
	"$(INTDIR)\Measures.obj" \
	"$(INTDIR)\ModulVer.obj" \
	"$(INTDIR)\MProcDlg.obj" \
	"$(INTDIR)\PPUAdd.obj" \
	"$(INTDIR)\PPUDel.obj" \
	"$(INTDIR)\PPUEdit.obj" \
	"$(INTDIR)\RefList.obj" \
	"$(INTDIR)\RGridFrm.obj" \
	"$(INTDIR)\RptDoc.obj" \
	"$(INTDIR)\RptFInfo.obj" \
	"$(INTDIR)\RptFrame.obj" \
	"$(INTDIR)\RptPgCol.obj" \
	"$(INTDIR)\RptPgFmt.obj" \
	"$(INTDIR)\RptPgFrm.obj" \
	"$(INTDIR)\RptPgGph.obj" \
	"$(INTDIR)\RptPgQry.obj" \
	"$(INTDIR)\RptPgSrt.obj" \
	"$(INTDIR)\RptPgWkl.obj" \
	"$(INTDIR)\RptUgDao.obj" \
	"$(INTDIR)\RptView.obj" \
	"$(INTDIR)\SampFrm.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\SampRef.obj" \
	"$(INTDIR)\SampView.obj" \
	"$(INTDIR)\ScanPage.obj" \
	"$(INTDIR)\ScanSht.obj" \
	"$(INTDIR)\ScnChain.obj" \
	"$(INTDIR)\ScnSumPg.obj" \
	"$(INTDIR)\SetupDlg.obj" \
	"$(INTDIR)\SPickDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TempDisp.obj" \
	"$(INTDIR)\TimeDisp.obj" \
	"$(INTDIR)\Tube.obj" \
	"$(INTDIR)\Ugcbtype.obj" \
	"$(INTDIR)\Ugcell.obj" \
	"$(INTDIR)\Ugceltyp.obj" \
	"$(INTDIR)\Ugcnrbtn.obj" \
	"$(INTDIR)\Ugctafnt.obj" \
	"$(INTDIR)\Ugctarrw.obj" \
	"$(INTDIR)\Ugctbutn.obj" \
	"$(INTDIR)\Ugctelps.obj" \
	"$(INTDIR)\Ugctpie.obj" \
	"$(INTDIR)\Ugctpro1.obj" \
	"$(INTDIR)\Ugctprog.obj" \
	"$(INTDIR)\Ugctrado.obj" \
	"$(INTDIR)\Ugctrl.obj" \
	"$(INTDIR)\Ugctsarw.obj" \
	"$(INTDIR)\Ugctspin.obj" \
	"$(INTDIR)\Ugdao.obj" \
	"$(INTDIR)\Ugdltype.obj" \
	"$(INTDIR)\Ugdrgdrp.obj" \
	"$(INTDIR)\Ugdrwhnt.obj" \
	"$(INTDIR)\Ugdtasrc.obj" \
	"$(INTDIR)\Ugedit.obj" \
	"$(INTDIR)\Uggrid.obj" \
	"$(INTDIR)\Ughint.obj" \
	"$(INTDIR)\Ughscrol.obj" \
	"$(INTDIR)\Uglstbox.obj" \
	"$(INTDIR)\Ugmemman.obj" \
	"$(INTDIR)\Ugmultis.obj" \
	"$(INTDIR)\Ugprint.obj" \
	"$(INTDIR)\Ugroutin.obj" \
	"$(INTDIR)\Ugsidehd.obj" \
	"$(INTDIR)\UGTab.obj" \
	"$(INTDIR)\Ugtophdg.obj" \
	"$(INTDIR)\Ugvscrol.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\UserDB.obj" \
	"$(INTDIR)\UserFlag.obj" \
	"$(INTDIR)\UsrAdmin.obj" \
	"$(INTDIR)\WklDoc.obj" \
	"$(INTDIR)\WklFrame.obj" \
	"$(INTDIR)\WklMods.obj" \
	"$(INTDIR)\WklRead.obj" \
	"$(INTDIR)\WklRef.obj" \
	"$(INTDIR)\WklView.obj" \
	"$(INTDIR)\Lucept.res"

"$(OUTDIR)\Advance.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Lucept - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\.\Debug\Advance.hlp" "$(OUTDIR)\Advance.exe" "$(OUTDIR)\EventSnk.tlb" "$(OUTDIR)\Lucept.bsc"


CLEAN :
	-@erase "$(INTDIR)\ACCMgr.obj"
	-@erase "$(INTDIR)\ACCMgr.sbr"
	-@erase "$(INTDIR)\ACCtnr.obj"
	-@erase "$(INTDIR)\ACCtnr.sbr"
	-@erase "$(INTDIR)\ACCtrl.obj"
	-@erase "$(INTDIR)\ACCtrl.sbr"
	-@erase "$(INTDIR)\acdisp.obj"
	-@erase "$(INTDIR)\acdisp.sbr"
	-@erase "$(INTDIR)\AddCSamp.obj"
	-@erase "$(INTDIR)\AddCSamp.sbr"
	-@erase "$(INTDIR)\Addreag.obj"
	-@erase "$(INTDIR)\Addreag.sbr"
	-@erase "$(INTDIR)\ADefDlg.obj"
	-@erase "$(INTDIR)\ADefDlg.sbr"
	-@erase "$(INTDIR)\ADPgACal.obj"
	-@erase "$(INTDIR)\ADPgACal.sbr"
	-@erase "$(INTDIR)\ADPgCal.obj"
	-@erase "$(INTDIR)\ADPgCal.sbr"
	-@erase "$(INTDIR)\ADPgFlag.obj"
	-@erase "$(INTDIR)\ADPgFlag.sbr"
	-@erase "$(INTDIR)\ADPgFld.obj"
	-@erase "$(INTDIR)\ADPgFld.sbr"
	-@erase "$(INTDIR)\ADPgMain.obj"
	-@erase "$(INTDIR)\ADPgMain.sbr"
	-@erase "$(INTDIR)\ADPgName.obj"
	-@erase "$(INTDIR)\ADPgName.sbr"
	-@erase "$(INTDIR)\ADPrpSh.obj"
	-@erase "$(INTDIR)\ADPrpSh.sbr"
	-@erase "$(INTDIR)\AssayNew.obj"
	-@erase "$(INTDIR)\AssayNew.sbr"
	-@erase "$(INTDIR)\Assays.obj"
	-@erase "$(INTDIR)\Assays.sbr"
	-@erase "$(INTDIR)\AssaySel.obj"
	-@erase "$(INTDIR)\AssaySel.sbr"
	-@erase "$(INTDIR)\AssyDef.obj"
	-@erase "$(INTDIR)\AssyDef.sbr"
	-@erase "$(INTDIR)\AssyPrDc.obj"
	-@erase "$(INTDIR)\AssyPrDc.sbr"
	-@erase "$(INTDIR)\AssyPrVw.obj"
	-@erase "$(INTDIR)\AssyPrVw.sbr"
	-@erase "$(INTDIR)\BatCalc.obj"
	-@erase "$(INTDIR)\BatCalc.sbr"
	-@erase "$(INTDIR)\Batch.obj"
	-@erase "$(INTDIR)\Batch.sbr"
	-@erase "$(INTDIR)\BatchPg.obj"
	-@erase "$(INTDIR)\BatchPg.sbr"
	-@erase "$(INTDIR)\BatRef.obj"
	-@erase "$(INTDIR)\BatRef.sbr"
	-@erase "$(INTDIR)\BCFrame.obj"
	-@erase "$(INTDIR)\BCFrame.sbr"
	-@erase "$(INTDIR)\BGridPg.obj"
	-@erase "$(INTDIR)\BGridPg.sbr"
	-@erase "$(INTDIR)\BPickDlg.obj"
	-@erase "$(INTDIR)\BPickDlg.sbr"
	-@erase "$(INTDIR)\BTabCtrl.obj"
	-@erase "$(INTDIR)\BTabCtrl.sbr"
	-@erase "$(INTDIR)\CalTypeD.obj"
	-@erase "$(INTDIR)\CalTypeD.sbr"
	-@erase "$(INTDIR)\CondFrm.obj"
	-@erase "$(INTDIR)\CondFrm.sbr"
	-@erase "$(INTDIR)\Conduct.obj"
	-@erase "$(INTDIR)\Conduct.sbr"
	-@erase "$(INTDIR)\CugRpt.obj"
	-@erase "$(INTDIR)\CugRpt.sbr"
	-@erase "$(INTDIR)\CugWkl.obj"
	-@erase "$(INTDIR)\CugWkl.sbr"
	-@erase "$(INTDIR)\CustPal.obj"
	-@erase "$(INTDIR)\CustPal.sbr"
	-@erase "$(INTDIR)\DaoUtils.obj"
	-@erase "$(INTDIR)\DaoUtils.sbr"
	-@erase "$(INTDIR)\dibapi.obj"
	-@erase "$(INTDIR)\dibapi.sbr"
	-@erase "$(INTDIR)\DIBUtil.obj"
	-@erase "$(INTDIR)\DIBUtil.sbr"
	-@erase "$(INTDIR)\Eventsnk.obj"
	-@erase "$(INTDIR)\Eventsnk.sbr"
	-@erase "$(INTDIR)\EventSnk.tlb"
	-@erase "$(INTDIR)\FldProp.obj"
	-@erase "$(INTDIR)\FldProp.sbr"
	-@erase "$(INTDIR)\GenBox.obj"
	-@erase "$(INTDIR)\GenBox.sbr"
	-@erase "$(INTDIR)\GenDef.obj"
	-@erase "$(INTDIR)\GenDef.sbr"
	-@erase "$(INTDIR)\Generics.obj"
	-@erase "$(INTDIR)\Generics.sbr"
	-@erase "$(INTDIR)\GnDfPrDc.obj"
	-@erase "$(INTDIR)\GnDfPrDc.sbr"
	-@erase "$(INTDIR)\GnDfPrVw.obj"
	-@erase "$(INTDIR)\GnDfPrVw.sbr"
	-@erase "$(INTDIR)\HandCtrl.obj"
	-@erase "$(INTDIR)\HandCtrl.sbr"
	-@erase "$(INTDIR)\hidedlg.obj"
	-@erase "$(INTDIR)\hidedlg.sbr"
	-@erase "$(INTDIR)\IniFiles.obj"
	-@erase "$(INTDIR)\IniFiles.sbr"
	-@erase "$(INTDIR)\InjSeq.obj"
	-@erase "$(INTDIR)\InjSeq.sbr"
	-@erase "$(INTDIR)\LccInq.obj"
	-@erase "$(INTDIR)\LccInq.sbr"
	-@erase "$(INTDIR)\LccMgr.obj"
	-@erase "$(INTDIR)\LccMgr.sbr"
	-@erase "$(INTDIR)\LCCProxy.obj"
	-@erase "$(INTDIR)\LCCProxy.sbr"
	-@erase "$(INTDIR)\LCCUser.obj"
	-@erase "$(INTDIR)\LCCUser.sbr"
	-@erase "$(INTDIR)\LccVDoc.obj"
	-@erase "$(INTDIR)\LccVDoc.sbr"
	-@erase "$(INTDIR)\LccView.obj"
	-@erase "$(INTDIR)\LccView.sbr"
	-@erase "$(INTDIR)\LCDWnd.obj"
	-@erase "$(INTDIR)\LCDWnd.sbr"
	-@erase "$(INTDIR)\LEDDisp.obj"
	-@erase "$(INTDIR)\LEDDisp.sbr"
	-@erase "$(INTDIR)\Level.obj"
	-@erase "$(INTDIR)\Level.sbr"
	-@erase "$(INTDIR)\LiftWnd.obj"
	-@erase "$(INTDIR)\LiftWnd.sbr"
	-@erase "$(INTDIR)\LogonDlg.obj"
	-@erase "$(INTDIR)\LogonDlg.sbr"
	-@erase "$(INTDIR)\Lucept.obj"
	-@erase "$(INTDIR)\Lucept.pch"
	-@erase "$(INTDIR)\Lucept.res"
	-@erase "$(INTDIR)\Lucept.sbr"
	-@erase "$(INTDIR)\LucSamp.obj"
	-@erase "$(INTDIR)\LucSamp.sbr"
	-@erase "$(INTDIR)\LucyBar.obj"
	-@erase "$(INTDIR)\LucyBar.sbr"
	-@erase "$(INTDIR)\LucyDoc.obj"
	-@erase "$(INTDIR)\LucyDoc.sbr"
	-@erase "$(INTDIR)\LucyFrm.obj"
	-@erase "$(INTDIR)\LucyFrm.sbr"
	-@erase "$(INTDIR)\LucyView.obj"
	-@erase "$(INTDIR)\LucyView.sbr"
	-@erase "$(INTDIR)\LumActor.obj"
	-@erase "$(INTDIR)\LumActor.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MDefDlg.obj"
	-@erase "$(INTDIR)\MDefDlg.sbr"
	-@erase "$(INTDIR)\MeasDef.obj"
	-@erase "$(INTDIR)\MeasDef.sbr"
	-@erase "$(INTDIR)\MeasNew.obj"
	-@erase "$(INTDIR)\MeasNew.sbr"
	-@erase "$(INTDIR)\MeasPrDc.obj"
	-@erase "$(INTDIR)\MeasPrDc.sbr"
	-@erase "$(INTDIR)\MeasPrVw.obj"
	-@erase "$(INTDIR)\MeasPrVw.sbr"
	-@erase "$(INTDIR)\MeasSel.obj"
	-@erase "$(INTDIR)\MeasSel.sbr"
	-@erase "$(INTDIR)\Measures.obj"
	-@erase "$(INTDIR)\Measures.sbr"
	-@erase "$(INTDIR)\ModulVer.obj"
	-@erase "$(INTDIR)\ModulVer.sbr"
	-@erase "$(INTDIR)\MProcDlg.obj"
	-@erase "$(INTDIR)\MProcDlg.sbr"
	-@erase "$(INTDIR)\PPUAdd.obj"
	-@erase "$(INTDIR)\PPUAdd.sbr"
	-@erase "$(INTDIR)\PPUDel.obj"
	-@erase "$(INTDIR)\PPUDel.sbr"
	-@erase "$(INTDIR)\PPUEdit.obj"
	-@erase "$(INTDIR)\PPUEdit.sbr"
	-@erase "$(INTDIR)\RefList.obj"
	-@erase "$(INTDIR)\RefList.sbr"
	-@erase "$(INTDIR)\RGridFrm.obj"
	-@erase "$(INTDIR)\RGridFrm.sbr"
	-@erase "$(INTDIR)\RptDoc.obj"
	-@erase "$(INTDIR)\RptDoc.sbr"
	-@erase "$(INTDIR)\RptFInfo.obj"
	-@erase "$(INTDIR)\RptFInfo.sbr"
	-@erase "$(INTDIR)\RptFrame.obj"
	-@erase "$(INTDIR)\RptFrame.sbr"
	-@erase "$(INTDIR)\RptPgCol.obj"
	-@erase "$(INTDIR)\RptPgCol.sbr"
	-@erase "$(INTDIR)\RptPgFmt.obj"
	-@erase "$(INTDIR)\RptPgFmt.sbr"
	-@erase "$(INTDIR)\RptPgFrm.obj"
	-@erase "$(INTDIR)\RptPgFrm.sbr"
	-@erase "$(INTDIR)\RptPgGph.obj"
	-@erase "$(INTDIR)\RptPgGph.sbr"
	-@erase "$(INTDIR)\RptPgQry.obj"
	-@erase "$(INTDIR)\RptPgQry.sbr"
	-@erase "$(INTDIR)\RptPgSrt.obj"
	-@erase "$(INTDIR)\RptPgSrt.sbr"
	-@erase "$(INTDIR)\RptPgWkl.obj"
	-@erase "$(INTDIR)\RptPgWkl.sbr"
	-@erase "$(INTDIR)\RptUgDao.obj"
	-@erase "$(INTDIR)\RptUgDao.sbr"
	-@erase "$(INTDIR)\RptView.obj"
	-@erase "$(INTDIR)\RptView.sbr"
	-@erase "$(INTDIR)\SampFrm.obj"
	-@erase "$(INTDIR)\SampFrm.sbr"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\Sample.sbr"
	-@erase "$(INTDIR)\SampRef.obj"
	-@erase "$(INTDIR)\SampRef.sbr"
	-@erase "$(INTDIR)\SampView.obj"
	-@erase "$(INTDIR)\SampView.sbr"
	-@erase "$(INTDIR)\ScanPage.obj"
	-@erase "$(INTDIR)\ScanPage.sbr"
	-@erase "$(INTDIR)\ScanSht.obj"
	-@erase "$(INTDIR)\ScanSht.sbr"
	-@erase "$(INTDIR)\ScnChain.obj"
	-@erase "$(INTDIR)\ScnChain.sbr"
	-@erase "$(INTDIR)\ScnSumPg.obj"
	-@erase "$(INTDIR)\ScnSumPg.sbr"
	-@erase "$(INTDIR)\SetupDlg.obj"
	-@erase "$(INTDIR)\SetupDlg.sbr"
	-@erase "$(INTDIR)\SPickDlg.obj"
	-@erase "$(INTDIR)\SPickDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TempDisp.obj"
	-@erase "$(INTDIR)\TempDisp.sbr"
	-@erase "$(INTDIR)\TimeDisp.obj"
	-@erase "$(INTDIR)\TimeDisp.sbr"
	-@erase "$(INTDIR)\Tube.obj"
	-@erase "$(INTDIR)\Tube.sbr"
	-@erase "$(INTDIR)\Ugcbtype.obj"
	-@erase "$(INTDIR)\Ugcbtype.sbr"
	-@erase "$(INTDIR)\Ugcell.obj"
	-@erase "$(INTDIR)\Ugcell.sbr"
	-@erase "$(INTDIR)\Ugceltyp.obj"
	-@erase "$(INTDIR)\Ugceltyp.sbr"
	-@erase "$(INTDIR)\Ugcnrbtn.obj"
	-@erase "$(INTDIR)\Ugcnrbtn.sbr"
	-@erase "$(INTDIR)\Ugctafnt.obj"
	-@erase "$(INTDIR)\Ugctafnt.sbr"
	-@erase "$(INTDIR)\Ugctarrw.obj"
	-@erase "$(INTDIR)\Ugctarrw.sbr"
	-@erase "$(INTDIR)\Ugctbutn.obj"
	-@erase "$(INTDIR)\Ugctbutn.sbr"
	-@erase "$(INTDIR)\Ugctelps.obj"
	-@erase "$(INTDIR)\Ugctelps.sbr"
	-@erase "$(INTDIR)\Ugctpie.obj"
	-@erase "$(INTDIR)\Ugctpie.sbr"
	-@erase "$(INTDIR)\Ugctpro1.obj"
	-@erase "$(INTDIR)\Ugctpro1.sbr"
	-@erase "$(INTDIR)\Ugctprog.obj"
	-@erase "$(INTDIR)\Ugctprog.sbr"
	-@erase "$(INTDIR)\Ugctrado.obj"
	-@erase "$(INTDIR)\Ugctrado.sbr"
	-@erase "$(INTDIR)\Ugctrl.obj"
	-@erase "$(INTDIR)\Ugctrl.sbr"
	-@erase "$(INTDIR)\Ugctsarw.obj"
	-@erase "$(INTDIR)\Ugctsarw.sbr"
	-@erase "$(INTDIR)\Ugctspin.obj"
	-@erase "$(INTDIR)\Ugctspin.sbr"
	-@erase "$(INTDIR)\Ugdao.obj"
	-@erase "$(INTDIR)\Ugdao.sbr"
	-@erase "$(INTDIR)\Ugdltype.obj"
	-@erase "$(INTDIR)\Ugdltype.sbr"
	-@erase "$(INTDIR)\Ugdrgdrp.obj"
	-@erase "$(INTDIR)\Ugdrgdrp.sbr"
	-@erase "$(INTDIR)\Ugdrwhnt.obj"
	-@erase "$(INTDIR)\Ugdrwhnt.sbr"
	-@erase "$(INTDIR)\Ugdtasrc.obj"
	-@erase "$(INTDIR)\Ugdtasrc.sbr"
	-@erase "$(INTDIR)\Ugedit.obj"
	-@erase "$(INTDIR)\Ugedit.sbr"
	-@erase "$(INTDIR)\Uggrid.obj"
	-@erase "$(INTDIR)\Uggrid.sbr"
	-@erase "$(INTDIR)\Ughint.obj"
	-@erase "$(INTDIR)\Ughint.sbr"
	-@erase "$(INTDIR)\Ughscrol.obj"
	-@erase "$(INTDIR)\Ughscrol.sbr"
	-@erase "$(INTDIR)\Uglstbox.obj"
	-@erase "$(INTDIR)\Uglstbox.sbr"
	-@erase "$(INTDIR)\Ugmemman.obj"
	-@erase "$(INTDIR)\Ugmemman.sbr"
	-@erase "$(INTDIR)\Ugmultis.obj"
	-@erase "$(INTDIR)\Ugmultis.sbr"
	-@erase "$(INTDIR)\Ugprint.obj"
	-@erase "$(INTDIR)\Ugprint.sbr"
	-@erase "$(INTDIR)\Ugroutin.obj"
	-@erase "$(INTDIR)\Ugroutin.sbr"
	-@erase "$(INTDIR)\Ugsidehd.obj"
	-@erase "$(INTDIR)\Ugsidehd.sbr"
	-@erase "$(INTDIR)\UGTab.obj"
	-@erase "$(INTDIR)\UGTab.sbr"
	-@erase "$(INTDIR)\Ugtophdg.obj"
	-@erase "$(INTDIR)\Ugtophdg.sbr"
	-@erase "$(INTDIR)\Ugvscrol.obj"
	-@erase "$(INTDIR)\Ugvscrol.sbr"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\User.sbr"
	-@erase "$(INTDIR)\UserDB.obj"
	-@erase "$(INTDIR)\UserDB.sbr"
	-@erase "$(INTDIR)\UserFlag.obj"
	-@erase "$(INTDIR)\UserFlag.sbr"
	-@erase "$(INTDIR)\UsrAdmin.obj"
	-@erase "$(INTDIR)\UsrAdmin.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WklDoc.obj"
	-@erase "$(INTDIR)\WklDoc.sbr"
	-@erase "$(INTDIR)\WklFrame.obj"
	-@erase "$(INTDIR)\WklFrame.sbr"
	-@erase "$(INTDIR)\WklMods.obj"
	-@erase "$(INTDIR)\WklMods.sbr"
	-@erase "$(INTDIR)\WklRead.obj"
	-@erase "$(INTDIR)\WklRead.sbr"
	-@erase "$(INTDIR)\WklRef.obj"
	-@erase "$(INTDIR)\WklRef.sbr"
	-@erase "$(INTDIR)\WklView.obj"
	-@erase "$(INTDIR)\WklView.sbr"
	-@erase "$(OUTDIR)\Advance.exe"
	-@erase "$(OUTDIR)\Advance.map"
	-@erase "$(OUTDIR)\Lucept.bsc"
	-@erase ".\.\Debug\Advance.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Lucept.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Lucept.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/o"$(OUTDIR)\Lucept.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ACCMgr.sbr" \
	"$(INTDIR)\ACCtnr.sbr" \
	"$(INTDIR)\ACCtrl.sbr" \
	"$(INTDIR)\acdisp.sbr" \
	"$(INTDIR)\AddCSamp.sbr" \
	"$(INTDIR)\Addreag.sbr" \
	"$(INTDIR)\ADefDlg.sbr" \
	"$(INTDIR)\ADPgACal.sbr" \
	"$(INTDIR)\ADPgCal.sbr" \
	"$(INTDIR)\ADPgFlag.sbr" \
	"$(INTDIR)\ADPgFld.sbr" \
	"$(INTDIR)\ADPgMain.sbr" \
	"$(INTDIR)\ADPgName.sbr" \
	"$(INTDIR)\ADPrpSh.sbr" \
	"$(INTDIR)\AssayNew.sbr" \
	"$(INTDIR)\Assays.sbr" \
	"$(INTDIR)\AssaySel.sbr" \
	"$(INTDIR)\AssyDef.sbr" \
	"$(INTDIR)\AssyPrDc.sbr" \
	"$(INTDIR)\AssyPrVw.sbr" \
	"$(INTDIR)\BatCalc.sbr" \
	"$(INTDIR)\Batch.sbr" \
	"$(INTDIR)\BatchPg.sbr" \
	"$(INTDIR)\BatRef.sbr" \
	"$(INTDIR)\BCFrame.sbr" \
	"$(INTDIR)\BGridPg.sbr" \
	"$(INTDIR)\BPickDlg.sbr" \
	"$(INTDIR)\BTabCtrl.sbr" \
	"$(INTDIR)\CalTypeD.sbr" \
	"$(INTDIR)\CondFrm.sbr" \
	"$(INTDIR)\Conduct.sbr" \
	"$(INTDIR)\CugRpt.sbr" \
	"$(INTDIR)\CugWkl.sbr" \
	"$(INTDIR)\CustPal.sbr" \
	"$(INTDIR)\DaoUtils.sbr" \
	"$(INTDIR)\dibapi.sbr" \
	"$(INTDIR)\DIBUtil.sbr" \
	"$(INTDIR)\Eventsnk.sbr" \
	"$(INTDIR)\FldProp.sbr" \
	"$(INTDIR)\GenBox.sbr" \
	"$(INTDIR)\GenDef.sbr" \
	"$(INTDIR)\Generics.sbr" \
	"$(INTDIR)\GnDfPrDc.sbr" \
	"$(INTDIR)\GnDfPrVw.sbr" \
	"$(INTDIR)\HandCtrl.sbr" \
	"$(INTDIR)\hidedlg.sbr" \
	"$(INTDIR)\IniFiles.sbr" \
	"$(INTDIR)\InjSeq.sbr" \
	"$(INTDIR)\LccInq.sbr" \
	"$(INTDIR)\LccMgr.sbr" \
	"$(INTDIR)\LCCProxy.sbr" \
	"$(INTDIR)\LCCUser.sbr" \
	"$(INTDIR)\LccVDoc.sbr" \
	"$(INTDIR)\LccView.sbr" \
	"$(INTDIR)\LCDWnd.sbr" \
	"$(INTDIR)\LEDDisp.sbr" \
	"$(INTDIR)\Level.sbr" \
	"$(INTDIR)\LiftWnd.sbr" \
	"$(INTDIR)\LogonDlg.sbr" \
	"$(INTDIR)\Lucept.sbr" \
	"$(INTDIR)\LucSamp.sbr" \
	"$(INTDIR)\LucyBar.sbr" \
	"$(INTDIR)\LucyDoc.sbr" \
	"$(INTDIR)\LucyFrm.sbr" \
	"$(INTDIR)\LucyView.sbr" \
	"$(INTDIR)\LumActor.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MDefDlg.sbr" \
	"$(INTDIR)\MeasDef.sbr" \
	"$(INTDIR)\MeasNew.sbr" \
	"$(INTDIR)\MeasPrDc.sbr" \
	"$(INTDIR)\MeasPrVw.sbr" \
	"$(INTDIR)\MeasSel.sbr" \
	"$(INTDIR)\Measures.sbr" \
	"$(INTDIR)\ModulVer.sbr" \
	"$(INTDIR)\MProcDlg.sbr" \
	"$(INTDIR)\PPUAdd.sbr" \
	"$(INTDIR)\PPUDel.sbr" \
	"$(INTDIR)\PPUEdit.sbr" \
	"$(INTDIR)\RefList.sbr" \
	"$(INTDIR)\RGridFrm.sbr" \
	"$(INTDIR)\RptDoc.sbr" \
	"$(INTDIR)\RptFInfo.sbr" \
	"$(INTDIR)\RptFrame.sbr" \
	"$(INTDIR)\RptPgCol.sbr" \
	"$(INTDIR)\RptPgFmt.sbr" \
	"$(INTDIR)\RptPgFrm.sbr" \
	"$(INTDIR)\RptPgGph.sbr" \
	"$(INTDIR)\RptPgQry.sbr" \
	"$(INTDIR)\RptPgSrt.sbr" \
	"$(INTDIR)\RptPgWkl.sbr" \
	"$(INTDIR)\RptUgDao.sbr" \
	"$(INTDIR)\RptView.sbr" \
	"$(INTDIR)\SampFrm.sbr" \
	"$(INTDIR)\Sample.sbr" \
	"$(INTDIR)\SampRef.sbr" \
	"$(INTDIR)\SampView.sbr" \
	"$(INTDIR)\ScanPage.sbr" \
	"$(INTDIR)\ScanSht.sbr" \
	"$(INTDIR)\ScnChain.sbr" \
	"$(INTDIR)\ScnSumPg.sbr" \
	"$(INTDIR)\SetupDlg.sbr" \
	"$(INTDIR)\SPickDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TempDisp.sbr" \
	"$(INTDIR)\TimeDisp.sbr" \
	"$(INTDIR)\Tube.sbr" \
	"$(INTDIR)\Ugcbtype.sbr" \
	"$(INTDIR)\Ugcell.sbr" \
	"$(INTDIR)\Ugceltyp.sbr" \
	"$(INTDIR)\Ugcnrbtn.sbr" \
	"$(INTDIR)\Ugctafnt.sbr" \
	"$(INTDIR)\Ugctarrw.sbr" \
	"$(INTDIR)\Ugctbutn.sbr" \
	"$(INTDIR)\Ugctelps.sbr" \
	"$(INTDIR)\Ugctpie.sbr" \
	"$(INTDIR)\Ugctpro1.sbr" \
	"$(INTDIR)\Ugctprog.sbr" \
	"$(INTDIR)\Ugctrado.sbr" \
	"$(INTDIR)\Ugctrl.sbr" \
	"$(INTDIR)\Ugctsarw.sbr" \
	"$(INTDIR)\Ugctspin.sbr" \
	"$(INTDIR)\Ugdao.sbr" \
	"$(INTDIR)\Ugdltype.sbr" \
	"$(INTDIR)\Ugdrgdrp.sbr" \
	"$(INTDIR)\Ugdrwhnt.sbr" \
	"$(INTDIR)\Ugdtasrc.sbr" \
	"$(INTDIR)\Ugedit.sbr" \
	"$(INTDIR)\Uggrid.sbr" \
	"$(INTDIR)\Ughint.sbr" \
	"$(INTDIR)\Ughscrol.sbr" \
	"$(INTDIR)\Uglstbox.sbr" \
	"$(INTDIR)\Ugmemman.sbr" \
	"$(INTDIR)\Ugmultis.sbr" \
	"$(INTDIR)\Ugprint.sbr" \
	"$(INTDIR)\Ugroutin.sbr" \
	"$(INTDIR)\Ugsidehd.sbr" \
	"$(INTDIR)\UGTab.sbr" \
	"$(INTDIR)\Ugtophdg.sbr" \
	"$(INTDIR)\Ugvscrol.sbr" \
	"$(INTDIR)\User.sbr" \
	"$(INTDIR)\UserDB.sbr" \
	"$(INTDIR)\UserFlag.sbr" \
	"$(INTDIR)\UsrAdmin.sbr" \
	"$(INTDIR)\WklDoc.sbr" \
	"$(INTDIR)\WklFrame.sbr" \
	"$(INTDIR)\WklMods.sbr" \
	"$(INTDIR)\WklRead.sbr" \
	"$(INTDIR)\WklRef.sbr" \
	"$(INTDIR)\WklView.sbr"

"$(OUTDIR)\Lucept.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\PhilUtil\Debug\PhilUtil.lib ..\Olectra\Debug\Olectra.lib winmm.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\Advance.map" /debug /machine:I386 /out:"$(OUTDIR)\Advance.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ACCMgr.obj" \
	"$(INTDIR)\ACCtnr.obj" \
	"$(INTDIR)\ACCtrl.obj" \
	"$(INTDIR)\acdisp.obj" \
	"$(INTDIR)\AddCSamp.obj" \
	"$(INTDIR)\Addreag.obj" \
	"$(INTDIR)\ADefDlg.obj" \
	"$(INTDIR)\ADPgACal.obj" \
	"$(INTDIR)\ADPgCal.obj" \
	"$(INTDIR)\ADPgFlag.obj" \
	"$(INTDIR)\ADPgFld.obj" \
	"$(INTDIR)\ADPgMain.obj" \
	"$(INTDIR)\ADPgName.obj" \
	"$(INTDIR)\ADPrpSh.obj" \
	"$(INTDIR)\AssayNew.obj" \
	"$(INTDIR)\Assays.obj" \
	"$(INTDIR)\AssaySel.obj" \
	"$(INTDIR)\AssyDef.obj" \
	"$(INTDIR)\AssyPrDc.obj" \
	"$(INTDIR)\AssyPrVw.obj" \
	"$(INTDIR)\BatCalc.obj" \
	"$(INTDIR)\Batch.obj" \
	"$(INTDIR)\BatchPg.obj" \
	"$(INTDIR)\BatRef.obj" \
	"$(INTDIR)\BCFrame.obj" \
	"$(INTDIR)\BGridPg.obj" \
	"$(INTDIR)\BPickDlg.obj" \
	"$(INTDIR)\BTabCtrl.obj" \
	"$(INTDIR)\CalTypeD.obj" \
	"$(INTDIR)\CondFrm.obj" \
	"$(INTDIR)\Conduct.obj" \
	"$(INTDIR)\CugRpt.obj" \
	"$(INTDIR)\CugWkl.obj" \
	"$(INTDIR)\CustPal.obj" \
	"$(INTDIR)\DaoUtils.obj" \
	"$(INTDIR)\dibapi.obj" \
	"$(INTDIR)\DIBUtil.obj" \
	"$(INTDIR)\Eventsnk.obj" \
	"$(INTDIR)\FldProp.obj" \
	"$(INTDIR)\GenBox.obj" \
	"$(INTDIR)\GenDef.obj" \
	"$(INTDIR)\Generics.obj" \
	"$(INTDIR)\GnDfPrDc.obj" \
	"$(INTDIR)\GnDfPrVw.obj" \
	"$(INTDIR)\HandCtrl.obj" \
	"$(INTDIR)\hidedlg.obj" \
	"$(INTDIR)\IniFiles.obj" \
	"$(INTDIR)\InjSeq.obj" \
	"$(INTDIR)\LccInq.obj" \
	"$(INTDIR)\LccMgr.obj" \
	"$(INTDIR)\LCCProxy.obj" \
	"$(INTDIR)\LCCUser.obj" \
	"$(INTDIR)\LccVDoc.obj" \
	"$(INTDIR)\LccView.obj" \
	"$(INTDIR)\LCDWnd.obj" \
	"$(INTDIR)\LEDDisp.obj" \
	"$(INTDIR)\Level.obj" \
	"$(INTDIR)\LiftWnd.obj" \
	"$(INTDIR)\LogonDlg.obj" \
	"$(INTDIR)\Lucept.obj" \
	"$(INTDIR)\LucSamp.obj" \
	"$(INTDIR)\LucyBar.obj" \
	"$(INTDIR)\LucyDoc.obj" \
	"$(INTDIR)\LucyFrm.obj" \
	"$(INTDIR)\LucyView.obj" \
	"$(INTDIR)\LumActor.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDefDlg.obj" \
	"$(INTDIR)\MeasDef.obj" \
	"$(INTDIR)\MeasNew.obj" \
	"$(INTDIR)\MeasPrDc.obj" \
	"$(INTDIR)\MeasPrVw.obj" \
	"$(INTDIR)\MeasSel.obj" \
	"$(INTDIR)\Measures.obj" \
	"$(INTDIR)\ModulVer.obj" \
	"$(INTDIR)\MProcDlg.obj" \
	"$(INTDIR)\PPUAdd.obj" \
	"$(INTDIR)\PPUDel.obj" \
	"$(INTDIR)\PPUEdit.obj" \
	"$(INTDIR)\RefList.obj" \
	"$(INTDIR)\RGridFrm.obj" \
	"$(INTDIR)\RptDoc.obj" \
	"$(INTDIR)\RptFInfo.obj" \
	"$(INTDIR)\RptFrame.obj" \
	"$(INTDIR)\RptPgCol.obj" \
	"$(INTDIR)\RptPgFmt.obj" \
	"$(INTDIR)\RptPgFrm.obj" \
	"$(INTDIR)\RptPgGph.obj" \
	"$(INTDIR)\RptPgQry.obj" \
	"$(INTDIR)\RptPgSrt.obj" \
	"$(INTDIR)\RptPgWkl.obj" \
	"$(INTDIR)\RptUgDao.obj" \
	"$(INTDIR)\RptView.obj" \
	"$(INTDIR)\SampFrm.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\SampRef.obj" \
	"$(INTDIR)\SampView.obj" \
	"$(INTDIR)\ScanPage.obj" \
	"$(INTDIR)\ScanSht.obj" \
	"$(INTDIR)\ScnChain.obj" \
	"$(INTDIR)\ScnSumPg.obj" \
	"$(INTDIR)\SetupDlg.obj" \
	"$(INTDIR)\SPickDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TempDisp.obj" \
	"$(INTDIR)\TimeDisp.obj" \
	"$(INTDIR)\Tube.obj" \
	"$(INTDIR)\Ugcbtype.obj" \
	"$(INTDIR)\Ugcell.obj" \
	"$(INTDIR)\Ugceltyp.obj" \
	"$(INTDIR)\Ugcnrbtn.obj" \
	"$(INTDIR)\Ugctafnt.obj" \
	"$(INTDIR)\Ugctarrw.obj" \
	"$(INTDIR)\Ugctbutn.obj" \
	"$(INTDIR)\Ugctelps.obj" \
	"$(INTDIR)\Ugctpie.obj" \
	"$(INTDIR)\Ugctpro1.obj" \
	"$(INTDIR)\Ugctprog.obj" \
	"$(INTDIR)\Ugctrado.obj" \
	"$(INTDIR)\Ugctrl.obj" \
	"$(INTDIR)\Ugctsarw.obj" \
	"$(INTDIR)\Ugctspin.obj" \
	"$(INTDIR)\Ugdao.obj" \
	"$(INTDIR)\Ugdltype.obj" \
	"$(INTDIR)\Ugdrgdrp.obj" \
	"$(INTDIR)\Ugdrwhnt.obj" \
	"$(INTDIR)\Ugdtasrc.obj" \
	"$(INTDIR)\Ugedit.obj" \
	"$(INTDIR)\Uggrid.obj" \
	"$(INTDIR)\Ughint.obj" \
	"$(INTDIR)\Ughscrol.obj" \
	"$(INTDIR)\Uglstbox.obj" \
	"$(INTDIR)\Ugmemman.obj" \
	"$(INTDIR)\Ugmultis.obj" \
	"$(INTDIR)\Ugprint.obj" \
	"$(INTDIR)\Ugroutin.obj" \
	"$(INTDIR)\Ugsidehd.obj" \
	"$(INTDIR)\UGTab.obj" \
	"$(INTDIR)\Ugtophdg.obj" \
	"$(INTDIR)\Ugvscrol.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\UserDB.obj" \
	"$(INTDIR)\UserFlag.obj" \
	"$(INTDIR)\UsrAdmin.obj" \
	"$(INTDIR)\WklDoc.obj" \
	"$(INTDIR)\WklFrame.obj" \
	"$(INTDIR)\WklMods.obj" \
	"$(INTDIR)\WklRead.obj" \
	"$(INTDIR)\WklRef.obj" \
	"$(INTDIR)\WklView.obj" \
	"$(INTDIR)\Lucept.res"

"$(OUTDIR)\Advance.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Lucept - Win32 MacroDebug"

OUTDIR=.\Macro
INTDIR=.\Macro
# Begin Custom Macros
OutDir=.\Macro
# End Custom Macros

ALL : ".\.\Macro\Lucept.hlp" "$(OUTDIR)\Lucept.exe" "$(OUTDIR)\EventSnk.tlb" "$(OUTDIR)\Lucept.bsc"


CLEAN :
	-@erase "$(INTDIR)\ACCMgr.obj"
	-@erase "$(INTDIR)\ACCMgr.sbr"
	-@erase "$(INTDIR)\ACCtnr.obj"
	-@erase "$(INTDIR)\ACCtnr.sbr"
	-@erase "$(INTDIR)\ACCtrl.obj"
	-@erase "$(INTDIR)\ACCtrl.sbr"
	-@erase "$(INTDIR)\acdisp.obj"
	-@erase "$(INTDIR)\acdisp.sbr"
	-@erase "$(INTDIR)\AddCSamp.obj"
	-@erase "$(INTDIR)\AddCSamp.sbr"
	-@erase "$(INTDIR)\Addreag.obj"
	-@erase "$(INTDIR)\Addreag.sbr"
	-@erase "$(INTDIR)\ADefDlg.obj"
	-@erase "$(INTDIR)\ADefDlg.sbr"
	-@erase "$(INTDIR)\ADPgACal.obj"
	-@erase "$(INTDIR)\ADPgACal.sbr"
	-@erase "$(INTDIR)\ADPgCal.obj"
	-@erase "$(INTDIR)\ADPgCal.sbr"
	-@erase "$(INTDIR)\ADPgFlag.obj"
	-@erase "$(INTDIR)\ADPgFlag.sbr"
	-@erase "$(INTDIR)\ADPgFld.obj"
	-@erase "$(INTDIR)\ADPgFld.sbr"
	-@erase "$(INTDIR)\ADPgMain.obj"
	-@erase "$(INTDIR)\ADPgMain.sbr"
	-@erase "$(INTDIR)\ADPgName.obj"
	-@erase "$(INTDIR)\ADPgName.sbr"
	-@erase "$(INTDIR)\ADPrpSh.obj"
	-@erase "$(INTDIR)\ADPrpSh.sbr"
	-@erase "$(INTDIR)\AssayNew.obj"
	-@erase "$(INTDIR)\AssayNew.sbr"
	-@erase "$(INTDIR)\Assays.obj"
	-@erase "$(INTDIR)\Assays.sbr"
	-@erase "$(INTDIR)\AssaySel.obj"
	-@erase "$(INTDIR)\AssaySel.sbr"
	-@erase "$(INTDIR)\AssyDef.obj"
	-@erase "$(INTDIR)\AssyDef.sbr"
	-@erase "$(INTDIR)\AssyPrDc.obj"
	-@erase "$(INTDIR)\AssyPrDc.sbr"
	-@erase "$(INTDIR)\AssyPrVw.obj"
	-@erase "$(INTDIR)\AssyPrVw.sbr"
	-@erase "$(INTDIR)\BatCalc.obj"
	-@erase "$(INTDIR)\BatCalc.sbr"
	-@erase "$(INTDIR)\Batch.obj"
	-@erase "$(INTDIR)\Batch.sbr"
	-@erase "$(INTDIR)\BatchPg.obj"
	-@erase "$(INTDIR)\BatchPg.sbr"
	-@erase "$(INTDIR)\BatRef.obj"
	-@erase "$(INTDIR)\BatRef.sbr"
	-@erase "$(INTDIR)\BCFrame.obj"
	-@erase "$(INTDIR)\BCFrame.sbr"
	-@erase "$(INTDIR)\BGridPg.obj"
	-@erase "$(INTDIR)\BGridPg.sbr"
	-@erase "$(INTDIR)\BPickDlg.obj"
	-@erase "$(INTDIR)\BPickDlg.sbr"
	-@erase "$(INTDIR)\BTabCtrl.obj"
	-@erase "$(INTDIR)\BTabCtrl.sbr"
	-@erase "$(INTDIR)\CalTypeD.obj"
	-@erase "$(INTDIR)\CalTypeD.sbr"
	-@erase "$(INTDIR)\CondFrm.obj"
	-@erase "$(INTDIR)\CondFrm.sbr"
	-@erase "$(INTDIR)\Conduct.obj"
	-@erase "$(INTDIR)\Conduct.sbr"
	-@erase "$(INTDIR)\CugRpt.obj"
	-@erase "$(INTDIR)\CugRpt.sbr"
	-@erase "$(INTDIR)\CugWkl.obj"
	-@erase "$(INTDIR)\CugWkl.sbr"
	-@erase "$(INTDIR)\CustPal.obj"
	-@erase "$(INTDIR)\CustPal.sbr"
	-@erase "$(INTDIR)\DaoUtils.obj"
	-@erase "$(INTDIR)\DaoUtils.sbr"
	-@erase "$(INTDIR)\dibapi.obj"
	-@erase "$(INTDIR)\dibapi.sbr"
	-@erase "$(INTDIR)\DIBUtil.obj"
	-@erase "$(INTDIR)\DIBUtil.sbr"
	-@erase "$(INTDIR)\Eventsnk.obj"
	-@erase "$(INTDIR)\Eventsnk.sbr"
	-@erase "$(INTDIR)\EventSnk.tlb"
	-@erase "$(INTDIR)\FldProp.obj"
	-@erase "$(INTDIR)\FldProp.sbr"
	-@erase "$(INTDIR)\GenBox.obj"
	-@erase "$(INTDIR)\GenBox.sbr"
	-@erase "$(INTDIR)\GenDef.obj"
	-@erase "$(INTDIR)\GenDef.sbr"
	-@erase "$(INTDIR)\Generics.obj"
	-@erase "$(INTDIR)\Generics.sbr"
	-@erase "$(INTDIR)\GnDfPrDc.obj"
	-@erase "$(INTDIR)\GnDfPrDc.sbr"
	-@erase "$(INTDIR)\GnDfPrVw.obj"
	-@erase "$(INTDIR)\GnDfPrVw.sbr"
	-@erase "$(INTDIR)\HandCtrl.obj"
	-@erase "$(INTDIR)\HandCtrl.sbr"
	-@erase "$(INTDIR)\hidedlg.obj"
	-@erase "$(INTDIR)\hidedlg.sbr"
	-@erase "$(INTDIR)\IniFiles.obj"
	-@erase "$(INTDIR)\IniFiles.sbr"
	-@erase "$(INTDIR)\InjSeq.obj"
	-@erase "$(INTDIR)\InjSeq.sbr"
	-@erase "$(INTDIR)\LccInq.obj"
	-@erase "$(INTDIR)\LccInq.sbr"
	-@erase "$(INTDIR)\LccMgr.obj"
	-@erase "$(INTDIR)\LccMgr.sbr"
	-@erase "$(INTDIR)\LCCProxy.obj"
	-@erase "$(INTDIR)\LCCProxy.sbr"
	-@erase "$(INTDIR)\LCCUser.obj"
	-@erase "$(INTDIR)\LCCUser.sbr"
	-@erase "$(INTDIR)\LccVDoc.obj"
	-@erase "$(INTDIR)\LccVDoc.sbr"
	-@erase "$(INTDIR)\LccView.obj"
	-@erase "$(INTDIR)\LccView.sbr"
	-@erase "$(INTDIR)\LCDWnd.obj"
	-@erase "$(INTDIR)\LCDWnd.sbr"
	-@erase "$(INTDIR)\LEDDisp.obj"
	-@erase "$(INTDIR)\LEDDisp.sbr"
	-@erase "$(INTDIR)\Level.obj"
	-@erase "$(INTDIR)\Level.sbr"
	-@erase "$(INTDIR)\LiftWnd.obj"
	-@erase "$(INTDIR)\LiftWnd.sbr"
	-@erase "$(INTDIR)\LogonDlg.obj"
	-@erase "$(INTDIR)\LogonDlg.sbr"
	-@erase "$(INTDIR)\Lucept.obj"
	-@erase "$(INTDIR)\Lucept.pch"
	-@erase "$(INTDIR)\Lucept.res"
	-@erase "$(INTDIR)\Lucept.sbr"
	-@erase "$(INTDIR)\LucSamp.obj"
	-@erase "$(INTDIR)\LucSamp.sbr"
	-@erase "$(INTDIR)\LucyBar.obj"
	-@erase "$(INTDIR)\LucyBar.sbr"
	-@erase "$(INTDIR)\LucyDoc.obj"
	-@erase "$(INTDIR)\LucyDoc.sbr"
	-@erase "$(INTDIR)\LucyFrm.obj"
	-@erase "$(INTDIR)\LucyFrm.sbr"
	-@erase "$(INTDIR)\LucyView.obj"
	-@erase "$(INTDIR)\LucyView.sbr"
	-@erase "$(INTDIR)\LumActor.obj"
	-@erase "$(INTDIR)\LumActor.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MDefDlg.obj"
	-@erase "$(INTDIR)\MDefDlg.sbr"
	-@erase "$(INTDIR)\MeasDef.obj"
	-@erase "$(INTDIR)\MeasDef.sbr"
	-@erase "$(INTDIR)\MeasNew.obj"
	-@erase "$(INTDIR)\MeasNew.sbr"
	-@erase "$(INTDIR)\MeasPrDc.obj"
	-@erase "$(INTDIR)\MeasPrDc.sbr"
	-@erase "$(INTDIR)\MeasPrVw.obj"
	-@erase "$(INTDIR)\MeasPrVw.sbr"
	-@erase "$(INTDIR)\MeasSel.obj"
	-@erase "$(INTDIR)\MeasSel.sbr"
	-@erase "$(INTDIR)\Measures.obj"
	-@erase "$(INTDIR)\Measures.sbr"
	-@erase "$(INTDIR)\ModulVer.obj"
	-@erase "$(INTDIR)\ModulVer.sbr"
	-@erase "$(INTDIR)\MProcDlg.obj"
	-@erase "$(INTDIR)\MProcDlg.sbr"
	-@erase "$(INTDIR)\PPUAdd.obj"
	-@erase "$(INTDIR)\PPUAdd.sbr"
	-@erase "$(INTDIR)\PPUDel.obj"
	-@erase "$(INTDIR)\PPUDel.sbr"
	-@erase "$(INTDIR)\PPUEdit.obj"
	-@erase "$(INTDIR)\PPUEdit.sbr"
	-@erase "$(INTDIR)\RefList.obj"
	-@erase "$(INTDIR)\RefList.sbr"
	-@erase "$(INTDIR)\RGridFrm.obj"
	-@erase "$(INTDIR)\RGridFrm.sbr"
	-@erase "$(INTDIR)\RptDoc.obj"
	-@erase "$(INTDIR)\RptDoc.sbr"
	-@erase "$(INTDIR)\RptFInfo.obj"
	-@erase "$(INTDIR)\RptFInfo.sbr"
	-@erase "$(INTDIR)\RptFrame.obj"
	-@erase "$(INTDIR)\RptFrame.sbr"
	-@erase "$(INTDIR)\RptPgCol.obj"
	-@erase "$(INTDIR)\RptPgCol.sbr"
	-@erase "$(INTDIR)\RptPgFmt.obj"
	-@erase "$(INTDIR)\RptPgFmt.sbr"
	-@erase "$(INTDIR)\RptPgFrm.obj"
	-@erase "$(INTDIR)\RptPgFrm.sbr"
	-@erase "$(INTDIR)\RptPgGph.obj"
	-@erase "$(INTDIR)\RptPgGph.sbr"
	-@erase "$(INTDIR)\RptPgQry.obj"
	-@erase "$(INTDIR)\RptPgQry.sbr"
	-@erase "$(INTDIR)\RptPgSrt.obj"
	-@erase "$(INTDIR)\RptPgSrt.sbr"
	-@erase "$(INTDIR)\RptPgWkl.obj"
	-@erase "$(INTDIR)\RptPgWkl.sbr"
	-@erase "$(INTDIR)\RptUgDao.obj"
	-@erase "$(INTDIR)\RptUgDao.sbr"
	-@erase "$(INTDIR)\RptView.obj"
	-@erase "$(INTDIR)\RptView.sbr"
	-@erase "$(INTDIR)\SampFrm.obj"
	-@erase "$(INTDIR)\SampFrm.sbr"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\Sample.sbr"
	-@erase "$(INTDIR)\SampRef.obj"
	-@erase "$(INTDIR)\SampRef.sbr"
	-@erase "$(INTDIR)\SampView.obj"
	-@erase "$(INTDIR)\SampView.sbr"
	-@erase "$(INTDIR)\ScanPage.obj"
	-@erase "$(INTDIR)\ScanPage.sbr"
	-@erase "$(INTDIR)\ScanSht.obj"
	-@erase "$(INTDIR)\ScanSht.sbr"
	-@erase "$(INTDIR)\ScnChain.obj"
	-@erase "$(INTDIR)\ScnChain.sbr"
	-@erase "$(INTDIR)\ScnSumPg.obj"
	-@erase "$(INTDIR)\ScnSumPg.sbr"
	-@erase "$(INTDIR)\SetupDlg.obj"
	-@erase "$(INTDIR)\SetupDlg.sbr"
	-@erase "$(INTDIR)\SPickDlg.obj"
	-@erase "$(INTDIR)\SPickDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TempDisp.obj"
	-@erase "$(INTDIR)\TempDisp.sbr"
	-@erase "$(INTDIR)\TimeDisp.obj"
	-@erase "$(INTDIR)\TimeDisp.sbr"
	-@erase "$(INTDIR)\Tube.obj"
	-@erase "$(INTDIR)\Tube.sbr"
	-@erase "$(INTDIR)\Ugcbtype.obj"
	-@erase "$(INTDIR)\Ugcbtype.sbr"
	-@erase "$(INTDIR)\Ugcell.obj"
	-@erase "$(INTDIR)\Ugcell.sbr"
	-@erase "$(INTDIR)\Ugceltyp.obj"
	-@erase "$(INTDIR)\Ugceltyp.sbr"
	-@erase "$(INTDIR)\Ugcnrbtn.obj"
	-@erase "$(INTDIR)\Ugcnrbtn.sbr"
	-@erase "$(INTDIR)\Ugctafnt.obj"
	-@erase "$(INTDIR)\Ugctafnt.sbr"
	-@erase "$(INTDIR)\Ugctarrw.obj"
	-@erase "$(INTDIR)\Ugctarrw.sbr"
	-@erase "$(INTDIR)\Ugctbutn.obj"
	-@erase "$(INTDIR)\Ugctbutn.sbr"
	-@erase "$(INTDIR)\Ugctelps.obj"
	-@erase "$(INTDIR)\Ugctelps.sbr"
	-@erase "$(INTDIR)\Ugctpie.obj"
	-@erase "$(INTDIR)\Ugctpie.sbr"
	-@erase "$(INTDIR)\Ugctpro1.obj"
	-@erase "$(INTDIR)\Ugctpro1.sbr"
	-@erase "$(INTDIR)\Ugctprog.obj"
	-@erase "$(INTDIR)\Ugctprog.sbr"
	-@erase "$(INTDIR)\Ugctrado.obj"
	-@erase "$(INTDIR)\Ugctrado.sbr"
	-@erase "$(INTDIR)\Ugctrl.obj"
	-@erase "$(INTDIR)\Ugctrl.sbr"
	-@erase "$(INTDIR)\Ugctsarw.obj"
	-@erase "$(INTDIR)\Ugctsarw.sbr"
	-@erase "$(INTDIR)\Ugctspin.obj"
	-@erase "$(INTDIR)\Ugctspin.sbr"
	-@erase "$(INTDIR)\Ugdao.obj"
	-@erase "$(INTDIR)\Ugdao.sbr"
	-@erase "$(INTDIR)\Ugdltype.obj"
	-@erase "$(INTDIR)\Ugdltype.sbr"
	-@erase "$(INTDIR)\Ugdrgdrp.obj"
	-@erase "$(INTDIR)\Ugdrgdrp.sbr"
	-@erase "$(INTDIR)\Ugdrwhnt.obj"
	-@erase "$(INTDIR)\Ugdrwhnt.sbr"
	-@erase "$(INTDIR)\Ugdtasrc.obj"
	-@erase "$(INTDIR)\Ugdtasrc.sbr"
	-@erase "$(INTDIR)\Ugedit.obj"
	-@erase "$(INTDIR)\Ugedit.sbr"
	-@erase "$(INTDIR)\Uggrid.obj"
	-@erase "$(INTDIR)\Uggrid.sbr"
	-@erase "$(INTDIR)\Ughint.obj"
	-@erase "$(INTDIR)\Ughint.sbr"
	-@erase "$(INTDIR)\Ughscrol.obj"
	-@erase "$(INTDIR)\Ughscrol.sbr"
	-@erase "$(INTDIR)\Uglstbox.obj"
	-@erase "$(INTDIR)\Uglstbox.sbr"
	-@erase "$(INTDIR)\Ugmemman.obj"
	-@erase "$(INTDIR)\Ugmemman.sbr"
	-@erase "$(INTDIR)\Ugmultis.obj"
	-@erase "$(INTDIR)\Ugmultis.sbr"
	-@erase "$(INTDIR)\Ugprint.obj"
	-@erase "$(INTDIR)\Ugprint.sbr"
	-@erase "$(INTDIR)\Ugroutin.obj"
	-@erase "$(INTDIR)\Ugroutin.sbr"
	-@erase "$(INTDIR)\Ugsidehd.obj"
	-@erase "$(INTDIR)\Ugsidehd.sbr"
	-@erase "$(INTDIR)\UGTab.obj"
	-@erase "$(INTDIR)\UGTab.sbr"
	-@erase "$(INTDIR)\Ugtophdg.obj"
	-@erase "$(INTDIR)\Ugtophdg.sbr"
	-@erase "$(INTDIR)\Ugvscrol.obj"
	-@erase "$(INTDIR)\Ugvscrol.sbr"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\User.sbr"
	-@erase "$(INTDIR)\UserDB.obj"
	-@erase "$(INTDIR)\UserDB.sbr"
	-@erase "$(INTDIR)\UserFlag.obj"
	-@erase "$(INTDIR)\UserFlag.sbr"
	-@erase "$(INTDIR)\UsrAdmin.obj"
	-@erase "$(INTDIR)\UsrAdmin.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WklDoc.obj"
	-@erase "$(INTDIR)\WklDoc.sbr"
	-@erase "$(INTDIR)\WklFrame.obj"
	-@erase "$(INTDIR)\WklFrame.sbr"
	-@erase "$(INTDIR)\WklMods.obj"
	-@erase "$(INTDIR)\WklMods.sbr"
	-@erase "$(INTDIR)\WklRead.obj"
	-@erase "$(INTDIR)\WklRead.sbr"
	-@erase "$(INTDIR)\WklRef.obj"
	-@erase "$(INTDIR)\WklRef.sbr"
	-@erase "$(INTDIR)\WklView.obj"
	-@erase "$(INTDIR)\WklView.sbr"
	-@erase "$(OUTDIR)\Lucept.bsc"
	-@erase "$(OUTDIR)\Lucept.exe"
	-@erase "$(OUTDIR)\Lucept.ilk"
	-@erase "$(OUTDIR)\Lucept.pdb"
	-@erase ".\.\Macro\Lucept.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Lucept.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /P /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Lucept.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Lucept.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ACCMgr.sbr" \
	"$(INTDIR)\ACCtnr.sbr" \
	"$(INTDIR)\ACCtrl.sbr" \
	"$(INTDIR)\acdisp.sbr" \
	"$(INTDIR)\AddCSamp.sbr" \
	"$(INTDIR)\Addreag.sbr" \
	"$(INTDIR)\ADefDlg.sbr" \
	"$(INTDIR)\ADPgACal.sbr" \
	"$(INTDIR)\ADPgCal.sbr" \
	"$(INTDIR)\ADPgFlag.sbr" \
	"$(INTDIR)\ADPgFld.sbr" \
	"$(INTDIR)\ADPgMain.sbr" \
	"$(INTDIR)\ADPgName.sbr" \
	"$(INTDIR)\ADPrpSh.sbr" \
	"$(INTDIR)\AssayNew.sbr" \
	"$(INTDIR)\Assays.sbr" \
	"$(INTDIR)\AssaySel.sbr" \
	"$(INTDIR)\AssyDef.sbr" \
	"$(INTDIR)\AssyPrDc.sbr" \
	"$(INTDIR)\AssyPrVw.sbr" \
	"$(INTDIR)\BatCalc.sbr" \
	"$(INTDIR)\Batch.sbr" \
	"$(INTDIR)\BatchPg.sbr" \
	"$(INTDIR)\BatRef.sbr" \
	"$(INTDIR)\BCFrame.sbr" \
	"$(INTDIR)\BGridPg.sbr" \
	"$(INTDIR)\BPickDlg.sbr" \
	"$(INTDIR)\BTabCtrl.sbr" \
	"$(INTDIR)\CalTypeD.sbr" \
	"$(INTDIR)\CondFrm.sbr" \
	"$(INTDIR)\Conduct.sbr" \
	"$(INTDIR)\CugRpt.sbr" \
	"$(INTDIR)\CugWkl.sbr" \
	"$(INTDIR)\CustPal.sbr" \
	"$(INTDIR)\DaoUtils.sbr" \
	"$(INTDIR)\dibapi.sbr" \
	"$(INTDIR)\DIBUtil.sbr" \
	"$(INTDIR)\Eventsnk.sbr" \
	"$(INTDIR)\FldProp.sbr" \
	"$(INTDIR)\GenBox.sbr" \
	"$(INTDIR)\GenDef.sbr" \
	"$(INTDIR)\Generics.sbr" \
	"$(INTDIR)\GnDfPrDc.sbr" \
	"$(INTDIR)\GnDfPrVw.sbr" \
	"$(INTDIR)\HandCtrl.sbr" \
	"$(INTDIR)\hidedlg.sbr" \
	"$(INTDIR)\IniFiles.sbr" \
	"$(INTDIR)\InjSeq.sbr" \
	"$(INTDIR)\LccInq.sbr" \
	"$(INTDIR)\LccMgr.sbr" \
	"$(INTDIR)\LCCProxy.sbr" \
	"$(INTDIR)\LCCUser.sbr" \
	"$(INTDIR)\LccVDoc.sbr" \
	"$(INTDIR)\LccView.sbr" \
	"$(INTDIR)\LCDWnd.sbr" \
	"$(INTDIR)\LEDDisp.sbr" \
	"$(INTDIR)\Level.sbr" \
	"$(INTDIR)\LiftWnd.sbr" \
	"$(INTDIR)\LogonDlg.sbr" \
	"$(INTDIR)\Lucept.sbr" \
	"$(INTDIR)\LucSamp.sbr" \
	"$(INTDIR)\LucyBar.sbr" \
	"$(INTDIR)\LucyDoc.sbr" \
	"$(INTDIR)\LucyFrm.sbr" \
	"$(INTDIR)\LucyView.sbr" \
	"$(INTDIR)\LumActor.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MDefDlg.sbr" \
	"$(INTDIR)\MeasDef.sbr" \
	"$(INTDIR)\MeasNew.sbr" \
	"$(INTDIR)\MeasPrDc.sbr" \
	"$(INTDIR)\MeasPrVw.sbr" \
	"$(INTDIR)\MeasSel.sbr" \
	"$(INTDIR)\Measures.sbr" \
	"$(INTDIR)\ModulVer.sbr" \
	"$(INTDIR)\MProcDlg.sbr" \
	"$(INTDIR)\PPUAdd.sbr" \
	"$(INTDIR)\PPUDel.sbr" \
	"$(INTDIR)\PPUEdit.sbr" \
	"$(INTDIR)\RefList.sbr" \
	"$(INTDIR)\RGridFrm.sbr" \
	"$(INTDIR)\RptDoc.sbr" \
	"$(INTDIR)\RptFInfo.sbr" \
	"$(INTDIR)\RptFrame.sbr" \
	"$(INTDIR)\RptPgCol.sbr" \
	"$(INTDIR)\RptPgFmt.sbr" \
	"$(INTDIR)\RptPgFrm.sbr" \
	"$(INTDIR)\RptPgGph.sbr" \
	"$(INTDIR)\RptPgQry.sbr" \
	"$(INTDIR)\RptPgSrt.sbr" \
	"$(INTDIR)\RptPgWkl.sbr" \
	"$(INTDIR)\RptUgDao.sbr" \
	"$(INTDIR)\RptView.sbr" \
	"$(INTDIR)\SampFrm.sbr" \
	"$(INTDIR)\Sample.sbr" \
	"$(INTDIR)\SampRef.sbr" \
	"$(INTDIR)\SampView.sbr" \
	"$(INTDIR)\ScanPage.sbr" \
	"$(INTDIR)\ScanSht.sbr" \
	"$(INTDIR)\ScnChain.sbr" \
	"$(INTDIR)\ScnSumPg.sbr" \
	"$(INTDIR)\SetupDlg.sbr" \
	"$(INTDIR)\SPickDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TempDisp.sbr" \
	"$(INTDIR)\TimeDisp.sbr" \
	"$(INTDIR)\Tube.sbr" \
	"$(INTDIR)\Ugcbtype.sbr" \
	"$(INTDIR)\Ugcell.sbr" \
	"$(INTDIR)\Ugceltyp.sbr" \
	"$(INTDIR)\Ugcnrbtn.sbr" \
	"$(INTDIR)\Ugctafnt.sbr" \
	"$(INTDIR)\Ugctarrw.sbr" \
	"$(INTDIR)\Ugctbutn.sbr" \
	"$(INTDIR)\Ugctelps.sbr" \
	"$(INTDIR)\Ugctpie.sbr" \
	"$(INTDIR)\Ugctpro1.sbr" \
	"$(INTDIR)\Ugctprog.sbr" \
	"$(INTDIR)\Ugctrado.sbr" \
	"$(INTDIR)\Ugctrl.sbr" \
	"$(INTDIR)\Ugctsarw.sbr" \
	"$(INTDIR)\Ugctspin.sbr" \
	"$(INTDIR)\Ugdao.sbr" \
	"$(INTDIR)\Ugdltype.sbr" \
	"$(INTDIR)\Ugdrgdrp.sbr" \
	"$(INTDIR)\Ugdrwhnt.sbr" \
	"$(INTDIR)\Ugdtasrc.sbr" \
	"$(INTDIR)\Ugedit.sbr" \
	"$(INTDIR)\Uggrid.sbr" \
	"$(INTDIR)\Ughint.sbr" \
	"$(INTDIR)\Ughscrol.sbr" \
	"$(INTDIR)\Uglstbox.sbr" \
	"$(INTDIR)\Ugmemman.sbr" \
	"$(INTDIR)\Ugmultis.sbr" \
	"$(INTDIR)\Ugprint.sbr" \
	"$(INTDIR)\Ugroutin.sbr" \
	"$(INTDIR)\Ugsidehd.sbr" \
	"$(INTDIR)\UGTab.sbr" \
	"$(INTDIR)\Ugtophdg.sbr" \
	"$(INTDIR)\Ugvscrol.sbr" \
	"$(INTDIR)\User.sbr" \
	"$(INTDIR)\UserDB.sbr" \
	"$(INTDIR)\UserFlag.sbr" \
	"$(INTDIR)\UsrAdmin.sbr" \
	"$(INTDIR)\WklDoc.sbr" \
	"$(INTDIR)\WklFrame.sbr" \
	"$(INTDIR)\WklMods.sbr" \
	"$(INTDIR)\WklRead.sbr" \
	"$(INTDIR)\WklRef.sbr" \
	"$(INTDIR)\WklView.sbr"

"$(OUTDIR)\Lucept.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Lucept.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Lucept.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ACCMgr.obj" \
	"$(INTDIR)\ACCtnr.obj" \
	"$(INTDIR)\ACCtrl.obj" \
	"$(INTDIR)\acdisp.obj" \
	"$(INTDIR)\AddCSamp.obj" \
	"$(INTDIR)\Addreag.obj" \
	"$(INTDIR)\ADefDlg.obj" \
	"$(INTDIR)\ADPgACal.obj" \
	"$(INTDIR)\ADPgCal.obj" \
	"$(INTDIR)\ADPgFlag.obj" \
	"$(INTDIR)\ADPgFld.obj" \
	"$(INTDIR)\ADPgMain.obj" \
	"$(INTDIR)\ADPgName.obj" \
	"$(INTDIR)\ADPrpSh.obj" \
	"$(INTDIR)\AssayNew.obj" \
	"$(INTDIR)\Assays.obj" \
	"$(INTDIR)\AssaySel.obj" \
	"$(INTDIR)\AssyDef.obj" \
	"$(INTDIR)\AssyPrDc.obj" \
	"$(INTDIR)\AssyPrVw.obj" \
	"$(INTDIR)\BatCalc.obj" \
	"$(INTDIR)\Batch.obj" \
	"$(INTDIR)\BatchPg.obj" \
	"$(INTDIR)\BatRef.obj" \
	"$(INTDIR)\BCFrame.obj" \
	"$(INTDIR)\BGridPg.obj" \
	"$(INTDIR)\BPickDlg.obj" \
	"$(INTDIR)\BTabCtrl.obj" \
	"$(INTDIR)\CalTypeD.obj" \
	"$(INTDIR)\CondFrm.obj" \
	"$(INTDIR)\Conduct.obj" \
	"$(INTDIR)\CugRpt.obj" \
	"$(INTDIR)\CugWkl.obj" \
	"$(INTDIR)\CustPal.obj" \
	"$(INTDIR)\DaoUtils.obj" \
	"$(INTDIR)\dibapi.obj" \
	"$(INTDIR)\DIBUtil.obj" \
	"$(INTDIR)\Eventsnk.obj" \
	"$(INTDIR)\FldProp.obj" \
	"$(INTDIR)\GenBox.obj" \
	"$(INTDIR)\GenDef.obj" \
	"$(INTDIR)\Generics.obj" \
	"$(INTDIR)\GnDfPrDc.obj" \
	"$(INTDIR)\GnDfPrVw.obj" \
	"$(INTDIR)\HandCtrl.obj" \
	"$(INTDIR)\hidedlg.obj" \
	"$(INTDIR)\IniFiles.obj" \
	"$(INTDIR)\InjSeq.obj" \
	"$(INTDIR)\LccInq.obj" \
	"$(INTDIR)\LccMgr.obj" \
	"$(INTDIR)\LCCProxy.obj" \
	"$(INTDIR)\LCCUser.obj" \
	"$(INTDIR)\LccVDoc.obj" \
	"$(INTDIR)\LccView.obj" \
	"$(INTDIR)\LCDWnd.obj" \
	"$(INTDIR)\LEDDisp.obj" \
	"$(INTDIR)\Level.obj" \
	"$(INTDIR)\LiftWnd.obj" \
	"$(INTDIR)\LogonDlg.obj" \
	"$(INTDIR)\Lucept.obj" \
	"$(INTDIR)\LucSamp.obj" \
	"$(INTDIR)\LucyBar.obj" \
	"$(INTDIR)\LucyDoc.obj" \
	"$(INTDIR)\LucyFrm.obj" \
	"$(INTDIR)\LucyView.obj" \
	"$(INTDIR)\LumActor.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDefDlg.obj" \
	"$(INTDIR)\MeasDef.obj" \
	"$(INTDIR)\MeasNew.obj" \
	"$(INTDIR)\MeasPrDc.obj" \
	"$(INTDIR)\MeasPrVw.obj" \
	"$(INTDIR)\MeasSel.obj" \
	"$(INTDIR)\Measures.obj" \
	"$(INTDIR)\ModulVer.obj" \
	"$(INTDIR)\MProcDlg.obj" \
	"$(INTDIR)\PPUAdd.obj" \
	"$(INTDIR)\PPUDel.obj" \
	"$(INTDIR)\PPUEdit.obj" \
	"$(INTDIR)\RefList.obj" \
	"$(INTDIR)\RGridFrm.obj" \
	"$(INTDIR)\RptDoc.obj" \
	"$(INTDIR)\RptFInfo.obj" \
	"$(INTDIR)\RptFrame.obj" \
	"$(INTDIR)\RptPgCol.obj" \
	"$(INTDIR)\RptPgFmt.obj" \
	"$(INTDIR)\RptPgFrm.obj" \
	"$(INTDIR)\RptPgGph.obj" \
	"$(INTDIR)\RptPgQry.obj" \
	"$(INTDIR)\RptPgSrt.obj" \
	"$(INTDIR)\RptPgWkl.obj" \
	"$(INTDIR)\RptUgDao.obj" \
	"$(INTDIR)\RptView.obj" \
	"$(INTDIR)\SampFrm.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\SampRef.obj" \
	"$(INTDIR)\SampView.obj" \
	"$(INTDIR)\ScanPage.obj" \
	"$(INTDIR)\ScanSht.obj" \
	"$(INTDIR)\ScnChain.obj" \
	"$(INTDIR)\ScnSumPg.obj" \
	"$(INTDIR)\SetupDlg.obj" \
	"$(INTDIR)\SPickDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TempDisp.obj" \
	"$(INTDIR)\TimeDisp.obj" \
	"$(INTDIR)\Tube.obj" \
	"$(INTDIR)\Ugcbtype.obj" \
	"$(INTDIR)\Ugcell.obj" \
	"$(INTDIR)\Ugceltyp.obj" \
	"$(INTDIR)\Ugcnrbtn.obj" \
	"$(INTDIR)\Ugctafnt.obj" \
	"$(INTDIR)\Ugctarrw.obj" \
	"$(INTDIR)\Ugctbutn.obj" \
	"$(INTDIR)\Ugctelps.obj" \
	"$(INTDIR)\Ugctpie.obj" \
	"$(INTDIR)\Ugctpro1.obj" \
	"$(INTDIR)\Ugctprog.obj" \
	"$(INTDIR)\Ugctrado.obj" \
	"$(INTDIR)\Ugctrl.obj" \
	"$(INTDIR)\Ugctsarw.obj" \
	"$(INTDIR)\Ugctspin.obj" \
	"$(INTDIR)\Ugdao.obj" \
	"$(INTDIR)\Ugdltype.obj" \
	"$(INTDIR)\Ugdrgdrp.obj" \
	"$(INTDIR)\Ugdrwhnt.obj" \
	"$(INTDIR)\Ugdtasrc.obj" \
	"$(INTDIR)\Ugedit.obj" \
	"$(INTDIR)\Uggrid.obj" \
	"$(INTDIR)\Ughint.obj" \
	"$(INTDIR)\Ughscrol.obj" \
	"$(INTDIR)\Uglstbox.obj" \
	"$(INTDIR)\Ugmemman.obj" \
	"$(INTDIR)\Ugmultis.obj" \
	"$(INTDIR)\Ugprint.obj" \
	"$(INTDIR)\Ugroutin.obj" \
	"$(INTDIR)\Ugsidehd.obj" \
	"$(INTDIR)\UGTab.obj" \
	"$(INTDIR)\Ugtophdg.obj" \
	"$(INTDIR)\Ugvscrol.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\UserDB.obj" \
	"$(INTDIR)\UserFlag.obj" \
	"$(INTDIR)\UsrAdmin.obj" \
	"$(INTDIR)\WklDoc.obj" \
	"$(INTDIR)\WklFrame.obj" \
	"$(INTDIR)\WklMods.obj" \
	"$(INTDIR)\WklRead.obj" \
	"$(INTDIR)\WklRef.obj" \
	"$(INTDIR)\WklView.obj" \
	"$(INTDIR)\Lucept.res"

"$(OUTDIR)\Lucept.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Lucept.dep")
!INCLUDE "Lucept.dep"
!ELSE 
!MESSAGE Warning: cannot find "Lucept.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Lucept - Win32 Release" || "$(CFG)" == "Lucept - Win32 Debug" || "$(CFG)" == "Lucept - Win32 MacroDebug"
SOURCE=.\ACCMgr.cpp

"$(INTDIR)\ACCMgr.obj"	"$(INTDIR)\ACCMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ACCtnr.cpp

"$(INTDIR)\ACCtnr.obj"	"$(INTDIR)\ACCtnr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ACCtrl.cpp

"$(INTDIR)\ACCtrl.obj"	"$(INTDIR)\ACCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\acdisp.cpp

"$(INTDIR)\acdisp.obj"	"$(INTDIR)\acdisp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\AddCSamp.cpp

"$(INTDIR)\AddCSamp.obj"	"$(INTDIR)\AddCSamp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Addreag.cpp

"$(INTDIR)\Addreag.obj"	"$(INTDIR)\Addreag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADefDlg.cpp

"$(INTDIR)\ADefDlg.obj"	"$(INTDIR)\ADefDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADPgACal.cpp

"$(INTDIR)\ADPgACal.obj"	"$(INTDIR)\ADPgACal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADPgCal.cpp

"$(INTDIR)\ADPgCal.obj"	"$(INTDIR)\ADPgCal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADPgFlag.cpp

"$(INTDIR)\ADPgFlag.obj"	"$(INTDIR)\ADPgFlag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADPgFld.cpp

"$(INTDIR)\ADPgFld.obj"	"$(INTDIR)\ADPgFld.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADPgMain.cpp

"$(INTDIR)\ADPgMain.obj"	"$(INTDIR)\ADPgMain.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADPgName.cpp

"$(INTDIR)\ADPgName.obj"	"$(INTDIR)\ADPgName.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ADPrpSh.cpp

"$(INTDIR)\ADPrpSh.obj"	"$(INTDIR)\ADPrpSh.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\AssayNew.cpp

"$(INTDIR)\AssayNew.obj"	"$(INTDIR)\AssayNew.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Assays.cpp

"$(INTDIR)\Assays.obj"	"$(INTDIR)\Assays.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\AssaySel.cpp

"$(INTDIR)\AssaySel.obj"	"$(INTDIR)\AssaySel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\AssyDef.cpp

"$(INTDIR)\AssyDef.obj"	"$(INTDIR)\AssyDef.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\AssyPrDc.cpp

"$(INTDIR)\AssyPrDc.obj"	"$(INTDIR)\AssyPrDc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\AssyPrVw.cpp

"$(INTDIR)\AssyPrVw.obj"	"$(INTDIR)\AssyPrVw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\BatCalc.cpp

"$(INTDIR)\BatCalc.obj"	"$(INTDIR)\BatCalc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Batch.cpp

"$(INTDIR)\Batch.obj"	"$(INTDIR)\Batch.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\BatchPg.cpp

"$(INTDIR)\BatchPg.obj"	"$(INTDIR)\BatchPg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\BatRef.cpp

"$(INTDIR)\BatRef.obj"	"$(INTDIR)\BatRef.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\BCFrame.cpp

"$(INTDIR)\BCFrame.obj"	"$(INTDIR)\BCFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\BGridPg.cpp

"$(INTDIR)\BGridPg.obj"	"$(INTDIR)\BGridPg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\BPickDlg.cpp

"$(INTDIR)\BPickDlg.obj"	"$(INTDIR)\BPickDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\BTabCtrl.cpp

"$(INTDIR)\BTabCtrl.obj"	"$(INTDIR)\BTabCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\CalTypeD.cpp

"$(INTDIR)\CalTypeD.obj"	"$(INTDIR)\CalTypeD.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\CondFrm.cpp

"$(INTDIR)\CondFrm.obj"	"$(INTDIR)\CondFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Conduct.cpp

"$(INTDIR)\Conduct.obj"	"$(INTDIR)\Conduct.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\CugRpt.CPP

"$(INTDIR)\CugRpt.obj"	"$(INTDIR)\CugRpt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\CugWkl.CPP

"$(INTDIR)\CugWkl.obj"	"$(INTDIR)\CugWkl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\CustPal.cpp

"$(INTDIR)\CustPal.obj"	"$(INTDIR)\CustPal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\DaoUtils.cpp

"$(INTDIR)\DaoUtils.obj"	"$(INTDIR)\DaoUtils.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\dibapi.cpp

"$(INTDIR)\dibapi.obj"	"$(INTDIR)\dibapi.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\DIBUtil.cpp

"$(INTDIR)\DIBUtil.obj"	"$(INTDIR)\DIBUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Eventsnk.cpp

"$(INTDIR)\Eventsnk.obj"	"$(INTDIR)\Eventsnk.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\EventSnk.odl

!IF  "$(CFG)" == "Lucept - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\EventSnk.tlb" /mktyplib203 /win32 

"$(OUTDIR)\EventSnk.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Lucept - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\EventSnk.tlb" /mktyplib203 /win32 

"$(OUTDIR)\EventSnk.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Lucept - Win32 MacroDebug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\EventSnk.tlb" /mktyplib203 /win32 

"$(OUTDIR)\EventSnk.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\FldProp.cpp

"$(INTDIR)\FldProp.obj"	"$(INTDIR)\FldProp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\GenBox.cpp

"$(INTDIR)\GenBox.obj"	"$(INTDIR)\GenBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\GenDef.cpp

"$(INTDIR)\GenDef.obj"	"$(INTDIR)\GenDef.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Generics.cpp

"$(INTDIR)\Generics.obj"	"$(INTDIR)\Generics.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\GnDfPrDc.cpp

"$(INTDIR)\GnDfPrDc.obj"	"$(INTDIR)\GnDfPrDc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\GnDfPrVw.cpp

"$(INTDIR)\GnDfPrVw.obj"	"$(INTDIR)\GnDfPrVw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\HandCtrl.cpp

"$(INTDIR)\HandCtrl.obj"	"$(INTDIR)\HandCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\hidedlg.cpp

"$(INTDIR)\hidedlg.obj"	"$(INTDIR)\hidedlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\IniFiles.cpp

"$(INTDIR)\IniFiles.obj"	"$(INTDIR)\IniFiles.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\InjSeq.cpp

"$(INTDIR)\InjSeq.obj"	"$(INTDIR)\InjSeq.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LccInq.cpp

"$(INTDIR)\LccInq.obj"	"$(INTDIR)\LccInq.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LccMgr.cpp

"$(INTDIR)\LccMgr.obj"	"$(INTDIR)\LccMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LCCProxy.cpp

"$(INTDIR)\LCCProxy.obj"	"$(INTDIR)\LCCProxy.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LCCUser.cpp

"$(INTDIR)\LCCUser.obj"	"$(INTDIR)\LCCUser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LccVDoc.cpp

"$(INTDIR)\LccVDoc.obj"	"$(INTDIR)\LccVDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LccView.cpp

"$(INTDIR)\LccView.obj"	"$(INTDIR)\LccView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LCDWnd.cpp

"$(INTDIR)\LCDWnd.obj"	"$(INTDIR)\LCDWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LEDDisp.cpp

"$(INTDIR)\LEDDisp.obj"	"$(INTDIR)\LEDDisp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Level.cpp

"$(INTDIR)\Level.obj"	"$(INTDIR)\Level.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LiftWnd.cpp

"$(INTDIR)\LiftWnd.obj"	"$(INTDIR)\LiftWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LogonDlg.cpp

"$(INTDIR)\LogonDlg.obj"	"$(INTDIR)\LogonDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Lucept.cpp

"$(INTDIR)\Lucept.obj"	"$(INTDIR)\Lucept.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\hlp\lucept.hpj

!IF  "$(CFG)" == "Lucept - Win32 Release"

OutDir=.\Release
ProjDir=.
TargetName=Advance
InputPath=.\hlp\lucept.hpj

"$(INTDIR)\Advance.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	$(ProjDir)\makehelp.bat
<< 
	

!ELSEIF  "$(CFG)" == "Lucept - Win32 Debug"

OutDir=.\Debug
ProjDir=.
TargetName=Advance
InputPath=.\hlp\lucept.hpj

"$(INTDIR)\Advance.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	$(ProjDir)\makehelp.bat
<< 
	

!ELSEIF  "$(CFG)" == "Lucept - Win32 MacroDebug"

OutDir=.\Macro
ProjDir=.
TargetName=Lucept
InputPath=.\hlp\lucept.hpj

"$(INTDIR)\Lucept.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	$(ProjDir)\makehelp.bat
<< 
	

!ENDIF 

SOURCE=.\Lucept.rc

!IF  "$(CFG)" == "Lucept - Win32 Release"


"$(INTDIR)\Lucept.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\Lucept.res" /i ".\Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Lucept - Win32 Debug"


"$(INTDIR)\Lucept.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\Lucept.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Lucept - Win32 MacroDebug"


"$(INTDIR)\Lucept.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\Lucept.res" /i ".\Macro" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\LucSamp.cpp

"$(INTDIR)\LucSamp.obj"	"$(INTDIR)\LucSamp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LucyBar.cpp

"$(INTDIR)\LucyBar.obj"	"$(INTDIR)\LucyBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LucyDoc.cpp

"$(INTDIR)\LucyDoc.obj"	"$(INTDIR)\LucyDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LucyFrm.cpp

"$(INTDIR)\LucyFrm.obj"	"$(INTDIR)\LucyFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LucyView.cpp

"$(INTDIR)\LucyView.obj"	"$(INTDIR)\LucyView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\LumActor.cpp

"$(INTDIR)\LumActor.obj"	"$(INTDIR)\LumActor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MDefDlg.cpp

"$(INTDIR)\MDefDlg.obj"	"$(INTDIR)\MDefDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MeasDef.cpp

"$(INTDIR)\MeasDef.obj"	"$(INTDIR)\MeasDef.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MeasNew.cpp

"$(INTDIR)\MeasNew.obj"	"$(INTDIR)\MeasNew.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MeasPrDc.cpp

"$(INTDIR)\MeasPrDc.obj"	"$(INTDIR)\MeasPrDc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MeasPrVw.cpp

"$(INTDIR)\MeasPrVw.obj"	"$(INTDIR)\MeasPrVw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MeasSel.cpp

"$(INTDIR)\MeasSel.obj"	"$(INTDIR)\MeasSel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Measures.cpp

"$(INTDIR)\Measures.obj"	"$(INTDIR)\Measures.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ModulVer.cpp

"$(INTDIR)\ModulVer.obj"	"$(INTDIR)\ModulVer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\MProcDlg.cpp

"$(INTDIR)\MProcDlg.obj"	"$(INTDIR)\MProcDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\PPUAdd.cpp

"$(INTDIR)\PPUAdd.obj"	"$(INTDIR)\PPUAdd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\PPUDel.cpp

"$(INTDIR)\PPUDel.obj"	"$(INTDIR)\PPUDel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\PPUEdit.cpp

"$(INTDIR)\PPUEdit.obj"	"$(INTDIR)\PPUEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RefList.cpp

"$(INTDIR)\RefList.obj"	"$(INTDIR)\RefList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RGridFrm.cpp

"$(INTDIR)\RGridFrm.obj"	"$(INTDIR)\RGridFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptDoc.cpp

"$(INTDIR)\RptDoc.obj"	"$(INTDIR)\RptDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptFInfo.cpp

"$(INTDIR)\RptFInfo.obj"	"$(INTDIR)\RptFInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptFrame.cpp

"$(INTDIR)\RptFrame.obj"	"$(INTDIR)\RptFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptPgCol.cpp

"$(INTDIR)\RptPgCol.obj"	"$(INTDIR)\RptPgCol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptPgFmt.cpp

"$(INTDIR)\RptPgFmt.obj"	"$(INTDIR)\RptPgFmt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptPgFrm.cpp

"$(INTDIR)\RptPgFrm.obj"	"$(INTDIR)\RptPgFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptPgGph.cpp

"$(INTDIR)\RptPgGph.obj"	"$(INTDIR)\RptPgGph.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptPgQry.cpp

"$(INTDIR)\RptPgQry.obj"	"$(INTDIR)\RptPgQry.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptPgSrt.cpp

"$(INTDIR)\RptPgSrt.obj"	"$(INTDIR)\RptPgSrt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptPgWkl.cpp

"$(INTDIR)\RptPgWkl.obj"	"$(INTDIR)\RptPgWkl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptUgDao.cpp

"$(INTDIR)\RptUgDao.obj"	"$(INTDIR)\RptUgDao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\RptView.cpp

"$(INTDIR)\RptView.obj"	"$(INTDIR)\RptView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\SampFrm.cpp

"$(INTDIR)\SampFrm.obj"	"$(INTDIR)\SampFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Sample.cpp

"$(INTDIR)\Sample.obj"	"$(INTDIR)\Sample.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\SampRef.cpp

"$(INTDIR)\SampRef.obj"	"$(INTDIR)\SampRef.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\SampView.cpp

"$(INTDIR)\SampView.obj"	"$(INTDIR)\SampView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ScanPage.cpp

"$(INTDIR)\ScanPage.obj"	"$(INTDIR)\ScanPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ScanSht.cpp

"$(INTDIR)\ScanSht.obj"	"$(INTDIR)\ScanSht.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ScnChain.cpp

"$(INTDIR)\ScnChain.obj"	"$(INTDIR)\ScnChain.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\ScnSumPg.cpp

"$(INTDIR)\ScnSumPg.obj"	"$(INTDIR)\ScnSumPg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\SetupDlg.cpp

"$(INTDIR)\SetupDlg.obj"	"$(INTDIR)\SetupDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\SPickDlg.cpp

"$(INTDIR)\SPickDlg.obj"	"$(INTDIR)\SPickDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Lucept - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Lucept.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Lucept.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Lucept - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Lucept.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Lucept.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Lucept - Win32 MacroDebug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Lucept.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /P /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Lucept.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TempDisp.cpp

"$(INTDIR)\TempDisp.obj"	"$(INTDIR)\TempDisp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\TimeDisp.cpp

"$(INTDIR)\TimeDisp.obj"	"$(INTDIR)\TimeDisp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Tube.cpp

"$(INTDIR)\Tube.obj"	"$(INTDIR)\Tube.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugcbtype.cpp

"$(INTDIR)\Ugcbtype.obj"	"$(INTDIR)\Ugcbtype.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugcell.cpp

"$(INTDIR)\Ugcell.obj"	"$(INTDIR)\Ugcell.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugceltyp.cpp

"$(INTDIR)\Ugceltyp.obj"	"$(INTDIR)\Ugceltyp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugcnrbtn.cpp

"$(INTDIR)\Ugcnrbtn.obj"	"$(INTDIR)\Ugcnrbtn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctafnt.cpp

"$(INTDIR)\Ugctafnt.obj"	"$(INTDIR)\Ugctafnt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctarrw.cpp

"$(INTDIR)\Ugctarrw.obj"	"$(INTDIR)\Ugctarrw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctbutn.cpp

"$(INTDIR)\Ugctbutn.obj"	"$(INTDIR)\Ugctbutn.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctelps.cpp

"$(INTDIR)\Ugctelps.obj"	"$(INTDIR)\Ugctelps.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctpie.cpp

"$(INTDIR)\Ugctpie.obj"	"$(INTDIR)\Ugctpie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctpro1.cpp

"$(INTDIR)\Ugctpro1.obj"	"$(INTDIR)\Ugctpro1.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctprog.cpp

"$(INTDIR)\Ugctprog.obj"	"$(INTDIR)\Ugctprog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctrado.cpp

"$(INTDIR)\Ugctrado.obj"	"$(INTDIR)\Ugctrado.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctrl.cpp

"$(INTDIR)\Ugctrl.obj"	"$(INTDIR)\Ugctrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctsarw.cpp

"$(INTDIR)\Ugctsarw.obj"	"$(INTDIR)\Ugctsarw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugctspin.cpp

"$(INTDIR)\Ugctspin.obj"	"$(INTDIR)\Ugctspin.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugdao.cpp

"$(INTDIR)\Ugdao.obj"	"$(INTDIR)\Ugdao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugdltype.cpp

"$(INTDIR)\Ugdltype.obj"	"$(INTDIR)\Ugdltype.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugdrgdrp.cpp

"$(INTDIR)\Ugdrgdrp.obj"	"$(INTDIR)\Ugdrgdrp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugdrwhnt.cpp

"$(INTDIR)\Ugdrwhnt.obj"	"$(INTDIR)\Ugdrwhnt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugdtasrc.cpp

"$(INTDIR)\Ugdtasrc.obj"	"$(INTDIR)\Ugdtasrc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugedit.cpp

"$(INTDIR)\Ugedit.obj"	"$(INTDIR)\Ugedit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Uggrid.cpp

"$(INTDIR)\Uggrid.obj"	"$(INTDIR)\Uggrid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ughint.cpp

"$(INTDIR)\Ughint.obj"	"$(INTDIR)\Ughint.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ughscrol.cpp

"$(INTDIR)\Ughscrol.obj"	"$(INTDIR)\Ughscrol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Uglstbox.cpp

"$(INTDIR)\Uglstbox.obj"	"$(INTDIR)\Uglstbox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugmemman.cpp

"$(INTDIR)\Ugmemman.obj"	"$(INTDIR)\Ugmemman.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugmultis.cpp

"$(INTDIR)\Ugmultis.obj"	"$(INTDIR)\Ugmultis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugprint.cpp

"$(INTDIR)\Ugprint.obj"	"$(INTDIR)\Ugprint.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugroutin.cpp

"$(INTDIR)\Ugroutin.obj"	"$(INTDIR)\Ugroutin.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugsidehd.cpp

"$(INTDIR)\Ugsidehd.obj"	"$(INTDIR)\Ugsidehd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\UGTab.cpp

"$(INTDIR)\UGTab.obj"	"$(INTDIR)\UGTab.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugtophdg.cpp

"$(INTDIR)\Ugtophdg.obj"	"$(INTDIR)\Ugtophdg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\Ugvscrol.cpp

"$(INTDIR)\Ugvscrol.obj"	"$(INTDIR)\Ugvscrol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\User.cpp

"$(INTDIR)\User.obj"	"$(INTDIR)\User.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\UserDB.cpp

"$(INTDIR)\UserDB.obj"	"$(INTDIR)\UserDB.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\UserFlag.cpp

"$(INTDIR)\UserFlag.obj"	"$(INTDIR)\UserFlag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\UsrAdmin.cpp

"$(INTDIR)\UsrAdmin.obj"	"$(INTDIR)\UsrAdmin.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\WklDoc.cpp

"$(INTDIR)\WklDoc.obj"	"$(INTDIR)\WklDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\WklFrame.cpp

"$(INTDIR)\WklFrame.obj"	"$(INTDIR)\WklFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\WklMods.cpp

"$(INTDIR)\WklMods.obj"	"$(INTDIR)\WklMods.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\WklRead.cpp

"$(INTDIR)\WklRead.obj"	"$(INTDIR)\WklRead.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\WklRef.cpp

"$(INTDIR)\WklRef.obj"	"$(INTDIR)\WklRef.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"


SOURCE=.\WklView.cpp

"$(INTDIR)\WklView.obj"	"$(INTDIR)\WklView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Lucept.pch"



!ENDIF 

