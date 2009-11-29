#include "StdAfx.h"
#include "text_templates.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
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
}


#endif // #ifdef _DEBUG