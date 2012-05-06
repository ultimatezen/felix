#include "StdAfx.h"
#include "ImportMultitermFile.h"
#include "ImportDialog.h"
#include "memory_local.h"
#include "record.h"
#include "record_local.h"
#include "input_device_file.h"


CImportMultitermFile::CImportMultitermFile(CProgressListener *listener,
										   app_props::props_ptr props) :
	m_listener(listener),
	m_props(props),
	m_memory(new mem_engine::memory_local(props))
{
}

CImportMultitermFile::~CImportMultitermFile(void)
{
}

/** Import a multiterm version 5.5 file
*/
void CImportMultitermFile::import( const CString &file_name, input_device_ptr input )
{
	input->ensure_file_exists(file_name);

	// multiterm 6 file?
	if ( input->get_file_bom(file_name) == file::BOM_LE )
	{
		import_multiterm6( file_name ) ;
	}
	else // ok, it's a multiterm 5 file
	{
		import_multiterm55( file_name ) ;
	}

	m_listener->OnProgressDoneLoad(m_memory->size()) ;
}



//! Import a MultiTerm version 5.5 glossary.
void CImportMultitermFile::import_multiterm55( const CString &file_name )
{
	file::view glossary_view ;
	LPCSTR file_text = (LPCSTR) glossary_view.create_view_readonly( file_name ) ;
	ATLASSERT( ! IsBadStringPtrA( file_text, strlen( file_text ) ) ) ;

	// set up our reader
	textstream_reader< char > reader ;
	reader.set_buffer( file_text ) ;


	CWaitCursor wait_cursor ;

	int num_lines = 0 ;
	std::set< tstring > languages ;
	string col1, col2 ;
	while ( reader.empty() == false )
	{
		boost::tie(col1, col2) = get_multiterm55_line(reader) ;
		if ( col1 != "Notes" && col2.find("***") == string::npos) 
		{
			languages.insert( string2tstring( col1 ) ) ;
		}
		++num_lines ;
	}
	if ( num_lines )
	{
		--num_lines ;
	}

	wait_cursor.Restore() ;

	CImportDialog dialog ;
	dialog.set_languages( languages ) ;
	if ( IDCANCEL == dialog.DoModal() )
	{
		throw except::CException( IDS_IMPORT_ABORTED ) ;
	}

	const string source_lang = string2string( dialog.get_source_plain() ) ;
	const string trans_lang = string2string( dialog.get_trans_plain() ) ;

	reader.rewind() ;

	m_memory->set_location(file_name) ;

	m_listener->OnProgressInit( file_name, 0, num_lines / 2 ) ;

	import_multiterm_55_text(reader, source_lang, trans_lang, m_memory);
}

/** Import a multiterm version 5.5 file given the file text,
* source language, and target language.
*/
void CImportMultitermFile::import_multiterm_55_text( textstream_reader< char >& reader, 
											   const std::string& source_lang, 
											   const std::string& trans_lang, 
											   mem_engine::memory_pointer& mem )
{

	mem_engine::MemoryInfo *mem_info = m_memory->get_memory_info() ;
	mem_info->set_source_language(string2wstring(source_lang)) ;
	mem_info->set_target_language(string2wstring(trans_lang)) ;

	mem_engine::record_pointer record(new mem_engine::record_local()) ;
	string col1, col2 ;
	while ( reader.empty() == false )
	{
		boost::tie(col1, col2) = get_multiterm55_line(reader) ;
		if ( col1 == source_lang )
		{
			record->set_source( massage_line(col2) ) ;
		}
		else if ( col1 == trans_lang )
		{
			record->set_trans( massage_line(col2) ) ;
		}
		else if ( col1 == "Notes" )
		{
			record->set_context( massage_line(col2) ) ;
		}

		if ( record->is_valid_record() )
		{
			mem->add_record( record ) ;
			record = mem_engine::record_pointer(new mem_engine::record_local()) ;
		}

		m_listener->OnProgressLoadUpdate( mem->size() ) ;
	}
}

//! Load a multiterm version 5.5 line.
const strcols CImportMultitermFile::get_multiterm55_line(textstream_reader< char > &reader) const
{
	ATLASSERT( ! reader.empty() ) ;

	reader.find( "<", true ) ;
	const string col1 = reader.getline('>', true) ;
	const string col2 = reader.getline_delims("\r\n", true ) ;
	reader.eat_whitespace() ;
	return strcols(col1, col2) ;
}

//! Import a multiterm version 6.0 file.
void CImportMultitermFile::import_multiterm6( const CString &file_name )
{
	file::view glossary_view ;
	LPCWSTR file_text = (LPCWSTR) glossary_view.create_view_readonly( file_name ) ;
	ATLASSERT( ! IsBadStringPtrW( file_text, wcslen( file_text ) ) ) ;
	// make sure it is Unicode text
	TRUE_ENFORCE( 0xFEFF == file_text[0],  IDS_NOT_UNICODE_FILE ) ;

	m_memory->set_location(file_name) ;
	m_listener->OnProgressInit( file_name, 
				0, 
				get_num_lines(file_text+1) / 2 ) ;

	// eat that first marker char
	import_multiterm_6_text(file_text+1);
}

//! Import a multiterm 6 file from a text buffer
void CImportMultitermFile::import_multiterm_6_text(LPCWSTR file_text)
{
	// set up our reader
	textstream_reader< wchar_t > reader ;
	reader.set_buffer( file_text ) ; 

	CWaitCursor wait_cursor ;

	int num_lines = 0 ;
	while ( reader.empty() == false )
	{
		reader.jump_to_first_of( L"\r\n", true ) ;
		reader.eat_whitespace() ;
		++num_lines ;
	}

	wait_cursor.Restore() ;

	reader.rewind() ;

	// eat the first line (just declares column headings)
	wstring first_line_col1, first_line_col2 ;
	boost::tie(first_line_col1, first_line_col2, boost::tuples::ignore) = get_multiterm6_line(reader) ;

	std::set<tstring> languages ;
	languages.insert( string2tstring(first_line_col1) ) ;
	languages.insert( string2tstring(first_line_col2) ) ;

#ifdef UNIT_TEST
	tstring source_lang = *(languages.begin()) ;
#else
	CImportDialog dialog ;
	dialog.set_languages( languages ) ;
	if ( IDCANCEL == dialog.DoModal() )
	{
		throw except::CException(IDS_IMPORT_ABORTED) ;
	}
	tstring source_lang = dialog.get_source_plain() ;
#endif

	wstring sourcelang, translang ;
	bool source_is_first_column( true ) ;
	if ( first_line_col1 == string2wstring( source_lang ) )
	{
		source_is_first_column = true ;

		sourcelang = first_line_col1 ;
		translang = first_line_col2 ;
	}
	else
	{
		source_is_first_column = false ;

		sourcelang = first_line_col2 ;
		translang = first_line_col1 ;
	}
	mem_engine::MemoryInfo *mem_info = m_memory->get_memory_info() ;
	mem_info->set_source_language(sourcelang) ;
	mem_info->set_target_language(translang) ;

	wstring col1, col2, col3 ;
	while ( reader.empty() == false )
	{
		boost::tie(col1, col2, col3) = get_multiterm6_line(reader) ;
		mem_engine::record_pointer record(new mem_engine::record_local()) ;
		if ( source_is_first_column )
		{
			record->set_source( massage_line(col1) ) ;
			record->set_trans( massage_line(col2) ) ;
		}
		else
		{
			record->set_source( massage_line(col2) ) ;
			record->set_trans( massage_line(col1) ) ;
		}
		record->set_context( massage_line(col3) ) ;
		record->create() ;

		if ( record->is_valid_record() ) 
		{
			m_memory->add_record( record ) ;

			m_listener->OnProgressLoadUpdate( m_memory->size() ) ;
		}
	}
}


//! Load a multiterm version 6.0 line.
const wstrcols CImportMultitermFile::get_multiterm6_line( textstream_reader< wchar_t > &reader) const
{
	ATLASSERT( ! reader.empty() ) ;

	const wstring col1 = reader.getline(L'\t', true ) ;
	reader.eat_whitespace() ;
	const wstring col2 = reader.getline(L'\t', true ) ;
	reader.eat_whitespace() ;
	const wstring col3 = reader.getline_delims(L"\n\r", true ) ;
	reader.eat_whitespace() ;
	return wstrcols(col1, col2, col3) ;
}

const size_t CImportMultitermFile::get_num_lines( LPCWSTR file_text ) const
{
	// set up our reader
	textstream_reader< wchar_t > reader ;
	reader.set_buffer( file_text ) ; 

	CWaitCursor wait_cursor ;

	size_t num_lines = 0 ;
	while ( reader.empty() == false )
	{
		reader.jump_to_first_of( L"\r\n", true ) ;
		reader.eat_whitespace() ;
		++num_lines ;
	}

	wait_cursor.Restore() ;
	return num_lines ;
}

const wstring CImportMultitermFile::massage_line( const wstring line ) const
{
	wstring tmp(line) ;
	boost::replace_all( tmp, L"&", L"&amp;" ) ;
	boost::replace_all( tmp, L"<", L"&lt;" ) ;
	boost::replace_all( tmp, L">", L"&gt;" ) ;
	return tmp ;
}
const wstring CImportMultitermFile::massage_line( const string &line ) const
{
	string tmp(line) ;
	boost::replace_all( tmp, "&", "&amp;" ) ;
	boost::replace_all( tmp, "<", "&lt;" ) ;
	boost::replace_all( tmp, ">", "&gt;" ) ;
	return string2wstring(tmp) ;
}
