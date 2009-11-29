#include "stdafx.h"
#include "match_filters.h"
#include <boost/regex.hpp>

namespace memory_searcher
{

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

	void search_runner::get_matches( memory_pointer mem, search_match_multiset& matches )
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

	std::vector<wstring> & search_runner::get_terms()
	{
		return m_terms ;
	}

	void search_runner::clear_terms()
	{
		m_terms.clear() ;
	}

	bool search_runner::is_match( record_pointer rec )
	{
		foreach(wstring term, m_terms)
		{
			if (! term_matches(rec, term))
			{
				return false ;
			}
		}
		return true ;
	}

	/*
	Allows tags.
	*/
	bool search_runner::term_matches( record_pointer rec, const wstring &term )
	{
		wstring tag ;

		tag = L"source:" ; // Search in source field
		if (str::starts_with(term, tag))
		{
			wstring query = str::make_lower_out(term.substr(tag.size())) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			return str::make_lower_out(rec->get_source_cmp()).find(query) != wstring::npos ;
		}
		tag = L"trans:" ; // Search in translation field
		if (str::starts_with(term, tag))
		{
			wstring query = str::make_lower_out(term.substr(tag.size())) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			return str::make_lower_out(rec->get_trans_cmp()).find(query) != wstring::npos ;
		}
		tag = L"context:" ; // Search in context field
		if (str::starts_with(term, tag))
		{
			wstring query = str::make_lower_out(term.substr(tag.size())) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			return str::make_lower_out(rec->get_context_cmp()).find(query) != wstring::npos ;
		}
		// Created
		tag = L"created-by:" ; // Search in creator field
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			return rec->get_creator().find(query) != wstring::npos ;
		}
		tag = L"created:" ; // Search in modifier field
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			misc_wrappers::date thedate ;
			mod_date(thedate, query) ;
			misc_wrappers::date created_date = rec->get_created() ;
			return thedate.wYear == created_date.wYear
				&& thedate.wMonth == created_date.wMonth
				&& thedate.wDay == created_date.wDay ;
		}
		tag = L"created-before:" ; // Search for records created before date (YYYY-MM-DD format)
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			misc_wrappers::date thedate ;
			mod_date(thedate, query) ;
			return rec->get_created() < thedate ;
		}
		tag = L"created-after:" ; // Search for records created after date (YYYY-MM-DD format)
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			misc_wrappers::date thedate ;
			mod_date(thedate, query) ;
			return rec->get_created() > thedate ;
		}
		// Modified
		tag = L"modified-by:" ; // Search in modifier field
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			return rec->get_modified_by().find(query) != wstring::npos ;
		}
		tag = L"modified:" ; // Search in modifier field
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			misc_wrappers::date thedate ;
			mod_date(thedate, query) ;
			misc_wrappers::date mod_date = rec->get_modified() ;
			return thedate.wYear == mod_date.wYear
				&& thedate.wMonth == mod_date.wMonth
				&& thedate.wDay == mod_date.wDay ;
		}
		tag = L"modified-before:" ; // Search for records modified before date (YYYY-MM-DD format)
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			if (str::trim(query).empty())
			{
				return true ;
			}
			misc_wrappers::date thedate ;
			mod_date(thedate, query) ;
			return rec->get_modified() < thedate ;
		}
		tag = L"modified-after:" ; // Search for records modified after date (YYYY-MM-DD format)
		if (str::starts_with(term, tag))
		{
			wstring query = term.substr(tag.size()) ;
			misc_wrappers::date thedate ;
			mod_date(thedate, query) ;
			return rec->get_modified() > thedate ;
		}
		// Reliability
		tag = L"reliability:" ; // Search for records with the specified reliability
		if (str::starts_with(term, tag))
		{
			wstring rhs = term.substr(tag.size()) ;
			if (str::trim(rhs).empty())
			{
				return true ;
			}
			size_t query = boost::lexical_cast<size_t>(rhs) ;
			return rec->get_reliability() == query ;
		}
		tag = L"reliability-gt:" ; // Search for records with greater than the specified reliability
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_reliability() > query ;
		}
		tag = L"reliability-gte:" ; // Search for records with at least the specified reliability
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_reliability() >= query ;
		}
		tag = L"reliability-lt:" ; // Search for records with less than the specified reliability
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_reliability() < query ;
		}
		tag = L"reliability-lte:" ; // Search for records with no more than than the specified reliability
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_reliability() <= query ;
		}
		// Validated
		tag = L"validated:" ; // Search for records that are validated ("true") or not validated ("false")
		if (str::starts_with(term, tag))
		{
			wstring rhs = term.substr(tag.size()) ;
			if (str::trim(rhs).empty())
			{
				return true ;
			}
			bool query = string2bool(rhs) ;
			return rec->is_validated() == query ;
		}
		// Reference Count
		tag = L"refcount:" ; // Search for records with the specified reference count
		if (str::starts_with(term, tag))
		{
			wstring rhs = term.substr(tag.size()) ;
			if (str::trim(rhs).empty())
			{
				return true ;
			}
			size_t query = boost::lexical_cast<size_t>(rhs) ;
			return rec->get_refcount() == query ;
		}
		tag = L"refcount-gt:" ; // Search for records with greater than the specified reference count
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_refcount() > query ;
		}
		tag = L"refcount-gte:" ; // Search for records with at least the specified reference count
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_refcount() >= query ;
		}
		tag = L"refcount-lt:" ; // Search for records with less than the specified reference count
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_refcount() < query ;
		}
		tag = L"refcount-lte:" ; // Search for records with no more than than the specified reference count
		if (str::starts_with(term, tag))
		{
			size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
			return rec->get_refcount() >= query ;
		}

		// search in source, trans, and context fields
		wstring query = str::make_lower_out(term) ;
		if (str::make_lower_out(rec->get_source_cmp()).find(query) != wstring::npos)
		{
			return true ;
		}
		if (str::make_lower_out(rec->get_trans_cmp()).find(query) != wstring::npos)
		{
			return true ;
		}
		if (str::make_lower_out(rec->get_context_cmp()).find(query) != wstring::npos)
		{
			return true ;
		}
		return false ;
	}

	void search_runner::mod_date( misc_wrappers::date &thedate, wstring datestring )
	{
		textstream_reader<wchar_t> reader ;
		reader.set_buffer(datestring.c_str()) ;
		std::vector<wstring> bits ;
		reader.split(bits, L"-/") ;
		if (bits.size() >= 1)
		{
			thedate.set_year(bits[0]) ;
		}
		if (bits.size() >= 2)
		{
			thedate.set_month(bits[1]) ;
		}
		if (bits.size() >= 3)
		{
			thedate.set_day(bits[2]) ;
		}
	}
}