#include "stdafx.h"
#include "action_delete_matches.h"
#include "memory_local.h"
#include "record_local.h"
#include "search_match.h"
#include "FelixModelInterface.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"

BOOST_AUTO_TEST_SUITE(TestDeleteMatchesAction)

	using namespace mem_engine ;
	using namespace action ;
	typedef std::vector<search_match_ptr> match_vec ;

	record_pointer make_record(string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		return rec ;
	}
	search_match_ptr make_match(memory_pointer mem, record_pointer rec)
	{
		search_match_ptr match(new search_match) ;
		match->set_record(rec) ;
		mem->add_record(rec) ;
		match->set_memory_id(mem->get_id()) ;
		return match ;
	}

	BOOST_AUTO_TEST_CASE(instantiate)
	{
		FelixModelInterfaceFake model ;
		match_vec matches ;

		DeleteMatchesAction deleter(&model, matches) ;
		BOOST_CHECK_EQUAL(deleter.m_controller->size(), 0u) ;
	}

	BOOST_AUTO_TEST_CASE(delete_one)
	{
		FelixModelInterfaceFake model ;
		memory_pointer mem = model.add_memory() ;

		record_pointer rec = make_record("foo", "bar") ;

		match_vec matches ;
		matches.push_back(make_match(mem, rec)) ;

		BOOST_CHECK_EQUAL(mem->size(), 1u) ;

		DeleteMatchesAction deleter(&model, matches) ;
		deleter.redo() ;
		BOOST_CHECK_EQUAL(mem->size(), 0u) ;
	}

	BOOST_AUTO_TEST_CASE(delete_two)
	{
		FelixModelInterfaceFake model ;
		memory_pointer mem = model.add_memory() ;

		record_pointer rec1 = make_record("foo1", "bar") ;
		record_pointer rec2 = make_record("foo2", "bar") ;

		match_vec matches ;
		matches.push_back(make_match(mem, rec1)) ;
		matches.push_back(make_match(mem, rec2)) ;

		BOOST_CHECK_EQUAL(mem->size(), 2u) ;

		DeleteMatchesAction deleter(&model, matches) ;
		deleter.redo() ;
		BOOST_CHECK_EQUAL(mem->size(), 0u) ;
	}

	BOOST_AUTO_TEST_CASE(undo_delete_one)
	{
		FelixModelInterfaceFake model ;
		memory_pointer mem = model.add_memory() ;
		record_pointer rec = make_record("foo", "bar") ;

		match_vec matches ;
		matches.push_back(make_match(mem, rec)) ;

		BOOST_CHECK_EQUAL(mem->size(), 1u) ;

		DeleteMatchesAction deleter(&model, matches) ;
		deleter.redo() ;
		deleter.undo() ;
		BOOST_CHECK_EQUAL(mem->size(), 1u) ;
	}

	BOOST_AUTO_TEST_CASE(undo_delete_two)
	{
		FelixModelInterfaceFake model ;
		memory_pointer mem = model.add_memory() ;
		record_pointer rec1 = make_record("foo1", "bar") ;
		record_pointer rec2 = make_record("foo2", "bar") ;

		match_vec matches ;
		matches.push_back(make_match(mem, rec1)) ;
		matches.push_back(make_match(mem, rec2)) ;

		BOOST_CHECK_EQUAL(mem->size(), 2u) ;

		DeleteMatchesAction deleter(&model, matches) ;
		deleter.redo() ;
		deleter.undo() ;
		BOOST_CHECK_EQUAL(mem->size(), 2u) ;
	}
	//BOOST_AUTO_TEST_CASE(test_name)
	//{
	//	FelixModelInterfaceFake model ;
	//	match_vec matches ;

	//	DeleteMatchesAction deleter(&model, matches) ;
	//	BOOST_CHECK_EQUAL(deleter.name(), L"Delete Matches") ;
	//}
BOOST_AUTO_TEST_SUITE_END()

#endif
