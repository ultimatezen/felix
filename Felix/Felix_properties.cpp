#include "stdafx.h"
#include "Felix_properties.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( properties_loaded_historyTestCase, Constructor )
	{
		app_props::properties_loaded_history props ;
		for ( int i = 0 ; i < app_props::NumMems ; ++i )
		{
			ASSERT_EQUALS ( props.m_data.m_glosses[i][0], _T('\0') ) ; 
			ASSERT_EQUALS ( props.m_data.m_mems[i][0], _T('\0') ) ; 

			//ASSERT_EQUALS ( props.m_data.m_glosses[i][MAX_PATH-1], _T('\0') ) ; 
			//ASSERT_EQUALS ( props.m_data.m_mems[i][MAX_PATH-1], _T('\0') ) ; 

			//ASSERT_EQUALS ( props.m_data.m_glosses[i][MAX_PATH/2], _T('\0') ) ; 
			//ASSERT_EQUALS ( props.m_data.m_mems[i][MAX_PATH/2], _T('\0') ) ; 
		}


		ASSERT_EQUALS ( props.m_data.m_num_gloss, 0 ) ;
		ASSERT_EQUALS ( props.m_data.m_num_mems, 0 ) ;

		props.read_from_registry() ;

		_tcscpy_s( props.m_data.m_glosses[4], 260, _T("C:\\test.txt") ) ;
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		app_props::properties_loaded_history props2(props) ;

		ASSERT_EQUALS ( props.m_data.m_num_gloss, props2.m_data.m_num_gloss ) ; 
		ASSERT_EQUALS ( props.m_data.m_num_mems, props2.m_data.m_num_mems ) ; 

		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[0], props2.m_data.m_mems[0] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[1], props2.m_data.m_mems[1] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[2], props2.m_data.m_mems[2] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[3], props2.m_data.m_mems[3] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[4], props2.m_data.m_mems[4] ) ) ; 

		ASSERT_EQUALS ( 0, _tcscmp( props2.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[0], props2.m_data.m_glosses[0] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[1], props2.m_data.m_glosses[1] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[2], props2.m_data.m_glosses[2] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[3], props2.m_data.m_glosses[3] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[4], props2.m_data.m_glosses[4] ) ) ; 

	}

	TEST( properties_loaded_historyTestCase, AssignmentOperator )
	{
		app_props::properties_loaded_history props ;
		props.read_from_registry() ;

		_tcscpy_s( props.m_data.m_glosses[4], 260, _T("C:\\test.txt") ) ;
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		app_props::properties_loaded_history props2 ;

		props2 = props ;

		ASSERT_EQUALS ( props.m_data.m_num_gloss, props2.m_data.m_num_gloss ) ; 
		ASSERT_EQUALS ( props.m_data.m_num_mems, props2.m_data.m_num_mems ) ; 

		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[0], props2.m_data.m_mems[0] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[1], props2.m_data.m_mems[1] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[2], props2.m_data.m_mems[2] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[3], props2.m_data.m_mems[3] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_mems[4], props2.m_data.m_mems[4] ) ) ; 

		ASSERT_EQUALS ( 0, _tcscmp( props2.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[0], props2.m_data.m_glosses[0] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[1], props2.m_data.m_glosses[1] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[2], props2.m_data.m_glosses[2] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[3], props2.m_data.m_glosses[3] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( props.m_data.m_glosses[4], props2.m_data.m_glosses[4] ) ) ; 

	}

	TEST( properties_loaded_historyTestCase, ReadAndWrite )
	{
		app_props::properties_loaded_history old_props ;
		old_props.read_from_registry() ;

		app_props::properties_loaded_history props1 ;
		props1.read_from_registry() ;

	  _tcscpy_s( props1.m_data.m_mems[0], 260, _T("C:\\test.txt") ) ;
		props1.m_data.m_num_mems = 1 ;

		props1.write_to_registry() ;

		app_props::properties_loaded_history props2 ;
		props2.read_from_registry() ;

		ASSERT_EQUALS ( 0, _tcscmp( props2.m_data.m_mems[0], _T("C:\\test.txt") ) ) ; 
		ASSERT_EQUALS ( 1, props2.m_data.m_num_mems ) ; 

		old_props.write_to_registry() ;

		props2.read_from_registry() ;

		ASSERT_EQUALS ( old_props.m_data.m_num_gloss, props2.m_data.m_num_gloss ) ; 
		ASSERT_EQUALS ( old_props.m_data.m_num_mems, props2.m_data.m_num_mems ) ; 

		ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_mems[0], props2.m_data.m_mems[0] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_mems[1], props2.m_data.m_mems[1] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_mems[2], props2.m_data.m_mems[2] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_mems[3], props2.m_data.m_mems[3] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_mems[4], props2.m_data.m_mems[4] ) ) ; 


		//ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_glosses[0], props2.m_data.m_glosses[0] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_glosses[1], props2.m_data.m_glosses[1] ) ) ; 
		//ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_glosses[2], props2.m_data.m_glosses[2] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_glosses[3], props2.m_data.m_glosses[3] ) ) ; 
		ASSERT_EQUALS ( 0, _tcscmp( old_props.m_data.m_glosses[4], props2.m_data.m_glosses[4] ) ) ; 

	}

	TEST( properties_memoryTestCase, Constructor )
	{
		app_props::properties_memory props ;

		ASSERT_EQUALS ( props.m_data.m_min_score, 50u ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_case, TRUE ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_width, FALSE ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_hir_kat, FALSE ) ; 
		ASSERT_EQUALS ( props.m_data.m_plaintext, FALSE ) ; 
		ASSERT_EQUALS ( props.m_data.m_assess_format_penalty, FALSE ) ; 

		props.read_from_registry() ;
		app_props::properties_memory props2(props) ;

		ASSERT_EQUALS ( props.m_data.m_min_score,		props2.m_data.m_min_score ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_case,	props2.m_data.m_ignore_case ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_width, props2.m_data.m_ignore_width ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_hir_kat,			props2.m_data.m_ignore_hir_kat ) ; 
		ASSERT_EQUALS ( props.m_data.m_plaintext,					props2.m_data.m_plaintext ) ; 
		ASSERT_EQUALS ( props.m_data.m_assess_format_penalty, props2.m_data.m_assess_format_penalty ) ; 

		//props2.read_from_registry() ;

		//ASSERT_EQUALS ( props.m_data.m_min_score,		props2.m_data.m_min_score ) ; 
		//ASSERT_EQUALS ( props.m_data.m_ignore_case,	props2.m_data.m_ignore_case ) ; 
		//ASSERT_EQUALS ( props.m_data.m_ignore_width, props2.m_data.m_ignore_width ) ; 
		//ASSERT_EQUALS ( props.m_data.m_ignore_hir_kat,			props2.m_data.m_ignore_hir_kat ) ; 
		//ASSERT_EQUALS ( props.m_data.m_plaintext,					props2.m_data.m_plaintext ) ; 
		//ASSERT_EQUALS ( props.m_data.m_assess_format_penalty, props2.m_data.m_assess_format_penalty ) ; 
	}

	TEST( properties_glossaryTestCase, Constructor )
	{
		app_props::properties_glossary props ;

		ASSERT_EQUALS ( props.m_data.m_min_score, 100u ) ; 
		ASSERT_EQUALS ( props.m_data.m_max_add, 15u ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_case, TRUE ) ; 
		ASSERT_EQUALS ( props.m_data.m_plaintext, FALSE ) ; 
		ASSERT_EQUALS ( props.m_data.m_to_lower, FALSE ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_width, FALSE ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_hir_kat, FALSE ) ; 

		app_props::properties_glossary props2(props) ;

		ASSERT_EQUALS ( props.m_data.m_min_score,			props2.m_data.m_min_score ) ; 
		ASSERT_EQUALS ( props.m_data.m_max_add,			props2.m_data.m_max_add ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_case,		props2.m_data.m_ignore_case ) ; 
		ASSERT_EQUALS ( props.m_data.m_plaintext,			props2.m_data.m_plaintext ) ; 
		ASSERT_EQUALS ( props.m_data.m_to_lower,			props2.m_data.m_to_lower ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_width,		props2.m_data.m_ignore_width ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_hir_kat,	props2.m_data.m_ignore_hir_kat ) ; 

		//props.read_from_registry() ;
		//props2.read_from_registry() ;

		//ASSERT_EQUALS ( props.m_data.m_min_score,			props2.m_data.m_min_score ) ; 
		//ASSERT_EQUALS ( props.m_data.m_max_add,			props2.m_data.m_max_add ) ; 
		//ASSERT_EQUALS ( props.m_data.m_ignore_case,		props2.m_data.m_ignore_case ) ; 
		//ASSERT_EQUALS ( props.m_data.m_plaintext,			props2.m_data.m_plaintext ) ; 
		//ASSERT_EQUALS ( props.m_data.m_to_lower,			props2.m_data.m_to_lower ) ; 
		//ASSERT_EQUALS ( props.m_data.m_ignore_width,		props2.m_data.m_ignore_width ) ; 
		//ASSERT_EQUALS ( props.m_data.m_ignore_hir_kat,	props2.m_data.m_ignore_hir_kat ) ; 

	}

	TEST( properties_glossaryTestCase, StoreValsInRegistry )
	{
		app_props::properties_glossary props ;
		app_props::properties_glossary props2 ;
		app_props::properties_glossary props3 ;
		props3.read_from_registry() ;

		props.m_data.m_min_score = 53u ;
		props.m_data.m_max_add, 37u ;
		props.m_data.m_ignore_case = TRUE ; 
		props.m_data.m_plaintext = TRUE ; 
		props.m_data.m_to_lower = TRUE ; 
		props.m_data.m_ignore_width = TRUE ; 
		props.m_data.m_ignore_hir_kat = TRUE ; 

		props.write_to_registry() ;
		props2.read_from_registry() ;

		ASSERT_EQUALS ( props.m_data.m_min_score,			props2.m_data.m_min_score ) ; 
		ASSERT_EQUALS ( props.m_data.m_max_add,			props2.m_data.m_max_add ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_case,		props2.m_data.m_ignore_case ) ; 
		ASSERT_EQUALS ( props.m_data.m_plaintext,			props2.m_data.m_plaintext ) ; 
		ASSERT_EQUALS ( props.m_data.m_to_lower,			props2.m_data.m_to_lower ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_width,		props2.m_data.m_ignore_width ) ; 
		ASSERT_EQUALS ( props.m_data.m_ignore_hir_kat,	props2.m_data.m_ignore_hir_kat ) ; 

		props3.write_to_registry() ;
		//props2.read_from_registry() ;

		//ASSERT_EQUALS ( props3.m_data.m_min_score,			props2.m_data.m_min_score ) ; 
		//ASSERT_EQUALS ( props3.m_data.m_max_add,			props2.m_data.m_max_add ) ; 
		//ASSERT_EQUALS ( props3.m_data.m_ignore_case,		props2.m_data.m_ignore_case ) ; 
		//ASSERT_EQUALS ( props3.m_data.m_plaintext,			props2.m_data.m_plaintext ) ; 
		//ASSERT_EQUALS ( props3.m_data.m_to_lower,			props2.m_data.m_to_lower ) ; 
		//ASSERT_EQUALS ( props3.m_data.m_ignore_width,		props2.m_data.m_ignore_width ) ; 
		//ASSERT_EQUALS ( props3.m_data.m_ignore_hir_kat,	props2.m_data.m_ignore_hir_kat ) ; 

	}

	TEST( properties_algoTestCase, StoreValsInRegistry )
	{
		app_props::properties_algorithm props ;
		app_props::properties_algorithm props2 ;
		app_props::properties_algorithm props3 ;
		props3.read_from_registry() ;

		props.m_data.m_match_algo = IDC_ALGO_CHAR ;

		props.write_to_registry() ;
		props2.read_from_registry() ;

		ASSERT_EQUALS ( props.m_data.m_match_algo,			props2.m_data.m_match_algo ) ; 

		//props3.write_to_registry() ;
		//props2.read_from_registry() ;

		//ASSERT_EQUALS ( props3.m_data.m_gloss_match_algo,	props2.m_data.m_gloss_match_algo ) ; 
		//ASSERT_EQUALS ( props3.m_data.m_match_algo,			props2.m_data.m_match_algo ) ; 

		//props.m_data.m_gloss_match_algo = 1 ;
		//props.m_data.m_match_algo = 1 ;

		//props.write_to_registry() ;
		//props2.read_from_registry() ;

		//ASSERT_EQUALS ( IDC_ALGO_CHAR_GLOSS,	props2.m_data.m_gloss_match_algo ) ; 
		//ASSERT_EQUALS ( IDC_ALGO_CHAR,			props2.m_data.m_match_algo ) ; 

		props3.write_to_registry() ;
	}

}

#endif // #ifdef _DEBUG
