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
ITL				31-Jan-02			File added by CAR 887/002

************************************************************************************/

////////////////////////////////////////////////////////////////////////////
//
//	Event.cpp - implementation of CLogEvent class
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Event.h"
#include "UsrAdmin.h"
#include "SystemLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////
//	CLogEvent::CLogEvent ()
//
CLogEvent::CLogEvent( ) :
	m_oEventTime( CTime::GetCurrentTime()),
	m_oEventUser(_T("")), 
	m_oEventName(_T("")), 
	m_oEventDetails(_T(""))
{
}

/////////////////////////////////////////////////////
//	CLogEvent::CLogEvent(	int EventID, LPCTSTR EventUser, LPCTSTR EventName,
//					LPCTSTR EventDetails)
//
//
CLogEvent::CLogEvent(CTime EventTime, LPCTSTR EventUser, LPCTSTR EventName, LPCTSTR EventDetails) :
	m_oEventID( 0),
	m_oEventTime		(EventTime),
	m_oEventUser		(EventUser), 
	m_oEventName		(EventName),
	m_oEventDetails	(EventDetails)
{
}

/////////////////////////////////////////////////////
//	CLogEvent::CLogEvent(CLogEvent & rhs)
//		copy constructor
//
CLogEvent::CLogEvent(CLogEvent & rhs)
{
	m_oEventID = rhs.m_oEventID;
	m_oEventTime = rhs.m_oEventTime;
	m_oEventUser = rhs.m_oEventUser;
	m_oEventName = rhs.m_oEventName;
	m_oEventDetails	= rhs.m_oEventDetails;
}

/////////////////////////////////////////////////////
//	CLogEvent::~CLogEvent()
//
CLogEvent::~CLogEvent()
{
}


/////////////////////////////////////////////////////
//	CLogEvent::operator =
//
//	tests for equality of address, not value
//  should be OK except if there is a multiple inheritance
//  in CLogEvent.
//
CLogEvent& CLogEvent::operator =(const CLogEvent& rhs)
{
	if (this == &rhs) 
	{ 
		return *this;
	} 
	else 
	{
		m_oEventID			= rhs.m_oEventID;
		m_oEventTime			= rhs.m_oEventTime;
		m_oEventUser			= rhs.m_oEventUser;
		m_oEventName			= rhs.m_oEventName;
		m_oEventDetails		= rhs.m_oEventDetails;
		return *this; 
	}
}

void CLogEvent::EventSet(const CString & NewEventName, 
					  const CString & NewEventDetails)
{
	m_oEventID		= 0;  // set when added to log
	m_oEventTime		= CTime::GetCurrentTime();
	m_oEventUser		= gGateKeeper.CurrentUserNameGet();
	m_oEventName		= NewEventName;
	m_oEventDetails	= NewEventDetails;
}
