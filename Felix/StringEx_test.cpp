#include "StdAfx.h"
#include "stringex.h"

#include "File.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	// join
	TEST(TestStringEx, join)
	{
		std::vector<string> items ;
		items += "spam", "eggs", "bacon" ;
		const string joined = str::join(" ", items) ;
		SimpleString expected = "spam eggs bacon" ;
		ASSERT_EQUALS_V(expected, SimpleString(joined.c_str())) ;
	}
	// starts_with
	TEST(TestStringEx, starts_with_true)
	{
		wstring text = L"fool" ;
		ASSERT_TRUE(str::starts_with(text, L"foo")) ;
	}
	TEST(TestStringEx, starts_with_false)
	{
		wstring text = L"fool" ;
		ASSERT_TRUE(!str::starts_with(text, L"fob")) ;
	}
	// ends_with
	TEST(TestStringEx, ends_with_true)
	{
		wstring text = L"fool" ;
		ASSERT_TRUE(str::ends_with(text, L"ool")) ;
	}
	TEST(TestStringEx, ends_with_false)
	{
		wstring text = L"fool" ;
		ASSERT_TRUE(!str::ends_with(text, L"fol")) ;
	}
	TEST(TestStringEx, ends_with_too_long)
	{
		wstring text = L"fool" ;
		ASSERT_TRUE(!str::ends_with(text, L"ofool")) ;
	}
	// is_fullwidth_lower
	TEST( TestStringEx, is_fullwidth_lower_false )
	{
		wchar_t c = L'Ａ' ;
		ASSERT_TRUE(! str::is_fullwidth_lower(c)) ;
	}
	TEST( TestStringEx, is_fullwidth_lower_true )
	{
		wchar_t c = L'ａ' ;
		ASSERT_TRUE(str::is_fullwidth_lower(c)) ;
	}

	// is_fullwidth_number
	TEST( TestStringEx, is_fullwidth_number_true)
	{
		wchar_t c = L'４' ;
		ASSERT_TRUE(str::is_fullwidth_number(c)) ;
	}
	TEST( TestStringEx, is_fullwidth_number_false)
	{
		wchar_t c = L'Ａ' ;
		ASSERT_TRUE(! str::is_fullwidth_number(c)) ;
	}

	// is_fullwidth_upper
	TEST( TestStringEx, is_fullwidth_upper_true )
	{
		wchar_t c = L'Ａ' ;
		ASSERT_TRUE(str::is_fullwidth_upper(c)) ;
	}
	TEST( TestStringEx, is_fullwidth_upper_false )
	{
		wchar_t c = L'A' ;
		ASSERT_TRUE(!str::is_fullwidth_upper(c)) ;
	}


	// normalize_fullwidth_upper
	TEST( TestStringEx, normalize_fullwidth_upper )
	{
		wchar_t c = L'Ａ' ;
		ASSERT_TRUE(L'A' == str::normalize_fullwidth_upper(c)) ;
	}
	// normalize_fullwidth_lower
	TEST( TestStringEx, normalize_fullwidth_lower )
	{
		wchar_t c = L'ａ' ;
		ASSERT_TRUE(L'a' == str::normalize_fullwidth_lower(c)) ;
	}
	// normalize_fullwidth_number
	TEST( TestStringEx, normalize_fullwidth_number)
	{
		wchar_t c = L'４' ;
		ASSERT_TRUE(L'4' == str::normalize_fullwidth_number(c)) ;
	}

	// normalize_width
	TEST( NormalizeWidthTest, FullWidthPercent )
	{
		file::view fview ;
		wstring ftext = (LPCWSTR)fview.create_view(_T("c:\\dev\\test files\\FullWidthPercent.txt")) ;
		str::normalize_width( ftext ) ;
		// we skip character 0 because that is just the BOM
		ASSERT_EQUALS_M( L"%60%", ftext.substr(1), "The percent signs must be narrowed properly" ) ;
	}
	TEST(TestStringEx, is_asian_true)
	{
		wchar_t c = L'日' ;
		ASSERT_TRUE(str::is_asian(c)) ;
	}
	TEST(TestStringEx, is_asian_false)
	{
		wchar_t c = L'a' ;
		ASSERT_TRUE(! str::is_asian(c)) ;
	}
	TEST( test_is_asian, yes )
	{
		ASSERT_TRUE(str::is_asian(L'自')) ;
		ASSERT_TRUE(str::is_asian(L'ら')) ;
		ASSERT_TRUE(str::is_asian(L'カ')) ;
		ASSERT_TRUE(str::is_asian(L'。')) ;
		ASSERT_TRUE(str::is_asian(L'ー')) ;
	}
	TEST( test_is_asian, no )
	{
		ASSERT_TRUE(! str::is_asian(L'a')) ;
		ASSERT_TRUE(! str::is_asian(L'1')) ;
		ASSERT_TRUE(! str::is_asian(L'~')) ;
	}

	TEST(TestStringEx, is_asian_false_is_double_byte_true)
	{
		wchar_t c = L'α' ; // U+03B1
		ASSERT_TRUE(! str::is_asian(c)) ;
		ASSERT_TRUE(str::is_double_byte(c)) ;
	}
	TEST(TestStringEx, is_double_byte_true)
	{
		wchar_t c = L'日' ;
		ASSERT_TRUE(str::is_double_byte(c)) ;
	}
	TEST(TestStringEx, is_double_byte_false)
	{
		wchar_t c = L'a' ;
		ASSERT_TRUE(! str::is_double_byte(c)) ;
	}
	TEST(TestStringEx, make_lower_wstring)
	{
		wstring actual = L"HELLO" ;
		str::make_lower(actual) ;
		wstring expected = L"hello" ;
		ASSERT_EQUALS(expected, actual) ;
	}
	TEST(TestStringEx, make_lower_string)
	{
		string actual = "HELLO, world" ;
		str::make_lower(actual) ;
		string expected = "hello, world" ;
		ASSERT_EQUALS(expected, actual) ;
	}
	TEST(TestStringEx, make_upper_wstring)
	{
		wstring actual = L"hello" ;
		str::make_upper(actual) ;
		wstring expected = L"HELLO" ;
		ASSERT_EQUALS(expected, actual) ;
	}
	TEST(TestStringEx, make_upper_string)
	{
		string actual = "hello, WORLD" ;
		str::make_upper(actual) ;
		string expected = "HELLO, WORLD" ;
		ASSERT_EQUALS(expected, actual) ;
	}

	TEST(TestStringEx, is_hiragana)
	{
		wstring actual   = L"あいうえおかきくけこがぎぐげこさしすせそざじずぜぞたちつてとだぢづでどなにぬねのはひふへほばびぶべぼぱぴぷぺぽまもむめもらりるれろわをやゆよゃゅょ" ;
		foreach(wchar_t c, actual)
		{
			ASSERT_TRUE(str::is_hiragana(c)) ;
		}
	}
	TEST(TestStringEx, is_katakana)
	{
		wstring actual   = L"アイウエオカキクケコガギグゲコサシスセソザジズゼゾタチツテトダヂヅデドナニヌネノハヒフヘホバビブベボパピプペポマモムメモラリルレロワヲヤユヨャュョ" ;
		foreach(wchar_t c, actual)
		{
			ASSERT_TRUE(str::is_katakana(c)) ;
		}
	}

	TEST(TestStringEx, normalize_hiragana_to_katakana_nihongo)
	{
		wstring actual   = L"にほんご" ;
		str::normalize_hiragana_to_katakana(actual) ;
		wstring expected =  L"ニホンゴ";
		ASSERT_EQUALS(expected, actual) ;
	}
	TEST(TestStringEx, normalize_hiragana_to_katakana_mixed)
	{
		wstring actual   = L"Felixとはメモリ型翻訳支援ツールである。" ;
		str::normalize_hiragana_to_katakana(actual) ;
		wstring expected = L"Felixトハメモリ型翻訳支援ツールデアル。" ;
		ASSERT_EQUALS(expected, actual) ;
	}

	TEST(TestStringEx, ltrim)
	{
		wstring original = L"   hello" ;
		CStringA actual = CW2A(str::ltrim(original).c_str()) ;
		ASSERT_EQUALS_V(CStringA("hello"), actual) ;
	}
	TEST(TestStringEx, rtrim)
	{
		wstring original = L"hello  " ;
		CStringA actual = CW2A(str::rtrim(original).c_str()) ;
		ASSERT_EQUALS_V(CStringA("hello"), actual) ;
	}
	TEST(TestStringEx, trim)
	{
		wstring original = L" \t  hello\n  " ;
		CStringA actual = CW2A(str::trim(original).c_str()) ;
		ASSERT_EQUALS_V(CStringA("hello"), actual) ;
	}

	TEST(TestStringEx, is_number_true)
	{
		wstring num_a = L"10" ;
		ASSERT_TRUE(str::is_number(num_a)) ;

		wstring num_b = L"10%" ;
		ASSERT_TRUE(str::is_number(num_b)) ;

		wstring num_c = L" 10.4 " ;
		ASSERT_TRUE(str::is_number(num_c)) ;

		wstring num_d = L" 10% " ;
		ASSERT_TRUE(str::is_number(num_d)) ;

		wstring num_e = L" 10.4% " ;
		ASSERT_TRUE(str::is_number(num_e)) ;
	}
	TEST(TestStringEx, is_number_comma_true)
	{
		wstring num_a = L"10,000" ;
		ASSERT_TRUE(str::is_number(num_a)) ;
	}

	TEST(TestStringEx, is_number_false)
	{
		wstring num_a = L"10 foo" ;
		ASSERT_TRUE(! str::is_number(num_a)) ;

		wstring num_b = L"10% bar" ;
		ASSERT_TRUE(! str::is_number(num_b)) ;

		wstring num_c = L" 10.4 spam" ;
		ASSERT_TRUE(! str::is_number(num_c)) ;

		wstring num_d = L"egg 10% " ;
		ASSERT_TRUE(! str::is_number(num_d)) ;

		wstring num_e = L"" ;
		ASSERT_TRUE(! str::is_number(num_e)) ;
	}

}

#endif