#include "StdAfx.h"
#include ".\sourceandhtmledit.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "string_logger.h"

BOOST_FIXTURE_TEST_SUITE( TestCSourceAndHtmlEdit, StringLoggerTester )

	BOOST_AUTO_TEST_CASE(instantiate)
	{
		BOOST_CHECK_NO_THROW(CSourceAndHtmlEdit()) ;
	}
	BOOST_AUTO_TEST_CASE(test_create)
	{
		CSourceAndHtmlEdit edit ;
		logging::set_logging_level(LOGGING_VERBOSE);
		edit.create(CWindow(), CWindow(), 0);
		BOOST_CHECK(has_text("Creating source and HTML edit window")) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif