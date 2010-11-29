// SortedListView.cpp: implementation of the CSortedListView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortedListView.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortedListView::CSortedListView()
: m_sort_column(0)
{

}

CSortedListView::~CSortedListView()
{

}

CString CSortedListView::GetItemText( int row, int col )
{
	static ATL::CString item_text ;
	CListViewCtrl::GetItemText( row, col, item_text ) ;
	return item_text ;
}

