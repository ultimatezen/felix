#pragma once

#include "record.h"

class WindowListener
{
public:
	virtual void show_edit_dialog( mem_engine::record_pointer &record, 
								   const int memory_id, 
								   UINT title_id ) = 0 ;
	virtual void set_new_record(mem_engine::record_pointer rec) = 0 ;
	virtual void set_review_record(mem_engine::record_pointer rec) = 0 ;
	virtual mem_engine::record_pointer get_new_record() = 0 ;
	virtual mem_engine::record_pointer get_review_record() = 0 ;

	virtual mem_engine::search_match_ptr get_item_under_edit() = 0 ;
	virtual void set_item_under_edit(mem_engine::search_match_ptr match) = 0 ;

	virtual void set_bg_color_if_needed() = 0 ;
	virtual void check_mousewheel() = 0 ;

	virtual mem_engine::search_match_ptr get_current_match() = 0 ;
	virtual void redo_lookup( mem_engine::search_match_ptr match, bool do_gloss = false ) = 0 ;

	// user feedback
	virtual bool user_feedback( const CString &feedback, int pane = 0) = 0 ;
	virtual bool user_feedback( const UINT id, int pane = 0) = 0 ;
	virtual bool user_feedback( const tstring &feedback, int pane = 0) = 0 ;

};