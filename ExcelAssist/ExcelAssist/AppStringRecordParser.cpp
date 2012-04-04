#include "StdAfx.h"
#include "AppStringRecordParser.h"
#include "Exceptions.h"

/*!
 * \brief
 * Constructor - Takes file text and initializes reader to it.
  */
CAppStringRecordParser::CAppStringRecordParser( LPCWSTR file_text ) :
	m_reader( file_text )
{
}



/*!
 * \brief
 * Write brief comment for had_more_records here.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for had_more_records here.
 * 
 * \remarks
 * Write remarks for had_more_records here.
 * 
 * \see
 * Separate items with the '|' character.
 */
bool CAppStringRecordParser::had_more_records(void)
{
	return m_reader.find( L"<record>" ) ;
}

/*!
 */
app_str_entry_ptr CAppStringRecordParser::get_next_record(void)
{
	m_reader.find( L"<record>", true ) ;
	if ( m_reader.empty() ) 
	{
		THROW_PROGRAM_EXCEPTION( _T("Reader must not be empty") ) ;
	}
	reader_type::bookmark_type pos_start = m_reader.get_current_pos() ;
	m_reader.find( L"</record>", false ) ;
	reader_type::bookmark_type pos_end = m_reader.get_current_pos() ;
	
	wstring node_text( pos_start, pos_end ) ;

	CAppStringTextNodeParser node_parser( node_text.c_str() ) ;

	app_str_entry_ptr entry(new CAppStringEntry) ;
	std::vector<wstring> keys ;
	keys.push_back(L"text") ;
	keys.push_back(L"tooltip") ;
	keys.push_back(L"id") ;
	keys.push_back(L"trans") ;
	keys.push_back(L"trans_tt") ;
	foreach(wstring key, keys)
	{
		entry->set_entry( key, node_parser.get_node_string( key ) ) ;
	}

	return entry ;
}

