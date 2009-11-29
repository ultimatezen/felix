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

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	// defaults

	using namespace app_props ;

	TEST( app_props_tests, NumMems)
	{
		ASSERT_EQUALS_V(15, NumMems) ;
	}
	TEST( properties_general_tests, user_name_default_RyanVista)
	{
		properties_general props ;
		SimpleString actual = (LPCSTR)CStringA(props.m_data.m_user_name); 
		SimpleString expected = "Ryan" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

}

#endif // #ifdef _DEBUG
