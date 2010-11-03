#pragma once

#include "file.h"
#include <boost/shared_ptr.hpp>

class InputDevice
{
public:
	virtual file::file::BYTE_ORDER_MARK get_file_bom( const CString filename ) = 0 ;
	virtual wstring get_creation_time(const CString location) = 0 ;
	// Throw an exception if the file doesn't exist.
	virtual void ensure_file_exists( const CString filename ) = 0 ;
	virtual unsigned int get_size(const CString filename) = 0 ;
	virtual char* create_view_char(const CString filename) = 0 ;
	virtual wchar_t* create_view_wchar(const CString filename) = 0 ;
	virtual bool exists(const CString filename) = 0;
	virtual void delete_file(const CString filename) = 0;
	virtual bool is_open() = 0;
	virtual void close() = 0;
	virtual void open(const CString filename) = 0 ;
};

typedef boost::shared_ptr<InputDevice> input_device_ptr ;