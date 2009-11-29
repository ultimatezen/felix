/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "AppStringEntry.h"
#include "textstream_reader.h"
#include "AppStringTextNodeParser.h"

/*!
 * \brief
 * Parses the string file (xml)
 */
class CAppStringRecordParser
{
	typedef textstream_reader< wchar_t > reader_type ;
public:
	CAppStringRecordParser( LPCWSTR file_text );
	bool had_more_records(void);
	app_str_entry_ptr get_next_record(void);

private:

	reader_type m_reader;

};
