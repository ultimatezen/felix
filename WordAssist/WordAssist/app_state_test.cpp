#include "stdafx.h"
#include "app_state.h"


#include <boost/test/unit_test.hpp>

static const wchar_t maru_char_array[] =
{
	0x0021	// EXCLAMATION MARK
	, 0x002E	// FULL STOP
	, 0x003F	// QUESTION MARK

	, 0x3002	// IDEOGRAPHIC FULL STOP
	, 0xFF61	// HALFWIDTH IDEOGRAPHIC FULL STOP

	, 0xFF0E	// FULLWIDTH FULL STOP
	, 0xFF1F	// FULLWIDTH QUESTION MARK
	, 0xFF01	// FULLWIDTH EXCLAMATION MARK

	, L'\0'		// end of array
} ;

BOOST_AUTO_TEST_SUITE( test_app_state )

BOOST_AUTO_TEST_CASE( initial_values_seg_chars )
{
	app_state state ;
	CString expected = CW2T(maru_char_array) ;
	CString actual(state.m_segChars) ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}

BOOST_AUTO_TEST_CASE( initial_values_manual_url )
{
	app_state state ;
	CString expected = _T("http://felix-cat.com/media/manuals/felix/5.html") ;
	CString actual(state.m_manual_url) ;
	BOOST_CHECK_EQUAL( expected, actual) ;
}
BOOST_AUTO_TEST_SUITE_END()
