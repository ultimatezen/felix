#include "StdAfx.h"
#include "data_exporter.h"
#include "record_local.h"
#include "memory_local.h"
#include "ProgressListener.h"
#include "output_device_fake.h"
#include "MockListener.h"
#include "felix_factory.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

	using namespace mem_engine ;
	using namespace app_props ;
	//////////////////////////////////////////////////////////////////////////
	// TradosDataExporter
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( test_TestTradosDataExporter )

	BOOST_AUTO_TEST_CASE( internal_date_to_trados_date )
	{
		std::set< wstring > fonts ;
		CMockListener listener ;
		properties_memory mem_props ;
		TradosDataExporter exporter(fonts, &listener, &mem_props) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = output_device_ptr (device) ;

		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 1 ;
		thedate.wDay = 17 ;
		thedate.wHour = 12; 
		thedate.wMinute = 0 ;
		thedate.wSecond = 0 ;
		thedate.wMilliseconds = 0 ;

		string actual = exporter.internal_date_to_trados_date(thedate) ;
		string expected = "17012000, 12:00:00" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( open_destination )
	{
		std::set< wstring > fonts ;
		CMockListener listener ;
		properties_memory mem_props ;
		TradosDataExporter exporter(fonts, &listener, &mem_props) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = output_device_ptr (device) ;

		exporter.open_destination(_T("foo.txt")) ;
		BOOST_CHECK_EQUAL(2u, device->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string("open"), device->m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(string("foo.txt"), device->m_sensing_variable[1]) ;
	}
	BOOST_AUTO_TEST_CASE( write_preamble )
	{
		std::set< wstring > fonts ;
		CMockListener listener ;
		properties_memory mem_props ;
		TradosDataExporter exporter(fonts, &listener, &mem_props) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = output_device_ptr (device) ;

		exporter.write_preamble() ;
		BOOST_CHECK_EQUAL(3, (int)device->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string("write_string"), device->m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(string("write_string"), device->m_sensing_variable[1]) ;
		BOOST_CHECK_EQUAL(string("write_string"), device->m_sensing_variable[2]) ;
	}
	BOOST_AUTO_TEST_CASE( create_unicode_escape )
	{
		std::set< wstring > fonts ;
		CMockListener listener ;
		properties_memory mem_props ;
		TradosDataExporter exporter(fonts, &listener, &mem_props) ;

		string escape = exporter.create_unicode_escape(L't', 't') ;
		string actual = escape ;
		string expected = "\\uc1\\u116 t" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	// multiterm_data_exporter_55
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( test_multiterm_data_exporter_55 )

	BOOST_AUTO_TEST_CASE( export_gloss)
	{
		CMockListener dummy ;
		multiterm_data_exporter_55 exporter(&dummy) ;

		exporter.set_source("Japanese") ;
		exporter.set_target("English") ;

		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = boost::shared_ptr<OutputDevice>(device) ;

		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
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
		string expected = (format(fmt)
					% "Japanese" % "English"
					% string2string(rec1->get_source_plain())
					% string2string(rec1->get_trans_plain())
					% string2string(rec2->get_source_plain())
					% string2string(rec2->get_trans_plain())
					).str() ;

		BOOST_CHECK_EQUAL(expected, string(string2string(device->m_value))) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	// test_multiterm_data_exporter_6
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( test_multiterm_data_exporter_6 )

	BOOST_AUTO_TEST_CASE( export_gloss)
	{
		CMockListener dummy ;
		multiterm_data_exporter_6 exporter(&dummy) ;

		exporter.set_source(L"Japanese") ;
		exporter.set_target(L"English") ;

		OutputDeviceFake *device = new OutputDeviceFake ;
		exporter.m_file = boost::shared_ptr<OutputDevice>(device) ;

		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
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
		string expected = string2string(fmt, CP_UTF8) ;

		BOOST_CHECK_EQUAL(expected, 
			string(string2string(device->m_value, CP_UTF8))) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
