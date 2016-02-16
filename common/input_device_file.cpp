#include "stdafx.h"
#include "input_device_file.h"
#include "stringconversions.h"

unsigned int InputDeviceFile::get_size( const CString filename )
{
	return file::file::size(filename) ;
}

void InputDeviceFile::ensure_file_exists( const CString file_name )
{
	const file::CPath path(file_name) ;
	if ( ! path.FileExists() ) // whoops -- file not there
	{
		CString msg ;
#ifdef FELIX_APPLICATION
		msg.FormatMessage( IDS_MSG_NO_FILE, file_name ) ;
#else // IDS_MSG_NO_FILE not defined
		msg = _T("File not found: ") + file_name ;
#endif
		THROW_WIN_EXCEPTION( msg ) ;
	}
}

wstring InputDeviceFile::get_creation_time( const CString location )
{
	FILETIME file_time ;
	bool was_open = m_target.is_open() ;
	if (! was_open)
	{
		m_target.open_always(location) ;
	}
	m_target.get_creation_time(&file_time); 
	if (! was_open)
	{
		m_target.close() ;
	}

	// Get created on from file creation date (but the memory could be older than
	// this if it has been copied, etc.)
	misc_wrappers::date created_date( file_time ) ;
	return string2wstring( created_date.get_date_time_string( ) );
}

file::BYTE_ORDER_MARK InputDeviceFile::get_file_bom( const CString filename )
{
	file::file import_file ;
	import_file.open_read(filename) ;
	const file::BYTE_ORDER_MARK bom = import_file.get_bom() ;
	import_file.close() ;
	return bom ;
}

wchar_t* InputDeviceFile::create_view_wchar( const CString filename )
{
	m_view.dispose_of_view() ;
	return (wchar_t*)m_view.create_view(filename) ;
}

char* InputDeviceFile::create_view_char( const CString filename )
{
	m_view.dispose_of_view() ;
	return (char *)m_view.create_view(filename) ;
}
const char* InputDeviceFile::create_view_const_char(const CString filename)
{
	m_view.dispose_of_view() ;
	return (const char*)m_view.create_view_readonly(filename) ;
}

bool InputDeviceFile::is_open()
{
	return m_target.is_open() ;
}

void InputDeviceFile::close()
{
	m_target.close() ;
}

void InputDeviceFile::open( const CString filename )
{
	m_target.close() ;
	m_target.open_read(filename) ;
}

bool InputDeviceFile::exists( const CString filename )
{
	return m_target.exists(filename) ;
}

void InputDeviceFile::delete_file(const CString filename) 
{
	if (! m_target.delete_file(filename))
	{
		THROW_WIN_EXCEPTION(_T("Failed to delete file ") + filename) ;
	}
}
