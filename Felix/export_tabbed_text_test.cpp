#include "stdafx.h"
#include "export_tabbed_text.h"
#include "record_local.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "output_device_fake.h"

BOOST_AUTO_TEST_SUITE( TestTabbedTextExporter )

	using namespace tabbed_export ;
	using namespace std ;
	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE(test_write_line)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);

		TabbedTextExporter exporter(output) ;
		vector<string> cols ;
		cols += "apple", "banana" ;
		exporter.write_line(cols) ;

		wstring expected = L"apple\tbanana\r\n" ;
		BOOST_CHECK_EQUAL(expected, fake_output->m_value) ;
	}

	BOOST_AUTO_TEST_CASE(sense_write_headers)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);

		TabbedTextExporter exporter(output) ;
		exporter.write_headers() ;
		BOOST_CHECK_EQUAL(1u, exporter.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("write_headers", exporter.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(2u, fake_output->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("write_string", fake_output->m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL("write_string", fake_output->m_sensing_variable[1]) ;
	}
	BOOST_AUTO_TEST_CASE(test_write_memory)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);

		TabbedTextExporter exporter(output) ;
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"source 1") ;
		rec1->set_trans(L"trans 1") ;
		rec1->set_context(L"context 1") ;
		rec1->set_reliability(5u) ;
		rec1->set_validated(true) ;
		rec1->set_created(L"1999/01/02 03:04:05") ;
		rec1->set_modified(L"2005/10/11 10:11:12") ;

		record_pointer rec2(new record_local) ;
		rec2->set_source(L"source 2") ;
		rec2->set_trans(L"trans 2") ;
		rec2->set_context(L"context 2") ;
		rec2->set_reliability(5u) ;
		rec2->set_validated(true) ;
		rec2->set_created(L"1999/01/02 03:04:05") ;
		rec2->set_modified(L"2005/10/11 10:11:12") ;

		trans_set records ;
		records.insert(rec1) ;
		records.insert(rec2) ;
		exporter.write_memory(records.begin(), records.end()) ;

		wstring expected = L"Source\tTrans\tContext\tReliability\tCreated\tModified\tVerified\r\n"
			L"source 1\ttrans 1\tcontext 1\t5\t1999/01/02 03:04:05\t2005/10/11 10:11:12\ttrue\r\n"
			L"source 2\ttrans 2\tcontext 2\t5\t1999/01/02 03:04:05\t2005/10/11 10:11:12\ttrue\r\n" ;
		BOOST_CHECK_EQUAL(expected, fake_output->m_value) ;
	}
	BOOST_AUTO_TEST_CASE(test_write_headers)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);

		TabbedTextExporter exporter(output) ;
		exporter.write_headers() ;

		wstring expected = L"Source\tTrans\tContext\tReliability\tCreated\tModified\tVerified\r\n" ;
		BOOST_CHECK_EQUAL(expected, fake_output->m_value) ;
	}
	BOOST_AUTO_TEST_CASE(sense_write_record)
	{
		output_device_ptr output (new OutputDeviceFake);

		TabbedTextExporter exporter(output) ;

		record_pointer rec(new record_local) ;
		exporter.write_record(rec) ;
		BOOST_CHECK_EQUAL(1u, exporter.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("write_record", exporter.m_sensing_variable[0]) ;
	}

	BOOST_AUTO_TEST_CASE(test_write_record)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);

		TabbedTextExporter exporter(output) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		rec->set_context(L"context") ;
		rec->set_reliability(5u) ;
		rec->set_validated(true) ;
		rec->set_created(L"1999/01/02 03:04:05") ;
		rec->set_modified(L"2005/10/11 10:11:12") ;
		exporter.write_record(rec) ;

		wstring expected = L"source\ttrans\tcontext\t5\t1999/01/02 03:04:05\t2005/10/11 10:11:12\ttrue\r\n" ;
		BOOST_CHECK_EQUAL(expected, fake_output->m_value) ;
	}
	BOOST_AUTO_TEST_CASE(test_write_records)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);

		TabbedTextExporter exporter(output) ;
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"source 1") ;
		rec1->set_trans(L"trans 1") ;
		rec1->set_context(L"context 1") ;
		rec1->set_reliability(5u) ;
		rec1->set_validated(true) ;
		rec1->set_created(L"1999/01/02 03:04:05") ;
		rec1->set_modified(L"2005/10/11 10:11:12") ;

		record_pointer rec2(new record_local) ;
		rec2->set_source(L"source 2") ;
		rec2->set_trans(L"trans 2") ;
		rec2->set_context(L"context 2") ;
		rec2->set_reliability(5u) ;
		rec2->set_validated(true) ;
		rec2->set_created(L"1999/01/02 03:04:05") ;
		rec2->set_modified(L"2005/10/11 10:11:12") ;

		trans_set records ;
		records.insert(rec1) ;
		records.insert(rec2) ;
		exporter.write_records(records.begin(), records.end()) ;

		wstring expected = L"source 1\ttrans 1\tcontext 1\t5\t1999/01/02 03:04:05\t2005/10/11 10:11:12\ttrue\r\n"
			L"source 2\ttrans 2\tcontext 2\t5\t1999/01/02 03:04:05\t2005/10/11 10:11:12\ttrue\r\n" ;
		BOOST_CHECK_EQUAL(expected, fake_output->m_value) ;
	}
	BOOST_AUTO_TEST_CASE(test_massage_text_field_newline)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);
		TabbedTextExporter exporter(output) ;
		wstring text = L"foo\nbar\r\nbaz" ;
		string actual = exporter.massage_text_field(text) ;
		string expected = "foo bar baz" ;
		BOOST_CHECK_EQUAL(actual, expected) ;

	}
	BOOST_AUTO_TEST_CASE(test_massage_text_field_tab)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output (fake_output);
		TabbedTextExporter exporter(output) ;
		wstring text = L"foo\tbaz" ;
		string actual = exporter.massage_text_field(text) ;
		string expected = "foo baz" ;
		BOOST_CHECK_EQUAL(actual, expected) ;

	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
