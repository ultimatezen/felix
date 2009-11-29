// Properties.cpp: implementation of the CProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Properties.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*!
  Constructor for CPropertiesDlgE class
 */
CPropertiesDlgE::CPropertiesDlgE( const app_state &props ) :
	m_properties( props )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW  ;

	m_title.LoadString( IDS_PROPSHEET_TITLE_E ) ;
	m_psh.pszCaption = (LPCTSTR)m_title ;
	
	AddPage(m_page1) ;
	AddPage(m_page2) ;
	AddPage(m_page3) ;
	AddPage(m_page4) ;

	m_properties.read_from_registry() ;

	m_page1.set_props( &m_properties ) ;
	m_page2.set_props( &m_properties ) ;
	m_page3.set_props( &m_properties ) ;
	m_page4.set_props( &m_properties ) ;

	SetActivePage(0);
}

/*!
  Constructor for CPropertiesDlgJ class
 */
CPropertiesDlgJ::CPropertiesDlgJ( const app_state &props ) :
	m_properties( props )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW  ;
	
	m_title.LoadString( IDS_PROPSHEET_TITLE_J ) ;
	m_psh.pszCaption = (LPCTSTR)m_title ;
	
	AddPage(m_page1) ;
	AddPage(m_page2) ;
	AddPage(m_page3) ;
	AddPage(m_page4) ;

	m_properties.read_from_registry() ;

	m_page1.set_props( &m_properties ) ;
	m_page2.set_props( &m_properties ) ;
	m_page3.set_props( &m_properties ) ;
	m_page4.set_props( &m_properties ) ;
	
	SetActivePage(0);
}