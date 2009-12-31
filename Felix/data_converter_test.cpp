#include "StdAfx.h"
#include "data_converter.h"
#include "record_local.h"

#include "easyunit/testharness.h"
#include "output_device_fake.h"

#ifdef UNIT_TEST

namespace easyunit
{
	//////////////////////////////////////////////////////////////////////////
	// TestCXml2RecordConverter
	//////////////////////////////////////////////////////////////////////////
	TEST( TestCXml2RecordConverter, load_source )
	{
		LPCWSTR text = L"<![CDATA[spam]]></source><trans><![CDATA[context]]></trans>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_source() ;
		mem_engine::record_pointer record = converter.m_record ;
		ASSERT_EQUALS(record->get_source_rich(), L"spam") ;
		ASSERT_EQUALS(record->get_trans_rich(), L"") ;
	}
	TEST( TestCXml2RecordConverter, load_trans )
	{
		LPCWSTR text = L"<![CDATA[spam]]></trans><context><![CDATA[context]]></context>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_trans() ;
		mem_engine::record_pointer record = converter.m_record ;
		ASSERT_EQUALS(record->get_source_rich(), L"") ;
		ASSERT_EQUALS(record->get_trans_rich(), L"spam") ;
		ASSERT_EQUALS(record->get_context_rich(), L"") ;
	}
	TEST( TestCXml2RecordConverter, load_context)
	{
		LPCWSTR text = L"<![CDATA[spam]]></context><trans><![CDATA[context]]></trans>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_context() ;
		mem_engine::record_pointer record = converter.m_record ;
		ASSERT_EQUALS(record->get_context_rich(), L"spam") ;
		ASSERT_EQUALS(record->get_source_rich(), L"") ;
		ASSERT_EQUALS(record->get_trans_rich(), L"") ;
	}
	TEST( TestCXml2RecordConverter, load_richtext_node)
	{
		LPCWSTR text = L"<![CDATA[spam]]></context><trans><![CDATA[context]]></trans>" ;
		_bstr_t end_tag = L"</context>" ;
		LPCWSTR start, end ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_richtext_node(end_tag, start, end) ;
		wstring context(start, end) ;
		ASSERT_EQUALS(context, L"spam") ;
	}
	TEST( TestCXml2RecordConverter, load_date_created)
	{
		LPCWSTR text = L"2009/12/05 21:06:10</date_created>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_date_created() ;
		mem_engine::record_pointer record = converter.m_record ;
		misc_wrappers::date created = record->get_created() ;
		ASSERT_EQUALS_V(2009, (int)created.wYear) ;
		ASSERT_EQUALS_V(12, (int)created.wMonth) ;
		ASSERT_EQUALS_V(5, (int)created.wDay) ;
		ASSERT_EQUALS_V(21, (int)created.wHour) ;
		ASSERT_EQUALS_V(6, (int)created.wMinute) ;
		ASSERT_EQUALS_V(10, (int)created.wSecond) ;
	}
	TEST( TestCXml2RecordConverter, load_extra_record_throws)
	{
		wstring tag = L"record" ;
		try
		{
			CXml2RecordConverter converter ;
			converter.load_extra(tag) ;
			FAIL_M("Should fail if it hits a 'record' tag") ;

		}
		catch (except::CException&)
		{
			ASSERT_TRUE(TRUE) ;
		}
	}
	TEST( TestCXml2RecordConverter, load_last_modified)
	{
		LPCWSTR text = L"2009/12/05 21:06:10</last_modified>" ;

		CXml2RecordConverter converter ;
		converter.m_parser.set_buffer(text) ;
		converter.load_last_modified() ;
		mem_engine::record_pointer record = converter.m_record ;
		misc_wrappers::date modified = record->get_modified() ;
		ASSERT_EQUALS_V(2009, (int)modified.wYear) ;
		ASSERT_EQUALS_V(12, (int)modified.wMonth) ;
		ASSERT_EQUALS_V(5, (int)modified.wDay) ;
		ASSERT_EQUALS_V(21, (int)modified.wHour) ;
		ASSERT_EQUALS_V(6, (int)modified.wMinute) ;
		ASSERT_EQUALS_V(10, (int)modified.wSecond) ;
	}
	TEST( TestCXml2RecordConverter, test_convert_from_node )
	{
		LPCWSTR text = L">"
			L"<source><![CDATA[spam]]></source>"
			L"<trans><![CDATA[egg]]></trans>"
			L"<id>10</id>"
			L"</record>" ;

		CXml2RecordConverter converter ;
		mem_engine::record_pointer record = converter.convert_from_xml_node(text) ;
		ASSERT_EQUALS(record->get_source_rich(), L"spam") ;
		ASSERT_EQUALS(record->get_trans_rich(), L"egg") ;
		ASSERT_EQUALS_V(10, (int)record->get_id()) ;
	}
	TEST( TestCXml2RecordConverter, test_convert_from_node_creator )
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
		ASSERT_EQUALS(record->get_creator(), L"creator abc") ;
	}
	TEST( TestCXml2RecordConverter, test_convert_from_node_modified_by )
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
		ASSERT_EQUALS(record->get_modified_by(), L"modified by xyz") ;
	}
	TEST(TestCXml2RecordConverter, PlainTextNode_plain)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"spam</foo>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		SimpleString actual = string2string(converter.PlainTextNode(end_tag)).c_str() ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCXml2RecordConverter, PlainTextNode_rich)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"<![CDATA[spam]]></foo>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		SimpleString actual = string2string(converter.PlainTextNode(end_tag)).c_str() ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCXml2RecordConverter, strip_cdata)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"<![CDATA[spam]]></foo><other>" ;
		SimpleString actual = string2string(converter.strip_cdata(text)).c_str() ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCXml2RecordConverter, SizeNode)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"1001</foo>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		int actual = (int)converter.SizeNode(end_tag) ;
		int expected = 1001 ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCXml2RecordConverter, SizeNode_rich)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"<![CDATA[54]]></foo><other>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		const _bstr_t end_tag = L"</foo>" ;
		int actual = (int)converter.SizeNode(end_tag) ;
		int expected = 54 ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCXml2RecordConverter, get_validated_value_true)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"true</validated>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		ASSERT_TRUE(converter.get_validated_value()) ;
	}
	TEST(TestCXml2RecordConverter, get_validated_value_false)
	{
		CXml2RecordConverter converter ;
		const wstring text = L"false</validated>" ;
		converter.m_parser.set_buffer(text.c_str()) ;
		ASSERT_TRUE(!converter.get_validated_value()) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// CRecord2XmlConverter
	//////////////////////////////////////////////////////////////////////////
	TEST(TestCRecord2XmlConverter, init_char_conversion)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;
		ASSERT_EQUALS_V(SimpleString("\1"), converter.m_from_strings[0].c_str()) ;
		ASSERT_EQUALS_V(SimpleString("&#1;"), converter.m_to_strings[0].c_str()) ;
	}
	TEST(TestCRecord2XmlConverter, convert_from_record)
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
		ASSERT_TRUE_M(actual.find("<id>10</id>") != string::npos, SimpleString(err)) ;
	}

	TEST(TestCRecord2XmlConverter, convert_cdata_node_spam)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;

		string start_tag = "<spam>" ;
		string end_tag = "</spam>" ;
		wstring text = L"spam" ;
		converter.convert_cdata_node(start_tag, end_tag, text) ;

		CStringA actual = output_device.m_value.c_str() ;
		CStringA expected = "<spam><![CDATA[spam]]></spam>" ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCRecord2XmlConverter, convert_cdata_node_control_codes)
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

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCRecord2XmlConverter, convert_text_node_spam)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;

		string start_tag = "<spam>" ;
		string end_tag = "</spam>" ;
		string text = "spam" ;
		converter.convert_text_node(start_tag, end_tag, text) ;

		CStringA actual = output_device.m_value.c_str() ;
		CStringA expected = "<spam>spam</spam>" ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

}


#endif // UNIT_TEST