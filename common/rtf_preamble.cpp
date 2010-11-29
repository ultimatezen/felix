// rtf_preamble.cpp: implementation of the rtf_preamble class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rtf_preamble.h"
#include "StringEx.h"
#include "StringConversions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace rtf
{
	rtf_preamble::rtf_preamble() :
		m_default_lang(0),
		m_default_asian_lang(0),
		m_default_codepage(0)
	{
		
	}
	
	rtf_preamble::~rtf_preamble()
	{
		
	}

	bool rtf_preamble::parse_element(const string &element)
	{
		LPCSTR element_text = element.c_str() ;

		if ( ! _strnicmp( element_text, "deflangfe", strlen("deflangfe") ) )
		{
			string num_text = element_text + strlen( "deflangfe" ) ;
			m_default_asian_lang = string2long( num_text ) ;
			return true ;
		}
		if ( ! _strnicmp( element_text, "deflang", strlen("deflang") ) )
		{
			string num_text = element_text + strlen( "deflang" ) ;
			m_default_lang = string2long( num_text ) ;
			return true ;
		}
		if ( ! _strnicmp( element_text, "ansicpg", strlen("ansicpg") ) )
		{
			string num_text = element_text + strlen( "ansicpg" ) ;
			m_default_codepage = string2long( num_text ) ;
			return true ;
		}

		return false ; // we did not parse it
	}

}

