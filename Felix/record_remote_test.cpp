/********************************************************************
Unit tests for \ref record_remote
*********************************************************************/

#include "StdAfx.h"
#include "record_remote.h"

#include <iterator>

#include "easyunit/testharness.h"

#ifdef TEST_REMOTE_MEMS

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;

	TEST( test_record_remote, set_trans ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			//	void set_trans( const wstring rich_trans ) ;
			rec.set_trans( L"here is a wstring text" ) ;
			SimpleString actual = string2string(rec.get_trans_rich()).c_str() ;
			SimpleString expected = "here is a wstring text" ;
			ASSERT_EQUALS_V(expected, actual) ;
			ASSERT_TRUE ( rec.get_trans_plain() == L"here is a wstring text" ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}
	TEST( test_record_remote, set_trans_modified_changes ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			wstring billy = L"billy" ;
			rec.set_modified_by(billy) ;
			ASSERT_EQUALS(rec.get_modified_by(), L"billy") ;

			wstring me = L"Ryan Francis Ginstrom the First but Not Last" ;
			mem_engine::set_record_username(me) ;

			rec.set_source(L"crippy croppy") ;
			ASSERT_EQUALS(rec.get_modified_by(), L"Ryan Francis Ginstrom the First but Not Last") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}


	//	bool is_validated() const  ;
	TEST( test_record_remote, is_validated ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.is_validated() == false ) ; 

			rec.set_validated_on( ) ;
			ASSERT_TRUE ( rec.is_validated() == true ) ; 

			rec.set_validated_off() ;
			ASSERT_TRUE ( rec.is_validated() == false ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}

	//	size_t get_refcount() const ;
	TEST( test_record_remote, get_refcount ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

			rec.increment_refcount() ;

			ASSERT_TRUE ( rec.get_refcount() == 1u ) ; 

		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}


	//	set_reliability( const _bstr_t &text ) ;
	//	set_reliability( const wstring reliability ) ;
	//	set_reliability( const size_t reliability ) ;
	TEST( test_record_remote, set_reliability ) 
	{
		try
		{

			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 

			rec.set_reliability( 10u ) ;

			CStringA err_msg ;
			err_msg.Format("Expected reliability of 9 but got %d", rec.get_reliability()) ;
			ASSERT_TRUE_M( rec.get_reliability() == 9u, (LPCSTR)err_msg) ; 

		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}

	}

	//	void translation_record_tester::set_validated( const bool validated );
	//	void translation_record_tester::set_validated( const wstring validated );
	TEST( test_record_remote, set_validated ) 
	{
		try
		{

			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.is_validated() == false ) ; 


			rec.set_validated_on() ;

			ASSERT_TRUE ( rec.is_validated() == true ) ; 

			rec.set_validated_off() ;

			ASSERT_TRUE ( rec.is_validated() == false ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}




	//	void translation_record_tester::increment_reliability();
	TEST( test_record_remote, increment_reliability ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 

			rec.increment_reliability() ;
			ASSERT_TRUE ( rec.get_reliability() == 1u ) ; 

		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}

	//	void translation_record_tester::reset_refcount();
	TEST( test_record_remote, reset_refcount ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			rec.set_refcount( 10u ) ;
			rec.reset_refcount() ;
			ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}


	TEST( test_record_remote, test_clone)
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			rec->set_trans(L"spam") ;
			rec->set_source(L"egg") ;

			record_pointer cloned = record_pointer(rec->clone()) ;

			CString msg ;
			msg.Format(L"spam - %s", cloned->get_source_rich().c_str()) ;
			ASSERT_EQUALS_M(rec->get_source_rich(), cloned->get_source_rich(), (LPCSTR)CT2A(msg)) ;
			ASSERT_EQUALS(rec->get_trans_rich(), cloned->get_trans_rich()) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}



	TEST( test_record_remote, set_id)
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			ASSERT_EQUALS(rec->get_id(), 0u) ;
			rec->set_id(3u) ;
			ASSERT_EQUALS(rec->get_id(), 3u) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}
	TEST( test_record_remote, copy_id)
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			rec->set_id(3u) ;

			record_remote rec2(rec);
			ASSERT_EQUALS(rec2.get_id(), 3u) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}
	// copy_from_self
	TEST( test_record_remote, copy_from_self_source)
	{
		record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->set_source(L"spam") ;
		record_pointer other(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->copy_from_self(other) ;
		SimpleString actual = (LPCSTR)CStringA(other->get_source_rich().c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST( test_record_remote, copy_from_self_modified_by)
	{
		record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->set_modified_by(L"spam") ;
		record_pointer other(new record_remote(L"Felix.RemoteRecordFake")) ;
		rec->copy_from_self(other) ;
		SimpleString actual = (LPCSTR)CStringA(other->get_modified_by().c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_TRUE(other->get_source_rich().empty()) ;
	}

}


#endif // #ifdef UNIT_TEST

#endif // #ifdef TEST_REMOTE_MEMS
