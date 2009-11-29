#include "StdAfx.h"
#include "distance.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( TestDistance, test_min2 )
	{
		Distance d ;
		ASSERT_EQUALS( d.min2(10, 0), 0) ;
		ASSERT_EQUALS( d.min2(1, 10), 1) ;
		ASSERT_EQUALS( d.min2(5, 5), 5) ;
	}
	TEST( TestDistance, test_min3 )
	{
		Distance d ;
		ASSERT_EQUALS( d.min3(10, 0, 5), 0) ;
		ASSERT_EQUALS( d.min3(2, 5, 10), 2) ;
		ASSERT_EQUALS( d.min3(5, 5, 5), 5) ;
	}
	TEST( TestDistance, test_edist_spam_ham )
	{
		Distance d ;
		wstring spam = L"spam" ;
		wstring ham = L"ham" ;
		ASSERT_EQUALS_V((int)d.edist(spam, ham), 2) ;
		ASSERT_EQUALS_V((int)d.edist(ham, spam), 2) ;
	}
	TEST( TestDistance, test_edist_nihon_nihongo )
	{
		Distance d ;
		wstring nihon = L"日本" ;
		wstring nihongo = L"日本語" ;
		ASSERT_EQUALS_V((int)d.edist(nihon, nihongo), 1) ;
		ASSERT_EQUALS_V((int)d.edist(nihongo, nihon), 1) ;
	}

	TEST( TestDistance, test_subdist_spam_ham )
	{
		Distance d ;
		wstring needle = L"spam" ;
		wstring haystack = L"I love ham." ;
		ASSERT_EQUALS_V((int)d.subdist(needle, haystack), 2) ;
	}
	TEST( TestDistance, test_subdist_weather )
	{
		Distance d ;
		wstring needle = L"晴天だ" ;
		wstring haystack = L"本日は晴天なり。" ;
		ASSERT_EQUALS_V((int)d.subdist(needle, haystack), 1) ;
	}

}


#endif 
