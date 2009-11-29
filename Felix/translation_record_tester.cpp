#include "StdAfx.h"
#include <iterator>

#ifdef _DEBUG

#include ".\translation_record_tester.h"

void test_translation_record()
{
	translation_record_tester tester_obj ;
	tester_obj.run_test_suite() ;
}


// Run the test suite for translation_record
void translation_record_tester::run_test_suite(void)
{
	test_set_rich_trans() ;

	test_set_rich_context() ;

	test_set_rich_source() ;

	// construction
	test_constructor_default() ;
	test_constructor_copy() ;

	// assignment
	test_operator_assignment() ;

	//	iterator_type data_begin() ;
	test_begin() ;
	//	iterator_type data_end() ;
	test_end() ;

	test_item_exists() ;
	test_get_item() ;

	test_get_source() ;
	test_get_rich_source() ;
	test_get_trans() ;
	test_get_rich_trans() ;
	test_get_context() ;
	test_get_rich_context() ;

	test_get_reliability() ;
	test_is_validated() ;
	test_is_validated() ;
	test_get_created() ;
	test_get_modified() ;
	test_get_refcount() ;

	test_set_item() ;

	test_set_validated() ;

	test_set_created() ;

	test_set_modified() ;

	test_set_reliability() ;

	test_set_refcount() ;

	test_create() ;

	test_modify() ;

	test_validate() ;
	test_clear() ;

	test_clear_user_strings() ;
	test_increment_reliability() ;
	test_reset_refcount() ;
	test_increment_refcount() ;

	test_is_valid_record() ;
	test_remove_item() ;

	MessageBeep( MB_ICONINFORMATION ) ;
}

void translation_record_tester::test_set_rich_trans() 
{
	translation_record rec ;
	
//	void set_rich_trans( const CComBSTR &text ) ;
	CComBSTR bstr_text = L"This is rich text" ;
	rec.set_rich_trans( bstr_text ) ;
	ATLASSERT ( rec.get_rich_trans() == L"This is rich text" ) ; 
	ATLASSERT ( rec.get_trans() == L"This is rich text" ) ; 

	bstr_text = L"<b>This is rich text</b>" ;

	rec.set_rich_trans( bstr_text ) ;
	ATLASSERT ( rec.get_rich_trans() == L"<b>This is rich text</b>" ) ; 
	ATLASSERT ( rec.get_trans() == L"This is rich text" ) ; 

	bstr_text = L"Here <EM>is <U>some </U>test</EM> text with <U><FONT color=#ff0000>lots of <STRONG>formatting</STRONG></FONT></U>!" ;

	rec.set_rich_trans( bstr_text ) ;
	ATLASSERT ( rec.get_rich_trans() == L"Here <EM>is <U>some </U>test</EM> text with <U><FONT color=#ff0000>lots of <STRONG>formatting</STRONG></FONT></U>!" ) ; 
	ATLASSERT ( rec.get_trans() == L"Here is some test text with lots of formatting!" ) ; 

//	void set_rich_trans( LPCWSTR start, LPCWSTR end ) ;
	LPCWSTR cstr_text = L"here is a cstring text" ;
	LPCWSTR cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_trans( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_trans() == L"here is a cstring text" ) ; 
	ATLASSERT ( rec.get_trans() == L"here is a cstring text" ) ; 

	cstr_text = L"<b>here is a cstring text</b>" ;
	cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_trans( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_trans() == L"<b>here is a cstring text</b>" ) ; 
	ATLASSERT ( rec.get_trans() == L"here is a cstring text" ) ; 


	cstr_text = L"<TD>Here <FONT face=\"Monotype Corsiva\"><EM>is <U>some </U>test</EM> text "
		L"with</FONT> <U><FONT color=#ff0000>lots of "
		L"<STRONG>formatting</STRONG></FONT></U>!</TD>" ;
	cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_trans( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_trans() == cstr_text ) ; 
	ATLASSERT ( rec.get_trans() == L"Here is some test text with lots of formatting!" ) ; 

//	void set_rich_trans( const wstring &rich_trans ) ;
	wstring wstr_text = L"here is a wstring text" ;
	rec.set_rich_trans( wstr_text ) ;
	ATLASSERT ( rec.get_rich_trans() == wstr_text ) ; 
	ATLASSERT ( rec.get_trans() == wstr_text ) ; 

	wstr_text = L"<b>here is a wstring text</b>" ;
	rec.set_rich_trans( wstr_text ) ;
	ATLASSERT ( rec.get_rich_trans() == L"<b>here is a wstring text</b>" ) ; 
	ATLASSERT ( rec.get_trans() == L"here is a wstring text" ) ; 

	wstr_text = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
	rec.set_rich_trans( wstr_text ) ;
	ATLASSERT ( rec.get_rich_trans() == wstr_text ) ; 
	ATLASSERT ( rec.get_trans() == L"Variations on the theme..." ) ; 

}
void translation_record_tester::test_set_rich_context() 
{
	translation_record rec ;

	//	void set_rich_context( const CComBSTR &text ) ;
	CComBSTR bstr_text = L"This is rich text" ;
	rec.set_rich_context( bstr_text ) ;
	ATLASSERT ( rec.get_rich_context() == L"This is rich text" ) ; 
	ATLASSERT ( rec.get_context() == L"This is rich text" ) ; 

	bstr_text = L"<b>This is rich text</b>" ;

	rec.set_rich_context( bstr_text ) ;
	ATLASSERT ( rec.get_rich_context() == L"<b>This is rich text</b>" ) ; 
	ATLASSERT ( rec.get_context() == L"This is rich text" ) ; 

	bstr_text = L"Here <EM>is <U>some </U>test</EM> text with <U><FONT color=#ff0000>lots of <STRONG>formatting</STRONG></FONT></U>!" ;

	rec.set_rich_context( bstr_text ) ;
	ATLASSERT ( rec.get_rich_context() == L"Here <EM>is <U>some </U>test</EM> text with <U><FONT color=#ff0000>lots of <STRONG>formatting</STRONG></FONT></U>!" ) ; 
	ATLASSERT ( rec.get_context() == L"Here is some test text with lots of formatting!" ) ; 

	//	void set_rich_context( LPCWSTR start, LPCWSTR end );
	LPCWSTR cstr_text = L"here is a cstring text" ;
	LPCWSTR cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_context( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_context() == L"here is a cstring text" ) ; 
	ATLASSERT ( rec.get_context() == L"here is a cstring text" ) ; 

	cstr_text = L"<b>here is a cstring text</b>" ;
	cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_context( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_context() == L"<b>here is a cstring text</b>" ) ; 
	ATLASSERT ( rec.get_context() == L"here is a cstring text" ) ; 


	cstr_text = L"<TD>Here <FONT face=\"Monotype Corsiva\"><EM>is <U>some </U>test</EM> text "
		L"with</FONT> <U><FONT color=#ff0000>lots of "
		L"<STRONG>formatting</STRONG></FONT></U>!</TD>" ;
	cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_context( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_context() == cstr_text ) ; 
	ATLASSERT ( rec.get_context() == L"Here is some test text with lots of formatting!" ) ; 

	//	void set_rich_context( const wstring &rich_context );
	wstring wstr_text = L"here is a wstring text" ;
	rec.set_rich_context( wstr_text ) ;
	ATLASSERT ( rec.get_rich_context() == wstr_text ) ; 
	ATLASSERT ( rec.get_context() == wstr_text ) ; 

	wstr_text = L"<b>here is a wstring text</b>" ;
	rec.set_rich_context( wstr_text ) ;
	ATLASSERT ( rec.get_rich_context() == L"<b>here is a wstring text</b>" ) ; 
	ATLASSERT ( rec.get_context() == L"here is a wstring text" ) ; 

	wstr_text = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
	rec.set_rich_context( wstr_text ) ;
	ATLASSERT ( rec.get_rich_context() == wstr_text ) ; 
	ATLASSERT ( rec.get_context() == L"Variations on the theme..." ) ; 
}

void translation_record_tester::test_set_rich_source() 
{
	translation_record rec ;

	//	void set_rich_source( const CComBSTR &text ) ;
	CComBSTR bstr_text = L"This is rich text" ;
	rec.set_rich_source( bstr_text ) ;
	ATLASSERT ( rec.get_rich_source() == L"This is rich text" ) ; 
	ATLASSERT ( rec.get_source() == L"This is rich text" ) ; 

	bstr_text = L"<b>This is rich text</b>" ;

	rec.set_rich_source( bstr_text ) ;
	ATLASSERT ( rec.get_rich_source() == L"<b>This is rich text</b>" ) ; 
	ATLASSERT ( rec.get_source() == L"This is rich text" ) ; 

	bstr_text = L"Here <EM>is <U>some </U>test</EM> text with <U><FONT color=#ff0000>lots of <STRONG>formatting</STRONG></FONT></U>!" ;

	rec.set_rich_source( bstr_text ) ;
	ATLASSERT ( rec.get_rich_source() == L"Here <EM>is <U>some </U>test</EM> text with <U><FONT color=#ff0000>lots of <STRONG>formatting</STRONG></FONT></U>!" ) ; 
	ATLASSERT ( rec.get_source() == L"Here is some test text with lots of formatting!" ) ; 

	//	void set_rich_source( LPCWSTR start, LPCWSTR end );
	LPCWSTR cstr_text = L"here is a cstring text" ;
	LPCWSTR cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_source( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_source() == L"here is a cstring text" ) ; 
	ATLASSERT ( rec.get_source() == L"here is a cstring text" ) ; 

	cstr_text = L"<b>here is a cstring text</b>" ;
	cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_source( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_source() == L"<b>here is a cstring text</b>" ) ; 
	ATLASSERT ( rec.get_source() == L"here is a cstring text" ) ; 


	cstr_text = L"<TD>Here <FONT face=\"Monotype Corsiva\"><EM>is <U>some </U>test</EM> text "
		L"with</FONT> <U><FONT color=#ff0000>lots of "
		L"<STRONG>formatting</STRONG></FONT></U>!</TD>" ;
	cstr_text_end = cstr_text + wcslen( cstr_text ) ;
	rec.set_rich_source( cstr_text, cstr_text_end ) ;
	ATLASSERT ( rec.get_rich_source() == cstr_text ) ; 
	ATLASSERT ( rec.get_source() == L"Here is some test text with lots of formatting!" ) ; 

	//	void set_rich_source( const wstring &rich_source ) ;	
	wstring wstr_text = L"here is a wstring text" ;
	rec.set_rich_source( wstr_text ) ;
	ATLASSERT ( rec.get_rich_source() == wstr_text ) ; 
	ATLASSERT ( rec.get_source() == wstr_text ) ; 

	wstr_text = L"<b>here is a wstring text</b>" ;
	rec.set_rich_source( wstr_text ) ;
	ATLASSERT ( rec.get_rich_source() == L"<b>here is a wstring text</b>" ) ; 
	ATLASSERT ( rec.get_source() == L"here is a wstring text" ) ; 

	wstr_text = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
	rec.set_rich_source( wstr_text ) ;
	ATLASSERT ( rec.get_rich_source() == wstr_text ) ; 
	ATLASSERT ( rec.get_source() == L"Variations on the theme..." ) ; 
}

// construction
//	translation_record( ) ;
void translation_record_tester::test_constructor_default()
{
	misc_wrappers::date pre_date ;
	pre_date.set_to_local_time() ;

	translation_record rec ;

	misc_wrappers::date created_date( rec.get_created() ) ;
	misc_wrappers::date modified_date( rec.get_modified() ) ;

	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;
	
	ATLASSERT ( created_date <= modified_date ) ; 
	ATLASSERT ( pre_date <= created_date ) ;
	ATLASSERT ( pre_date <= modified_date ) ; 
	ATLASSERT ( created_date <= post_date ) ; 
	ATLASSERT ( modified_date <= post_date ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 

	// strings

	ATLASSERT ( rec.get_trans() == L"" ) ; 
	ATLASSERT ( rec.get_source() == L"" ) ; 
	ATLASSERT ( rec.get_context() == L"" ) ; 
	ATLASSERT ( rec.get_rich_context() == L"" ) ; 
	ATLASSERT ( rec.get_rich_trans() == L"" ) ; 
	ATLASSERT ( rec.get_rich_source() == L"" ) ; 

	ATLASSERT ( rec.get_item( L"Non_Existing" ) == L"" ) ; 
	
	ATLASSERT ( rec.data_begin() == rec.data_end() ) ; 
	ATLASSERT ( rec.remove_item( L"Non_Existing" ) == false ) ; 
	
	ATLASSERT ( rec.get_refcount() == 0 ) ; 
	ATLASSERT ( rec.is_validated() == false ) ;
	
	ATLASSERT ( rec.is_valid_record() == false ) ; 
	
	ATLASSERT ( rec.get_reliability() == 0u ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 

}
//	translation_record( const translation_record &rec ) 
void translation_record_tester::test_constructor_copy() 
{
	translation_record rec ;

	//	void set_rich_source( const CComBSTR &text ) ;
	wstring wstr_text = L"This is rich text" ;
	rec.set_rich_source( wstr_text ) ;
	wstr_text = L"This is rich trans" ;
	rec.set_rich_trans( wstr_text ) ;
	wstr_text = L"This is rich context" ;
	rec.set_rich_context( wstr_text ) ;

	rec.set_item( L"key", wstr_text ) ;
	rec.increment_refcount( ) ;
	rec.set_reliability( 10 ) ;
	rec.set_validated_on( ) ;

	translation_record rec2( rec ) ;

	ATLASSERT ( true == records_are_equal( rec, rec2 ) ) ; 
}

// assignment
//	translation_record &operator =( const translation_record &rec )  
void translation_record_tester::test_operator_assignment() 
{
	translation_record rec ;

	//	void set_rich_source( const CComBSTR &text ) ;
	wstring wstr_text = L"This is rich text" ;
	rec.set_rich_source( wstr_text ) ;
	wstr_text = L"This is rich trans" ;
	rec.set_rich_trans( wstr_text ) ;
	wstr_text = L"This is rich context" ;
	rec.set_rich_context( wstr_text ) ;

	rec.set_item( L"key", wstr_text ) ;
	rec.increment_refcount( ) ;
	rec.set_reliability( 10 ) ;
	rec.set_validated_on( ) ;

	translation_record rec2 ;

	ATLASSERT ( false == records_are_equal( rec2, rec ) ) ; 
	ATLASSERT ( false == records_are_equal( rec, rec2 ) ) ; 

	rec2 = rec ;

	ATLASSERT ( true == records_are_equal( rec, rec2 ) ) ; 
	ATLASSERT ( true == records_are_equal( rec2, rec ) ) ; 
}

//	iterator_type data_begin() 
void translation_record_tester::test_begin() 
{
	translation_record rec ;

	ATLASSERT ( rec.data_begin() == rec.data_end() ) ; 
	wstring wstr_text = L"val" ;
	rec.set_item( L"key", wstr_text ) ;

	translation_record::record_data_iterator pos = rec.data_begin() ;
	ATLASSERT ( pos->first == L"key" ) ; 
	ATLASSERT ( pos->second == L"val" ) ; 

	std::advance( pos, 1 ) ;
	
	ATLASSERT ( pos == rec.data_end() ) ; 
}
//	iterator_type data_end() ;
void translation_record_tester::test_end() 
{
	translation_record rec ;

	ATLASSERT ( rec.data_begin() == rec.data_end() ) ; 
	wstring wstr_text = L"val" ;
	rec.set_item( L"key", wstr_text ) ;
	ATLASSERT ( rec.data_begin() != rec.data_end() ) ; 
}

//	bool item_exists( const wstring &key ) const ;
void translation_record_tester::test_item_exists() 
{
	translation_record rec ;

	ATLASSERT ( false == rec.item_exists( L"key" ) ) ; 
	
	rec.set_item( L"key", L"val" ) ;

	ATLASSERT ( true == rec.item_exists( L"key" ) ) ; 

}
//	const wstring get_item( const wstring &key ) const ;
void translation_record_tester::test_get_item() 
{
	translation_record rec ;

	ATLASSERT ( L"" == rec.get_item( L"key" ) ) ; 

	rec.set_item( L"key", L"val" ) ;

	ATLASSERT ( L"val" == rec.get_item( L"key" ) ) ; 

}

// translation_record::get_source
//	const wstring &get_source() const ;
void translation_record_tester::test_get_source() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_source() == L"" ) ; 

	wstring w_text = L"here is some plain source" ;
	rec.set_rich_source( w_text ) ;
	ATLASSERT ( w_text == rec.get_source() ) ; 

	w_text = L"<b>here is some rich source</b>" ;
	rec.set_rich_source( w_text ) ;
	ATLASSERT ( L"here is some rich source" == rec.get_source() ) ; 
}

// translation_record::get_rich_source
//	const wstring &get_rich_source() const ;
void translation_record_tester::test_get_rich_source() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_rich_source() == L"" ) ; 

	wstring w_text = L"here is some plain source" ;
	rec.set_rich_source( w_text ) ;
	ATLASSERT ( w_text == rec.get_rich_source() ) ; 

	w_text = L"<b>here is some rich source</b>" ;
	rec.set_rich_source( w_text ) ;
	ATLASSERT ( w_text == rec.get_rich_source() ) ; 

}

// translation_record::get_trans
//	const wstring &get_trans() const ;
void translation_record_tester::test_get_trans() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_trans() == L"" ) ; 

	wstring w_text = L"here is some plain trans" ;
	rec.set_rich_trans( w_text ) ;
	ATLASSERT ( w_text == rec.get_trans() ) ; 

	w_text = L"<b>here is some rich trans</b>" ;
	rec.set_rich_trans( w_text ) ;
	ATLASSERT ( L"here is some rich trans" == rec.get_trans() ) ; 

}

// translation_record::get_rich_trans
//	const wstring &get_rich_trans() const ;
void translation_record_tester::test_get_rich_trans() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_rich_trans() == L"" ) ; 

	wstring w_text = L"here is some plain trans" ;
	rec.set_rich_trans( w_text ) ;
	ATLASSERT ( w_text == rec.get_rich_trans() ) ; 

	w_text = L"<b>here is some rich trans</b>" ;
	rec.set_rich_trans( w_text ) ;
	ATLASSERT ( w_text == rec.get_rich_trans() ) ; 
}

// translation_record::get_context
//	const wstring &get_context() const ;
void translation_record_tester::test_get_context() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_context() == L"" ) ; 

	wstring w_text = L"here is some plain context" ;
	rec.set_rich_context( w_text ) ;
	ATLASSERT ( w_text == rec.get_context() ) ; 

	w_text = L"<b>here is some rich context</b>" ;
	rec.set_rich_context( w_text ) ;
	ATLASSERT ( L"here is some rich context" == rec.get_context() ) ; 

}
// translation_record::get_context
//	const wstring &get_rich_context() const  ;
void translation_record_tester::test_get_rich_context() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_rich_context() == L"" ) ; 
	
	wstring w_text = L"here is some plain context" ;
	rec.set_rich_context( w_text ) ;
	ATLASSERT ( w_text == rec.get_rich_context() ) ; 

	w_text = L"<b>here is some rich context</b>" ;
	rec.set_rich_context( w_text ) ;
	ATLASSERT ( w_text == rec.get_rich_context() ) ; 
}

//	size_t get_reliability() const ;
void translation_record_tester::test_get_reliability() 
{
	translation_record rec ;
	
	ATLASSERT ( rec.get_reliability() == 0u ) ; 
	
	rec.set_reliability( 5u ) ;

	ATLASSERT ( rec.get_reliability() == 5u ) ; 
	
}
//	bool is_validated() const  ;
void translation_record_tester::test_is_validated() 
{
	translation_record rec ;

	ATLASSERT ( rec.is_validated() == false ) ; 

	rec.set_validated_on( ) ;

	ATLASSERT ( rec.is_validated() == true ) ; 

	rec.set_validated_off() ;

	ATLASSERT ( rec.is_validated() == false ) ; 

	rec.set_validated_on( ) ;

	ATLASSERT ( rec.is_validated() == true ) ; 
	
	rec.set_validated_off() ;
	
	ATLASSERT ( rec.is_validated() == false ) ; 
}

//	const misc_wrappers::date &get_created() const  ;
void translation_record_tester::test_get_created() 
{
	misc_wrappers::date pre_date ;
	pre_date.set_to_local_time() ;

	create_delay() ;

	translation_record rec ;
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
	
	create_delay() ;

	misc_wrappers::date rec_created_date = rec.get_created() ;
	ATLASSERT ( rec_created_date == rec.get_created() ) ; 
	
	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;

	ATLASSERT ( pre_date < rec_created_date ) ;
	ATLASSERT ( rec_created_date < post_date ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
}
//	const misc_wrappers::date &get_modified() const  ;
void translation_record_tester::test_get_modified() 
{
	misc_wrappers::date pre_date ;
	pre_date.set_to_local_time() ;

	create_delay() ;

	translation_record rec ;
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 

	create_delay() ;

	misc_wrappers::date rec_modified_date = rec.get_modified() ;
	ATLASSERT ( rec_modified_date == rec.get_modified() ) ; 

	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;

	ATLASSERT ( pre_date < rec_modified_date ) ;
	ATLASSERT ( rec_modified_date < post_date ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
}

//	size_t get_refcount() const ;
void translation_record_tester::test_get_refcount() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_refcount() == 0u ) ; 

	rec.increment_refcount() ;

	ATLASSERT ( rec.get_refcount() == 1u ) ; 

	rec.increment_refcount() ;
	
	ATLASSERT ( rec.get_refcount() == 2u ) ; 

	rec.reset_refcount() ;

	ATLASSERT ( rec.get_refcount() == 0u ) ; 
	
}

//	void translation_record_tester::set_item( const wstring &key, const wstring &value ) ;
void translation_record_tester::test_set_item() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_item( L"key" ) == L"" ) ; 

	rec.set_item( L"key", L"val" ) ;
	
	ATLASSERT ( rec.get_item( L"key" ) == L"val" ) ; 

	rec.set_item( L"key", L"" ) ;

	ATLASSERT ( rec.item_exists( L"key" ) == false ) ; 
	ATLASSERT ( rec.get_item( L"key" ) == L"" ) ; 

	rec.set_item( L"key", L"val" ) ;

	ATLASSERT ( rec.get_item( L"key" ) == L"val" ) ; 
}


//	set_reliability( const CComBSTR &text ) ;
//	set_reliability( const wstring &reliability ) ;
//	set_reliability( const size_t reliability ) ;
void translation_record_tester::test_set_reliability() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_reliability() == 0u ) ; 

	rec.set_reliability( 10u ) ;

	ATLASSERT ( rec.get_reliability() == 9u ) ; 

	rec.set_reliability( 5u ) ;

	ATLASSERT ( rec.get_reliability() == 5u ) ; 
	
}

//	void translation_record_tester::set_validated( const bool validated );
//	void translation_record_tester::set_validated( const wstring &validated );
void translation_record_tester::test_set_validated() 
{
	translation_record rec ;

	ATLASSERT ( rec.is_validated() == false ) ; 

	rec.set_validated_off() ;

	ATLASSERT ( rec.is_validated() == false ) ; 

	rec.set_validated_on() ;

	ATLASSERT ( rec.is_validated() == true ) ; 
	
	rec.set_validated_off() ;

	ATLASSERT ( rec.is_validated() == false ) ; 
}

//	void translation_record_tester::set_created( const misc_wrappers::date &created );
//	void translation_record_tester::set_created( const wstring &created );
void translation_record_tester::test_set_created() 
{
	misc_wrappers::date pre_date ;
	pre_date.set_to_local_time() ;

	create_delay() ;

	translation_record rec ;
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 

	create_delay() ;

	misc_wrappers::date rec_created_date = rec.get_created() ;
	ATLASSERT ( rec_created_date == rec.get_created() ) ; 

	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 

	ATLASSERT ( pre_date < rec_created_date ) ;
	ATLASSERT ( rec_created_date < post_date ) ; 

	rec.set_created( pre_date ) ;
	ATLASSERT ( pre_date == rec.get_created() ) ; 
	
	rec.set_created( post_date ) ;
	ATLASSERT ( post_date == rec.get_created() ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
	
}

//	void translation_record_tester::set_modified( const misc_wrappers::date &modified ) ;
//	void translation_record_tester::set_modified( const wstring &modified ) ;
void translation_record_tester::test_set_modified() 
{
	misc_wrappers::date pre_date ;
	pre_date.set_to_local_time() ;

	create_delay() ;

	translation_record rec ;
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 

	create_delay() ;

	misc_wrappers::date rec_modified_date = rec.get_modified() ;
	ATLASSERT ( rec_modified_date == rec.get_modified() ) ; 

	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;

	ATLASSERT ( pre_date < rec_modified_date ) ;
	ATLASSERT ( rec_modified_date < post_date ) ; 

	rec.set_modified( pre_date ) ;
	ATLASSERT ( pre_date == rec.get_modified() ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 

	rec.set_modified( post_date ) ;
	ATLASSERT ( post_date == rec.get_modified() ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
}



//	void translation_record_tester::set_refcount( size_t count );
void translation_record_tester::test_set_refcount()
{
	translation_record rec ;

	ATLASSERT ( rec.get_refcount() == 0u ) ; 

	rec.set_refcount( 10u ) ;

	ATLASSERT ( rec.get_refcount() == 10u ) ; 

	rec.set_refcount( 5u ) ;

	ATLASSERT ( rec.get_refcount() == 5u ) ; 
}

// set date created
//	bool create( ) ;
void translation_record_tester::test_create() 
{
	translation_record rec ;

	create_delay() ;

	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;

	create_delay() ;

	rec.create() ;

	misc_wrappers::date rec_created_date = rec.get_created() ;

	ATLASSERT ( post_date < rec_created_date ) ;

	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
}
// set the last-modifed date
//	bool modify ( ) ;
void translation_record_tester::test_modify() 
{
	translation_record rec ;

	create_delay() ;

	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;

	create_delay() ;

	rec.modify() ;

	misc_wrappers::date rec_created_date = rec.get_modified() ;

	ATLASSERT ( post_date < rec_created_date ) ;
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
}
// validate the record
//	bool validate() ;
void translation_record_tester::test_validate() 
{
	translation_record rec ;

	ATLASSERT ( rec.is_validated() == false ) ; 
	
	rec.set_validated_off() ;

	ATLASSERT ( rec.is_validated() == false ) ; 

	rec.set_validated_on() ;

	ATLASSERT ( rec.is_validated() == true ) ; 
	
	rec.set_validated_off() ;

	ATLASSERT ( rec.is_validated() == false ) ; 
}
// clear the record
//	bool clear( );
void translation_record_tester::test_clear() 
{
	translation_record rec, empty_rec ;

	//	void set_rich_source( const CComBSTR &text ) ;
	wstring wstr_text = L"This is rich text" ;
	rec.set_rich_source( wstr_text ) ;
	wstr_text = L"This is rich trans" ;
	rec.set_rich_trans( wstr_text ) ;
	wstr_text = L"This is rich context" ;
	rec.set_rich_context( wstr_text ) ;

	rec.set_item( L"key", wstr_text ) ;
	rec.increment_refcount( ) ;
	rec.set_reliability( 10 ) ;
	rec.set_validated_on( ) ;

	ATLASSERT ( false == records_are_equal( rec, empty_rec ) ) ; 
	
	rec.clear() ;
	rec.create() ;

	translation_record rec2 ;
	rec.set_created( rec.get_created() ) ;
	rec.set_modified( rec.get_modified() ) ;

	ATLASSERT ( records_are_equal( rec, rec2 ) ) ; 

	create_delay() ;

	misc_wrappers::date post_date ;
	post_date.set_to_local_time() ;

	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 
	ATLASSERT ( rec.get_created() < post_date ) ; 
	ATLASSERT ( rec.get_modified() < post_date ) ; 

	// strings

	ATLASSERT ( rec.get_trans() == L"" ) ; 
	ATLASSERT ( rec.get_source() == L"" ) ; 
	ATLASSERT ( rec.get_context() == L"" ) ; 
	ATLASSERT ( rec.get_rich_context() == L"" ) ; 
	ATLASSERT ( rec.get_rich_trans() == L"" ) ; 
	ATLASSERT ( rec.get_rich_source() == L"" ) ; 

	ATLASSERT ( rec.get_item( L"Non_Existing" ) == L"" ) ; 

	ATLASSERT ( rec.data_begin() == rec.data_end() ) ; 
	ATLASSERT ( rec.remove_item( L"Non_Existing" ) == false ) ; 

	ATLASSERT ( rec.get_refcount() == 0u ) ; 
	ATLASSERT ( rec.is_validated() == false ) ;

	ATLASSERT ( rec.is_valid_record() == false ) ; 

	ATLASSERT ( rec.get_reliability() == 0u ) ; 
	ATLASSERT ( rec.get_created() <= rec.get_modified() ) ; 


}

//	void translation_record_tester::clear_user_strings() ;
void translation_record_tester::test_clear_user_strings() 
{
	translation_record rec ;

	ATLASSERT ( rec.data_begin() == rec.data_end() ) ; 
	
	rec.set_item( L"key", L"val" ) ;

	ATLASSERT ( rec.data_begin() != rec.data_end() ) ; 

	rec.clear_user_strings() ;

	ATLASSERT ( rec.data_begin() == rec.data_end() ) ; 

}
//	void translation_record_tester::increment_reliability();
void translation_record_tester::test_increment_reliability() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_reliability() == 0u ) ; 

	rec.increment_reliability() ;
	
	ATLASSERT ( rec.get_reliability() == 1u ) ; 

	rec.increment_reliability() ;

	ATLASSERT ( rec.get_reliability() == 2u ) ; 

	rec.increment_reliability() ;

	ATLASSERT ( rec.get_reliability() == 3u ) ; 
}
//	void translation_record_tester::reset_refcount();
void translation_record_tester::test_reset_refcount() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_refcount() == 0u ) ; 

	rec.set_refcount( 10u ) ;

	ATLASSERT ( rec.get_refcount() == 10u ) ; 

	rec.reset_refcount() ;

	ATLASSERT ( rec.get_refcount() == 0u ) ; 
}
//	void translation_record_tester::increment_refcount();
void translation_record_tester::test_increment_refcount() 
{
	translation_record rec ;

	ATLASSERT ( rec.get_refcount() == 0u ) ; 

	rec.increment_refcount() ;

	ATLASSERT ( rec.get_refcount() == 1u ) ; 

	rec.increment_refcount() ;

	ATLASSERT ( rec.get_refcount() == 2u ) ; 

	rec.increment_refcount() ;

	ATLASSERT ( rec.get_refcount() == 3u ) ; 
}


//	bool is_valid_record();
void translation_record_tester::test_is_valid_record() 
{
	translation_record rec ;

	ATLASSERT ( false == rec.is_valid_record() ) ; 
	
	rec.set_rich_trans( wstring(L"trans") ) ;

	ATLASSERT ( false == rec.is_valid_record() ) ;

	rec.set_rich_source( wstring(L"source") ) ;

	ATLASSERT ( true == rec.is_valid_record() ) ; 
	
	rec.clear() ;

	ATLASSERT ( false == rec.is_valid_record() ) ; 
	
	rec.set_rich_source( wstring(L"source") ) ;

	ATLASSERT ( false == rec.is_valid_record() ) ; 

	rec.set_rich_context( wstring(L"set_rich_context") ) ;

	ATLASSERT ( false == rec.is_valid_record() ) ; 

	rec.increment_reliability() ;
	rec.increment_refcount() ;

	ATLASSERT ( false == rec.is_valid_record() ) ; 

	rec.set_validated_on() ;
	ATLASSERT ( false == rec.is_valid_record() ) ; 

	rec.set_rich_trans( wstring(L"trans") ) ;
	ATLASSERT ( true == rec.is_valid_record() ) ; 
}
//	bool remove_item( const wstring &key);
void translation_record_tester::test_remove_item() 
{
	translation_record rec ;

	rec.set_item( L"key", L"val" ) ;

	ATLASSERT ( rec.get_item( L"key" ) == L"val" ) ; 

	rec.remove_item( L"key" ) ;

	ATLASSERT ( rec.get_item( L"key" ) == L"" ) ; 
	ATLASSERT ( rec.data_begin() == rec.data_end() ) ; 
}


// Create a time delay for testing of dates
void translation_record_tester::create_delay(void)
{
	for ( int i = 0 ; i < 50000 ; ++i )
	{
		string delay_string = int2string( i ) ;
	}
}

// Are the two records equal?
bool translation_record_tester::records_are_equal(translation_record& lhs, translation_record& rhs)
{
	if ( lhs.get_reliability() != rhs.get_reliability() ) 
	{ 
		return false ; 
	}
	if ( lhs.get_trans() != rhs.get_trans()  )
	{ 
		return false ; 
	}
	if ( lhs.get_rich_trans() != rhs.get_rich_trans() ) 
	{ 
		return false ; 
	}
	if ( lhs.get_source() != rhs.get_source() ) 
	{ 
		return false ; 
	}
	if ( lhs.get_rich_source() != rhs.get_rich_source() )
	{ 
		return false ; 
	}
	if ( lhs.get_context() != rhs.get_context() )
	{ 
		return false ; 
	}
	if ( lhs.get_rich_context() != rhs.get_rich_context() ) 
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

#endif // #ifdef _DEBUG
