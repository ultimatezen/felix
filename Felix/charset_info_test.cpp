#include "StdAfx.h"
#include "charset_info.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	// cp_from_lang_str
	TEST( test_charset_info, cp_from_lang_str_af )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_lang_str("AF"), 1252) ;
	}
	TEST( test_charset_info, cp_from_lang_str_ZH_TW )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_lang_str("ZH-TW"), 950) ;
	}
	TEST( test_charset_info, cp_from_lang_str_ZH_CN )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_lang_str("ZH-CN"), 936) ;
	}
	TEST( test_charset_info, cp_from_lang_str_ZH )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_lang_str("ZH"), 950) ;
	}


	TEST( test_charset_info, cp_from_lang_str_IN )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_lang_str("IN"), 1252) ;
	}
	TEST( test_charset_info, cp_from_lang_str_Gaelic )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_lang_str("Gaelic"), 1252) ;
	}
	TEST( test_charset_info, cp_from_lang_str_JA )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_lang_str("JA"), 932) ;
	}

	// cp_from_charset
	TEST( test_charset_info, cp_from_charset_ANSI_CHARSET )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_charset(ANSI_CHARSET), (int)CP_ACP) ;
	}

	TEST( test_charset_info, cp_from_charset_DEFAULT_CHARSET )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_charset(DEFAULT_CHARSET), (int)CP_ACP) ;
	}
	TEST( test_charset_info, cp_from_charset_SYMBOL_CHARSET )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_charset(SYMBOL_CHARSET), (int)CP_SYMBOL) ;
	}
	TEST( test_charset_info, cp_from_charset_SHIFTJIS_CHARSET )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_charset(SHIFTJIS_CHARSET), (int)932) ;
	}
	TEST( test_charset_info, cp_from_charset_HANGEUL_CHARSET )
	{
		charset_info cinfo ;
		ASSERT_EQUALS_V((int)cinfo.cp_from_charset(HANGEUL_CHARSET), (int)949) ;
	}

}


#endif // #ifdef _DEBUG