/*!
	@file ControlWindows.h
	@brief Interfaces for custom control classes
	@date 2005/06/25
	Time: 13:24:06
	@author Ryan Ginstrom
 */

#pragma once

#include "atlcrack.h"		// message crackers
#include "MessageMapDefines.h" // for custom message map stuff

#include "atlctrls.h"

#pragma warning( disable:4995 ) // #pragma deprecated
#include "atlctrlw.h"
#pragma warning( default:4995 ) // #pragma deprecated

#include "atlctrlx.h"			// CPaneContainer
#include "atlsplit.h"			// CSplitterWindow
#include "StringEx.h"			// extended string/textstream support
#include "StringConversions.h"	// convert to/from wstrings/strings/tstrings


/**
	@class CMyTreeView 
	@brief Custom TreeView control.
	Has DeleteAll method
 */
class CMyTreeView 
	: public CWindowImpl< CMyTreeView, CTreeViewCtrlEx >
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CTreeViewCtrl::GetWndClassName())

	void DeleteAll( HTREEITEM item = TVI_ROOT )
	{
		if ( item != TVI_ROOT )
		{

			HTREEITEM sibling = GetNextSiblingItem( item ) ;

			if ( sibling != NULL )
				DeleteAll( sibling ) ;
		}

		HTREEITEM child = GetChildItem( item ) ;

		if ( child != NULL )
			DeleteAll( child ) ;

		DeleteItem( item ) ;
	}


	BEGIN_MSG_MAP(CMyTreeView)
	END_MSG_MAP()

} ;

/**
	@class CMyListView 
	@brief Customized ListView control.
 */
class CMyListView 
	: 
	public CWindowImpl<CMyListView, CListViewCtrl>
{
	// for fetching item text

public:

	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	BEGIN_MSG_MAP(CMyListView)

	END_MSG_MAP()

	// GetFirstSelected
	// Parameters
	//		none
	// Return
	//		int		-- index of first selected item, -1 if no items selected
	int GetFirstSelected( void )
	{
		int count = GetItemCount() ;
		for ( int i = 0 ; i < count ; i++ )
		{
			if ( IsSelected( i ) )
				return i ;
		}
		return -1 ;
	}
	bool IsSelected( int nIndex )
	{
		return ( ( GetItemState( nIndex, LVIS_SELECTED ) & LVIS_SELECTED ) != 0 ) ;
	}
	tstring GetItemText(int nIndex, int pos = 0)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		str::tbuffer buff ;
		LVITEM lvi;
		memset(&lvi, 0, sizeof(LVITEM));
		lvi.iSubItem = pos;

		int nRes = 0;
		for(int nLen = 256; ; nLen *= 2)
		{
			lvi.cchTextMax = nLen;
			lvi.pszText = buff.buffer( nLen ) ;
			if(lvi.pszText == NULL)
			{
				nRes = 0;
				break;
			}
			nRes  = (int)::SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM)nIndex, (LPARAM)&lvi);
			if(nRes < nLen - 1)
				break;
		}
		return buff.str() ;
	}
	int GetItemInt(int nIndex, int pos)
	{
		return boost::lexical_cast< long >( GetItemText( nIndex, pos) ) ;
	}
} ;

