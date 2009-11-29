#include "StdAfx.h"
#include "data_exporter.h"

#include "easyunit/testharness.h"
#include "output_device_fake.h"

#ifdef _DEBUG

using namespace memory_engine ;

namespace easyunit
{
	// cp_from_lang_str
	TEST( TestTradosDataExporter, test_intantiate )
	{
		std::set< wstring > fonts ;
		CProgressListenerDummy listener ;
		TradosDataExporter exporter(fonts, &listener) ;
	}
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