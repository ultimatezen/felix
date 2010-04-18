#include "StdAfx.h"
#include "memory_header.h"
#include "output_device_fake.h"
#include "Felix_properties.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( memory_header_test_case )

	// defaults
	using boost::contains ;

	BOOST_AUTO_TEST_CASE( get_header_default_modified_by_RyanVista)
	{
		app_props::properties_general props; 
		props.read_from_registry() ;

		mem_engine::memory_header header ;
		BOOST_CHECK(header.get_modified_by() == props.m_data.m_user_name) ;
	}
	BOOST_AUTO_TEST_CASE( get_header_default_modified_by_equals_creator)
	{
		mem_engine::memory_header header ;
		BOOST_CHECK(header.get_creator() == header.get_modified_by()) ;
	}
	BOOST_AUTO_TEST_CASE( get_header_default_modified_on_equals_created_on)
	{
		mem_engine::memory_header header ;
		BOOST_CHECK(header.get_created_on() == header.get_modified_on()) ;
	}

	// getters and setters

	BOOST_AUTO_TEST_CASE( get_header_getset_modified_by)
	{
		mem_engine::memory_header header ;
		header.set_modified_by(L"spam") ;
		BOOST_CHECK(header.get_modified_by() == L"spam") ;
	}
	BOOST_AUTO_TEST_CASE( get_header_getset_modified_on)
	{
		mem_engine::memory_header header ;
		header.set_modified_on(L"spam") ;
		BOOST_CHECK(header.get_modified_on() == L"spam") ;
	}

	// parsing

	BOOST_AUTO_TEST_CASE( get_header_field_empty)
	{
		mem_engine::memory_header header ;

		wc_reader reader ;
		LPCWSTR text = L"" ;
		reader.set_buffer(text) ;
		CStringA field = header.get_header_field(reader, L"spam").c_str() ;
		BOOST_CHECK_EQUAL(field, "") ;
	}
	BOOST_AUTO_TEST_CASE( get_header_field_broken)
	{
		mem_engine::memory_header header ;

		wc_reader reader ;
		LPCWSTR text = L"<spam>egg</sp<one>two</one>" ;
		reader.set_buffer(text) ;
		CStringA field = header.get_header_field(reader, L"spam").c_str() ;
		BOOST_CHECK_EQUAL(field, "") ;
	}
	BOOST_AUTO_TEST_CASE( get_header_field_spam_egg)
	{
		mem_engine::memory_header header ;

		wc_reader reader ;
		LPCWSTR text = L"<spam>egg</spam>" ;
		reader.set_buffer(text) ;
		CStringA field = header.get_header_field(reader, L"spam").c_str() ;
		BOOST_CHECK_EQUAL("egg", field) ;
	}
	BOOST_AUTO_TEST_CASE( read_header)
	{
		mem_engine::memory_header header ;

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
		BOOST_CHECK_EQUAL(string(string2string(header.get_creator()).c_str()), "RyanVista") ;
		BOOST_CHECK_EQUAL(string(string2string(header.get_created_on()).c_str()), "2007/08/23 13:52:38") ;
		BOOST_CHECK_EQUAL(string(string2string(header.get_creation_tool()).c_str()), "Felix") ;
		BOOST_CHECK_EQUAL(string(string2string(header.get_creation_tool_version()).c_str()), "1.0") ;
		BOOST_CHECK_EQUAL(header.get_count(), 6) ;
		BOOST_CHECK_EQUAL(header.is_locked(), false) ;
		BOOST_CHECK_EQUAL(header.is_memory(), false) ;
		// this one is the default value
		BOOST_CHECK_EQUAL(string("RyanVista"), (LPCSTR)(CStringA(header.get_modified_by().c_str()))) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Test writing
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( write_header_creator)
	{
		mem_engine::memory_header header ;
		header.set_creator(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<creator>spam</creator>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_creator_empty)
	{
		mem_engine::memory_header header ;
		header.set_creator(L"") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(! boost::contains(output_device.m_value, L"<creator>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_modified_by_default)
	{
		mem_engine::memory_header header ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<modified_by>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_modified_on)
	{
		mem_engine::memory_header header ;
		header.set_modified_on(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<modified_on>spam</modified_on>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_field)
	{
		mem_engine::memory_header header ;
		header.set_field(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<field>spam</field>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_created_on)
	{
		mem_engine::memory_header header ;
		header.set_created_on(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<created_on>spam</created_on>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_source_language)
	{
		mem_engine::memory_header header ;
		header.set_source_language(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<source_language>spam</source_language>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_target_language)
	{
		mem_engine::memory_header header ;
		header.set_target_language(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<target_language>spam</target_language>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_client)
	{
		mem_engine::memory_header header ;
		header.set_client(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<client>spam</client>")) ;
	}

	BOOST_AUTO_TEST_CASE( write_header_creation_tool)
	{
		mem_engine::memory_header header ;
		header.set_creation_tool(L"spam egg") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<creation_tool>spam egg</creation_tool>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_creation_tool_version)
	{
		mem_engine::memory_header header ;
		header.set_creation_tool_version(L"spam") ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<creation_tool_version>spam</creation_tool_version>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_num_records)
	{
		mem_engine::memory_header header ;
		header.set_count(10) ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<num_records>10</num_records>")) ;
	}

	BOOST_AUTO_TEST_CASE( write_header_locked_on)
	{
		mem_engine::memory_header header ;
		header.set_locked_on() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<locked>true</locked>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_locked_off)
	{
		mem_engine::memory_header header ;
		header.set_locked_off() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<locked>false</locked>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_is_memory_on)
	{
		mem_engine::memory_header header ;
		header.set_is_memory_on() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<is_memory>true</is_memory>")) ;
	}
	BOOST_AUTO_TEST_CASE( write_header_is_memory_off)
	{
		mem_engine::memory_header header ;
		header.set_is_memory_off() ;

		OutputDeviceFake output_device ;
		header.write_header(&output_device) ;

		BOOST_CHECK(contains(output_device.m_value, L"<is_memory>false</is_memory>")) ;
	}

BOOST_AUTO_TEST_SUITE_END()
