# Microsoft Developer Studio Project File - Name="Lucept" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LUCEPT - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Lucept.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/PFI/lucept", PBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Lucept - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_GXDLL" /D "DO_CGX" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\PhilUtil\Release\PhilUtil.lib ..\Olectra\Release\Olectra.lib winmm.lib htmlhelp.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:".\Release\Advance.exe"

!ELSEIF  "$(CFG)" == "Lucept - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_GXDLL" /D "NO_USER_ADMIN" /D "DO_CGX" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\PhilUtil\Debug\PhilUtil.lib ..\Olectra\Debug\Olectra.lib winmm.lib htmlhelp.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /out:"Debug/Advance.exe"

!ELSEIF  "$(CFG)" == "Lucept - Win32 MacroDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Lucept__"
# PROP BASE Intermediate_Dir "Lucept__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Macro"
# PROP Intermediate_Dir ".\Macro"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /O2 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_GXDLL" /FR /Yu"stdafx.h" /FD /P /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Macro/Lucept.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Lucept - Win32 Release"
# Name "Lucept - Win32 Debug"
# Name "Lucept - Win32 MacroDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ACCMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ACCtnr.cpp
# End Source File
# Begin Source File

SOURCE=.\ACCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\acdisp.cpp
# End Source File
# Begin Source File

SOURCE=.\AddCSamp.cpp
# End Source File
# Begin Source File

SOURCE=.\Addreag.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPgACal.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPgCal.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPgFil.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPgFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPgFld.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPgMain.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPgName.cpp
# End Source File
# Begin Source File

SOURCE=.\ADPrpSh.cpp
# End Source File
# Begin Source File

SOURCE=.\AssayNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Assays.cpp
# End Source File
# Begin Source File

SOURCE=.\AssaySel.cpp
# End Source File
# Begin Source File

SOURCE=.\AssyDef.cpp
# End Source File
# Begin Source File

SOURCE=.\AssyPrDc.cpp
# End Source File
# Begin Source File

SOURCE=.\AssyPrVw.cpp
# End Source File
# Begin Source File

SOURCE=.\BatCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\Batch.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchPg.cpp
# End Source File
# Begin Source File

SOURCE=.\BatRef.cpp
# End Source File
# Begin Source File

SOURCE=.\BCFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\BGridPg.cpp
# End Source File
# Begin Source File

SOURCE=.\BPickDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CalTypeD.cpp
# End Source File
# Begin Source File

SOURCE=.\CondFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Conduct.cpp
# End Source File
# Begin Source File

SOURCE=.\CugRpt.CPP
# End Source File
# Begin Source File

SOURCE=.\CugWkl.CPP
# End Source File
# Begin Source File

SOURCE=.\CustPal.cpp
# End Source File
# Begin Source File

SOURCE=.\DaoUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\dibapi.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\Eventsnk.cpp
# End Source File
# Begin Source File

SOURCE=.\EventSnk.odl
# End Source File
# Begin Source File

SOURCE=.\FldProp.cpp
# End Source File
# Begin Source File

SOURCE=.\GenBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GenDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Generics.cpp
# End Source File
# Begin Source File

SOURCE=.\GnDfPrDc.cpp
# End Source File
# Begin Source File

SOURCE=.\GnDfPrVw.cpp
# End Source File
# Begin Source File

SOURCE=.\GXBatchGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\HandCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\hidedlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\InjSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\LccInq.cpp
# End Source File
# Begin Source File

SOURCE=.\LccMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\LCCProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\LCCUser.cpp
# End Source File
# Begin Source File

SOURCE=.\LccVDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LccView.cpp
# End Source File
# Begin Source File

SOURCE=.\LCDWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LEDDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\Level.cpp
# End Source File
# Begin Source File

SOURCE=.\LiftWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LogonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Lucept.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\lucept.hpj

!IF  "$(CFG)" == "Lucept - Win32 Release"

# Begin Custom Build
OutDir=.\Release
ProjDir=.
TargetName=Advance
InputPath=.\hlp\lucept.hpj

".\$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(ProjDir)\makehelp.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "Lucept - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
ProjDir=.
TargetName=Advance
InputPath=.\hlp\lucept.hpj

".\$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(ProjDir)\makehelp.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "Lucept - Win32 MacroDebug"

# Begin Custom Build
OutDir=.\Macro
ProjDir=.
TargetName=Lucept
InputPath=.\hlp\lucept.hpj

".\$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(ProjDir)\makehelp.bat

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Lucept.rc
# End Source File
# Begin Source File

SOURCE=.\LucSamp.cpp
# End Source File
# Begin Source File

SOURCE=.\LucyBar.cpp
# End Source File
# Begin Source File

SOURCE=.\LucyDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LucyFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\LucyView.cpp
# End Source File
# Begin Source File

SOURCE=.\LumActor.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MDefDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasDef.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasNew.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasPrDc.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasPrVw.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasSel.cpp
# End Source File
# Begin Source File

SOURCE=.\Measures.cpp
# End Source File
# Begin Source File

SOURCE=.\MProcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswordChange.cpp
# End Source File
# Begin Source File

SOURCE=.\PPUAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\PPUEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RefList.cpp
# End Source File
# Begin Source File

SOURCE=.\RGridFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\RptDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RptFInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RptFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\RptPgCol.cpp
# End Source File
# Begin Source File

SOURCE=.\RptPgFmt.cpp
# End Source File
# Begin Source File

SOURCE=.\RptPgFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\RptPgGph.cpp
# End Source File
# Begin Source File

SOURCE=.\RptPgQry.cpp
# End Source File
# Begin Source File

SOURCE=.\RptPgSrt.cpp
# End Source File
# Begin Source File

SOURCE=.\RptPgWkl.cpp
# End Source File
# Begin Source File

SOURCE=.\RptUgDao.cpp
# End Source File
# Begin Source File

SOURCE=.\RptView.cpp
# End Source File
# Begin Source File

SOURCE=.\SampFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Sample.cpp
# End Source File
# Begin Source File

SOURCE=.\SampRef.cpp
# End Source File
# Begin Source File

SOURCE=.\SampView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanSht.cpp
# End Source File
# Begin Source File

SOURCE=.\ScnChain.cpp
# End Source File
# Begin Source File

SOURCE=.\ScnSumPg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SPickDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysLogView.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemLog.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemLogFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\TempDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\Tube.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugcbtype.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugcell.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugceltyp.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugcnrbtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctafnt.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctarrw.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctbutn.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctelps.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctpie.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctpro1.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctprog.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctrado.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctsarw.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugctspin.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugdao.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugdltype.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugdrgdrp.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugdrwhnt.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugdtasrc.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugedit.cpp
# End Source File
# Begin Source File

SOURCE=.\Uggrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Ughint.cpp
# End Source File
# Begin Source File

SOURCE=.\Ughscrol.cpp
# End Source File
# Begin Source File

SOURCE=.\Uglstbox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugmemman.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugmultis.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugprint.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugroutin.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugsidehd.cpp
# End Source File
# Begin Source File

SOURCE=.\UGTab.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugtophdg.cpp
# End Source File
# Begin Source File

SOURCE=.\Ugvscrol.cpp
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\UserDB.cpp
# End Source File
# Begin Source File

SOURCE=.\UserFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\UsrAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\WklDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\WklFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\WklMods.cpp
# End Source File
# Begin Source File

SOURCE=.\WklRead.cpp
# End Source File
# Begin Source File

SOURCE=.\WklRef.cpp
# End Source File
# Begin Source File

SOURCE=.\WklView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ACCMgr.h
# End Source File
# Begin Source File

SOURCE=.\ACCtnr.h
# End Source File
# Begin Source File

SOURCE=.\acctrl.h
# End Source File
# Begin Source File

SOURCE=.\acdisp.h
# End Source File
# Begin Source File

SOURCE=.\AddCSamp.h
# End Source File
# Begin Source File

SOURCE=.\Addreag.h
# End Source File
# Begin Source File

SOURCE=.\ADPgACal.h
# End Source File
# Begin Source File

SOURCE=.\ADPgCal.h
# End Source File
# Begin Source File

SOURCE=.\ADPgFil.h
# End Source File
# Begin Source File

SOURCE=.\ADPgFlag.h
# End Source File
# Begin Source File

SOURCE=.\ADPgFld.h
# End Source File
# Begin Source File

SOURCE=.\ADPgMain.h
# End Source File
# Begin Source File

SOURCE=.\ADPgName.h
# End Source File
# Begin Source File

SOURCE=.\ADPrpSh.h
# End Source File
# Begin Source File

SOURCE=.\AssayNew.h
# End Source File
# Begin Source File

SOURCE=.\Assays.h
# End Source File
# Begin Source File

SOURCE=.\AssaySel.h
# End Source File
# Begin Source File

SOURCE=.\AssyDef.h
# End Source File
# Begin Source File

SOURCE=.\AssyPrDc.h
# End Source File
# Begin Source File

SOURCE=.\AssyPrVw.h
# End Source File
# Begin Source File

SOURCE=.\BatCalc.h
# End Source File
# Begin Source File

SOURCE=.\Batch.h
# End Source File
# Begin Source File

SOURCE=.\BatchPg.h
# End Source File
# Begin Source File

SOURCE=.\BatRef.h
# End Source File
# Begin Source File

SOURCE=.\BCFrame.h
# End Source File
# Begin Source File

SOURCE=.\BGridPg.h
# End Source File
# Begin Source File

SOURCE=.\BPickDlg.h
# End Source File
# Begin Source File

SOURCE=.\BTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CalTypeD.h
# End Source File
# Begin Source File

SOURCE=.\chart2d.h
# End Source File
# Begin Source File

SOURCE=.\CondFrm.h
# End Source File
# Begin Source File

SOURCE=.\Conduct.h
# End Source File
# Begin Source File

SOURCE=.\CugRpt.h
# End Source File
# Begin Source File

SOURCE=.\CugWkl.h
# End Source File
# Begin Source File

SOURCE=.\CustPal.h
# End Source File
# Begin Source File

SOURCE=.\DaoUtils.h
# End Source File
# Begin Source File

SOURCE=.\dibapi.h
# End Source File
# Begin Source File

SOURCE=.\Event.h
# End Source File
# Begin Source File

SOURCE=.\Eventsnk.h
# End Source File
# Begin Source File

SOURCE=.\FldEnum.h
# End Source File
# Begin Source File

SOURCE=.\FldProp.h
# End Source File
# Begin Source File

SOURCE=.\GenBox.h
# End Source File
# Begin Source File

SOURCE=.\GenDef.h
# End Source File
# Begin Source File

SOURCE=.\Generics.h
# End Source File
# Begin Source File

SOURCE=.\GnDfPrDc.h
# End Source File
# Begin Source File

SOURCE=.\GnDfPrVw.h
# End Source File
# Begin Source File

SOURCE=.\GXBatchGrid.h
# End Source File
# Begin Source File

SOURCE=.\HandCtrl.h
# End Source File
# Begin Source File

SOURCE=.\hidedlg.h
# End Source File
# Begin Source File

SOURCE=.\IniFiles.h
# End Source File
# Begin Source File

SOURCE=.\InjSeq.h
# End Source File
# Begin Source File

SOURCE=.\LCCDef.h
# End Source File
# Begin Source File

SOURCE=.\LccInq.h
# End Source File
# Begin Source File

SOURCE=.\LccMgr.h
# End Source File
# Begin Source File

SOURCE=.\lccproxy.h
# End Source File
# Begin Source File

SOURCE=.\LCCUser.h
# End Source File
# Begin Source File

SOURCE=.\LccVDoc.h
# End Source File
# Begin Source File

SOURCE=.\LccView.h
# End Source File
# Begin Source File

SOURCE=.\LCDWnd.h
# End Source File
# Begin Source File

SOURCE=.\LEDDisp.h
# End Source File
# Begin Source File

SOURCE=.\Level.h
# End Source File
# Begin Source File

SOURCE=.\LiftWnd.h
# End Source File
# Begin Source File

SOURCE=.\LogonDlg.h
# End Source File
# Begin Source File

SOURCE=.\LucColor.h
# End Source File
# Begin Source File

SOURCE=.\LucDef.h
# End Source File
# Begin Source File

SOURCE=.\LuceErr.h
# End Source File
# Begin Source File

SOURCE=.\Lucept.h
# End Source File
# Begin Source File

SOURCE=.\LucOle.h
# End Source File
# Begin Source File

SOURCE=.\LucSamp.h
# End Source File
# Begin Source File

SOURCE=.\Lucy.h
# End Source File
# Begin Source File

SOURCE=.\LucyBar.h
# End Source File
# Begin Source File

SOURCE=.\Lucydef.h
# End Source File
# Begin Source File

SOURCE=.\LucyDoc.h
# End Source File
# Begin Source File

SOURCE=.\LucyFrm.h
# End Source File
# Begin Source File

SOURCE=.\LucyView.h
# End Source File
# Begin Source File

SOURCE=.\LumActor.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MDefDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasDef.h
# End Source File
# Begin Source File

SOURCE=.\MeasNew.h
# End Source File
# Begin Source File

SOURCE=.\MeasPrDc.h
# End Source File
# Begin Source File

SOURCE=.\MeasPrVw.h
# End Source File
# Begin Source File

SOURCE=.\MeasSel.h
# End Source File
# Begin Source File

SOURCE=.\Measures.h
# End Source File
# Begin Source File

SOURCE=.\MProcDlg.h
# End Source File
# Begin Source File

SOURCE=.\PasswordChange.h
# End Source File
# Begin Source File

SOURCE=.\PPUAdd.h
# End Source File
# Begin Source File

SOURCE=.\PPUEdit.h
# End Source File
# Begin Source File

SOURCE=.\RefList.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RGridFrm.h
# End Source File
# Begin Source File

SOURCE=.\RptDoc.h
# End Source File
# Begin Source File

SOURCE=.\RptFInfo.h
# End Source File
# Begin Source File

SOURCE=.\RptFrame.h
# End Source File
# Begin Source File

SOURCE=.\RptPgCol.h
# End Source File
# Begin Source File

SOURCE=.\RptPgFmt.h
# End Source File
# Begin Source File

SOURCE=.\RptPgFrm.h
# End Source File
# Begin Source File

SOURCE=.\RptPgGph.h
# End Source File
# Begin Source File

SOURCE=.\RptPgQry.h
# End Source File
# Begin Source File

SOURCE=.\RptPgSrt.h
# End Source File
# Begin Source File

SOURCE=.\RptPgWkl.h
# End Source File
# Begin Source File

SOURCE=.\RptUgDao.h
# End Source File
# Begin Source File

SOURCE=.\RptView.h
# End Source File
# Begin Source File

SOURCE=.\SampFrm.h
# End Source File
# Begin Source File

SOURCE=.\Sample.h
# End Source File
# Begin Source File

SOURCE=.\SampRef.h
# End Source File
# Begin Source File

SOURCE=.\SampView.h
# End Source File
# Begin Source File

SOURCE=.\ScanPage.h
# End Source File
# Begin Source File

SOURCE=.\ScanSht.h
# End Source File
# Begin Source File

SOURCE=.\ScnChain.h
# End Source File
# Begin Source File

SOURCE=.\ScnSumPg.h
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\SPickDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysLogView.h
# End Source File
# Begin Source File

SOURCE=.\SystemLog.h
# End Source File
# Begin Source File

SOURCE=.\SystemLogFilter.h
# End Source File
# Begin Source File

SOURCE=.\TempDisp.h
# End Source File
# Begin Source File

SOURCE=.\TimeDisp.h
# End Source File
# Begin Source File

SOURCE=.\Tube.h
# End Source File
# Begin Source File

SOURCE=.\TubeMap.h
# End Source File
# Begin Source File

SOURCE=.\Ugcbtype.h
# End Source File
# Begin Source File

SOURCE=.\Ugcell.h
# End Source File
# Begin Source File

SOURCE=.\Ugceltyp.h
# End Source File
# Begin Source File

SOURCE=.\Ugcnrbtn.h
# End Source File
# Begin Source File

SOURCE=.\Ugctafnt.h
# End Source File
# Begin Source File

SOURCE=.\Ugctarrw.h
# End Source File
# Begin Source File

SOURCE=.\Ugctbutn.h
# End Source File
# Begin Source File

SOURCE=.\Ugctelps.h
# End Source File
# Begin Source File

SOURCE=.\Ugctpie.h
# End Source File
# Begin Source File

SOURCE=.\Ugctpro1.h
# End Source File
# Begin Source File

SOURCE=.\Ugctprog.h
# End Source File
# Begin Source File

SOURCE=.\Ugctrado.h
# End Source File
# Begin Source File

SOURCE=.\Ugctrl.h
# End Source File
# Begin Source File

SOURCE=.\Ugctsarw.h
# End Source File
# Begin Source File

SOURCE=.\Ugctspin.h
# End Source File
# Begin Source File

SOURCE=.\Ugdao.h
# End Source File
# Begin Source File

SOURCE=.\Ugdefine.h
# End Source File
# Begin Source File

SOURCE=.\Ugdltype.h
# End Source File
# Begin Source File

SOURCE=.\Ugdrgdrp.h
# End Source File
# Begin Source File

SOURCE=.\Ugdrwhnt.h
# End Source File
# Begin Source File

SOURCE=.\Ugdtasrc.h
# End Source File
# Begin Source File

SOURCE=.\Ugedit.h
# End Source File
# Begin Source File

SOURCE=.\Uggrid.h
# End Source File
# Begin Source File

SOURCE=.\Ughint.h
# End Source File
# Begin Source File

SOURCE=.\Ughscrol.h
# End Source File
# Begin Source File

SOURCE=.\Uglstbox.h
# End Source File
# Begin Source File

SOURCE=.\Ugmemman.h
# End Source File
# Begin Source File

SOURCE=.\Ugmultis.h
# End Source File
# Begin Source File

SOURCE=.\Ugprint.h
# End Source File
# Begin Source File

SOURCE=.\Ugsidehd.h
# End Source File
# Begin Source File

SOURCE=.\Ugstruct.h
# End Source File
# Begin Source File

SOURCE=.\UGTab.h
# End Source File
# Begin Source File

SOURCE=.\Ugtophdg.h
# End Source File
# Begin Source File

SOURCE=.\Ugvscrol.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\UserDB.h
# End Source File
# Begin Source File

SOURCE=.\UserFlag.h
# End Source File
# Begin Source File

SOURCE=.\UserPriv.h
# End Source File
# Begin Source File

SOURCE=.\UsrAdmin.h
# End Source File
# Begin Source File

SOURCE=.\WklDef.h
# End Source File
# Begin Source File

SOURCE=.\WklDoc.h
# End Source File
# Begin Source File

SOURCE=.\WklFrame.h
# End Source File
# Begin Source File

SOURCE=.\WklMods.h
# End Source File
# Begin Source File

SOURCE=.\WklRead.h
# End Source File
# Begin Source File

SOURCE=.\WklRef.h
# End Source File
# Begin Source File

SOURCE=.\wklview.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp60001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\busyd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\busyu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chain.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graph_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graph_f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graph_x.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graphvie.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grid_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grid_f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grid_u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grid_x.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gridview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\injectd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\injectf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\injectu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\injectx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lift.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logoutd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logoutf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logoutu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logoutx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Lucept.ico
# End Source File
# Begin Source File

SOURCE=.\res\Lucept.rc2
# End Source File
# Begin Source File

SOURCE=.\res\lumin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\maintend.bmp
# End Source File
# Begin Source File

SOURCE=.\res\maintenf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\maintenu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\maintenx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\manualmd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\manualmf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\manualmu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\manualmx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\newrkldd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\newrkldf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\newrkldu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\newrkldx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextbatc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextsamp.bmp
# End Source File
# Begin Source File

SOURCE=.\Null.cur
# End Source File
# Begin Source File

SOURCE=.\res\offline.bmp
# End Source File
# Begin Source File

SOURCE=.\res\offlined.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prevbatc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prevbath.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prevsamp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\readyd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\readyu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\report.bmp
# End Source File
# Begin Source File

SOURCE=.\res\report.ico
# End Source File
# Begin Source File

SOURCE=.\res\scanchai.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stoprun_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workload.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workload.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\Advance.ini
# End Source File
# End Target
# End Project
# Section Lucept : {8847CF32-2610-11CF-A640-9986B64D9618}
# 	2:5:Class:CDerivedLong
# 	2:10:HeaderFile:derivedlong.h
# 	2:8:ImplFile:derivedlong.cpp
# End Section
# Section Lucept : {FC2C9F26-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CSymbolStyle
# 	2:10:HeaderFile:symbolstyle.h
# 	2:8:ImplFile:symbolstyle.cpp
# End Section
# Section Lucept : {05478A94-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDataIndex
# 	2:10:HeaderFile:dataindex.h
# 	2:8:ImplFile:dataindex.cpp
# End Section
# Section Lucept : {D86FB464-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartGroupCollection
# 	2:10:HeaderFile:chartgroupcollection.h
# 	2:8:ImplFile:chartgroupcollection.cpp
# End Section
# Section Lucept : {245631F2-F6C4-11CF-811C-0020AF19EE14}
# 	2:5:Class:CCandle
# 	2:10:HeaderFile:candle.h
# 	2:8:ImplFile:candle.cpp
# End Section
# Section Lucept : {DB21314D-FC3B-11D0-A196-444553540000}
# 	2:21:DefaultSinkHeaderFile:acdisp.h
# 	2:16:DefaultSinkClass:CAssyCalc
# End Section
# Section Lucept : {D86FB468-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartStyleCollection
# 	2:10:HeaderFile:chartstylecollection.h
# 	2:8:ImplFile:chartstylecollection.cpp
# End Section
# Section Lucept : {5DF9BF61-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CDerivedBoolean
# 	2:10:HeaderFile:derivedboolean.h
# 	2:8:ImplFile:derivedboolean.cpp
# End Section
# Section Lucept : {5DF9BF6D-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CCurrencyVF
# 	2:10:HeaderFile:currencyvf.h
# 	2:8:ImplFile:currencyvf.cpp
# End Section
# Section Lucept : {92D71E9C-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CLabel
# 	2:10:HeaderFile:label.h
# 	2:8:ImplFile:label.cpp
# End Section
# Section Lucept : {05478A98-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CMarkerCollection
# 	2:10:HeaderFile:markercollection.h
# 	2:8:ImplFile:markercollection.cpp
# End Section
# Section Lucept : {FC2C9F20-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDerivedDouble
# 	2:10:HeaderFile:deriveddouble.h
# 	2:8:ImplFile:deriveddouble.cpp
# End Section
# Section Lucept : {FC2C9F10-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartArea
# 	2:10:HeaderFile:chartarea.h
# 	2:8:ImplFile:chartarea.cpp
# End Section
# Section Lucept : {5DF9BF71-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CPercentageVF
# 	2:10:HeaderFile:percentagevf.h
# 	2:8:ImplFile:percentagevf.cpp
# End Section
# Section Lucept : {7BF6C920-333E-11D0-8126-0020AF19EE14}
# 	2:5:Class:CPolar
# 	2:10:HeaderFile:polar.h
# 	2:8:ImplFile:polar.cpp
# End Section
# Section Lucept : {FC2C9F1C-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CTimeScale
# 	2:10:HeaderFile:timescale.h
# 	2:8:ImplFile:timescale.cpp
# End Section
# Section Lucept : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section Lucept : {F4CC7DE2-9AE0-11CF-B9E2-00A024169263}
# 	2:5:Class:CActionMapCollection
# 	2:10:HeaderFile:actionmapcollection.h
# 	2:8:ImplFile:actionmapcollection.cpp
# End Section
# Section Lucept : {D86FB46A-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CLabelCollection
# 	2:10:HeaderFile:labelcollection.h
# 	2:8:ImplFile:labelcollection.cpp
# End Section
# Section Lucept : {5DF9BF75-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CScientificVF
# 	2:10:HeaderFile:scientificvf.h
# 	2:8:ImplFile:scientificvf.cpp
# End Section
# Section Lucept : {DB213195-FC3B-11D0-A196-444553540000}
# 	2:21:DefaultSinkHeaderFile:assylib.h
# 	2:16:DefaultSinkClass:CAssyLib
# End Section
# Section Lucept : {C23B5F18-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DCheckNewPointsResult
# 	2:10:HeaderFile:chart2dchecknewpointsresult.h
# 	2:8:ImplFile:chart2dchecknewpointsresult.cpp
# End Section
# Section Lucept : {05478A92-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDataCoord
# 	2:10:HeaderFile:datacoord.h
# 	2:8:ImplFile:datacoord.cpp
# End Section
# Section Lucept : {D86FB46E-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CValueLabelCollection
# 	2:10:HeaderFile:valuelabelcollection.h
# 	2:8:ImplFile:valuelabelcollection.cpp
# End Section
# Section Lucept : {FC2C9F14-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CView3D
# 	2:10:HeaderFile:view3d.h
# 	2:8:ImplFile:view3d.cpp
# End Section
# Section Lucept : {C23B5F13-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DDataCoordResult
# 	2:10:HeaderFile:chart2ddatacoordresult.h
# 	2:8:ImplFile:chart2ddatacoordresult.cpp
# End Section
# Section Lucept : {FC2C9F24-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CLineStyle
# 	2:10:HeaderFile:linestyle.h
# 	2:8:ImplFile:linestyle.cpp
# End Section
# Section Lucept : {8847CF30-2610-11CF-A640-9986B64D9618}
# 	2:5:Class:CInterior
# 	2:10:HeaderFile:interior.h
# 	2:8:ImplFile:interior.cpp
# End Section
# Section Lucept : {92D71E98-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CBorder
# 	2:10:HeaderFile:border.h
# 	2:8:ImplFile:border.cpp
# End Section
# Section Lucept : {D86FB472-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartLabelCollection
# 	2:10:HeaderFile:chartlabelcollection.h
# 	2:8:ImplFile:chartlabelcollection.cpp
# End Section
# Section Lucept : {245631F0-F6C4-11CF-811C-0020AF19EE14}
# 	2:5:Class:CHiLoOpenClose
# 	2:10:HeaderFile:hiloopenclose.h
# 	2:8:ImplFile:hiloopenclose.cpp
# End Section
# Section Lucept : {D86FB462-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartGroup
# 	2:10:HeaderFile:chartgroup.h
# 	2:8:ImplFile:chartgroup.cpp
# End Section
# Section Lucept : {92D71E93-25A8-11CF-A640-9986B64D9618}
# 	2:21:DefaultSinkHeaderFile:chart2d.h
# 	2:16:DefaultSinkClass:CChart2D1
# End Section
# Section Lucept : {FC2C9F18-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CFillStyle
# 	2:10:HeaderFile:fillstyle.h
# 	2:8:ImplFile:fillstyle.cpp
# End Section
# Section Lucept : {8847CF34-2610-11CF-A640-9986B64D9618}
# 	2:5:Class:CLocation
# 	2:10:HeaderFile:location.h
# 	2:8:ImplFile:location.cpp
# End Section
# Section Lucept : {FC2C9F28-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CGridLines
# 	2:10:HeaderFile:gridlines.h
# 	2:8:ImplFile:gridlines.cpp
# End Section
# Section Lucept : {5DF9BF79-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CValueFormat
# 	2:10:HeaderFile:valueformat.h
# 	2:8:ImplFile:valueformat.cpp
# End Section
# Section Lucept : {2BB6A8B0-268F-11CF-A641-E996DF2C9318}
# 	2:5:Class:CLegend
# 	2:10:HeaderFile:legend.h
# 	2:8:ImplFile:legend.cpp
# End Section
# Section Lucept : {5DF9BF6B-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CNumberVF
# 	2:10:HeaderFile:numbervf.h
# 	2:8:ImplFile:numbervf.cpp
# End Section
# Section Lucept : {D86FB466-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartStyle
# 	2:10:HeaderFile:chartstyle.h
# 	2:8:ImplFile:chartstyle.cpp
# End Section
# Section Lucept : {05478A96-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CCoord
# 	2:10:HeaderFile:coord.h
# 	2:8:ImplFile:coord.cpp
# End Section
# Section Lucept : {92D71E9A-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CTitle
# 	2:10:HeaderFile:title.h
# 	2:8:ImplFile:title.cpp
# End Section
# Section Lucept : {FC2C9F2A-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CAxisCollection
# 	2:10:HeaderFile:axiscollection.h
# 	2:8:ImplFile:axiscollection.cpp
# End Section
# Section Lucept : {FC2C9F1A-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CPlotArea
# 	2:10:HeaderFile:plotarea.h
# 	2:8:ImplFile:plotarea.cpp
# End Section
# Section Lucept : {86E43510-1D29-11D0-B3FB-0020AFD59EF6}
# 	2:5:Class:CStringCollection
# 	2:10:HeaderFile:stringcollection.h
# 	2:8:ImplFile:stringcollection.cpp
# End Section
# Section Lucept : {8027AB63-85DC-11D0-B4AF-0020AFD59EF6}
# 	2:5:Class:CDerivedEnum
# 	2:10:HeaderFile:derivedenum.h
# 	2:8:ImplFile:derivedenum.cpp
# End Section
# Section Lucept : {FC2C9F1E-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDerivedString
# 	2:10:HeaderFile:derivedstring.h
# 	2:8:ImplFile:derivedstring.cpp
# End Section
# Section Lucept : {5DF9BF73-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CFractionVF
# 	2:10:HeaderFile:fractionvf.h
# 	2:8:ImplFile:fractionvf.cpp
# End Section
# Section Lucept : {5DF9BF6F-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CAccountingVF
# 	2:10:HeaderFile:accountingvf.h
# 	2:8:ImplFile:accountingvf.cpp
# End Section
# Section Lucept : {F4CC7DE0-9AE0-11CF-B9E2-00A024169263}
# 	2:5:Class:CActionMap
# 	2:10:HeaderFile:actionmap.h
# 	2:8:ImplFile:actionmap.cpp
# End Section
# Section Lucept : {05478A90-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CMarker
# 	2:10:HeaderFile:marker.h
# 	2:8:ImplFile:marker.cpp
# End Section
# Section Lucept : {FC2C9F22-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CAxis
# 	2:10:HeaderFile:axis.h
# 	2:8:ImplFile:axis.cpp
# End Section
# Section Lucept : {C23B5F11-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DCoordResult
# 	2:10:HeaderFile:chart2dcoordresult.h
# 	2:8:ImplFile:chart2dcoordresult.cpp
# End Section
# Section Lucept : {FC2C9F12-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CBar
# 	2:10:HeaderFile:bar.h
# 	2:8:ImplFile:bar.cpp
# End Section
# Section Lucept : {DB213193-FC3B-11D0-A196-444553540000}
# 	2:5:Class:CAssyLib
# 	2:10:HeaderFile:assylib.h
# 	2:8:ImplFile:assylib.cpp
# End Section
# Section Lucept : {84611366-05BC-11D1-A196-444553540000}
# 	2:5:Class:CAssyCalc
# 	2:10:HeaderFile:assycalc.h
# 	2:8:ImplFile:assycalc.cpp
# End Section
# Section Lucept : {D86FB460-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChart2DData
# 	2:10:HeaderFile:chart2ddata.h
# 	2:8:ImplFile:chart2ddata.cpp
# End Section
# Section Lucept : {D86FB470-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartLabel
# 	2:10:HeaderFile:chartlabel.h
# 	2:8:ImplFile:chartlabel.cpp
# End Section
# Section Lucept : {D86FB46C-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CValueLabel
# 	2:10:HeaderFile:valuelabel.h
# 	2:8:ImplFile:valuelabel.cpp
# End Section
# Section Lucept : {FC2C9F16-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CPie
# 	2:10:HeaderFile:pie.h
# 	2:8:ImplFile:pie.cpp
# End Section
# Section Lucept : {6E5B3FD4-4C5D-11D0-813A-0020AF19EE14}
# 	2:5:Class:CImage
# 	2:10:HeaderFile:image.h
# 	2:8:ImplFile:image.cpp
# End Section
# Section Lucept : {92D71E91-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CChart2D
# 	2:10:HeaderFile:chart2d.h
# 	2:8:ImplFile:chart2d.cpp
# End Section
# Section Lucept : {C23B5F15-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DDataIndexResult
# 	2:10:HeaderFile:chart2ddataindexresult.h
# 	2:8:ImplFile:chart2ddataindexresult.cpp
# End Section
