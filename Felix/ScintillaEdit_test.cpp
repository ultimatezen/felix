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

	// set_working_encoding
	BOOST_AUTO_TEST_CASE( set_working_encoding )
	{
		CScintillaEdit edit ;
		edit.set_working_encoding(CP_ACP) ;
		BOOST_CHECK_EQUAL( (int)edit.m_working_encoding, (int)CP_ACP ) ;
	}

	// PreTranslateMessage
	BOOST_AUTO_TEST_CASE( PreTranslateMessage )
	{
		CScintillaEdit edit ;
		BOOST_CHECK(!edit.PreTranslateMessage(NULL)) ;
	}

	// set_correct_lexer
	BOOST_AUTO_TEST_CASE( set_lexer_html )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.html")) ;
		BOOST_CHECK_EQUAL(1, (int)edit.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(edit.m_sensing_variable[0].c_str()), "lexer:html") ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST
