#include "stdafx.h"
#include "matcher.h"
#include "logging.h"
#include "replacer.h"
namespace mem_search
{
	using namespace boost ;
	using namespace misc_wrappers ;
	using namespace replacer ;
	// comparison functions
	bool text_match( const wstring needle, const wstring haystack )
	{
		if (boost::trim_copy(needle).empty())
		{
			return true ;
		}
		return boost::icontains(haystack, needle) ;
	}
	bool regex_match( const wstring needle, const wstring haystack )
	{
		if (boost::trim_copy(needle).empty())
		{
			return true ;
		}
		const wstring pattern  = L"(" + needle + L")" ;
		boost::wregex search_regex( pattern, boost::regex::extended|boost::regex::icase) ;
		return boost::regex_search(haystack, search_regex) ;
	}

	bool sizet_match(size_t lhs, size_t rhs)
	{
		return lhs == rhs ;
	}

	text_cmp_result get_text_compare(const wstring query)
	{
		const wstring REGEX_TAG = L"regex:" ;
		if (boost::starts_with(query, REGEX_TAG))
		{
			return std::make_pair(replacer::getrest(query, REGEX_TAG),
				regex_match) ;
		}
		else
		{
			return std::make_pair(query,
				text_match) ;
		}
	}
	num_compare get_num_compare(const wstring tag) 
	{
		textstream_reader<wchar_t> reader ;
		reader.set_buffer(tag.c_str()) ;
		std::vector<wstring> bits ;
		reader.split(bits, L"-") ;
		if (bits.size() < 2)
		{
			return sizet_match ;
		}
		if (bits[1] == L"lt")
		{
			return std::less<size_t>() ;
		}
		if (bits[1] == L"lte")
		{
			return std::less_equal<size_t>() ;
		}
		if (bits[1] == L"gt")
		{
			return std::greater<size_t>() ;
		}
		if (bits[1] == L"gte")
		{
			return std::greater_equal<size_t>() ;
		}
		return sizet_match ;

	}
	date_compare get_date_compare(const wstring tag)
	{
		if (tag.find(L"before") != wstring::npos)
		{
			return std::less<const date>() ;
		}
		if (tag.find(L"after") != wstring::npos)
		{
			return date_after ;
		}
		return dates_match ;

	}

	size_t parse_num(const wstring text)
	{
		try
		{
			return lexical_cast<size_t>(boost::trim_copy(text)) ;
		}
		catch (std::exception& e)
		{
			logging::log_warn("lexical cast exception - converting search query term to number: " + string2string(text)) ;
			logging::log_warn(e.what()) ;
		}
		return 0 ;
	}

	//////////////////////////////////////////////////////////////////////////
	// matcher class defs
	//////////////////////////////////////////////////////////////////////////

	// source
	matcher_source::matcher_source( wstring needle, text_compare comparator ) :
		m_needle(needle),
		m_comparator(comparator)
	{

	}

	bool matcher_source::is_match( record_pointer record )
	{
		return m_comparator(m_needle, record->get_source_rich()) ;
	}

	// trans
	matcher_trans::matcher_trans( wstring needle, text_compare comparator ) :
		m_needle(needle),
		m_comparator(comparator)
	{

	}

	bool matcher_trans::is_match( record_pointer record )
	{
		return m_comparator(m_needle, record->get_trans_rich()) ;
	}

	// context
	matcher_context::matcher_context( wstring needle, text_compare comparator ) :
		m_needle(needle),
		m_comparator(comparator)
	{

	}

	bool matcher_context::is_match( record_pointer record )
	{
		return m_comparator(m_needle, record->get_context_rich()) ;
	}

	// generic
	matcher_generic::matcher_generic( wstring needle, text_compare comparator ) :
		m_needle(needle),
		m_comparator(comparator)
	{

	}

	bool matcher_generic::is_match( record_pointer record )
	{
		return m_comparator(m_needle, record->get_source_rich()) ||
			m_comparator(m_needle, record->get_trans_rich()) || 
			m_comparator(m_needle, record->get_context_rich()) ;
	}

	// refcount
	matcher_refcount::matcher_refcount( size_t lhs, num_compare comparator ) : 
	m_lhs(lhs), m_comparator(comparator)
	{

	}

	bool matcher_refcount::is_match( record_pointer record )
	{
		return m_comparator(record->get_refcount(), m_lhs) ;
	}

	// reliability
	matcher_reliability::matcher_reliability( size_t lhs, num_compare comparator ) : 
	m_lhs(lhs), m_comparator(comparator)
	{

	}

	bool matcher_reliability::is_match( record_pointer record )
	{
		return m_comparator(record->get_reliability(), m_lhs) ;
	}

	// validated
	matcher_validated::matcher_validated( bool val ) : m_val(val)
	{

	}

	bool matcher_validated::is_match( record_pointer record )
	{
		return record->is_validated() == m_val ;
	}

	// modified
	matcher_modified::matcher_modified( misc_wrappers::date cmp_date, date_compare comparator ) :
	m_date(cmp_date), m_comparator(comparator)
	{

	}
	bool matcher_modified::is_match( record_pointer record )
	{
		return m_comparator(record->get_modified(), m_date) ;
	}

	// created 
	matcher_created::matcher_created( misc_wrappers::date cmp_date, date_compare comparator ) :
	m_date(cmp_date), m_comparator(comparator)
	{

	}
	bool matcher_created::is_match( record_pointer record )
	{
		return m_comparator(record->get_created(), m_date) ;
	}


	// created by
	matcher_created_by::matcher_created_by( wstring needle, text_compare comparator ) :
	m_needle(needle),
		m_comparator(comparator)
	{

	}

	bool matcher_created_by::is_match( record_pointer record )
	{
		return m_comparator(m_needle, record->get_creator()) ;
	}

	matcher_modified_by::matcher_modified_by( wstring needle, text_compare comparator ) :
	m_needle(needle),
		m_comparator(comparator)
	{

	}

	bool matcher_modified_by::is_match( record_pointer record )
	{
		return m_comparator(m_needle, record->get_modified_by()) ;
	}
	// get_matcher
	matcher_ptr get_matcher(const wstring query)
	{
		const wstring SOURCE_TAG = L"source:" ;
		const wstring TRANS_TAG = L"trans:" ;
		const wstring CONTEXT_TAG = L"context:" ;
		const wstring CREATED_BY_TAG = L"created-by:" ;
		const wstring MODIFIED_BY_TAG = L"modified-by:" ;
		const wstring VALIDATED_TAG = L"validated:" ;
		// refcount tags
		const wstring REFCOUNT_TAG = L"refcount:" ;
		const wstring REFCOUNT_LT_TAG = L"refcount-lt:" ;
		const wstring REFCOUNT_LTE_TAG = L"refcount-lte:" ;
		const wstring REFCOUNT_GT_TAG = L"refcount-gt:" ;
		const wstring REFCOUNT_GTE_TAG = L"refcount-gte:" ;
		// refcount tags
		const wstring RELIABILITY_TAG = L"reliability:" ;
		const wstring RELIABILITY_LT_TAG = L"reliability-lt:" ;
		const wstring RELIABILITY_LTE_TAG = L"reliability-lte:" ;
		const wstring RELIABILITY_GT_TAG = L"reliability-gt:" ;
		const wstring RELIABILITY_GTE_TAG = L"reliability-gte:" ;
		// refcount tags
		const wstring CREATED_TAG = L"created:" ;
		const wstring CREATED_ON_TAG = L"created-on:" ;
		const wstring CREATED_LT_TAG = L"created-before:" ;
		const wstring CREATED_GT_TAG = L"created-after:" ;
		// refcount tags
		const wstring MODIFIED_TAG = L"modified:" ;
		const wstring MODIFIED_ON_TAG = L"modified-on:" ;
		const wstring MODIFIED_LT_TAG = L"modified-before:" ;
		const wstring MODIFIED_GT_TAG = L"modified-after:" ;

		if (query.empty())
		{
			return matcher_ptr(new matcher_nullop) ;
		}

		try
		{
			if (boost::starts_with(query, CREATED_TAG)
				|| boost::starts_with(query, CREATED_ON_TAG)
				|| boost::starts_with(query, CREATED_LT_TAG)
				|| boost::starts_with(query, CREATED_GT_TAG))
			{
				size_t pos = query.find(L':') ;
				date_compare comparator = get_date_compare(query.substr(0, pos)) ;
				date compare_date ;
				try
				{
					mod_date(compare_date, query.substr(pos+1)) ;
				}
				catch (std::exception& e)
				{
					logging::log_warn("error parsing date: " + string2string(query)) ;
					logging::log_warn(e.what()) ;
					return matcher_ptr(new matcher_nullop(false)) ;
				}
				if (! compare_date.wYear)
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				return matcher_ptr(new matcher_created(compare_date, comparator)) ;
			}
			if (boost::starts_with(query, MODIFIED_TAG)
				|| boost::starts_with(query, MODIFIED_ON_TAG)
				|| boost::starts_with(query, MODIFIED_LT_TAG)
				|| boost::starts_with(query, MODIFIED_GT_TAG))
			{
				size_t pos = query.find(L':') ;
				date_compare comparator = get_date_compare(query.substr(0, pos)) ;
				date compare_date ;
				try
				{
					mod_date(compare_date, query.substr(pos+1)) ;
				}
				catch (std::exception& e)
				{
					logging::log_warn("error parsing date: " + string2string(query)) ;
					logging::log_warn(e.what()) ;
					return matcher_ptr(new matcher_nullop(false)) ;
				}
				if (! compare_date.wYear)
				{
					return matcher_ptr(new matcher_nullop) ;
				}			
				return matcher_ptr(new matcher_modified(compare_date, comparator)) ;
			}

			if (boost::starts_with(query, VALIDATED_TAG))
			{
				wstring rhs = replacer::getrest(query, VALIDATED_TAG) ;
				boost::trim(rhs) ;
				if (rhs.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				bool val = string2bool(rhs) ;
				return matcher_ptr(new matcher_validated(val)) ;
			}

			if (boost::starts_with(query, RELIABILITY_TAG)
				|| boost::starts_with(query, RELIABILITY_LT_TAG)
				|| boost::starts_with(query, RELIABILITY_LTE_TAG)
				|| boost::starts_with(query, RELIABILITY_GT_TAG)
				|| boost::starts_with(query, RELIABILITY_GTE_TAG))
			{
				size_t pos = query.find(L':') ;
				num_compare comparator = get_num_compare(query.substr(0, pos)) ;
				wstring rhs = query.substr(pos+1) ;
				if (rhs.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				size_t num = parse_num(rhs) ;
				return matcher_ptr(new matcher_reliability(num, comparator)) ;
			}
			if (boost::starts_with(query, REFCOUNT_TAG)
				|| boost::starts_with(query, REFCOUNT_LT_TAG)
				|| boost::starts_with(query, REFCOUNT_LTE_TAG)
				|| boost::starts_with(query, REFCOUNT_GT_TAG)
				|| boost::starts_with(query, REFCOUNT_GTE_TAG))
			{
				size_t pos = query.find(L':') ;
				num_compare comparator = get_num_compare(query.substr(0, pos)) ;
				wstring rhs = query.substr(pos+1) ;
				if (rhs.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				size_t num = parse_num(rhs) ;
				return matcher_ptr(new matcher_refcount(num, comparator)) ;
			}
			// text tags
			if (boost::starts_with(query, SOURCE_TAG))
			{
				wstring rest = replacer::getrest(query, SOURCE_TAG) ;
				if (rest.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				text_cmp_result result = get_text_compare(rest) ;
				return matcher_ptr(new matcher_source(result.first, result.second)) ;
			}
			if (boost::starts_with(query, TRANS_TAG))
			{
				wstring rest = replacer::getrest(query, TRANS_TAG) ;
				if (rest.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				text_cmp_result result = get_text_compare(rest) ;
				return matcher_ptr(new matcher_trans(result.first, result.second)) ;
			}
			if (boost::starts_with(query, CONTEXT_TAG))
			{
				wstring rest = replacer::getrest(query, CONTEXT_TAG) ;
				if (rest.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				text_cmp_result result = get_text_compare(rest) ;
				return matcher_ptr(new matcher_context(result.first, result.second)) ;
			}
			if (boost::starts_with(query, CREATED_BY_TAG))
			{
				wstring rest = replacer::getrest(query, CREATED_BY_TAG) ;
				if (rest.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				text_cmp_result result = get_text_compare(rest) ;
				return matcher_ptr(new matcher_created_by(result.first, result.second)) ;
			}		
			if (boost::starts_with(query, MODIFIED_BY_TAG))
			{
				wstring rest = replacer::getrest(query, MODIFIED_BY_TAG) ;
				if (rest.empty())
				{
					return matcher_ptr(new matcher_nullop) ;
				}
				text_cmp_result result = get_text_compare(rest) ;
				return matcher_ptr(new matcher_modified_by(result.first, result.second)) ;
			}
			// search in source, trans, and context fields

			text_cmp_result result = get_text_compare(query) ;
			return matcher_ptr(new matcher_generic(result.first, result.second)) ;

		}
		catch (std::exception& e)
		{
			logging::log_warn("Failed to parse search term") ;
			logging::log_warn(e.what()) ;
			return matcher_ptr(new matcher_nullop(false)) ;
		}
	}
}
