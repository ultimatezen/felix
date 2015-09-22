#include "stdafx.h"
#include "ManagerViewQCSettings.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"
#include "manager_window_listener_fake.h"
#include "view_interface_fake.h"

BOOST_AUTO_TEST_SUITE( TestManagerViewQCSettings )

using namespace mem_engine ;
using namespace mgrview ;
using namespace cpptempl ;

// This sets up and initializes the view class
struct qcsettings_view_setup
{
	ManagerViewQCSettings view; 
	model_iface_ptr model ;
	ManagerWindowListenerFake listener ;
	view_interface_fake interface_view ;

	qcsettings_view_setup(app_props::props_ptr props) : 
		view(props),
		model(new FelixModelInterfaceFake)
	{
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
	BOOST_AUTO_TEST_CASE(test_sets_message_on_content)
	{
		app_props::props_ptr props(new app_props::properties) ;
		qcsettings_view_setup setup(props) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data, & props->m_qc_props.m_data) ;
		BOOST_CHECK(data.has(L"message")) ;
		BOOST_CHECK(data[L"numbers"]->getvalue().empty()) ;
		BOOST_CHECK(data[L"allcaps"]->getvalue().empty()) ;
		BOOST_CHECK(data[L"gloss"]->getvalue().empty()) ;
		BOOST_CHECK(data[L"live"]->getvalue().empty()) ;
	}
	BOOST_AUTO_TEST_CASE(test_set_template_data_defaults)
	{
		app_props::props_ptr props(new app_props::properties) ;
		qcsettings_view_setup setup(props) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data, & props->m_qc_props.m_data) ;
		BOOST_CHECK(data[L"numbers"]->getvalue().empty()) ;
		BOOST_CHECK(data[L"allcaps"]->getvalue().empty()) ;
		BOOST_CHECK(data[L"gloss"]->getvalue().empty()) ;
		BOOST_CHECK(data[L"live"]->getvalue().empty()) ;
	}
	BOOST_AUTO_TEST_CASE(test_make_mem_list_empty)
	{
		app_props::props_ptr props(new app_props::properties) ;
		qcsettings_view_setup setup(props) ;

		cpptempl::data_map data ;
		setup.view.make_mem_list(setup.model, data) ;
		BOOST_CHECK(data[L"glosses"]->empty()) ;
	}
	BOOST_AUTO_TEST_CASE(test_make_mem_list)
	{
		app_props::props_ptr props(new app_props::properties) ;
		qcsettings_view_setup setup(props) ;

		memory_pointer mem1 = setup.model->add_memory() ;
		memory_pointer mem2 = setup.model->add_memory() ;

		mem1->set_location("c:\\first\\location") ;
		mem2->set_location("c:\\second\\location") ;

		cpptempl::data_map data ;
		setup.view.make_mem_list(setup.model, data) ;
		BOOST_CHECK_EQUAL(data[L"glosses"]->getlist().size(), 2u) ;
		BOOST_CHECK_EQUAL(data[L"glosses"]->getlist()[0]->getmap()[L"name"]->getvalue(), L"c:\\second\\location") ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
