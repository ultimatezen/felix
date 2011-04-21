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

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

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
		BOOST_CHECK_EQUAL((int)LANG_JAPANESE, props.m_data.m_preferred_gui_lang) ;
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


BOOST_AUTO_TEST_SUITE( properties_loaded_history_xml_tests )

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
		props.load_xml_props_type(doc, items, "loaded_mems") ;

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
		props.load_xml_props_type(doc, items, "loaded_mems") ;

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

		string text = props.get_xml_doc() ;
		string expected = "<?xml version=\"1.0\"?>\n"
			"<loaded_history>\n"
			"	<loaded_mems>\n"
			"		<filename>C:\\m_mems.txt</filename>\n"
			"	</loaded_mems>\n"
			"	<loaded_gloss>\n"
			"		<filename>C:\\m_glosses.txt</filename>\n"
			"	</loaded_gloss>\n"
			"	<loaded_remote_mems>\n"
			"		<filename>C:\\m_remote_mems.txt</filename>\n"
			"	</loaded_remote_mems>\n"
			"	<loaded_remote_gloss>\n"
			"		<filename>C:\\m_remote_glosses.txt</filename>\n"
			"	</loaded_remote_gloss>\n"
			"</loaded_history>\n" ;

		BOOST_CHECK_EQUAL(text, expected) ;
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
		BOOST_CHECK_EQUAL ( props.m_data.m_assess_format_penalty, FALSE ) ; 

		props.read_from_registry() ;
		app_props::properties_memory props2(props) ;

		BOOST_CHECK_EQUAL ( props.m_data.m_min_score,		props2.m_data.m_min_score ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_case,	props2.m_data.m_ignore_case ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_width, props2.m_data.m_ignore_width ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_ignore_hir_kat,			props2.m_data.m_ignore_hir_kat ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_plaintext,					props2.m_data.m_plaintext ) ; 
		BOOST_CHECK_EQUAL ( props.m_data.m_assess_format_penalty, props2.m_data.m_assess_format_penalty ) ; 
	}
BOOST_AUTO_TEST_SUITE_END()

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

BOOST_AUTO_TEST_SUITE_END()


#endif
