#include "StdAfx.h"
#include "NagDialog.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

#ifdef UNIT_TEST
namespace easyunit
{
	TEST( TestCNagDialog, instantiate)
	{
		CNagDialog dialog ;
		ASSERT_EQUALS_V(IDD_NAG_BOX, CNagDialog::IDD) ;
	}

	// CExportDialog message processing
	TEST( TestCNagDialog, test_message_WM_INITDIALOG)
	{
		CNagDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCNagDialog, test_message_IDOK)
	{
		CNagDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCNagDialog, test_message_IDCANCEL)
	{
		CNagDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCNagDialog, test_message_ZERO)
	{
		CNagDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
}
#endif
#endif