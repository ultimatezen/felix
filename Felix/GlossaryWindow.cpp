// GlossaryWindow.cpp : Implementation of CGlossaryWindow
#include "StdAfx.h"
#include "Felix.h"
#include "GlossaryWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGlossaryWindow

STDMETHODIMP CGlossaryWindow::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IGlossaryWindow ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IGlossaryWindow ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IGlossaryWindow ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IGlossaryWindow ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE;
}

STDMETHODIMP CGlossaryWindow::get_HWND(long *pVal)
{
	*pVal = (long)m_glossary_window->m_hWnd ;

	return S_OK;
}

STDMETHODIMP CGlossaryWindow::get_Glossaries(IMemories **pVal)
{
	HRESULT hr = S_OK ;

	try
	{
		CComObject< CMemories >* the_memories = 0 ;

		create_memories(the_memories);	

		the_memories->set_memories( m_glossary_window->get_memories() ) ;

		if( SUCCEEDED( hr ) )
		{
			hr = the_memories->QueryInterface( pVal ) ;
		}

		the_memories->Release() ;
	}
	catch ( CComException &e ) 
	{
		return e.code() ;
	}
	
	return hr ;
}

STDMETHODIMP CGlossaryWindow::get_Visible(BOOL *pVal)
{
	if ( m_glossary_window->IsWindowVisible() )
		*pVal = VARIANT_TRUE ;
	else
		*pVal = VARIANT_FALSE ;

	return S_OK;
}

STDMETHODIMP CGlossaryWindow::put_Visible(BOOL newVal)
{
	if ( newVal )
		m_glossary_window->ShowWindow( SW_SHOWNOACTIVATE ) ;
	else
		m_glossary_window->ShowWindow( SW_HIDE ) ;
	
	return S_OK;
}

void CGlossaryWindow::create_memories(ATL::CComObject< CMemories >* the_memories)
{
	COM_ENFORCE( CComObject< CMemories >::CreateInstance( &the_memories ), "Failed to create instance of CMemories" ) ;
}