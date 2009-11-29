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