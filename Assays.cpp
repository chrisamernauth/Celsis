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

// Assays.cpp: implementation of the CAssays class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Assays.h"
#include "resource.h"
#include "AssyDef.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAssays::CAssays()
{
	// Since the app owns an instance of this we must delay string
	// initialization until the app has had time to find the string
	// resources.  Call Init to do this when the app is prepared.
}

CAssays::~CAssays()
{
	DeleteAll();
}

BOOL CAssays::Init()
{
	m_errMsgBadID.LoadString(IDS_BAD_ASSAY_ID);	
	m_errMsgNoneDefined.LoadString(IDS_NO_ASSAYS_DEFINED);	

	return InitFromDisk();
}

//////////////////////////////////////////////////////////////////////
// Type specific New and Delete
//////////////////////////////////////////////////////////////////////

CGenDef* CAssays::MakeNewDef()
{
	return new CAssayDef;
}

BOOLEAN  CAssays::DeleteDef(CGenDef* pDef)
{
	ASSERT(pDef->IsKindOf(RUNTIME_CLASS(CAssayDef)));

	delete pDef;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Type specific New and Delete
//////////////////////////////////////////////////////////////////////

CAssayDef* CAssays::GetDefPtr(long ID)
{
	return (CAssayDef*)CGenerics::GetGenDefPtr(ID);
}

CAssayDef* CAssays::GetDefPtrAH(long ID)
{
	return (CAssayDef*)CGenerics::GetGenDefPtrAH(ID);
}
