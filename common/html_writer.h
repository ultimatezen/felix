// html_WRITER.h: interface for the html_WRITER class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTML_WRITER_H__D0970207_F98D_42FF_8AAB_530460990095__INCLUDED_)
#define AFX_HTML_WRITER_H__D0970207_F98D_42FF_8AAB_530460990095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "html_processing.h"
// #include "HtmlDocument.h"

class html_writer : public html_processing 
{
public:
	
private:
	textstream_reader< wchar_t > m_reader ;
	
public:

	// construction/destruction
	html_writer() 
	{
		init_state() ;
	}
	virtual ~html_writer(){}

	// =====================
	// pure virtual functions
	// =====================

	virtual void write_text( const wstring &text ) = 0 ;
	virtual void apply_linebreak() = 0 ;
	virtual void apply_paragraph() = 0 ;

	// =====================
	// base functions
	// =====================

	void write_html( wstring html_text ) ;


private:

	void parse_text()  ;
	void parse_tag() ;
	void pop_tag( const wstring &tag ) ;
	void parse_font( const wstring &attributes )  ;
	void parse_span( const wstring &attributes )  ;
	void parse_paragraph( const wstring &attributes )  ;
	bool parse_attribute( textstream_reader<wchar_t> &reader, wstring &key, wstring &val ) ;

};

#endif // !defined(AFX_HTML_WRITER_H__D0970207_F98D_42FF_8AAB_530460990095__INCLUDED_)
