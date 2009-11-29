/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "stringex.h"
#include "textstream_reader.h"

/*!
 * \brief
 * Write brief comment for CAppStringTextNodeParser here.
 * 
 * Write detailed description for CAppStringTextNodeParser here.
 * 
 * \remarks
 * Write remarks for CAppStringTextNodeParser here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class CAppStringTextNodeParser
{
	typedef textstream_reader< wchar_t > reader_type ;
	/*!
	 * \brief
	 * Write brief comment for m_reader here.
	 * 
	 * Write detailed description for m_reader here.
	 * 
	 * \remarks
	 * Write remarks for m_reader here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	reader_type m_reader ;

public:
	CAppStringTextNodeParser( LPCWSTR node_text );
	~CAppStringTextNodeParser(void) ;
	const wstring get_node_string(wstring tag_name) ;
private:
	wstring clean_up_string(const wstring& dirty_string) ;
};
