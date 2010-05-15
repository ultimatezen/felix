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

	BOOST_AUTO_TEST_CASE(ensure_path_exists_throw)
	{
		BOOST_CHECK_THROW(ensure_file_exists(_T("c:\\path\\does\\not\\exist.ftm")), except::CWinException) ;
	}
	BOOST_AUTO_TEST_CASE(ensure_path_exists_nothrow)
	{
		BOOST_CHECK_NO_THROW(ensure_file_exists(_T("c:\\dev\\cpp\\Assistant Suite\\Felix\\TranslationMemory_test.cpp"))) ;
	}

BOOST_AUTO_TEST_SUITE_END()
