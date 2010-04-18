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
BOOST_AUTO_TEST_SUITE( test_properties )


	// defaults

	using namespace app_props ;

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