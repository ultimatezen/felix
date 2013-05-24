// html_WRITER.h: interface for the html_WRITER class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTML_WRITER_H__D0970207_F98D_42FF_8AAB_530460990095__INCLUDED_)
#define AFX_HTML_WRITER_H__D0970207_F98D_42FF_8AAB_530460990095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "html_parser.h"

class html_writer : public writer_interface
{
public:

	html_parser m_parser ;
	
public:

	// construction/destruction
	html_writer() : m_parser(this)
	{
		init_state() ;
	}
	virtual ~html_writer(){}

	// =====================
	// pure virtual functions
	// =====================

	// =====================
	// base functions
	// =====================

	void write_html( wstring html_text ) ;

private:
	void init_state()
	{
		m_parser.init_state() ;
	}

};

#endif // !defined(AFX_HTML_WRITER_H__D0970207_F98D_42FF_8AAB_530460990095__INCLUDED_)
