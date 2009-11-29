#include "stdafx.h"
#include "Library.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST(test_library, null_arg)
	{
		CLibrary library(NULL) ;
		ASSERT_TRUE(! library.is_loaded()) ;
	}
	TEST(test_library, rich_edit)
	{
		CLibrary library(_T("RICHED20.DLL")) ;
		ASSERT_TRUE(library.is_loaded()) ;
	}
	TEST(test_library, get_proc_address)
	{
		CLibrary library(_T("ArmAccess.DLL")) ;
		ASSERT_TRUE(library.is_loaded()) ;
		FARPROC proc_address = library.get_proc_address("InstallKeyLater") ;
		ASSERT_TRUE(proc_address) ;
	}

}


#endif