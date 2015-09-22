// ***************************************************************
//  AppStringTextNodeParser   version:  1.0  - date: 08/10/2007
//  -------------------------------------------------------------
//  Parses a text node of the app-strings XML file
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "StdAfx.h"
#include "AppStringTextNodeParser.h"
#include "StringConversions.h"

/*!

 */
CAppStringTextNodeParser::CAppStringTextNodeParser( LPCWSTR node_text ) :
	m_reader( node_text )
{
}

/*!
 */
CAppStringTextNodeParser::~CAppStringTextNodeParser(void)
{
}

/*!

 */
const wstring CAppStringTextNodeParser::get_node_string(wstring tag_name)
{
	m_reader.rewind() ;

	wstring start_tag = L"<" + tag_name + L">" ;
	if ( ! m_reader.find( start_tag.c_str(), true ) ) 
	{
		return wstring();
	}

	reader_type::bookmark_type pos_start = m_reader.get_current_pos() ;

	wstring end_tag = L"</" + tag_name + L">" ;
	m_reader.find( end_tag.c_str(), false ) ;

	reader_type::bookmark_type pos_end = m_reader.get_current_pos() ;

	wstring dirty_string( pos_start, pos_end ) ;

	return clean_up_string( dirty_string ) ;
}

/*!

 */
wstring CAppStringTextNodeParser::clean_up_string(const wstring& dirty_string)
{
	wstring clean_string = dirty_string ;
	boost::replace_all( clean_string, L"&gt;",  L">" ) ;
	boost::replace_all( clean_string, L"&lt;",  L"<" ) ;
	boost::replace_all( clean_string, L"&amp;", L"&" ) ;
	boost::replace_all( clean_string, L"\\t",   L"\t" ) ;
	boost::replace_all( clean_string, L"\\n",   L"\n" ) ;
	boost::replace_all( clean_string, L"\\r",   L"\r" ) ;

	return clean_string ;
}


