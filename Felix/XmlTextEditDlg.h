/*!
	@file XmlTextEditDlg.h
	@brief Interface class for CXmlTextEditDlg
	@date 2005/06/25
	Time: 14:39:31
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "WideRichEdit.h"	// CWideRichEdit
#include "Exceptions.h"		// exception handling
#include "resource.h"		// IDD_XML_TEXT_EDIT define
#include "WindowExceptionHandler.h"

/**
	@class CXmlTextEditDlg 
	@brief For editing xml text (eliminate or keep as "rescue" tool?).
 */
class CXmlTextEditDlg 
	: public CDialogImpl<CXmlTextEditDlg>,
		public CWindowExceptionHandler< CXmlTextEditDlg >
{

	CWideRichEdit	m_edit ;
	CStatic		m_edit_box ;
	wstring		m_text ;

public:
	enum { IDD = IDD_XML_TEXT_EDIT };

	CXmlTextEditDlg();
	virtual ~CXmlTextEditDlg();

	void set_text( const wstring &text )
	{
		m_text = text ;
	}
	wstring get_text( )
	{
		return m_text ;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{

		// set icon
		SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

		// set up static where edit will go
		m_edit_box.Attach( GetDlgItem( IDC_EDIT ) ) ;
		// get its position
		RECT rc ;
		m_edit_box.GetWindowRect( &rc ) ;
		ScreenToClient( &rc ) ;

		// create the edit window and move it into place
		ATLASSERT( IsWindow() ) ;
		m_edit.Create(m_hWnd, rc, TEXT("RichEdit20W"), WS_CHILD | WS_VISIBLE | ES_SAVESEL | ES_MULTILINE | WS_VSCROLL | WS_CLIPSIBLINGS | WS_CLIPCHILDREN , WS_EX_CLIENTEDGE ) ;
		ATLASSERT( m_edit.IsWindow() ) ;
		m_edit.MoveWindow( &rc, TRUE ) ;
		m_edit.ShowWindow( SW_SHOW ) ;
		m_edit.SetWindowPos( HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;
		m_edit.SetText( m_text ) ;
		return TRUE;
	}
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_text =  m_edit.GetText() ;

		EndDialog(wID);
		return 0L ;
	}
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0L ;
	}

	BEGIN_MSG_MAP(CXmlTextEditDlg)
	MSG_TRY
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};

