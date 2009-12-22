/********************************************************************
Unit tests for \ref record_local
*********************************************************************/

#include "StdAfx.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
	// A helper function:
	// Are the two records equal?
	bool records_are_equal(record_pointer lhs, record_pointer rhs)
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

	bool records_are_equal(record_local &lhs, record_local &rhs)
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

	TEST( test_record_local, set_trans ) 
	{
		record_local rec ;

		//	void set_trans( const wstring &rich_trans ) ;
		wstring wstr_text = L"here is a wstring text" ;
		rec.set_trans( wstr_text ) ;
		ASSERT_TRUE ( rec.get_trans_rich() == wstr_text ) ; 
		ASSERT_TRUE ( rec.get_trans_plain() == wstr_text ) ; 
	}

	TEST( test_record_local, set_context ) 
	{
		record_local rec ;

		//	void set_context( const wstring &rich_context );
		wstring wstr_text = L"<b>here is a wstring text</b>" ;
		rec.set_context( wstr_text ) ;
		ASSERT_TRUE ( rec.get_context_rich() == L"<b>here is a wstring text</b>" ) ; 
		ASSERT_TRUE ( rec.get_context_plain() == L"here is a wstring text" ) ; 
	}
	TEST( test_record_local, set_source_wstring ) 
	{
		record_local rec ;

		//	void set_source( const wstring &rich_source ) ;	
		wstring wstr_text = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
		rec.set_source( wstr_text ) ;
		ASSERT_TRUE ( rec.get_source_rich() == wstr_text ) ; 
		ASSERT_TRUE ( rec.get_source_plain() == L"Variations on the theme..." ) ; 
	}

	// construction
	//	translation_record( ) ;
	TEST( test_record_local, constructor_default )
	{
		record_local rec ;

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

		ASSERT_TRUE ( rec.get_refcount() == 0 ) ; 
		ASSERT_TRUE ( rec.is_validated() == false ) ;

		ASSERT_TRUE ( rec.is_valid_record() == false ) ; 

		ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 
		ASSERT_TRUE ( rec.get_created() <= rec.get_modified() ) ; 

	}

	//	translation_record( const translation_record &rec ) 
	TEST( test_record_local, CopyConstructor ) 
	{
		record_pointer rec(new record_local()) ;

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

		record_pointer rec2(new record_local(rec)) ;

		ASSERT_TRUE ( records_are_equal( rec, rec2 ) ) ; 
	}

	// assignment
	//	translation_record &operator =( const translation_record &rec )  
	TEST( test_record_local, operator_assignment ) 
	{
		record_local rec ;

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

		record_local rec2 ;

		ASSERT_TRUE( ! records_are_equal( rec2, rec ) ) ; 

		rec2 = rec ;

		ASSERT_TRUE ( records_are_equal( rec, rec2 ) ) ; 
	}

	//	iterator_type data_begin() 
	TEST( test_record_local, Testbegin ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 
		wstring wstr_text = L"val" ;
		rec.set_item( L"key", wstr_text ) ;

		translation_record::record_data_iterator pos = rec.data_begin() ;
		ASSERT_TRUE ( pos->first == L"key" ) ; 
		ASSERT_TRUE ( pos->second == L"val" ) ; 

		std::advance( pos, 1 ) ;

		ASSERT_TRUE ( pos == rec.data_end() ) ; 
	}

	//	iterator_type data_end() ;
	TEST( test_record_local, Testend ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 
		wstring wstr_text = L"val" ;
		rec.set_item( L"key", wstr_text ) ;
		ASSERT_TRUE ( rec.data_begin() != rec.data_end() ) ; 
	}

	//	bool item_exists( const wstring &key ) const ;
	TEST( test_record_local, item_exists ) 
	{
		record_local rec ;

		ASSERT_TRUE ( false == rec.item_exists( L"key" ) ) ; 

		rec.set_item( L"key", L"val" ) ;

		ASSERT_TRUE ( true == rec.item_exists( L"key" ) ) ; 

	}

	//	const wstring get_item( const wstring &key ) const ;
	TEST( test_record_local, get_item ) 
	{
		record_local rec ;

		ASSERT_TRUE ( L"" == rec.get_item( L"key" ) ) ; 

		rec.set_item( L"key", L"val" ) ;

		ASSERT_TRUE ( L"val" == rec.get_item( L"key" ) ) ; 

	}

	//	size_t get_reliability() const ;
	TEST( test_record_local, get_reliability ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 

		rec.set_reliability( 5u ) ;

		ASSERT_TRUE ( rec.get_reliability() == 5u ) ; 

	}

	//	bool is_validated() const  ;
	TEST( test_record_local, is_validated ) 
	{
		record_local rec ;

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

	//	const misc_wrappers::date &get_modified() const  ;
	TEST( test_record_local, CreatedAndModifiedTimes ) 
	{
		record_local rec ;
		misc_wrappers::date created_date = rec.get_created() ;
		ASSERT_EQUALS( created_date, rec.get_created() ) ;
		ASSERT_TRUE ( created_date <= rec.get_modified() ) ; 

	}
	//	size_t get_refcount() const ;
	TEST( test_record_local, get_refcount ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

		rec.increment_refcount() ;

		ASSERT_TRUE ( rec.get_refcount() == 1u ) ; 

		rec.increment_refcount() ;

		ASSERT_TRUE ( rec.get_refcount() == 2u ) ; 

		rec.reset_refcount() ;

		ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

	}

	//	void translation_record_tester::set_item( const wstring &key, const wstring &value ) ;
	TEST( test_record_local, set_item ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_item( L"key" ) == L"" ) ; 

		rec.set_item( L"key", L"val" ) ;

		ASSERT_TRUE ( rec.get_item( L"key" ) == L"val" ) ; 

		rec.set_item( L"key", L"" ) ;

		ASSERT_TRUE ( rec.item_exists( L"key" ) == false ) ; 
		ASSERT_TRUE ( rec.get_item( L"key" ) == L"" ) ; 

		rec.set_item( L"key", L"val" ) ;

		ASSERT_TRUE ( rec.get_item( L"key" ) == L"val" ) ; 
	}


	//	set_reliability( const _bstr_t &text ) ;
	//	set_reliability( const wstring &reliability ) ;
	//	set_reliability( const size_t reliability ) ;
	TEST( test_record_local, set_reliability	) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 

		rec.set_reliability( 10u ) ;

		ASSERT_TRUE ( rec.get_reliability() == 9u ) ; 

		rec.set_reliability( 5u ) ;

		ASSERT_TRUE ( rec.get_reliability() == 5u ) ; 

	}

	//	void translation_record_tester::set_validated( const bool validated );
	//	void translation_record_tester::set_validated( const wstring &validated );
	TEST( test_record_local, set_validated ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.is_validated() == false ) ; 

		rec.set_validated_off() ;

		ASSERT_TRUE ( rec.is_validated() == false ) ; 

		rec.set_validated_on() ;

		ASSERT_TRUE ( rec.is_validated() == true ) ; 

		rec.set_validated_off() ;

		ASSERT_TRUE ( rec.is_validated() == false ) ; 
	}

	//	void translation_record_tester::set_refcount( size_t count );
	TEST( test_record_local, set_refcount )
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

		rec.set_refcount( 10u ) ;

		ASSERT_TRUE ( rec.get_refcount() == 10u ) ; 

		rec.set_refcount( 5u ) ;

		ASSERT_TRUE ( rec.get_refcount() == 5u ) ; 
	}

	// set date created
	//	bool create( ) ;
	TEST( test_record_local, create ) 
	{
		record_local rec ;

		misc_wrappers::date first_created_date = rec.get_created() ;
		ASSERT_EQUALS( first_created_date, rec.get_created() ) ;

		Sleep( 5 ) ;

		rec.create() ;
		misc_wrappers::date second_created_date = rec.get_created() ;

		ASSERT_TRUE ( first_created_date < second_created_date ) ;
		ASSERT_EQUALS ( rec.get_created(), rec.get_modified() ) ; 
	}

	// set the last-modifed date
	//	bool modify ( ) ;
	TEST( test_record_local, modify ) 
	{
		record_local rec ;

		misc_wrappers::date first_mod_date = rec.get_modified() ;
		ASSERT_EQUALS( first_mod_date, rec.get_modified() ) ;

		Sleep( 5 ) ;

		rec.modify() ;
		misc_wrappers::date second_mod_date = rec.get_modified() ;
		ASSERT_EQUALS( second_mod_date, rec.get_modified() ) ;

		ASSERT_TRUE ( first_mod_date < second_mod_date ) ;
		ASSERT_TRUE ( rec.get_created() < rec.get_modified() ) ; 
	}

	// validate the record
	//	bool validate() ;
	TEST( test_record_local, validate ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.is_validated() == false ) ; 

		rec.set_validated_off() ;

		ASSERT_TRUE ( rec.is_validated() == false ) ; 

		rec.set_validated_on() ;

		ASSERT_TRUE ( rec.is_validated() == true ) ; 

		rec.set_validated_off() ;

		ASSERT_TRUE ( rec.is_validated() == false ) ; 
	}

	//	void translation_record_tester::clear_user_strings() ;
	TEST( test_record_local, clear_user_strings ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 

		rec.set_item( L"key", L"val" ) ;

		ASSERT_TRUE ( rec.data_begin() != rec.data_end() ) ; 

		rec.clear_user_strings() ;

		ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 

	}

	//	void translation_record_tester::increment_reliability();
	TEST( test_record_local, increment_reliability ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_reliability() == 0u ) ; 

		rec.increment_reliability() ;

		ASSERT_TRUE ( rec.get_reliability() == 1u ) ; 

		rec.increment_reliability() ;

		ASSERT_TRUE ( rec.get_reliability() == 2u ) ; 

		rec.increment_reliability() ;

		ASSERT_TRUE ( rec.get_reliability() == 3u ) ; 
	}

	//	void translation_record_tester::reset_refcount();
	TEST( test_record_local, reset_refcount ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

		rec.set_refcount( 10u ) ;

		ASSERT_TRUE ( rec.get_refcount() == 10u ) ; 

		rec.reset_refcount() ;

		ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 
	}

	//	void translation_record_tester::increment_refcount();
	TEST( test_record_local, increment_refcount ) 
	{
		record_local rec ;

		ASSERT_TRUE ( rec.get_refcount() == 0u ) ; 

		rec.increment_refcount() ;

		ASSERT_TRUE ( rec.get_refcount() == 1u ) ; 

		rec.increment_refcount() ;

		ASSERT_TRUE ( rec.get_refcount() == 2u ) ; 

		rec.increment_refcount() ;

		ASSERT_TRUE ( rec.get_refcount() == 3u ) ; 
	}


	//	bool is_valid_record();
	TEST( test_record_local, is_valid_record ) 
	{
		record_local rec ;

		ASSERT_TRUE ( false == rec.is_valid_record() ) ; 

		rec.set_trans( wstring(L"trans") ) ;

		ASSERT_TRUE ( false == rec.is_valid_record() ) ;

		rec.set_source( wstring(L"source") ) ;

		ASSERT_TRUE ( true == rec.is_valid_record() ) ; 
	}

	//	bool remove_item( const wstring &key);
	TEST( test_record_local, remove_item ) 
	{
		record_local rec ;

		rec.set_item( L"key", L"val" ) ;

		ASSERT_TRUE ( rec.get_item( L"key" ) == L"val" ) ; 

		rec.remove_item( L"key" ) ;

		ASSERT_TRUE ( rec.get_item( L"key" ) == L"" ) ; 
		ASSERT_TRUE ( rec.data_begin() == rec.data_end() ) ; 
	}

	TEST( test_record_local, clone)
	{
		record_pointer rec(new record_local()) ;
		rec->set_trans(L"spam") ;
		rec->set_source(L"egg") ;

		record_pointer cloned = record_pointer(rec->clone()) ;

		ASSERT_EQUALS(rec->get_source_rich(), cloned->get_source_rich()) ;
		ASSERT_EQUALS(rec->get_trans_rich(), cloned->get_trans_rich()) ;
	}
	TEST( test_record_local, test_internal_copy)
	{
		record_pointer rec(new record_local()) ;
		rec->set_trans(L"spam") ;
		rec->set_source(L"egg") ;

		record_pointer copied(new record_local()) ;
		copied->internal_copy(rec) ;

		ASSERT_EQUALS(rec->get_source_rich(), copied->get_source_rich()) ;
		ASSERT_EQUALS(rec->get_trans_rich(), copied->get_trans_rich()) ;
	}
	TEST( test_record_local, test_copy_constructor)
	{
		record_pointer rec(new record_local()) ;
		rec->set_trans(L"spam") ;
		rec->set_source(L"egg") ;

		record_pointer copied = rec->clone() ;

		ASSERT_EQUALS(rec->get_source_rich(), copied->get_source_rich()) ;
		ASSERT_EQUALS(rec->get_trans_rich(), copied->get_trans_rich()) ;
	}
	TEST( test_record_local, id_init)
	{
		record_pointer rec(new record_local()) ;
		ASSERT_EQUALS(rec->get_id(), 0u) ;
	}
	TEST( test_record_local, set_id)
	{
		record_pointer rec(new record_local()) ;
		ASSERT_EQUALS(rec->get_id(), 0u) ;
		rec->set_id(3u) ;
		ASSERT_EQUALS(rec->get_id(), 3u) ;
	}
	TEST( test_record_local, copy_id)
	{
		record_pointer rec(new record_local()) ;
		rec->set_id(3u) ;

		record_local rec2(rec);
		ASSERT_EQUALS(rec2.get_id(), 3u) ;
	}

	TEST( test_record, set_record_username)
	{
		set_record_username(L"spam") ;
		wstring username = get_record_username() ;
		SimpleString actual = (LPCSTR)CStringA(username.c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST( test_record_local, creator_init)
	{
		set_record_username(L"creator_init") ;

		record_pointer rec(new record_local()) ;

		SimpleString actual = (LPCSTR)CStringA(rec->get_creator().c_str()) ;
		SimpleString expected = "creator_init" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_record_local, modified_by_init)
	{
		set_record_username(L"modified_by_init") ;

		record_pointer rec(new record_local()) ;

		SimpleString actual = (LPCSTR)CStringA(rec->get_modified_by().c_str()) ;
		SimpleString expected = "modified_by_init" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST( test_record_local, create_changes_creator)
	{

		record_pointer rec(new record_local()) ;
		rec->set_creator(L"spam") ;
		SimpleString actual = (LPCSTR)CStringA(rec->get_creator().c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;

		set_record_username(L"create") ;
		rec->create() ;

		actual = (LPCSTR)CStringA(rec->get_creator().c_str()) ;
		expected = "create" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST( test_record_local, modify_changes_modified_by)
	{
		record_pointer rec(new record_local()) ;
		rec->set_modified_by(L"spam") ;
		SimpleString actual = (LPCSTR)CStringA(rec->get_modified_by().c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;

		set_record_username(L"modify") ;
		rec->modify() ;

		actual = (LPCSTR)CStringA(rec->get_modified_by().c_str()) ;
		expected = "modify" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	// copy_from_self
	TEST( test_record_local, copy_from_self_source)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(L"spam") ;
		record_pointer other(new record_local()) ;
		rec->copy_from_self(other) ;
		SimpleString actual = (LPCSTR)CStringA(other->get_source_rich().c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_record_local, copy_from_self_creator)
	{
		record_pointer rec(new record_local()) ;
		rec->set_creator(L"spam") ;
		record_pointer other(new record_local()) ;
		rec->copy_from_self(other) ;
		SimpleString actual = (LPCSTR)CStringA(other->get_creator().c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_record_local, copy_from_self_modified_by)
	{
		record_pointer rec(new record_local()) ;
		rec->set_modified_by(L"spam") ;
		record_pointer other(new record_local()) ;
		rec->copy_from_self(other) ;
		SimpleString actual = (LPCSTR)CStringA(other->get_modified_by().c_str()) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_TRUE(other->get_source_rich().empty()) ;
	}

}

#endif
