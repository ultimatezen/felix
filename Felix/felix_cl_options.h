#pragma once

#include "logging.h"

// Tokenizes the command line
void parse_command_line(LPCTSTR text, std::vector<tstring> &tokens) ;

/* Parses the command line into program startup options
*/
struct commandline_options
{
	std::vector<tstring> m_tm_files ;
	std::vector<tstring> m_glossary_files ;
	std::vector<tstring> m_xml_files ;
	std::vector<tstring> m_tmx_files ;
	std::vector<tstring> m_trados_text_files ;
	std::vector<tstring> m_multiterm_files ;

	LOGGING_LEVEL m_logging_level ;
	
	tstring m_prefs_file ;
	WORD m_language ;

	commandline_options(LPCTSTR text, WORD language=LANG_ENGLISH);
	WORD parse_lang(const tstring lang) const;
};
