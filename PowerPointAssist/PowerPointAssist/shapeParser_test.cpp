#include "stdafx.h"
#include "ShapeParser.h"
#include "StrategyFactory.h"
#include "globalApp.h"
#include "textrangeparser.h"

#ifdef UNIT_TEST

namespace easyunit
{

//////////////////////////////////////////////////////////////////////////
// SimpleDocParseTest unit tests =================================
//////////////////////////////////////////////////////////////////////////

	TEST( SimpleDocParseTest, ParseFirstPage )
	{
		PowerPoint::_ApplicationPtr mApp ;
		mApp = globalApp::get() ;
		ASSERT_TRUE_M( mApp, "Failed to get application pointer") ;
	}


	TEST( strategyFactoryTest, NullShape )
	{
		parsingStrategy strat = strategyFactory::create( NULL ) ;

		ASSERT_EQUALS_M( strat, NULL, "create with null shape must produce null strategy" ) ;

	}
}

#endif // #ifdef _DEBUG
