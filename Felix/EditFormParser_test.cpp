#include "stdafx.h"
#include "EditFormParser.h"
#include "document_wrapper_fake.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( EditFormParserTests )

	BOOST_AUTO_TEST_CASE(Test_creator)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"creator")->set_inner_text(L"Bozo") ;
		
		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK_EQUAL(string2string(parser.creator()), "Bozo");
	}
	BOOST_AUTO_TEST_CASE(Test_field)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"field")->set_inner_text(L"Electronics") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK_EQUAL(string2string(parser.field()), "Electronics");
	}
	BOOST_AUTO_TEST_CASE(Test_created_on)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"created_on")->set_inner_text(L"2010/08/01 11:11:11") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK_EQUAL(string2string(parser.created_on()), "2010/08/01 11:11:11");
	}

	BOOST_AUTO_TEST_CASE(Test_source_language)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"source_language")->set_inner_text(L"Japanese") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK_EQUAL(string2string(parser.source_language()), "Japanese");
	}

	BOOST_AUTO_TEST_CASE(Test_target_language)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"target_language")->set_inner_text(L"English") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK_EQUAL(string2string(parser.target_language()), "English");
	}

	BOOST_AUTO_TEST_CASE(Test_client)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"client")->set_inner_text(L"Bozo") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK_EQUAL(string2string(parser.client()), "Bozo");
	}

	BOOST_AUTO_TEST_CASE(Test_is_memory_true)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"mem_type_tm")->set_attribute(L"checked", L"true") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK(parser.is_memory());
	}

	BOOST_AUTO_TEST_CASE(Test_is_memory_false)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"mem_type_tm")->set_attribute(L"checked", L"false") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK(!parser.is_memory());
	}

	BOOST_AUTO_TEST_CASE(Test_locked_true)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"locked")->set_attribute(L"checked", L"true") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK(parser.locked());
	}
	BOOST_AUTO_TEST_CASE(Test_locked_false)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"locked")->set_attribute(L"checked", L"false") ;

		mgrview::EditFormParser parser(doc) ;

		BOOST_CHECK(!parser.locked());
	}

BOOST_AUTO_TEST_SUITE_END()

#endif 
