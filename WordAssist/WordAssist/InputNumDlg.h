#pragma once

#include "Exceptions.h"			// CException
#include "WindowExceptionHandler.h"

#include "app_state.h"

#include "resource.h"

#include "resource_string.h"
#include "MessageMapDefines.h" 

class CInputNumDlg: 
	public CDialogImpl<CInputNumDlg>, 
	CWindowExceptionHandler< CInputNumDlg >
{

	    CButton                 m_idcancel;
	    CEdit                   m_idcInputEdit;
	    CButton                 m_idok;
	
		int						m_selection ;

	public:

		enum {IDD = IDD_INPUT_NUM_DLG};

	BEGIN_MSG_MAP_EX(CInputNumDlg)
		try
		{
			MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
			BEGIN_CMD_HANDLER_EX
				CMD_HANDLER_EX_ID(IDCANCEL, OnIdcancel)
				CMD_HANDLER_EX_ID(IDOK, OnIdok)
			END_CMD_HANDLER_EX
		}
	catch ( except::CSWException &sw_e ) { return handle_exception( sw_e, _T("Failure in Input number dialog") ) ; } 
		catch ( _com_error &e )      { return handle_exception( e, _T("Failure in Input number dialog") ) ; } 
		catch ( except::CComException &e )   { return handle_exception( e, _T("Failure in Input number dialog") ) ; } 
		catch ( except::CWinException &e )   { return handle_exception( e, _T("Failure in Input number dialog") ) ; } 
		catch ( except::CException &e )      { return handle_exception( e, _T("Failure in Input number dialog") ) ; } 
		catch ( std::exception &e )  { return handle_exception( e, _T("Failure in Input number dialog") ) ; }
	END_MSG_MAP() 

	CInputNumDlg() : m_selection(-1){}
		
	int get_selection() { return m_selection ; }
	
	LRESULT OnInitDialog( )
	{
		BANNER("CInputNumDlg::OnInitDialog()");

		// Attach member controls to their resources.
	    m_idcancel                  = GetDlgItem(IDCANCEL);
	    m_idcInputEdit              = GetDlgItem(IDC_INPUT_EDIT);
	    m_idok                      = GetDlgItem(IDOK);

		app_state properties ;
		properties.read_from_registry() ;

		if ( properties.get_preferred_gui_lang() == LANG_JAPANESE )
		{
			m_idcancel.SetWindowText( R2T( IDS_CANCEL_J ) ) ;
			SetWindowText( R2T( IDS_INPUT_NUM_J ) ) ;
		}

		m_idcInputEdit.SetWindowText( _T("0") ) ;
		m_idcInputEdit.SetFocus() ;
		m_idcInputEdit.SetSel( 0, -1 ) ;
		return 0;

	} // OnInitDialog

	LRESULT OnIdcancel(WORD wID )
	{
		END_DLG ;

	} // OnIdcancel

	LRESULT OnIdok(WORD wID )
	{
		TCHAR num_text[MAX_NUM_STRLEN] = {0} ;
		m_idcInputEdit.GetWindowText( num_text, MAX_NUM_STRLEN ) ;
		
		if ( num_text[0] != 0 )
		{
			m_selection = _ttoi( num_text ) ;
		}

		END_DLG ;
		
	} // OnIdok

}; // class CInputNumDlg
