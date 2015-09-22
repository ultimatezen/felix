//////////////////////////////////////////////////////////////////////////////
//            CTA++, C++ Test Aider/Visual Studio Integration v2.0          //
//                                                                          //
//                     GENERATED TEST SCRIPT FILE                           //
//                                                                          //
//            Copyright (c) [generated parts] 2003-2004 Testwell Oy         //
//////////////////////////////////////////////////////////////////////////////
//
// CTA/VSI: File created: Dec 16 2004 00:51:38
// CTA/VSI: Last CTA/VSI update: Dec 16 2004 00:51:38
// Last user update: <date> (when edit, you may wish to maintain this)
//
// Here you may wish to insert version control hooks, like
// $RCSfile$, $Revision$, $Date$, $Author$ and $Log$
//
// Note 1. Lines starting with '// CTA/VSI:' are auxiliary lines to
// CTA++/Visual Studio Integration. DO NOT EDIT THESE LINES!
// Note 2. Lines starting with '// TODO:' advises where especially you
// can and sometimes must insert test code. You can edit these freely.
//////////////////////////////////////////////////////////////////////////////
//
// TODO: Uncomment the following  definition, if old streams...
// #define CTA_OLDSTREAM

// Enable the CTA++ macro language and class interface
#include <cta.h>

// TODO: Include code under test headers as required by C++ rules.
// Remember extern "C" {...} wrapping if testing plain C code.

// Get CTA_ prefixless aliases for the test macros defined in 'cta.h'
// If name conflicts, see CTA++ User's Guide for resolving.
#include <ctanames.h>

// Images //
// TODO: Add image macros IMAGE... ENDIMAGE and IMAGE_CAST
// to display objects of user's type

// Exception handlers //
// TODO: User's exceptions to be caught by name by the test driver
// (this means: when the user's code escapes with one of the exceptions
// below, CTA++ is able to report the type of the exception)
HANDLER
	CATCH(CTA_Exception_InternalError)
ENDHANDLER

// Stubs //
// TODO: Add stub functions (if introduced them manually)
// Remember extern "C" {...} wrapping, if stubbing plain C code.

// CTA/VSI: Include stub function files above

// Test case functions //
// TODO: Define test case functions

// CTA/VSI: Define test case functions above

// The main test script function //
int main(int argc, char* argv[])
{
	// Create a test driver object 'd'
	DRIVER(d);

	// TODO: Add object definitions needed in test case function registrations

// CTA/VSI: Register test cases in 'd' above

	// Run the test cases and return status code to the shell level
	return d.run(argc, argv);
}
// EOF
