#include "StdAfx.h"
#include "data_importer.h"
#include "memory_local.h"
#include "ProgressListener.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( test_trados_data_importer )


	using namespace mem_engine;

	BOOST_AUTO_TEST_CASE( load )
	{
		CProgressListenerDummy dummy ;
		trados_data_importer importer(&dummy) ;

		importer.open_data_source( _T("C:\\dev\\Test Files\\Trados Parsing\\TEST_ONE_SENTENCE_SIMPLE.txt") ) ;

		importer.set_source_language( _T("JA") ) ;
		importer.set_target_language( _T("EN-US") ) ;

		mem_engine::memory_pointer mem(new mem_engine::memory_local());
		bool LoadSuccess = importer.load(_T("C:\\dev\\Test Files\\Trados Parsing\\TEST_ONE_SENTENCE_SIMPLE.txt"), mem ) ;

		BOOST_CHECK( LoadSuccess ) ;

		CStringA expected = "1" ;
		CStringA actual ;
		actual.Format( "%d", mem->size() ) ;
		BOOST_CHECK_EQUAL( expected, actual ) ;		
	}

	/*
	process lines:
	<CrD>13122002, 01:55:01
	<CrU>TANAKA
	<Seg L=EN-US>The{\cs6\f1\cf6 <:cs "Function" 1>} Argus{\cs6\f1\cf6 <:/cs>} program has been specially designed for viewing and analyzing these images.
	*/
	BOOST_AUTO_TEST_CASE( process_line_Seg )
	{
		string line = "<Seg L=EN-US>The{\\cs6\\f1\\cf6 <:cs \"Function\" 1>} Argus{\\cs6\\f1\\cf6 <:/cs>} program has been specially designed for viewing and analyzing these images." ;
		CProgressListenerDummy dummy ;
		trados_data_importer importer(&dummy) ;
		importer.clear_record() ;
		importer.m_source_language = "EN-US" ;
		importer.load_font_table() ;

		importer.process_line(line) ;
		record_pointer rec = importer.m_current_record ;

		string actual = string2string(rec->get_source_rich());
		string expected = "The<font face=\"MS UI Gothic\"><font color=\"#ff0000\">&#60;:cs \"Function\" 1></font></font> Argus<font face=\"MS UI Gothic\"><font color=\"#ff0000\">&#60;:/cs></font></font> program has been specially designed for viewing and analyzing these images." ;

		BOOST_MESSAGE(actual) ;
		BOOST_MESSAGE(expected) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( process_line_Seg_new_type )
	{
		string line = "<Seg L=EN-US>The{\\cs6\\f1\\cf6 <:cs \"Function\" 1>} Argus{\\cs6\\f1\\cf6 <:/cs>} program has been specially designed for viewing and analyzing these images." ;
		CProgressListenerDummy dummy ;
		trados_data_importer importer(&dummy) ;
		importer.clear_record() ;
		importer.m_source_language = "EN-US" ;
		importer.m_is_new_type = true ;
		importer.load_font_table() ;

		importer.process_line(line) ;
		record_pointer rec = importer.m_current_record ;

		string actual = string2string(rec->get_source_rich()).c_str() ;

		string expected = "The Argus program has been specially designed for viewing and analyzing these images." ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( process_line_CrD )
	{
		string line = "<CrD>13122002, 01:55:01" ;
		CProgressListenerDummy dummy ;
		trados_data_importer importer(&dummy) ;
		importer.clear_record() ;

		importer.process_line(line) ;
		record_pointer rec = importer.m_current_record ;

		misc_wrappers::date created = rec->get_created() ;

		BOOST_CHECK_EQUAL(2002, (int)created.wYear) ;
		BOOST_CHECK_EQUAL(12, (int)created.wMonth) ;
		BOOST_CHECK_EQUAL(13, (int)created.wDay) ;
		BOOST_CHECK_EQUAL(1, (int)created.wHour) ;
		BOOST_CHECK_EQUAL(55, (int)created.wMinute) ;
		BOOST_CHECK_EQUAL(1, (int)created.wSecond) ;
	}
	BOOST_AUTO_TEST_CASE( process_line_CrU )
	{
		string line = "<CrU>TANAKA" ;
		CProgressListenerDummy dummy ;
		trados_data_importer importer(&dummy) ;
		importer.clear_record() ;

		importer.process_line(line) ;
		record_pointer rec = importer.m_current_record ;

		string actual = string2string(rec->get_creator()).c_str() ;

		BOOST_CHECK_EQUAL("TANAKA", actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
