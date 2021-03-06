/////////////////////////////////////////////////////////////////////////////////////////
//  UserStringDlg.cpp: implementation of CUserStringDlg class.
/////////////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "resource.h"
#include "UserStringDlg.h"

LRESULT CUserStringDlg::OnInitDialog( )
{
	SENSE("OnInitDialog") ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
	
	
#ifndef UNIT_TEST
	// create the key window 
	TRUE_ENFORCE( FALSE != m_key_edit.SubclassWindow( GetDlgItem( IDC_SOURCE_BOX ) ), IDS_RICHEDIT_CREATE_FAILED ) ;
	// create the value window
	TRUE_ENFORCE( FALSE != m_value_edit.SubclassWindow( GetDlgItem( IDC_TRANS_BOX ) ), IDS_RICHEDIT_CREATE_FAILED ) ;
#endif
	
	m_key_edit.SetFocus() ;
	m_key_edit.SetSel( 0, 0 ) ;

	return TRUE;
} 

LRESULT CUserStringDlg::OnClose( )
{
	SENSE("OnCancel") ;
#ifndef UNIT_TEST
	EndDialog(IDCANCEL);
#endif
	return 0L ;
} 

LRESULT CUserStringDlg::OnOK(const WORD wID )
{
	SENSE("OnOK") ;
	m_key = m_key_edit.GetText() ;
	m_value = m_value_edit.GetText() ;

	END_DLG ;
} 

const std::wstring& CUserStringDlg::GetKey() const
{
	return m_key;
}
const std::wstring& CUserStringDlg::GetValue() const
{
	return m_value ;
}

/////////////////////////////////////////////////////////////////////////////////////////
