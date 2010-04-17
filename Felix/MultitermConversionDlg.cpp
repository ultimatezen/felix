// MultitermConversionDlg.cpp: implementation of the CMultitermConversionDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MultitermConversionDlg.h"
#include "resource_string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultitermConversionDlg::CMultitermConversionDlg()
:
	m_is_import(false),
	m_multiterm_version( IDC_MULTI6 )
{

	
}

CMultitermConversionDlg::~CMultitermConversionDlg()
{
	
}

bool CMultitermConversionDlg::is_import() 
{
	return m_is_import ;
}
void CMultitermConversionDlg::put_import_on() 
{
	m_is_import = true ;
}
void CMultitermConversionDlg::put_import_off() 
{
	m_is_import = false ;	
}

int CMultitermConversionDlg::get_multiterm_version()
{
	return m_multiterm_version ;	
}
void CMultitermConversionDlg::set_multiterm_version( int setting ) 
{
	ATLASSERT( setting == IDC_MULTI6 || setting || IDC_MULTI55 ) ;
	m_multiterm_version = setting ;
}

// Function name	: OnInitDialog
// Description	    : 
// Return type		: LRESULT 
LRESULT CMultitermConversionDlg::OnInitDialog( )
{
	SENSE( "OnInitDialog" ) ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	SendDlgItemMessage( IDC_MULTI6, BM_SETCHECK, TRUE, 0 ) ;

	if ( m_is_import ) SetWindowText( resource_string( IDS_MULTITERM_IMPORT_TITLE ) ) ;
	else SetWindowText( resource_string( IDS_MULTITERM_EXPORT_TITLE ) ) ;

	return TRUE;
}

// Function name	: OnOK
// Description	    : 
// Return type		: LRESULT 
LRESULT CMultitermConversionDlg::OnOK( )
{
	SENSE( "OnOK" ) ;

#ifndef UNIT_TEST
	EndDialog(IDOK);
#endif
	return 0;
}

// Function name	: OnCancel
// Description	    : 
// Return type		: LRESULT 
LRESULT CMultitermConversionDlg::OnCancel( )
{
	SENSE( "OnCancel" ) ;

#ifndef UNIT_TEST
	EndDialog(IDCANCEL);
#endif
	return 0;
}

// Function name	: OnMulti55
// Description	    : 
// Return type		: LRESULT 
LRESULT CMultitermConversionDlg::OnMulti55( )
{
	BANNER( "CMultitermConversionDlg::OnMulti55" ) ;

	m_multiterm_version = IDC_MULTI55 ;
	return 0;
}

// Function name	: OnMulti66
// Description	    : 
// Return type		: LRESULT 
LRESULT CMultitermConversionDlg::OnMulti66( )
{
	BANNER( "CMultitermConversionDlg::OnMulti66" ) ;

	m_multiterm_version = IDC_MULTI6 ;
	return 0;
}