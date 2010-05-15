// ***************************************************************
//  @brief Unit tests for Felix_properties
//  -------------------------------------------------------------
//  match_score_test   version:  1.0   |  @date: 2006-07-09
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "StdAfx.h"
#include "Felix_properties.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

// test_properties_loaded_history
BOOST_AUTO_TEST_SUITE( test_properties_loaded_history )

	using namespace app_props ;

	BOOST_AUTO_TEST_CASE(initial_values)
	{
		properties_loaded_history props ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_mems) ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_gloss) ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_remote_mems) ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_remote_gloss) ;

		for (int i = 0 ; i < app_props::NumMems ; ++i)
		{
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_mems[i])) ;
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_glosses[i])) ;
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_remote_mems[i])) ;
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_remote_glosses[i])) ;
		}
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_properties_general )

	using namespace app_props ;

	BOOST_AUTO_TEST_CASE(initial_values)
	{
		properties_general props ;
		BOOST_CHECK_EQUAL((int)IDC_MEMSIZE_FULL, props.m_data.m_window_size) ;
		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, props.m_data.m_preferred_gui_lang) ;
		BOOST_CHECK(! props.m_data.m_load_prev_mem_on_startup) ;
		BOOST_CHECK(! props.m_data.m_load_prev_gloss_on_startup) ;
		BOOST_CHECK(props.m_data.m_show_markup) ;
		BOOST_CHECK(props.m_data.m_first_launch) ;
		BOOST_CHECK_EQUAL((int)IDC_MERGE, props.m_data.m_merge_choice) ;
		BOOST_CHECK(props.m_data.m_query_merge) ;
		BOOST_CHECK_EQUAL(L"Ryan", wstring(props.m_data.m_user_name)) ;
	}


	BOOST_AUTO_TEST_CASE( test_NumMems_15 )
	{
		BOOST_CHECK_EQUAL(15, NumMems) ;
	}
	BOOST_AUTO_TEST_CASE( user_name_default_Ryan)
	{
		properties_general props ;
		wstring actual(props.m_data.m_user_name); 
		wstring expected = L"Ryan" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif