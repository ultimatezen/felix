/*!
	@brief Interface for the view_interface class.
	@author Ryan Ginstrom
 */

#pragma once

#include "record.h"
#include "query.h"
#include "memory_model.h"
#include "background_processor.h"

#include "HtmlView.h"
#include "HtmlDocument.h"
#include <vector>
#include "data_converter.h"
#include "WebPage.h"
/**
	@class view_interface 
	@brief Interface to the view -- adds various functions specific to Felix.
 */
class view_interface
{
	typedef std::vector< memory_engine::search_match_ptr > MatchListType ;
	typedef boost::shared_ptr<memory_engine::memory_model> MemoryControllerType ;
	typedef memory_engine::record_pointer record_pointer ;
	typedef memory_engine::search_match_ptr SearchMatchType ;
	typedef memory_engine::translation_match_query_trans TransMatchQueryTrans ;

public:
	// view
	html::CHtmlView	m_view ;

	operator html::CHtmlView&() { return m_view ; }

	void set_listener( html::CHtmlViewListener *listener ) ;
	void ensure_document_complete(HACCEL accel, HWND hwnd) ;
	void ensure_navigation_complete(HACCEL accel, HWND hwnd) ;
	const wstring get_selection_text() ;

	void create( HWND parent, HWND &client ) ;

	void set_text( const wstring &text ) ;
	void load_resource( LPCTSTR resource_name ) ;
	void navigate(LPCTSTR url) ;
	void do_bold() ;
	void do_underline();
	void do_italic() ;
	void do_delete() ;
	void set_bg_color( const wstring &color )  ;
	wstring get_bg_color() ;
	html::document_ptr get_document() ;

	void set_scroll_pos( long pos )
	{
		m_view.set_scroll_pos(pos) ;
	}
	bool is_edit_mode() ;
	void put_edit_mode( bool setting ) ;

	void run_script(CString script)
	{
		html::CWebPage page ;
		page.SetDocument(m_view.get_document()) ;
		page.CallJScript(script) ;
	}
	// edit stuff

	// =========================
	// for entering edit mode
	// =========================

	void handle_enter_edit_mode_new_record_glossary( ) ;
	void handle_enter_edit_mode_concordance_glossary( memory_engine::search_query_glossary &matches ) ;

	void handle_enter_edit_mode_new_record() ;

	void handle_enter_edit_mode_match( TransMatchQueryTrans &matches ) ;

	record_pointer get_match_record( TransMatchQueryTrans &matches );
	void handle_enter_edit_mode_concordance( memory_engine::search_query_mainframe &matches ) ;

	// =========================
	// for leaving edit mode
	// =========================
	virtual void handle_leave_edit_mode_new( record_pointer &record ) ;

	void handle_leave_edit_mode_match( MemoryControllerType memories, TransMatchQueryTrans &matches ) ;
	bool handle_leave_edit_mode_concordance_glossary( MemoryControllerType memories, memory_engine::search_query_glossary &matches ) ;
	bool handle_leave_edit_mode_new_record_glossary( MemoryControllerType memories, record_pointer &record ) ;

	bool handle_leave_edit_mode_concordance( MemoryControllerType memories, memory_engine::search_query_mainframe &matches ) ;

	//bool handle_leave_edit_mode_register( memory_engine::search_match &glossary_registration_top, memory_engine::search_match glossary_registration_bottom ) ;

	void scroll_element_into_view( const wstring &current_id ) ;

	BOOL PreTranslateMessage( LPMSG pMsg ) ;

	BOOL ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult) ;

	void Move( LPRECT rect ) ;

	wstring get_doc_path( const wstring doc_url );
private:
	html::collection_ptr get_element_collection() ;

	void clean_up_urls( html::collection_ptr &collection ) ;

};
