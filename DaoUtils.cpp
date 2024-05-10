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
//	DaoUtils.cpp
//
//
#include "stdafx.h"
#include "DaoUtils.h"

///////////////////////////////////////////////////////////////////////////////
//
//
//
void DaoUtils::DaoExceptionBox(CDaoException* e, 
								 const CString& File, 
								 int Line, 
								 LPCTSTR RoutineSpecificStr ) 
{
	CString temp;
	int		i;
	int		k;

	k = e->GetErrorCount(); 
	for(i = 0; i < k; i++) 
	{
		e->GetErrorInfo(i);
		temp.Format(
			_T("DAO Exception(s) Count %d of %d:\
\r\nFile: %s\
\r\nLine: %d\
\r\nRoutine: %s\
\r\n\r\nError No: %d:\
\r\nAFX Dao Error No: %d\
\r\n%s\r\n\r\nSource: %s\
\r\n\r\nHelpFile: %s"),
			i + 1, k,
			File, Line, RoutineSpecificStr, 
			e->m_pErrorInfo->m_lErrorCode, 
			e->m_nAfxDaoError,
			e->m_pErrorInfo->m_strDescription, 
			e->m_pErrorInfo->m_strSource,
			e->m_pErrorInfo->m_strHelpFile
		);//end temp.Format

		AfxMessageBox(temp, MB_ICONEXCLAMATION);
	}//end for

	//delete the exception object
	e->Delete();
}

///////////////////////////////////////////////////////////////////////////////
//
//
//
CString DaoUtils::OleVarToString(const COleVariant& var)
{
	CString str;
		
	switch (var.vt) 
	{
    case VT_BSTR:
//	case VT_BSTRT:
      str = (LPCTSTR) var.bstrVal; // narrow characters in DAO
      break;
    case VT_I2:
      str.Format(_T("%d"), (int) var.iVal);
      break;
    case VT_I4:
      str.Format(_T("%ld"), var.lVal);
      break;
    case VT_R4:
      str.Format(_T("%f"), (double) var.fltVal);
      break;
    case VT_R8:
	  //arbitrarily set to 3 significant digits 
      str.Format(_T("%.3lf"), var.dblVal); 
      break;
    case VT_DATE:
		{
			COleDateTime dt(var); 
			long whole_num_part; 

			whole_num_part = (long)dt.m_dt;
			if(whole_num_part == 0)
			{	//a time-only object
				str = dt.Format(_T("%X"));
			}
			else if(dt.m_dt - whole_num_part == 0.0)
			{	//a date-only object
				str = dt.Format(_T("%x"));
			}
			else
			{	//a date-time object
				str = dt.Format(_T("%x %X"));
			}
		}
      break;
    case VT_BOOL:
 		str = (var.boolVal == 0) ? _T("FALSE") : _T("TRUE");  
      break;
    case VT_NULL:
      str =  _T("null");
      break;
    default:
      str.Format(_T("Unk type %d\n"), var.vt);
      TRACE(_T("Unknown type %d\n"), var.vt);
    }
	return str;
}

///////////////////////////////////////////////////////////////////////////////
//
//
//
int DaoUtils::SQLOperatorsTypeGet(const int dbType)
{
	switch(dbType)
	{
	case dbDate:
	case dbDouble:
	case dbFloat:
	case dbInteger:
	case dbLong:
	case dbNumeric:
	case dbSingle:
	case dbTime:
		return NUMERIC_OPS;

	case dbText:
		return STRING_OPS;

	case dbBoolean:
		return BOOLEAN_OPS;

 	default:
		return DEFAULT_OPS;
    }
}

///////////////////////////////////////////////////////////////////////////////
//	BOOL DaoUtils::IsEmpty(CDaoRecordset& r)
//
//	Recordsets can be empty because no records have been added, or because
//  all records have been deleted.  The way in which records have been deleted,
//  whether by forward or backward traverse, will affect the EOF and BOF
//  conditions which are the only way to tell if the set contains any records.
//
//  Moving to first and last records can be a big performance hit on a list with
//  lots of records.  It may be better to test for empty condition at only one
//  end of the set, if you know the set's life history.
//
BOOL DaoUtils::IsEmpty(CDaoRecordset& r)
{

	try
	{
		if(r.IsBOF() && r.IsEOF())
		{	//empty for a new recordset
			return TRUE;
		}
		else if(r.IsBOF())
		{	//potentially empty by reverse deletion
			r.MoveLast();	 //this will set EOF TRUE, if set is empty
		}
		else
		{	//potentially empty by forward deletion
			r.MoveFirst();   //this will set BOF TRUE, if set is empty
		}
		return (r.IsBOF() && r.IsEOF()); //same test again
	}
	catch(CDaoException* e)
	{
		DaoExceptionBox(e, __FILE__, __LINE__,_T("DaoUtils::IsEmpty"));
	}
	catch(...)
	{
		throw;
	}
	return (r.IsBOF() && r.IsEOF()); //same test again
}

long DaoUtils::RecordCountGet(CDaoRecordset& r)
{
	ASSERT(r.IsOpen());
	long count = -2; //to distinguish from GetRecordCount's -1 value

	try
	{
		if(IsEmpty(r))
		{
			count = 0;
		}
		else
		{
			r.MoveFirst();
			r.MoveLast();
			count = r.GetRecordCount();
		}
	}
	catch(CDaoException* e)
	{
		DaoExceptionBox(e, __FILE__, __LINE__,_T("DaoUtils::RecordCountGet"));
	}
	catch(...)
	{
		throw;
	}

	return count;
}

long DaoUtils::RecordsRemoveAll(CDaoRecordset& r)
{
	ASSERT(r.IsOpen());
	ASSERT(r.CanUpdate());

	long count		= 0;
	long countdown	= 0;
	
	try
	{
		if(countdown = RecordCountGet(r))
		{
			r.MoveFirst();
			while(countdown--)
			{
				r.Delete();
				r.MoveNext();
				count++;
			}
			r.MoveFirst();
			ASSERT(r.IsEOF());
			ASSERT(r.IsBOF());
		}
	}
	catch(CDaoException* e)
	{
		DaoExceptionBox(e, __FILE__, __LINE__,_T("DaoUtils::RemoveRecords"));
	}
	catch(...)
	{
		throw;
	}

	return count;
}

///////////////////////////////////////////////////////////////////////////////
//	All debug routines must be defined in the following section
//
//
#ifdef _DEBUG
///////////////////////////////////////////////////////////////////////////////
void DaoUtils::TableDefTrace(CDaoDatabase* pDB, const LPCTSTR lpszName)
{
	ASSERT(pDB);
	ASSERT(lpszName);

	CDaoTableDefInfo tInfo;
	pDB->GetTableDefInfo(lpszName, tInfo, AFX_DAO_ALL_INFO);
	
	TRACE(_T("DaoUtils::TableDefTrace: name: %s\n"), lpszName);
	TRACE(_T("Name:				%s\n"),	tInfo.m_strName);
	TRACE(_T("Updatable:		%d\n"),	tInfo.m_bUpdatable);
	TRACE(_T("Attributes:		%ld\n"),tInfo.m_lAttributes);
	TRACE(_T("Connect:			%s\n"),	tInfo.m_strConnect);
}

void DaoUtils::QueryDefTrace(CDaoDatabase* pDB, const LPCTSTR lpszName)
{
	ASSERT(pDB);
	ASSERT(lpszName);

	CDaoQueryDefInfo tInfo;
	pDB->GetQueryDefInfo(lpszName, tInfo, AFX_DAO_ALL_INFO);

	TRACE(_T("DaoUtils::QueryDefTrace:\n"));
	TRACE(_T("Name:				%s\n"),	tInfo.m_strName);
	TRACE(_T("Type:				%d\n"),	tInfo.m_nType);
	TRACE(_T("Updatable:		%d\n"),	tInfo.m_bUpdatable);
	TRACE(_T("ReturnsRecords:	%d\n"),	tInfo.m_bReturnsRecords);
	TRACE(_T("SQL:				%s\n"),	tInfo.m_strSQL);
	TRACE(_T("Connect:			%s\n"),	tInfo.m_strConnect);
}

void DaoUtils::RecordsetTrace(CDaoRecordset* pRS, LPCTSTR s)
{
	ASSERT_VALID(pRS);
	
	TRACE(_T("DaoUtils::RecordsetTrace:  %s\n"), s);
	TRACE(_T("m_nFields:		%d\n"), pRS->m_nFields);
	TRACE(_T("Field count		%d\n"), pRS->GetFieldCount());
	TRACE(_T("Params:			%d\n"),	pRS->m_nParams);
	TRACE(_T("Filter:			%s\n"),	pRS->m_strFilter);
	TRACE(_T("Sort:				%s\n"),	pRS->m_strSort);
	TRACE(_T("SQL:				%s\n"),	pRS->GetSQL());
	TRACE(_T("Record Count:		%ld\n"),RecordCountGet(*pRS));
}

void FieldValTraceCommon(const CDaoFieldInfo& tFldInfo)
{
	TRACE(_T("Name:				%s\n"),	tFldInfo.m_strName);
	TRACE(_T("Type:				%d\n"),	tFldInfo. m_nType);
	TRACE(_T("Size:				%ld\n"),tFldInfo. m_lSize);	 
	TRACE(_T("Ordinal pos:		%d\n"),	tFldInfo. m_nOrdinalPosition);
}

void DaoUtils::FieldValTrace(CDaoRecordset* pRecSet, const int index)
{
	ASSERT(index >= 0);
	CDaoFieldInfo tFldInfo;

	pRecSet->GetFieldInfo(index, tFldInfo,AFX_DAO_ALL_INFO);

	TRACE(_T("DaoUtils::FieldValTrace: by index: %d\n"), index);
	FieldValTraceCommon(tFldInfo);
}

void DaoUtils::FieldValTrace(CDaoRecordset* pRecSet, const LPCTSTR lpszName)
{
	ASSERT(lpszName != NULL);
	CDaoFieldInfo tFldInfo;

	pRecSet->GetFieldInfo(lpszName, tFldInfo,AFX_DAO_ALL_INFO);

	TRACE(_T("DaoUtils::FieldValTrace: by name: %s\n"), lpszName);
	FieldValTraceCommon(tFldInfo);
}
#endif
///////////////////////////////////////////////////////////////////////////////
//End debug specific routines
///////////////////////////////////////////////////////////////////////////////
