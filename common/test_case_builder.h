// test_case_builder.h: interface for the test_case_builder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST_CASE_BUILDER_H__813FBC05_2929_4D84_ADE6_4A87767CEF6F__INCLUDED_)
#define AFX_TEST_CASE_BUILDER_H__813FBC05_2929_4D84_ADE6_4A87767CEF6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "file.h"
#include "StringEx.h"
#include <list>
#include "class_representation.h"


class test_case_builder  
{
	CString		m_header_file_name ;
	CString		m_cpp_file_name ;
	CString		m_test_rig_file_name ;
	
	prog_lines	m_header_lines ;
	prog_lines	m_cpp_lines ;
	prog_lines	m_testrig_lines ;
	
public:
	void break_into_lines( const CString &header_file, prog_lines &lines );
	void parse_header_file();
	test_case_builder( const CString &header_file );
	virtual ~test_case_builder();
};

#endif // !defined(AFX_TEST_CASE_BUILDER_H__813FBC05_2929_4D84_ADE6_4A87767CEF6F__INCLUDED_)
