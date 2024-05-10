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

// GenBox.cpp : implementation file
//

#include "stdafx.h"
#include "GenBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenBox
/////////////////////////////////////////////////////////////////////////////

CGenBox::CGenBox()
{
	m_editable = true;
	m_pGeneric = NULL;
}

CGenBox::~CGenBox()
{
}


BEGIN_MESSAGE_MAP(CGenBox, CComboBox)
	//{{AFX_MSG_MAP(CGenBox)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Load the box, and optionally select assay
/////////////////////////////////////////////////////////////////////////////

bool CGenBox::Init(CGenerics* pGenerics, long selectID)
{
	m_pGeneric = pGenerics;
	// Register the box
	pGenerics->RegGenBox(this);
	// Load the box
	pGenerics->LoadBox(this);
	// Make selection
	return pGenerics->SetBoxSelection(this, selectID);
}

bool CGenBox::Init(CGenerics* pGenerics, LPCTSTR selectName)
{
	m_pGeneric = pGenerics;
	// Register the box
	pGenerics->RegGenBox(this);
	// Load the box
	pGenerics->LoadBox(this);
	// Make selection
	return pGenerics->SetBoxSelection(this, selectName);
}

bool CGenBox::InitAH(CGenerics* pGenerics, long selectID)
{
	m_pGeneric = pGenerics;
	// Register the box
	pGenerics->RegGenBox(this);
	// Load the box
	pGenerics->LoadBoxAH(this);
	// Make selection
	return pGenerics->SetBoxSelection(this, selectID);
}

bool CGenBox::InitAH(CGenerics* pGenerics, LPCTSTR selectName)
{
	m_pGeneric = pGenerics;
	// Register the box
	pGenerics->RegGenBox(this);
	// Load the box
	pGenerics->LoadBoxAH(this);
	// Make selection
	return pGenerics->SetBoxSelection(this, selectName);
}

bool CGenBox::InitMH(CGenerics* pGenerics, long selectID)
{
	m_pGeneric = pGenerics;
	// Register the box
	pGenerics->RegGenBox(this);
	// Load the box
	pGenerics->LoadBoxMH(this);
	// Make selection
	return pGenerics->SetBoxSelection(this, selectID);
}

bool CGenBox::InitMH(CGenerics* pGenerics, LPCTSTR selectName)
{
	m_pGeneric = pGenerics;
	// Register the box
	pGenerics->RegGenBox(this);
	// Load the box
	pGenerics->LoadBoxMH(this);
	// Make selection
	return pGenerics->SetBoxSelection(this, selectName);
}


void CGenBox::OnDestroy() 
{
	// Release registration
	if (m_pGeneric != NULL)
	{
		m_pGeneric->RelGenBox(this);
	}

	CComboBox::OnDestroy();
}
	
void CGenBox::EnableEditing(bool enable)
{
	m_editable = enable;
	
	if (m_editable)
	{
		if(m_pGeneric != NULL)
		{
			if (m_pGeneric->GetCount() > 0)
			{
				if (GetSafeHwnd() != NULL)
				{
					EnableWindow(true);
				}
			}
		}
	}
	else if (GetSafeHwnd() != NULL)
	{
		EnableWindow(false);
	}
};


