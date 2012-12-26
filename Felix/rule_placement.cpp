#include "stdafx.h"
#include "rule_placement.h"
#include "record_local.h"
#include "logging.h"

namespace mem_engine
{
namespace placement
{
	//////////////////////////////////////////////////////////////////////////
	// regex_rule class
	//////////////////////////////////////////////////////////////////////////

	// Gets all of the substrings that match this rule.
	bool regex_rule::get_matches( const wstring haystack, std::vector<wstring> &matches ) const
	{
		boost::wsmatch what ;
		auto start = haystack.begin() ;
		auto end = haystack.end() ;
		while(boost::regex_search(start, end, what, m_expr))
		{
			matches.push_back(what[0]) ;
			start = what[0].second ;
		}
		return ! matches.empty();
	}

	// Calculates the replacement strings for the matches found in get_matches.
	bool regex_rule::get_replacements( const std::vector<wstring> &matches, std::vector<repl_t> &replacements ) const
	{
		foreach(wstring m, matches)
		{
			const wstring result = boost::regex_replace(m, m_expr, m_repl);
			replacements.push_back(repl_t(m, result)) ;
		}
		return ! replacements.empty() ;
	}


	//////////////////////////////////////////////////////////////////////////
	// regex_rules class
	//////////////////////////////////////////////////////////////////////////

	// Get matches for the specified text
	size_t regex_rules::get_matches(search_match_container &matches, const wstring text)
	{
		std::vector<repl_t> replacements ;
		foreach(regex_ptr rule, m_rules)
		{
			this->get_placements(rule, text, replacements) ;
		}

		foreach(repl_t repl, replacements)
		{
			matches.insert(this->make_match(repl.first, repl.second)) ;
		}

		return matches.size() ;
	}

	// Get the placements for a given rule.
	// We accumulate the placements, but get a fresh batch of matches for each rule.
	size_t regex_rules::get_placements(regex_ptr rule, const wstring haystack, std::vector<repl_t> &replacements )
	{
		std::vector<wstring> matches ;
		rule->get_matches(haystack, matches) ;
		rule->get_replacements(matches, replacements) ;

		return replacements.size() ;
	}
	// Load rules from file in preferences.
	void regex_rules::load(input_device_ptr input, output_device_ptr output)
	{
		m_rules.clear() ;
		wstring filename = get_config_filename(L"felix_rules.frules", output) ;
		string text = get_file_text(filename, input) ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		ATLASSERT ( result.status == pugi::status_ok ) ; 
		// abort on failure
		if (result.status != pugi::status_ok)
		{
			logging::log_warn("Failed to load: " + string2string(filename) + string("\n\t") + result.description()) ;
			return ;
		}

		this->parse(doc) ;
	}

	// Parse the XML doc into regex rules.
	void regex_rules::parse( pugi::xml_document &doc )
	{
		pugi::xml_node nodes = doc.child("rules") ;
		for (pugi::xml_node node = nodes.child("rule"); 
			node; 
			node = node.next_sibling("rule"))
		{
			// Only load enabled rules
			if (read_xml_ulong(node, "enabled"))
			{
				m_rules.push_back(regex_ptr(new regex_rule(read_xml_string(node, "name"),
									read_xml_string(node, "source"), 
									read_xml_string(node, "target")))) ;
			}
		}
	}

	// Creates a new match with the source and trans strings.
	// This entails creating the wrapped record object, and then setting the record values to the match.
	mem_engine::search_match_ptr regex_rules::make_match( const wstring source, const wstring trans )
	{
		record_pointer rec(new record_local) ;
		rec->set_source(source) ;
		rec->set_trans(trans) ;
		search_match_ptr match(new search_match) ;
		match->set_memory_id(RULE_MEMORY_ID) ;
		match->set_memory_location(L"Rules") ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		return match ;
	}

	//////////////////////////////////////////////////////////////////////////
	// rule_placer
	//////////////////////////////////////////////////////////////////////////

	wstring rule_placer::get_str(pairings_t &pairings, const CharType index) const
	{
		wstring s ; 
		foreach(pairing_t p, pairings)
		{
			if (p.get_char(index))
			{
				s += p.get_char(index) ;
			}
		}
		return s ;

	}

	// Try to place the glossary match in our source/trans from the query.
	bool rule_placer::place( pairings_t &pairings, repl_t &trans, hole_pair_t &holes )
	{

		const wstring source = this->get_str(pairings, SOURCE) ; 
		const wstring query = this->get_str(pairings, QUERY) ;

		foreach(regex_ptr rule, this->m_rules.m_rules)
		{
			if (place_rule(rule, pairings, trans, holes, source, query))
			{
				return true ;
			}
		}
		return false ;
/*
		// It's a valid placement, so create the new pairings
		create_new_pairings(pairings, holes.lhs);

		// replace the translation
		replace_trans_term(L"query", L"source", trans);

		return true ;
*/
	}

	bool rule_placer::place_rule( regex_ptr rule, 
				pairings_t &pairings, 
				trans_pair &trans, 
				hole_pair_t &holes, 
				const wstring source, 
				const wstring query)
	{
		repl_t s_replacement ;
		if (! this->get_rule_replacement_source(holes.lhs, rule, source, trans.first, s_replacement))
		{
			return false ;
		}
		repl_t q_replacement ;
		if (! this->get_rule_replacement_query(holes.rhs, rule, query, s_replacement, q_replacement))
		{
			return false ;
		}

		const size_t start = query.find(q_replacement.first) ;
		const size_t len = q_replacement.first.size() ;
		
		pairings_t pairvec ;
		// start
		for(size_t i=0 ; i < start ; ++i)
		{
			pairvec.push_back(pairings[i]) ;
		}

		// middle
		foreach(wchar_t c, q_replacement.first)
		{
			pairvec.push_back(pairing_entity(c, PLACEMENT, c)) ;
		}

		// end
		for(size_t i=start + len ; i < pairings.size() ; ++i)
		{
			pairvec.push_back(pairings[i]) ;
		}

		pairings.swap(pairvec) ;
		
		// The translation
		replace_trans_term(q_replacement.second, s_replacement.second, trans);

		return true ;

	}
	bool rule_placer::get_rule_replacement_source( const hole_t &hole, 
								regex_ptr rule, 
								const wstring source, 
								const wstring trans, 
								repl_t &replacement )
	{
		std::vector<wstring> matches ;
		if (! rule->get_matches(source, matches))
		{
			return false ;
		}
		std::vector<repl_t> replacements;
		if (! rule->get_replacements(matches, replacements))
		{
			return false ;
		}

		foreach(repl_t rep, replacements)
		{
			if (num_hits(rep.first, source) == 1 && num_hits(rep.second, trans) == 1)
			{
				if (hole_fits(hole, rep.first, source))
				{
					replacement = rep ;
					return true ;
				}
			}
		}
		return false ;
	}
	bool rule_placer::get_rule_replacement_query( const hole_t &hole, 
								regex_ptr rule, 
								const wstring text, 
								const repl_t &s_replacement, 
								repl_t &replacement )
	{
		std::vector<wstring> matches ;
		if (! rule->get_matches(text, matches))
		{
			return false ;
		}
		std::vector<repl_t> replacements;
		if (! rule->get_replacements(matches, replacements))
		{
			return false ;
		}

		foreach(repl_t rep, replacements)
		{
			if (num_hits(rep.first, text) == 1)
			{
				if (hole_fits(hole, rep.first, text) && s_replacement != rep)
				{
					replacement = rep ;
					return true ;
				}
			}
		}

		return false ;
	}

	// Does the hole fit in the placement?
	bool rule_placer::hole_fits( const hole_t &hole, const wstring repl, const wstring text )
	{
		size_t start = text.find(repl) ;
		if (start == wstring::npos)
		{
			return false ;
		}
		if (start > hole.start)
		{
			return false ;
		}
		if (start + repl.size() < hole.start + hole.len)
		{
			return false ;
		}
		return true ;
	}


	// How many matches are in the translation?
	// We want to make sure it is in the translation once, and only once.
	size_t rule_placer::num_hits( const wstring needle, const wstring haystack ) const
	{
		size_t count = 0 ;
		if (needle.empty() || haystack.empty())
		{
			return count ;
		}
		for(size_t pos = haystack.find(needle) ; pos != wstring::npos; pos = haystack.find(needle, pos + needle.size()))
		{
			++count ;
		}
		return count ;
	}


	// Replace the translated term in the translation
	void rule_placer::replace_trans_term( const wstring qword, const wstring trans_plain, repl_t &trans ) const
	{
		const static wstring placement_fmt( L"<span class=\"placement\">%s</span>" ) ;
		wstring replacement = ( boost::wformat( placement_fmt ) % qword ).str() ;
		boost::replace_first(trans.first, trans_plain, qword) ;
		boost::replace_first(trans.second, trans_plain, replacement) ;
		fix_html_entities(trans.first) ;
		fix_html_entities(trans.second) ;
		fix_match_spans(trans.second) ;
	}


}
}
