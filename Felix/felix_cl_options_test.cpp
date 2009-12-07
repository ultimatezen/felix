/*!
@brief Unit tests for felix_cl_options
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "felix_cl_options.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST(test_parse_command_line, simple)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		ASSERT_EQUALS_V(1, (int)tokens.size()) ;
		ASSERT_EQUALS(_T("foo.txt"), tokens[0]) ;
	}
	TEST(test_parse_command_line, two)
	{
		LPCTSTR command_line_text = _T("foo.txt bar.txt") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		ASSERT_EQUALS_V(2, (int)tokens.size()) ;
		ASSERT_EQUALS(_T("foo.txt"), tokens[0]) ;
		ASSERT_EQUALS(_T("bar.txt"), tokens[1]) ;
	}
	TEST(test_parse_command_line, one_with_space)
	{
		LPCTSTR command_line_text = _T("\"c:\\program files\\foo.txt\"") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		ASSERT_EQUALS_V(1, (int)tokens.size()) ;
		ASSERT_EQUALS(_T("c:\\program files\\foo.txt"), tokens[0]) ;
	}

	TEST(test_parse_command_line, two_one_with_space)
	{
		LPCTSTR command_line_text = _T("foo.txt \"c:\\program files\\foo.txt\"") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		ASSERT_EQUALS_V(2, (int)tokens.size()) ;
		ASSERT_EQUALS(_T("foo.txt"), tokens[0]) ;
		ASSERT_EQUALS(_T("c:\\program files\\foo.txt"), tokens[1]) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// commandline_options
	//////////////////////////////////////////////////////////////////////////

	// language
	TEST(test_commandline_options, language_default)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS((int)LANG_ENGLISH, (int)options.m_language) ;
	}
	TEST(test_commandline_options, language_english)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text, LANG_ENGLISH) ;

		ASSERT_EQUALS((int)LANG_ENGLISH, (int)options.m_language) ;
	}
	TEST(test_commandline_options, language_japanese)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text, LANG_JAPANESE) ;

		ASSERT_EQUALS((int)LANG_JAPANESE, (int)options.m_language) ;
	}

	TEST(test_commandline_options, parse_language_japanese)
	{
		commandline_options options(_T("")) ;

		ASSERT_EQUALS_V((int)LANG_JAPANESE, (int)options.parse_lang(_T("Japanese"))) ;
	}
	TEST(test_commandline_options, parse_language_ja)
	{
		commandline_options options(_T("")) ;

		ASSERT_EQUALS_V((int)LANG_JAPANESE, (int)options.parse_lang(_T("ja"))) ;
	}
	TEST(test_commandline_options, parse_language_jp)
	{
		commandline_options options(_T("")) ;

		ASSERT_EQUALS_V((int)LANG_JAPANESE, (int)options.parse_lang(_T("jp"))) ;
	}
	TEST(test_commandline_options, parse_language_english)
	{
		commandline_options options(_T("")) ;

		ASSERT_EQUALS_V((int)LANG_ENGLISH, (int)options.parse_lang(_T("English"))) ;
	}
	TEST(test_commandline_options, language_switch_english)
	{
		commandline_options options(_T("-lang English")) ;

		ASSERT_EQUALS_V((int)LANG_ENGLISH, (int)options.parse_lang(_T("English"))) ;
	}
	TEST(test_commandline_options, language_switch_japanese)
	{
		commandline_options options(_T("-lang Japanese")) ;

		ASSERT_EQUALS_V((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	TEST(test_commandline_options, language_switch_ja)
	{
		commandline_options options(_T("-lang ja")) ;

		ASSERT_EQUALS_V((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	TEST(test_commandline_options, language_switch_jp)
	{
		commandline_options options(_T("-lang jp")) ;

		ASSERT_EQUALS_V((int)LANG_JAPANESE, (int)options.m_language) ;
	}

	// single file
	TEST(test_commandline_options, felix_simple)
	{
		LPCTSTR command_line_text = _T("foo.ftm") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(1, (int)options.m_tm_files.size()) ;
		ASSERT_EQUALS(_T("foo.ftm"), options.m_tm_files[0]) ;
		ASSERT_EQUALS(_T(""), options.m_prefs_file) ;
	}
	TEST(test_commandline_options, glossary_simple)
	{
		LPCTSTR command_line_text = _T("foo.fgloss") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(1, (int)options.m_glossary_files.size()) ;
		ASSERT_EQUALS(_T("foo.fgloss"), options.m_glossary_files[0]) ;
	}
	TEST(test_commandline_options, tmx_simple)
	{
		LPCTSTR command_line_text = _T("foo.tmx") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(1, (int)options.m_tmx_files.size()) ;
		ASSERT_EQUALS(_T("foo.tmx"), options.m_tmx_files[0]) ;
	}
	TEST(test_commandline_options, trados_text_simple)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(1, (int)options.m_trados_text_files.size()) ;
		ASSERT_EQUALS(_T("foo.txt"), options.m_trados_text_files[0]) ;
	}
	TEST(test_commandline_options, xml_simple)
	{
		LPCTSTR command_line_text = _T("foo.xml") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(1, (int)options.m_xml_files.size()) ;
		ASSERT_EQUALS(_T("foo.xml"), options.m_xml_files[0]) ;
	}
	TEST(test_commandline_options, fprefs_simple)
	{
		LPCTSTR command_line_text = _T("foo.fprefs") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS(_T("foo.fprefs"), options.m_prefs_file) ;
	}

	// file with language switch
	TEST(test_commandline_options, felix_simple_lang_switch_none)
	{
		LPCTSTR command_line_text = _T("foo.ftm") ;

		commandline_options options(command_line_text, LANG_JAPANESE) ;

		ASSERT_EQUALS((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	TEST(test_commandline_options, felix_simple_lang_switch_japanese)
	{
		LPCTSTR command_line_text = _T("foo.ftm -lang Japanese") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	TEST(test_commandline_options, felix_simple_lang_switch_ja_def_eng)
	{
		LPCTSTR command_line_text = _T("foo.ftm -lang Japanese") ;

		commandline_options options(command_line_text, LANG_ENGLISH) ;

		ASSERT_EQUALS((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	TEST(test_commandline_options, felix_simple_lang_switch_english)
	{
		LPCTSTR command_line_text = _T("foo.ftm -lang English") ;

		commandline_options options(command_line_text) ;

		ASSERT_EQUALS((int)LANG_ENGLISH, (int)options.m_language) ;
	}

	// two files
	TEST(test_commandline_options, trados_text_two)
	{
		LPCTSTR command_line_text = _T("foo.txt bar.txt") ;
		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(2, (int)options.m_trados_text_files.size()) ;
		ASSERT_EQUALS(_T("foo.txt"), options.m_trados_text_files[0]) ;
		ASSERT_EQUALS(_T("bar.txt"), options.m_trados_text_files[1]) ;
	}
	TEST(test_commandline_options, fgloss_and_ftm)
	{
		LPCTSTR command_line_text = _T("foo.fgloss bar.ftm") ;
		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(1, (int)options.m_tm_files.size()) ;
		ASSERT_EQUALS(_T("bar.ftm"), options.m_tm_files[0]) ;
		ASSERT_EQUALS_V(1, (int)options.m_glossary_files.size()) ;
		ASSERT_EQUALS(_T("foo.fgloss"), options.m_glossary_files[0]) ;
	}

	// space in filename(s)
	TEST(test_commandline_options, trados_text_one_with_space)
	{
		LPCTSTR command_line_text = _T("\"c:\\program files\\foo.txt\"") ;
		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(1, (int)options.m_trados_text_files.size()) ;
		ASSERT_EQUALS(_T("c:\\program files\\foo.txt"), options.m_trados_text_files[0]) ;
	}
	TEST(test_commandline_options, trados_text_two_one_with_space)
	{
		LPCTSTR command_line_text = _T("foo.txt \"c:\\program files\\foo.txt\"") ;
		commandline_options options(command_line_text) ;

		ASSERT_EQUALS_V(2, (int)options.m_trados_text_files.size()) ;
		ASSERT_EQUALS(_T("foo.txt"), options.m_trados_text_files[0]) ;
		ASSERT_EQUALS(_T("c:\\program files\\foo.txt"), options.m_trados_text_files[1]) ;
	}

}

#endif 


