#pragma once

#include "ProgressListener.h"
#include "TranslationMemory.h"

using memory_engine::memory_pointer ;
using memory_engine::memory_local ;

class CImportMultitermFile
{
	CProgressListener *m_listener ;

public:
	memory_pointer	   m_memory ;

	CImportMultitermFile(CProgressListener *listener);
	~CImportMultitermFile(void);

	void import( const CString &file_name );

	file::file::BYTE_ORDER_MARK get_file_bom( const CString & file_name );
	void import_multiterm55( const CString &file_name );
	void import_multiterm_55_text(textstream_reader< char >& reader, const std::string& source_lang, const std::string& trans_lang, memory_engine::memory_pointer& mem);
	void get_multiterm55_line( textstream_reader< char > &reader, 
		string &col1, 
		string &col2 );

	void import_multiterm6(const CString &file_name);
	void import_multiterm_6_text(LPCWSTR file_text);
	size_t get_num_lines(LPCWSTR file_text);
	void get_multiterm6_line( textstream_reader< wchar_t > &reader, 
		wstring &col1, 
		wstring &col2, 
		wstring &col3 );

};
