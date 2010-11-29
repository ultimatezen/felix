// GenericFindDlg.h: interface for the CGenericFindDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERICFINDDLG_H__12E6547E_3B18_4163_B4A3_565D60FB3F02__INCLUDED_)
#define AFX_GENERICFINDDLG_H__12E6547E_3B18_4163_B4A3_565D60FB3F02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringEx.h"
#include "WideRichEdit.h"
#include "atlcrack.h"

class CGenericFindDlg  : 
		public	CWindowImpl< CGenericFindDlg >
{
	CWideRichEdit	m_edit ;
	CButton			m_ignore_case_button ;
	CButton			m_find_button ;
	CButton			m_cancel_button ;

	wstring			m_search_text ;
	bool			m_ignore_case ;

public:
	CGenericFindDlg();
	virtual ~CGenericFindDlg();

	wstring get_search_text() { return m_search_text ; }
	bool get_ignore_case() { return m_ignore_case ; }

	LRESULT OnCreate( LPCREATESTRUCT create_struct )
	{
		BANNER( "CGenericFindDlg::OnCreate" ) ;
		TRACE( create_struct->lpszName ) ;
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnClose( )
	{
		BANNER( "CGenericFindDlg::OnClose" ) ;
		m_search_text = m_edit.GetText() ;
		::PostMessage( GetParent(), UWM_USER_MESSAGE, IDOK, 0 ) ;
		ShowWindow( SW_HIDE ) ;
		return 0;
	}
	LRESULT OnCloseCmd( UINT control, int id, HWND hwnd )
	{
		BANNER( "CGenericFindDlg::OnCloseCmd" ) ;
		m_search_text = m_edit.GetText() ;
		::PostMessage( GetParent(), UWM_USER_MESSAGE, IDOK, 0 ) ;
		ShowWindow( SW_HIDE ) ;
		return 0;
	}
	LRESULT OnCancelCmd( UINT control, int id, HWND hwnd )
	{
		BANNER( "CGenericFindDlg::OnCancelCmd" ) ;
		::PostMessage( GetParent(), UWM_USER_MESSAGE, IDCANCEL, 0 ) ;
		ShowWindow( SW_HIDE ) ;
		return 0;
	}

	BEGIN_MSG_MAP_EX(CGenericFindDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_CLOSE(OnClose)
		COMMAND_ID_HANDLER_EX(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancelCmd)
	END_MSG_MAP()

};

#endif // !defined(AFX_GENERICFINDDLG_H__12E6547E_3B18_4163_B4A3_565D60FB3F02__INCLUDED_)
