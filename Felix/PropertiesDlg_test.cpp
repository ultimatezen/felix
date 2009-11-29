#include "StdAfx.h"
#include "PropertiesDlg.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCMemoryPage, instantiate)
	{
		CMemoryPage dialog ;
		ASSERT_EQUALS_V(IDD_PROPPAGE_MEMORY, CMemoryPage::IDD) ;
	}
	TEST( TestCGeneralPage, instantiate)
	{
		CGeneralPage dialog ;
		ASSERT_EQUALS_V(IDD_PROPPAGE_GENERAL, CGeneralPage::IDD) ;
	}
	TEST( TestCAlgorithmPage, instantiate)
	{
		CAlgorithmPage dialog ;
		ASSERT_EQUALS_V(IDD_PROPPAGE_ALGORITHM, CAlgorithmPage::IDD) ;
	}
	TEST( TestCGlossaryPage, instantiate)
	{
		CGlossaryPage dialog ;
		ASSERT_EQUALS_V(IDD_PROPPAGE_GLOSSARY, CGlossaryPage::IDD) ;
	}
	TEST( TestCViewPage, instantiate)
	{
		CViewPage dialog ;
		ASSERT_EQUALS_V(IDD_PROPPAGE_VIEW, CViewPage::IDD) ;
	}
	TEST( TestCPropertiesDlg, instantiate)
	{
		CPropertiesDlg dialog ;
		ASSERT_EQUALS_V(IDD_PROPSHEET, CPropertiesDlg::IDD) ;
	}

	// message processing
	TEST( TestCMemoryPage, test_message_WM_INITDIALOG)
	{
		CMemoryPage dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCGeneralPage, test_message_WM_INITDIALOG)
	{
		CGeneralPage dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCAlgorithmPage, test_message_WM_INITDIALOG)
	{
		CAlgorithmPage dialog ;
		app_props::properties_algorithm	properties ;
		dialog.set_props(&properties) ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCGlossaryPage, test_message_WM_INITDIALOG)
	{
		CGlossaryPage dialog ;
		app_props::properties_glossary	properties ;
		dialog.set_props(&properties) ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCViewPage, test_message_WM_INITDIALOG)
	{
		CViewPage dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
}
#endif

#endif