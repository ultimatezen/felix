#include "StdAfx.h"
#include "distance.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestDistance )


	BOOST_AUTO_TEST_CASE( test_min2 )
	{
		Distance d ;
		BOOST_CHECK_EQUAL( d.min2(10, 0), 0u) ;
		BOOST_CHECK_EQUAL( d.min2(1, 10), 1u) ;
		BOOST_CHECK_EQUAL( d.min2(5, 5), 5u) ;
	}
	BOOST_AUTO_TEST_CASE( test_min3 )
	{
		Distance d ;
		BOOST_CHECK_EQUAL( d.min3(10, 0, 5), 0u) ;
		BOOST_CHECK_EQUAL( d.min3(2, 5, 10), 2u) ;
		BOOST_CHECK_EQUAL( d.min3(5, 5, 5), 5u) ;
	}
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
	BOOST_AUTO_TEST_CASE( test_edist_nihon_nihongo )
	{
		Distance d ;
		wstring nihon = L"日本" ;
		wstring nihongo = L"日本語" ;
		BOOST_CHECK_EQUAL(d.edist(nihon, nihongo), 1u) ;
		BOOST_CHECK_EQUAL(d.edist(nihongo, nihon), 1u) ;
	}

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

BOOST_AUTO_TEST_SUITE_END()


#endif 
