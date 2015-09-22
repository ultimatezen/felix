#include "StdAfx.h"
#include ".\navurl.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CNavUrlTestCase )


	BOOST_AUTO_TEST_CASE( Constructor )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		BOOST_CHECK_EQUAL( true, dummy.m_call_history.empty() ) ;
	}

	BOOST_AUTO_TEST_CASE( tokenize_url )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		std::vector< wstring > tokens ;
		_bstr_t url(L"100:foo:bar:32") ;
		nav_url.tokenize_url( url, tokens ) ;

		BOOST_CHECK_EQUAL( 3u, tokens.size() ) ;
		BOOST_CHECK_EQUAL(  L"foo", tokens[0] ) ;
		BOOST_CHECK_EQUAL( L"bar", tokens[1] ) ;
		BOOST_CHECK_EQUAL( L"32", tokens[2] ) ;

		std::vector< wstring > tokens2 ;
		url = L"100:reg_gloss_add" ;
		nav_url.tokenize_url( url, tokens2 ) ;

		BOOST_CHECK_EQUAL( 1u, tokens2.size() ) ;
		BOOST_CHECK_EQUAL( L"reg_gloss_add", tokens2[0] ) ;

	}

	BOOST_AUTO_TEST_CASE( is_url )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		_bstr_t url(L"http://www.ginstrom.com/") ;
		BOOST_CHECK_EQUAL( true, nav_url.is_url(url ) ) ;
		url = L"http://felix-cat.com/" ;
		BOOST_CHECK_EQUAL( true, nav_url.is_url(url ) ) ;
		url = L"http://www.ginstrom.com/cgi-bin/search.cgi?action=5?id=35" ;
		BOOST_CHECK_EQUAL( true, nav_url.is_url(url ) ) ;

		url = L"reg_gloss_set_source" ;
		BOOST_CHECK_EQUAL( false, nav_url.is_url(url ) ) ;
		url = L"reg_gloss_set_source:0" ;
		BOOST_CHECK_EQUAL( false, nav_url.is_url(url ) ) ;
		url = L"reg_gloss_set_source|4" ;
		BOOST_CHECK_EQUAL( false, nav_url.is_url(url ) ) ;
	}

	BOOST_AUTO_TEST_CASE( NavRegGlossPage )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		//_bstr_t url(L"100:reg_gloss_set_source") ;
		//nav_url.process_url( url );
		//BOOST_CHECK_EQUAL( 1, dummy.m_call_history.size() ) ;
		//BOOST_CHECK_EQUAL( _T("OnNavRegGlossSetSource"), *(dummy.m_call_history.begin() ) ) ;

		//url = _bstr_t(L"100:reg_gloss_set_trans" ) ;
		//nav_url.process_url( url );
		//BOOST_CHECK_EQUAL( 2, dummy.m_call_history.size() ) ;
		//BOOST_CHECK_EQUAL( _T("OnNavRegGlossSetTrans"), *(dummy.m_call_history.begin() ) ) ;

		//url = _bstr_t(L"100:reg_gloss_back" ) ;
		//nav_url.process_url( url );
		//BOOST_CHECK_EQUAL( 3, dummy.m_call_history.size() ) ;
		//BOOST_CHECK_EQUAL( _T("OnNavRegGlossBack"), *(dummy.m_call_history.begin() ) ) ;

		//url = _bstr_t(L"100:reg_gloss_add" ) ;
		//nav_url.process_url( url );
		//BOOST_CHECK_EQUAL( 4, dummy.m_call_history.size() ) ;
		//BOOST_CHECK_EQUAL( _T("OnNavRegGlossAdd"), *(dummy.m_call_history.begin() ) ) ;

		_bstr_t url = _bstr_t(L"http://www.ginstrom.com/" ) ;
		nav_url.process_url( url );
		BOOST_CHECK_EQUAL( 0u, dummy.m_call_history.size() ) ;
		BOOST_CHECK( dummy.m_call_history.end() == dummy.m_call_history.begin() ) ;

	}


	BOOST_AUTO_TEST_CASE( NavAddedTransPage )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		_bstr_t url(L"100:edit:0") ;
		nav_url.process_url( url );
		BOOST_CHECK_EQUAL( 1u, dummy.m_call_history.size() ) ;
		BOOST_CHECK_EQUAL( _T("OnNavEdit"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"100:delete:0" ) ;
		nav_url.process_url( url );
		BOOST_CHECK_EQUAL( true, nav_url.should_cancel() ) ;
		BOOST_CHECK_EQUAL( 2u, dummy.m_call_history.size() ) ;
		BOOST_CHECK_EQUAL( _T("OnNavDelete"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"http://www.ginstrom.com/" ) ;
		nav_url.process_url( url );
		BOOST_CHECK_EQUAL( 2u, dummy.m_call_history.size() ) ;
		BOOST_CHECK_EQUAL( false, nav_url.should_cancel() ) ;
		BOOST_CHECK_EQUAL( _T("OnNavDelete"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"100:add_gloss:0" ) ;
		nav_url.process_url( url );
		BOOST_CHECK_EQUAL( 3u, dummy.m_call_history.size() ) ;
		BOOST_CHECK_EQUAL( _T("OnNavAddGloss"), *(dummy.m_call_history.begin() ) ) ;

		url = _bstr_t(L"100:add_to_gloss:0" ) ;
		nav_url.process_url( url );
		BOOST_CHECK_EQUAL( 4u, dummy.m_call_history.size() ) ;
		BOOST_CHECK_EQUAL( _T("OnNavAddToGloss"), *(dummy.m_call_history.begin() ) ) ;

	}

	BOOST_AUTO_TEST_CASE( should_cancel )
	{
		CNavInterfaceDummy dummy ;

		CNavUrl nav_url( static_cast< CNavInterface* >( &dummy ) ) ;

		BOOST_CHECK_EQUAL( true, nav_url.should_cancel() ) ;

		_bstr_t url = _bstr_t(L"http://www.ginstrom.com/" ) ;
		nav_url.process_url(url) ;
		BOOST_CHECK_EQUAL( false, nav_url.should_cancel() ) ;

		url = _bstr_t(L"reg_gloss_set_source") ;
		nav_url.process_url(url) ;
		BOOST_CHECK_EQUAL( true, nav_url.should_cancel() ) ;

	}
BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef _DEBUG
