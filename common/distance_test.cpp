#include "StdAfx.h"
#include "distance.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestDistance )

	using namespace std ;

	// min3
	BOOST_AUTO_TEST_CASE( test_min3 )
	{
		BOOST_CHECK_EQUAL( min3(10, 0, 5), 0u) ;
		BOOST_CHECK_EQUAL( min3(2, 5, 10), 2u) ;
		BOOST_CHECK_EQUAL( min3(5, 5, 5), 5u) ;
	}
	// edist
	BOOST_AUTO_TEST_CASE( test_edist_aaaa_aaba )
	{
		Distance d ;
		wstring spam = L"aaaa" ;
		wstring ham = L"aaba" ;
		BOOST_CHECK_EQUAL(d.edist(spam, ham), 1u) ;
		BOOST_CHECK_EQUAL(d.edist(ham, spam), 1u) ;
	}
	BOOST_AUTO_TEST_CASE( test_edist_aaba_aaaa )
	{
		Distance d ;
		wstring spam = L"aaba" ;
		wstring ham = L"aaaa" ;
		BOOST_CHECK_EQUAL(d.edist(spam, ham), 1u) ;
		BOOST_CHECK_EQUAL(d.edist(ham, spam), 1u) ;
	}
	BOOST_AUTO_TEST_CASE( test_edist_spam_ham )
	{
		Distance d ;
		wstring spam = L"spam" ;
		wstring ham = L"ham" ;
		BOOST_CHECK_EQUAL(d.edist(spam, ham), 2u) ;
		BOOST_CHECK_EQUAL(d.edist(ham, spam), 2u) ;
	}
	BOOST_AUTO_TEST_CASE( test_max_distance_exceeded )
	{
		Distance d ;
		wstring lhs = L"1234567890" ;
		wstring rhs = L"xxxxxx7890" ;
		BOOST_CHECK_EQUAL(d.edist(lhs, rhs), 10u) ;
		BOOST_CHECK_EQUAL(d.edist(rhs, lhs), 10u) ;
	}
	BOOST_AUTO_TEST_CASE( test_max_distance_not_exceeded )
	{
		Distance d ;
		d.set_minscore(0.4);
		wstring lhs = L"1234567890" ;
		wstring rhs = L"xxxxxx7890" ;
		BOOST_CHECK_EQUAL(d.edist(lhs, rhs), 6u) ;
		BOOST_CHECK_EQUAL(d.edist(rhs, lhs), 6u) ;
	}

	BOOST_AUTO_TEST_CASE( test_max_distance_exceeded_mid )
	{
		Distance d ;
		wstring lhs = L"1234567890" ;
		wstring rhs = L"12xxxxxx90" ;
		BOOST_CHECK_EQUAL(d.edist(lhs, rhs), 10u) ;
		BOOST_CHECK_EQUAL(d.edist(rhs, lhs), 10u) ;
	}
	BOOST_AUTO_TEST_CASE( test_max_distance_not_exceeded_mid )
	{
		Distance d ;
		d.set_minscore(0.4);
		wstring lhs = L"1234567890" ;
		wstring rhs = L"12xxxxxx90" ;
		BOOST_CHECK_EQUAL(d.edist(lhs, rhs), 6u) ;
		BOOST_CHECK_EQUAL(d.edist(rhs, lhs), 6u) ;
	}

	BOOST_AUTO_TEST_CASE( test_max_distance_exceeded_end )
	{
		Distance d ;
		wstring lhs = L"1234567890" ;
		wstring rhs = L"1234xxxxxx" ;
		BOOST_CHECK_EQUAL(d.edist(lhs, rhs), 10u) ;
		BOOST_CHECK_EQUAL(d.edist(rhs, lhs), 10u) ;
	}
	BOOST_AUTO_TEST_CASE( test_max_distance_not_exceeded_end )
	{
		Distance d ;
		d.set_minscore(0.4);
		wstring lhs = L"1234567890" ;
		wstring rhs = L"1234xxxxxx" ;
		BOOST_CHECK_EQUAL(d.edist(lhs, rhs), 6u) ;
		BOOST_CHECK_EQUAL(d.edist(rhs, lhs), 6u) ;
	}


	BOOST_AUTO_TEST_CASE( test_edist_nihon_nihongo )
	{
		Distance d ;
		wstring nihon = L"日本" ;
		wstring nihongo = L"日本語" ;
		BOOST_CHECK_EQUAL(d.edist(nihon, nihongo), 1u) ;
		BOOST_CHECK_EQUAL(d.edist(nihongo, nihon), 1u) ;
	}

	// edist_score
	BOOST_AUTO_TEST_CASE( test_edist_score_1 )
	{
		Distance d ;
		wstring a = L"foo" ;
		wstring b = L"foo" ;
		BOOST_CHECK_CLOSE(d.edist_score(a, b), 1.0, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE( test_edist_score_0_5 )
	{
		Distance d ;
		wstring a = L"aaa" ;
		wstring b = L"aaabbb" ;
		BOOST_CHECK_CLOSE(d.edist_score(a, b), 0.5, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE( test_edist_score_empty )
	{
		Distance d ;
		wstring a ;
		wstring b ;
		BOOST_CHECK_CLOSE(d.edist_score(a, b), 0.0, 0.0001) ;
	}

	// subdist
	BOOST_AUTO_TEST_CASE( test_subdist_spam_ham )
	{
		Distance d ;
		wstring needle = L"spam" ;
		wstring haystack = L"I love ham." ;
		BOOST_CHECK_EQUAL(d.subdist(needle, haystack), 2u) ;
	}
	BOOST_AUTO_TEST_CASE( test_subdist_weather )
	{
		Distance d ;
		wstring needle = L"晴天だ" ;
		wstring haystack = L"本日は晴天なり。" ;
		BOOST_CHECK_EQUAL(d.subdist(needle, haystack), 1u) ;
	}
	// subdist_score
	BOOST_AUTO_TEST_CASE( test_subdist_score_1 )
	{
		Distance d ;
		wstring needle = L"foo" ;
		wstring haystack = L"a fool and his money" ;
		BOOST_CHECK_CLOSE(d.subdist_score(needle, haystack), 1.0, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE( test_subdist_score_0 )
	{
		Distance d ;
		wstring needle = L"xxx" ;
		wstring haystack = L"yyy zzz" ;
		BOOST_CHECK_CLOSE(d.subdist_score(needle, haystack), 0.0, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE( test_subdist_score_0_empty_needle )
	{
		Distance d ;
		wstring needle ;
		wstring haystack = L"yyy zzz" ;
		BOOST_CHECK_CLOSE(d.subdist_score(needle, haystack), 0.0, 0.0001) ;
	}
	// len_1_edist
	BOOST_AUTO_TEST_CASE( test_len_1_edist_found )
	{
		Distance d ;
		wchar_t needle = L'a';
		wstring haystack = L"bbb a ccc" ;
		BOOST_CHECK_EQUAL(d.len_1_edist(needle, haystack.size(), haystack.c_str()), haystack.size() - 1u) ;
	}
	BOOST_AUTO_TEST_CASE( test_len_1_edist_not_found )
	{
		Distance d ;
		wchar_t needle = L'd';
		wstring haystack = L"bbb a ccc" ;
		BOOST_CHECK_EQUAL(d.len_1_edist(needle, haystack.size(), haystack.c_str()), haystack.size()) ;
	}

	// compute score
	BOOST_AUTO_TEST_CASE(compute_score_divide_0)
	{
		BOOST_CHECK_CLOSE(compute_score(0u, 5u), 0.0, 0.001) ;
	}
	BOOST_AUTO_TEST_CASE(compute_score_0_5)
	{
		BOOST_CHECK_CLOSE(compute_score(10u, 5u), 0.5, 0.001) ;
	}
	BOOST_AUTO_TEST_CASE(compute_score_1)
	{
		BOOST_CHECK_CLOSE(compute_score(5u, 0u), 1.0, 0.001) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif 
