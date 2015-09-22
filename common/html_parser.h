#pragma once

#include "html_processing.h"

class writer_interface
{
public:
	virtual void write_text( const wstring &text ) = 0 ;
	virtual void apply_linebreak() = 0 ;
	virtual void apply_paragraph() = 0 ;

};

class html_parser : public html_processing 
{
public:

	textstream_reader< wchar_t > m_reader ;
	writer_interface *listener ;

	html_parser(writer_interface* writer) : listener(writer)
	{
		init_state() ;
	}

	void write_html(const wstring &html_text);

	void set_buffer(const wstring &text)
	{
		m_reader.set_buffer( text.c_str() ) ;
	}
	bool empty()
	{
		return m_reader.empty() ;
	}
	void parse_text()  ;
	void parse_tag() ;
	void pop_tag( const wstring &tag ) ;
	void parse_font( const wstring &attributes )  ;
	void parse_span( const wstring &attributes )  ;
	void parse_paragraph( const wstring &attributes )  ;
	bool parse_attribute( textstream_reader<wchar_t> &reader, wstring &key, wstring &val ) ;

};