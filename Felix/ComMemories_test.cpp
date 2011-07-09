#include "StdAfx.h"
#include "App2.h"
#include "Memories.h"
#include "query.h"
#include "felix_model_fake.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

// test_properties_loaded_history
BOOST_AUTO_TEST_SUITE( TestComMemories )

	using namespace mem_engine;

	typedef CComObject< CMemories > memories_obj ;
	typedef CComPtr< memories_obj > memories_ptr ;

	BOOST_AUTO_TEST_CASE( test_instantiate )
	{
		memories_ptr mems ;
		HRESULT hr = memories_obj::CreateInstance( &mems ) ;

		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( mems ) ;
	}

	BOOST_AUTO_TEST_CASE( test_instantiate_count_0 )
	{
		memories_ptr mems ;
		memories_obj::CreateInstance( &mems ) ;

		long count = 10 ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 0) ;
	}
	BOOST_AUTO_TEST_CASE( test_instantiate_count_2 )
	{
		memories_ptr mems ;
		memories_obj::CreateInstance( &mems ) ;
		model_iface_ptr model(new FelixModelInterfaceFake) ;
		model->add_memory() ;
		model->add_memory() ;
		mems->set_model(model) ;

		long count = 10 ;
		BOOST_CHECK(SUCCEEDED(mems->get_Count(&count))) ;
		BOOST_CHECK_EQUAL(count, 2) ;
	}

	BOOST_AUTO_TEST_CASE( test_add )
	{
		memories_ptr mems ;
		memories_obj::CreateInstance( &mems ) ;
		model_iface_ptr model(new FelixModelInterfaceFake) ;
		mems->set_model(model) ;

		CComPtr<IMemory> mem ;
		BOOST_CHECK(SUCCEEDED(mems->Add(&mem))) ;

		long count = 0 ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 1) ;
	}

	BOOST_AUTO_TEST_CASE( test_load )
	{
		memories_ptr mems ;
		memories_obj::CreateInstance( &mems ) ;
		model_iface_ptr model(new FelixModelInterfaceFake) ;
		mems->set_model(model) ;

		CComPtr<IMemory> mem ;
		CComBSTR filename = L"C:\\test\\test_memories\\one_record.xml" ;
		BOOST_CHECK(SUCCEEDED(mems->Load(filename, &mem))) ;

		long count = 0 ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 1) ;

		ULONG memsize = 10 ;
		mem->GetSize(&memsize) ;
		BOOST_CHECK_EQUAL(memsize, 1u) ;
	}

	BOOST_AUTO_TEST_CASE( test_clear )
	{
		memories_ptr mems ;
		memories_obj::CreateInstance( &mems ) ;
		model_iface_ptr model(new FelixModelInterfaceFake) ;
		model->add_memory() ;
		mems->set_model(model) ;

		BOOST_CHECK(SUCCEEDED(mems->Clear())) ;

		long count = 0 ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 0) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif