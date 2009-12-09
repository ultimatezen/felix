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

	// CViewPage

	TEST( TestCViewPage, test_message_WM_INITDIALOG)
	{
		CViewPage dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCViewPage, Rect2Points)
	{
		CViewPage dialog ;
		RECT rc ;
		rc.top = 100 ;
		rc.bottom = 200 ;
		rc.left = 100 ;
		rc.right = 200 ;
		const size_t POINTS_IN_RECT = 5;
		POINT points[POINTS_IN_RECT] = {0};
		dialog.Rect2Points(rc, points) ;
		
		ASSERT_EQUALS_V(points[0].y, 100) ;
		ASSERT_EQUALS_V(points[0].x, 100) ;

		ASSERT_EQUALS_V(points[1].y, 100) ;
		ASSERT_EQUALS_V(points[1].x, 200) ;

		ASSERT_EQUALS_V(points[2].y, 200) ;
		ASSERT_EQUALS_V(points[2].x, 200) ;

		ASSERT_EQUALS_V(points[3].y, 200) ;
		ASSERT_EQUALS_V(points[3].x, 100) ;

		ASSERT_EQUALS_V(points[4].y, 100) ;
		ASSERT_EQUALS_V(points[4].x, 100) ;

		ASSERT_EQUALS_V(points[0].x, points[4].x) ;
		ASSERT_EQUALS_V(points[0].y, points[4].y) ;
	}

}
#endif

#endif