#pragma once

# ifdef _DEBUG
#	include "TestResultsPrinter.h"
#	include "ProgressBarTestRunner.h"
# endif

inline void run_unit_tests()
{
#ifdef UNIT_TEST
	easyunit::CATLResultsPrinter printer ;
	easyunit::CProgressBarTestRunner runner ;
	easyunit::TestRegistry::runAndPrint( 
		static_cast< easyunit::TestPrinter* >( &printer ), 
		static_cast< easyunit::TestRunner* >( &runner ) ) ;		
#endif // #ifdef _DEBUG
	
}