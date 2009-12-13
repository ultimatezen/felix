/*!
	@brief Unit tests for memory class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "TranslationMemory.h"

#include "Path.h"
#include "record_local.h"

#include "easyunit/testharness.h"
#include "MockListener.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_engine ;

	TEST(TestMemoryFunctions, get_load_failure_msg)
	{
		CString filename = "spam" ;
		CStringA actual = get_load_failure_msg(filename) ;
		SimpleString expected = "Failed to load file [spam]" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

	TEST(is_vista_or_later, ensure_true)
	{
		ASSERT_TRUE(is_vista_or_later()) ;
	}

}
#endif // UNIT_TEST
