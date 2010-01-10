#include "StdAfx.h"
#include "ScintillaEdit.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	// CTOR
	TEST( TestCScintillaEdit, constructor )
	{
		CScintillaEdit edit ;
		ASSERT_EQUALS_V( (int)edit.m_working_encoding, (int)CP_UTF8 ) ;
	}

	// set_working_encoding
	TEST( TestCScintillaEdit, set_working_encoding )
	{
		CScintillaEdit edit ;
		edit.set_working_encoding(CP_ACP) ;
		ASSERT_EQUALS_V( (int)edit.m_working_encoding, (int)CP_ACP ) ;
	}

	// PreTranslateMessage
	TEST( TestCScintillaEdit, PreTranslateMessage )
	{
		CScintillaEdit edit ;
		ASSERT_TRUE_M(!edit.PreTranslateMessage(NULL), "Should never try to handle PreTranslateMessage") ;
	}

	// set_correct_lexer
	TEST( TestCScintillaEdit, set_lexer_html )
	{
		CScintillaEdit edit ;
		edit.set_correct_lexer(_T("foo.html")) ;
		ASSERT_EQUALS_V(1, (int)edit.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(edit.m_sensing_variable[0].c_str()), "lexer:html") ;
	}

}


#endif // #ifdef UNIT_TEST
