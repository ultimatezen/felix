#include "StdAfx.h"
#include "ConvertUTF.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_convert_utf8 )

	// symbol_map
	BOOST_AUTO_TEST_CASE(test_simple)
	{
		const char* from = "simple" ;
		size_t from_size = strlen(from) ;
		boost::scoped_array<wchar_t> to(new wchar_t[from_size+1]) ;
		const UTF8* from_begin = reinterpret_cast<const unsigned char*>(from) ;
		UTF16* to_begin = static_cast<UTF16*>(to.get()) ;
		ConversionResult result = ConvertUTF8toUTF16(&from_begin, 
													 from_begin + from_size, 
													 &to_begin, 
													 to_begin + from_size) ;
		BOOST_CHECK_EQUAL(result, conversionOK) ;
		const wstring actual(to.get()) ;
		BOOST_CHECK_EQUAL(actual, L"simple") ;
	}
	BOOST_AUTO_TEST_CASE(test_Utf82Utf16Converter_simple)
	{
		Utf82Utf16Converter converter ;
		string simple = "simple" ;
		const wstring result = converter.convert(simple.c_str(), simple.c_str() + simple.size()) ;
		BOOST_CHECK_EQUAL(result, L"simple") ;
	}
	BOOST_AUTO_TEST_CASE(test_Utf82Utf16Converter_twice)
	{
		Utf82Utf16Converter converter ;
		string simple = "simple" ;
		wstring result = converter.convert(simple.c_str(), simple.c_str() + simple.size()) ;
		string complex = "complex" ;
		result = converter.convert(complex.c_str(), complex.c_str() + complex.size()) ;
		BOOST_CHECK_EQUAL(result, L"complex") ;
	}
	BOOST_AUTO_TEST_CASE(test_Utf82Utf16Converter_257_chars)
	{
		Utf82Utf16Converter converter ;
		string simple ;
		simple.reserve(257) ;
		while(simple.size() < 257)
		{
			simple += 's' ;
		}
		wstring result = converter.convert(simple.c_str(), simple.c_str() + simple.size()) ;
		BOOST_CHECK_EQUAL(result, string2wstring(simple)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif