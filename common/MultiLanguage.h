/*!
	@file MultiLanguage.h
	@brief interface for the CMultiLanguage class.
	@date 2005/06/25
	Time: 15:27:48
	@author Ryan Ginstrom
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
};

