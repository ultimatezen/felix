/*!
	@file CommonWindowFunctionality.h
	@brief Interface for common window functionality class
	@date 2005/06/25
	Time: 11:06:01
	@author Ryan Ginstrom
 */

#pragma once

#include "StringEx.h"				// string stuff
#include "memory_model.h"			// memory class
#include "MyRecentDocumentList.h"	// CMyRecentDocumentList
#include "HtmlView.h"				// the view class (CHtmlView)
#include "FindDlg.h"				// CFindDlg class
#include "EditModeFindDlg.h"		// CEditModeFindDlg
#include "EditTransRecordDialog.h"	// CEditTransRecordDialog class

#include "FelixStatusBar.h"

#include "view_interface.h"
#include "view_state.h"

#include "ProgressListener.h"
#include "Drop.h"	// CDropHandle
#include "QueryMergeDlg.h"

#include "NavInterface.h"
#include "WindowsMessage.h"
#include "window_listener.h"
#include "window_interface.h"

#include "menu_helper.h"
#include "zoom_dialog.h"
#include "active_window.h"

typedef boost::shared_ptr<CEditTransRecordDialog> edit_record_dlg_ptr ;


void add_common_tb_commands(std::vector< int > &commands) ;
void add_common_std_bitmaps(std::vector< int > &StdBitmaps) ;
void create_tb_imagelist(CImageList &images, std::vector< int > &StdBitmaps) ;



/**
	@class CCommonWindowFunctionality  
	@brief Unites functionality common to both CMainFrame and CGlossaryDialog.
 */
class CCommonWindowFunctionality : 
	public CWindow
	, public CProgressListener
	, public html::CHtmlViewListener
	, public CNavInterface
	, public WindowListener
	, public WindowInterface
	, public CZoomInterface
	, public ActiveWindow
{
public:
	DECLARE_SENSING_VAR ;

	typedef mem_engine::record_pointer record_type ;
	typedef mem_engine::search_match_ptr match_ptr ;
	typedef mem_engine::search_match match_type ;


	enum MERGE_CHOICE
	{
		MERGE_CHOICE_CANCEL,
		MERGE_CHOICE_MERGE,
		MERGE_CHOICE_SEPARATE
	};
	CFelixStatusBar m_statusbar ;
	

	record_type		m_new_record ;	  // last added record
	match_ptr		m_review_match ; // review mode
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

	// background processing
	// (Checks for messages on the queue)
	background_processor	m_background_processor ;
	std::map<int, int>		m_toolmap ;

	// ====================
	// status
	// ====================
	bool				m_silent_mode ; // give user feedback?
	bool				m_is_short_format ; // what kind of memory are we showing?

	int					m_mousewheel_count ;

	void SetPaneWidths(int* arrWidths, int nPanes) ;
	LRESULT add_remote_memory(mem_engine::model_ptr memories, mem_engine::memory_pointer mem);

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
	// These function pointers provide seams that we can
	// override for testing.
	boost::function<window_wrapper_ptr(HWND)> m_get_window ;
	boost::function<BOOL(MSG*)> m_pre_translate_msg ;

	CCommonWindowFunctionality( ) ;
	virtual ~CCommonWindowFunctionality();

	/************************************************************************/
	/* WindowInterface implementation                                       */
	/************************************************************************/
	// height
	long get_height();
	void set_height(long height);
	// width
	long get_width();
	void set_width(long width);

	// left
	long get_left();
	void set_left(long left);

	// top
	long get_top();
	void set_top(long top);

	void raise();

	WindowInterface* get_window_interface()
	{
		return this ;
	}
	/************************************************************************/
	/* window_wrapper implementation                                       */
	/************************************************************************/
	BOOL is_window()
	{
		window_wrapper_ptr window = this->m_get_window(this->m_hWnd) ;
		return window->IsWindow() ;
	}
	BOOL show_window(int show_command)
	{
		window_wrapper_ptr window = this->m_get_window(this->m_hWnd) ;
		return window->ShowWindow(show_command) ;
	}
	/************************************************************************/
	/* pure virtual methods 
	/************************************************************************/
	virtual input_device_ptr get_input_device() = 0 ;
	virtual output_device_ptr get_output_device() = 0 ;

	void init_state(ViewState *state);
	void init_tooltip_map( std::map<int, int> &toolmap );
	record_type get_new_record()
	{
		return m_new_record ;
	}
	match_ptr get_review_match()
	{
		return m_review_match ;
	}
	void set_new_record(record_type rec)
	{
		m_new_record = rec ;
	}
	void set_review_match(record_type rec, int mem_id)
	{
		m_review_match->set_record(rec) ;
		m_review_match->set_values_to_record() ;
		m_review_match->set_memory_id(mem_id) ;
	}
	match_ptr get_item_under_edit();
	void set_item_under_edit(match_ptr match);

	void set_bg_color(COLORREF c);
	bool show_mem_mgr_dlg(int title_id = 0) ;
	BOOL dropped_in_client( CDropHandle dropped ) ;
	BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message )  ;
	CString get_swe_error_message(CString const&failure_message);

	void SwapFindDialogs(const bool edit_mode_enabled);

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

	void handle_tooltip( LPNMHDR pnmh, int idCtrl, std::map<int, int> &toolmap );
	CString get_location() ;
	bool is_demo() const;

	bool user_feedback( const CString &feedback, int pane = 0) ;
	bool user_feedback( const UINT id, int pane = 0) ;
	bool user_feedback( const tstring &feedback, int pane = 0) ;
	mem_engine::memory_list & get_memories() ;
	INT_PTR user_wants_to_save( const CString &f_name ) ;
	INT_PTR check_save() ;

	virtual INT_PTR check_save_memory(mem_engine::memory_pointer mem) = 0;

	void initialize_values(void);
	void remove_memory(mem_engine::memory_pointer mem, int msgid);
	bool check_location() ;

	void init_edit_replace_window( int show_command = SW_HIDE ) ;
	void init_edit_find_window( int show_command = SW_HIDE ) ;
	HWND init_view() ;

	/************************************************************************/
	/* Common Message Handlers                                              */
	/************************************************************************/
	LRESULT on_file_save_all();
	LRESULT on_bold( ) ;
	LRESULT on_underline( ) ;
	LRESULT on_italic( ) ;
	LRESULT on_demo_check_excess_memories() ;
	LRESULT on_user_edit_search( LPARAM lParam ) ;

	void check_mousewheel();
	// Set zoom level in response to the zoom dialog, 
	// or loading from preferences.
	void set_zoom_level( int zoom_level );
	LRESULT OnMouseWheel( UINT, WPARAM wparam, LPARAM);

	// other helper functions
	bool check_delete() ;

	bool show_edit_dialog_for_new_entry( const int title_id ) ;
	bool init_edit_window(int show_command = SW_HIDE) ;
	bool init_find_window(int show_command = SW_HIDE, int title = 0 ) ;
	size_t get_size() ;
	void show_edit_dialog( mem_engine::record_pointer record, const int memory_id, UINT title_id = IDS_EDIT_GLOSS ) ;

	int prev_display_state() ;
	int next_display_state() ;

	void save_memory(mem_engine::memory_pointer mem) ;
	CString get_save_destination();

	void save_all_memories();
	void export_tabbed_text( CString file_name, mem_engine::memory_pointer mem );
	void export_excel( CString file_name, mem_engine::memory_pointer mem );

	void save_window_settings( LPCTSTR key_name );
	MERGE_CHOICE get_merge_choice( CQueryMergeDlg &dlg, app_props::properties_general *props ) ;

/************************************************************************/
/* Pure virtual methods to enable methods to be pulled up/polymorphism  */
/************************************************************************/

	virtual LPARAM handle_find() = 0 ;
	virtual model_iface_ptr get_model() = 0 ;
	virtual	void set_display_state( DISPLAY_STATE new_state ) = 0 ;
	virtual LRESULT on_view_edit_mode(WindowsMessage &message) = 0 ;
	virtual LPCTSTR get_save_ext() = 0 ;
	virtual LPCTSTR get_save_filter() = 0 ;
	virtual LPCTSTR get_open_filter() = 0 ;
	virtual void show_view_content() = 0 ;
	virtual bool set_window_title() = 0 ;
	virtual void do_save( mem_engine::memory_pointer mem ) = 0 ;
	virtual CString get_window_type_string() = 0 ;
	virtual bool check_for_clashes( mem_engine::memory_pointer mem ) = 0 ;

	virtual boost::shared_ptr<mem_engine::memory_model> get_memory_model() = 0 ;
	virtual edit_record_dlg_ptr get_editor() = 0 ;
	
} ;

