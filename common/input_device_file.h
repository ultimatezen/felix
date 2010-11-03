#pragma once

#include "input_device.h"
#include "miscwrappers.h"
#include "Exceptions.h"

class InputDeviceFile : public InputDevice
{
public:
	file::file m_target ;
	file::view m_view ;

	file::file::BYTE_ORDER_MARK get_file_bom( const CString filename );
	unsigned int get_size(const CString filename);
	void ensure_file_exists( const CString file_name );
	wstring get_creation_time(const CString location);
	wchar_t* create_view_wchar(const CString filename);
	char* create_view_char(const CString filename);
	bool exists(const CString filename);
	void delete_file(const CString filename)  ;

	bool is_open();
	void close();
	void open(const CString filename);
};