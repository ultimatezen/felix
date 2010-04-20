#include "StdAfx.h"
#include "data_converter.h"
#include "record_local.h"

#include "output_device_fake.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCXml2RecordConverter )

	//////////////////////////////////////////////////////////////////////////
	// TestCXml2RecordConverter
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( load_source )
	{
		LPCWSTR text = L"<![CDATA[spam]]></source><trans><![CDATA[context]]></trans>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_source() ;
		mem_engine::record_pointer record = converter.m_record ;
		BOOST_CHECK(record->get_source_rich() == L"spam") ;
		BOOST_CHECK(record->get_trans_rich() == L"") ;
	}
	BOOST_AUTO_TEST_CASE( load_trans )
	{
		LPCWSTR text = L"<![CDATA[spam]]></trans><context><![CDATA[context]]></context>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_trans() ;
		mem_engine::record_pointer record = converter.m_record ;
		BOOST_CHECK(record->get_source_rich() == L"") ;
		BOOST_CHECK(record->get_trans_rich() == L"spam") ;
		BOOST_CHECK(record->get_context_rich() == L"") ;
	}
	BOOST_AUTO_TEST_CASE( load_context)
	{
		LPCWSTR text = L"<![CDATA[spam]]></context><trans><![CDATA[context]]></trans>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_context() ;
		mem_engine::record_pointer record = converter.m_record ;
		BOOST_CHECK(record->get_context_rich() == L"spam") ;
		BOOST_CHECK(record->get_source_rich() == L"") ;
		BOOST_CHECK(record->get_trans_rich() == L"") ;
	}
	BOOST_AUTO_TEST_CASE( load_richtext_node)
	{
		LPCWSTR text = L"<![CDATA[spam]]></context><trans><![CDATA[context]]></trans>" ;
		_bstr_t end_tag = L"</context>" ;
		LPCWSTR start, end ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_richtext_node(end_tag, start, end) ;
		wstring context(start, end) ;
		BOOST_CHECK(context == L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( load_date_created)
	{
		LPCWSTR text = L"2009/12/05 21:06:10</date_created>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_date_created() ;
		mem_engine::record_pointer record = converter.m_record ;
		misc_wrappers::date created = record->get_created() ;
		BOOST_CHECK_EQUAL(2009, (int)created.wYear) ;
		BOOST_CHECK_EQUAL(12, (int)created.wMonth) ;
		BOOST_CHECK_EQUAL(5, (int)created.wDay) ;
		BOOST_CHECK_EQUAL(21, (int)created.wHour) ;
		BOOST_CHECK_EQUAL(6, (int)created.wMinute) ;
		BOOST_CHECK_EQUAL(10, (int)created.wSecond) ;
	}
	BOOST_AUTO_TEST_CASE( load_extra_record_throws)
	{
		wstring tag = L"record" ;
		CXml2RecordConverter converter ;
		BOOST_CHECK_THROW(converter.load_extra(tag), except::CException) ;
	}
	BOOST_AUTO_TEST_CASE( load_last_modified)
	{
		LPCWSTR text = L"2009/12/05 21:06:10</last_modified>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_last_modified() ;
		mem_engine::record_pointer record = converter.m_record ;
		misc_wrappers::date modified = record->get_modified() ;
		BOOST_CHECK_EQUAL(2009, (int)modified.wYear) ;
		BOOST_CHECK_EQUAL(12, (int)modified.wMonth) ;
		BOOST_CHECK_EQUAL(5, (int)modified.wDay) ;
		BOOST_CHECK_EQUAL(21, (int)modified.wHour) ;
		BOOST_CHECK_EQUAL(6, (int)modified.wMinute) ;
		BOOST_CHECK_EQUAL(10, (int)modified.wSecond) ;
	}
	BOOST_AUTO_TEST_CASE( test_convert_from_node )
	{
		LPCWSTR text = L">"
			L"<source><![CDATA[spam]]></source>"
			L"<trans><![CDATA[egg]]></trans>"
			L"<id>10</id>"
			L"</record>" ;

		CXml2RecordConverter converter ;
		mem_engine::record_pointer record = converter.convert_from_xml_node(text) ;
		BOOST_CHECK(record->get_source_rich() == L"spam") ;
		BOOST_CHECK(record->get_trans_rich() == L"egg") ;
		BOOST_CHECK_EQUAL(10, (int)record->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE( test_convert_from_node_creator )
	{
		LPCWSTR text = L">"
			L"<id>10</id>\n"
			L"<source><![CDATA[spam]]></source>\n"
			L"<trans><![CDATA[egg]]></trans>\n"
			L"<creator>creator abc</creator>\n"
			L"<modified_by>modified by xyz</modified_by>\n"
			L"</record>" ;

		CXml2RecordConverter converter ;
		mem_engine::record_pointer record = converter.convert_from_xml_node(text) ;
		BOOST_CHECK_EQUAL(record->get_creator(), L"creator abc") ;
	}
	BOOST_AUTO_TEST_CASE( test_convert_from_node_modified_by )
	{
		LPCWSTR text = L">"
			L"<id>10</id>\n"
			L"<source><![CDATA[spam]]></source>\n"
			L"<trans><![CDATA[egg]]></trans>\n"
			L"<creator>creator abc</creator>\n"
			L"<modified_by>modified by xyz</modified_by>\n"
			L"</record>" ;

		CXml2RecordConverter converter ;
		mem_engine::record_pointer record = converter.convert_from_xml_node(text) ;
		BOOST_CHECK_EQUAL(record->get_modified_by(), L"modified by xyz") ;
	}
	BOOST_AUTO_TEST_CASE( PlainTextNode_plain)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"spam</foo>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		string actual = string2string(converter.PlainTextNode(end_tag)).c_str() ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( PlainTextNode_rich)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"<![CDATA[spam]]></foo>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		string actual = string2string(converter.PlainTextNode(end_tag)).c_str() ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( strip_cdata)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"<![CDATA[spam]]></foo><other>" ;
		string actual = string2string(converter.strip_cdata(text)).c_str() ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( SizeNode)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"1001</foo>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		int actual = (int)converter.SizeNode(end_tag) ;
		int expected = 1001 ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( SizeNode_rich)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"<![CDATA[54]]></foo><other>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		int actual = (int)converter.SizeNode(end_tag) ;
		int expected = 54 ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_validated_value_true)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"true</validated>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		BOOST_CHECK(converter.get_validated_value()) ;
	}
	BOOST_AUTO_TEST_CASE( get_validated_value_false)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"false</validated>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		BOOST_CHECK(!converter.get_validated_value()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

	//////////////////////////////////////////////////////////////////////////
	// CRecord2XmlConverter
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( test_CRecord2XmlConverter )

	BOOST_AUTO_TEST_CASE( rec2xml_init_char_conversion)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;
		BOOST_CHECK_EQUAL(string("\1"), converter.m_from_strings[0]) ;
		BOOST_CHECK_EQUAL(string("&#1;"), converter.m_to_strings[0]) ;
	}
	BOOST_AUTO_TEST_CASE( rec2xml_convert_from_record)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;

		mem_engine::record_pointer rec(new mem_engine::record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		rec->set_id(10) ;

		converter.convert_from_record(rec) ;

		string actual = string2string(output_device.m_value) ;
		CStringA err ;
		err.Format("Record text: %s", actual.c_str()) ;
		BOOST_CHECK(actual.find("<id>10</id>") != string::npos) ;
	}

	BOOST_AUTO_TEST_CASE( rec2xml_convert_cdata_node_spam)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;

		string start_tag = "<spam>" ;
		string end_tag = "</spam>" ;
		wstring text = L"spam" ;
		converter.convert_cdata_node(start_tag, end_tag, text) ;

		CStringA actual = output_device.m_value.c_str() ;
		CStringA expected = "<spam><![CDATA[spam]]></spam>" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( rec2xml_convert_cdata_node_control_codes)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;

		string start_tag = "<spam>" ;
		string end_tag = "</spam>" ;
		wstring text ;
		text += wchar_t(1) ;
		text += wchar_t(3) ;
		text += wchar_t(6) ;
		text += wchar_t(9) ;
		text += wchar_t(12) ;
		converter.convert_cdata_node(start_tag, end_tag, text) ;

		CStringA actual = output_device.m_value.c_str() ;
		CStringA expected = "<spam><![CDATA[&#1;&#3;&#6;&#9;&#12;]]></spam>" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( rec2xml_convert_text_node_spam)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;

		string start_tag = "<spam>" ;
		string end_tag = "</spam>" ;
		string text = "spam" ;
		converter.convert_text_node(start_tag, end_tag, text) ;

		CStringA actual = output_device.m_value.c_str() ;
		CStringA expected = "<spam>spam</spam>" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
