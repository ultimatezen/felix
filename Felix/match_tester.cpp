#include "StdAfx.h"
#include ".\match_tester.h"

namespace mem_engine
{

	match_tester::match_tester(void) : m_search_match(new search_match)
	{
	}

	match_tester::~match_tester(void)
	{
	}


	// ==============================
	// class gloss_match_tester
	// ==============================

	// gloss_match_tester::test_source
	bool gloss_match_tester::test_source( record_pointer rec )
	{
		wstring to_find = rec->get_source_cmp() ;

		if ( wstring::npos == m_source_pattern.find( to_find ) )
			return false ;

		m_search_match->set_record( rec ) ;
		m_search_match->set_values_to_record() ;

		return true ;
	}


	// ==============================
	// class search_match_tester
	// ==============================

	// search_match_tester::search_match_tester
	search_match_tester::search_match_tester( const search_query_params &params ) : 
		m_cmp( params.m_ignore_width, params.m_ignore_hira_kata, params.m_ignore_case ),
		m_only_validated(params.m_only_validated),
		m_min_reliability(params.m_min_reliability)
	{
		m_source_pattern	= params.m_source ; 
		m_trans_pattern		= params.m_trans ;
		m_context_pattern	= params.m_context ;

		normalize( m_source_pattern ) ;
		normalize( m_trans_pattern ) ;
		normalize( m_context_pattern ) ;
	}


	// search_match_tester::test_source
	bool search_match_tester::test_source( record_pointer rec )
	{
		if ( m_source_pattern.empty() )
		{
			return true ;
		}
		wstring to_search = rec->get_source_plain() ;
		normalize( to_search ) ;

		size_t pos = to_search.find( m_source_pattern ) ;
		if ( pos == wstring::npos )
		{
			return false ;
		}

		m_search_match->get_markup()->SetSource( mark_up_string( rec->get_source_plain(), pos, m_source_pattern.size()) ) ;
		return true ;
	}

	// search_match_tester::test_trans
	bool search_match_tester::test_trans( record_pointer rec )
	{
		if ( m_trans_pattern.empty() )
		{
			return true ;
		}

		wstring to_search = rec->get_trans_plain() ;
		normalize( to_search ) ;

		size_t pos = to_search.find( m_trans_pattern ) ;
		if ( pos == wstring::npos )
		{
			return false ;
		}

		m_search_match->get_markup()->SetTrans( mark_up_string( rec->get_trans_plain(), pos, m_trans_pattern.size()) );
		return true ;
	}

	// search_match_tester::test_context
	bool search_match_tester::test_context( record_pointer rec )
	{
		if ( m_context_pattern.empty() )
		{
			return true ;
		}

		wstring to_search = rec->get_context_plain() ;
		normalize( to_search ) ;

		size_t pos = to_search.find( m_context_pattern ) ;
		if ( pos == wstring::npos )
		{
			return false ;
		}

		m_search_match->get_markup()->SetContext( mark_up_string( rec->get_context_plain(), pos, m_context_pattern.size()) ) ;
		return true ;
	}

	// Function name	: search_match_tester::test_validity
	bool search_match_tester::test_validity( const record_pointer rec ) const
	{
		return rec->is_validated() ;
	}

	// Function name	: search_match_tester::test_reliability
	bool search_match_tester::test_reliability( const record_pointer rec ) const
	{
		return ( m_min_reliability <= rec->get_reliability() ) ;
	}

	// Function name	: search_match_tester::is_match
	bool search_match_tester::is_match( record_pointer rec )
	{

		if ( m_only_validated )
		{
			if ( ! test_validity( rec ) )
			{
				return false ;
			}
		}
		if ( ! test_reliability( rec ) )
		{
			return false ;
		}

		markup_ptr markup = m_search_match->get_markup() ;
		markup->SetTrans( wstring() ) ;
		markup->SetContext( wstring() ) ;
		markup->SetSource( wstring() ) ;

		if (! test_source(rec))
		{
			return false ;
		}
		if (! test_trans(rec))
		{
			return false ;
		}
		if (! test_context(rec))
		{
			return false ;
		}
		return true ;
	}

	// Function name	: search_match_tester::normalize
	void search_match_tester::normalize( wstring &to_search )
	{
		m_cmp.normalize(to_search) ;
	}

	// Function name	: search_match_tester::mark_up_string
	wstring search_match_tester::mark_up_string( const wstring to_search, size_t start_pos, size_t len )
	{
		size_t len_needed = to_search.size() + 50 ;

		wstring out ;

		out.reserve( len_needed ) ;

		out += str::left( to_search, start_pos ) ;
		out += L"<span class=\"concordance_match\">" ;
		out += to_search.substr( start_pos, len ) ;
		out += L"</span>" ;
		out += to_search.substr( start_pos + len ) ;

		return out ;
	}


	// get_search_match
	search_match_ptr search_match_tester::get_search_match( record_pointer rec ) 
	{
		m_search_match->set_record(rec);

		markup_ptr markup = m_search_match->get_markup() ;

		if ( markup->GetSource().empty() )
		{
			markup->SetSource( rec->get_source_rich() );
		}

		if ( markup->GetTrans().empty() )
		{
			markup->SetTrans( rec->get_trans_rich() );
		}

		if ( markup->GetContext().empty() )
		{
			markup->SetContext( rec->get_context_rich() );
		}

		return m_search_match ;
	}


	// ==============================
	// class search_match_tester_regex
	// ==============================

	//! CTOR.
	search_match_tester_regex::search_match_tester_regex( const search_query_params &params ) : 
		m_params( params )
		, m_source_regex( L"(" + params.m_source + L")", params.m_ignore_case ? boost::regex::extended|boost::regex::icase : boost::regex::extended )
		, m_trans_regex( L"(" + params.m_trans + L")", params.m_ignore_case ? boost::regex::extended|boost::regex::icase : boost::regex::extended )
		, m_context_regex( L"(" + params.m_context + L")", params.m_ignore_case ? boost::regex::extended|boost::regex::icase : boost::regex::extended )
	{
	}

	// Function name	: search_match_tester_regex::test_source
	bool search_match_tester_regex::test_source( record_pointer rec )
	{
		if (m_params.m_source.empty())
		{
			return true ;
		}
		if(! boost::regex_search(rec->get_source_plain(), m_source_regex))
		{
			return false ;
		}
		wstring source = rec->get_source_plain() ;
		m_search_match->get_markup()->SetSource(boost::regex_replace(source, 
																	 m_source_regex, 
																	 L"<span class=\"concordance_match\">$1</span>")) ;
		return true ;
	}

	// Function name	: search_match_tester_regex::test_trans
	bool search_match_tester_regex::test_trans( record_pointer rec )
	{
		if (m_params.m_trans.empty())
		{
			return true ;
		}
		if(! boost::regex_search(rec->get_trans_plain(), m_trans_regex) )
		{
			return false ;
		}
		m_search_match->get_markup()->SetTrans(boost::regex_replace(rec->get_trans_plain(), 
			m_trans_regex, 
			L"<span class=\"concordance_match\">$1</span>")) ;
		return true ;
	}

	// Function name	: search_match_tester_regex::test_context
	bool search_match_tester_regex::test_context( record_pointer rec )
	{
		if (m_params.m_context.empty())
		{
			return true ;
		}
		if (!boost::regex_search(rec->get_context_plain(), m_context_regex))
		{
			return false ;
		}
		m_search_match->get_markup()->SetContext(boost::regex_replace(rec->get_context_plain(), 
			m_context_regex, 
			L"<span class=\"concordance_match\">$1</span>")) ;
		return true ;
		 
	}

	// Function name	: search_match_tester_regex::test_validity
	bool search_match_tester_regex::test_validity( const record_pointer rec ) const
	{
		return rec->is_validated() ;
	}

	// Function name	: search_match_tester_regex::test_reliability
	bool search_match_tester_regex::test_reliability( const record_pointer rec ) const
	{
		return ( m_params.m_min_reliability >= rec->get_reliability() ) ;
	}

	// Function name	: search_match_tester_regex::is_match
	bool search_match_tester_regex::is_match( record_pointer rec )
	{
		if ( m_params.m_only_validated )
		{
			if ( ! test_validity( rec ) )
				return false ;
		}
		if ( ! test_reliability( rec ) )
		{
			return false ;
		}
		if (! test_source(rec))
		{
			return false ;
		}
		if (! test_trans(rec))
		{
			return false ;
		}
		return test_context( rec ) ;
	}

	// get_search_match
	search_match_ptr search_match_tester_regex::get_search_match( record_pointer rec ) 
	{
		m_search_match->set_record(record_pointer(rec->clone()))	;

		markup_ptr markup = m_search_match->get_markup() ;

		if ( markup->GetSource().empty() )
		{
			markup->SetSource( rec->get_source_rich() );
		}
		if ( markup->GetTrans().empty() )
		{
			markup->SetTrans( rec->get_trans_rich() );
		}
		if ( markup->GetContext().empty() )
		{
			markup->SetContext( rec->get_context_rich() );
		}

		return m_search_match ;
	}
}
