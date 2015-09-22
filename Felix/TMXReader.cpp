#include "StdAfx.h"
#include ".\tmxreader.h"

#include "Tag.h"

#include "tag_stripper.h"

#include "TMXImportDlg.h"
#include "TMXImportLangsDlg.h"
#include "logging.h"
#include "record_local.h"
#include "input_device_file.h"

#ifdef UNIT_TEST
	#include <boost/test/unit_test.hpp>
#endif

using namespace mem_engine ;
using namespace except;

/**
	@union cracker
	@brief Used to break up wchar_t into char's.
 */
union cracker
{
	wchar_t	w ;
	char	c[2] ;
} ;

wstring tmx_handle_ampersand( wc_reader &reader, symbol_map &symbols) 
{
	wstring symbol_name = handle_ampersand(reader, symbols);
	if ( symbol_name == L"&")
	{
		return L"&amp;" ;
	}
	else if ( symbol_name == L"<")
	{
		return L"&lt;" ;
	}
	else if ( symbol_name == L">")
	{
		return L"&gt;" ;
	}
	else
	{
		return symbol_name ;
	}
}

// tmx_strip_tags
wstring tmx_strip_tags( const wstring raw_string )
{
	typedef textstream_reader< wchar_t > reader_type ;
	typedef textstream_reader< wchar_t >::bookmark_type bm_type ;

	static symbol_map symbols ;

	reader_type reader ;

	reader.set_buffer( raw_string.c_str() ) ;

	wstring stripped_text ;
	stripped_text.reserve( raw_string.size() ) ;

	while ( reader.empty() == false ) 
	{
		wstring chunk ;
		reader.getline( chunk, L"<&", false ) ;
		if ( chunk.empty() == false )
		{
			stripped_text += chunk ;
		}

		if ( reader.peek() == L'<' )
		{
			bm_type current_pos = reader.get_current_pos() ;

			reader.advance() ;

			// it = isolated tag. No beginning and end
			// This will usually contain trash tags.
			// The only one we will allow will be the <br> tag
			if ( towlower( reader.peek() ) == L'i' )
			{
				reader.advance( ) ;
				if ( towlower( reader.peek() )== L't' )
				{
					reader.find( L">", true ) ;
					wstring tagChunk = reader.getline(L'<', false ) ;
					boost::to_lower( tagChunk ) ;
					boost::trim( tagChunk ) ;
					if ( tagChunk == L"&lt;br&gt;" )
					{
						stripped_text += L"<br>" ;
					}
					else if ( tagChunk == L"&lt;br /&gt;" )
					{
						stripped_text += L"<br />" ;
					}
					reader.find( L">", true ) ;
				}
				else
				{
					reader.find( L">", true ) ;
				}
			}
			// it's not an <it> tag
			else
			{
				bool item_found = reader.jump_to_first_of( L"<>&", false ) ;

				if ( ! item_found || ! reader.current_is( L'>' ) )
				{
					reader.set_pos( current_pos ) ;

					ATLASSERT( reader.current_is( L'<' ) ) ;

					reader.get( stripped_text ) ;
				}
				else
				{
					ATLASSERT( item_found ) ;
					ATLASSERT( reader.current_is( L'>' ) ) ;

					reader.advance( ) ;
				}
			}

		}
		else if ( reader.peek() == L'&' )
		{
			stripped_text += handle_ampersand(reader, symbols);
		}
	}

	return stripped_text ;
}


// unknown_strip_tags
wstring unknown_strip_tags( const wstring raw_string )
{
	typedef textstream_reader< wchar_t > reader_type ;
	typedef textstream_reader< wchar_t >::bookmark_type bm_type ;

	static symbol_map symbols ;

	reader_type reader ;

	reader.set_buffer( raw_string.c_str() ) ;

	wstring stripped_text ;
	stripped_text.reserve( raw_string.size() ) ;

	while ( reader.empty() == false ) 
	{
		wstring chunk ;
		reader.getline( chunk, L"<&", false ) ;
		if ( chunk.empty() == false )
		{
			stripped_text += chunk ;
		}

		// eat all tags and their contents
		if ( reader.peek() == L'<' )
		{
			reader.find( L"</", true ) ;
			reader.jump_to_first_of( L'>', true ) ;
		}
		// convert xml symbols
		else if ( reader.peek() == L'&' )
		{
			stripped_text += tmx_handle_ampersand(reader, symbols);
		}
	}

	return stripped_text ;
}



CTMXReader::CTMXReader( mem_engine::memory_pointer mem, CProgressListener *listener ) :
	m_memory( mem ),
	m_listener(listener) ,
	m_rtf_importer(new tmx_data_importer)
{
	m_record = record_pointer(new record_local()) ;
}

CTMXReader::~CTMXReader(void)
{
}

// Load a TMX file into a memory
memory_pointer CTMXReader::load_tmx_memory(const CString & file_name, input_device_ptr input)
{
	logging::log_debug(string("Loading TMX memory file ") + static_cast<LPCSTR>(CT2A(file_name))) ;

	switch ( get_bom(file_name, input) )
	{
	case file::BOM_LE:
		logging::log_verbose("Loading TMX file as UTF-16 encoding") ;
		load_utf16(file_name, input) ;
		break ;
	case file::BOM_UTF8: case file::BOM_UNKNOWN:
		logging::log_verbose("Loading TMX file as UTF-8 encoding") ;
		load_utf8(file_name, input) ;
		break ;
	case file::BOM_UTF7: 
		logging::log_verbose("Loading TMX file as UTF-7 encoding") ;
		load_utf7(file_name, input) ;
		break ;
	case file::BOM_BE:
		logging::log_verbose("Loading TMX file as UTF-16-BE encoding") ;
		load_utf16be(file_name, input) ;
		break ;
	default:
		ATLASSERT ( FALSE && "We should never get here!" ) ; 
		
	}

	load_head() ;

	std::set< tstring > languages ;
	const size_t record_count = get_record_count(languages) ;
	std::set< tstring > target_languages ;
	FOREACH(tstring language, languages)
	{
		if (! boost::iequals(language, m_header.m_srclang))
		{
			target_languages.insert(language) ;
		}
	}
	ATLASSERT ( target_languages.empty() == false ) ; 

	// If the srclang isn't actually in the records, then have
	// the user select the source language as well.
	if (! srclang_in_languages(languages))
	{
		logging::log_verbose("Getting source and target languages from user") ;
		CTMXImportLangsDlg dlg ;
		dlg.set_languages( target_languages ) ;
#ifdef UNIT_TEST
		m_target_lang = L"EN" ;
		m_header.m_srclang = L"JA" ;
#else
		if ( IDCANCEL != dlg.DoModal() ) 
		{
			m_target_lang = string2wstring( dlg.get_trans_plain() ) ;
			m_header.m_srclang = string2wstring( dlg.get_source_plain() ) ;
		}
#endif
	}
	else
	{
		if ( target_languages.size() > 1 ) 
		{
#ifdef UNIT_TEST
			std::set< tstring > target ;
			set_only_target_lang(target); // will select EN-US
#else
			user_select_target_lang(target_languages);
#endif
		}
		else
		{
			set_only_target_lang(target_languages);
		}
	}


	MemoryInfo *mem_info = m_memory->get_memory_info() ;
	mem_info->set_source_language(m_header.m_srclang) ;
	mem_info->set_target_language(m_target_lang) ;

	if ( record_count == 0 ) 
	{
		return m_memory ;
	}

	m_listener->OnProgressInit(file_name, 0, record_count) ;

	load_body() ;

	m_listener->OnProgressDoneLoad(record_count) ;

	m_memory->set_saved_flag(true) ;

	return m_memory ;
}

void CTMXReader::load_head()
{
	bool success = m_reader.find( L"<header" ) ;
	ATLASSERT ( success ) ;

	wc_reader::bookmark_type head_start = m_reader.get_current_pos() ;

	success = m_reader.find( L">" ) ;
	ATLASSERT ( success ) ;

	wc_reader::bookmark_type head_tag_end = m_reader.get_current_pos() ;

	const wstring head_tag_text( head_start, head_tag_end ) ;

	CTag<wchar_t> head_tag ;
	head_tag.parse_tag( head_tag_text ) ;

	std::map< wstring, wstring > attributes ;
	head_tag.get_attributes( attributes ) ;

	m_header.set_attributes(attributes) ;

	MemoryInfo *mem_info = m_memory->get_memory_info() ;
	mem_info->set_creation_tool( m_header.m_creationtool ) ;
	mem_info->set_creation_tool_version( m_header.m_creationtoolversion ) ;

	ATLASSERT ( m_memory->get_memory_info()->get_creation_tool() == m_header.m_creationtool ) ; 
	ATLASSERT ( m_memory->get_memory_info()->get_creation_tool_version() == m_header.m_creationtoolversion ) ; 

	// for now, we will forget about the prop and other features...

	m_reader.find( L"</header>", true ) ;

	wc_reader::bookmark_type header_end = m_reader.get_current_pos() ;

	wstring header_text(head_tag_end, header_end ) ;

	wc_reader header_reader ;
	header_reader.set_buffer(header_text.c_str()) ;
	while (header_reader.find(L"<prop", false ) ) 
	{
		const wstring prop_tag_text = header_reader.getline(L'>', true ) ;
		CTag<wchar_t> prop_tag ;
		prop_tag.parse_tag( prop_tag_text ) ;
		const wstring type = prop_tag.get_attribute( L"type" ) ;
		if ( boost::iequals( type, L"RTFFontTable" ) )
		{
			const wstring font_table_text = header_reader.getline(L'>', true ) ;
			m_rtf_importer->parse_fonttbl( font_table_text ) ;
		}
	}
	
}
void CTMXReader::load_body()
{
	int current_record = 0 ;

	while ( m_reader.find( L"<tu", false ) ) 
	{
		wc_reader::bookmark_type tu_start = m_reader.get_current_pos() ;

		if (! m_reader.find( L"</tu>" ))
		{
			wstring error = L"TMX Reader failed to find end of TU segment. TMX file may be corrupt." ;
			m_errors.push_back(error) ;
			logging::log_error(string2string(error)) ;
			return ;
		}

		wc_reader::bookmark_type tu_end = m_reader.get_current_pos() ;

		const wstring tu_text( tu_start, tu_end ) ;

		++current_record ;
		m_listener->OnProgressLoadUpdate(current_record) ;

		load_tu( tu_text ) ;
	}
}

// Load a translation unit
void CTMXReader::load_tu(const wstring  tu_text)
{
	m_record = mem_engine::record_pointer(new record_local()) ;

	wc_reader tu_reader ;
	tu_reader.set_buffer(tu_text.c_str()) ;

	wc_reader::bookmark_type tu_tag_start = tu_reader.get_current_pos() ;

	if(! tu_reader.find(L">"))
	{
		wstring error = L"TMX Reader failed to find end of TU tag. TMX file may be corrupt." ;
		m_errors.push_back(error) ;
		logging::log_error(string2string(error)) ;
		return ;
	}
	
	wc_reader::bookmark_type tu_tag_end = tu_reader.get_current_pos() ;

	const wstring tu_tag_text(tu_tag_start, tu_tag_end) ;

	CTag<wchar_t> tu_tag ;
	tu_tag.parse_tag(tu_tag_text) ;

	std::map< wstring, wstring > attributes ;
	tu_tag.get_attributes( attributes ) ;

	tmx_reader::CTU tu ;
	tu.set_attributes(attributes) ;
	tu.reflect_attributes(m_record) ;

	wstring leftover_text ; 

	while ( tu_reader.find(L"<tuv", false) ) 
	{
		wc_reader::bookmark_type tuv_tag_start = tu_reader.get_current_pos() ;

		if(! tu_reader.find(L">"))
		{
			wstring error = L"TMX Reader failed to find end of TUV tag. TMX file may be corrupt." ;
			m_errors.push_back(error) ;
			logging::log_error(string2string(error)) ;
			return ;
		}

		wc_reader::bookmark_type tuv_tag_end = tu_reader.get_current_pos() ;

		wstring tuv_tag_text(tuv_tag_start, tuv_tag_end) ;

		CTag<wchar_t> tuv_tag ;
		tuv_tag.parse_tag(tuv_tag_text) ;

		wstring xml_lang = tuv_tag.get_attribute( L"xml:lang" ) ;
		if ( xml_lang.empty() )
		{
			xml_lang = tuv_tag.get_attribute( L"lang" ) ;
		}

		if(! tu_reader.find(L"</tuv>"))
		{
			wstring error = L"TMX Reader failed to find end of TUV segment. TMX file may be corrupt." ;
			m_errors.push_back(error) ;
			logging::log_error(string2string(error)) ;
			return ;
		}

		wc_reader::bookmark_type tuv_end = tu_reader.get_current_pos() ;
		wstring tuv_text( tuv_tag_end, tuv_end ) ;

		const wstring seg_text = get_seg_text( tuv_text ) ;

		if ( boost::iequals( xml_lang, m_header.m_srclang ) ) 
		{
			m_record->set_source(seg_text) ;
		}
		else if ( boost::iequals( xml_lang, m_target_lang ) )
		{
			m_record->set_trans(seg_text) ;
		}
		else
		{
			leftover_text = seg_text ;
		}
	}

	if (m_record->get_source_plain().empty())
	{
		m_record->set_source(leftover_text) ;
	}
	if ( m_record->is_valid_record() ) 
	{
		m_memory->add_record(m_record) ;
	}

}

const wstring CTMXReader::get_seg_text(const wstring& tuv_text)
{
	wc_reader seg_reader ;
	seg_reader.set_buffer( tuv_text.c_str() ) ;

	if(! seg_reader.find(L"<seg>", true))
	{
		wstring error = L"TMX Reader failed to find SEG tag. TMX file may be corrupt." ;
		m_errors.push_back(error) ;
		logging::log_error(string2string(error)) ;
		return wstring();
	}
	wc_reader::bookmark_type seg_tag_start = seg_reader.get_current_pos() ;

	if(! seg_reader.find(L"</seg>", false))
	{
		wstring error = L"TMX Reader failed to find SEG closing tag. TMX file may be corrupt." ;
		m_errors.push_back(error) ;
		logging::log_error(string2string(error)) ;
		return wstring();
	}

	wc_reader::bookmark_type seg_tag_end = seg_reader.get_current_pos() ;

	wstring seg_tag_text(seg_tag_start, seg_tag_end) ;

	if ( boost::iequals( m_header.m_datatype, L"rtf" ) ) 
	{
		return m_rtf_importer->seg2html( seg_tag_text ) ;
	}
	else if ( boost::iequals( m_header.m_datatype, L"html" ) )
	{
		return tmx_strip_tags( seg_tag_text ) ;
	}
	else
	{
		return unknown_strip_tags( seg_tag_text ) ;
	}
}

size_t CTMXReader::get_file_size(const CString & file_name)
{
	input_device_ptr input(new InputDeviceFile) ;
	return input->get_size(file_name) ;
}

file::BYTE_ORDER_MARK CTMXReader::get_bom(const CString & file_name, input_device_ptr input)
{
	return input->get_file_bom(file_name) ;
}

void CTMXReader::load_utf16(const CString & file_name, input_device_ptr input)
{
	logging::log_verbose("Loading TMX file with UTF-16 encoding") ;
	LPCWSTR raw_text = input->create_view_wchar(file_name) ;
	m_text = raw_text+1 ;
	m_reader.set_buffer(m_text.c_str()) ;
}

void CTMXReader::load_utf16be(const CString & file_name, input_device_ptr input)
{
	const size_t file_size = input->get_size(file_name) ;
	LPCWSTR raw_text = input->create_view_wchar(file_name) ;

	const size_t wfile_size = (file_size / 2 ) + 1 ;
	boost::scoped_array<wchar_t> buffer(new wchar_t[wfile_size]);
	LPWSTR flipped_text = buffer.get() ;

	cracker crack1, crack2 ;
	for ( size_t i = 1 ; i < wfile_size && raw_text[i] ; ++i )
	{
		crack1.w = raw_text[i] ;
		crack2.c[0] = crack1.c[1] ;
		crack2.c[1] = crack1.c[0] ;
		flipped_text[i] = crack2.w ;
	}
	m_text = flipped_text ;
	m_reader.set_buffer(m_text.c_str()+1) ;
}

void CTMXReader::load_utf8(const CString & file_name, input_device_ptr input)
{
	LPCSTR raw_text = input->create_view_char(file_name) ;
	const UINT bom_size = file::file::bom_size( input->get_file_bom(file_name) ) ;
	raw_text+= bom_size ;
	const int file_size = input->get_size(file_name) - bom_size ;

	const string text(raw_text, file_size) ;
	m_text = string2wstring(text) ;

	m_reader.set_buffer( m_text.c_str() ) ;
}
void CTMXReader::load_utf7(const CString & file_name, input_device_ptr input)
{
	LPCSTR raw_text = input->create_view_char(file_name) ;
	const UINT bom_size = file::file::bom_size( file::BOM_UTF7 ) ;
	raw_text+= bom_size ;
	const int file_size = input->get_size(file_name) - bom_size ;

	const string text(raw_text, file_size) ;
	m_text = string2wstring(text, CP_UTF7) ;

	m_reader.set_buffer( m_text.c_str() ) ;
}


int CTMXReader::get_record_count(std::set< tstring > &target_languages)
{
	int record_count = 0 ;
	m_reader.rewind() ;
	while ( m_reader.find(L"<tu", true ) ) 
	{
		if ( ! m_reader.current_is(L'v') ) 
		{
			record_count++ ;
		}
		else
		{
			parse_tuv_for_language(target_languages);
		}
	}
	m_reader.rewind() ;
	return record_count ;
}

// this should be done by parent --
// we need to decouple UI from this class!
void CTMXReader::user_select_target_lang(std::set< tstring >& target_languages)
{
	CTMXImportDlg dlg ;
	dlg.set_languages( target_languages ) ;
	if ( IDCANCEL == dlg.DoModal() ) 
	{
		return ;
	}
	m_target_lang = string2wstring( dlg.get_trans_plain() ) ;
}

// Number of target languages should be 1
// But we allow the possibility of it being 0 (just in case...)
void CTMXReader::set_only_target_lang(std::set< tstring >& target_languages)
{
	std::set< tstring >::iterator pos ;
	pos = target_languages.begin() ;
	if ( pos != target_languages.end() ) 
	{
		m_target_lang = string2wstring( *pos ) ;
	}
	// don't let the funky format stop us!
	else
	{
		m_target_lang = L"EN-US" ;
	}
}

void CTMXReader::parse_tuv_for_language(std::set< tstring >& target_languages)
{
	const wstring tuv_tag = m_reader.getline(L'>') ;

	wc_reader tuv_reader( tuv_tag.c_str() ) ;

	tuv_reader.find( L"lang=\"", true ) ;

	if ( tuv_reader.empty() )
	{
		return ;
	}

	const wstring lang_name = tuv_reader.getline(L'\"') ;
	target_languages.insert(boost::to_upper_copy(string2wstring( lang_name ))) ;

}

bool CTMXReader::srclang_in_languages( const std::set< tstring > &languages )
{
	return languages.find(boost::to_upper_copy(m_header.m_srclang)) != languages.end();
}
namespace tmx_reader
{
	void CTU::set_attributes( std::map< wstring, wstring > &attributes )
	{
		// optional attributes
		if ( attributes.find(L"tuid") != attributes.end() ) 
		{
			m_tuid = attributes[L"tuid"] ;
		}
		if ( attributes.find(L"usagecount") != attributes.end() ) 
		{
			m_usagecount = attributes[L"usagecount"] ;
		}
		if ( attributes.find(L"lastusagedate") != attributes.end() ) 
		{
			m_lastusagedate = attributes[L"lastusagedate"] ;
		}

		if ( attributes.find(L"creationtool") != attributes.end() ) 
		{
			m_creationtool = attributes[L"creationtool"] ;
		}
		if ( attributes.find(L"creationtoolversion") != attributes.end() ) 
		{
			m_creationtoolversion = attributes[L"creationtoolversion"] ;
		}
		if ( attributes.find(L"segtype") != attributes.end() ) 
		{
			m_segtype = attributes[L"segtype"] ;
		}
		if ( attributes.find(L"o-tmf") != attributes.end() ) 
		{
			m_o_tmf = attributes[L"o-tmf"] ;
		}
		if ( attributes.find(L"srclang") != attributes.end() ) 
		{
			m_srclang = attributes[L"srclang"] ;
		}
		if ( attributes.find(L"datatype") != attributes.end() ) 
		{
			m_datatype = attributes[L"datatype"] ;
		}

		set_optional_attributes(attributes);

	}

	void CTU::reflect_attributes( record_pointer record )
	{
		record->set_creator(m_creationid) ;
		record->set_modified_by(m_changeid) ;
		// Date in [ISO 8601] Format. The recommended pattern to use is: YYYYMMDDThhmmssZ
		const wstring canon_format = L"YYYYMMDDThhmmssZ" ;
		if (m_creationdate.size() == canon_format.size())
		{
			try
			{
				misc_wrappers::date thedate ;

				fill_date(thedate, m_creationdate);

				record->set_created(thedate) ;
			}
			catch (CException& e)
			{
				logging::log_error("Error reflecting record creation date from TMX file") ;
				logging::log_exception(e) ;
			}
			catch(boost::bad_lexical_cast& e)
			{
				logging::log_error("Error reflecting record creation date from TMX file") ;
				logging::log_error(e.what()) ;
			}
		}
		if (m_changedate.size() == canon_format.size())
		{
			try
			{
				misc_wrappers::date thedate ;
				fill_date(thedate, m_changedate);
				record->set_modified(thedate) ;
			}
			catch (CException& e)
			{
				logging::log_error("Error reflecting record change date from TMX file") ;
				logging::log_exception(e) ;
			}
			catch(boost::bad_lexical_cast& e)
			{
				logging::log_error("Error reflecting record change date from TMX file") ;
				logging::log_error(e.what()) ;
			}
		}
	}

	void CTU::fill_date( misc_wrappers::date &thedate, const wstring datestring )
	{
		wstring date_part ;

		date_part = datestring.substr(0, 4) ;
		thedate.set_year(date_part) ;
		date_part = datestring.substr(4, 2) ;
		thedate.set_month(date_part) ;
		date_part = datestring.substr(6, 2) ;
		thedate.set_day(date_part) ;

		date_part = datestring.substr(9, 2) ;
		thedate.set_hour(date_part) ;
		date_part = datestring.substr(11, 2) ;
		thedate.set_minute(date_part) ;
		date_part = datestring.substr(13, 2) ;
		thedate.set_second(date_part) ;
	}

	void tmx_data::set_optional_attributes( std::map< wstring, wstring > &attributes )
	{
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
}
// =====================>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( test_txmStripTags )

	BOOST_AUTO_TEST_CASE( TestSimple )
	{
		wstring toStrip = L"<bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		BOOST_CHECK_EQUAL( tmx_strip_tags( toStrip ), L"<b>hello</b>") ;
	}
	BOOST_AUTO_TEST_CASE( TestIt )
	{
		wstring toStrip = L"<it pos=\"Begin\">&lt;1&gt;</it><bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		BOOST_CHECK_EQUAL( tmx_strip_tags( toStrip ), L"<b>hello</b>") ;

		//toStrip = L"<it pos=\"Begin\">&lt;br&gt;</it><bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		//BOOST_CHECK_EQUAL( tmx_strip_tags( toStrip ), L"<br><b>hello</b>", "String should be '<b>hello</b>'" ) ;

		//toStrip = L"<it pos=\"Begin\">&lt;br /&gt;</it><bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		//BOOST_CHECK_EQUAL( tmx_strip_tags( toStrip ), L"<br /><b>hello</b>", "String should be '<b>hello</b>'" ) ;

		toStrip = L"<it pos=\"begin\" x=\"1\">&lt;1&gt;</it>Digitally Sign a Macro Project in Microsoft Word 2000" ;
		BOOST_CHECK_EQUAL( tmx_strip_tags( toStrip ), L"Digitally Sign a Macro Project in Microsoft Word 2000") ;
	}
	BOOST_AUTO_TEST_CASE( unknown_strip_tags_tests_simple)
	{
		wstring raw_string = L"foobar" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		BOOST_CHECK_EQUAL( raw_string, stripped_text) ;
	}
	BOOST_AUTO_TEST_CASE( unknown_strip_tags_tests_ampersand)
	{
		wstring raw_string = L"foo &amp; bar" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		BOOST_CHECK_EQUAL( raw_string, stripped_text) ;
	}

	BOOST_AUTO_TEST_CASE( unknown_strip_tags_tests_angle_brackets)
	{
		wstring raw_string = L"&lt;foo&gt;" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		BOOST_CHECK_EQUAL( raw_string, stripped_text) ;
	}

	BOOST_AUTO_TEST_CASE( unknown_strip_tags_tests_ignored_tags)
	{
		wstring raw_string = L"<b>foo</b>" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		wstring expected = L"" ;
		BOOST_CHECK_EQUAL( expected, stripped_text) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
