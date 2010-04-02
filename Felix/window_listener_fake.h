#pragma once
#include "window_listener.h"
#include "record_local.h"

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
	std::vector<int> m_feedback_int ;

	WindowListenerFake() : 
		new_rec(new rec_type)
		, review_rec(new rec_type)
		, current_match(new match_type)
	{
		current_match->set_record(rec_ptr(new rec_type)) ;
	}

	void show_edit_dialog( rec_ptr &record, 
		const int memory_id, 
		UINT title_id )
	{
		record ;
		memory_id ;
		title_id ;
		SENSE("show_edit_dialog") ;
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
