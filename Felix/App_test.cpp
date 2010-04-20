#include "stdafx.h"
#include "App.h"
#include <boost/test/unit_test.hpp>
#include "FelixApp.h"

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( AppTest )

	typedef CComPtr< CComObject< CApp > > appPtr ;

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		appPtr app ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &app ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( app ) ;
	}

	BOOST_AUTO_TEST_CASE( IsVisible )
	{
		appPtr app ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &app ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		
		VARIANT_BOOL is_visible = VARIANT_FALSE ;

		app->get_Visible( &is_visible ) ;

		BOOST_CHECK(is_visible == VARIANT_TRUE) ;
	}

	BOOST_AUTO_TEST_CASE( Quit)
	{
		app::get_app().m_sensing_variable.clear() ;
		appPtr ta ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &ta ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		ta->Quit() ;

		CMainFrame &main_frame = app::get_app() ;
		BOOST_CHECK_EQUAL( 1, (int)main_frame.m_sensing_variable.size() ) ;
		string expected = "exit_silently" ;
		string actual = main_frame.m_sensing_variable[0].c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( Lookup)
	{
		appPtr ta ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &ta ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		CComBSTR query = L"foo" ;

		ta->Lookup( query ) ;

		CComBSTR trans ;

		ta->get_Trans( &trans ) ;

		BOOST_CHECK( trans.Length() == 0 ) ;
	}

	BOOST_AUTO_TEST_CASE( ShowMarkup )
	{
		appPtr ta ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &ta ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		ta->put_ShowMarkup( VARIANT_FALSE ) ;

		VARIANT_BOOL markup_visible = VARIANT_FALSE ;

		ta->get_ShowMarkup( &markup_visible ) ;
		BOOST_CHECK( markup_visible == VARIANT_FALSE ) ;

		ta->put_ShowMarkup( VARIANT_TRUE ) ;
		ta->get_ShowMarkup( &markup_visible ) ;
		BOOST_CHECK( markup_visible == VARIANT_TRUE ) ;

	}
BOOST_AUTO_TEST_SUITE_END()

#endif