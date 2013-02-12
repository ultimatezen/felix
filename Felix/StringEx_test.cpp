#include "StdAfx.h"
#include "stringex.h"

#include "File.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestStringEx )

	// is_fullwidth_lower
	BOOST_AUTO_TEST_CASE( is_fullwidth_lower_false )
	{
		wchar_t c = L'Ａ' ;
		BOOST_CHECK(! str::is_fullwidth_lower(c)) ;
	}
	BOOST_AUTO_TEST_CASE( is_fullwidth_lower_true )
	{
		wchar_t c = L'ａ' ;
		BOOST_CHECK(str::is_fullwidth_lower(c)) ;
	}

	// is_fullwidth_number
	BOOST_AUTO_TEST_CASE( is_fullwidth_number_true)
	{
		wchar_t c = L'４' ;
		BOOST_CHECK(str::is_fullwidth_number(c)) ;
	}
	BOOST_AUTO_TEST_CASE( is_fullwidth_number_false)
	{
		wchar_t c = L'Ａ' ;
		BOOST_CHECK(! str::is_fullwidth_number(c)) ;
	}

	// is_fullwidth_upper
	BOOST_AUTO_TEST_CASE( is_fullwidth_upper_true )
	{
		wchar_t c = L'Ａ' ;
		BOOST_CHECK(str::is_fullwidth_upper(c)) ;
	}
	BOOST_AUTO_TEST_CASE( is_fullwidth_upper_false )
	{
		wchar_t c = L'A' ;
		BOOST_CHECK(!str::is_fullwidth_upper(c)) ;
	}

	BOOST_AUTO_TEST_CASE( is_number_true)
	{
		wstring num_a = L"10" ;
		BOOST_CHECK(str::is_number(num_a)) ;

		wstring num_b = L"10%" ;
		BOOST_CHECK(str::is_number(num_b)) ;

		wstring num_c = L" 10.4 " ;
		BOOST_CHECK(str::is_number(num_c)) ;

		wstring num_d = L" 10% " ;
		BOOST_CHECK(str::is_number(num_d)) ;

		wstring num_e = L" 10.4% " ;
		BOOST_CHECK(str::is_number(num_e)) ;
	}
	BOOST_AUTO_TEST_CASE( is_number_comma_true)
	{
		wstring num_a = L"10,000" ;
		BOOST_CHECK(str::is_number(num_a)) ;
	}

	BOOST_AUTO_TEST_CASE( is_number_false)
	{
		wstring num_a = L"10 foo" ;
		BOOST_CHECK(! str::is_number(num_a)) ;

		wstring num_b = L"10% bar" ;
		BOOST_CHECK(! str::is_number(num_b)) ;

		wstring num_c = L" 10.4 spam" ;
		BOOST_CHECK(! str::is_number(num_c)) ;

		wstring num_d = L"egg 10% " ;
		BOOST_CHECK(! str::is_number(num_d)) ;

		wstring num_e = L"" ;
		BOOST_CHECK(! str::is_number(num_e)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( NormalizeWidthTest )

	// normalize_width
	BOOST_AUTO_TEST_CASE( FullWidthPercent )
	{
		file::view fview ;
		wstring ftext = L"％６０％" ;
		str::normalize_width( ftext ) ;
		// we skip character 0 because that is just the BOM
		BOOST_CHECK_EQUAL( L"%60%", ftext) ;
}
// normalize_fullwidth_upper
BOOST_AUTO_TEST_CASE( normalize_fullwidth_upper )
{
	wchar_t c = L'Ａ' ;
	BOOST_CHECK(L'A' == str::normalize_fullwidth_upper(c)) ;
}
// normalize_fullwidth_lower
BOOST_AUTO_TEST_CASE( normalize_fullwidth_lower )
{
	wchar_t c = L'ａ' ;
	BOOST_CHECK(L'a' == str::normalize_fullwidth_lower(c)) ;
}
// normalize_fullwidth_number
BOOST_AUTO_TEST_CASE( normalize_fullwidth_number)
{
	wchar_t c = L'４' ;
	BOOST_CHECK(L'4' == str::normalize_fullwidth_number(c)) ;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( double_byte_tests )

BOOST_AUTO_TEST_CASE( is_asian_true)
{
	wchar_t c = L'日' ;
	BOOST_CHECK(str::is_asian(c)) ;
}
BOOST_AUTO_TEST_CASE( is_asian_false)
{
	wchar_t c = L'a' ;
	BOOST_CHECK(! str::is_asian(c)) ;
}

BOOST_AUTO_TEST_CASE( asian_yes )
	{
		BOOST_CHECK(str::is_asian(L'自')) ;
		BOOST_CHECK(str::is_asian(L'ら')) ;
		BOOST_CHECK(str::is_asian(L'カ')) ;
		BOOST_CHECK(str::is_asian(L'。')) ;
		BOOST_CHECK(str::is_asian(L'ー')) ;
	}
BOOST_AUTO_TEST_CASE( asian_no )
	{
		BOOST_CHECK(! str::is_asian(L'a')) ;
		BOOST_CHECK(! str::is_asian(L'1')) ;
		BOOST_CHECK(! str::is_asian(L'~')) ;
	}

BOOST_AUTO_TEST_CASE( is_asian_false_is_double_byte_true)
	{
		wchar_t c = L'α' ; // U+03B1
		BOOST_CHECK(! str::is_asian(c)) ;
		BOOST_CHECK(str::is_double_byte(c)) ;
	}
BOOST_AUTO_TEST_CASE( is_double_byte_true)
	{
		wchar_t c = L'日' ;
		BOOST_CHECK(str::is_double_byte(c)) ;
	}
BOOST_AUTO_TEST_CASE( is_double_byte_false)
	{
		wchar_t c = L'a' ;
		BOOST_CHECK(! str::is_double_byte(c)) ;
	}

BOOST_AUTO_TEST_CASE( is_hiragana)
	{
		wstring actual   = L"あいうえおかきくけこがぎぐげこさしすせそざじずぜぞたちつてとだぢづでどなにぬねのはひふへほばびぶべぼぱぴぷぺぽまもむめもらりるれろわをやゆよゃゅょ" ;
		FOREACH(wchar_t c, actual)
		{
			BOOST_CHECK(str::is_hiragana(c)) ;
		}
	}
BOOST_AUTO_TEST_CASE( is_katakana)
	{
		wstring actual   = L"アイウエオカキクケコガギグゲコサシスセソザジズゼゾタチツテトダヂヅデドナニヌネノハヒフヘホバビブベボパピプペポマモムメモラリルレロワヲヤユヨャュョ" ;
		FOREACH(wchar_t c, actual)
		{
			BOOST_CHECK(str::is_katakana(c)) ;
		}
	}

BOOST_AUTO_TEST_CASE( normalize_hiragana_to_katakana_nihongo)
	{
		wstring actual   = L"にほんご" ;
		str::normalize_hiragana_to_katakana(actual) ;
		wstring expected =  L"ニホンゴ";
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_CASE( normalize_hiragana_to_katakana_mixed)
	{
		wstring actual   = L"Felixとはメモリ型翻訳支援ツールである。" ;
		str::normalize_hiragana_to_katakana(actual) ;
		wstring expected = L"Felixトハメモリ型翻訳支援ツールデアル。" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()