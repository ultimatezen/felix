/*!
	@brief Unit tests for FelixModel class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "FelixModel.h"
#include "record_local.h"
#include "memory_local.h"

#ifdef UNIT_TEST
#include <boost/test/unit_test.hpp>
#include "memory_fake.h"

using namespace mem_engine ;

BOOST_AUTO_TEST_SUITE( TestFelixModel )

	record_pointer add_record(memory_pointer mem, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		mem->add_record(rec) ;
		return rec ;
	}
	record_pointer add_record(memory_pointer mem, string source, string trans)
	{
		wstring wsource = string2wstring(source) ;
		wstring wtrans = string2wstring(trans) ;
		return add_record(mem, wsource.c_str(), wtrans.c_str()) ;
	}


	/************************************************************************/
	/* tests start here                                                     */
	/************************************************************************/	

	// Tests for CMainFrame

	BOOST_AUTO_TEST_CASE(test_constructor)
	{
		app_props::props_ptr props(new app_props::properties) ;

		BOOST_CHECK_NO_THROW(FelixModel(props, true)) ;
	}

	BOOST_AUTO_TEST_CASE(test_size_0)
	{
		app_props::props_ptr props(new app_props::properties) ;
		FelixModel model(props) ;
		BOOST_CHECK_EQUAL(0u, model.size());
	}
	BOOST_AUTO_TEST_CASE(test_size_2)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;
		model.add_memory() ;
		model.add_memory() ;
		BOOST_CHECK_EQUAL(2u, model.size()) ;
	}

	BOOST_AUTO_TEST_CASE(test_get_memories_needing_saving_empty)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;
		memory_list memories ;
		model.get_memories_needing_saving(memories) ;
		BOOST_CHECK_EQUAL(0u, memories.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_remove_memory_by_id)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;
		auto mem = model.add_memory() ;
		BOOST_CHECK_EQUAL(1u, model.size());
		model.remove_memory_by_id(mem->get_id()) ;
		BOOST_CHECK_EQUAL(0u, model.size());
	}

	BOOST_AUTO_TEST_CASE(test_insert_memory)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model1(props) ;
		FelixModel model2(props) ;
		auto mem = model1.add_memory() ;
		BOOST_CHECK_EQUAL(0u, model2.size());
		model2.insert_memory(mem) ;
		BOOST_CHECK_EQUAL(1u, model2.size());
	}
	BOOST_AUTO_TEST_CASE(test_remove_record)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;
		auto mem1 = model.add_memory() ;
		auto mem2 = model.add_memory() ;
		record_pointer rec = add_record(mem1, "source1", "trans1") ;
		add_record(mem2, "source1", "trans1") ;
		BOOST_CHECK_EQUAL(1u, mem1->size());
		BOOST_CHECK_EQUAL(1u, mem2->size());
		model.remove_record(rec) ;
		BOOST_CHECK_EQUAL(0u, mem1->size());
		BOOST_CHECK_EQUAL(0u, mem2->size());
	}

	BOOST_AUTO_TEST_CASE(test_has_name_clash_empty)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;
		CString location = L"foo" ;
		BOOST_CHECK_EQUAL(false, model.has_name_clash(location));
	}
	BOOST_AUTO_TEST_CASE(test_has_name_clash_false)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;
		auto mem = model.add_memory();
		mem->set_location(L"D:\\dev\\testdata\\felix\\testgloss.fgloss") ;
		CString location = L"D:\\dev\\testdata\\felix\\testmem.ftm" ;
		BOOST_CHECK_EQUAL(false, model.has_name_clash(location));
	}
	BOOST_AUTO_TEST_CASE(test_has_name_clash_true)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;
		model.add_memory()->set_location(L"D:\\dev\\testdata\\felix\\testmem.ftm") ;
		model.add_memory()->set_location(L"D:\\dev\\testdata\\felix\\testmem.ftm") ;
		CString location = L"D:\\dev\\testdata\\felix\\testmem.ftm" ;
		BOOST_CHECK_EQUAL(true, model.has_name_clash(location));
	}

	BOOST_AUTO_TEST_CASE(test_record_loaded_memories)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;

		memory_fake *mem1 = new memory_fake ;
		mem1->m_is_local = true ;
		memory_pointer memp1(mem1) ;
		memp1->set_location(L"c:\\foo.ftm") ;

		memory_fake *mem2 = new memory_fake ;
		mem2->m_is_local = false ;
		memory_pointer memp2(mem2) ;
		memp2->set_location(L"http://example.com/") ;

		model.insert_memory(memp1) ;
		model.insert_memory(memp2) ;

		std::vector<wstring> local, remote ;

		model.record_loaded_memories(local, remote) ;

		BOOST_CHECK_EQUAL(1u, local.size());
		BOOST_CHECK_EQUAL(1u, remote.size());

		BOOST_CHECK_EQUAL(L"c:\\foo.ftm", local[0]) ;
		BOOST_CHECK_EQUAL(L"http://example.com/", remote[0]) ;

	}

	BOOST_AUTO_TEST_CASE(test_get_memory_by_name)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;

		auto mem1 = model.add_memory();
		auto mem2 = model.add_memory();
		mem1->set_location(L"c:\\foo.ftm") ;
		mem2->set_location(L"c:\\bar.ftm") ;

		auto byname = model.get_memory_by_name(L"c:\\bar.ftm") ;

		BOOST_CHECK_EQUAL(wstring(L"c:\\bar.ftm"), wstring((LPCWSTR)byname->get_location()));
		BOOST_CHECK_EQUAL(wstring((LPCWSTR)mem2->get_location()), wstring((LPCWSTR)byname->get_location()));

		BOOST_CHECK_EQUAL(mem2->get_id(), byname->get_id()) ;
		BOOST_CHECK(mem1->get_id() != mem2->get_id()) ;
	}


	BOOST_AUTO_TEST_CASE(test_get_memory_names)
	{
		app_props::props_ptr props(new app_props::properties) ;

		FelixModel model(props) ;

		auto mem1 = model.add_memory();
		auto mem2 = model.add_memory();
		mem1->set_location(L"c:\\foo.ftm") ;
		mem2->set_location(L"c:\\bar.ftm") ;

		std::vector<wstring> names ;
		model.get_memory_names(names) ;

		BOOST_CHECK_EQUAL(2u, names.size());
		BOOST_CHECK_EQUAL(names[1], L"foo.ftm");
		BOOST_CHECK_EQUAL(names[0], L"bar.ftm");
	}


BOOST_AUTO_TEST_SUITE_END()

#endif 


