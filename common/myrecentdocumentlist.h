/*!
	@file MyRecentDocumentList.h
	@brief Interfaces for customized recent docts lists.
	@date 2005/06/25
	Time: 13:24:43
	@author Ryan Ginstrom
 */

#pragma once

#include <atlapp.h>				// must be included before atlmisc.h
#include "atlmisc.h"
#include "StringEx.h"

/**
	@class CVariableLocationRecentDocumentList
	@brief This allows us to have more than one recent docs list location -- like if we have recent docs and recent folders.
 */
template< int t_nFirstID = ID_FILE_MRU_FIRST, int t_nLastID = ID_FILE_MRU_LAST >
class CVariableLocationRecentDocumentList  : 
	public CRecentDocumentListBase< CVariableLocationRecentDocumentList<t_nFirstID, t_nLastID>, MAX_PATH*2, t_nFirstID, t_nLastID >
{
public:
	virtual ~CVariableLocationRecentDocumentList()
	{
	}
	static bool CompactDocumentName(LPTSTR lpstrOut, LPCTSTR lpstrIn, int cchLen)
	{
		_tcscpy_s( lpstrOut, MAX_PATH*2, lpstrIn ) ;
		if ( ! ::PathCompactPath( ::GetWindowDC( ::GetActiveWindow() ), lpstrOut, cchLen ) )
		{
			return FALSE != ::PathCompactPath( ::GetWindowDC( ::GetActiveWindow() ), lpstrOut, cchLen + ( cchLen / 2 ) ) ;
		}
		return true ;
	}
	BOOL AddStringToList(const tstring &list_item)
	{
		return AddToList( list_item.c_str() ) ;
	}
	
};

/**
	@class CMyRecentDocumentList 
	@brief Customize recent docs list.
 */
class CMyRecentDocumentList 
	: public CVariableLocationRecentDocumentList< ID_FILE_MRU_FIRST,  ID_FILE_MRU_LAST >
{
public:
	CMyRecentDocumentList(){}
	virtual ~CMyRecentDocumentList(){}
};


