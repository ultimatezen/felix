// ***************************************************************
//  @brief Unit tests for \ref match_score.
//  -------------------------------------------------------------
//  match_score_test   version:  1.0   |  @date: 2006-07-09
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "StdAfx.h"
#include "match_score.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( match_scoreTestCase, Score )
	{
		match_score mscore ;

		ASSERT_EQUALS_DELTA( mscore.get_score(), 0.f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( mscore.GetBaseScore(), 0.f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( mscore.GetFormattingPenalty(), 0.f, 0.0001 ) ;
		ASSERT_EQUALS( false, mscore.HasFormattingPenalty() ) ;

		mscore.SetBaseScore( 0.5f ) ;
		ASSERT_EQUALS_DELTA( mscore.get_score(), 0.5f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( mscore.GetBaseScore(), 0.5f, 0.0001 ) ;

		mscore.SetFormattingPenalty( 0.1f ) ;
		ASSERT_EQUALS_DELTA( mscore.GetFormattingPenalty(), 0.1f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( mscore.GetBaseScore(), 0.5f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( mscore.get_score(), 0.4f, 0.0001 ) ;
		ASSERT_EQUALS( true, mscore.HasFormattingPenalty() ) ;
	}

	TEST( match_scoreTestCase, Assignment )
	{
		match_score mscore ;
		mscore.SetBaseScore( 0.5f ) ;
		mscore.SetFormattingPenalty( 0.1f ) ;

		match_score mscore2 = mscore ;

		ASSERT_EQUALS_DELTA( mscore2.GetFormattingPenalty(), 0.1f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( mscore2.GetBaseScore(), 0.5f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( mscore2.get_score(), 0.4f, 0.0001 ) ;
		ASSERT_EQUALS( true, mscore2.HasFormattingPenalty() ) ;

	}
}

#endif // #ifdef _DEBUG
