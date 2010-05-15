#include "stdafx.h"
#include "TmxImportLangsDlg.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCTMXImportLangsDlg )

BOOST_AUTO_TEST_CASE( instantiate)
{
	BOOST_CHECK_NO_THROW(CTMXImportLangsDlg()) ;
}
BOOST_AUTO_TEST_CASE( id_val)
{
	CTMXImportLangsDlg dialog ;
	BOOST_CHECK_EQUAL(IDD_SELECT_LANGS_DLG, CTMXImportLangsDlg::IDD) ;
}
BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
{
	CTMXImportLangsDlg dialog ;
	LRESULT lResult = 1 ;
	dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog"); 
	BOOST_CHECK_EQUAL(1, (int)lResult) ;
}
BOOST_AUTO_TEST_CASE( test_message_IDOK)
{
	CTMXImportLangsDlg dialog ;
	dialog.m_bModal = TRUE ;
	LRESULT lResult = 1 ;
	dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnOK"); 
	BOOST_CHECK_EQUAL( 0, (int)lResult) ;
}
BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
{
	CTMXImportLangsDlg dialog ;
	dialog.m_bModal = TRUE ;
	LRESULT lResult = 1 ;
	dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCancel"); 
	BOOST_CHECK_EQUAL( 0, (int)lResult) ;
}
BOOST_AUTO_TEST_CASE( test_message_ZERO)
{
	CTMXImportLangsDlg dialog ;
	LRESULT lResult = 1 ;
	BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL(FALSE, result) ;
	BOOST_CHECK_EQUAL(1, lResult) ;
}


BOOST_AUTO_TEST_SUITE_END()

#endif