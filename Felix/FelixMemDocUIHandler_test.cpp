#include "stdafx.h"
#include "FelixMemDocUIHandler.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CFelixMemDocUIHandlerTest )

	struct FakeGetMenuHandler
	{
		DECLARE_SENSING_VAR ;

		HRESULT GetMenu()
		{
			SENSE("GetMenu") ;
			return S_OK ;
		}
	};

	BOOST_AUTO_TEST_CASE( FinalConstruct )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		BOOST_CHECK(SUCCEEDED(handler->FinalConstruct()));
	}
	BOOST_AUTO_TEST_CASE( ShowContextMenu )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		FakeGetMenuHandler menu_handler ;
		handler->m_get_menu = boost::bind(&FakeGetMenuHandler::GetMenu, &menu_handler) ;
		HRESULT retval(50) ;
		BOOST_CHECK(SUCCEEDED(handler->ShowContextMenu(0, 0, 0, NULL, NULL, &retval))) ;
		BOOST_CHECK_EQUAL("GetMenu", menu_handler.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(1u, menu_handler.m_sensing_variable.size());
		BOOST_CHECK(retval == S_OK);
	}
	BOOST_AUTO_TEST_CASE( GetHostInfo )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		DWORD flags = 0 ;
		BOOST_CHECK(SUCCEEDED(handler->GetHostInfo(&flags, NULL))) ;
		DWORD expected = DOCHOSTUIFLAG_NO3DBORDER | 0x40000 ;
		BOOST_CHECK(flags == expected) ;
	}
	BOOST_AUTO_TEST_CASE( ShowUI )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		HRESULT retval = S_OK ;
		BOOST_CHECK(E_NOTIMPL == handler->ShowUI(0, NULL, NULL, NULL, NULL, &retval));
	}
	BOOST_AUTO_TEST_CASE( HideUI )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		BOOST_CHECK(E_NOTIMPL == handler->HideUI());
	}
	BOOST_AUTO_TEST_CASE( UpdateUI )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		BOOST_CHECK(E_NOTIMPL == handler->UpdateUI());
	}
	BOOST_AUTO_TEST_CASE( EnableModeless )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		BOOST_CHECK(E_NOTIMPL == handler->EnableModeless(VARIANT_FALSE));
	}
	BOOST_AUTO_TEST_CASE( OnDocWindowActivate )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		BOOST_CHECK(E_NOTIMPL == handler->OnDocWindowActivate(VARIANT_FALSE));
	}
	BOOST_AUTO_TEST_CASE( ResizeBorder )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		BOOST_CHECK(E_NOTIMPL == handler->ResizeBorder(0, 0, 0, 0, NULL, VARIANT_FALSE));
	}
	BOOST_AUTO_TEST_CASE( TranslateAccelerator )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		CComBSTR group = L"group" ;
		BOOST_CHECK(E_NOTIMPL == handler->TranslateAccelerator(NULL, 0, NULL, NULL, group, 0, NULL));
	}
	BOOST_AUTO_TEST_CASE( GetOptionKeyPath )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		CComBSTR key = L"group" ;
		BOOST_CHECK(E_NOTIMPL == handler->GetOptionKeyPath(&key, 0));
	}
	BOOST_AUTO_TEST_CASE( GetDropTarget )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		CComPtr<IUnknown> drop_target ;
		BOOST_CHECK(E_NOTIMPL == handler->GetDropTarget(NULL, &drop_target));
	}
	BOOST_AUTO_TEST_CASE( GetExternal )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		CComPtr<IDispatch> external ;
		BOOST_CHECK(E_NOTIMPL == handler->GetExternal(&external));
	}
	BOOST_AUTO_TEST_CASE( TranslateUrl )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		CComBSTR URL = L"http://ginstrom.com/" ;
		BOOST_CHECK(E_NOTIMPL == handler->TranslateUrl(0, URL, NULL));
	}
	BOOST_AUTO_TEST_CASE( FilterDataObject )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		BOOST_CHECK(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)));
		CComPtr<IUnknown> object ;
		BOOST_CHECK(E_NOTIMPL == handler->FilterDataObject(NULL, &object));
	}

BOOST_AUTO_TEST_SUITE_END()
#endif
