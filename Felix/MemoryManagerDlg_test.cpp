#include "StdAfx.h"
#include "MemoryManagerDlg.h"
#include "record_local.h"
#include "memory_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_engine ;

	void add_mems(CMemoryManagerDlg &dialog, CString name1, CString name2)
	{
		boost::shared_ptr<memory_model> model(new memory_model_mem) ;
		memory_pointer mem1(new memory_local) ;
		mem1->set_location(name1) ;
		memory_pointer mem2(new memory_local) ;
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
		boost::shared_ptr<memory_model> model(new memory_model_mem) ;
		memory_pointer mem1(new memory_local) ;
		mem1->set_location(name1) ;
		memory_pointer mem2(new memory_local) ;
		mem2->set_location(name2) ;
		memory_pointer mem3(new memory_local) ;
		mem3->set_location(name3) ;

		model->insert_memory(mem1) ;
		model->insert_memory(mem2) ;
		model->insert_memory(mem3) ;

		dialog.set_memories(model) ;
	}

	TEST( TestCMemoryManagerDlg, Create )
	{
		try
		{
			CMemoryManagerDlg dialog ;
			ASSERT_EQUALS_V(IDD_MEM_MGR_DLG, CMemoryManagerDlg::IDD) ;
		}
		catch (...)
		{
			FAIL_M( "Instantiating CMemoryManagerDlg blew up" ) ;
		}
	}
	TEST( TestCMemoryManagerDlg, get_memories)
	{
		boost::shared_ptr<memory_engine::memory_model> model(new memory_engine::memory_model_mem); 
		CMemoryManagerDlg dialog ;
		dialog.get_memories(model) ;
		ASSERT_EQUALS_V(0, (int)model->get_memories().size()) ;
	}
	TEST( TestCUserName, TestOperator )
	{
		ASSERT_EQUALS( L"Ryan", CUserName().as_wstring() ) ;

		CUserName user_name ;
		ASSERT_EQUALS( L"Ryan", user_name.as_wstring() ) ;

		wstring wstr_un = user_name ;
		ASSERT_EQUALS( L"Ryan", wstr_un ) ;
	}
	// message processing
	TEST( TestCMemoryManagerDlg, test_message_WM_INITDIALOG)
	{
		CMemoryManagerDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCMemoryManagerDlg, test_message_IDOK)
	{
		CMemoryManagerDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCMemoryManagerDlg, test_message_IDCANCEL)
	{
		CMemoryManagerDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCMemoryManagerDlg, test_message_ZERO)
	{
		CMemoryManagerDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}

	// post-refactoring tests
	TEST(TestCMemoryManagerDlg, get_memory_at_0)
	{
		CMemoryManagerDlg dialog ;
		add_mems(dialog, "c:\\location_0.xml", "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem_at_0 = dialog.get_memory_at(0) ;
		CStringA actual = mem_at_0->get_location() ;
		SimpleString expected = "c:\\location_2.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_memory_at_1)
	{
		CMemoryManagerDlg dialog ;
		add_mems(dialog, "c:\\location_0.xml", "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem_at_1 = dialog.get_memory_at(1) ;
		CStringA actual = mem_at_1->get_location() ;
		SimpleString expected = "c:\\location_1.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_memory_at_2)
	{
		CMemoryManagerDlg dialog ;
		add_mems(dialog, "c:\\location_0.xml", "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem_at_1 = dialog.get_memory_at(2) ;
		CStringA actual = mem_at_1->get_location() ;
		SimpleString expected = "c:\\location_0.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_memory_oob_throws)
	{
		CMemoryManagerDlg dialog ;
		try
		{
			dialog.get_memory_at(1) ;
			FAIL_M("Failed to throw on out of bounds memory") ;
		}
		catch (std::exception&)
		{
			ASSERT_TRUE(true) ;
		}
	}
	TEST(TestCMemoryManagerDlg, swap_memories_0)
	{
		CMemoryManagerDlg dialog ;
		add_mems(dialog, "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem = dialog.get_memory_at(0) ;
		CStringA actual = mem->get_location() ;
		SimpleString expected = "c:\\location_2.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;

		dialog.swap_memories(0) ;

		mem = dialog.get_memory_at(0) ;
		actual = mem->get_location() ;
		expected = "c:\\location_1.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, swap_memories_1)
	{
		CMemoryManagerDlg dialog ;
		add_mems(dialog, "c:\\location_1.xml", "c:\\location_2.xml") ;

		memory_pointer mem = dialog.get_memory_at(1) ;
		CStringA actual = mem->get_location() ;
		SimpleString expected = "c:\\location_1.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;

		dialog.swap_memories(0) ;

		mem = dialog.get_memory_at(1) ;
		actual = mem->get_location() ;
		expected = "c:\\location_2.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	// feedback strings
	
	TEST(TestCMemoryManagerDlg, get_info_for_item)
	{
		app_props::properties_general props; 
		props.read_from_registry() ;

		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
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
		ASSERT_EQUALS_V(SimpleString(expected.c_str()), SimpleString(actual)) ;
	}

	TEST(TestCMemoryManagerDlg, get_saving_feedback)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->set_location("spam") ;
		wstring text = dialog.get_saving_feedback(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "<b>Saving Memory to file [spam]...</b>" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_save_prompt)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->set_location("spam.xml") ;
		wstring text = dialog.get_save_prompt(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "The Memory file [spam.xml] has been modified since your last save.\r\n\r\nDo you wish to save it now?" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}


	TEST(TestCMemoryManagerDlg, get_loading_message)
	{
		CMemoryManagerDlg dialog ;
		wstring text = dialog.get_loading_message("c:\\foo\\bill.xml") ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "<b>Loading file [c:\\foo\\bill.xml]...</b>" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}


	TEST(TestCMemoryManagerDlg, get_memory_name_new)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_memory_name(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "New" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_memory_name_bill)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->set_location("c:\\foo\\bill.xml") ;
		wstring text = dialog.get_memory_name(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "bill.xml" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_creator_name_new)
	{
		app_props::properties_general props; 
		props.read_from_registry() ;

		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_creator_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = (LPCSTR)(CT2A(props.m_data.m_user_name)) ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_creator_name_empty)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->get_memory_info()->set_creator(L"") ;
		wstring text = dialog.get_creator_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "Default" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_creator_name_bill)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->get_memory_info()->set_creator(L"Bill") ;
		wstring text = dialog.get_creator_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "Bill" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	// get_field_name
	TEST(TestCMemoryManagerDlg, get_field_name_empty)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_field_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "Default" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_field_name_bill)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->get_memory_info()->set_field(L"Bill") ;
		wstring text = dialog.get_field_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "Bill" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

	// get_created_on
	TEST(TestCMemoryManagerDlg, get_created_on_empty)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring actual = dialog.get_created_on(mem->get_memory_info()) ;
		misc_wrappers::date nowdate ;
		nowdate.set_to_local_time() ;
		wstring expected = string2wstring(nowdate.get_date_string()) ;
		ASSERT_TRUE(actual.find(expected) != wstring::npos) ;
	}
	TEST(TestCMemoryManagerDlg, get_created_on_date)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->get_memory_info()->set_created_on(L"2008/11/04 14:49:56") ;
		wstring text = dialog.get_created_on(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "2008/11/04 14:49:56" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

	// get_source_lang
	TEST(TestCMemoryManagerDlg, get_source_lang_empty)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_source_lang(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "Default" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_source_lang_English)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->get_memory_info()->set_source_language(L"English") ;
		wstring text = dialog.get_source_lang(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "English" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

	// get_target_lang
	TEST(TestCMemoryManagerDlg, get_target_lang_empty)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_target_lang(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "Default" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_target_lang_nihongo)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->get_memory_info()->set_target_language(L"日本語") ;
		wstring actual = dialog.get_target_lang(mem->get_memory_info()) ;
		wstring expected = L"日本語" ;
		ASSERT_EQUALS(expected, actual) ;
	}

	// get_client_name
	TEST(TestCMemoryManagerDlg, get_client_name_empty)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_client_name(mem->get_memory_info()) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "Default" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_client_name_nihongo)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->get_memory_info()->set_client(L"日本語") ;
		wstring actual = dialog.get_client_name(mem->get_memory_info()) ;
		wstring expected = L"日本語" ;
		ASSERT_EQUALS(expected, actual) ;
	}

	// get_mem_size
	TEST(TestCMemoryManagerDlg, get_mem_size_0)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_mem_size(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "0" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_mem_size_1)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		wstring text = dialog.get_mem_size(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "1" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	// get_file_size
	TEST(TestCMemoryManagerDlg, get_file_size_0)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_file_size(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "0" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_file_size_1)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		mem->set_location("c:\\test\\bigmem.xml") ;
		wstring text = dialog.get_file_size(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "12.22 MBytes" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

	// get_reliability_range
	TEST(TestCMemoryManagerDlg, get_reliability_range_0)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;
		wstring text = dialog.get_reliability_range(mem) ;
		CStringA actual = text.c_str() ;
		SimpleString expected = "0 &ndash; 0 (Ave: 0.0)" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMemoryManagerDlg, get_reliability_range_2_9)
	{
		CMemoryManagerDlg dialog ;
		memory_pointer mem(new memory_local) ;

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
		SimpleString expected = "2 &ndash; 9 (Ave: 5.5)" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
}

#endif