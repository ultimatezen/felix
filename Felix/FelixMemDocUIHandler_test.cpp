#include "stdafx.h"
#include "FelixMemDocUIHandler.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CFelixMemDocUIHandlerTest )

	BOOST_AUTO_TEST_CASE( FinalConstruct )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		BOOST_CHECK(SUCCEEDED(handler->FinalConstruct()));
	}
	BOOST_AUTO_TEST_CASE( ShowUI )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		HRESULT retval = S_OK ;
		BOOST_CHECK(E_NOTIMPL == handler->ShowUI(0, NULL, NULL, NULL, NULL, &retval));
	}
BOOST_AUTO_TEST_SUITE_END()
#endif
