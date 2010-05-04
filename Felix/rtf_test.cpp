#include "StdAfx.h"
#include "rtf.h"


#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( rich_text_font_test )


	// rtf::my_font
	BOOST_AUTO_TEST_CASE( font_default_ctor)
	{
		rtf::my_font font ;
		BOOST_CHECK_EQUAL((int)font.get_charset(), (int)DEFAULT_CHARSET) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( font_table_entry_test )

	// rtf::font_table_entry
	BOOST_AUTO_TEST_CASE( font_table_entry_default_ctor)
	{
		rtf::font_table_entry  entry;

		BOOST_CHECK(entry.m_family.empty()) ;
		BOOST_CHECK(entry.m_charset.empty()) ;
		BOOST_CHECK(entry.m_pitch.empty()) ;
		BOOST_CHECK(entry.m_name.empty()) ;
		BOOST_CHECK(entry.m_code.empty()) ;
		BOOST_CHECK_EQUAL((int)entry.m_font.get_charset(), (int)DEFAULT_CHARSET) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( font_table_test )

	// rtf::font_table
	BOOST_AUTO_TEST_CASE( font_table_default_ctor)
	{
		rtf::font_table table ;
		BOOST_CHECK_EQUAL(0, (int)table.size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( rich_text_formatter_test )

	BOOST_AUTO_TEST_CASE( rich_text_formatter_init)
	{
		rtf::rich_text_formatter formatter ;
		BOOST_CHECK(formatter.m_package->mText.empty()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( format_package_test )

	BOOST_AUTO_TEST_CASE( format_packager_init)
	{
		rtf::format_package package ;
		BOOST_CHECK(! package.HasText()) ;
	}
	BOOST_AUTO_TEST_CASE( format_packager_copy_ctor )
	{
		rtf::format_package package1 ;
		package1.mText = L"text" ;
		rtf::format_package package2(package1) ;
		BOOST_CHECK(package2.HasText()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif