/*!
	@brief Unit tests for CTMXReader class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include ".\tmxreader.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{

	using namespace memory_engine ;

#define EXPORT_TEST_FILE_1A _T("C:\\dev\\Test Files\\TMXComplianceKit\\ExportTest1A.tmx")

	TEST(CTMXReaderTestCase, get_bom )
	{
		memory_engine::memory_pointer mem(new memory_engine::memory_local()) ;
		CProgressListenerDummy dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		ASSERT_EQUALS ( file::file::UTF8_BOM, tmx_reader.get_bom( EXPORT_TEST_FILE_1A ) ) ; 
	}

	TEST(CTMXReaderTestCase,load_tmx_memory)
	{
		memory_engine::memory_pointer mem(new memory_engine::memory_local()) ;
		CProgressListenerDummy dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		try
		{
			tmx_reader.load_tmx_memory( EXPORT_TEST_FILE_1A ) ;

			memory_engine::memory_header mem_header = mem->get_header() ;

			SimpleString actual = (LPCSTR)CStringA(mem_header.get_creation_tool().c_str()) ;
			ASSERT_EQUALS_V( "XYZTool", actual) ; 
			actual = (LPCSTR)CStringA(mem_header.get_creation_tool_version().c_str()) ;
			ASSERT_EQUALS_V( "1.0.0", actual ) ; 

			actual = (LPCSTR)CStringA(mem_header.get_source_language().c_str()) ;
			ASSERT_EQUALS_V( "EN-US", actual ) ;
			actual = (LPCSTR)CStringA(mem_header.get_target_language().c_str()) ;
			ASSERT_EQUALS_V( "FR-CA", actual ) ;

			ASSERT_EQUALS ( 4u, mem->size() ) ;  
		}
		catch ( CException &e ) 
		{
			string reason = CT2A( e.what() ) ;
			FAIL_M( reason.c_str() ) ;
		}

	}
	TEST( CTMXReaderTestCase, TestGetSegText )
	{
		memory_pointer mem(new memory_engine::memory_local()) ;
		CProgressListenerDummy dummy ;
		CTMXReader tmx_reader( mem, static_cast< CProgressListener* >( &dummy ) ) ;

		wstring toStrip = L"<seg><it pos=\"begin\" x=\"1\">&lt;1&gt;</it>Digitally Sign a Macro Project in Microsoft Word 2000</seg>" ;
		ASSERT_EQUALS_M( tmx_reader.get_seg_text( toStrip ), L"Digitally Sign a Macro Project in Microsoft Word 2000", "String should be 'Digitally Sign a Macro Project in Microsoft Word 2000'" ) ;
	}
	// CTU
	TEST( CTMXReader_CTU_TestCase, set_attributes_tuid )
	{
		tmx_reader::CTU ctu ;

		std::map<wstring, wstring> attributes ;
		attributes[L"tuid"] = L"10" ;

		ctu.set_attributes(attributes) ;

		ASSERT_EQUALS(ctu.m_tuid, L"10") ;
	}

	TEST( CTMXReader_CTU_TestCase, reflect_attributes_creator )
	{
		tmx_reader::CTU ctu ;
		ctu.m_creationid = L"Ryan" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;
		ASSERT_EQUALS(rec->get_creator(), L"Ryan") ;
	}
	TEST( CTMXReader_CTU_TestCase, reflect_attributes_modified_by )
	{
		tmx_reader::CTU ctu ;
		ctu.m_changeid = L"Ryan" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;
		ASSERT_EQUALS(rec->get_modified_by(), L"Ryan") ;
	}
	TEST( CTMXReader_CTU_TestCase, reflect_attributes_created )
	{
		tmx_reader::CTU ctu ;
		ctu.m_creationdate = L"20001011T123456Z" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;

		misc_wrappers::date actual = rec->get_created() ;

		ASSERT_EQUALS_V(2000, (int)actual.wYear) ;
		ASSERT_EQUALS_V(10, (int)actual.wMonth) ;
		ASSERT_EQUALS_V(11, (int)actual.wDay) ;

		ASSERT_EQUALS_V(12, (int)actual.wHour) ;
		ASSERT_EQUALS_V(34, (int)actual.wMinute) ;
		ASSERT_EQUALS_V(56, (int)actual.wSecond) ;
	}
	TEST( CTMXReader_CTU_TestCase, reflect_attributes_modified )
	{
		tmx_reader::CTU ctu ;
		ctu.m_changedate = L"20001011T123456Z" ;

		record_pointer rec(new record_local) ;

		ctu.reflect_attributes(rec) ;

		misc_wrappers::date actual = rec->get_modified() ;

		ASSERT_EQUALS_V(2000, (int)actual.wYear) ;
		ASSERT_EQUALS_V(10, (int)actual.wMonth) ;
		ASSERT_EQUALS_V(11, (int)actual.wDay) ;

		ASSERT_EQUALS_V(12, (int)actual.wHour) ;
		ASSERT_EQUALS_V(34, (int)actual.wMinute) ;
		ASSERT_EQUALS_V(56, (int)actual.wSecond) ;
	}
}

#endif

