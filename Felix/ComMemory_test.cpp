#include "StdAfx.h"
#include "App2.h"
#include "ComMemory.h"
#include "query.h"
#include "memory_local.h"
#include "miscwrappers.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

// test_properties_loaded_history
BOOST_AUTO_TEST_SUITE( TestComMemory)

	using namespace mem_engine;

	typedef CComPtr< IRecords > records_ptr ;

	typedef CComObject< CComMemory > mem_obj ;
	typedef CComPtr< mem_obj > mem_ptr ;

	record_pointer make_record(string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		return rec ;
	}

	BOOST_AUTO_TEST_CASE( test_instantiate )
	{
		mem_ptr mem ;
		HRESULT hr = mem_obj::CreateInstance( &mem ) ;

		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( mem ) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Records
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE(test_records_init_empty)
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;

		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		records_ptr records ;
		HRESULT hr = mem->get_Records(&records) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;

		long count = 10 ;
		records->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 0) ;
	}	
	BOOST_AUTO_TEST_CASE(test_records_count_2)
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;

		memory_pointer memory(new memory_local) ;
		memory->add_record(make_record("source a", "source a")) ;
		memory->add_record(make_record("source b", "source b")) ;

		mem->set_memory(memory) ;

		records_ptr records ;
		mem->get_Records(&records) ;

		long count = 10 ;
		records->get_Count(&count) ;
		BOOST_CHECK_EQUAL(count, 2) ;
	}	

	//////////////////////////////////////////////////////////////////////////
	// MemoryInfo
	//////////////////////////////////////////////////////////////////////////

	// creator
	BOOST_AUTO_TEST_CASE( get_creator )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		memory->get_memory_info()->set_creator(L"Charlie Chaplin") ;
		mem->set_memory(memory) ;

		CComBSTR creator ;
		mem->get_Creator(&creator) ;
		BOOST_CHECK_EQUAL("Charlie Chaplin", string(CW2A(creator))) ;
	}
	BOOST_AUTO_TEST_CASE( set_creator )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR creator = L"Mr. Splashypants";
		mem->put_Creator(creator) ;
		CComBSTR result ;
		mem->get_Creator(&result) ;
		BOOST_CHECK_EQUAL("Mr. Splashypants", string(CW2A(result))) ;
	}

	// modified_by
	BOOST_AUTO_TEST_CASE( get_modified_by )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		memory->get_memory_info()->set_modified_by(L"Charlie Chaplin") ;
		mem->set_memory(memory) ;

		CComBSTR result ;
		mem->get_ModifiedBy(&result) ;
		BOOST_CHECK_EQUAL("Charlie Chaplin", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( set_modified_by )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR creator = L"Mr. Splashypants";
		mem->put_ModifiedBy(creator) ;
		CComBSTR result ;
		mem->get_ModifiedBy(&result) ;
		BOOST_CHECK_EQUAL("Mr. Splashypants", string(CW2A(result))) ;
	}

	// field
	BOOST_AUTO_TEST_CASE( get_field_empty )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR result ;
		mem->get_Field(&result) ;
		BOOST_CHECK_EQUAL("", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( get_field )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		memory->get_memory_info()->set_field(L"Electronics") ;
		mem->set_memory(memory) ;

		CComBSTR result ;
		mem->get_Field(&result) ;
		BOOST_CHECK_EQUAL("Electronics", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( set_field )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR creator = L"IT";
		mem->put_Field(creator) ;
		CComBSTR result ;
		mem->get_Field(&result) ;
		BOOST_CHECK_EQUAL("IT", string(CW2A(result))) ;
	}

	// client
	BOOST_AUTO_TEST_CASE( get_client )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		memory->get_memory_info()->set_client(L"Charlie Chaplin") ;
		mem->set_memory(memory) ;

		CComBSTR result ;
		mem->get_Client(&result) ;
		BOOST_CHECK_EQUAL("Charlie Chaplin", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( set_client )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR creator = L"Bob";
		mem->put_Client(creator) ;
		CComBSTR result ;
		mem->get_Client(&result) ;
		BOOST_CHECK_EQUAL("Bob", string(CW2A(result))) ;
	}

	// source_lang
	BOOST_AUTO_TEST_CASE( get_source_language )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		memory->get_memory_info()->set_source_language(L"German") ;
		mem->set_memory(memory) ;

		CComBSTR result ;
		mem->get_SourceLanguage(&result) ;
		BOOST_CHECK_EQUAL("German", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( set_source_language )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR creator = L"French";
		mem->put_SourceLanguage(creator) ;
		CComBSTR result ;
		mem->get_SourceLanguage(&result) ;
		BOOST_CHECK_EQUAL("French", string(CW2A(result))) ;
	}

	// target_lang
	BOOST_AUTO_TEST_CASE( get_target_language )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		memory->get_memory_info()->set_target_language(L"Spanish") ;
		mem->set_memory(memory) ;

		CComBSTR result ;
		mem->get_TargetLanguage(&result) ;
		BOOST_CHECK_EQUAL("Spanish", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( set_target_language )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		CComBSTR creator = L"Chinese";
		mem->put_TargetLanguage(creator) ;
		CComBSTR result ;
		mem->get_TargetLanguage(&result) ;
		BOOST_CHECK_EQUAL("Chinese", string(CW2A(result))) ;
	}

	// created_on
	BOOST_AUTO_TEST_CASE( get_created_on )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		misc_wrappers::date created ;
		created.set_to_local_time() ;
		created.wYear = 1988 ;
		created.wMonth = 11 ;
		created.wDay = 12 ;
		memory->get_memory_info()->set_created_on(created.get_date_time_string()) ;
		mem->set_memory(memory) ;

		DATE result(0);
		mem->get_CreatedOn(&result) ;

		SYSTEMTIME memtime ;
		::VariantTimeToSystemTime(result, &memtime) ;
		BOOST_CHECK_EQUAL(1988, memtime.wYear) ;
		BOOST_CHECK_EQUAL(11, memtime.wMonth) ;
		BOOST_CHECK_EQUAL(12, memtime.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( set_created_on )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		misc_wrappers::date created ;
		created.set_to_local_time() ;
		created.wYear = 2003 ;
		created.wMonth = 6 ;
		created.wDay = 10 ;

		DATE setting(0);
		::SystemTimeToVariantTime(&created, &setting) ;

		mem->put_CreatedOn(setting) ;

		misc_wrappers::date result(memory->get_memory_info()->get_created_on()) ;

		BOOST_CHECK_EQUAL(2003, result.wYear) ;
		BOOST_CHECK_EQUAL(6, result.wMonth) ;
		BOOST_CHECK_EQUAL(10, result.wDay) ;
	}

	// created_on
	BOOST_AUTO_TEST_CASE( get_ModifiedOn )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		misc_wrappers::date modified ;
		modified.set_to_local_time() ;
		modified.wYear = 1988 ;
		modified.wMonth = 11 ;
		modified.wDay = 12 ;
		memory->get_memory_info()->set_modified_on(modified.get_date_time_string()) ;
		mem->set_memory(memory) ;

		DATE result(0);
		mem->get_ModifiedOn(&result) ;

		SYSTEMTIME memtime ;
		::VariantTimeToSystemTime(result, &memtime) ;
		BOOST_CHECK_EQUAL(1988, memtime.wYear) ;
		BOOST_CHECK_EQUAL(11, memtime.wMonth) ;
		BOOST_CHECK_EQUAL(12, memtime.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( set_ModifiedOn )
	{
		mem_ptr mem ;
		mem_obj::CreateInstance( &mem ) ;
		memory_pointer memory(new memory_local) ;
		mem->set_memory(memory) ;

		misc_wrappers::date modified ;
		modified.set_to_local_time() ;
		modified.wYear = 2015 ;
		modified.wMonth = 6 ;
		modified.wDay = 10 ;

		DATE setting(0);
		::SystemTimeToVariantTime(&modified, &setting) ;

		mem->put_ModifiedOn(setting) ;

		misc_wrappers::date result(memory->get_memory_info()->get_modified_on()) ;

		BOOST_CHECK_EQUAL(2015, result.wYear) ;
		BOOST_CHECK_EQUAL(6, result.wMonth) ;
		BOOST_CHECK_EQUAL(10, result.wDay) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif