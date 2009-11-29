#include "StdAfx.h"
#include "App2.h"
#include "FelixApp.h"
#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	using namespace memory_engine;

	typedef CComPtr< ISearchResult > result_ptr ;

	typedef CComObject< CApp2 > app2_obj ;
	typedef CComPtr< app2_obj > app2_ptr ;

	record_pointer make_record_app2(string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		return rec ;
	}


	TEST( TestApp2, Instantiate )
	{
		app2_ptr app ;
		HRESULT hr = app2_obj::CreateInstance( &app ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		ASSERT_TRUE( app ) ;
	}
	TEST( TestApp2, get_CurrentMatch )
	{
		app2_ptr app ;
		app2_obj::CreateInstance( &app ) ;
		app::get_app().add_record(make_record_app2("spam", "egg")) ;
		app::get_app().lookup(L"spam") ;
		result_ptr result ;
		HRESULT hr = app->get_CurrentMatch(&result) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		ASSERT_TRUE( result ) ;
		double score(5.0) ;
		result->get_Score(&score) ;
		ASSERT_EQUALS_DELTA_V(1.0, score, 0.00001) ;
	}
	TEST( TestApp2, get_record )
	{
		app::get_app().lookup(L"foo") ;
		app::get_app().set_translation(L"bar") ;

		app2_ptr app2 ;
		app2_obj::CreateInstance( &app2 ) ;

		result_ptr result ;
		app2->get_CurrentMatch(&result) ;
		
		CComPtr<IRecord> record ;
		HRESULT hr = result->get_Record(&record) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		ASSERT_TRUE( record ) ;
		CComBSTR source ;
		record->get_Source(&source) ;
		SimpleString actual = CStringA(source) ;
		SimpleString expected = "foo" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestApp2, ReflectChanges )
	{
		app::get_app().clear_memory() ;
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;
		record_pointer rec = make_record_app2("spam", "egg") ;
		app::get_app().add_record(rec) ;
		result_ptr result ;
		application->ReflectChanges(rec->get_id(),
			CComBSTR(L"changed"),
			CComBSTR(L"different")) ;

		memory_pointer mem = app::get_app().get_memory_model()->get_first_memory() ;
		record_pointer firstrec = *mem->begin() ;

		SimpleString expected = "changed" ;
		SimpleString actual = CStringA(firstrec->get_source_plain().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestApp2, ReviewTranslation )
	{
		app::get_app().clear_memory() ;
		app2_ptr application ;
		app2_obj::CreateInstance( &application ) ;
		record_pointer rec = make_record_app2("spam", "egg") ;
		app::get_app().add_record(rec) ;
		result_ptr result ;
		application->ReviewTranslation(rec->get_id(),
			CComBSTR(L"changed"),
			CComBSTR(L"different")) ;

		memory_pointer mem = app::get_app().get_memory_model()->get_first_memory() ;
		record_pointer firstrec = *mem->begin() ;

		SimpleString expected = "changed" ;
		SimpleString actual = CStringA(firstrec->get_source_plain().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

}

#endif