/********************************************************************
Unit tests for \ref record_remote
*********************************************************************/

#include "StdAfx.h"
#include "record_remote.h"

#include <iterator>

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_record_remote )


	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( set_trans ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			//	void set_trans( const wstring rich_trans ) ;
			rec.set_trans( L"here is a wstring text" ) ;
			string actual = string2string(rec.get_trans_rich()).c_str() ;
			string expected = "here is a wstring text" ;
			BOOST_CHECK_EQUAL(expected, actual) ;
			BOOST_CHECK ( rec.get_trans_plain() == L"here is a wstring text" ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}
	BOOST_AUTO_TEST_CASE( set_trans_modified_changes ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			wstring billy = L"billy" ;
			rec.set_modified_by(billy) ;
			BOOST_CHECK_EQUAL(rec.get_modified_by(), L"billy") ;

			wstring me = L"Ryan Francis Ginstrom the First but Not Last" ;
			mem_engine::set_record_username(me) ;

			rec.set_source(L"crippy croppy") ;
			BOOST_CHECK_EQUAL(rec.get_modified_by(), L"Ryan Francis Ginstrom the First but Not Last") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}


	//	bool is_validated() const  ;
	BOOST_AUTO_TEST_CASE( is_validated ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			BOOST_CHECK ( rec.is_validated() == false ) ; 

			rec.set_validated_on( ) ;
			BOOST_CHECK ( rec.is_validated() == true ) ; 

			rec.set_validated_off() ;
			BOOST_CHECK ( rec.is_validated() == false ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}

	//	size_t get_refcount() const ;
	BOOST_AUTO_TEST_CASE( get_refcount ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			BOOST_CHECK ( rec.get_refcount() == 0u ) ; 

			rec.increment_refcount() ;

			BOOST_CHECK ( rec.get_refcount() == 1u ) ; 

		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}


	//	set_reliability( const _bstr_t &text ) ;
	//	set_reliability( const wstring reliability ) ;
	//	set_reliability( const size_t reliability ) ;
	BOOST_AUTO_TEST_CASE( set_reliability ) 
	{
		try
		{

			record_remote rec(L"Felix.RemoteRecordFake") ;

			BOOST_CHECK ( rec.get_reliability() == 0u ) ; 

			rec.set_reliability( 10u ) ;

			BOOST_CHECK_EQUAL( rec.get_reliability(), 9u) ; 

		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}

	}

	//	void translation_record_tester::set_validated( const bool validated );
	//	void translation_record_tester::set_validated( const wstring validated );
	BOOST_AUTO_TEST_CASE( set_validated ) 
	{
		try
		{

			record_remote rec(L"Felix.RemoteRecordFake") ;

			BOOST_CHECK ( rec.is_validated() == false ) ; 


			rec.set_validated_on() ;

			BOOST_CHECK ( rec.is_validated() == true ) ; 

			rec.set_validated_off() ;

			BOOST_CHECK ( rec.is_validated() == false ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}




	//	void translation_record_tester::increment_reliability();
	BOOST_AUTO_TEST_CASE( increment_reliability ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			BOOST_CHECK ( rec.get_reliability() == 0u ) ; 

			rec.increment_reliability() ;
			BOOST_CHECK ( rec.get_reliability() == 1u ) ; 

		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}

	//	void translation_record_tester::reset_refcount();
	BOOST_AUTO_TEST_CASE( reset_refcount ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			rec.set_refcount( 10u ) ;
			rec.reset_refcount() ;
			BOOST_CHECK ( rec.get_refcount() == 0u ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}


	BOOST_AUTO_TEST_CASE( test_clone)
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			rec->set_trans(L"spam") ;
			rec->set_source(L"egg") ;

			record_pointer cloned = record_pointer(rec->clone()) ;

			BOOST_CHECK_EQUAL(rec->get_source_rich(), cloned->get_source_rich()) ;
			BOOST_CHECK_EQUAL(rec->get_trans_rich(), cloned->get_trans_rich()) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}



	BOOST_AUTO_TEST_CASE( set_id)
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			BOOST_CHECK_EQUAL(rec->get_id(), 0u) ;
			rec->set_id(3u) ;
			BOOST_CHECK_EQUAL(rec->get_id(), 3u) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}
	BOOST_AUTO_TEST_CASE( copy_id)
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			rec->set_id(3u) ;

			record_remote rec2(rec);
			BOOST_CHECK_EQUAL(rec2.get_id(), 3u) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}
	// copy_from_self
	BOOST_AUTO_TEST_CASE( copy_from_self_source)
	{
		record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->set_source(L"spam") ;
		record_pointer other(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->copy_from_self(other) ;
		string actual = (LPCSTR)CStringA(other->get_source_rich().c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( copy_from_self_modified_by)
	{
		record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->set_modified_by(L"spam") ;
		record_pointer other(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->copy_from_self(other) ;
		string actual = (LPCSTR)CStringA(other->get_modified_by().c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(other->get_source_rich().empty()) ;
	}

BOOST_AUTO_TEST_SUITE_END()
