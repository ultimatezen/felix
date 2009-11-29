#include "stdafx.h"
#include "felix_cl_options.h"
#include "file.h"

/** Parse files to load from the command line.
* has to handle quoted file names.
*/
void parse_command_line(LPCTSTR text, std::vector<tstring> &tokens)
{
	LPTSTR *szArglist;
	int nArgs;

	/* If this parameter is the empty string (""), the function returns 
	the path to the current executable file. */
	szArglist = CommandLineToArgvW(text, &nArgs);
	if( NULL == szArglist )
	{
		ATLTRACE("Failed to parse command line\n") ;
		return ;
	}

	for( int i=0; i<nArgs; i++) 
	{
		tokens.push_back(szArglist[i]) ;
	}

	// Free memory allocated for CommandLineToArgvW arguments.
	LocalFree(szArglist);
}

commandline_options::commandline_options( LPCTSTR text, WORD language/*=LANG_ENGLISH*/ ) : m_language(language)
{
	std::vector<tstring> tokens ;
	parse_command_line(text, tokens) ;

	for (size_t i=0 ; i < tokens.size() ; ++i)
	{
		tstring token = tokens[i] ;

		file::CFileExtension ext(CString(token.c_str())) ;
		if (ext.equals(_T(".ftm")))
		{
			m_tm_files.push_back(token) ;
		}
		else if (ext.equals(_T(".fgloss")))
		{
			m_glossary_files.push_back(token) ;
		}
		else if (ext.is_xml())
		{
			m_xml_files.push_back(token) ;
		}
		else if (ext.is_txt())
		{
			m_trados_text_files.push_back(token) ;
		}
		else if(ext.equals(_T(".tmx")))
		{
			m_tmx_files.push_back(token) ;
		}
		else if(ext.equals(_T(".fprefs")))
		{
			m_prefs_file = token ;
		}
		else if (boost::to_lower_copy(token) == _T("-lang"))
		{
			++i ;
			this->m_language = this->parse_lang(tokens[i]) ;
		}
	}
}


WORD commandline_options::parse_lang( tstring lang )
{
	boost::to_lower(lang) ;
	if (lang == _T("japanese"))
	{
		return LANG_JAPANESE ;
	}
	if (lang == _T("ja")) 
	{
		return LANG_JAPANESE ;
	}
	if (lang == _T("jp"))
	{
		return LANG_JAPANESE ;
	}
	return LANG_ENGLISH ;
}