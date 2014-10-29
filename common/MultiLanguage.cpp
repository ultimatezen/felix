// MultiLanguage.cpp: implementation of the CMultiLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MultiLanguage.h"
#include "Exceptions.h"

using namespace except ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiLanguage::CMultiLanguage()
{
	// CLSID_CMultiLanguage
	HRESULT hr = m_multi.CoCreateInstance( CLSID_CMultiLanguage ) ;

	if ( FAILED( hr ) )
	{
		throw CComException( _T("Failed to create Multilanguage object"), hr ) ;
	}
}

CMultiLanguage::~CMultiLanguage()
{

}

DetectEncodingInfo CMultiLanguage::detect_encoding( LPCSTR text, const int str_size /*= -1*/, const DWORD flags /*= 0*/, const DWORD pref_codepage /*= 0 */ )
{
	int in_size(str_size) ;
	if( in_size == -1 )
		in_size = static_cast< int >( strlen( text ) ) ;

	const int INFO_SIZE = 2 ;
	DetectEncodingInfo info[INFO_SIZE] ;
	int num_structs = INFO_SIZE ;
	HRESULT hr = m_multi->DetectInputCodepage( flags, pref_codepage, const_cast<LPSTR>(text), &in_size, info, &num_structs ) ;

	if ( FAILED( hr ) )
	{
		throw except::CComException( _T("Failed to detect input codepage"), hr ) ;
	}

	return info[0] ;
}

