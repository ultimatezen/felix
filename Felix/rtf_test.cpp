#include "StdAfx.h"
#include "rtf.h"
#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	// rtf::my_font
	TEST( test_my_font, default_ctor)
	{
		rtf::my_font font ;
		ASSERT_EQUALS_V((int)font.get_charset(), (int)DEFAULT_CHARSET) ;
	}

	// rtf::font_table_entry
	TEST( test_font_table_entry, default_ctor)
	{
		rtf::font_table_entry  entry;

		ASSERT_TRUE(entry.m_family.empty()) ;
		ASSERT_TRUE(entry.m_charset.empty()) ;
		ASSERT_TRUE(entry.m_pitch.empty()) ;
		ASSERT_TRUE(entry.m_name.empty()) ;
		ASSERT_TRUE(entry.m_code.empty()) ;
		ASSERT_EQUALS_V((int)entry.m_font.get_charset(), (int)DEFAULT_CHARSET) ;
	}

	// rtf::font_table
	TEST( test_font_table, default_ctor)
	{
		rtf::font_table table ;
		ASSERT_EQUALS_V(0, (int)table.size()) ;
	}

	TEST(rich_text_formatter_test, init)
	{
		rtf::rich_text_formatter formatter ;
		ASSERT_TRUE(formatter.m_package->mText.empty()) ;
	}


}

#endif 