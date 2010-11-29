// FileDialogEx.h: interface for the CFileDialogEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDIALOGEX_H__2F62C2F1_EF62_41A8_AA31_2C98CBF8C238__INCLUDED_)
#define AFX_FILEDIALOGEX_H__2F62C2F1_EF62_41A8_AA31_2C98CBF8C238__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDialogEx.h : header file
//

class CFileDialogEx : public CFileDialog  
{
public:

	// Constructor:
	CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL);

	// Overloaded method
	virtual int DoModal(LPCTSTR CurrentDir = NULL);
};

#endif // !defined(AFX_FILEDIALOGEX_H__2F62C2F1_EF62_41A8_AA31_2C98CBF8C238__INCLUDED_)
