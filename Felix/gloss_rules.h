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
		const wstring m_pattern ;
		const wstring m_repl ;

		regex_rule(wstring name, wstring pattern, wstring repl) :
			m_name(name),
			m_pattern(pattern),
			m_repl(repl)
		{

		}

	};

	typedef boost::shared_ptr<regex_rule> regex_ptr ;

	// a collection of glossary rules
	class regex_rules
	{
	public:

		std::vector<regex_ptr> m_rules ;

		void load(input_device_ptr input, output_device_ptr output);
		void parse(pugi::xml_document &doc);
	};
}
