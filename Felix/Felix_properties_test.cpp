// ***************************************************************
//  @brief Unit tests for Felix_properties
//  -------------------------------------------------------------
//  match_score_test   version:  1.0   |  @date: 2006-07-09
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "StdAfx.h"
#include "Felix_properties.h"
#include "output_device_fake.h"
#include "config_file.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

struct xml_string_writer_test: pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};
BOOST_AUTO_TEST_SUITE( test_properties )

	using namespace app_props ;

	BOOST_AUTO_TEST_CASE(test_get_config_filename)
	{
		output_device_ptr device(new OutputDeviceFake) ;
		CString filename = L"felix_rules.frules" ;
		wstring actual = get_config_filename(filename, device) ;
		wstring expected = L"D:\\Users\\Ryan\\AppData\\Local\\Felix\\prefs\\felix_rules.frules" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}


	BOOST_AUTO_TEST_CASE(assignment_memory)
	{
		props_ptr props1(new properties) ;
		props_ptr props2(new properties) ;

		props2->m_mem_props.set_min_score(13u) ;

		*props1 = *props2 ;
		BOOST_CHECK_EQUAL(13u, props1->m_mem_props.get_min_score()) ;
	}

	BOOST_AUTO_TEST_CASE(assignment_loaded_history_mems)
	{
		props_ptr props1(new properties) ;
		props_ptr props2(new properties) ;

		props2->m_history_props.m_loaded_mems.push_back(L"foo") ;

		*props1 = *props2 ;
		BOOST_CHECK_EQUAL(1u, props1->m_history_props.m_loaded_mems.size()) ;
		BOOST_CHECK_EQUAL(L"foo", props1->m_history_props.m_loaded_mems[0]) ;
	}

	BOOST_AUTO_TEST_CASE(assignment_loaded_history_gloss)
	{
		props_ptr props1(new properties) ;
		props_ptr props2(new properties) ;

		props2->m_history_props.m_loaded_gloss.push_back(L"foo") ;

		*props1 = *props2 ;
		BOOST_CHECK_EQUAL(1u, props1->m_history_props.m_loaded_gloss.size()) ;
		BOOST_CHECK_EQUAL(L"foo", props1->m_history_props.m_loaded_gloss[0]) ;
	}

	BOOST_AUTO_TEST_CASE(assignment_loaded_history_remote_mems)
	{
		props_ptr props1(new properties) ;
		props_ptr props2(new properties) ;

		props2->m_history_props.m_loaded_remote_mems.push_back(L"foo") ;

		*props1 = *props2 ;
		BOOST_CHECK_EQUAL(1u, props1->m_history_props.m_loaded_remote_mems.size()) ;
		BOOST_CHECK_EQUAL(L"foo", props1->m_history_props.m_loaded_remote_mems[0]) ;
	}

	BOOST_AUTO_TEST_CASE(assignment_loaded_history_remote_gloss)
	{
		props_ptr props1(new properties) ;
		props_ptr props2(new properties) ;

		props2->m_history_props.m_loaded_remote_gloss.push_back(L"foo") ;

		*props1 = *props2 ;
		BOOST_CHECK_EQUAL(1u, props1->m_history_props.m_loaded_remote_gloss.size()) ;
		BOOST_CHECK_EQUAL(L"foo", props1->m_history_props.m_loaded_remote_gloss[0]) ;
	}


	BOOST_AUTO_TEST_CASE(assignment__glossary)
	{
		props_ptr props1(new properties) ;
		props_ptr props2(new properties) ;

		props2->m_gloss_props.set_min_score(37u) ;

		*props1 = *props2 ;
		BOOST_CHECK_EQUAL(37u, props1->m_gloss_props.get_min_score()) ;
	}

	BOOST_AUTO_TEST_CASE(assignment_algorithm)
	{
		props_ptr props1(new properties) ;
		props_ptr props2(new properties) ;

		props2->m_alg_props.m_data.m_match_algo = 101 ;

		*props1 = *props2 ;
		BOOST_CHECK_EQUAL(101, props1->m_alg_props.m_data.m_match_algo) ;
	}

BOOST_AUTO_TEST_SUITE_END()
// test_properties_loaded_history
BOOST_AUTO_TEST_SUITE( test_properties_loaded_history )

	using namespace app_props ;

	BOOST_AUTO_TEST_CASE(initial_values)
	{
		properties_loaded_history props ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_mems) ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_gloss) ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_remote_mems) ;
		BOOST_CHECK_EQUAL(0, props.m_data.m_num_remote_gloss) ;

		for (int i = 0 ; i < app_props::NumMems ; ++i)
		{
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_mems[i])) ;
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_glosses[i])) ;
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_remote_mems[i])) ;
			BOOST_CHECK_EQUAL( 0u, _tcslen(props.m_data.m_remote_glosses[i])) ;
		}
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_properties_general )

	using namespace app_props ;

	BOOST_AUTO_TEST_CASE(initial_values)
	{
		properties_general props ;
		BOOST_CHECK_EQUAL((int)IDC_MEMSIZE_FULL, props.m_data.m_window_size) ;
		BOOST_CHECK_EQUAL((int)LANG_ENGLISH, props.m_data.m_preferred_gui_lang) ;
		BOOST_CHECK(! props.m_data.m_load_prev_mem_on_startup) ;
		BOOST_CHECK(! props.m_data.m_load_prev_gloss_on_startup) ;
		BOOST_CHECK(props.m_data.m_show_markup) ;
		BOOST_CHECK(props.m_data.m_first_launch) ;
		BOOST_CHECK_EQUAL((int)IDC_MERGE, props.m_data.m_merge_choice) ;
		BOOST_CHECK(props.m_data.m_query_merge) ;
		BOOST_CHECK(! props.m_data.m_old_mem_mgr) ;
		BOOST_CHECK_EQUAL(L"Ryan", wstring(props.m_data.m_user_name)) ;
	}


	BOOST_AUTO_TEST_CASE( test_NumMems_15 )
	{
		BOOST_CHECK_EQUAL(15, NumMems) ;
	}
	BOOST_AUTO_TEST_CASE( user_name_default_Ryan)
	{
		properties_general props ;
		wstring actual(props.m_data.m_user_name); 
		wstring expected = L"Ryan" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( properties_loaded_historyTestCase )

	BOOST_AUTO_TEST_CASE( Constructor )
	{
		app_props::properties_loaded_history props ;
		for ( int i = 0 ; i < app_props::NumMems ; ++i )
		{
			BOOST_CHECK_EQUAL ( props.m_data.m_glosses[i][0], _T('\0') ) ; 
			BOOST_CHECK_EQUAL ( props.m_data.m_mems[i][0], _T('\0') ) ; 
			BOOST_CHECK_EQUAL ( props.m_data.m_remote_mems[i][0], _T('\0') ) ; 
			BOOST_CHECK_EQUAL ( props.m_data.m_remote_glosses[i][0], _T('\0') ) ; 
		}


		BOOST_CHECK_EQUAL ( props.m_data.m_num_gloss, 0 ) ;
		BOOST_CHECK_EQUAL ( props.m_data.m_num_mems, 0 ) ;
		BOOST_CHECK_EQUAL ( props.m_data.m_num_remote_mems, 0 ) ;
		BOOST_CHECK_EQUAL ( props.m_data.m_num_remote_gloss, 0 ) ;

		props.read_from_registry() ;

		_tcscpy_s( props.m_data.m_glosses[4], 260, _T("C:\\test.txt") ) ;
		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		app_props::properties_loaded_history props2(props) ;

		BOOST_CHECK_EQUAL ( props.m_data.m_num_gloss, props2.m_data.m_num_gloss ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_num_mems, props2.m_data.m_num_mems ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_mems[2], props2.m_data.m_mems[2] ) ) ; 
		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_mems[3], props2.m_data.m_mems[3] ) ) ; 
		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_mems[4], props2.m_data.m_mems[4] ) ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( props2.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_glosses[0], props2.m_data.m_glosses[0] ) ) ; 
		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_glosses[1], props2.m_data.m_glosses[1] ) ) ; 
	}

	BOOST_AUTO_TEST_CASE( AssignmentOperator )
	{
		app_props::properties_loaded_history props ;
		props.read_from_registry() ;

		_tcscpy_s( props.m_data.m_glosses[4], 260, _T("C:\\test.txt") ) ;
		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		app_props::properties_loaded_history props2 ;

		props2 = props ;

		BOOST_CHECK_EQUAL ( props.m_data.m_num_gloss, props2.m_data.m_num_gloss ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_num_mems, props2.m_data.m_num_mems ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_mems[0], props2.m_data.m_mems[0] ) ) ; 
		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_mems[1], props2.m_data.m_mems[1] ) ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( props2.m_data.m_glosses[4], _T("C:\\test.txt") ) ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_glosses[3], props2.m_data.m_glosses[3] ) ) ; 
		BOOST_CHECK_EQUAL ( 0, _tcscmp( props.m_data.m_glosses[4], props2.m_data.m_glosses[4] ) ) ; 

	}

	BOOST_AUTO_TEST_CASE( ReadAndWrite )
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

		BOOST_CHECK_EQUAL ( 0, _tcscmp( props2.m_data.m_mems[0], _T("C:\\test.txt") ) ) ; 
		BOOST_CHECK_EQUAL ( 1, props2.m_data.m_num_mems ) ; 

		old_props.write_to_registry() ;

		props2.read_from_registry() ;

		BOOST_CHECK_EQUAL ( old_props.m_data.m_num_gloss, props2.m_data.m_num_gloss ) ; 
		BOOST_CHECK_EQUAL ( old_props.m_data.m_num_mems, props2.m_data.m_num_mems ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( old_props.m_data.m_mems[0], props2.m_data.m_mems[0] ) ) ; 
		BOOST_CHECK_EQUAL ( 0, _tcscmp( old_props.m_data.m_mems[1], props2.m_data.m_mems[1] ) ) ; 

		BOOST_CHECK_EQUAL ( 0, _tcscmp( old_props.m_data.m_glosses[3], props2.m_data.m_glosses[3] ) ) ; 
		BOOST_CHECK_EQUAL ( 0, _tcscmp( old_props.m_data.m_glosses[4], props2.m_data.m_glosses[4] ) ) ; 

	}

	BOOST_AUTO_TEST_CASE( copy_reg_props_empty )
	{
		app_props::properties_loaded_history props ;
		props.copy_reg_props() ;

		BOOST_CHECK_EQUAL ( props.m_loaded_mems.size(), 0u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss.size(), 0u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems.size(), 0u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss.size(), 0u ) ; 
	}
	BOOST_AUTO_TEST_CASE( copy_reg_props_1_mem )
	{
		app_props::properties_loaded_history props ;
		props.m_data.m_num_mems = 1 ;
		_tcscpy_s( props.m_data.m_mems[0], 260, _T("C:\\test.txt") ) ;

		props.copy_reg_props() ;

		BOOST_CHECK_EQUAL ( props.m_loaded_mems.size(), 1u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_mems[0], L"C:\\test.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss.size(), 0u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems.size(), 0u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss.size(), 0u ) ; 
	}
	BOOST_AUTO_TEST_CASE( copy_reg_props_1_each )
	{
		app_props::properties_loaded_history props ;
		props.m_data.m_num_mems = 1 ;
		props.m_data.m_num_gloss = 1 ;
		props.m_data.m_num_remote_mems = 1 ;
		props.m_data.m_num_remote_gloss = 1 ;
		_tcscpy_s( props.m_data.m_mems[0], 260, _T("C:\\m_mems.txt") ) ;
		_tcscpy_s( props.m_data.m_glosses[0], 260, _T("C:\\m_glosses.txt") ) ;
		_tcscpy_s( props.m_data.m_remote_mems[0], 260, _T("C:\\m_remote_mems.txt") ) ;
		_tcscpy_s( props.m_data.m_remote_glosses[0], 260, _T("C:\\m_remote_glosses.txt") ) ;

		props.copy_reg_props() ;

		BOOST_CHECK_EQUAL ( props.m_loaded_mems[0], L"C:\\m_mems.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss[0], L"C:\\m_glosses.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems[0], L"C:\\m_remote_mems.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss[0], L"C:\\m_remote_glosses.txt" ) ; 
	}
	BOOST_AUTO_TEST_CASE( copy_reg_props_2_mems )
	{
		app_props::properties_loaded_history props ;
		props.m_data.m_num_mems = 2 ;
		_tcscpy_s( props.m_data.m_mems[0], 260, _T("C:\\mem1.txt") ) ;
		_tcscpy_s( props.m_data.m_mems[1], 260, _T("C:\\mem2.txt") ) ;

		props.copy_reg_props() ;

		BOOST_CHECK_EQUAL ( props.m_loaded_mems.size(), 2u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_mems[0], L"C:\\mem1.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_mems[1], L"C:\\mem2.txt" ) ; 
	}

BOOST_AUTO_TEST_SUITE_END()



// properties_memory
BOOST_AUTO_TEST_SUITE( properties_memoryTestCase )

	BOOST_AUTO_TEST_CASE( Constructor )
	{
		app_props::properties_memory props ;

		BOOST_CHECK_EQUAL ( props.m_data.m_min_score, 50u ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_case, TRUE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_width, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_hir_kat, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_plaintext, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_place_gloss, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_place_numbers, TRUE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_place_rules, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_assess_format_penalty, FALSE ) ; 

		props.read_from_registry() ;
		app_props::properties_memory props2(props) ;

		BOOST_CHECK_EQUAL ( props.m_data.m_min_score,		props2.m_data.m_min_score ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_case,		props2.m_data.m_ignore_case ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_width,	props2.m_data.m_ignore_width ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_hir_kat,	props2.m_data.m_ignore_hir_kat ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_plaintext,		props2.m_data.m_plaintext ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_assess_format_penalty, props2.m_data.m_assess_format_penalty ) ; 
	}
BOOST_AUTO_TEST_SUITE_END()

// properties_glossary
BOOST_AUTO_TEST_SUITE( properties_glossaryTestCase )

	BOOST_AUTO_TEST_CASE( Constructor )
	{
		app_props::properties_glossary props ;

		BOOST_CHECK_EQUAL ( props.m_data.m_min_score, 100u ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_max_add, 15u ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_case, TRUE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_plaintext, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_to_lower, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_width, FALSE ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_hir_kat, FALSE ) ; 

		app_props::properties_glossary props2(props) ;

		BOOST_CHECK_EQUAL ( props.m_data.m_min_score,		props2.m_data.m_min_score ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_max_add,			props2.m_data.m_max_add ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_case,		props2.m_data.m_ignore_case ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_plaintext,		props2.m_data.m_plaintext ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_to_lower,		props2.m_data.m_to_lower ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_width,	props2.m_data.m_ignore_width ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_hir_kat,	props2.m_data.m_ignore_hir_kat ) ; 
	}

	BOOST_AUTO_TEST_CASE( StoreValsInRegistry )
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

		BOOST_CHECK_EQUAL ( props.m_data.m_min_score,		props2.m_data.m_min_score ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_max_add,			props2.m_data.m_max_add ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_case,		props2.m_data.m_ignore_case ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_plaintext,		props2.m_data.m_plaintext ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_to_lower,		props2.m_data.m_to_lower ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_width,	props2.m_data.m_ignore_width ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_hir_kat,	props2.m_data.m_ignore_hir_kat ) ; 

		props3.write_to_registry() ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( properties_algoTestCase )

	BOOST_AUTO_TEST_CASE( StoreValsInRegistry )
	{
		app_props::properties_algorithm props ;
		app_props::properties_algorithm props2 ;
		app_props::properties_algorithm props3 ;
		props3.read_from_registry() ;

		props.m_data.m_match_algo = IDC_ALGO_CHAR ;

		props.write_to_registry() ;
		props2.read_from_registry() ;

		BOOST_CHECK_EQUAL ( props.m_data.m_match_algo, props2.m_data.m_match_algo ) ; 

		props3.write_to_registry() ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( properties_qc_TestCase )

	BOOST_AUTO_TEST_CASE( init )
	{
		app_props::properties_qc props ;

		BOOST_CHECK( ! props.m_data.m_check_numbers ) ; 
		BOOST_CHECK( ! props.m_data.m_check_all_caps ) ; 
		BOOST_CHECK( ! props.m_data.m_check_gloss ) ; 
	}
	BOOST_AUTO_TEST_CASE( copy_constructor )
	{
		app_props::properties_qc props ;

		props.m_data.m_check_numbers = TRUE ; 
		props.m_data.m_check_all_caps = TRUE ; 
		props.m_data.m_check_gloss = TRUE ; 

		app_props::properties_qc props2(props) ;

		BOOST_CHECK( props.m_data.m_check_numbers ) ; 
		BOOST_CHECK( props.m_data.m_check_all_caps ) ; 
		BOOST_CHECK( props.m_data.m_check_gloss ) ; 
	}
	BOOST_AUTO_TEST_CASE( StoreValsInRegistry )
	{
		app_props::properties_qc props ;
		props.read_from_registry() ;
		app_props::properties_qc old_props(props) ;

		props.m_data.m_check_numbers = ! props.m_data.m_check_numbers ; 
		props.m_data.m_check_all_caps = ! props.m_data.m_check_all_caps ; 
		props.m_data.m_check_gloss = ! props.m_data.m_check_gloss ; 

		props.write_to_registry() ;
		app_props::properties_qc new_props ;
		new_props.read_from_registry() ;

		BOOST_CHECK_EQUAL ( props.m_data.m_check_numbers,	new_props.m_data.m_check_numbers ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_check_all_caps,	new_props.m_data.m_check_all_caps ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_check_gloss,		new_props.m_data.m_check_gloss ) ; 

		old_props.write_to_registry() ;
		new_props.read_from_registry() ;

		BOOST_CHECK_EQUAL ( old_props.m_data.m_check_numbers,	new_props.m_data.m_check_numbers ) ; 
		BOOST_CHECK_EQUAL ( old_props.m_data.m_check_all_caps,	new_props.m_data.m_check_all_caps ) ; 
		BOOST_CHECK_EQUAL ( old_props.m_data.m_check_gloss,		new_props.m_data.m_check_gloss ) ; 

	}
	BOOST_AUTO_TEST_CASE( qc_enabled_numbers )
	{
		app_props::properties_qc props ;

		BOOST_CHECK(! props.qc_enabled()) ;

		props.m_data.m_check_numbers = TRUE ;
		BOOST_CHECK(props.qc_enabled()) ;
	}
	BOOST_AUTO_TEST_CASE( qc_enabled_all_caps )
	{
		app_props::properties_qc props ;
		props.m_data.m_check_all_caps = TRUE ;
		BOOST_CHECK(props.qc_enabled()) ;
	}
	BOOST_AUTO_TEST_CASE( qc_enabled_gloss )
	{
		app_props::properties_qc props ;
		props.m_data.m_check_gloss = TRUE ;
		BOOST_CHECK(props.qc_enabled()) ;
	}
	BOOST_AUTO_TEST_CASE( live_checking )
	{
		app_props::properties_qc props ;
		BOOST_CHECK(!props.live_checking()) ;
		props.m_data.m_live_checking = TRUE ;
		BOOST_CHECK(props.live_checking()) ;
	}
	BOOST_AUTO_TEST_CASE(test_load_qc_props_no_glossnames)
	{
		string text = "<properties>\n"
			"<properties_qc>\n"
			"<check_numbers>true</check_numbers>\n"
			"<check_all_caps>true</check_all_caps>\n"
			"<check_gloss>true</check_gloss>\n"
			"<live_checking>true</live_checking>\n"
			"</properties_qc>\n"
			"</properties>\n" ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_qc props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK( props.m_data.m_check_numbers ) ; 
		BOOST_CHECK( props.m_data.m_check_all_caps ) ; 
		BOOST_CHECK( props.m_data.m_check_gloss ) ; 
		BOOST_CHECK( props.m_data.m_live_checking ) ; 
		BOOST_CHECK_EQUAL( 0u, props.m_qc_glosses.size() ) ;
	}
	BOOST_AUTO_TEST_CASE(test_load_qc_props_glossnames)
	{
		string text = "<properties>\n"
			" <properties_qc>\n"
			"  <check_numbers>true</check_numbers>\n"
			"  <check_all_caps>true</check_all_caps>\n"
			"  <check_gloss>true</check_gloss>\n"
			"  <live_checking>true</live_checking>\n"
			"  <qc_glosses>\n" 
			"    <filename>first.fgloss</filename>\n"
			"    <filename>http://127.0.0.1:8300/mems/2/</filename>\n"
			"  </qc_glosses>\n" 
			" </properties_qc>\n"
		"</properties>\n" ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_qc props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK( props.m_data.m_check_numbers ) ; 
		BOOST_CHECK( props.m_data.m_check_all_caps ) ; 
		BOOST_CHECK( props.m_data.m_check_gloss ) ; 
		BOOST_CHECK( props.m_data.m_live_checking ) ;
		wstring expected1 = L"first.fgloss" ;
		wstring expected2 = L"http://127.0.0.1:8300/mems/2/" ;
		BOOST_CHECK_EQUAL( 2u, props.m_qc_glosses.size() ) ;
		BOOST_CHECK_EQUAL( props.m_qc_glosses[0], expected1 ) ;
		BOOST_CHECK_EQUAL( props.m_qc_glosses[1], expected2 ) ;
	}
BOOST_AUTO_TEST_SUITE_END()

/************************************************************************/
/* XML TESTS                                                            */
/************************************************************************/

// properties_loaded_history

BOOST_AUTO_TEST_SUITE( properties_loaded_history_xml_tests )

	BOOST_AUTO_TEST_CASE(test_load_xml_props_type)
	{
		string text = "<props><filenames>\n"
			"<filename>first.txt</filename>\n"
			"<filename>second.txt</filename>\n"
			"</filenames></props>\n" ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_loaded_history props ;
		std::vector<wstring> items ;
		pugi::xml_node child = doc.child("props") ;
		app_props::load_xml_props_type(child, items, "filenames") ;

		wstring expected1 = L"first.txt" ;
		wstring expected2 = L"second.txt" ;
		BOOST_CHECK_EQUAL (items.size(), 2u) ; 
		BOOST_CHECK_EQUAL (items[0], expected1) ; 
		BOOST_CHECK_EQUAL (items[1], expected2) ; 
	}

	BOOST_AUTO_TEST_CASE( load_xml_loaded_history )
	{
		string text = "<loaded_history>\n"
			"<loaded_mems>\n"
			"<filename>foo.txt</filename>\n"
			"<filename>bar.txt</filename>\n"
			"</loaded_mems>\n"
			"</loaded_history>\n" ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_loaded_history props ;
		std::vector<wstring> items ;
		pugi::xml_node child = doc.child("loaded_history") ;
		app_props::load_xml_props_type(child, items, "loaded_mems") ;

		wstring expected1 = L"foo.txt" ;
		wstring expected2 = L"bar.txt" ;
		BOOST_CHECK_EQUAL (items.size(), 2u) ; 
		BOOST_CHECK_EQUAL (items[0], expected1) ; 
		BOOST_CHECK_EQUAL (items[1], expected2) ; 
	}

	BOOST_AUTO_TEST_CASE( load_xml_loaded_history_two_nodes )
	{
		string text = "<loaded_history>\n"
			"<loaded_gloss>\n"
			"<filename>gfoo.txt</filename>\n"
			"<filename>gbar.txt</filename>\n"
			"</loaded_gloss>\n"
			"<loaded_mems>\n"
			"<filename>foo.txt</filename>\n"
			"<filename>bar.txt</filename>\n"
			"</loaded_mems>\n"
			"</loaded_history>\n" ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_loaded_history props ;
		std::vector<wstring> items ;
		pugi::xml_node child = doc.child("loaded_history") ;
		app_props::load_xml_props_type(child, items, "loaded_mems") ;

		wstring expected1 = L"foo.txt" ;
		wstring expected2 = L"bar.txt" ;
		BOOST_CHECK_EQUAL (items.size(), 2u) ; 
		BOOST_CHECK_EQUAL (items[0], expected1) ; 
		BOOST_CHECK_EQUAL (items[1], expected2) ; 
	}

	BOOST_AUTO_TEST_CASE( load_xml_doc )
	{
		string text = "<loaded_history>\n"
			"<loaded_gloss>\n"
			"<filename>gfoo.txt</filename>\n"
			"<filename>gbar.txt</filename>\n"
			"</loaded_gloss>\n"
			"<loaded_mems>\n"
			"<filename>foo.txt</filename>\n"
			"<filename>bar.txt</filename>\n"
			"</loaded_mems>\n"
			"<loaded_remote_mems>\n"
			"<filename>rmfoo.txt</filename>\n"
			"<filename>rmbar.txt</filename>\n"
			"</loaded_remote_mems>\n"
			"<loaded_remote_gloss>\n"
			"<filename>rgfoo.txt</filename>\n"
			"<filename>rgbar.txt</filename>\n"
			"</loaded_remote_gloss>\n"
			"</loaded_history>\n" ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_loaded_history props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL ( props.m_loaded_mems.size(), 2u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss.size(), 2u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems.size(), 2u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss.size(), 2u ) ; 


		BOOST_CHECK_EQUAL ( props.m_loaded_mems[0], L"foo.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss[0], L"gfoo.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems[0], L"rmfoo.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss[0], L"rgfoo.txt" ) ; 

		BOOST_CHECK_EQUAL ( props.m_loaded_mems[1], L"bar.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss[1], L"gbar.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems[1], L"rmbar.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss[1], L"rgbar.txt" ) ; 

	}

	BOOST_AUTO_TEST_CASE( load_xml_doc_full )
	{
		string text = "<properties>\n"
			"<loaded_history>\n"
			"<loaded_gloss>\n"
			"<filename>gfoo.txt</filename>\n"
			"<filename>gbar.txt</filename>\n"
			"</loaded_gloss>\n"
			"<loaded_mems>\n"
			"<filename>foo.txt</filename>\n"
			"<filename>bar.txt</filename>\n"
			"</loaded_mems>\n"
			"<loaded_remote_mems>\n"
			"<filename>rmfoo.txt</filename>\n"
			"<filename>rmbar.txt</filename>\n"
			"</loaded_remote_mems>\n"
			"<loaded_remote_gloss>\n"
			"<filename>rgfoo.txt</filename>\n"
			"<filename>rgbar.txt</filename>\n"
			"</loaded_remote_gloss>\n"
			"</loaded_history>\n"
			"</properties>\n" ;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_loaded_history props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL ( props.m_loaded_mems.size(), 2u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss.size(), 2u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems.size(), 2u ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss.size(), 2u ) ; 


		BOOST_CHECK_EQUAL ( props.m_loaded_mems[0], L"foo.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss[0], L"gfoo.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems[0], L"rmfoo.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss[0], L"rgfoo.txt" ) ; 

		BOOST_CHECK_EQUAL ( props.m_loaded_mems[1], L"bar.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_gloss[1], L"gbar.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_mems[1], L"rmbar.txt" ) ; 
		BOOST_CHECK_EQUAL ( props.m_loaded_remote_gloss[1], L"rgbar.txt" ) ; 

	}
	BOOST_AUTO_TEST_CASE(write_xml_text)
	{
		app_props::properties_loaded_history props ;
		props.m_data.m_num_mems = 1 ;
		props.m_data.m_num_gloss = 1 ;
		props.m_data.m_num_remote_mems = 1 ;
		props.m_data.m_num_remote_gloss = 1 ;
		_tcscpy_s( props.m_data.m_mems[0], 260, _T("C:\\m_mems.txt") ) ;
		_tcscpy_s( props.m_data.m_glosses[0], 260, _T("C:\\m_glosses.txt") ) ;
		_tcscpy_s( props.m_data.m_remote_mems[0], 260, _T("C:\\m_remote_mems.txt") ) ;
		_tcscpy_s( props.m_data.m_remote_glosses[0], 260, _T("C:\\m_remote_glosses.txt") ) ;

		props.copy_reg_props() ;

		string text = props.make_xml_doc() ;
		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<loaded_history>\n"
			"		<loaded_mems>\n"
			"			<filename>C:\\m_mems.txt</filename>\n"
			"		</loaded_mems>\n"
			"		<loaded_gloss>\n"
			"			<filename>C:\\m_glosses.txt</filename>\n"
			"		</loaded_gloss>\n"
			"		<loaded_remote_mems>\n"
			"			<filename>C:\\m_remote_mems.txt</filename>\n"
			"		</loaded_remote_mems>\n"
			"		<loaded_remote_gloss>\n"
			"			<filename>C:\\m_remote_glosses.txt</filename>\n"
			"		</loaded_remote_gloss>\n"
			"	</loaded_history>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(text, expected) ;
	}

BOOST_AUTO_TEST_SUITE_END()

// properties_memory
BOOST_AUTO_TEST_SUITE( properties_memory_xml_tests )

	BOOST_AUTO_TEST_CASE( load_xml )
	{
		string text = "<properties>\n"
			"<properties_memory>\n"
			"<min_score>42</min_score>\n"
			// normalize
			"<ignore_case>false</ignore_case>\n"
			"<ignore_width>true</ignore_width>\n"
			"<ignore_hir_kat>true</ignore_hir_kat>\n"
			// formatting
			"<plaintext>true</plaintext>\n"
			"<assess_format_penalty>true</assess_format_penalty>\n"
			// placements
			"<place_numbers>false</place_numbers>\n"
			"<place_gloss>true</place_gloss>\n"
			"<place_rules>true</place_rules>\n"
			"<one_trans_per_source>true</one_trans_per_source>\n"
			"</properties_memory>\n" 
			"</properties>";

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_memory props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL (props.m_data.m_min_score, 42u) ; 

		BOOST_CHECK_EQUAL (props.m_data.m_ignore_case, FALSE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_ignore_width, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_ignore_hir_kat, TRUE) ; 

		BOOST_CHECK_EQUAL (props.m_data.m_plaintext, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_assess_format_penalty, TRUE) ; 

		BOOST_CHECK_EQUAL (props.m_data.m_place_numbers, FALSE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_place_gloss, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_place_rules, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_one_trans_per_source, TRUE) ; 
	}

	BOOST_AUTO_TEST_CASE(build_xml_doc)
	{
		app_props::properties_memory::props_data *data = &(app_props::get_props()->m_mem_props.m_data) ;

		data->m_min_score = 10 ;
		data->m_ignore_case = FALSE ;
		data->m_ignore_width = TRUE ;
		data->m_ignore_hir_kat = TRUE ;
		data->m_plaintext = TRUE ;
		data->m_assess_format_penalty = TRUE ;
		data->m_place_numbers = FALSE ;
		data->m_place_gloss = TRUE ;
		data->m_place_rules = TRUE ;


		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;

		app_props::get_props()->m_mem_props.build_xml_doc(preferences);
		xml_string_writer_test writer ;
		doc.save(writer) ;
		string actual = writer.result ;

		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<properties_memory>\n"
			"		<min_score>10</min_score>\n"
			"		<ignore_case>false</ignore_case>\n"
			"		<ignore_width>true</ignore_width>\n"
			"		<ignore_hir_kat>true</ignore_hir_kat>\n"
			"		<plaintext>true</plaintext>\n"
			"		<assess_format_penalty>true</assess_format_penalty>\n"
			"		<place_numbers>false</place_numbers>\n"
			"		<place_gloss>true</place_gloss>\n"
			"		<place_rules>true</place_rules>\n"
			"		<one_trans_per_source>false</one_trans_per_source>\n"
			"	</properties_memory>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( properties_glossary_xml_tests )

	// properties_glossary
	BOOST_AUTO_TEST_CASE( load_xml )
	{
		string text = "<properties>\n"
			"<properties_glossary>\n"

			"<min_score>24</min_score>\n"
			"<max_add>31</max_add>\n"

			"<numbering>0</numbering>\n"

			"<ignore_case>false</ignore_case>\n"
			"<plaintext>true</plaintext>\n"
			"<to_lower>true</to_lower>\n"

			"<ignore_width>true</ignore_width>\n"
			"<ignore_hir_kat>true</ignore_hir_kat>\n"
			"<simple_view>true</simple_view>\n"

			"<back_color>66</back_color>\n"

			"</properties_glossary>\n" 
			"</properties>";

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_glossary props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL (props.m_data.m_min_score, 24u) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_max_add, 31u) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_numbering, 0u) ; 

		BOOST_CHECK_EQUAL (props.m_data.m_ignore_case, FALSE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_plaintext, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_to_lower, TRUE) ; 

		BOOST_CHECK_EQUAL (props.m_data.m_ignore_width, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_ignore_hir_kat, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_simple_view, TRUE) ; 

		BOOST_CHECK_EQUAL (props.m_data.m_back_color, 66) ; 
	}

	BOOST_AUTO_TEST_CASE(build_xml_doc)
	{
		app_props::properties_glossary props ;
		app_props::properties_glossary::props_data *data = &props.m_data ;

		data->m_min_score = 44u ;
		data->m_max_add = 62u ;
		data->m_numbering = 0u ;
		data->m_back_color = 55 ;

		data->m_ignore_case = FALSE ;
		data->m_plaintext = TRUE ;
		data->m_to_lower = TRUE ;

		data->m_ignore_width = TRUE ;
		data->m_ignore_hir_kat = TRUE ;
		data->m_simple_view = TRUE ;

		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;
		props.build_xml_doc(preferences);
		xml_string_writer_test writer ;
		doc.save(writer) ;
		string actual = writer.result ;

		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<properties_glossary>\n"
			"		<min_score>44</min_score>\n"
			"		<max_add>62</max_add>\n"
			"		<numbering>0</numbering>\n"
			"		<back_color>55</back_color>\n"
			// case/formatting
			"		<ignore_case>false</ignore_case>\n"
			"		<plaintext>true</plaintext>\n"
			"		<to_lower>true</to_lower>\n"
			// normalization
			"		<ignore_width>true</ignore_width>\n"
			"		<ignore_hir_kat>true</ignore_hir_kat>\n"
			"		<simple_view>true</simple_view>\n"

			"	</properties_glossary>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// properties_algorithm
BOOST_AUTO_TEST_SUITE( properties_algorithm_xml_tests )

	BOOST_AUTO_TEST_CASE( load_xml )
	{
		string text = "<properties>\n"
			"<properties_algorithm>\n"
			"<match_algo>262</match_algo>\n" // IDC_ALGO_WORD
			"</properties_algorithm>\n" 
			"</properties>";

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_algorithm props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL (props.m_data.m_match_algo, 262) ; 

	}
	BOOST_AUTO_TEST_CASE(build_xml_doc)
	{
		app_props::properties_algorithm props ;
		app_props::properties_algorithm::props_data *data = &props.m_data ;

		data->m_match_algo = 262 ;

		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;
		props.build_xml_doc(preferences);
		xml_string_writer_test writer ;
		doc.save(writer) ;
		string actual = writer.result ;

		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<properties_algorithm>\n"
			"		<match_algo>262</match_algo>\n"
			"	</properties_algorithm>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// properties_view
BOOST_AUTO_TEST_SUITE( properties_view_xml_tests )

	BOOST_AUTO_TEST_CASE( load_xml )
	{
		string text = "<properties>\n"
			"<properties_view>\n"
			"<single_screen_matches>true</single_screen_matches>\n" 

			"<back_color>11</back_color>\n" 
			"<query_color>12</query_color>\n" 
			"<source_color>13</source_color>\n" 
			"<trans_color>14</trans_color>\n" 

			"</properties_view>\n" 
			"</properties>";

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_view props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL (props.m_data.m_back_color, 11) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_query_color, 12) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_source_color, 13) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_trans_color, 14) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_single_screen_matches, TRUE) ; 

	}

	BOOST_AUTO_TEST_CASE(build_xml_doc)
	{
		app_props::properties_view props ;
		app_props::properties_view::props_data *data = &props.m_data ;

		data->m_back_color = 44 ;
		data->m_query_color = 62 ;
		data->m_source_color = 0 ;
		data->m_trans_color = 55 ;

		data->m_single_screen_matches = FALSE ;

		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;
		props.build_xml_doc(preferences);
		xml_string_writer_test writer ;
		doc.save(writer) ;
		string actual = writer.result ;

		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<properties_view>\n"
			"		<back_color>44</back_color>\n"
			"		<query_color>62</query_color>\n"
			"		<source_color>0</source_color>\n"
			"		<trans_color>55</trans_color>\n"
			"		<single_screen_matches>false</single_screen_matches>\n"
			"	</properties_view>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// properties_qc
BOOST_AUTO_TEST_SUITE( properties_qc_xml_tests )

	BOOST_AUTO_TEST_CASE( load_xml )
	{
		string text = "<properties>\n"
			"<properties_qc>\n"
			"<check_numbers>true</check_numbers>\n" 
			"<check_all_caps>true</check_all_caps>\n" 
			"<check_gloss>true</check_gloss>\n" 
			"<live_checking>true</live_checking>\n" 
			"</properties_qc>\n" 
			"</properties>";

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_qc props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL (props.m_data.m_check_numbers, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_check_all_caps, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_check_gloss, TRUE) ; 
		BOOST_CHECK_EQUAL (props.m_data.m_live_checking, TRUE) ; 

	}

	BOOST_AUTO_TEST_CASE(build_xml_doc)
	{
		app_props::properties_qc props ;
		app_props::properties_qc::props_data *data = &props.m_data ;

		data->m_check_numbers = TRUE ;
		data->m_check_all_caps = TRUE ;
		data->m_check_gloss = TRUE ;
		data->m_live_checking = TRUE ;

		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;
		props.build_xml_doc(preferences);
		xml_string_writer_test writer ;
		doc.save(writer) ;
		string actual = writer.result ;

		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<properties_qc>\n"
			"		<check_numbers>true</check_numbers>\n"
			"		<check_all_caps>true</check_all_caps>\n"
			"		<check_gloss>true</check_gloss>\n"
			"		<live_checking>true</live_checking>\n"
			"		<qc_glosses />\n"
			"	</properties_qc>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}

	BOOST_AUTO_TEST_CASE(build_xml_doc_glosses)
	{
		app_props::properties_qc props ;
		app_props::properties_qc::props_data *data = &props.m_data ;

		data->m_check_numbers = TRUE ;
		data->m_check_all_caps = TRUE ;
		data->m_check_gloss = TRUE ;
		data->m_live_checking = TRUE ;
		props.m_qc_glosses.push_back(L"first.fgloss") ;
		props.m_qc_glosses.push_back(L"http://ms.felix-cat.com/mems/3") ;

		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;
		props.build_xml_doc(preferences);
		xml_string_writer_test writer ;
		doc.save(writer) ;
		string actual = writer.result ;

		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<properties_qc>\n"
			"		<check_numbers>true</check_numbers>\n"
			"		<check_all_caps>true</check_all_caps>\n"
			"		<check_gloss>true</check_gloss>\n"
			"		<live_checking>true</live_checking>\n"
			"		<qc_glosses>\n"
			"			<filename>first.fgloss</filename>\n"
			"			<filename>http://ms.felix-cat.com/mems/3</filename>\n"
			"		</qc_glosses>\n"
			"	</properties_qc>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// properties_general
BOOST_AUTO_TEST_SUITE( properties_general_xml_tests )

	BOOST_AUTO_TEST_CASE( load_xml )
	{
		string text = "<properties>\n"
			"<properties_general>\n"
			"<window_size>200</window_size>\n" 
			"<preferred_gui_lang>444</preferred_gui_lang>\n" 

			"<load_prev_mem_on_startup>true</load_prev_mem_on_startup>\n" 
			"<load_prev_gloss_on_startup>true</load_prev_gloss_on_startup>\n" 
			"<show_markup>false</show_markup>\n" 
			"<first_launch>false</first_launch>\n" 

			"<merge_choice>475</merge_choice>\n" 

			"<query_merge>false</query_merge>\n" 
			"<old_mem_mgr>true</old_mem_mgr>\n" 
			"<must_login>true</must_login>\n"
			"<user_name>jerry</user_name>\n" 
			"</properties_general>\n" 
			"</properties>";

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(text.c_str());
		BOOST_CHECK_EQUAL ( result.status, pugi::status_ok ) ; 

		app_props::properties_general props ;
		props.parse_xml_doc(doc) ;

		BOOST_CHECK_EQUAL (props.m_data.m_window_size, 200) ;
		BOOST_CHECK_EQUAL (props.m_data.m_preferred_gui_lang, 444) ;

		BOOST_CHECK_EQUAL (props.m_data.m_load_prev_mem_on_startup, TRUE) ;
		BOOST_CHECK_EQUAL (props.m_data.m_load_prev_gloss_on_startup, TRUE) ;
		BOOST_CHECK_EQUAL (props.m_data.m_show_markup, FALSE) ;
		BOOST_CHECK_EQUAL (props.m_data.m_first_launch, FALSE) ;

		BOOST_CHECK_EQUAL (props.m_data.m_merge_choice, 475) ;

		BOOST_CHECK_EQUAL (props.m_data.m_query_merge, FALSE) ;
		BOOST_CHECK_EQUAL (props.m_data.m_old_mem_mgr, TRUE) ;
		BOOST_CHECK_EQUAL (props.m_data.m_must_login, TRUE) ;


		BOOST_CHECK_EQUAL (wstring(props.m_data.m_user_name), L"jerry") ;

	}


	BOOST_AUTO_TEST_CASE(build_xml_doc)
	{
		app_props::properties_general props ;
		app_props::properties_general::props_data *data = &props.m_data ;

		data->m_window_size = 1 ;
		data->m_preferred_gui_lang = 2 ;
		data->m_merge_choice = 4 ;

		data->m_load_prev_mem_on_startup = TRUE ;
		data->m_load_prev_gloss_on_startup =TRUE ;
		data->m_show_markup = TRUE ;
		data->m_first_launch = TRUE ;

		data->m_query_merge = TRUE ;
		data->m_old_mem_mgr = TRUE ;

		_tcscpy_s( data->m_user_name, MAX_PATH, L"Bozo The Clown") ;

		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;
		props.build_xml_doc(preferences);
		xml_string_writer_test writer ;
		doc.save(writer) ;
		string actual = writer.result ;

		string expected = "<?xml version=\"1.0\"?>\n"
			"<properties>\n"
			"	<properties_general>\n"
			"		<window_size>1</window_size>\n"
			"		<preferred_gui_lang>2</preferred_gui_lang>\n"
			"		<merge_choice>4</merge_choice>\n"

			"		<load_prev_mem_on_startup>true</load_prev_mem_on_startup>\n"
			"		<load_prev_gloss_on_startup>true</load_prev_gloss_on_startup>\n"
			"		<show_markup>true</show_markup>\n"
			"		<first_launch>true</first_launch>\n"

			"		<query_merge>true</query_merge>\n"
			"		<old_mem_mgr>true</old_mem_mgr>\n"
			"		<must_login>false</must_login>\n"

			"		<user_name>Bozo The Clown</user_name>\n"

			"	</properties_general>\n"
			"</properties>\n" ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}


BOOST_AUTO_TEST_SUITE_END()
#endif
