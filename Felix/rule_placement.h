/* glossary rules.

	regex-based.
*/
#pragma once
#include "TranslationMemory.h"
#include "MatchStringPairing.h"
#include "placement_holes.h"

#include "Felix_properties.h" // get_config_filename
#include "input_device.h" // InputDevice
#include "output_device.h"

namespace mem_engine
{
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace placement
{
	typedef std::pair<wstring, wstring> repl_t ;

	// a glossary rule.
	class regex_rule
	{
	public:
		const wstring m_name ;
		const boost::wregex m_expr;
		const wstring m_repl ;

		regex_rule(wstring name, wstring pattern, wstring repl) :
			m_name(name),
			m_expr(pattern, boost::regex::extended|boost::regex::icase),
			m_repl(repl)
		{

		}

		// Gets the matches for the search string.
		bool get_matches(const wstring haystack, std::vector<wstring> &matches) const ;
		// Gets the replacements for all matches found.
		bool get_replacements(const std::vector<wstring> &matches, std::vector<repl_t> &replacements) const ;

	};

	typedef boost::shared_ptr<regex_rule> regex_ptr ;

	// a collection of glossary rules
	class regex_rules
	{
	public:

		std::vector<regex_ptr> m_rules ;

		size_t get_placements(regex_ptr rule, const wstring haystack, std::vector<repl_t> &replacements);
		size_t get_matches(search_match_container &matches, const wstring text) ;

		// Load from preferences file
		void load(input_device_ptr input, output_device_ptr output);
		// Parse preferences file
		void parse(pugi::xml_document &doc);

		search_match_ptr make_match(const wstring source, const wstring trans);

	};

	/** 
	Make rule placements in queries.
	*/
	class rule_placer
	{
		regex_rules &m_rules ;
	public:
		rule_placer(regex_rules &rules) : m_rules(rules)
		{
		}
		// Get placement candidates
		bool place(pairings_t &pairings, trans_pair &trans, hole_pair_t &holes);

		bool is_valid_placement( const hole_pair_t &holes, const pairings_t & pairings, const wstring & trans,
			search_match_container &q_matches, search_match_container &s_matches);
		// matches with source containing `text`
		size_t get_matches(search_match_container &matches, const wstring text);

		// Number of times needle is found in haystack
		size_t num_hits(const wstring needle, const wstring haystack) const;

		// Get the source matches that have gloss hits in translation
		size_t get_trans_subset(search_match_container &matches, const wstring trans) const;

		void replace_trans_term( const wstring qword, const wstring trans_plain, trans_pair & trans ) const;
		void create_new_pairings( pairings_t &pairings, const hole_pair_t &holes ) const;
	};
}
}
