#include "StdAfx.h"
#include "text_templates.h"
#include "path.h"
#include "file.h"
#include "Exceptions.h"
#include "logging.h"

CString get_template_filename(CString filename)
{
	TCHAR szPath[MAX_PATH];
	COM_ENFORCE(SHGetFolderPath(NULL, 
		CSIDL_LOCAL_APPDATA, 
		NULL, 
		0, 
		szPath), _T("Failed to retrieve local app data folder") ) ; 

	fs::wpath pathname = fs::wpath(szPath) 
		/ _T("Felix") 
		/ _T("html")
		/ R2T(IDS_LANG_CODE)
		/ filename ;
	CString tpl_filename = CString(pathname.string().c_str()) ;
	file::CPath fullpath(tpl_filename) ;
	if (! fullpath.FileExists())
	{
		ATLTRACE("** Html file doesn't exist\n") ;
		TRACE(tpl_filename) ;

		file::CPath modpath ;
		modpath.GetModulePath(_Module.GetModuleInstance()) ;
		fs::wpath fullpathname = fs::wpath((LPCTSTR)modpath.Path()) 
			/ _T("html")
			/ R2T(IDS_LANG_CODE)
			/ filename ;
		CString module_filename = CString(fullpathname.string().c_str()) ;
		try
		{
			CDispatchWrapper utils(L"Felix.Utilities") ;
			CComVariant source(module_filename) ;
			CComVariant dest(tpl_filename) ;
			utils.method(L"CopyHtml", source, dest) ;
		}
		catch (_com_error& e)
		{
			logging::log_error("Failed to force HTML files") ;
			logging::log_exception(e) ;
#ifndef UNIT_TEST
			ATLASSERT(FALSE && "Failed to force HTML files") ;
#endif
		}
		return module_filename ;
	}
	return tpl_filename ;
}

wstring get_template_text(CString filename)
{
	CString full_path = get_template_filename(filename) ;
	file::view file_view ;
	string raw_text(static_cast<LPCSTR>(file_view.create_view_readonly(full_path))) ;
	return string2wstring(raw_text, CP_UTF8) ;
}