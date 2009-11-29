// ClipboardBackup.cpp: implementation of the CClipboardBackup class.
//
// Copyright 2006 (c) RegExLab.com
//
// Author: Ê·ÊÙÎ° (sswater shi)
//
// 2006/05/20 02:03:04
//

#include "stdafx.h"
#include "ClipboardBackup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CClipboardBackup::CClipboardBackup()
{
	this->Backup() ;
}

CClipboardBackup::~CClipboardBackup()
{
	this->Restore() ;
}

BOOL CClipboardBackup::Backup()
{
	if( ! m_lstData.empty()  )
		return FALSE;

	if( ! ::OpenClipboard(NULL) )
		return FALSE;

	UINT format = 0;
	while( (format = ::EnumClipboardFormats(format)) != 0 )
	{
		ClipboardData data;
		data.m_nFormat = format;

		// skip some formats
		if( format == CF_BITMAP || format == CF_METAFILEPICT || format == CF_PALETTE || format == CF_OWNERDISPLAY ||
			format == CF_DSPMETAFILEPICT || format == CF_DSPBITMAP ||
			( format >= CF_PRIVATEFIRST && format <= CF_PRIVATELAST ) )
		{
			continue;
		}

		// get format name
		if( format <= 14 )
			data.m_szFormatName[0] = 0;
		else if( GetClipboardFormatName(format, data.m_szFormatName, 100) == 0 )
			data.m_szFormatName[0] = 0;

		// get handle
		HANDLE hMem = ::GetClipboardData( format );
		if( hMem == NULL )
			continue;

		// copy handle
		switch( format )
		{
		case CF_ENHMETAFILE:
		case CF_DSPENHMETAFILE:
			data.m_hData = ::CopyMetaFile((HMETAFILE)hMem, NULL);
			break;

		default:
			{
				int    size = ::GlobalSize(hMem);
				LPVOID pMem = ::GlobalLock( hMem );

				data.m_hData   = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, size );
				LPVOID pNewMem = ::GlobalLock( data.m_hData );

				memcpy(pNewMem, pMem, size);

				::GlobalUnlock(hMem);
				::GlobalUnlock(data.m_hData);
			}
		}

		m_lstData.push_back(data) ;
	}

	::CloseClipboard();

	return TRUE;
}

BOOL CClipboardBackup::Restore()
{
	if( m_lstData.empty() )
		return FALSE;

	if( ! ::OpenClipboard(NULL) )
		return FALSE;

	::EmptyClipboard();

	for ( ClipListType::iterator pos = m_lstData.begin() ; pos != m_lstData.end() ; ++pos )
	{
		ClipboardData & data = *pos ;

		UINT format = data.m_nFormat;

		if( data.m_szFormatName[0] != 0 )
		{
			UINT u = RegisterClipboardFormat( data.m_szFormatName );
			if( u > 0 ) format = u;
		}

		::SetClipboardData( format, data.m_hData );
	}

	::CloseClipboard();

	m_lstData.clear();

	return TRUE;
}
