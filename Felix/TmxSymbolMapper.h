#pragma once

#include "textstream_reader.h"
#include <map>
#include "stringex.h"
#include "tag_stripper.h"

class CTmxSymbolMapper
{
	symbol_map m_symbols ;
	
public:
	bool exists( const wstring &key ) const ;
	const wchar_t get_val( const wstring &key ) const ;
	CTmxSymbolMapper(void);
	~CTmxSymbolMapper(void);

	void handleAmp( wc_reader &reader, wstring &stripped_text );

	wstring strip_html_symbols( const wstring &raw_wstring );
};
