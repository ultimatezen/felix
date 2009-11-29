#include "StdAfx.h"
#include ".\navurl.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG
    
namespace easyunit
{
	TEST( CNavUrlTestCase, Constructor )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		ASSERT_EQUALS( true, dummy.m_call_history.empty() ) ;
	}

	TEST( CNavUrlTestCase, tokenize_url )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		std::vector< wstring > tokens ;
		_bstr_t url(L"100:foo:bar:32") ;
		nav_url.tokenize_url( url, tokens ) ;

		ASSERT_EQUALS( 3, tokens.size() ) ;
		ASSERT_EQUALS(  L"foo", tokens[0] ) ;
		ASSERT_EQUALS( L"bar", tokens[1] ) ;
		ASSERT_EQUALS( L"32", tokens[2] ) ;

		std::vector< wstring > tokens2 ;
		url = L"100:reg_gloss_add" ;
		nav_url.tokenize_url( url, tokens2 ) ;

		ASSERT_EQUALS( 1, tokens2.size() ) ;
		ASSERT_EQUALS( L"reg_gloss_add", tokens2[0] ) ;

	}

	TEST( CNavUrlTestCase, is_url )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		_bstr_t url(L"http://www.ginstrom.com/") ;
		ASSERT_EQUALS( true, nav_url.is_url(url ) ) ;
		url = L"http://felix-cat.com/" ;
		ASSERT_EQUALS( true, nav_url.is_url(url ) ) ;
		url = L"http://www.ginstrom.com/cgi-bin/search.cgi?action=5?id=35" ;
		ASSERT_EQUALS( true, nav_url.is_url(url ) ) ;

		url = L"reg_gloss_set_source" ;
		ASSERT_EQUALS( false, nav_url.is_url(url ) ) ;
		url = L"reg_gloss_set_source:0" ;
		ASSERT_EQUALS( false, nav_url.is_url(url ) ) ;
		url = L"reg_gloss_set_source|4" ;
		ASSERT_EQUALS( false, nav_url.is_url(url ) ) ;
	}

	TEST( CNavUrlTestCase, NavRegGlossPage )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		//_bstr_t url(L"100:reg_gloss_set_source") ;
		//nav_url.process_url( url );
		//ASSERT_EQUALS( 1, dummy.m_call_history.size() ) ;
		//ASSERT_EQUALS( _T("OnNavRegGlossSetSource"), *(dummy.m_call_history.begin() ) ) ;

		//url = _bstr_t(L"100:reg_gloss_set_trans" ) ;
		//nav_url.process_url( url );
		//ASSERT_EQUALS( 2, dummy.m_call_history.size() ) ;
		//ASSERT_EQUALS( _T("OnNavRegGlossSetTrans"), *(dummy.m_call_history.begin() ) ) ;

		//url = _bstr_t(L"100:reg_gloss_back" ) ;
		//nav_url.process_url( url );
		//ASSERT_EQUALS( 3, dummy.m_call_history.size() ) ;
		//ASSERT_EQUALS( _T("OnNavRegGlossBack"), *(dummy.m_call_history.begin() ) ) ;

		//url = _bstr_t(L"100:reg_gloss_add" ) ;
		//nav_url.process_url( url );
		//ASSERT_EQUALS( 4, dummy.m_call_history.size() ) ;
		//ASSERT_EQUALS( _T("OnNavRegGlossAdd"), *(dummy.m_call_history.begin() ) ) ;

		_bstr_t url = _bstr_t(L"http://www.ginstrom.com/" ) ;
		nav_url.process_url( url );
		ASSERT_EQUALS( 0, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( dummy.m_call_history.end(), dummy.m_call_history.begin() ) ;

	}

	TEST( CNavUrlTestCase, NavInitialPage )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		_bstr_t url(L"100:show_about") ;
		nav_url.process_url( url );
		ASSERT_EQUALS( 1, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( _T("OnNavShowAbout"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"http://www.ginstrom.com/" ) ;
		nav_url.process_url( url );
		ASSERT_EQUALS( 1, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( _T("OnNavShowAbout"), *(dummy.m_call_history.begin() ) ) ;

	}

	TEST( CNavUrlTestCase, NavAddedTransPage )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		_bstr_t url(L"100:edit:0") ;
		nav_url.process_url( url );
		ASSERT_EQUALS( 1, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( _T("OnNavEdit"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"100:delete:0" ) ;
		nav_url.process_url( url );
		ASSERT_EQUALS( true, nav_url.should_cancel() ) ;
		ASSERT_EQUALS( 2, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( _T("OnNavDelete"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"http://www.ginstrom.com/" ) ;
		nav_url.process_url( url );
		ASSERT_EQUALS( 2, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( false, nav_url.should_cancel() ) ;
		ASSERT_EQUALS( _T("OnNavDelete"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"100:add_gloss:0" ) ;
		nav_url.process_url( url );
		ASSERT_EQUALS( 3, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( _T("OnNavAddGloss"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"100:add_to_gloss:0" ) ;
		nav_url.process_url( url );
		ASSERT_EQUALS( 4, dummy.m_call_history.size() ) ;
		ASSERT_EQUALS( _T("OnNavAddToGloss"), *(dummy.m_call_history.begin() ) ) ;

	}

	TEST( CNavUrlTestCase, should_cancel )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		ASSERT_EQUALS( true, nav_url.should_cancel() ) ;

		_bstr_t url = _bstr_t(L"http://www.ginstrom.com/" ) ;
		nav_url.process_url(url) ;
		ASSERT_EQUALS( false, nav_url.should_cancel() ) ;

		url = _bstr_t(L"reg_gloss_set_source") ;
		nav_url.process_url(url) ;
		ASSERT_EQUALS( true, nav_url.should_cancel() ) ;

	}
}


#endif // #ifdef _DEBUG
