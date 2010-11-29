// RTFParser.h: interface for the RTFParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTFPARSER_H__B98CB128_B462_4FA9_BA79_BEC12252CD58__INCLUDED_)
#define AFX_RTFPARSER_H__B98CB128_B462_4FA9_BA79_BEC12252CD58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parser.h"
#include "StringEx.h"
#include <vector>
#include "rtf.h"
#include "rtf_preamble.h"

namespace rtf
{
	typedef std::vector< COLORREF > color_table ;
	
	class RTFParser : public Parser
	{
		rtf_preamble	m_preamble ;
		font_table		m_fonts ;
		color_table		m_colors ;
		
		c_reader		m_reader ;
		
	public:
		COLORREF parse_color_table_entry( const string &entry );
		bool parse_body();
		bool parse_preamble();
		bool parse_color_table( );
		bool parse_font_table( );
		bool parse_rtf_stream( LPCSTR stream_text );
		RTFParser();
		virtual ~RTFParser();
		
	};
}

#endif // !defined(AFX_RTFPARSER_H__B98CB128_B462_4FA9_BA79_BEC12252CD58__INCLUDED_)
