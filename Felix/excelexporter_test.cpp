#include "stdafx.h"
#include "excelexporter.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCExcelExporter )

	BOOST_AUTO_TEST_CASE( process_cell_text_empty)
	{
		wstring text ;
		wstring out = process_cell_text(text) ;
		BOOST_CHECK(out.empty()) ;
	}
	BOOST_AUTO_TEST_CASE( process_cell_text_foo)
	{
		wstring text = L"foo";
		wstring out = process_cell_text(text) ;
		BOOST_CHECK_EQUAL(string(string2string(out).c_str()), "foo") ;
	}
	BOOST_AUTO_TEST_CASE( process_cell_text_equals_sign)
	{
		wstring text = L"=foo" ;
		wstring out = process_cell_text(text) ;
		BOOST_CHECK_EQUAL(string(string2string(out).c_str()), "\'=foo") ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif