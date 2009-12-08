#include "StdAfx.h"
#include "StringConversions.h"
#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	// file_size2string
	TEST(StringConversions, file_size2string_6_Bytes)
	{
		SimpleString actual = file_size2string(6).c_str() ;
		SimpleString expected = "6 Bytes" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(StringConversions, file_size2string_64_KBytes)
	{
		SimpleString actual = file_size2string(64 * ONEK).c_str() ;
		SimpleString expected = "64 KBytes" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(StringConversions, file_size2string_6_MBytes)
	{
		SimpleString actual = file_size2string(6*ONEM).c_str() ;
		SimpleString expected = "6 MBytes" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(StringConversions, file_size2string_3_5_GBytes)
	{
		double oneg = static_cast<double>(ONEG) ;
		double mult = 3.5 ;
		size_t file_size = static_cast<size_t>(oneg * mult) ;
		SimpleString actual = file_size2string(file_size).c_str() ;
		SimpleString expected = "3.5 GBytes" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	// BSTR2wstring
	TEST( StringConversions, TestBSTR2wstring_bstr_t )
	{
		wstring w = BSTR2wstring( _bstr_t(L"foo") ) ;
		CStringA actual = CW2A(w.c_str()) ;
		ASSERT_EQUALS_V("foo", actual) ;
	}
	TEST( StringConversions, TestBSTR2wstring_CComBSTR )
	{
		wstring w = BSTR2wstring( CComBSTR(L"bar") ) ;
		CStringA actual = CW2A(w.c_str()) ;
		ASSERT_EQUALS_V("bar", actual) ;
	}

	TEST(StringConversions, TestStreamOperator_bstr)
	{
		_bstr_t bstr = L"foo" ;
		wstring w ;
		w << static_cast<BSTR>(bstr) ;
		ASSERT_EQUALS_M( w, L"foo", "w should equal 'foo'" ) ;

		w << L"bar" ;
		ASSERT_EQUALS_M( w, L"foobar", "w should equal 'foobar'" ) ;
	}
	TEST( StringConversions, string2string_from_wstring )
	{
		wstring w = L"Hello, world!" ;
		string s = string2string( w, CP_UTF8 ) ;
		ASSERT_EQUALS_M( s, "Hello, world!", "s should equal 'Hello, world!'" ) ;
	}
	TEST( StringConversions, string2string_from_string )
	{
		string s = "Goodbye!" ;
		string s2 = string2string( s ) ;
		ASSERT_EQUALS_M( s2, "Goodbye!", "s2 should equal 'Goodbye!'" ) ;
	}
	TEST( StringConversions, string2wstring_from_string )
	{
		string s = "Hello, world!" ;
		wstring w = string2wstring( s, CP_UTF8 ) ;
		ASSERT_EQUALS_M( w, L"Hello, world!", "w should equal 'Hello, world!'" ) ;

		s = "Goodbye!" ;
		w = string2wstring( s, CP_UTF8 ) ;
		ASSERT_EQUALS_M( w, L"Goodbye!", "w should equal 'Goodbye!'" ) ;
	}
	TEST( StringConversions, string2wstring_from_japanese )
	{
		wstring original = L"“ú–{Œê" ;
		string s = string2string(original, CP_UTF8) ;
		wstring w = string2wstring( s, CP_UTF8 ) ;
		ASSERT_EQUALS_M( w, L"“ú–{Œê", "w should equal '“ú–{Œê'" ) ;
	}
	TEST( StringConversions, string2wstring_from_wstring )
	{
		wstring w = L"Goodbye!" ;
		wstring w2 = string2wstring(w) ;
		ASSERT_EQUALS_M( w2, L"Goodbye!", "w2 should equal 'Goodbye!'" ) ;
	}
	TEST(StringConversions, string2double_wstring_ok)
	{
		wstring inval = L"2.5" ;
		double outval = string2double(inval) ;
		ASSERT_EQUALS_DELTA(outval, 2.5, 0.000001) ;
	}
	TEST(StringConversions, string2double_wstring_bad)
	{
		wstring inval = L"foo" ;
		try
		{
			string2double(inval) ;
			FAIL_M("Should have failed to convert 'foo'") ;
		}
		catch (CException&)
		{
			ASSERT_TRUE(true) ;
		}
	}
	TEST(StringConversions, double2string_prec_3)
	{
		CStringA actual = double2string(101.3, 3).c_str() ;
		CStringA expected = "101.300" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(StringConversions, double2string_prec_1)
	{
		CStringA actual = double2string(101.35, 1).c_str() ;
		CStringA expected = "101.3" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(StringConversions, double2percent_string_ok)
	{
		double val = 0.9 ;
		CStringA outval = double2percent_string(val).c_str() ;
		ASSERT_EQUALS_V("90%", outval) ;
	}

}

#endif // #ifdef _DEBUG
