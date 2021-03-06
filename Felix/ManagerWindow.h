#pragma once

#include "MessageMapDefines.h"	// for CMD_HANDER_EX et al
#include "TranslationMemory.h"
#include "memory_model.h"
#include "view_interface.h"
#include "match_filters.h"
#include "element_wrapper.h"
#include "document_wrapper.h"
#include "pagination.h"
#include "Exceptions.h"
#include "SearchWindow.h"
#include "ProgressListener.h"
#include "ManagerView.h"
#include "undoable_action.h"
#include "FelixModelInterface.h"
#include "search_match.h"
#include "Felix_properties.h"
#include "FelixStatusBar.h"

const static string ACTION_NAME_TRIM = "trim" ;
const static string ACTION_NAME_STRIP = "strip" ;

typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
		WS_EX_OVERLAPPEDWINDOW> ManagerWindowTraits;

//! The new management window
//! Uses hosted IE browser control to show content
class CManagerWindow : 
	public CWindowImpl<CManagerWindow, CWindow, ManagerWindowTraits>
	, public html::CHtmlViewListener
	, public CMessageFilter
	, public mgrview::FelixManagerWindowListener
	, public CProgressListener
{
	typedef std::shared_ptr<mem_engine::memory_model> memory_controller ;
	typedef std::vector<mem_engine::search_match_ptr> match_vec ;
	typedef mem_engine::search_match_ptr search_match_ptr ;

	typedef std::shared_ptr<mgrview::ManagerView> mgr_state_ptr ;
public:

	model_iface_ptr m_mem_model ;
	model_iface_ptr m_gloss_model ;
	HWND			m_hWndStatusBar ;

	//! location of window settings
	CString m_settings_key ;

	//! the HTML view
	frame_view m_view ;

	//! any messages to stick to top of search results
	wstring			m_message ;	
	CAccelerator	m_accelerator ;

	mgr_state_ptr	m_current_state ;
	int				m_title_id ;
	size_t		    m_current_item ;
	bool			m_is_memory ;

	FrameListener	*m_listener ;
	CFelixStatusBar m_statusbar ;

	app_props::props_ptr m_props ;

	action::undo_action_ptr m_undo ;

	std::vector<mem_engine::search_match_ptr> m_qc_matches; 

public:
	DECLARE_WND_CLASS(_T("ManagerWindowClass"))
	//! sensing stuff for unit testing
	DECLARE_SENSING_VAR;

	wstring get_message();
	std::vector<mem_engine::search_match_ptr> & get_qc_matches()
	{
		return m_qc_matches; 
	}

	bool init_status_bar();

	doc3_wrapper_ptr get_doc3();
	BOOL PreTranslateMessage(LPMSG pMsg);
	CManagerWindow(app_props::props_ptr props,
					int title_id=IDS_SEARCH_MANAGER_TITLE, 
					LPCTSTR key=_T("MemoryMangerWindow"),
					FrameListener *listener=NULL);
	void set_mem_window(bool is_mem);
	void save_window_settings();

	void set_mem_model(model_iface_ptr model);
	void set_gloss_model(model_iface_ptr model);

	void set_active_state(mgr_state_ptr mgr_state);

	void swap_memories( model_iface_ptr model,
											const int index );

	mem_engine::memory_iterator get_mem_iter_at(model_iface_ptr model,
											int sel );
	CString get_save_prompt( mem_engine::memory_pointer mem );
	bool getMemName(mem_engine::memory_pointer mem) const;

	void add_memory_files(model_iface_ptr model,
		std::vector<CString> import_files);

	void add_memory_file( model_iface_ptr model, CString filename );
	// ========================
	// URL navigation
	// ========================

	//! return true to cancel!
	bool OnBeforeNavigate2(_bstr_t url);

	void redo_delete_record();
	void undo_delete_record();
	void handle_undodelete(doc3_wrapper_ptr doc);


	void wait_for_doc_complete();

	mem_engine::memory_pointer get_mem(string memtype, size_t item_num);
	// ========================
	// CProgressListener
	// ========================
	void OnProgressInit( const CString &file_name, size_t min_val, size_t max_val );
	bool OnProgressLoadUpdate( size_t current_val ) ; //!< true to continue
	bool OnProgressWriteUpdate( size_t current_val )  ; //!< true to continue
	void OnProgressDoneWrite( size_t final_val ) ;
	void OnProgressDoneLoad( size_t final_val ) ;

	// ========================
	// Navigation handlers
	// ========================

	bool nav_empty(const std::vector<string> &tokens);
	//! navigation to various category pages
	bool nav_start(const std::vector<string> &tokens) ;
	bool nav_memories(const std::vector<string> &tokens) ;
	bool nav_glossaries(const std::vector<string> &tokens) ;

	//! moving items in list
	bool nav_moveup(const std::vector<string> &tokens) ;
	bool nav_movedown(const std::vector<string> &tokens) ;

	// undo/redo
	bool undo(const std::vector<string> &tokens);
	wstring create_undo_msg(const std::vector<string> &tokens) const;

	bool redo(const std::vector<string> &tokens);
	wstring create_redo_msg(const std::vector<string> &tokens) const;

	// actions
	bool actions(const std::vector<string> &tokens);
	bool perform_action(const std::vector<string> &tokens);

	void set_undo_action(const string &action, const string &memtype, const string &item);
	// crud
	bool nav_view(const std::vector<string> &tokens) ;
	bool nav_edit(const std::vector<string> &tokens) ;
	bool nav_qc(const std::vector<string> &tokens) ;
	bool nav_browse(const std::vector<string> &tokens) ;
	bool nav_browse_page(const std::vector<string> &tokens) ;
	bool nav_qc_browse(const std::vector<string> &tokens) ;
	bool nav_qc_browse_page(const std::vector<string> &tokens) ;
	bool nav_remove(const std::vector<string> &tokens) ;
	bool nav_addnew(const std::vector<string> &tokens) ;
	bool nav_load(const std::vector<string> &tokens) ;
	bool remove_all(const std::vector<string> &tokens);

	bool delete_record(const std::vector<string> &tokens);
	bool edit_record(const std::vector<string> &tokens);

	bool delete_record_qc(const std::vector<string> &tokens);
	bool edit_record_qc(const std::vector<string> &tokens);


	bool handle_edit_memory(const std::vector<string> &tokens, doc3_wrapper_ptr doc);
	bool handle_qc_settings(doc3_wrapper_ptr doc);

	void set_qc_prop_values( doc3_wrapper_ptr doc );
	bool import_tmx(std::vector<CString> files);

	/** import a single tmx file. */
	bool import_tmx( const CString &file_name );

	bool import_trados(std::vector<CString> files);
	/** Imports a Trados memory.
	 */
	bool import_trados(const CString &trados_file_name);
	// ========================
	// message map
	// ========================
	void import_multiterm(std::vector<CString> import_files);
	//! Import a multiterm file
	void import_multiterm( const CString &file_name );
	void import_tabbed_text( const CString &file_name );

	// Win32 message handlers
	LRESULT OnCreate(UINT, WPARAM, LPARAM);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM);
	LRESULT OnSize(UINT, WPARAM, LPARAM);

	LRESULT OnToggleHelp();
	LRESULT OnInitView();

	BEGIN_MSG_MAP_EX(CManagerWindow)
		try
		{
			// don't use MSG_HANDLER_0 for OnCreate
			// we need the arguments to pass to DefWindowProc
			MESSAGE_HANDLER_EX(WM_CREATE, OnCreate) 
				MESSAGE_HANDLER_EX(WM_SIZE, OnSize) 
				MESSAGE_HANDLER_EX(WM_DESTROY, OnDestroy) 

				BEGIN_CMD_HANDLER_EX

				CMD_HANDLER_EX_0(IDC_STARTUP_CHECKS, OnInitView)

				END_CMD_HANDLER_EX
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception (Manager Window)") ;
			logging::log_exception(e) ;
			e.notify_user( _T("Error in Manager Window"), MB_OK, _T("Manager Window Error"), m_hWnd ) ;		 
		}
		catch (_com_error& e)
		{
			logging::log_error("COM exception (Manager Window)") ;
			logging::log_exception(e) ;
			except::CComException com_exception(_T("COM Error"), e) ;		 
			com_exception.notify_user( _T("COM Error in Manager Window"), MB_OK, _T("COM Exception"), m_hWnd ) ;		 
		}
		catch (std::exception& e)
		{
			logging::log_error("std::exception (Manager Window)") ;
			logging::log_error(e.what()) ;
			const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND ;
			::MessageBox( m_hWnd, CA2T(e.what()), _T("C Runtime Error in Manager Window"), msg_flags ) ;  
		}
	END_MSG_MAP()

};