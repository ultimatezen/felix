#include "stdafx.h"
#include "longest_common_substring.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( Test_LCS )

	BOOST_AUTO_TEST_CASE( first_empty )
	{
		LCS lcs ;
		wstring actual = lcs.longest_common_substring(L"", L"I love ponies") ;
		BOOST_CHECK_EQUAL(actual, L"") ;
	}
	BOOST_AUTO_TEST_CASE( second_empty )
	{
		LCS lcs ;
		wstring actual = lcs.longest_common_substring(L"I love food", L"") ;
		BOOST_CHECK_EQUAL(actual, L"") ;
	}
	BOOST_AUTO_TEST_CASE( simple_start )
	{
		LCS lcs ;
		wstring actual = lcs.longest_common_substring(L"I love food", L"I love ponies") ;
		BOOST_CHECK_EQUAL(actual, L"I love ") ;
	}

	BOOST_AUTO_TEST_CASE( simple_end )
	{
		LCS lcs ;
		wstring actual = lcs.longest_common_substring(L"I hated raspberries", L"I love raspberries") ;
		BOOST_CHECK_EQUAL(actual, wstring(L" raspberries")) ;
	}
	BOOST_AUTO_TEST_CASE( simple_middle )
	{
		LCS lcs ;
		wstring actual = lcs.longest_common_substring(L"I shit on food", L"We shit on ponies") ;
		BOOST_CHECK_EQUAL(actual, L" shit on ") ;
	}
	BOOST_AUTO_TEST_CASE( set_start )
	{
		LCS lcs ;
		std::set<wstring> strings ;
		strings.insert(L"I love ponies") ;
		strings.insert(L"I love elephants") ;
		strings.insert(L"You can tell that I only love elephants") ;
		strings.insert(L"I love pot") ;
		strings.insert(L"pot is what I love") ;
		wstring actual = lcs.longest_common_substring(strings) ;
		BOOST_CHECK_EQUAL(actual, L" love") ;
	}
	BOOST_AUTO_TEST_CASE( set_empty )
	{
		LCS lcs ;
		std::set<wstring> strings ;
		wstring actual = lcs.longest_common_substring(strings) ;
		BOOST_CHECK_EQUAL(actual, L"") ;
	}
BOOST_AUTO_TEST_SUITE_END()
#endif