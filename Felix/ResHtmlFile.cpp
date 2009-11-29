#include "StdAfx.h"
#include ".\reshtmlfile.h"

CResHtmlFile::CResHtmlFile(LPCTSTR res_name, UINT codepage)
{
	HINSTANCE hInst = _Module.GetResourceInstance() ;

	HRSRC hrsrc = FindResource(hInst,res_name,RT_HTML);	// リソースに組み込んだ時の名前が「jpg」の場合
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
