/*!
@brief Unit tests for felix_cl_options
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "felix_cl_options.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_parse_command_line )


	BOOST_AUTO_TEST_CASE( simple)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		BOOST_CHECK_EQUAL(1u, tokens.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.txt"), tokens[0]) ;
	}
	BOOST_AUTO_TEST_CASE( two)
	{
		LPCTSTR command_line_text = _T("foo.txt bar.txt") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		BOOST_CHECK_EQUAL(2u, tokens.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.txt"), tokens[0]) ;
		BOOST_CHECK_EQUAL(_T("bar.txt"), tokens[1]) ;
	}
	BOOST_AUTO_TEST_CASE( one_with_space)
	{
		LPCTSTR command_line_text = _T("\"c:\\program files\\foo.txt\"") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		BOOST_CHECK_EQUAL(1u, tokens.size()) ;
		BOOST_CHECK_EQUAL(_T("c:\\program files\\foo.txt"), tokens[0]) ;
	}

	BOOST_AUTO_TEST_CASE( two_one_with_space)
	{
		LPCTSTR command_line_text = _T("foo.txt \"c:\\program files\\foo.txt\"") ;
		std::vector<tstring> tokens ;

		parse_command_line(command_line_text, tokens) ;

		BOOST_CHECK_EQUAL(2u, tokens.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.txt"), tokens[0]) ;
		BOOST_CHECK_EQUAL(_T("c:\\program files\\foo.txt"), tokens[1]) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	// commandline_options
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( test_commandline_options )
	// language
	BOOST_AUTO_TEST_CASE( language_default)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL((int)LANG_ENGLISH, (int)options.m_language) ;
	}
	BOOST_AUTO_TEST_CASE( language_english)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text, LANG_ENGLISH) ;

		BOOST_CHECK_EQUAL((int)LANG_ENGLISH, (int)options.m_language) ;
	}
	BOOST_AUTO_TEST_CASE( language_japanese)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text, LANG_JAPANESE) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.m_language) ;
	}

	BOOST_AUTO_TEST_CASE( parse_language_japanese)
	{
		commandline_options options(_T("")) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.parse_lang(_T("Japanese"))) ;
	}
	BOOST_AUTO_TEST_CASE( parse_language_ja)
	{
		commandline_options options(_T("")) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.parse_lang(_T("ja"))) ;
	}
	BOOST_AUTO_TEST_CASE( parse_language_jp)
	{
		commandline_options options(_T("")) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.parse_lang(_T("jp"))) ;
	}
	BOOST_AUTO_TEST_CASE( parse_language_english)
	{
		commandline_options options(_T("")) ;

		BOOST_CHECK_EQUAL((int)LANG_ENGLISH, (int)options.parse_lang(_T("English"))) ;
	}
	BOOST_AUTO_TEST_CASE( language_switch_english)
	{
		commandline_options options(_T("-lang English")) ;

		BOOST_CHECK_EQUAL((int)LANG_ENGLISH, (int)options.parse_lang(_T("English"))) ;
	}
	BOOST_AUTO_TEST_CASE( language_switch_japanese)
	{
		commandline_options options(_T("-lang Japanese")) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	BOOST_AUTO_TEST_CASE( language_switch_ja)
	{
		commandline_options options(_T("-lang ja")) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	BOOST_AUTO_TEST_CASE( language_switch_jp)
	{
		commandline_options options(_T("-lang jp")) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.m_language) ;
	}

	// single file
	BOOST_AUTO_TEST_CASE( felix_simple)
	{
		LPCTSTR command_line_text = _T("foo.ftm") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(1u, options.m_tm_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.ftm"), options.m_tm_files[0]) ;
		BOOST_CHECK_EQUAL(_T(""), options.m_prefs_file) ;
	}
	BOOST_AUTO_TEST_CASE( glossary_simple)
	{
		LPCTSTR command_line_text = _T("foo.fgloss") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(1u, options.m_glossary_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.fgloss"), options.m_glossary_files[0]) ;
	}
	BOOST_AUTO_TEST_CASE( tmx_simple)
	{
		LPCTSTR command_line_text = _T("foo.tmx") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(1u, options.m_tmx_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.tmx"), options.m_tmx_files[0]) ;
	}
	BOOST_AUTO_TEST_CASE( trados_text_simple)
	{
		LPCTSTR command_line_text = _T("foo.txt") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(1u, options.m_trados_text_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.txt"), options.m_trados_text_files[0]) ;
	}
	BOOST_AUTO_TEST_CASE( xml_simple)
	{
		LPCTSTR command_line_text = _T("foo.xml") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(1u, options.m_xml_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.xml"), options.m_xml_files[0]) ;
	}
	BOOST_AUTO_TEST_CASE( fprefs_simple)
	{
		LPCTSTR command_line_text = _T("foo.fprefs") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(_T("foo.fprefs"), options.m_prefs_file) ;
	}

	// file with language switch
	BOOST_AUTO_TEST_CASE( felix_simple_lang_switch_none)
	{
		LPCTSTR command_line_text = _T("foo.ftm") ;

		commandline_options options(command_line_text, LANG_JAPANESE) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	BOOST_AUTO_TEST_CASE( felix_simple_lang_switch_japanese)
	{
		LPCTSTR command_line_text = _T("foo.ftm -lang Japanese") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	BOOST_AUTO_TEST_CASE( felix_simple_lang_switch_ja_def_eng)
	{
		LPCTSTR command_line_text = _T("foo.ftm -lang Japanese") ;

		commandline_options options(command_line_text, LANG_ENGLISH) ;

		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, (int)options.m_language) ;
	}
	BOOST_AUTO_TEST_CASE( felix_simple_lang_switch_english)
	{
		LPCTSTR command_line_text = _T("foo.ftm -lang English") ;

		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL((int)LANG_ENGLISH, (int)options.m_language) ;
	}

	// two files
	BOOST_AUTO_TEST_CASE( trados_text_two)
	{
		LPCTSTR command_line_text = _T("foo.txt bar.txt") ;
		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(2u, options.m_trados_text_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.txt"), options.m_trados_text_files[0]) ;
		BOOST_CHECK_EQUAL(_T("bar.txt"), options.m_trados_text_files[1]) ;
	}
	BOOST_AUTO_TEST_CASE( fgloss_and_ftm)
	{
		LPCTSTR command_line_text = _T("foo.fgloss bar.ftm") ;
		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(1u, options.m_tm_files.size()) ;
		BOOST_CHECK_EQUAL(_T("bar.ftm"), options.m_tm_files[0]) ;
		BOOST_CHECK_EQUAL(1u, options.m_glossary_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.fgloss"), options.m_glossary_files[0]) ;
	}

	// space in filename(s)
	BOOST_AUTO_TEST_CASE( trados_text_one_with_space)
	{
		LPCTSTR command_line_text = _T("\"c:\\program files\\foo.txt\"") ;
		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(1u, options.m_trados_text_files.size()) ;
		BOOST_CHECK_EQUAL(_T("c:\\program files\\foo.txt"), options.m_trados_text_files[0]) ;
	}
	BOOST_AUTO_TEST_CASE( trados_text_two_one_with_space)
	{
		LPCTSTR command_line_text = _T("foo.txt \"c:\\program files\\foo.txt\"") ;
		commandline_options options(command_line_text) ;

		BOOST_CHECK_EQUAL(2u, options.m_trados_text_files.size()) ;
		BOOST_CHECK_EQUAL(_T("foo.txt"), options.m_trados_text_files[0]) ;
		BOOST_CHECK_EQUAL(_T("c:\\program files\\foo.txt"), options.m_trados_text_files[1]) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif 


