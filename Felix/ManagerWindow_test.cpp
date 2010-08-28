#include "StdAfx.h"
#include "ManagerWindow.h"
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#include "record_local.h"
#include "memory_local.h"
#include "felix_model_fake.h"
#include "ManagerViewFake.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

struct ManagerWindowTestSetup
{
	FelixModelInterfaceFake mem_model ;
	FelixModelInterfaceFake gloss_model ;
	mgrview::ManagerViewFake *view ;

	ManagerWindowTestSetup(CManagerWindow *window)
	{
		window->set_mem_model(&mem_model) ;
		window->set_gloss_model(&gloss_model) ;
		view = new mgrview::ManagerViewFake ;
		window->m_current_state = mgrview::mgr_ptr(view) ;
	}
	void add_mems(size_t num)
	{
		for (size_t i = 0 ; i < num ; ++i)
		{
			mem_model.add_memory() ;
		}
	}
	void add_glosses(size_t num)
	{
		for (size_t i = 0 ; i < num ; ++i)
		{
			gloss_model.add_memory() ;
		}
	}
};

BOOST_AUTO_TEST_SUITE( TestCManagerWindowNavTests )

using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE(ends_with_pound)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"/foo#" ;
		BOOST_CHECK(! window.OnBeforeNavigate2(url)) ;
	}
	// empty
	BOOST_AUTO_TEST_CASE( test_nav_empty)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_empty") ;
	}
	// moving items in list
	BOOST_AUTO_TEST_CASE(get_mem_iter_at_1)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem1 = setup.mem_model.memory_at(1) ;

		memory_pointer retrieved = *window.get_mem_iter_at(&setup.mem_model, 1) ;

		BOOST_CHECK_EQUAL( mem1->get_id(), retrieved->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE(get_mem_iter_at_0)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem0 = setup.mem_model.memory_at(0) ;

		memory_pointer retrieved = *window.get_mem_iter_at(&setup.mem_model, 0) ;

		BOOST_CHECK_EQUAL(mem0->get_id(), retrieved->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE(swap_memories)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem1 = setup.mem_model.memory_at(1) ;

		window.swap_memories(&setup.mem_model, 0) ;

		BOOST_CHECK_EQUAL( mem1->get_id(), setup.mem_model.memory_at(0)->get_id()) ;
	}

	BOOST_AUTO_TEST_CASE( test_nav_moveup)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(2) ;
		memory_pointer mem0 = setup.mem_model.memory_at(0) ;
		memory_pointer mem1 = setup.mem_model.memory_at(1) ;
		int id0 = mem0->get_id() ;
		int id1 = mem1->get_id() ;
		BOOST_CHECK(mem0->get_id() != mem1->get_id()) ;

		_bstr_t url = L"c:\\foo/1/mem/moveup" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_moveup") ;

		mem0 = setup.mem_model.memory_at(0) ;
		mem1 = setup.mem_model.memory_at(1) ;
		BOOST_CHECK_EQUAL(id0, mem1->get_id()) ;
		BOOST_CHECK_EQUAL(id1, mem0->get_id()) ;

		BOOST_CHECK_EQUAL(setup.view->m_sensing_variable[0], "show_content") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_movedown)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_glosses(2) ;

		memory_pointer mem0 = setup.gloss_model.memory_at(0) ;
		memory_pointer mem1 = setup.gloss_model.memory_at(1) ;
		int id0 = mem0->get_id() ;
		int id1 = mem1->get_id() ;
		BOOST_CHECK(id0 != id1) ;

		_bstr_t url = L"c:\\foo/0/gloss/movedown" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_movedown") ;

		mem0 = setup.gloss_model.memory_at(0) ;
		mem1 = setup.gloss_model.memory_at(1) ;
		BOOST_CHECK_EQUAL(id0, mem1->get_id()) ;
		BOOST_CHECK_EQUAL(id1, mem0->get_id()) ;

		BOOST_CHECK_EQUAL(setup.view->m_sensing_variable[0], "show_content") ;
	}
	// navigation to various category pages
	BOOST_AUTO_TEST_CASE( test_nav_start)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/1/start" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_start") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_memories)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/1/memories" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_memories") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_glossaries)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/1/glossaries" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_glossaries") ;
	}
	// crud
	BOOST_AUTO_TEST_CASE(test_nav_view)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		_bstr_t url = L"c:\\foo/0/mem/view" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_view") ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_edit)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		mgrview::ManagerViewFake *view = new mgrview::ManagerViewFake ;
		window.m_current_state = mgrview::mgr_ptr(view) ;

		_bstr_t url = L"c:\\foo/bar/1/edit" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_edit") ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_browse)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		// browse / mem_type / page / item
		_bstr_t url = L"c:\\foo/0/0/mem/browse" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_browse") ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_browse_page)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		// browse / mem_type / page / item
		_bstr_t url = L"c:\\3/goto_page" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_browse_page") ;
	}	
	BOOST_AUTO_TEST_CASE(test_nav_remove)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;
		setup.add_mems(1) ;

		_bstr_t url = L"c:\\foo/0/mem/remove" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_remove") ;
		BOOST_CHECK_EQUAL(0u, setup.mem_model.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_addnew_mem)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/mem/addnew" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_addnew") ;
		BOOST_CHECK_EQUAL(1u, setup.mem_model.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_addnew_gloss)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/gloss/addnew" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_addnew") ;
		BOOST_CHECK_EQUAL(1u, setup.gloss_model.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_nav_load)
	{
		CManagerWindow window ;
		ManagerWindowTestSetup setup(&window) ;

		_bstr_t url = L"c:\\foo/bar/mem/load" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_load") ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif