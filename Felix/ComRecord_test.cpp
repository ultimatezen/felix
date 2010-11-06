#include "StdAfx.h"
#include "ComRecord.h"
#include "record_local.h"
#include "miscwrappers.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestComRecord )

	using namespace mem_engine ;

	typedef CComPtr< CComObject< CRecord > > com_rec_ptr ;

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		com_rec_ptr rec ;
		HRESULT hr = CComObject< CRecord >::CreateInstance( &rec ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( rec ) ;
	}
	// trans
	BOOST_AUTO_TEST_CASE( trans_empty )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR result ;
		rec->get_Trans(&result) ;
		BOOST_CHECK_EQUAL("", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( trans )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR trans = L"spam" ;
		rec->put_Trans(trans) ;
		CComBSTR result ;
		rec->get_Trans(&result) ;
		BOOST_CHECK_EQUAL("spam", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( trans_plain )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR trans = L"<b><font color=\"red\">spam &amp; eggs</font></b>" ;
		rec->put_Trans(trans) ;
		CComBSTR result ;
		rec->get_PlainTrans(&result) ;
		BOOST_CHECK_EQUAL("spam & eggs", string(CW2A(result))) ;
	}

	// source
	BOOST_AUTO_TEST_CASE( source )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR source = L"spam" ;
		rec->put_Source(source) ;
		CComBSTR result ;
		rec->get_Source(&result) ;
		BOOST_CHECK_EQUAL("spam", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( source_rich )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR source = L"<b>spam</b>" ;
		rec->put_Source(source) ;
		CComBSTR result ;
		rec->get_Source(&result) ;
		BOOST_CHECK_EQUAL("<b>spam</b>", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( source_plain )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR source = L"<b>spam</b>" ;
		rec->put_Source(source) ;
		CComBSTR result ;
		rec->get_PlainSource(&result) ;
		BOOST_CHECK_EQUAL("spam", string(CW2A(result))) ;
	}

	BOOST_AUTO_TEST_CASE( source_empty )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR result ;
		rec->get_Source(&result) ;
		BOOST_CHECK_EQUAL("", string(CW2A(result))) ;
	}
	// context
	BOOST_AUTO_TEST_CASE( context )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR context = L"context" ;
		rec->put_Context(context) ;
		CComBSTR result ;
		rec->get_Context(&result) ;
		BOOST_CHECK_EQUAL("context", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( context_plain )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR context = L"<p>context</p><br/>" ;
		rec->put_Context(context) ;
		CComBSTR result ;
		rec->get_PlainContext(&result) ;
		BOOST_CHECK_EQUAL("context", string(CW2A(result))) ;
	}
	// created by
	BOOST_AUTO_TEST_CASE( created_by )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR creator = L"creator" ;
		rec->put_CreatedBy(creator) ;
		CComBSTR result ;
		rec->get_CreatedBy(&result) ;
		BOOST_CHECK_EQUAL("creator", string(CW2A(result))) ;
	}
	// modified by
	BOOST_AUTO_TEST_CASE( modified_by )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR modified_by = L"modified_by" ;
		rec->put_ModifiedBy(modified_by) ;
		CComBSTR result ;
		rec->get_ModifiedBy(&result) ;
		BOOST_CHECK_EQUAL("modified_by", string(CW2A(result))) ;
	}
	BOOST_AUTO_TEST_CASE( modified_by_changes_record )
	{
		record_pointer record(new record_local) ;
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->set_record(record) ;
		CComBSTR modified_by = L"modified_by" ;
		rec->put_ModifiedBy(modified_by) ;
		BOOST_CHECK_EQUAL("modified_by", string(string2string(record->get_modified_by()).c_str())) ;
	}
	// refcount
	BOOST_AUTO_TEST_CASE( get_refcount )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->put_RefCount(10) ;
		ULONG result(0);
		rec->get_RefCount(&result) ;
		BOOST_CHECK_EQUAL(10u, result) ;
	}
	// date created
	BOOST_AUTO_TEST_CASE( get_created )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		
		record_pointer record(new record_local) ;
		rec->set_record(record) ;
		misc_wrappers::date created ;
		created.set_to_local_time() ;
		created.wYear = 2005 ;
		created.wMonth = 7 ;
		created.wDay = 10 ;
		record->set_created(created) ;

		DATE result(0);
		rec->get_DateCreated(&result) ;

		SYSTEMTIME rectime ;
		::VariantTimeToSystemTime(result, &rectime) ;
		BOOST_CHECK_EQUAL(2005, rectime.wYear) ;
		BOOST_CHECK_EQUAL(7, rectime.wMonth) ;
	}

	BOOST_AUTO_TEST_CASE( set_created )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;

		record_pointer record(new record_local) ;
		rec->set_record(record) ;
		misc_wrappers::date created ;
		created.set_to_local_time() ;
		created.wYear = 2003 ;
		created.wMonth = 6 ;
		created.wDay = 10 ;

		DATE setting(0);
		::SystemTimeToVariantTime(&created, &setting) ;

		rec->put_DateCreated(setting) ;

		misc_wrappers::date result = record->get_created() ;

		BOOST_CHECK_EQUAL(2003, result.wYear) ;
		BOOST_CHECK_EQUAL(6, result.wMonth) ;
		BOOST_CHECK_EQUAL(10, result.wDay) ;
	}


	// date modified
	BOOST_AUTO_TEST_CASE( get_modified )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;

		record_pointer record(new record_local) ;
		rec->set_record(record) ;
		misc_wrappers::date modified ;
		modified.set_to_local_time() ;
		modified.wYear = 2005 ;
		modified.wMonth = 7 ;
		record->set_modified(modified) ;

		DATE result(0);
		rec->get_LastModified(&result) ;

		SYSTEMTIME rectime ;
		::VariantTimeToSystemTime(result, &rectime) ;
		BOOST_CHECK_EQUAL(2005, rectime.wYear) ;
		BOOST_CHECK_EQUAL(7, rectime.wMonth) ;
	}

	BOOST_AUTO_TEST_CASE( set_modified )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;

		record_pointer record(new record_local) ;
		rec->set_record(record) ;
		misc_wrappers::date modified ;
		modified.set_to_local_time() ;
		modified.wYear = 2003 ;
		modified.wMonth = 2 ;
		modified.wDay = 10 ;

		DATE setting(0);
		::SystemTimeToVariantTime(&modified, &setting) ;

		rec->put_LastModified(setting) ;

		misc_wrappers::date result = record->get_modified() ;

		BOOST_CHECK_EQUAL(2003, result.wYear) ;
		BOOST_CHECK_EQUAL(2, result.wMonth) ;
	}

	// id
	BOOST_AUTO_TEST_CASE( get_id_0 )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		ULONG result = 10;
		rec->get_Id(&result) ;
		BOOST_CHECK_EQUAL(0u, result) ;
	}
	BOOST_AUTO_TEST_CASE( set_id )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->put_Id(10) ;
		ULONG result = 0;
		rec->get_Id(&result) ;
		BOOST_CHECK_EQUAL(10u, result) ;
	}

	// reliability
	BOOST_AUTO_TEST_CASE( get_reliability_0 )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		ULONG result = 10;
		rec->get_Reliability(&result) ;
		BOOST_CHECK_EQUAL(0u, result) ;
	}
	BOOST_AUTO_TEST_CASE( set_reliability )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->put_Reliability(5u) ;
		ULONG result = 0;
		rec->get_Reliability(&result) ;
		BOOST_CHECK_EQUAL(5u, result) ;
	}
	// validated
	BOOST_AUTO_TEST_CASE(get_validated_inits_to_false)
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		VARIANT_BOOL result = VARIANT_TRUE;
		rec->get_Validated(&result) ;
		BOOST_CHECK_EQUAL((int)VARIANT_FALSE, (int)result) ;
	}
	BOOST_AUTO_TEST_CASE( set_validated )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->put_Validated(VARIANT_TRUE) ;
		VARIANT_BOOL result = VARIANT_FALSE;
		rec->get_Validated(&result) ;
		BOOST_CHECK_EQUAL((int)VARIANT_TRUE, (int)result) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif