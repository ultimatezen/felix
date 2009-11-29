#include "stdafx.h"
#include "easyunit/testharness.h"
#include "AppStringEntry.h"

#ifdef _DEBUG


namespace easyunit
{
	TEST( CAppStringEntryTest, GetSet )
	{
		CAppStringEntry entry ;
		entry.set_entry( L"text", L"foo" ) ;
		entry.set_entry( L"tooltip", L"bar" ) ;
		entry.set_entry( L"id", L"foo2" ) ;
		entry.set_entry( L"type", L"bar2" ) ;
		ASSERT_EQUALS( L"foo", entry.get_entry(L"text") ) ;
		ASSERT_EQUALS( L"bar", entry.get_entry(L"tooltip") ) ;
		ASSERT_EQUALS( L"foo2", entry.get_entry(L"id") ) ;
		ASSERT_EQUALS( L"bar2", entry.get_entry(L"type") ) ;

		CAppStringEntry entry2( entry ) ;
		ASSERT_EQUALS( L"foo", entry2.get_entry(L"text") ) ;
		ASSERT_EQUALS( L"bar", entry2.get_entry(L"tooltip") ) ;
		ASSERT_EQUALS( L"foo2", entry2.get_entry(L"id") ) ;
		ASSERT_EQUALS( L"bar2", entry2.get_entry(L"type") ) ;

		entry2.set_entry( L"text", L"test1" ) ;
		entry2.set_entry( L"tooltip", L"test2" ) ;

		ASSERT_EQUALS( L"test1", entry2.get_entry(L"text") ) ;
		ASSERT_EQUALS( L"test2", entry2.get_entry(L"tooltip") ) ;
	}

	TEST( CAppStringEntryTest, GetSetFalse )
	{
		CAppStringEntry entry ;
		entry.set_entry( L"text", L"foo" ) ;
		entry.set_entry( L"tooltip", L"bar" ) ;

		ASSERT_EQUALS( L"foo", entry.get_entry(L"text") ) ;
		ASSERT_EQUALS( L"", entry.get_entry(L"idontexist") ) ;

		CAppStringEntry entry2( entry ) ;
		ASSERT_EQUALS( L"foo", entry2.get_entry(L"text") ) ;
		ASSERT_EQUALS( L"", entry2.get_entry(L"meneither") ) ;

		entry2.set_entry( L"text2", L"test1" ) ;
		ASSERT_EQUALS( L"test1", entry2.get_entry(L"text2") ) ;
		ASSERT_EQUALS( L"", entry2.get_entry(L"meneither") ) ;

		CAppStringEntry entry3 = entry2 ;
		ASSERT_EQUALS( L"foo", entry3.get_entry(L"text") ) ;
		ASSERT_EQUALS( L"", entry3.get_entry(L"idontexist") ) ;
	}

	TEST( CAppStringEntryTest, AssignmentOperator )
	{
		CAppStringEntry entry ;
		entry.set_entry( L"text", L"foo" ) ;
		entry.set_entry( L"tooltip", L"bar" ) ;
		entry.set_entry( L"id", L"foo2" ) ;
		entry.set_entry( L"type", L"bar2" ) ;

		CAppStringEntry entry2 ;

		entry2 = entry ;

		ASSERT_EQUALS( L"foo", entry2.get_entry(L"text") ) ;
		ASSERT_EQUALS( L"bar", entry2.get_entry(L"tooltip") ) ;
		ASSERT_EQUALS( L"foo2", entry2.get_entry(L"id") ) ;
		ASSERT_EQUALS( L"bar2", entry2.get_entry(L"type") ) ;
	}

}
#endif
