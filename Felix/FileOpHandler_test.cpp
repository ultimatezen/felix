#include "stdafx.h"
#include "FileOpHandler.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestFileOpHandler )

	using namespace fileops ;

	BOOST_AUTO_TEST_CASE( test_addExtensionAsNeeded_no )
	{
		CString filename = "foo.txt" ;
		CString ext = ".doc" ;
		add_extension_as_needed(filename, ext) ;
		string actual = (LPCSTR)CT2A(filename) ;
		string expected = "foo.txt" ;
		BOOST_CHECK_EQUAL(actual, expected) ;
	}

	BOOST_AUTO_TEST_CASE( test_addExtensionAsNeeded_yes )
	{
		CString filename = "foo" ;
		CString ext = ".doc" ;
		add_extension_as_needed(filename, ext) ;
		string actual = (LPCSTR)CT2A(filename) ;
		string expected = "foo.doc" ;
		BOOST_CHECK_EQUAL(actual, expected) ;
	}

	BOOST_AUTO_TEST_CASE( test_get_local_appdata_folder )
	{
		CString out = get_local_appdata_folder() ;
		string actual = (LPCSTR)CT2A(out) ;
		string expected = "D:\\Users\\Ryan\\AppData\\Local" ;
		BOOST_CHECK_EQUAL(actual, expected) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif