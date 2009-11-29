#include "StdAfx.h"
#include ".\contentpresenter.h"

#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	class FakeContentPresenter : public content_presenter::CContentPresenter
	{
	public:
		FakeContentPresenter(html::CHtmlView &view) :
			content_presenter::CContentPresenter(view)
		{

		}
		void present(void)
		{

		}
	};

	TEST( content_presenter_test, Instantiate)
	{
		try
		{
			html::CHtmlView view ;
			FakeContentPresenter presenter(view) ;
			ASSERT_TRUE(true) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate CContentPresenter class in test harness" ) ;
		}
	}
}

#endif