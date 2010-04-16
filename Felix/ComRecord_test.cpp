#include "StdAfx.h"
#include "ComRecord.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	using namespace mem_engine ;

	typedef CComPtr< CComObject< CRecord > > com_rec_ptr ;

	TEST( TestComRecord, Instantiate )
	{
		com_rec_ptr rec ;
		HRESULT hr = CComObject< CRecord >::CreateInstance( &rec ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		ASSERT_TRUE( rec ) ;
	}
	TEST( TestComRecord, trans_empty )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR result ;
		rec->get_Trans(&result) ;
		ASSERT_EQUALS_V("", SimpleString(CW2A(result))) ;
	}
	TEST( TestComRecord, trans )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR trans = L"spam" ;
		rec->put_Trans(trans) ;
		CComBSTR result ;
		rec->get_Trans(&result) ;
		ASSERT_EQUALS_V("spam", SimpleString(CW2A(result))) ;
	}
	TEST( TestComRecord, source )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR source = L"spam" ;
		rec->put_Source(source) ;
		CComBSTR result ;
		rec->get_Source(&result) ;
		ASSERT_EQUALS_V("spam", SimpleString(CW2A(result))) ;
	}
	TEST( TestComRecord, source_empty )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR result ;
		rec->get_Source(&result) ;
		ASSERT_EQUALS_V("", SimpleString(CW2A(result))) ;
	}
	TEST( TestComRecord, context )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR context = L"context" ;
		rec->put_Context(context) ;
		CComBSTR result ;
		rec->get_Context(&result) ;
		ASSERT_EQUALS_V("context", SimpleString(CW2A(result))) ;
	}
	TEST( TestComRecord, created_by )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR creator = L"creator" ;
		rec->put_CreatedBy(creator) ;
		CComBSTR result ;
		rec->get_CreatedBy(&result) ;
		ASSERT_EQUALS_V("creator", SimpleString(CW2A(result))) ;
	}
	TEST( TestComRecord, modified_by )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		CComBSTR modified_by = L"modified_by" ;
		rec->put_ModifiedBy(modified_by) ;
		CComBSTR result ;
		rec->get_ModifiedBy(&result) ;
		ASSERT_EQUALS_V("modified_by", SimpleString(CW2A(result))) ;
	}
	TEST( TestComRecord, modified_by_changes_record )
	{
		record_pointer record(new record_local) ;
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->set_record(record) ;
		CComBSTR modified_by = L"modified_by" ;
		rec->put_ModifiedBy(modified_by) ;
		ASSERT_EQUALS_V("modified_by", SimpleString(string2string(record->get_modified_by()).c_str())) ;
	}
	TEST( TestComRecord, get_id_0 )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		ULONG result = 10;
		rec->get_Id(&result) ;
		ASSERT_EQUALS_V(0, (int)result) ;
	}
	TEST( TestComRecord, get_id )
	{
		com_rec_ptr rec ;
		CComObject< CRecord >::CreateInstance( &rec ) ;
		rec->put_Id(10) ;
		ULONG result = 0;
		rec->get_Id(&result) ;
		ASSERT_EQUALS_V(10, (int)result) ;
	}
}

#endif