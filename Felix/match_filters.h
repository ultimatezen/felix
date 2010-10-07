#pragma once

#include "record.h"
#include "TranslationMemory.h"
#include "miscwrappers.h"

namespace mem_search
{
	using namespace mem_engine ;

	// Modify the date according to the date string.
	void mod_date(misc_wrappers::date &thedate, 
				  const wstring datestring ) ;
	/* Do the dates match?
	   Allows year or year/month matching. For example:
	    2010 == 2010-10-10
		2010-10 == 2010-10-10
	*/
	bool dates_match(const misc_wrappers::date &query,
		const misc_wrappers::date &entry) ;

	/* Is the query before the record date?
	   Allows year or year/month matching. For example:
	    2010 < 2010-10-10
		2010-10 < 2010-11-10
		BUT:
		! 2010-02 < 2010-02-01 (they are equal)
	*/
	bool date_after(const misc_wrappers::date &query,
		const misc_wrappers::date &entry) ;

	// This class runs searches using the search terms provided.
	class search_runner
	{
		std::vector<wstring> m_terms ;
	public:
		// term management...
		const std::vector<wstring> &get_terms();
		void add_term(const wstring term);
		void remove_term(const size_t i);
		void clear_terms();

		// Get all the matches for a given memory
		void get_matches(memory_pointer mem, search_match_container& matches);
		// Does the record match our terms?
		bool is_match(const record_pointer rec) const;
		// Does the record match a given search term?
		bool term_matches(const record_pointer rec, const wstring term) const;

		bool textfield_name_match( const wstring &term, const wstring &tag, const wstring haystack ) const;

		wstring extract_needle( const wstring &term, const wstring &tag ) const;
		bool textfield_match( const wstring needle, const wstring haystack ) const;
	};

}