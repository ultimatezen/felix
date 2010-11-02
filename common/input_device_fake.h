#pragma once

class InputDeviceFake : public InputDevice
{
public:
	DECLARE_SENSING_VAR ;

	bool m_file_exists ;
	wstring m_filetime ;

	InputDeviceFake() : m_file_exists(true){}

	void ensure_file_exists( const CString & file_name )
	{
		if ( ! m_file_exists ) // whoops -- file not there
		{
			CString msg ;
			msg.FormatMessage( IDS_MSG_NO_FILE, file_name ) ;
			THROW_WIN_EXCEPTION( msg ) ;
		}
	}


	wstring get_creation_time(const CString location )
	{
		location ;
		SENSE("set_creation_time") ;
		SENSE(CT2A(location)) ;
		return m_filetime ;
	}
};