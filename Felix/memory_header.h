/*!
	@file memory_header.h
	@brief interface for the memory_header class.
	@date 2005/06/25
	Time: 14:48:55
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#include "StringEx.h"
#include "File.h"
#include "MiscWrappers.h"
#include "output_device.h"

namespace memory_engine
{

// TODO:
// Change properties to std::map of wstrings

/**
	@class memory_header  
	@brief Wrapper for memory header.
 */
class memory_header  
{
	wstring				m_creator ;
	wstring				m_field ;
	wstring				m_created_on ;
	wstring				m_source_language ;
	wstring				m_target_language ;
	wstring				m_client ;

	wstring				m_creation_tool ;
	wstring				m_creation_tool_version ;

	long					m_count ;
	bool					m_is_locked ;
	bool					m_is_memory ;

	wstring			m_modified_by ;
	wstring			m_modified_on ;
public:
	memory_header(const memory_header &rhs);
	memory_header();
	virtual ~memory_header();
	memory_header &operator=(const memory_header &rhs);
	void set_creator_to_current_user();

	wstring get_current_user();
	void read_header(LPCWSTR text) ;
	void write_header(OutputDevice *out_file) ;
	wstring get_header_field(wc_reader &reader, LPCWSTR tag) ;

	wstring get_creator() const ;
	wstring get_field() const ;
	wstring get_created_on() const ;

	wstring get_modified_by() const { return m_modified_by ; }
	wstring get_modified_on() const { return m_modified_on ; }

	wstring get_source_language() const ;
	wstring get_target_language() const ;
	wstring get_client() const ;
	wstring get_creation_tool() const ;
	wstring get_creation_tool_version() const ;
	long get_count() const ;
	bool is_locked() const ;
	bool is_memory() const ;

	void set_creator( const wstring &setting ) ;
	void set_field( const wstring &setting ) ;
	void set_created_on( const wstring &setting ) ;
	void set_source_language( const wstring &setting ) ;
	void set_target_language( const wstring &setting ) ;
	void set_client( const wstring &setting ) ;
	void set_count( const long setting ) ;
	void set_locked_on( ) ;
	void set_locked_off( ) ;
	void set_is_memory_on() ;
	void set_is_memory_off() ;
	void set_creation_tool( const wstring &setting );
	void set_creation_tool_version( const wstring &setting );

	void set_modified_by(const wstring modified_by) { m_modified_by = modified_by ; }
	void set_modified_on(const wstring modified_on) { m_modified_on = modified_on ; }
	void set_modified_now();
	void modified_by_current_user();

	void fill_value(wc_reader &reader, LPCWSTR tag, wstring &val);
	void fill_value(wc_reader &reader, LPCWSTR tag, bool &val);

private:
	void internal_copy( const memory_header &rhs );

	void set_created_on_to_local_time();
};

}