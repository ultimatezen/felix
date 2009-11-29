// Properties.cpp : Implementation of CProperties
#include "StdAfx.h"
#include "Felix.h"
#include "Properties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


/////////////////////////////////////////////////////////////////////////////
// CProperties

STDMETHODIMP CProperties::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IProperties ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IProperties ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IProperties ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IProperties ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE ;

}

STDMETHODIMP CProperties::get_MemoryProperties(IMemoryProperties **pVal)
{
	CComObject< CMemoryProperties >* the_props = 0 ;
	
	HRESULT hr = CComObject< CMemoryProperties >::CreateInstance( &the_props ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	the_props->AddRef() ;
	
	if( SUCCEEDED( hr ) )
	{
		hr = the_props->QueryInterface( pVal ) ;
	}
	
	the_props->Release() ;
	
	return hr ;
}

STDMETHODIMP CProperties::get_GlossaryProperties(IGlossaryProperties **pVal)
{
	CComObject< CGlossaryProperties >* the_props = 0 ;
	
	HRESULT hr = CComObject< CGlossaryProperties >::CreateInstance( &the_props ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	the_props->AddRef() ;
	
	if( SUCCEEDED( hr ) )
	{
		hr = the_props->QueryInterface( pVal ) ;
	}
	
	the_props->Release() ;
	
	return hr ;
}

STDMETHODIMP CProperties::get_GeneralProperties(IGeneralProperties **pVal)
{
	CComObject< CGeneralProperties >* the_props = 0 ;
	
	HRESULT hr = CComObject< CGeneralProperties >::CreateInstance( &the_props ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	the_props->AddRef() ;
	
	if( SUCCEEDED( hr ) )
	{
		hr = the_props->QueryInterface( pVal ) ;
	}
	
	the_props->Release() ;
	
	return hr ;
}
