#include "StdAfx.h"
#include "App2.h"
#include "ComRecords.h"
#include "query.h"
#include "memory_local.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

// test_properties_loaded_history
BOOST_AUTO_TEST_SUITE( TestComRecords )

	using namespace mem_engine;

	typedef CComObject< CRecords > records_obj ;
	typedef CComPtr< records_obj > records_ptr ;


	record_pointer make_record(string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		return rec ;
	}

	BOOST_AUTO_TEST_CASE( test_instantiate )
	{
		records_ptr recs ;
		HRESULT hr = records_obj::CreateInstance( &recs ) ;

		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( recs ) ;
	}

	BOOST_AUTO_TEST_CASE( test_instantiate_count_0 )
	{
		records_ptr recs ;
		records_obj::CreateInstance( &recs ) ;

		long count = 10 ;
		recs->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 0) ;
	}

	BOOST_AUTO_TEST_CASE( test_records_count_2 )
	{
		records_ptr recs ;
		records_obj::CreateInstance( &recs ) ;

		record_collection_type records ;
		record_pointer rec1 = make_record("source 1", "trans 1") ;
		mem_engine::memory_local mem(app_props::get_props()) ;
		records[mem.get_key(rec1)] = rec1 ;
		record_pointer rec2 = make_record("source 2", "trans 2") ;
		records[mem.get_key(rec2)] = rec2 ;
		long count = 10 ;
		recs->set_records(records) ;
		recs->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 2) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif