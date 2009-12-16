/*!
 * \file TMXWriter
 *
 * \brief
 * Interface file for CTMXWriter class.
 *
 * \date 2007/01/31
 * \version 1.0
 * \author Ryan Ginstrom
 *
 * Detailed description here.
 *
 * Copyright (C) Ryan Ginstrom.
 */

#pragma once

#include "atlstr.h"
#include "TranslationMemory.h"

#include "ProgressListener.h"
#include "textstream_reader.h"
#include "File.h"
#include "StringEx.h"
#include "ProgressListener.h"

#include "TextTemplate.h"

/**
	@class CTMXWriter
	@brief Export a memory into a TMX file.
 */
class CTMXWriter
{
VISIBLE_TO_TESTS

	
	file::file						m_file ;
	mem_engine::memory_pointer	m_memory ;


	wstring			m_src_lang ;
	wstring			m_target_lang ;

	int					m_iLevel ;
	CProgressListener	*m_listener ;

	text_tmpl::CTextTemplate textTemplate ;
	wstring tmplText ;

	// junk included by SDLX
	int		m_tuid ;
	wstring	m_source_file_name ;
	wstring	m_user_name ;

public:
	CTMXWriter(CProgressListener *listener);
	~CTMXWriter(void);

	void write_memory(mem_engine::memory_pointer mem);

	void cleanup();
	void prepareToWrite();
	void set_src_lang( const wstring &lang ) ;
	void set_target_lang( const wstring &lang ) ;

	// So we can unit test...

	void write_header( void );
	void write_footer( void );

	void write_tus( void );
	void write_tu( mem_engine::record_pointer rec );

	wstring make_tu( mem_engine::record_pointer rec, const wstring tmpl_text );
	wstring get_segment( const wstring & text);

	wstring get_tag( wc_reader & reader) ;

	std::wstring cleanUpForTMX( std::wstring& text_chunk ) ;
	wstring tmx_date( const misc_wrappers::date &ta_date ) ;
};
