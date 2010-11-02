/*!
	@brief Unit tests for memory class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "TranslationMemory.h"

#include "Path.h"
#include "record_local.h"
#include "MockListener.h"
#include "memory_local.h"
#include <boost/test/unit_test.hpp>
#include "input_device_file.h"

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
		InputDeviceFile input ;
		BOOST_CHECK_THROW(input.ensure_file_exists(_T("c:\\path\\does\\not\\exist.ftm")), except::CWinException) ;
	}
	BOOST_AUTO_TEST_CASE(ensure_path_exists_nothrow)
	{
		InputDeviceFile input ;
		BOOST_CHECK_NO_THROW(input.ensure_file_exists(_T("c:\\dev\\cpp\\Assistant Suite\\Felix\\TranslationMemory_test.cpp"))) ;
	}
	BOOST_AUTO_TEST_CASE(get_correct_encoding_xml_utf8_sq)
	{
		string text = "<?xml version='1.0' encoding='utf-8'?>" ;
		memory_local mem ;
		BOOST_CHECK_EQUAL((UINT)CP_UTF8, (UINT)mem.get_correct_encoding(text.c_str(), text.size())) ;
	}
	BOOST_AUTO_TEST_CASE(get_correct_encoding_xml_utf8_dq)
	{
		string text = "<?xml version='1.0' encoding=\"utf-8\"?>" ;
		memory_local mem ;
		BOOST_CHECK_EQUAL((UINT)CP_UTF8, (UINT)mem.get_correct_encoding(text.c_str(), text.size())) ;
	}
BOOST_AUTO_TEST_SUITE_END()
