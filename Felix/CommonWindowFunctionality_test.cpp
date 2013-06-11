#include "StdAfx.h"
#include "CommonWindowFunctionality.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CCommonWindowFunctionalityTest )

BOOST_AUTO_TEST_CASE( get_mousewheel_command_increase )
{
	CString command = get_mousewheel_command(4) ;
	wstring actual = static_cast<LPCWSTR>(command) ;
	wstring expected = L"increaseFont" ;
	BOOST_CHECK_EQUAL(actual, expected) ;
}

BOOST_AUTO_TEST_CASE( get_mousewheel_command_decrease)
{
	CString command = get_mousewheel_command(-4) ;
	wstring actual = static_cast<LPCWSTR>(command) ;
	wstring expected = L"decreaseFont" ;
	BOOST_CHECK_EQUAL(actual, expected) ;
}

BOOST_AUTO_TEST_SUITE_END()
#endif
