#include "stdafx.h"
#include "ManagerViewActions.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"
#include "manager_window_listener_fake.h"
#include "view_interface_fake.h"

BOOST_AUTO_TEST_SUITE( TestManagerViewActions )

	using namespace mem_engine ;
	using namespace mgrview ;
	using namespace cpptempl ;

	// This sets up and initializes the view class
	struct action_view_setup
	{
		ManagerViewActions view; 
		FelixModelInterfaceFake model ;
		ManagerWindowListenerFake listener ;
		view_interface_fake interface_view ;

		action_view_setup(size_t num=0u) : view(num)
		{
			for (size_t i=0 ; i<=num ; ++i)
			{
				model.add_memory() ;
			}
			listener.m_message = L"foo" ;

			view.set_view(&interface_view) ;
			view.set_mem_model(&model) ;
			view.set_gloss_model(&model) ;
			view.set_listener(&listener) ;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// ViewState functions
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE(init)
	{
		BOOST_CHECK_NO_THROW(ManagerViewActions(0u)) ;
	}
	BOOST_AUTO_TEST_CASE(sets_message_on_content)
	{
		action_view_setup setup(0u) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data) ;
		BOOST_CHECK(data.find(L"message") != data.end()) ;
	}
	BOOST_AUTO_TEST_CASE(test_show_content_view)
	{
		action_view_setup setup(0u) ;

		cpptempl::data_map data ;
		setup.view.show_content() ;
		BOOST_CHECK_EQUAL(2u, setup.interface_view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("set_text", setup.interface_view.m_sensing_variable[0]) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
