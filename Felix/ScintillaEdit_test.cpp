#include "StdAfx.h"
#include "ScintillaEdit.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCScintillaEdit )


	// CTOR
	BOOST_AUTO_TEST_CASE( constructor )
	{
		CScintillaEdit edit ;
		BOOST_CHECK_EQUAL( (int)edit.m_working_encoding, (int)CP_UTF8 ) ;
	}

	// working encoding
	BOOST_AUTO_TEST_CASE( set_working_encoding )
	{
		CScintillaEdit edit ;
		edit.set_working_encoding(CP_ACP) ;
		BOOST_CHECK_EQUAL( (int)edit.get_working_encoding(), (int)CP_ACP ) ;
	}

	// PreTranslateMessage
	BOOST_AUTO_TEST_CASE( PreTranslateMessage )
	{
		CScintillaEdit edit ;
		BOOST_CHECK(!edit.PreTranslateMessage(NULL)) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// set_correct_lexer
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( set_lexer_html )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.html")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:html") ;
	}
	BOOST_AUTO_TEST_CASE( set_lexer_xml )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.xml")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:xml") ;
	}
	BOOST_AUTO_TEST_CASE( set_lexer_php )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.php")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:php") ;
	}
	BOOST_AUTO_TEST_CASE( set_lexer_py )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.py")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:python") ;
	}
	BOOST_AUTO_TEST_CASE( set_lexer_css )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.css")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:css") ;
	}
	BOOST_AUTO_TEST_CASE( set_lexer_cpp )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.cpp")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:cpp") ;
	}
	BOOST_AUTO_TEST_CASE( set_lexer_perl )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.pl")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:perl") ;
	}
	BOOST_AUTO_TEST_CASE( set_lexer_js )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.js")) ;
		BOOST_CHECK_EQUAL(1u, edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(edit.m_sensing_variable[0], "lexer:js") ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST
