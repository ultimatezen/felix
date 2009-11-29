#pragma once

#include "textstream_reader.h"
#include <map>
#include "stringex.h"

class CTmxSymbolMapper
{
	std::map< wstring, wchar_t > m_symbols ;
	
public:
	bool exists( const wstring &key );
	const wchar_t get_val( const wstring &key );
	CTmxSymbolMapper(void);
	~CTmxSymbolMapper(void);

	void handleAmp( wc_reader &reader, wstring &stripped_text );

	wstring strip_html_symbols( const wstring &raw_wstring );
};
