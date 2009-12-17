#include "StdAfx.h"
#include "text_templates.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace text_tmpl;
	TEST( test_text_templates, start_file )
	{
		CStringA filename = get_template_filename(_T("start.html")) ;
		ASSERT_EQUALS_V("C:/Users/Ryan/AppData/Local/Felix/html/en/start.html", filename) ;
	}
	TEST( test_text_templates, glossary_start )
	{
		CStringA filename = get_template_filename(_T("start_gloss.html")) ;
		ASSERT_EQUALS_V("C:/Users/Ryan/AppData/Local/Felix/html/en/start_gloss.html", filename) ;
	}
	TEST( test_text_templates, get_template_text_match_none )
	{
		wstring text = get_template_text(_T("match_none.txt")) ;
		ASSERT_TRUE(text.find(L"{$query}") != wstring::npos) ;
	}
	TEST( test_text_templates, calculate_template_filename )
	{
		string filename = (LPCSTR)CStringA(calculate_template_filename("foo.txt")) ;
		SimpleString actual = str::make_lower_out(filename).c_str() ;
		SimpleString expected = "c:/users/ryan/appdata/local/felix/html/en/foo.txt" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_text_templates, calculate_module_template_filename )
	{
		string filename = (LPCSTR)CStringA(calculate_module_template_filename("foo.txt")) ;
		SimpleString actual = str::make_lower_out(filename).c_str() ;
		SimpleString expected = "c:/dev/cpp/assistant suite/felix/testing/html/en/foo.txt" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

}


#endif