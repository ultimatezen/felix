#include "stdafx.h"
#include "action_trim_spaces.h"
#include "record_local.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestTrimSpacesAction)

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
	me::memory_pointer mem(new me::memory_local) ;
	action::TrimSpacesAction trimmer(mem) ;
	BOOST_CHECK_EQUAL(trimmer.m_new->size(), 0u) ;
}

BOOST_AUTO_TEST_CASE(strip_one_record)
{
	me::memory_pointer mem(new me::memory_local) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L"<b>foo</b>" ;
	BOOST_CHECK_EQUAL(expected, after->get_source_rich()) ;
}
BOOST_AUTO_TEST_CASE(strip_one_record_trans)
{
	me::memory_pointer mem(new me::memory_local) ;
	add_record(mem, " poop ", " <b> foo </b> ") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L"<b>foo</b>" ;
	BOOST_CHECK_EQUAL(expected, after->get_trans_rich()) ;
}
BOOST_AUTO_TEST_CASE(undo_strip_one_record)
{
	me::memory_pointer mem(new me::memory_local) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	trimmer.undo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L" <b> foo </b> " ;
	BOOST_CHECK_EQUAL(expected, after->get_source_rich()) ;
}

BOOST_AUTO_TEST_SUITE_END()

#endif
