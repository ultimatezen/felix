// Properties.cpp: implementation of the CProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Properties.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*!
 The English version of the properties dialog
*/
CPropertiesDlgE::CPropertiesDlgE( const app_state &props ) :
m_properties( props )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW  ;

	m_title = CW2T(L"Felix User Preferences") ;
	m_psh.pszCaption = (LPCTSTR)m_title ;

	AddPage(m_page1) ;
	AddPage(m_page2) ;
	AddPage(m_page3) ;

	m_properties.read_from_registry() ;

	m_page1.set_props( &m_properties ) ;
	m_page2.set_props( &m_properties ) ;
	m_page3.set_props( &m_properties ) ;

	SetActivePage(0);
}

/*!
 The Japanese version of the properties dialog.
*/
CPropertiesDlgJ::CPropertiesDlgJ( const app_state &props ) :
m_properties( props )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW  ;

	m_title = CW2T(L"Felix ÉÜÅ[ÉUê›íË") ;
	m_psh.pszCaption = (LPCTSTR)m_title ;

	AddPage(m_page1) ;
	AddPage(m_page2) ;
	AddPage(m_page3) ;

	m_properties.read_from_registry() ;

	m_page1.set_props( &m_properties ) ;
	m_page2.set_props( &m_properties ) ;
	m_page3.set_props( &m_properties ) ;

	SetActivePage(0);
}

template< int id_type >
CPageView<id_type>::CPageView()
{

}

template< int id_type >
void CPageView<id_type>::set_props( app_state *props )
{
	m_properties = props ;
}

template< int id_type >
LRESULT CPageView<id_type>::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	DoDataExchange(FALSE); // FALSE means copy TO the dialog

	return TRUE;
}

template< int id_type >
int CPageView<id_type>::OnApply()
{
	DoDataExchange(TRUE) ;
	return PSNRET_NOERROR  ;
}

template< int id_type >
void CPageSegmentation<id_type>::set_props( app_state *props )
{
	m_properties = props ;
}

template< int id_type >
LRESULT CPageSegmentation<id_type>::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	BANNER("CPageSegmentation::OnInitDialog") ;

	DoDataExchange(FALSE); // FALSE means copy TO the dialog

	// we need to manually set the radio buttons, because the DDX
	// doesn't work for them.
	// The check boxes are handled in the DDX.
	SendDlgItemMessage( m_properties->m_skipJ, BM_SETCHECK, TRUE, 0 ) ;

	return TRUE;
}

template< int id_type >
int CPageSegmentation<id_type>::OnApply()
{
	BANNER("CPageSegmentation::OnApply") ;
	DoDataExchange(TRUE) ;

	// PSNRET_NOERROR = apply OK
	// PSNRET_INVALID = apply not OK, return to this page
	// PSNRET_INVALID_NOCHANGEPAGE = apply not OK, don't change focus
	return PSNRET_NOERROR  ;
}

template< int id_type >
BOOL CPageSegmentation<id_type>::OnKillActive()
{
	BANNER("CPageSegmentation::OnKillActive") ;
	DoDataExchange(TRUE);

	// FALSE = allow deactivate
	// TRUE = prevent deactivation
	return FALSE;
}

template< int id_type >
LRESULT CPageSegmentation<id_type>::OnSkip( WORD, WORD skip_code, HWND, BOOL& )
{
	m_properties->m_skipJ = skip_code ;
	return 0;
}

template< int id_type >
void CPageTransHist<id_type>::set_props( app_state *props )
{
	m_properties = props ;
}

template< int id_type >
LRESULT CPageTransHist<id_type>::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	BANNER("CPageTransHist::OnInitDialog") ;

	DoDataExchange(FALSE); // FALSE means copy TO the dialog

	return TRUE;
}

template< int id_type >
int CPageTransHist<id_type>::OnApply()
{
	BANNER("CPageTransHist::OnApply") ;
	DoDataExchange(TRUE) ;

	// PSNRET_NOERROR = apply OK
	// PSNRET_INVALID = apply not OK, return to this page
	// PSNRET_INVALID_NOCHANGEPAGE = apply not OK, don't change focus
	return PSNRET_NOERROR  ;
}

template< int id_type >
BOOL CPageTransHist<id_type>::OnKillActive()
{
	BANNER("CPageTransHist::OnKillActive") ;
	DoDataExchange(TRUE);

	// FALSE = allow deactivate
	// TRUE = prevent deactivation
	return FALSE;
}