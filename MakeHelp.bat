@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by LUCEPT.HPJ. >"hlp\LUCEPT.hm"
echo. >>"hlp\LUCEPT.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\LUCEPT.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\LUCEPT.hm"
echo. >>"hlp\LUCEPT.hm"
echo // Prompts (IDP_*) >>"hlp\LUCEPT.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\LUCEPT.hm"
echo. >>"hlp\LUCEPT.hm"
echo // Resources (IDR_*) >>"hlp\LUCEPT.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\LUCEPT.hm"
echo. >>"hlp\LUCEPT.hm"
echo // Dialogs (IDD_*) >>"hlp\LUCEPT.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\LUCEPT.hm"
echo. >>"hlp\LUCEPT.hm"
echo // Frame Controls (IDW_*) >>"hlp\LUCEPT.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\LUCEPT.hm"
REM -- Make help for Project LUCEPT


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\LUCEPT.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\ADVANCE.HLP" goto :Error
if not exist "hlp\LUCEPT.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\ADVANCE.HLP" Debug
if exist Debug\nul copy "hlp\LUCEPT.cnt" Debug
if exist Release\nul copy "hlp\ADVANCE.HLP" Release
if exist Release\nul copy "hlp\LUCEPT.cnt" Release
echo.
goto :done

:Error
echo hlp\LUCEPT.hpj(1) : error: Problem encountered creating help file

:done
echo.
