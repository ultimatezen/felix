#include "stdafx.h"
#include "element_wrapper.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_element_wrapper_html )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		BOOST_CHECK_NO_THROW(element_wrapper_html());
	}
	BOOST_AUTO_TEST_CASE( get_inner_text)
	{
		element_wrapper_html wrapper ;
		string actual = string2string(wrapper.get_inner_text()).c_str() ;
		string expected = "text" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_tag)
	{
		element_wrapper_html wrapper ;
		string actual = string2string(wrapper.get_tag()).c_str() ;
		string expected = "tag" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_id)
	{
		element_wrapper_html wrapper ;
		string actual = string2string(wrapper.get_id()).c_str() ;
		string expected = "id" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_attribute)
	{
		element_wrapper_html wrapper ;
		string actual = string2string(wrapper.get_attribute(L"key")).c_str() ;
		string expected = "attribute" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( set_attribute)
	{
		element_wrapper_html wrapper ;
		wrapper.set_attribute(L"key", L"val") ;
		BOOST_CHECK_EQUAL(3, (int)wrapper.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string("set_attribute"), wrapper.m_sensing_variable[0].c_str()) ;
		BOOST_CHECK_EQUAL(string("key"), wrapper.m_sensing_variable[1].c_str()) ;
		BOOST_CHECK_EQUAL(string("val"), wrapper.m_sensing_variable[2].c_str()) ;
	}
	BOOST_AUTO_TEST_CASE( set_inner_text)
	{
		element_wrapper_html wrapper ;
		wrapper.set_inner_text(L"text") ;
		BOOST_CHECK_EQUAL(2u, wrapper.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string("set_inner_text"), wrapper.m_sensing_variable[0].c_str()) ;
		BOOST_CHECK_EQUAL(string("text"), wrapper.m_sensing_variable[1].c_str()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif