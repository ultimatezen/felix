#include "StdAfx.h"
#include "text_templates.h"
#include "path.h"
#include "file.h"
#include "Exceptions.h"
#include "logging.h"
// shell API support
#include "FileOpHandler.h"

namespace cpptempl
{

	CString calculate_template_filename(const CString filename)
	{
		file::CPath modpath(fileops::get_local_appdata_folder()) ;
		modpath.Append(_T("Felix")) ;
		modpath.Append(_T("html")) ;
		modpath.Append(R2T(IDS_LANG_CODE)) ;
		modpath.Append(filename) ;
		return modpath.Path() ;
	}
	CString calculate_module_template_filename(const CString filename)
	{
		file::CPath modpath ;
		modpath.GetModulePath(_Module.GetModuleInstance()) ;
		modpath.Append(_T("html")) ;
		modpath.Append(R2T(IDS_LANG_CODE)) ;
		modpath.Append(filename) ;
		return modpath.Path() ;
	}

	CString get_template_filename(const CString filename)
	{
		const CString tpl_filename = calculate_template_filename(filename) ;
		const file::CPath fullpath(tpl_filename) ;
		if (! fullpath.FileExists())
		{
			logging::log_warn("Template file not found: " + string(static_cast<LPCSTR>(CW2A(tpl_filename, CP_UTF8)))) ;
		}
		return tpl_filename ;
	}

	wstring get_template_text(const CString filename)
	{
		const CString full_path = get_template_filename(filename) ;
		file::view file_view ;
		string raw_text(static_cast<LPCSTR>(file_view.create_view_readonly(full_path))) ;
		if (raw_text.empty())
		{
			logging::log_error("Failed to load template text from " + string((LPCSTR)CStringA(full_path))) ;
		}
		return string2wstring(raw_text, CP_UTF8) ;
	}

}