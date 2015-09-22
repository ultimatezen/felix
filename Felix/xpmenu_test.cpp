#include "stdafx.h"
#include "xpmenu/ToolBarXP.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( ToolBarXPTest )

	BOOST_AUTO_TEST_CASE(handled )
	{
		CToolBarXP toolbar ;
		toolbar.SetMsgHandled(TRUE) ;
		BOOST_CHECK(toolbar.IsMsgHandled()) ;
		toolbar.SetMsgHandled(FALSE) ;
		BOOST_CHECK(! toolbar.IsMsgHandled()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
