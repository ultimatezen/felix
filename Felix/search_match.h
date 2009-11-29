#pragma once

#include "record.h"				// translation record
#include "match_score.h"		// match_score
#include "markup_strings.h"	// markup_ptr
#include <set>
#include "MatchStringPairing.h"

namespace memory_engine
{

/**
@class search_match
@brief Wrapper for a search match.
*/
class search_match
{
	record_pointer		m_record ;

	markup_ptr			m_markup ;

	match_score			m_score ;

	wstring				m_memory_location ;
	int					m_memory_id ;

	CMatchStringPairing	m_MatchPairing ;

	bool				m_HasPlacement ;
public:
	search_match( ) : 
			m_memory_id(0),
			m_HasPlacement(false)
		{
			m_record = record_pointer(new record_local()) ;
		}

	search_match( const search_match &cpy )
	{
		internal_copy( cpy ) ;
	}
	search_match& operator= ( const search_match &cpy )
	{
		internal_copy( cpy ) ;
		return *this ;
	}

	void Placement(){ m_HasPlacement = true ; }
	bool HasPlacement() { return m_HasPlacement ; }

	CMatchStringPairing& MatchPairing() { return m_MatchPairing ; }

	void set_memory_id( const int id ) { m_memory_id = id ; }
	const int get_memory_id() const { return m_memory_id ; }
	void set_memory_location( const wstring &loc ) ;
	const wstring &get_memory_location() const ;

	record_pointer get_record() ;
	void set_record( record_pointer rec ) ;

	markup_ptr get_markup() ;

	const bool has_formatting_penalty() const ;
	const double get_formatting_penalty() const ;
	const double get_base_score() const ;
	const double get_score() const ;

	void set_base_score(double score) ;
	void set_formatting_penalty(double formatting_pentaly)  ;

	//
	void set_values_to_record() ;

private:
	void internal_copy( const search_match &cpy )
	{
		m_record = record_pointer( cpy.m_record->clone() ) ;
		*m_markup = *(cpy.m_markup) ;

		m_score = cpy.m_score  ;

		m_memory_location = cpy.m_memory_location ;
		m_memory_id = cpy.m_memory_id ;
	}
};

typedef boost::shared_ptr< search_match > search_match_ptr ;

// ================
// struct search_match_compare 
// comparison function for search matches
// ================
struct search_match_compare : std::binary_function< search_match_ptr, search_match_ptr, bool >
{
	bool operator()( const search_match_ptr &lhs, const search_match_ptr &rhs )
	{
		record_pointer lrec = lhs->get_record() ;
		record_pointer rrec = rhs->get_record() ;
		if (lrec->get_refcount() > rrec->get_refcount()) 
		{
			return true ;
		}
		if (lrec->get_refcount() < rrec->get_refcount()) 
		{
			return false ;
		}

		if (lrec->get_reliability() > rrec->get_reliability()) 
		{
			return true ;
		}
		if (lrec->get_reliability() < rrec->get_reliability()) 
		{
			return false ;
		}

		if (lrec->is_validated() && ! rrec->is_validated()) 
		{
			return true ;
		}
		if (! lrec->is_validated() && rrec->is_validated()) 
		{
			return false ;
		}

		if (lrec->get_source_plain().size() > rrec->get_source_plain().size()) 
		{
			return true ;
		}
		if (lrec->get_source_plain().size() < rrec->get_source_plain().size()) 
		{
			return false ;
		}

		if (lrec->get_trans_plain().size() > rrec->get_trans_plain().size()) 
		{
			return true ;
		}
		if (lrec->get_trans_plain().size() < rrec->get_trans_plain().size()) 
		{
			return false ;
		}

		if (lrec->get_id() > rrec->get_id()) 
		{
			return true ;
		}
		if (lrec->get_id() < rrec->get_id()) 
		{
			return false ;
		}

		return(lrec->get_created() < rrec->get_created()) ;
	}
} ;

/** 
@struct translation_match_compare 
@brief comparison function for translation matches.
*/
struct translation_match_compare : std::binary_function< search_match_ptr, search_match_ptr, bool >
{
	bool operator() ( const search_match_ptr &lhs, const search_match_ptr &rhs  )
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
} ;

// ================
// multisets for search_match structs
// ================

// typedef std::multiset < search_match_ptr, search_match_compare > search_match_multiset ;
typedef std::multiset < search_match_ptr, search_match_compare > search_match_multiset ;
// typedef std::multiset < search_match_ptr, translation_match_compare > translation_match_multiset ;
typedef std::multiset < search_match_ptr, translation_match_compare > TransMatchContainer ;

}