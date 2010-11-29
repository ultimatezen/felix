// SortedListView.h: interface for the CSortedListView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTEDLISTVIEW_H__C9748526_F14B_4CBB_91F5_109C644F3C52__INCLUDED_)
#define AFX_SORTEDLISTVIEW_H__C9748526_F14B_4CBB_91F5_109C644F3C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "atlcrack.h"				// message crackers
#include "MessageMapDefines.h"		// for some extra message map defines
#include "atlctrls.h"
#include "Exceptions.h"
#include "stringex.h"
#include "CStringInterface.h"


#ifdef _CHEAT_VC_CLASSVIEW
#define __hide {
#define __end_hide }
#elif (!defined __hide) && (!defined __end_hide)
#define __hide
#define __end_hide
#endif

struct _list_sort_struct
{
	CListViewCtrl	*ctrl ;
	int				column ;
};

inline int CALLBACK SortTextAscending(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{
	static ATL::CString s1, s2 ;
	
	_list_sort_struct *sort_struct = (_list_sort_struct *)lParamSort ;
	
	CListViewCtrl	*sort_control = sort_struct->ctrl ;
	sort_control->GetItemText( (int)lParam1, sort_struct->column, s1 ) ;
	sort_control->GetItemText( (int)lParam2, sort_struct->column, s2 ) ;
	
	return ( 0 > str::lexical_compare( (LPCTSTR)s1, (LPCTSTR)s2 ) ) ;
}

inline int CALLBACK SortTextDescending(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{
	static CString s1, s2 ;
	
	_list_sort_struct *sort_struct = (_list_sort_struct *)lParamSort ;
	
	CListViewCtrl	*sort_control = sort_struct->ctrl ;
	sort_control->GetItemText( (int)lParam1, sort_struct->column, s1 ) ;
	sort_control->GetItemText( (int)lParam2, sort_struct->column, s2 ) ;
	
	return ( 0 < str::lexical_compare( (LPCTSTR)s1, (LPCTSTR)s2 ) ) ;
}

class CSortedListView  : 
public CWindowImpl<CSortedListView, CListViewCtrl > 
{
	int	m_sort_column ;
	int nColCount ;

public:
	CSortedListView();
	virtual ~CSortedListView();

	CString GetItemText( int row, int col ) ;
	// SortTextItems	- Sort the list based on column text
	// Returns		- Returns true for success
	// nCol			- column that contains the text to be sorted
	// bAscending		- indicate sort order
	BOOL SortTextItems( int nCol, BOOL bAscending )
	{
		nColCount = GetHeader().GetItemCount() ;
		
		if ( nCol >= nColCount ) 
			return FALSE;
		
		_list_sort_struct sort_struct ;
		
		sort_struct.ctrl = (CListViewCtrl*)this ;
		sort_struct.column = nCol ;
		
		if ( bAscending )
		{
			SortItemsEx( (PFNLVCOMPARE)SortTextAscending, (LPARAM)&sort_struct ) ;
		}
		else
		{
			SortItemsEx( (PFNLVCOMPARE)SortTextDescending, (LPARAM)&sort_struct ) ;
		}
		
		return TRUE ;
	}

__hide
	BEGIN_MSG_MAP_EX( CSortedListView )
		if( uMsg == WM_DROPFILES )
		{
			lResult = ::SendMessage( GetParent(), uMsg, wParam, lParam ) ;
			return TRUE ;
		}
	END_MSG_MAP()
__end_hide
			
	
};


#endif // !defined(AFX_SORTEDLISTVIEW_H__C9748526_F14B_4CBB_91F5_109C644F3C52__INCLUDED_)
