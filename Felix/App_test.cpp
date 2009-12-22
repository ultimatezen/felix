#include "stdafx.h"
#include "App.h"
#include "easyunit/testharness.h"
#include "FelixApp.h"

#ifdef UNIT_TEST

namespace easyunit
{
	typedef CComPtr< CComObject< CApp > > appPtr ;

	TEST( AppTest, Instantiate )
	{
		appPtr app ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &app ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		ASSERT_TRUE( app ) ;
	}

	TEST( AppTest, IsVisible )
	{
		appPtr app ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &app ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		
		VARIANT_BOOL is_visible = VARIANT_FALSE ;

		app->get_Visible( &is_visible ) ;

		ASSERT_EQUALS_M( is_visible, VARIANT_TRUE, "Window should be reported as visible" ) ;
	}

	TEST( AppTest, Quit)
	{
		app::get_app().m_sensing_variable.clear() ;
		appPtr ta ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &ta ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;

		ta->Quit() ;

		CMainFrame &main_frame = app::get_app() ;
		ASSERT_EQUALS_V( 1, (int)main_frame.m_sensing_variable.size() ) ;
		SimpleString expected = "exit_silently" ;
		SimpleString actual = main_frame.m_sensing_variable[0].c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST( AppTest, Lookup)
	{
		appPtr ta ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &ta ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;

		CComBSTR query = L"foo" ;

		ta->Lookup( query ) ;

		CComBSTR trans ;

		ta->get_Trans( &trans ) ;

		ASSERT_TRUE( trans.Length() == 0 )
	}

	TEST( AppTest, ShowMarkup )
	{
		appPtr ta ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &ta ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;

		ta->put_ShowMarkup( VARIANT_FALSE ) ;

		VARIANT_BOOL markup_visible = VARIANT_FALSE ;

		ta->get_ShowMarkup( &markup_visible ) ;
		ASSERT_EQUALS( markup_visible, VARIANT_FALSE ) ;

		ta->put_ShowMarkup( VARIANT_TRUE ) ;
		ta->get_ShowMarkup( &markup_visible ) ;
		ASSERT_EQUALS( markup_visible, VARIANT_TRUE ) ;

	}
}

#endif