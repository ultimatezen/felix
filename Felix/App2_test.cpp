#include "StdAfx.h"
#include "App2.h"
#include "FelixApp.h"
#include "query.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

#include "amop/include/MockObject.h"

BOOST_AUTO_TEST_SUITE( TestApp2 )

	using namespace mem_engine;

	typedef CComPtr< ISearchResult > result_ptr ;
	typedef CComPtr< IMemory > com_mem_ptr ;

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	record_pointer make_record(string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		return rec ;
	}

	search_match_ptr make_match(string source, string trans, int id=0, string name="new")
	{
		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		match->set_memory_id(id) ;
		match->set_memory_location(string2wstring(name)) ;

		return match ;
	}
	BOOST_AUTO_TEST_CASE( get_active_memory )
	{
		app2_ptr app ;
		HRESULT create_success = app2_obj::CreateInstance( &app ) ;
		BOOST_CHECK( SUCCEEDED( create_success ) ) ;
		BOOST_CHECK( app ) ;

		if (app)
		{
			com_mem_ptr mem ;
			HRESULT hr = app->get_ActiveMemory(&mem) ;
			BOOST_CHECK( SUCCEEDED( hr ) ) ;
			BOOST_CHECK( mem ) ;
		}
	}
	BOOST_AUTO_TEST_CASE( get_active_glossary )
	{
		app2_ptr app ;
		app2_obj::CreateInstance( &app ) ;
		BOOST_CHECK( app ) ;

		if (app)
		{
			com_mem_ptr mem ;
			HRESULT hr = app->get_ActiveGlossary(&mem) ;
			BOOST_CHECK( SUCCEEDED( hr ) ) ;
			BOOST_CHECK( mem ) ;
		}
	}

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		app2_ptr app ;
		HRESULT hr = app2_obj::CreateInstance( &app ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( app ) ;
	}
	BOOST_AUTO_TEST_CASE( get_CurrentMatch )
	{
		app2_ptr app ;
		app2_obj::CreateInstance( &app ) ;
		BOOST_CHECK( app ) ;
		if (app)
		{
			app::get_app().add_record(make_record("spam", "egg")) ;
			app::get_app().lookup(L"spam") ;
			result_ptr result ;
			HRESULT hr = app->get_CurrentMatch(&result) ;
			BOOST_CHECK( SUCCEEDED( hr ) ) ;
			BOOST_CHECK( result ) ;
			double score(5.0) ;
			result->get_Score(&score) ;
			BOOST_CHECK_CLOSE(1.0, score, 0.00001) ;
		}
	}
	BOOST_AUTO_TEST_CASE( get_record )
	{
		app::get_app().lookup(L"foo") ;
		app::get_app().set_translation(L"bar") ;

		app2_ptr app2 ;
		app2_obj::CreateInstance( &app2 ) ;
		BOOST_CHECK( app2 ) ;

		if (app2)
		{
			result_ptr result ;
			app2->get_CurrentMatch(&result) ;

			CComPtr<IRecord> record ;
			HRESULT hr = result->get_Record(&record) ;
			BOOST_CHECK( SUCCEEDED( hr ) ) ;
			BOOST_CHECK( record ) ;
			CComBSTR source ;
			record->get_Source(&source) ;
			string actual = CStringA(source) ;
			string expected = "foo" ;
			BOOST_CHECK_EQUAL(expected, actual) ;
		}
	}
	BOOST_AUTO_TEST_CASE( ReflectChanges )
	{
		app::get_app().clear_memory() ;
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;
		BOOST_CHECK(application) ;

		if (application)
		{
			record_pointer rec = make_record("spam", "egg") ;
			app::get_app().add_record(rec) ;

			application->ReflectChanges(rec->get_id(),
				CComBSTR(L"changed"),
				CComBSTR(L"different")) ;

			memory_pointer mem = app::get_app().get_memory_model()->get_first_memory() ;
			record_pointer firstrec = mem->get_records().begin()->second ;

			string expected = "changed" ;
			string actual = CStringA(firstrec->get_source_plain().c_str()) ;
			BOOST_CHECK_EQUAL(expected, actual) ;
		}
	}
	BOOST_AUTO_TEST_CASE( ReviewTranslation )
	{
		app::get_app().clear_memory() ;
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;

		BOOST_CHECK(application) ;

		if (application)
		{
			record_pointer rec = make_record("spam", "egg") ;
			app::get_app().add_record(rec) ;
			result_ptr result ;
			application->ReviewTranslation(rec->get_id(),
				CComBSTR(L"changed"),
				CComBSTR(L"different")) ;

			memory_pointer mem = app::get_app().get_memory_model()->get_first_memory() ;
			record_pointer firstrec = mem->get_records().begin()->second ;

			string expected = "changed" ;
			string actual = CStringA(firstrec->get_source_plain().c_str()) ;
			BOOST_CHECK_EQUAL(expected, actual) ;
		}
	}

BOOST_AUTO_TEST_SUITE_END()

// CurrentMatches
BOOST_AUTO_TEST_SUITE(TestApp2CurrentMatches)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;


	BOOST_AUTO_TEST_CASE(current_matches_init_empty)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;
		BOOST_CHECK(application) ;

		if (application)
		{
			CComPtr<ISearchResults> matches ;
			HRESULT hr = application->get_CurrentMatches(&matches) ;
			BOOST_CHECK(SUCCEEDED(hr)) ;

			long count(0) ;
			matches->get_Count(&count) ;
			BOOST_CHECK_EQUAL(0, count) ;
		}
	}

BOOST_AUTO_TEST_SUITE_END()

// get_CurrentGlossMatches
BOOST_AUTO_TEST_SUITE(TestApp2CurrentGlossMatches)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	BOOST_AUTO_TEST_CASE(current_gloss_matches_init_empty)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;
		BOOST_CHECK(application) ;

		if (application)
		{
			CComPtr<ISearchResults> matches ;
			app::get_app().clear_memory() ;
			HRESULT hr = application->get_CurrentGlossMatches(&matches) ;
			BOOST_CHECK(SUCCEEDED(hr)) ;

			long count(0) ;
			matches->get_Count(&count) ;
			BOOST_CHECK_EQUAL(0, count) ;
		}
	}

BOOST_AUTO_TEST_SUITE_END()

// get_MemoryWindow
BOOST_AUTO_TEST_SUITE(TestApp2MemoryWindow)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	// CurrentGlossMatches
	BOOST_AUTO_TEST_CASE(init)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;

		CComPtr<IMemoryWindow> mem_window ;
		HRESULT hr = application->get_MemoryWindow(&mem_window) ;
		BOOST_CHECK(SUCCEEDED(hr)) ;
		BOOST_CHECK(mem_window) ;
	}

BOOST_AUTO_TEST_SUITE_END()

// get_GlossaryWindow
BOOST_AUTO_TEST_SUITE(TestApp2GlossaryWindow)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	BOOST_AUTO_TEST_CASE(init)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;

		CComPtr<IMemoryWindow> gloss_window ;
		HRESULT hr = application->get_GlossaryWindow(&gloss_window) ;
		BOOST_CHECK(SUCCEEDED(hr)) ;
		BOOST_CHECK(gloss_window) ;
	}

BOOST_AUTO_TEST_SUITE_END()

// get_Memories
BOOST_AUTO_TEST_SUITE(TestApp2Memories)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	BOOST_AUTO_TEST_CASE(init)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;

		CComPtr<IMemories> mems ;
		HRESULT hr = application->get_Memories(&mems) ;
		BOOST_CHECK(SUCCEEDED(hr)) ;
		BOOST_CHECK(mems) ;
	}
	BOOST_AUTO_TEST_CASE(one_memory)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;
		app::get_app().get_model()->clear() ;
		app::get_app().get_model()->add_memory() ;

		CComPtr<IMemories> mems ;
		application->get_Memories(&mems) ;
		long count(0) ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(1, count) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// get_Glossaries
BOOST_AUTO_TEST_SUITE(TestApp2Glossaries)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	BOOST_AUTO_TEST_CASE(init)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;

		CComPtr<IMemories> mems ;
		HRESULT hr = application->get_Glossaries(&mems) ;
		BOOST_CHECK(SUCCEEDED(hr)) ;
		BOOST_CHECK(mems) ;
	}
	BOOST_AUTO_TEST_CASE(one_glossary)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;
		app::get_app().get_glossary_window()->get_model()->clear() ;
		app::get_app().get_glossary_window()->get_model()->add_memory() ;

		CComPtr<IMemories> mems ;
		application->get_Glossaries(&mems) ;
		long count(0) ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(1, count) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// get_ActiveMemory
BOOST_AUTO_TEST_SUITE(TestApp2ActiveMemory)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	BOOST_AUTO_TEST_CASE(init)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;

		CComPtr<IMemory> mem ;
		HRESULT hr = application->get_ActiveMemory(&mem) ;
		BOOST_CHECK(SUCCEEDED(hr)) ;
		BOOST_CHECK(mem) ;
	}

BOOST_AUTO_TEST_SUITE_END()

// get_ActiveGlossary
BOOST_AUTO_TEST_SUITE(TestApp2ActiveGlossary)

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	BOOST_AUTO_TEST_CASE(init)
	{
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;

		CComPtr<IMemory> gloss;
		HRESULT hr = application->get_ActiveGlossary(&gloss) ;
		BOOST_CHECK(SUCCEEDED(hr)) ;
		BOOST_CHECK(gloss) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif