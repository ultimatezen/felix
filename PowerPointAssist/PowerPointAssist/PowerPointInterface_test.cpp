#include "stdafx.h"
#include "PowerPointInterface.h"
#include "Hooker.h"

#include "globalApp.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{

//////////////////////////////////////////////////////////////////////////
// SimpleDocPPInterfaceTest unit tests =================================
//////////////////////////////////////////////////////////////////////////

	TEST( SimpleDocPPInterfaceTest, Initialize )
	{
		PowerPoint::_ApplicationPtr mApp ;
		mApp = globalApp::get() ;
		ASSERT_TRUE( mApp ) ;
	}



}

#endif // #ifdef _DEBUG
