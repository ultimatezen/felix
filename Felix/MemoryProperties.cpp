// MemoryProperties.cpp : Implementation of CMemoryProperties
#include "StdAfx.h"
#include "Felix.h"
#include "MemoryProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMemoryProperties

STDMETHODIMP CMemoryProperties::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IMemoryProperties ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IMemoryProperties ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IMemoryProperties ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IMemoryProperties ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE ;

}

STDMETHODIMP CMemoryProperties::get_MatchingAlgorithm(short *pVal)
{
	*pVal = (short)m_algo_props.m_data.m_match_algo ;

	return S_OK;
}

STDMETHODIMP CMemoryProperties::put_MatchingAlgorithm(short newVal)
{
	ATLASSERT( newVal == IDC_ALGO_CHAR || newVal == IDC_ALGO_WORD ) ;

	m_algo_props.m_data.m_match_algo = newVal ;

	m_algo_props.write_to_registry() ;

	get_mainframe().reflect_preferences() ;

	return S_OK;
}
