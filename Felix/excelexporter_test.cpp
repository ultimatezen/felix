#include "stdafx.h"
#include "excelexporter.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCExcelExporter, process_cell_text_empty)
	{
		wstring text ;
		wstring out = process_cell_text(text) ;
		ASSERT_TRUE_M(out.empty(), "Output of empty string should be empty") ;
	}
	TEST( TestCExcelExporter, process_cell_text_foo)
	{
		wstring text = L"foo";
		wstring out = process_cell_text(text) ;
		ASSERT_EQUALS_V(SimpleString(string2string(out).c_str()), "foo") ;
	}
	TEST( TestCExcelExporter, process_cell_text_equals_sign)
	{
		wstring text = L"=foo" ;
		wstring out = process_cell_text(text) ;
		ASSERT_EQUALS_V(SimpleString(string2string(out).c_str()), "\'=foo") ;
	}

}

#endif