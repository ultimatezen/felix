// trados_data_converter.cpp: implementation of the trados_data_converter class.
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "miscwrappers.h"		// date
#include "data_converter.h"
#include "DebugUtilities.h"	// debug macros etc.
#include "record_local.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

using namespace mem_engine ;
using namespace except ;

// tags

const static string record_tag_narrow			= "<record>\n" ; 
const static string source_tag_narrow			= "  <source>" ; 
const static string trans_tag_narrow			= "  <trans>" ;
const static string date_created_tag_narrow	= "  <date_created>" ;	
const static string last_modified_tag_narrow	= "  <last_modified>" ;	
const static string reliability_tag_narrow	= "  <reliability>" ;		
const static string validated_tag_narrow		= "  <validated>" ;		
const static string context_tag_narrow		= "  <context>" ;
const static string refcount_tag_narrow		= "  <ref_count>" ;
const static string creator_tag_narrow		= "  <creator>" ;
const static string modified_by_tag_narrow	= "  <modified_by>" ;
const static string id_tag_narrow				= "  <id>" ;

// end tags		
const static _bstr_t record_tag_end			= L"</record>\n" ; 
const static _bstr_t source_tag_end			= L"</source>\n" ; 
const static _bstr_t trans_tag_end			= L"</trans>\n" ;
const static _bstr_t date_created_tag_end	= L"</date_created>\n" ;	
const static _bstr_t last_modified_tag_end	= L"</last_modified>\n" ;	
const static _bstr_t reliability_tag_end		= L"</reliability>\n" ;		
const static _bstr_t validated_tag_end		= L"</validated>\n" ;		
const static _bstr_t context_tag_end			= L"</context>\n" ;		
const static _bstr_t refcount_tag_end		= L"</ref_count>\n" ;

const static string record_tag_end_narrow				= "</record>\n" ; 
const static string source_tag_end_narrow				= "</source>\n" ; 
const static string trans_tag_end_narrow				= "</trans>\n" ;
const static string date_created_tag_end_narrow		= "</date_created>\n" ;	
const static string last_modified_tag_end_narrow		= "</last_modified>\n" ;	
const static string reliability_tag_end_narrow		= "</reliability>\n" ;		
const static string validated_tag_end_narrow			= "</validated>\n" ;		
const static string context_tag_end_narrow			= "</context>\n" ;		
const static string refcount_tag_end_narrow			= "</ref_count>\n" ;
const static string creator_tag_end_narrow			= "</creator>\n" ;
const static string modified_by_tag_end_narrow		= "</modified_by>\n" ;
const static string id_tag_end_narrow = "</id>\n" ;


/************************************************************************/
/* helper functions                                                     */
/************************************************************************/

wstring TradosTag2AssistTag( const wstring &node )
{
	if ( node == L"Crd" )
	{
		return L"date_created"  ;
	}
	else if ( node == L"ChD" )
	{
		return L"last_modified"  ;
	}
	else if ( node == L"Seg source" )
	{
		return L"source" ;
	}
	else if ( node == L"Seg trans" )
	{
		return L"trans" ;
	}

	return node ;

}

wstring TradosDate2FelixDate(const wstring &trados_date)
{
	misc_wrappers::date felix_date ;

	// 30032003, 14:36:00
	ATLASSERT( trados_date.length() == 18 ) ;
	wstring date_unit ;
	date_unit += trados_date[0] ; // 30
	date_unit += trados_date[1] ;
	felix_date.set_day( date_unit ) ;
	date_unit.erase() ;

	date_unit += trados_date[2] ; // 03
	date_unit += trados_date[3] ;
	felix_date.set_month( date_unit ) ;
	date_unit.erase() ;

	date_unit += trados_date[4] ; // 2003
	date_unit += trados_date[5] ;
	date_unit += trados_date[6] ;
	date_unit += trados_date[7] ;
	felix_date.set_year( date_unit ) ;
	date_unit.erase() ;
	// ,
	// <space>

	date_unit += trados_date[10] ; // 14
	date_unit += trados_date[11] ;
	felix_date.set_hour( date_unit ) ;
	date_unit.erase() ;
	// :

	date_unit += trados_date[13] ; // 36
	date_unit += trados_date[14] ;
	felix_date.set_minute( date_unit ) ;
	date_unit.erase() ;
	// :
	date_unit += trados_date[16] ; // 00
	date_unit += trados_date[17] ;
	felix_date.set_second( date_unit ) ;
	date_unit.erase() ;

	return string2wstring(felix_date.get_date_time_string());
}

// ==============================
// CXml2RecordConverter
// ==============================

// constructors
CXml2RecordConverter::CXml2RecordConverter( ) 
{
	m_record = mem_engine::record_pointer(new record_local()) ;
}
CXml2RecordConverter::CXml2RecordConverter( const CXml2RecordConverter &cpy ) : 
	m_record( cpy.m_record )
{
}


// Function name	: CXml2RecordConverter::operator =
// Description	    : 
// Return type		: CXml2RecordConverter &CXml2RecordConverter::operator 
// Argument         : const CXml2RecordConverter &cpy
CXml2RecordConverter &CXml2RecordConverter::operator =( const CXml2RecordConverter &cpy )
{
	m_record	= ( cpy.m_record ) ;
	return *this ;
}


// Function name	: &CXml2RecordConverter::convert_from_xml_node
// Description	    : 
// Return type		: translation_record 
// Argument         : const wstring &node
record_pointer CXml2RecordConverter::convert_from_xml_node( LPCWSTR node )
{
	m_parser.set_buffer( node ) ;

	// clear the record
	m_record = record_pointer((new record_local())) ;

	m_parser.find(L"<", true ) ;
	wstring tag = m_parser.getline(L'>', true ) ;

	tag_name_holder &tags = tag_name_holder::instance() ;
	while ( m_parser.empty() == false )
	{
		// load appropriate value
		if ( tag ==  tags.source_tag )
			load_source( ) ;
		else if ( tag == tags.trans_tag )
			load_trans( ) ;
		else if ( tag == tags.date_created_tag )
			load_date_created( ) ;
		else if ( tag == tags.last_modified_tag )
			load_last_modified( ) ;
		else if ( tag == tags.reliability_tag )
			load_reliability( ) ;
		else if ( tag == tags.validated_tag )
			load_validated( ) ;
		else if ( tag == tags.context_tag )
			load_context( ) ;
		else if ( tag == tags.refcount_tag )
			load_refcount( ) ;
		else load_extra( tag ) ;

		// next child
		m_parser.find( L"<", true ) ;
		tag = m_parser.getline(L'>', true ) ;
	}

	return m_record ;
}



/**
 * @method:    load_source
 * FullName:  CXml2RecordConverter::load_source
 * Access:    public 
 * @retval   void
 * Qualifier:
 */
void CXml2RecordConverter::load_source( )
{
	tag_name_holder &tags = tag_name_holder::instance() ;

	bookmark_type bookmark_start, bookmark_end ;

	load_richtext_node( tags.source_tag_end, bookmark_start, bookmark_end ) ;

	if ( bookmark_start == bookmark_end )
	{
		return ;
	}

	wstring source(bookmark_start, bookmark_end) ;
	str::replace_all(source, L"&#32;", L" ") ;
	m_record->set_source( source ) ;

}

/**
 * @method:    load_trans
 * FullName:  CXml2RecordConverter::load_trans
 * Access:    public 
 * @retval   void
 * Qualifier:
 */
void CXml2RecordConverter::load_trans( ) 
{		
	tag_name_holder &tags = tag_name_holder::instance() ;

	bookmark_type bookmark_start, bookmark_end ;

	load_richtext_node( tags.trans_tag_end, bookmark_start, bookmark_end ) ;

	if ( bookmark_start == bookmark_end )
	{
		return ;
	}

	// set the tag and text
	wstring trans(bookmark_start, bookmark_end) ;
	str::replace_all(trans, L"&#32;", L" ") ;
	m_record->set_trans( trans ) ;
}

// Function name	: CXml2RecordConverter::load_context
// Description	    : 
// Return type		: void 
// Argument         : const wstring &node
void CXml2RecordConverter::load_context( ) 
{
	tag_name_holder &tags = tag_name_holder::instance() ;

	bookmark_type bookmark_start, bookmark_end ;

	load_richtext_node( tags.context_tag_end, bookmark_start, bookmark_end ) ;

	if ( bookmark_start == bookmark_end )
	{
		return ;
	}

	// set the tag and text
	wstring context(bookmark_start, bookmark_end) ;
	str::replace_all(context, L"&#32;", L" ") ;
	m_record->set_context( context ) ;
}

/**
 * load_richtext_node
 */
void CXml2RecordConverter::load_richtext_node( const _bstr_t &EndTag,
										bookmark_type &bookmark_start,
										bookmark_type &bookmark_end )
{
	ATLASSERT( 0 != EndTag.length() ) ;

	reader_type::bookmark_type bookmark = m_parser.get_current_pos() ;

	bool has_cdata(true) ;
	// start of record
	if ( ! m_parser.find( L"<![CDATA[", true ) )
	{
		has_cdata = false ;
		m_parser.set_pos( bookmark ) ;
	}

	bookmark_start = m_parser.get_current_pos() ;

	// end of record
	if ( has_cdata )
	{
		ATLVERIFY( m_parser.find( L"]]>", false ) ) ;
	}
	else
	{
		ATLVERIFY( m_parser.find( EndTag, false ) ) ;
	}

	bookmark_end = m_parser.get_current_pos() ;
	// text for this record

	// parse to end of tag
	ATLVERIFY( m_parser.find( EndTag, true ) ) ;
	ATLASSERT( bookmark_start != bookmark_end ) ;
}


// Function name	: CXml2RecordConverter::load_date_created
// Description	    : 
// Return type		: void 
// Argument         : const wstring &node
void CXml2RecordConverter::load_date_created( ) 
{
	tag_name_holder &tags = tag_name_holder::instance() ;

	m_record->set_created( PlainTextNode( tags.date_created_tag_end ) ) ;
}


// Function name	: CXml2RecordConverter::load_last_modified
// Description	    : 
// Return type		: void 
// Argument         : const wstring &node
void CXml2RecordConverter::load_last_modified( ) 
{
	tag_name_holder &tags = tag_name_holder::instance() ;

	m_record->set_modified( PlainTextNode( tags.last_modified_tag_end ) ) ;
}

/**
* @method:    load_extra
* FullName:  CXml2RecordConverter::load_extra
* Access:    public 
* @retval   void
* Qualifier:
* @param const wstring & tag
*/
void CXml2RecordConverter::load_extra(const wstring & tag)
{
	ATLASSERT( tag.empty() == false ) ;

	if ( tag == L"record" )
	{
		throw except::CException( IDS_CORRUPT_FILE ) ;
	}

	const _bstr_t end_tag = L"</" + string2BSTR(tag) + L">" ;

	// set the tag and text
	if (tag == L"id")
	{
		m_record->set_id(boost::lexical_cast<size_t>(PlainTextNode(end_tag))) ;
	}
	else if (tag == L"creator")
	{
		m_record->set_creator(PlainTextNode(end_tag)) ;
	}
	else if (tag == L"modified_by")
	{
		m_record->set_modified_by(PlainTextNode(end_tag)) ;
	}
	else
	{
		m_record->set_item(tag, PlainTextNode(end_tag)) ;
	}

}

wstring CXml2RecordConverter::PlainTextNode( const _bstr_t &EndTag )
{
	// start of record
	const bookmark_type bookmark_start = m_parser.get_current_pos() ;
	ATLASSERT( bookmark_start != NULL ) ;
	// end tag
	// end of record
	ATLVERIFY( m_parser.find( EndTag, false ) ) ;
	const bookmark_type bookmark_end = m_parser.get_current_pos() ;
	ATLASSERT( bookmark_end != NULL ) ;
	// text for this record
	const wstring record_text( bookmark_start, bookmark_end ) ;

	// parse to end of tag
	ATLVERIFY( m_parser.find( EndTag, true ) ) ;

	ATLASSERT( record_text.empty() == false ) ;

	// is it a CDATA node?
	return strip_if_needed(record_text) ;
}


// Function name	: CXml2RecordConverter::load_reliability
// Description	    : 
// Return type		: void 
// Argument         : const wstring &node
void CXml2RecordConverter::load_reliability( ) 
{
	tag_name_holder &tags = tag_name_holder::instance() ;

	m_record->set_reliability( SizeNode( tags.reliability_tag_end) ) ;
}

size_t CXml2RecordConverter::SizeNode( const _bstr_t &EndTag )
{
	ATLASSERT( 0 != EndTag.length() ) ;

	// start of record
	const bookmark_type bookmark_start = m_parser.get_current_pos() ;
	ATLASSERT( bookmark_start != NULL ) ;
	// end of record
	ATLVERIFY( m_parser.find( EndTag, false ) ) ;
	const bookmark_type bookmark_end = m_parser.get_current_pos() ;
	ATLASSERT( bookmark_end != NULL ) ;
	// text for this record
	const wstring record_text( bookmark_start, bookmark_end ) ;

	// parse to end of tag
	ATLVERIFY( m_parser.find( EndTag, true ) ) ;

	return static_cast< size_t >( string2ulong( strip_if_needed(record_text) ) )  ;
}


// Function name	: CXml2RecordConverter::load_refcount
// Description	    : 
// Return type		: void 
// Argument         : const wstring &node
void CXml2RecordConverter::load_refcount( ) 
{
	tag_name_holder &tags = tag_name_holder::instance() ;

	m_record->set_refcount( SizeNode( tags.refcount_tag_end) ) ;
}

// Function name	: CXml2RecordConverter::load_validated
// Description	    : 
// Return type		: void 
// Argument         : const wstring &node
void CXml2RecordConverter::load_validated( ) 
{
	if ( get_validated_value() )	
	{
		m_record->set_validated_on() ;
	}
	else
	{
		m_record->set_validated_off() ;
	}

}


const wstring CXml2RecordConverter::strip_cdata(const wstring &data) const
{
	// get rid of the CDATA tags
	reader_type local_reader ;
	local_reader.set_buffer( data.c_str() ) ;

	local_reader.find( L"<![CDATA[", true ) ;
	const bookmark_type bookmark_start = local_reader.get_current_pos() ;

	local_reader.find( L"]]>", false ) ;
	const bookmark_type bookmark_end = local_reader.get_current_pos() ;

	const wstring text( bookmark_start, bookmark_end ) ;

	ATLASSERT( text.empty() == false ) ;

	return text ;
}

const wstring CXml2RecordConverter::strip_if_needed( const wstring &record_text ) const
{
	if ( record_text.find( L"<![CDATA[" ) != wstring::npos )
	{
		// get rid of the CDATA tags
		return strip_cdata( record_text ) ;
	}
	else
	{
		return record_text ;
	}
}

bool CXml2RecordConverter::get_validated_value()
{
	tag_name_holder &tags = tag_name_holder::instance() ;

	// start of record
	const bookmark_type bookmark_start = m_parser.get_current_pos() ;
	// end of record
	m_parser.find( tags.validated_tag_end, false ) ;
	const bookmark_type bookmark_end = m_parser.get_current_pos() ;
	// text for this record
	const wstring record_text( bookmark_start, bookmark_end ) ;

	// parse to end of tag
	ATLVERIFY(m_parser.find( tags.validated_tag_end, true )) ;

	const wstring truth_text = boost::to_lower_copy(strip_if_needed(record_text)) ;
	if ( truth_text == L"true" || truth_text == L"yes" )	
	{
		return true ;
	}
	return false ;
}
// ========================
// CRecord2XmlConverter methods
// ========================

// constructors
CRecord2XmlConverter::CRecord2XmlConverter( OutputDevice *xml_file )
: m_file( xml_file )
{
	m_record = mem_engine::record_pointer(new record_local()) ;

	init_char_conversion();
}
CRecord2XmlConverter::CRecord2XmlConverter( const CRecord2XmlConverter &cpy )
	: m_doc( cpy.m_doc ),
	m_node( cpy.m_node ),
	m_record( cpy.m_record ),
	m_file( cpy.m_file )
{
	init_char_conversion();
}

// assignment operator
CRecord2XmlConverter &CRecord2XmlConverter::operator =( const CRecord2XmlConverter &cpy )
{
	m_doc		= cpy.m_doc ;
	m_node		= cpy.m_node ;
	m_record	= cpy.m_record ;
	m_file      = cpy.m_file ;

	return *this ;
}


// write the record to the file
bool CRecord2XmlConverter::convert_from_record( const record_pointer rec ) 
{
	try
	{
		m_file->write( record_tag_narrow ) ;

		m_record = rec ;
		
		convert_text_node(id_tag_narrow, 
						id_tag_end_narrow, 
						boost::lexical_cast<string>(m_record->get_id())) ;

		convert_source() ;
		convert_trans() ;
		convert_context() ;
		convert_created() ;
		convert_modified() ;
		convert_reliability() ;
		convert_validated() ;
		convert_refcount() ;

		convert_text_node
			( 
			creator_tag_narrow, 
			creator_tag_end_narrow, 
			string2string(m_record->get_creator(), CP_UTF8)  
			) ;
		convert_text_node
			( 
			modified_by_tag_narrow, 
			modified_by_tag_end_narrow, 
			string2string(m_record->get_modified_by(), CP_UTF8)  
			) ;


		convert_rest() ;

		m_file->write( record_tag_end_narrow ) ;

	}
	catch ( CException &e ) 
	{
		e.add_to_message( _T("Failed to write record to file!") ) ;
		throw except::CException( e ) ;
	}

	return true ;
}


// Function name	: CRecord2XmlConverter::convert_source
bool CRecord2XmlConverter::convert_source()
{
	return convert_cdata_node( source_tag_narrow, 
							   source_tag_end_narrow, 
							   m_record->get_source_rich() ) ;
}

// Function name	: CRecord2XmlConverter::convert_trans
bool CRecord2XmlConverter::convert_trans() 
{
	return convert_cdata_node( trans_tag_narrow, 
							   trans_tag_end_narrow, 
							   m_record->get_trans_rich() ) ;
}


// Function name	: CRecord2XmlConverter::convert_context
bool CRecord2XmlConverter::convert_context() 
{
	if( m_record->get_context_rich().empty() ) 
	{
		return true ;
	}

	return convert_cdata_node( context_tag_narrow,
							   context_tag_end_narrow, 
							    m_record->get_context_rich() ) ;
}

// Function name	: CRecord2XmlConverter::convert_created
bool CRecord2XmlConverter::convert_created()
{
	return convert_text_node
	( 
		date_created_tag_narrow, 
		date_created_tag_end_narrow, 
		string2string( m_record->get_created().get_date_time_string() ) 
	) ;
}

// Function name	: CRecord2XmlConverter::convert_modified
bool CRecord2XmlConverter::convert_modified() 
{
	return convert_text_node
	( 
		last_modified_tag_narrow, 
		last_modified_tag_end_narrow, 
		string2string( m_record->get_modified().get_date_time_string() ) 
	) ;
}

bool CRecord2XmlConverter::convert_rest() 
{

#ifdef _DEBUG
	tag_name_holder &tags = tag_name_holder::instance() ;
#endif

	translation_record::record_data_iterator pos ;
	// add each of the remaining properties as CDATA sections
	// first: tag
	// second: value
	for ( pos = m_record->data_begin() ; pos != m_record->data_end() ; ++pos )
	{
		ATLASSERT( pos->first != tags.source_tag ) ;
		ATLASSERT( pos->first != tags.trans_tag ) ;
		ATLASSERT( pos->first != tags.context_tag ) ;
		ATLASSERT( pos->first != tags.date_created_tag ) ;
		ATLASSERT( pos->first != tags.last_modified_tag ) ;
		ATLASSERT ( pos->first != tags.reliability_tag ) ;
		ATLASSERT ( pos->first != tags.refcount_id ) ; 
		ATLASSERT ( pos->first != tags.validated_tag ) ; 

		const string tag = string2string( pos->first ) ;
		const string start_tag = "<" + tag + ">" ;
		// we don't want the attributes showing up in the end tag
		const size_t index = tag.find_first_of( ' ' ) ;
		const string end_tag = "</" + tag.substr( 0, index ) + ">" ;

		convert_cdata_node( start_tag, 
						    end_tag, 
							str::replace( pos->second, L"<", L"&lt;" )) ;
	}
	return true ;
}


// Function name	: CRecord2XmlConverter::convert_key_val_pair
bool CRecord2XmlConverter::convert_cdata_node( const string &tag, const string &end_tag, const wstring &text )
{

#define BEGIN_CDATA_SECTION "<![CDATA[" 
#define END_CDATA_SECTION "]]>" 

	string vertical_tab ;

	CStringA to ;

	// <SomeTag><!CDATA[SomeText]]></SomeTag>
	try
	{
		m_file->write( tag ) ;

		ATLASSERT( strlen( BEGIN_CDATA_SECTION ) == 9 ) ;
		m_file->write( BEGIN_CDATA_SECTION, 9 )  ;

		string atext = string2string( text, CP_UTF8 ) ;

		for (size_t i = 0 ; i < m_from_strings.size() ; ++i )
		{
			str::replace_all(atext, 
				m_from_strings[i], 
				m_to_strings[i]) ;
		}

		m_file->write( atext ) ;

		ATLASSERT( strlen( END_CDATA_SECTION ) == 3 ) ;
		m_file->write( END_CDATA_SECTION, 3 ) ;

		m_file->write( end_tag ) ;
	}
	catch ( CException &e ) 
	{
		e.add_to_message( "Failed to write CDATA node!" ) ;
		throw except::CException( e ) ;
	}

	return true ;

}

bool CRecord2XmlConverter::convert_text_node( const string &tag, const string &end_tag, const string &text )
{
	try
	{
		m_file->write( tag ) ;
		m_file->write( text ) ;
		m_file->write( end_tag ) ;
	}
	catch ( CException &e ) 
	{
		e.add_to_message( "Failed to write text node!" ) ;
		throw except::CException( e ) ;
	}

	return true ;
}

bool CRecord2XmlConverter::convert_validated(void)
{
	return convert_text_node
		( 
		validated_tag_narrow, 
		validated_tag_end_narrow, 
		bool2string( m_record->is_validated() ) 
		) ;
}

bool CRecord2XmlConverter::convert_reliability(void)
{
	return convert_text_node
		( 
		reliability_tag_narrow, 
		reliability_tag_end_narrow, 
		ulong2string( static_cast< unsigned long >( m_record->get_reliability() ) ) 
		) ;
}

bool CRecord2XmlConverter::convert_refcount(void)
{
	return convert_text_node
		( 
		refcount_tag_narrow, 
		refcount_tag_end_narrow, 
		ulong2string( static_cast< unsigned long >( m_record->get_refcount() ) ) 
		) ;
}

void CRecord2XmlConverter::init_char_conversion()
{
	char from[2] = {0} ;
	for (int i = 1 ; i < 32 ; ++i )
	{
		from[0] = char(i) ;
		m_from_strings.push_back(from) ;
		m_to_strings.push_back((format("&#%1%;") % i).str()) ;
	}
	from[0] = char(127) ;
	m_from_strings.push_back(from) ;
	m_to_strings.push_back("&#127;") ;
}
