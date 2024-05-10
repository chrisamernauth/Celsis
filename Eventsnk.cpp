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

// EventSnk.cpp : implementation file
//

#include "stdafx.h"
#include "EventSnk.h"
#include "LucOle.h"
#include "LccMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLCCEventSink

IMPLEMENT_DYNCREATE(CLCCEventSink, CCmdTarget)

CLCCEventSink::CLCCEventSink()
{
	m_pMgr = NULL;
	EnableAutomation();
}

CLCCEventSink::~CLCCEventSink()
{
}


void CLCCEventSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// delete the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CLCCEventSink, CCmdTarget)
	//{{AFX_MSG_MAP(CLCCEventSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLCCEventSink, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CLCCEventSink)
	DISP_FUNCTION(CLCCEventSink, "Event1", Event1, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CLCCEventSink, "Event2", Event2, VT_EMPTY, VTS_BSTR)
#ifndef TWO_EVENT_SINK
	DISP_FUNCTION(CLCCEventSink, "Event3", Event3, VT_EMPTY, VTS_BSTR)
	#ifndef THREE_EVENT_SINK
		DISP_FUNCTION(CLCCEventSink, "Event4", Event4, VT_EMPTY, VTS_BSTR)
	#endif
#endif
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION(CLCCEventSink, "Event5", Event5, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_ILCCEventSink to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// SAMPLECP
// we are going to use this GUID in elsewhere than this file
// so I am remarking it out here and defining it in 
BEGIN_INTERFACE_MAP(CLCCEventSink, CCmdTarget)
	INTERFACE_PART(CLCCEventSink, IID_ILCCEventSink, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLCCEventSink message handlers

void CLCCEventSink::Event1(long lCode) 
{
	TRACE1( "Event1( %d)\n", lCode);
	if (m_pMgr != NULL)
	{
        m_pMgr->OnLccException(lCode);
	}
}

void CLCCEventSink::Event2(LPCTSTR eventText) 
{
	TRACE1( "Event2( %s)\n", eventText);
	if (m_pMgr != NULL)
	{
        m_pMgr->OnLccCSDone();
	}
}


#ifndef TWO_EVENT_SINK
void CLCCEventSink::Event3(LPCTSTR eventText) 
{
	TRACE1( "Event3( %s)\n", eventText);
	if (m_pMgr != NULL)
	{
		m_pMgr->OnLccNewData();
	}
}

	#ifndef THREE_EVENT_SINK
	void CLCCEventSink::Event4(LPCTSTR eventText) 
	{
	TRACE1( "Event4( %s)\n", eventText);
		if (m_pMgr != NULL)
		{
			m_pMgr->OnLccMsg(eventText);
		}
	}
	#endif
#endif

void CLCCEventSink::Event5(LPCTSTR strEventText) 
{
	TRACE1( "Event5( %s)\n", strEventText);
	if (m_pMgr != NULL)
	{
        m_pMgr->OnFindVialDone();
	}
}

void CLCCEventSink::RegisterMgr(CLccMgr* pMgr)
{
	m_pMgr = pMgr;
}

