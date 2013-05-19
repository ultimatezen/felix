#include "stdafx.h"
#include "config_file.h"


#include "Path.h"
#include "logging.h"
#include "Exceptions.h"
#include "comdef.h"

CString get_config_filename(CString filename, output_device_ptr output)
{
	file::CPath pathname(get_local_appdata()) ;
	pathname.Append(_T("Felix")) ;
	pathname.Append(_T("prefs")) ;
	output->ensure_dirs(pathname.Path()) ;
	pathname.Append(filename) ;
	return pathname.Path() ;
}

CString get_local_appdata()
{
	TCHAR szPath[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, // hwndOwner
		CSIDL_LOCAL_APPDATA,		  // nFolder
		(HANDLE)NULL,				  // hToken (-1 means "default user")
		SHGFP_TYPE_CURRENT,			  // dwFlags 
		szPath) ;
	if (! SUCCEEDED(hr))
	{
		logging::log_error("Failed to get local app data folder") ;
		_com_error ce(hr) ;
		logging::log_exception(ce) ;
		return CString();
	}

	return CString(static_cast<LPCTSTR>(szPath)) ;
}

string get_config_text(CString filename, output_device_ptr output, input_device_ptr input)
{
	CString config_filename = get_config_filename(filename, output) ;
	return get_file_text(config_filename, input) ;
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