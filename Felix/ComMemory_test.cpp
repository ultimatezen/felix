#include "StdAfx.h"
#include "App2.h"
#include "ComMemory.h"
#include "query.h"
#include "memory_local.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

// test_properties_loaded_history
BOOST_AUTO_TEST_SUITE( TestComMemory)

	using namespace mem_engine;

	typedef CComPtr< IRecords > records_ptr ;

	typedef CComObject< CComMemory > mem_obj ;
	typedef CComPtr< mem_obj > mem_ptr ;

	record_pointer make_record(string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		return rec ;
	}

	BOOST_AUTO_TEST_CASE( test_instantiate )
	{
		mem_ptr mem ;
		HRESULT hr = mem_obj::CreateInstance( &mem ) ;

		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( mem ) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Records
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(test_records_init_empty)
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;

		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		records_ptr records ;
		HRESULT hr = mem->get_Records(&records) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		long count = 10 ;
		records->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 0) ;
	}	
	BOOST_AUTO_TEST_CASE(test_records_count_2)
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;

		memory_pointer memory(new memory_local) ;
		memory->add_record(make_record("source a", "source a")) ;
		memory->add_record(make_record("source b", "source b")) ;

		mem->set_memory(memory) ;

		records_ptr records ;
		mem->get_Records(&records) ;

		long count = 10 ;
		records->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 2) ;
	}	

	//////////////////////////////////////////////////////////////////////////
	// MemoryInfo
	//////////////////////////////////////////////////////////////////////////

	// creator
	BOOST_AUTO_TEST_CASE( get_creator )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		memory->get_memory_info()->set_creator(L"Charlie Chaplin") ;
		mem->set_memory(memory) ;

		CComBSTR creator ;
		mem->get_Creator(&creator) ;
		BOOST_CHECK_EQUAL("Charlie Chaplin", string(CW2A(creator))) ;
	}
	BOOST_AUTO_TEST_CASE( set_creator )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR creator = L"Mr. Splashypants";
		mem->put_Creator(creator) ;
		CComBSTR result ;
		mem->get_Creator(&result) ;
		BOOST_CHECK_EQUAL("Mr. Splashypants", string(CW2A(result))) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif