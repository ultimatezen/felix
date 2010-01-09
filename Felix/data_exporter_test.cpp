#include "StdAfx.h"
#include "data_exporter.h"
#include "record_local.h"
#include "memory_local.h"

#include "easyunit/testharness.h"
#include "output_device_fake.h"

#ifdef UNIT_TEST


namespace easyunit
{
	using namespace mem_engine ;

	//////////////////////////////////////////////////////////////////////////
	// TradosDataExporter
	//////////////////////////////////////////////////////////////////////////
	TEST( TestTradosDataExporter, internal_date_to_trados_date )
	{
		std::set< wstring > fonts ;
		CProgressListenerDummy listener ;
		TradosDataExporter exporter(fonts, &listener) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = device_ptr(device) ;

		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 1 ;
		thedate.wDay = 17 ;
		thedate.wHour = 12; 
		thedate.wMinute = 0 ;
		thedate.wSecond = 0 ;
		thedate.wMilliseconds = 0 ;

		SimpleString actual = exporter.internal_date_to_trados_date(thedate).c_str() ;
		SimpleString expected = "17012000, 12:00:00" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestTradosDataExporter, open_destination )
	{
		std::set< wstring > fonts ;
		CProgressListenerDummy listener ;
		TradosDataExporter exporter(fonts, &listener) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = device_ptr(device) ;

		exporter.open_destination(_T("foo.txt")) ;
		ASSERT_EQUALS_V(2, (int)device->m_calls.size()) ;
		ASSERT_EQUALS_V(SimpleString("open"), device->m_calls[0].c_str()) ;
		ASSERT_EQUALS_V(SimpleString("foo.txt"), device->m_calls[1].c_str()) ;
	}
	TEST( TestTradosDataExporter, write_preamble )
	{
		std::set< wstring > fonts ;
		CProgressListenerDummy listener ;
		TradosDataExporter exporter(fonts, &listener) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = device_ptr(device) ;

		exporter.write_preamble() ;
		ASSERT_EQUALS_V(3, (int)device->m_calls.size()) ;
		ASSERT_EQUALS_V(SimpleString("write_string"), device->m_calls[0].c_str()) ;
		ASSERT_EQUALS_V(SimpleString("write_string"), device->m_calls[1].c_str()) ;
		ASSERT_EQUALS_V(SimpleString("write_string"), device->m_calls[2].c_str()) ;
	}
	TEST( TestTradosDataExporter, create_unicode_escape )
	{
		std::set< wstring > fonts ;
		CProgressListenerDummy listener ;
		TradosDataExporter exporter(fonts, &listener) ;

		string escape = exporter.create_unicode_escape(L't', 't') ;
		SimpleString actual = escape.c_str() ;
		SimpleString expected = "\\uc1\\u116 t" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// multiterm_data_exporter_55
	//////////////////////////////////////////////////////////////////////////

	TEST(test_multiterm_data_exporter_55, export_gloss)
	{
		CProgressListenerDummy dummy ;
		multiterm_data_exporter_55 exporter(&dummy) ;

		exporter.set_source("Japanese") ;
		exporter.set_target("English") ;

		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = boost::shared_ptr<OutputDevice>(device) ;

		memory_pointer mem(new memory_local) ;
		record_pointer rec1(new record_local) ;
		record_pointer rec2(new record_local) ;
		rec1->set_source(L"‚è‚ñ‚²") ;
		rec1->set_trans(L"apple") ;

		rec2->set_source(L"“ú–{Œê") ;
		rec2->set_trans(L"Japanese") ;

		mem->add_record(rec1) ;
		mem->add_record(rec2) ;

		exporter.export_gloss(mem) ;

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

		ASSERT_EQUALS_V(expected, SimpleString(string2string(device->m_value).c_str())) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// test_multiterm_data_exporter_6
	//////////////////////////////////////////////////////////////////////////

	TEST(test_multiterm_data_exporter_6, export_gloss)
	{
		CProgressListenerDummy dummy ;
		multiterm_data_exporter_6 exporter(&dummy) ;

		exporter.set_source(L"Japanese") ;
		exporter.set_target(L"English") ;

		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = boost::shared_ptr<OutputDevice>(device) ;

		memory_pointer mem(new memory_local) ;
		record_pointer rec1(new record_local) ;
		record_pointer rec2(new record_local) ;
		rec1->set_source(L"‚è‚ñ‚²") ;
		rec1->set_trans(L"apple") ;

		rec2->set_source(L"“ú–{Œê") ;
		rec2->set_trans(L"Japanese") ;

		mem->add_record(rec1) ;
		mem->add_record(rec2) ;

		exporter.export_gloss(mem) ;

		wstring fmt = L"Japanese\tEnglish\tNotes\n" ;
		fmt += L"‚è‚ñ‚²\tapple\t-\n" ;
		fmt += L"“ú–{Œê\tJapanese\t-\n" ;
		SimpleString expected = string2string(fmt, CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, 
			SimpleString(string2string(device->m_value, CP_UTF8).c_str())) ;
	}

}


#endif // #ifdef _DEBUG