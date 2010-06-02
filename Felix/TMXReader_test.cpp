/*!
	@brief Unit tests for CTMXReader class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include ".\tmxreader.h"
#include "record_local.h"
#include "memory_local.h"
#include "memory_info.h"
#include "MockListener.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CTMXReaderTestCase )

	using namespace mem_engine ;
	using namespace except ;

#define EXPORT_TEST_FILE_1A _T("C:\\dev\\Test Files\\TMXComplianceKit\\ExportTest1A.tmx")

	BOOST_AUTO_TEST_CASE( get_bom )
	{
		mem_engine::memory_pointer mem(new mem_engine::memory_local()) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		BOOST_CHECK_EQUAL ( file::file::UTF8_BOM, tmx_reader.get_bom( EXPORT_TEST_FILE_1A ) ) ; 
	}
	BOOST_AUTO_TEST_CASE( srclang_in_languages_true )
	{
		std::set<tstring> languages ;
		languages.insert(L"EN") ;
		languages.insert(L"JA") ;

		mem_engine::memory_pointer mem(new mem_engine::memory_local()) ;
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

		mem_engine::memory_pointer mem(new mem_engine::memory_local()) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;
		tmx_reader.m_header.m_srclang = L"*all*" ;
		BOOST_CHECK(! tmx_reader.srclang_in_languages(languages)) ; 
	}

	BOOST_AUTO_TEST_CASE(load_tmx_memory)
	{
		mem_engine::memory_pointer mem(new mem_engine::memory_local()) ;
		CMockListener dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		try
		{
			tmx_reader.load_tmx_memory( EXPORT_TEST_FILE_1A ) ;

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
		memory_pointer mem(new mem_engine::memory_local()) ;
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

