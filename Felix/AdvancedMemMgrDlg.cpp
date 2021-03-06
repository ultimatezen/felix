/*!
	@brief implementation of CAdvancedMemMgrDlg class.
	@author Ryan Ginstrom
 */

#include "stdafx.h"
#include "resource.h"
#include "AdvancedMemMgrDlg.h"
#include "record.h"


LRESULT CAdvancedMemMgrDlg::OnInitDialog( )
{
	SENSE("OnInitDialog") ;

	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_ICON ) ), TRUE ) ;

	m_idcReliabilitySpin = GetDlgItem(IDC_RELIABILITY_SPIN);
	m_idcReliabilitySpin.SetRange(mem_engine::MIN_RELIABILITY, 
								  mem_engine::MAX_RELIABILITY) ;

	CString text ;
	if ( m_reliability <= 10 )
	{
		text.Format( _T("%d"), m_reliability ) ;
	}

#ifdef UNIT_TEST
	return TRUE ;
#else

	m_idcReliabilityEdit = GetDlgItem(IDC_RELIABILITY_EDIT);
	m_idcReliabilityEdit.SetWindowText( text ) ;

	m_idcValidity = GetDlgItem( IDC_VALIDITY ) ;
	m_idcValidity.SetCheck( m_validation ) ;

	m_idcLocked = GetDlgItem(IDC_LOCKED);
	m_idcLocked.SetCheck( m_lock ) ;

	return 0;
#endif

} 

LRESULT CAdvancedMemMgrDlg::OnCancel(WORD wID )
{
	SENSE("OnCancel") ;
	END_DLG ;
} 

LRESULT CAdvancedMemMgrDlg::OnIdok(WORD wID )
{
	SENSE("OnOK") ;
	wID ;
	m_lock = m_idcLocked.GetCheck() ;
	m_validation = m_idcValidity.GetCheck() ;

#ifndef UNIT_TEST
	CString reliability_text ;
	m_idcReliabilityEdit.GetWindowText(reliability_text) ;

	m_reliability = parse_reliability(reliability_text);
#endif

	END_DLG ;
} 

int CAdvancedMemMgrDlg::parse_reliability( CString reliability_text )
{
	if ( reliability_text.IsEmpty() )
	{
		return -1 ;
	}
	else
	{
		return boost::lexical_cast<int, tstring>(tstring(reliability_text)) ;
	}
}

int CAdvancedMemMgrDlg::get_validation() const
{
	return m_validation ;
}

size_t CAdvancedMemMgrDlg::get_reliability() const
{
	return static_cast<size_t>(max(m_reliability, 0)) ;
}

int CAdvancedMemMgrDlg::get_lock() const
{
	return m_lock ;
}

void CAdvancedMemMgrDlg::set_validation( const int setting )
{
	m_validation = setting ;
}

void CAdvancedMemMgrDlg::set_reliability( const int setting )
{
	m_reliability = setting ;
}

void CAdvancedMemMgrDlg::set_lock( const int setting )
{
	m_lock = setting ;
}