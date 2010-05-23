#include "stdafx.h"

#include "AppStringRecordParser.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( CAppStringRecordParserTest )

	BOOST_AUTO_TEST_CASE(test_EmptyFile )
	{
		wstring empty_file_text ;
		CAppStringRecordParser parser( empty_file_text.c_str() ) ;
		BOOST_CHECK_EQUAL( false, parser.had_more_records() ) ;
	}

	BOOST_AUTO_TEST_CASE(test_SingleRecordFile )
	{
		wstring file_text = L"<record>foo</record>" ;
		CAppStringRecordParser parser( file_text.c_str() ) ;
		BOOST_CHECK_EQUAL( true, parser.had_more_records() ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
