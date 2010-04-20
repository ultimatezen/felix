#include "StdAfx.h"
#include "ui.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( ui_test )

	// windows_ui
	BOOST_AUTO_TEST_CASE( windows_ui_test_init )
	{
		BOOST_CHECK_NO_THROW(windows_ui()) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// file_dlg
	//////////////////////////////////////////////////////////////////////////

	// CTOR
	BOOST_AUTO_TEST_CASE( file_dlg_test_constructor )
	{
		file_dlg dlg(false, NULL) ;
		BOOST_CHECK(dlg.m_dialog.m_ofn.hwndOwner == NULL) ;
		BOOST_CHECK(dlg.m_parent_hwnd == NULL) ;
		BOOST_CHECK(dlg.m_dialog.m_ofn.lpstrFilter == NULL) ;
		BOOST_CHECK(dlg.m_dialog.m_ofn.lpstrDefExt == NULL) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST
