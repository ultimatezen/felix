// ***************************************************************
//  @brief Unit tests for match_score.
// ***************************************************************
#include "StdAfx.h"
#include "match_score.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( match_scoreTestCase )

	BOOST_AUTO_TEST_CASE( init )
	{
		match_score mscore ;

		BOOST_CHECK_CLOSE( mscore.get_score(), (double)0.f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.GetBaseScore(), (double)0.f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.GetFormattingPenalty(), (double)0.f, 0.0001 ) ;
		BOOST_CHECK_EQUAL( false, mscore.HasFormattingPenalty() ) ;
	}

	BOOST_AUTO_TEST_CASE( set_to_0_5 )
	{
		match_score mscore ;

		mscore.SetBaseScore( 0.5f ) ;
		BOOST_CHECK_CLOSE( mscore.get_score(), (double)0.5f, 0.0001 ) ;
		BOOST_CHECK_CLOSE( mscore.GetBaseScore(), (double)0.5f, 0.0001 ) ;
		BOOST_CHECK_EQUAL( false, mscore.HasFormattingPenalty() ) ;
	}

	BOOST_AUTO_TEST_CASE( formatting_penalty )
	{
		match_score mscore ;

		mscore.SetBaseScore( 0.5f ) ;
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
#endif
