#include "StdAfx.h"
#include ".\reshtmlfile.h"

#include "File.h"

#include "resource.h"
#include "resource_string.h" // R2T

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( ResFileTestCase )

	BOOST_AUTO_TEST_CASE(TestLoadStartupPage )
	{
		HINSTANCE hInst = _Module.GetResourceInstance() ;

		HRSRC hrsrc = FindResource(hInst,R2T(IDS_OPENING_PAGE),RT_HTML);	// ���\�[�X�ɑg�ݍ��񂾎��̖��O���ujpg�v�̏ꍇ
		//DWORD dwFileSize = SizeofResource(hInst, hrsrc);

		HANDLE hres = LoadResource(hInst,hrsrc);
		char* pcPos = (char*)LockResource(hres);
		BOOST_CHECK_EQUAL( 0, StrCmpNA(pcPos, "<html>\r\n<head>", strlen("<html>\r\n<head>") ) ) ;


		HRSRC hrsrc2 = FindResource(hInst,R2T(IDS_OPENING_PAGE),RT_HTML);	// ���\�[�X�ɑg�ݍ��񂾎��̖��O���ujpg�v�̏ꍇ
		//DWORD dwFileSize2 = SizeofResource(hInst, hrsrc2);

		HANDLE hres2 = LoadResource(hInst,hrsrc2);
		char* pcPos2 = (char*)LockResource(hres2);
		BOOST_CHECK_EQUAL( 0, StrCmpNA(pcPos2, "<html>\r\n<head>", strlen("<html>\r\n<head>") ) ) ;

	}
BOOST_AUTO_TEST_SUITE_END()

#endif 


