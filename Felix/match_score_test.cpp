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

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( match_scoreTestCase )


	BOOST_AUTO_TEST_CASE( Score )
	{
		match_score mscore ;

		BOOST_CHECK_CLOSE( mscore.get_score(), (double)0.f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.GetBaseScore(), (double)0.f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.GetFormattingPenalty(), (double)0.f, 0.0001 ) ;
		BOOST_CHECK_EQUAL( false, mscore.HasFormattingPenalty() ) ;

		mscore.SetBaseScore( 0.5f ) ;
		BOOST_CHECK_CLOSE( mscore.get_score(), (double)0.5f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.GetBaseScore(), (double)0.5f, 0.0001 ) ;

		mscore.SetFormattingPenalty( 0.1f ) ;
		BOOST_CHECK_CLOSE( mscore.GetFormattingPenalty(), (double)0.1f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.GetBaseScore(), (double)0.5f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.get_score(), (double)0.4f, 0.0001 ) ;
		BOOST_CHECK_EQUAL( true, mscore.HasFormattingPenalty() ) ;
	}

	BOOST_AUTO_TEST_CASE( copy_constructor )
	{
		match_score mscore ;
		mscore.SetBaseScore( 0.5f ) ;
		mscore.SetFormattingPenalty( 0.1f ) ;

		match_score mscore2(mscore) ;

		BOOST_CHECK_CLOSE( mscore2.GetFormattingPenalty(), (double)0.1f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore2.GetBaseScore(), (double)0.5f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore2.get_score(), (double)0.4f, 0.0001 ) ;
		BOOST_CHECK_EQUAL( true, mscore2.HasFormattingPenalty() ) ;

	}

	BOOST_AUTO_TEST_CASE( Assignment )
	{
		match_score mscore ;
		mscore.SetBaseScore( 0.5f ) ;
		mscore.SetFormattingPenalty( 0.1f ) ;

		match_score mscore2 ;
		
		mscore2 = mscore ;

		BOOST_CHECK_CLOSE( mscore2.GetFormattingPenalty(), (double)0.1f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore2.GetBaseScore(), (double)0.5f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore2.get_score(), (double)0.4f, 0.0001 ) ;
		BOOST_CHECK_EQUAL( true, mscore2.HasFormattingPenalty() ) ;

	}

BOOST_AUTO_TEST_SUITE_END()
