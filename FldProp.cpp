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

// FlProp.cpp: implementation of the CSampleDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FldProp.h"
#include "resource.h"
#include "Lucept.h"
#include "AssyDef.h"
#include "WklDoc.h"
#include "UsrAdmin.h"	// ITL: Added by CAR 887/002
#include "MainFrm.h"

// Re-read field	-- string listing samples which are re-reads of this one
// Initial data	-- string type
// Copy data for template -- BOOLEAN

// Write enable mask specifies a free form field 

static FieldDefaults initialProp[SF_FIELD_COUNT] = 
{
//   ID				Title		   Col Width WrEnMask		Data Type		Cell Type		Extended Cell Type		Alignment	
	{SF_POSITION  ,IDS_SF_POSITION  ,0	,80  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT	,DAO_SIZE_COUNT	},
	{SF_NAME	  ,IDS_SF_NAME	    ,1	,80  ,0x1F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT 	,DAO_SIZE_NAME	},
	{SF_TYPE_XFI  ,IDS_SF_TYPE	    ,2	,90  ,0x0F000001	,UG_TEXTDATA	,UGCT_DROPLIST	,UGCT_DROPLISTHIDEBUTTON,UG_ALIGNLEFT	,DAO_SIZE_STRING},
	{SF_STATUS	  ,IDS_SF_STAT      ,4	,80  ,0x00000000	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STAT	},
	{SF_ATTITUDE  ,IDS_SF_ATTITUDE  ,-2	,40  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_COUNT	},
	{SF_REPLS	  ,IDS_SF_REPLS     ,3	,50  ,0x0F000001	,UG_NUMBERDATA	,UGCT_SPINBUTTON,UGCT_DROPLISTHIDEBUTTON,UG_ALIGNCENTER	,DAO_SIZE_COUNT	},
	{SF_RLU		  ,IDS_SF_RLU	    ,7	,60  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU_SD	  ,IDS_SF_RLU_SD    ,-2	,80  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_CV		  ,IDS_SF_CV	    ,8	,60  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT	,DAO_SIZE_RLU	},
	{SF_RESULT	  ,IDS_SF_RESULT    ,5	,75  ,0x00000000	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_NAME	}, 
	{SF_NOTIF	  ,IDS_SF_NOTIF     ,6	,180 ,0x00000000	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_DETAIL},
	{SF_BKG		  ,IDS_SF_BKG	    ,9	,60  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG_SD	  ,IDS_SF_BKG_SD    ,-2	,80  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG_CV	  ,IDS_SF_BKG_CV    ,-2	,80  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_NRESULT	  ,IDS_SF_NRESULT	,-2 ,80  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_NUNITS	  ,IDS_SF_NUNITS    ,-2	,60  ,0x0F000000	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STAT	},
	{SF_SAMP_DATE ,IDS_SF_SAMP_DATE	,-2	,100 ,0x1E00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNCENTER	,DAO_SIZE_DATE	},
	{SF_READ_DATE ,IDS_SF_READ_DATE	,10	,80  ,0x0C000000	,UG_DATEDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNCENTER	,DAO_SIZE_DATE	},
	{SF_READ_TIME ,IDS_SF_READ_TIME	,11	,80  ,0x0C000000	,UG_TIMEDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNCENTER	,DAO_SIZE_DATE	},
	{SF_LOCATION  ,IDS_SF_LOCATION	,12	,100 ,0x1F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_NAME	},
	{SF_PRODUCT	  ,IDS_SF_PRODUCT	,13	,80  ,0x1F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_NAME	},
	{SF_LOT		  ,IDS_SF_LOT	    ,14	,60  ,0x1F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_NAME	},
	{SF_COMMENT	  ,IDS_SF_COMMENT	,16	,180 ,0x1F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_DETAIL},
	{SF_FLAG1	  ,IDS_SF_FLAG1     ,-2	,50  ,0x00000000	,UG_BOOLDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_BOOL	},
	{SF_FLAG2	  ,IDS_SF_FLAG2     ,-2	,50  ,0x00000000	,UG_BOOLDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_BOOL	},
	{SF_FLAG3	  ,IDS_SF_FLAG3     ,-2	,50  ,0x00000000	,UG_BOOLDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_BOOL	},
	{SF_RLU1	  ,IDS_SF_RLU1	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU2	  ,IDS_SF_RLU2	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU3	  ,IDS_SF_RLU3	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU4	  ,IDS_SF_RLU4	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU5	  ,IDS_SF_RLU5	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU6	  ,IDS_SF_RLU6	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU7	  ,IDS_SF_RLU7	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU8	  ,IDS_SF_RLU8	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU9	  ,IDS_SF_RLU9	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_RLU10	  ,IDS_SF_RLU10     ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG1	  ,IDS_SF_BKG1	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG2	  ,IDS_SF_BKG2	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG3	  ,IDS_SF_BKG3	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG4	  ,IDS_SF_BKG4	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG5	  ,IDS_SF_BKG5	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG6	  ,IDS_SF_BKG6	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG7	  ,IDS_SF_BKG7	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG8	  ,IDS_SF_BKG8	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG9	  ,IDS_SF_BKG9	    ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_BKG10	  ,IDS_SF_BKG10     ,-2	,80  ,0x0C000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT 	,DAO_SIZE_RLU	},
	{SF_TEXT01	  ,IDS_SF_TEXT01	,17	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT02	  ,IDS_SF_TEXT02 	,18	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT03	  ,IDS_SF_TEXT03	,19	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT04	  ,IDS_SF_TEXT04 	,20	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT05	  ,IDS_SF_TEXT05	,21	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT06	  ,IDS_SF_TEXT06	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_STRING },
	{SF_TEXT07	  ,IDS_SF_TEXT07	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_STRING },
	{SF_TEXT08	  ,IDS_SF_TEXT08 	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_STRING },
	{SF_TEXT09	  ,IDS_SF_TEXT09	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_STRING },
	{SF_TEXT10	  ,IDS_SF_TEXT10	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_STRING },
	{SF_TEXT11	  ,IDS_SF_TEXT11	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_STRING },
	{SF_TEXT12	  ,IDS_SF_TEXT12 	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT13	  ,IDS_SF_TEXT13	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT14	  ,IDS_SF_TEXT14 	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_TEXT15	  ,IDS_SF_TEXT15	,-2	,80  ,0x3F00FFFF	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT  	,DAO_SIZE_STRING },
	{SF_REFER	  ,IDS_SF_REFER 	,15	,180 ,0x00000000	,UG_TEXTDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_DETAIL },
	{SF_VIRTUAL	  ,IDS_SF_VIRTUAL	,-2	,40  ,0x03000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNLEFT	,DAO_SIZE_COUNT	 },
	{SF_TEMPERATURE ,IDS_SF_TEMPERATURE	,-2	,40  ,0x00000000	,UG_NUMBERDATA	,UGCT_NORMAL	,UGCT_NORMALSINGLELINE	,UG_ALIGNRIGHT	,DAO_SIZE_NAME	 },
};

static CFieldProperties defaultProperties;
static CString defaultPath;
static bool defaultsRead = false;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CFP_ALLOW_EDITING
#undef  CFP_ALLOW_EDITING

HFONT	CFieldProperties::m_hBoldFont;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction/Serialization
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CFieldProperties, CObject,
				 VERSIONABLE_SCHEMA | FLDPROP_SER_SCHEMA);

CFieldProperties::CFieldProperties()
{
	m_hBoldFont = NULL;

	// Defaults initialized in InitDefaults()
	if (this == &defaultProperties)
	{
		return;
	}

	// Normal case -- just copy defaults
	*this = defaultProperties;
}

CFieldProperties::CFieldProperties(CFieldProperties& source)
{
	*this = source;
}

CFieldProperties::~CFieldProperties()
{

}

bool CFieldProperties::GetIndexReadOnly(int iField, long status)
{ 
#ifdef CFP_ALLOW_EDITING
	// Allow editing in all fields
	return false;
#else
	ASSERT(iField >= 0 && iField < m_fieldCount);
	ASSERT(status >= 0 && status < 24);	// 24 bits reserved for status
	return (m_aFldProp[iField].WrEnMask & 1 << status) == 0;
#endif
}

long CFieldProperties::SetIndexReadOnly(long iField, long status)
{
	ASSERT(iField >= 0 && iField < m_fieldCount);

	//!!! should it shift by status and only set certain bits off?
	return (m_aFldProp[iField].WrEnMask = 0); 
}

////////////////////////////////////////////////////////////////////////
//  Assignment Operator
////////////////////////////////////////////////////////////////////////

CFieldProperties& CFieldProperties::operator =(CFieldProperties& source)
{
	int iField;

	m_fieldCount = source.m_fieldCount;
	m_columnCount = 0;

	m_hBoldFont = source.m_hBoldFont;

	// Clear lookup maps
	m_indexFromCol.RemoveAll();
	m_indexFromID.RemoveAll();

	for (iField = 0; iField < SF_FIELD_COUNT; iField++)
	{
		if( iField < m_fieldCount) {
			m_aFldProp[iField].ID = 				source.m_aFldProp[iField].ID;
			m_aFldProp[iField].title = 				source.m_aFldProp[iField].title;
			m_aFldProp[iField].viewCol = 			source.m_aFldProp[iField].viewCol;
			m_aFldProp[iField].width = 				source.m_aFldProp[iField].width;
			m_aFldProp[iField].WrEnMask = 			source.m_aFldProp[iField].WrEnMask;
			m_aFldProp[iField].dataType = 			source.m_aFldProp[iField].dataType;
			m_aFldProp[iField].cellType = 			source.m_aFldProp[iField].cellType;
			m_aFldProp[iField].cellTypeEx = 		source.m_aFldProp[iField].cellTypeEx;
			m_aFldProp[iField].alignment = 			source.m_aFldProp[iField].alignment;
		} else {
			m_aFldProp[iField].ID =			initialProp[iField].ID;
			m_aFldProp[iField].title.LoadString(initialProp[iField].IDS_DEFAULT_TITLE);
			m_aFldProp[iField].viewCol =	initialProp[iField].viewCol;
			m_aFldProp[iField].width =		initialProp[iField].width;
			m_aFldProp[iField].WrEnMask =	initialProp[iField].WrEnMask;
			m_aFldProp[iField].dataType =	initialProp[iField].dataType;
			m_aFldProp[iField].cellType =	initialProp[iField].cellType;
			m_aFldProp[iField].cellTypeEx = initialProp[iField].cellTypeEx;
			m_aFldProp[iField].alignment =  initialProp[iField].alignment;
		}

		// Create ID map entry
		ASSERT(m_aFldProp[iField].ID != LUC_ID_UNDEF);
		m_indexFromID.SetAt(m_aFldProp[iField].ID, iField);

		// Create column map entry if column is visible
		if (m_aFldProp[iField].viewCol != LUC_COL_UNDEF)
		{
			m_indexFromCol.SetAt(m_aFldProp[iField].viewCol, iField);
			m_columnCount++;
		}

		// Free form fields must be text type
		ASSERT(!IsFreeForm(m_aFldProp[iField].WrEnMask) || 
			m_aFldProp[iField].dataType == UG_TEXTDATA);
	}

	m_assayName = source.m_assayName;	// OBSOLETE
	m_sampTypeList = source.m_sampTypeList;	

	return *this;
}

void CFieldProperties::Serialize( CArchive& archive )
{
	int iField;

	// Always save in the latest format
	if (archive.IsStoring())
	{		
		archive << FLDPROP_MAGIC_INT;
		archive << FLDPROP_SER_SCHEMA;

		// Save field count
		archive << m_fieldCount;
		for (iField = 0; iField < m_fieldCount; iField++)
		{
			archive << m_aFldProp[iField].ID;
			
			// These are only true variables 
			archive << m_aFldProp[iField].viewCol;
			archive << m_aFldProp[iField].width;
			archive << m_aFldProp[iField].WrEnMask; //in case altered by PharmaSec

			if (IsRenamable(defaultProperties.m_aFldProp[iField].WrEnMask))
			{	// Save renamable field titles when writing so they can 
				// be checked later for consistency
				archive << m_aFldProp[iField].title;
			}
		}
	}
	else
	{ // IsLoading
	
		int	 firstInt = 0;
		UINT version  = 0;

		// Look for magic int
		archive >> firstInt;
		if (firstInt != FLDPROP_MAGIC_INT)
		{ //ERROR
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
		
		/* Get version and post update warning, if necessary.
		If fldProp is part of workload document don't give
		notice, it's already been done.
		Get file version from parent document*/
		archive >> version;
// pcw 26-9-2014 field properties changed for major new revision of software: suppress warning.
//		if (version < FLDPROP_SER_SCHEMA)
//		{
//			CWklDoc* pDoc = (CWklDoc*)archive.m_pDocument;
//			if (pDoc == NULL || !pDoc->IsKindOf(RUNTIME_CLASS(CWklDoc)))
//			{
//				CString msg;
//				msg.Format(IDS_FP_VER_UPDATE, version, FLDPROP_SER_SCHEMA);
//				AfxMessageBox(msg);
//			}
//		}

		switch(version)
		{
		case FLDPROP_SER_SCHEMA:
			SerLoadVer4(archive);
			break;
		case 3:
			SerLoadVer3(archive);
			break;
		case 2:
			SerLoadVer2(archive);
			break;
		default:
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
	}	// End of loading
};


void CFieldProperties::SerLoadVer2(CArchive &archive)
{
	m_columnCount = 0;

	archive >> m_fieldCount;
	for (int iField = 0; iField <= SF_VIRTUAL; iField++)
	{
		archive >> m_aFldProp[iField].ID;
		archive >> m_aFldProp[iField].viewCol;
		archive >> m_aFldProp[iField].width;

		if (IsRenamable(defaultProperties.m_aFldProp[iField].WrEnMask))	
		{
			// Free form fields allow renaming
			// Check titles when reading 
			CString readTitle;
			archive >> readTitle;
			if ((readTitle != m_aFldProp[iField].title) &&
				(gGateKeeper.CurrentUserNameGet() != _T("")))
			{
				CString errMsg;
				errMsg.Format(IDS_FIELD_NAME_CHANGED, readTitle, m_aFldProp[iField].title);
				AfxMessageBox(errMsg);
			}
		}

		// Create ID map entry
		m_indexFromID.SetAt(m_aFldProp[iField].ID, iField);

		// Create column map entry if column is visible
		if (m_aFldProp[iField].viewCol != LUC_COL_UNDEF)
		{
			m_indexFromCol.SetAt(m_aFldProp[iField].viewCol,iField);
			m_columnCount++;
		}

		// Free form fields must be text type
		ASSERT(!IsFreeForm(m_aFldProp[iField].WrEnMask) || m_aFldProp[iField].dataType == UG_TEXTDATA);
	}
}

void CFieldProperties::SerLoadVer3(CArchive &archive)
{
	m_columnCount = 0;

	archive >> m_fieldCount;
	for (int iField = 0; iField <= SF_VIRTUAL; iField++)
	{
		archive >> m_aFldProp[iField].ID;
		archive >> m_aFldProp[iField].viewCol;
		archive >> m_aFldProp[iField].width;

		//new in version 3:
		archive >> m_aFldProp[iField].WrEnMask; //in case altered by PharmaSec

		if ((IsRenamable(defaultProperties.m_aFldProp[iField].WrEnMask)))
		{	// Free form fields allow renaming
			// Check titles when reading 
			CString readTitle;
			archive >> readTitle;
			if (readTitle != m_aFldProp[iField].title && m_FieldCheck)	// ITL: This gives a one shot at checking
			{
				CString errMsg;
				errMsg.Format(IDS_FIELD_NAME_CHANGED, readTitle, m_aFldProp[iField].title);
				AfxMessageBox(errMsg);
			}
		}

		// Create ID map entry
		m_indexFromID.SetAt(m_aFldProp[iField].ID, iField);

		// Create column map entry if column is visible
		if (m_aFldProp[iField].viewCol != LUC_COL_UNDEF)
		{
			m_indexFromCol.SetAt(m_aFldProp[iField].viewCol,iField);
			m_columnCount++;
		}

		// Free form fields must be text type
		ASSERT(!IsFreeForm(m_aFldProp[iField].WrEnMask) || m_aFldProp[iField].dataType == UG_TEXTDATA);
	}
		m_FieldCheck = false; // ITL: Control field check
}

/**
 * Version 4: as per version 3 but with added temperature column.
 */
void CFieldProperties::SerLoadVer4(CArchive &archive)
{
	m_columnCount = 0;

	archive >> m_fieldCount;
	for (int iField = 0; iField < m_fieldCount; iField++)
	{
		archive >> m_aFldProp[iField].ID;
		archive >> m_aFldProp[iField].viewCol;
		archive >> m_aFldProp[iField].width;

		//new in version 3:
		archive >> m_aFldProp[iField].WrEnMask; //in case altered by PharmaSec

		if ((IsRenamable(defaultProperties.m_aFldProp[iField].WrEnMask)))
		{	// Free form fields allow renaming
			// Check titles when reading 
			CString readTitle;
			archive >> readTitle;
			if (readTitle != m_aFldProp[iField].title && m_FieldCheck)	// ITL: This gives a one shot at checking
			{
				CString errMsg;
				errMsg.Format(IDS_FIELD_NAME_CHANGED, readTitle, m_aFldProp[iField].title);
				AfxMessageBox(errMsg);
			}
		}

		// Create ID map entry
		m_indexFromID.SetAt(m_aFldProp[iField].ID, iField);

		// Create column map entry if column is visible
		if (m_aFldProp[iField].viewCol != LUC_COL_UNDEF)
		{
			m_indexFromCol.SetAt(m_aFldProp[iField].viewCol,iField);
			m_columnCount++;
		}

		// Free form fields must be text type
		ASSERT(!IsFreeForm(m_aFldProp[iField].WrEnMask) || m_aFldProp[iField].dataType == UG_TEXTDATA);
	}
		m_FieldCheck = false; // ITL: Control field check
}
////////////////////////////////////////////////////////////////////////
//  Hide all fields
////////////////////////////////////////////////////////////////////////

void CFieldProperties::HideAllFields()
{
	// Clear column list
	for (int iField = 0; iField < m_fieldCount; iField++)
	{
		m_aFldProp[iField].viewCol = LUC_COL_UNDEF;
	}

	// Remove all keys from map
	m_indexFromCol.RemoveAll();

	m_columnCount = 0;
}


////////////////////////////////////////////////////////////////////////
//  Passed field ID indicates field to be addes as next column in grid
////////////////////////////////////////////////////////////////////////

BOOLEAN CFieldProperties::ShowIDNext(long fieldID)
{
	int index;

	if (!m_indexFromID.Lookup(fieldID, index))
	{	// Bad field ID
		return FALSE;
	}

	if (m_aFldProp[index].viewCol != LUC_COL_UNDEF)
	{	// Field already showing
		return FALSE;
	}
	
	// Set view column
	m_aFldProp[index].viewCol = m_columnCount;

	// Add key to map
	m_indexFromCol.SetAt(m_aFldProp[index].viewCol, index);

	// Increment column count
	m_columnCount++;
	
	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Set Column Width
////////////////////////////////////////////////////////////////////////

BOOLEAN CFieldProperties::SetColWidth(int iCol, int width)
{
	int index;

	if (!m_indexFromCol.Lookup(iCol, index))
	{
		return FALSE;
	}

	m_aFldProp[index].width = width;
	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// Assay is used to determine allowable sample types for droplist
////////////////////////////////////////////////////////////////////////

void CFieldProperties::SetSampTypeList(CAssayDef* pAssay)
{
	if (pAssay != NULL)
	{
		// Create sample type list string
		m_sampTypeList.Empty();
		int num = pAssay->GetNumSampTypes(); 
		for (int i = 0; i < num; i++)
		{	// Add next sample type
			m_sampTypeList += pAssay->IndexGetSampName(i);
			// Add newline
			m_sampTypeList += _T("\n");
		}
	}
	else
	{
		// Only normal samples are allowed
		m_sampTypeList.LoadString(IDS_STNAME_NORMAL);
	}
}

bool CFieldProperties::StaticDefaultsRead()
{
		// INI file field section header.	
		CString	iniFieldSect;
		VERIFY(iniFieldSect.LoadString(IDS_INI_SECT_FIELDS));	
		CLuceptApp* app	= (CLuceptApp *)AfxGetApp();	// needed to access INI file

		// Set number of fields
		defaultProperties.m_fieldCount = SF_FIELD_COUNT;
		defaultProperties.m_columnCount = 0;

		// Load default values
		for(int iField = 0; iField < defaultProperties.m_fieldCount;iField++)
		{
			defaultProperties.m_aFldProp[iField].ID			= initialProp[iField].ID;
			defaultProperties.m_aFldProp[iField].viewCol    = initialProp[iField].viewCol;
			defaultProperties.m_aFldProp[iField].width      = initialProp[iField].width;
			defaultProperties.m_aFldProp[iField].WrEnMask   = initialProp[iField].WrEnMask;
			defaultProperties.m_aFldProp[iField].dataType   = initialProp[iField].dataType;
			defaultProperties.m_aFldProp[iField].cellType   = initialProp[iField].cellType;
			defaultProperties.m_aFldProp[iField].cellTypeEx = initialProp[iField].cellTypeEx;
			defaultProperties.m_aFldProp[iField].alignment  = initialProp[iField].alignment;

			// Default field title is a string resource ID
			// Normal field title property is a CString
			VERIFY(defaultProperties.m_aFldProp[iField].title.
				LoadString(initialProp[iField].IDS_DEFAULT_TITLE));

			// Titles from INI file will override defaults
			if(IsRenamable(defaultProperties.m_aFldProp[iField].WrEnMask))
			{	// Look for item = default field title in section Fields
				CString newTitle = app->GetLuceptProfileString(iniFieldSect, 
					defaultProperties.m_aFldProp[iField].title, _T(""));
				if(!newTitle.IsEmpty())
				{	// Make sure the name is not in use
					int iOld;

					for( iOld = 0; iOld < iField; iOld++)
					{	// !!! still doesn't check if pre-defined
						// field is at higher index
						if(defaultProperties.m_aFldProp[iOld].title.CompareNoCase(newTitle) == 0)
						{	// Found a match
							CString errMsg;
							errMsg.Format(_T("Duplicate field name: '%s'"), newTitle);
							AfxMessageBox(errMsg);
							break;
						}
					}

					if(iOld == iField && PUtil::IsValidDAOFieldName(newTitle))
					{	// Use new field name
						defaultProperties.m_aFldProp[iField].title = newTitle;
					}
					else if (iOld == iField)
					{	// Post error message 
						CString errMsg;
						errMsg.Format(IDS_ERR_USERTITLE, newTitle);
						AfxMessageBox(errMsg);
					}
				}
			}

			// Create ID map entry
			ASSERT(defaultProperties.m_aFldProp[iField].ID != LUC_ID_UNDEF);
			defaultProperties.m_indexFromID.SetAt(defaultProperties.m_aFldProp[iField].ID, iField);

			// Create column map entry if column is visible
			if(defaultProperties.m_aFldProp[iField].viewCol !=	LUC_COL_UNDEF)
			{
				defaultProperties.m_indexFromCol.SetAt(defaultProperties.m_aFldProp[iField].viewCol,iField);
				defaultProperties.m_columnCount++;
			}

			// Free form fields must be text type
			ASSERT(!IsFreeForm(defaultProperties.m_aFldProp[iField].WrEnMask) ||
				defaultProperties.m_aFldProp[iField].dataType == UG_TEXTDATA);
		}//end for

		// Default sample type list
		defaultProperties.SetSampTypeList(NULL);

		return true;
}

bool CFieldProperties::InitDefaults(LPCTSTR path)
{

	// First load internal static defaults
	// Static Class Initialization (execute once for application)
	// Initializes a static object, defaultProperties 
	if(!defaultsRead)
	{	// Do delayed construction on default object	
		defaultsRead = StaticDefaultsRead();
	} 

	// First call to this function must specify the path,
	// successive calls may use saved path information
	if(path != NULL)
	{
		defaultPath = path;
	}
	else
	{
		if (!defaultPath.IsEmpty())
		{
			path = defaultPath;
		}
		else
		{
			return false;
		}
	}

	// Try to open the specified file
	try
	{
		CFile infile(path, CFile::modeRead | CFile::shareDenyWrite);
		CArchive	archive(&infile, CArchive::load);

		// Load default properties from disk
		defaultProperties.Serialize(archive);
		archive.Close();

		defaultsRead = true;
		// That's it.  CFile destructor will close the file.
	}
	catch (CFileException* e)
	{
		if(e->m_cause == CFileException::fileNotFound)
		{
			e->Delete();
			return false;
		}
		else
		{
			// Report error
			LPVOID lpMsgBuf;
			
			DWORD fmtRV = ::FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL, e->m_lOsError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(LPTSTR) &lpMsgBuf, 0, NULL); 

			AfxMessageBox((LPCTSTR)lpMsgBuf);

			LocalFree(lpMsgBuf);
			e->Delete();
			return false;
		}
	}

	return true;
}

bool CFieldProperties::SetDefaults()
{
	// Default file path must have been previously set by call to InitDefaults
	if (defaultPath.IsEmpty())
	{
		return false;
	}

	// Try to open the specified file for writing.  Overwrite any existing.
	try
	{
		CFile outfile(defaultPath, CFile::modeCreate |
			CFile::modeWrite | CFile::shareExclusive);
		CArchive	archive(&outfile, CArchive::store);

		// Copy contents to default
		defaultProperties = *this;

		// Store default properties to disk
		defaultProperties.Serialize(archive);
		archive.Close();

		// That's it.  CFile destructor will close the file.
	}
	catch (CFileException* e)
	{	// Report error
		LPVOID lpMsgBuf;
		
		DWORD fmtRV = ::FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL, e->m_lOsError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 0, NULL); 

		AfxMessageBox((LPCTSTR)lpMsgBuf);

		LocalFree(lpMsgBuf);
		e->Delete();
		return false;
	}

	return true;
};

bool CFieldProperties::GetDefaults()
{
	*this = defaultProperties;

	return true;
};

long CFieldProperties::GetIndexDaoSize(int iField)
{
	ASSERT(iField >= 0 && iField < SF_FIELD_COUNT);

	return initialProp[iField].DAOSize;
}

short CFieldProperties::GetIndexDAOType(int iField)
{
	ASSERT(iField >= 0 && iField < SF_FIELD_COUNT);

	int UGtype = initialProp[iField].dataType;

	switch(UGtype) 
	{
	case UG_TEXTDATA:
		return dbText;
	case UG_BOOLDATA:	
		return dbBoolean;
	case UG_NUMBERDATA:
		return dbDouble;
	case UG_DATEDATA:
	case UG_TIMEDATA:
		return dbDate;
	default:
		return dbText;
	}
}

CString CFieldProperties::GetIndexTitle(int iField)
{
	ASSERT(iField >= 0 && iField < SF_FIELD_COUNT);
	ASSERT(defaultsRead);

	return defaultProperties.m_aFldProp[iField].title;
}

CString CFieldProperties::GetIDTitle(long fieldID)
{
	return GetIndexTitle(
		defaultProperties.GetIDIndex(fieldID));
}

////////////////////////////////////////////////////////////////////////
// Do a simple inefficient search by title
////////////////////////////////////////////////////////////////////////

long CFieldProperties::GetTitleID(LPCTSTR title)
{
	long ID = LUC_FLD_UNDEF;

	ASSERT(defaultsRead);
	for (int iField = 0; iField < defaultProperties.m_fieldCount; iField++)
	{
		if (defaultProperties.m_aFldProp[iField].title.
			CompareNoCase(title) == 0)
		{	// Found a match
			ID = defaultProperties.m_aFldProp[iField].ID;
			break;
		}
	}

	return ID;
}




