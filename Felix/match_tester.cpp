#include "StdAfx.h"
#include ".\match_tester.h"

namespace memory_engine
{

	match_tester::match_tester(void) : m_search_match(new search_match)
	{
	}

	match_tester::~match_tester(void)
	{
	}

	// ==============================
	// class search_match_tester
	// ==============================

	// Function name	: search_match_tester::search_match_tester
	// Description	    : 
	// Return type		: 
	// Argument         : search_query_params &params
	search_match_tester::search_match_tester( const search_query_params &params ) : m_params( params )
	{
		m_source_pattern	= params.m_source ; 
		m_trans_pattern		= params.m_trans ;
		m_context_pattern	= params.m_context ;

		if ( m_params.m_ignore_case )
		{
			str::make_lower( m_source_pattern ) ;
			str::make_lower( m_trans_pattern ) ;
			str::make_lower( m_context_pattern ) ;
		}
		if ( m_params.m_ignore_hira_kata )
		{
			str::normalize_hiragana_to_katakana( m_source_pattern ) ;
			str::normalize_hiragana_to_katakana( m_trans_pattern ) ;
			str::normalize_hiragana_to_katakana( m_context_pattern ) ;
		}
		if ( m_params.m_ignore_width )
		{
			str::normalize_width( m_source_pattern ) ;
			str::normalize_width( m_trans_pattern ) ;
			str::normalize_width( m_context_pattern ) ;
		}
	}


	// Function name	: gloss_match_tester::test_source
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool gloss_match_tester::test_source( record_pointer rec )
	{
		wstring to_find = rec->get_source_cmp() ;

		if ( wstring::npos == m_source_pattern.find( to_find ) )
			return false ;

		m_search_match = search_match_ptr(new search_match) ;
		m_search_match->set_record( rec ) ;
		m_search_match->set_values_to_record() ;

		return true ;
	}


	// Function name	: search_match_tester::test_source
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester::test_source( record_pointer rec )
	{
		m_search_match->get_markup()->SetTrans( wstring() ) ;
		m_search_match->get_markup()->SetContext( wstring() ) ;

		if ( m_source_pattern.empty() )
		{
			m_search_match->get_markup()->SetSource( wstring() ) ;
			return true ;
		}
		wstring to_search = rec->get_source_plain() ;
		check_lower( to_search ) ;
		size_t pos = to_search.find( m_source_pattern ) ;
		if ( pos == wstring::npos )
		{
			m_search_match->get_markup()->SetSource( wstring() ) ;
			return false ;
		}
		m_search_match->get_markup()->SetSource( mark_up_string( rec->get_source_plain(), pos, m_source_pattern.size()) ) ;
		return true ;
	}

	// Function name	: search_match_tester::test_trans
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester::test_trans( record_pointer rec )
	{
		if ( ! test_source( rec ) ) return false ;

		if ( m_trans_pattern.empty() )
		{
			return true ;
		}

		wstring to_search = rec->get_trans_plain() ;
		check_lower( to_search ) ;

		size_t pos = to_search.find( m_trans_pattern ) ;
		if ( pos == wstring::npos )
			return false ;

		m_search_match->get_markup()->SetTrans( mark_up_string( rec->get_trans_plain(), pos, m_trans_pattern.size()) );
		return true ;
	}

	// Function name	: search_match_tester::test_context
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester::test_context( record_pointer rec )
	{
		if ( ! test_trans( rec ) ) return false ;

		if ( m_context_pattern.empty() )
		{
			return true ;
		}

		wstring to_search = rec->get_context_plain() ;
		check_lower( to_search ) ;

		size_t pos = to_search.find( m_context_pattern ) ;
		if ( pos == wstring::npos )
			return false ;

		m_search_match->get_markup()->SetContext( mark_up_string( rec->get_context_plain(), pos, m_context_pattern.size()) ) ;
		return true ;

	}

	// Function name	: search_match_tester::test_validity
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester::test_validity( const record_pointer rec ) const
	{
		return rec->is_validated() ;
	}

	// Function name	: search_match_tester::test_reliability
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester::test_reliability( const record_pointer rec ) const
	{
		return ( m_params.m_min_reliability <= rec->get_reliability() ) ;
	}

	// Function name	: search_match_tester::is_match
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester::is_match( record_pointer rec )
	{
		m_search_match = search_match_ptr(new search_match) ;
		if ( m_params.m_only_validated )
		{
			if ( ! test_validity( rec ) )
				return false ;
		}
		if ( ! test_reliability( rec ) )
			return false ;

		return test_context( rec ) ;
	}
	// private:

	// Function name	: search_match_tester::check_lower
	// Description	    : 
	// Return type		: void 
	// Argument         : wstring &to_search
	void search_match_tester::check_lower( wstring &to_search )
	{
		if ( m_params.m_ignore_case )
		{
			str::make_lower( to_search ) ;
		}
		if ( m_params.m_ignore_hira_kata )
		{
			str::normalize_hiragana_to_katakana( to_search ) ;
		}
		if ( m_params.m_ignore_width )
		{
			str::normalize_width( to_search ) ;
		}
	}

	// Function name	: search_match_tester::mark_up_string
	// Description	    : 
	// Return type		: wstring 
	// Argument         :  const wstring &to_search
	// Argument         : size_t start_pos
	// Argument         : size_t len
	wstring search_match_tester::mark_up_string( const wstring &to_search, size_t start_pos, size_t len )
	{
		size_t len_needed = to_search.size() + 50 ;

		wstring out ;

		out.reserve( len_needed ) ;

		out += str::left( to_search, start_pos ) ;
		out += L"<span style=\"background:#FFFF99\">" ;
		out += to_search.substr( start_pos, len ) ;
		out += L"</span>" ;
		out += to_search.substr( start_pos + len ) ;

		return out ;
	}

	search_match_ptr search_match_tester::get_search_match( record_pointer rec ) 
	{
		m_search_match->set_record(rec);

		markup_ptr markup = m_search_match->get_markup() ;

		if ( markup->GetSource().empty() )
			markup->SetSource( rec->get_source_rich() );

		if ( markup->GetTrans().empty() )
			markup->SetTrans( rec->get_trans_rich() );

		if ( markup->GetContext().empty() )
			markup->SetContext( rec->get_context_rich() );

		return m_search_match ;
	}


	// ==============================
	// class search_match_tester_regex
	// ==============================

	//! CTOR.
	search_match_tester_regex::search_match_tester_regex( const search_query_params &params ) : 
	m_params( params )
	{
		// set patterns
		m_source_regex.set_pattern ( params.m_source  ) ;
		m_trans_regex.set_pattern ( params.m_trans ) ;
		m_context_regex.set_pattern ( params.m_context  ) ;
		// set ignore case
		m_source_regex.set_ignore_case( params.m_ignore_case ) ;
		m_trans_regex.set_ignore_case( params.m_ignore_case ) ;
		m_context_regex.set_ignore_case( params.m_ignore_case ) ;
	}

	// Function name	: search_match_tester_regex::test_source
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester_regex::test_source( record_pointer rec )
	{
		return m_source_regex.test( rec->get_source_plain()  ) ;
	}

	// Function name	: search_match_tester_regex::test_trans
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester_regex::test_trans( record_pointer rec )
	{
		if ( ! test_source( rec ) ) return false ;

		return m_trans_regex.test( rec->get_trans_plain()  ) ;
	}

	// Function name	: search_match_tester_regex::test_context
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester_regex::test_context( record_pointer rec )
	{
		if ( ! test_trans( rec ) ) return false ;

		return m_context_regex.test( rec->get_context_plain()  ) ;
	}

	// Function name	: search_match_tester_regex::test_validity
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester_regex::test_validity( const record_pointer rec ) const
	{
		return rec->is_validated() ;
	}

	// Function name	: search_match_tester_regex::test_reliability
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester_regex::test_reliability( const record_pointer rec ) const
	{
		return ( m_params.m_min_reliability >= rec->get_reliability() ) ;
	}

	// Function name	: search_match_tester_regex::is_match
	// Description	    : 
	// Return type		: bool 
	// Argument         : record_pointer rec
	bool search_match_tester_regex::is_match( record_pointer rec )
	{
		m_search_match = search_match_ptr(new search_match) ;
		if ( m_params.m_only_validated )
		{
			if ( ! test_validity( rec ) )
				return false ;
		}
		if ( ! test_reliability( rec ) )
			return false ;

		return test_context( rec ) ;
	}

	search_match_ptr search_match_tester_regex::get_search_match( record_pointer rec ) 
	{
		m_search_match->set_record(record_pointer(rec->clone()))	;

		markup_ptr markup = m_search_match->get_markup() ;

		markup->SetSource( rec->get_source_rich() ) ;
		markup->SetTrans( rec->get_trans_rich() ) ;
		markup->SetContext( rec->get_context_rich() ) ;

		return m_search_match ;
	}
}
