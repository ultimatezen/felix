// FileDialogEx.cpp: implementation of the CFileDialogEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileDialogEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) : CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}

int CFileDialogEx::DoModal(LPCTSTR CurrentDir)
{
	m_ofn.lpstrInitialDir = CurrentDir;
	m_ofn.Flags &= ~OFN_ENABLEHOOK;
	m_ofn.hwndOwner = PreModal();
	int nResult;
	if (m_bOpenFileDialog)
		nResult = ::GetOpenFileName(&m_ofn);
	else
		nResult = ::GetSaveFileName(&m_ofn);
	return nResult;
}
