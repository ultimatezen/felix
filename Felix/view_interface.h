/*!
	@brief Interface for the frame_view class.
	@author Ryan Ginstrom
 */

#pragma once

#include "record.h"
#include "query.h"
#include "background_processor.h"
#include "FelixModelInterface.h"
#include "HtmlView.h"
#include "HtmlDocument.h"
#include "data_converter.h"
#include "WebPage.h"

// This is an interface to the underlying view.
// (Wraps an HTML view)
class view_interface
{
public:
	typedef mem_engine::translation_match_query TransMatchQueryTrans ;
	typedef model_iface_ptr MemoryControllerType ;
	typedef std::vector< mem_engine::search_match_ptr > MatchListType ;
	typedef mem_engine::search_match_ptr SearchMatchType ;

	virtual bool is_edit_mode() = 0 ;
	virtual void put_edit_mode( bool setting ) = 0 ;
	virtual void ensure_document_complete() = 0 ;
	virtual void ensure_navigation_complete() = 0 ;
	virtual void navigate(LPCTSTR url) = 0 ;
	virtual void set_text( const wstring text ) = 0 ;
	virtual void set_scroll_pos( long pos ) = 0 ;
	virtual void scroll_element_into_view( const wstring current_id ) = 0 ;

	// =========================
	// for entering edit mode
	// =========================

	virtual void handle_enter_edit_mode_new_record_glossary( ) = 0 ;
	virtual void handle_enter_edit_mode_concordance_glossary( mem_engine::felix_query *matches ) = 0  ;

	virtual void handle_enter_edit_mode_new_record() = 0  ;

	virtual void handle_enter_edit_mode_match( mem_engine::felix_query *matches ) = 0  ;

	virtual mem_engine::record_pointer get_match_record( mem_engine::felix_query *matches ) = 0 ;
	virtual void handle_enter_edit_mode_concordance( mem_engine::felix_query *matches ) = 0  ;

	// =========================
	// for leaving edit mode
	// =========================
	virtual void handle_leave_edit_mode_new( mem_engine::record_pointer &record ) = 0 ;

	virtual void handle_leave_edit_mode_match( MemoryControllerType memories, mem_engine::felix_query *matches ) = 0 ;
	virtual bool handle_leave_edit_mode_concordance_glossary( MemoryControllerType memories, mem_engine::felix_query *matches ) = 0 ;
	virtual bool handle_leave_edit_mode_new_record_glossary( MemoryControllerType memories, mem_engine::record_pointer &record ) = 0 ;

	virtual bool handle_leave_edit_mode_concordance( MemoryControllerType memories, mem_engine::felix_query *matches ) = 0 ;

	virtual html::document3_ptr get_document3() = 0 ;

	virtual void run_script(CString script) = 0 ;
	virtual void run_script(CString script, CString arg1) = 0 ;
};

/**
	@class frame_view 
	@brief Interface to the view -- adds various functions specific to Felix.
 */
class frame_view : public view_interface
{

public:
	// view
	html::CHtmlView	m_view ;
	HACCEL m_accel ;
	HWND   m_parent ;

	frame_view() : m_accel(NULL), m_parent(NULL)
	{
	}
	operator html::CHtmlView&() { return m_view ; }
	void set_accel(HACCEL accel)
	{
		m_accel = accel ;
	}

	void set_listener( html::CHtmlViewListener *listener ) ;
	void ensure_document_complete() ;
	void ensure_navigation_complete() ;
	void destroy();
	const wstring get_selection_text() ;

	void create( HWND parent, HWND &client, CString filename=CString() ) ;

	void set_text( const wstring text ) ;
	void load_resource( LPCTSTR resource_name ) ;
	void navigate(LPCTSTR url) ;
	void do_bold() ;
	void do_underline();
	void do_italic() ;
	void do_delete() ;
	void set_bg_color( const wstring color )  ;
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
#ifdef UNIT_TEST
		script ;
		return ;
#else
		html::CWebPage page ;
		page.SetDocument(m_view.get_document()) ;
		page.CallJScript(script) ;
#endif		
	}
	void run_script(CString script, CString arg1)
	{
#ifdef UNIT_TEST
		script ;
		arg1 ;
		return ;
#else
		html::CWebPage page ;
		page.SetDocument(m_view.get_document()) ;
		page.CallJScript(script, arg1) ;
#endif		
	}	
	
	// edit stuff

	// =========================
	// for entering edit mode
	// =========================

	void handle_enter_edit_mode_new_record_glossary( ) ;
	void handle_enter_edit_mode_concordance_glossary( mem_engine::felix_query *matches ) ;

	void handle_enter_edit_mode_new_record() ;

	void handle_enter_edit_mode_match( mem_engine::felix_query *matches ) ;

	mem_engine::record_pointer get_match_record( mem_engine::felix_query *matches );
	void handle_enter_edit_mode_concordance( mem_engine::felix_query *matches ) ;

	// =========================
	// for leaving edit mode
	// =========================
	void handle_leave_edit_mode_new( mem_engine::record_pointer &record ) ;

	void handle_leave_edit_mode_match( MemoryControllerType memories, mem_engine::felix_query *matches ) ;
	bool handle_leave_edit_mode_concordance_glossary( MemoryControllerType memories, mem_engine::felix_query *matches ) ;
	bool handle_leave_edit_mode_new_record_glossary( MemoryControllerType memories, mem_engine::record_pointer &record ) ;

	bool handle_leave_edit_mode_concordance( MemoryControllerType memories, mem_engine::felix_query *matches ) ;

	//bool handle_leave_edit_mode_register( mem_engine::search_match &glossary_registration_top, mem_engine::search_match glossary_registration_bottom ) ;

	void scroll_element_into_view( const wstring current_id ) ;

	// Windows messaging stuff
	BOOL PreTranslateMessage( LPMSG pMsg ) ;

	BOOL ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult) ;

	void Move( LPRECT rect ) ;

	wstring get_doc_path( const wstring doc_url );

	html::document3_ptr get_document3()
	{
		return m_view.get_document3() ;
	}

private:
	html::collection_ptr get_element_collection() ;

	void clean_up_urls( html::collection_ptr &collection ) ;

};
