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

	// Try to place the glossary match in our source/trans from the query.
	bool rule_placer::place( pairings_t &pairings, std::pair< wstring, wstring > &trans, hole_pair_t &holes )
	{

		/* First, see if it's a valid placement.
			* Got to:
			*	1. Have a gloss match for the query hole.
			*	2. Have a gloss match for the source hole.
			*	3. Have a match between the gloss trans and the source translation.
			*/

		search_match_container q_matches ;
		search_match_container s_matches ;
		if (! is_valid_placement(holes, pairings, trans.first, q_matches, s_matches))
		{
			return false ;
		}

		// It's a valid placement, so create the new pairings
		create_new_pairings(pairings, holes);

		// Now, do the replacement in the translation segment.
		auto qmatch = *q_matches.begin() ;
		auto qrec = qmatch->get_record() ;
		auto smatch = *s_matches.begin() ;
		auto srec = smatch->get_record() ;

		// replace the translation
		replace_trans_term(qrec->get_trans_plain(), srec->get_trans_plain(), trans);

		return true ;
	}

	// Narrow down gloss matches to those whose translations are in trans.
	size_t rule_placer::get_trans_subset( search_match_container &matches, const wstring trans ) const
	{
		search_match_container tmp ;
		foreach(search_match_ptr match, matches)
		{
			wstring gloss_hit = match->get_record()->get_trans_plain() ;
			// IFF it occurs once in trans
			if (this->num_hits(gloss_hit, trans) == 1)
			{
				tmp.insert(match) ;
			}
		}
		std::swap(matches, tmp) ;
		return matches.size() ;
	}

	// How many matches are in the translation?
	// We want to make sure it is in the translation once, and only once.
	size_t rule_placer::num_hits( const wstring needle, const wstring haystack ) const
	{
		size_t count = 0 ;
		for(size_t pos = haystack.find(needle) ; pos != wstring::npos; pos = haystack.find(needle, pos+1))
		{
			++count ;
		}
		return count ;
	}

	// Redo the pairings based on the placement.
	void rule_placer::create_new_pairings( pairings_t &pairings, const hole_pair_t &holes ) const
	{
		const wstring query = holes.lhs.get_str_query(pairings) ;
		std::vector<pairing_t> pairvec ;
		pairvec.assign(pairings.begin(), pairings.end()) ;

		pairings.clear() ;

		// start
		for(size_t i=0 ; i < holes.lhs.start ; ++i)
		{
			pairings.push_back(pairvec[i]) ;
		}

		// middle
		foreach(wchar_t c, query)
		{
			pairings.push_back(pairing_entity(c, PLACEMENT, c)) ;
		}

		// end
		for(size_t i=holes.lhs.start + holes.lhs.len ; i < pairvec.size() ; ++i)
		{
			pairings.push_back(pairvec[i]) ;
		}

	}

	// Replace the translated term in the translation
	void rule_placer::replace_trans_term( const wstring qword, const wstring trans_plain, std::pair< wstring, wstring > & trans ) const
	{
		const static wstring placement_fmt( L"<span class=\"placement\">%s</span>" ) ;
		wstring replacement = ( boost::wformat( placement_fmt ) % qword ).str() ;
		boost::replace_first(trans.first, trans_plain, qword) ;
		boost::replace_first(trans.second, trans_plain, replacement) ;
		fix_html_entities(trans.first) ;
		fix_html_entities(trans.second) ;
		fix_match_spans(trans.second) ;
	}

	// Gathers up the matches for the query and source, and determines whether this is a valid placement.
	bool rule_placer::is_valid_placement( const hole_pair_t &holes, const pairings_t & pairings, const wstring & trans, search_match_container &q_matches, search_match_container &s_matches )
	{
		const wstring query = holes.rhs.get_str_query(pairings) ;
		if(m_rules.get_matches(q_matches, query) == 0)
		{
			return false ;
		}

		const wstring source = holes.lhs.get_str_source(pairings); 
		if(m_rules.get_matches(s_matches, source) == 0)
		{
			return false ;
		}
		if(this->get_trans_subset(s_matches, trans) == 0)
		{
			return false ;
		}
		return true ;
	}
}
}
