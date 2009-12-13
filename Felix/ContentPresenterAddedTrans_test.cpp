#include "StdAfx.h"
#include ".\contentpresenteraddedtrans.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	using namespace content_presenter ;
	using namespace memory_engine ;

	TEST( TestCContentPresenterAddedTrans, Instantiate )
	{
		CHtmlView view ;
		record_pointer record(new record_local()) ;
		CContentPresenterAddedTrans presenter(view, record) ;
		ASSERT_TRUE(true) ;
	}
	TEST( TestCContentPresenterAddedTrans, generate_text )
	{
		CHtmlView view ;
		record_pointer record(new record_local()) ;
		record->set_source(wstring(L"spam")) ;
		record->set_trans(wstring(L"egg")) ;
		CContentPresenterAddedTrans presenter(view, record) ;
		wstring out = presenter.generate_text() ;
		ASSERT_TRUE(out.find(L"spam") != wstring::npos) ;
		ASSERT_TRUE(out.find(L"egg") != wstring::npos) ;
	}

}


#endif 