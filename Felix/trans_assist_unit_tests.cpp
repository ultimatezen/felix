#include "stdafx.h"
#include "trans_assist_unit_tests.h"
#include "test_suite.h"

#include "namespace_file_test_harness.h"
#include "array_tester.h"
#include "basic_buffer_tester.h"
#include "matrix_test_harness.h"
#include "translation_record_tester.h"
#include "test_string_stuff.h"

#ifdef _DEBUG


void do_unit_tests()
{
	run_namespace_file_tests() ;
	test_Array() ;
	test_basic_buffer() ;
	run_matrix_tests() ;

	test_translation_record() ;

	test_string_stuff() ;

}

#endif // ifdef _DEBUG
