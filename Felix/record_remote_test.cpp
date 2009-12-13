/********************************************************************
Unit tests for \ref record_remote
*********************************************************************/

#include "StdAfx.h"
#include "record_remote.h"

#include <iterator>

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_engine ;
	// A helper function:
	// Are the two records equal?
	bool are_records_equal(record_pointer lhs, record_pointer rhs)
	{
		if ( lhs->get_reliability() != rhs->get_reliability() ) 
		{ 
			return false ; 
		}
		if ( lhs->get_trans_plain() != rhs->get_trans_plain()  )
		{ 
			return false ; 
		}
		if ( lhs->get_trans_rich() != rhs->get_trans_rich() ) 
		{ 
			return false ; 
		}
		if ( lhs->get_source_plain() != rhs->get_source_plain() ) 
		{ 
			return false ; 
		}
		if ( lhs->get_source_rich() != rhs->get_source_rich() )
		{ 
			return false ; 
		}
		if ( lhs->get_context_plain() != rhs->get_context_plain() )
		{ 
			return false ; 
		}
		if ( lhs->get_context_rich() != rhs->get_context_rich() ) 
		{ 
			return false ; 
		} 
		if ( lhs->is_validated() != rhs->is_validated() ) 
		{ 
			return false ; 
		}
		if ( lhs->get_refcount() != rhs->get_refcount() ) 
		{ 
			return false ; 
		}

		translation_record::record_data_iterator pos ;
		for ( pos = lhs->data_begin() ; pos != lhs->data_end() ; ++pos )
		{
			if ( pos->second != rhs->get_item( pos->first ) ) 
			{
				return false ;
			}
		}
		for ( pos = rhs->data_begin() ; pos != rhs->data_end() ; ++pos )
		{
			if ( pos->second != lhs->get_item( pos->first ) ) 
			{
				return false ;
			}
		}

		return true ;
	}

	bool are_records_equal(record_remote &lhs, record_remote &rhs)
	{
		if ( lhs.get_reliability() != rhs.get_reliability() ) 
		{ 
			return false ; 
		}
		if ( lhs.get_trans_plain() != rhs.get_trans_plain()  )
		{ 
			return false ; 
		}
		if ( lhs.get_trans_rich() != rhs.get_trans_rich() ) 
		{ 
			return false ; 
		}
		if ( lhs.get_source_plain() != rhs.get_source_plain() ) 
		{ 
			return false ; 
		}
		if ( lhs.get_source_rich() != rhs.get_source_rich() )
		{ 
			return false ; 
		}
		if ( lhs.get_context_plain() != rhs.get_context_plain() )
		{ 
			return false ; 
		}
		if ( lhs.get_context_rich() != rhs.get_context_rich() ) 
		{ 
			return false ; 
		} 
		if ( lhs.is_validated() != rhs.is_validated() ) 
		{ 
			return false ; 
		}
		if ( lhs.get_refcount() != rhs.get_refcount() ) 
		{ 
			return false ; 
		}

		translation_record::record_data_iterator pos ;
		for ( pos = lhs.data_begin() ; pos != lhs.data_end() ; ++pos )
		{
			if ( pos->second != rhs.get_item( pos->first ) ) 
			{
				return false ;
			}
		}
		for ( pos = rhs.data_begin() ; pos != rhs.data_end() ; ++pos )
		{
			if ( pos->second != lhs.get_item( pos->first ) ) 
			{
				return false ;
			}
		}

		return true ;
	}


	TEST( test_record_remote, set_trans ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			//	void set_trans( const wstring &rich_trans ) ;
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
			memory_engine::set_record_username(me) ;

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

	TEST( test_record_remote, set_context ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			//	void set_context( const wstring &rich_context );
			wstring wstr_text = L"<b>here is a wstring text</b>" ;
			rec.set_context( wstr_text ) ;
			ASSERT_TRUE ( rec.get_context_rich() == L"<b>here is a wstring text</b>" ) ; 
			ASSERT_TRUE ( rec.get_context_plain() == L"here is a wstring text" ) ; 
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
	TEST( test_record_remote, set_source_wstring ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			//	void set_source( const wstring &rich_source ) ;	
			wstring wstr_text = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
			rec.set_source( wstr_text ) ;
			ASSERT_TRUE ( rec.get_source_rich() == wstr_text ) ; 
			ASSERT_TRUE ( rec.get_source_plain() == L"Variations on the theme..." ) ; 
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

	// construction
	//	translation_record( ) ;
	TEST( test_record_remote, constructor_default )
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			misc_wrappers::date post_date ;
			post_date.set_to_local_time() ;

			ASSERT_TRUE ( rec.get_created() <= post_date ) ; 
			ASSERT_TRUE ( rec.get_modified() <= post_date ) ; 
			ASSERT_TRUE ( rec.get_created() <= rec.get_modified() ) ; 

			// strings

			ASSERT_TRUE ( rec.get_trans_plain() == L"" ) ; 
			ASSERT_TRUE ( rec.get_source_plain() == L"" ) ; 
			ASSERT_TRUE ( rec.get_context_plain() == L"" ) ; 
			ASSERT_TRUE ( rec.get_context_rich() == L"" ) ; 
			ASSERT_TRUE ( rec.get_trans_rich() == L"" ) ; 
			ASSERT_TRUE ( rec.get_source_rich() == L"" ) ; 

			ASSERT_TRUE ( rec.get_item( L"Non_Existing" ) == L"" ) ; 

			ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 
			ASSERT_TRUE ( rec.remove_item( L"Non_Existing" ) == false ) ; 

			CStringA err_msg ;
			err_msg.Format("Expected ref count of 0 but got %d", rec.get_refcount()) ;
			ASSERT_TRUE_M( rec.get_refcount() == 0, (LPCSTR)err_msg ) ; 
			ASSERT_TRUE ( rec.is_validated() == false ) ;

			ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 
			ASSERT_TRUE ( rec.get_created() <= rec.get_modified() ) ; 

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

	//	translation_record( const translation_record &rec ) 
	TEST( test_record_remote, CopyConstructor ) 
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;

			//	void set_source( const _bstr_t &text ) ;
			wstring wstr_text = L"This is rich text" ;
			rec->set_source( wstr_text ) ;
			wstr_text = L"This is rich trans" ;
			rec->set_trans( wstr_text ) ;
			wstr_text = L"This is rich context" ;
			rec->set_context( wstr_text ) ;

			rec->set_item( L"key", wstr_text ) ;
			rec->increment_refcount( ) ;
			rec->set_reliability( 10 ) ;
			rec->set_validated_on( ) ;

			record_pointer rec2(new record_remote(rec)) ;

			ASSERT_TRUE ( are_records_equal( rec, rec2 ) ) ; 
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

	// assignment
	//	translation_record &operator =( const translation_record &rec )  
	TEST( test_record_remote, operator_assignment ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			//	void set_source( const _bstr_t &text ) ;
			wstring wstr_text = L"This is rich text" ;
			rec.set_source( wstr_text ) ;
			wstr_text = L"This is rich trans" ;
			rec.set_trans( wstr_text ) ;
			wstr_text = L"This is rich context" ;
			rec.set_context( wstr_text ) ;

			rec.set_item( L"key", wstr_text ) ;
			rec.increment_refcount( ) ;
			rec.set_reliability( 10 ) ;
			rec.set_validated_on( ) ;

			record_remote rec2(L"Felix.RemoteRecordFake") ;

			rec2 = rec ;

			ASSERT_TRUE ( are_records_equal( rec, rec2 ) ) ; 
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

	//	iterator_type data_begin() 
	TEST( test_record_remote, Testbegin ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 
			wstring wstr_text = L"val" ;
			rec.set_item( L"key", wstr_text ) ;

			translation_record::record_data_iterator pos = rec.data_begin() ;
			ASSERT_TRUE(pos == rec.data_end()) ;
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

	//	iterator_type data_end() ;
	TEST( test_record_remote, Testend ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 
			wstring wstr_text = L"val" ;
			rec.set_item( L"key", wstr_text ) ;
			ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 
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

	//	bool item_exists( const wstring &key ) const ;
	TEST( test_record_remote, item_exists ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( false == rec.item_exists( L"key" ) ) ; 

			rec.set_item( L"key", L"val" ) ;

			ASSERT_TRUE ( ! rec.item_exists( L"key" ) ) ; 

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

	//	const wstring get_item( const wstring &key ) const ;
	TEST( test_record_remote, get_item ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( L"" == rec.get_item( L"key" ) ) ; 

			rec.set_item( L"key", L"val" ) ;

			ASSERT_TRUE ( L"" == rec.get_item( L"key" ) ) ; 

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

	//	size_t get_reliability() const ;
	TEST( test_record_remote, get_reliability ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 

			rec.set_reliability( 5u ) ;

			ASSERT_TRUE ( rec.get_reliability() == 5u ) ; 

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

	//	const misc_wrappers::date &get_modified() const  ;
	TEST( test_record_remote, CreatedAndModifiedTimes ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;
			misc_wrappers::date created_date = rec.get_created() ;
			ASSERT_EQUALS( created_date, rec.get_created() ) ;
			ASSERT_TRUE ( created_date <= rec.get_modified() ) ; 
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

			rec.increment_refcount() ;

			ASSERT_TRUE ( rec.get_refcount() == 2u ) ; 

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

	//	void translation_record_tester::set_item( const wstring &key, const wstring &value ) ;
	TEST( test_record_remote, set_item ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.get_item( L"key" ) == L"" ) ; 

			rec.set_item( L"key", L"val" ) ;

			ASSERT_TRUE ( rec.get_item( L"key" ) == L"" ) ; 
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
	//	set_reliability( const wstring &reliability ) ;
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

			rec.set_reliability( 5u ) ;

			ASSERT_TRUE ( rec.get_reliability() == 5u ) ; 
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
	//	void translation_record_tester::set_validated( const wstring &validated );
	TEST( test_record_remote, set_validated ) 
	{
		try
		{

			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.is_validated() == false ) ; 

			rec.set_validated_off() ;

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

	//	void translation_record_tester::set_refcount( size_t count );
	TEST( test_record_remote, set_refcount )
	{
		try
		{		
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

			rec.set_refcount( 10u ) ;
			ASSERT_TRUE ( rec.get_refcount() == 10u ) ; 

			rec.set_refcount( 5u ) ;
			ASSERT_TRUE ( rec.get_refcount() == 5u ) ; 
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


	// validate the record
	//	bool validate() ;
	TEST( test_record_remote, validate ) 
	{
		try
		{

			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.is_validated() == false ) ; 

			rec.set_validated_off() ;
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

			rec.increment_reliability() ;
			ASSERT_TRUE ( rec.get_reliability() == 2u ) ; 

			rec.increment_reliability() ;
			ASSERT_TRUE ( rec.get_reliability() == 3u ) ; 
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

			ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

			rec.set_refcount( 10u ) ;
			ASSERT_TRUE ( rec.get_refcount() == 10u ) ; 

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

	//	void translation_record_tester::increment_refcount();
	TEST( test_record_remote, increment_refcount ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

			rec.increment_refcount() ;
			ASSERT_TRUE ( rec.get_refcount() == 1u ) ; 

			rec.increment_refcount() ;
			ASSERT_TRUE ( rec.get_refcount() == 2u ) ; 

			rec.increment_refcount() ;
			ASSERT_TRUE ( rec.get_refcount() == 3u ) ; 
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

	//	bool remove_item( const wstring &key);
	TEST( test_record_remote, remove_item ) 
	{
		try
		{
			record_remote rec(L"Felix.RemoteRecordFake") ;

			rec.set_item( L"key", L"val" ) ;
			ASSERT_TRUE ( rec.get_item( L"key" ) == L"" ) ; 
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
	TEST( test_record_remote, test_internal_copy)
	{
		try
		{
			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			rec->set_trans(L"spam") ;
			rec->set_source(L"egg") ;

			record_pointer copied(new record_remote(L"Felix.RemoteRecordFake")) ;
			copied->internal_copy(rec) ;

			CString msg ;
			msg.Format(L"spam - %s", copied->get_source_rich().c_str()) ;
			ASSERT_EQUALS_M(rec->get_source_rich(), copied->get_source_rich(), (LPCSTR)CT2A(msg)) ;
			ASSERT_EQUALS(rec->get_trans_rich(), copied->get_trans_rich()) ;
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
	TEST( test_record_remote, id_init)
	{
		try
		{
			record_pointer rec(new record_remote()) ;
			ASSERT_EQUALS(rec->get_id(), 0u) ;
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

}


#endif // #ifdef _DEBUG
