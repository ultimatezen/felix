#include "stdafx.h"
#include "ManagerViewBrowse.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"
#include "manager_window_listener_fake.h"
#include "view_interface_fake.h"

BOOST_AUTO_TEST_SUITE( TestManagerViewBrowse )

	using namespace mem_engine ;
	using namespace mgrview ;
	using namespace cpptempl ;

	struct browse_view_setup
	{
		ManagerViewBrowse view; 
		FelixModelInterfaceFake model ;
		ManagerWindowListenerFake listener ;
		view_interface_fake interface_view ;

		browse_view_setup(size_t num=0u) : view(num)
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
		BOOST_CHECK_NO_THROW(ManagerViewBrowse(0u)) ;
	}
	BOOST_AUTO_TEST_CASE(sets_message_on_content)
	{
		browse_view_setup setup(0u) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data) ;
		BOOST_CHECK(data.find(L"message") != data.end()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
