#include "stdafx.h"
#include "Library.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_library )


	BOOST_AUTO_TEST_CASE(null_arg_ctor)
	{
		CLibrary library(NULL) ;
		BOOST_CHECK(! library.is_loaded()) ;
	}
	BOOST_AUTO_TEST_CASE(null_arg_load)
	{
		CLibrary library(NULL) ;
		BOOST_CHECK(! library.load(NULL)) ;
	}
	BOOST_AUTO_TEST_CASE( rich_edit)
	{
		CLibrary library(_T("RICHED20.DLL")) ;
		BOOST_CHECK(library.is_loaded()) ;
	}
	BOOST_AUTO_TEST_CASE( get_proc_address)
	{
		CLibrary library(_T("ArmAccess.DLL")) ;
		BOOST_CHECK(library.is_loaded()) ;
		FARPROC proc_address = library.get_proc_address("InstallKeyLater") ;
		BOOST_CHECK(proc_address) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif