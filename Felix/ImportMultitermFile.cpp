#include "StdAfx.h"
#include "ImportMultitermFile.h"
#include "ImportDialog.h"
#include "memory_local.h"
#include "record.h"
#include "record_local.h"

CImportMultitermFile::CImportMultitermFile(CProgressListener *listener) :
	m_listener(listener),
	m_memory(new memory_engine::memory_local())
{
}

CImportMultitermFile::~CImportMultitermFile(void)
{
}

/** Import a multiterm version 5.5 file
*/
void CImportMultitermFile::import( const CString &file_name )
{
	ensure_file_exists(file_name) ;

	// multiterm 6 file?
	if ( get_file_bom(file_name) == file::file::LE_BOM )
	{
		import_multiterm6( file_name ) ;
	}
	else // ok, it's a multiterm 5 file
	{
		import_multiterm55( file_name ) ;
	}

	m_listener->OnProgressDoneLoad(m_memory->size()) ;
}

//! Get the byte order mark of the file.
//! We use this to tell if it's a multiterm-6.0 glossary
//! (multiterm 6.0 files will be UTF-16 with LE BOM)
const file::file::BYTE_ORDER_MARK CImportMultitermFile::get_file_bom( const CString & file_name ) const
{
	file::file import_file ;
	import_file.open_existing( file_name ) ;
	const file::file::BYTE_ORDER_MARK bom = import_file.get_bom() ;
	import_file.close() ;
	return bom ;
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
	while ( reader.empty() == false )
	{
		const strcols cols = get_multiterm55_line(reader) ;
		if ( cols.get<0>() != "Notes" && cols.get<1>().find("***") == string::npos) 
		{
			languages.insert( string2tstring( cols.get<0>() ) ) ;
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
											   memory_engine::memory_pointer& mem )
{

	memory_engine::memory_header header = m_memory->get_header() ;
	header.set_source_language(string2wstring(source_lang)) ;
	header.set_target_language(string2wstring(trans_lang)) ;
	mem->set_header(header) ;

	memory_engine::record_pointer record(new memory_engine::record_local()) ;
	while ( reader.empty() == false )
	{
		const strcols cols = get_multiterm55_line(reader) ;
		if ( cols.get<0>() == source_lang )
		{
			record->set_source( massage_line(cols.get<1>()) ) ;
		}
		else if ( cols.get<0>() == trans_lang )
		{
			record->set_trans( massage_line(cols.get<1>()) ) ;
		}
		else if ( cols.get<0>() == "Notes" )
		{
			record->set_context( massage_line(cols.get<1>()) ) ;
		}

		if ( record->is_valid_record() )
		{
			mem->add_record( record ) ;
			record = memory_engine::record_pointer(new memory_engine::record_local()) ;
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
	const wstrcols first_line = get_multiterm6_line(reader) ;

	std::set<tstring> languages ;
	languages.insert( string2tstring( first_line.get<0>() ) ) ;
	languages.insert( string2tstring( first_line.get<1>() ) ) ;

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
	if ( first_line.get<0>() == string2wstring( source_lang ) )
	{
		source_is_first_column = true ;

		sourcelang = first_line.get<0>() ;
		translang = first_line.get<1>() ;
	}
	else
	{
		source_is_first_column = false ;

		sourcelang = first_line.get<1>() ;
		translang = first_line.get<0>() ;
	}
	memory_engine::memory_header header = m_memory->get_header() ;
	header.set_source_language(sourcelang) ;
	header.set_target_language(translang) ;
	m_memory->set_header(header) ;

	while ( reader.empty() == false )
	{
		const wstrcols cols = get_multiterm6_line(reader) ;
		memory_engine::record_pointer record(new memory_engine::record_local()) ;
		if ( source_is_first_column )
		{
			record->set_source( massage_line(cols.get<0>()) ) ;
			record->set_trans( massage_line(cols.get<1>()) ) ;
		}
		else
		{
			record->set_source( massage_line(cols.get<1>()) ) ;
			record->set_trans( massage_line(cols.get<0>()) ) ;
		}
		record->set_context( massage_line(cols.get<2>()) ) ;
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

const wstring CImportMultitermFile::massage_line( const wstring &line ) const
{
	wstring tmp(line) ;
	str::replace_all( tmp, L"&", L"&amp;" ) ;
	str::replace_all( tmp, L"<", L"&lt;" ) ;
	str::replace_all( tmp, L">", L"&gt;" ) ;
	return tmp ;
}
const wstring CImportMultitermFile::massage_line( const string &line ) const
{
	string tmp(line) ;
	str::replace_all( tmp, "&", "&amp;" ) ;
	str::replace_all( tmp, "<", "&lt;" ) ;
	str::replace_all( tmp, ">", "&gt;" ) ;
	return string2wstring(tmp) ;
}
