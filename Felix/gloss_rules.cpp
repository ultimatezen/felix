#include "stdafx.h"
#include "gloss_rules.h"

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

	bool regex_rule::get_replacements( const std::vector<wstring> &matches, std::vector<std::pair<wstring, wstring> > &replacements ) const
	{
		foreach(wstring m, matches)
		{
			const wstring result = boost::regex_replace(m, m_expr, m_repl);
			replacements.push_back(std::make_pair(m, result)) ;
		}
		return ! replacements.empty() ;
	}


	//////////////////////////////////////////////////////////////////////////
	// regex_rules class
	//////////////////////////////////////////////////////////////////////////

	// Load rules from file in preferences.
	void regex_rules::load(input_device_ptr input, output_device_ptr output)
	{
		m_rules.clear() ;
		wstring filename = get_config_filename(L"felix_rules.frules", output) ;
		string text = get_file_text(filename, input) ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		ATLASSERT ( result.status == pugi::status_ok ) ; 

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
}
