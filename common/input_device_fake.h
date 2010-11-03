#pragma once
#include <boost/shared_array.hpp>

class InputDeviceFake : public InputDevice
{
public:
	DECLARE_SENSING_VAR ;

	bool m_file_exists ;
	wstring m_filetime ;
	unsigned int m_size ;
	file::file::BYTE_ORDER_MARK m_bom ;
	boost::shared_array<char> m_view_char ;
	boost::shared_array<wchar_t> m_view_wchar ;
	bool m_is_open ;

	InputDeviceFake() : 
		m_file_exists(true), 
		m_size(0),
		m_bom(file::file::UNKNOWN_BOM),
		m_is_open(true)
		{}

	file::file::BYTE_ORDER_MARK get_file_bom( const CString filename )
	{
		filename ;
		SENSE("get_file_bom") ;
		SENSE(CT2A(filename)) ;
		return m_bom ;
	}

	unsigned int get_size(const CString filename)
	{
		return m_size ;
	}
	void ensure_file_exists( const CString filename )
	{
		if ( ! m_file_exists ) // whoops -- file not there
		{
			CString msg ;
			msg.FormatMessage( IDS_MSG_NO_FILE, filename ) ;
			THROW_WIN_EXCEPTION( msg ) ;
		}
	}

	wstring get_creation_time(const CString filename )
	{
		filename ;
		SENSE("set_creation_time") ;
		SENSE(CT2A(filename)) ;
		return m_filetime ;
	}
	void set_view(string text)
	{
		m_view_char = boost::shared_array<char>(new char[text.size()+1]) ;
		strcpy_s(m_view_char.get(), text.size()+1, text.c_str()) ;
		m_size = text.size() ;
	}
	void set_view(wstring text)
	{
		m_view_wchar = boost::shared_array<wchar_t>(new wchar_t[text.size()+1]) ;
		wcscpy_s(m_view_wchar.get(), text.size()+1, text.c_str()) ;
	}	
	wchar_t* create_view_wchar(const CString filename)
	{
		return m_view_wchar.get() ;
	}
	char* create_view_char(const CString filename)
	{
		return m_view_char.get() ;
	}
	bool is_open()
	{
		return m_is_open ;
	}
	void close()
	{
		SENSE("close") ;
	}
	void open(const CString filename) 
	{
		filename ;
		SENSE("open") ;
		SENSE(CT2A(filename)) ;
	}
};