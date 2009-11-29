// ***************************************************************
//  @brief Unit tests for \ref CMatchString.
//  -------------------------------------------------------------
//  MatchString_test   version:  1.0   |  @date: 2006-07-09
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "StdAfx.h"
#include ".\matchstring.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( matchstringTestCase, PerfectMatch )
	{
		CMatchString ms ;

		ms.cell_match( L'a' ) ;
		ms.cell_match( L'b' ) ;
		ms.cell_match( L'c' ) ;

		ASSERT_EQUALS( L"cba", ms.mark_up_string() ) ;
	}

	TEST( matchstringTestCase, OneNotMatched )
	{
		CMatchString ms ;

		ms.cell_match( L'a' ) ;
		ms.cell_mismatch( L'b' ) ;
		ms.cell_match( L'c' ) ;

		ASSERT_EQUALS( L"c<b><span style=\"background:red\">b</span></b>a", ms.mark_up_string() ) ;

		// make sure that clears it
		ms.cell_match( L'a' ) ;
		ms.cell_mismatch( L'b' ) ;
		ms.cell_match( L'c' ) ;

		ASSERT_EQUALS( L"c<b><span style=\"background:red\">b</span></b>a", ms.mark_up_string() ) ;
	}

	TEST( matchstringTestCase, TwoNotMatched )
	{
		CMatchString ms ;

		ms.cell_match( L'a' ) ;
		ms.cell_mismatch( L'b' ) ;
		ms.cell_mismatch( L'c' ) ;
		ms.cell_match( L'd' ) ;

		ASSERT_EQUALS( L"d<b><span style=\"background:red\">cb</span></b>a", ms.mark_up_string() ) ;
	}
}

#endif 
