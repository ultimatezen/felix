/*!
	Unit tests for CTMXReader class
 */

#include "StdAfx.h"
#include "tmxreader.h"
#include "record_local.h"
#include "memory_local.h"
#include "memory_info.h"
#include "MockListener.h"
#include "input_device_file.h"
#include "logging.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "input_device_fake.h"
#include "string_logger.h"

// File definitions for testing
#define EXPORT_TEST_FILE_1A _T("..\\TestData\\ExportTest1A.tmx")
#define BROKEN_TMX_FILE _T("..\\TestData\\broken.tmx")

const static string TEST_1A_TEXT = string2string(wstring(L"<?xml version=\"1.0\" ?>"
L"<!DOCTYPE tmx SYSTEM \"tmx14.dtd\">"
L"<tmx version=\"1.4\">"
L" <header creationtoolversion=\"1.0.0\" datatype=\"winres\" segtype=\"sentence\" adminlang=\"EN-US\" srclang=\"EN-US\" o-tmf=\"abc\" creationtool=\"XYZTool\" >"
L" </header>"
L" <body>"
L"  <tu>"
L"   <tuv xml:lang=\"en-us\"><seg>Text with 3 spaces [   ] and 2 [  ]</seg></tuv>"
L"   <tuv xml:lang=\"fr-ca\"><seg>Texte avec 3 espaces [   ] et 2 [  ]</seg></tuv>"
L"  </tu>"
L"  <tu>"
L"   <tuv xml:lang=\"en-us\"><seg>Text with 1 tab [	] and 2 [		]</seg></tuv>"
L"   <tuv xml:lang=\"fr-ca\"><seg>Texte avec 1 tab [	] et 2 [		]</seg></tuv>"
L"  </tu>"
L"  <tu>"
L"   <tuv xml:lang=\"en-us\"><seg>&lt;=lt, >=gt, &amp;=amp, {=obrace, }=cbrace</seg></tuv>"
L"   <tuv xml:lang=\"fr-ca\"><seg>&lt;=lt, >=gt, &amp;=amp, {=obrace, }=cbrace</seg></tuv>"
L"  </tu>"
L"  <tu>"
L"   <tuv xml:lang=\"en-us\"><seg>é=eacute,  =nbsp, ¤=curr, ÿ=yumlaut, ©=copy, §=para</seg></tuv>"
L"   <tuv xml:lang=\"fr-ca\"><seg>é=eacute,  =nbsp, ¤=curr, ÿ=yumlaut, ©=copy, §=para</seg></tuv>"
L"  </tu>"
L"  </body>"
L"</tmx>"));


BOOST_FIXTURE_TEST_SUITE( CTMXReaderTestCase, StringLoggerTester )

	using namespace mem_engine ;
	using namespace except ;


	BOOST_AUTO_TEST_CASE( get_bom )
	{
		app_props::properties_memory props; 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(app_props::get_props())) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		input_device_ptr input(new InputDeviceFile) ;
		BOOST_CHECK_EQUAL ( file::BOM_UTF8, tmx_reader.get_bom( EXPORT_TEST_FILE_1A, input) ) ; 
	}

	BOOST_AUTO_TEST_CASE( test_broken_tmx_file )
	{
		app_props::properties_memory props; 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(app_props::get_props())) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		try
		{
			input_device_ptr input(new InputDeviceFile) ;
			tmx_reader.load_tmx_memory( BROKEN_TMX_FILE, input ) ;

			BOOST_CHECK_EQUAL(1u, mem->size()) ;  
			BOOST_CHECK_EQUAL(1u, tmx_reader.m_errors.size()) ;
			BOOST_CHECK(tmx_reader.m_memory->is_saved()) ;
		}
		catch ( CException &e ) 
		{
			string reason = CT2A( e.format_message_for_message_box() ) ;
			BOOST_FAIL( reason.c_str() ) ;
		}
	}
	BOOST_AUTO_TEST_CASE( test_broken_tmx_file_log )
	{
		app_props::properties_memory props; 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(app_props::get_props())) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		input_device_ptr input(new InputDeviceFile) ;
		tmx_reader.load_tmx_memory( BROKEN_TMX_FILE, input ) ;

		string logout = get_log();

		BOOST_CHECK(logout.find("TMX Reader failed to find end of TU segment") != string::npos) ;
	}
	BOOST_AUTO_TEST_CASE( srclang_in_languages_true )
	{
		std::set<tstring> languages ;
		languages.insert(L"EN") ;
		languages.insert(L"JA") ;

		app_props::properties_memory props; 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(app_props::get_props())) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;
		tmx_reader.m_header.m_srclang = L"EN" ;
		BOOST_CHECK(tmx_reader.srclang_in_languages(languages)) ; 
	}
	BOOST_AUTO_TEST_CASE( srclang_in_languages_false)
	{
		std::set<tstring> languages ;
		languages.insert(L"EN") ;
		languages.insert(L"JA") ;

		app_props::properties_memory props; 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(app_props::get_props())) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;
		tmx_reader.m_header.m_srclang = L"*all*" ;
		BOOST_CHECK(! tmx_reader.srclang_in_languages(languages)) ; 
	}

	BOOST_AUTO_TEST_CASE(load_tmx_memory)
	{
		app_props::properties_memory props; 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(app_props::get_props())) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		try
		{
			InputDeviceFake *fake = new InputDeviceFake ;
			fake->set_view(TEST_1A_TEXT) ;
			input_device_ptr input(fake) ;
			tmx_reader.load_tmx_memory( EXPORT_TEST_FILE_1A, input ) ;

			BOOST_CHECK(tmx_reader.m_memory->is_saved()) ;

			mem_engine::MemoryInfo *mem_info = mem->get_memory_info() ;

			string actual = (LPCSTR)CStringA(mem_info->get_creation_tool().c_str()) ;
			BOOST_CHECK_EQUAL( "XYZTool", actual) ; 
			actual = (LPCSTR)CStringA(mem_info->get_creation_tool_version().c_str()) ;
			BOOST_CHECK_EQUAL( "1.0.0", actual ) ; 

			actual = (LPCSTR)CStringA(mem_info->get_source_language().c_str()) ;
			BOOST_CHECK_EQUAL( "EN-US", actual ) ;
			actual = (LPCSTR)CStringA(mem_info->get_target_language().c_str()) ;
			BOOST_CHECK_EQUAL( "FR-CA", actual ) ;

			BOOST_CHECK_EQUAL ( 4u, mem->size() ) ;  
		}
		catch ( CException &e ) 
		{
			string reason = CT2A( e.what() ) ;
			BOOST_FAIL( reason.c_str() ) ;
		}

	}
	BOOST_AUTO_TEST_CASE( TestGetSegText )
	{
		app_props::properties_memory props; 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(app_props::get_props())) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		wstring toStrip = L"<seg><it pos=\"begin\" x=\"1\">&lt;1&gt;</it>Digitally Sign a Macro Project in Microsoft Word 2000</seg>" ;
		BOOST_CHECK_EQUAL( tmx_reader.get_seg_text( toStrip ), L"Digitally Sign a Macro Project in Microsoft Word 2000") ;
	}
BOOST_AUTO_TEST_SUITE_END()

	// CTU
BOOST_AUTO_TEST_SUITE( CTMXReader_CTU_TestCase )
	using namespace mem_engine ;
	using namespace except ;


	BOOST_AUTO_TEST_CASE( set_attributes_tuid )
	{
		tmx_reader::CTU ctu ;

		std::map<wstring, wstring> attributes ;
		attributes[L"tuid"] = L"10" ;

		ctu.set_attributes(attributes) ;

		BOOST_CHECK_EQUAL(ctu.m_tuid, L"10") ;
	}

	BOOST_AUTO_TEST_CASE( reflect_attributes_creator )
	{
		tmx_reader::CTU ctu ;
		ctu.m_creationid = L"Ryan" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;
		BOOST_CHECK_EQUAL(rec->get_creator(), L"Ryan") ;
	}
	BOOST_AUTO_TEST_CASE( reflect_attributes_modified_by )
	{
		tmx_reader::CTU ctu ;
		ctu.m_changeid = L"Ryan" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;
		BOOST_CHECK_EQUAL(rec->get_modified_by(), L"Ryan") ;
	}
	BOOST_AUTO_TEST_CASE( reflect_attributes_created )
	{
		tmx_reader::CTU ctu ;
		ctu.m_creationdate = L"20001011T123456Z" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;

		misc_wrappers::date actual = rec->get_created() ;

		BOOST_CHECK_EQUAL(2000, (int)actual.wYear) ;
		BOOST_CHECK_EQUAL(10, (int)actual.wMonth) ;
		BOOST_CHECK_EQUAL(11, (int)actual.wDay) ;

		BOOST_CHECK_EQUAL(12, (int)actual.wHour) ;
		BOOST_CHECK_EQUAL(34, (int)actual.wMinute) ;
		BOOST_CHECK_EQUAL(56, (int)actual.wSecond) ;
	}
	BOOST_AUTO_TEST_CASE( reflect_attributes_modified )
	{
		tmx_reader::CTU ctu ;
		ctu.m_changedate = L"20001011T123456Z" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;

		misc_wrappers::date actual = rec->get_modified() ;

		BOOST_CHECK_EQUAL(2000, (int)actual.wYear) ;
		BOOST_CHECK_EQUAL(10, (int)actual.wMonth) ;
		BOOST_CHECK_EQUAL(11, (int)actual.wDay) ;

		BOOST_CHECK_EQUAL(12, (int)actual.wHour) ;
		BOOST_CHECK_EQUAL(34, (int)actual.wMinute) ;
		BOOST_CHECK_EQUAL(56, (int)actual.wSecond) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif

