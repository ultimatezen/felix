#include "StdAfx.h"
#include "StringConversions.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_StringConversions )


// file_size2string
BOOST_AUTO_TEST_CASE( file_size2string_6_Bytes)
{
	string actual = file_size2string(6).c_str() ;
	string expected = "6 Bytes" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( file_size2string_64_KBytes)
{
	string actual = file_size2string(64 * ONEK).c_str() ;
	string expected = "64 KBytes" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( file_size2string_6_MBytes)
{
	string actual = file_size2string(6*ONEM).c_str() ;
	string expected = "6 MBytes" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( file_size2string_3_5_GBytes)
{
	double oneg = static_cast<double>(ONEG) ;
	double mult = 3.5 ;
	size_t file_size = static_cast<size_t>(oneg * mult) ;
	string actual = file_size2string(file_size).c_str() ;
	string expected = "3.5 GBytes" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}

// BSTR2wstring
BOOST_AUTO_TEST_CASE( TestBSTR2wstring_bstr_t )
{
	wstring w = BSTR2wstring( _bstr_t(L"foo") ) ;
	CStringA actual = CW2A(w.c_str()) ;
	BOOST_CHECK_EQUAL("foo", actual) ;
}
BOOST_AUTO_TEST_CASE( TestBSTR2wstring_CComBSTR )
{
	wstring w = BSTR2wstring( CComBSTR(L"bar") ) ;
	CStringA actual = CW2A(w.c_str()) ;
	BOOST_CHECK_EQUAL("bar", actual) ;
}

BOOST_AUTO_TEST_CASE( TestStreamOperator_bstr)
{
	_bstr_t bstr = L"foo" ;
	wstring w ;
	w << static_cast<BSTR>(bstr) ;
	BOOST_CHECK_EQUAL( w, L"foo") ;

	w << L"bar" ;
	BOOST_CHECK_EQUAL( w, L"foobar") ;
}
BOOST_AUTO_TEST_CASE( string2string_from_wstring )
{
	wstring w = L"Hello, world!" ;
	string s = string2string( w, CP_UTF8 ) ;
	BOOST_CHECK_EQUAL( s, "Hello, world!") ;
}
BOOST_AUTO_TEST_CASE( string2string_from_string )
{
	string s = "Goodbye!" ;
	string s2 = string2string( s ) ;
	BOOST_CHECK_EQUAL( s2, "Goodbye!") ;
}
BOOST_AUTO_TEST_CASE( string2wstring_from_string )
{
	string s = "Hello, world!" ;
	wstring w = string2wstring( s, CP_UTF8 ) ;
	BOOST_CHECK_EQUAL( w, L"Hello, world!") ;

	s = "Goodbye!" ;
	w = string2wstring( s, CP_UTF8 ) ;
	BOOST_CHECK_EQUAL( w, L"Goodbye!") ;
}
BOOST_AUTO_TEST_CASE( string2wstring_from_japanese )
{
	wstring original = L"“ú–{Œê" ;
	string s = string2string(original, CP_UTF8) ;
	wstring w = string2wstring( s, CP_UTF8 ) ;
	BOOST_CHECK_EQUAL( w, L"“ú–{Œê") ;
}
BOOST_AUTO_TEST_CASE( string2wstring_from_wstring )
{
	wstring w = L"Goodbye!" ;
	wstring w2 = string2wstring(w) ;
	BOOST_CHECK_EQUAL( w2, L"Goodbye!") ;
}
BOOST_AUTO_TEST_CASE( string2double_wstring_ok)
{
	wstring inval = L"2.5" ;
	double outval = string2double(inval) ;
	BOOST_CHECK_CLOSE(outval, (double)2.5, 0.000001) ;
}
BOOST_AUTO_TEST_CASE( string2double_wstring_bad)
{
	wstring inval = L"foo" ;
	BOOST_CHECK_THROW(string2double(inval), std::exception) ;
}
BOOST_AUTO_TEST_CASE( double2string_prec_3)
{
	CStringA actual = double2string(101.3, 3).c_str() ;
	CStringA expected = "101.300" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( double2string_prec_1)
{
	CStringA actual = double2string(101.35, 1).c_str() ;
	CStringA expected = "101.3" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( double2percent_string_ok)
{
	double val = 0.9 ;
	CStringA outval = double2percent_string(val).c_str() ;
	BOOST_CHECK_EQUAL("90%", outval) ;
}

BOOST_AUTO_TEST_SUITE_END()
