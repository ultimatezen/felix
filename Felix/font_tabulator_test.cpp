#include "stdafx.h"
#include "font_tabulator.h"
#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_font_tabulator )

	BOOST_AUTO_TEST_CASE( init)
	{
		BOOST_CHECK_NO_THROW(font_tabulator());
	}

BOOST_AUTO_TEST_SUITE_END()