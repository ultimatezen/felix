/*!
	@file data_converter.h
	@brief Interface file for XML <=> record converter classes.
	@date 2005/06/25
	Time: 14:38:15
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "record.h"			// translation_record
#include "File.h"			// File I/O

#include "resource.h"
#include "tag_name_holder.h"
#include "output_device.h"

wstring TradosTag2AssistTag( const wstring node ) ;
wstring TradosDate2FelixDate(const wstring trados_date) ;

/**
	@class CXml2RecordConverter 
	@brief Converts xml nodes into memory records (for reading files).
 */
class CXml2RecordConverter 
{
	VISIBLE_TO_TESTS

	typedef textstream_reader< wchar_t >				reader_type ;
	typedef textstream_reader< wchar_t >::bookmark_type bookmark_type ;

	reader_type m_parser ;

	mem_engine::record_pointer	m_record ;

public:
	const wstring strip_cdata( const wstring data ) const;
	// constructors
	CXml2RecordConverter( ) ;
	CXml2RecordConverter( const CXml2RecordConverter &cpy ) ;
	// assignment
	CXml2RecordConverter &operator =( const CXml2RecordConverter &cpy ) ;

	// Load the xml values into a translation record
	mem_engine::record_pointer convert_from_xml_node( LPCWSTR node ) ;

	// The spillover not hard wired into the record structure
	void load_extra(const wstring tag ) ;
	void load_source( ) ;
	void load_trans( ) ;
	void load_context( ) ; 
	void load_date_created( ) ;
	void load_last_modified( ) ;
	void load_reliability( ) ;
	void load_refcount( ) ;
	void load_validated( ) ;

	bool get_validated_value();

	size_t SizeNode( const _bstr_t &EndTag ) ;

	const wstring strip_if_needed( const wstring record_text) const;
	wstring PlainTextNode( const _bstr_t &EndTag ) ;
	void load_richtext_node( const _bstr_t &EndTag,
				bookmark_type &bookmark_start,
				bookmark_type &bookmark_end ) ;
	// get the text from the children of this node
} ;


/**
	@class CRecord2XmlConverter 
	@brief Convert a record to xml format (for output to file).
 */
class CRecord2XmlConverter
{
	wstring				m_doc ;
	wstring				m_node ;
	mem_engine::record_pointer		m_record ;
	OutputDevice		*m_file ;

public:

	std::vector<string> m_from_strings ;
	std::vector<string> m_to_strings ;

	// constructors
	CRecord2XmlConverter( OutputDevice *xml_file ) ;
	CRecord2XmlConverter( const CRecord2XmlConverter &cpy ) ;
	// assignment
	CRecord2XmlConverter &operator =( const CRecord2XmlConverter &cpy ) ;

	void init_char_conversion();
	~CRecord2XmlConverter() {}

	// convert_from_xml_node
	bool convert_from_record( const mem_engine::record_pointer rec ) ;

	bool convert_source()  ;
	bool convert_trans()  ;
	bool convert_context()  ; 
	bool convert_created()  ;
	bool convert_modified()  ;
	bool convert_rest()  ;
	bool convert_cdata_node( const string &tag, const string &end_tag, const wstring text ) ;
	bool convert_text_node( const string &tag, const string &end_tag, const string &text ) ;

	bool convert_reliability(void) ;
	bool convert_refcount(void) ;
	bool convert_validated(void) ;
} ;


