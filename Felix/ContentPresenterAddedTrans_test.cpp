#include "StdAfx.h"
#include ".\contentpresenteraddedtrans.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCContentPresenterAddedTrans )

	using namespace content_presenter ;
	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE(Instantiate )
	{
		CHtmlView view ;
		record_pointer record(new record_local()) ;
		CContentPresenterAddedTrans presenter(view, record) ;
		BOOST_CHECK(true) ;
	}
	BOOST_AUTO_TEST_CASE( generate_text )
	{
		CHtmlView view ;
		record_pointer record(new record_local()) ;
		record->set_source(wstring(L"spam")) ;
		record->set_trans(wstring(L"egg")) ;
		CContentPresenterAddedTrans presenter(view, record) ;
		wstring out = presenter.generate_text() ;
		BOOST_CHECK(out.find(L"spam") != wstring::npos) ;
		BOOST_CHECK(out.find(L"egg") != wstring::npos) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif 