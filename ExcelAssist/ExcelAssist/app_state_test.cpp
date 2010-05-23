#include "stdafx.h"
#include "app_state.h"

#ifdef UNIT_TEST


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_app_state )

	BOOST_AUTO_TEST_CASE(test_toolbar_visible)
	{
		app_state properties ;
		BOOST_CHECK(properties.get_toolbar_visible()) ;
		properties.set_toolbar_visible(FALSE) ;
		BOOST_CHECK(! properties.get_toolbar_visible()) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif
