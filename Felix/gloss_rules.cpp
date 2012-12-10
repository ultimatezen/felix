#include "stdafx.h"
#include "gloss_rules.h"

namespace placement
{
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

	void regex_rules::parse( pugi::xml_document &doc )
	{
		pugi::xml_node nodes = doc.child("rules") ;
		for (pugi::xml_node node = nodes.child("rule"); 
			node; 
			node = node.next_sibling("rule"))
		{
			if (read_xml_ulong(node, "enabled"))
			{
				m_rules.push_back(regex_ptr(new regex_rule(L"", L"", L""))) ;
			}
		}
	}
}
