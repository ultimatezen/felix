#include "stdafx.h"
#include ".\MemoryWindow.h"


#ifdef UNIT_TEST
#include <boost/test/unit_test.hpp>
#include "window_interface_fake.h"

BOOST_AUTO_TEST_SUITE( TestMemoryWindow )

	typedef CComObject< CMemoryWindow > win_obj ;
	typedef CComPtr< win_obj > win_ptr ;

	BOOST_AUTO_TEST_CASE( test_init )
	{
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;
		BOOST_CHECK( win ) ;
	}

	// get
	BOOST_AUTO_TEST_CASE( test_get_top )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		long val ;
		win->get_Top(&val) ;
		BOOST_CHECK_EQUAL(0, val) ;

		wi.set_top(100) ;

		win->get_Top(&val) ;
		BOOST_CHECK_EQUAL(100, val) ;
	}
	BOOST_AUTO_TEST_CASE( test_get_left )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		long val ;
		win->get_Left(&val) ;
		BOOST_CHECK_EQUAL(0, val) ;

		wi.set_left(100) ;

		win->get_Left(&val) ;
		BOOST_CHECK_EQUAL(100, val) ;
	}
	BOOST_AUTO_TEST_CASE( test_get_height )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		long val ;
		win->get_Height(&val) ;
		BOOST_CHECK_EQUAL(0, val) ;

		wi.set_height(100) ;

		win->get_Height(&val) ;
		BOOST_CHECK_EQUAL(100, val) ;
	}
	BOOST_AUTO_TEST_CASE( test_get_width )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		long val ;
		win->get_Width(&val) ;
		BOOST_CHECK_EQUAL(0, val) ;

		wi.set_width(100) ;

		win->get_Width(&val) ;
		BOOST_CHECK_EQUAL(100, val) ;
	}

	// set
	BOOST_AUTO_TEST_CASE( test_set_top )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		win->put_Top(100) ;

		BOOST_CHECK_EQUAL(wi.get_top(), 100) ;
	}
	BOOST_AUTO_TEST_CASE( test_set_left )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		win->put_Left(100) ;

		BOOST_CHECK_EQUAL(wi.get_left(), 100) ;
	}
	BOOST_AUTO_TEST_CASE( test_set_height )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		win->put_Height(100) ;

		BOOST_CHECK_EQUAL(wi.get_height(), 100) ;
	}
	BOOST_AUTO_TEST_CASE( test_set_width )
	{
		WindowInterfaceFake wi ;
		win_ptr win ;
		BOOST_CHECK( SUCCEEDED( win_obj::CreateInstance( &win ) ) ) ;

		win->set_window(&wi) ;

		win->put_Width(100) ;

		BOOST_CHECK_EQUAL(wi.get_width(), 100) ;
	}



BOOST_AUTO_TEST_SUITE_END()
#endif