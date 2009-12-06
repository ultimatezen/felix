#include "StdAfx.h"
#include "pagination.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	// getting basic info
	TEST(TestPaginator, default_1)
	{
		Paginator paginator ;
		paginator.set_num_records(1) ;
		ASSERT_EQUALS_V(1, (int)paginator.get_num_pages()) ;
		ASSERT_EQUALS_V(0, (int)paginator.get_current_page()) ;
		ASSERT_EQUALS_V(0, (int)paginator.get_start()) ;
		ASSERT_EQUALS_V(1, (int)paginator.get_end()) ;
		ASSERT_TRUE(! paginator.has_next()) ;
		ASSERT_TRUE(! paginator.has_prev()) ;
	}

	TEST(TestPaginator, next_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		ASSERT_TRUE(! paginator.has_prev()) ;
		ASSERT_TRUE(paginator.has_next()) ;
		paginator.next_page() ;
		ASSERT_EQUALS_V(3, (int)paginator.get_num_pages()) ;
		ASSERT_EQUALS_V(1, (int)paginator.get_current_page()) ;
		ASSERT_EQUALS_V(20, (int)paginator.get_start()) ;
		ASSERT_EQUALS_V(40, (int)paginator.get_end()) ;
		ASSERT_TRUE(paginator.has_prev()) ;
	}
	TEST(TestPaginator, next_page_30)
	{
		Paginator paginator ;
		paginator.set_num_records(30) ;
		paginator.next_page() ;
		ASSERT_EQUALS_V(1, (int)paginator.get_current_page()) ;
		ASSERT_EQUALS_V(20, (int)paginator.get_start()) ;
		ASSERT_EQUALS_V(30, (int)paginator.get_end()) ;
		ASSERT_TRUE(paginator.has_prev()) ;
		ASSERT_TRUE(! paginator.has_next()) ;
	}


	TEST(TestPaginator, prev_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		paginator.next_page() ;
		ASSERT_EQUALS_V(1, (int)paginator.get_current_page()) ;
		paginator.prev_page() ;
		ASSERT_EQUALS_V(0, (int)paginator.get_current_page()) ;
		ASSERT_EQUALS_V(0, (int)paginator.get_start()) ;
		ASSERT_EQUALS_V(20, (int)paginator.get_end()) ;
		ASSERT_TRUE(! paginator.has_prev()) ;
		ASSERT_TRUE(paginator.has_next()) ;
	}

	// pagination text
	TEST(test_get_pagination_text, default_1)
	{
		Paginator paginator ;
		paginator.set_num_records(1) ;
		wstring text = get_pagination_text(paginator) ;

		ASSERT_TRUE(text.find(L"Previous") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"Next") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"/next_page") == wstring::npos) ;
		ASSERT_TRUE(text.find(L"/prev_page") == wstring::npos) ;
	}
	TEST(test_get_pagination_text, first_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		wstring text = get_pagination_text(paginator) ;

		ASSERT_TRUE(text.find(L"Previous") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"Next") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"/next_page") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"/prev_page") == wstring::npos) ;
	}
	TEST(test_get_pagination_text, next_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		paginator.next_page() ;
		wstring text = get_pagination_text(paginator) ;

		ASSERT_TRUE(text.find(L"Previous") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"Next") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"/next_page") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"/prev_page") != wstring::npos) ;
	}
}

#endif