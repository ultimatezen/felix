#include "stdafx.h"
#include "element_wrapper.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( test_element_wrapper_html, instantiate)
	{
		element_wrapper_html wrapper ;
		ASSERT_TRUE_M(TRUE, "Should not blow up") ;
	}
	TEST( test_element_wrapper_html, get_inner_text)
	{
		element_wrapper_html wrapper ;
		SimpleString actual = string2string(wrapper.get_inner_text()).c_str() ;
		SimpleString expected = "text" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_element_wrapper_html, get_tag)
	{
		element_wrapper_html wrapper ;
		SimpleString actual = string2string(wrapper.get_tag()).c_str() ;
		SimpleString expected = "tag" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_element_wrapper_html, get_id)
	{
		element_wrapper_html wrapper ;
		SimpleString actual = string2string(wrapper.get_id()).c_str() ;
		SimpleString expected = "id" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_element_wrapper_html, get_attribute)
	{
		element_wrapper_html wrapper ;
		SimpleString actual = string2string(wrapper.get_attribute(L"key")).c_str() ;
		SimpleString expected = "attribute" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( test_element_wrapper_html, set_attribute)
	{
		element_wrapper_html wrapper ;
		wrapper.set_attribute(L"key", L"val") ;
		ASSERT_EQUALS_V(3, (int)wrapper.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString("set_attribute"), wrapper.m_sensing_variable[0].c_str()) ;
		ASSERT_EQUALS_V(SimpleString("key"), wrapper.m_sensing_variable[1].c_str()) ;
		ASSERT_EQUALS_V(SimpleString("val"), wrapper.m_sensing_variable[2].c_str()) ;
	}
	TEST( test_element_wrapper_html, set_inner_text)
	{
		element_wrapper_html wrapper ;
		wrapper.set_inner_text(L"text") ;
		ASSERT_EQUALS_V(2, (int)wrapper.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString("set_inner_text"), wrapper.m_sensing_variable[0].c_str()) ;
		ASSERT_EQUALS_V(SimpleString("text"), wrapper.m_sensing_variable[1].c_str()) ;
	}
}

#endif