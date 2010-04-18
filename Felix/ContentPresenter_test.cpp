#include "StdAfx.h"
#include ".\contentpresenter.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( content_presenter_test )

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

	BOOST_AUTO_TEST_CASE( Instantiate)
	{
		try
		{
			html::CHtmlView view ;
			FakeContentPresenter presenter(view) ;
			BOOST_CHECK(true) ;
		}
		catch (...)
		{
			BOOST_FAIL( "Failed to instantiate CContentPresenter class in test harness" ) ;
		}
	}
BOOST_AUTO_TEST_SUITE_END()