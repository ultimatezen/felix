#include "StdAfx.h"
#include "numberfmt.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( CNumberFmtTestCase )

BOOST_AUTO_TEST_CASE( GetFormattedNumberInt )
{
	CNumberFmt fm ;

	int i = 1000 ;

	CString n = fm.Format( i ) ;

	BOOST_CHECK_EQUAL( n, _T("1,000" ) ) ;
}

BOOST_AUTO_TEST_CASE( GetFormattedNumberSizeT )
{
	CNumberFmt fm ;

	size_t s = 10001 ;

	CString n = fm.Format( s ) ;

	BOOST_CHECK_EQUAL( n, _T("10,001" ) ) ;
}

BOOST_AUTO_TEST_CASE( SmallNum )
{
	CNumberFmt fm ;

	size_t s = 5 ;

	CString n = fm.Format( s ) ;

	BOOST_CHECK_EQUAL( n, _T("5" ) ) ;
}

BOOST_AUTO_TEST_CASE( ReallyBigNum )
{
	CNumberFmt fm ;

	size_t s = 100100100 ;

	CString n = fm.Format( s ) ;

	BOOST_CHECK_EQUAL( n, _T("100,100,100" ) ) ;
}


	BOOST_AUTO_TEST_CASE( zero )
	{
		CNumberFmt fmt ;
		CStringA actual = fmt.Format(0) ;
		CStringA expected = "0" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( format_1_234 )
	{
		CNumberFmt fmt ;
		CStringA actual = fmt.Format(1234) ;
		CStringA expected = "1,234" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( format_1_001_001 )
	{
		CNumberFmt fmt ;
		CStringA actual = fmt.Format(1001001) ;
		CStringA expected = "1,001,001" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif