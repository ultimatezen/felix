#include "stdafx.h"
#include "ShapeParser.h"
#include "StrategyFactory.h"
#include "globalApp.h"
#include "textrangeparser.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( SimpleDocParseTest )


//////////////////////////////////////////////////////////////////////////
// SimpleDocParseTest unit tests =================================
//////////////////////////////////////////////////////////////////////////


	BOOST_AUTO_TEST_CASE(test_NullShape )
	{
		parsingStrategy strat = strategyFactory::create( NULL ) ;

		BOOST_CHECK( ! strat ) ;

	}

BOOST_AUTO_TEST_SUITE_END()

#endif

