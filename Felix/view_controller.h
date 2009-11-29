// view_controller.h: interface for the view_controller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_CONTROLLER_H__191BD9B0_0F6D_426C_AF3E_9B15C1ABA8B6__INCLUDED_)
#define AFX_VIEW_CONTROLLER_H__191BD9B0_0F6D_426C_AF3E_9B15C1ABA8B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TranslationMemory.h"		// memory class
#include "HtmlView.h"
#include "query.h"
#include "resource.h"

#ifdef _CHEAT_VC_CLASSVIEW
#define __hide {
#define __end_hide }
#elif (!defined __hide) && (!defined __end_hide)
#define __hide
#define __end_hide
#endif

class view_controller  
{
	// The HTML view
	CHtmlView				m_view ;
	// the html content
	wstring					m_html_content ;
	
	// the match records for glossary registration
	search_match			m_glossary_registration_top ;
	search_match			m_glossary_registration_bottom ;
	
	// pointer to newly added record, to allow editing
	record_pointer			m_new_record ;
	
	translation_match_query_trans		m_trans_matches ;
	search_query_mainframe				m_search_matches_mainframe ;
	search_query_glossary				m_search_matches_glossary ;
	

public:
	view_controller();
	virtual ~view_controller();
	HWND create( HWND parent ) ;

	void clean_up_urls( CHtmlElementCollection &collection ) ;
	CComPtr< IHTMLElementCollection > get_element_collection() ;

	long get_num_matches() { return m_trans_matches.size() ; }

	bool format_background_color() ;
	COLORREF get_background_color() ;
	new_glossary_added() ;

	LRESULT OnBold( ) ;
	LRESULT OnUnderline( ) ;
	LRESULT OnItalic( ) ;

__hide
	BEGIN_MSG_MAP_EX(view_controller)

		// ----
		
		BEGIN_CMD_HANDLER_EX
		
			// formatting
			CMD_HANDLER_EX_0(IDC_UNDERLINE, OnUnderline)
			CMD_HANDLER_EX_0(IDC_BOLD, OnBold)
			CMD_HANDLER_EX_0(IDC_ITALIC, OnItalic)

			PASS_CMDS_TO_VIEW_EX( m_view )

		END_CMD_HANDLER_EX
					
	END_MSG_MAP()
__end_hide

} ;


#endif // !defined(AFX_VIEW_CONTROLLER_H__191BD9B0_0F6D_426C_AF3E_9B15C1ABA8B6__INCLUDED_)
