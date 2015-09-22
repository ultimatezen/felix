// Parser.cpp: implementation of the Parser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Parser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Parser::Parser() :
	m_parse_font_face( TRUE ),
	m_parse_font_color( TRUE ),
	m_parse_font_bold( TRUE ),
	m_parse_font_italic( TRUE ),
	m_parse_font_underline( TRUE ),
	m_parse_font_super_sub( TRUE )
{

}

Parser::~Parser()
{

}

void Parser::set_parse_font_face( BOOL setting )
{
	m_parse_font_face = setting ; 
}
void Parser::set_parse_font_color( BOOL setting )
{
	m_parse_font_color = setting ; 
}
void Parser::set_parse_font_bold( BOOL setting )
{
	m_parse_font_bold = setting ; 
}
void Parser::set_parse_font_italic( BOOL setting )
{
	m_parse_font_italic = setting ; 
}
void Parser::set_parse_font_underline( BOOL setting )
{
	m_parse_font_underline = setting ; 
}
void Parser::set_parse_font_super_sub( BOOL setting )
{
	m_parse_font_super_sub = setting ;
}

