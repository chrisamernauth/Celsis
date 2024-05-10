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

///////////////////////////////////////////////////////////////////////////////
// Application scope external constants
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "IniFiles.h"

///////////////////////////////////////////////////////////////////////////////
//!!!Should search all files for GetProfile* and GetPrivateProfile*
// and use these constants in preference to string literals

///////////////////////////////////////////////////////////////////////////////
// ADVANCE.INI File constants
extern LPCTSTR gIniSec_Paths				= _T("Paths");
extern LPCTSTR gIniEnt_AssayDataPath		= _T("AssayDataPath");
extern LPCTSTR gIniEnt_MeasDataPath			= _T("MeasDataPath");
extern LPCTSTR gIniEnt_ResultDataPath		= _T("ResultDataPath");

extern LPCTSTR gIniSec_ParmDefs				= _T("Parameter Defaults");
extern LPCTSTR gIniEnt_MaxTubesInBatch		= _T("MaxTubesInBatch");
extern LPCTSTR gIniEnt_DefSampleRepls		= _T("DefSampleRepls");
extern LPCTSTR gIniEnt_ReagentMaxMLs		= _T("ReagentMaxMLs");
extern LPCTSTR gIniEnt_AssayCalcID			= _T("AssayCalcID");
extern LPCTSTR gIniEnt_PrimeCycles			= _T("PrimeCycles");

extern LPCTSTR gIniSec_Features				= _T("Features");
extern LPCTSTR gIniEnt_DbgApps				= _T("DebugApps");
extern LPCTSTR gIniEnt_SampStats			= _T("UseSampleStatistics");
extern LPCTSTR gIniEnt_EconoPrime			= _T("EconoPrime");
extern LPCTSTR gIniEnt_PharmaSec			= _T("PharmaSec");
extern LPCTSTR gIniEnt_FontSize				= _T("FontSize");

extern LPCTSTR gIniSec_Labels				= _T("Labels");
extern LPCTSTR gIniEnt_Inj1					= _T("InjName1");
extern LPCTSTR gIniEnt_Inj2					= _T("InjName2");
extern LPCTSTR gIniEnt_Inj3					= _T("InjName3");

extern LPCTSTR gIniSec_Luminometer			= _T("Luminometer");
extern LPCTSTR gIniEnt_Model				= _T("Model");

extern LPCTSTR gIniSec_ReagentLevels		= _T("ReagentLevels");
extern LPCTSTR gIniEnt_LastReagent1Level	= _T("LastReagent1Level");
extern LPCTSTR gIniEnt_LastReagent2Level	= _T("LastReagent2Level");
extern LPCTSTR gIniEnt_LastReagent3Level	= _T("LastReagent3Level");
extern LPCTSTR gIniEnt_LastReagentPLevel	= _T("LastReagentPLevel");

//LCC.INI File constants
extern LPCTSTR gLccIniSec_Settings			= _T("Settings");
extern LPCTSTR gLccIniEnt_ChainLength		= _T("ChainLength");
extern LPCTSTR gLccIniEnt_HiTubes			= _T("HiTubes");
extern LPCTSTR gLccIniEnt_HiPosition		= _T("HiPosition");

//Global Variables based on INI file settings
//!!! might be worth creating a _T("CHardwareLimits") class to contain these
extern long	gMaxRgntVol			= 0;
extern long	gLuminometerModel	= 0;// == MODEL_SEDAN;
extern long	gMaxTubesInBatch	= 0;
extern long	gMaxBatches			= 0;
extern long gMaxSamples			= 0;

extern long	gChainLength		= 0;
extern long gHiTubes			= 0;
extern long	gUseSampleStatistics= 0;
//
////////////////////////////////////////////////////////////////////////////////
