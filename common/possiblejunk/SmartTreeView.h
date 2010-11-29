// SortedTreeView.h: interface for the CSmartTreeView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTEDTREEVIEW_H__A0E2DA7B_7C77_4677_807B_427652974883__INCLUDED_)
#define AFX_SORTEDTREEVIEW_H__A0E2DA7B_7C77_4677_807B_427652974883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CStringInterface.h"

#include "atlcrack.h"				// message crackers
#include "MessageMapDefines.h"		// for some extra message map defines
#include "atlctrls.h"
#include "Exceptions.h"

#ifdef _CHEAT_VC_CLASSVIEW
#define __hide {
#define __end_hide }
#elif (!defined __hide) && (!defined __end_hide)
#define __hide
#define __end_hide
#endif


//typedef CTreeItemT< ATL::CWindow >		tree_item_type ;

class CSmartTreeView : 
public CWindowImpl<CSmartTreeView, CTreeViewCtrlEx>
{
	int	m_active_subitem ;
public:
	bool SortItems( CTreeItem &item, bool is_recursive = true );

	DECLARE_WND_SUPERCLASS(NULL, CTreeViewCtrl::GetWndClassName())
	
	CSmartTreeView();
	virtual ~CSmartTreeView();
	
	BOOL PreTranslateMessage(MSG* pMsg) 
	{
		pMsg ;
		return FALSE ; 
	}

	
__hide
	BEGIN_MSG_MAP_EX(CSmartTreeView)
		
	if( uMsg == UWM_USER_MESSAGE )
	{
		lResult = ::SendMessage( GetParent(), UWM_USER_MESSAGE, wParam, lParam ) ;
		return TRUE ;
	}
		
	DEFAULT_REFLECTION_HANDLER( )
			
	END_MSG_MAP()
__end_hide
			
};

#endif // !defined(AFX_SORTEDTREEVIEW_H__A0E2DA7B_7C77_4677_807B_427652974883__INCLUDED_)
