
#include "StdAfx.h"
#include "search_match.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( translation_match_compareTest )

	using namespace mem_engine ;

	/************************************************************************
	 *																		*	
	 * Unit Tests                                                           *
	 *																		*	
	 ************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	// translation_match_compare
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( Reliability )
	{	
		search_match_ptr a(new search_match) ;
		record_pointer ra(new record_local()) ;
		ra->set_reliability( 3 ) ;
		a->set_record( ra ) ;

		search_match_ptr b(new search_match) ;
		record_pointer rb(new record_local()) ;
		rb->set_reliability( 4 ) ;
		b->set_record( rb ) ;

		BOOST_CHECK( translation_match_compare()( b, a ) ) ;
		BOOST_CHECK( ! translation_match_compare()( a, b ) ) ;
	}
	BOOST_AUTO_TEST_CASE( ReliabilityTie )
	{	
		search_match_ptr a(new search_match) ;
		record_pointer ra(new record_local()) ;
		ra->set_reliability( 3 ) ;
		a->set_record( ra ) ;

		search_match_ptr b(new search_match) ;
		record_pointer rb(new record_local()) ;
		rb->set_reliability( 3 ) ;
		b->set_record( rb ) ;

		// set dates equal to ensure tie
		misc_wrappers::date modified ;
		modified.set_to_local_time() ;

		ra->set_modified(modified) ;
		rb->set_modified(modified) ;

		BOOST_CHECK( ! translation_match_compare()( b, a ) ) ;
		BOOST_CHECK( ! translation_match_compare()( a, b ) ) ;
	}

	BOOST_AUTO_TEST_CASE( Validated )
	{	
		search_match_ptr a(new search_match) ;
		record_pointer ra(new record_local()) ;
		ra->set_validated_on() ;
		a->set_record( ra ) ;

		search_match_ptr b(new search_match) ;
		record_pointer rb(new record_local()) ;
		rb->set_validated_off() ;
		b->set_record( rb ) ;

		BOOST_CHECK( translation_match_compare()( a, b ) ) ;
		BOOST_CHECK( ! translation_match_compare()( b, a ) ) ;
	}
	BOOST_AUTO_TEST_CASE( ValidatedBothTrue )
	{	
		search_match_ptr a(new search_match) ;
		record_pointer ra(new record_local()) ;
		ra->set_validated_on() ;
		a->set_record( ra ) ;

		search_match_ptr b(new search_match) ;
		record_pointer rb(new record_local()) ;
		rb->set_validated_on() ;
		b->set_record( rb ) ;

		// set dates equal to ensure tie
		misc_wrappers::date modified ;
		modified.set_to_local_time() ;

		ra->set_modified(modified) ;
		rb->set_modified(modified) ;

		BOOST_CHECK( ! translation_match_compare()( a, b ) ) ;
		BOOST_CHECK( ! translation_match_compare()( b, a ) ) ;
	}
	BOOST_AUTO_TEST_CASE( ValidatedBothFalse )
	{	
		search_match_ptr a(new search_match) ;
		record_pointer ra(new record_local()) ;
		ra->set_validated_off() ;
		a->set_record( ra ) ;

		search_match_ptr b(new search_match) ;
		record_pointer rb(new record_local()) ;
		rb->set_validated_off() ;
		b->set_record( rb ) ;

		// set dates equal to ensure tie
		misc_wrappers::date modified ;
		modified.set_to_local_time() ;

		ra->set_modified(modified) ;
		rb->set_modified(modified) ;

		BOOST_CHECK( ! translation_match_compare()( a, b ) ) ;
		BOOST_CHECK( ! translation_match_compare()( b, a ) ) ;
	}

	BOOST_AUTO_TEST_CASE( Score )
	{	
		search_match_ptr a(new search_match) ;
		a->set_base_score( 0.0 ) ;

		search_match_ptr b(new search_match) ;
		b->set_base_score( 0.1 ) ;

		BOOST_CHECK( translation_match_compare()( b, a ) ) ;
		BOOST_CHECK( ! translation_match_compare()( a, b ) ) ;
	}

	BOOST_AUTO_TEST_CASE( ScoreTie )
	{	
		search_match_ptr a(new search_match) ;
		a->set_base_score( 0.5 ) ;

		search_match_ptr b(new search_match) ;
		b->set_base_score( 0.5 ) ;

		// set dates equal to ensure tie

		record_pointer ra(new record_local()) ;
		a->set_record( ra ) ;

		record_pointer rb(new record_local()) ;
		b->set_record( rb ) ;

		misc_wrappers::date modified ;
		modified.set_to_local_time() ;

		ra->set_modified(modified) ;
		rb->set_modified(modified) ;

		BOOST_CHECK( ! translation_match_compare()( b, a ) ) ;
		BOOST_CHECK( ! translation_match_compare()( a, b ) ) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	//  search_match
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( search_matchTestCase )
using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( Score )
	{
		search_match match ;

		BOOST_CHECK_CLOSE( match.get_score(), 0.0, 0.0001 ) ;
		BOOST_CHECK_CLOSE( match.get_base_score(), 0.0, 0.0001 ) ;
		BOOST_CHECK_CLOSE( match.get_formatting_penalty(), 0.0, 0.0001 ) ;
		BOOST_CHECK_EQUAL( false, match.has_formatting_penalty() ) ;

		match.set_base_score( 0.5f ) ;
		BOOST_CHECK_CLOSE( match.get_score(), 0.5, 0.0001 ) ;
		BOOST_CHECK_CLOSE( match.get_base_score(), 0.5, 0.0001 ) ;

		match.set_formatting_penalty( 0.1 ) ;
		BOOST_CHECK_CLOSE( match.get_formatting_penalty(), 0.1, 0.0001 ) ;
		BOOST_CHECK_CLOSE( match.get_base_score(), 0.5, 0.0001 ) ;
		BOOST_CHECK_CLOSE( match.get_score(), 0.4, 0.0001 ) ;
		BOOST_CHECK_EQUAL( true, match.has_formatting_penalty() ) ;

	}

	BOOST_AUTO_TEST_CASE( Markup )
	{
		search_match match ;

		markup_ptr pMarkup = match.get_markup() ;

		pMarkup->SetSource( L"source" ) ;
		pMarkup->SetTrans( L"trans" ) ;
		pMarkup->SetContext( L"context" ) ;

		BOOST_CHECK_EQUAL(  L"source", match.get_markup()->GetSource() ) ;
		BOOST_CHECK_EQUAL(  L"trans", match.get_markup()->GetTrans() ) ;
		BOOST_CHECK_EQUAL(  L"context", match.get_markup()->GetContext() ) ;

	}

	BOOST_AUTO_TEST_CASE( Assignment )
	{
		search_match_ptr match1(new search_match), match2(new search_match) ;

		markup_ptr pMarkup = match1->get_markup() ;

		pMarkup->SetSource( L"source" ) ;
		pMarkup->SetTrans( L"trans" ) ;
		pMarkup->SetContext( L"context" ) ;

		match2 = match1 ;

		BOOST_CHECK_EQUAL(  L"source", match2->get_markup()->GetSource() ) ;
		BOOST_CHECK_EQUAL(  L"trans", match2->get_markup()->GetTrans() ) ;
		BOOST_CHECK_EQUAL(  L"context", match2->get_markup()->GetContext() ) ;
	}
BOOST_AUTO_TEST_SUITE_END()
