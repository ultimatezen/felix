#include "StdAfx.h"
#include "MinViewDlg.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestMinViewDlg, instantiate)
	{
		CMinViewDlg dialog ;
		ASSERT_EQUALS_V(IDD_MIN_VIEW_DLG, CMinViewDlg::IDD) ;
	}
	// CExportDialog message processing
	TEST( TestMinViewDlg, test_message_WM_INITDIALOG)
	{
		CMinViewDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( "OnInitDialog", SimpleString(dialog.m_sensing_variable[0].c_str())); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestMinViewDlg, test_message_ZERO)
	{
		CMinViewDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}

}

#endif