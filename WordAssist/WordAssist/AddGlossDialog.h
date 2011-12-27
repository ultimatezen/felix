#pragma once

#include <atlctrls.h> // for member controls
#include "Exceptions.h"			// CException
#include "WindowExceptionHandler.h"

#include "app_state.h"

#include "resource.h"

#include "resource_string.h"
#include "MessageMapDefines.h" 

template <int ID_TYPE>
class CAddGlossDlgT: 
	public CDialogImpl<CAddGlossDlgT<ID_TYPE> >, 
	CWindowExceptionHandler< CAddGlossDlgT<ID_TYPE> >
{
public:

	CEdit           m_source_edit ;
	CEdit			m_trans_edit ;

	CString m_source ;
	CString m_trans ;

	typedef CAddGlossDlgT<ID_TYPE> TheClass ;

	const static int IDD = ID_TYPE ;

	BEGIN_MSG_MAP_EX(TheClass)
	try
	{
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID(IDCANCEL, OnIdcancel)
			CMD_HANDLER_EX_ID(IDOK, OnIdok)
		END_CMD_HANDLER_EX
	}
	catch ( CSWException &sw_e ) { return handle_exception( sw_e, _T("Failure in add glossary entry dialog") ) ; } 
	catch ( _com_error &e )      { return handle_exception( e, _T("Failure in add glossary entry dialog") ) ; } 
	catch ( CComException &e )   { return handle_exception( e, _T("Failure in add glossary entry dialog") ) ; } 
	catch ( CWinException &e )   { return handle_exception( e, _T("Failure in add glossary entry dialog") ) ; } 
	catch ( CException &e )      { return handle_exception( e, _T("Failure in add glossary entry dialog") ) ; } 
	catch ( std::exception &e )  { return handle_exception( e, _T("Failure in add glossary entry dialog") ) ; }
	END_MSG_MAP() 

	LRESULT OnInitDialog( )
	{
		BANNER("CAddGlossDlgT::OnInitDialog()");

		// Attach member controls to their resources.
		m_source_edit.Attach(GetDlgItem(IDC_SOURCE_EDIT)) ;
		m_trans_edit.Attach(GetDlgItem(IDC_TRANS_EDIT)) ;

		m_trans_edit.SetWindowText(m_trans) ;
		m_source_edit.SetWindowText(m_source) ;
		if (m_trans.IsEmpty() && ! m_source.IsEmpty())
		{
			m_trans_edit.SetFocus() ;
			m_trans_edit.SetSel( 0, -1 ) ;
		}
		else
		{
			m_source_edit.SetFocus() ;
			m_source_edit.SetSel( 0, -1 ) ;
		}
		return 0;
	} // OnInitDialog

	LRESULT OnIdcancel(WORD wID )
	{
		END_DLG ;

	} // OnIdcancel

	LRESULT OnIdok(WORD wID )
	{
		m_source_edit.GetWindowText(m_source) ;
		m_trans_edit.GetWindowText(m_trans) ;

		END_DLG ;

	} // OnIdok

}; // class CAddGlossDlgT

typedef CAddGlossDlgT<IDD_ADD_GLOSS_E_DLG> CAddGlossDlgE ;
typedef CAddGlossDlgT<IDD_ADD_GLOSS_J_DLG> CAddGlossDlgJ ;
