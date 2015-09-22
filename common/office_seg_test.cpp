#include "StdAfx.h"
#include "office_seg.h"
#include "office_seg_fake.h"

#ifdef UNIT_TEST
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestCMainFrame )

	BOOST_AUTO_TEST_CASE(test_init)
	{
		office_seg_ptr seg(new OfficeSegFake) ;
		BOOST_CHECK(seg) ;
	}
	BOOST_AUTO_TEST_CASE( get_record_translation_standard)
	{
		office_seg_ptr seg(new OfficeSegFake) ;
		BOOST_CHECK(seg) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif