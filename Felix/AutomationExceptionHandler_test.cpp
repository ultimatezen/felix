#include "stdafx.h"
#include "AutomationExceptionHandler.h"
#include "easyunit/testharness.h"
#include "App.h"

#ifdef UNIT_TEST

namespace easyunit
{
	typedef CComPtr< CComObject< CApp > > appPtr ;
	TEST( TestAutomationExceptionHandler, std_exception)
	{
		appPtr app ;
		CComObject< CApp >::CreateInstance( &app ) ;
		std::exception e("boom!") ;
		CString msg(_T("msg")) ;
		ASSERT_EQUALS_V((int)E_FAIL, (int)app.p->handle_exception(e, msg)) ;
	}

}

#endif