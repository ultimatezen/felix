#include "stdafx.h"
#include "ManagerViewDetails.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"

BOOST_AUTO_TEST_SUITE( TestManagerViewDetails )

	using namespace mem_engine ;
	using namespace mgrview ;
	
	//////////////////////////////////////////////////////////////////////////
	// ViewState functions
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE(init)
	{
		BOOST_CHECK_NO_THROW(ManagerViewDetails(0u, true)) ;
	}


BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
