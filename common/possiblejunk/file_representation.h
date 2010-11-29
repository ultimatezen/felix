// file_representation.h: interface for the file_representation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE_REPRESENTATION_H__0699B6EE_94DD_4E81_AC93_91E4B2C94DDF__INCLUDED_)
#define AFX_FILE_REPRESENTATION_H__0699B6EE_94DD_4E81_AC93_91E4B2C94DDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "class_representation.h"
#include <list>

typedef std::list< class_representation > class_list ;
typedef std::list< string > include_list ;
typedef std::list< string > define_list ;

class file_representation  
{
	class_list	m_classes ;
	method_list	m_methods ;
	variable_list m_variables ;
	include_list m_includes ;
	define_list	m_defines ;

public:
	void suss_file( prog_lines &lines );
	file_representation( const file_representation &rhs );
	file_representation();
	virtual ~file_representation();
	file_representation& operator =(const file_representation &rhs) ;

private:
	void internal_copy( const file_representation &rhs );
};

#endif // !defined(AFX_FILE_REPRESENTATION_H__0699B6EE_94DD_4E81_AC93_91E4B2C94DDF__INCLUDED_)
