/*!
	@brief Interface file for CTMXReader class.
	@author Ryan Ginstrom
 */

#pragma once

#include "atlstr.h"
#include "TranslationMemory.h"
#include "data_importer.h"

#include <map>

#include "ProgressListener.h"


namespace tmx_reader
{
	using namespace memory_engine ;
	struct CTU
	{
		// optional
		wstring m_tuid ;
		wstring m_usagecount ;
		wstring m_lastusagedate ;

		wstring m_creationtool ;
		wstring m_creationtoolversion ;
		wstring m_segtype ; // "block", "paragraph", "sentence", or "phrase".
		wstring m_o_tmf ; // original translation memory format
		wstring m_srclang ; // 
		wstring m_datatype ; 

		wstring m_o_encoding ; // original encoding
		wstring m_creationdate ;	// Date in [ISO 8601] Format. The recommended pattern to use is: YYYYMMDDThhmmssZ  

		wstring m_creationid ;
		wstring m_changedate ;
		wstring m_changeid ;

	public:
		void set_attributes( std::map< wstring, wstring > &attributes );
		void reflect_attributes(record_pointer record);

		void fill_date( misc_wrappers::date &thedate, const wstring &datestring );
	};
}


/**
	@class CTMXReader
	@brief Reads in TMX memory files and loads them into a native memory class.
 */
class CTMXReader
{
	str::wbuffer	m_buffer ;
	wc_reader		m_reader ;
	file::view		m_view ;
	memory_engine::memory_pointer m_memory ;
	memory_engine::record_pointer	m_record ;

	boost::shared_ptr< tmx_data_importer > m_rtf_importer ;

	CProgressListener	*m_listener ;

	wstring			m_target_lang ;


	struct CTMXHeader
	{
		// required
		wstring	m_creationtool ;
		wstring m_creationtoolversion ;
		wstring m_segtype ; // "block", "paragraph", "sentence", or "phrase".
		wstring m_o_tmf ; // original translation memory format
		wstring m_adminlang ; // not case sensitive
		wstring m_srclang ; // 
		wstring m_datatype ; // Text. The recommended values for the datatype attribute are as follow (this list is not exhaustive):
									//- "unknown" = undefined (default)
									//	- "alptext" = WinJoust data.
									//	- "cdf" = Channel Definition Format.
									//	- "cmx" = Corel CMX Format.
									//	- "cpp" = C and C++ style text.
									//	- "hptag" = HP-Tag.
									//	- "html" = HTML, DHTML, etc.
									//	- "interleaf" = Interleaf documents.
									//	- "ipf" = IPF/BookMaster.
									//	- "java" = Java, source and property files.
									//	- "javascript" = JavaScript, ECMAScript scripts.
									//	- "lisp" = Lisp.
									//	- "mif" = Framemaker MIF, MML, etc.
									//	- "opentag" = OpenTag data.
									//	- "pascal" = Pascal, Delphi style text.
									//	- "plaintext" = Plain text.
									//	- "pm" = PageMaker.
									//	- "rtf" = Rich Text Format.
									//	- "sgml" = SGML.
									//	- "stf-f" = S-Tagger for FrameMaker.
									//	- "stf-i" = S-Tagger for Interleaf.
									//	- "transit" = Transit data.
									//	- "vbscript" = Visual Basic scripts.
									//	- "winres" = Windows resources from RC, DLL, EXE.
									//	- "xml" = XML.
									//	- "xptag" = Quark XPressTag.


		// optional
		wstring m_o_encoding ; // original encoding
		wstring m_creationdate ;	// Date in [ISO 8601] Format. The recommended pattern to use is: YYYYMMDDThhmmssZ  
											// Where: YYYY is the year (4 digits), 
											// MM is the month (2 digits), 
											// DD is the day (2 digits), 
											// hh is the hours (2 digits), 
											// mm is the minutes (2 digits), 
											// ss is the second (2 digits), and 
											// Z indicates the time is UTC time. For example:
											// date="20020125T210600Z"
											// is January 25, 2002 at 9:06pm GMT

		wstring m_creationid, m_changedate, m_changeid ;

	public:
		CTMXHeader() :
			m_datatype(L"unknown")
		{}

		void set_attributes( std::map< wstring, wstring > &attributes )
		{
			m_creationtool = attributes[L"creationtool"] ;
			m_creationtoolversion = attributes[L"creationtoolversion"] ;
			m_segtype = attributes[L"segtype"] ;
			m_o_tmf = attributes[L"o-tmf"] ;
			m_adminlang = attributes[L"adminlang"] ;
			m_srclang = attributes[L"srclang"] ;
			m_datatype = attributes[L"datatype"] ;

			// optional attributes
			if ( attributes.find(L"o-encoding") != attributes.end() ) 
			{
				m_o_encoding = attributes[L"o-encoding"] ;
			}
			if ( attributes.find(L"creationdate") != attributes.end() ) 
			{
				m_creationdate = attributes[L"creationdate"] ;
			}
			if ( attributes.find(L"creationid") != attributes.end() ) 
			{
				m_creationid = attributes[L"creationid"] ;
			}
			if ( attributes.find(L"changedate") != attributes.end() ) 
			{
				m_changedate = attributes[L"changedate"] ;
			}
			if ( attributes.find(L"changeid") != attributes.end() ) 
			{
				m_changeid = attributes[L"changeid"] ;
			}
		}

	};

	CTMXHeader m_header ;



public:
	CTMXReader( memory_engine::memory_pointer mem, CProgressListener *listener ) ;
	~CTMXReader(void);
	// Load a TMX file into a memory
	memory_engine::memory_pointer load_tmx_memory(const CString & file_name);

	void load_head() ;
	void load_body() ;

	size_t get_file_size(const CString & file_name);
	file::file::BYTE_ORDER_MARK get_bom(const CString & file_name);
	void load_utf16(const CString & file_name);
	void load_utf16be(const CString & file_name) ;
	void load_utf8(const CString & file_name);
	void load_utf7(const CString & file_name);
	// Load a translation unit
	void load_tu(const wstring & tu_text);
	// load a seg tag
	const wstring get_seg_text(const wstring& tuv_tag_text);
	int get_record_count(std::set< tstring > &target_languages);


private:
	void parse_tuv_for_language(std::set< tstring >& target_languages);
	void set_only_target_lang(std::set< tstring >& target_languages);
	void user_select_target_lang(std::set< tstring >& target_languages);
};
