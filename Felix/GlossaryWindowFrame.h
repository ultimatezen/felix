/*!
	@file GlossaryWindowFrame.h
	@brief The glossary dialog
	@date 2005/06/23
	Time: 21:30:03
	@author Ryan Ginstrom
 */

#pragma once

#include "resource.h"
#include "TranslationMemory.h"			// memory
										// record_pointer
#include "SearchWindow.h"
#include "ManagerWindow.h"		// CManagerWindow
#include "MemoryManagerDlg.h"	// CMemoryManagerDlg

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

#include "WindowExceptionHandler.h"

#include "GlossaryDlgListener.h" // CGlossaryDlgListener

#include "FileOpHandler.h"
#include "WindowsMessage.h"
#include "logging.h"
#include "FelixModelInterface.h"

#include "view_state_initial.h"
#include "view_state_new.h"
#include "view_state_concordance.h"
#include "view_state_match.h"

typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	WS_EX_APPWINDOW | WS_EX_WINDOWEDGE> CGlossaryWindowTraits;

/**
	@brief Class for a glossary window.

	This window holds one or more glossaries. There can be multiple glossary windows.

	The GlossaryWindowFrame class owns the 
 */
class GlossaryWindowFrame :
		public CWindowImpl< GlossaryWindowFrame, CCommonWindowFunctionality, CGlossaryWindowTraits >
		, public CWindowExceptionHandler< GlossaryWindowFrame >
		, public FrameListener
		, public EditRecordInterface
{
	VISIBLE_TO_TESTS

	// various user preferences
	appstate_glossary					m_appstate ;
	app_props::props_ptr				m_props ;
	app_props::properties_glossary		*m_properties_gloss ;

	// the status bar
	HWND						m_hWndStatusBar ;

	CAccelerator				m_accelerator ;

	mem_engine::search_query_glossary		m_search_matches ;
	mem_engine::search_query_glossary		m_concordance_matches ;

	// flag to tell us whether this is the main glossary dialog (for creating title)
	bool							m_is_main ;
	HWND							m_hWndClient ;
	edit_record_dlg_ptr m_editor ;

	CGlossaryWinListener		*m_listener ;

	CToolBarCtrl				m_toolbar ;
	// the memory controller
	model_iface_ptr					m_model ;

	CSearchWindow		m_search_window ;
	CManagerWindow		m_manager_window ;
	CMemoryManagerDlg	m_old_manager_window ;

	ViewStateInitialGloss		m_view_state_initial ;
	ViewStateNewGloss			m_view_state_new;
	ViewStateConcordanceGloss	m_view_state_concordance ;
	ViewStateMatchGloss			m_view_state_match ;

	bool m_is_trans_concordance ;

	input_device_ptr	m_input_device ;
	output_device_ptr	m_output_device ;

public:
	// seams for testing
	boost::function<HWND(HWND)> m_create ;
	boost::function<void(const int)> m_apply_settings ;
	boost::function<INT_PTR(void)> m_check_save ;

	static const int IDD = IDD_GLOSSARYVIEW ;
	DECLARE_WND_CLASS(_T("Glossary Window"))

	GlossaryWindowFrame(app_props::props_ptr props) ;
	virtual ~GlossaryWindowFrame( ) ;

	mem_engine::felix_query *get_current_matches();
	input_device_ptr get_input_device();
	output_device_ptr get_output_device();
	void get_qc_messages(mem_engine::record_pointer record, std::vector<wstring> &messages);
	app_props::props_ptr get_properties();
	void apply_window_settings(const int show_cmd);
	HWND create(HWND parent);

	void set_bg_color_if_needed();

	void set_display_state( DISPLAY_STATE new_state );

	void check_load_history() ;

	void load_history();
	void check_save_history() ;
	MERGE_CHOICE check_empty_on_load() ;
	bool has_name_clash(CString location);

	model_iface_ptr get_model()
	{
		return m_model ;
	}
	INT_PTR check_save_memory( mem_engine::memory_pointer mem );

	// importing/exporting multiterm files
	void import_tabbed_text( const CString &file_name );
	void import_multiterm(std::vector<CString> import_files);
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

	void show_view_content() ;

	void do_save( mem_engine::memory_pointer mem ) ;

	void config_matches_for_gloss_lookup(const std::wstring& query_text) ;

	void perform_gloss_lookup() ;

	void prep_for_gloss_lookup(const std::wstring& query_text) ;

	std::wstring get_record_translation(mem_engine::record_pointer entry) ;

	void give_added_record_feedback(mem_engine::memory_pointer& mem) ;

	void load_felix_files(std::vector<CString> import_files) ;

	void give_user_search_feedback() ;

	void prep_user_search() ;

	void prep_concordance_search(const std::wstring& query_string) ;

	void perform_concordance_search() ;
	void config_concordance_search_settings() ;
	void show_concordance_results() ;

	void show_post_edit_content() ;
	void route_nav_command(LPMSG pMsg) ;

	void SetEditModeMenuItems(const bool edit_mode_enabled);
	void ToggleEditMode() ;

	void HandleEditModeFind() ;

	bool OnBeforeNavigate2( _bstr_t url ) ;
	void set_listener( CGlossaryWinListener *listener) ;

	void put_show_marking( VARIANT_BOOL setting ) ;
	VARIANT_BOOL get_show_marking() ;

	bool is_main() { return m_is_main ; }
	void set_main_on() { m_is_main = true ; }
	void set_main_off() { m_is_main = false ; }

	// these need to be public because they are commands from the CMainFrame
	void set_up_initial_size();
	void set_ui_language();

	bool clear_memory() ;

	LRESULT handle_find() ;
	
	size_t num_matches() ;
	bool load( const CString file_name, const bool check_empty = true  ) ;

	const MERGE_CHOICE get_merge_type( const bool check_empty );

	wstring build_glossary_list(mem_engine::search_query_glossary &search_matches) ;
	void lookup(const std::wstring& query_text) ;
	wstring get_glossary_entry(const int index) ;

	int get_local_index( const int index );

	bool add_record(mem_engine::record_pointer record, const CString gloss_name = CString() ) ;
	bool add_record(mem_engine::record_pointer record, const size_t i );

	bool add_record(mem_engine::memory_pointer mem, mem_engine::record_pointer record );
	bool show_new_record() ;
	bool handle_open() ;
	mem_engine::record_pointer get_record( size_t pos ) ; 

	bool set_main ( bool setting ) ; 
	bool set_window_title() ;

	CString get_memory_name( mem_engine::memory_pointer mem);
	bool pre_shutdown_save_check() ;
	
	bool get_translation_concordances(const wstring query_string) ;
	bool get_concordances( const wstring query_string )  ;
	LPCTSTR get_save_ext();

	bool exit_silently() ;
	BOOL PreTranslateMessage( LPMSG pMsg ) ;


	void retrieve_record_results_state();

	void set_doc_ui_handler();
	HRESULT show_doc_context_menu();

	//============================
	// Message handlers
	//============================
	LRESULT on_view_switch() ;

	LRESULT on_trans_concordance() ;
	LRESULT on_source_concordance() ;

	LRESULT on_mru_file_open(WORD wID ) ;

	void open_mru_file( WORD wID, input_device_ptr input );
	LRESULT on_create( ) ;

	void apply_mousewheel_setting();


	void apply_reg_bg_color();
	LRESULT on_close_command( ) ;
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam ) ;

	LRESULT OnFileExport( ) ;
	LRESULT on_file_new( ) ;
	LRESULT on_file_open(  ) ;
	LRESULT on_file_connect();
	LRESULT on_file_save( ) ;
	LRESULT on_file_save_as(  ) ;

	void save_memory_as( mem_engine::memory_pointer mem );

	LRESULT on_memory_close();

	LRESULT on_find( ) ;
	LRESULT on_new_search( );
	LRESULT on_edit_replace( ) ;
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

	LRESULT on_view_simple() ;

	LRESULT on_view_match( ) ;
	LRESULT on_view_search( )	;

	LRESULT on_view_zoom();
	LRESULT on_toggle_views();


	// user messages
	LRESULT on_user_editEntry( LPARAM lParam ) ;
	LRESULT on_user_delete( size_t lParam ) ;
	LRESULT on_user_search( LPARAM lParam ) ;
	LRESULT on_user_edit_replace( LPARAM lParam ) ;
	LRESULT OnUserAdd( LPARAM lParam ) ;
	LRESULT OnUserPrev( LPARAM lParam ) ;

	void add_edit_record(mem_engine::record_pointer new_record, LPARAM display_state);
	void edit_edit_record(mem_engine::record_pointer new_record, LPARAM display_state);

	// TOOLS
	LRESULT on_tools_memory_manager();
	
	// helpers
	LRESULT handle_user_search() ;

	// ===================
	// Drag and Drop
	// ===================
	LRESULT OnDrop( HDROP dropped );

	// ========================
	// message map
	// ========================
	BEGIN_MSG_MAP_EX( GlossaryWindowFrame )
	MSG_TRY
		MSG_HANDLER_0(WM_CREATE, on_create)
		MSG_HANDLER_0(WM_DESTROY, OnDestroy)
		// we need the generic handler, because we pass on the size command to the status bar
		MESSAGE_HANDLER_EX(0x020A/*WM_MOUSEWHEEL*/, OnMouseWheel)
		MESSAGE_HANDLER_EX(WM_SIZE, OnSize)
		MSG_WM_DROPFILES( OnDrop )

		MSG_HANDLER_WIN_MSG(WM_ACTIVATE, on_activate)

		// ----
		BEGIN_USER_HANDLER_EX
			USER_HANDLER_EX(ID_USER_SEARCH, on_user_search )
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

			CMD_HANDLER_EX_0(ID_EDIT_REPLACE, on_edit_replace )

			CMD_HANDLER_EX_0(IDC_GLOSS_SIMPLE_VIEW, on_view_simple)

			// view menu
			else if ( ID_VIEW_EDIT_MODE == LOWORD( wParam ) ) 
			{ 
				WindowsMessage message( hWnd, uMsg, wParam, lParam ) ;
				SetMsgHandled(TRUE); 
				lResult = (LRESULT)on_view_edit_mode( message); 
				if(IsMsgHandled()) 
				{
					return TRUE; 
				}
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
			string err_msg = "Structured Windows Exception" ;

			report_structured_windows_exception(err_msg, sw_e) ;
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

	BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message );


	void edit_record( mem_engine::record_pointer rec ) ;


	void refresh_menu();
	void refresh_mru_doc_list(HMENU menu);

	model_iface_ptr get_memory_model();
	CString get_window_type_string() ;
	bool is_glossary_window();

	void add_glossary(mem_engine::memory_pointer mem);

	void seed_random_numbers();
	bool check_for_clashes( mem_engine::memory_pointer mem ) ;
	void init_toolbar() ;
	void reflect_tb_vis() ;
	void reflect_sb_vis() ;

	LRESULT OnToolTipTextW(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/);

	void add_glossary_specific_tooltips();


	void delete_record(mem_engine::record_pointer rec) ;

	void load_mousewheel_setting() ;

	mem_engine::search_match_ptr get_current_match();
	void redo_lookup( mem_engine::search_match_ptr match, bool do_gloss = false );
	edit_record_dlg_ptr get_editor();
	bool is_short_format();
	bool is_single_page();
	void set_menu_checkmark(int item_id, bool is_checked);
};


typedef std::shared_ptr< GlossaryWindowFrame > gloss_window_pointer ;
