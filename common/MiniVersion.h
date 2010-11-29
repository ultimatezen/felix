/*!
	@file MiniVersion.h
	@brief MiniVersion.h  Version 1.1.
	@date 2005/06/25
	Time: 15:25:02
	@author Ryan Ginstrom
 */

/** 
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like, except
// that you may not sell this source code.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
*/

#pragma once

#pragma warning( disable:4005 ) // macro redefinition

/**
	@class CMiniVersion
	@brief Wrapper for mini version info.
 */
class CMiniVersion
{
// constructors
public:
	CMiniVersion(LPCSTR lpszPath = NULL);
	BOOL Init();
	void Release();

// operations
public:

// attributes
public:
	// fixed info
	BOOL GetFileVersion(WORD *pwVersion);
	BOOL GetProductVersion(WORD* pwVersion);
	BOOL GetFileFlags(DWORD& rdwFlags);
	BOOL GetFileOS(DWORD& rdwOS);
	BOOL GetFileType(DWORD& rdwType);
	BOOL GetFileSubtype(DWORD& rdwType);	

	// string info
	BOOL GetCompanyName(LPSTR lpszCompanyName, int nSize);
	BOOL GetFileDescription(LPSTR lpszFileDescription, int nSize);
	BOOL GetProductName(LPSTR lpszProductName, int nSize);

// implementation
protected:
	BOOL GetFixedInfo(VS_FIXEDFILEINFO& rFixedInfo);
	BOOL GetStringInfo(LPCSTR lpszKey, LPSTR lpszValue);

	BYTE*		m_pData;
	DWORD		m_dwHandle;
	WORD		m_wFileVersion[4];
	WORD		m_wProductVersion[4];
	DWORD		m_dwFileFlags;
	DWORD		m_dwFileOS;
	DWORD		m_dwFileType;
	DWORD		m_dwFileSubtype;

	char		m_szPath[MAX_PATH*2];
	char		m_szCompanyName[MAX_PATH*2];
	char		m_szProductName[MAX_PATH*2];
	char		m_szFileDescription[MAX_PATH*2];
};

