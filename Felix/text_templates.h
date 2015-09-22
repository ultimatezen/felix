#pragma once

namespace cpptempl
{
	CString get_template_filename(CString filename) ;
	wstring get_template_text(CString filename) ;
	CString calculate_template_filename(const CString filename) ;
	CString calculate_module_template_filename(const CString filename) ;
}
