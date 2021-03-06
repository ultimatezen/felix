#pragma once

#include "record.h"
#include "search_match.h"

class WindowListener
{
public:

	// various display states
	enum DISPLAY_STATE
	{ 
		INIT_DISPLAY_STATE , 		  // the startup screen
		NEW_RECORD_DISPLAY_STATE , 	  // a new record has been added
		MATCH_DISPLAY_STATE , 		  // matches are being displayed
		CONCORDANCE_DISPLAY_STATE ,	  // concordances are being displayed
		TRANS_REVIEW_STATE,			  // view translations entry
	} ;
	DISPLAY_STATE		m_display_state ;

	// Function name	: get_display_sate
	DISPLAY_STATE get_display_state()
	{
		return m_display_state ;
	}


	virtual bool check_delete() = 0 ;
	virtual void show_edit_dialog( mem_engine::record_pointer record, 
								   const int memory_id, 
								   UINT title_id ) = 0 ;
	virtual void set_new_record(mem_engine::record_pointer rec) = 0 ;
	virtual void set_review_match(mem_engine::record_pointer rec, int mem_id) = 0 ;
	virtual mem_engine::record_pointer get_new_record() = 0 ;
	virtual mem_engine::search_match_ptr get_review_match() = 0 ;

	virtual void get_qc_messages(mem_engine::record_pointer record, std::vector<wstring> &messages) = 0 ;

	virtual mem_engine::search_match_ptr get_item_under_edit() = 0 ;
	virtual void set_item_under_edit(mem_engine::search_match_ptr match) = 0 ;

	virtual void set_bg_color_if_needed() = 0 ;
	virtual void check_mousewheel() = 0 ;

	virtual mem_engine::search_match_ptr get_current_match() = 0 ;
	virtual void redo_lookup( mem_engine::search_match_ptr match, bool do_gloss = false ) = 0 ;

	virtual bool is_short_format() = 0 ;
	virtual bool is_single_page() = 0 ;

	// UI control
	virtual void set_menu_checkmark(int item_id, bool is_checked) = 0 ;
	// user feedback
	virtual bool user_feedback( const CString &feedback, int pane = 0) = 0 ;
	virtual bool user_feedback( const UINT id, int pane = 0) = 0 ;
	virtual bool user_feedback( const tstring &feedback, int pane = 0) = 0 ;

};