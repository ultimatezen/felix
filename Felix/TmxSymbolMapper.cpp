#include "StdAfx.h"
#include "TmxSymbolMapper.h"
#include "tag_stripper.h"

CTmxSymbolMapper::CTmxSymbolMapper(void)
{
	const wstring lt(L"lt") ;
	const wstring gt(L"gt") ;
	const wstring amp(L"amp") ;
	ATLASSERT(m_symbols.exists(lt)) ;
	ATLASSERT(m_symbols.exists(gt)) ;
	ATLASSERT(m_symbols.exists(amp)) ;

	m_symbols.erase(lt) ;
	m_symbols.erase(gt) ;
	m_symbols.erase(amp) ;

	ATLASSERT(!m_symbols.exists(lt)) ;
	ATLASSERT(!m_symbols.exists(gt)) ;
	ATLASSERT(!m_symbols.exists(amp)) ;
}

CTmxSymbolMapper::~CTmxSymbolMapper(void)
{
}

wstring CTmxSymbolMapper::strip_html_symbols( const wstring raw_wstring )
{
	typedef textstream_reader< wchar_t > reader_type ;
	typedef textstream_reader< wchar_t >::bookmark_type bm_type ;

	reader_type reader ;

	reader.set_buffer( raw_wstring.c_str() ) ;

	wstring stripped_text ;
	stripped_text.reserve( raw_wstring.size() ) ;

	while ( reader.empty() == false ) 
	{
		const wstring chunk = reader.getline(L'&', false ) ;
		if ( chunk.empty() == false )
		{
			stripped_text += chunk ;
		}

		if ( reader.peek() == L'&' )
		{
			handleAmp( reader, stripped_text ) ;
		}
	}

	return stripped_text ;
}

void CTmxSymbolMapper::handleAmp( wc_reader &reader, wstring &stripped_text )
{
	stripped_text += handle_ampersand(reader, this->m_symbols) ;
}

bool CTmxSymbolMapper::exists( const wstring key ) const
{
	return m_symbols.exists(key) ;
}

const wchar_t CTmxSymbolMapper::get_val( const wstring key ) const
{
	return m_symbols.get_val(key) ;
}