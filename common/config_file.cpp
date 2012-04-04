#include "stdafx.h"
#include "config_file.h"


#include "Path.h"
#include "logging.h"
#include "Exceptions.h"


CString get_config_filename(CString filename, output_device_ptr output)
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
		except::CComException e(hr) ;
		logging::log_exception(e) ;
		return CString();
	}

	file::CPath pathname(CString(static_cast<LPCTSTR>(szPath))) ;
	pathname.Append(_T("Felix")) ;
	pathname.Append(_T("prefs")) ;
	output->ensure_dirs(pathname.Path()) ;
	pathname.Append(filename) ;
	return pathname.Path() ;
}

wstring get_config_text(CString filename, output_device_ptr output, input_device_ptr input)
{
	CString config_filename = get_config_filename(filename, output) ;
	if (! input->exists(config_filename))
	{
		return wstring() ;
	}
	try
	{
		string raw_text(input->create_view_const_char(config_filename)) ;
		return string2wstring(raw_text, CP_UTF8) ;
	}
	catch (except::CException& e)
	{
		logging::log_error("Program exception: configuration file could not be read") ;
		logging::log_exception(e) ;
		return wstring() ;
	}
}
