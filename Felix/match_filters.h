#pragma once

#include "record.h"
#include "TranslationMemory.h"
#include "miscwrappers.h"
#include "matcher.h"

namespace mem_search
{
	using namespace mem_engine ;

	// This class runs searches using the search terms provided.
	class search_runner
	{
		std::vector<wstring> m_terms ;
		std::vector<matcher_ptr> m_matchers ;
		wstring m_required_name ;

	public:
		// term management...
		const std::vector<wstring> &get_terms();
		void add_term(const wstring term);
		void remove_term(const size_t i);
		void clear_terms();

		// Get all the matches for a given memory
		void get_matches(memory_pointer mem, search_match_container& matches);

		void set_matchers();
		bool is_wanted_mem(memory_pointer mem);
		// Does the record match our terms?
		bool is_match(const record_pointer rec) const;

	};

}