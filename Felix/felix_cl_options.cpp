#include "stdafx.h"
#include "felix_cl_options.h"
#include "file.h"

/** Parse files to load from the command line.
* has to handle quoted file names.
*/
void parse_command_line(LPCTSTR text, std::vector<tstring> &tokens)
{
	LPWSTR *szArglist;
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

commandline_options::commandline_options( LPCTSTR text, WORD language/*=LANG_ENGLISH*/ ) : 
	m_language(language),
	m_logging_level(LOGGING_WARN),
	m_new_prefs_format(false)
{
	std::vector<tstring> tokens ;
	parse_command_line(text, tokens) ;

	for (size_t i=0 ; i < tokens.size() ; ++i)
	{
		tstring token = tokens[i] ;

		if(! parse_filename(token))
		{
			if (boost::to_lower_copy(token) == _T("-lang"))
			{
				++i ;
				this->m_language = this->parse_lang(tokens[i]) ;
			}
			else if (boost::to_lower_copy(token) == _T("-logging"))
			{
				++i ;
				tstring level = tokens[i] ;
				parse_logging_level(level);
			}
		}
	}
}


WORD commandline_options::parse_lang( const tstring lang ) const
{
	const std::set<wstring> langs = list_of(L"japanese") (L"ja") (L"jp") ;
	if (std::find(langs.begin(), langs.end(), boost::to_lower_copy(lang)) == langs.end())
	{
		return LANG_ENGLISH ;
	}
	return LANG_JAPANESE ;
}

bool commandline_options::parse_filename( tstring &token )
{
	const file::CFileExtension ext(CString(token.c_str())) ;
	if (ext.equals(_T(".ftm")))
	{
		m_tm_files.push_back(token) ;
		return true ;
	}
	else if (ext.equals(_T(".fgloss")))
	{
		m_glossary_files.push_back(token) ;
		return true ;
	}
	else if (ext.is_xml())
	{
		m_xml_files.push_back(token) ;
		return true ;
	}
	else if (ext.is_txt())
	{
		m_trados_text_files.push_back(token) ;
		return true ;
	}
	else if(ext.equals(_T(".tmx")))
	{
		m_tmx_files.push_back(token) ;
		return true ;
	}
	else if(ext.equals(_T(".fprefs")))
	{
		m_prefs_file = token ;
		m_new_prefs_format = false ;
		return true ;
	}
	else if(ext.equals(_T(".fprefx")))
	{
		m_prefs_file = token ;
		m_new_prefs_format = true ;
		return true ;
	}
	return false ;
}

void commandline_options::parse_logging_level( tstring level )
{
	if (boost::to_lower_copy(level) == _T("error"))
	{
		m_logging_level = LOGGING_ERROR ;
	}
	else if (boost::to_lower_copy(level) == _T("warn"))
	{
		m_logging_level = LOGGING_WARN ;
	}
	else if (boost::to_lower_copy(level) == _T("debug"))
	{
		m_logging_level = LOGGING_DEBUG ;
	}
	else if (boost::to_lower_copy(level) == _T("verbose"))
	{
		m_logging_level = LOGGING_VERBOSE ;
	}
	else
	{
		m_logging_level = LOGGING_DEBUG ;
	}
}
