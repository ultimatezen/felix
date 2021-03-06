#pragma once

#include "ProgressListener.h"
#include "TranslationMemory.h"
#include "memory_local.h"
#include "input_device.h"

typedef boost::tuple<string, string> strcols ;
typedef boost::tuple<wstring, wstring, wstring> wstrcols ;

class CImportMultitermFile
{
	CProgressListener *m_listener ;

public:
	mem_engine::memory_pointer	m_memory ;
	app_props::props_ptr		m_props ;

	CImportMultitermFile(CProgressListener *listener,
						app_props::props_ptr props);
	~CImportMultitermFile(void);

	void import( const CString &file_name, input_device_ptr input );

	void import_multiterm55( const CString &file_name );
	void import_multiterm_55_text(textstream_reader< char >& reader, const std::string& source_lang, const std::string& trans_lang, mem_engine::memory_pointer& mem);
	const strcols get_multiterm55_line(textstream_reader< char > &reader) const;
	const wstring massage_line(const string &line) const;

	void import_multiterm6(const CString &file_name);
	void import_multiterm_6_text(LPCWSTR file_text);
	const wstring massage_line(const wstring line) const;
	const size_t get_num_lines(LPCWSTR file_text) const;
	const wstrcols get_multiterm6_line(textstream_reader< wchar_t > &reader) const;

};
