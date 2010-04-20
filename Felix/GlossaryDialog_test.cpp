#include "StdAfx.h"
#include "GlossaryDialog.h"
#include "record_local.h"
#include "memory_local.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestGlossaryWindow )

	using namespace mem_engine;
	BOOST_AUTO_TEST_CASE( get_record_translation_standard)
	{
		CGlossaryWindow gloss ;
		gloss.m_properties_gloss.m_data.m_to_lower = FALSE ;
		gloss.m_properties_gloss.m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"spam") ;

		wstring normalized = gloss.get_record_translation(record) ;

		string expected = "spam" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(1, (int)record->get_refcount()) ;
	}
	BOOST_AUTO_TEST_CASE( get_record_translation_lower)
	{
		CGlossaryWindow gloss ;
		gloss.m_properties_gloss.m_data.m_to_lower = TRUE ;
		gloss.m_properties_gloss.m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"SpaM") ;

		wstring normalized = gloss.get_record_translation(record) ;

		string expected = "spam" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_record_translation_plain)
	{
		CGlossaryWindow gloss ;
		gloss.m_properties_gloss.m_data.m_to_lower = FALSE ;
		gloss.m_properties_gloss.m_data.m_plaintext = TRUE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<bold>spam &lt; &amp; &gt; eggs</bold>") ;

		BOOST_CHECK_EQUAL(record->get_trans_plain(), L"spam < & > eggs") ;

		wstring normalized = gloss.get_record_translation(record) ;

		string expected = "spam &lt; &amp; &gt; eggs" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	/*********************************************************************/
	/* multiterm stuff                                                   */
	/*********************************************************************/
	BOOST_AUTO_TEST_CASE( load_multiterm6)
	{
		CGlossaryWindow gloss ;
		CString filename = _T("c:\\test\\Multiterm.6.0.sample.txt") ;
		gloss.import_multiterm(filename) ;
		BOOST_CHECK_EQUAL(1, (int)gloss.m_memories->size()) ;
		mem_engine::memory_pointer mem = gloss.m_memories->get_first_memory() ;
		BOOST_CHECK_EQUAL(50, (int)mem->size()) ;
	}
	BOOST_AUTO_TEST_CASE( export_gloss_mt55)
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
		string expected = (format(fmt.c_str())
			% "Japanese" % "English"
			% string2string(rec1->get_source_plain())
			% string2string(rec1->get_trans_plain())
			% string2string(rec2->get_source_plain())
			% string2string(rec2->get_trans_plain())
			).str().c_str() ;

		file::view fview ;
		string actual = (LPSTR)fview.create_view(filename) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( export_gloss_mt6)
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
		string expected = string2string(fmt, CP_UTF8).c_str() ;

		file::view fview ;
		LPWSTR raw_text = (LPWSTR)fview.create_view(filename) ;
		string actual = string2string(wstring(raw_text+1), CP_UTF8).c_str() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	// CGlossaryWindow message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCLOSE)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CGlossaryWindow dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif 
