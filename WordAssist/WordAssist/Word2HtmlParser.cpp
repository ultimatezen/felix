// Word2HtmlParser.cpp : Implementation of CWord2HtmlParser

#include "stdafx.h"
#include "Word2HtmlParser.h"
#include "WordParser.h"
#include "logging.h"
#include "input_device_file.h"

using namespace except ;


CWord2HtmlParser::CWord2HtmlParser() :
m_plaintext(false)
{
	input_device_ptr input(new InputDeviceFile) ;
	output_device_ptr output(new OutputDeviceFile) ;
	string file_text = get_config_text(_T("abbreviations.txt"), output, input) ;
	m_abbreviations.load(string2wstring(file_text)) ;
}

STDMETHODIMP CWord2HtmlParser::RangeToHtml(IDispatch *word_range, 
											  BSTR* html_text)
{
	try
	{
		word_sel_ptr selection_ptr = word_range ;

		WordSelection selection = selection_ptr ;

		std::shared_ptr<WordParser> parser 
			= std::shared_ptr<WordParser>(new WordParser( selection, m_properties, &m_abbreviations )) ;

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
			parser = std::shared_ptr<WordParser>() ;
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
