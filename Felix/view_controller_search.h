// view_controller_search.h: interface for the view_controller_search class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_CONTROLLER_SEARCH_H__FCE1EC12_E568_4529_8A42_1B7F8FD064AB__INCLUDED_)
#define AFX_VIEW_CONTROLLER_SEARCH_H__FCE1EC12_E568_4529_8A42_1B7F8FD064AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "view_controller.h"

class view_controller_search : public view_controller  
{
public:
	view_controller_search();
	virtual ~view_controller_search();

};

//////////////////////////////////////////////////////////////////////////

class view_controller_search_mainframe : public view_controller_search  
{
	search_query_mainframe				m_search_matches ;

public:
	view_controller_search_mainframe();
	virtual ~view_controller_search_mainframe();
	
};

//////////////////////////////////////////////////////////////////////////

class view_controller_search_gloss : public view_controller_search  
{
	search_query_glossary	m_search_matches ;

public:
	view_controller_search_gloss();
	virtual ~view_controller_search_gloss();
	
};


#endif // !defined(AFX_VIEW_CONTROLLER_SEARCH_H__FCE1EC12_E568_4529_8A42_1B7F8FD064AB__INCLUDED_)
