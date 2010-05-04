#include "stdafx.h"

#include "AppStringRecordParser.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( CAppStringRecordParserTest, EmptyFile )
	{
		wstring empty_file_text ;
		CAppStringRecordParser parser( empty_file_text.c_str() ) ;
		ASSERT_EQUALS( false, parser.had_more_records() ) ;
	}

	TEST( CAppStringRecordParserTest, SingleRecordFile )
	{
		wstring file_text = L"<record>foo</record>" ;
		CAppStringRecordParser parser( file_text.c_str() ) ;
		ASSERT_EQUALS( true, parser.had_more_records() ) ;
	}

}
#endif
