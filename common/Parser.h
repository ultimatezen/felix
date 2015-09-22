// Parser.h: interface for the Parser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__C71F4335_EB95_44CC_ADCC_DAE03E2157AF__INCLUDED_)
#define AFX_PARSER_H__C71F4335_EB95_44CC_ADCC_DAE03E2157AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Parser  
{

protected:
	BOOL	m_parse_font_face ;
	BOOL	m_parse_font_color ;
	BOOL	m_parse_font_bold ;
	BOOL	m_parse_font_italic ;
	BOOL	m_parse_font_underline ;
	BOOL	m_parse_font_super_sub ;

public:
	Parser();
	virtual ~Parser();

	void set_parse_font_face( BOOL setting ) ;
	void set_parse_font_color( BOOL setting ) ;
	void set_parse_font_bold( BOOL setting ) ;
	void set_parse_font_italic( BOOL setting ) ;
	void set_parse_font_underline( BOOL setting ) ;
	void set_parse_font_super_sub( BOOL setting ) ;

};

#endif // !defined(AFX_PARSER_H__C71F4335_EB95_44CC_ADCC_DAE03E2157AF__INCLUDED_)
