#include "stdafx.h"
#include "app_state.h"
#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
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
	TEST( app_stateTest, initial_values_seg_chars )
	{
		try
		{
			app_state state ;
			CString expected = CW2T(maru_char_array) ;
			CString actual(state.m_segChars) ;
			ASSERT_EQUALS_M( expected, actual, "Seg chars don\'t match expectation") ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate app_props class in harness" ) ;
		}
	}
	TEST( app_stateTest, initial_values_manual_url )
	{
		try
		{
			app_state state ;
			CString expected = _T("http://felix-cat.com/media/manuals/felix/5.html") ;
			CString actual(state.m_manual_url) ;
			ASSERT_EQUALS_M( expected, actual, "Manual URL doesn\'t match expectation") ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate app_props class in harness" ) ;
		}
	}
}
#endif
