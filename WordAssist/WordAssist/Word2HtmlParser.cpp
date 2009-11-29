// Word2HtmlParser.cpp : Implementation of CWord2HtmlParser

#include "stdafx.h"
#include "Word2HtmlParser.h"
#include "WordParser.h"
#include "logging.h"
// CWord2HtmlParser

STDMETHODIMP CWord2HtmlParser::RangeToHtml(IDispatch *word_range, 
											  BSTR* html_text)
{
	try
	{
		word_sel_ptr selection_ptr = word_range ;

		WordSelection selection = selection_ptr ;

		boost::shared_ptr<WordParser> parser = boost::shared_ptr<WordParser>(new WordParser( selection )) ;

		if ( m_plaintext )
		{
			_bstr_t text = parser->segment_as_plaintext();
			*html_text = ::SysAllocStringLen(text, text.length()) ;
			return S_OK ;
		}

		app_state properties ;
		properties.read_from_registry() ;

		parser->set_parse_font_face( properties.get_font_face() ) ;
		parser->set_parse_font_color( properties.get_font_color() ) ;
		parser->set_parse_font_bold( properties.get_font_bold() ) ;
		parser->set_parse_font_italic( properties.get_font_italic() ) ;
		parser->set_parse_font_underline( properties.get_font_underline() ) ;
		parser->set_parse_font_super_sub( properties.get_font_super_sub() ) ;

		_bstr_t text = parser->segment_as_html() ;
		*html_text = ::SysAllocStringLen(text, text.length()) ;

		try
		{
			parser = boost::shared_ptr<WordParser>() ;
		}
		catch (CException& e)
		{
			logging::log_error("CWord2HtmlParser::RangeToHtml - Error destroying WordParser object") ;
			logging::log_exception(e) ;
		}
	}
	catch (CException &e) 
	{
		logging::log_error("CWord2HtmlParser::RangeToHtml - Error converting Word selection to HTML via COM") ;
		logging::log_exception(e) ;
		return E_FAIL ;
	}
	catch(...)
	{
		return E_FAIL ;
	}

	return S_OK;
}
