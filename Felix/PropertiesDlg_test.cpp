#include "StdAfx.h"
#include "PropertiesDlg.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCPropertiesDlg )

	BOOST_AUTO_TEST_CASE(instantiate)
	{
		CPropertiesDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_PROPSHEET, CPropertiesDlg::IDD) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestCMemoryPage )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CMemoryPage dialog ;
		BOOST_CHECK_EQUAL(IDD_PROPPAGE_MEMORY, CMemoryPage::IDD) ;
	}
	// message processing

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( TestCGeneralPage )
	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CGeneralPage dialog ;
		BOOST_CHECK_EQUAL(IDD_PROPPAGE_GENERAL, CGeneralPage::IDD) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestCAlgorithmPage )
	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CAlgorithmPage dialog ;
		BOOST_CHECK_EQUAL(IDD_PROPPAGE_ALGORITHM, CAlgorithmPage::IDD) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestCGlossaryPage )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CGlossaryPage dialog ;
		BOOST_CHECK_EQUAL(IDD_PROPPAGE_GLOSSARY, CGlossaryPage::IDD) ;
	}

	// CViewPage
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestCViewPage )
	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CViewPage dialog ;
		BOOST_CHECK_EQUAL(IDD_PROPPAGE_VIEW, CViewPage::IDD) ;
	}

	BOOST_AUTO_TEST_CASE( Rect2Points)
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
		
		BOOST_CHECK_EQUAL(points[0].y, 100) ;
		BOOST_CHECK_EQUAL(points[0].x, 100) ;

		BOOST_CHECK_EQUAL(points[1].y, 100) ;
		BOOST_CHECK_EQUAL(points[1].x, 200) ;

		BOOST_CHECK_EQUAL(points[2].y, 200) ;
		BOOST_CHECK_EQUAL(points[2].x, 200) ;

		BOOST_CHECK_EQUAL(points[3].y, 200) ;
		BOOST_CHECK_EQUAL(points[3].x, 100) ;

		BOOST_CHECK_EQUAL(points[4].y, 100) ;
		BOOST_CHECK_EQUAL(points[4].x, 100) ;

		BOOST_CHECK_EQUAL(points[0].x, points[4].x) ;
		BOOST_CHECK_EQUAL(points[0].y, points[4].y) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif