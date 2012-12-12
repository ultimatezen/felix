/* glossary rules.

	regex-based.
*/

#include "Felix_properties.h" // get_config_filename
#include "input_device.h" // InputDevice
#include "output_device.h"

namespace placement
{
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
		bool get_replacements(const std::vector<wstring> &matches, std::vector<std::pair<wstring, wstring> > &replacements) const ;

	};

	typedef boost::shared_ptr<regex_rule> regex_ptr ;

	// a collection of glossary rules
	class regex_rules
	{
	public:

		std::vector<regex_ptr> m_rules ;

		// Load from preferences file
		void load(input_device_ptr input, output_device_ptr output);
		// Parse preferences file
		void parse(pugi::xml_document &doc);
	};
}
