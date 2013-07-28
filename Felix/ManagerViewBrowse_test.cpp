#include "stdafx.h"
#include "ManagerViewBrowse.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"
#include "manager_window_listener_fake.h"
#include "view_interface_fake.h"
#include "Felix_properties.h"

BOOST_AUTO_TEST_SUITE( TestManagerViewBrowse )

	using namespace mem_engine ;
	using namespace mgrview ;
	using namespace cpptempl ;
	using namespace app_props ;

	// This sets up and initializes the view class
	struct browse_view_setup
	{
		ManagerViewBrowse view; 
		model_iface_ptr model ;
		ManagerWindowListenerFake listener ;
		view_interface_fake interface_view ;

		browse_view_setup(size_t num=0u) : view(num),
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
		BOOST_CHECK_NO_THROW(ManagerViewBrowse(0u)) ;
	}
	BOOST_AUTO_TEST_CASE(sets_message_on_content)
	{
		browse_view_setup setup(0u) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data) ;
		BOOST_CHECK(data.has(L"message")) ;
	}
	BOOST_AUTO_TEST_CASE(populate_item)
	{
		browse_view_setup setup(0u) ;

		cpptempl::data_map item ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"aaa") ;
		rec->set_trans(L"bbb") ;
		app_props::props_ptr prop(new properties) ;
		memory_pointer mem(new memory_local(prop)) ;
		mem->add_record(rec) ;

		setup.view.populate_item(item, 0u, mem) ;

		BOOST_CHECK_EQUAL(item[L"num"]->getvalue(), L"1") ;
		BOOST_CHECK_EQUAL(item[L"num0"]->getvalue(), L"0") ;
		BOOST_CHECK_EQUAL(item[L"source"]->getvalue(), L"aaa") ;
		BOOST_CHECK_EQUAL(item[L"trans"]->getvalue(), L"bbb") ;
		BOOST_CHECK_EQUAL(item[L"mem"]->getvalue(), L"New") ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
