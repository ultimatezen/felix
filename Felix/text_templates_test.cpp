#include "StdAfx.h"
#include "text_templates.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_text_templates )

	using namespace cpptempl;
	BOOST_AUTO_TEST_CASE( start_file )
	{
		CStringA filename = get_template_filename(_T("start.html")) ;
		BOOST_CHECK_EQUAL("C:\\Users\\Ryan\\AppData\\Local\\Felix\\html\\en\\start.html", filename) ;
	}
	BOOST_AUTO_TEST_CASE( glossary_start )
	{
		CStringA filename = get_template_filename(_T("start_gloss.html")) ;
		BOOST_CHECK_EQUAL("C:\\Users\\Ryan\\AppData\\Local\\Felix\\html\\en\\start_gloss.html", filename) ;
	}
	BOOST_AUTO_TEST_CASE( get_template_text_match_none )
	{
		wstring text = get_template_text(_T("match_none.txt")) ;
		BOOST_CHECK(boost::contains(text, L"{$query}")) ;
	}
	BOOST_AUTO_TEST_CASE( test_calculate_template_filename )
	{
		string filename = (LPCSTR)CStringA(calculate_template_filename("foo.txt")) ;
		string actual = boost::to_lower_copy(filename) ;
		string expected = "c:\\users\\ryan\\appdata\\local\\felix\\html\\en\\foo.txt" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( test_calculate_module_template_filename )
	{
		string filename = (LPCSTR)CStringA(calculate_module_template_filename("foo.txt")) ;
		string actual = boost::to_lower_copy(filename) ;
		string expected = "c:\\dev\\cpp\\assistant suite\\felix\\testing\\html\\en\\foo.txt" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif