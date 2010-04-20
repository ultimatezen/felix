#include "stdafx.h"
#include ".\extrastringsview.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCExtraStringsView )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CExtraStringsView dialog ;
		BOOST_CHECK(true) ;
	}
	BOOST_AUTO_TEST_CASE( eliminate_whitespace_and_punctuation )
	{
		CExtraStringsView dlg ;
		wstring setting = L"Hello, world!" ;
		wstring key ;
		dlg.eliminate_whitespace_and_punctuation( setting, key ) ;
		BOOST_CHECK_EQUAL( L"Hello__world_", key ) ;
	}

	BOOST_AUTO_TEST_SUITE_END()

#endif