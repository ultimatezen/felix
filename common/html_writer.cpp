#include "stdafx.h"
#include "html_writer.h"


// =====================
// base functions
// =====================

void html_writer::write_html( const wstring html_text )
{
	// &#32; entities will keep us from reading tags
	wstring local(html_text) ;
	boost::replace_all(local, L"&#32;", L" ") ;

	m_parser.set_buffer(local); 

	m_parser.push_defaults() ;

	while ( m_parser.empty() == false )
	{
		m_parser.parse_text() ;
		m_parser.parse_tag() ;
	}

	m_parser.restore_original_states() ;
}
