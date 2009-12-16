#include "StdAfx.h"
#include ".\tmxreader.h"

#include "ConvertUTF.h"

#include "Tag.h"

#include "tag_stripper.h"

#include "TMXImportDlg.h"
#include "logging.h"
#include "record_local.h"

#include "easyunit/testharness.h"

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

// tmx_strip_tags
wstring tmx_strip_tags( const wstring &raw_string )
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
					str::make_lower( tagChunk ) ;
					str::trim( tagChunk ) ;
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
			reader.advance() ;
			reader.getline( chunk, L"; <", false ) ;
			if ( chunk.empty() == false )
			{
				if ( chunk[0] == L'#' )
				{
					reader.eat_if( L';' ) ;
					ATLASSERT ( chunk.size() > 1 ) ; 
					if ( chunk[1] == L'x' || chunk[1] == L'X' ) 
					{
						stripped_text += (wchar_t)string2ulong( chunk.substr(2), 16 ) ;
					}
					else
					{
						stripped_text += (wchar_t)string2ulong( chunk.substr(1) ) ;
					}
				}
				else
				{
					if ( symbols.exists( chunk ) )
					{
						reader.eat_if( L';' ) ;
						stripped_text += symbols.get_val( chunk ) ;
					}
					else // it was not a symbol tag
					{
						stripped_text += L"&" ;
						stripped_text += chunk ;
					}
				}
			}
		}
	}

	return stripped_text ;
}

// unknown_strip_tags
wstring unknown_strip_tags( const wstring &raw_string )
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
			reader.advance() ;
			reader.getline( chunk, L"; <", false ) ;
			if ( chunk.empty() == false )
			{
				// numeric symbol code
				if ( chunk[0] == L'#' )
				{
					reader.eat_if( L';' ) ;
					ATLASSERT ( chunk.size() > 1 ) ; 
					if ( chunk[1] == L'x' || chunk[1] == L'X' ) 
					{
						stripped_text += (wchar_t)string2ulong( chunk.substr(2), 16 ) ;
					}
					else
					{
						stripped_text += (wchar_t)string2ulong( chunk.substr(1) ) ;
					}
				}
				// named entity
				else
				{
					if ( symbols.exists( chunk ) )
					{
						reader.eat_if( L';' ) ;
						wchar_t symbol_name = symbols.get_val( chunk ) ;
						if ( symbol_name == L'&')
						{
							stripped_text += L"&amp;" ;
						}
						else if ( symbol_name == L'<')
						{
							stripped_text += L"&lt;" ;
						}
						else if ( symbol_name == L'>')
						{
							stripped_text += L"&gt;" ;
						}
						else
						{
							stripped_text += symbol_name ;
						}
					}
					else // it was not a symbol tag
					{
						stripped_text += L"&" ;
						stripped_text += chunk ;
					}
				}
			}
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
memory_pointer CTMXReader::load_tmx_memory(const CString & file_name)
{
	switch ( get_bom(file_name) )
	{
	case file::file::LE_BOM:
		load_utf16(file_name) ;
		break ;
	case file::file::UTF8_BOM: case file::file::UNKNOWN_BOM:
		load_utf8(file_name) ;
		break ;
	case file::file::UTF7_BOM: 
		load_utf7(file_name) ;
		break ;
	case file::file::BE_BOM:
		load_utf16be(file_name) ;
		break ;
	default:
		ATLASSERT ( FALSE && "We should never get here!" ) ; 
		
	}

	load_head() ;

	std::set< tstring > target_languages ;
	int record_count = get_record_count(target_languages) ;

	ATLASSERT ( target_languages.empty() == false ) ; 
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
		if ( str::equal_nocase( type, L"RTFFontTable" ) )
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

		ATLVERIFY(m_reader.find( L"</tu>" )) ;

		wc_reader::bookmark_type tu_end = m_reader.get_current_pos() ;

		const wstring tu_text( tu_start, tu_end ) ;

		++current_record ;
		m_listener->OnProgressLoadUpdate(current_record) ;

		load_tu( tu_text ) ;
	}
}

// Load a translation unit
void CTMXReader::load_tu(const wstring & tu_text)
{
	m_record = mem_engine::record_pointer(new record_local()) ;

	wc_reader tu_reader ;
	tu_reader.set_buffer(tu_text.c_str()) ;

	wc_reader::bookmark_type tu_tag_start = tu_reader.get_current_pos() ;

	ATLVERIFY(tu_reader.find(L">")) ;
	
	wc_reader::bookmark_type tu_tag_end = tu_reader.get_current_pos() ;

	const wstring tu_tag_text(tu_tag_start, tu_tag_end) ;

	CTag<wchar_t> tu_tag ;
	tu_tag.parse_tag(tu_tag_text) ;

	std::map< wstring, wstring > attributes ;
	tu_tag.get_attributes( attributes ) ;

	tmx_reader::CTU tu ;
	tu.set_attributes(attributes) ;
	tu.reflect_attributes(m_record) ;

	while ( tu_reader.find(L"<tuv", false) ) 
	{
		wc_reader::bookmark_type tuv_tag_start = tu_reader.get_current_pos() ;

		ATLVERIFY(tu_reader.find(L">")) ;

		wc_reader::bookmark_type tuv_tag_end = tu_reader.get_current_pos() ;

		wstring tuv_tag_text(tuv_tag_start, tuv_tag_end) ;

		CTag<wchar_t> tuv_tag ;
		tuv_tag.parse_tag(tuv_tag_text) ;

		wstring xml_lang = tuv_tag.get_attribute( L"xml:lang" ) ;
		if ( xml_lang.empty() )
		{
			xml_lang = tuv_tag.get_attribute( L"lang" ) ;
		}

		ATLVERIFY(tu_reader.find(L"</tuv>")) ;

		wc_reader::bookmark_type tuv_end = tu_reader.get_current_pos() ;
		wstring tuv_text( tuv_tag_end, tuv_end ) ;

		const wstring seg_text = get_seg_text( tuv_text ) ;

		if ( str::equal_nocase( xml_lang, m_header.m_srclang ) ) 
		{
			m_record->set_source(seg_text) ;
		}
		else if ( str::equal_nocase( xml_lang, m_target_lang ) )
		{
			m_record->set_trans(seg_text) ;
		}
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

	ATLVERIFY(seg_reader.find(L"<seg>", true)) ;
	
	wc_reader::bookmark_type seg_tag_start = seg_reader.get_current_pos() ;

	ATLVERIFY(seg_reader.find(L"</seg>", false)) ;

	wc_reader::bookmark_type seg_tag_end = seg_reader.get_current_pos() ;

	wstring seg_tag_text(seg_tag_start, seg_tag_end) ;

	if ( str::equal_nocase( m_header.m_datatype, L"rtf" ) ) 
	{
		return m_rtf_importer->seg2html( seg_tag_text ) ;
	}
	else if ( str::equal_nocase( m_header.m_datatype, L"html" ) )
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
	return file::file::size( file_name ) ;
}

file::file::BYTE_ORDER_MARK CTMXReader::get_bom(const CString & file_name)
{
	file::file tmx_file ;
	tmx_file.open_existing( file_name ) ;
	ATLASSERT ( tmx_file.is_open() ) ; 

	file::file::BYTE_ORDER_MARK bom = tmx_file.get_bom() ;
	tmx_file.close() ;

	return bom ;
}

void CTMXReader::load_utf16(const CString & file_name)
{
	LPCWSTR raw_text = (LPCWSTR)m_view.create_view(file_name) ;
	m_reader.set_buffer(raw_text+1) ;
}

void CTMXReader::load_utf16be(const CString & file_name)
{
	const size_t file_size = get_file_size(file_name) ;

	LPCWSTR raw_text = (LPCWSTR)m_view.create_view(file_name) ;

	boost::scoped_array<wchar_t> buffer(new wchar_t[(file_size / 2 ) + 1]);
	LPWSTR flipped_text = buffer.get() ;

	cracker crack1, crack2 ;
	for ( size_t i = 1 ; raw_text[i] && i < ( file_size / 2 ) + 1 ; ++i )
	{
		crack1.w = raw_text[i] ;
		crack2.c[0] = crack1.c[1] ;
		crack2.c[1] = crack1.c[0] ;
		flipped_text[i] = crack2.w ;
	}
	m_text = flipped_text ;
	m_reader.set_buffer(m_text.c_str()+1) ;
}

void CTMXReader::load_utf8(const CString & file_name)
{
	LPCSTR raw_text = (LPCSTR)m_view.create_view(file_name) ;
	raw_text+= file::file::bom_size( get_bom(file_name) ) ;
	const int file_size = get_file_size( file_name ) - file::file::bom_size( get_bom(file_name) ) ;

	const string text(raw_text, file_size) ;
	m_text = string2wstring(text, CP_UTF8) ;

	m_reader.set_buffer( m_text.c_str() ) ;
}
void CTMXReader::load_utf7(const CString & file_name)
{
	LPCSTR raw_text = (LPCSTR)m_view.create_view(file_name) ;
	raw_text+= file::file::bom_size( file::file::UTF7_BOM ) ;
	const int file_size = get_file_size( file_name ) - file::file::bom_size( file::file::UTF7_BOM ) ;

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
	if ( ! str::equal_nocase( lang_name, m_header.m_srclang ) )
	{
		target_languages.insert(boost::to_upper_copy(string2tstring( lang_name ))) ;
	}

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
				e ;
				logging::log_exception(e) ;
			}
			catch(boost::bad_lexical_cast& e)
			{
				e ;
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
				logging::log_exception(e) ;
			}
			catch(boost::bad_lexical_cast& e)
			{
				logging::log_error(e.what()) ;
			}
		}
	}

	void CTU::fill_date( misc_wrappers::date &thedate, const wstring &datestring )
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
}
// =====================>

#ifdef _DEBUG

namespace easyunit
{
	TEST( txmStripTags, TestSimple )
	{
		wstring toStrip = L"<bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		ASSERT_EQUALS_M( tmx_strip_tags( toStrip ), L"<b>hello</b>", "String should be '<b>hello</b>'" ) ;
	}
	TEST( txmStripTags, TestIt )
	{
		wstring toStrip = L"<it pos=\"Begin\">&lt;1&gt;</it><bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		ASSERT_EQUALS_M( tmx_strip_tags( toStrip ), L"<b>hello</b>", "String should be '<b>hello</b>'" ) ;

		//toStrip = L"<it pos=\"Begin\">&lt;br&gt;</it><bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		//ASSERT_EQUALS_M( tmx_strip_tags( toStrip ), L"<br><b>hello</b>", "String should be '<b>hello</b>'" ) ;

		//toStrip = L"<it pos=\"Begin\">&lt;br /&gt;</it><bpt>&lt;b&gt;</bpt>hello<ept>&lt;/b&gt;</ept>" ;
		//ASSERT_EQUALS_M( tmx_strip_tags( toStrip ), L"<br /><b>hello</b>", "String should be '<b>hello</b>'" ) ;

		toStrip = L"<it pos=\"begin\" x=\"1\">&lt;1&gt;</it>Digitally Sign a Macro Project in Microsoft Word 2000" ;
		ASSERT_EQUALS_M( tmx_strip_tags( toStrip ), L"Digitally Sign a Macro Project in Microsoft Word 2000", "String should be 'Digitally Sign a Macro Project in Microsoft Word 2000'" ) ;
	}
	TEST( unknown_strip_tags_tests, simple)
	{
		wstring raw_string = L"foobar" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		ASSERT_EQUALS_M( raw_string, stripped_text, "Expected foobar") ;
	}
	TEST( unknown_strip_tags_tests, ampersand)
	{
		wstring raw_string = L"foo &amp; bar" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		ASSERT_EQUALS_M( raw_string, stripped_text, "Expected 'foo &amp; bar'") ;
	}

	TEST( unknown_strip_tags_tests, angle_brackets)
	{
		wstring raw_string = L"&lt;foo&gt;" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		ASSERT_EQUALS_M( raw_string, stripped_text, "Expected '&lt;foo&gt;'") ;
	}

	TEST( unknown_strip_tags_tests, ignored_tags)
	{
		wstring raw_string = L"<b>foo</b>" ;

		wstring stripped_text = unknown_strip_tags( raw_string ) ;

		wstring expected = L"" ;
		TRACE( stripped_text ) ;
		ASSERT_EQUALS_M( expected, stripped_text, "Expected ''") ;
	}
}

#endif
