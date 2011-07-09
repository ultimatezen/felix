#include "stdafx.h"
#include "action_trim_spaces.h"
#include "record_local.h"
#include <boost/test/unit_test.hpp>
#include "felix_factory.h"

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
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	action::TrimSpacesAction trimmer(mem) ;
	BOOST_CHECK_EQUAL(trimmer.m_new->size(), 0u) ;
}

BOOST_AUTO_TEST_CASE(strip_one_record)
{
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L"<b>foo</b>" ;
	BOOST_CHECK_EQUAL(expected, after->get_source_rich()) ;
}
BOOST_AUTO_TEST_CASE(strip_one_record_old_has_info)
{
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;
	mem->get_memory_info()->set_field(L"PB Gargling") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	wstring actual = trimmer.m_old->get_memory_info()->get_field() ;
	wstring expected = L"PB Gargling" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE(strip_one_record_trans)
{
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	add_record(mem, " poop ", " <b> foo </b> ") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L"<b>foo</b>" ;
	BOOST_CHECK_EQUAL(expected, after->get_trans_rich()) ;
}
BOOST_AUTO_TEST_CASE(undo_strip_one_record)
{
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	add_record(mem, " <b> foo </b> ", "blarg") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	trimmer.undo() ;
	me::record_pointer after = trimmer.m_new->get_record_at(0u) ;
	wstring expected = L" <b> foo </b> " ;
	BOOST_CHECK_EQUAL(expected, after->get_source_rich()) ;
}
BOOST_AUTO_TEST_CASE(undo_trim_one_record_location)
{
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	add_record(mem, "<b>foo</b>", "blarg") ;
	mem->set_location("c:\\foo\\bar.ftm") ;

	action::TrimSpacesAction stripper(mem) ;
	stripper.redo() ;
	stripper.undo() ;
	wstring actual = static_cast<LPCWSTR>(stripper.m_new->get_location()) ;
	wstring expected = L"c:\\foo\\bar.ftm" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE(undo_trim_one_record_location_new)
{
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	add_record(mem, "<b>foo</b>", "blarg") ;

	action::TrimSpacesAction stripper(mem) ;
	stripper.redo() ;
	stripper.undo() ;
	wstring actual = static_cast<LPCWSTR>(stripper.m_new->get_location()) ;
	wstring expected = L"New" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE(undo_strip_field)
{
	app_props::properties_memory props ;
	mem_engine::memory_pointer mem(new me::memory_local(&props)) ;
	add_record(mem, "<b>foo</b>", "blarg") ;
	mem->get_memory_info()->set_field(L"PB Gargling") ;

	action::TrimSpacesAction trimmer(mem) ;
	trimmer.redo() ;
	trimmer.undo() ;
	wstring actual = trimmer.m_new->get_memory_info()->get_field() ;
	wstring expected = L"PB Gargling" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}

BOOST_AUTO_TEST_SUITE_END()

#endif
