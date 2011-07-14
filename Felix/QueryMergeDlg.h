#pragma once

#include "resource.h"
#include "Exceptions.h"		// exception handling
#include "logging.h"
#include "ATLCTRLS.H"
#include <atlcrack.h>

/**
@class CQueryMergeDlg
@brief As the user whether they want to merge memories.
*/
class CQueryMergeDlg : 
	public CDialogImpl<CQueryMergeDlg, TWindow>
	, public CWinDataExchange< CQueryMergeDlg >
{

public:
	BOOL m_bDontAsk;
	int m_iMerge;

	CStatic m_merge_static ;

	CString m_merge_message ;
	CString m_title ;

	static const int IDD = IDD_QUERY_MERGE_DLG ;
	DECLARE_SENSING_VAR ;

	CQueryMergeDlg(int title_res_id, int text_res_id, const CString mem_name);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRadioMerge(WORD, WORD, HWND, BOOL& );
	LRESULT OnRadioSeparate(WORD, WORD, HWND, BOOL& );

	BEGIN_MSG_MAP(CQueryMergeDlg)
		try
		{
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_HANDLER_EX(IDC_DUP_MSG, STN_DBLCLK, OnDupMsgStnDblClk)
			COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
			COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)

			COMMAND_HANDLER(IDC_MERGE, BN_CLICKED, OnRadioMerge)
			COMMAND_HANDLER(IDC_SEPARATE, BN_CLICKED, OnRadioSeparate)
		}
		catch( except::CException &e )
		{
			logging::log_error("Error handling Query Merge Dialog message") ;
			logging::log_exception(e) ;
			e.notify_user( resource_string(IDS_MSG_ACTION_FAILED) ) ;
		}
		catch ( std::exception &e ) 
		{
			logging::log_error("Standard Library error handling Query Merge Dialog message") ;
			logging::log_error(e.what()) ;
			CString emsg = CA2CT( e.what() ) ;
			except::CException myEx( emsg ) ;
			myEx.notify_user( resource_string(IDS_MSG_ACTION_FAILED) ) ;
		}
		catch ( _com_error &e ) 
		{
			logging::log_error("COM Error handling Query Merge Dialog message") ;
			logging::log_exception(e) ;
			except::CComException comEx( e ) ;
			comEx.notify_user( resource_string(IDS_MSG_ACTION_FAILED) ) ;
		}
	END_MSG_MAP()

	BEGIN_DDX_MAP(CQueryMergeDlg)
		DDX_CHECK(IDC_DONT_ASK, m_bDontAsk)
	END_DDX_MAP()


	LRESULT OnDupMsgStnDblClk(int wNotifyCode, UINT wID, HWND hWndCtl);
};