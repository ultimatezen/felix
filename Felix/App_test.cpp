#include "stdafx.h"
#include "App.h"
#include <boost/test/unit_test.hpp>
#include "FelixApp.h"

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestApp )

	using namespace mem_engine ;
	typedef CComPtr< CComObject< CApp > > appPtr ;

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		appPtr app ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &app ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( app ) ;
	}


	BOOST_AUTO_TEST_CASE(add_memory_entry)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		CComBSTR source = L"add_memory_entry_source" ;
		CComBSTR trans = L"add_memory_entry_trans" ;
		CComBSTR context = L"add_memory_entry_context" ;

		app::get_app().clear_memory() ;
		felix->AddMemoryEntry(source, trans, context) ;

		memory_pointer mem = app::get_app().get_model()->get_first_memory() ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		record_pointer rec = mem->get_record_at(0) ;

		BOOST_CHECK_EQUAL(L"add_memory_entry_source", rec->get_source_rich()) ;
	}
	BOOST_AUTO_TEST_CASE(add_glossary_entry)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		CComBSTR source = L"add_glossary_entry_source" ;
		CComBSTR trans = L"add_glossary_entry_trans" ;
		CComBSTR context = L"add_glossary_entry_context" ;

		app::get_app().clear_memory() ;
		felix->AddGlossaryEntry(source, trans, context) ;

		memory_pointer mem = app::get_app().get_glossary_windows()[0]->get_model()->get_first_memory() ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		record_pointer rec = mem->get_record_at(0) ;

		BOOST_CHECK_EQUAL(L"add_glossary_entry_trans", rec->get_trans_rich()) ;
	}
	BOOST_AUTO_TEST_CASE(Lookup_emtpy_trans)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		CComBSTR query = L"foo" ;

		felix->Lookup( query ) ;

		CComBSTR trans ;

		felix->get_Trans( &trans ) ;

		BOOST_CHECK( trans.Length() == 0 ) ;
	}

	BOOST_AUTO_TEST_CASE(Lookup_emtpy_query)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		CComBSTR query = L"foo" ;

		felix->Lookup( query ) ;

		CComBSTR retrieved_query ;
		felix->get_Query( &retrieved_query ) ;

		BOOST_CHECK_EQUAL( L"foo", retrieved_query ) ;
	}

	BOOST_AUTO_TEST_CASE(lookup_2_records)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;

		CComBSTR source = L"lookup source 1" ;
		CComBSTR trans = L"lookup trans 1" ;
		CComBSTR context = L"lookup context 1" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"source 2" ;
		trans = L"trans 2" ;
		context = L"context 2" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"lookup source" ;
		felix->Lookup( query ) ;

		CComBSTR retrieved_trans ;
		felix->get_Trans( &retrieved_trans ) ;

		BOOST_CHECK_EQUAL(L"lookup trans 1", retrieved_trans) ;
	}
	BOOST_AUTO_TEST_CASE(lookup_2_records_perfect_score)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;

		CComBSTR source = L"lookup source 1" ;
		CComBSTR trans = L"lookup trans 1" ;
		CComBSTR context = L"lookup context 1" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"source 2" ;
		trans = L"trans 2" ;
		context = L"context 2" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"lookup source 1" ;
		felix->Lookup( query ) ;

		double score ;
		felix->get_Score( &score ) ;

		BOOST_CHECK_CLOSE(1.0, score, 0.001) ;
	}

	BOOST_AUTO_TEST_CASE(lookup_trans_2_records)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;

		CComBSTR source = L"lookup source 1" ;
		CComBSTR trans = L"lookup trans 1" ;
		CComBSTR context = L"lookup context 1" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"source 2" ;
		trans = L"trans 2" ;
		context = L"context 2" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"lookup trans 1" ;
		felix->LookupTrans( query ) ;

		CComBSTR retrieved_trans ;
		felix->get_Trans( &retrieved_trans ) ;

		BOOST_CHECK_EQUAL(L"lookup trans 1", retrieved_trans) ;
	}

	BOOST_AUTO_TEST_CASE(concordance_2_records)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;

		CComBSTR source = L"lookup source 1" ;
		CComBSTR trans = L"lookup trans 1" ;
		CComBSTR context = L"lookup context 1" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"source 2" ;
		trans = L"trans 2" ;
		context = L"context 2" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"source" ;
		felix->Concordance( query ) ;

		BOOST_CHECK_EQUAL(2u, app::get_app().m_search_matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE(trans_concordance_2_records)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;
		BOOST_CHECK_EQUAL(0u, app::get_app().m_search_matches.size()) ;

		CComBSTR source = L"lookup source 1" ;
		CComBSTR trans = L"lookup trans 1" ;
		CComBSTR context = L"lookup context 1" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"source 2" ;
		trans = L"trans 2" ;
		context = L"context 2" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"trans" ;
		felix->TransConcordance( query ) ;

		BOOST_CHECK_EQUAL(2u, app::get_app().m_search_matches.size()) ;
	}


	BOOST_AUTO_TEST_CASE(gloss_lookup_2_records)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		CComBSTR source = L"aaaa" ;
		CComBSTR trans = L"aaaa trans" ;
		CComBSTR context = L"aaaa" ;
		felix->AddGlossaryEntry(source, trans, context) ;

		source = L"bbb" ;
		trans = L"bbb" ;
		context = L"bbb" ;
		felix->AddGlossaryEntry(source, trans, context) ;

		CComBSTR query = L"aaaa bbb" ;
		felix->Lookup( query ) ;

		short num_matches(0) ;
		felix->get_NumGlossMatches(&num_matches) ;

		BOOST_CHECK_EQUAL(2, num_matches) ;

		CComBSTR retrieved_trans ;
		felix->get_GlossMatch(0, &retrieved_trans ) ;

		BOOST_CHECK_EQUAL(L"aaaa trans", retrieved_trans) ;
	}


	BOOST_AUTO_TEST_CASE(lookup_3_next_next)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;
		memory_pointer mem = app::get_app().get_model()->get_first_memory() ;

		CComBSTR source = L"aaa source" ;
		CComBSTR trans = L"aaa trans" ;
		CComBSTR context = L"aaa context" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"aa source" ;
		trans = L"bbb trans" ;
		context = L"bbb context" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"a source" ;
		trans = L"ccc trans" ;
		context = L"ccc context" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"aaa source" ;
		felix->Lookup( query ) ;

		short num_matches(0) ;
		felix->get_NumMatches(&num_matches) ;

		BOOST_CHECK_EQUAL(3, num_matches) ;
		felix->NextTrans() ;
		felix->NextTrans() ;

		CComBSTR retrieved_trans ;
		felix->get_Trans(&retrieved_trans) ;

		BOOST_CHECK_EQUAL(wstring(L"ccc trans"), BSTR2wstring(retrieved_trans)) ;
	}

	BOOST_AUTO_TEST_CASE(lookup_3_prev_prev)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;

		CComBSTR source = L"aaa source" ;
		CComBSTR trans = L"aaa trans" ;
		CComBSTR context = L"aaa context" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"aa source" ;
		trans = L"bbb trans" ;
		context = L"bbb context" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"a source" ;
		trans = L"ccc trans" ;
		context = L"ccc context" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"aaa source" ;
		felix->Lookup( query ) ;

		short num_matches(0) ;
		felix->get_NumMatches(&num_matches) ;

		BOOST_CHECK_EQUAL(3, num_matches) ;
		felix->PrevTrans() ;
		felix->PrevTrans() ;

		CComBSTR retrieved_trans ;
		felix->get_Trans(&retrieved_trans) ;

		BOOST_CHECK_EQUAL(wstring(L"bbb trans"), BSTR2wstring(retrieved_trans)) ;
	}
	BOOST_AUTO_TEST_CASE(lookup_2_delete_one)
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		app::get_app().clear_memory() ;

		CComBSTR source = L"lookup source" ;
		CComBSTR trans = L"i should be first" ;
		CComBSTR context = L"context 1" ;
		felix->AddMemoryEntry(source, trans, context) ;

		source = L"lookup source 2" ;
		trans = L"i should be second" ;
		context = L"context 2" ;
		felix->AddMemoryEntry(source, trans, context) ;

		CComBSTR query = L"lookup source" ;
		felix->Lookup( query ) ;
		felix->DeleteMemEntry() ;

		CComBSTR retrieved_trans ;
		felix->get_Trans( &retrieved_trans ) ;

		BOOST_CHECK_EQUAL(L"i should be second", retrieved_trans) ;
	}
	BOOST_AUTO_TEST_CASE( ShowMarkup )
	{
		appPtr felix ;
		HRESULT hr = CComObject< CApp >::CreateInstance( &felix ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		felix->put_ShowMarkup( VARIANT_FALSE ) ;

		VARIANT_BOOL markup_visible = VARIANT_FALSE ;

		felix->get_ShowMarkup( &markup_visible ) ;
		BOOST_CHECK( markup_visible == VARIANT_FALSE ) ;

		felix->put_ShowMarkup( VARIANT_TRUE ) ;
		felix->get_ShowMarkup( &markup_visible ) ;
		BOOST_CHECK( markup_visible == VARIANT_TRUE ) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif