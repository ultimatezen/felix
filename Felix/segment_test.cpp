#include "StdAfx.h"
#include "segment.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCmpmaker )

	BOOST_AUTO_TEST_CASE( test_ctor )
	{
		CmpMaker maker1(true, true, true) ;
		CmpMaker maker2(maker1) ;

		BOOST_CHECK(maker2.m_ignore_width) ;
		BOOST_CHECK(maker2.m_ignore_hira_kata) ;
		BOOST_CHECK(maker2.m_ignore_case) ;
	}
	BOOST_AUTO_TEST_CASE( test_assigment )
	{
		CmpMaker maker1 ;
		BOOST_CHECK(! maker1.m_ignore_width) ;
		BOOST_CHECK(! maker1.m_ignore_hira_kata) ;
		BOOST_CHECK(! maker1.m_ignore_case) ;

		CmpMaker maker2(true, true, true) ;
		maker1 = maker2 ;

		BOOST_CHECK(maker1.m_ignore_width) ;
		BOOST_CHECK(maker1.m_ignore_hira_kata) ;
		BOOST_CHECK(maker1.m_ignore_case) ;
	}
	BOOST_AUTO_TEST_CASE( test_all_false )
	{
		CmpMaker maker ;
		wstring rich = L"<b>FOO</b><i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"FOOBAR" ) ;
	}
	BOOST_AUTO_TEST_CASE( test_ignore_case )
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		wstring rich = L"<b>FOO</b><i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"foobar" ) ;
	}
	BOOST_AUTO_TEST_CASE( test_ignore_width )
	{
		CmpMaker maker ;
		maker.m_ignore_width = true ;
		wstring rich = L"<b>FOO</b>ÇPÇP<i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"FOO11BAR" ) ;
	}
	BOOST_AUTO_TEST_CASE( test_ignore_hira_kata )
	{
		CmpMaker maker ;
		maker.m_ignore_hira_kata = true ;
		wstring rich = L"<b>FOO</b>Ç†ÇŒ<i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"FOOÉAÉoBAR" ) ;
	}
	BOOST_AUTO_TEST_CASE( test_all_true )
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		maker.m_ignore_width = true ;
		maker.m_ignore_hira_kata = true ;
		wstring rich = L"ÇPÇP<b> FOO </b>Ç†ÇŒ<i> BAR </i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"11 foo ÉAÉo bar " ) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	// Segment
BOOST_AUTO_TEST_SUITE( TestSegment )
	BOOST_AUTO_TEST_CASE( test_instantiate)
	{
		CmpMaker maker ;
		Segment segment(maker) ;
		BOOST_CHECK_EQUAL(segment.rich(), L"") ;
		BOOST_CHECK_EQUAL(segment.plain(), L"") ;
		BOOST_CHECK_EQUAL(segment.cmp(), L"") ;
	}
	BOOST_AUTO_TEST_CASE( test_instantiate_base_ctor)
	{
		Segment segment ;
		BOOST_CHECK_EQUAL(segment.rich(), L"") ;
		BOOST_CHECK_EQUAL(segment.plain(), L"") ;
		BOOST_CHECK_EQUAL(segment.cmp(), L"") ;
	}
	BOOST_AUTO_TEST_CASE( test_simple)
	{
		CmpMaker maker ;
		wstring value = L"<b>spam</b>" ;
		Segment segment(maker, value) ;
		BOOST_CHECK_EQUAL(segment.rich(), L"<b>spam</b>") ;
		BOOST_CHECK_EQUAL(segment.plain(), L"spam") ;
		BOOST_CHECK_EQUAL(segment.cmp(), L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( test_simple_copy_ctor)
	{
		CmpMaker maker ;
		wstring value = L"<b>spam</b>" ;
		Segment segment(maker, value) ;
		Segment segment2(segment) ;
		BOOST_CHECK_EQUAL(segment2.rich(), L"<b>spam</b>") ;
		BOOST_CHECK_EQUAL(segment2.plain(), L"spam") ;
		BOOST_CHECK_EQUAL(segment2.cmp(), L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( test_simple_assignment)
	{
		CmpMaker maker ;
		wstring value = L"<b>spam</b>" ;
		Segment segment(maker, value) ;
		Segment segment2 ;
		segment2 = segment ;
		BOOST_CHECK_EQUAL(segment2.rich(), L"<b>spam</b>") ;
		BOOST_CHECK_EQUAL(segment2.plain(), L"spam") ;
		BOOST_CHECK_EQUAL(segment2.cmp(), L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( test_set_value_base_ctor)
	{
		wstring value = L"<b>spam</b>" ;
		Segment segment ;
		segment.set_value(value) ;
		BOOST_CHECK_EQUAL(segment.rich(), L"<b>spam</b>") ;
		BOOST_CHECK_EQUAL(segment.plain(), L"spam") ;
		BOOST_CHECK_EQUAL(segment.cmp(), L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( test_ignore_case)
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		wstring value = L"<b>SPAM</b>" ;
		Segment segment(maker, value) ;
		BOOST_CHECK_EQUAL(segment.rich(), L"<b>SPAM</b>") ;
		BOOST_CHECK_EQUAL(segment.plain(), L"SPAM") ;
		BOOST_CHECK_EQUAL(segment.cmp(), L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( test_set_value)
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		Segment segment(maker) ;
		wstring value = L"<b>SPAM</b>" ;
		segment.set_value(value) ;
		BOOST_CHECK_EQUAL(segment.rich(), L"<b>SPAM</b>") ;
		BOOST_CHECK_EQUAL(segment.plain(), L"SPAM") ;
		BOOST_CHECK_EQUAL(segment.cmp(), L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( test_set_value_twice)
	{
		CmpMaker maker ;
		Segment segment(maker) ;
		wstring value = L"<b>eggs</b>" ;
		segment.set_value(value) ;
		wstring value2 = L"<span class='notmuch'><b>spam</b></span>" ;
		segment.set_value(value2) ;
		BOOST_CHECK_EQUAL(segment.rich(), L"<span class='notmuch'><b>spam</b></span>") ;
		BOOST_CHECK_EQUAL(segment.plain(), L"spam") ;
		BOOST_CHECK_EQUAL(segment.cmp(), L"spam") ;
	}
BOOST_AUTO_TEST_SUITE_END()
#endif