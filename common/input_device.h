#pragma once

#include "file.h"
#include "miscwrappers.h"
#include "Exceptions.h"

class InputDevice
{
public:
	virtual wstring get_creation_time(const CString location) = 0 ;
	// Throw an exception if the file doesn't exist.
	virtual void ensure_file_exists( const CString & file_name ) = 0 ;
};

class InputDeviceFile : public InputDevice
{
public:
	file::file m_target ;
	void ensure_file_exists( const CString & file_name ) 
	{
		const file::CPath path(file_name) ;
		if ( ! path.FileExists() ) // whoops -- file not there
		{
			CString msg ;
			msg.FormatMessage( IDS_MSG_NO_FILE, file_name ) ;
			THROW_WIN_EXCEPTION( msg ) ;
		}
	}
	wstring get_creation_time(const CString location)
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
};