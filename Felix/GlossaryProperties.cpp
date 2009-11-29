// GlossaryProperties.cpp : Implementation of CGlossaryProperties
#include "StdAfx.h"
#include "Felix.h"
#include "GlossaryProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlossaryProperties



STDMETHODIMP CGlossaryProperties::get_GetEntriesAsPlaintext(BOOL *pVal)
{
	if ( m_props.m_data.m_plaintext )
	{
		*pVal = VARIANT_TRUE ;
	}
	else
	{
		*pVal = VARIANT_FALSE ;
	}

	return S_OK;
}

STDMETHODIMP CGlossaryProperties::put_GetEntriesAsPlaintext(BOOL newVal)
{
	if ( ! newVal )
	{
		m_props.m_data.m_plaintext = false ;
	}
	else
	{
		m_props.m_data.m_plaintext = true ;
	}

	m_props.write_to_registry() ;

	get_mainframe().reflect_preferences() ;
	
	return S_OK;
}
