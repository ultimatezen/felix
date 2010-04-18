#include "StdAfx.h"
#include "charset_info.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_charset_info )

	// cp_from_lang_str
	BOOST_AUTO_TEST_CASE( cp_from_lang_str_af )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_lang_str("AF"), 1252) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_lang_str_ZH_TW )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_lang_str("ZH-TW"), 950) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_lang_str_ZH_CN )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_lang_str("ZH-CN"), 936) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_lang_str_ZH )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_lang_str("ZH"), 950) ;
	}


	BOOST_AUTO_TEST_CASE( cp_from_lang_str_IN )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_lang_str("IN"), 1252) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_lang_str_Gaelic )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_lang_str("Gaelic"), 1252) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_lang_str_JA )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_lang_str("JA"), 932) ;
	}

	// cp_from_charset
	BOOST_AUTO_TEST_CASE( cp_from_charset_ANSI_CHARSET )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_charset(ANSI_CHARSET), (int)CP_ACP) ;
	}

	BOOST_AUTO_TEST_CASE( cp_from_charset_DEFAULT_CHARSET )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_charset(DEFAULT_CHARSET), (int)CP_ACP) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_charset_SYMBOL_CHARSET )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_charset(SYMBOL_CHARSET), (int)CP_SYMBOL) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_charset_SHIFTJIS_CHARSET )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_charset(SHIFTJIS_CHARSET), (int)932) ;
	}
	BOOST_AUTO_TEST_CASE( cp_from_charset_HANGEUL_CHARSET )
	{
		charset_info cinfo ;
		BOOST_CHECK_EQUAL((int)cinfo.cp_from_charset(HANGEUL_CHARSET), (int)949) ;
	}

BOOST_AUTO_TEST_SUITE_END()