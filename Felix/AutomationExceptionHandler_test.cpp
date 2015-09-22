#include "stdafx.h"
#include "AutomationExceptionHandler.h"
#include "App.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestAutomationExceptionHandler )

	typedef CComPtr< CComObject< CApp > > appPtr ;
	BOOST_AUTO_TEST_CASE( std_exception)
	{
		appPtr app ;
		CComObject< CApp >::CreateInstance( &app ) ;
		std::exception e("boom!") ;
		CString msg(_T("msg")) ;
		BOOST_CHECK_EQUAL((int)E_FAIL, (int)app.p->handle_exception(e, msg)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif