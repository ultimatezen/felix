#include "stdafx.h"
#include "TabbedTextImporter.h"
#include "MockListener.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCTabbedTextImporter )

	using namespace mem_engine ;
	BOOST_AUTO_TEST_CASE( intialize )
	{
		BOOST_CHECK_NO_THROW( CTabbedTextImporter(NULL) ) ;
	}

	BOOST_AUTO_TEST_CASE( add_line_no_context )
	{
		CTabbedTextImporter importer(NULL) ;
		wstring line = L"source\ttrans" ;
		importer.add_record(line) ;

		BOOST_CHECK_EQUAL(importer.m_memory->size(), 1u) ;
	}

	BOOST_AUTO_TEST_CASE( add_line_with_context )
	{
		CTabbedTextImporter importer(NULL) ;
		wstring line = L"source\ttrans\tcontext" ;
		importer.add_record(line) ;

		BOOST_CHECK_EQUAL(importer.m_memory->size(), 1u) ;
		record_pointer rec = importer.m_memory->get_record_at(0) ;
		BOOST_CHECK_EQUAL(L"context", rec->get_context_rich()) ;
	}

	BOOST_AUTO_TEST_CASE( add_records )
	{
		CMockListener listener ;
		CTabbedTextImporter importer(&listener) ;
		wstring text = L"source 1\ttrans 1\r\nsource 2\ttrans 2" ;
		importer.add_records(text) ;

		BOOST_CHECK_EQUAL(importer.m_memory->size(), 2u) ;
	}

	BOOST_AUTO_TEST_CASE( add_file )
	{
		CMockListener listener ;
		CTabbedTextImporter importer(&listener) ;
		CString filename = _T("C:\\test\\tab_delim_gloss.txt") ;
		importer.load_file(filename) ;

		BOOST_CHECK_EQUAL(importer.m_memory->size(), 2u) ;
		record_pointer rec = importer.m_memory->get_record_at(0) ;
		BOOST_CHECK_EQUAL(L"�p��", rec->get_source_rich()) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif
