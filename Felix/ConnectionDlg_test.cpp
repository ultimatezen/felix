#include "stdafx.h"
#include "ConnectionDlg.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCConnectionDlg )

	using namespace app_props ;

	props_ptr make_props()
	{
		static app_props::props_ptr props ;
		if (! props)
		{
			props = app_props::props_ptr(new app_props::properties) ;
		}
		return props ;
	}
	BOOST_AUTO_TEST_CASE( IDOK_Message)
	{
		app_props::props_ptr props = make_props(); 
		CConnectionDlg dialog(props) ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	// message processing
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		app_props::props_ptr props = make_props(); 
		CConnectionDlg dialog(props) ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
