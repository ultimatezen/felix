// test_case_builder.cpp: implementation of the test_case_builder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "test_case_builder.h"
#include "file_representation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

test_case_builder::test_case_builder( const CString &header_file ) :
	m_header_file_name( header_file )
{
	CPath path( m_header_file_name ) ;
	ATLASSERT( path.FileExists() ) ;

	break_into_lines( m_header_file_name, m_header_lines ) ;

	file_representation file_rep ;
	file_rep.suss_file( m_header_lines ) ;
	

	path.RenameExtension( _T(".cpp") ) ;
	m_cpp_file_name = path.Path() ;

	if ( path.FileExists() ) 
	{
		break_into_lines( path.Path(), m_cpp_lines ) ;
	}

	path.RemoveExtension() ;
	m_test_rig_file_name = path.Path() ;
	m_test_rig_file_name += _T("_testrig.cpp") ;
	path = m_test_rig_file_name ;

	if ( path.FileExists() ) 
	{
		break_into_lines( path.Path(), m_testrig_lines ) ;
	}

}

test_case_builder::~test_case_builder()
{

}

void test_case_builder::parse_header_file()
{

}

void test_case_builder::break_into_lines(const CString &header_file, prog_lines &lines)
{
	file::view fview ;
	LPCSTR file_text = (LPCSTR)fview.create_view( header_file ) ;
	ATLASSERT( file_text != NULL ) ;

	textstream_reader< char > reader ;
	reader.set_buffer( file_text ) ;

	string line ;
	while ( reader.getline(line)) 
	{
		TRACE( line ) ;
		lines.push_back( line ) ;
	}
}
