#include "StdAfx.h"
#include "Exceptions.h"

#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	using namespace except ;

	TEST( ExceptionTest, Instantiate)
	{
		try
		{
			CException e(_T("exception")) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate CException class in test harness" ) ;
		}
		ASSERT_TRUE(true) ;
	}
	TEST( ExceptionTest, what)
	{
			CException e(_T("exception")) ;
			ASSERT_EQUALS( e.what(), CString("exception") ) ;
	}
	TEST( ExceptionTest, message)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_message(text) ;
		ASSERT_EQUALS( e.what(), text ) ;
	}
	TEST( ExceptionTest, description)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_description(text) ;
		ASSERT_EQUALS( e.description(), text ) ;
	}
	TEST( ExceptionTest, bottom_message)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_bottom_message(text) ;
		ASSERT_EQUALS( e.bottom_message(), text ) ;
	}
	TEST( ExceptionTest, add_to_message)
	{
		CException e(_T("\nexception")) ;

		CString text( "message" ) ;
		e.set_message(text) ;
		e.add_to_message(text) ;
		TRACE( e.what() ) ;
		ASSERT_EQUALS( e.what(), _T("message\r\n\r\nmessage") ) ;
	}
	TEST( ExceptionTest, add_to_message_constructor)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.add_to_message(text) ;
		TRACE( e.what() ) ;
		ASSERT_EQUALS( e.what(), _T("message\r\n\r\nexception") ) ;
	}
	TEST( ExceptionTest, add_to_description)
	{
		CException e(_T("exception")) ;

		CString text( "message" ) ;
		e.set_description(text) ;
		e.add_to_description(text) ;
		TRACE( e.description() ) ;
		ASSERT_EQUALS( e.description(), _T("message\r\n\r\nmessage") ) ;
	}

}

#endif
