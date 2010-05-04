#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( glossCapTest, Simple)
	{
		_bstr_t cap = glossCap( _bstr_t(L"foo"), 1 ) ;
		_bstr_t expected(L"foo &1    (Alt+1)") ;
		ASSERT_EQUALS( cap, expected) ;

		cap = glossCap( _bstr_t(L"foo"), 6 ) ;
		expected = (L"foo &6    (Alt+6)") ;
		ASSERT_EQUALS( cap, expected) ;

		cap = glossCap( _bstr_t(L"foo"), 10 ) ;
		expected = (L"foo 1&0   (Alt+0)") ;
		ASSERT_EQUALS( cap, expected) ;
	}
	TEST( glossDescTest, Simple)
	{
		_bstr_t cap = glossDesc( _bstr_t(L"foo"), 1 ) ;
		_bstr_t expected(L"foo 1") ;
		ASSERT_EQUALS( cap, expected) ;

		cap = glossDesc( _bstr_t(L"foo"), 5 ) ;
		expected = (L"foo 5") ;
		ASSERT_EQUALS( cap, expected) ;

		cap = glossDesc( _bstr_t(L"foo"), 10 ) ;
		expected = (L"foo 10") ;
		ASSERT_EQUALS( cap, expected) ;
	}
	typedef CComPtr< CComObject< CConnect > > connPtr ;

	TEST( CConnectTest, Instantiate )
	{
		try
		{
			connPtr conn ;
			HRESULT hr = CComObject< CConnect >::CreateInstance( &conn ) ;
			ASSERT_TRUE( SUCCEEDED( hr ) ) ;
			ASSERT_TRUE( conn ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate CConnect class in harness" ) ;
		}
	}
}
#endif
