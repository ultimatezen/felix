/*!
	@file Picture.h
	@brief Interface file for CPicture
	@date 2005/06/29
	Time: 22:04:05
	@author Ryan Ginstrom
	@dir C:\Programming\MyLibrary
 */

#pragma once

#include "atlfile.h" // CAtlFile
#include "atltypes.h" // CRect

/**
	@class CPicture 
	@brief Wrapper for IPicture.
 */
class CPicture 
{
public:
	CPicture();
	~CPicture();

	// Load frm various sources
	BOOL Load(UINT nIDRes) ;
	BOOL Load(LPCTSTR pszPathName) ;
	BOOL Load(CAtlFile& file );
	BOOL Load(IStream* pstm) ;

	// render to device context
	BOOL Render(HDC hDC, CRect rc=CRect(0,0,0,0),
		LPCRECT prcMFBounds=NULL) const;

	CSize GetImageSize(HDC hDC=NULL) const;

	operator IPicture*() 
	{
		return m_spIPicture;
	}

	void GetHIMETRICSize(OLE_XSIZE_HIMETRIC& cx, OLE_YSIZE_HIMETRIC& cy) const 
	{
		cx = cy = 0;
		const_cast<CPicture*>(this)->m_hr =
			m_spIPicture->get_Width(&cx);
		ATLASSERT(SUCCEEDED(m_hr));
		const_cast<CPicture*>(this)->m_hr =
			m_spIPicture->get_Height(&cy);
		ATLASSERT(SUCCEEDED(m_hr));
	}

	void Free() 
	{
		if (m_spIPicture) 
		{
			m_spIPicture.Release();
		}
	}

private:
	CComPtr< IStream > GetIStreamFromFile(CAtlFile & file) const;
	CComPtr< IStream > GetIStreamFromFile(LPCTSTR pszPathName) const;
	CComPtr< IStream > GetIStreamFromMemory(BYTE * pMem, DWORD len)
		const;
	void HIMETRICtoDP(HDC hDC, LPSIZE lpSize) const;

protected:
	CComQIPtr<IPicture> m_spIPicture;	 // ATL smart pointer to IPicture
	HRESULT m_hr;								// last error code
} ;


