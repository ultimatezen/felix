/*!
 * interface of the CMainFrame class.
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */

#pragma once

#include "GlossaryDialog.h"			// CGlossaryDialog
#include "SearchWindow.h"

#include "TranslationMemory.h"		// memory
#include "background_processor.h"
#include "CommonWindowFunctionality.h"	// functionality common to mainframe and
										// glossary dialogs

#include "xpmenu/MenuXP.h"
#include "xpmenu/ToolBarXP.h"

#include "MinViewDlg.h"
#include "WindowExceptionHandler.h"
#include "NavUrl.h"

#include "RegisterGlossDlg.h"
#include "FileOpHandler.h"

#include "WindowsMessage.h"
#include "MainFrameModel.h"

#include "TMXWriter.h"		// CTMXWriter
#include "zoom_dialog.h"
#include "felix_cl_options.h"

// view states
#include "view_state_initial.h"
#include "view_state_new.h"
#include "view_state_concordance.h"
#include "view_state_match.h"
#include "view_state_review.h"

CString get_help_file_path( CString path ) ;
CString get_docs_path() ;

enum
{
	USER_LOOKUP_SOURCE = 1,
	USER_LOOKUP_TRANS,
	USER_SAVE_MEMORIES
};
/**
	@class CMainFrame 							   
	@brief The main app window.
 */
class CMainFrame : 							   
		public CFrameWindowImpl< CMainFrame, CCommonWindowFunctionality >
		, public CUpdateUI<CMainFrame>
		, public CMessageFilter
		, public CIdleHandler
		, public CWindowExceptionHandler< CMainFrame >
		, public CGlossaryWinListener
		, public CZoomInterface
{
VISIBLE_TO_TESTS
	typedef CFrameWindowImpl< CMainFrame, CCommonWindowFunctionality > frame_class ;

	typedef mem_engine::record_pointer record_type ;
	typedef mem_engine::memory_pointer memory_type ;

	// ====================
	// various records
	// ====================

	appstate_mainframe		m_appstate ;

	CMinViewDlg				m_min_view ;

	CCommandBarCtrlXP		m_CmdBar ;
	CToolBarXP				m_stdToolbar ;

	 // ====================
	// translation matches
	// ====================
	mem_engine::translation_match_query		m_trans_matches ;
	mem_engine::search_query_mainframe		m_search_matches ;

	// the match records for glossary registration
	CRegisterGlossDlg		m_reg_gloss_dlg ;
	edit_record_dlg_ptr m_editor ;

	// ====================
	// glossary interface
	// ====================
	gloss_window_list		m_glossary_windows ;

	ViewStateInitialMain	m_view_state_initial ;
	ViewStateNewMain		m_view_state_new ;
	ViewStateConcordanceMain m_view_state_concordance ;
	ViewStateMatchMain		m_view_state_match ;
	ViewStateReview			m_view_state_review ;

	// ====================
	// misc internal stuff
	// ====================

	// various user preferences
	app_props::props_ptr m_properties ;
	// the memory controller
	mem_engine::memory_model_mem	m_silent_memories ;

	CSearchWindow		m_search_window ;
public:
	static const int IDD = IDR_MAINFRAME ;

	wstring m_deferred_query ;

	typedef std::map< UINT, boost::function< LRESULT( WindowsMessage& ) >  > messageMapType ;
	messageMapType m_message_map ;
	messageMapType m_user_message_map ;
	messageMapType m_command_message_map ;
	FelixModelInterface *m_model ;

	~CMainFrame() ;
	CMainFrame( FelixModelInterface *model ) ;

	FelixModelInterface* get_model()
	{
		return m_model ;
	}

	void AddMenuBitmap( const int BitmapId, const int CmdId ) ;

	void set_zoom_level(int zoom_level);
	LRESULT on_view_zoom(WindowsMessage &);

	void register_event_listener( UINT id,  boost::function< LRESULT( WindowsMessage& ) > listenerFunction )
	{
		this->m_message_map[id] = listenerFunction ;
	}
	void register_user_event_listener( UINT id,  boost::function< LRESULT( WindowsMessage& ) > listenerFunction )
	{
		this->m_user_message_map[id] = listenerFunction ;
	}
	void register_command_event_listener( UINT id,  boost::function< LRESULT( WindowsMessage& ) > listenerFunction )
	{
		this->m_command_message_map[id] = listenerFunction ;
	}


	bool OnBeforeNavigate2( _bstr_t url ) ;
	void OnNavEdit( long index );
	void OnNavDelete( long index );
	void OnNavAddGloss( long index );
	void OnNavAddToGloss( long index );

	/************************************************************************/
	/* CGlossaryDlgListener implementation                                  */
	/************************************************************************/
	INT_PTR gloss_check_save_location( memory_type mem ) ;
	bool gloss_add_record( record_type rec );
	void gloss_view_switch( HWND hwnd ) ;

	void add_by_id(size_t recid, wstring source, wstring trans);
	void view_by_id(size_t recid, wstring source, wstring trans);

	wstring get_review_content( memory_type mem );
	/** Set whether difference markup will be shown.
	 */
	void put_show_marking( VARIANT_BOOL setting ) ;
	/** Get whether difference markup will be shown.
	 */
	VARIANT_BOOL get_show_marking() ;

	/** Exit without querying about saving memories, etc.
	 */
	bool exit_silently() ;

	mem_engine::memory_list& get_silent_memories() { return m_silent_memories.get_memories() ; } 

	gloss_window_list& get_glossary_windows()
	{
		return m_glossary_windows ;
	}

	bool set_window_title() ;

	CString get_active_mem_name();
	bool add_glossary_window(gloss_window_pointer gloss_window, int show_cmd = SW_SHOWNOACTIVATE ) ;

	// -------------
	// initialization

	// location
	bool set_location( const CString &location );

	// =================
	// memory operations
	// =================

	// loading and saving the memory
	// get glossary entry
	wstring get_glossary_entry( short index ) ;
	// concordance
	bool get_concordances( const wstring query );
	void source_concordance_feedback();

	bool get_translation_concordances( const wstring query_string );
	void translation_concordance_feedback();

	void concordance_feedback(const wstring plain_text, size_t num) ;

	// delete this translation
	void delete_current_translation( );
	// look up this wstring (rich text)
	bool lookup( const wstring query );
	bool correct_trans( const wstring trans );
	bool lookup_trans( const wstring query );

	void init_trans_matches_for_lookup(const wstring & query) ;

	void init_lookup_properties(const app_props::props_ptr source, 
							mem_engine::search_query_params &dest);
	void get_matches(mem_engine::trans_match_container matches);

	// set the translation for the current query
	bool set_translation( const wstring &translation );
	bool clear_memory() ;

	// helper
	bool add_record( record_type record ) ;

	// information about the memory
	long get_num_matches() { return m_trans_matches.size() ; }

	// the current query
	wstring get_current_query( );
	// the current translation
	wstring get_current_translation( );

	wstring get_record_translation(record_type record);

	// =================
	// ui stuff
	// =================

	bool show_current_match();
	void show_full_view_match();
	bool prev_match( );
	bool next_match( );

	//CSmartCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/ )
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/ )
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/ )

	//============================
	// Message handlers
	//============================
	LRESULT on_create(WindowsMessage &message);

	void set_doc_ui_handler();
	HRESULT get_doc_context_menu();

	void load_util_settings();
	void init_background_processor();
	void init_item_colors();
	LRESULT on_destroy(WindowsMessage &message) ;

	void save_settings_destroy();
	void save_util_settings();
	LRESULT on_close(WindowsMessage &message) ;

	void save_settings_close();
	LRESULT on_edit_entry(WindowsMessage &message) ;

	// command handlers

	// file menu
	LRESULT on_mru_file_open(WindowsMessage &message) ;

	LRESULT on_file_exit(WindowsMessage &message);
	LRESULT on_new_glossary(WindowsMessage &message);
	LRESULT on_file_new(WindowsMessage &message);
	LRESULT on_file_open(WindowsMessage &message);
	LRESULT on_file_save(WindowsMessage &message);
	LRESULT on_file_save_as(WindowsMessage &message);

	LRESULT on_memory_close(WindowsMessage &message);

	LRESULT on_file_connect(UINT, int, HWND);
	// edit
	LRESULT on_edit_replace(WindowsMessage &message);
	LRESULT on_delete_entry(WindowsMessage &message);
	LRESULT on_find(WindowsMessage &message);
	LRESULT on_concordance(WindowsMessage &message);

	LRESULT on_new_search( WindowsMessage &message  );

	// view
	LRESULT on_view_edit_mode(WindowsMessage &message);
	LRESULT on_view_toolbar(WindowsMessage &message);
	LRESULT on_view_status_bar(WindowsMessage &message);
	LRESULT on_view_match(WindowsMessage &message) ;

	void recalculate_match( mem_engine::search_match_ptr match, mem_engine::search_query_params &params );
	LRESULT on_view_search(WindowsMessage &message) ;
	//LRESULT OnViewReg( ) ;
	LRESULT on_view_min_begin(WindowsMessage &message);
	LRESULT on_view_switch(WindowsMessage &message);
	// Function name	: change_display_state
	void set_display_state( DISPLAY_STATE new_state );

	// help
	LRESULT show_about_dialog(WindowsMessage &message);
	LRESULT on_help(WindowsMessage &message);

	void check_shell_execute_result( int result, const CString & filePath );
	LRESULT on_register_gloss(WindowsMessage &message);
	LRESULT on_glossary_source(WindowsMessage &message);
	LRESULT on_glossary_trans(WindowsMessage &message);
	LRESULT on_help_faq(WindowsMessage &message);
	LRESULT on_help_register(WindowsMessage &message);

	LRESULT on_help_check_updates(WindowsMessage &message);
	// tools menu
	LRESULT on_tools_preferences(WindowsMessage &message) ;
	LRESULT on_tools_switch_language(WindowsMessage &message) ;
	LRESULT on_tools_memory_manager(WindowsMessage &message);

	LRESULT on_tools_load_preferences(WindowsMessage &message);

	void load_preferences( const CString filename );
	LRESULT on_tools_save_preferences(WindowsMessage &message);

	void create_process( CString &command, CString error_message );

	LRESULT on_check_demo(WindowsMessage &message);
	LRESULT on_trans_concordance(WindowsMessage &message);
	LRESULT on_source_concordance(WindowsMessage &message);
	LRESULT OnFormatBackgroundColor(WindowsMessage &message) ;
	CColorRef GetBackColor();

	LRESULT on_test_exception(  WindowsMessage &message ) ;


	LRESULT on_add(WindowsMessage &message) ;
	

	// messaging helpers

	// handling of UWM_USER_MESSAGE messages
	LRESULT on_user_register(LPARAM num );

	record_type get_reg_gloss_record( const size_t num );

	LRESULT on_user_delete( LPARAM num );

	void remove_record_from_glossaries(record_type rec);
	void remove_match_record( mem_engine::search_match_ptr match );
	void remove_record_from_mem_id( record_type rec, int mem_id );
	void deleted_new_record_feedback();
	LRESULT on_user_edit(WindowsMessage &message);

	LRESULT on_user_search(WindowsMessage &message) ;
	LRESULT on_user_edit_replace(WindowsMessage &message) ;
	LRESULT on_user_edit_search(WindowsMessage &message);
	LRESULT on_demo_check_excess_memories(WindowsMessage &message);
	LRESULT on_user_replace_edit_record(WindowsMessage &message);

	LRESULT on_file_save_all(WindowsMessage &message);
	LRESULT on_bold(WindowsMessage &message);
	LRESULT on_underline(WindowsMessage &message);
	LRESULT on_italic(WindowsMessage &message);

	LRESULT on_user_retrieve_edit_record( WindowsMessage &message ) ;

	LRESULT on_user_add_to_glossary( LPARAM lParam );
	LRESULT on_user_nav( LPARAM lParam );
	LRESULT on_user_save(WindowsMessage &message);
	LRESULT on_user_view_min_end(WindowsMessage &message);
	LRESULT on_user_toggle_markup(WindowsMessage &message)  ;

	LRESULT on_user_lookup_source(WindowsMessage&);
	LRESULT on_user_lookup_trans(WindowsMessage&);

	LRESULT on_toggle_views(WindowsMessage &message);

	// ===================
	// Drag and Drop
	// ===================
	LRESULT on_drop(WindowsMessage &message);

	//
	// other helpers
	LRESULT handle_find() ;
	wstring create_concordance_list( );

	wstring get_concordance_content_title();
	void handle_foreign_file_save(memory_type& mem, const file::CFileExtension& ext);

	void feedback_loaded_mem( memory_type mem );

	std::wstring get_match_content();
private: 
	void check_load_history() ;

	void load_history();
	void check_command_line(commandline_options &options) ;

	void check_save_history() ;

	bool has_more_memory_history( mem_engine::memory_iterator pos, size_t mem_num );
	BOOL should_save_memory_history( );
	bool export_trados( const CString &file_name ) ;
	bool export_tmx( const CString &file_name );
	void set_exporter_src_and_target_langs(CExportDialog &dialog, CTMXWriter &exporter);

	void do_save( memory_type mem ) ;

public:
	void add_record_to_memory( record_type record );
	void look_up_in_glossaries( const wstring &query );
	void set_up_window_size();
	void set_up_ui_state();
	void set_up_status_bar();
	void set_up_recent_docs_list();
	void set_up_command_bars();
	void set_up_default_initial_size();
	void add_memory( memory_type mem );
	void reflect_preferences();
	WORD get_current_gui_language();
	void SetUILanguage( WORD lang_id );

	void set_module_library( WORD lang_id );
	void put_visible( int visibility );
	
	bool load( const CString file_name, const bool check_empty = true ) ;

	bool load_felix_memory( bool check_empty, const CString & file_name );
	void loading_file_feedback( const CString & file_name );
	void load_failed_feedback( const CString & file_name );

	bool import_trados( const CString &file_name ) ;
	bool import_trados( const file::OpenDlgList &files ) ;
	bool import_tmx( const file::OpenDlgList &files ) ;
	bool import_tmx( const CString &file_name ) ;

	void set_translation_at( short index, const wstring &translation );

	void set_ui_to_current_language();
	void destroy_all_gloss_windows();
	void report_memory_after_load( size_t original_num );
	wstring get_translation_at( short index = -1 );
	MERGE_CHOICE check_empty_on_load();


	bool show_view_content();
	int get_focus_glossary(HWND focus_hwnd = ::GetFocus());
	gloss_window_pointer get_glossary_window();

	double get_score(const short index = -1);
	bool register_trans_to_glossary(const wstring &trans) ;

	void provide_user_trans_feedback() ;
	void provide_user_search_feedback() ;

	void match_count_feedback(size_t num) ;

	void perform_user_search() ;
	void show_user_search_results() ;

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT( ID_VIEW_TOOLBAR,	UPDUI_MENUPOPUP)
		UPDATE_ELEMENT( ID_VIEW_STATUS_BAR,	UPDUI_MENUPOPUP)
		UPDATE_ELEMENT( ID_VIEW_EDIT_MODE,	UPDUI_MENUPOPUP)
		UPDATE_ELEMENT( ID_VIEW_MATCH,		UPDUI_MENUPOPUP)
		UPDATE_ELEMENT( ID_VIEW_SEARCH,		UPDUI_MENUPOPUP)
		UPDATE_ELEMENT( ID_VIEW_REG,		UPDUI_MENUPOPUP)
		UPDATE_ELEMENT( ID_EDIT_REPLACE,	UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	//BEGIN_MSG_MAP_EX(CMainFrame)
public: 
	messageMapType * get_message_map( UINT message );
	UINT get_message_key( UINT message, WPARAM wParam );
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0);

	BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message ) 
	{
		return CCommonWindowFunctionality::handle_sw_exception(e, failure_message) ;
	}
//////////////////////////////////////////////////////////////////////////



	void set_lang_of_gloss_windows();

	void refresh_command_bar();
	void refresh_mru_doc_list(HMENU menu);

	LPCTSTR get_save_filter() ;
	LPCTSTR get_open_filter() ;
	LPCTSTR get_save_ext()
	{
		static LPCTSTR memory_file_ext = _T("ftm") ;

		return memory_file_ext ;
	}
	bool gloss_win_shutdown_check();

	void save_rebar_settings() ;


	CString get_window_type_string();
	bool check_for_clashes( memory_type mem )
	{
		return IDCANCEL != gloss_check_save_location( mem ) ;
	}
	boost::shared_ptr<mem_engine::memory_model> get_memory_model()
	{
		return m_model->get_memories() ;
	}

	void set_bg_color_if_needed();

    virtual void addToMessageLoop();
	void get_matches(mem_engine::trans_match_container &matches, mem_engine::search_query_params &params) ;

	void check_placement( mem_engine::trans_match_container &PlacedMatches, mem_engine::search_match_ptr match);

	LRESULT OnToolTipTextW(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/);

	// the current match
	mem_engine::search_match_ptr get_current_match();

	void redo_lookup( mem_engine::search_match_ptr match, bool do_gloss = false ) ;
	edit_record_dlg_ptr get_editor() 
	{
		return m_editor ;
	}
	bool is_short_format()
	{
		return m_is_short_format ;
	}
	bool is_single_page()
	{
		return !! m_properties->m_view_props.m_data.m_single_screen_matches ;
	}


};


