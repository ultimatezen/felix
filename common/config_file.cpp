#include "stdafx.h"
#include "config_file.h"

#include "Path.h"
#include "logging.h"
#include "Exceptions.h"
#include "comdef.h"
#include "FileOpHandler.h"


string get_config_text(CString filename, output_device_ptr output, input_device_ptr input)
{
	CString config_filename = get_config_filename(filename, output) ;
	return get_file_text(config_filename, input) ;
}

CString get_config_filename(CString filename, output_device_ptr output)
{
	file::CPath pathname(get_config_path()) ;
	ensure_path(output, pathname.Path()) ;
	pathname.Append(filename) ;
	return pathname.Path() ;
}
CString get_config_path()
{
	file::CPath pathname(get_local_appdata()) ;
	pathname.Append(_T("Felix")) ;
	pathname.Append(_T("prefs")) ;
	return pathname.Path() ;
}
CString get_local_appdata()
{
	try
	{
		return fileops::get_local_appdata_folder() ;
	}
	catch (except::CComException &e)
	{
		logging::log_error("Failed to get local app data folder") ;
		logging::log_exception(e) ;
		return CString();
	}
}
void ensure_path(output_device_ptr output, CString pathname)
{
	output->ensure_dirs(pathname) ;
}


string get_file_text(CString filename, input_device_ptr input)
{
	if (! input->exists(filename))
	{
		logging::log_error("File does not exist.") ;
		return string() ;
	}
	try
	{
		return string(input->create_view_const_char(filename)) ;
	}
	catch (except::CException& e)
	{
		logging::log_error("Program exception: File could not be read.") ;
		logging::log_exception(e) ;
		return string() ;
	}
}