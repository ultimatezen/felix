/*!
	@file File_test.cpp
	@brief Unit tests for file namespace
	@date 2005/06/02
	Time: 11:20:14
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "File.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST


BOOST_AUTO_TEST_CASE( fileTestCase_bom_size )
{
	BOOST_CHECK_EQUAL ( 2u, file::file::bom_size( file::BOM_LE ) ) ;
	BOOST_CHECK_EQUAL ( 2u, file::file::bom_size( file::BOM_BE ) ) ;
	BOOST_CHECK_EQUAL ( 3u, file::file::bom_size( file::BOM_UTF8 ) ) ;
	BOOST_CHECK_EQUAL ( 5u, file::file::bom_size( file::BOM_UTF7 ) ) ;
}


//////////////////////////////////////////////////////////////////////////
// CFileExtension
//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( CFileExtensionTestCase )

using namespace except;
#define TMP_FILE _T("C:\\dev\\TestFile.txt")


	BOOST_AUTO_TEST_CASE( construction )
	{
		file::CFileExtension ext(_T("Hello.txt")) ;
		string expected = ".txt" ;
		string actual = (LPCSTR)CStringA(ext.get()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( assignment )
	{
		file::CFileExtension ext(_T("Hello.txt")) ;
		ext = _T("foo.xls") ;
		string expected = ".xls" ;
		string actual = (LPCSTR)CStringA(ext.get()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	// equals
	BOOST_AUTO_TEST_CASE( equals_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.equals( _T(".doc"))) ;
	}
	BOOST_AUTO_TEST_CASE( equals_true )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(ext.equals( _T(".txt"))) ;
	}
	BOOST_AUTO_TEST_CASE( equals_empty_true )
	{
		file::CFileExtension ext = _T("Hello") ;
		BOOST_CHECK(ext.equals( _T(""))) ;
	}
	BOOST_AUTO_TEST_CASE( equals_empty_false )
	{
		file::CFileExtension ext = _T("Hello.rtf") ;
		BOOST_CHECK(!ext.equals( _T(""))) ;
	}
	BOOST_AUTO_TEST_CASE( equals_nodot)
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(ext.equals( _T("txt"))) ;
	}
	// html
	BOOST_AUTO_TEST_CASE( is_html_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_html()) ;
	}
	BOOST_AUTO_TEST_CASE( is_html_true )
	{
		file::CFileExtension ext = _T("Hello.html") ;
		BOOST_CHECK(ext.is_html()) ;
	}
	BOOST_AUTO_TEST_CASE( is_htm_true )
	{
		file::CFileExtension ext = _T("Hello.htm") ;
		BOOST_CHECK(ext.is_html()) ;
	}

	// xml
	BOOST_AUTO_TEST_CASE( is_xml_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_xml()) ;
	}
	BOOST_AUTO_TEST_CASE( is_xml_true )
	{
		file::CFileExtension ext = _T("Hello.xml") ;
		BOOST_CHECK(ext.is_xml()) ;
	}

	// txt
	BOOST_AUTO_TEST_CASE( is_txt_false )
	{
		file::CFileExtension ext = _T("Hello.doc") ;
		BOOST_CHECK(! ext.is_txt()) ;
	}
	BOOST_AUTO_TEST_CASE( is_txt_true )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(ext.is_txt()) ;
	}

	// doc
	BOOST_AUTO_TEST_CASE( is_doc_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_doc()) ;
	}
	BOOST_AUTO_TEST_CASE( is_doc_true )
	{
		file::CFileExtension ext = _T("Hello.doc") ;
		BOOST_CHECK(ext.is_doc()) ;
	}

	// bat
	BOOST_AUTO_TEST_CASE( is_bat_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_bat()) ;
	}
	BOOST_AUTO_TEST_CASE( is_bat_true )
	{
		file::CFileExtension ext = _T("Hello.bat") ;
		BOOST_CHECK(ext.is_bat()) ;
	}

	// css
	BOOST_AUTO_TEST_CASE( is_css_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_css()) ;
	}
	BOOST_AUTO_TEST_CASE( is_css_true )
	{
		file::CFileExtension ext = _T("Hello.css") ;
		BOOST_CHECK(ext.is_css()) ;
	}

	// cpp
	BOOST_AUTO_TEST_CASE( is_cpp_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_cpp()) ;
	}
	BOOST_AUTO_TEST_CASE( is_cpp_true )
	{
		file::CFileExtension ext = _T("Hello.cpp") ;
		BOOST_CHECK(ext.is_cpp()) ;
	}
	BOOST_AUTO_TEST_CASE( is_cpp_h_true )
	{
		file::CFileExtension ext = _T("Hello.h") ;
		BOOST_CHECK(ext.is_cpp()) ;
	}

	// py
	BOOST_AUTO_TEST_CASE( is_py_false )
	{
		file::CFileExtension ext = _T("Hello.cpp") ;
		BOOST_CHECK(! ext.is_python()) ;
	}
	BOOST_AUTO_TEST_CASE( is_py_true )
	{
		file::CFileExtension ext = _T("Hello.py") ;
		BOOST_CHECK(ext.is_python()) ;
	}
	BOOST_AUTO_TEST_CASE( is_py_pyw_true )
	{
		file::CFileExtension ext = _T("Hello.pyw") ;
		BOOST_CHECK(ext.is_python()) ;
	}

	// pl
	BOOST_AUTO_TEST_CASE( is_perl_false )
	{
		file::CFileExtension ext = _T("Hello.cpp") ;
		BOOST_CHECK(! ext.is_perl()) ;
	}
	BOOST_AUTO_TEST_CASE( is_perl_true )
	{
		file::CFileExtension ext = _T("Hello.pl") ;
		BOOST_CHECK(ext.is_perl()) ;
	}
	BOOST_AUTO_TEST_CASE( is_perl_cgi_true )
	{
		file::CFileExtension ext = _T("Hello.cgi") ;
		BOOST_CHECK(ext.is_perl()) ;
	}

	// php
	BOOST_AUTO_TEST_CASE( is_php_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_php()) ;
	}
	BOOST_AUTO_TEST_CASE( is_php_true )
	{
		file::CFileExtension ext = _T("Hello.php") ;
		BOOST_CHECK(ext.is_php()) ;
	}
	BOOST_AUTO_TEST_CASE( is_php_h_true )
	{
		file::CFileExtension ext = _T("Hello.php3") ;
		BOOST_CHECK(ext.is_php()) ;
	}

	// js
	BOOST_AUTO_TEST_CASE( is_js_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_js()) ;
	}
	BOOST_AUTO_TEST_CASE( is_js_true )
	{
		file::CFileExtension ext = _T("Hello.js") ;
		BOOST_CHECK(ext.is_js()) ;
	}

	// vbs
	BOOST_AUTO_TEST_CASE( is_vbs_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_vbs()) ;
	}
	BOOST_AUTO_TEST_CASE( is_vbs_true )
	{
		file::CFileExtension ext = _T("Hello.vbs") ;
		BOOST_CHECK(ext.is_vbs()) ;
	}

	// rtf
	BOOST_AUTO_TEST_CASE( is_rtf_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_rtf()) ;
	}
	BOOST_AUTO_TEST_CASE( is_rtf_true )
	{
		file::CFileExtension ext = _T("Hello.rtf") ;
		BOOST_CHECK(ext.is_rtf()) ;
	}


	// script
	BOOST_AUTO_TEST_CASE( is_script_false )
	{
		file::CFileExtension ext = _T("Hello.txt") ;
		BOOST_CHECK(! ext.is_script()) ;
	}
	BOOST_AUTO_TEST_CASE( is_script_vbs_true )
	{
		file::CFileExtension ext = _T("Hello.vbs") ;
		BOOST_CHECK(ext.is_script()) ;
	}
	BOOST_AUTO_TEST_CASE( is_script_js_true )
	{
		file::CFileExtension ext = _T("Hello.js") ;
		BOOST_CHECK(ext.is_script()) ;
	}
	BOOST_AUTO_TEST_CASE( is_script_py_true )
	{
		file::CFileExtension ext = _T("Hello.py") ;
		BOOST_CHECK(ext.is_script()) ;
	}
	BOOST_AUTO_TEST_CASE( is_script_au_true )
	{
		file::CFileExtension ext = _T("Hello.au3") ;
		BOOST_CHECK(ext.is_script()) ;
	}
	BOOST_AUTO_TEST_CASE( is_script_cgi_true )
	{
		file::CFileExtension ext = _T("Hello.cgi") ;
		BOOST_CHECK(ext.is_script()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// name
//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( TestName )
	BOOST_AUTO_TEST_CASE(file_name_part)
	{
		file::name filename = _T("foo.txt") ;

		string expected = "foo" ;
		string actual = (LPCSTR)CStringA(filename.file_name_part()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(extension_part)
	{
		file::name filename = _T("foo.txt") ;

		string expected = ".txt" ;
		string actual = (LPCSTR)CStringA(filename.extension_part()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(extension_empty_false)
	{
		file::name filename = _T("foo.txt") ;
		BOOST_CHECK(filename.has_extension()) ;
	}
	BOOST_AUTO_TEST_CASE(extension_empty_true)
	{
		file::name filename = _T("foo") ;
		BOOST_CHECK(!filename.has_extension()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// OpenDlgList
BOOST_AUTO_TEST_SUITE(TestBytesToBom)
	BOOST_AUTO_TEST_CASE(bytes_to_bom_unknown)
	{
		BYTE bytes[5] = {0, 0, 0, 0, 0} ;
		BOOST_CHECK_EQUAL(file::bytes_to_bom(bytes), file::BOM_UNKNOWN) ;
	}
	BOOST_AUTO_TEST_CASE(bytes_to_bom_be)
	{
		BYTE bytes[5] = {0xFE, 0xFF, 0, 0, 0} ;
		BOOST_CHECK_EQUAL(file::bytes_to_bom(bytes), file::BOM_BE) ;
	}
	BOOST_AUTO_TEST_CASE(bytes_to_bom_le)
	{
		BYTE bytes[5] = {0xFF, 0xFE, 0, 0, 0} ;
		BOOST_CHECK_EQUAL(file::bytes_to_bom(bytes), file::BOM_LE) ;
	}
	BOOST_AUTO_TEST_CASE(bytes_to_bom_utf8)
	{
		BYTE bytes[5] = {0xEF, 0xBB, 0xBF, 0, 0} ;
		BOOST_CHECK_EQUAL(file::bytes_to_bom(bytes), file::BOM_UTF8) ;
	}
	BOOST_AUTO_TEST_CASE(bytes_to_bom_utf8_alternate)
	{
		BYTE bytes[5] = {0x3F, 0xBB, 0xBF, 0, 0} ;
		BOOST_CHECK_EQUAL(file::bytes_to_bom(bytes), file::BOM_UTF8) ;
	}
	BOOST_AUTO_TEST_CASE(bytes_to_bom_utf7)
	{
		BYTE bytes[5] = {0x2B, 0x2F, 0x76, 0x38, 0x2D} ;
		BOOST_CHECK_EQUAL(file::bytes_to_bom(bytes), file::BOM_UTF7) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( file_name_name_part_tests )
	BOOST_AUTO_TEST_CASE( simple)
	{
		file::name fname = _T("foo.txt") ;
		CStringA actual = fname.file_name_part() ;
		string expected = "foo" ;
		BOOST_CHECK_EQUAL(expected, string((LPCSTR)actual)) ;

	}
	BOOST_AUTO_TEST_CASE( no_dot)
	{
		file::name fname = _T("foo") ;
		CStringA actual = fname.file_name_part() ;
		string expected = "foo" ;
		BOOST_CHECK_EQUAL(expected, string((LPCSTR)actual)) ;

	}
BOOST_AUTO_TEST_SUITE_END()
#endif 

