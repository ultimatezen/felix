#include "StdAfx.h"
#include "segment.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG
namespace easyunit
{
	TEST( TestCmpmaker, test_ctor )
	{
		CmpMaker maker1(true, true, true) ;
		CmpMaker maker2(maker1) ;

		ASSERT_TRUE(maker2.m_ignore_width) ;
		ASSERT_TRUE(maker2.m_ignore_hira_kata) ;
		ASSERT_TRUE(maker2.m_ignore_case) ;
	}
	TEST( TestCmpmaker, test_assigment )
	{
		CmpMaker maker1 ;
		ASSERT_TRUE(! maker1.m_ignore_width) ;
		ASSERT_TRUE(! maker1.m_ignore_hira_kata) ;
		ASSERT_TRUE(! maker1.m_ignore_case) ;

		CmpMaker maker2(true, true, true) ;
		maker1 = maker2 ;

		ASSERT_TRUE(maker1.m_ignore_width) ;
		ASSERT_TRUE(maker1.m_ignore_hira_kata) ;
		ASSERT_TRUE(maker1.m_ignore_case) ;
	}
	TEST( TestCmpmaker, test_all_false )
	{
		CmpMaker maker ;
		wstring rich = L"<b>FOO</b><i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		ASSERT_EQUALS( stripped, L"FOOBAR" ) ;
	}
	TEST( TestCmpmaker, test_ignore_case )
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		wstring rich = L"<b>FOO</b><i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		ASSERT_EQUALS( stripped, L"foobar" ) ;
	}
	TEST( TestCmpmaker, test_ignore_width )
	{
		CmpMaker maker ;
		maker.m_ignore_width = true ;
		wstring rich = L"<b>FOO</b>ÇPÇP<i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		ASSERT_EQUALS( stripped, L"FOO11BAR" ) ;
	}
	TEST( TestCmpmaker, test_ignore_hira_kata )
	{
		CmpMaker maker ;
		maker.m_ignore_hira_kata = true ;
		wstring rich = L"<b>FOO</b>Ç†ÇŒ<i>BAR</i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		ASSERT_EQUALS( stripped, L"FOOÉAÉoBAR" ) ;
	}
	TEST( TestCmpmaker, test_all_true )
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		maker.m_ignore_width = true ;
		maker.m_ignore_hira_kata = true ;
		wstring rich = L"ÇPÇP<b> FOO </b>Ç†ÇŒ<i> BAR </i>" ;
		wstring stripped = maker.make_cmp( rich ) ;
		TRACE(stripped) ;
		ASSERT_EQUALS( stripped, L"11 foo ÉAÉo bar " ) ;
	}
	// Segment
	TEST(TestSegment, test_instantiate)
	{
		CmpMaker maker ;
		Segment segment(maker) ;
		ASSERT_EQUALS(segment.rich(), L"") ;
		ASSERT_EQUALS(segment.plain(), L"") ;
		ASSERT_EQUALS(segment.cmp(), L"") ;
	}
	TEST(TestSegment, test_instantiate_base_ctor)
	{
		Segment segment ;
		ASSERT_EQUALS(segment.rich(), L"") ;
		ASSERT_EQUALS(segment.plain(), L"") ;
		ASSERT_EQUALS(segment.cmp(), L"") ;
	}
	TEST(TestSegment, test_simple)
	{
		CmpMaker maker ;
		wstring value = L"<b>spam</b>" ;
		Segment segment(maker, value) ;
		ASSERT_EQUALS(segment.rich(), L"<b>spam</b>") ;
		ASSERT_EQUALS(segment.plain(), L"spam") ;
		ASSERT_EQUALS(segment.cmp(), L"spam") ;
	}
	TEST(TestSegment, test_simple_copy_ctor)
	{
		CmpMaker maker ;
		wstring value = L"<b>spam</b>" ;
		Segment segment(maker, value) ;
		Segment segment2(segment) ;
		ASSERT_EQUALS(segment2.rich(), L"<b>spam</b>") ;
		ASSERT_EQUALS(segment2.plain(), L"spam") ;
		ASSERT_EQUALS(segment2.cmp(), L"spam") ;
	}
	TEST(TestSegment, test_simple_assignment)
	{
		CmpMaker maker ;
		wstring value = L"<b>spam</b>" ;
		Segment segment(maker, value) ;
		Segment segment2 ;
		segment2 = segment ;
		ASSERT_EQUALS(segment2.rich(), L"<b>spam</b>") ;
		ASSERT_EQUALS(segment2.plain(), L"spam") ;
		ASSERT_EQUALS(segment2.cmp(), L"spam") ;
	}
	TEST(TestSegment, test_set_value_base_ctor)
	{
		wstring value = L"<b>spam</b>" ;
		Segment segment ;
		segment.set_value(value) ;
		ASSERT_EQUALS(segment.rich(), L"<b>spam</b>") ;
		ASSERT_EQUALS(segment.plain(), L"spam") ;
		ASSERT_EQUALS(segment.cmp(), L"spam") ;
	}
	TEST(TestSegment, test_ignore_case)
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		wstring value = L"<b>SPAM</b>" ;
		Segment segment(maker, value) ;
		ASSERT_EQUALS(segment.rich(), L"<b>SPAM</b>") ;
		ASSERT_EQUALS(segment.plain(), L"SPAM") ;
		ASSERT_EQUALS(segment.cmp(), L"spam") ;
	}
	TEST(TestSegment, test_set_value)
	{
		CmpMaker maker ;
		maker.m_ignore_case = true ;
		Segment segment(maker) ;
		wstring value = L"<b>SPAM</b>" ;
		segment.set_value(value) ;
		ASSERT_EQUALS(segment.rich(), L"<b>SPAM</b>") ;
		ASSERT_EQUALS(segment.plain(), L"SPAM") ;
		ASSERT_EQUALS(segment.cmp(), L"spam") ;
	}
	TEST(TestSegment, test_set_value_twice)
	{
		CmpMaker maker ;
		Segment segment(maker) ;
		wstring value = L"<b>eggs</b>" ;
		segment.set_value(value) ;
		wstring value2 = L"<span class='notmuch'><b>spam</b></span>" ;
		segment.set_value(value2) ;
		ASSERT_EQUALS(segment.rich(), L"<span class='notmuch'><b>spam</b></span>") ;
		ASSERT_EQUALS(segment.plain(), L"spam") ;
		ASSERT_EQUALS(segment.cmp(), L"spam") ;
	}
}
#endif
