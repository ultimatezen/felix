#include "StdAfx.h"
#include "MemoryManagerDlg.h"
#include "record_local.h"
#include "memory_local.h"
#include "FelixModelInterface.h"
#include <boost/test/unit_test.hpp>
#include "felix_factory.h"

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCMemoryManagerDlg )


	using namespace mem_engine ;
	using namespace app_props ;

	typedef boost::shared_ptr<memory_model> model_ptr ;

	model_ptr get_model()
	{
		app_props::props_ptr props = FelixFactory().make_props() ;

		return model_ptr(new memory_model_mem(props)) ;

	}

	void add_mems(CMemoryManagerDlg &dialog, CString name1, CString name2)
	{
		model_ptr model = get_model() ;
		memory_pointer mem1 = model->create_memory() ;
		mem1->set_location(name1) ;
		memory_pointer mem2 = model->create_memory() ;
		mem2->set_location(name2) ;

		model->insert_memory(mem1) ;
		model->insert_memory(mem2) ;

		dialog.set_memories(model) ;
	}
	void add_mems(CMemoryManagerDlg &dialog, 
		CString name1, 
		CString name2,
		CString name3)
	{
		model_ptr model = get_model() ;
		memory_pointer mem1 = model->create_memory() ;
		mem1->set_location(name1) ;
		memory_pointer mem2 = model->create_memory() ;
		mem2->set_location(name2) ;
		memory_pointer mem3 = model->create_memory() ;
		mem3->set_location(name3) ;

		model->insert_memory(mem1) ;
		model->insert_memory(mem2) ;
		model->insert_memory(mem3) ;

		dialog.set_memories(model) ;
	}

	BOOST_AUTO_TEST_CASE( Create )
	{
		try
		{
			CMemoryManagerDlg dialog (get_props()) ;
			BOOST_CHECK_EQUAL(IDD_MEM_MGR_DLG, CMemoryManagerDlg::IDD) ;
		}
		catch (...)
		{
			BOOST_FAIL( "Instantiating CMemoryManagerDlg blew up" ) ;
		}
	}
	BOOST_AUTO_TEST_CASE( get_memories)
	{
		model_ptr model = get_model() ;
		CMemoryManagerDlg dialog (get_props()) ;
		dialog.get_memories(model) ;
		BOOST_CHECK_EQUAL(0u, model->get_memories().size()) ;
	}
	BOOST_AUTO_TEST_CASE( TestOperator )
	{
		BOOST_CHECK( L"Ryan" == CUserName().as_wstring() ) ;

		CUserName user_name ;
		BOOST_CHECK( L"Ryan" == user_name.as_wstring() ) ;

		wstring wstr_un = user_name ;
		BOOST_CHECK( L"Ryan" == wstr_un ) ;
	}
	// message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog"); 
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnOK"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(FALSE, result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}

	// post-refactoring tests
	BOOST_AUTO_TEST_CASE( get_memory_at_0)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		add_mems(dialog, "c:\\location_0.xml", "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem_at_0 = dialog.get_memory_at(0) ;
		CStringA actual = mem_at_0->get_location() ;
		string expected = "c:\\location_2.xml" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_memory_at_1)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		add_mems(dialog, "c:\\location_0.xml", "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem_at_1 = dialog.get_memory_at(1) ;
		CStringA actual = mem_at_1->get_location() ;
		string expected = "c:\\location_1.xml" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_memory_at_2)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		add_mems(dialog, "c:\\location_0.xml", "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem_at_1 = dialog.get_memory_at(2) ;
		CStringA actual = mem_at_1->get_location() ;
		string expected = "c:\\location_0.xml" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_memory_oob_throws)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		try
		{
			dialog.get_memory_at(1) ;
			BOOST_FAIL("Failed to throw on out of bounds memory") ;
		}
		catch (std::exception&)
		{
			BOOST_CHECK(true) ;
		}
	}
	BOOST_AUTO_TEST_CASE( swap_memories_0)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		add_mems(dialog, "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem = dialog.get_memory_at(0) ;
		CStringA actual = mem->get_location() ;
		string expected = "c:\\location_2.xml" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;

		dialog.swap_memories(0) ;

		mem = dialog.get_memory_at(0) ;
		actual = mem->get_location() ;
		expected = "c:\\location_1.xml" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( swap_memories_1)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		add_mems(dialog, "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem = dialog.get_memory_at(1) ;
		CStringA actual = mem->get_location() ;
		string expected = "c:\\location_1.xml" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;

		dialog.swap_memories(0) ;

		mem = dialog.get_memory_at(1) ;
		actual = mem->get_location() ;
		expected = "c:\\location_2.xml" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	// feedback strings
	
	BOOST_AUTO_TEST_CASE( get_info_for_item)
	{
		app_props::properties_general props; 
		props.read_from_registry() ;

		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->set_location("spam") ;
		mem->get_memory_info()->set_created_on(L"2008/11/04 14:49:56") ;
		wstring text = dialog.get_info_for_item(mem) ;
		CStringA actual = text.c_str() ;
		string expected = string("<table width=\"100%\" class=\"info\">\r\n")
			+ "<tr><td bgcolor=\"#F2F2F2\"><b>File Name:</b></td><td id=\"file_name\" bgcolor=\"#F2F2F2\">spam</td></tr>\r\n"
			+ "<tr><td><b>Creator:</b></td><td id=\"creator\">"
			+ (LPCSTR)CT2A(props.m_data.m_user_name)
			+ "</td></tr>\r\n"
			+ "<tr><td bgcolor=\"#F0F0F0\"><b>Field:</b></td><td id=\"field\" bgcolor=\"#F0F0F0\"><b>Default</b></td></tr>\r\n"
			+ "<tr><td><b>Created:</b></td><td id=\"created_on\">2008/11/04 14:49:56</td></tr>\r\n"
			+ "<tr><td bgcolor=\"#F0F0F0\"><b>Source Language:</b></td><td id=\"source_language\" bgcolor=\"#F0F0F0\"><b>Default</b></td></tr>\r\n"
			+ "<tr><td><b>Target Language:</b></td><td id=\"target_language\"><b>Default</b></td></tr>\r\n"
			+ "<tr><td bgcolor=\"#F0F0F0\"><b>Client:</b></td><td id=\"client\" bgcolor=\"#F0F0F0\"><b>Default</b></td></tr>\r\n"
			+ "<tr><td><b>Num. Entries:</b></td><td id=\"mem_size\">0</td></tr>\r\n"
			+ "<tr><td bgcolor=\"#F0F0F0\"><b>File Size:</b></td><td id=\"file_size\" bgcolor=\"#F0F0F0\">0</td></tr>\r\n"
			+ "<tr><td><b>Reliability:</b></td><td id=\"reliability\">0 &ndash; 0 (Ave: 0.0)</td></tr>\r\n"
			+ "<tr><td bgcolor=\"#F0F0F0\"><b>Validated %</b></td><td id=\"validated\" bgcolor=\"#F0F0F0\">0%</td></tr>\r\n"
			+ "<tr><td><b>Locked:</b></td><td id=\"locked\">false</td></tr>\r\n"
			+ "</table><p align=right><b><a href=\"100:100\">Advanced...</a></b></p>" ;
		BOOST_CHECK_EQUAL(string(expected.c_str()), string(actual)) ;
	}

	BOOST_AUTO_TEST_CASE( get_saving_feedback)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->set_location("spam") ;
		wstring text = dialog.get_saving_feedback(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "<b>Saving Memory to file [spam]...</b>" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_save_prompt)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->set_location("spam.xml") ;
		wstring text = dialog.get_save_prompt(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "The Memory file [spam.xml] has been modified since your last save.\r\n\r\nDo you wish to save it now?" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}


	BOOST_AUTO_TEST_CASE( get_loading_message)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		wstring text = dialog.get_loading_message("c:\\foo\\bill.xml") ;
		CStringA actual = text.c_str() ;
		string expected = "<b>Loading file [c:\\foo\\bill.xml]...</b>" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}


	BOOST_AUTO_TEST_CASE( get_memory_name_new)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_memory_name(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "New" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_memory_name_bill)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->set_location("c:\\foo\\bill.xml") ;
		wstring text = dialog.get_memory_name(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "bill" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_creator_name_new)
	{
		CMemoryManagerDlg dialog (get_props()) ;

		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_creator_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Ryan" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_creator_name_empty)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->get_memory_info()->set_creator(L"") ;
		wstring text = dialog.get_creator_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Default" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_creator_name_bill)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->get_memory_info()->set_creator(L"Bill") ;
		wstring text = dialog.get_creator_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Bill" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	// get_field_name
	BOOST_AUTO_TEST_CASE( get_field_name_empty)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_field_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Default" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_field_name_bill)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->get_memory_info()->set_field(L"Bill") ;
		wstring text = dialog.get_field_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Bill" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}

	// get_created_on
	BOOST_AUTO_TEST_CASE( get_created_on_empty)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring actual = dialog.get_created_on(mem->get_memory_info()) ;
		misc_wrappers::date nowdate ;
		nowdate.set_to_local_time() ;
		wstring expected = string2wstring(nowdate.get_date_string()) ;
		BOOST_CHECK(actual.find(expected) != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( get_created_on_date)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->get_memory_info()->set_created_on(L"2008/11/04 14:49:56") ;
		wstring text = dialog.get_created_on(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "2008/11/04 14:49:56" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}

	// get_source_lang
	BOOST_AUTO_TEST_CASE( get_source_lang_empty)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_source_lang(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Default" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_source_lang_English)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->get_memory_info()->set_source_language(L"English") ;
		wstring text = dialog.get_source_lang(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "English" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}

	// get_target_lang
	BOOST_AUTO_TEST_CASE( get_target_lang_empty)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_target_lang(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Default" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_target_lang_nihongo)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->get_memory_info()->set_target_language(L"日本語") ;
		wstring actual = dialog.get_target_lang(mem->get_memory_info()) ;
		wstring expected = L"日本語" ;
		BOOST_CHECK(expected == actual) ;
	}

	// get_client_name
	BOOST_AUTO_TEST_CASE( get_client_name_empty)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_client_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		string expected = "Default" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_client_name_nihongo)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->get_memory_info()->set_client(L"日本語") ;
		wstring actual = dialog.get_client_name(mem->get_memory_info()) ;
		wstring expected = L"日本語" ;
		BOOST_CHECK(expected == actual) ;
	}

	// get_mem_size
	BOOST_AUTO_TEST_CASE( get_mem_size_0)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_mem_size(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "0" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_mem_size_1)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		wstring text = dialog.get_mem_size(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "1" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	// get_file_size
	BOOST_AUTO_TEST_CASE( get_file_size_0)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_file_size(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "0" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_file_size_1)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		mem->set_location("c:\\test\\bigmem.xml") ;
		wstring text = dialog.get_file_size(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "12.22 MBytes" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}

	// get_reliability_range
	BOOST_AUTO_TEST_CASE( get_reliability_range_0)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;
		wstring text = dialog.get_reliability_range(mem) ;
		CStringA actual = text.c_str() ;
		string expected = "0 &ndash; 0 (Ave: 0.0)" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_reliability_range_2_9)
	{
		CMemoryManagerDlg dialog (get_props()) ;
		model_ptr model = get_model() ;
		memory_pointer mem = model->create_memory() ;

		record_pointer rec1(new record_local) ;
		rec1->set_reliability(2) ;
		rec1->set_source(L"foo1") ;
		rec1->set_trans(L"bar1") ;
		mem->add_record(rec1) ;

		record_pointer rec2(new record_local) ;
		rec2->set_reliability(9) ;
		rec2->set_source(L"foo2") ;
		rec2->set_trans(L"bar2") ;
		mem->add_record(rec2) ;

		wstring text = dialog.get_reliability_range(mem) ;

		CStringA actual = text.c_str() ;
		string expected = "2 &ndash; 9 (Ave: 5.5)" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
