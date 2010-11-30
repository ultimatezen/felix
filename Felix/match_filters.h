#pragma once

#include "record.h"
#include "TranslationMemory.h"
#include "miscwrappers.h"
#include "matcher.h"

namespace mem_search
{
	using namespace mem_engine ;


	bool textfield_match( const wstring needle, const wstring haystack ) ;

	// This class runs searches using the search terms provided.
	class search_runner
	{
		std::vector<wstring> m_terms ;
		std::vector<matcher_ptr> m_matchers ;

	public:
		// term management...
		const std::vector<wstring> &get_terms();
		void add_term(const wstring term);
		void remove_term(const size_t i);
		void clear_terms();

		// Get all the matches for a given memory
		void get_matches(memory_pointer mem, search_match_container& matches);

		void set_matchers();
		// Does the record match our terms?
		bool is_match(const record_pointer rec) const;
		// Does the record match a given search term?
		bool term_matches(const record_pointer rec, const wstring term) const;

		bool textfield_name_match( const wstring &term, const wstring &tag, const wstring haystack ) const;

	};

}