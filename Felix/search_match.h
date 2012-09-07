#pragma once

#include "record.h"				// translation record
#include "match_score.h"		// match_score
#include "markup_strings.h"	// markup_ptr
#include <set>
#include "MatchStringPairing.h"

namespace mem_engine
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

	placement::match_string_pairing	m_match_pairing ;

	bool				m_has_placement ;

	std::vector<wstring> m_qc_messages ;
public:
	search_match( );
	search_match(record_pointer record) : 
		m_memory_id(0)
		, m_has_placement(false)
		, m_record(record)
		, m_markup(new markup_strings)
	{
	}

	void set_qc_messages(const std::vector<wstring> &qc_messages)
	{
		std::vector<wstring> temp(qc_messages) ;
		m_qc_messages.swap(temp) ;
	}
	void get_qc_messages(std::vector<wstring> &qc_messages)
	{
		std::vector<wstring> temp(m_qc_messages) ;
		qc_messages.swap(temp) ;
	}

	search_match( const search_match &cpy );
	search_match& operator= ( const search_match &cpy );

	void set_placement_on();
	bool has_placement();

	placement::match_string_pairing& match_pairing();

	void set_memory_id( const int id );
	const int get_memory_id() const;
	void set_memory_location( const wstring loc ) ;
	const wstring get_memory_location() const ;

	record_pointer get_record() ;
	void set_record( record_pointer rec ) ;

	markup_ptr get_markup() ;

	const bool has_formatting_penalty() const ;
	const double get_formatting_penalty() const ;
	const double get_base_score() const ;
	const double get_score() const ;

	void set_base_score(const double score) ;
	void set_formatting_penalty(const double formatting_pentaly)  ;

	//
	void set_values_to_record() ;

private:
	void internal_copy( const search_match &cpy );
};

typedef boost::shared_ptr< search_match > search_match_ptr ;

// ================
// struct search_match_compare 
// comparison function for search matches
// ================
struct search_match_compare : std::binary_function< search_match_ptr, search_match_ptr, bool >
{
	bool operator()( const search_match_ptr &lhs, const search_match_ptr &rhs ) const;
} ;

/** 
@struct translation_match_compare 
@brief comparison function for translation matches.
*/
struct translation_match_compare : std::binary_function< search_match_ptr, search_match_ptr, bool >
{
	bool operator() ( const search_match_ptr &lhs, const search_match_ptr &rhs  ) const ;
} ;

// ================
// multisets for search_match structs
// ================

// typedef std::multiset < search_match_ptr, search_match_compare > search_match_container ;
typedef std::multiset < search_match_ptr, search_match_compare > search_match_container ;
// typedef std::multiset < search_match_ptr, translation_match_compare > translation_match_multiset ;
typedef std::multiset < search_match_ptr, translation_match_compare > trans_match_container ;

}