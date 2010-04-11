/*!
	@file GlossaryDialog.cpp
	@brief implementation file for CGlossaryDialog
	@date 2005/06/25
	Time: 10:04:34
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#include "StdAfx.h"
#include "GlossaryDialog.h"
#include "RegSettings.h"
#include "system_message.h"

#include "Drop.h"	// CDrop

#include "HtmlDocColorSetter.h"

#include "ContentPresenterSimple.h"

#include <atlframe.h>

#include "NumberFmt.h"
#include "xpmenu/Tools.h"		// CWindowRect

#include "ExcelExporter.h"
#include "stringconversions.h"
#include "text_templates.h"
#include "ConnectionDlg.h"
#include "QueryMergeDlg.h"
#include "output_device.h"
#include "ImportMultitermFile.h"
#include "FelixMemDocUIHandler.h"
#include "record_local.h"
#include "ConcordanceDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

#define ZOOM_KEY CComVariant(L"GlossWindowZoom")

using namespace mem_engine ;
using namespace except ;
using namespace html ;
using namespace text_tmpl;

// CTOR
CGlossaryWindow::CGlossaryWindow( ) : 
m_is_main( false ),
m_listener( NULL),
m_editor(new CEditTransRecordDialog)
{ 
	m_is_short_format = true ;
	m_silent_mode = false ;
	m_mousewheel_count = 0;
	m_new_record = record_pointer(new record_local);
	m_review_record = record_pointer(new record_local);
	m_item_under_edit = search_match_ptr(new match_type(record_pointer(new record_local)));

	// initialize states
	this->init_state(&m_view_state_initial) ;
	this->init_state(&m_view_state_new) ;
	this->init_state(&m_view_state_concordance) ;
	m_view_state_concordance.set_search_matches(&m_search_matches) ;
	this->init_state(&m_view_state_match) ;
	this->init_state(&m_view_state_lookup) ;

	set_display_state( INIT_DISPLAY_STATE ) ;
	m_view_state = &m_view_state_initial ;
	//m_new_record = record_pointer(new record_local()) ;
	seed_random_numbers();

	m_memories = m_model.get_memories() ; 
	m_editor->m_is_glossary = true ;

}

// DTOR
CGlossaryWindow::~CGlossaryWindow() 
{ 
}


//============================
// Message handlers
//============================

/** Load a memory from the MRU list.
*/
LRESULT CGlossaryWindow::on_mru_file_open( WORD wID )
{
	CString file_name ;
	m_mru.GetFromList(wID, file_name ) ;
	ATLASSERT( ! file_name.IsEmpty() ) ;
	
	try
	{
		ensure_file_exists(file_name);
		if ( load( file_name ) )
		{
			m_mru.MoveToTop(wID) ;
			set_window_title() ;
		}
		else 
		{
			m_mru.RemoveFromList( wID ) ;
		}
	}
	catch (...)
	{
		m_mru.RemoveFromList( wID ) ;
		throw ;
	}

	return 0;
}

//! Show the find dialog.
LRESULT CGlossaryWindow::handle_find()
{
	init_find_window( SW_RESTORE, IDS_GLOSS_SEARCH ) ;

	ATLASSERT( m_find.IsWindow() ) ;
	ATLASSERT( m_find.IsWindowVisible() ) ;

	return 0 ;
}

LRESULT CGlossaryWindow::OnInitDialog( )
{
	SENSE("OnInitDialog") ;

	logging::log_debug("Initializing glossary dialog") ;

	load_reg_settings();

	load_util_settings() ;
	// create and instantiate html view
	m_hWndClient = init_view() ;

	// set content
	ATLASSERT(m_view_state == &m_view_state_initial) ;
	m_view_state->show_content() ;

	// check it in the menu
	EnableMenuItem( GetMenu(), ID_EDIT_REPLACE, TRUE ) ;

	init_status_bar() ;
	init_toolbar() ;

	wait_until_view_not_busy() ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	set_up_recently_used_doclist() ;
	
	set_window_title() ;

	// query user for color
	apply_reg_bg_color();

	// set focus back to parent
	::SetFocus( GetParent() ) ;
	ATLASSERT( GetFocus() == GetParent() || ::IsChild( GetParent(), GetFocus() ) ) ;
	
	::DragAcceptFiles( m_hWnd, TRUE ) ;

	size_client_and_status_bar() ;

	m_accelerator.LoadAccelerators(IDR_MAINFRAME) ;
	m_background_processor.set_hwnd(*this) ;
	m_background_processor.set_accelerator(m_accelerator) ;

	check_load_history() ;

	apply_mousewheel_setting();

	wait_until_view_not_busy() ;
	set_doc_ui_handler() ;
	return 0;
}

void CGlossaryWindow::check_load_history()
{
	if ( ! is_main() ) 
	{
		return ;
	}

	app_props::properties_general gen_props ;
	gen_props.read_from_registry() ;
	if ( ! gen_props.m_data.m_load_prev_gloss_on_startup ) 
	{
		return ;
	}

	load_history();
}

LRESULT CGlossaryWindow::on_close_command( )
{
	SENSE("OnClose") ;

	if ( check_save() == IDCANCEL) 
	{
		// ignore the command
		SetMsgHandled( TRUE ) ;
		return 1 ;
	}
	 
	ATLASSERT( IsWindow() ) ;

	DestroyWindow() ;
	return 0 ;
}

/** Response to WM_SIZE message.
 * First resize toolbar and status bar, then resize client to
 * fit in rest.
 */
LRESULT CGlossaryWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	SetMsgHandled( FALSE ) ;

	// pass the message to the status bar and toolbar.
	ATLASSERT( m_statusbar.m_mp_sbar.IsWindow() ) ;
	m_statusbar.m_mp_sbar.SendMessage( uMsg, wParam, lParam ) ;
	m_toolbar.SendMessage( uMsg, wParam, lParam ) ;

	size_client_and_status_bar() ;

	return 0;
}

//! Import a multiterm file
void CGlossaryWindow::import_multiterm( const CString &file_name )
{
	CImportMultitermFile importer(this) ;
	importer.import(file_name) ;
	m_memories->insert_memory(importer.m_memory) ;
	set_window_title() ;
}

//! Export a multiterm 6 file.
void CGlossaryWindow::export_multiterm_6( memory_pointer mem, 
										  const CString &file_name )
{
	CExportDialog export_dialog ;
	
	if ( IDCANCEL == export_dialog.DoModal() )
	{
		return ;
	}

	const wstring source_lang = string2wstring(export_dialog.get_source_full()) ;
	const wstring trans_lang = string2wstring(export_dialog.get_trans_full()) ;

	user_feedback( IDS_MSG_EXPORTING_RECORDS ) ;

	export_multiterm_6_sub(source_lang, trans_lang, file_name, mem);
}
void CGlossaryWindow::export_multiterm_6_sub( const wstring source_lang, 
											  const wstring trans_lang, 
											  const CString & file_name, 
											  memory_pointer mem )
{
	multiterm_data_exporter_6 exporter(this) ;
	exporter.set_source(source_lang) ;
	exporter.set_target(trans_lang) ;
	exporter.open_destination(file_name) ;

	exporter.export_gloss(mem) ;

	mem->set_location(file_name) ;
}

void CGlossaryWindow::export_multiterm_55( memory_pointer mem, 
										   const CString &file_name )
{
	CExportDialog export_dialog ;
	
	if ( IDCANCEL == export_dialog.DoModal() )
	{
		user_feedback(IDS_CANCELLED_ACTION) ;
		return ;
	}
	const string source_lang = string2string(export_dialog.get_source_full()) ;
	const string trans_lang = string2string(export_dialog.get_trans_full()) ;

	user_feedback( IDS_MSG_EXPORTING_RECORDS ) ;
	export_multiterm_55_sub(source_lang, trans_lang, file_name, mem);
}
void CGlossaryWindow::export_multiterm_55_sub( const string source_lang, 
											   const string trans_lang, 
											   const CString & file_name, 
											   memory_pointer mem )
{
	multiterm_data_exporter_55 exporter(this) ;

	exporter.set_source(source_lang) ;
	exporter.set_target(trans_lang) ;

	exporter.open_destination(file_name) ;

	exporter.export_gloss(mem) ;

	mem->set_location( file_name ) ;
}

// Function name	: CGlossaryDialog::on_file_new
// Description	    : 
// Return type		: LRESULT 
LRESULT CGlossaryWindow::on_file_new( )
{
	BANNER( "CGlossaryDialog::on_file_new" ) ;

	if ( check_save() == IDCANCEL)
	{
		return 0 ;
	}
	
	m_memories->add_memory() ;

	m_view_interface.set_text( wstring() ) ;
	check_mousewheel() ;
	set_window_title() ;
	
	user_feedback( IDS_NEW ) ;

	return 0 ;
}

LRESULT CGlossaryWindow::OnFormatBackgroundColor()
{
	BANNER( "CGlossaryDialog::OnFormatBackgroundColor" ) ;
	
	CHtmlDocument doc = m_view_interface.get_document() ;
	
	CHtmlDocColorSetter setter ;

	m_properties_gloss.m_data.m_back_color = setter.SetDocBGColor( doc ) ;
	m_properties_gloss.write_to_registry() ;
	
	return 0 ;
}


LRESULT CGlossaryWindow::on_file_open( )
{
	BANNER( "CGlossaryDialog::on_file_open" ) ;

	handle_open() ;
	return 0 ;
}


LRESULT CGlossaryWindow::on_file_save( )
{
	if ( m_memories->empty() )
	{
		return 0L ;
	}

	memory_pointer mem = m_memories->get_first_memory() ; 

	if ( mem->is_new() ) 
	{
		return on_file_save_as() ;
	}

	do_save( mem ) ;

	return 0L ;
}

void CGlossaryWindow::do_save( memory_pointer mem )
{
	file::CFileExtension ext = mem->get_location() ;

	if ( ! (ext.is_xml()  || ext.equals(_T(".fgloss"))))
	{
		switch ( wants_to_save_in_native_format() ) 
		{
		case IDYES:
			{
				if ( ext.equals( _T(".xls" ) ) ) 
				{
					CExcelExporter exporter ( static_cast< CProgressListener* >( this ) ) ;
					exporter.export_excel( mem, mem->get_location() ) ; 
				}
				else
				{
					CMultitermConversionDlg multi_dlg ;
					if ( IDCANCEL == multi_dlg.DoModal( *this ) ) 
					{
						break ;
					}
					if ( IDC_MULTI55 == multi_dlg.get_multiterm_version() ) 
					{
						export_multiterm_55( mem, mem->get_location() ) ;
					}
					else
					{
						export_multiterm_6( mem, mem->get_location() ) ;
					}
				}

			}
			break ;
		case IDNO:
			save_memory( mem ) ;
			break ;
		case IDCANCEL:
			return ;
		}
	}
	else  
	{
		save_memory( mem ) ;
	}

	set_window_title() ;

}

// File -> Save As...
LRESULT CGlossaryWindow::on_file_save_as( )
{
	if ( m_memories->empty() )
	{
		MessageBeep( MB_ICONSTOP ) ;
		return 0L ;
	}
	// clearing location won't work, because we want to offer the current location
	// as the default file name
	CString file_name ;
	memory_pointer mem = m_memories->get_first_memory() ;
	if ( mem->is_new() == false )
	{
		file_name = mem->get_location() ;
	}

	save_file_dlg dialog ;

	if ( ! file_name.IsEmpty() ) 
	{
		file::CPath path( file_name ) ;
		path.RemoveExtension() ;
		dialog.set_default_file( path.Path() ) ;
	}

	CString dialog_title ;
	dialog_title.FormatMessage( IDS_SAVE, resource_string( IDS_GLOSSARY ) ) ;
	dialog.set_prompt( dialog_title ) ;

	dialog.set_filter( get_save_filter() ) ;

	CString save_as_file_name = dialog.get_save_file() ;

	if ( save_as_file_name.IsEmpty() )
	{
		return false ;
	}

	int selected_index = dialog.get_selected_index() ;

	switch( selected_index ) 
	{
	case 1: case 6:
		logging::log_debug("Saving glossary as fgloss file") ;
		fileops::addExtensionAsNeeded( save_as_file_name,  _T( ".fgloss" ) ) ;
		break;

	case 2:
		logging::log_debug("Saving glossary as xml file") ;
		fileops::addExtensionAsNeeded( save_as_file_name,  _T( ".xml" ) ) ;
		break;

	case 3:
		logging::log_debug("Exporting glossary as Multiterm 5.5 file") ;
		fileops::addExtensionAsNeeded( save_as_file_name,  _T( ".txt" ) ) ;
		export_multiterm_55( mem, save_as_file_name ) ;
		return 0L ;

	case 4:
		logging::log_debug("Exporting glossary as Multiterm 6.0 file") ;
		fileops::addExtensionAsNeeded( save_as_file_name,  _T( ".txt" ) ) ;
		export_multiterm_6( mem, save_as_file_name ) ;
		return 0L ;

	case 5:
		{
			logging::log_debug("Exporting glossary as Excel workbook") ;
			fileops::addExtensionAsNeeded( save_as_file_name,  _T( ".xls" ) ) ;
			CExcelExporter exporter( static_cast< CProgressListener* >( this ) ) ;
			exporter.export_excel( m_memories->get_first_memory(), save_as_file_name ) ;
			return 0L ;
		}

	default:
		logging::log_warn("Unknown case is switch statement") ;
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
		logging::log_debug("Saving glossary as fgloss file") ;
		fileops::addExtensionAsNeeded( save_as_file_name,  _T( ".fgloss" ) ) ;
		return 0L ;
	}

	// If the name changes, then we make the current user into the creator
	CString old_location = mem->get_location() ;

	mem->set_location( save_as_file_name ) ;

	save_memory( mem ) ;

	if ( 0 != old_location.CompareNoCase( save_as_file_name ) )
	{
		mem->set_creator_to_current_user( ) ;
	}

	set_window_title() ;

	return 0L ;

}

void CGlossaryWindow::HandleEditModeFind()
{
	if ( m_edit_replace.IsWindow() && m_edit_replace.IsWindowVisible() )
	{
		m_edit_replace.ShowWindow( SW_HIDE ) ;
	}
	m_edit_find.set_document( m_view_interface.get_document() ) ;
	init_edit_find_window( SW_RESTORE ) ;
}

LRESULT CGlossaryWindow::on_find( )
{
	BANNER( "CGlossaryDialog::on_find" ) ;

	if ( m_view_interface.is_edit_mode() )
	{
		HandleEditModeFind();
	}
	else
	{
		handle_find() ;
	}

	return 0L ;
}

LRESULT CGlossaryWindow::on_edit_replace( )
{
	BANNER( "CGlossaryDialog::on_edit_replace" ) ;

	if ( m_view_interface.is_edit_mode() )
	{
		if ( m_edit_find.IsWindow() && m_edit_find.IsWindowVisible() )
			m_edit_find.ShowWindow( SW_HIDE ) ;
		m_edit_replace.set_document( m_view_interface.get_document() ) ;
		init_edit_replace_window( SW_RESTORE ) ;
	}
	else
	{
		::MessageBeep( MB_ICONSTOP ) ;
	}
	return 0 ;
}

LRESULT CGlossaryWindow::on_delete( )
{
	return on_user_delete( 0 ) ;
}

// ===============================


LRESULT CGlossaryWindow::on_add( )
{
	show_edit_dialog_for_new_entry( IDS_ADD_GLOSS_ENTRY ) ;
	return 0;
}


LRESULT CGlossaryWindow::OnDestroy(  )
{
	check_save() ;

	save_prefs();

	m_statusbar.release() ;

	if(m_search_window.IsWindow())
	{
		m_search_window.DestroyWindow() ;
	}


	return 0 ;
}


// ====================
// Other member functions
// ====================


size_t CGlossaryWindow::num_matches()
{
	return m_search_matches.size() ;
}

CGlossaryWindow::MERGE_CHOICE CGlossaryWindow::check_empty_on_load()
{
	if ( m_memories->empty() ) 
	{
		return MERGE_CHOICE_SEPARATE ;
	}

	user_feedback( IDS_GLOSSARY_OPEN ) ;

	memory_pointer first_mem = m_memories->get_first_memory() ;

	CQueryMergeDlg dlg(IDS_MERGE_GLOSS_TITLE, 
		IDS_MERGE_GLOSS_TEXT, 
		fs::wpath(first_mem->get_location()).leaf());

	return get_merge_choice(dlg);

}

bool CGlossaryWindow::load(const CString file_name, const bool check_empty /*= true*/ )
{
	if ( ! ::PathFileExists( file_name ) ) 
	{
		return false ;
	}

	const MERGE_CHOICE should_merge = get_merge_type(check_empty);

	if (should_merge == MERGE_CHOICE_CANCEL)
	{
		return false ;
	}

	user_feedback( system_message( IDS_MSG_LOADING, file::name( file_name ).file_name() ) ) ;

	// merge or add?
	memory_pointer mem ;
	if (should_merge == MERGE_CHOICE_SEPARATE)
	{
		mem = m_memories->add_memory() ;
	}
	else
	{
		ATLASSERT(should_merge == MERGE_CHOICE_MERGE) ;
		mem = m_memories->get_first_memory() ;
	}
	mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

	// remember how many records we have now
	int original_num(0) ;
	original_num = mem->size() ;


	bool success = false ;
	try
	{
		success = mem->load( file_name )  ;
	}
	catch ( ... ) 
	{
		mem->set_listener(NULL) ;
		if (should_merge == MERGE_CHOICE_SEPARATE)
		{
			m_memories->remove_memory_by_id( mem->get_id() ) ;
		}
		throw ;
	}

	mem->set_listener(NULL) ;
	set_window_title() ;

	if ( success )
	{ 
		// success
		::PostMessage( m_hWnd, WM_COMMAND, MAKEWPARAM( IDC_DEMO_CHECK_EXCESS, 100 ), 0 ) ;
		
		m_mru.AddToList(  file_name ) ;
	}
	else
	{
		// set the file name and body element in our memory for loading
		file::CPath path( file_name ) ;
		// we failed to load the memory
		user_feedback( get_load_failure_msg(path.FindFileName()) ) ; //
		m_memories->remove_memory_by_id( mem->get_id() ) ;
	}
	
	return success ;

}


wstring CGlossaryWindow::create_concordance_list(search_query_glossary &search_matches)
{
	wstring html_content ;

	html_content += L"<b>" + R2WSTR( IDS_SEARCH_RESULTS ) + wstring(L":</b>") ;

	if ( search_matches.size() == 0 )
	{
		CStringW msg = system_message_w( IDS_FOUND_X_MATCHES, int_arg_w( 0 ) ) ;
		html_content << L"<p>" <<  static_cast< LPCWSTR >( msg )  << L"</p>" ;
		return html_content ;
	}

	search_matches.m_start_numbering = m_properties_gloss.m_data.m_numbering ;
	return search_matches.get_html_long() ;
}


wstring CGlossaryWindow::build_glossary_list(search_query_glossary &search_matches)
{
	wstring html_content ;

	html_content << L"<b>" << R2WSTR( IDS_SEARCH_RESULTS ) << L":</b>" ;

	if ( search_matches.empty() )
	{
		html_content << L"<p>" << system_message_w( IDS_FOUND_X_MATCHES, 
			int_arg_w( search_matches.size() ) ) ;

		return html_content ;
	}

	search_matches.m_start_numbering = m_properties_gloss.m_data.m_numbering ;
	html_content << search_matches.get_html_short() ;

	return html_content  ;
}


void CGlossaryWindow::handle_glossary_lookup(const std::wstring& query_text)
{
	prep_for_gloss_lookup(query_text);

	perform_gloss_lookup();

	show_gloss_lookup_results();
}

void CGlossaryWindow::prep_for_gloss_lookup(const std::wstring& query_text)
{
	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	// remember where we were, we may want to navigate back
	// remember where we are, makes a difference how we respond to user input
	set_display_state( LOOKUP_DISPLAY_STATE ) ;

	config_matches_for_gloss_lookup(query_text);
}

void CGlossaryWindow::show_gloss_lookup_results()
{
	wstring content = build_glossary_list(m_search_matches) ;
	m_view_interface.set_text( content  ) ;
	m_view_interface.set_scroll_pos(0) ;
	check_mousewheel() ;
	// give the user feedback
	give_gloss_lookup_feedback();
}

void CGlossaryWindow::perform_gloss_lookup()
{
	search_match_container matches ;
	m_memories->get_glossary_matches( matches, m_search_matches.m_params ) ;
	m_search_matches.set_matches( matches ) ;
}

void CGlossaryWindow::give_gloss_lookup_feedback()
{
	if ( m_search_matches.size() == 1 )
	{
		user_feedback( IDS_FOUND_1_MATCH ) ;
	}
	else
	{
		user_feedback(system_message(IDS_FOUND_X_MATCHES, int_arg( m_search_matches.size()))) ;
	}
}

void CGlossaryWindow::config_matches_for_gloss_lookup(const std::wstring& query_text)
{
	// our various parameters
	m_search_matches.m_params.m_ignore_case =		!! m_properties_gloss.m_data.m_ignore_case ;
	m_search_matches.m_params.m_ignore_width =		!! m_properties_gloss.m_data.m_ignore_width ;
	m_search_matches.m_params.m_ignore_hira_kata =	!! m_properties_gloss.m_data.m_ignore_hir_kat ;

	// source
	m_search_matches.set_query_rich(query_text) ;
}


void CGlossaryWindow::report_deleted_entry()
{
	CString feedback_string = resource_string( IDS_DELETED_ENTRY ) ;
	feedback_string += _T(" ") ;

	memory_pointer mem = m_memories->get_first_memory() ;

	feedback_string += system_message( IDS_CURRENT_SIZE, get_window_type_string(), int_arg( mem->size() ) ) ;

	user_feedback( feedback_string ) ;
}

// 

wstring CGlossaryWindow::get_glossary_entry(const int index)
{
	int localIndex = index - m_properties_gloss.m_data.m_numbering ;

	if ( localIndex == -1 )
	{
		localIndex = 9 ; // 10th item by 0 index
	}

	ATLASSERT( localIndex >= 0 && static_cast<size_t>( localIndex )  < m_search_matches.size() ) ;

	if ( localIndex < 0 || static_cast<size_t>( localIndex )  >= m_search_matches.size() ) // idiot deflector code
	{
		user_feedback(IDS_OUT_OF_RANGE) ;
		return wstring( ) ;
	}

	record_pointer entry_record(new record_local()) ;

	if ( index == 0 && get_display_state() == NEW_RECORD_DISPLAY_STATE ) 
	{
		entry_record = m_new_record ;
	}
	else
	{
		entry_record = m_search_matches.at( localIndex )->get_record() ;
	}

	return get_record_translation(entry_record);
}

std::wstring CGlossaryWindow::get_record_translation(record_pointer& entry)
{
	entry->increment_refcount() ;

	wstring trans ;
	if ( m_properties_gloss.m_data.m_plaintext )
	{
		trans = entry->get_trans_plain() ;
		str::replace_all(trans, L"&", L"&amp;") ;
		str::replace_all(trans, L">", L"&gt;") ;
		str::replace_all(trans, L"<", L"&lt;") ;
	}
	else
	{
		trans = entry->get_trans_rich() ;
	}

	if ( m_properties_gloss.m_data.m_to_lower )
	{
		str::make_lower( trans ) ;
	}

	return trans ;		

}



bool CGlossaryWindow::add_record(record_pointer record, const CString gloss_name )
{
	// Add record
	memory_pointer mem(new mem_engine::memory_local()) ;
	if ( gloss_name.IsEmpty() ) 
	{
		mem = m_memories->get_first_memory() ;
	}
	else
	{
		ATLASSERT ( ::PathFileExists( gloss_name ) ) ; 
		mem = m_memories->get_memory_by_name( gloss_name ) ;
	}
	
	if ( !  mem->add_record( record ) )
	{
		if ( mem->is_locked() )
		{
			user_feedback( IDS_GLOSSARY_LOCKED ) ;
			return false ;
		}
		else
		{
			ATLASSERT( mem->record_exists( record ) ) ;
			user_feedback( IDS_ENTRY_EXISTED ) ; // Entry already existed. Updated with any new information. 
		}
	}
	else
	{
		give_added_record_feedback(mem);	
	}
	
	wstring query = m_search_matches.get_query_rich( ) ;

	if ( get_display_state() == LOOKUP_DISPLAY_STATE )
	{
		handle_glossary_lookup( query ) ;
	}
	else if ( get_display_state() == CONCORDANCE_DISPLAY_STATE )
	{
		get_concordances( query ) ;
	}
	// this is to allow the entry to be edited or deleted
	m_new_record = record ;

	// remember where we were, we may want to navigate back
	// remember where we are, makes a difference how we respond to user input
	set_display_state( NEW_RECORD_DISPLAY_STATE ) ;

	show_view_content() ;
	check_mousewheel() ;
	return true ;
}

bool CGlossaryWindow::add_record( record_pointer record, const size_t i )
{
	memory_iterator mempos = this->get_memories().begin() ;
	std::advance(mempos, i) ;
	memory_pointer mem = *mempos ;

	if ( ! mem->add_record( record ) )
	{
		if ( mem->is_locked() )
		{
			user_feedback( IDS_GLOSSARY_LOCKED ) ;
			return false ;
		}
		else
		{
			ATLASSERT( mem->record_exists( record ) ) ;
			user_feedback( IDS_ENTRY_EXISTED ) ; // Entry already existed. Updated with any new information. 
		}
	}
	else
	{
		CString content = _T("[");
		content += get_memory_name(mem) ;
		content += _T("] ");
		content	+= resource_string(IDS_ADDED_TRANSLATION) ;
		content += _T(" ") ;
		content	+=  system_message( IDS_CURRENT_SIZE, get_window_type_string(), int_arg( mem->size() ) ) ;
		user_feedback( content ) ;
	}

	const wstring query = m_search_matches.get_query_rich( ) ;

	if ( get_display_state() == LOOKUP_DISPLAY_STATE )
	{
		handle_glossary_lookup( query ) ;
	}
	else if ( get_display_state() == CONCORDANCE_DISPLAY_STATE )
	{
		get_concordances( query ) ;
	}
	// this is to allow the entry to be edited or deleted
	m_new_record = record ;

	// remember where we were, we may want to navigate back
	// remember where we are, makes a difference how we respond to user input
	set_display_state( NEW_RECORD_DISPLAY_STATE ) ;

	show_view_content() ;
	check_mousewheel() ;
	return true ;

}
void CGlossaryWindow::give_added_record_feedback(memory_pointer& mem)
{
	CString content = resource_string(IDS_ADDED_TRANSLATION ) ;
	content	+= _T(" ") ;
	content	+=  system_message( IDS_CURRENT_SIZE, get_window_type_string(), int_arg( mem->size() ) ) ;
	user_feedback( content ) ;
}

// Got to make this a template!
bool CGlossaryWindow::show_new_record()
{
	BANNER( "CGlossaryDialog::show_new_record()" ) ;

	wstring html_content ;
	html_content << L"<b>" << R2W( IDS_MSG_ADDED_GLOSS_ENTRY_TITLE ) << L"</b>" ;
	// source & trans
	html_content << L"<table class=\"added\" id=\"0\">\n" 
				   << L"<tr><td><b>" << R2W( IDS_SOURCE_TITLE ) << L"</b></td><td class=\"match_content\" id=\"" << R2W( IDS_SOURCE_ID ) << L"\">" << m_new_record->get_source_rich() << L"</td></tr>"
				   << L"<tr><td><b>" << R2W( IDS_TRANS_TITLE )  << L"</b></td><td class=\"match_content\" id=\"" << R2W( IDS_TRANS_ID ) << L"\">" << m_new_record->get_trans_rich() << L"</td></tr>"
				   << L"<tr><td><b>" << R2W( IDS_CONTEXT_TITLE ) << L"</b></td>" ;
	// context
	if ( m_new_record->get_context_plain().empty() )
		html_content << L"<td class=\"match_content\" id=\"context_tmp\"><b><font color=\"red\">" << R2W( IDS_NO_CONTEXT ) << L"</font></b>" ;
	else
		html_content << L"<td class=\"match_content\" id=\"" << R2W( IDS_CONTEXT_ID ) << L"\">" << m_new_record->get_context_rich() ;
	html_content << L"</td></tr>\n" ;
	html_content << L"</table>" ;
	// links
	html_content	<< L"<p><hr>"
			<< L"<a href=\"" << int2wstring( IDC_EDIT ) << L":0\" title=\"" << R2W( IDS_EDIT_TITLE ) << L"\">" << R2W( IDS_EDIT ) << L"</a>" 
			<< L" | <a href=\"" << int2wstring( IDC_DELETE ) << L":0\" title=\"" << R2W( IDS_DELETE_TITLE) << L"\">" << R2W( IDS_DELETE ) << L"</a>"
			<< L" | <a href=\"" << int2wstring( IDC_ADD ) << L":0\" title=\"" << R2W( IDS_ADD2MEM_TITLE ) << L"\">" << R2W( IDS_ADD_TO_MEMORY ) << L"</a>" ;

	html_content << L"<p> <a href=\"" << int2wstring( IDC_PREV ) << L":0\">" << R2W( IDS_BACK ) << L"</a>" ;

	m_view_interface.set_text( html_content ) ;
	m_view_interface.set_scroll_pos(0) ;
	check_mousewheel() ;

	return true ;
}

// File -> Open
bool CGlossaryWindow::handle_open()
{
	user_feedback( IDS_OPEN_GLOSS_FILE ) ;

	// get the file name
	open_file_dlg dialog ;

	dialog.set_prompt( R2T( IDS_OPEN_GLOSS_FILE ) ) ;

	dialog.set_filter( get_open_filter() ) ;

	file::OpenDlgList import_files ;
	if ( ! dialog.get_open_files( import_files ) ) 
	{
		user_feedback( IDS_CANCELLED_ACTION ) ;
		return false ;
	}

	switch(dialog.get_selected_index()) 
	{
	case 1: case 3:
		load_felix_files( import_files ) ;
		break;

	case 2:
		import_multiterm( import_files ) ;
		break ;

	default:
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
	}

	set_window_title() ;
	return true ;
}

void CGlossaryWindow::load_felix_files(file::OpenDlgList& import_files)
{
	foreach(CString filename, import_files.m_filenames)
	{
		load(filename) ;
	}
}

void CGlossaryWindow::import_multiterm( const file::OpenDlgList &import_files )
{
	foreach(CString filename, import_files.m_filenames)
	{
		import_multiterm(filename) ;
	}
	set_window_title() ;
}

// Makes a fresh record so that we can modify it without
// changing the actual glossary entry.
record_pointer CGlossaryWindow::get_record( size_t pos ) 
{ 
	BANNER( "CGlossaryDialog::get_record()" ) ;
	ATLASSERT( pos >= 0 && pos < m_search_matches.size() ) ;
	record_pointer record(m_search_matches.at( pos )->get_record()->clone()) ;  
	return record ;
}


LRESULT CGlossaryWindow::handle_user_search()
{
	prep_user_search();

	mem_engine::search_match_container matches ;
	this->get_memory_model()->perform_search( matches, m_search_matches.m_params ) ;
	m_search_matches.set_matches( matches ) ;	

	// give the user feedback
	show_user_search_results();

	return 0L ;
}

void CGlossaryWindow::prep_user_search()
{
	// remember where we are, makes a difference how we respond to user input
	set_display_state( CONCORDANCE_DISPLAY_STATE ) ;

	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	m_search_matches.clear() ;
	m_search_matches.m_params = m_find.get_search_params() ;
}

void CGlossaryWindow::show_user_search_results()
{
	wstring content = create_concordance_list(m_search_matches) ;
	m_view_interface.set_text( content  ) ;
	check_mousewheel() ;

	give_user_search_feedback();
}

void CGlossaryWindow::give_user_search_feedback()
{
	if ( m_search_matches.size() == 1 )
	{
		user_feedback( IDS_FOUND_1_MATCH ) ;
	}
	else 
	{
		CString msg ; 
		msg.FormatMessage( IDS_FOUND_X_MATCHES, int_arg( m_search_matches.size() ) ) ;
		user_feedback( msg ) ;
	}

}


// toggle toolbar visibility
LRESULT CGlossaryWindow::on_view_toolbar( )
{
	ATLASSERT( m_toolbar.IsWindow() ) ;
	ATLASSERT( m_toolbar.IsWindowVisible() == m_appstate.m_is_toolbar_visible ) ;

	m_appstate.m_is_toolbar_visible = ! m_appstate.m_is_toolbar_visible ;

	reflect_tb_vis() ;

	// get the client rect
	size_client_and_status_bar() ;

	return 0L ;
}

// toggle status bar visibility
LRESULT CGlossaryWindow::on_view_status_bar( )
{
	ATLASSERT( ::IsWindow( m_hWndStatusBar ) ) ;
	ATLASSERT( ::IsWindowVisible(m_hWndStatusBar) == m_appstate.m_is_statusbar_visible ) ;

	m_appstate.m_is_statusbar_visible = ! m_appstate.m_is_statusbar_visible;

	reflect_sb_vis() ;

	// resize the view to fit the new area
	size_client_and_status_bar() ;

	return 0L ;
}

// View -> Edit Mode
LRESULT CGlossaryWindow::on_view_edit_mode(WindowsMessage &)
{
	SENSE( "on_view_edit_mode" ) ;
#ifdef UNIT_TEST
	return 0L ;
#endif

	const bool edit_mode_enabled = m_view_interface.is_edit_mode() ;

	SetEditModeMenuItems(edit_mode_enabled);

	// do the switcheroo on the find windows...
	// swap out the various find dialogs...
	SwapFindDialogs(edit_mode_enabled);
	ToggleEditMode(edit_mode_enabled) ;

	return 0L ;
}

void CGlossaryWindow::ToggleEditMode(const bool edit_mode_enabled)
{
	switch( get_display_state() )
	{
	case INIT_DISPLAY_STATE: 
		m_view_interface.put_edit_mode( ! edit_mode_enabled ) ;
		break ;
	case NEW_RECORD_DISPLAY_STATE:
		handle_new_record_edit( edit_mode_enabled ) ;
		break ;
	case LOOKUP_DISPLAY_STATE: 
	case CONCORDANCE_DISPLAY_STATE:
		handle_concordance_edit( edit_mode_enabled ) ;
		break ;
	default:
		ATLASSERT( "We are in an unknown state" && FALSE ) ;
		break ;
	}
}

void CGlossaryWindow::SetEditModeMenuItems(const bool edit_mode_enabled)
{
	CheckMenuItem( GetMenu(), ID_VIEW_EDIT_MODE, ( edit_mode_enabled ? MF_UNCHECKED : MF_CHECKED ) ) ;
	EnableMenuItem( GetMenu(), ID_EDIT_REPLACE, !! edit_mode_enabled ) ;
}

// We have two find dialogs: 
// * one for edit mode (find in the browser);
// * and one for non-edit mode (quick search in glossary)
// Swap them when we change the edit mode.
void CGlossaryWindow::SwapFindDialogs(const bool edit_mode_enabled)
{
	if ( edit_mode_enabled )
	{
		if ( m_edit_find.IsWindow() && m_edit_find.IsWindowVisible() )
		{
			m_edit_find.ShowWindow( SW_HIDE ) ;
			handle_find() ;
		}
		if ( m_edit_replace.IsWindow() && m_edit_replace.IsWindowVisible() )
			m_edit_replace.ShowWindow( SW_HIDE ) ;
	}
	else
	{
		if ( m_find.IsWindow() && m_find.IsWindowVisible() )
		{
			m_find.ShowWindow( SW_HIDE ) ;
			m_edit_find.ShowWindow( SW_SHOW ) ;
		}
	}

}

// Leaving edit mode finishes our edits.
void CGlossaryWindow::handle_new_record_edit( bool edit_mode_enabled )
{
	if ( ! edit_mode_enabled ) // we are entering edit mode
	{
		handle_enter_edit_mode_new_record() ;
	}
	else
	{
		handle_leave_edit_mode_new_record() ;
	}
}

// Leaving edit mode finishes our edits.
void CGlossaryWindow::handle_concordance_edit( bool edit_mode_enabled ) 
{
	if ( ! edit_mode_enabled ) // we are entering edit mode
	{
		handle_enter_edit_mode_concordance() ;
	}
	else 
	{
		handle_leave_edit_mode_concordance() ;
	}
}

// =========================
// for entering edit mode
// =========================
void CGlossaryWindow::handle_enter_edit_mode_new_record()
{
	user_feedback( IDS_ENTERING_EDIT_MODE ) ;
	m_view_interface.handle_enter_edit_mode_new_record_glossary( ) ;
	user_feedback( IDS_IN_EDIT_MODE ) ;
}


void CGlossaryWindow::handle_enter_edit_mode_concordance()
{
	user_feedback( IDS_ENTERING_EDIT_MODE ) ;
	m_view_interface.handle_enter_edit_mode_concordance_glossary( &m_search_matches ) ;
	user_feedback( IDS_IN_EDIT_MODE ) ;
}

// =========================
// for leaving edit mode
// =========================
void CGlossaryWindow::handle_leave_edit_mode_new_record()
{
	user_feedback( IDS_LEAVING_EDIT_MODE ) ;
	
	if( false == m_view_interface.handle_leave_edit_mode_new_record_glossary( m_memories, m_new_record ) )
	{
		m_view_interface.set_text( R2WSTR( IDS_POST_EDIT_ALL_DELETED ) ) ;
		check_mousewheel() ;
		m_view_interface.set_scroll_pos(0) ;
		user_feedback( IDS_DELETED_ENTRY ) ;
		m_new_record = record_pointer(new record_local()) ;
		::MessageBeep( MB_ICONINFORMATION ) ;
	}
	else
	{
		add_record( m_new_record ) ;

		user_feedback( IDS_LEFT_EDIT_MODE ) ;
	}

}

void CGlossaryWindow::handle_leave_edit_mode_concordance()
{
	user_feedback( IDS_LEAVING_EDIT_MODE ) ;
	
	if( false == m_view_interface.handle_leave_edit_mode_concordance_glossary( m_memories, 
																			   &m_search_matches ) )
	{
		m_view_interface.set_text( R2WSTR( IDS_POST_EDIT_ALL_DELETED ) ) ;
		check_mousewheel() ;
		user_feedback( IDS_DELETED_ENTRY ) ;
		::MessageBeep( MB_ICONINFORMATION ) ;
		return ;
	}

	wstring content ;

	if ( get_display_state() == LOOKUP_DISPLAY_STATE )
	{
		content = build_glossary_list(m_search_matches) ;
	}
	else
	{
		content = create_concordance_list(m_search_matches) ;
	}
	m_view_interface.set_text( content ) ;
	check_mousewheel() ;
	user_feedback( IDS_LEFT_EDIT_MODE ) ;

}

// Make this the main glossary/not the main glossary
bool CGlossaryWindow::set_main ( bool setting ) 
{ 
	m_is_main = setting ;
	return set_window_title() ;
}

// Set the window title.
// Title consists of main declaration (if any) + Glossary - [Main Gloss]
bool CGlossaryWindow::set_window_title()
{
	CString file_name ;
	if ( ! m_memories->empty() )
	{
		memory_pointer mem = m_memories->get_first_memory() ;
		file_name = get_memory_name(mem);
	} 

	CString title ;
	if ( m_is_main )
	{
		ATLVERIFY(title.LoadString( IDS_MAIN )) ;
		title += _T(" ") ;
	}
	title += R2TS( IDS_GLOSSARY ) ;
	title += _T(" [") ;
	title += ( file_name.IsEmpty() ? R2TS( IDS_NEW ) : file_name ) ;
	title += _T("]") ;

	return FALSE != SetWindowText( title ) ;

}							

// Before shutting down, see if there are any glossaries
// that need saving.
bool CGlossaryWindow::pre_shutdown_save_check()
{
	if ( m_memories->empty() )
	{
		return true ;
	} 

	if ( check_save() == IDCANCEL)
	{
		// user does not want to close window
		return false ;
	}

	return true ;
}

// Exit without prompting to save memories.
bool CGlossaryWindow::exit_silently()
{
	memory_list memories ;
	m_memories->get_memories_needing_saving( memories ) ;

	memory_iterator mem_pos ;
	
	for( mem_pos = memories.begin() ; mem_pos != memories.end() ; ++mem_pos ) 
	{
		memory_pointer mem = *mem_pos ;
		mem->set_saved_flag( true ) ;
	}

	return true ;
}

BOOL CGlossaryWindow::PreTranslateMessage( LPMSG pMsg )
{
	ENSURE_FOCUS

	if ( IsWindow() && m_accelerator.TranslateAccelerator( *this, pMsg ) )
	{
		return TRUE ;
	}
	// intercept enter key in order to not shut down dialog 
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		return m_view_interface.PreTranslateMessage( pMsg ) ;
	}

	if ( IsDialogMessage( pMsg ) )
		return TRUE ;

	return m_view_interface.PreTranslateMessage( pMsg ) ;
}

bool CGlossaryWindow::clear_memory()
{
	memory_pointer mem = m_memories->get_first_memory() ;
	mem->clear_memory() ;

	m_new_record = record_pointer(new record_local()) ;
	set_window_title() ;
	
	m_view_interface.set_text( R2WSTR( IDS_GLOSSARY_CLEARED ) ) ;
	check_mousewheel() ;
	user_feedback( system_message( IDS_CLEARED_MEMORY, get_window_type_string() ) ) ;
	m_view_interface.set_scroll_pos(0) ;
	return true ;
}

void CGlossaryWindow::set_properties_gloss( const app_props::properties_glossary &props )
{
	m_properties_gloss = props ;

	m_memories->set_properties_gloss( props ) ;

	show_view_content() ;

	wait_until_view_not_busy() ;

	// query user for color
	set_bg_color((COLORREF)m_properties_gloss.m_data.m_back_color);

}
void CGlossaryWindow::set_properties_algo( const app_props::properties_algorithm &props )
{
	m_properties_algo = props ;

	m_memories->set_properties_algo( props ) ;
}


// ====================
// message handlers
// ====================

// User wants to edit an entry.
LRESULT CGlossaryWindow::on_user_editEntry( LPARAM lParam )
{
	// get the record
	switch( get_display_state() )
	{
	case INIT_DISPLAY_STATE:
	case NEW_RECORD_DISPLAY_STATE:
		{
			memory_pointer mem = m_memories->get_first_memory() ;

			search_match_ptr match(new mem_engine::search_match) ;
			match->set_record(m_new_record) ;
			match->set_memory_id(mem->get_id()) ;
			this->set_item_under_edit(match) ;

			show_edit_dialog( m_new_record, mem->get_id() ) ;

			return 0L ;
		}
	case LOOKUP_DISPLAY_STATE:
	case CONCORDANCE_DISPLAY_STATE:
		{
			// check for out of bounds condition
			TRUE_ENFORCE( static_cast<size_t>(lParam) < m_search_matches.size(), R2T( IDS_OUT_OF_RANGE ) ) ;

			m_search_matches.set_current( lParam ) ;
			break ;
		}
	}

	search_match_ptr match = m_search_matches.current() ;
	this->set_item_under_edit(match) ;
	record_pointer record = match->get_record() ;
	show_edit_dialog( record, match->get_memory_id(), IDS_EDIT_GLOSS ) ;
	return 0L ;
}

// User wants to delete an entry.
LRESULT CGlossaryWindow::on_user_delete( LPARAM number )
{
	switch( get_display_state() )
	{
	case NEW_RECORD_DISPLAY_STATE:
		{
			return delete_from_new_state();		
		}
	case LOOKUP_DISPLAY_STATE:
		{
			return delete_from_lookup_state( number );
		}
	case CONCORDANCE_DISPLAY_STATE:
		{
			return delete_from_concordance_state( number );
		}
	}

	return 0L ;
}

int CGlossaryWindow::delete_from_concordance_state(LPARAM number)
{
	if ( ! check_delete() )  
		return 0 ;

	TRUE_ENFORCE( static_cast<size_t>(number) < m_search_matches.size(), R2T( IDS_OUT_OF_RANGE ) ) ;

	search_match_ptr match = m_search_matches.at( number ) ;

	try
	{
		m_memories->remove_record( match->get_record(), match->get_memory_id() ) ;

		m_search_matches.erase_at( number ) ;

		// feedback
		report_deleted_entry() ;
	}
	catch (CProgramException& e)
	{
		e.notify_user("Failed to remove record: memory not found") ;
	}
	show_user_search_results();

	return 0 ;
}

int CGlossaryWindow::delete_from_lookup_state(LPARAM number)
{
	if ( ! check_delete() )  
	{
		return 0 ;
	}

	if (static_cast<size_t>(number) >= m_search_matches.size())
	{
		MessageBeep(MB_ICONEXCLAMATION) ;
		user_feedback(IDS_OUT_OF_RANGE) ;
		return 0 ;
	}

	search_match_ptr match = m_search_matches.at( number ) ;

	try
	{
		m_memories->remove_record( match->get_record(), match->get_memory_id() ) ;
		m_search_matches.erase_at( number ) ;
		// feedback
		report_deleted_entry() ;
	}
	catch (CProgramException& e)
	{
		e.notify_user("Failed to remove record: glossary not found") ;
	}

	m_view_interface.set_text(build_glossary_list(m_search_matches)) ;
	check_mousewheel() ;

	return 0 ;
}

int CGlossaryWindow::delete_from_new_state()
{
	if ( ! check_delete() ) 
	{
		return 0 ;
	}

	memory_pointer mem = m_memories->get_first_memory() ;

	mem->erase( m_new_record ) ;
	m_new_record = record_pointer(new record_local()) ;
	m_view_interface.set_text( wstring( L"<b>" ) + R2WSTR( IDS_DELETED_ENTRY ) + wstring( L"</b>" ) ) ;
	user_feedback(IDS_DELETED_ENTRY) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;

	report_deleted_entry() ;
	return 0 ;

}

LRESULT CGlossaryWindow::on_user_search( LPARAM /* lParam */ )
{
	return handle_user_search() ;
}

LRESULT CGlossaryWindow::on_user_edit_replace( LPARAM /* lParam */  )
{
	m_edit_find.set_search_params( m_edit_replace.get_search_params() ) ;
	return 0L ;
}

LRESULT CGlossaryWindow::OnUserAdd( LPARAM lParam )
{
	// If we are in the new record display state, there is only one
	// record being displayed, and we have already set m_record to
	// this value. Therefore, all we need to do is pass a reference
	// to this member variable (this will force the CMainFrame to create a
	// new record, rather than a reference-counted copy of this one), and
	// we're done.
	if ( get_display_state() != NEW_RECORD_DISPLAY_STATE )
	{
		m_new_record = m_search_matches.at( lParam )->get_record() ;
	}

	m_listener->gloss_add_record( m_new_record->clone() ) ;

	return 0L ;
}

LRESULT CGlossaryWindow::OnUserPrev( LPARAM /* lParam */ )
{
	set_display_state( LOOKUP_DISPLAY_STATE ) ;
	show_user_search_results();
	return 0L ;
}

LRESULT CGlossaryWindow::on_user_retrieve_edit_record( LPARAM lParam )
{
	// This message has been sent to us by the edit dialog.
	// Since we passed a reference counted record pointer, all
	// we need to do is refresh the display -- the information is already
	// updated

	ATLASSERT( m_editor->get_memory_id() > 0 ) ;

	// set the new display_state (we set this when we called up the editor)
	set_display_state ( static_cast< DISPLAY_STATE >( lParam ) );
	switch ( get_display_state() )
	{
	case INIT_DISPLAY_STATE:
		{
			ATLASSERT(m_view_state == &m_view_state_initial) ;
			m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
											   m_editor->get_new_record()) ;
			break ;
		}
	case NEW_RECORD_DISPLAY_STATE:
		{
			ATLASSERT(m_view_state == &m_view_state_new) ;
			m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
											   m_editor->get_new_record()) ;
			break ;
		}
	case LOOKUP_DISPLAY_STATE:
		{
			ATLASSERT(m_view_state == &m_view_state_lookup) ;
			m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
											   m_editor->get_new_record()) ;
			break ;
		}

	case MATCH_DISPLAY_STATE:
		{
			ATLASSERT(m_view_state == &m_view_state_match) ;
			m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
											   m_editor->get_new_record()) ;
			break ;
		}
	case CONCORDANCE_DISPLAY_STATE:
		{
			ATLASSERT(m_view_state == &m_view_state_concordance) ;
			m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
											   m_editor->get_new_record()) ;
			break ;
		}

	default:
		ATLASSERT( "Unknown state" && FALSE ) ;
	}
	show_view_content() ;
	return 0L ;
}


CString CGlossaryWindow::get_window_type_string()
{
	return resource_string(IDS_GLOSSARY) ;
}

void CGlossaryWindow::route_nav_command(LPMSG pMsg)
{
	switch( pMsg->wParam )
	{
	case IDC_EDIT:
		on_user_editEntry( pMsg->lParam ) ;
		break ;
	case IDC_DELETE:
		on_user_delete( pMsg->lParam ) ;
		break ;
	case IDC_ADD:
		OnUserAdd( pMsg->lParam ) ;
		break ;
	case IDC_PREV:
		OnUserPrev( pMsg->lParam ) ;
		break ;
	}
}

bool CGlossaryWindow::OnBeforeNavigate2( _bstr_t url )
{
	try
	{
		const CString possible_xml_file(static_cast< LPCWSTR >( url )) ;
		const file::CFileExtension ext(possible_xml_file) ;
		if (ext.is_xml() || ext.equals(_T(".fgloss")))
		{
			this->load(possible_xml_file) ;
			return true ; // should cancel
		}
		else if (ext.is_txt())
		{
			this->import_multiterm(possible_xml_file) ;
		}

		const wstring raw_command = BSTR2wstring( url );

		// will return NULL if fails to parse
		destination_parser parser ;
		LPMSG pMsg = parser.parse( raw_command ) ;

		if ( ! pMsg ) 
		{
			const file::CFileExtension html_ext = CString( COLE2CT(url) ) ;
			// If it's an HTML file, don't cancel.
			return ! html_ext.is_html() ;
		}

		route_nav_command(pMsg) ;
	}
	catch (CException& e)
	{
		e.notify_user(_T("Failed to process action")) ;
		return true ;
	}
	catch (_com_error& e)
	{
		CComException cx(e) ;
		cx.notify_user(_T("Failed to process action\rAn error occurred connecting to a COM server.")) ;
		return true ;
	}

	return true ;
}

void CGlossaryWindow::set_listener( CGlossaryWinListener *listener)
{
	m_listener = listener ;
}

void CGlossaryWindow::get_gloss_names( std::list< CString > &names )
{
	if ( m_memories->empty() ) 
	{
		m_memories->add_memory() ;
	}

	ATLASSERT ( ! m_memories->empty() ) ; 

	foreach(memory_pointer mem, m_memories->get_memories())
	{
		names.push_back(mem->get_location( )) ;
	}
	ATLASSERT ( ! names.empty() ) ; 
	ATLASSERT ( names.size() == m_memories->size() ) ; 
}

void CGlossaryWindow::put_show_marking( VARIANT_BOOL setting )
{
	if ( setting == VARIANT_FALSE ) 
	{
		m_search_matches.m_params.m_show_marking = false ;
	}
	else
	{
		m_search_matches.m_params.m_show_marking = true ;
	}
}

VARIANT_BOOL CGlossaryWindow::get_show_marking()
{
	if ( m_search_matches.m_params.m_show_marking == false ) 
	{
		return VARIANT_FALSE ;
	}
	else
	{
		return VARIANT_TRUE ;
	}
}

void CGlossaryWindow::show_post_edit_content()
{
	show_view_content() ;
	user_feedback( IDS_EDIT_COMPLETE ) ;
}


void CGlossaryWindow::set_ui_language()
{
	wait_until_view_not_busy() ;

	refresh_menu() ;

	refresh_windows() ;

	refresh_view_content() ;

	// set new window title
	set_window_title() ;

	wait_until_view_not_busy() ;
	// query user for color
	set_bg_color((COLORREF)m_properties_gloss.m_data.m_back_color);

	user_feedback( IDS_CHANGED_LANGUAGES ) ;
}


void CGlossaryWindow::show_view_content()
{
	if (! IsWindow())
	{
		return ;
	}
	switch ( get_display_state()  )
	{
	case NEW_RECORD_DISPLAY_STATE:
		{
			ATLASSERT(m_view_state == &m_view_state_new) ;
			m_view_state->show_content() ;
			return ;
		}
	case LOOKUP_DISPLAY_STATE: // = glossary lookup
		{
			m_view_interface.set_text(build_glossary_list(m_search_matches)) ;
			m_view_interface.set_scroll_pos(0) ;
			check_mousewheel() ;
			return ;
		}
	case CONCORDANCE_DISPLAY_STATE: // = search from find dialog
		{
			show_user_search_results();
			return ;
		}
	case INIT_DISPLAY_STATE:
		{
			ATLASSERT(m_view_state == &m_view_state_initial) ;
			m_view_state->show_content() ;

			return ;
		}
	default:
		ATLASSERT( "Unkown display state in CGlossaryDialog" && FALSE ) ;
	}
}

LRESULT CGlossaryWindow::on_view_match( ) 
{
	set_display_state( LOOKUP_DISPLAY_STATE ) ;
	show_view_content() ;

	CheckMenuItem( GetMenu(), ID_VIEW_MATCH,  MF_CHECKED   ) ;
	CheckMenuItem( GetMenu(), ID_VIEW_SEARCH, MF_UNCHECKED ) ;

	return 0L ;
}


LRESULT CGlossaryWindow::on_view_search( ) 
{
	set_display_state( CONCORDANCE_DISPLAY_STATE ) ;
	show_view_content() ;

	CheckMenuItem( GetMenu(), ID_VIEW_MATCH,  MF_UNCHECKED ) ;
	CheckMenuItem( GetMenu(), ID_VIEW_SEARCH, MF_CHECKED   ) ;

	return 0L ;
}

LRESULT CGlossaryWindow::on_source_concordance()
{
	get_concordances(m_view_interface.get_selection_text()) ;
	return 0L ;
}


LRESULT CGlossaryWindow::on_trans_concordance()
{
	get_translation_concordances(m_view_interface.get_selection_text()) ;
	
	return 0L ;
}

bool CGlossaryWindow::get_concordances( const wstring query_string )
{
	// an empty string would retrieve everything - probably not what the user wants!
	if ( query_string.empty() )
	{
		::MessageBeep(MB_ICONEXCLAMATION) ;
		user_feedback(IDS_EMPTY_QUERY) ;
		return false ;
	}

	prep_concordance_search(query_string);
	
	perform_concordance_search();
	
	show_concordance_results();
	m_view_interface.set_scroll_pos(0) ;

	return true ; 
}

void CGlossaryWindow::show_concordance_results()
{
	set_display_state ( CONCORDANCE_DISPLAY_STATE ) ;
	show_view_content() ;

	// give the user feedback
	CString status_text = system_message
		( 
		IDS_FOUND_X_MATCHES_FOR_STRING, 
		int_arg( m_search_matches.size() ), 
		CString( m_search_matches.get_source_plain().c_str() )
		) ;
	user_feedback( status_text ) ;
}

void CGlossaryWindow::perform_concordance_search()
{
	search_match_container matches ;
	m_memories->perform_search( matches, m_search_matches.m_params ) ;

	m_search_matches.set_matches( matches ) ;
}

void CGlossaryWindow::config_concordance_search_settings()
{
	m_search_matches.m_params.m_ignore_case = true ;
	m_search_matches.m_params.m_ignore_width =		!! m_properties_gloss.m_data.m_ignore_width ;
	m_search_matches.m_params.m_ignore_hira_kata =	!! m_properties_gloss.m_data.m_ignore_hir_kat ;
}

void CGlossaryWindow::prep_concordance_search(const std::wstring& query_string)
{
	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	// this will hold our matches
	m_search_matches.clear() ;
	m_search_matches.set_query_rich( query_string ) ;

	config_concordance_search_settings();
}

bool CGlossaryWindow::get_translation_concordances(const wstring query_string)
{
	// an empty string would retrieve everything - probably not what the user wants!
	if ( query_string.empty() )
	{
		::MessageBeep(MB_ICONEXCLAMATION) ;
		user_feedback(IDS_EMPTY_QUERY) ;
		return false ;
	}

	// this will hold our matches
	m_search_matches.clear() ;
	m_search_matches.set_trans( query_string ) ;

	m_search_matches.m_params.m_ignore_case = true ;

	perform_concordance_search() ;
	
	// remember where we were
	// in the future, make an array of states to allow Explorer-style page navigation
	set_display_state (CONCORDANCE_DISPLAY_STATE) ;
	show_view_content() ;
	
	// give the user feedback
	const wstring plain_trans = m_search_matches.get_trans_plain() ;
	user_feedback( system_message(IDS_FOUND_X_MATCHES_FOR_STRING, 
								int_arg( m_search_matches.size() ), 
								CString( plain_trans.c_str()))) ;
	return true ; 
}

LRESULT CGlossaryWindow::OnDrop(HDROP dropped)
{
	if ( ! dropped_in_client( dropped ) ) 
	{
		SetMsgHandled( FALSE ) ;
		return 0L ;
	}

	const CDrop drop( dropped ) ;

	const UINT num_files = drop.NumDragFiles() ;

	if ( ! num_files ) 
	{
		SetMsgHandled( FALSE ) ;
		return 0L ;
	}

	for ( UINT current_file=0 ; current_file < num_files ; ++current_file )
	{
		load( drop.DragQueryFile( current_file ) ) ;
	}
	
	return 0L ;
}

// Show the Glossary Manager dialog.
// If the top glossary changed, we'll need to
// change the window title.
LRESULT CGlossaryWindow::on_tools_memory_manager()
{
	if ( show_mem_mgr_dlg(IDS_GLOSSARY_MANAGER_TITLE) )
	{
		set_window_title() ;
	}
	
	return 0L ;
}

// User wants to switch focus to memory window.
LRESULT CGlossaryWindow::on_view_switch()
{
	m_listener->gloss_view_switch(*this) ;
	return 0L ;
}

void CGlossaryWindow::set_up_initial_size()
{
	// get dimensions of desktop
	const CWindowRect destop_rect(GetDesktopWindow()) ;
	
	// get dimensions of this dialog
	CWindowRect dialog_rect(*this) ;

	// calculate dialog size
	const int width = dialog_rect.right - dialog_rect.left ;
	const int height = dialog_rect.bottom - dialog_rect.top ;
	dialog_rect.right = destop_rect.right ;
	dialog_rect.left = destop_rect.right - width ;
	dialog_rect.top = destop_rect.top ;
	dialog_rect.bottom = dialog_rect.top + height ;
	
	// justify dialog right, place it under parent
	MoveWindow( &dialog_rect, TRUE ) ;
	SetWindowPos( HWND_BOTTOM, &dialog_rect, SWP_NOACTIVATE | SWP_NOMOVE ) ;
}

void CGlossaryWindow::size_client_and_status_bar()
{
	static const int SB_WIDTH_THRESHOLD = 400 ;
	static const int SB_PANE2_WIDTH = 100 ;
	static const int SB_PANE2_PROPORTION = 4 ;
	// get the client rect
	CClientRect client_rect(*this) ;
	
#ifdef UNIT_TEST
	return ;
#endif
	// if the status bar is visible, subtract its height from the height of the browser view
	if ( m_appstate.m_is_statusbar_visible )
	{
		const CWindowRect statusbar_rect(m_statusbar.m_mp_sbar.m_hWnd) ;
		client_rect.bottom -= statusbar_rect.Height() ;
		
		const int width = statusbar_rect.Width() ;

		BOOL success = FALSE ;
		if ( width > SB_WIDTH_THRESHOLD )
		{
			success = m_statusbar.m_mp_sbar.SetPaneWidth( ID_PANE_2, SB_PANE2_WIDTH ) ;
		}
		else
		{
			success = m_statusbar.m_mp_sbar.SetPaneWidth( ID_PANE_2, ( width / SB_PANE2_PROPORTION )  ) ;
		}
		ATLASSERT( success ) ;
	}
	if ( m_appstate.m_is_toolbar_visible ) 
	{
		const CWindowRect toolbar_rect(m_toolbar.m_hWnd) ;
		client_rect.top += toolbar_rect.Height() ;
	}
	// resize edit window
	m_view_interface.Move( &client_rect ) ;
}

void CGlossaryWindow::reflect_tb_vis()
{
	if ( ! m_appstate.m_is_toolbar_visible )
	{
		CheckMenuItem( GetMenu(), ID_VIEW_TOOLBAR, MF_UNCHECKED ) ;

		m_toolbar.ShowWindow( SW_HIDE ) ;
		//	Window won't be visible yet
		ATLASSERT( ! m_statusbar.m_mp_sbar.IsWindowVisible() ) ;
	}
	else
	{
		CheckMenuItem( GetMenu(), ID_VIEW_TOOLBAR, MF_CHECKED ) ;
		m_toolbar.ShowWindow( SW_SHOWNOACTIVATE ) ;
	}
}

// Set up the toolbar buttons.
void CGlossaryWindow::init_toolbar()
{
	// 0 is for separators.
	std::vector< int > commands ;
	commands += 
		ID_FILE_NEW,	ID_FILE_OPEN, ID_MEMORY_CLOSE,	SEP_ID,
		ID_FILE_SAVE,	ID_FILE_SAVE_ALL, SEP_ID,
		ID_NEXT_PANE,   SEP_ID,
		ID_EDIT_CUT,	ID_EDIT_COPY,	ID_EDIT_PASTE,	SEP_ID,
		ID_EDIT_FIND ;
	std::vector< int > StdBitmaps ;
	StdBitmaps += 
		IDB_NEW_DOCUMENT,	IDB_OPEN, IDB_MEMORY_CLOSE,	
		IDB_SAVE,	IDB_SAVEMANY,
		IDB_SWITCH_VIEWS,
		IDB_CUT,			IDB_COPY,	IDB_PASTE,	
		IDB_SEARCH ;

	m_toolbar = CToolBarXP().Create24BitToolBarCtrl(*this, commands, FALSE,
		ATL_SIMPLE_TOOLBAR_STYLE);

#ifdef UNIT_TEST
	return ;
#endif
	m_toolbar.SetBitmapSize(BM_SIZE, BM_SIZE) ;
	// set the system icon list into the tree view
	CImageList images ;
	images.Create(BM_SIZE, BM_SIZE, ILC_COLOR24 | ILC_MASK, 0, StdBitmaps.size() + 1 ) ;

	foreach(int img_id, StdBitmaps)
	{
		CBitmap bmp ;
		ATLVERIFY(bmp.LoadBitmap( img_id )) ;
		images.Add( bmp, MAGENTA ) ;
	}
	m_toolbar.SetImageList( images ) ;

	reflect_tb_vis();
}

void CGlossaryWindow::reflect_sb_vis()
{
	if ( ! m_appstate.m_is_statusbar_visible )
	{
		CheckMenuItem( GetMenu(), ID_VIEW_STATUS_BAR, MF_UNCHECKED ) ;

		m_statusbar.m_mp_sbar.ShowWindow( SW_HIDE ) ;
		//	Window won't be visible yet
		ATLASSERT( ! m_statusbar.m_mp_sbar.IsWindowVisible() ) ;
	}
	else
	{
		CheckMenuItem( GetMenu(), ID_VIEW_STATUS_BAR, MF_CHECKED ) ;
		m_statusbar.m_mp_sbar.ShowWindow( SW_SHOWNOACTIVATE ) ;
	}

}

bool CGlossaryWindow::init_status_bar()
{
	const int PANE_WIDTH = 100 ;

	// create status bar
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP ;
	m_hWndStatusBar = ::CreateStatusWindow( dwStyle, R2T( ATL_IDS_IDLEMESSAGE ), m_hWnd, ATL_IDW_STATUS_BAR ) ;
	
	ATLASSERT( TWindow( m_hWndStatusBar ).IsWindow() ) ;
	
#ifdef UNIT_TEST
	return true ;
#endif

	ATLVERIFY(m_statusbar.m_mp_sbar.SubclassWindow( m_hWndStatusBar )) ;
	ATLASSERT( m_statusbar.m_mp_sbar.IsWindow() ) ;
	int arrParts[] = 
	{ 
		ID_DEFAULT_PANE, 
		ID_PANE_2
	};

	const int num_array_parts = sizeof(arrParts) / sizeof(arrParts[0]) ;
	ATLVERIFY(m_statusbar.m_mp_sbar.SetPanes(arrParts, num_array_parts, false));

	ATLVERIFY(m_statusbar.m_mp_sbar.SetPaneWidth( arrParts[1], PANE_WIDTH )) ;

	m_statusbar.m_mp_sbar.UpdatePanesLayout() ;
	reflect_sb_vis();
	return !! m_statusbar.m_mp_sbar.IsWindow() ;
}

void CGlossaryWindow::wait_until_view_not_busy()
{
	m_view_interface.set_accel(m_accelerator) ;
	m_view_interface.ensure_navigation_complete() ;
}

void CGlossaryWindow::set_up_recently_used_doclist()
{
	const int MAX_NUM_ENTRIES = 15 ;
	const int MAX_ITEM_LEN = 400 ; // pixels

	// init recent documents list
    m_mru.SetMenuHandle(::GetSubMenu(GetMenu(), 0));
    m_mru.ReadFromRegistry( R2T( IDS_REG_KEY_GLOSS ) );
	
    m_mru.SetMaxEntries(MAX_NUM_ENTRIES);	
	m_mru.SetMaxItemLength(MAX_ITEM_LEN) ;
}

void CGlossaryWindow::check_save_history()
{
	// We only save the history for the main glossary.
	if ( ! is_main() ) 
	{
		return ;
	}

	app_props::properties_general gen_props ;
	gen_props.read_from_registry() ;

	app_props::properties_loaded_history history_props ;
	history_props.read_from_registry() ;

	size_t mem_num = 0 ;
	size_t remote_num = 0 ;
	for ( memory_iterator pos = m_memories->begin() ; 
		pos != m_memories->end() && mem_num < app_props::NumMems ; 
		++pos )
	{
		memory_pointer mem = *pos ;
		const CString location = mem->get_fullpath() ;
		if ( ::PathFileExists(location) && mem->is_local() ) 
		{
			StringCbCopy( history_props.m_data.m_glosses[mem_num], MAX_PATH, location ) ;
			ATLASSERT ( remote_num + mem_num < m_memories->size() ) ;
			mem_num++ ;
		}
		else if (! mem->is_local())
		{
			StringCbCopy( history_props.m_data.m_remote_glosses[remote_num], MAX_PATH, location ) ;
			ATLASSERT ( remote_num + mem_num < m_memories->size() ) ;
			remote_num++ ;
		}
	}
	history_props.m_data.m_num_gloss = mem_num ;
	history_props.m_data.m_num_remote_gloss = remote_num ;
	history_props.write_to_registry() ;
}

void CGlossaryWindow::refresh_menu()
{
	HINSTANCE h = _Module.GetResourceInstance() ;
	ATLASSERT( h != NULL ) ;
	LPCTSTR str = MAKEINTRESOURCE( IDR_GLOSSARYMENU ) ;
	HMENU menu = GetMenu() ;
	ATLASSERT( menu != NULL ) ;
	if (menu)
	{
		::DestroyMenu( menu ) ;
	}
	menu = ::LoadMenu( h, str ) ;
	ATLASSERT( menu != NULL ) ;
	SetMenu( menu ) ;

	refresh_mru_doc_list(menu);
}

void CGlossaryWindow::refresh_mru_doc_list(HMENU menu)
{
	// write our recent docs
	m_mru.WriteToRegistry( R2T( IDS_REG_KEY_GLOSS ) );
	// reset the menu handle for the recent docs list...
	m_mru.SetMenuHandle(::GetSubMenu(menu, 0));
	// read our doc history back for the new menu
	m_mru.ReadFromRegistry( R2T( IDS_REG_KEY_GLOSS ) );
}

LPCTSTR CGlossaryWindow::get_save_filter()
{
	static LPCTSTR glossary_file_filter = 
		_T("Felix Glossary File (*.fgloss)\0*.fgloss\0")
		_T("XML File (*.xml)\0*.xml\0")
		_T("Multiterm 5.5 (*.txt)\0*.txt\0")
		_T("Multiterm 6.0 (*.txt)\0*.txt\0")
		_T("Excel File (*.xls)\0*.xls\0") ;

	return glossary_file_filter ;
}

LPCTSTR CGlossaryWindow::get_open_filter()
{
	static LPCTSTR glossary_file_filter = 
		_T("Felix Glossary Files (*.fgloss;*.xml)\0*.fgloss;*.xml\0")
		_T("Multiterm Files (*.txt)\0*.txt\0")
		_T("All files (*.*)\0*.*\0") ;

	return glossary_file_filter ;
}

// This is just to fool would-be crackers into going on a goose chase.
// Consider removing.
void CGlossaryWindow::seed_random_numbers()
{
	misc_wrappers::date current_date ;
	current_date.set_to_local_time() ;
	srand( current_date.wMilliseconds ) ;
}

bool CGlossaryWindow::check_for_clashes( memory_pointer mem )
{
	return IDCANCEL != m_listener->gloss_check_save_location( mem ) ;
}

void CGlossaryWindow::edit_record( record_pointer rec )
{
	memory_pointer mem = m_memories->get_first_memory() ;
	show_edit_dialog( rec, mem->get_id() ) ;
}

LRESULT CGlossaryWindow::on_file_connect()
{
	CConnectionDlg dlg ;
	if (IDCANCEL == dlg.DoModal(*this))
	{
		return 0L ;
	}
	memory_pointer mem = dlg.m_memory ;
	m_memories->insert_memory(mem) ;

	CString feedback ;
	feedback.FormatMessage(IDS_CONNECTED_MEMORY, (LPCTSTR)mem->get_location()) ;
	user_feedback(feedback) ;

	this->set_window_title() ;
	return 0L ;
}

LRESULT CGlossaryWindow::OnToolTipTextW(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
{

	static std::map<int, int> toolmap ;

	if( toolmap.empty() )
	{
		toolmap[ID_FILE_NEW] = IDS_NEW_TOOLBAR ;
		toolmap[ID_FILE_OPEN] = IDS_OPEN_TOOLBAR ;
		toolmap[ID_MEMORY_CLOSE] = IDS_GLOSSARY_CLOSE ;
		toolmap[ID_FILE_SAVE] = IDS_SAVE_GLOSSARY ;
		toolmap[ID_FILE_SAVE_ALL] = IDS_SAVE_ALL_GLOSSARIES ;
		toolmap[ID_EDIT_CUT] = ID_EDIT_CUT ;
		toolmap[ID_EDIT_COPY] = ID_EDIT_COPY ;
		toolmap[ID_EDIT_PASTE] = ID_EDIT_PASTE ;
		toolmap[ID_EDIT_FIND] = IDS_SEARCH_TOOLBAR_GLOSS ;
		toolmap[ID_TOOLS_PREFERENCES] = ID_TOOLS_PREFERENCES ;
		toolmap[ID_HELP] = ID_HELP  ;
		toolmap[ID_APP_ABOUT] = ID_APP_ABOUT ;
		toolmap[ID_NEXT_PANE] = ID_NEXT_PANE ;
	}

	LPNMTTDISPINFOW pDispInfo = (LPNMTTDISPINFOW)pnmh;
	pDispInfo->szText[0] = 0;

	if((idCtrl != 0) && !(pDispInfo->uFlags & TTF_IDISHWND))
	{
		const int cchBuff = 256;
		wchar_t szBuff[cchBuff];
		szBuff[0] = 0;
		const int id = toolmap[idCtrl] ;
		const int nRet = ::LoadStringW(ModuleHelper::GetResourceInstance(), id, szBuff, cchBuff);
		for(int i = 0; i < nRet; i++)
		{
			pDispInfo->szText[i] = szBuff[i] ;
			pDispInfo->szText[i+1] = 0 ;
			if(szBuff[i] == L'\n')
			{
				SecureHelper::strncpyW_x(pDispInfo->szText, _countof(pDispInfo->szText), &szBuff[i + 1], _TRUNCATE);
				break;
			}
		}
	}

	return 0;
}

void CGlossaryWindow::delete_record(record_pointer rec)
{
	m_memories->get_first_memory()->erase(rec) ;
}


void CGlossaryWindow::load_util_settings()
{
	try
	{
		CDispatchWrapper utils(L"Felix.Utilities") ;
		CComVariant val = utils.method(L"LoadProp", ZOOM_KEY) ;
		if (val.vt != VT_NULL)
		{
			m_mousewheel_count = min(max(val.intVal, -10), 10) ;
		}
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to retrieve mousewheel setting") ;
		logging::log_exception(e) ;
	}
}

void CGlossaryWindow::save_util_settings()
{
	try
	{
		// save zoom state
		CDispatchWrapper utils(L"Felix.Utilities") ;
		utils.method(L"SaveProp", ZOOM_KEY, CComVariant(m_mousewheel_count)) ;
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to save utility settings") ;
		logging::log_exception(e) ;
	}
}

LRESULT CGlossaryWindow::on_view_zoom()
{
	CZoomDlg dlg ;
	dlg.m_zoom_level = m_mousewheel_count ;
	dlg.m_interface = static_cast< CZoomInterface* >( this ) ;
	if ( dlg.DoModal() == IDOK )
	{
		m_mousewheel_count = dlg.m_zoom_level ;
	}
	else if (dlg.m_zoom_level != m_mousewheel_count)
	{
		this->set_zoom_level(m_mousewheel_count) ;
	}
	return 0L ;
}

void CGlossaryWindow::set_zoom_level( int zoom_level )
{
	m_view_interface.run_script("resetFontSizes") ;
	if (zoom_level)
	{
		CString command = _T("decreaseFont") ;
		if (zoom_level > 0)
		{
			command = _T("increaseFont") ;
		}
		for (int i = 0 ; i < abs(zoom_level); ++i)
		{
			m_view_interface.run_script(command) ;
		}
	}
}

void CGlossaryWindow::load_history()
{
	m_memories->clear() ;
	app_props::properties_loaded_history history_props ;
	history_props.read_from_registry() ;
	for ( int i = history_props.m_data.m_num_gloss ; 0 < i  ; --i )
	{
		load( history_props.m_data.m_glosses[i-1], false ) ;
	}
	for ( int i = history_props.m_data.m_num_remote_gloss ; 0 < i  ; --i )
	{
		try
		{
			memory_remote *mem = new memory_remote() ;
			memory_pointer pmem(mem) ;
			mem->connect(history_props.m_data.m_remote_glosses[i-1]) ;
			this->add_glossary(pmem) ;
		}
		catch (CException& e)
		{
			history_props.m_data.m_num_remote_gloss = i-1 ;
			logging::log_error("Failed to load remote glossary") ;
			logging::log_error(string2string(history_props.m_data.m_remote_glosses[i-1])) ;
			logging::log_exception(e) ;
			this->FlashWindow(FALSE) ;
		}
	}
	set_window_title() ;
}

void CGlossaryWindow::save_prefs()
{
	m_appstate.write_to_registry() ;
	//	m_properties_gloss.write_to_registry() ;

	m_mru.WriteToRegistry( R2T( IDS_REG_KEY_GLOSS ) );

	check_save_history() ;
	save_window_settings( _T("MainGlossary") ) ;
	save_util_settings() ;
}

void CGlossaryWindow::load_reg_settings()
{
	m_appstate.read_from_registry() ;
	m_properties_gloss.read_from_registry() ;
	m_properties_algo.read_from_registry() ;
}

void CGlossaryWindow::apply_reg_bg_color()
{
	set_bg_color((COLORREF)m_properties_gloss.m_data.m_back_color);
}

void CGlossaryWindow::apply_mousewheel_setting()
{
#ifdef UNIT_TEST
	return ;
#endif
	if (m_mousewheel_count)
	{
		CString command = _T("decreaseFont") ;
		if (m_mousewheel_count > 0)
		{
			command = _T("increaseFont") ;
		}
		for (int i = 0 ; i < abs(m_mousewheel_count); ++i)
		{
			m_view_interface.run_script(command) ;
		}
	}
}

LRESULT CGlossaryWindow::on_new_search()
{
	m_search_window.set_mem_window(false) ;
	if (! m_search_window.IsWindow())
	{
		m_search_window.Create(NULL) ;
	}

	m_search_window.set_mem_controller( this->get_memory_model() ) ;
	m_search_window.ShowWindow(SW_SHOW) ;
	m_search_window.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE) ;

	return 0L ;
}

LRESULT CGlossaryWindow::on_toggle_views()
{
	if (this->get_display_state() == LOOKUP_DISPLAY_STATE)
	{
		this->set_display_state(CONCORDANCE_DISPLAY_STATE) ;
	}
	else
	{
		this->set_display_state(LOOKUP_DISPLAY_STATE) ;
	}
	this->show_view_content() ;
	return 0L ;
}

CString CGlossaryWindow::get_memory_name( memory_pointer mem )
{
	if (! mem->is_local())
	{
		return mem->get_location() ;
	}
	else
	{
		file::name fname =  mem->get_location() ;
		return fname.file_name_part() ;
	}
}


// Override the browser context menu.
void CGlossaryWindow::set_doc_ui_handler()
{
#ifdef UNIT_TEST
	return ;
#endif
	CComObject<CFelixMemDocUIHandler> *pUIH = NULL;
	HRESULT hr = CComObject<CFelixMemDocUIHandler>::CreateInstance (&pUIH);
	if (SUCCEEDED(hr))
	{
		pUIH->m_get_menu = boost::bind(&CGlossaryWindow::get_doc_context_menu, this) ;
		// Make our custom DocHostUIHandler the window.external handler
		CComQIPtr<IDocHostUIHandlerDispatch> pIUIH = pUIH;
		hr = m_view_interface.m_view.SetExternalUIHandler(pIUIH) ;
	}
	ATLASSERT(SUCCEEDED(hr)) ;
}

// Show context menu in response to right click in browser.
HRESULT CGlossaryWindow::get_doc_context_menu()
{
	BANNER("CGlossaryWindow::get_doc_context_menu") ;
	CMenu menu ;

	CImageList images ;
	const int grow_size = 2 ;
	images.Create(BM_SIZE, BM_SIZE, ILC_COLOR24 | ILC_MASK, 0, grow_size ) ;

	menu.CreatePopupMenu() ;
	add_popup_item(menu, ID_EDIT_COPY, IDS_POPUP_COPY) ;
	add_popup_separator(menu) ;
	add_popup_item(menu, ID_NEXT_PANE, IDS_POPUP_SWITCH_VIEWS) ;
	add_popup_separator(menu) ;
	add_popup_item(menu, ID_EDIT_DELETE, IDS_POPUP_DELETE) ;

	POINT ptScreen ;
	::GetCursorPos(&ptScreen) ;
	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON,	ptScreen.x, ptScreen.y, m_hWnd, NULL) ;

	return S_OK ;
}

/*
 Close the top memory on the stack, if any.
 Change window title to reflect new top memory, if any.
 */
LRESULT CGlossaryWindow::on_memory_close()
{
	BANNER("CGlossaryWindow::on_memory_close") ;
	// base case -- there are no memories
	if (this->get_memory_model()->get_memories().empty())
	{
		return 0L ;
	}

	// See if the top memory needs saving
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;
	if (! mem->is_saved())
	{
		switch( user_wants_to_save( mem->get_location() ) ) 
		{
		case IDNO :
			// Set it as saved, and carry on
			mem->set_saved_flag( true ) ;
			break;

		case IDYES :
			// Save it, then carry on
			if ( IDCANCEL == LetUserSaveMemory(mem) )
			{
				return 0L ;
			}
			break ;

		case IDCANCEL :
			// User wants to abort the whole thing
			return 0L ;

		default :
			// WTF...
			ATLASSERT( "Unknown response!" && FALSE ) ;
			return 0L ;
		}
	}

	// Now remove it.
	this->get_memory_model()->remove_memory_by_id(mem->get_id()) ;
	this->set_window_title() ;

	user_feedback( system_message(IDS_CLOSED_GLOSSARY, mem->get_location()) ) ;
	return 0L ;
}

const CGlossaryWindow::MERGE_CHOICE CGlossaryWindow::get_merge_type( const bool check_empty )
{
	if (check_empty)
	{
		return check_empty_on_load() ;
	}
	return MERGE_CHOICE_SEPARATE ;
}

LRESULT CGlossaryWindow::on_edit_concordance()
{
	CConcordanceDialog dialog ;
	if ( IDCANCEL == dialog.DoModal() )
	{
		return 0L ;
	}

	const wstring query = string2wstring( dialog.get_text() ) ;
	if ( query.length() > 0 )
	{
		get_concordances( query ) ;
	}
	return 0L ;
}

LRESULT CGlossaryWindow::on_edit_edit()
{
	return on_user_editEntry(0) ;
}

LRESULT CGlossaryWindow::on_edit_delete()
{
	return on_delete() ;
}

void CGlossaryWindow::set_bg_color_if_needed()
{
	const CColorRef color((COLORREF)m_properties_gloss.m_data.m_back_color) ;
	if (! color.is_white())
	{
		m_view_interface.set_bg_color(color.as_wstring()) ;
	}
}

void CGlossaryWindow::set_display_state( DISPLAY_STATE new_state )
{
	switch(new_state)
	{
	case NEW_RECORD_DISPLAY_STATE:
		m_view_state = &m_view_state_new ;
		break ;
	case INIT_DISPLAY_STATE:
		m_view_state = &m_view_state_initial ;
		break ;
	case LOOKUP_DISPLAY_STATE:
		m_view_state = &m_view_state_lookup;
		break ;
	case MATCH_DISPLAY_STATE:
		m_view_state = &m_view_state_match;
		break ;
	case CONCORDANCE_DISPLAY_STATE:
		m_view_state = &m_view_state_concordance;
		break ;
	}
	m_display_state = new_state ;
}