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
		FelixModelInterfaceFake model ;
		model.m_model->add_memory() ;
		model.m_model->add_memory() ;
		mems->set_model(&model) ;

		long count = 10 ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 2) ;
	}

	BOOST_AUTO_TEST_CASE( test_add )
	{
		memories_ptr mems ;
		memories_obj::CreateInstance( &mems ) ;
		FelixModelInterfaceFake model ;
		mems->set_model(&model) ;

		CComPtr<IMemory> mem ;
		mems->Add(&mem) ;

		long count = 0 ;
		mems->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 1) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif