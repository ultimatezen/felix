// memory_header.cpp: implementation of the memory_header class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "memory_header.h"
#include "UserName.h"
#include "Felix_properties.h"
#include "record.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

namespace mem_engine
{


	void memory_header::set_created_on_to_local_time()
	{
		misc_wrappers::date created_date ;
		created_date.set_to_local_time() ;

		m_created_on = string2wstring( created_date.get_date_time_string( ) ) ;
	}

	memory_header::memory_header(const memory_header &rhs) :
	m_count(-1),
		m_is_locked( false ),
		m_is_memory( true ),
		m_creation_tool(L"Felix"),
		m_creation_tool_version(string2wstring(VERSION)),
		m_creator( CUserName().as_wstring() )
	{
		set_created_on_to_local_time() ;
		get_current_user() ;
		internal_copy( rhs ) ;
	}

	memory_header::~memory_header()
	{

	}

	void memory_header::internal_copy(const memory_header &rhs)
	{
		m_creator					= rhs.m_creator ;
		m_field						= rhs.m_field ;
		m_created_on				= rhs.m_created_on ;
		m_source_language			= rhs.m_source_language ;
		m_target_language			= rhs.m_target_language ;
		m_client					= rhs.m_client ;
		m_count						= rhs.m_count ;
		m_is_locked					= rhs.m_is_locked ;
		m_is_memory					= rhs.m_is_memory ;
		m_creation_tool				= rhs.m_creation_tool ;
		m_creation_tool_version		= rhs.m_creation_tool_version ; 

		m_modified_on				= rhs.m_modified_on ;
		m_modified_by				= rhs.m_modified_by ;
	}

	// getters

	wstring memory_header::get_creator() const  
	{
		return m_creator ;
	}
	wstring memory_header::get_field() const 
	{
		return m_field ;
	}
	wstring memory_header::get_created_on() const 
	{
		return m_created_on ;
	}
	wstring memory_header::get_source_language() const 
	{
		return m_source_language ;
	}
	wstring memory_header::get_target_language() const 
	{
		return m_target_language ;
	}
	wstring memory_header::get_client() const 
	{
		return m_client ;
	}
	long memory_header::get_count() const 
	{
		return m_count ;
	}
	bool memory_header::is_locked() const 
	{
		return m_is_locked ;
	}
	bool memory_header::is_memory() const 
	{
		return m_is_memory ;
	}

	wstring memory_header::get_creation_tool() const 
	{
		return m_creation_tool ;
	}

	wstring memory_header::get_creation_tool_version() const 
	{
		return m_creation_tool_version ;
	}

	// setters

	void memory_header::set_creator( const wstring setting ) 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_creator = setting ;
	}
	void memory_header::set_field( const wstring setting ) 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_field = setting ;
	}
	void memory_header::set_created_on( const wstring setting ) 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_created_on = setting ;
	}
	void memory_header::set_source_language( const wstring setting ) 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_source_language = setting ;
	}
	void memory_header::set_target_language( const wstring setting ) 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_target_language = setting ;
	}
	void memory_header::set_client( const wstring setting ) 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_client = setting ;
	}
	void memory_header::set_count( const long setting ) 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_count = setting ;
	}
	void memory_header::set_locked_on( ) 
	{
		m_is_locked = true ;
	}
	void memory_header::set_locked_off( ) 
	{
		m_is_locked = false ;
	}
	void memory_header::set_is_memory_on() 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_is_memory = true ;
	}
	void memory_header::set_is_memory_off() 
	{
		if ( is_locked() )
		{
			return ;
		}
		m_is_memory = false ;
	}

	void memory_header::set_creation_tool( const wstring setting )
	{
		m_creation_tool = setting ;
	}

	void memory_header::set_creation_tool_version( const wstring setting )
	{
		m_creation_tool_version = setting ;
	}

	// assignment

	memory_header& memory_header::operator =(const memory_header &rhs)
	{
		internal_copy( rhs ) ;
		return *this ;
	}

	// parsing

	void memory_header::read_header(LPCWSTR text)
	{
		ATLASSERT( text != NULL ) ;

		wc_reader reader ;
		reader.set_buffer( text	) ;

		fill_value(reader, L"creator", m_creator) ;
		fill_value(reader, L"created_on", m_created_on) ;

		fill_value(reader, L"modified_by", m_modified_by) ;
		fill_value(reader, L"modified_on", m_modified_on) ;

		fill_value(reader, L"field", m_field) ;

		fill_value(reader, L"source_language", m_source_language) ;
		fill_value(reader, L"target_language", m_target_language) ;

		fill_value(reader, L"client", m_client) ;
		fill_value(reader, L"locked", m_is_locked) ;
		fill_value(reader, L"is_memory", m_is_memory) ;
		fill_value(reader, L"creation_tool", m_creation_tool) ;
		fill_value(reader, L"creation_tool_version", m_creation_tool_version) ;

		// num_records
		wstring header_field = get_header_field( reader, L"num_records" ) ;

		if ( header_field.empty() == false )
		{
			m_count = string2ulong( header_field ) ;
		}

	}

	void memory_header::fill_value( wc_reader &reader, LPCWSTR tag, wstring &val )
	{
		wstring header_field = get_header_field( reader, tag ) ;

		if ( header_field.empty() == false )
		{
			val = header_field;
		}
	}
	void memory_header::fill_value( wc_reader &reader, LPCWSTR tag, bool &val )
	{
		wstring header_field = get_header_field( reader, tag ) ;

		if ( header_field.empty() == false )
		{
			val = ( 0 == _wcsicmp(header_field.c_str(), L"true" ) ) ;
		}
	}


	wstring memory_header::get_header_field(wc_reader &reader, LPCWSTR tag)
	{
		wstring start_tag ;
		start_tag << L"<" << tag << L">" ;

		wstring end_tag ;
		end_tag << L"</" << tag << L">" ;

		wstring header_field ;

		reader.find( start_tag.c_str(), true ) ;
		if( reader.empty() == false )
		{
			wc_reader::bookmark_type start = reader.get_current_pos() ;
			reader.find( end_tag.c_str(), false ) ;
			wc_reader::bookmark_type end = reader.get_current_pos() ;

			header_field.append( start, end ) ;
		}

		reader.rewind() ;

		return header_field ;
	}
	// writing

	void memory_header::write_header(OutputDevice *out_file)
	{
		out_file->write( "<head>\r\n" ) ;

		string line ;
		line.reserve( 256 ) ;

		if ( m_creator.empty() == false )
		{
			line = "<creator>" + string2string( m_creator, CP_UTF8 ) + "</creator>\r\n" ;
			out_file->write( line ) ;
		}
		if ( m_created_on.empty() == false )
		{
			line = "<created_on>" + string2string( m_created_on, CP_UTF8 ) + "</created_on>\r\n" ;
			out_file->write( line ) ;
		}

		if ( m_modified_by.empty() == false )
		{
			line = "<modified_by>" + string2string( m_modified_by, CP_UTF8 ) + "</modified_by>\r\n" ;
			out_file->write( line ) ;
		}
		if ( m_modified_on.empty() == false )
		{
			line = "<modified_on>" + string2string( m_modified_on, CP_UTF8 ) + "</modified_on>\r\n" ;
			out_file->write( line ) ;
		}


		if ( m_field.empty() == false )
		{
			line = "<field>" + string2string( m_field, CP_UTF8 ) + "</field>\r\n" ;
			out_file->write( line ) ;
		}
		if ( m_source_language.empty() == false )
		{
			line = "<source_language>" + string2string( m_source_language, CP_UTF8 )  + "</source_language>\r\n" ;
			out_file->write( line ) ;
		}
		if ( m_target_language.empty() == false )
		{
			line = "<target_language>" + string2string( m_target_language, CP_UTF8 ) + "</target_language>\r\n" ;
			out_file->write( line ) ;
		}
		if ( m_client.empty() == false )
		{
			line = "<client>" + string2string( m_client, CP_UTF8 ) + "</client>\r\n" ;
			out_file->write( line ) ;
		}

		if ( m_creation_tool.empty() == false )
		{
			line = "<creation_tool>" + string2string( m_creation_tool, CP_UTF8 ) + "</creation_tool>\r\n" ;
			out_file->write( line ) ;
		}
		if ( m_creation_tool_version.empty() == false )
		{
			line = "<creation_tool_version>" + string2string( m_creation_tool_version, CP_UTF8 ) + "</creation_tool_version>\r\n" ;
			out_file->write( line ) ;
		}

		line = "<num_records>" + ulong2string( get_count() ) + "</num_records>\r\n" ;
		out_file->write( line ) ;

		line = "<locked>" + bool2string( is_locked() ) + "</locked>\r\n" ;
		out_file->write( line ) ;

		line = "<is_memory>" + bool2string( is_memory() ) + "</is_memory>\r\n" ;
		out_file->write( line ) ;

		out_file->write( "</head>\r\n\r\n" )  ;
	}


	void memory_header::set_creator_to_current_user()
	{
		wstring creator = get_current_user();
		set_creator( creator ) ;
	}

	void memory_header::set_modified_now()
	{
		misc_wrappers::date modified_date ;
		modified_date.set_to_local_time() ;

		m_modified_on = string2wstring( modified_date.get_date_time_string( ) ) ;
	}

	wstring memory_header::get_current_user()
	{
		app_props::properties_general props ;
		props.read_from_registry() ;
		wstring user_name = CT2W(props.m_data.m_user_name) ;
		set_record_username(user_name) ;
		return user_name ;
	}

	void memory_header::modified_by_current_user()
	{
		m_modified_by = this->get_current_user() ;
	}

	void memory_header::set_is_memory( bool setting )
	{
		m_is_memory = setting ;
	}

	void memory_header::set_is_locked( bool setting )
	{
		m_is_locked = setting ;
	}

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	memory_header::memory_header() :
			m_count(-1),
			m_is_locked( false ),
			m_is_memory( true ),
			m_creation_tool(L"Felix"),
			m_creation_tool_version(string2wstring(VERSION))
	{
		set_created_on_to_local_time() ;
		m_modified_on = m_created_on ;

		// refresh user name 
		set_creator_to_current_user() ;

		m_modified_by = m_creator ;
	}

}