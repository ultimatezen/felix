#pragma once

#include "MessageMapDefines.h"	// for CMD_HANDER_EX et al
#include "TranslationMemory.h"
#include "memory_model.h"
#include "HtmlView.h"
#include "match_filters.h"
#include "element_wrapper.h"
#include "document_wrapper.h"
#include "pagination.h"
#include "Exceptions.h"

typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
					WS_EX_OVERLAPPEDWINDOW> SearchWindowTraits;

wstring escape_entities(const wstring text) ;

wstring get_filter_text(const std::vector<wstring> & terms) ;
const wstring retrieve_input_value( element_wrapper_ptr input_box ) ;

// The new search window
// Uses hosted IE browser control to show content
class CSearchWindow : 
	public CWindowImpl<CSearchWindow, TWindow, SearchWindowTraits>
	, public html::CHtmlViewListener
	, public CMessageFilter

{
	typedef boost::shared_ptr<mem_engine::memory_model> memory_controller ;
	typedef std::vector<mem_engine::search_match_ptr> match_vec ;
	typedef mem_engine::search_match_ptr search_match_ptr ;
public:
	// the mainframe/glossary window passes in a list of memories
	memory_controller m_controller ;
	// location of window settings
	CString m_settings_key ;
	// window title
	CString m_title ;

	// the HTML view
	html::CHtmlView m_view ;

	// performs searches, wraps list of search terms
	memory_searcher::search_runner m_search_runner ;

	// for paginating search results
	Paginator m_paginator ;

	// holds pointer to deleted match (to enable undo)
	mem_engine::search_match_ptr m_deleted_match ;

	match_vec m_matches ;
	match_vec m_replace_matches ;
	size_t m_current_match ;
	// any messages to stick to top of search results
	wstring m_message ;	
	CAccelerator				m_accelerator ;

public:
	// sensing stuff for unit testing
	DECLARE_WND_CLASS(_T("SearchWindowClass"))
	DECLARE_SENSING_VAR ;


	doc3_wrapper_ptr get_doc3();
	BOOL PreTranslateMessage(LPMSG pMsg);
	CSearchWindow() : 
		m_settings_key(_T("MemorySearchWindow"))
		, m_title(_T("Search Memory"))
		, m_current_match(0)
	{

	}
	void set_mem_window(bool is_mem);
	void save_window_settings();

	void set_mem_controller(memory_controller controller);

	// ========================
	// URL navigation
	// ========================

	// return true to cancel!
	bool OnBeforeNavigate2(_bstr_t url);

	// navigation handlers
	void handle_deletefilter(doc3_wrapper_ptr doc, wstring url);
	void handle_editrecord(doc3_wrapper_ptr doc, wstring url);
	void handle_deleterecord(doc3_wrapper_ptr doc, wstring url);

	// replace page nav handlers
	void handle_replace_find(doc3_wrapper_ptr doc);
	void handle_replace_replace(doc3_wrapper_ptr doc);
	void handle_replace_all(doc3_wrapper_ptr doc, 
							wstring search_template,
							wstring replace_template);

	bool replace_in_memory( search_match_ptr match,
				const wstring replace_from, 
				const wstring replace_to );

	void delete_record(search_match_ptr match);
	void handle_undodelete(doc3_wrapper_ptr doc);

	void show_search_page();
	void handle_gotoreplace();
	void show_search_results_page();
	void show_replace_results_page();
	void perform_search(doc3_wrapper_ptr doc);

	size_t get_pos_arg(const wstring url);
	mem_engine::search_match_ptr get_match_at( const size_t i );

	void retrieve_and_show_matches( doc3_wrapper_ptr doc );
	void show_search_results(doc3_wrapper_ptr doc, match_vec &matches);
	void get_search_matches(match_vec &matches);
	void show_replace_results(doc3_wrapper_ptr doc, match_vec &matches);

	void perform_replace(doc3_wrapper_ptr doc, mem_engine::record_pointer rec);
	void get_replace_matches(match_vec &matches,
		const wstring replace_from);
	void set_filterbox_text( doc3_wrapper_ptr doc, const std::vector<wstring> &terms );
	void wait_for_doc_complete();

	// ========================
	// message map
	// ========================

	// Win32 message handlers
	LRESULT OnCreate(UINT, WPARAM, LPARAM);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM);
	LRESULT OnSize(UINT, WPARAM, LPARAM);

	LRESULT OnNewSearch();
	LRESULT OnSearch();
	LRESULT OnReplace();
	LRESULT OnToggleHelp();
	BEGIN_MSG_MAP_EX(CSearchWindow)
		try
		{
			// don't use MSG_HANDLER_0 for OnCreate
			// we need the arguments to pass to DefWindowProc
			MESSAGE_HANDLER_EX(WM_CREATE, OnCreate) 
			MESSAGE_HANDLER_EX(WM_SIZE, OnSize) 
			MESSAGE_HANDLER_EX(WM_DESTROY, OnDestroy) 

			BEGIN_CMD_HANDLER_EX

				CMD_HANDLER_EX_0(ID_NEW_SEARCH,		OnNewSearch)
				CMD_HANDLER_EX_0(ID_SEARCH,			OnSearch)
				CMD_HANDLER_EX_0(ID_REPLACE,		OnReplace)
				CMD_HANDLER_EX_0(ID_TOGGLE_HELP,	OnToggleHelp)

			END_CMD_HANDLER_EX
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception") ;
			logging::log_exception(e) ;
			e.notify_user( _T("Error in Search Window"), MB_OK, _T("Search Error"), m_hWnd ) ;		 
		}
		catch (_com_error& e)
		{
			logging::log_error("COM exception") ;
			logging::log_exception(e) ;
			except::CComException com_exception(_T("COM Error"), e) ;		 
			com_exception.notify_user( _T("COM Error in Search Window"), MB_OK, _T("COM Exception"), m_hWnd ) ;		 
		}
		catch (std::exception& e)
		{
			logging::log_error("std::exception") ;
			logging::log_error(e.what()) ;
			const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND ;
			::MessageBox( m_hWnd, CA2T(e.what()), _T("C Runtime Error"), msg_flags ) ;  
		}


	END_MSG_MAP()

};