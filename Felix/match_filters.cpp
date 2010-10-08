#include "stdafx.h"
#include "match_filters.h"
#include <boost/regex.hpp>
#include "logging.h"
#include "replacer.h"

namespace mem_search
{

	void mod_date( misc_wrappers::date &query_date, const wstring datestring )
	{
		textstream_reader<wchar_t> reader ;
		reader.set_buffer(datestring.c_str()) ;
		std::vector<wstring> bits ;
		reader.split(bits, L"-/") ;
		if (bits.size() >= 1)
		{
			query_date.set_year(boost::trim_copy(bits[0])) ;
		}
		if (bits.size() >= 2)
		{
			query_date.set_month(boost::trim_copy(bits[1])) ;
		}
		if (bits.size() >= 3)
		{
			query_date.set_day(boost::trim_copy(bits[2])) ;
		}
	}

	bool dates_match(const misc_wrappers::date &query,
					 const misc_wrappers::date &entry)
	{
		// year
		if (query.wYear != entry.wYear)
		{
			return false ;
		}
		// month
		ATLASSERT(query.wYear == entry.wYear) ;
		if (! query.wMonth)
		{
			return true ;
		}
		if (query.wMonth != entry.wMonth)
		{
			return false ;
		}
		// day
		ATLASSERT(query.wMonth == entry.wMonth) ;
		if (! query.wDay)
		{
			return true ;
		}
		if (query.wDay != entry.wDay)
		{
			return false ;
		}
		return true ;
	}
	bool date_after(const misc_wrappers::date &query,
		const misc_wrappers::date &entry)
	{
		if (dates_match(query, entry))
		{
			return false ;
		}
		return query < entry ;

	}

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
	bool search_runner::term_matches( const record_pointer rec, const wstring term ) const
	{
		wstring tag ;
		const wstring SOURCE_TAG = L"source:" ;
		const wstring TRANS_TAG = L"trans:" ;
		const wstring CONTEXT_TAG = L"context:" ;

		try
		{
			if (boost::starts_with(term, SOURCE_TAG))
			{
				return textfield_name_match(term, SOURCE_TAG, rec->get_source_plain());
			}
			if (boost::starts_with(term, TRANS_TAG))
			{
				return textfield_name_match(term, TRANS_TAG, rec->get_trans_plain());
			}
			if (boost::starts_with(term, CONTEXT_TAG))
			{
				return textfield_name_match(term, CONTEXT_TAG, rec->get_context_plain());
			}
			// Created
			tag = L"created-by:" ; // Search in creator field
			if (boost::starts_with(term, tag))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				if (boost::trim_copy(query).empty())
				{
					return true ;
				}
				return rec->get_creator().find(query) != wstring::npos ;
			}

			// Search in modifier field
			if (boost::starts_with(term, L"created:") || boost::starts_with(term, L"created-on:"))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				if (boost::trim_copy(query).empty())
				{
					return true ;
				}
				misc_wrappers::date query_date ;
				mod_date(query_date, query) ;
				misc_wrappers::date created_date = rec->get_created() ;
				return dates_match(query_date, created_date) ;
			}
			tag = L"created-before:" ; // Search for records created before date (YYYY-MM-DD format)
			if (boost::starts_with(term, tag))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				misc_wrappers::date query_date ;
				mod_date(query_date, query) ;
				return rec->get_created() < query_date ;
			}
			tag = L"created-after:" ; // Search for records created after date (YYYY-MM-DD format)
			if (boost::starts_with(term, tag))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				misc_wrappers::date query_date ;
				mod_date(query_date, query) ;
				misc_wrappers::date date_created = rec->get_created() ;
				return date_after(query_date, date_created) ;
			}
			// Modified
			if (boost::starts_with(term, L"modified-by:"))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				if (boost::trim_copy(query).empty())
				{
					return true ;
				}
				return rec->get_modified_by().find(query) != wstring::npos ;
			}

			if (boost::starts_with(term, L"modified:") || boost::starts_with(term, L"modified-on:"))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				if (boost::trim_copy(query).empty())
				{
					return true ;
				}
				misc_wrappers::date query_date ;
				mod_date(query_date, query) ;
				misc_wrappers::date modified_date = rec->get_modified() ;
				return dates_match(query_date, modified_date) ;
			}
			tag = L"modified-before:" ; // Search for records modified before date (YYYY-MM-DD format)
			if (boost::starts_with(term, tag))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				if (boost::trim_copy(query).empty())
				{
					return true ;
				}
				misc_wrappers::date query_date ;
				mod_date(query_date, query) ;
				return rec->get_modified() < query_date ;
			}
			tag = L"modified-after:" ; // Search for records modified after date (YYYY-MM-DD format)
			if (boost::starts_with(term, tag))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				misc_wrappers::date query_date ;
				mod_date(query_date, query) ;
				misc_wrappers::date date_modified = rec->get_modified() ;
				return date_after(query_date, date_modified) ;
			}
			// Reliability

			// Search for records with the specified reliability
			if (boost::starts_with(term, L"reliability:"))
			{
				wstring query = term.substr(term.find(L':')+1) ;
				if (boost::trim_copy(query).empty())
				{
					return true ;
				}
				return rec->get_reliability() == boost::lexical_cast<size_t>(query) ;
			}
			tag = L"reliability-gt:" ; // Search for records with greater than the specified reliability
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_reliability() > query ;
			}
			tag = L"reliability-gte:" ; // Search for records with at least the specified reliability
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_reliability() >= query ;
			}
			tag = L"reliability-lt:" ; // Search for records with less than the specified reliability
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_reliability() < query ;
			}
			tag = L"reliability-lte:" ; // Search for records with no more than than the specified reliability
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_reliability() <= query ;
			}
			// Validated
			tag = L"validated:" ; // Search for records that are validated ("true") or not validated ("false")
			if (boost::starts_with(term, tag))
			{
				wstring rhs = term.substr(tag.size()) ;
				if (boost::trim_copy(rhs).empty())
				{
					return true ;
				}
				bool query = string2bool(rhs) ;
				return rec->is_validated() == query ;
			}
			// Reference Count
			tag = L"refcount:" ; // Search for records with the specified reference count
			if (boost::starts_with(term, tag))
			{
				wstring rhs = term.substr(tag.size()) ;
				if (boost::trim_copy(rhs).empty())
				{
					return true ;
				}
				size_t query = boost::lexical_cast<size_t>(rhs) ;
				return rec->get_refcount() == query ;
			}
			tag = L"refcount-gt:" ; // Search for records with greater than the specified reference count
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_refcount() > query ;
			}
			tag = L"refcount-gte:" ; // Search for records with at least the specified reference count
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_refcount() >= query ;
			}
			tag = L"refcount-lt:" ; // Search for records with less than the specified reference count
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_refcount() < query ;
			}
			tag = L"refcount-lte:" ; // Search for records with no more than than the specified reference count
			if (boost::starts_with(term, tag))
			{
				size_t query = boost::lexical_cast<size_t>(term.substr(tag.size())) ;
				return rec->get_refcount() >= query ;
			}

			// search in source, trans, and context fields

			return ( textfield_match(term, rec->get_source_plain()) ||
				     textfield_match(term, rec->get_trans_plain()) ||
					 textfield_match(term, rec->get_context_plain())
					 ) ;
		}
		catch (std::exception& e)
		{
			logging::log_error("CRT exception") ;
			logging::log_error(e.what()) ;
			return false ;
		}
	}

	bool search_runner::textfield_name_match( const wstring &term, const wstring &tag, const wstring haystack ) const
	{
		const wstring needle = replacer::getrest(term, tag);
		return textfield_match(needle, haystack);
	}

	bool search_runner::textfield_match( const wstring needle, const wstring haystack ) const
	{
		if (boost::trim_copy(needle).empty())
		{
			return true ;
		}
		const wstring REGEX_TAG = L"regex:" ;
		if (boost::starts_with(needle, REGEX_TAG))
		{
			const wstring pattern  = L"(" + replacer::getrest(needle, REGEX_TAG) + L")" ;
			boost::wregex search_regex( pattern, boost::regex::extended|boost::regex::icase) ;
			return boost::regex_search(haystack, search_regex) ;
		}
		else
		{
			return boost::icontains(haystack, needle) ;
		}
	}
}