/*!
	@brief implementation file for search_match class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "search_match.h"
#include "record_local.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif
namespace mem_engine
{

search_match::search_match() : 
	m_memory_id(0),
	m_HasPlacement(false),
	m_markup(new markup_strings),
	m_record(new record_local)
{
}

	search_match::search_match( const search_match &cpy )
	{
		internal_copy( cpy ) ;
	}
	// formatting penalty

void search_match::set_formatting_penalty(const double formatting_pentaly)
{
	m_score.SetFormattingPenalty( formatting_pentaly ) ;
}
const double search_match::get_formatting_penalty() const
{
	return m_score.GetFormattingPenalty() ;
}

const bool search_match::has_formatting_penalty() const 
{
	return m_score.HasFormattingPenalty() ;
}

// base score
void search_match::set_base_score(const double score)
{
	m_score.SetBaseScore( score ) ;
}
const double search_match::get_base_score() const
{
	return m_score.GetBaseScore() ;
}


// total score
const double search_match::get_score() const
{
	return m_score.get_score() ;
}

// record

record_pointer search_match::get_record()
{
	return m_record ;
}
void search_match::set_record( record_pointer rec )
{
	m_record = rec ;
}

// markup
markup_ptr search_match::get_markup()
{
	return m_markup ;
}

// memory
void search_match::set_memory_location(const wstring& memory_location)
{
	m_memory_location = memory_location;
}

const wstring& search_match::get_memory_location() const
{
	return m_memory_location;
}

//
void search_match::set_values_to_record()
{
	m_markup->SetSource( m_record->get_source_rich() ) ;
	m_markup->SetTrans( m_record->get_trans_rich() ) ;
	m_markup->SetContext( m_record->get_context_rich() ) ;
}

void search_match::internal_copy( const search_match &cpy )
{
	m_record = record_pointer( cpy.m_record->clone() ) ;
	*m_markup = *(cpy.m_markup) ;

	m_score = cpy.m_score  ;

	m_memory_location = cpy.m_memory_location ;
	m_memory_id = cpy.m_memory_id ;

	m_MatchPairing = cpy.m_MatchPairing ;
	m_HasPlacement = cpy.m_HasPlacement ;
}

search_match& search_match::operator=( const search_match &cpy )
{
	internal_copy( cpy ) ;
	return *this ;
}

void search_match::Placement()
{
	m_HasPlacement = true ;
}

bool search_match::HasPlacement()
{
	return m_HasPlacement ;
}

CMatchStringPairing& search_match::MatchPairing()
{
	return m_MatchPairing ;
}

void search_match::set_memory_id( const int id )
{
	m_memory_id = id ;
}

const int search_match::get_memory_id() const
{
	return m_memory_id ;
}
bool search_match_compare::operator()( const search_match_ptr &lhs, const search_match_ptr &rhs ) const
{
	const record_pointer lrec = lhs->get_record() ;
	const record_pointer rrec = rhs->get_record() ;

	// refcount
	if (lrec->get_refcount() > rrec->get_refcount()) 
	{
		return true ;
	}
	if (lrec->get_refcount() < rrec->get_refcount()) 
	{
		return false ;
	}

	// reliability
	if (lrec->get_reliability() > rrec->get_reliability()) 
	{
		return true ;
	}
	if (lrec->get_reliability() < rrec->get_reliability()) 
	{
		return false ;
	}

	// validated
	if (lrec->is_validated() && ! rrec->is_validated()) 
	{
		return true ;
	}
	if (! lrec->is_validated() && rrec->is_validated()) 
	{
		return false ;
	}

	// source
	if (lrec->get_source_cmp().size() > rrec->get_source_cmp().size()) 
	{
		return true ;
	}
	if (lrec->get_source_cmp().size() < rrec->get_source_cmp().size()) 
	{
		return false ;
	}

	// trans
	if (lrec->get_trans_cmp().size() > rrec->get_trans_cmp().size()) 
	{
		return true ;
	}
	if (lrec->get_trans_cmp().size() < rrec->get_trans_cmp().size()) 
	{
		return false ;
	}

	// id
	if (lrec->get_id() > rrec->get_id()) 
	{
		return true ;
	}
	if (lrec->get_id() < rrec->get_id()) 
	{
		return false ;
	}

	// created
	return(lrec->get_created() < rrec->get_created()) ;
}

bool translation_match_compare::operator()( const search_match_ptr &lhs, const search_match_ptr &rhs ) const
{
	// score
	if ( lhs->get_score() > rhs->get_score() )
		return true ;
	if ( lhs->get_score() < rhs->get_score() )
		return false ;

	// placement
	if ( ! lhs->HasPlacement() && rhs->HasPlacement() )
	{
		return true ;
	}

	record_pointer lrec = lhs->get_record() ;
	record_pointer rrec = rhs->get_record() ;

	// reliability
	if ( lrec->get_reliability() > rrec->get_reliability() ) 
		return true ;
	if ( lrec->get_reliability() < rrec->get_reliability() ) 
		return false ;

	// validated
	if ( lrec->is_validated() && ! rrec->is_validated() )
		return true ;
	if ( ! lrec->is_validated() && rrec->is_validated() )
		return false ;

	// refcount
	if ( lrec->get_refcount() > rrec->get_refcount() ) 
		return true ;
	if ( lrec->get_refcount() < rrec->get_refcount() ) 
		return false ;

	// date
	if ( lrec->get_modified() > rrec->get_modified() ) 
		return true ;
	if ( lrec->get_modified() < rrec->get_modified() ) 
		return false ;

	return false ;
}
}
