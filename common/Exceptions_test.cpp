#include "StdAfx.h"
#include "Exceptions.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( ExceptionTest )

	using namespace except ;

	BOOST_AUTO_TEST_CASE( Instantiate)
	{
		BOOST_CHECK_NO_THROW(CException(_T("exception"))) ;
	}
	BOOST_AUTO_TEST_CASE( what)
	{
		CException e(_T("exception")) ;
		BOOST_CHECK_EQUAL( e.what(), CString("exception") ) ;
	}
	BOOST_AUTO_TEST_CASE( message)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_message(text) ;
		BOOST_CHECK_EQUAL( e.what(), text ) ;
	}
	BOOST_AUTO_TEST_CASE( description)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_description(text) ;
		BOOST_CHECK_EQUAL( e.description(), text ) ;
	}
	BOOST_AUTO_TEST_CASE( bottom_message)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_bottom_message(text) ;
		BOOST_CHECK_EQUAL( e.bottom_message(), text ) ;
	}
	BOOST_AUTO_TEST_CASE( add_to_message)
	{
		CException e(_T("\nexception")) ;

		CString text( "message" ) ;
		e.set_message(text) ;
		e.add_to_message(text) ;
		BOOST_CHECK_EQUAL( e.what(), _T("message\r\n\r\nmessage") ) ;
	}
	BOOST_AUTO_TEST_CASE( add_to_message_constructor)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.add_to_message(text) ;
		BOOST_CHECK_EQUAL( e.what(), _T("message\r\n\r\nexception") ) ;
	}
	BOOST_AUTO_TEST_CASE( add_to_description)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_description(text) ;
		e.add_to_description(text) ;
		BOOST_CHECK_EQUAL( e.description(), _T("message\r\n\r\nmessage") ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( CComExceptionTest )

	using namespace except ;

	BOOST_AUTO_TEST_CASE( errnum_as_string)
	{
		CComException e(_T("exception")) ;

		tstring actual = (LPCWSTR)e.errnum_as_string() ;
		tstring expected = _T("0x80004005") ;
		BOOST_CHECK_EQUAL( actual, expected ) ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( CSWExceptionTest )

	using namespace except ;

	BOOST_AUTO_TEST_CASE( address_as_string)
	{
		CSWException e(_T("exception"), 0, NULL) ;

		tstring actual = (LPCWSTR)e.address_as_string((ULONG_PTR)0x12345678) ;
		tstring expected = _T("0x12345678") ;
		BOOST_CHECK_EQUAL( actual, expected ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
