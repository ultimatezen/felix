#include "StdAfx.h"
#include "TmxSymbolMapper.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCTMXImportDlg )

	BOOST_AUTO_TEST_CASE( exists_true)
	{
		CTmxSymbolMapper mapper ;
		BOOST_CHECK(mapper.exists(L"nbsp")) ;
		BOOST_CHECK(mapper.exists(L"iexcl")) ;
		BOOST_CHECK(mapper.exists(L"brvbar")) ;
		BOOST_CHECK(mapper.exists(L"plusmn")) ;
	}
	BOOST_AUTO_TEST_CASE( exists_false)
	{
		CTmxSymbolMapper mapper ;
		BOOST_CHECK(! mapper.exists(L"")) ;
		BOOST_CHECK(! mapper.exists(L"spam")) ;
	}
	BOOST_AUTO_TEST_CASE( get_val_ok)
	{
		CTmxSymbolMapper mapper ;
		BOOST_CHECK_EQUAL(wchar_t(0x2287), mapper.get_val(L"supe")) ;
		BOOST_CHECK_EQUAL(wchar_t(0x2284), mapper.get_val(L"nsub")) ;
		BOOST_CHECK_EQUAL(wchar_t(0x2282), mapper.get_val(L"sub")) ;
		BOOST_CHECK_EQUAL(wchar_t(0x2208), mapper.get_val(L"isin")) ;
	}
	BOOST_AUTO_TEST_CASE( get_val_bad)
	{
		CTmxSymbolMapper mapper ;
		BOOST_CHECK_EQUAL(wchar_t(0), mapper.get_val(L"spammy davis")) ;
	}
	BOOST_AUTO_TEST_CASE( strip_html_symbols_quot)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"&quot;spam&quot;" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		BOOST_CHECK_EQUAL(string("\"spam\""), 
			string(string2string(out_text).c_str())) ;
	}
	BOOST_AUTO_TEST_CASE( strip_html_symbols_entity)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam &#x2126; egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam " ;
		expected += wchar_t(0x2126) ;
		expected += L" egg" ;
		BOOST_CHECK_EQUAL(string(string2string(expected).c_str()), 
			string(string2string(out_text).c_str())) ;
	}
	BOOST_AUTO_TEST_CASE( strip_html_symbols_entity_base10)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam&#64;egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam@egg" ;
		BOOST_CHECK_EQUAL(string(string2string(expected).c_str()), 
			string(string2string(out_text).c_str())) ;
	}
	BOOST_AUTO_TEST_CASE( strip_html_symbols_bad_symbol)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam &gargle; egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam &gargle; egg" ;
		BOOST_CHECK_EQUAL(string(string2string(expected).c_str()), 
			string(string2string(out_text).c_str())) ;
	}
	BOOST_AUTO_TEST_CASE( strip_html_symbols_bad_ampersand)
	{
		CTmxSymbolMapper mapper ;
		wstring in_text = L"spam & egg" ;
		wstring out_text = mapper.strip_html_symbols(in_text) ;
		wstring expected = L"spam & egg" ;
		BOOST_CHECK_EQUAL(string(string2string(expected).c_str()), 
			string(string2string(out_text).c_str())) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif