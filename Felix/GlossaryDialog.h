/*!
	@file GlossaryDialog.h
	@brief The glossary dialog
	@date 2005/06/23
	Time: 21:30:03
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "resource.h"
#include "TranslationMemory.h"			// memory
										// record_pointer
										/// ...
#include "xpmenu/MenuXP.h"
#include "xpmenu/ToolBarXP.h"
#include "SearchWindow.h"

//#include "HtmlView.h"				// CHtmlView
//#include "FindDlg.h"										// CFindDlg
#include "MyRecentDocumentList.h"	// CMyRecentDocumentList
#include "ui.h"					// Save/Open dialog boxes
#include "ImportDialog.h"
#include "data_exporter.h"
//#include "background_processor.h"
#include "CommonWindowFunctionality.h"	// functionality common to mainframe and
										// glossary dialogs

#include "Felix_properties.h"				// properties_glossary struct

#include "MultitermConversionDlg.h"		// CMultitermConversionDlg

#include <list>
#include "WindowExceptionHandler.h"

#include "GlossaryDlgListener.h" // CGlossaryDlgListener

#include "FileOpHandler.h"
#include "WindowsMessage.h"
#include "logging.h"
#include "zoom_dialog.h"
#include "FelixModelInterface.h"

#include "view_state_initial.h"
#include "view_state_new.h"
#include "view_state_concordance.h"
#include "view_state_match.h"
#include "view_state_gloss_lookup.h"

/**
	@class CGlossaryDialog 
	@brief Class for a glossary window.
 */
class CGlossaryWindow :
		public CDialogImpl< CGlossaryWindow, CCommonWindowFunctionality >
		, public CWindowExceptionHandler< CGlossaryWindow >
		, public CZoomInterface
{
	VISIBLE_TO_TESTS


	appstate_glossary			m_appstate ;

	// the status bar
	HWND						m_hWndStatusBar ;

	CAccelerator				m_accelerator ;

	// pointer to newly added record, to allow editing
	mem_engine::search_query_glossary		m_search_matches ;

	// various user preferences
	app_props::properties_glossary		m_properties_gloss ;
	app_props::properties_algorithm		m_properties_algo ;

	// flag to tell us whether this is the main glossary dialog (for creating title)
	bool							m_is_main ;
	HWND							m_hWndClient ;
	edit_record_dlg_ptr m_editor ;

	CGlossaryWinListener		*m_listener ;

	CToolBarCtrl				m_toolbar ;
	// the memory controller
	FelixModel					m_model ;
	boost::shared_ptr<mem_engine::memory_model>		m_memories ;

	CSearchWindow		m_search_window ;

	ViewStateInitialGloss		m_view_state_initial ;
	ViewStateNewGloss			m_view_state_new;
	ViewStateConcordanceGloss	m_view_state_concordance ;
	ViewStateMatchGloss			m_view_state_match ;
	ViewStateGlossLookup		m_view_state_lookup ;

public:
	void set_zoom_level(int zoom_level);
	void set_bg_color_if_needed();

	void set_display_state( DISPLAY_STATE new_state );

	void check_load_history() ;

	void load_history();
	void check_save_history() ;
	MERGE_CHOICE check_empty_on_load() ;

	FelixModelInterface* get_model()
	{
		return &m_model ;
	}


	// importing/exporting multiterm files
	void import_multiterm( const file::OpenDlgList &import_files ) ;
	void import_multiterm( const CString &multiterm_filename ) ;

	void export_multiterm_55( mem_engine::memory_pointer mem, 
							  const CString &file_name ) ;

	void export_multiterm_55_sub( const string source_lang, 
								  string trans_lang, 
								  const CString & file_name, 
								  mem_engine::memory_pointer mem );
	void export_multiterm_6( mem_engine::memory_pointer mem, 
							 const CString &file_name ) ;

	void export_multiterm_6_sub( const wstring source_lang, 
								 const wstring trans_lang, 
								 const CString & file_name, 
								 mem_engine::memory_pointer mem );
	void set_up_recently_used_doclist() ;
	void wait_until_view_not_busy() ;
	bool init_status_bar() ;
	void size_client_and_status_bar() ;

	bool show_view_content() ;

	void do_save( mem_engine::memory_pointer mem ) ;

	int delete_from_new_state() ;

	int delete_from_lookup_state(LPARAM number) ;

	int delete_from_concordance_state(LPARAM number) ;

	void config_matches_for_gloss_lookup(const std::wstring& query_text) ;

	void give_gloss_lookup_feedback() ;

	void perform_gloss_lookup() ;

	void show_gloss_lookup_results() ;

	void prep_for_gloss_lookup(const std::wstring& query_text) ;

	std::wstring get_record_translation(mem_engine::record_pointer& entry) ;

	void give_added_record_feedback(mem_engine::memory_pointer& mem) ;

	void load_felix_files(file::OpenDlgList& import_files) ;

	void give_user_search_feedback() ;

	void prep_user_search() ;
	void show_user_search_results() ;

	void prep_concordance_search(const std::wstring& query_string) ;

	void perform_concordance_search() ;
	void config_concordance_search_settings() ;
	void show_concordance_results() ;

	void show_post_edit_content() ;
	void route_nav_command(LPMSG pMsg) ;

	void SwapFindDialogs(const bool edit_mode_enabled);
	void SetEditModeMenuItems(const bool edit_mode_enabled);
	void ToggleEditMode(const bool edit_mode_enabled) ;

	void HandleEditModeFind() ;

public:
	static const int IDD = IDD_GLOSSARYVIEW ;

	bool OnBeforeNavigate2( _bstr_t url ) ;
	void set_listener( CGlossaryWinListener *listener) ;
	void get_gloss_names( std::list< CString > &names ) ;

	void put_show_marking( VARIANT_BOOL setting ) ;
	VARIANT_BOOL get_show_marking() ;

	bool is_main() { return m_is_main ; }
	void set_main_on() { m_is_main = true ; }
	void set_main_off() { m_is_main = false ; }

	// these need to be public because they are commands from the CMainFrame
	void set_up_initial_size();
	void set_ui_language();

	bool clear_memory() ;

	void set_properties_gloss( const app_props::properties_glossary &props ) ;
	void set_properties_algo( const app_props::properties_algorithm &props ) ;

	LRESULT handle_find() ;
	
	size_t num_matches() ;
	bool load( const CString file_name, const bool check_empty = true  ) ;

	const MERGE_CHOICE get_merge_type( const bool check_empty );
	wstring create_concordance_list(mem_engine::search_query_glossary &search_matches) ;
	wstring build_glossary_list(mem_engine::search_query_glossary &search_matches) ;
	void handle_glossary_lookup(const std::wstring& query_text) ;
	void report_deleted_entry() ;
	wstring get_glossary_entry(const int index) ;
	bool add_record(mem_engine::record_pointer record, const CString gloss_name = CString() ) ;
	bool add_record(mem_engine::record_pointer record, const size_t i );
	bool show_new_record() ;
	bool handle_open() ;
	mem_engine::record_pointer get_record( size_t pos ) ; 
	void handle_new_record_edit( bool edit_mode_enabled ) ;
	void handle_concordance_edit( bool edit_mode_enabled ) ; 
	void handle_enter_edit_mode_new_record() ;
	void handle_enter_edit_mode_concordance() ;
	void handle_leave_edit_mode_new_record() ;
	void handle_leave_edit_mode_concordance() ;
	bool set_main ( bool setting ) ; 
	bool set_window_title() ;

	CString get_memory_name( mem_engine::memory_pointer mem);
	bool pre_shutdown_save_check() ;
	
	bool get_translation_concordances(const wstring query_string) ;
	bool get_concordances( const wstring query_string )  ;
	LPCTSTR get_save_ext()
	{
		static LPCTSTR memory_file_ext = _T("fgloss") ;

		return memory_file_ext ;
	}

	CGlossaryWindow( ) ;
	~CGlossaryWindow( ) ;

	bool exit_silently() ;
	BOOL PreTranslateMessage( LPMSG pMsg ) ;


	void retrieve_record_results_state();

	void set_doc_ui_handler();
	HRESULT get_doc_context_menu();

	//============================
	// Message handlers
	//============================
	LRESULT on_view_switch() ;

	LRESULT on_trans_concordance() ;
	LRESULT on_source_concordance() ;

	LRESULT on_mru_file_open(WORD wID ) ;

	LRESULT OnInitDialog( ) ;

	void apply_mousewheel_setting();
	void apply_reg_bg_color();
	void load_reg_settings();
	LRESULT on_close_command( ) ;
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam ) ;

	LRESULT OnFileExport( ) ;
	LRESULT on_file_new( ) ;
	LRESULT on_file_open(  ) ;
	LRESULT on_file_connect();
	LRESULT on_file_save( ) ;
	LRESULT on_file_save_as(  ) ;

	LRESULT on_memory_close();

	LRESULT on_find( ) ;
	LRESULT on_new_search( );
	LRESULT on_edit_replace( ) ;
	LRESULT on_delete( ) ;
	LRESULT on_add( ) ;
	LRESULT OnDestroy( ) ;

	LRESULT on_edit_concordance();
	LRESULT on_edit_edit();
	LRESULT on_edit_delete();


	void save_prefs();
	// view 
	LRESULT on_view_edit_mode( WindowsMessage &message ) ;
	LRESULT on_view_status_bar( ) ;
	LRESULT on_view_toolbar( ) ;
	LRESULT on_view_match( ) ;
	LRESULT on_view_search( )	;

	LRESULT on_view_zoom();
	LRESULT on_toggle_views();


	// user messages
	LRESULT on_user_editEntry( LPARAM lParam ) ;
	LRESULT on_user_delete( LPARAM lParam ) ;
	LRESULT on_user_search( LPARAM lParam ) ;
	LRESULT on_user_edit_replace( LPARAM lParam ) ;
	LRESULT OnUserAdd( LPARAM lParam ) ;
	LRESULT OnUserPrev( LPARAM lParam ) ;
	LRESULT on_user_retrieve_edit_record( LPARAM lParam ) ;

	// TOOLS
	LRESULT on_tools_memory_manager();
	
		// helpers
		LRESULT on_user_retrieve_edit_recordNew() ;
		LRESULT on_user_retrieve_edit_recordLookup() ;
		LRESULT on_user_retrieve_edit_recordConcordance() ;
		LRESULT handle_user_search() ;

	// ===================
	// Drag and Drop
	// ===================
	LRESULT OnDrop( HDROP dropped );

	// ========================
	// message map
	// ========================
	BEGIN_MSG_MAP_EX( CGlossaryWindow )
	MSG_TRY
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		MSG_HANDLER_0(WM_DESTROY, OnDestroy)
		// we need the generic handler, because we pass on the size command to the status bar
		MESSAGE_HANDLER_EX(0x020A/*WM_MOUSEWHEEL*/, OnMouseWheel)
		MESSAGE_HANDLER_EX(WM_SIZE, OnSize)
		MSG_WM_DROPFILES( OnDrop )

		// ----
		BEGIN_USER_HANDLER_EX
			USER_HANDLER_EX(ID_USER_SEARCH, on_user_search )
			USER_HANDLER_EX(IDC_RETRIEVE_EDIT_RECORD, on_user_retrieve_edit_record )
			USER_HANDLER_EX_0(IDC_REPLACE_EDIT_RECORD, on_user_replace_edit_record )
			USER_HANDLER_EX(ID_EDIT_FIND, on_user_edit_search )
			USER_HANDLER_EX(ID_EDIT_REPLACE, on_user_edit_replace )
		END_USER_HANDLER_EX

		// ----
		BEGIN_CMD_HANDLER_EX

			CMD_HANDLER_EX_0( IDC_DEMO_CHECK_EXCESS,	on_demo_check_excess_memories )

			// concordance
			CMD_HANDLER_EX_0(IDC_SOURCE_CONCORDANCE_SEL, on_source_concordance)
			CMD_HANDLER_EX_0(IDC_TRANS_CONCORDANCE_SEL, on_trans_concordance)

			CMD_HANDLER_EX_0(ID_TOOLS_MEMORY_MGR,		on_tools_memory_manager)
										  
			// formatting
			CMD_HANDLER_EX_0(IDC_UNDERLINE, on_underline)
			CMD_HANDLER_EX_0(IDC_BOLD, on_bold)
			CMD_HANDLER_EX_0(IDC_ITALIC, on_italic)
			CMD_HANDLER_EX_0(ID_FORMAT_BGCOLOR, OnFormatBackgroundColor)

			// file menu
			CMD_HANDLER_EX_0(ID_FILE_NEW, on_file_new)
			CMD_HANDLER_EX_0(ID_FILE_OPEN, on_file_open)
			CMD_HANDLER_EX_0(ID_FILE_SAVE, on_file_save)
			CMD_HANDLER_EX_0(ID_FILE_SAVE_AS, on_file_save_as)
			CMD_HANDLER_EX_0(ID_FILE_SAVE_ALL, on_file_save_all)
			CMD_HANDLER_EX_0(ID_FILE_CONNECT, on_file_connect)
			CMD_HANDLER_EX_0(ID_MEMORY_CLOSE, on_memory_close)
			CMD_RANGE_HANDLER_EX_ID( ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, on_mru_file_open )

			// edit menu
			CMD_HANDLER_EX_0(ID_EDIT_ADD, on_add )
			CMD_HANDLER_EX_0(ID_EDIT_FIND, on_new_search )

			CMD_HANDLER_EX_0(ID_FIND_QUICKSEARCH, on_find )

			CMD_HANDLER_EX_0(IDD_CONCORDANCE, on_edit_concordance)
			CMD_HANDLER_EX_0(IDD_EDIT_ENTRY, on_edit_edit)
			CMD_HANDLER_EX_0(ID_EDIT_DELETE, on_edit_delete)

			CMD_HANDLER_EX_0(ID_EDIT_DELETE, on_delete )
			CMD_HANDLER_EX_0(ID_EDIT_REPLACE, on_edit_replace )

			// view menu
		else if ( ID_VIEW_EDIT_MODE == LOWORD( wParam ) ) 
	{ 
		WindowsMessage message( hWnd, uMsg, wParam, lParam ) ;
		SetMsgHandled(TRUE); 
		lResult = (LRESULT)on_view_edit_mode( message); 
		if(IsMsgHandled()) 
			return TRUE; 
		}

			CMD_HANDLER_EX_0(ID_NEXT_PANE, on_toggle_views)


			CMD_HANDLER_EX_0(ID_VIEW_STATUS_BAR, on_view_status_bar)
			CMD_HANDLER_EX_0(ID_VIEW_TOOLBAR, on_view_toolbar)
			CMD_HANDLER_EX_0(ID_VIEW_MATCH, on_view_match)
			CMD_HANDLER_EX_0(ID_VIEW_SEARCH, on_view_search)
			CMD_HANDLER_EX_0(ID_VIEW_SWITCH, on_view_switch)

			CMD_HANDLER_EX_0(ID_VIEW_ZOOM, on_view_zoom)
			// from dialogs
			CMD_HANDLER_EX_0(IDOK, on_close_command)
			CMD_HANDLER_EX_0(IDCANCEL, on_close_command)
			CMD_HANDLER_EX_0(IDCLOSE, on_close_command)

			// chain to members
			if( m_view_interface.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult) ) 
				return TRUE; 

		END_CMD_HANDLER_EX
		
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFOW, OnToolTipTextW)
		}
		catch ( except::CSWException &sw_e ) 
		{ 
			logging::log_error("Structured Windows Exception") ;
			logging::log_exception(sw_e) ;
			CString language = L"English" ;
			const CString lang_code = resource_string(IDS_LANG_CODE) ;
			if(lang_code == L"jp")
			{
				language = L"Japanese" ;
			}
			logging::send_report(language, sw_e.get_filename()) ;
			return FALSE ;
		} 
		catch ( _com_error &e ) 
		{ 
			logging::log_exception(e) ;
			CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
			fail_msg += _T(": COM ERROR") ;
			except::sb_text(m_hWndStatusBar, fail_msg ) ; 
			return handle_exception( e, fail_msg ) ; 
		} 
		catch ( except::CComException &e ) 
		{ 
			logging::log_exception(e) ;
			CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
			fail_msg += _T(": COM EXCEPTION") ;
			except::sb_text(m_hWndStatusBar, fail_msg ) ; 
			return handle_exception( e, fail_msg ) ;
		} 
		catch ( except::CWinException &e ) 
		{ 
			logging::log_exception(e) ;
			CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
			fail_msg += _T(": WINDOWS ERROR") ;
			except::sb_text(m_hWndStatusBar, fail_msg ) ; 
			return handle_exception( e, fail_msg ) ;
		} 
		catch ( except::CException &e ) 
		{ 
			logging::log_exception(e) ;
			CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
			fail_msg += _T(": EXCEPTION") ;
			except::sb_text(m_hWndStatusBar, fail_msg ) ; 
			return handle_exception( e, fail_msg ) ; 
		} 
		catch ( std::exception &e ) 
		{ 
			logging::log_error(e.what()) ;
			CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
			fail_msg += _T(": RUNTIME ERROR") ;
			except::sb_text(m_hWndStatusBar, fail_msg ) ; 
			return handle_exception( e, fail_msg ) ; 
		}

	END_MSG_MAP()

	LRESULT OnFormatBackgroundColor() ;

	BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message ) 
	{
		return CCommonWindowFunctionality::handle_sw_exception(e, failure_message) ;
	}


	void edit_record( mem_engine::record_pointer rec ) ;


	void refresh_menu();
	void refresh_mru_doc_list(HMENU menu);

	LPCTSTR get_save_filter() ;
	LPCTSTR get_open_filter() ;

	boost::shared_ptr<mem_engine::memory_model> create_memory_model()
	{
		return boost::shared_ptr<mem_engine::memory_model>(new mem_engine::memory_model_gloss()) ;
	}
	boost::shared_ptr<mem_engine::memory_model> get_memory_model() 
	{
		return m_memories ;
	}
	CString get_window_type_string() ;

	void add_glossary(mem_engine::memory_pointer mem)
	{
		m_memories->insert_memory( mem ) ;
		check_mousewheel() ;
		set_window_title() ;
	}

	void seed_random_numbers();
	bool check_for_clashes( mem_engine::memory_pointer mem ) ;
	void init_toolbar() ;
	void reflect_tb_vis() ;
	void reflect_sb_vis() ;

	LRESULT OnToolTipTextW(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/);
	void delete_record(mem_engine::record_pointer rec) ;

	void load_util_settings() ;
	void save_util_settings() ;

	mem_engine::search_match_ptr get_current_match()
	{
		return m_search_matches.current() ;
	}
	void redo_lookup( mem_engine::search_match_ptr match, bool do_gloss = false )
	{
		match ;
		do_gloss ;
	}
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
		return true ;
	}

};


typedef boost::shared_ptr< CGlossaryWindow > gloss_window_pointer ;
typedef std::vector< gloss_window_pointer > gloss_window_list ;
typedef std::vector< gloss_window_pointer >::iterator gloss_window_iterator ;

