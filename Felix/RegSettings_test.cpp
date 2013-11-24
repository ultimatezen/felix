#include "stdafx.h"
#include "RegSettings.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestRegSettings )

	BOOST_AUTO_TEST_CASE(TestCWindowPlacement)
	{
		CWindowPlacement placement ;
		BOOST_CHECK_EQUAL(placement.ptMinPosition.y, 0) ;
		BOOST_CHECK_EQUAL(placement.ptMaxPosition.x, 0) ;
		BOOST_CHECK(placement.showCmd == SW_SHOWNORMAL) ;
	}
	BOOST_AUTO_TEST_CASE(TestCWindowSettings)
	{
		CWindowSettings settings ;
		BOOST_CHECK_EQUAL(settings.m_WindowPlacement.ptMinPosition.x, 0) ;
		BOOST_CHECK_EQUAL(settings.m_WindowPlacement.ptMaxPosition.y, 0) ;
		BOOST_CHECK(settings.m_WindowPlacement.showCmd == SW_SHOWNORMAL) ;
	}
	BOOST_AUTO_TEST_CASE(TestCReBarSettings)
	{
		CReBarSettings settings ;
		BOOST_CHECK(settings.m_cbBandCount == 0) ;
		BOOST_CHECK(settings.m_pBands == NULL) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif