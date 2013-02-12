#include "StdAfx.h"
#include "stringex.h"

#include "File.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestStringEx )

	// is_fullwidth_lower
	BOOST_AUTO_TEST_CASE( is_fullwidth_lower_false )
	{
		wchar_t c = L'�`' ;
		BOOST_CHECK(! str::is_fullwidth_lower(c)) ;
	}
	BOOST_AUTO_TEST_CASE( is_fullwidth_lower_true )
	{
		wchar_t c = L'��' ;
		BOOST_CHECK(str::is_fullwidth_lower(c)) ;
	}

	// is_fullwidth_number
	BOOST_AUTO_TEST_CASE( is_fullwidth_number_true)
	{
		wchar_t c = L'�S' ;
		BOOST_CHECK(str::is_fullwidth_number(c)) ;
	}
	BOOST_AUTO_TEST_CASE( is_fullwidth_number_false)
	{
		wchar_t c = L'�`' ;
		BOOST_CHECK(! str::is_fullwidth_number(c)) ;
	}

	// is_fullwidth_upper
	BOOST_AUTO_TEST_CASE( is_fullwidth_upper_true )
	{
		wchar_t c = L'�`' ;
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
		wstring ftext = L"���U�O��" ;
		str::normalize_width( ftext ) ;
		// we skip character 0 because that is just the BOM
		BOOST_CHECK_EQUAL( L"%60%", ftext) ;
}
// normalize_fullwidth_upper
BOOST_AUTO_TEST_CASE( normalize_fullwidth_upper )
{
	wchar_t c = L'�`' ;
	BOOST_CHECK(L'A' == str::normalize_fullwidth_upper(c)) ;
}
// normalize_fullwidth_lower
BOOST_AUTO_TEST_CASE( normalize_fullwidth_lower )
{
	wchar_t c = L'��' ;
	BOOST_CHECK(L'a' == str::normalize_fullwidth_lower(c)) ;
}
// normalize_fullwidth_number
BOOST_AUTO_TEST_CASE( normalize_fullwidth_number)
{
	wchar_t c = L'�S' ;
	BOOST_CHECK(L'4' == str::normalize_fullwidth_number(c)) ;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( double_byte_tests )

BOOST_AUTO_TEST_CASE( is_asian_true)
{
	wchar_t c = L'��' ;
	BOOST_CHECK(str::is_asian(c)) ;
}
BOOST_AUTO_TEST_CASE( is_asian_false)
{
	wchar_t c = L'a' ;
	BOOST_CHECK(! str::is_asian(c)) ;
}

BOOST_AUTO_TEST_CASE( asian_yes )
	{
		BOOST_CHECK(str::is_asian(L'��')) ;
		BOOST_CHECK(str::is_asian(L'��')) ;
		BOOST_CHECK(str::is_asian(L'�J')) ;
		BOOST_CHECK(str::is_asian(L'�B')) ;
		BOOST_CHECK(str::is_asian(L'�[')) ;
	}
BOOST_AUTO_TEST_CASE( asian_no )
	{
		BOOST_CHECK(! str::is_asian(L'a')) ;
		BOOST_CHECK(! str::is_asian(L'1')) ;
		BOOST_CHECK(! str::is_asian(L'~')) ;
	}

BOOST_AUTO_TEST_CASE( is_asian_false_is_double_byte_true)
	{
		wchar_t c = L'��' ; // U+03B1
		BOOST_CHECK(! str::is_asian(c)) ;
		BOOST_CHECK(str::is_double_byte(c)) ;
	}
BOOST_AUTO_TEST_CASE( is_double_byte_true)
	{
		wchar_t c = L'��' ;
		BOOST_CHECK(str::is_double_byte(c)) ;
	}
BOOST_AUTO_TEST_CASE( is_double_byte_false)
	{
		wchar_t c = L'a' ;
		BOOST_CHECK(! str::is_double_byte(c)) ;
	}

BOOST_AUTO_TEST_CASE( is_hiragana)
	{
		wstring actual   = L"�������������������������������������������������������ĂƂ����ÂłǂȂɂʂ˂̂͂Ђӂւق΂тԂׂڂς҂Ղ؂ۂ܂��ނ߂����������������" ;
		FOREACH(wchar_t c, actual)
		{
			BOOST_CHECK(str::is_hiragana(c)) ;
		}
	}
BOOST_AUTO_TEST_CASE( is_katakana)
	{
		wstring actual   = L"�A�C�E�G�I�J�L�N�P�R�K�M�O�Q�R�T�V�X�Z�\�U�W�Y�[�]�^�`�c�e�g�_�a�d�f�h�i�j�k�l�m�n�q�t�w�z�o�r�u�x�{�p�s�v�y�|�}����������������������������������" ;
		FOREACH(wchar_t c, actual)
		{
			BOOST_CHECK(str::is_katakana(c)) ;
		}
	}

BOOST_AUTO_TEST_CASE( normalize_hiragana_to_katakana_nihongo)
	{
		wstring actual   = L"�ɂق�" ;
		str::normalize_hiragana_to_katakana(actual) ;
		wstring expected =  L"�j�z���S";
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_CASE( normalize_hiragana_to_katakana_mixed)
	{
		wstring actual   = L"Felix�Ƃ̓������^�|��x���c�[���ł���B" ;
		str::normalize_hiragana_to_katakana(actual) ;
		wstring expected = L"Felix�g�n�������^�|��x���c�[���f�A���B" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()