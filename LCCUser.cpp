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

// LccUser.cpp: implementation of the CLccUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LccUser.h"
#include "LuceErr.h"
#include "Lucept.h"
#include "LccDef.h" 
#include "LccMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLccUser::CLccUser()
{
	m_lastAsyncError = KO_OK;
}

CLccUser::~CLccUser()
{
	if (GetLccMgr() != NULL)
	{
		VERIFY(GetLccMgr()->UnregisterUser(this) == KO_OK);
	}
}


void CLccUser::CSStatusHandler(long CSStatus, long ClassID, long ProcedureID)
{
	// Don't use this notification to process end of control session
	// Use the CSDoneHandler to avoid circular function calls
	// caused by end of control session changing status initiating
	// end of control session.
}

void CLccUser::LumConnectHandler(bool connected)
{
}

void  CLccUser::LccMsgHandler(CString msg)
{
}

void CLccUser::AsyncLccExceptionHandler(long exception)
{
	m_lastAsyncError = exception;
}

void CLccUser::NewReadDataHandler(int tubeIndex, double RLU, double bkgnd, 
	long flags)
{
}

long CLccUser::ReqReleaseControl()
{
	// CLccUser possessing control may release or not. 
	// If it chooses to release, it is responsible for closing
	// any active control sessions. 
	// If it refuses to release, return KO_LUC_REFUSE_RELEASE
	// If it can release immediately, return KO_OK
	// If it must finish a lengthy shutdown process, but then will
	// release, return KO_LUC_RELEASE_PENDING, and call 
	// RelinquishControl() when done.

	// Default LccUser is greedy
	return KO_LUC_REFUSE_RELEASE;
}

void CLccUser::LccAvailableHandler()
{
}

////////////////////////////////////////////////////////////////////////
// LccMgr funciton call wrappers
////////////////////////////////////////////////////////////////////////

long CLccUser::RegisterWithLcc()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}
	else
	{
		return GetLccMgr()->RegisterUser(this);
	}
}

long CLccUser::UnregisterWithLcc()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->UnregisterUser(this);
}


long CLccUser::RequestControl()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->RequestControl(this);
}


long CLccUser::RelinquishControl()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	if (GetLccMgr()->IsController(this))
	{
		return GetLccMgr()->RelinquishControl(this);
	}
	else
	{	// Already not the controller
		return KO_OK;
	}
}


long CLccUser::CSOpen(long classID, long procID)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSOpen(this, classID, procID);
}


CLccMgr* CLccUser::GetLccMgr()
{ 
	return ((CLuceptApp*)AfxGetApp())->GetLccMgr(); 
}


bool CLccUser::IsController()
{
	if (GetLccMgr() == NULL)
	{
		return false;
	}

	return GetLccMgr()->IsController(this);
}

long CLccUser::CSStart()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSStart(this);
}

long CLccUser::CSClose()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSClose(this);
}

long CLccUser::CSSetParam(CParam & param)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

#ifdef DEBUG
	// This a time consuming assertion so avoid it at runtime
	if (GetLccMgr()->CSGetStatus() != LCC_CS_OPENED)
	{
		return KO_LUC_CS_NOT_OPEN;
	}
#endif	

	return GetLccMgr()->CSSetParam(this, param.m_ID, param.m_value);
}

long CLccUser::CSStop(long stopMode)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSStop(this, stopMode);
}

long CLccUser::CSGetStatus()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSGetStatus();
}

long CLccUser::Inquire(long inquireID, double* value)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->Inquire(inquireID, value);
}

long CLccUser::Inquire(long inquireID, double *inVal, CString &outStr)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->Inquire(inquireID, inVal, outStr);
}

long CLccUser::CSGetReagent(long injectorID, long* pVolume)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSGetReagent(injectorID, pVolume);
}

long CLccUser::CSGetTimeLeft(long* pMilliseconds)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSGetTimeLeft(pMilliseconds);
}

long CLccUser::CSGetData(long iArray, long iElement, double * pVal)
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->CSGetData(iArray, iElement, pVal);
}

void CLccUser::TickHandler()
{
	// By default, do nothing
}


// Any overriding functions should call this base implementation
// after processing to post error message and clear logged error.
// or at least clear the error themselves.
void CLccUser::CSDoneHandler(long CSStatus)
{
	if (m_lastAsyncError != KO_OK)
	{
		CString errMsg;

		if (CSStatus != LCC_CS_FAULTED)
		{	// LCC not reporting errors properly
			errMsg.Format(IDS_LCC_ASYNC_NOT_FAULTED,m_lastAsyncError, CSStatus);
			AfxMessageBox(errMsg);
		}

		switch (m_lastAsyncError)
		{
		case KO_LUM_ERR_DOOR_OPEN:
			VERIFY(errMsg.LoadString(IDS_LUM_DOOR_OPEN));
			break;

		default:
			errMsg.Format(IDS_LCC_ASYNC_ERR, m_lastAsyncError);
		}

		AfxMessageBox(errMsg);
		m_lastAsyncError = KO_OK;	
	}
}

long CLccUser::ConnectLum()
{
	if (GetLccMgr() == NULL)
	{
		return KO_LUC_LCC_UNCONNECTED;
	}

	return GetLccMgr()->Sync(this);
}


bool CLccUser::IsLccAvailable()
{
	if (GetLccMgr() == NULL)
	{
		return false;
	}

	return GetLccMgr()->IsLccAvailable();
}

void CLccUser::FindVialEndHandler()
{
	// Empty base method.
}

bool CLccUser::IsLumOnline()
{
	if (GetLccMgr() == NULL)
	{
		return false;
	}

	double connStat;
	long rv = GetLccMgr()->Inquire(LCC_INQ_LCCSTAT, &connStat);
	
	return (rv == KO_OK && connStat > 0.0);
}
