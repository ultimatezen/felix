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
		bool place_rule(regex_ptr rule, 
			pairings_t &pairings, 
			trans_pair &trans, 
			hole_pair_t &holes, 
			const wstring source, 
			const wstring query);

		wstring get_str(pairings_t &pairings, const CharType index) const ;

		bool get_rule_replacement_query( const hole_t &hole, regex_ptr rule, const wstring text, const repl_t &s_replacement, repl_t &replacement );
		bool get_rule_replacement_source( const hole_t &hole, regex_ptr rule, const wstring source, const wstring trans, repl_t &replacement );

		bool hole_fits(const hole_t &hole, const wstring repl, const wstring text);

		// Number of times needle is found in haystack
		size_t num_hits(const wstring needle, const wstring haystack) const;

		void replace_trans_term( const wstring qword, const wstring trans_plain, trans_pair & trans ) const;
	};
}
}
