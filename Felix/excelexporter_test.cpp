#include "stdafx.h"
#include "excelexporter.h"
#include "memory_local.h"
#include "record_local.h"
#include "felix_factory.h"
#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

#include "ExcelInterfaceFake.h"
#include "MockListener.h"
#include "input_device_fake.h"

using namespace mem_engine ;

BOOST_AUTO_TEST_SUITE( TestCExcelExporter )

	BOOST_AUTO_TEST_CASE( process_cell_text_empty)
	{
		wstring text ;
		wstring out = process_cell_text(text) ;
		BOOST_CHECK(out.empty()) ;
	}
	BOOST_AUTO_TEST_CASE( process_cell_text_foo)
	{
		wstring text = L"foo";
		wstring out = process_cell_text(text) ;
		BOOST_CHECK_EQUAL(string2string(out), "foo") ;
	}
	BOOST_AUTO_TEST_CASE( process_cell_text_equals_sign)
	{
		wstring text = L"=foo" ;
		wstring out = process_cell_text(text) ;
		BOOST_CHECK_EQUAL(string2string(out), "\'=foo") ;
	}

	// write_header
	BOOST_AUTO_TEST_CASE( test_write_header_cells )
	{
		ExcelInterfaceFake *excel = new ExcelInterfaceFake ;
		ExcelInterfacePtr excelptr(excel) ;

		CMockListener listener ;
		CExcelExporter exporter(&listener, excelptr) ;
		exporter.write_header() ;

		typedef ExcelInterfaceFake::celltext celltext;
		typedef ExcelInterfaceFake::cellcoord cellcoord;

		std::vector<celltext> expected ;
		expected += celltext(cellcoord(1, 1), L"Source"),
			celltext(cellcoord(1, 2), L"Trans"),
			celltext(cellcoord(1, 3), L"Context"),
			celltext(cellcoord(1, 4), L"Reliability"),
			celltext(cellcoord(1, 5), L"Created"),
			celltext(cellcoord(1, 6), L"Modified"),
			celltext(cellcoord(1, 7), L"Verified") ;

		for (size_t i = 0 ; i < expected.size() ; ++i)
		{
			BOOST_CHECK_EQUAL(excel->m_cell_text[i].first.first, expected[i].first.first) ;
			BOOST_CHECK_EQUAL(excel->m_cell_text[i].first.second, expected[i].first.second) ;
			BOOST_CHECK_EQUAL(excel->m_cell_text[i].second, expected[i].second) ;
		}
	}
	BOOST_AUTO_TEST_CASE( test_write_header_bold )
	{
		ExcelInterfaceFake *excel = new ExcelInterfaceFake ;
		ExcelInterfacePtr excelptr(excel) ;

		CMockListener listener ;
		CExcelExporter exporter(&listener, excelptr) ;
		exporter.write_header() ;

		typedef ExcelInterfaceFake::cellcoord cellcoord;

		std::vector<cellcoord> expected ;
		expected += cellcoord(1, 1),
			cellcoord(1, 2),
			cellcoord(1, 3),
			cellcoord(1, 4),
			cellcoord(1, 5),
			cellcoord(1, 6),
			cellcoord(1, 7) ;

		for (size_t i = 0 ; i < expected.size() ; ++i)
		{
			BOOST_CHECK_EQUAL(excel->m_bold_cells[i].first, expected[i].first) ;
			BOOST_CHECK_EQUAL(excel->m_bold_cells[i].second, expected[i].second) ;
		}

		BOOST_CHECK(excel->m_location == L"") ;
	}
	// write_record
	BOOST_AUTO_TEST_CASE( test_write_record )
	{
		ExcelInterfaceFake *excel = new ExcelInterfaceFake ;
		ExcelInterfacePtr excelptr(excel) ;

		CMockListener listener ;
		CExcelExporter exporter(&listener, excelptr) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"<bold>trans</bold>") ;
		rec->set_context(L"&lt;foo&gt;") ;
		rec->set_reliability(1u) ;
		rec->set_created(L"1999/10/01 09:15:14") ;
		rec->set_modified(L"2008/11/12 11:12:13") ;
		exporter.write_record(rec, 2) ;

		typedef ExcelInterfaceFake::celltext celltext;
		typedef ExcelInterfaceFake::cellcoord cellcoord;

		std::vector<celltext> expected ;
		expected += celltext(cellcoord(2, 1), L"source"),
			celltext(cellcoord(2, 2), L"trans"),
			celltext(cellcoord(2, 3), L"<foo>"),
			celltext(cellcoord(2, 4), L"1"),
			celltext(cellcoord(2, 5), L"1999/10/01 09:15:14"),
			celltext(cellcoord(2, 6), L"2008/11/12 11:12:13"),
			celltext(cellcoord(2, 7), L"false") ;

		for (size_t i = 0 ; i < expected.size() ; ++i)
		{
			BOOST_CHECK_EQUAL(excel->m_cell_text[i].first.first, expected[i].first.first) ;
			BOOST_CHECK_EQUAL(excel->m_cell_text[i].first.second, expected[i].first.second) ;
			BOOST_CHECK_EQUAL(excel->m_cell_text[i].second, expected[i].second) ;
		}
	}
	// export_excel
	BOOST_AUTO_TEST_CASE( test_export_excel_empty_mem )
	{
		ExcelInterfaceFake *excel = new ExcelInterfaceFake ;
		ExcelInterfacePtr excelptr(excel) ;

		CMockListener listener ;
		CExcelExporter exporter(&listener, excelptr) ;

		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;

		input_device_ptr input(new InputDeviceFake) ;
		exporter.export_excel(mem, _T("foo.xls"), input) ;

		typedef ExcelInterfaceFake::cellcoord cellcoord;

		std::vector<cellcoord> expected ;
		expected += cellcoord(1, 1),
			cellcoord(1, 2),
			cellcoord(1, 3),
			cellcoord(1, 4),
			cellcoord(1, 5),
			cellcoord(1, 6),
			cellcoord(1, 7) ;

		for (size_t i = 0 ; i < expected.size() ; ++i)
		{
			BOOST_CHECK_EQUAL(excel->m_bold_cells[i].first, expected[i].first) ;
			BOOST_CHECK_EQUAL(excel->m_bold_cells[i].second, expected[i].second) ;
		}
		BOOST_CHECK_EQUAL(7u, excel->m_cell_text.size()) ;
		BOOST_CHECK(excel->m_location == L"foo.xls") ;
	}

	BOOST_AUTO_TEST_CASE( test_export_excel_non_empty_mem )
	{
		ExcelInterfaceFake *excel = new ExcelInterfaceFake ;
		ExcelInterfacePtr excelptr(excel) ;

		CMockListener listener ;
		CExcelExporter exporter(&listener, excelptr) ;

		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"<bold>trans</bold>") ;

		mem->add_record(rec) ;

		input_device_ptr input(new InputDeviceFake) ;
		exporter.export_excel(mem, _T("foo.xls"), input) ;

		typedef ExcelInterfaceFake::cellcoord cellcoord;

		BOOST_CHECK_EQUAL(excel->m_cell_text[7].second, wstring(L"source")) ;
		BOOST_CHECK_EQUAL(excel->m_cell_text[8].second, wstring(L"trans")) ;
	}


BOOST_AUTO_TEST_SUITE_END()

#endif