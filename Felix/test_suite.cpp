#include "test_suite.h"
#include "StdAfx.h"

#include "namespace_file_test_harness.h"
#include "array_tester.h"
#include "basic_buffer_tester.h"
#include "matrix_test_harness.h"
#include "mainframe_tester.h"
#include "translation_record_tester.h"
#include "test_string_stuff.h"

#ifdef _DEBUG


void run_test_suite()
{
	run_namespace_file_tests() ;
	test_Array() ;
	test_basic_buffer() ;
	run_matrix_tests() ;

	test_translation_record() ;

	test_string_stuff() ;

	test_mainframe() ;

	MessageBeep( MB_ICONINFORMATION ) ;
}

#endif // #ifdef _DEBUG
