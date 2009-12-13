#include "StdAfx.h"
#include "GlossaryDialog.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{

	TEST( TestGlossaryWindow, get_record_translation_standard)
	{
		CGlossaryWindow gloss ;
		gloss.m_properties_gloss.m_data.m_to_lower = FALSE ;
		gloss.m_properties_gloss.m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"spam") ;

		wstring normalized = gloss.get_record_translation(record) ;

		SimpleString expected = "spam" ;
		SimpleString actual = (LPCSTR)CStringA(normalized.c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_V(1, (int)record->get_refcount()) ;
	}
	TEST( TestGlossaryWindow, get_record_translation_lower)
	{
		CGlossaryWindow gloss ;
		gloss.m_properties_gloss.m_data.m_to_lower = TRUE ;
		gloss.m_properties_gloss.m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"SpaM") ;

		wstring normalized = gloss.get_record_translation(record) ;

		SimpleString expected = "spam" ;
		SimpleString actual = (LPCSTR)CStringA(normalized.c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestGlossaryWindow, get_record_translation_plain)
	{
		CGlossaryWindow gloss ;
		gloss.m_properties_gloss.m_data.m_to_lower = FALSE ;
		gloss.m_properties_gloss.m_data.m_plaintext = TRUE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<bold>spam &lt; &amp; &gt; eggs</bold>") ;

		ASSERT_EQUALS(record->get_trans_plain(), L"spam < & > eggs") ;

		wstring normalized = gloss.get_record_translation(record) ;

		SimpleString expected = "spam &lt; &amp; &gt; eggs" ;
		SimpleString actual = (LPCSTR)CStringA(normalized.c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

	/*********************************************************************/
	/* multiterm stuff                                                   */
	/*********************************************************************/
	TEST( TestGlossaryWindow, load_multiterm6)
	{
		CGlossaryWindow gloss ;
		CString filename = _T("c:\\test\\Multiterm.6.0.sample.txt") ;
		gloss.import_multiterm(filename) ;
		ASSERT_EQUALS_V(1, (int)gloss.m_memories->size()) ;
		memory_engine::memory_pointer mem = gloss.m_memories->get_first_memory() ;
		ASSERT_EQUALS_V(50, (int)mem->size()) ;
	}
	TEST(TestGlossaryWindow, export_gloss_mt55)
	{
		CGlossaryWindow gloss ;
		CString filename = _T("c:\\test\\mt.55.output.txt") ;
		::DeleteFile(filename) ;
		string source("Japanese") ;
		string target("English") ;

		memory_pointer mem(new memory_local) ;
		record_pointer rec1(new record_local) ;
		record_pointer rec2(new record_local) ;
		rec1->set_source(L"‚è‚ñ‚²") ;
		rec1->set_trans(L"apple") ;

		rec2->set_source(L"“ú–{Œê") ;
		rec2->set_trans(L"Japanese") ;

		mem->add_record(rec1) ;
		mem->add_record(rec2) ;

		gloss.export_multiterm_55_sub(source, target, filename, mem) ;

		string fmt = "<%1%>%3%\r\n" ;
		fmt += "<%2%>%4%\r\n" ;
		fmt += "<Notes>-\r\n" ;
		fmt += "***\r\n" ;
		fmt += "<%1%>%5%\r\n" ;
		fmt += "<%2%>%6%\r\n" ;
		fmt += "<Notes>-\r\n" ;
		fmt += "***\r\n" ;
		SimpleString expected = (format(fmt.c_str())
			% "Japanese" % "English"
			% string2string(rec1->get_source_plain())
			% string2string(rec1->get_trans_plain())
			% string2string(rec2->get_source_plain())
			% string2string(rec2->get_trans_plain())
			).str().c_str() ;

		file::view fview ;
		SimpleString actual = (LPSTR)fview.create_view(filename) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestGlossaryWindow, export_gloss_mt6)
	{
		CGlossaryWindow gloss ;
		CString filename = _T("c:\\test\\mt.6.output.txt") ;
		::DeleteFile(filename) ;

		wstring source(L"Japanese") ;
		wstring target(L"English") ;

		memory_pointer mem(new memory_local) ;
		record_pointer rec1(new record_local) ;
		record_pointer rec2(new record_local) ;
		rec1->set_source(L"‚è‚ñ‚²") ;
		rec1->set_trans(L"apple") ;

		rec2->set_source(L"“ú–{Œê") ;
		rec2->set_trans(L"Japanese") ;

		mem->add_record(rec1) ;
		mem->add_record(rec2) ;

		gloss.export_multiterm_6_sub(source, target, filename, mem) ;

		wstring fmt = L"Japanese\tEnglish\tNotes\n" ;
		fmt += L"‚è‚ñ‚²\tapple\t-\n" ;
		fmt += L"“ú–{Œê\tJapanese\t-\n" ;
		SimpleString expected = string2string(fmt, CP_UTF8).c_str() ;

		file::view fview ;
		LPWSTR raw_text = (LPWSTR)fview.create_view(filename) ;
		SimpleString actual = string2string(wstring(raw_text+1), CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

	// CGlossaryWindow message processing
	TEST( TestGlossaryWindow, test_message_WM_INITDIALOG)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestGlossaryWindow, test_message_IDOK)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestGlossaryWindow, test_message_IDCANCEL)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestGlossaryWindow, test_message_IDCLOSE)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestGlossaryWindow, test_message_ZERO)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
}


#endif // #ifdef _DEBUG