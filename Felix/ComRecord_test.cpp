#include "StdAfx.h"
#include "ComRecord.h"
#include "record_local.h"

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
	BOOST_AUTO_TEST_CASE( source_empty )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR result ;
		rec->get_Source(&result) ;
		BOOST_CHECK_EQUAL("", string(CW2A(result))) ;
	}
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
	BOOST_AUTO_TEST_CASE( get_id_0 )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		ULONG result = 10;
		rec->get_Id(&result) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
	}
	BOOST_AUTO_TEST_CASE( get_id )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->put_Id(10) ;
		ULONG result = 0;
		rec->get_Id(&result) ;
		BOOST_CHECK_EQUAL(10, (int)result) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif