#pragma once

#include "record.h"
#include "replacer.h"

namespace mem_search
{
	using namespace mem_engine ;

	typedef boost::function<bool (wstring, wstring)> text_compare ;
	typedef boost::function<bool (const misc_wrappers::date, const misc_wrappers::date)> date_compare ;
	typedef boost::function<bool (size_t, size_t)> num_compare ;

	typedef std::pair<wstring, text_compare> text_cmp_result ;

	// custom comparison functions
	bool text_match( const wstring needle, const wstring haystack ) ;
	bool regex_match( const wstring needle, const wstring haystack ) ;
	bool sizet_match(size_t lhs, size_t rhs) ;
	size_t parse_num(const wstring text) ;

	// retrieve the proper comparison functions
	text_cmp_result get_text_compare(const wstring query) ;
	num_compare get_num_compare(const wstring tag) ;
	date_compare get_date_compare(const wstring tag) ;

	/* matcher virtual base class
	* matcher subclasses carry the value to compare against, and the function
	*	for comparison.
	* For example, if the tag is `reliability-gt:7`, then the functor object
	*	will carry the value 7 (parsed in to size_t), ad a "greater than" comparison function.
	*	It also knows to compare this to the reliability field of the passed-in record.
	*/
	class matcher
	{
	public:
		virtual bool is_match(record_pointer record) = 0 ;
	};
	typedef boost::shared_ptr<matcher> matcher_ptr ;

	// created on
	class matcher_nullop : public matcher
	{
		bool m_val ;
	public:
		matcher_nullop(bool val=true) : m_val(val){}
		bool is_match(record_pointer)
		{
			return m_val ;
		}
	};
	// created on
	class matcher_created : public matcher
	{
		misc_wrappers::date m_date ;
		date_compare m_comparator ;
	public:
		matcher_created(misc_wrappers::date cmp_date, date_compare comparator);

		bool is_match(record_pointer record);
	};
	// modified on
	class matcher_modified : public matcher
	{
		misc_wrappers::date m_date ;
		date_compare m_comparator ;
	public:
		matcher_modified(misc_wrappers::date cmp_date, date_compare comparator);

		  bool is_match(record_pointer record);
	};
	// validated
	class matcher_validated : public matcher
	{
		bool m_val ;
	public:
		matcher_validated(bool val);

		bool is_match(record_pointer record);
	};

	// refcount
	class matcher_refcount : public matcher
	{
		size_t m_lhs ;
		num_compare m_comparator ;
	public:
		matcher_refcount(size_t lhs, num_compare comparator);

		bool is_match(record_pointer record);
	};

	// reliability
	class matcher_reliability : public matcher
	{
		size_t m_lhs ;
		num_compare m_comparator ;
	public:
		matcher_reliability(size_t lhs, num_compare comparator);

		bool is_match(record_pointer record);
	};

	// source
	class matcher_source : public matcher
	{
		wstring m_needle ;
		text_compare m_comparator ;
	public:
		matcher_source(wstring needle, text_compare comparator);
		bool is_match(record_pointer record);
	};

	// trans
	class matcher_trans : public matcher
	{
		wstring m_needle ;
		text_compare m_comparator ;
	public:
		matcher_trans(wstring needle, text_compare comparator);
		bool is_match(record_pointer record);
	};

	// context
	class matcher_context : public matcher
	{
		wstring m_needle ;
		text_compare m_comparator ;
	public:
		matcher_context(wstring needle, text_compare comparator);
		bool is_match(record_pointer record);
	};

	// generic
	// (no field tag: searches source, trans, and context; regex tag allowed)
	class matcher_generic : public matcher
	{
		wstring m_needle ;
		text_compare m_comparator ;
	public:
		matcher_generic(wstring needle, text_compare comparator);
		bool is_match(record_pointer record);
	};

	// created by
	class matcher_created_by : public matcher
	{
		wstring m_needle ;
		text_compare m_comparator ;
	public:
		matcher_created_by(wstring needle, text_compare comparator);
		bool is_match(record_pointer record);
	};

	// modified by
	class matcher_modified_by : public matcher
	{
		wstring m_needle ;
		text_compare m_comparator ;
	public:
		matcher_modified_by(wstring needle, text_compare comparator);
		bool is_match(record_pointer record);
	};

	// get matcher
	// feed in query, and get back appropriate matcher function object
	matcher_ptr get_matcher(const wstring query) ;

}
