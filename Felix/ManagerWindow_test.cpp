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
BOOST_AUTO_TEST_SUITE( TestCManagerWindowBasic )

	using namespace mem_engine ;

	// get_pos_arg
	BOOST_AUTO_TEST_CASE( get_pos_arg_back_1)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;
		wstring url = L"c:\\foo\\bar\\1\\deletefilter" ;
		BOOST_CHECK_EQUAL(1u, window.get_pos_arg(url)) ;
	}
	BOOST_AUTO_TEST_CASE( get_pos_arg_forward_1)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;
		wstring url = L"c:\\foo/bar/1/deletefilter" ;
		BOOST_CHECK_EQUAL(1u, window.get_pos_arg(url)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestCManagerWindowNavTests )

using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE(ends_with_pound)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"/foo#" ;
		BOOST_CHECK(! window.OnBeforeNavigate2(url)) ;
	}
	// empty
	BOOST_AUTO_TEST_CASE( test_nav_empty)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_empty") ;
	}
	// moving items in list
	BOOST_AUTO_TEST_CASE(get_pos_at_1)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model ;
		mem_model.add_memory() ;
		mem_model.add_memory() ;
		memory_pointer mem1 = mem_model.memory_at(1) ;

		memory_pointer retrieved = *window.get_pos_at(&mem_model, 1) ;

		BOOST_CHECK_EQUAL( mem1->get_id(), retrieved->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE(get_pos_at_0)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model ;
		mem_model.add_memory() ;
		mem_model.add_memory() ;
		memory_pointer mem0 = mem_model.memory_at(0) ;

		memory_pointer retrieved = *window.get_pos_at(&mem_model, 0) ;

		BOOST_CHECK_EQUAL(mem0->get_id(), retrieved->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE(swap_memories)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model ;
		mem_model.add_memory() ;
		mem_model.add_memory() ;
		memory_pointer mem1 = mem_model.memory_at(1) ;

		window.swap_memories(&mem_model, 0) ;

		BOOST_CHECK_EQUAL( mem1->get_id(), mem_model.memory_at(0)->get_id()) ;
	}

	BOOST_AUTO_TEST_CASE( test_nav_moveup)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		mem_model.add_memory() ;
		mem_model.add_memory() ;
		memory_pointer mem0 = mem_model.memory_at(0) ;
		memory_pointer mem1 = mem_model.memory_at(1) ;
		int id0 = mem0->get_id() ;
		int id1 = mem1->get_id() ;
		BOOST_CHECK(mem0->get_id() != mem1->get_id()) ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		mgrview::ManagerViewFake *view = new mgrview::ManagerViewFake ;
		window.m_current_state = mgrview::mgr_ptr(view) ;

		_bstr_t url = L"c:\\foo/1/mem/moveup" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_moveup") ;

		mem0 = mem_model.memory_at(0) ;
		mem1 = mem_model.memory_at(1) ;
		BOOST_CHECK_EQUAL(id0, mem1->get_id()) ;
		BOOST_CHECK_EQUAL(id1, mem0->get_id()) ;

		BOOST_CHECK_EQUAL(view->m_sensing_variable[0], "show_content") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_movedown)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		gloss_model.add_memory() ;
		gloss_model.add_memory() ;

		memory_pointer mem0 = gloss_model.memory_at(0) ;
		memory_pointer mem1 = gloss_model.memory_at(1) ;
		int id0 = mem0->get_id() ;
		int id1 = mem1->get_id() ;
		BOOST_CHECK(id0 != id1) ;

		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		mgrview::ManagerViewFake *view = new mgrview::ManagerViewFake ;
		window.m_current_state = mgrview::mgr_ptr(view) ;

		_bstr_t url = L"c:\\foo/0/gloss/movedown" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_movedown") ;

		mem0 = gloss_model.memory_at(0) ;
		mem1 = gloss_model.memory_at(1) ;
		BOOST_CHECK_EQUAL(id0, mem1->get_id()) ;
		BOOST_CHECK_EQUAL(id1, mem0->get_id()) ;

		BOOST_CHECK_EQUAL(view->m_sensing_variable[0], "show_content") ;
	}
	// navigation to various category pages
	BOOST_AUTO_TEST_CASE( test_nav_start)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/bar/1/start" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_start") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_memories)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/bar/1/memories" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_memories") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_glossaries)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/bar/1/glossaries" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_glossaries") ;
	}
	// crud
	BOOST_AUTO_TEST_CASE( test_nav_view)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		mem_model.add_memory() ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/0/mem/view" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_view") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_edit)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		mem_model.get_memories()->add_memory() ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/bar/1/edit" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_edit") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_browse)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		mem_model.get_memories()->add_memory() ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		// browse / mem_type / page / item
		_bstr_t url = L"c:\\foo/0/0/mem/browse" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_browse") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_remove)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/bar/1/remove" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_remove") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_addnew)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/bar/1/addnew" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_addnew") ;
	}
	BOOST_AUTO_TEST_CASE( test_nav_load)
	{
		CManagerWindow window ;
		FelixModelInterfaceFake mem_model, gloss_model ;
		window.set_mem_model(&mem_model) ;
		window.set_gloss_model(&gloss_model) ;

		_bstr_t url = L"c:\\foo/bar/1/load" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "nav_load") ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif