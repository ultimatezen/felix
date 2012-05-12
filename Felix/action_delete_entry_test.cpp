#include "stdafx.h"
#include "action_delete_entry.h"
#include "memory_local.h"
#include "record_local.h"
#include "search_match.h"
#include "felix_factory.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestDeleteEntryAction)

	namespace me = mem_engine ;

	me::record_pointer add_record(me::memory_pointer &mem, string source, string trans)
	{
		me::record_pointer rec(new me::record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		mem->add_record(rec) ;
		return rec ;
	}

	BOOST_AUTO_TEST_CASE(instantiate)
	{
		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
		me::record_pointer rec = add_record(mem, "foo", "blarg") ;
		action::ActionDeleteEntry deleter(mem, rec) ;
		BOOST_CHECK_EQUAL(deleter.m_mem->size(), 1u) ;
	}
	BOOST_AUTO_TEST_CASE(add_and_delete_one_record)
	{
		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
		me::record_pointer rec = add_record(mem, "foo", "blarg") ;

		action::ActionDeleteEntry deleter(mem, rec) ;
		BOOST_CHECK_EQUAL(deleter.m_mem->size(), 1u) ;
		deleter.redo() ;
		BOOST_CHECK_EQUAL(deleter.m_mem->size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(delete_and_undelete_one_record)
	{
		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
		me::record_pointer rec = add_record(mem, "foo", "blarg") ;

		action::ActionDeleteEntry deleter(mem, rec) ;
		deleter.redo() ;
		BOOST_CHECK_EQUAL(deleter.m_mem->size(), 0u) ;
		deleter.undo() ;
		BOOST_CHECK_EQUAL(deleter.m_mem->size(), 1u) ;
	}
	BOOST_AUTO_TEST_CASE(get_name)
	{
		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
		me::record_pointer rec = add_record(mem, "foo", "blarg") ;

		action::ActionDeleteEntry deleter(mem, rec) ;
		BOOST_CHECK_EQUAL(deleter.name(), wstring(L"Delete Entry")) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
