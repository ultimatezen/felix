#include "stdafx.h"
#include "ManagerViewStart.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"
#include "manager_window_listener_fake.h"
#include "view_interface_fake.h"

BOOST_AUTO_TEST_SUITE( TestManagerViewStart )

	using namespace mem_engine ;
	using namespace mgrview ;
	using namespace cpptempl ;

	// This sets up and initializes the view class
	template<class START_TYPE>
	struct start_view_setup
	{
		START_TYPE view; 
		model_iface_ptr model ;
		ManagerWindowListenerFake listener ;
		view_interface_fake interface_view ;

		start_view_setup(size_t num=0) :
			model(new FelixModelInterfaceFake)
		{
			for (size_t i=0 ; i<=num ; ++i)
			{
				model->add_memory() ;
			}
			listener.m_message = L"foo" ;

			view.set_view(&interface_view) ;
			view.set_mem_model(model) ;
			view.set_gloss_model(model) ;
			view.set_listener(&listener) ;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// ViewState functions
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE(init)
	{
		BOOST_CHECK_NO_THROW(ManagerViewStart()) ;
	}
	BOOST_AUTO_TEST_CASE(init_mem)
	{
		BOOST_CHECK_NO_THROW(ManagerViewStartMem()) ;
	}	
	BOOST_AUTO_TEST_CASE(init_gloss)
	{
		BOOST_CHECK_NO_THROW(ManagerViewStartGloss()) ;
	}

	// set_template_data
	BOOST_AUTO_TEST_CASE(set_template_data_gen)
	{
		start_view_setup<ManagerViewStart> setup(0u) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data) ;
		BOOST_CHECK(data.find(L"message") != data.end()) ;
		BOOST_CHECK(data.find(L"glosslist") != data.end()) ;
		BOOST_CHECK(data.find(L"memlist") != data.end()) ;
	}
	BOOST_AUTO_TEST_CASE(set_template_data_mem)
	{
		start_view_setup<ManagerViewStartMem> setup(0u) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data) ;
		BOOST_CHECK(data.find(L"message") != data.end()) ;
		BOOST_CHECK(data.find(L"glosslist") == data.end()) ;
		BOOST_CHECK(data.find(L"memlist") != data.end()) ;
	}
	BOOST_AUTO_TEST_CASE(set_template_data_gloss)
	{
		start_view_setup<ManagerViewStartGloss> setup(0u) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data) ;
		BOOST_CHECK(data.find(L"message") != data.end()) ;
		BOOST_CHECK(data.find(L"glosslist") != data.end()) ;
		BOOST_CHECK(data.find(L"memlist") == data.end()) ;
	}

	// show_content
	BOOST_AUTO_TEST_CASE(test_show_content_view)
	{
		start_view_setup<ManagerViewStart> setup(0u) ;

		cpptempl::data_map data ;
		setup.view.show_content() ;
		BOOST_CHECK_EQUAL(2u, setup.interface_view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("set_text", setup.interface_view.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(test_show_content_view_mem)
	{
		start_view_setup<ManagerViewStartMem> setup(0u) ;

		cpptempl::data_map data ;
		setup.view.show_content() ;
		BOOST_CHECK_EQUAL(2u, setup.interface_view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("set_text", setup.interface_view.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(test_show_content_view_gloss)
	{
		start_view_setup<ManagerViewStartGloss> setup(0u) ;

		cpptempl::data_map data ;
		setup.view.show_content() ;
		BOOST_CHECK_EQUAL(2u, setup.interface_view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("set_text", setup.interface_view.m_sensing_variable[0]) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
