#if !defined(AFX_trados_data_exporter_H__C0E64BCD_075F_4188_BC7C_22C5852C62C2__INCLUDED_)
#define AFX_trados_data_exporter_H__C0E64BCD_075F_4188_BC7C_22C5852C62C2__INCLUDED_

#pragma once

#include "File.h"
#include "TranslationMemory.h"
#include <map>						// map word color numbers to html-style RGB tags
#include <set>
#include <stack>					// remember the code pages
#include "rtf.h"					// rtf-style font table, font defs, etc.
#include "resource.h"				// message ids
#include "ArmadilloStuff.h"
#include "background_processor.h"	// AKA poor-man's multithreading

#include "ProgressListener.h"		// CProgressListener

#include "output_device.h"

typedef std::map< wstring, string > color_tag_map ;

/**
 * The TradosDataExporter class. 
 * Exports Felix memories as Trados(R) text-format memories. 
 */
class TradosDataExporter
{
	VISIBLE_TO_TESTS

	typedef std::set< wstring > font_set ;
	
	typedef mem_engine::record_pointer record_type ;
	std::stack< unsigned int >	m_codepages ;
	rtf::font_table		m_fonts ;
	color_tag_map	m_colors ;

	UINT			m_old_codepage ;

	const char* TRADOS_PREAMBLE ;
	const char* TRADOS_STYLE_SHEET ;

	string		m_source_tag ;
	string		m_target_tag ;
	string		m_source_lang ;
	string		m_target_lang ;
	output_device_ptr 	m_file ;

	CProgressListener		*m_listener ;

	textstream_reader< wchar_t >	m_reader ;

	mem_engine::memory_pointer		m_memory ;

public:
	int m_p_depth;

	void export_trados( mem_engine::memory_pointer mem ) ;
	bool does_char_roundtrip(const wchar_t *c);
	string create_unicode_escape(wchar_t c, char best_fit);
	void set_source( tstring source ) ;
	void set_target( tstring target ) ;
	bool open_destination( const CString &destination ) ;
	bool write_preamble() ;
	bool write_record( record_type record ) ;
	bool write_spillage( record_type record ) ;
	bool write_created( const misc_wrappers::date &date ) ;
	bool write_modified( const misc_wrappers::date &date ) ;
	string internal_date_to_trados_date( const misc_wrappers::date &date ) ;
	bool write_source( const wstring source ) ;
	bool write_rich_part( const wstring rich_part ) ;
	bool write_trans( const wstring trans ) ;
	const wstring replace_tags( const wstring text ) ;
	bool write_plain_text( const wstring text ) ;
	bool write_formatting_tag( const wstring wide_tag ) ;
	bool write_font( const wstring font_info ) ;
	bool write_font_face( const wstring value ) ;
	bool write_rich_text( const wstring rich_text ) ;
	bool write_span( const wstring span_info ) ;
	bool write_key_value_pair( wstring key, wstring value ) ;
	string get_color( const wstring value ) ;
	void write_string( const string &str ) ;
	TradosDataExporter( font_set &fonts, CProgressListener *listener );
	virtual ~TradosDataExporter();

};

/**
 * Base class of common functions for exporting 
 * memories in MultiTerm v. 5.5 format.
 */
class multiterm_data_exporter_55
{
	typedef mem_engine::record_pointer record_type ;

	string	m_source_lang ;
	string	m_target_lang ;
	UINT	m_source_cp ;
	UINT	m_trans_cp ;
	CProgressListener *m_listener ;
public:
	output_device_ptr  m_file ;
	multiterm_data_exporter_55(CProgressListener *listener) ;
	~multiterm_data_exporter_55() ;
	void export_gloss(mem_engine::memory_pointer &mem);
	void set_source( const string source ) ;
	void set_target( const string target ) ;
	void open_destination( const CString destination ) ;
	void write_record( const record_type &rec ) ;

private:
	string write_line( const string &lang, const string &str ) ;
	string prep_string( const string &raw_string ) ;
} ;

/**
 *	The multiterm6_data_exporter class. Exports a memory in Trados(R) MultiTerm 6.0 format (Unicode).
 */
class multiterm_data_exporter_6
{
	typedef boost::shared_ptr<OutputDevice> output_device_ptr  ;
	wstring m_source_lang ;
	wstring m_target_lang ;
	
	CProgressListener *m_listener ;

public:
	output_device_ptr  m_file ;
	multiterm_data_exporter_6(CProgressListener *listener) :
		m_listener(listener)
	{}
	~multiterm_data_exporter_6()
	{ m_file->close() ; }
	void export_gloss(mem_engine::memory_pointer mem)
	{
		m_listener->OnProgressInit( mem->get_location(), 0, mem->size() ) ;
		wstring notes = L"Notes" ;
		write_line( m_source_lang, m_target_lang, notes ) ;

		int i = 0 ;
		foreach(mem_engine::record_pointer record, mem->get_records())
		{
			write_record(record) ;

			m_listener->OnProgressWriteUpdate( ++i ) ;
		}

		m_listener->OnProgressDoneWrite( i ) ;
	}
	void set_source( wstring source ) { m_source_lang = source ; }
	void set_target( wstring target ) { m_target_lang = target ; }
	void open_destination( const CString destination );
	void write_record( const mem_engine::record_pointer &rec );
private:
	void write_line( wstring col1, wstring col2, wstring col3 );
	wstring prep_string( wstring line );
} ;


#endif // !defined(AFX_trados_data_exporter_H__C0E64BCD_075F_4188_BC7C_22C5852C62C2__INCLUDED_)
