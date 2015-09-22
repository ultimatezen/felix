// GlossaryWindows.cpp : Implementation of CGlossaryWindows
#include "StdAfx.h"
#include "Felix.h"
#include "GlossaryWindows.h"
#include "GlossaryWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


struct _CopyVariantFromGlossWindow
{
	static HRESULT copy( VARIANT *lhs, const gloss_window_pointer *rhs )
	{
		ATLASSERT( IsValidWritePointer( lhs ) ) ;
		ATLASSERT( IsValidReadPointer( (*rhs).get_const_pointer() ) ) ;

		CComObject< CGlossaryWindow >* the_gloss_window = 0 ;
		
		HRESULT hr = CComObject< CGlossaryWindow >::CreateInstance( &the_gloss_window ) ;

		ASSERT_HRESULT(  hr ) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		
		the_gloss_window->AddRef() ;
		
		the_gloss_window->set_glossary_window( *rhs ) ;
		
		if( SUCCEEDED( hr ) )
		{
			hr = the_gloss_window->QueryInterface( &lhs->punkVal ) ;
			lhs->vt = VT_UNKNOWN ;

			ASSERT_HRESULT(  hr ) ;
		}
		
		the_gloss_window->Release() ;
		
		return hr ;
	}
	
	static void init( VARIANT *p ) 
	{
		ATLASSERT( IsValidWritePointer( p ) ) ;

		// return type is VOID
		VariantInit( p ) ; 
	}
	static void destroy( VARIANT *p ) 
	{ 
		ATLASSERT( IsValidWritePointer( p  ) ) ;

		_HR0( VariantClear( p ) );
	}
};

typedef CComEnumOnSTL< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
		_CopyVariantFromGlossWindow, gloss_window_list > GlossWindowEnum ;


/////////////////////////////////////////////////////////////////////////////
// CGlossaryWindows

STDMETHODIMP CGlossaryWindows::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IGlossaryWindows ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IGlossaryWindows ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IGlossaryWindows ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IGlossaryWindows ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE;
}

STDMETHODIMP CGlossaryWindows::get_Count(long *pVal)
{
	ATLASSERT( IsValidWritePointer( pVal ) ) ;
	*pVal = m_glossary_windows->size() ;

	return S_OK;
}

STDMETHODIMP CGlossaryWindows::get_Item(long index, IGlossaryWindow **pVal)
{
	if ( index < 1 || static_cast< size_t >( index ) > m_glossary_windows->size() )
		return E_INVALIDARG ;
	
	CComObject< CGlossaryWindow >* the_gloss_window = 0 ;
	
	HRESULT hr = CComObject< CGlossaryWindow >::CreateInstance( &the_gloss_window ) ;
	ASSERT_HRESULT( hr ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	the_gloss_window->AddRef() ;
	
	gloss_window_iterator pos = m_glossary_windows->begin() ;
	
	std::advance( pos, index-1 ) ;
	
	the_gloss_window->set_glossary_window( *pos ) ;
	
	hr = the_gloss_window->QueryInterface( pVal ) ;
	
	the_gloss_window->Release() ;
	
	return hr ;
}

STDMETHODIMP CGlossaryWindows::get__NewEnum(IUnknown **pVal)
{
	*pVal = 0 ;
	
	CComObject< GlossWindowEnum >* pe = 0 ;
	
	
	HRESULT hr = CComObject< GlossWindowEnum >::CreateInstance( &pe ) ;
	ASSERT_HRESULT( hr ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	pe->AddRef() ;
	
	pe->Init( this->GetUnknown(), *m_glossary_windows ) ;
	
	ASSERT_HRESULT( hr ) ;
	if( SUCCEEDED( hr ) )
	{
		hr = pe->QueryInterface( pVal ) ;
	}
	
	pe->Release() ;
	
	return hr ;
}

STDMETHODIMP CGlossaryWindows::Add(IGlossaryWindow **pVal)
{
	CComObject< CGlossaryWindow >* the_gloss_window = 0 ;
	
	HRESULT hr = CComObject< CGlossaryWindow >::CreateInstance( &the_gloss_window ) ;
	
	ASSERT_HRESULT( hr ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	the_gloss_window->AddRef() ;
	
	gloss_window_pointer gloss_window ;

	app::get_app().add_glossary_window( gloss_window, SW_HIDE ) ;

	the_gloss_window->set_glossary_window( gloss_window ) ;

	ASSERT_HRESULT( hr ) ;
	if( SUCCEEDED( hr ) )
	{
		hr = the_gloss_window->QueryInterface( pVal ) ;
	}
	
	the_gloss_window->Release() ;
	
	return hr ;
}

STDMETHODIMP CGlossaryWindows::Remove(VARIANT item)
{
	if ( item.vt == VT_UNKNOWN )
	{
		CComQIPtr< IGlossaryWindow > the_gloss_window( item.punkVal ) ;
		
		long lwnd ;

		the_gloss_window->get_HWND( &lwnd ) ;

		CWindow wnd( (HWND)lwnd ) ;

		wnd.SendMessage( WM_DESTROY, 0, 0 ) ;

		for ( auto pos = m_glossary_windows->begin() ; pos != m_glossary_windows->end() ;  )
		{
			if ( ! (*pos)->IsWindow() )
			{
				m_glossary_windows->erase(pos) ;	
				break ;
			}
			else ++pos ;
		}

		return S_OK;
	}
	if ( item.lVal < 1 || static_cast< size_t >( item.lVal ) > m_glossary_windows->size() )
		return E_INVALIDARG ;
	
	gloss_window_iterator pos = m_glossary_windows->begin() ;
	
	std::advance( pos, ( item.lVal -1 ) ) ;
	
	(*pos)->OnDestroy() ;
	
	for ( auto pos = m_glossary_windows->begin() ; pos != m_glossary_windows->end() ;  )
	{
		if ( ! (*pos)->IsWindow() )
		{
			m_glossary_windows->erase(pos) ;
			break ;
		}
		else ++pos ;
	}
	
	return S_OK;
}
