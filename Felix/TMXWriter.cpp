/*!
 * \file TMXWriter.cpp
 */

#include "StdAfx.h"
#include ".\tmxwriter.h"
#include "Tag.h"	// CTag
#include "File.h"
#include "memory_local.h"

#include "ResHtmlFile.h"

#include "TmxSymbolMapper.h"
#include "Felix_properties.h"

CTMXWriter::CTMXWriter(CProgressListener *listener, 
					   app_props::properties_memory *props,
					   wstring username) :
	m_listener(listener)
	, m_iLevel(0)
	, m_tuid(1)
	, m_memory(new mem_engine::memory_local(props))
	, m_file(new OutputDeviceFile)
	, m_user_name(username)
{
}

CTMXWriter::~CTMXWriter(void)
{
}

/*!
 * \brief
 * Write the memory to a TMX file.
  */
void CTMXWriter::write_memory(mem_engine::memory_pointer mem)
{
	m_memory = mem ;
	prepareToWrite();

	write_header() ;

	write_tus() ;

	write_footer() ;

	cleanup();
}

/*!
 * \brief
 * Write the TMX header.
 *
 * Uses a text template to fill in the source language and version.
 * 
 * \remarks
 Sample header:
<?xml version="1.0" ?>
<!DOCTYPE tmx SYSTEM "tmx14.dtd">
<tmx version="1.4.5">
  <header
    creationtool="Felix"
    creationtoolversion="1.5"
    datatype="html"
    segtype="sentence"
    adminlang="EN-US"
    srclang="JA"
    o-tmf="FelixTM"
  >
  </header>
  <body> * 

	\todo
	Get the version from our Version resource

 */
void CTMXWriter::write_header(void)
{
	m_data[L"srclang"] = cpptempl::make_data(m_src_lang ) ;
	string v = string(VERSION) ;
	m_data[L"version"] = cpptempl::make_data(string2wstring(v) ) ;

	CResHtmlFile resFile( _T("TMX_HEAD") ) ;
	const wstring tmplText = (LPCWSTR)resFile.text() ;
	ATLASSERT( ! tmplText.empty() ) ;

	m_file->write( cpptempl::parse(tmplText, m_data) ) ;
}

/*!
 * \brief
 * Writes the TMX footer.
 * 
 * Footer text: "  </body>\n</tmx>"
 */
void CTMXWriter::write_footer(void)
{
	m_file->write( L"  </body>\n</tmx>") ;
}

/*!
 * \brief
 * Writes a tu segment for a memory record.
 * 
 * \param mem_engine::record_pointer rec
 * The record to write
 * 
 * Uses a text template, and fills in necessary info from record.
 */
void CTMXWriter::write_tu(mem_engine::record_pointer rec)
{
	m_file->write( this->make_tu(rec, tmplText) ) ;
}

/*!
  \brief
  Writes all the tus in the memory.
  
  Assigns the source and trans languages to the template, then
  Loops through the memory's records, writing each one.
  
	\remarks
	Requires a progress listener to be set.
	Assumes m_tuid == 1

	\todo
	Partially compile text template to avoid filling fixded information over and over

 */
void CTMXWriter::write_tus(void)
{
	CResHtmlFile resFile( _T("TMX_TU") ) ;
	tmplText = (LPCWSTR)resFile.text() ;

	m_data[L"srclang"] = cpptempl::make_data(m_src_lang ) ;
	m_data[L"translang"] = cpptempl::make_data(m_target_lang ) ;

	ATLASSERT( m_tuid == 1 ) ;
	m_tuid = 1 ;
	foreach( mem_engine::record_pointer rec, m_memory->get_records())
	{
		write_tu( rec ) ;
		m_listener->OnProgressWriteUpdate( m_tuid ) ;
		m_tuid++ ;
	}
}

/*!
 * \brief
 * Converts the trans or source segment into TMX format.
 * 
 * Replaces tags and HTML character entities (&rquo; etc.)
 */
wstring CTMXWriter::get_segment(const wstring  text)
{

	ATLASSERT ( ! text.empty() ) ; 
	
	wstring seg ;

	wc_reader reader ;

	wstring localtmp(text) ;
	boost::replace_all(localtmp, L" & ", L" &amp; ") ;
	boost::replace_all(localtmp, L"< ", L"&lt; ") ;
	reader.set_buffer( localtmp.c_str() ) ;

	while ( ! reader.empty() ) 
	{
		wstring text_chunk = reader.getline(L'<', false ) ; // false means don't eat
		if ( ! text_chunk.empty() ) 
		{
			ATLASSERT ( text_chunk.find( L'<' ) == wstring::npos ) ; 
			
			boost::replace_all(text_chunk, L">", L"&gt;") ;
			seg += ( cleanUpForTMX( text_chunk ) ) ;
		}
		if ( reader.current_is( L'<' ) ) 
		{
			seg += get_tag( reader ) ;
		}
	}
	return seg ;
}

/*!
 * \brief
 * Get rid of any remaining control (non-printable) characters before writing to the file.
  */
std::wstring CTMXWriter::cleanUpForTMX(std::wstring& text_chunk)
{
	ATLASSERT ( text_chunk.find(L'<') == wstring::npos ) ;
	CTmxSymbolMapper mapper ;
	wstring stripped = mapper.strip_html_symbols( text_chunk ) ;

	wstring::iterator pos ;

	for ( pos = stripped.begin() ; pos != stripped.end() ; )
	{
		if ( static_cast< unsigned short >( *pos ) < 20u )
		{
			pos = stripped.erase( pos ) ;
		}
		else
		{
			++pos ;
		}
	}

	return stripped ;
}

/*!
 * \brief
 * Convert a tag into TMX format.
  */
wstring CTMXWriter::get_tag(wc_reader & reader)
{
	ATLASSERT ( reader.current_is( L'<' ) ) ; 
	
	wstring tag_text = reader.getline(L'>', false ) ;
	ATLASSERT ( reader.current_is( L'>' ) ) ;
	reader.get( tag_text ) ;
	ATLASSERT ( tag_text.find( L'&' ) == wstring::npos ) ; 

	CTag< wchar_t > tag ;
	tag.parse_tag( tag_text ) ;

	boost::replace_all( tag_text, L"<", L"&lt;" ) ;
	boost::replace_all( tag_text, L">", L"&gt;" ) ;

	m_data[L"tagtext"] = cpptempl::make_data(tag_text ) ;
	if ( tag.is_end_tag() ) 
	{
		ATLASSERT ( m_iLevel > 0 ) ; 
		
		m_data[L"eptid"] = cpptempl::make_data(int2wstring( m_iLevel ) ) ;
		m_iLevel-- ;
		return cpptempl::parse( L"<ept i=\"{$eptid}\">{$tagtext}</ept>", m_data ) ;

	}
	else if ( tag.tag_is( L"br" ) || tag.tag_is( L"hr") )
	{
		return cpptempl::parse( L"<it pos=\"begin\" x=\"1\">{$tagtext}</it>", m_data ) ;
	}
	else
	{
		m_iLevel++ ;
		m_data[L"eptid"] = cpptempl::make_data(int2wstring( m_iLevel ) ) ;
		return cpptempl::parse( L"<bpt i=\"{$eptid}\" x=\"{$eptid}\">{$tagtext}</bpt>", m_data ) ;
	}
}


/*!
 * \brief
 * Get a date/time string in TMX format.
  */
wstring CTMXWriter::tmx_date( const misc_wrappers::date &ta_date ) 
{
	return string2wstring( ta_date.get_date_time_string( _T("yyyyMMdd'T'"), _T( "HHmmss'Z'" ), _T( "" ) ) ) ;
}

/*!
 * \brief
 * Set the source language.
 */
void CTMXWriter::set_src_lang( const wstring lang ) 
{
	m_src_lang = boost::to_upper_copy(lang) ;
}

/*!
 * \brief
 * Set the target language.
 */
void CTMXWriter::set_target_lang( const wstring lang ) 
{
	m_target_lang = boost::to_upper_copy(lang) ;
}

void CTMXWriter::prepareToWrite()
{
	m_tuid = 1 ;

	m_user_name = string2wstring( m_memory->get_memory_info()->get_creator() ) ;
	const CString location = m_memory->get_location() ;
	m_source_file_name = CT2W( file::name( location ).file_name() ) ;

	m_listener->OnProgressInit( location, 0, m_memory->size() ) ;

	m_file->open( location ) ;
}

void CTMXWriter::cleanup()
{
	m_file->close() ;

	m_listener->OnProgressDoneWrite( m_memory->size() ) ;
}

wstring CTMXWriter::make_tu( mem_engine::record_pointer rec, const wstring tmpl_text )
{
	// Assign variable values
	m_data[L"id"] = cpptempl::make_data(ulong2wstring( rec->get_id() ) ) ;

	m_data[L"creationdate"] = cpptempl::make_data(tmx_date( rec->get_created() ) ) ;
	m_data[L"changedate"] = cpptempl::make_data(tmx_date( rec->get_modified() ) ) ;

	// last usage
	m_data[L"lastusagedate"] = cpptempl::make_data(tmx_date( rec->get_modified() ) ) ;

	m_data[L"creationid"] = cpptempl::make_data(rec->get_creator() ) ;
	m_data[L"changeid"] = cpptempl::make_data(rec->get_modified_by() ) ;
	m_data[L"usagecount"] = cpptempl::make_data(ulong2wstring( rec->get_refcount() ) ) ;

	m_iLevel = 0 ;
	m_data[L"srcseg"] = cpptempl::make_data(get_segment( rec->get_source_rich() ) ) ;
	m_iLevel = 0 ;
	m_data[L"transseg"] = cpptempl::make_data(get_segment( rec->get_trans_rich() ) ) ;

	return cpptempl::parse(tmpl_text, m_data) ;
}
