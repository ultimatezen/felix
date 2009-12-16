/*!
Unit tests for MemoryInfo implemented by memory_remote

Fields:
	modified_by
	name
	creator
	notes
	source_language
	normalize_case
	normalize_hira
	created_on
	client
	normalize_width
	modified_on
	memtype
	size
	id
	target_language
*/

#include "StdAfx.h"
#include "memory_remote.h"
#include "Path.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
	TEST( test_memory_remote_info, target_language)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			MemoryInfo *mem_info = mem.get_memory_info() ;
			SimpleString actual = string2string(mem_info->get_target_language()).c_str() ;
			SimpleString expected = "Japanese" ;

			ASSERT_EQUALS_V(expected, actual) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}
	TEST( test_memory_remote_info, modified_by)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			MemoryInfo *mem_info = mem.get_memory_info() ;
			SimpleString actual = string2string(mem_info->get_modified_by()).c_str() ;
			SimpleString expected = "Taro Mod" ;

			ASSERT_EQUALS_V(expected, actual) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}
	TEST( test_memory_remote_info, created_on)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			MemoryInfo *mem_info = mem.get_memory_info() ;
			SimpleString actual = string2string(mem_info->get_created_on()).c_str() ;
			SimpleString expected = "1999/11/12 11:22:33" ;

			ASSERT_EQUALS_V(expected, actual) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}

}


#endif // #ifdef UNIT_TEST
