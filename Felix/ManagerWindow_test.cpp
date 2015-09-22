#include "StdAfx.h"
#include "ManagerWindow.h"
#include "record_local.h"
#include "memory_local.h"
#include "EditFormParser.h"
#include "document_wrapper_fake.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#include "felix_model_fake.h"
#include "ManagerViewFake.h"
#include "fake_undoable_action.h"
#include "frame_listener_fake.h"

struct ManagerWindowTestSetup
{
	model_iface_ptr mem_model ;
	model_iface_ptr gloss_model ;
	mgrview::ManagerViewFake *view ;
	FrameListenerFake listener ;

	ManagerWindowTestSetup(CManagerWindow *window) :
		mem_model(new FelixModelInterfaceFake),
		gloss_model(new FelixModelInterfaceFake)
	{
		window->m_listener = &listener ;
		window->set_mem_model(mem_model) ;
		window->set_gloss_model(gloss_model) ;
		view = new mgrview::ManagerViewFake ;
		window->m_current_state = mgrview::mgr_ptr(view) ;
	}
	void add_mems(size_t num)
	{
		for (size_t i = 0 ; i < num ; ++i)
		{
			mem_model->add_memory() ;
		}
	}
	void add_glosses(size_t num)
	{
		for (size_t i = 0 ; i < num ; ++i)
		{
			gloss_model->add_memory() ;
		}
	}
};

BOOST_AUTO_TEST_SUITE( TestCManagerWindowNavTests )

	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE(ends_with_pound)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"/foo#" ;
		BOOST_CHECK(! window.OnBeforeNavigate2(url)) ;
	}
	// empty
	BOOST_AUTO_TEST_CASE( test_nav_empty)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_empty") ;
	}
	// moving items in list
	BOOST_AUTO_TEST_CASE(get_mem_iter_at_1)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem1 = setup.mem_model->memory_at(1) ;

		memory_pointer retrieved = *window.get_mem_iter_at(setup.mem_model, 1) ;

		BOOST_CHECK_EQUAL( mem1->get_id(), retrieved->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE(get_mem_iter_at_0)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem0 = setup.mem_model->memory_at(0) ;

		memory_pointer retrieved = *window.get_mem_iter_at(setup.mem_model, 0) ;

		BOOST_CHECK_EQUAL(mem0->get_id(), retrieved->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE(swap_memories)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem1 = setup.mem_model->memory_at(1) ;

		window.swap_memories(setup.mem_model, 0) ;

		BOOST_CHECK_EQUAL( mem1->get_id(), setup.mem_model->memory_at(0)->get_id()) ;
	}

	BOOST_AUTO_TEST_CASE( test_nav_moveup)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem0 = setup.mem_model->memory_at(0) ;
		memory_pointer mem1 = setup.mem_model->memory_at(1) ;
		size_t id0 = mem0->get_id() ;
		size_t id1 = mem1->get_id() ;
		BOOST_CHECK(mem0->get_id() != mem1->get_id()) ;

		_bstr_t url = L"c:\\foo/1/mem/moveup" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_moveup") ;

		mem0 = setup.mem_model->memory_at(0) ;
		mem1 = setup.mem_model->memory_at(1) ;
		BOOST_CHECK_EQUAL(id0, mem1->get_id()) ;
		BOOST_CHECK_EQUAL(id1, mem0->get_id()) ;

		BOOST_CHECK_EQUAL(setup.view->m_sensing_variable[0], "show_content") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_movedown)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_glosses(2) ;

		memory_pointer mem0 = setup.gloss_model->memory_at(0) ;
		memory_pointer mem1 = setup.gloss_model->memory_at(1) ;
		size_t id0 = mem0->get_id() ;
		size_t id1 = mem1->get_id() ;
		BOOST_CHECK(id0 != id1) ;

		_bstr_t url = L"c:\\foo/0/gloss/movedown" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_movedown") ;

		mem0 = setup.gloss_model->memory_at(0) ;
		mem1 = setup.gloss_model->memory_at(1) ;
		BOOST_CHECK_EQUAL(id0, mem1->get_id()) ;
		BOOST_CHECK_EQUAL(id1, mem0->get_id()) ;

		BOOST_CHECK_EQUAL(setup.view->m_sensing_variable[0], "show_content") ;
	}
	// actions
	BOOST_AUTO_TEST_CASE(test_actions)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;

		action::FakeUndoableAction *fake = new action::FakeUndoableAction ;
		window.m_undo = action::undo_action_ptr(fake) ;
		_bstr_t url = L"/1/mem/actions" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "actions") ;
	}
	BOOST_AUTO_TEST_CASE(test_perform_action)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;

		action::FakeUndoableAction *fake = new action::FakeUndoableAction ;
		window.m_undo = action::undo_action_ptr(fake) ;

		_bstr_t url = L"/1/mem/trim/perform_action" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "perform_action") ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[2], "redo") ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[3], "nav_view") ;

		BOOST_CHECK_EQUAL(fake->m_sensing_variable[0], "redo") ;
	}
	// undo/redo
	BOOST_AUTO_TEST_CASE(test_undo)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;

		action::FakeUndoableAction *fake = new action::FakeUndoableAction ;
		window.m_undo = action::undo_action_ptr(fake) ;
		_bstr_t url = L"/1/mem/undo" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "undo") ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[2], "nav_view") ;

		BOOST_CHECK_EQUAL(fake->m_sensing_variable[0], "undo") ;
	}
	BOOST_AUTO_TEST_CASE(test_redo)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;

		action::FakeUndoableAction *fake = new action::FakeUndoableAction ;
		window.m_undo = action::undo_action_ptr(fake) ;
		_bstr_t url = L"/1/mem/redo" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "redo") ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[2], "nav_view") ;

		BOOST_CHECK_EQUAL(fake->m_sensing_variable[0], "redo") ;
	}
	BOOST_AUTO_TEST_CASE(test_undo_msg)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		action::FakeUndoableAction *fake = new action::FakeUndoableAction ;
		window.m_undo = action::undo_action_ptr(fake) ;

		std::vector<string> nav_tokens ;
		nav_tokens += "redo", "mem", "1" ;

		wstring actual = window.create_undo_msg(nav_tokens) ;
		wstring expected = L"Performed action [FakeUndoableAction]. <a href=\"/1/mem/undo\">Undo</a>" ;
		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE(test_redo_msg)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		action::FakeUndoableAction *fake = new action::FakeUndoableAction ;
		window.m_undo = action::undo_action_ptr(fake) ;

		std::vector<string> nav_tokens ;
		nav_tokens += "undo", "mem", "1" ;

		wstring actual = window.create_redo_msg(nav_tokens) ;
		wstring expected = L"Undid action [FakeUndoableAction]. <a href=\"/1/mem/redo\">Redo</a>" ;
		BOOST_CHECK_EQUAL(actual, expected) ;
	}

	// navigation to various category pages
	BOOST_AUTO_TEST_CASE(test_nav_start)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/1/start" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_start") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_memories)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/1/memories" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_memories") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_glossaries)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/1/glossaries" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_glossaries") ;
	}
	// crud
	BOOST_AUTO_TEST_CASE(test_nav_view)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		_bstr_t url = L"c:\\foo/0/mem/view" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_view") ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_edit)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		mgrview::ManagerViewFake *view = new mgrview::ManagerViewFake ;
		window.m_current_state = mgrview::mgr_ptr(view) ;

		_bstr_t url = L"c:\\0/mem/edit" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_edit") ;
	}
	BOOST_AUTO_TEST_CASE(test_submit_edit)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"creator")->set_attribute(L"value", L"Bozo") ;
		wrapper->get_element_by_id(L"field")->set_attribute(L"value", L"Ships") ;
		wrapper->get_element_by_id(L"created_on")->set_attribute(L"value", L"2010/08/01 11:11:11") ;
		wrapper->get_element_by_id(L"source_language")->set_attribute(L"value", L"Japanese") ;
		wrapper->get_element_by_id(L"target_language")->set_attribute(L"value", L"English") ;
		wrapper->get_element_by_id(L"client")->set_attribute(L"value", L"George Lucas") ;
		wrapper->get_element_by_id(L"mem_type_tm")->set_attribute(L"checked", L"true") ;
		wrapper->get_element_by_id(L"locked")->set_attribute(L"checked", L"true") ;


		mgrview::ManagerViewFake *view = new mgrview::ManagerViewFake ;
		window.m_current_state = mgrview::mgr_ptr(view) ;

		std::vector<string> tokens ;
		tokens += "submit_edit", "mem", "0" ;

		window.handle_edit_memory(tokens, doc) ;
		mem_engine::MemoryInfo *header = setup.mem_model->memory_at(0)->get_memory_info() ;
		BOOST_CHECK_EQUAL(header->get_creator(), L"Bozo") ;
		BOOST_CHECK_EQUAL(header->get_field(), L"Ships") ;
		BOOST_CHECK_EQUAL(header->get_created_on(), L"2010/08/01 11:11:11") ;
		BOOST_CHECK_EQUAL(header->get_source_language(), L"Japanese") ;
		BOOST_CHECK_EQUAL(header->get_target_language(), L"English") ;
		BOOST_CHECK_EQUAL(header->get_client(), L"George Lucas") ;
		BOOST_CHECK(header->is_memory()) ;
		BOOST_CHECK(header->is_locked()) ;
	}

	BOOST_AUTO_TEST_CASE(test_nav_browse)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		// browse / mem_type / page / item
		_bstr_t url = L"c:\\foo/0/0/mem/browse" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_browse") ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_browse_page)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		// browse / mem_type / page / item
		_bstr_t url = L"c:\\3/goto_page" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_browse_page") ;
	}	
	BOOST_AUTO_TEST_CASE(test_nav_remove)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		_bstr_t url = L"c:\\foo/0/mem/remove" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_remove") ;
		BOOST_CHECK_EQUAL(0u, setup.mem_model->size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_addnew_mem)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/mem/addnew" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_addnew") ;
		BOOST_CHECK_EQUAL(1u, setup.mem_model->size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_addnew_gloss)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/gloss/addnew" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_addnew") ;
		BOOST_CHECK_EQUAL(1u, setup.gloss_model->size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_load)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/mem/load" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_load") ;
	}
	BOOST_AUTO_TEST_CASE(test_submit_qc_no_glosses)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		app_props::properties_qc *qcprops = &props->m_qc_props ;

		qcprops->m_data.m_check_numbers = TRUE ;
		qcprops->m_data.m_check_all_caps = TRUE ;
		qcprops->m_data.m_check_gloss = TRUE ;
		qcprops->m_data.m_live_checking = TRUE ;
		qcprops->m_qc_glosses.push_back(L"c:\\first\\location") ;
		
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"numbers")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(L"allcaps")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(L"gloss")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(L"live")->set_attribute(L"checked", L"false") ;

		mgrview::ManagerViewFake *view = new mgrview::ManagerViewFake ;
		window.m_current_state = mgrview::mgr_ptr(view) ;

		std::vector<string> tokens ;
		tokens += "submit_qc" ;

		window.set_qc_prop_values(doc) ;

		BOOST_CHECK(! qcprops->m_data.m_check_numbers) ;
		BOOST_CHECK(! qcprops->m_data.m_check_all_caps) ;
		BOOST_CHECK(! qcprops->m_data.m_check_gloss) ;
		BOOST_CHECK(! qcprops->m_data.m_live_checking) ;
		BOOST_CHECK(qcprops->m_qc_glosses.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(test_submit_qc_glosses)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CManagerWindow window(props) ;
		ManagerWindowTestSetup setup(&window) ;

		app_props::properties_qc *qcprops = &props->m_qc_props ;

		qcprops->m_data.m_check_numbers = TRUE ;
		qcprops->m_data.m_check_all_caps = TRUE ;
		qcprops->m_data.m_check_gloss = TRUE ;
		qcprops->m_data.m_live_checking = TRUE ;
		qcprops->m_qc_glosses.push_back(L"c:\\first\\location") ;

		memory_pointer gloss1 = setup.gloss_model->add_memory() ;
		gloss1->set_location(L"gloss1") ;
		memory_pointer gloss2 = setup.gloss_model->add_memory() ;
		gloss2->set_location(L"gloss2") ;

		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"numbers")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(L"allcaps")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(L"gloss")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(L"live")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(boost::lexical_cast<wstring>(gloss1->get_id()))->set_attribute(L"checked", L"true") ;
		wrapper->get_element_by_id(boost::lexical_cast<wstring>(gloss2->get_id()))->set_attribute(L"checked", L"false") ;

		mgrview::ManagerViewFake *view = new mgrview::ManagerViewFake ;
		window.m_current_state = mgrview::mgr_ptr(view) ;

		std::vector<string> tokens ;
		tokens += "submit_qc" ;

		window.set_qc_prop_values(doc) ;

		BOOST_CHECK_EQUAL(1u, qcprops->m_qc_glosses.size()) ;
		BOOST_CHECK_EQUAL(qcprops->m_qc_glosses[0], wstring(L"gloss1")) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif