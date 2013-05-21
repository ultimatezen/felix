#include "stdafx.h"
#include "GlossaryWindow.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST


// test_properties_loaded_history
BOOST_AUTO_TEST_SUITE( TestGlossaryWindow )

	typedef CComObject< CGlossaryWindow > gw_obj ;
	typedef CComPtr< gw_obj > gw_ptr ;

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		gw_ptr gloss ;
		HRESULT hr = gw_obj::CreateInstance( &gloss ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
	}


BOOST_AUTO_TEST_SUITE_END()

#endif