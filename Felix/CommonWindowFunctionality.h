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

#include "ProgressListener.h"

#include "Drop.h"	// CDropHandle

#include "NavInterface.h"
#include "WindowsMessage.h"
#include "QueryMergeDlg.h"
using namespace memory_engine ;

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
{
public:
	DECLARE_SENSING_VAR ;
	// various display states
	enum DISPLAY_STATE
	{ 
		INIT_DISPLAY_STATE , 		  // the startup screen
		NEW_RECORD_DISPLAY_STATE , 	  // a new record has been added
		MATCH_DISPLAY_STATE , 		  // a translation match is being displayed
		CONCORDANCE_DISPLAY_STATE ,	  // concordances are being displayed
		REG_GLOSS_DISPLAY_STATE ,	  // the register glossary entries screen
		LOOKUP_DISPLAY_STATE,		  // glossary lookup screen
		TRANS_REVIEW_STATE,			  // view translations entry
	} ;

	enum MERGE_CHOICE
	{
		MERGE_CHOICE_CANCEL,
		MERGE_CHOICE_MERGE,
		MERGE_CHOICE_SEPARATE
	};
	CFelixStatusBar m_statusbar ;
	
	DISPLAY_STATE		m_display_state ;

	// Save the last deleted match, so we can undo...
	search_match_ptr		m_deleted_match ;

	// recent docs list
	CMyRecentDocumentList		m_mru ;
	// view

	view_interface			m_view_interface ;

	// find dialog
	CFindDlg				m_find ;
	CEditModeFindDlg		m_edit_find ;
	CEditModeReplaceDlg		m_edit_replace ;
	// edit record dialog
	CEditTransRecordDialog  m_editor ;

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
	INT_PTR LetUserSaveMemory(memory_engine::memory_pointer& mem);
	void instantiate_dlg(int res_id, DLGPROC lpDialogProc);
	void refresh_windows() ;
	void refresh_view_content();

public:
	void set_bg_color(COLORREF c);
	bool show_mem_mgr_dlg(int title_id = 0) ;
	BOOL dropped_in_client( CDropHandle dropped ) ;
	BOOL handle_sw_exception( CSWException &e, const CString &failure_message )  ;
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
	bool is_demo() ;

	bool user_feedback( const CString &feedback, int pane = 0) ;
	bool user_feedback( const UINT id, int pane = 0) ;
	bool user_feedback( const tstring &feedback, int pane = 0) ;
	memory_list & get_memories() ;
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
	size_t get_size()  ;
	void show_edit_dialog( memory_engine::record_pointer &record, const int memory_id, UINT title_id = IDS_EDIT_GLOSS ) ;

	void set_display_state( DISPLAY_STATE new_state ) ;
	int get_display_state() ;
	int prev_display_state() ;
	int next_display_state() ;

	void save_memory(memory_pointer mem) ;
	CString get_save_destination();

	void save_all_memories();

	void save_window_settings( LPCTSTR key_name );
	MERGE_CHOICE get_merge_choice( CQueryMergeDlg &dlg );
/************************************************************************/
/* Pure virtual methods to enable methods to be pulled up/polymorphism  */
/************************************************************************/

	virtual LRESULT on_view_edit_mode(WindowsMessage &message) = 0 ;
	virtual LPCTSTR get_save_ext() = 0 ;
	virtual LPCTSTR get_save_filter() = 0 ;
	virtual LPCTSTR get_open_filter() = 0 ;
	virtual bool show_view_content() = 0 ;
	virtual bool set_window_title() = 0 ;
	virtual void do_save( memory_pointer mem ) = 0 ;
	virtual CString get_window_type_string() = 0 ;
	virtual bool check_for_clashes( memory_pointer mem ) = 0 ;
	virtual void set_new_record(record_pointer rec) = 0 ;
	virtual record_pointer get_new_record() = 0 ;
	virtual void retrieve_record_new_state() = 0 ;
	virtual void retrieve_record_results_state() = 0 ;

	virtual boost::shared_ptr<memory_engine::memory_model> get_memory_model() = 0 ;
} ;

