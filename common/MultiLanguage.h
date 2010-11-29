/*!
	@file MultiLanguage.h
	@brief interface for the CMultiLanguage class.
	@date 2005/06/25
	Time: 15:27:48
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#include "StringEx.h"
#include "Exceptions.h"
#include "mlang.h"

const static int CP_UNICODE = 1200 ;
const static int CP_UNICODE_BE = 1201 ;

/**
	@class CMultiLanguage  
	@brief Wrapper for IMultiLanguage2.
 */
class CMultiLanguage  
{
	CComPtr< IMultiLanguage2 >	m_multi ;

public:
	CMultiLanguage();
	virtual ~CMultiLanguage();
	DetectEncodingInfo detect_encoding( LPCSTR text, const int str_size = -1, const DWORD flags = 0, const DWORD pref_codepage = 0 );

	LPBYTE convert_to_utf8( LPBYTE source_str, 
		UINT source_size, 
		LPBYTE dest_str, 
		UINT dest_size, 
		DWORD dwEncoding = CP_UNICODE_BE );

	UINT convert_to_unicode( char *source_str, 
						     UINT str_size, 
							 wchar_t *dest_str, 
							 UINT dest_size, 
							 DWORD dwEncoding = CP_UTF8 );
};

