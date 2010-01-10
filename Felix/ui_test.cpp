#include "StdAfx.h"
#include "ui.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	// windows_ui
	TEST( windows_ui_test, init )
	{
		windows_ui ui ;
		ASSERT_TRUE_M(TRUE, "Should not blow up") ;
	}

	//////////////////////////////////////////////////////////////////////////
	// file_dlg
	//////////////////////////////////////////////////////////////////////////

	// CTOR
	TEST( file_dlg_test, constructor )
	{
		file_dlg dlg(false, NULL) ;
		ASSERT_EQUALS_M(dlg.m_dialog.m_ofn.hwndOwner, NULL, "Parent should be null") ;
		ASSERT_EQUALS_M(dlg.m_parent_hwnd, NULL, "Parent should be null") ;
		ASSERT_EQUALS_M(dlg.m_dialog.m_ofn.lpstrFilter, NULL, "lpstrFilter should be null") ;
		ASSERT_EQUALS_M(dlg.m_dialog.m_ofn.lpstrDefExt, NULL, "lpstrDefExt should be null") ;
	}

}


#endif // #ifdef UNIT_TEST
