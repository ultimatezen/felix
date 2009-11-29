#include "StdAfx.h"
#include "ComRecord.h"

#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
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