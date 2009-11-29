#pragma once

#include "record.h"
#include "TranslationMemory.h"
#include "miscwrappers.h"

namespace memory_searcher
{
	using namespace memory_engine ;

	class search_runner
	{
		std::vector<wstring> m_terms ;
	public:
		std::vector<wstring> &get_terms();
		void add_term(const wstring term);
		void remove_term(const size_t i);
		void clear_terms();
		void get_matches(memory_pointer mem, search_match_multiset& matches);
		bool is_match(record_pointer rec);
		bool term_matches(record_pointer rec, const wstring &term);
		void mod_date(misc_wrappers::date &thedate, wstring datestring);

	};

}