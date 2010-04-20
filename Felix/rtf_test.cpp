#include "StdAfx.h"
#include "rtf.h"


#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( rich_text_formatter_test )


	// rtf::my_font
	BOOST_AUTO_TEST_CASE( font_default_ctor)
	{
		rtf::my_font font ;
		BOOST_CHECK_EQUAL((int)font.get_charset(), (int)DEFAULT_CHARSET) ;
	}

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

	// rtf::font_table
	BOOST_AUTO_TEST_CASE( font_table_default_ctor)
	{
		rtf::font_table table ;
		BOOST_CHECK_EQUAL(0, (int)table.size()) ;
	}

	BOOST_AUTO_TEST_CASE( rich_text_formatter_init)
	{
		rtf::rich_text_formatter formatter ;
		BOOST_CHECK(formatter.m_package->mText.empty()) ;
	}

	BOOST_AUTO_TEST_SUITE_END()

#endif