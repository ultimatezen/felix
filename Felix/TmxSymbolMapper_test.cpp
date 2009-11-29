#include "StdAfx.h"
#include "TmxSymbolMapper.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCTMXImportDlg, exists_true)
	{
		CTmxSymbolMapper mapper ;
		ASSERT_TRUE(mapper.exists(L"nbsp")) ;
		ASSERT_TRUE(mapper.exists(L"iexcl")) ;
		ASSERT_TRUE(mapper.exists(L"brvbar")) ;
		ASSERT_TRUE(mapper.exists(L"plusmn")) ;
	}
	TEST( TestCTMXImportDlg, exists_false)
	{
		CTmxSymbolMapper mapper ;
		ASSERT_TRUE(! mapper.exists(L"")) ;
		ASSERT_TRUE(! mapper.exists(L"spam")) ;
	}
	TEST( TestCTMXImportDlg, get_val_ok)
	{
		CTmxSymbolMapper mapper ;
		ASSERT_EQUALS(wchar_t(0x2287), mapper.get_val(L"supe")) ;
		ASSERT_EQUALS(wchar_t(0x2284), mapper.get_val(L"nsub")) ;
		ASSERT_EQUALS(wchar_t(0x2282), mapper.get_val(L"sub")) ;
		ASSERT_EQUALS(wchar_t(0x2208), mapper.get_val(L"isin")) ;
	}
	TEST( TestCTMXImportDlg, get_val_bad)
	{
		CTmxSymbolMapper mapper ;
		ASSERT_EQUALS(wchar_t(0), mapper.get_val(L"spammy davis")) ;
	}
	TEST( TestCTMXImportDlg, strip_html_symbols_quot)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"&quot;spam&quot;" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		ASSERT_EQUALS_V(SimpleString("\"spam\""), 
			SimpleString(string2string(out_text).c_str())) ;
	}
	TEST( TestCTMXImportDlg, strip_html_symbols_entity)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam &#x2126; egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam " ;
		expected += wchar_t(0x2126) ;
		expected += L" egg" ;
		ASSERT_EQUALS_V(SimpleString(string2string(expected, CP_UTF8).c_str()), 
			SimpleString(string2string(out_text, CP_UTF8).c_str())) ;
	}
	TEST( TestCTMXImportDlg, strip_html_symbols_entity_base10)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam&#64;egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam@egg" ;
		ASSERT_EQUALS_V(SimpleString(string2string(expected, CP_UTF8).c_str()), 
			SimpleString(string2string(out_text, CP_UTF8).c_str())) ;
	}
	TEST( TestCTMXImportDlg, strip_html_symbols_bad_symbol)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam &gargle; egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam &gargle; egg" ;
		ASSERT_EQUALS_V(SimpleString(string2string(expected, CP_UTF8).c_str()), 
			SimpleString(string2string(out_text, CP_UTF8).c_str())) ;
	}
	TEST( TestCTMXImportDlg, strip_html_symbols_bad_ampersand)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam & egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam & egg" ;
		ASSERT_EQUALS_V(SimpleString(string2string(expected, CP_UTF8).c_str()), 
			SimpleString(string2string(out_text, CP_UTF8).c_str())) ;
	}
}

#endif