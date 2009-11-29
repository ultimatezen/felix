#pragma once

#include "record.h"

#ifdef _DEBUG

class translation_record_tester 
{
public:
	// Run the test suite for translation_record
	void run_test_suite(void);

	// construction
	//	translation_record( ) ;
	void test_constructor_default() ;
	//	translation_record( const translation_record &rec ) ;
	void test_constructor_copy() ;

	// assignment
	//	translation_record &operator =( const translation_record &rec )  ;
	void test_operator_assignment() ;

	//	iterator_type data_begin() ;
	void test_begin() ;
	//	iterator_type data_end() ;
	void test_end() ;

	//	bool item_exists( const wstring &key ) const ;
	void test_item_exists() ;
	//	const wstring get_item( const wstring &key ) const ;
	void test_get_item() ;

	// translation_record::get_source
	//	const wstring &get_source() const ;
	void test_get_source() ;

	// translation_record::get_rich_source
	//	const wstring &get_rich_source() const ;
	void test_get_rich_source() ;

	// translation_record::get_trans
	//	const wstring &get_trans() const ;
	void test_get_trans() ;

	// translation_record::get_rich_trans
	//	const wstring &get_rich_trans() const ;
	void test_get_rich_trans() ;

	// translation_record::get_context
	//	const wstring &get_context() const ;
	void test_get_context() ;
	// translation_record::get_context
	//	const wstring &get_rich_context() const  ;
	void test_get_rich_context() ;

	//	size_t get_reliability() const ;
	void test_get_reliability() ;
	//	bool test_is_validated() const  ;
	void test_is_validated() ;
	//	const misc_wrappers::date &get_created() const  ;
	void test_get_created() ;
	//	const misc_wrappers::date &get_modified() const  ;
	void test_get_modified() ;
	//	size_t get_refcount() const ;
	void test_get_refcount() ;

	//	void set_item( const wstring &key, const wstring &value ) ;
	void test_set_item() ;

	//	void set_rich_trans( const CComBSTR &text ) ;
	//	void set_rich_trans( LPCWSTR start, LPCWSTR end ) ;
	//	void set_rich_trans( const wstring &rich_trans ) ;
	void test_set_rich_trans() ;
	//	void set_rich_context( LPCWSTR start, LPCWSTR end );
	//	void set_rich_context( const CComBSTR &text ) ;
	//	void set_rich_context( const wstring &rich_context );
	void test_set_rich_context() ;
	//	void set_rich_source( const wstring &rich_source ) ;	
	//	void set_rich_source( LPCWSTR start, LPCWSTR end );
	//	void set_rich_source( const CComBSTR &text ) ;
	void test_set_rich_source() ;

	//	void set_reliability( const wstring &reliability ) ;
	//	void set_reliability( const size_t reliability ) ;
	//	void set_reliability( const CComBSTR &text ) ;
	void test_set_reliability() ;

	//	void set_validated( const wstring &validated );
	//	void set_validated( const bool validated );
	void test_set_validated() ;

	//	void set_created( const misc_wrappers::date &created );
	//	void set_created( const wstring &created );
	void test_set_created() ;

	//	void set_modified( const misc_wrappers::date &modified ) ;
	//	void set_modified( const wstring &modified ) ;
	void test_set_modified() ;

	//	void set_refcount( size_t count );
	void test_set_refcount() ;

	// set date created
	//	bool create( ) ;
	void test_create() ;
	// set the last-modifed date
	//	bool modify ( ) ;
	void test_modify() ;
	// validate the record
	//	bool validate() ;
	void test_validate() ;
	// clear the record
	//	bool clear( );
	void test_clear() ;

	//	void clear_user_strings() ;
	void test_clear_user_strings() ;
	//	void increment_reliability();
	void test_increment_reliability() ;
	//	void reset_refcount();
	void test_reset_refcount() ;
	//	void increment_refcount();
	void test_increment_refcount() ;
	//	bool is_valid_record();
	void test_is_valid_record() ;
	//	bool remove_item( const wstring &key);
	void test_remove_item() ;

private:
	// Create a time delay for testing of dates
	void create_delay(void);
	// Are the two records equal?
	bool records_are_equal(translation_record& lhs, translation_record& rhs);
};

void test_translation_record() ;

#endif // #ifdef _DEBUG
