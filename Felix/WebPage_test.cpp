#include "stdafx.h"
#include "WebPage.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCWebPage )

BOOST_AUTO_TEST_CASE( instantiate)
{
	BOOST_CHECK_NO_THROW(html::CWebPage()) ;
}
BOOST_AUTO_TEST_CASE( TestGetLastError)
{
	html::CWebPage page ;
	page.m_strError = _T("error test") ;
	BOOST_CHECK_EQUAL(_T("error test"), page.GetLastError()) ;
}
BOOST_AUTO_TEST_CASE( TestGetSystemErrorMessage)
{
	CString msg = html::GetSystemErrorMessage(ERROR_FILE_NOT_FOUND) ;
	wstring actual = CT2W(msg) ;
	boost::trim(actual) ;
	wstring expected = L"The system cannot find the file specified." ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( TestGetNextToken)
{
	CString source = _T("foo bar") ;
	CString delims = _T(" \t\r\n") ;
	CString next_token = html::GetNextToken(source, delims) ;
	string actual = CT2A(next_token) ;
	string expected = "foo" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}

BOOST_AUTO_TEST_SUITE_END()

#endif