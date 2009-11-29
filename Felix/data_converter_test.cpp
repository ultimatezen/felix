#include "StdAfx.h"
#include "data_converter.h"

#include "easyunit/testharness.h"
#include "output_device_fake.h"

#ifdef _DEBUG

namespace easyunit
{
	// cp_from_lang_str
	TEST( TestCXml2RecordConverter, test_convert_from_node )
	{
		LPCWSTR text = L">"
			L"<source><![CDATA[spam]]></source>"
			L"<trans><![CDATA[egg]]></trans>"
			L"<id>10</id>"
			L"</record>" ;

		CXml2RecordConverter converter ;
		memory_engine::record_pointer record = converter.convert_from_xml_node(text) ;
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
		memory_engine::record_pointer record = converter.convert_from_xml_node(text) ;
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
		memory_engine::record_pointer record = converter.convert_from_xml_node(text) ;
		ASSERT_EQUALS(record->get_modified_by(), L"modified by xyz") ;
	}

	// CRecord2XmlConverter
	TEST(TestCRecord2XmlConverter, convert_from_record)
	{
		OutputDeviceFake output_device ;
		CRecord2XmlConverter converter(&output_device) ;

		memory_engine::record_pointer rec(new memory_engine::record_local) ;
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


#endif 