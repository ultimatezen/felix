// MiniVersion.cpp  Version 1.1
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
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiniVersion.h"
#include "stdio.h"

#include "DebugUtilities.h"


#pragma message("\tautomatic link to VERSION.LIB")
#pragma comment(lib, "version.lib")


/** Gets the version information for the file at lpszPath.
 */
CMiniVersion::CMiniVersion(LPCSTR lpszPath)
{
	ZeroMemory(m_szPath, sizeof(m_szPath));

	if (lpszPath && lpszPath[0] != 0)
	{
		strncpy_s(m_szPath, MAX_PATH*2, lpszPath, sizeof(m_szPath)-1);
	}

	//  Prefer initialization to assignment in constructors
	m_pData = NULL;
	m_dwHandle = 0;

	for (int i = 0; i < 4; i++)
	{
		m_wFileVersion[i] = 0;
		m_wProductVersion[i] = 0;
	}

	m_dwFileFlags = 0;
	m_dwFileOS = 0;
	m_dwFileType = 0;
	m_dwFileSubtype = 0;

	ZeroMemory(m_szCompanyName, sizeof(m_szCompanyName));
	ZeroMemory(m_szProductName, sizeof(m_szProductName));
	ZeroMemory(m_szFileDescription, sizeof(m_szFileDescription));

	Init();
}


/** Initializes the version information, given the path at m_szPath.
 */
BOOL CMiniVersion::Init()
{
	DWORD dwHandle;
	DWORD dwSize;
	BOOL rc;

	dwSize = ::GetFileVersionInfoSizeA((LPSTR)(LPCSTR)m_szPath, &dwHandle);
	if (dwSize == 0)
		return FALSE;
		
	m_pData = new BYTE [dwSize + 1];	
	ZeroMemory(m_pData, dwSize+1);

	rc = ::GetFileVersionInfoA((LPSTR)(LPCSTR)m_szPath, dwHandle, dwSize, m_pData);
	if (!rc)
		return FALSE;

	// get fixed info

//#pragma warning( disable:4701 ) // local variable may be used without having been initialized

	VS_FIXEDFILEINFO FixedInfo = {0};
	
	if (GetFixedInfo(FixedInfo))
	{
		m_wFileVersion[0] = HIWORD(FixedInfo.dwFileVersionMS);
		m_wFileVersion[1] = LOWORD(FixedInfo.dwFileVersionMS);
		m_wFileVersion[2] = HIWORD(FixedInfo.dwFileVersionLS);
		m_wFileVersion[3] = LOWORD(FixedInfo.dwFileVersionLS);

		m_wProductVersion[0] = HIWORD(FixedInfo.dwProductVersionMS);
		m_wProductVersion[1] = LOWORD(FixedInfo.dwProductVersionMS);
		m_wProductVersion[2] = HIWORD(FixedInfo.dwProductVersionLS);
		m_wProductVersion[3] = LOWORD(FixedInfo.dwProductVersionLS);

		m_dwFileFlags   = FixedInfo.dwFileFlags;
		m_dwFileOS      = FixedInfo.dwFileOS;
		m_dwFileType    = FixedInfo.dwFileType;
		m_dwFileSubtype = FixedInfo.dwFileSubtype;
	}
	else
		return FALSE;

//#pragma warning( default:4701 ) // local variable may be used without having been initialized

	// get string info

	GetStringInfo("CompanyName",     m_szCompanyName);
	GetStringInfo("FileDescription", m_szFileDescription);
	GetStringInfo("ProductName",     m_szProductName);

	return TRUE;		
}

///////////////////////////////////////////////////////////////////////////////
// Release
void CMiniVersion::Release()
{
	// do this manually, because we can't use objects requiring
	// a dtor within an exception handler
	if (m_pData)
		delete [] m_pData;
	m_pData = NULL;
}


///////////////////////////////////////////////////////////////////////////////
// GetFileVersion
BOOL CMiniVersion::GetFileVersion(WORD * pwVersion)
{
	ATLASSERT(FALSE && "Buggy! DO NOT USE!!!") ;
	for (int i = 0; i < 4; i++)
		*pwVersion++ = m_wFileVersion[i];
	return TRUE;
}					 	 

///////////////////////////////////////////////////////////////////////////////
// GetProductVersion
BOOL CMiniVersion::GetProductVersion(WORD * pwVersion)
{
	for (int i = 0; i < 4; i++)
		*pwVersion++ = m_wProductVersion[i];
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetFileFlags
/*!
 * \brief
 * Write brief comment for GetFileFlags here.
 * 
 * \param rdwFlags
 * Description of parameter rdwFlags.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetFileFlags here.
 * 
 * \remarks
 * Write remarks for GetFileFlags here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetFileFlags(DWORD& rdwFlags)
{
	rdwFlags = m_dwFileFlags;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetFileOS
/*!
 * \brief
 * Write brief comment for GetFileOS here.
 * 
 * \param rdwOS
 * Description of parameter rdwOS.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetFileOS here.
 * 
 * \remarks
 * Write remarks for GetFileOS here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetFileOS(DWORD& rdwOS)
{
	rdwOS = m_dwFileOS;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetFileType
/*!
 * \brief
 * Write brief comment for GetFileType here.
 * 
 * \param rdwType
 * Description of parameter rdwType.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetFileType here.
 * 
 * \remarks
 * Write remarks for GetFileType here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetFileType(DWORD& rdwType)
{
	rdwType = m_dwFileType;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetFileSubtype
/*!
 * \brief
 * Write brief comment for GetFileSubtype here.
 * 
 * \param rdwType
 * Description of parameter rdwType.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetFileSubtype here.
 * 
 * \remarks
 * Write remarks for GetFileSubtype here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetFileSubtype(DWORD& rdwType)
{
	rdwType = m_dwFileSubtype;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetCompanyName
/*!
 * \brief
 * Write brief comment for GetCompanyName here.
 * 
 * \param lpszCompanyName
 * Description of parameter lpszCompanyName.
 * 
 * \param nSize
 * Description of parameter nSize.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetCompanyName here.
 * 
 * \remarks
 * Write remarks for GetCompanyName here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetCompanyName(LPSTR lpszCompanyName, int nSize)
{
	if (!lpszCompanyName)
		return FALSE;
	ZeroMemory(lpszCompanyName, nSize);
	strncpy_s(lpszCompanyName, MAX_PATH*2, m_szCompanyName, nSize-1);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetFileDescription
/*!
 * \brief
 * Write brief comment for GetFileDescription here.
 * 
 * \param lpszFileDescription
 * Description of parameter lpszFileDescription.
 * 
 * \param nSize
 * Description of parameter nSize.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetFileDescription here.
 * 
 * \remarks
 * Write remarks for GetFileDescription here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetFileDescription(LPSTR lpszFileDescription, int nSize)
{
	if (!lpszFileDescription)
		return FALSE;
	ZeroMemory(lpszFileDescription, nSize);
	strncpy_s(lpszFileDescription, MAX_PATH*2, m_szFileDescription, nSize-1);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetProductName
/*!
 * \brief
 * Write brief comment for GetProductName here.
 * 
 * \param lpszProductName
 * Description of parameter lpszProductName.
 * 
 * \param nSize
 * Description of parameter nSize.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetProductName here.
 * 
 * \remarks
 * Write remarks for GetProductName here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetProductName(LPSTR lpszProductName, int nSize)
{
	if (!lpszProductName)
		return FALSE;
	ZeroMemory(lpszProductName, nSize);
	strncpy_s(lpszProductName, MAX_PATH*2, m_szProductName, nSize-1);
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// protected methods
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// GetFixedInfo
/*!
 * \brief
 * Write brief comment for GetFixedInfo here.
 * 
 * \param rFixedInfo
 * Description of parameter rFixedInfo.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetFixedInfo here.
 * 
 * \remarks
 * Write remarks for GetFixedInfo here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetFixedInfo(VS_FIXEDFILEINFO& rFixedInfo)
{
	BOOL rc;
	UINT nLength;
	VS_FIXEDFILEINFO *pFixedInfo = NULL;

	if (!m_pData)
		return FALSE;

	if (m_pData)
		rc = ::VerQueryValue(m_pData, _T("\\"), (void **) &pFixedInfo, &nLength);
	else
		rc = FALSE;
		
	if (rc)
		memcpy (&rFixedInfo, pFixedInfo, sizeof (VS_FIXEDFILEINFO));	

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// GetStringInfo
/*!
 * \brief
 * Write brief comment for GetStringInfo here.
 * 
 * \param lpszKey
 * Description of parameter lpszKey.
 * 
 * \param lpszValue
 * Description of parameter lpszValue.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for GetStringInfo here.
 * 
 * \remarks
 * Write remarks for GetStringInfo here.
 * 
 * \see
 * Separate items with the '|' character.
 */
BOOL CMiniVersion::GetStringInfo(LPCSTR lpszKey, LPSTR lpszReturnValue)
{
	BOOL rc;
	DWORD *pdwTranslation;
	UINT nLength;
	LPSTR lpszValue;
	
	if (m_pData == NULL)
		return FALSE;

	if (!lpszReturnValue)
		return FALSE;

	if (!lpszKey)
		return FALSE;

	*lpszReturnValue = 0;

	rc = ::VerQueryValueA(m_pData, "\\VarFileInfo\\Translation", 
								(void**) &pdwTranslation, &nLength);
	if (!rc)
		return FALSE;

	format key = format("\\StringFileInfo\\%04x%04x\\%s") 
		% LOWORD (*pdwTranslation)
		% HIWORD (*pdwTranslation)
		% lpszKey ;

	rc = ::VerQueryValueA(m_pData, key.str().c_str(), (void**) &lpszValue, &nLength);

	if (!rc)
		return FALSE;
		
	strcpy_s(lpszReturnValue, MAX_PATH*2, lpszValue);

	return TRUE;
}
