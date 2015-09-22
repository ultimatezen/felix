#pragma once

#include "WindowExceptionHandler.h"

template< int dlg_id >
class CNavQueryDlg : 
	public CDialogImpl<CNavQueryDlg<dlg_id> >, 
	CWindowExceptionHandler< CNavQueryDlg<dlg_id>  >
{
public:
	enum { IDD = dlg_id };

	CNavQueryDlg() : m_dont_ask(FALSE), m_nav_type(IDC_NEXT_SLIDE) {}

	BOOL		m_dont_ask ;
	int			m_nav_type ;

	LRESULT OnNavCmd(WORD, WORD id, HWND, BOOL& )
	{
		ATLASSERT(id == IDC_NEXT_SLIDE || 
				  id == IDC_SLIDE_TOP) ;
		m_nav_type = id ;
		return 0;
	}

	BEGIN_MSG_MAP(CNavQueryDlg<dlg_id>)
		MSG_TRY
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_NEXT_SLIDE, BN_CLICKED, OnNavCmd)
		COMMAND_HANDLER(IDC_SLIDE_TOP, BN_CLICKED, OnNavCmd)
		MSG_CATCH( _T("Action failed") )
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SendDlgItemMessage( m_nav_type, BM_SETCHECK, TRUE, 0 ) ;
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString window_text ;

		m_dont_ask = (BST_CHECKED == SendDlgItemMessage(IDC_DONT_ASK, BM_GETCHECK, 0, 0)) ;

		EndDialog(wID);
		return 0;
	}
};

typedef CNavQueryDlg<IDD_NAV_QUERY_DLG_E> CNavQueryDlgE ;
typedef CNavQueryDlg<IDD_NAV_QUERY_DLG> CNavQueryDlgJ ;