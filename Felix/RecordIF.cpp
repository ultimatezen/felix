// RecordIF.cpp : Implementation of CRecord
#include "StdAfx.h"
#include "Felix.h"
#include "RecordIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRecord

#include "atlcom.h"

STDMETHODIMP CRecord::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IRecord ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IRecord ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IRecord ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IRecord ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE ;
}

STDMETHODIMP CRecord::get_Source(BSTR *pVal)
{
	// TODO: Add your implementation code here
	wstring wval = m_record->get_source() ;

	*pVal = ::SysAllocStringLen( wval.c_str(), wval.size() ) ;
	return S_OK ;
}

STDMETHODIMP CRecord::get_RichSource(BSTR *pVal)
{
	// TODO: Add your implementation code here
	wstring wval = m_record->get_rich_source() ;
	
	*pVal = ::SysAllocStringLen( wval.c_str(), wval.size() ) ;
	return S_OK ;
}

STDMETHODIMP CRecord::put_RichSource(BSTR newVal)
{
	CComBSTR bval( newVal ) ;
	m_record->set_rich_source( bval ) ;

	return S_OK;
}

STDMETHODIMP CRecord::get_Context(BSTR *pVal)
{
	// TODO: Add your implementation code here
	wstring wval = m_record->get_context() ;
	
	*pVal = ::SysAllocStringLen( wval.c_str(), wval.size() ) ;
	return S_OK ;
}

STDMETHODIMP CRecord::get_RichContext(BSTR *pVal)
{
	// TODO: Add your implementation code here
	wstring wval = m_record->get_rich_context() ;
	
	*pVal = ::SysAllocStringLen( wval.c_str(), wval.size() ) ;
	return S_OK ;
}

STDMETHODIMP CRecord::put_RichContext(BSTR newVal)
{
	CComBSTR bval( newVal ) ;
	m_record->set_rich_context( bval ) ;

	return S_OK;
}

STDMETHODIMP CRecord::get_DateCreated(DATE *pVal)
{
	SystemTimeToVariantTime( (SYSTEMTIME*)&m_record->get_created(), pVal ) ;

	return S_OK;
}

STDMETHODIMP CRecord::get_DateModified(DATE *pVal)
{
	SystemTimeToVariantTime( (SYSTEMTIME*)&m_record->get_modified(), pVal ) ;
	
	return S_OK;
}

STDMETHODIMP CRecord::get_IsValidated(BOOL *pVal)
{
	if ( m_record->is_validated() )
		*pVal = VARIANT_TRUE ;
	else *pVal = VARIANT_FALSE ;

	return S_OK;
}

STDMETHODIMP CRecord::put_IsValidated(BOOL newVal)
{
	if ( newVal == VARIANT_FALSE ) 
	{
		m_record->set_validated_on() ;
	}
	else
	{
		m_record->set_validated_off() ;
	}

	return S_OK;
}

STDMETHODIMP CRecord::get_Reliability(long *pVal)
{
	*pVal = m_record->get_reliability() ;

	return S_OK;
}

STDMETHODIMP CRecord::put_Reliability(long newVal)
{
	m_record->set_reliability( newVal ) ;

	return S_OK;
}

STDMETHODIMP CRecord::get_Properties(BSTR key, BSTR *pVal)
{
	wstring wkey( (LPCWSTR)key, SysStringLen( key ) ) ;

	if ( ! m_record->item_exists( wkey ) )
	{
		CComBSTR bval ;
		
		return bval.CopyTo( pVal ) ;
	}
	
	*pVal = ::SysAllocStringLen( wkey.c_str(), wkey.size() ) ;
	return S_OK ;
}

STDMETHODIMP CRecord::put_Properties(BSTR key, BSTR newVal)
{
	wstring wkey( (LPCWSTR)key, SysStringLen( key ) ) ;
	wstring wval( (LPCWSTR)newVal, SysStringLen( newVal ) ) ;

	m_record->set_item( wkey, wval ) ;

	return S_OK;
}

STDMETHODIMP CRecord::get_Translation(BSTR *pVal)
{
	// TODO: Add your implementation code here
	wstring wval = m_record->get_trans() ;
	
	*pVal = ::SysAllocStringLen( wval.c_str(), wval.size() ) ;
	return S_OK ;

}

STDMETHODIMP CRecord::get_RichTranslation(BSTR *pVal)
{
	// TODO: Add your implementation code here
	wstring wval = m_record->get_rich_trans() ;
	
	*pVal = ::SysAllocStringLen( wval.c_str(), wval.size() ) ;
	return S_OK ;
}

STDMETHODIMP CRecord::put_RichTranslation(BSTR newVal)
{
	CComBSTR bval( newVal ) ;
	m_record->set_rich_trans( bval ) ;

	return S_OK;
}
