/*!
	@file CommonWindowFunctionality.h
	@brief Interface for common window functionality class
	@date 2005/06/25
	Time: 11:06:01
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "StringEx.h"				// string stuff
#include "memory_model.h"			// memory class
#include "MyRecentDocumentList.h"	// CMyRecentDocumentList
#include "HtmlView.h"				// the view class (CHtmlView)
#include "FindDlg.h"				// CFindDlg class
#include "EditModeFindDlg.h"		// CEditModeFindDlg
#include "EditTransRecordDialog.h"	// CEditTransRecordDialog class

#include <ATLCTRLX.H>
#include "FelixStatusBar.h"

#include "view_interface.h"
#include "view_state.h"

#include "ProgressListener.h"

#include "Drop.h"	// CDropHandle

#include "NavInterface.h"
#include "WindowsMessage.h"
#include "QueryMergeDlg.h"
#include "window_listener.h"

void add_popup_item(CMenu &menu, int command_id, int text_id) ;
void add_popup_separator(CMenu &menu) ;



/**
	@class CCommonWindowFunctionality  
	@brief Unites functionality common to both CMainFrame and CGlossaryDialog.
 */
class CCommonWindowFunctionality : 
	public TWindow
	, public CProgressListener
	, public html::CHtmlViewListener
	, public CNavInterface
	, public WindowListener
{
public:
	DECLARE_SENSING_VAR ;

	typedef mem_engine::record_pointer record_type ;
	typedef mem_engine::search_match_ptr match_ptr ;
	typedef mem_engine::search_match match_type ;

	static const int BM_SIZE = 16 ;
	static const int SEP_ID = 0 ;
	static const COLORREF MAGENTA = RGB( 255, 0, 255 ) ;


	enum MERGE_CHOICE
	{
		MERGE_CHOICE_CANCEL,
		MERGE_CHOICE_MERGE,
		MERGE_CHOICE_SEPARATE
	};
	CFelixStatusBar m_statusbar ;
	

	record_type		m_new_record ;	  // last added record
	record_type		m_review_record ; // review mode
	match_ptr		m_item_under_edit ;

	// Save the last deleted match, so we can undo...
	mem_engine::search_match_ptr		m_deleted_match ;

	// recent docs list
	CMyRecentDocumentList		m_mru ;
	// view

	frame_view			m_view_interface ;
	ViewState			*m_view_state ;


	// find dialog
	CFindDlg				m_find ;
	CEditModeFindDlg		m_edit_find ;
	CEditModeReplaceDlg		m_edit_replace ;
	// edit record dialog
	typedef boost::shared_ptr<CEditTransRecordDialog> edit_record_dlg_ptr ;

	// background processing
	// (Checks for messages on the queue)
	background_processor	m_background_processor ;


	// ====================
	// status
	// ====================
	bool				m_silent_mode ; // give user feedback?
	bool				m_is_short_format ; // what kind of memory are we showing?

	int					m_mousewheel_count ;

	void SetPaneWidths(int* arrWidths, int nPanes) ;

private:

	void refresh_editor_window() ;
	void refresh_replace_window() ;
	void refresh_edit_find_window() ;
	void refresh_find_window() ;

protected:
	INT_PTR LetUserSaveMemory(mem_engine::memory_pointer& mem);
	void instantiate_dlg(int res_id, DLGPROC lpDialogProc);
	void refresh_windows() ;
	void refresh_view_content();

public:
	void init_state(ViewState *state)
	{
		state->set_model(this->get_model()) ;
		state->set_window_listener(this) ;
		state->set_view(&m_view_interface) ;
	}
	record_type get_new_record()
	{
		return m_new_record ;
	}
	record_type get_review_record()
	{
		return m_review_record ;
	}
	void set_new_record(record_type rec)
	{
		m_new_record = rec ;
	}
	void set_review_record(record_type rec)
	{
		m_review_record = rec ;
	}
	match_ptr get_item_under_edit()
	{
		return m_item_under_edit ;
	}
	void set_item_under_edit(match_ptr match)
	{
		m_item_under_edit = match ;
	}

	void set_bg_color(COLORREF c);
	bool show_mem_mgr_dlg(int title_id = 0) ;
	BOOL dropped_in_client( CDropHandle dropped ) ;
	BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message )  ;
	CString get_swe_error_message(CString const&failure_message);

	INT_PTR wants_to_save_in_native_format() ;
	void OnProgressInit( const CString &file_name, size_t min_val, size_t max_val ) ;
	void set_up_prog_bar(const int max_size ) ;
	void OnProgressDoneLoad( size_t final_val ) ;
	void OnProgressDoneWrite( size_t final_val ) ;
	void final_feedback( UINT msg_id, size_t final_val ) ;
	bool OnProgressLoadUpdate( size_t current_val ) ; // true to continue
	bool OnProgressWriteUpdate( size_t current_val ) ; // true to continue
	void do_progress_msg( size_t current_val, UINT msg_id ) ;
	void do_percent_done_msg( size_t current_val ) ;

	CString get_location() ;
	bool is_demo() const;

	bool user_feedback( const CString &feedback, int pane = 0) ;
	bool user_feedback( const UINT id, int pane = 0) ;
	bool user_feedback( const tstring &feedback, int pane = 0) ;
	mem_engine::memory_list & get_memories() ;
	INT_PTR user_wants_to_save( const CString &f_name ) ;
	INT_PTR check_save() ;
	bool check_location() ;
	void init_edit_replace_window( int show_command = SW_HIDE ) ;
	void init_edit_find_window( int show_command = SW_HIDE ) ;

	CCommonWindowFunctionality( ) ;
	virtual ~CCommonWindowFunctionality()
	{
	}
	HWND init_view() ;

	/************************************************************************/
	/* Common Message Handlers                                              */
	/************************************************************************/
	LRESULT on_user_replace_edit_record( ) ;
	LRESULT on_file_save_all();
	LRESULT on_bold( ) ;
	LRESULT on_underline( ) ;
	LRESULT on_italic( ) ;
	LRESULT on_demo_check_excess_memories() ;
	LRESULT on_user_edit_search( LPARAM lParam ) ;

	void check_mousewheel();

	LRESULT OnMouseWheel( UINT, WPARAM wparam, LPARAM);

	// other helper functions
	bool check_delete() ;

	bool show_edit_dialog_for_new_entry( const int title_id ) ;
	bool init_edit_window(int show_command = SW_HIDE) ;
	bool init_find_window(int show_command = SW_HIDE, int title = 0 ) ;
	size_t get_size() ;
	void show_edit_dialog( mem_engine::record_pointer &record, const int memory_id, UINT title_id = IDS_EDIT_GLOSS ) ;

	int prev_display_state() ;
	int next_display_state() ;

	void save_memory(mem_engine::memory_pointer mem) ;
	CString get_save_destination();

	void save_all_memories();

	void save_window_settings( LPCTSTR key_name );
	MERGE_CHOICE get_merge_choice( CQueryMergeDlg &dlg ) ;

/************************************************************************/
/* Pure virtual methods to enable methods to be pulled up/polymorphism  */
/************************************************************************/

	virtual FelixModelInterface* get_model() = 0 ;
	virtual	void set_display_state( DISPLAY_STATE new_state ) = 0 ;
	virtual LRESULT on_view_edit_mode(WindowsMessage &message) = 0 ;
	virtual LPCTSTR get_save_ext() = 0 ;
	virtual LPCTSTR get_save_filter() = 0 ;
	virtual LPCTSTR get_open_filter() = 0 ;
	virtual bool show_view_content() = 0 ;
	virtual bool set_window_title() = 0 ;
	virtual void do_save( mem_engine::memory_pointer mem ) = 0 ;
	virtual CString get_window_type_string() = 0 ;
	virtual bool check_for_clashes( mem_engine::memory_pointer mem ) = 0 ;

	virtual boost::shared_ptr<mem_engine::memory_model> get_memory_model() = 0 ;
	virtual edit_record_dlg_ptr get_editor() = 0 ;
	
} ;

