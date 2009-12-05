#include "StdAfx.h"
#include ".\reshtmlfile.h"
#include "logging.h"

CResHtmlFile::CResHtmlFile(LPCTSTR res_name, UINT codepage)
{
	HINSTANCE hInst = _Module.GetResourceInstance() ;

	HRSRC hrsrc = FindResource(hInst,res_name,RT_HTML);	// リソースに組み込んだ時の名前が「jpg」の場合
	if (! hrsrc)
	{
		CString msg ;
		msg.Format(_T("Failed to retrieve resource: %s"), res_name) ;
		logging::log_error(static_cast<LPCSTR>(CStringA(msg))) ;
		return ;
	}

	DWORD dwFileSize = SizeofResource(hInst, hrsrc);

	HANDLE hres = LoadResource(hInst,hrsrc);
	ATLASSERT( hres != NULL ) ;
	if ( hres == NULL )
	{
		return ;
	}
	string pcPos( (char*)LockResource(hres), dwFileSize ) ;
	m_file_text = CA2W( pcPos.c_str(), codepage ) ;
}

CResHtmlFile::~CResHtmlFile(void)
{
}
