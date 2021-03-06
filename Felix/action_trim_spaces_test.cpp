#include "stdafx.h"
#include "action_trim_spaces.h"
#include "record_local.h"
#include <boost/test/unit_test.hpp>
#include "felix_factory.h"

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestTrimSpacesAction)

	namespace me = mem_engine ;
	using namespace app_props ;

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
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	action::ActionTrimSpaces trimmer(mem) ;
	BOOST_CHECK_EQUAL(trimmer.m_new->size(), 0u) ;
}

BOOST_AUTO_TEST_CASE(strip_one_record)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;

	action::ActionTrimSpaces trimmer(mem) ;
	trimmer.redo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L"<b>foo</b>" ;
	BOOST_CHECK_EQUAL(expected, after->get_source_rich()) ;
}
BOOST_AUTO_TEST_CASE(strip_one_record_old_has_info)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;
	mem->get_memory_info()->set_field(L"PB Gargling") ;

	action::ActionTrimSpaces trimmer(mem) ;
	trimmer.redo() ;
	wstring actual = trimmer.m_old->get_memory_info()->get_field() ;
	wstring expected = L"PB Gargling" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE(strip_one_record_trans)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	add_record(mem, " poop ", " <b> foo </b> ") ;

	action::ActionTrimSpaces trimmer(mem) ;
	trimmer.redo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L"<b>foo</b>" ;
	BOOST_CHECK_EQUAL(expected, after->get_trans_rich()) ;
}
BOOST_AUTO_TEST_CASE(undo_strip_one_record)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;

	action::ActionTrimSpaces trimmer(mem) ;
	trimmer.redo() ;
	trimmer.undo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L" <b> foo </b> " ;
	BOOST_CHECK_EQUAL(expected, after->get_source_rich()) ;
}
BOOST_AUTO_TEST_CASE(undo_trim_one_record_location)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	add_record(mem, "<b>foo</b>", "blarg") ;
	mem->set_location("c:\\foo\\bar.ftm") ;

	action::ActionTrimSpaces stripper(mem) ;
	stripper.redo() ;
	stripper.undo() ;
	wstring actual = static_cast<LPCWSTR>(stripper.m_new->get_location()) ;
	wstring expected = L"c:\\foo\\bar.ftm" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE(undo_trim_one_record_location_new)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	add_record(mem, "<b>foo</b>", "blarg") ;

	action::ActionTrimSpaces stripper(mem) ;
	stripper.redo() ;
	stripper.undo() ;
	wstring actual = static_cast<LPCWSTR>(stripper.m_new->get_location()) ;
	wstring expected = L"New" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE(undo_strip_field)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	add_record(mem, "<b>foo</b>", "blarg") ;
	mem->get_memory_info()->set_field(L"PB Gargling") ;

	action::ActionTrimSpaces trimmer(mem) ;
	trimmer.redo() ;
	trimmer.undo() ;
	wstring actual = trimmer.m_new->get_memory_info()->get_field() ;
	wstring expected = L"PB Gargling" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE(get_name)
{
	me::memory_pointer mem(new me::memory_local(get_props())) ;
	action::ActionTrimSpaces trimmer(mem) ;
	BOOST_CHECK_EQUAL(trimmer.name(), wstring(L"Trim Spaces")) ;
}
BOOST_AUTO_TEST_SUITE_END()

#endif
