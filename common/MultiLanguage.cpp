// MultiLanguage.cpp: implementation of the CMultiLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MultiLanguage.h"
#include "Exceptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

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

UINT CMultiLanguage::convert_to_unicode( char *source_str, UINT str_size, wchar_t *dest_str, UINT dest_size, DWORD dwEncoding /*= CP_UTF8 */ )
{
	DWORD dwMode = 0 ;

	UINT in_size(str_size) ;
	if( in_size == -1 )
	{
		in_size = static_cast< UINT >( strlen( source_str ) ) ;
	}

	HRESULT hr = m_multi->ConvertStringToUnicode( &dwMode, 
								dwEncoding, 
								source_str, &in_size, 
								dest_str, &dest_size ) ;
	if ( FAILED( hr ) )
	{
		throw except::CComException( _T("Failed to convert string to utf-16"), hr ) ;
	}

	return dest_size ;
}

LPBYTE CMultiLanguage::convert_to_utf8( LPBYTE source_str, UINT source_size, LPBYTE dest_str, UINT dest_size, DWORD dwEncoding /*= CP_UNICODE_BE */ )
{
	DWORD mode(0) ;
	HRESULT hr = m_multi->ConvertString(&mode, 
							dwEncoding, 
							CP_UTF8, 
							source_str, &source_size, 
							dest_str, &dest_size) ;

	if ( FAILED( hr ) )
	{
		throw except::CComException( _T("Failed to convert string to utf-8"), hr ) ;
	}
	return dest_str ;
}