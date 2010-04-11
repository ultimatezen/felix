#pragma once
#include "window_listener.h"
#include "record_local.h"

#ifdef UNIT_TEST
class WindowListenerFake : public WindowListener
{
	typedef mem_engine::record_pointer rec_ptr ;
	typedef mem_engine::record_local rec_type ;
	typedef mem_engine::search_match_ptr match_ptr ;
	typedef mem_engine::search_match match_type ;
	
public:
	DECLARE_SENSING_VAR ;
	rec_ptr new_rec ;
	rec_ptr review_rec ;
	match_ptr current_match ;
	match_ptr item_under_edit ;
	std::vector<int> m_feedback_int ;
	bool m_is_short_format ;
	bool m_is_single_page ;
	bool m_should_delete ;

	WindowListenerFake() : 
		new_rec(new rec_type)
		, review_rec(new rec_type)
		, current_match(new match_type(rec_ptr(new rec_type)))
		, item_under_edit(new match_type(rec_ptr(new rec_type)))
		, m_is_short_format(false)
		, m_is_single_page(false) 
		, m_should_delete(true)
	{
	}

	void show_edit_dialog( rec_ptr record, 
		const int memory_id, 
		UINT title_id )
	{
		record ;
		memory_id ;
		title_id ;
		SENSE("show_edit_dialog") ;
	}
	bool check_delete()
	{
		return m_should_delete ;
	}
	void set_new_record(rec_ptr rec)
	{
		new_rec = rec ;
		SENSE("set_new_record") ;
	}
	rec_ptr get_new_record()
	{
		SENSE("get_new_record") ;
		return new_rec ;
	}
	void set_review_record(rec_ptr rec)
	{
		review_rec = rec ;
		SENSE("set_review_record") ;
	}
	rec_ptr get_review_record()
	{
		SENSE("get_review_record") ;
		return review_rec ;
	}
	match_ptr get_item_under_edit()
	{
		SENSE("get_item_under_edit") ;
		return item_under_edit ;
	}
	void set_item_under_edit(match_ptr match)
	{
		SENSE("set_item_under_edit") ;
		item_under_edit = match ;
	}

	void set_bg_color_if_needed() 
	{
		SENSE("set_bg_color_if_needed") ;
	}
	void check_mousewheel() 
	{
		SENSE("check_mousewheel") ;
	}
	mem_engine::search_match_ptr get_current_match()
	{
		SENSE("get_current_match") ;
		return current_match ;
	}
	void redo_lookup( mem_engine::search_match_ptr match, bool do_gloss)
	{
		match ;
		do_gloss ;
		SENSE("redo_lookup") ;
	}

	bool is_short_format()
	{
		SENSE("is_short_format") ;
		return m_is_short_format ;
	}
	bool is_single_page()
	{
		SENSE("is_single_page") ;
		return m_is_single_page ;
	}
	// UI control
	void set_menu_checkmark(int item_id, bool is_checked)
	{
		SENSE("set_menu_checkmark") ;
		SENSE(int2string(item_id)) ;
		SENSE(bool2string(is_checked)) ;
	}

	// user feedback
	virtual bool user_feedback( const CString &feedback, int pane = 0)
	{
		SENSE("user_feedback") ;
		SENSE(string2string(tstring(feedback))) ;
		SENSE(ulong2string(pane)) ;
		return true ;
	}
	virtual bool user_feedback( const UINT id, int pane = 0)
	{
		m_feedback_int.push_back(id) ;
		SENSE("user_feedback") ;
		SENSE(ulong2string(id)) ;
		SENSE(ulong2string(pane)) ;
		return true ;
	}
	virtual bool user_feedback( const tstring &feedback, int pane = 0) 
	{
		SENSE("user_feedback") ;
		SENSE(string2string(tstring(feedback))) ;
		SENSE(ulong2string(pane)) ;
		return true ;
	}

};
#endif
