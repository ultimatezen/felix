#include "StdAfx.h"
#include "ImportMultitermFile.h"
#include "ImportDialog.h"
#include "record.h"

CImportMultitermFile::CImportMultitermFile(CProgressListener *listener) :
	m_listener(listener),
	m_memory(new memory_local())
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
file::file::BYTE_ORDER_MARK CImportMultitermFile::get_file_bom( const CString & file_name )
{
	file::file import_file ;
	import_file.open_existing( file_name ) ;
	file::file::BYTE_ORDER_MARK bom = import_file.get_bom() ;
	import_file.close() ;
	return bom ;
}

//! Import a MultiTerm version 5.5 glossary.
void CImportMultitermFile::import_multiterm55( const CString &file_name )
{
	file::view glossary_view ;
	LPCSTR file_text = (LPCSTR) glossary_view.create_view( file_name ) ;
	ATLASSERT( ! IsBadStringPtrA( file_text, strlen( file_text ) ) ) ;

	// set up our reader
	textstream_reader< char > reader ;
	reader.set_buffer( file_text ) ;

	string col1, col2 ;

	int num_lines = 0 ;

	CWaitCursor wait_cursor ;

	std::set< tstring > languages ;
	while ( reader.empty() == false )
	{
		get_multiterm55_line( reader, col1, col2 ) ;
		if ( col1 != "Notes" && col1.find("***") == string::npos) 
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

	string source_lang = string2string( dialog.get_source_plain() ) ;
	string trans_lang = string2string( dialog.get_trans_plain() ) ;

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

	string col1, col2 ;
	memory_engine::record_pointer record(new memory_engine::record_local()) ;
	while ( reader.empty() == false )
	{
		get_multiterm55_line( reader, col1, col2 ) ;
		str::replace_all( col1, "&", "&amp;" ) ;
		str::replace_all( col1, "<", "&lt;" ) ;
		str::replace_all( col2, "&", "&amp;" ) ;
		str::replace_all( col2, "<", "&lt;" ) ;
		if ( col1 == source_lang )
		{
			record->set_source( string2wstring( col2 ) ) ;
		}
		else if ( col1 == trans_lang )
		{
			record->set_trans( string2wstring( col2 ) ) ;
		}
		else if ( col1 == "Notes" )
		{
			record->set_context( string2wstring( col2 ) ) ;
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
void CImportMultitermFile::get_multiterm55_line( textstream_reader< char > &reader, string &col1, string &col2 )
{
	ATLASSERT( ! reader.empty() ) ;

	reader.find( "<", true ) ;
	reader.getline( col1, '>', true ) ;
	reader.getline( col2, "\r\n", true ) ;
	reader.eat_whitespace() ;
}

//! Import a multiterm version 6.0 file.
void CImportMultitermFile::import_multiterm6( const CString &file_name )
{
	file::view glossary_view ;
	LPCWSTR file_text = (LPCWSTR) glossary_view.create_view( file_name ) ;
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

	wstring col1, col2, col3 ;
	// eat the first line (just declares column headings)
	get_multiterm6_line( reader, col1, col2, col3 ) ;

	std::set<tstring> languages ;
	languages.insert( string2tstring( col1 ) ) ;
	languages.insert( string2tstring( col2 ) ) ;

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
	if ( col1 == string2wstring( source_lang ) )
	{
		source_is_first_column = true ;

		sourcelang = col1 ;
		translang = col2 ;
	}
	else
	{
		source_is_first_column = false ;

		sourcelang = col2 ;
		translang = col1 ;
	}
	memory_engine::memory_header header = m_memory->get_header() ;
	header.set_source_language(sourcelang) ;
	header.set_target_language(translang) ;
	m_memory->set_header(header) ;

	while ( reader.empty() == false )
	{
		get_multiterm6_line( reader, col1, col2, col3 ) ;
		str::replace_all( col1, L"&", L"&amp;" ) ;
		str::replace_all( col1, L"<", L"&lt;" ) ;
		str::replace_all( col2, L"&", L"&amp;" ) ;
		str::replace_all( col2, L"<", L"&lt;" ) ;
		str::replace_all( col3, L"&", L"&amp;" ) ;
		str::replace_all( col3, L"<", L"&lt;" ) ;
		memory_engine::record_pointer record(new memory_engine::record_local()) ;
		if ( source_is_first_column )
		{
			record->set_source( col1 ) ;
			record->set_trans( col2 ) ;
		}
		else
		{
			record->set_source( col2 ) ;
			record->set_trans( col1 ) ;
		}
		record->set_context( col3 ) ;
		record->create() ;

		if ( record->is_valid_record() ) 
		{
			m_memory->add_record( record ) ;

			m_listener->OnProgressLoadUpdate( m_memory->size() ) ;
		}
	}
}


//! Load a multiterm version 6.0 line.
void CImportMultitermFile::get_multiterm6_line( textstream_reader< wchar_t > &reader, wstring &col1, wstring &col2, wstring &col3 )
{
	ATLASSERT( ! reader.empty() ) ;

	reader.getline( col1, L'\t', true ) ;
	reader.eat_whitespace() ;
	reader.getline( col2, L'\t', true ) ;
	reader.eat_whitespace() ;
	reader.getline( col3, L"\n\r", true ) ;
	reader.eat_whitespace() ;
}

size_t CImportMultitermFile::get_num_lines( LPCWSTR file_text )
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