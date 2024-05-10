/**
 * Report Terms- a container class defining all the data used in the terms of a report.
 */
#include "stdafx.h"

#include "ReportTerms.h"
#include "RptDoc.h"

struct {
	enum ReportRelation m_nRelation;
	LPCTSTR m_strOp;
} g_oReportRelations[] = {
	{ FILTER_LESSTHAN,					_T("<") },
	{ FILTER_EQUALLESSTHAN,				_T("<=") },	
	{ FILTER_NOTEQUAL,					_T("<>") },
	{ FILTER_EQUAL,						_T("=") },
	{ FILTER_MORETHAN,					_T(">") },
	{ FILTER_EQUALMORETHAN,				_T(">=") },	
	{ FILTER_LIKE,						_T("LIKE") },
	{ FILTER_NOT_LIKE,					_T("NOT LIKE") }	
};

struct {
	enum ReportFormula m_nFormula;
	LPCTSTR m_strDescription;
} g_oReportFormulas[] = {
	{ FORMULA_AVERAGE,			_T("Average")},
	{ FORMULA_MAXIMUM,			_T("Maximum")},
	{ FORMULA_MINIMUM,			_T("Minimum")},
	{ FORMULA_STDDEV,			_T("Standard Deviation")},
	{ FORMULA_VARIANCE,			_T("Coefficient of Variance")}
};

struct {
	enum ReportFormat m_nFormat;
	LPCTSTR m_strDescription;
	LPCTSTR m_strDefaultText;
} g_oReportFormats[] = {
	{ FORMAT_STANDARD_HEADER,			_T("Standard Header"), 					_T("Celsis Workload Report") },
	{ FORMAT_REPORT_HEADER,				_T("Report Header"),					_T("User Definable Report Header") },
	{ FORMAT_DATA_GRID,					_T("Data Grid in report"),				_T("") },
	{ FORMAT_WORKLOAD_LIST_SUMMARY,		_T("Workload List Sumary"),				_T("") },
	{ FORMAT_SORT_CRITERIA_SUMMARY,		_T("Sort Criteria Summary"),			_T("") },
	{ FORMAT_QUERY_CONDITION_SUMMARY,	_T("Query Condition Summary"),			_T("") },
	{ FORMAT_DATA_INTEGRITY_SUMMARY,	_T("Data Integrity Summary"),			_T("") },
	{ FORMAT_REPORT_FOOTER,				_T("Report Footer"),					_T("User Definable Report Footer")},
	{ FORMAT_STANDARD_FOOTER,			_T("Standard Footer"),					_T("File: {filename} Printed: {when}")},
	{ FORMAT_INCLUDE_RETESTS,			_T("Retests"),							_T("")},
};

struct {
	enum ReportGraph m_nGraph;
	LPCTSTR m_strDescription;
	int m_nIcon;
} g_oReportGraphs[] = {
	{ GRAPH_LINE,				_T("Line Graph"),			5 },
	{ GRAPH_AREA,				_T("Area Graph"),			6 },
	{ GRAPH_BAR,				_T("Bar Graph"),			7 }
};

const LPCTSTR CReportTermColumn::COLUMN_SORTED_SEQUENCE = _T("[Sorted Sequence]");

/**
 * Given the relation, return a string representation.
 */
LPCTSTR CReportTermFilter::GetReleationString( enum ReportRelation nRelation)
{
	// Assumes the array is ordered by enum value.
	return g_oReportRelations[nRelation].m_strOp;
}

/**
 * Populate a combo with Relation options.
 * @param oCombo The combo to be populated.
 */
void CReportTermFilter::PopulateRelationCombo( CComboBox *pCombo, bool bText)
{
	pCombo->ResetContent();

	for( int i=0; i < NO_FILTER_RELATIONS; i++) {
		if( bText == false && (i == FILTER_LIKE || i == FILTER_NOT_LIKE)) {
			continue;
		}
		
		int nIndex = pCombo->AddString( g_oReportRelations[i].m_strOp);

		pCombo->SetItemData( nIndex, g_oReportRelations[i].m_nRelation);
	}
}

/**
 * Given the formula, return a string representation.
 */
LPCTSTR CReportTermFormula::GetFormulaString( enum ReportFormula nFormula)
{
	return g_oReportFormulas[nFormula].m_strDescription;
}

/**
 * Populate a combo with Formula options.
 * @param oCombo The combo to be populated.
 */
void CReportTermFormula::PopulateFormulaCombo( CComboBox *pCombo)
{
	pCombo->Clear();

	for( int i=0; i < NO_FORMULA; i++) {
		int nIndex = pCombo->AddString( g_oReportFormulas[i].m_strDescription);

		pCombo->SetItemData( nIndex, g_oReportFormulas[i].m_nFormula);
	}
}

/**
 * Populate a combo with Format options.
 * @param oCombo The combo to be populated.
 */
void CReportTermFormat::PopulateFormatCombo( CComboBox *pCombo)
{
	pCombo->Clear();

	for( int i=0; i < NO_FORMAT; i++) {
		int nIndex = pCombo->AddString( g_oReportFormats[i].m_strDescription);

		pCombo->SetItemData( nIndex, g_oReportFormats[i].m_nFormat);
	}
}

/**
 * Get the default text for a particular format option.
 * @param nFormat The format option
 * @returns The default text for the option or NULL if there is no associated text
 */
LPCTSTR CReportTermFormat::GetFormatDefaultText( enum ReportFormat nFormat)
{
	return g_oReportFormats[nFormat].m_strDefaultText;
}

/**
 * Get the string that describes a format option
 * @param nFormat The format option
 * @returns The description of the format option.
 */
LPCTSTR CReportTermFormat::GetFormatString( enum ReportFormat nFormat)
{
	return g_oReportFormats[nFormat].m_strDescription;
}

/**
 * Given the graph type, return a string representation.
 */
LPCTSTR CReportTermGraph::GetGraphString( enum ReportGraph nGraph)
{
	return g_oReportGraphs[nGraph].m_strDescription;
}

/**
 * Given the graph type, return the icon index
 */
int CReportTermGraph::GetGraphIcon( enum ReportGraph nGraph)
{
	return g_oReportGraphs[nGraph].m_nIcon;
}


CReportTerms::~CReportTerms()
{
	Clear();
}

/**
 * Set contents of term from an sql string.
 *
 * Examples:
 *       <Column> ASC
 *       <column> DESC
 *       <Column>
 * These strings are machine-generated so the parser does not need to be totally bullet proof.
 *
 * @param strSQL the sql string to be parsed
 * @returns true if the sql seems legit.
 */
bool CReportTermSort::SetFromSQL( CString &strSQL)
{
	CString strParse = strSQL;

	/*
	 * Skip leading spaces.
	 */
	CString strSpaces = strParse.SpanIncluding( _T(" \t"));
	strParse = strParse.Mid( strSpaces.GetLength());

	/*
	 * Look for end of column name: whitespace.
	 */
	CString strColumn = strParse.SpanExcluding( _T("\t "));

	if( strColumn.IsEmpty()) {
		return false;
	}

	strParse = strParse.Mid( strColumn.GetLength());

	/*
	 * Skip spaces before direction.
	 */
	strSpaces = strParse.SpanIncluding( _T(" \t"));
	strParse = strParse.Mid( strSpaces.GetLength());

	bool bAscending = true;

	if( strParse.Left( 3).CompareNoCase( _T("asc")) == 0) {
		strParse = strParse.Mid( 3);
	} else if( strParse.Left( 4).CompareNoCase( _T("desc")) == 0) {
		bAscending = false;
		strParse = strParse.Mid( 4);
	}

	if( !strParse.IsEmpty()) {
		return false;
	}

	SetColumn( strColumn);
	SetAscending( bAscending);

	return true;
}

/**
 * Set contents of term from an sql string.
 *
 * Examples:
 *       AND <Column> != 123
 *       <column> == 'wibble'
 *       OR <Column> == #23/8/1961#
 * These strings are machine-generated so the parser does not need to be totally bullet proof.
 *
 * @param strSQL the sql string to be parsed
 * @returns true if the sql seems legit.
 */
bool CReportTermFilter::SetFromSQL( CString &strSQL)
{
	bool bAnd = true;
	CString strParse = strSQL;

	/*
	 * Skip leading spaces
	 */
	CString strSpaces = strParse.SpanIncluding( _T(" \t"));
	strParse = strParse.Mid( strSpaces.GetLength());

	/*
	 * Look for leading AND or OR terms.
	 */
	if( strParse.Left( 4).CompareNoCase( _T( "and ")) == 0) {
		bAnd = true;
		strParse = strParse.Mid( 4);
	} else if( strParse.Left( 3).CompareNoCase( _T( "or ")) == 0) {
		bAnd = false;
		strParse = strParse.Mid( 3);
	}

	/*
	 * Skip leading spaces.
	 */
	strSpaces = strParse.SpanIncluding( _T(" \t"));
	strParse = strParse.Mid( strSpaces.GetLength());

	/*
	 * Look for end of column name: whitespace or comparison.
	 */
	CString strColumn = strParse.SpanExcluding( _T("\t "));

	if( strColumn.IsEmpty()) {
		return false;
	}

	strParse = strParse.Mid( strColumn.GetLength());

	/*
	 * Skip spaces before comparison.
	 */
	strSpaces = strParse.SpanIncluding( _T(" \t"));
	strParse = strParse.Mid( strSpaces.GetLength());

	/*
	 * Parse in the comparison.
	 */
	enum ReportRelation nRelation = (enum ReportRelation)-1;
	
	for( int i=0; i < NO_FILTER_RELATIONS; i++) {
		int nLen = _tcslen( g_oReportRelations[i].m_strOp);

		if( strParse.Left( nLen).CompareNoCase( g_oReportRelations[i].m_strOp) == 0 &&
				strParse.Mid( nLen, 1).CompareNoCase( _T(" ")) == 0) {

			nRelation = g_oReportRelations[i].m_nRelation;

			strParse = strParse.Mid( nLen);
			break;
		}
	}

	if( nRelation == -1) {
		return false;
	}

	/*
	 * Skip spaces after comparison.
	 */
	strSpaces = strParse.SpanIncluding( _T(" \t"));
	strParse = strParse.Mid( strSpaces.GetLength());

	/*
	 * See if the value has a ' or # prefix
	 */
	CString strPrefix = strParse.SpanIncluding( _T("'#"));

	CString strValue;

	if( !strPrefix.IsEmpty()) {
		/*
		 * It does so the end of the value must end with the same prefix.
		 */
		if( strParse.Right(1).Compare( strPrefix.Left(1)) != 0) {
			return false;
		}

		/*
		 * Strip out the value.
		 */
		strValue = strParse.Mid( 1, strParse.GetLength() - 2);
	} else {
		strValue = strParse;
	}

	strValue.Replace( _T("''"), _T("'"));

	SetColumn( strColumn);
	SetRelation( nRelation);
	SetOr( !bAnd);  // There was a 50/50 chance of me choosing the right sense for the bool
	SetValue( strValue);

	return true;
}

/**
 * Get the sql for a filter term. This is more complex because it relies on the type of the column.
 * Bear in mind this is two way and this should be parsable by SetFromSQL
 * @param nIndex The index of the filter term.
 * @param pInfo Information about the database column.
 * @returns The SQL.
 */
 CString CReportTermFilter::GetSQL( int nIndex, CDaoFieldInfo *pInfo)
 {
 	CString strFmt;

 	switch( pInfo->m_nType) {
		case dbBoolean:
		case dbByte:
		case dbInteger:
		case dbLong:
		case dbCurrency:
		case dbSingle:
		case dbDouble:
		case dbLongBinary:
		default:
			strFmt.Format( _T("%s %s %s"), GetColumn(), GetReleationString( m_nRelation), m_strValue);
			break;

		case dbDate:
			strFmt.Format( _T("%s %s #%s#"), GetColumn(), GetReleationString( m_nRelation), m_strValue);
			break;

		case dbText:
		case dbMemo:
		case dbGUID:
			{
				CString strTmp = m_strValue;

				// Escape apostrophes in case there are any smart arses out there
				strTmp.Replace( _T("'"), _T("''"));
			
				strFmt.Format( _T("%s %s '%s'"), GetColumn(), GetReleationString( m_nRelation), strTmp);
			}
			break;
 	}

	if( nIndex > 0) {
		if( m_bOr) {
			strFmt = _T(" OR ") + strFmt;
		} else {
			strFmt = _T(" AND ") + strFmt;
		}
	}

 	return strFmt;
 }

CString CReportTermFormat::Format( int nIndex)
{
	CString strFormat;
	CString strInclude = m_bInclude ? _T("Include") : _T("Omit");
	CString strDefaultText = GetFormatDefaultText( m_nFormat);

	if( strDefaultText.IsEmpty()) {
		strFormat.Format( _T( "%s %s"), strInclude, GetFormatString( m_nFormat));
	} else {
		if( m_bInclude) {
			strFormat.Format( _T( "%s %s = %s"), strInclude, GetFormatString( m_nFormat), m_strText);
		} else {
			strFormat.Format( _T( "%s %s"), strInclude, GetFormatString( m_nFormat));
		}
	}

	return strFormat;
}

/** 
 * A factory for parsing the input string and generating appropriate graph term objects.
 * @param strParsee Objects are parsed from this string. The string is consumed from this variable.
 * Returns a null if unable to parse one.
 */
CReportTermGraph *CReportTermGraph::Parse( CString &strParsee)
{
	if( strParsee.Left( 7).CompareNoCase( _T("graph:\n")) != 0) {
		return NULL;
	}

	strParsee = strParsee.Mid( 7);

	int nType = ParseInt( _T("type"), strParsee, GRAPH_LINE);
	CString strXAxis = ParseString( _T("xaxis"), strParsee, _T(""));
	CString strYAxis = ParseString( _T("yaxis"), strParsee, _T(""));
	COLORREF nColour1 = (COLORREF)ParseInt( _T("colour1"), strParsee, RGB( 0, 139, 152));
	COLORREF nColour2 = (COLORREF)ParseInt( _T("colour2"), strParsee, RGB( 0, 0, 0));
	CString strTitle = ParseString( _T("title"), strParsee, _T(""));
	CString strTitleX = ParseString( _T("titlex"), strParsee, _T(""));

	bool bHiddenX = ParseBool( _T("hiddenx"), strParsee, false);
	bool bLogarithmicX = ParseBool( _T("logarithmicx"), strParsee, false);
	bool bReverseX = ParseBool( _T("reversedx"), strParsee, false);

	CString strTitleY = ParseString( _T("titleY"), strParsee, _T(""));

	bool bHiddenY = ParseBool( _T("hiddeny"), strParsee, false);
	bool bLogarithmicY = ParseBool( _T("logarithmicy"), strParsee, false);
	bool bReverseY = ParseBool( _T("reversedy"), strParsee, false);
	bool bPercentageY = ParseBool( _T("percentagey"), strParsee, false);

	double nMinimumX = ParseDouble( _T("minimumx"), strParsee, 0.0);
	double nMaximumX = ParseDouble( _T("maximumx"), strParsee, 0.0);
	double nOriginX = ParseDouble( _T("originx"), strParsee, 0.0);

	bool bCheckAutoMinimumX = ParseBool( _T("checkautominimumx"), strParsee, true);
	bool bCheckAutoMaximumX = ParseBool( _T("checkautomaximumx"), strParsee, true);
	bool bCheckAutoOriginX = ParseBool( _T("checkautooriginx"), strParsee, true);

	double nMinimumY = ParseDouble( _T("minimumy"), strParsee, 0.0);
	double nMaximumY = ParseDouble( _T("maximumy"), strParsee, 0.0);
	double nOriginY = ParseDouble( _T("originy"), strParsee, 0.0);

	bool bCheckAutoMinimumY = ParseBool( _T("checkautominimumy"), strParsee, true);
	bool bCheckAutoMaximumY = ParseBool( _T("checkautomaximumy"), strParsee, true);
	bool bCheckAutoOriginY = ParseBool( _T("checkautooriginy"), strParsee, true);

	if( strParsee.Left( 9).CompareNoCase( _T("endgraph\n")) != 0) {
		return NULL;
	}

	strParsee = strParsee.Mid( 9);

	return &( (new CReportTermGraph( (enum ReportGraph)nType))
						->SetTitle( strTitle)
						.SetXAxis( strXAxis)
						.SetYAxis( strYAxis)
						.SetColour1( nColour1)
						.SetColour2( nColour2)
						.SetTitleX( strTitleX)
						.SetTitleY( strTitleY)
						.SetHiddenX( bHiddenX)
						.SetLogarithmicX( bLogarithmicX)
						.SetReversedX( bReverseX)
						.SetHiddenY( bHiddenY)
						.SetLogarithmicY( bLogarithmicY)
						.SetReversedY( bReverseY)
						.SetPercentageY( bPercentageY)
						.SetMinimumX( nMinimumX)
						.SetMaximumX( nMaximumX)
						.SetOriginX( nOriginX)
						.SetAutoMinimumX( bCheckAutoMinimumX)
						.SetAutoMaximumX( bCheckAutoMaximumX)
						.SetAutoOriginX( bCheckAutoOriginX)
						.SetMinimumY( nMinimumY)
						.SetMaximumY( nMaximumY)
						.SetOriginY( nOriginY)
						.SetAutoMinimumY( bCheckAutoMinimumY)
						.SetAutoMaximumY( bCheckAutoMaximumY)
						.SetAutoOriginY( bCheckAutoOriginY)
					);

}

/**
 * Parse a string from input string. the string is assumed to be in the form:
 *    {name}={value}\n
 * @param  strName    Name of string being parsed off
 * @param  strParsee  String being parsed: the target string is removed from this
 * @param  strDefault Default value if the named item is not at the start of the parsed string
 * @return            Returns the default value if the string is not found or the value parsed.
 */
CString CReportTermGraph::ParseString( LPCTSTR strName, CString &strParsee, LPCTSTR strDefault)
{
	int nNameLen = _tcslen( strName);

	if( strParsee.GetLength() < nNameLen + 1) {
		return strDefault;
	}

	if( strParsee.Left( nNameLen).CompareNoCase( strName) != 0 ||
		strParsee.Mid( nNameLen, 1).CompareNoCase( _T("=")) != 0) {
		return strDefault;
	}

	int nIndex = strParsee.Find( _T('\n'));

	CString strValue = strParsee.Mid( nNameLen + 1, nIndex - (nNameLen + 1));
	strParsee = strParsee.Mid( nIndex + 1);

	return strValue;
}

/**
 * Parse a boolean value from an input string
 * @param  strName   See description of ParseString
 * @param  strParsee See description of ParseString
 * @param  nDefault  See description of ParseString
 * @return           See description of ParseString
 */
bool CReportTermGraph::ParseBool( LPCTSTR strName, CString &strParsee, bool nDefault)
{
	CString strValue = ParseString( strName, strParsee, nDefault ? _T("1") : _T("0"));
	LPTSTR strWhoGAF;

	return _tcstol( strValue, &strWhoGAF, 10) != 0;	
}

/**
 * Parse a double value from an input string
 * @param  strName   See description of ParseString
 * @param  strParsee See description of ParseString
 * @param  nDefault  See description of ParseString
 * @return           See description of ParseString
 */
double CReportTermGraph::ParseDouble( LPCTSTR strName, CString &strParsee, double nDefault)
{
	CString strValue = ParseString( strName, strParsee, _T("I'm the urban spaceman baby"));
	LPTSTR strWhoGAF;

	if( strValue.CompareNoCase( _T("I'm the urban spaceman baby")) == 0) {
		return nDefault;
	}

	return _tcstod( strValue, &strWhoGAF);	
}

/**
 * Parse an integer value from an input string
 * @param  strName   See description of ParseString
 * @param  strParsee See description of ParseString
 * @param  nDefault  See description of ParseString
 * @return           See description of ParseString
 */
int CReportTermGraph::ParseInt( LPCTSTR strName, CString &strParsee, int nDefault)
{
	CString strValue = ParseString( strName, strParsee, _T("I'm the urban spaceman baby"));
	LPTSTR strWhoGAF;

	if( strValue.CompareNoCase( _T("I'm the urban spaceman baby")) == 0) {
		return nDefault;
	}

	return _tcstol( strValue, &strWhoGAF, 10);	
}

/**
 * Dump graph term object in string format.
 * @return A string containing an encoded graph term object.
 */
CString CReportTermGraph::Dump()
{
	CString strOut;
	CString strFmt;

	strOut += _T("Graph:\n");
	strFmt.Format( _T( "Type=%d\n"), m_nGraph);
	strOut += strFmt;
	strFmt.Format( _T( "XAxis=%s\n"), m_strXAxis);
	strOut += strFmt;
	strFmt.Format( _T( "YAxis=%s\n"), m_strYAxis);
	strOut += strFmt;
	strFmt.Format( _T( "Colour1=%d\n"), m_nColour1);
	strOut += strFmt;
	strFmt.Format( _T( "Colour2=%d\n"), m_nColour2);
	strOut += strFmt;
	strFmt.Format( _T( "Title=%s\n"), m_strTitle);
	strOut += strFmt;
	strFmt.Format( _T( "TitleX=%s\n"), m_strTitleX);
	strOut += strFmt;
	strFmt.Format( _T( "HiddenX=%d\n"), m_bHiddenX);
	strOut += strFmt;
	strFmt.Format( _T( "LogarithmicX=%d\n"), m_bLogarithmicX);
	strOut += strFmt;
	strFmt.Format( _T( "ReversedX=%d\n"), m_bReversedX);
	strOut += strFmt;
	strFmt.Format( _T( "TitleY=%s\n"), m_strTitleY);
	strOut += strFmt;
	strFmt.Format( _T( "HiddenY=%d\n"), m_bHiddenY);
	strOut += strFmt;
	strFmt.Format( _T( "LogarithmicY=%d\n"), m_bLogarithmicY);
	strOut += strFmt;
	strFmt.Format( _T( "ReversedY=%d\n"), m_bReversedY);
	strOut += strFmt;
	strFmt.Format( _T( "PercentageY=%d\n"), m_bPercentageY);
	strOut += strFmt;

	strFmt.Format( _T( "MinimumX=%g\n"), m_nMinimumX);
	strOut += strFmt;
	strFmt.Format( _T( "MaximumX=%g\n"), m_nMaximumX);
	strOut += strFmt;
	strFmt.Format( _T( "OriginX=%g\n"), m_nOriginX);
	strOut += strFmt;

	strFmt.Format( _T( "CheckAutoMinimumX=%d\n"), m_bCheckAutoMinimumX);
	strOut += strFmt;
	strFmt.Format( _T( "CheckAutoMaximumX=%d\n"), m_bCheckAutoMaximumX);
	strOut += strFmt;
	strFmt.Format( _T( "CheckAutoOriginX=%d\n"), m_bCheckAutoOriginX);
	strOut += strFmt;

	strFmt.Format( _T( "MinimumY=%g\n"), m_nMinimumY);
	strOut += strFmt;
	strFmt.Format( _T( "MaximumY=%g\n"), m_nMaximumY);
	strOut += strFmt;
	strFmt.Format( _T( "OriginY=%g\n"), m_nOriginY);
	strOut += strFmt;

	strFmt.Format( _T( "CheckAutoMinimumY=%d\n"), m_bCheckAutoMinimumY);
	strOut += strFmt;
	strFmt.Format( _T( "CheckAutoMaximumY=%d\n"), m_bCheckAutoMaximumY);
	strOut += strFmt;
	strFmt.Format( _T( "CheckAutoOriginY=%d\n"), m_bCheckAutoOriginY);
	strOut += strFmt;

	strOut += _T("EndGraph\n");

	return strOut;
}

 /**
  * Add default terms to report.
  */
void CReportTerms::AddDefaultTerms()
{
	m_oFormats.push_back( new CReportTermFormat( FORMAT_STANDARD_HEADER, CReportTermFormat::GetFormatDefaultText( FORMAT_STANDARD_HEADER), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_REPORT_HEADER, CReportTermFormat::GetFormatDefaultText( FORMAT_REPORT_HEADER), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_DATA_GRID, CReportTermFormat::GetFormatDefaultText( FORMAT_DATA_GRID), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_WORKLOAD_LIST_SUMMARY, CReportTermFormat::GetFormatDefaultText( FORMAT_WORKLOAD_LIST_SUMMARY), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_SORT_CRITERIA_SUMMARY, CReportTermFormat::GetFormatDefaultText( FORMAT_SORT_CRITERIA_SUMMARY), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_QUERY_CONDITION_SUMMARY, CReportTermFormat::GetFormatDefaultText( FORMAT_QUERY_CONDITION_SUMMARY), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_DATA_INTEGRITY_SUMMARY, CReportTermFormat::GetFormatDefaultText( FORMAT_DATA_INTEGRITY_SUMMARY), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_REPORT_FOOTER, CReportTermFormat::GetFormatDefaultText( FORMAT_REPORT_FOOTER), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_STANDARD_FOOTER, CReportTermFormat::GetFormatDefaultText( FORMAT_STANDARD_FOOTER), true));
	m_oFormats.push_back( new CReportTermFormat( FORMAT_INCLUDE_RETESTS, CReportTermFormat::GetFormatDefaultText( FORMAT_INCLUDE_RETESTS), true));
}

/**
 * Clear all terms from this container.
 */
void CReportTerms::Clear()
{
	for( CReportColumnIterator it = m_oColumns.begin(); it != m_oColumns.end(); ++it) {
		delete *it;
	}	
	m_oColumns.clear();
	for( CReportSortIterator it = m_oSorts.begin(); it != m_oSorts.end(); ++it) {
		delete *it;
	}	
	m_oSorts.clear();
	for( CReportFilterIterator it = m_oFilters.begin(); it != m_oFilters.end(); ++it) {
		delete *it;
	}	
	m_oFilters.clear();
	for( CReportFormulaIterator it = m_oFormulas.begin(); it != m_oFormulas.end(); ++it) {
		delete *it;
	}	
	m_oFormulas.clear();
	for( CReportFormatIterator it = m_oFormats.begin(); it != m_oFormats.end(); ++it) {
		delete *it;
	}	
	m_oFormats.clear();
	for( CReportGraphIterator it = m_oGraphs.begin(); it != m_oGraphs.end(); ++it) {
		delete *it;
	}	
	m_oGraphs.clear();
}

/**
 * Determine whether the specified column is included in the report.
 * @param strColumn the name of the column
 * @returns true if it is in the report
 */
bool CReportTerms::HasColumn( LPCTSTR strColumn)
{
	for( CReportColumnIterator it = m_oColumns.begin(); it != m_oColumns.end(); ++it) {
    	if( (*it)->GetColumn().CompareNoCase( strColumn) == 0) {
    		return true;
    	}
	}	

	return false;
}

/**
 * Determine whether the specified column is being used in the report.
 * @param strColumn the name of the column
 * @returns true if it is used in the report
 */
bool CReportTerms::UsesColumn( LPCTSTR strColumn)
{
	for( CReportSortIterator it = m_oSorts.begin(); it != m_oSorts.end(); ++it) {
    	if( (*it)->GetColumn().CompareNoCase( strColumn) == 0) {
    		return true;
    	}
	}	

	for( CReportFilterIterator it = m_oFilters.begin(); it != m_oFilters.end(); ++it) {
    	if( (*it)->GetColumn().CompareNoCase( strColumn) == 0) {
    		return true;
    	}
	}	

	for( CReportFormulaIterator it = m_oFormulas.begin(); it != m_oFormulas.end(); ++it) {
    	if( (*it)->GetColumn().CompareNoCase( strColumn) == 0) {
    		return true;
    	}
	}	

	for( CReportGraphIterator it = m_oGraphs.begin(); it != m_oGraphs.end(); ++it) {
    	if( (*it)->GetXAxis().CompareNoCase( strColumn) == 0) {
    		return true;
    	}
    	if( (*it)->GetYAxis().CompareNoCase( strColumn) == 0) {
    		return true;
    	}
	}	

	return false;
}

/**
 * Add a new column to the report
 * @param strColumn the name of the column to be added
 */
void CReportTerms::AddColumn( LPCTSTR strColumn)
{
	if( HasColumn( strColumn)) {
		return;
	}

	m_oColumns.push_back( new CReportTermColumn( strColumn));
}

/**
 * Delete a column from the report.
 * @param strColumn the name of the column to delete.
 */
void CReportTerms::DeleteColumn( LPCTSTR strColumn)
{
	for( CReportColumnIterator it = m_oColumns.begin(); it != m_oColumns.end(); ++it) {
    	if( (*it)->GetColumn().CompareNoCase( strColumn) == 0) {
    		CReportTermColumn *pColumn = *it;

    		m_oColumns.erase( it);
    		delete pColumn;

    		return;
    	}
	}	
}

/**
 * Get available columns
 * @param pDocument Report document with access to all available columns
 * @param oColumns an array to be stuffed with the available columns
 * @param bGetAll if true, get all available columns, otherwise only get ones selected by user
 * @param bNumeric if true, only get columns that are numeric
 */
 void CReportTerms::GetAvailableColumns( CRptDoc *pDocument, CStringArray &oColumns, bool bGetAll, bool bNumeric)
 {
	if( bGetAll == true || m_oColumns.size() == 0) {
		pDocument->GetAvailableColumns( oColumns, bNumeric);
	} else {
		for( CReportColumnIterator it = m_oColumns.begin(); it != m_oColumns.end(); ++it) {
			if( bNumeric == false || pDocument->IsColumnNumeric( (*it)->GetColumn())) {
				oColumns.Add( (*it)->GetColumn());
			}
		}
	}
}

/**
 * Delete specified term from the report.
 * @param nType the type of term to delete.
 * @param nIndex the index of the term to be deleted.
 * @returns true on success
 */
bool CReportTerms::DeleteTerm( enum ReportTermType nType, int nIndex)
{
	switch( nType) {
		case TERM_COLUMN:
			{
				CReportTermColumn *pColumn = m_oColumns[nIndex];

				m_oColumns.erase( m_oColumns.begin() + nIndex);

				delete pColumn;
			}
			return true;

		case TERM_SORT:
			{
				CReportTermSort *pSort = m_oSorts[nIndex];

				m_oSorts.erase( m_oSorts.begin() + nIndex);

				delete pSort;
			}
			return true;
			
		case TERM_FILTER:
			{
				CReportTermFilter *pFilter = m_oFilters[nIndex];

				m_oFilters.erase( m_oFilters.begin() + nIndex);

				delete pFilter;
			}
			return true;
			
		case TERM_FORMULA:
			{
				CReportTermFormula *pFormula = m_oFormulas[nIndex];

				m_oFormulas.erase( m_oFormulas.begin() + nIndex);

				delete pFormula;
			}
			return true;
			
		case TERM_FORMAT:
			{
				CReportTermFormat *pFormat = m_oFormats[nIndex];

				m_oFormats.erase( m_oFormats.begin() + nIndex);

				delete pFormat;
			}
			return true;
			
		case TERM_GRAPH:
			{
				CReportTermGraph *pGraph = m_oGraphs[nIndex];

				m_oGraphs.erase( m_oGraphs.begin() + nIndex);

				delete pGraph;
			}
			return true;
			
		default:
			break;
	}

	return false;
}

/**
 * Return a pointer to a specified report term.
 * @Param The type of report term
 * @param The index of the report term
 * @returns a pointer to the item or NULL if illegal.
 */
CReportTerm *CReportTerms::GetTerm( enum ReportTermType nType, int nIndex)
{
	switch( nType) {
		case TERM_COLUMN:
			return m_oColumns[nIndex];

		case TERM_SORT:
			return m_oSorts[nIndex];
			
		case TERM_FILTER:
			return m_oFilters[nIndex];
			
		case TERM_FORMULA:
			return m_oFormulas[nIndex];
			
		case TERM_FORMAT:
			return m_oFormats[nIndex];
			
		case TERM_GRAPH:
			return m_oGraphs[nIndex];
			
		default:
			break;
	}

	return NULL;
}

/**
 * Test whether the specified formatting option has been selected.
 * @param  nFormat The formatting option being tested for.
 * @return         true if it has been asked for.
 */
bool CReportTerms::HasFormat( enum ReportFormat nFormat)
{
	for( CReportFormatIterator oIt = m_oFormats.begin();
			oIt != m_oFormats.end();
			oIt++) {
		if( (*oIt)->GetFormat() == nFormat) {
			return (*oIt)->GetInclude();
		}
	}

	return true;
}

/**
 * Return the value associated with the specified formatting option.
 * @param  nFormat The formatting option to get a value for.
 * @return         The value associated with the formatting option.
 */
CString CReportTerms::GetFormatValue( enum ReportFormat nFormat)
{
	for( CReportFormatIterator oIt = m_oFormats.begin();
			oIt != m_oFormats.end();
			oIt++) {
		if( (*oIt)->GetFormat() == nFormat) {
			return (*oIt)->GetText();
		}
	}

	return _T("");
}

void CReportTerms::ParseGraphs( CString &strParse)
{
	while( true) {
		CReportTermGraph *pGraph = CReportTermGraph::Parse( strParse);
		if( pGraph == NULL) {
			break;
		}
		m_oGraphs.push_back( pGraph);
	}
}

CString CReportTerms::DumpGraphs()
{
	CString strBuild;

	for( CReportGraphIterator oIt = m_oGraphs.begin(); oIt != m_oGraphs.end(); oIt++) {
		strBuild += (*oIt)->Dump();
	}

	return strBuild;
}
