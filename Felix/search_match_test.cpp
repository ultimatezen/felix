
#include "StdAfx.h"
#include "search_match.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST
namespace easyunit
{
	using namespace memory_engine ;

	/************************************************************************
	 *																		*	
	 * Unit Tests                                                           *
	 *																		*	
	 ************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	// translation_match_compare
	//////////////////////////////////////////////////////////////////////////

	TEST( translation_match_compareTest, Reliability )
	{	
		search_match_ptr a(new search_match) ;
		record_pointer ra(new record_local()) ;
		ra->set_reliability( 3 ) ;
		a->set_record( ra ) ;

		search_match_ptr b(new search_match) ;
		record_pointer rb(new record_local()) ;
		rb->set_reliability( 4 ) ;
		b->set_record( rb ) ;

		ASSERT_TRUE( translation_match_compare()( b, a ) ) ;
		ASSERT_TRUE( ! translation_match_compare()( a, b ) ) ;
	}
	TEST( translation_match_compareTest, ReliabilityTie )
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

		ASSERT_TRUE( ! translation_match_compare()( b, a ) ) ;
		ASSERT_TRUE( ! translation_match_compare()( a, b ) ) ;
	}

	TEST( translation_match_compareTest, Validated )
	{	
		search_match_ptr a(new search_match) ;
		record_pointer ra(new record_local()) ;
		ra->set_validated_on() ;
		a->set_record( ra ) ;

		search_match_ptr b(new search_match) ;
		record_pointer rb(new record_local()) ;
		rb->set_validated_off() ;
		b->set_record( rb ) ;

		ASSERT_TRUE( translation_match_compare()( a, b ) ) ;
		ASSERT_TRUE( ! translation_match_compare()( b, a ) ) ;
	}
	TEST( translation_match_compareTest, ValidatedBothTrue )
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

		ASSERT_TRUE( ! translation_match_compare()( a, b ) ) ;
		ASSERT_TRUE( ! translation_match_compare()( b, a ) ) ;
	}
	TEST( translation_match_compareTest, ValidatedBothFalse )
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

		ASSERT_TRUE( ! translation_match_compare()( a, b ) ) ;
		ASSERT_TRUE( ! translation_match_compare()( b, a ) ) ;
	}

	TEST( translation_match_compareTest, Score )
	{	
		search_match_ptr a(new search_match) ;
		a->set_base_score( 0.0 ) ;

		search_match_ptr b(new search_match) ;
		b->set_base_score( 0.1 ) ;

		ASSERT_TRUE( translation_match_compare()( b, a ) ) ;
		ASSERT_TRUE( ! translation_match_compare()( a, b ) ) ;
	}

	TEST( translation_match_compareTest, ScoreTie )
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

		ASSERT_TRUE( ! translation_match_compare()( b, a ) ) ;
		ASSERT_TRUE( ! translation_match_compare()( a, b ) ) ;
	}

	//////////////////////////////////////////////////////////////////////////
	//  search_match
	//////////////////////////////////////////////////////////////////////////

	TEST( search_matchTestCase, Score )
	{
		search_match match ;

		ASSERT_EQUALS_DELTA( match.get_score(), 0.f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( match.get_base_score(), 0.f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( match.get_formatting_penalty(), 0.f, 0.0001 ) ;
		ASSERT_EQUALS( false, match.has_formatting_penalty() ) ;

		match.set_base_score( 0.5f ) ;
		ASSERT_EQUALS_DELTA( match.get_score(), 0.5f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( match.get_base_score(), 0.5f, 0.0001 ) ;

		match.set_formatting_penalty( 0.1f ) ;
		ASSERT_EQUALS_DELTA( match.get_formatting_penalty(), 0.1f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( match.get_base_score(), 0.5f, 0.0001 ) ;
		ASSERT_EQUALS_DELTA( match.get_score(), 0.4f, 0.0001 ) ;
		ASSERT_EQUALS( true, match.has_formatting_penalty() ) ;

	}

	TEST( search_matchTestCase, Markup )
	{
		search_match match ;

		markup_ptr pMarkup = match.get_markup() ;

		pMarkup->SetSource( L"source" ) ;
		pMarkup->SetTrans( L"trans" ) ;
		pMarkup->SetContext( L"context" ) ;

		ASSERT_EQUALS(  L"source", match.get_markup()->GetSource() ) ;
		ASSERT_EQUALS(  L"trans", match.get_markup()->GetTrans() ) ;
		ASSERT_EQUALS(  L"context", match.get_markup()->GetContext() ) ;

	}

	TEST( search_matchTestCase, Assignment )
	{
		search_match_ptr match1(new search_match), match2(new search_match) ;

		markup_ptr pMarkup = match1->get_markup() ;

		pMarkup->SetSource( L"source" ) ;
		pMarkup->SetTrans( L"trans" ) ;
		pMarkup->SetContext( L"context" ) ;

		match2 = match1 ;

		ASSERT_EQUALS(  L"source", match2->get_markup()->GetSource() ) ;
		ASSERT_EQUALS(  L"trans", match2->get_markup()->GetTrans() ) ;
		ASSERT_EQUALS(  L"context", match2->get_markup()->GetContext() ) ;
	}
}

#endif // #ifdef _DEBUG
