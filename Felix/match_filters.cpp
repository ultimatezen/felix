#include "stdafx.h"
#include "match_filters.h"
#include <boost/regex.hpp>
#include "logging.h"
#include "replacer.h"
#include "matcher.h"

namespace mem_search
{

	using namespace replacer ;



	void search_runner::add_term( const wstring term )
	{
		m_terms.push_back(term) ;
	}

	void search_runner::remove_term( const size_t i )
	{
		std::vector<wstring>::iterator pos = m_terms.begin() ;
		std::advance(pos, i) ;
		m_terms.erase(pos) ;
	}

	void search_runner::get_matches( memory_pointer mem, search_match_container& matches )
	{
		set_matchers();

		if (is_wanted_mem(mem))
		{
			foreach(record_pointer rec, mem->get_records())
			{
				if (is_match(rec))
				{
					search_match_ptr match(new search_match) ;
					match->set_record(rec) ;
					match->get_markup()->SetTrans( rec->get_trans_rich() ) ;
					match->set_memory_location(wstring(mem->get_location())) ;
					match->set_memory_id(mem->get_id()) ;
					matches.insert(match) ;
				}
			}
		}
		
	}

	bool search_runner::is_wanted_mem( memory_pointer mem )
	{
		if (! m_required_name.empty())
		{
			wstring haystack = get_fname_from_loc(static_cast<const wchar_t *>(mem->get_location())) ;
			return boost::icontains(haystack, m_required_name) ;
		}
		return true ;
	}
	const std::vector<wstring> & search_runner::get_terms()
	{
		return m_terms ;
	}

	void search_runner::clear_terms()
	{
		m_terms.clear() ;
	}

	bool search_runner::is_match( const record_pointer rec ) const
	{

		foreach(matcher_ptr matcher, m_matchers)
		{
			if (! matcher->is_match(rec))
			{
				return false ;
			}
		}
		return true ;
	}



	void search_runner::set_matchers()
	{
		m_required_name.clear() ;
		m_matchers.clear() ;

		const wstring MEM_TAG = L"mem:" ;
		const wstring GLOSS_TAG = L"gloss:" ;
		foreach(wstring term, m_terms)
		{
			if (boost::starts_with(term, MEM_TAG) || boost::starts_with(term, GLOSS_TAG))
			{
				size_t pos = term.find(L':') + 1 ;
				m_required_name = term.substr(pos) ;
			}
			else
			{
				m_matchers.push_back(get_matcher(term)) ;
			}
		}
	}

}