#include "stdafx.h"
#include "AppStringEntry.h"

#ifdef UNIT_TEST


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( CAppStringEntryTest )

	BOOST_AUTO_TEST_CASE(test_GetSet )
	{
		CAppStringEntry entry ;
		entry.set_entry( L"text", L"foo" ) ;
		entry.set_entry( L"tooltip", L"bar" ) ;
		entry.set_entry( L"id", L"foo2" ) ;
		entry.set_entry( L"type", L"bar2" ) ;
		BOOST_CHECK_EQUAL( L"foo", entry.get_entry(L"text") ) ;
		BOOST_CHECK_EQUAL( L"bar", entry.get_entry(L"tooltip") ) ;
		BOOST_CHECK_EQUAL( L"foo2", entry.get_entry(L"id") ) ;
		BOOST_CHECK_EQUAL( L"bar2", entry.get_entry(L"type") ) ;

		CAppStringEntry entry2( entry ) ;
		BOOST_CHECK_EQUAL( L"foo", entry2.get_entry(L"text") ) ;
		BOOST_CHECK_EQUAL( L"bar", entry2.get_entry(L"tooltip") ) ;
		BOOST_CHECK_EQUAL( L"foo2", entry2.get_entry(L"id") ) ;
		BOOST_CHECK_EQUAL( L"bar2", entry2.get_entry(L"type") ) ;

		entry2.set_entry( L"text", L"test1" ) ;
		entry2.set_entry( L"tooltip", L"test2" ) ;

		BOOST_CHECK_EQUAL( L"test1", entry2.get_entry(L"text") ) ;
		BOOST_CHECK_EQUAL( L"test2", entry2.get_entry(L"tooltip") ) ;
	}

	BOOST_AUTO_TEST_CASE(test_GetSetFalse )
	{
		CAppStringEntry entry ;
		entry.set_entry( L"text", L"foo" ) ;
		entry.set_entry( L"tooltip", L"bar" ) ;

		BOOST_CHECK_EQUAL( L"foo", entry.get_entry(L"text") ) ;
		BOOST_CHECK_EQUAL( L"", entry.get_entry(L"idontexist") ) ;

		CAppStringEntry entry2( entry ) ;
		BOOST_CHECK_EQUAL( L"foo", entry2.get_entry(L"text") ) ;
		BOOST_CHECK_EQUAL( L"", entry2.get_entry(L"meneither") ) ;

		entry2.set_entry( L"text2", L"test1" ) ;
		BOOST_CHECK_EQUAL( L"test1", entry2.get_entry(L"text2") ) ;
		BOOST_CHECK_EQUAL( L"", entry2.get_entry(L"meneither") ) ;

		CAppStringEntry entry3 = entry2 ;
		BOOST_CHECK_EQUAL( L"foo", entry3.get_entry(L"text") ) ;
		BOOST_CHECK_EQUAL( L"", entry3.get_entry(L"idontexist") ) ;
	}

	BOOST_AUTO_TEST_CASE(test_AssignmentOperator )
	{
		CAppStringEntry entry ;
		entry.set_entry( L"text", L"foo" ) ;
		entry.set_entry( L"tooltip", L"bar" ) ;
		entry.set_entry( L"id", L"foo2" ) ;
		entry.set_entry( L"type", L"bar2" ) ;

		CAppStringEntry entry2 ;

		entry2 = entry ;

		BOOST_CHECK_EQUAL( L"foo", entry2.get_entry(L"text") ) ;
		BOOST_CHECK_EQUAL( L"bar", entry2.get_entry(L"tooltip") ) ;
		BOOST_CHECK_EQUAL( L"foo2", entry2.get_entry(L"id") ) ;
		BOOST_CHECK_EQUAL( L"bar2", entry2.get_entry(L"type") ) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif
