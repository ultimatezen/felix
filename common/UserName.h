/*!
	@file UserName.h
	@brief Interface for the CUserName class.
	@date 2005/06/25
	Time: 15:34:25
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "StringEx.h"

/**
	@class CUserName
	@brief Wrapper for the GetUserName API function.
 */
class CUserName
{
	CString m_user_name ;

public:

	CUserName(void)
	{
		DWORD path_len = MAX_PATH ;
		::GetUserName( m_user_name.GetBuffer( MAX_PATH ), &path_len ) ;
		m_user_name.ReleaseBuffer() ;
	}

	operator wstring() const
	{
		return as_wstring() ;
	}

	wstring as_wstring() const
	{
		ATLASSERT ( ! m_user_name.IsEmpty() ) ; 
		wstring user_name = CT2W( m_user_name ) ;
		return user_name ;
	}
	string as_string() const
	{
		ATLASSERT ( ! m_user_name.IsEmpty() ) ; 
		wstring user_name = CT2W( m_user_name ) ;
		return string2string( user_name, CP_UTF8 ) ;
	}
};
