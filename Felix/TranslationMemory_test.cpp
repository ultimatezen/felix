/*!
	@brief Unit tests for memory class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "TranslationMemory.h"

#include "Path.h"
#include "record_local.h"
#include "MockListener.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestMemoryFunctions )

	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( test_get_load_failure_msg)
	{
		CString filename = "spam" ;
		CString actual = get_load_failure_msg(filename) ;
		CString expected = "Failed to load file [spam]" ;
		BOOST_CHECK(expected == actual) ;
	}

	BOOST_AUTO_TEST_CASE( ensure_true)
	{
		BOOST_CHECK(is_vista_or_later()) ;
	}

BOOST_AUTO_TEST_SUITE_END()
