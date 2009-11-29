#include "StdAfx.h"
#include ".\reshtmlfile.h"

#include "File.h"

#include "resource.h"
#include "resource_string.h" // R2T

#include "easyunit/testharness.h"

#ifdef _DEBUG
namespace easyunit
{

	TEST( ResFileTestCase, TestLoadStartupPage )
	{
		HINSTANCE hInst = _Module.GetResourceInstance() ;

		HRSRC hrsrc = FindResource(hInst,R2T(IDS_OPENING_PAGE),RT_HTML);	// リソースに組み込んだ時の名前が「jpg」の場合
		//DWORD dwFileSize = SizeofResource(hInst, hrsrc);

		HANDLE hres = LoadResource(hInst,hrsrc);
		char* pcPos = (char*)LockResource(hres);
		ASSERT_EQUALS( 0, StrCmpNA(pcPos, "<html>\r\n<head>", strlen("<html>\r\n<head>") ) ) ;


		HRSRC hrsrc2 = FindResource(hInst,R2T(IDS_OPENING_PAGE),RT_HTML);	// リソースに組み込んだ時の名前が「jpg」の場合
		//DWORD dwFileSize2 = SizeofResource(hInst, hrsrc2);

		HANDLE hres2 = LoadResource(hInst,hrsrc2);
		char* pcPos2 = (char*)LockResource(hres2);
		ASSERT_EQUALS( 0, StrCmpNA(pcPos2, "<html>\r\n<head>", strlen("<html>\r\n<head>") ) ) ;

	}
}

#endif 


