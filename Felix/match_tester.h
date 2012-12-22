/*!
	@file match_tester.h
	@brief memory searching classes
	@date 2005/06/23
	Time: 6:31:55
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "query.h"					// query structs
#include "segment.h"				// CmpMaker

namespace mem_engine
{

/** Base class for matching engines */
class match_tester
{
protected:
	search_match_ptr	m_search_match ;

public:
	void set_search_match(search_match_ptr match)
	{
		m_search_match = match ;
	}
	match_tester(void);
	~match_tester(void);
};

/** The glossary entry matching engine */
class gloss_match_tester : public match_tester
{
	wstring				m_source_pattern ;

public:
	gloss_match_tester(const wstring query) : 
			m_source_pattern(query)
		{
		}
		search_match_ptr get_search_match(  ) 
		{ 
			return m_search_match ; 
		} 
		bool test_source( record_pointer rec ) ;

} ;

/** User search matching engine */
class search_match_tester : public match_tester
{
	VISIBLE_TO_TESTS ;

	CmpMaker m_cmp ;
	size_t	 m_min_reliability ;
	bool	 m_only_validated ;

	wstring				m_source_pattern ;
	wstring				m_trans_pattern ;
	wstring				m_context_pattern ;

public:
	search_match_tester( const search_query_params &params ) ;
	search_match_ptr get_search_match( record_pointer rec ); 
	bool test_source_only( record_pointer rec ) ;
	bool test_source( record_pointer rec ) ;
	bool test_trans( record_pointer rec ) ;
	bool test_context( record_pointer rec ) ;
	bool test_validity( const record_pointer rec ) const ;
	bool test_reliability( const record_pointer rec ) const ;
	bool is_match( record_pointer rec ) ;

private:
	VISIBLE_TO_TESTS ;

	void normalize( wstring &to_search ) ;
	wstring mark_up_string( const wstring to_search, size_t start_pos, size_t len ) ;
} ;

/** Glossary matching engine with regular expressions */
class search_match_tester_regex : public match_tester
{
	VISIBLE_TO_TESTS ;

	search_query_params m_params ;

	boost::wregex m_source_regex ;
	boost::wregex m_trans_regex ;
	boost::wregex m_context_regex ;

public:
	search_match_tester_regex( const search_query_params &params ) ;
	search_match_ptr get_search_match( record_pointer rec ); 
	bool test_source( record_pointer rec ) ;
	bool test_trans( record_pointer rec ) ;
	bool test_context( record_pointer rec ) ;
	bool test_validity( const record_pointer rec ) const ;
	bool test_reliability( const record_pointer rec ) const ;
	bool is_match( record_pointer rec ) ;
} ;

}