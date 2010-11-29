#include "stdafx.h"
#include "picture.h"
#include "logging.h"

CPicture::CPicture()
{
}

CPicture::~CPicture()
{
}

//////////////////
// Load from resource. Looks for "IMAGE" type.
//
BOOL CPicture::Load(UINT nIDRes)
{
	// find resource in resource file
	HINSTANCE hInst = _AtlModule.GetResourceInstance();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nIDRes), _T("IMAGE") ) ; // type
	if (!hRsrc)
	{
		return FALSE;
	}

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
	{
		return FALSE;
	}

	// create memory file and load it
	CComPtr< IStream > spStream = GetIStreamFromMemory( lpRsrc, len );

	return Load( spStream );
}

//////////////////
// Load from path name.
//
BOOL CPicture::Load(LPCTSTR pszPathName)
{
	CComPtr< IStream > pStream = GetIStreamFromFile( pszPathName );
	return Load(pStream);
}

//////////////////
// Load from CFile
//
BOOL CPicture::Load(CAtlFile& file)
{
	CComPtr< IStream > pStream = GetIStreamFromFile( file );
	return Load(pStream);
}

//////////////////
// Load from stream (IStream). This is the one that really does it: call
// OleLoadPicture to do the work.
//
BOOL CPicture::Load(IStream* pstm)
{
	Free();
	m_hr = OleLoadPicture(pstm, 0, FALSE,
		IID_IPicture, (void**)&m_spIPicture);

	return SUCCEEDED( m_hr ) ;
}

//////////////////
// Render to device context. Covert to HIMETRIC for IPicture.
//
BOOL CPicture::Render(HDC hDC, CRect rc, LPCRECT prcMFBounds) const
{
	ATLASSERT(hDC);

	if (rc.IsRectNull()) {
		CSize sz = GetImageSize(hDC);
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	long hmWidth,hmHeight; // HIMETRIC units
	GetHIMETRICSize(hmWidth, hmHeight);
	m_spIPicture->Render(hDC, rc.left, rc.top, rc.Width(), rc.Height(),
		0, hmHeight, hmWidth, -hmHeight, prcMFBounds);

	return TRUE;
}

//////////////////
// Get image size in pixels. Converts from HIMETRIC to device coords.
//
CSize CPicture::GetImageSize(HDC hDC) const
{
	if (!m_spIPicture)
		return CSize(0,0);

	LONG hmWidth, hmHeight; // HIMETRIC units
	m_spIPicture->get_Width(&hmWidth);
	m_spIPicture->get_Height(&hmHeight);
	CSize sz(hmWidth,hmHeight);
	if (hDC==NULL)
		hDC = ::GetWindowDC( NULL );
	if (hDC)
	{
		HIMETRICtoDP(hDC, &sz);
	}
	return sz;
}

CComPtr< IStream > CPicture::GetIStreamFromFile(CAtlFile & file) const
{
	ULONGLONG ull;
	if (SUCCEEDED(file.GetSize(ull)))
	{
		boost::scoped_array<BYTE> buff(new BYTE[boost::numeric_cast<unsigned int>(ull)]) ;

		return GetIStreamFromMemory( buff.get(), DWORD(ull) );
	}

	return NULL;
}

CComPtr< IStream > CPicture::GetIStreamFromFile(LPCTSTR pszPathName) const
{
	ATLASSERT(pszPathName != NULL) ;

	CAtlFile file;
	if (SUCCEEDED(file.Create( pszPathName, GENERIC_READ,
		FILE_SHARE_READ,
		OPEN_EXISTING )))
	{
		return GetIStreamFromFile( file );
	}
	else
	{
		if (pszPathName != NULL)
		{
			logging::log_error("Failed to get IStream from file: " + string(CT2A(pszPathName))) ;
		}
		return NULL;
	}
}

CComPtr< IStream > CPicture::GetIStreamFromMemory(BYTE * pb, DWORD len)
const
{
	ATLTRACE("CPicture::GetIStreamFromMemory\n") ;
	HGLOBAL const hg = ::GlobalAlloc( GHND, len );
	if (hg)
	{
		BYTE * pbNew = static_cast< BYTE * > (::GlobalLock( hg ));
		if (pbNew)
		{
			std::copy( pb, pb + len, pbNew );

			::SetLastError( NO_ERROR );
			if (::GlobalUnlock( hg ) == 0 && NO_ERROR ==
				::GetLastError())
			{
				CComPtr< IStream > spStream;
				::CreateStreamOnHGlobal( hg, TRUE, &spStream
					);
				return spStream;
			}
			if (NO_ERROR != ::GetLastError())
			{
				throw except::CWinException(CString("GetIStreamFromMemory - Failed to create GlobalAlloc")) ;
			}
		}
	}

	return NULL;
}

void CPicture::HIMETRICtoDP(HDC hDC, LPSIZE lpSize) const
{
	int nMapMode = GetMapMode( hDC );
	if (nMapMode < MM_ISOTROPIC &&
		nMapMode != MM_TEXT)
	{
		// when using a constrained map mode, map against physical inch
			SetMapMode(hDC, MM_HIMETRIC);
		POINT pt = { 0 };
		LPtoDP(hDC, &pt, 1);
		SetMapMode(hDC, nMapMode);
		lpSize->cx = pt.x;
		lpSize->cy = pt.y;
	}
	else
	{
		// map against logical inch for non-constrained mapping modes
			int cxPerInch, cyPerInch;
		ATLASSERT(hDC != NULL);
		cxPerInch = GetDeviceCaps(hDC, LOGPIXELSX);
		cyPerInch = GetDeviceCaps(hDC, LOGPIXELSY);
		ATLASSERT(cxPerInch != 0 && cyPerInch != 0);
		lpSize->cx = MulDiv(lpSize->cx, cxPerInch, 2540);
		lpSize->cy = MulDiv(lpSize->cy, cyPerInch, 2540);
	}
}
