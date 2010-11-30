/********************************************************************
Unit tests for \ref record_local
*********************************************************************/

#include "StdAfx.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_record_local )


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

	string ws2ss(const wstring text)
	{
		return string(string2string(text).c_str()) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// initialize
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( init_created) 
	{
		misc_wrappers::date local_time ;
		local_time.set_to_local_time() ;
		record_local rec ;

		BOOST_CHECK_EQUAL(rec.get_created().wYear, local_time.wYear) ;
		BOOST_CHECK_EQUAL(rec.get_created().wMonth, local_time.wMonth) ;
		BOOST_CHECK_EQUAL(rec.get_created().wDay, local_time.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( init_modified) 
	{
		misc_wrappers::date local_time ;
		local_time.set_to_local_time() ;
		record_local rec ;

		BOOST_CHECK_EQUAL(rec.get_modified().wYear, local_time.wYear) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wMonth, local_time.wMonth) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wDay, local_time.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( init_creator) 
	{
		const wstring username = L"Sam" ;
		mem_engine::set_record_username(username) ;
		record_local rec ;

		BOOST_CHECK_EQUAL(ws2ss(username), ws2ss(rec.get_creator())) ;
	}
	BOOST_AUTO_TEST_CASE( init_modified_by) 
	{
		const wstring username = L"Sam" ;
		mem_engine::set_record_username(username) ;
		record_local rec ;

		BOOST_CHECK_EQUAL(ws2ss(username), ws2ss(rec.get_modified_by())) ;
	}


	//////////////////////////////////////////////////////////////////////////
	// get/set text
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( set_trans ) 
	{
		record_local rec ;

		//	void set_trans( const wstring rich_trans ) ;
		wstring wstr_text = L"here is a wstring text" ;
		rec.set_trans( wstr_text ) ;
		BOOST_CHECK_EQUAL ( rec.get_trans_rich(), wstr_text ) ; 
		BOOST_CHECK_EQUAL ( rec.get_trans_plain(), wstr_text ) ; 
	}

	BOOST_AUTO_TEST_CASE( set_context ) 
	{
		record_local rec ;

		//	void set_context( const wstring rich_context );
		wstring wstr_text = L"<b>here is a wstring text</b>" ;
		rec.set_context( wstr_text ) ;
		BOOST_CHECK_EQUAL ( rec.get_context_rich(), L"<b>here is a wstring text</b>" ) ; 
		BOOST_CHECK_EQUAL ( rec.get_context_plain(), L"here is a wstring text" ) ; 
	}
	BOOST_AUTO_TEST_CASE( set_source_wstring ) 
	{
		record_local rec ;

		//	void set_source( const wstring rich_source ) ;	
		wstring wstr_text = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
		rec.set_source( wstr_text ) ;
		BOOST_CHECK_EQUAL ( rec.get_source_rich(), wstr_text ) ; 
		BOOST_CHECK_EQUAL ( rec.get_source_plain(), L"Variations on the theme..." ) ; 
	}

	//////////////////////////////////////////////////////////////////////////
	// construction
	//////////////////////////////////////////////////////////////////////////

	//	translation_record( ) ;
	BOOST_AUTO_TEST_CASE( constructor_default )
	{
		record_local rec ;

		misc_wrappers::date post_date ;
		post_date.set_to_local_time() ;

		BOOST_CHECK ( rec.get_created() <= post_date ) ; 
		BOOST_CHECK ( rec.get_modified() <= post_date ) ; 
		BOOST_CHECK ( rec.get_created() <= rec.get_modified() ) ; 

		// strings

		BOOST_CHECK ( rec.get_trans_plain() == L"" ) ; 
		BOOST_CHECK ( rec.get_source_plain() == L"" ) ; 
		BOOST_CHECK ( rec.get_context_plain() == L"" ) ; 
		BOOST_CHECK ( rec.get_context_rich() == L"" ) ; 
		BOOST_CHECK ( rec.get_trans_rich() == L"" ) ; 
		BOOST_CHECK ( rec.get_source_rich() == L"" ) ; 

		BOOST_CHECK ( rec.get_item( L"Non_Existing" ) == L"" ) ; 

		BOOST_CHECK ( rec.data_begin() == rec.data_end() ) ; 
		BOOST_CHECK ( rec.remove_item( L"Non_Existing" ) == false ) ; 

		BOOST_CHECK ( rec.get_refcount() == 0 ) ; 
		BOOST_CHECK ( rec.is_validated() == false ) ;

		BOOST_CHECK ( rec.is_valid_record() == false ) ; 

		BOOST_CHECK ( rec.get_reliability() == 0u ) ; 
		BOOST_CHECK ( rec.get_created() <= rec.get_modified() ) ; 

	}

	//	translation_record( const translation_record &rec ) 
	BOOST_AUTO_TEST_CASE( CopyConstructor ) 
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

		BOOST_CHECK ( records_are_equal( rec, rec2 ) ) ; 
	}

	// assignment
	//	translation_record &operator =( const translation_record &rec )  
	BOOST_AUTO_TEST_CASE( operator_assignment ) 
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

		BOOST_CHECK( ! records_are_equal( rec2, rec ) ) ; 

		rec2 = rec ;

		BOOST_CHECK ( records_are_equal( rec, rec2 ) ) ; 
	}

	//	iterator_type data_begin() 
	BOOST_AUTO_TEST_CASE( Testbegin ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.data_begin() == rec.data_end() ) ; 
		wstring wstr_text = L"val" ;
		rec.set_item( L"key", wstr_text ) ;

		translation_record::record_data_iterator pos = rec.data_begin() ;
		BOOST_CHECK ( pos->first == L"key" ) ; 
		BOOST_CHECK ( pos->second == L"val" ) ; 

		std::advance( pos, 1 ) ;

		BOOST_CHECK ( pos == rec.data_end() ) ; 
	}

	//	iterator_type data_end() ;
	BOOST_AUTO_TEST_CASE( Testend ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.data_begin() == rec.data_end() ) ; 
		wstring wstr_text = L"val" ;
		rec.set_item( L"key", wstr_text ) ;
		BOOST_CHECK ( rec.data_begin() != rec.data_end() ) ; 
	}

	//	bool item_exists( const wstring key ) const ;
	BOOST_AUTO_TEST_CASE( item_exists ) 
	{
		record_local rec ;

		BOOST_CHECK ( false == rec.item_exists( L"key" ) ) ; 

		rec.set_item( L"key", L"val" ) ;

		BOOST_CHECK ( true == rec.item_exists( L"key" ) ) ; 
	}

	//	const wstring get_item( const wstring key ) const ;
	BOOST_AUTO_TEST_CASE( get_item ) 
	{
		record_local rec ;

		BOOST_CHECK ( L"" == rec.get_item( L"key" ) ) ; 

		rec.set_item( L"key", L"val" ) ;

		BOOST_CHECK ( L"val" == rec.get_item( L"key" ) ) ; 
	}

	//	size_t get_reliability() const ;
	BOOST_AUTO_TEST_CASE( get_reliability ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_reliability() == 0u ) ; 

		rec.set_reliability( 5u ) ;

		BOOST_CHECK ( rec.get_reliability() == 5u ) ; 

	}

	//	bool is_validated() const  ;
	BOOST_AUTO_TEST_CASE( is_validated ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 

		rec.set_validated_on( ) ;

		BOOST_CHECK ( rec.is_validated() == true ) ; 

		rec.set_validated_off() ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 

		rec.set_validated_on( ) ;

		BOOST_CHECK ( rec.is_validated() == true ) ; 

		rec.set_validated_off() ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 
	}

	//	const misc_wrappers::date &get_modified() const  ;
	BOOST_AUTO_TEST_CASE( CreatedAndModifiedTimes ) 
	{
		record_local rec ;
		misc_wrappers::date created_date = rec.get_created() ;
		BOOST_CHECK( created_date == rec.get_created() ) ;
		BOOST_CHECK ( created_date <= rec.get_modified() ) ; 

	}
	//	size_t get_refcount() const ;
	BOOST_AUTO_TEST_CASE( get_refcount ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_refcount() == 0u ) ; 

		rec.increment_refcount() ;

		BOOST_CHECK ( rec.get_refcount() == 1u ) ; 

		rec.increment_refcount() ;

		BOOST_CHECK ( rec.get_refcount() == 2u ) ; 

		rec.reset_refcount() ;

		BOOST_CHECK ( rec.get_refcount() == 0u ) ; 

	}

	//	void translation_record_tester::set_item( const wstring key, const wstring value ) ;
	BOOST_AUTO_TEST_CASE( set_item ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_item( L"key" ) == L"" ) ; 

		rec.set_item( L"key", L"val" ) ;

		BOOST_CHECK ( rec.get_item( L"key" ) == L"val" ) ; 

		rec.set_item( L"key", L"" ) ;

		BOOST_CHECK ( rec.item_exists( L"key" ) == false ) ; 
		BOOST_CHECK ( rec.get_item( L"key" ) == L"" ) ; 

		rec.set_item( L"key", L"val" ) ;

		BOOST_CHECK ( rec.get_item( L"key" ) == L"val" ) ; 
	}


	//	set_reliability( const _bstr_t &text ) ;
	//	set_reliability( const wstring reliability ) ;
	//	set_reliability( const size_t reliability ) ;
	BOOST_AUTO_TEST_CASE( set_reliability	) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_reliability() == 0u ) ; 

		rec.set_reliability( 10u ) ;

		BOOST_CHECK ( rec.get_reliability() == 9u ) ; 

		rec.set_reliability( 5u ) ;

		BOOST_CHECK ( rec.get_reliability() == 5u ) ; 
	}

	//	void translation_record_tester::set_validated( const bool validated );
	//	void translation_record_tester::set_validated( const wstring validated );
	BOOST_AUTO_TEST_CASE( set_validated ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 

		rec.set_validated_off() ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 

		rec.set_validated_on() ;

		BOOST_CHECK ( rec.is_validated() == true ) ; 

		rec.set_validated_off() ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 
	}

	//	void translation_record_tester::set_refcount( size_t count );
	BOOST_AUTO_TEST_CASE( set_refcount )
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_refcount() == 0u ) ; 

		rec.set_refcount( 10u ) ;

		BOOST_CHECK ( rec.get_refcount() == 10u ) ; 

		rec.set_refcount( 5u ) ;

		BOOST_CHECK ( rec.get_refcount() == 5u ) ; 
	}

	//////////////////////////////////////////////////////////////////////////
	/// dates
	//////////////////////////////////////////////////////////////////////////

	// set date created
	//	bool create( ) ;
	BOOST_AUTO_TEST_CASE( create ) 
	{
		record_local rec ;

		misc_wrappers::date first_created_date = rec.get_created() ;
		BOOST_CHECK( first_created_date == rec.get_created() ) ;

		rec.create() ;
		misc_wrappers::date second_created_date = rec.get_created() ;

		BOOST_CHECK ( first_created_date <= second_created_date ) ;
		BOOST_CHECK ( rec.get_created() == rec.get_modified() ) ; 
	}

	// set the last-modifed date
	//	bool modify ( ) ;
	BOOST_AUTO_TEST_CASE( modify ) 
	{
		record_local rec ;

		misc_wrappers::date first_mod_date = rec.get_modified() ;
		BOOST_CHECK( first_mod_date == rec.get_modified() ) ;

		rec.modify() ;
		misc_wrappers::date second_mod_date = rec.get_modified() ;
		BOOST_CHECK( second_mod_date == rec.get_modified() ) ;

		BOOST_CHECK ( first_mod_date <= second_mod_date ) ;
		BOOST_CHECK ( rec.get_created() <= rec.get_modified() ) ; 
	}

	BOOST_AUTO_TEST_CASE( set_modified_good)
	{
		const wstring datestr = L"2009/10/01 5:13:01" ;
		record_local rec ;
		rec.set_modified(datestr) ;
		
		BOOST_CHECK_EQUAL(rec.get_modified().wYear, 2009) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wMonth, 10) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wDay, 1) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wHour, 5) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wMinute, 13) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wSecond, 1) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wMilliseconds, 0) ;

	}
	BOOST_AUTO_TEST_CASE( set_modified_bad)
	{
		const wstring datestr = L"01/10/2000 5/13/01" ;
		misc_wrappers::date now ;
		record_local rec ;
		rec.set_modified(datestr) ;

		now.set_to_local_time() ;

		BOOST_CHECK_EQUAL(rec.get_modified().wYear, now.wYear) ;
	}

	BOOST_AUTO_TEST_CASE( set_created_good)
	{
		const wstring datestr = L"2009/10/01 5:13:01" ;
		record_local rec ;
		rec.set_created(datestr) ;

		BOOST_CHECK_EQUAL(rec.get_created().wYear, 2009) ;
		BOOST_CHECK_EQUAL(rec.get_created().wMonth, 10) ;
		BOOST_CHECK_EQUAL(rec.get_created().wDay, 1) ;
		BOOST_CHECK_EQUAL(rec.get_created().wHour, 5) ;
		BOOST_CHECK_EQUAL(rec.get_created().wMinute, 13) ;
		BOOST_CHECK_EQUAL(rec.get_created().wSecond, 1) ;
		BOOST_CHECK_EQUAL(rec.get_created().wMilliseconds, 0) ;

	}
	BOOST_AUTO_TEST_CASE( set_created_bad)
	{
		const wstring datestr = L"01/10/2000 5/13/01" ;
		misc_wrappers::date now ;
		record_local rec ;
		rec.set_created(datestr) ;

		now.set_to_local_time() ;

		BOOST_CHECK_EQUAL(rec.get_created().wYear, now.wYear) ;
		BOOST_CHECK_EQUAL(rec.get_created().wMonth, now.wMonth) ;
	}

	// reset created
	BOOST_AUTO_TEST_CASE( set_modified_before_created)
	{
		const wstring datestr = L"2005/10/1 12:13:01" ;
		record_local rec ;
		misc_wrappers::date created_date ;
		created_date.set_to_local_time() ;
		rec.set_created(created_date) ;
		misc_wrappers::date modified_date(datestr) ;
		rec.set_modified(modified_date) ;

		BOOST_CHECK_EQUAL((int)misc_wrappers::Delta(rec.get_created(), modified_date), 0) ;
		BOOST_CHECK_EQUAL(rec.get_created().wYear, 2005) ;
	}
	BOOST_AUTO_TEST_CASE( set_created_after_modified)
	{
		const wstring datestr = L"2005/10/1 12:13:01" ;
		record_local rec ;
		rec.set_modified(datestr) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wYear, 2005) ;

		misc_wrappers::date created_date ;
		created_date.set_to_local_time() ;
		rec.set_created(created_date) ;

		BOOST_CHECK_EQUAL((int)misc_wrappers::Delta(rec.get_modified(), created_date), 0) ;
		BOOST_CHECK(rec.get_modified().wYear > 2005) ;
	}

	BOOST_AUTO_TEST_CASE( set_modified_after_created)
	{
		const wstring created_datestr = L"1999/10/1 12:13:01" ;
		const wstring modified_datestr = L"2005/10/1 12:13:01" ;

		record_local rec ;
		rec.set_created(created_datestr) ;
		rec.set_modified(modified_datestr) ;

		BOOST_CHECK_EQUAL(rec.get_created().wYear, 1999) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wYear, 2005) ;
	}
	BOOST_AUTO_TEST_CASE( set_created_before_modified)
	{
		const wstring created_datestr = L"1999/10/1 12:13:01" ;
		const wstring modified_datestr = L"2005/10/1 12:13:01" ;

		record_local rec ;
		rec.set_modified(modified_datestr) ;
		rec.set_created(created_datestr) ;

		BOOST_CHECK_EQUAL(rec.get_created().wYear, 1999) ;
		BOOST_CHECK_EQUAL(rec.get_modified().wYear, 2005) ;
	}

	// validate the record
	//	bool validate() ;
	BOOST_AUTO_TEST_CASE( validate ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 

		rec.set_validated_off() ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 

		rec.set_validated_on() ;

		BOOST_CHECK ( rec.is_validated() == true ) ; 

		rec.set_validated_off() ;

		BOOST_CHECK ( rec.is_validated() == false ) ; 
	}

	//	void translation_record_tester::clear_user_strings() ;
	BOOST_AUTO_TEST_CASE( clear_user_strings ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.data_begin() == rec.data_end() ) ; 

		rec.set_item( L"key", L"val" ) ;

		BOOST_CHECK ( rec.data_begin() != rec.data_end() ) ; 

		rec.clear_user_strings() ;

		BOOST_CHECK ( rec.data_begin() == rec.data_end() ) ; 

	}

	//	void translation_record_tester::increment_reliability();
	BOOST_AUTO_TEST_CASE( increment_reliability ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_reliability() == 0u ) ; 

		rec.increment_reliability() ;

		BOOST_CHECK ( rec.get_reliability() == 1u ) ; 

		rec.increment_reliability() ;

		BOOST_CHECK ( rec.get_reliability() == 2u ) ; 

		rec.increment_reliability() ;

		BOOST_CHECK ( rec.get_reliability() == 3u ) ; 
	}

	//	void translation_record_tester::reset_refcount();
	BOOST_AUTO_TEST_CASE( reset_refcount ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_refcount() == 0u ) ; 

		rec.set_refcount( 10u ) ;

		BOOST_CHECK ( rec.get_refcount() == 10u ) ; 

		rec.reset_refcount() ;

		BOOST_CHECK ( rec.get_refcount() == 0u ) ; 
	}

	//	void translation_record_tester::increment_refcount();
	BOOST_AUTO_TEST_CASE( increment_refcount ) 
	{
		record_local rec ;

		BOOST_CHECK ( rec.get_refcount() == 0u ) ; 

		rec.increment_refcount() ;

		BOOST_CHECK ( rec.get_refcount() == 1u ) ; 

		rec.increment_refcount() ;

		BOOST_CHECK ( rec.get_refcount() == 2u ) ; 

		rec.increment_refcount() ;

		BOOST_CHECK ( rec.get_refcount() == 3u ) ; 
	}


	//	bool is_valid_record();
	BOOST_AUTO_TEST_CASE( is_valid_record_true) 
	{
		record_local rec ;

		rec.set_trans(L"trans") ;
		rec.set_source(L"source") ;

		BOOST_CHECK ( true == rec.is_valid_record() ) ; 
	}
	BOOST_AUTO_TEST_CASE( is_valid_record_false) 
	{
		record_local rec ;

		BOOST_CHECK (! rec.is_valid_record()) ; 

		rec.set_trans(L"trans") ;

		BOOST_CHECK (! rec.is_valid_record()) ;
	}

	//	bool remove_item( const wstring key);
	BOOST_AUTO_TEST_CASE( remove_item ) 
	{
		record_local rec ;

		rec.set_item( L"key", L"val" ) ;

		BOOST_CHECK ( rec.get_item( L"key" ) == L"val" ) ; 

		rec.remove_item( L"key" ) ;

		BOOST_CHECK ( rec.get_item( L"key" ) == L"" ) ; 
		BOOST_CHECK ( rec.data_begin() == rec.data_end() ) ; 
	}

	BOOST_AUTO_TEST_CASE( clone)
	{
		record_pointer rec(new record_local()) ;
		rec->set_trans(L"spam") ;
		rec->set_source(L"egg") ;

		record_pointer cloned = record_pointer(rec->clone()) ;

		BOOST_CHECK_EQUAL(rec->get_source_rich(), cloned->get_source_rich()) ;
		BOOST_CHECK_EQUAL(rec->get_trans_rich(), cloned->get_trans_rich()) ;
	}
	BOOST_AUTO_TEST_CASE( test_internal_copy)
	{
		record_pointer rec(new record_local()) ;
		rec->set_trans(L"spam") ;
		rec->set_source(L"egg") ;

		record_pointer copied(new record_local()) ;
		copied->internal_copy(rec) ;

		BOOST_CHECK_EQUAL(rec->get_source_rich(), copied->get_source_rich()) ;
		BOOST_CHECK_EQUAL(rec->get_trans_rich(), copied->get_trans_rich()) ;
	}
	BOOST_AUTO_TEST_CASE( test_copy_constructor)
	{
		record_pointer rec(new record_local()) ;
		rec->set_trans(L"spam") ;
		rec->set_source(L"egg") ;

		record_pointer copied = rec->clone() ;

		BOOST_CHECK_EQUAL(rec->get_source_rich(), copied->get_source_rich()) ;
		BOOST_CHECK_EQUAL(rec->get_trans_rich(), copied->get_trans_rich()) ;
	}
	BOOST_AUTO_TEST_CASE( id_init)
	{
		record_pointer rec(new record_local()) ;
		BOOST_CHECK_EQUAL(rec->get_id(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE( set_id)
	{
		record_pointer rec(new record_local()) ;
		BOOST_CHECK_EQUAL(rec->get_id(), 0u) ;
		rec->set_id(3u) ;
		BOOST_CHECK_EQUAL(rec->get_id(), 3u) ;
	}
	BOOST_AUTO_TEST_CASE( copy_id)
	{
		record_pointer rec(new record_local()) ;
		rec->set_id(3u) ;

		record_local rec2(rec);
		BOOST_CHECK_EQUAL(rec2.get_id(), 3u) ;
	}

	BOOST_AUTO_TEST_CASE(test_set_record_username)
	{
		set_record_username(L"spam") ;
		wstring username = get_record_username() ;
		string actual = (LPCSTR)CStringA(username.c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( creator_init)
	{
		set_record_username(L"creator_init") ;

		record_pointer rec(new record_local()) ;

		string actual = (LPCSTR)CStringA(rec->get_creator().c_str()) ;
		string expected = "creator_init" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( test_modified_by_init)
	{
		set_record_username(L"modified_by_init") ;

		record_pointer rec(new record_local()) ;

		string actual = (LPCSTR)CStringA(rec->get_modified_by().c_str()) ;
		string expected = "modified_by_init" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( create_changes_creator)
	{

		record_pointer rec(new record_local()) ;
		rec->set_creator(L"spam") ;
		string actual = (LPCSTR)CStringA(rec->get_creator().c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		set_record_username(L"create") ;
		rec->create() ;

		actual = (LPCSTR)CStringA(rec->get_creator().c_str()) ;
		expected = "create" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( modify_changes_modified_by)
	{
		record_pointer rec(new record_local()) ;
		rec->set_modified_by(L"spam") ;
		string actual = (LPCSTR)CStringA(rec->get_modified_by().c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		set_record_username(L"modify") ;
		rec->modify() ;

		expected = "modify" ;
		BOOST_CHECK_EQUAL(expected, ws2ss(rec->get_modified_by())) ;
	}

	// copy_from_self
	BOOST_AUTO_TEST_CASE( copy_from_self_source)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(L"spam") ;
		record_pointer other(new record_local()) ;
		rec->copy_from_self(other) ;
		string actual = (LPCSTR)CStringA(other->get_source_rich().c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( copy_from_self_creator)
	{
		record_pointer rec(new record_local()) ;
		rec->set_creator(L"spam") ;
		record_pointer other(new record_local()) ;
		rec->copy_from_self(other) ;
		string actual = (LPCSTR)CStringA(other->get_creator().c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( copy_from_self_modified_by)
	{
		record_pointer rec(new record_local()) ;
		rec->set_modified_by(L"spam") ;
		record_pointer other(new record_local()) ;
		rec->copy_from_self(other) ;
		string actual = (LPCSTR)CStringA(other->get_modified_by().c_str()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(other->get_source_rich().empty()) ;
	}

BOOST_AUTO_TEST_SUITE_END()