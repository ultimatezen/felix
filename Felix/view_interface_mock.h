#pragma once

#include "view_interface.h"
#include "CStringInterface.h"
#include "StringEx.h"
#include <list>

class view_interface_mock :
	public view_interface
{
public:
	wstring	m_body_text ;
	CString	m_resource_name ;
	std::list< string >	m_call_history ;

	void create( HWND parent, HWND &client )
	{
		parent ;
		client ;
		m_call_history.push_back( "create" ) ;
	}
	void set_text( const wstring &text )
	{
		m_body_text = text ;
		m_call_history.push_back( "set_text" ) ;
	}
	void load_resource( LPCTSTR resource_name )
	{
		m_resource_name = resource_name ;
		m_call_history.push_back( "load_resource" ) ;
	}

	void do_bold() 
	{
		m_call_history.push_back( "do_bold" ) ;
	}
	void do_underline()
	{
		m_call_history.push_back( "do_underline" ) ;
	}
	void do_italic()
	{
		m_call_history.push_back( "do_italic" ) ;
	}
	void do_delete()
	{
		m_call_history.push_back( "do_delete" ) ;
	}
	void set_bg_color( const wstring &color ) 
	{
		color ;
		m_call_history.push_back( "set_bg_color" ) ;
	}
	wstring get_bg_color()
	{
		m_call_history.push_back( "get_bg_color" ) ;
		return wstring() ;
	}

	document_ptr get_document()
	{
		m_call_history.push_back( "get_document" ) ;
		return document_ptr() ;
	}

	bool is_edit_mode()
	{
		m_call_history.push_back( "is_edit_mode" ) ;
		return true ;
	}
	void put_edit_mode( bool setting )
	{
		m_call_history.push_back( "put_edit_mode" ) ;
		setting ;
	}

	// edit mode stuff
	void handle_enter_edit_mode_new_record_glossary( )
	{
		m_call_history.push_back( "handle_enter_edit_mode_new_record_glossary" ) ;
	}
	void handle_enter_edit_mode_concordance_glossary( search_query_glossary &matches )
	{
		matches ;
		m_call_history.push_back( "handle_enter_edit_mode_concordance_glossary" ) ;
	}
	bool handle_leave_edit_mode_new_record_glossary( memory_controller &memories, record_pointer &record )
	{
		memories ;
		record ;
		m_call_history.push_back( "handle_leave_edit_mode_new_record_glossary" ) ;
		return true ;
	}
	bool handle_leave_edit_mode_concordance_glossary(  memory_controller &memories, search_query_glossary &matches )
	{
		memories ;
		matches ;
		m_call_history.push_back( "handle_leave_edit_mode_concordance_glossary" ) ;
		return true ;
	}

	void handle_enter_edit_mode_register( )
	{
		m_call_history.push_back( "handle_enter_edit_mode_register" ) ;
	}

	void handle_enter_edit_mode_new_record()
	{
		m_call_history.push_back( "handle_enter_edit_mode_new_record" ) ;
	}

	void handle_enter_edit_mode_match( translation_match_query_trans &matches )
	{
		matches ;
		m_call_history.push_back( "handle_enter_edit_mode_match" ) ;
	}

	void handle_enter_edit_mode_concordance( search_query_mainframe &matches )
	{
		matches ;
		m_call_history.push_back( "handle_enter_edit_mode_concordance" ) ;
	}
	void handle_leave_edit_mode_match( memory_controller &memories, translation_match_query_trans &matches )
	{
		memories ;
		matches ;
		m_call_history.push_back( "handle_leave_edit_mode_match" ) ;
	}
	bool handle_leave_edit_mode_concordance( memory_controller &memories, search_query_mainframe &matches )
	{
		memories ;
		matches ;
		m_call_history.push_back( "handle_leave_edit_mode_concordance" ) ;
		return true ;
	}
	bool handle_leave_edit_mode_register( search_match &glossary_registration_top, search_match glossary_registration_bottom )
	{
		glossary_registration_top ;
		glossary_registration_bottom ;
		m_call_history.push_back( "handle_leave_edit_mode_register" ) ;
		return true ;
	}
	void ensure_document_complete(HACCEL, HWND)
	{
		m_call_history.push_back( "ensure_document_complete" ) ;
	}
	void ensure_navigation_complete(HACCEL, HWND)
	{
		m_call_history.push_back( "ensure_navigation_complete" ) ;
	}

	wstring get_selection_text()
	{
		return wstring(L"selection_text") ;
	}
	// other stuff
	void scroll_element_into_view( const wstring &id )
	{
		id ;
		m_call_history.push_back( "scroll_element_into_view" ) ;
	}

	BOOL PreTranslateMessage( LPMSG pMsg )
	{
		pMsg ;
		m_call_history.push_back( "PreTranslateMessage" ) ;
		return FALSE ;
	}
	BOOL ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult)
	{
		hWnd ;
		uMsg ;
		wParam ;
		lParam ;
		lResult ;

		return FALSE ;
	}
	void Move( LPRECT rect )
	{
		rect ;
	}

	void run_test_suite()
	{

	}

	view_interface_mock(void);
	~view_interface_mock(void);
};
