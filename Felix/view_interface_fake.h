#pragma once

#include "view_interface.h"
#include "record.h"
#include "record_local.h"

class view_interface_fake : public view_interface
{

public:
	DECLARE_SENSING_VAR ;
	typedef mem_engine::record_pointer rec_ptr ;

	bool m_is_edit_mode ;
	rec_ptr m_rec ;
	wstring m_text ;

	view_interface_fake() : 
		m_is_edit_mode(false)
		, m_rec(new mem_engine::record_local)
	{
	}

	bool is_edit_mode()
	{
		SENSE("is_edit_mode") ;
		return m_is_edit_mode ;
	}
	void put_edit_mode( bool setting )
	{
		SENSE("put_edit_mode") ;
		m_is_edit_mode = setting ;
	}
	void ensure_document_complete()
	{
		SENSE("ensure_document_complete") ;
	}
	void ensure_navigation_complete()
	{
		SENSE("ensure_navigation_complete") ;
	}
	void navigate(LPCTSTR url)
	{
		url ;
		SENSE("navigate") ;
		SENSE(string2string(wstring(url))) ;
	}
	void set_text( const wstring text ) 
	{
		m_text = text ;
		SENSE("set_text") ;
		SENSE(string2string(text)) ;
	}
	void set_scroll_pos( long pos )
	{
		pos ;
		SENSE("set_scroll_pos") ;
		SENSE(int2string(pos)) ;
	}
	void scroll_element_into_view( const wstring current_id )
	{
		current_id ;
		SENSE("scroll_element_into_view") ;
		SENSE(string2string(current_id)) ;
	}

	// =========================
	// for entering edit mode
	// =========================
	void handle_enter_edit_mode_new_record_glossary( )
	{
		SENSE("handle_enter_edit_mode_new_record_glossary") ;
	}
	void handle_enter_edit_mode_concordance_glossary( mem_engine::felix_query *matches ) 
	{
		matches ;
		SENSE("handle_enter_edit_mode_concordance_glossary") ;
	}
	void handle_enter_edit_mode_new_record() 
	{
		SENSE("handle_enter_edit_mode_new_record") ;
	}
	void handle_enter_edit_mode_match( mem_engine::felix_query *matches )
	{
		matches ;
		SENSE("handle_enter_edit_mode_match") ;
	}
	rec_ptr get_match_record( mem_engine::felix_query *matches )
	{
		matches ;
		SENSE("get_match_record") ;
		return m_rec ;
	}
	void handle_enter_edit_mode_concordance( mem_engine::felix_query *matches )
	{
		matches ;
		SENSE("handle_enter_edit_mode_concordance") ;
	}

	// =========================
	// for leaving edit mode
	// =========================
	void handle_leave_edit_mode_new( rec_ptr &record ) 
	{
		record ;
		SENSE("handle_leave_edit_mode_new") ;
	}
	void handle_leave_edit_mode_match( MemoryControllerType memories, 
									   mem_engine::felix_query *matches ) 
	{
		memories ;
		matches ;
		SENSE("handle_leave_edit_mode_match") ;
	}
	bool handle_leave_edit_mode_concordance_glossary( MemoryControllerType memories, 
													  mem_engine::felix_query *matches ) 
	{
		memories ;
		matches ;
		SENSE("handle_leave_edit_mode_concordance_glossary") ;
		return true ;
	}
	bool handle_leave_edit_mode_new_record_glossary( MemoryControllerType memories, 
													 rec_ptr &record ) 
	{
		memories ;
		record ;
		SENSE("handle_leave_edit_mode_new_record_glossary") ;
		return true ;
	}
	bool handle_leave_edit_mode_concordance( MemoryControllerType memories, 
											 mem_engine::felix_query *matches ) 
	{
		memories ;
		matches ;
		SENSE("handle_leave_edit_mode_concordance") ;
		return true ;
	}

	html::document3_ptr get_document3() 
	{
		SENSE("get_document3") ;
		return html::document3_ptr(NULL) ;
	}

	void run_script(CString) 
	{
		SENSE("run_script(CString)") ;
	}
	void run_script(CString, CString) 
	{
		SENSE("run_script(CString, CString)") ;
	}

};

