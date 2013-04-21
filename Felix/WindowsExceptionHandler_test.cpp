#include "stdafx.h"
#include "WindowExceptionHandler.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( test_emode_search_params )

	BOOST_AUTO_TEST_CASE(test_get_language_code)
	{
		_Module.set_library( _T("lang\\JpnResource.dll") ) ;
		BOOST_CHECK_EQUAL(get_language_code(), L"Japanese") ;
		_Module.set_library( _T("lang\\EngResource.dll") ) ;
		BOOST_CHECK_EQUAL(get_language_code(), L"English") ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif