// file_representation.cpp: implementation of the file_representation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "file_representation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

file_representation::file_representation()
{

}

file_representation::~file_representation()
{

}

file_representation::file_representation(const file_representation &rhs)
{
	internal_copy(rhs) ;
}

file_representation& file_representation::operator =(const file_representation &rhs)
{
	internal_copy(rhs) ;
	return *this ;
}

void file_representation::internal_copy(const file_representation &rhs)
{
	class_list	classes( rhs.m_classes ) ;
	m_classes.swap( classes ) ;

	method_list	methods( rhs.m_methods ) ;
	m_methods.swap( methods ) ;
	
	variable_list variables( rhs.m_variables ) ;
	m_variables.swap( variables ) ;
	
	include_list includes( rhs.m_includes ) ;
	m_includes.swap( includes ) ;
	
	define_list	defines( rhs.m_defines ) ;
	m_defines.swap( defines ) ;
}

void file_representation::suss_file(prog_lines &lines)
{
	prog_lines::iterator pos ;
	for ( pos = lines.begin() ; pos != lines.end() ; ++pos )
	{
		string line = *pos ;
		if ( line.substr(0,2) == "//" ) 
		{

		}
		else if ( line.substr(0,2) == "/*" ) 
		{
			while ( line.find( "*/" ) != string::npos ) 
			{
				++pos ;
				line = *pos ;
			}
		}
		else if ( line.substr(0,strlen("#include") ) == "#include" ) 
		{
			textstream_reader< char > reader ;
			reader.set_buffer(line.c_str()) ;
			reader.jump_to_first_of("\"<") ;
			string include_lib ;
			reader.getline(include_lib, "\">" ) ;
			m_includes.push_back(include_lib) ;
		}
		else if ( line.find( "class" ) != string::npos ) 
		{
			TRACE( line ) ;
			class_representation cls_rep ;
			prog_lines::iterator end_of_class_pos = cls_rep.suss_class( pos ) ;
			TRACE( *end_of_class_pos ) ;
		}
	}
}
