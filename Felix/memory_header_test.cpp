#include "StdAfx.h"
#include "memory_header.h"
#include "output_device_fake.h"
#include "Felix_properties.h"
#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	// defaults

	TEST( memory_header_test_case, get_header_default_modified_by_RyanVista)
	{
		app_props::properties_general props; 
		props.read_from_registry() ;

		memory_engine::memory_header header ;
		ASSERT_EQUALS(header.get_modified_by(), props.m_data.m_user_name) ;
	}
	TEST( memory_header_test_case, get_header_default_modified_by_equals_creator)
	{
		memory_engine::memory_header header ;
		ASSERT_EQUALS(header.get_creator(), header.get_modified_by()) ;
	}
	TEST( memory_header_test_case, get_header_default_modified_on_equals_created_on)
	{
		memory_engine::memory_header header ;
		ASSERT_EQUALS(header.get_created_on(), header.get_modified_on()) ;
	}

	// getters and setters

	TEST( memory_header_test_case, get_header_getset_modified_by)
	{
		memory_engine::memory_header header ;
		header.set_modified_by(L"spam") ;
		ASSERT_EQUALS(header.get_modified_by(), L"spam") ;
	}
	TEST( memory_header_test_case, get_header_getset_modified_on)
	{
		memory_engine::memory_header header ;
		header.set_modified_on(L"spam") ;
		ASSERT_EQUALS(header.get_modified_on(), L"spam") ;
	}

	// parsing

	TEST( memory_header_test_case, get_header_field_empty)
	{
		memory_engine::memory_header header ;

		wc_reader reader ;
		LPCWSTR text = L"" ;
		reader.set_buffer(text) ;
		CStringA field = header.get_header_field(reader, L"spam").c_str() ;
		ASSERT_EQUALS_V(field, "") ;
	}
	TEST( memory_header_test_case, get_header_field_broken)
	{
		memory_engine::memory_header header ;

		wc_reader reader ;
		LPCWSTR text = L"<spam>egg</sp<one>two</one>" ;
		reader.set_buffer(text) ;
		CStringA field = header.get_header_field(reader, L"spam").c_str() ;
		ASSERT_EQUALS_V(field, "") ;
	}
	TEST( memory_header_test_case, get_header_field_spam_egg)
	{
		memory_engine::memory_header header ;

		wc_reader reader ;
		LPCWSTR text = L"<spam>egg</spam>" ;
		reader.set_buffer(text) ;
		CStringA field = header.get_header_field(reader, L"spam").c_str() ;
		ASSERT_EQUALS_V("egg", field) ;
	}
	TEST( memory_header_test_case, read_header)
	{
		memory_engine::memory_header header ;

		wc_reader reader ;
		LPCWSTR header_text = L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE memory >\n<!-- Created by Felix v 1.0 -->\n<memory>\n<head>\n<creator>RyanVista</creator>\n<modified_by>RyanVista</modified_by>\n<created_on>2007/08/23 13:52:38</created_on>\n<modified_on>2007/08/23 13:52:38</modified_on>\n<creation_tool>Felix</creation_tool>\n<creation_tool_version>1.0</creation_tool_version>\n<num_records>6</num_records>\n<locked>false</locked>\n<is_memory>false</is_memory>\n</head>\n<records><record>" ;
		/*
			<head>
			<creator>RyanVista</creator>
			<modified_by>RyanVista</modified_by>
			<created_on>2007/08/23 13:52:38</created_on>
			<modified_on>2007/08/23 13:52:38</modified_on>
			<creation_tool>Felix</creation_tool>
			<creation_tool_version>1.0</creation_tool_version>
			<num_records>6</num_records>
			<locked>false</locked>
			<is_memory>false</is_memory>
			</head>
		*/
		app_props::properties_general props; 
		props.read_from_registry() ;

		header.read_header(header_text) ;
		ASSERT_EQUALS(header.get_creator(), L"RyanVista") ;
		ASSERT_EQUALS(header.get_created_on(), L"2007/08/23 13:52:38") ;
		ASSERT_EQUALS(header.get_creation_tool(), L"Felix") ;
		ASSERT_EQUALS(header.get_creation_tool_version(), L"1.0") ;
		ASSERT_EQUALS(header.get_count(), 6) ;
		ASSERT_EQUALS(header.is_locked(), false) ;
		ASSERT_EQUALS(header.is_memory(), false) ;
		// this one is the default value
		ASSERT_EQUALS(SimpleString("RyanVista"), (LPCSTR)(CStringA(header.get_modified_by().c_str()))) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Test writing
	//////////////////////////////////////////////////////////////////////////

	TEST( memory_header_test_case, write_header_creator)
	{
		memory_engine::memory_header header ;
		header.set_creator(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<creator>spam</creator>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_creator_empty)
	{
		memory_engine::memory_header header ;
		header.set_creator(L"") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<creator>") == wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_modified_by_default)
	{
		memory_engine::memory_header header ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<modified_by>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_modified_on)
	{
		memory_engine::memory_header header ;
		header.set_modified_on(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<modified_on>spam</modified_on>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_field)
	{
		memory_engine::memory_header header ;
		header.set_field(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<field>spam</field>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_created_on)
	{
		memory_engine::memory_header header ;
		header.set_created_on(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<created_on>spam</created_on>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_source_language)
	{
		memory_engine::memory_header header ;
		header.set_source_language(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<source_language>spam</source_language>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_target_language)
	{
		memory_engine::memory_header header ;
		header.set_target_language(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<target_language>spam</target_language>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_client)
	{
		memory_engine::memory_header header ;
		header.set_client(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<client>spam</client>") != wstring::npos) ;
	}

	TEST( memory_header_test_case, write_header_creation_tool)
	{
		memory_engine::memory_header header ;
		header.set_creation_tool(L"spam egg") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<creation_tool>spam egg</creation_tool>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_creation_tool_version)
	{
		memory_engine::memory_header header ;
		header.set_creation_tool_version(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<creation_tool_version>spam</creation_tool_version>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_num_records)
	{
		memory_engine::memory_header header ;
		header.set_count(10) ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<num_records>10</num_records>") != wstring::npos) ;
	}

	TEST( memory_header_test_case, write_header_locked_on)
	{
		memory_engine::memory_header header ;
		header.set_locked_on() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<locked>true</locked>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_locked_off)
	{
		memory_engine::memory_header header ;
		header.set_locked_off() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<locked>false</locked>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_is_memory_on)
	{
		memory_engine::memory_header header ;
		header.set_is_memory_on() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<is_memory>true</is_memory>") != wstring::npos) ;
	}
	TEST( memory_header_test_case, write_header_is_memory_off)
	{
		memory_engine::memory_header header ;
		header.set_is_memory_off() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		ASSERT_TRUE(output_device.m_value.find(L"<is_memory>false</is_memory>") != wstring::npos) ;
	}

}

#endif // #ifdef _DEBUG
