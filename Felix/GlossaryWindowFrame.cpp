/*!
	@file GlossaryDialog.cpp
	@brief implementation file for CGlossaryDialog
	@date 2005/06/25
	Time: 10:04:34
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "GlossaryWindowFrame.h"
#include "RegSettings.h"
#include "system_message.h"

#include "Drop.h"	// CDrop

#include "HtmlDocColorSetter.h"


#include "NumberFmt.h"
#include "xpmenu/Tools.h"		// CWindowRect

#include "stringconversions.h"
#include "text_templates.h"
#include "ConnectionDlg.h"
#include "QueryMergeDlg.h"
#include "output_device.h"
#include "ImportMultitermFile.h"
#include "FelixMemDocUIHandler.h"
#include "record_local.h"
#include "ConcordanceDialog.h"
#include "TabbedTextImporter.h"
#include "input_device_file.h"
#include "output_device.h"
#include "memory_remote.h"
#include "FelixModel.h"

#include "CredVault.h"
#include "file_dialog.h"

using namespace mem_engine ;
using namespace except ;
using namespace html ;

// CTOR
GlossaryWindowFrame::GlossaryWindowFrame(app_props::props_ptr props) : 
	m_props(props),
	m_is_main(false),
	m_listener(nullptr),
	m_editor(new CEditTransRecordDialog),
	m_is_trans_concordance(false),
	m_manager_window(props, IDS_GLOSSARY_MANAGER_TITLE, _T("MemoryMangerWindowGloss"), this),
	m_old_manager_window(props),
	m_search_window(this),
	m_input_device(new InputDeviceFile),
	m_output_device(new OutputDeviceFile),
	m_model(new FelixModel(props,
							false))
{ 
	// assign functions
	m_get_window = &get_window_real ;
	m_pre_translate_msg = boost::bind(&GlossaryWindowFrame::PreTranslateMessage, this, _1) ;
	m_create = boost::bind(&GlossaryWindowFrame::create, this, _1) ;
	m_check_save = boost::bind(&GlossaryWindowFrame::check_save, this) ;
	m_apply_settings = boost::bind(&GlossaryWindowFrame::apply_window_settings, this, _1) ;

	// other props
	m_is_active = false ;
	m_properties_gloss = &m_props->m_gloss_props ;
	initialize_values() ;
	m_editor->m_interface = this ;
	m_editor->m_is_glossary = true ;

	// initialize states
	this->init_state(&m_view_state_initial) ;
	this->init_state(&m_view_state_new) ;
	this->init_state(&m_view_state_concordance) ;
	m_view_state_concordance.set_search_matches(&m_concordance_matches) ;
	m_view_state_concordance.set_app_props(this->m_properties_gloss) ;

	this->init_state(&m_view_state_match) ;
	m_view_state_match.set_search_matches(&m_search_matches) ;
	m_view_state_match.set_app_props(this->m_properties_gloss) ;

	// display state
	set_display_state( INIT_DISPLAY_STATE ) ;
	ATLASSERT(m_view_state == &m_view_state_initial) ;

	seed_random_numbers();

}

// DTOR
GlossaryWindowFrame::~GlossaryWindowFrame() 
{ 
}


//============================
// Message handlers
//============================

/** Load a memory from the MRU list.
*/
LRESULT GlossaryWindowFrame::on_mru_file_open( WORD wID )
{
	open_mru_file(wID, get_input_device());

	return 0;
}

//! Show the find dialog.
LRESULT GlossaryWindowFrame::handle_find()
{
	init_find_window( SW_RESTORE, IDS_GLOSS_SEARCH ) ;

	ATLASSERT( m_find.IsWindow() ) ;
	ATLASSERT( m_find.IsWindowVisible() ) ;

	return 0 ;
}

LRESULT GlossaryWindowFrame::on_create( )
{
	SENSE("OnInitDialog") ;

	logging::log_debug("Initializing glossary dialog") ;

	load_mousewheel_setting() ;
	// create and instantiate html view

	m_hWndClient = init_view() ;

	// set content
	ATLASSERT(m_view_state == &m_view_state_initial) ;
	m_view_state->show_content() ;

	init_status_bar() ;
	init_toolbar() ;
	CheckMenuItem( GetMenu(), IDC_GLOSS_SIMPLE_VIEW, ( m_properties_gloss->get_simple_view() ? MF_CHECKED : MF_UNCHECKED) ) ;

	wait_until_view_not_busy() ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
	
	set_window_title() ;

	apply_reg_bg_color();

	// set focus back to parent
	::SetFocus( GetParent() ) ;
	ATLASSERT( GetFocus() == GetParent() || ::IsChild( GetParent(), GetFocus() ) ) ;
	
	::DragAcceptFiles( m_hWnd, TRUE ) ;

	HMENU hmenu = LoadMenu ( _Module.GetResourceInstance(),  // _AtlBaseModule in VC7
		MAKEINTRESOURCE(IDR_GLOSSARYMENU) );
	SetMenu(hmenu) ;
	size_client_and_status_bar() ;
	set_up_recently_used_doclist() ;

	m_accelerator.LoadAccelerators(IDR_MAINFRAME) ;
	m_background_processor.set_hwnd(*this) ;
	m_background_processor.set_accelerator(m_accelerator) ;

	check_load_history() ;

	apply_mousewheel_setting();

	wait_until_view_not_busy() ;
	set_doc_ui_handler() ;
	return 0;
}

void GlossaryWindowFrame::check_load_history()
{
	if ( ! is_main() ) 
	{
		return ;
	}

	if ( ! m_props->m_gen_props.load_prev_gloss_on_startup() ) 
	{
		return ;
	}

	load_history();
}

LRESULT GlossaryWindowFrame::on_close_command( )
{
	SENSE("OnClose") ;

	if ( check_save() == IDCANCEL) 
	{
		// ignore the command
		SetMsgHandled( TRUE ) ;
		return 1 ;
	}

#ifndef UNIT_TEST
	ATLASSERT( IsWindow() ) ;
	DestroyWindow() ;
#endif

	return 0 ;
}

/** Response to WM_SIZE message.
 * First resize toolbar and status bar, then resize client to
 * fit in rest.
 */
LRESULT GlossaryWindowFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetMsgHandled( FALSE ) ;

	// pass the message to the status bar and toolbar.
	ATLASSERT( m_statusbar.m_mp_sbar.IsWindow() ) ;
	m_statusbar.m_mp_sbar.SendMessage( uMsg, wParam, lParam ) ;
	m_toolbar.SendMessage( uMsg, wParam, lParam ) ;

	size_client_and_status_bar() ;

	return 0;
}


void GlossaryWindowFrame::import_tabbed_text( const CString &file_name )
{
	CTabbedTextImporter importer(this, m_props) ;
	importer.load_file(file_name, get_input_device()) ;
	importer.m_memory->set_is_memory(false) ;
	m_model->insert_memory(importer.m_memory) ;
	set_window_title() ;
}

//! Import a multiterm file
void GlossaryWindowFrame::import_multiterm( const CString &file_name )
{
	CImportMultitermFile importer(this, m_props) ;
	importer.import(file_name, get_input_device()) ;
	importer.m_memory->set_is_memory(false) ;
	m_model->insert_memory(importer.m_memory) ;
	set_window_title() ;
}

//! Export a multiterm 6 file.
void GlossaryWindowFrame::export_multiterm_6( memory_pointer mem, 
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
void GlossaryWindowFrame::export_multiterm_6_sub( const wstring source_lang, 
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

void GlossaryWindowFrame::export_multiterm_55( memory_pointer mem, 
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
void GlossaryWindowFrame::export_multiterm_55_sub( const string source_lang, 
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
LRESULT GlossaryWindowFrame::on_file_new( )
{
	BANNER( "CGlossaryDialog::on_file_new" ) ;

	if ( check_save() == IDCANCEL)
	{
		return 0 ;
	}
	
	m_model->add_memory() ;

	m_view_interface.set_text(wstring()) ;

	check_mousewheel() ;
	set_window_title() ;
	
	user_feedback( IDS_NEW ) ;

	return 0 ;
}

LRESULT GlossaryWindowFrame::OnFormatBackgroundColor()
{
	BANNER( "CGlossaryDialog::OnFormatBackgroundColor" ) ;
	
	CHtmlDocument doc = m_view_interface.get_document() ;
	
	CHtmlDocColorSetter setter ;

	m_properties_gloss->m_data.m_back_color = setter.SetDocBGColor( doc ) ;
	m_properties_gloss->write_to_registry() ;
	
	return 0 ;
}


LRESULT GlossaryWindowFrame::on_file_open( )
{
	BANNER( "CGlossaryDialog::on_file_open" ) ;

	handle_open() ;
	return 0 ;
}


LRESULT GlossaryWindowFrame::on_file_save( )
{
	SENSE("on_file_save") ;
	if ( m_model->empty() )
	{
		return 0L ;
	}
#ifndef UNIT_TEST
	memory_pointer mem = m_model->get_first_memory() ; 

	if ( mem->is_new() ) 
	{
		return on_file_save_as() ;
	}

	do_save( mem ) ;
#endif

	return 0L ;
}

void GlossaryWindowFrame::do_save( memory_pointer mem )
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
					export_excel(mem->get_location(), mem) ;
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
LRESULT GlossaryWindowFrame::on_file_save_as( )
{
	if ( m_model->empty() )
	{
		MessageBeep( MB_ICONSTOP ) ;
		return 0L ;
	}
	// clearing location won't work, because we want to offer the current location
	// as the default file name
	memory_pointer mem = m_model->get_first_memory() ;

	save_memory_as(mem);

	return 0L ;

}

void GlossaryWindowFrame::HandleEditModeFind()
{
	if ( m_edit_replace.IsWindow() && m_edit_replace.IsWindowVisible() )
	{
		m_edit_replace.ShowWindow( SW_HIDE ) ;
	}
	m_edit_find.set_document( m_view_interface.get_document() ) ;
	init_edit_find_window( SW_RESTORE ) ;
}

LRESULT GlossaryWindowFrame::on_find( )
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

LRESULT GlossaryWindowFrame::on_edit_replace( )
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
		on_new_search() ;
		m_search_window.show_replace_page() ;
	}
	return 0 ;
}


// ===============================


LRESULT GlossaryWindowFrame::on_add( )
{
	m_editor->m_is_add = false ;
	show_edit_dialog_for_new_entry( IDS_ADD_GLOSS_ENTRY ) ;
	return 0;
}


LRESULT GlossaryWindowFrame::OnDestroy(  )
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


size_t GlossaryWindowFrame::num_matches()
{
	return m_search_matches.size() ;
}


/** Check for existing memories loaded.
*
* Check whether the user wants to merge the glossaries.
* The only difference between this and the memory frame version is the
* glossary string ID values.
*/
GlossaryWindowFrame::MERGE_CHOICE GlossaryWindowFrame::check_empty_on_load()
{
	if ( m_model->empty() ) 
	{
		return MERGE_CHOICE_SEPARATE ;
	}

	user_feedback( IDS_GLOSSARY_OPEN ) ;

	memory_pointer first_mem = m_model->get_first_memory() ;

	CQueryMergeDlg dlg(IDS_MERGE_GLOSS_TITLE, 
		IDS_MERGE_GLOSS_TEXT, 
		file::CPath(first_mem->get_location()).FindFileName());

	return get_merge_choice(dlg, &m_props->m_gen_props);

}

bool GlossaryWindowFrame::load(const CString file_name, const bool check_empty /*= true*/ )
{
	if ( ! ::PathFileExists( file_name ) ) 
	{
		logging::log_warn(
			string("Glossary file does not exist: ") + 
			string2string(static_cast<LPCWSTR>(file_name)));
		return false ;
	}

	const MERGE_CHOICE should_merge = get_merge_type(check_empty);

	if (should_merge == MERGE_CHOICE_CANCEL)
	{
		return true ;
	}

	user_feedback( system_message( IDS_MSG_LOADING, file::name( file_name ).file_name() ) ) ;

	// merge or add?
	memory_pointer mem ;
	if (should_merge == MERGE_CHOICE_SEPARATE)
	{
		mem = m_model->add_memory() ;
	}
	else
	{
		ATLASSERT(should_merge == MERGE_CHOICE_MERGE) ;
		mem = m_model->get_first_memory() ;
	}

	mem->set_is_memory(false);
	mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

	bool success = false ;
	try
	{
		success = mem->load( file_name )  ;
	}
	catch ( ... ) 
	{
		logging::log_error("Failed to load glossary");
		if (should_merge == MERGE_CHOICE_SEPARATE)
		{
			mem->set_listener(nullptr);
			m_model->remove_memory_by_id( mem->get_id() ) ;
		}
		throw ;
	}

	mem->set_listener(nullptr);

	if ( success )
	{ 
		set_window_title() ;
		mem->set_is_memory(false) ;
		m_mru.AddToList(  file_name ) ;
		m_props->m_history_props.m_glossary_location = 
			static_cast<LPCWSTR>(file_name) ;
	}
	else
	{
		// set the file name and body element in our memory for loading
		file::CPath path( file_name ) ;
		// we failed to load the memory
		user_feedback( get_load_failure_msg(path.FindFileName()) ) ; //
		m_model->remove_memory_by_id( mem->get_id() ) ;
	}
	
	return success ;

}


wstring GlossaryWindowFrame::build_glossary_list(search_query_glossary &search_matches)
{
	wstring html_content ;

	html_content << L"<b>" << R2WSTR( IDS_SEARCH_RESULTS ) << L":</b>" ;

	if ( search_matches.empty() )
	{
		html_content << L"<p>" << system_message_w( IDS_FOUND_X_MATCHES, 
			int_arg_w( search_matches.size() ) ) ;

		return html_content ;
	}

	search_matches.m_start_numbering = m_properties_gloss->m_data.m_numbering ;
	html_content << search_matches.get_html_short() ;

	return html_content  ;
}


void GlossaryWindowFrame::lookup(const std::wstring& query_text)
{
	prep_for_gloss_lookup(query_text);
	perform_gloss_lookup();
	m_view_state_match.show_content() ;
}

void GlossaryWindowFrame::prep_for_gloss_lookup(const std::wstring& query_text)
{
	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	m_search_matches.set_start_numbering(m_properties_gloss->get_numbering()) ;
	set_display_state( MATCH_DISPLAY_STATE ) ;
	config_matches_for_gloss_lookup(query_text);
}

// Here is where we do the actual glossary lookup.
void GlossaryWindowFrame::perform_gloss_lookup()
{
	search_match_container matches ;
	// TM matches
	m_model->get_glossary_matches( matches, m_search_matches.m_params ) ;
	// rule matches
	m_listener->get_regex_rules()->get_matches(matches, m_search_matches.m_params.m_source) ;
	// This container will hold the results that we display later.
	m_search_matches.set_matches( matches ) ;
}

void GlossaryWindowFrame::config_matches_for_gloss_lookup(const std::wstring& query_text)
{
	// our various parameters
	m_search_matches.m_params.m_ignore_case =		m_properties_gloss->get_ignore_case() ;
	m_search_matches.m_params.m_ignore_width =		m_properties_gloss->get_ignore_width() ;
	m_search_matches.m_params.m_ignore_hira_kata =	m_properties_gloss->get_m_ignore_hir_kat() ;

	// source
	m_search_matches.set_query(query_text) ;
}


// 

wstring GlossaryWindowFrame::get_glossary_entry(const int index)
{
	const size_t local_index = static_cast<size_t>(get_local_index(index));

	record_pointer record = m_view_state->get_specified_record(local_index) ;
	return get_record_translation(record);
}

std::wstring GlossaryWindowFrame::get_record_translation(record_pointer entry)
{
	return m_view_state->retrieve_record_trans(entry,
						record_string_prefs(m_properties_gloss->is_plaintext(),
										     m_properties_gloss->is_to_lower())) ;
}



bool GlossaryWindowFrame::add_record(record_pointer record, const CString gloss_name )
{
	// Add record
	memory_pointer mem ;
	if ( gloss_name.IsEmpty() ) 
	{
		mem = m_model->get_first_memory() ;
	}
	else
	{
		ATLASSERT ( ::PathFileExists( gloss_name ) ) ; 
		mem = m_model->get_memory_by_name( gloss_name ) ;
	}
	
	return add_record(mem, record);
}

bool GlossaryWindowFrame::add_record( record_pointer record, const size_t i )
{
	auto model = this->get_model() ;
	auto mem = model->get_memory_at(i) ;
	return add_record(mem, record);
}

bool GlossaryWindowFrame::add_record( memory_pointer mem, record_pointer record )
{
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

	if ( get_display_state() == MATCH_DISPLAY_STATE )
	{
		lookup( query ) ;
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
void GlossaryWindowFrame::give_added_record_feedback(memory_pointer& mem)
{
	CString content = resource_string(IDS_ADDED_TRANSLATION ) ;
	content	+= _T(" ") ;
	content	+=  system_message( IDS_CURRENT_SIZE, get_window_type_string(), int_arg( mem->size() ) ) ;
	user_feedback( content ) ;
}

// Got to make this a template!
bool GlossaryWindowFrame::show_new_record()
{
	BANNER( "CGlossaryDialog::show_new_record()" ) ;

	m_view_state_new.show_content() ;

	return true ;
}

// File -> Open
bool GlossaryWindowFrame::handle_open()
{
	user_feedback( IDS_OPEN_GLOSS_FILE ) ;

	// get the file name
	file_open_dialog dialog;

	dialog.set_title(R2T(IDS_OPEN_GLOSS_FILE));
	dialog.set_file_filter(get_gloss_open_filter());
	dialog.allow_multi_select();
	if (!m_props->m_history_props.m_glossary_location.empty())
	{
		dialog.set_last_save(m_props->m_history_props.m_glossary_location);
	}
	user_feedback(IDS_OPEN);

	if ( ! dialog.show( ) ) 
	{
		user_feedback( IDS_CANCELLED_ACTION ) ;
		return false ;
	}

	auto filenames = dialog.get_open_destinations();

	switch(dialog.get_selected_index()) 
	{
	case 1: case 4:
		load_felix_files(filenames);
		break;

	case 2:
		import_multiterm(filenames);
		break ;

	case 3:
		{
			for(auto filename: filenames)
			{
				import_tabbed_text(filename) ;
			}
		}
		break ;

	default:
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
	}

	set_window_title() ;
	return true ;
}

void GlossaryWindowFrame::load_felix_files(std::vector<CString> import_files)
{
	for(CString filename: import_files)
	{
		load(filename) ;
	}
}

void GlossaryWindowFrame::import_multiterm(std::vector<CString> import_files)
{
	for(CString filename: import_files)
	{
		import_multiterm(filename) ;
	}
	set_window_title() ;
}

// Makes a fresh record so that we can modify it without
// changing the actual glossary entry.
record_pointer GlossaryWindowFrame::get_record( size_t pos ) 
{ 
	BANNER( "CGlossaryDialog::get_record()" ) ;
	ATLASSERT( pos < m_search_matches.size() ) ;
	record_pointer record(m_search_matches.at( pos )->get_record()->clone()) ;  
	return record ;
}


LRESULT GlossaryWindowFrame::handle_user_search()
{
	prep_user_search();

	m_is_trans_concordance = false ;
	perform_concordance_search() ;

	// give the user feedback
	this->set_display_state(CONCORDANCE_DISPLAY_STATE) ;
	this->show_view_content() ;

	return 0L ;
}

void GlossaryWindowFrame::prep_user_search()
{
	// remember where we are, makes a difference how we respond to user input
	set_display_state( CONCORDANCE_DISPLAY_STATE ) ;

	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	m_concordance_matches.clear() ;
	m_concordance_matches.m_params = m_find.get_search_params() ;
}

void GlossaryWindowFrame::give_user_search_feedback()
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
LRESULT GlossaryWindowFrame::on_view_toolbar( )
{
	ATLASSERT( m_toolbar.IsWindow() ) ;
	ATLASSERT( m_toolbar.IsWindowVisible() == m_appstate.m_is_toolbar_visible ) ;

	m_appstate.m_is_toolbar_visible = ! m_appstate.m_is_toolbar_visible ;

	reflect_tb_vis() ;

	// get the client rect
	size_client_and_status_bar() ;

	return 0L ;
}
// toggle simple view
LRESULT GlossaryWindowFrame::on_view_simple( )
{
	m_properties_gloss->m_data.m_simple_view = ! m_properties_gloss->m_data.m_simple_view ;
	CheckMenuItem( GetMenu(), IDC_GLOSS_SIMPLE_VIEW, ( m_properties_gloss->get_simple_view() ? MF_CHECKED : MF_UNCHECKED) ) ;
	m_properties_gloss->write_to_registry() ;
	this->show_view_content() ;

	return 0L ;
}

// toggle status bar visibility
LRESULT GlossaryWindowFrame::on_view_status_bar( )
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
LRESULT GlossaryWindowFrame::on_view_edit_mode(WindowsMessage &)
{
	SENSE( "on_view_edit_mode" ) ;
#ifdef UNIT_TEST
	return 0L ;
#else

	const bool edit_mode_enabled = m_view_interface.is_edit_mode() ;

	SetEditModeMenuItems(edit_mode_enabled);

	// do the switcheroo on the find windows...
	// swap out the various find dialogs...
	SwapFindDialogs(edit_mode_enabled);
	ToggleEditMode() ;

	return 0L ;
#endif
}

void GlossaryWindowFrame::ToggleEditMode()
{
	m_view_state->handle_toggle_edit_mode() ;
}

void GlossaryWindowFrame::SetEditModeMenuItems(const bool edit_mode_enabled)
{
	CheckMenuItem( GetMenu(), ID_VIEW_EDIT_MODE, ( edit_mode_enabled ? MF_UNCHECKED : MF_CHECKED ) ) ;
	CheckMenuItem( GetMenu(), IDC_GLOSS_SIMPLE_VIEW, ( m_properties_gloss->get_simple_view() ? MF_CHECKED : MF_UNCHECKED) ) ;
}




// Make this the main glossary/not the main glossary
bool GlossaryWindowFrame::set_main ( bool setting ) 
{ 
	m_is_main = setting ;
	return set_window_title() ;
}

// Set the window title.
// Title consists of main declaration (if any) + Glossary - [Main Gloss]
bool GlossaryWindowFrame::set_window_title()
{
	CString file_name ;
	if ( ! m_model->empty() )
	{
		memory_pointer mem = m_model->get_first_memory() ;
		file_name = get_memory_name(mem);
	} 

	CString title ;
	if ( m_is_main )
	{
		if(title.LoadString( IDS_MAIN )) 
		{
			title += _T(" ") ;
		}
		else
		{
			title = _T("Main ") ;
		}
	}
	title += R2TS( IDS_GLOSSARY ) ;
	title += _T(" [") ;
	title += ( file_name.IsEmpty() ? R2TS( IDS_NEW ) : file_name ) ;
	title += _T("]") ;

#ifdef UNIT_TEST 
	return true ;
#else
	return FALSE != SetWindowText(title) ;
#endif


}							

// Before shutting down, see if there are any glossaries
// that need saving.
bool GlossaryWindowFrame::pre_shutdown_save_check()
{
	if ( m_model->empty() )
	{
		return true ;
	} 

	if ( m_check_save() == IDCANCEL)
	{
		// user does not want to close window
		return false ;
	}

	return true ;
}

// Exit without prompting to save memories.
bool GlossaryWindowFrame::exit_silently()
{
	SENSE("exit_silently") ;
	memory_list memories ;
	m_model->get_memories_needing_saving( memories ) ;
	
	FOREACH( memory_pointer mem, memories) 
	{
		mem->set_saved_flag( true ) ;
	}

	return true ;
}

BOOL GlossaryWindowFrame::PreTranslateMessage( LPMSG pMsg )
{
	ENSURE_ACTIVE

	if ( IsWindow() && m_accelerator.TranslateAccelerator( *this, pMsg ) )
	{
		return TRUE ;
	}

	// prevent the view from eating our menu shortcut keys...
	if (is_menu_key_pressed())
	{
		return FALSE ;
	}
	return m_view_interface.PreTranslateMessage( pMsg ) ;
}

bool GlossaryWindowFrame::clear_memory()
{
	memory_pointer mem = m_model->get_first_memory() ;
	mem->clear_memory() ;
	m_search_matches.clear() ;
	m_concordance_matches.clear() ;

	m_new_record = record_pointer(new record_local()) ;
	set_window_title() ;
	
	m_view_interface.set_text(R2W(IDS_GLOSSARY_CLEARED)) ;

	check_mousewheel() ;
	user_feedback( system_message( IDS_CLEARED_MEMORY, get_window_type_string() ) ) ;
	m_view_interface.set_scroll_pos(0) ;
	return true ;
}


// ====================
// message handlers
// ====================

// User wants to edit an entry.
LRESULT GlossaryWindowFrame::on_user_editEntry( LPARAM lParam )
{	
	SENSE("on_user_edit") ;

	const size_t num = static_cast<size_t>(lParam) ;
	m_view_state->set_current(num) ;
	m_view_state->on_user_edit() ;

	return 0L ;
}

// User wants to delete an entry.
LRESULT GlossaryWindowFrame::on_user_delete( size_t number )
{
	m_view_state->delete_match(number) ;
	return 0L ;
}

LRESULT GlossaryWindowFrame::on_user_search( LPARAM /* lParam */ )
{
	return handle_user_search() ;
}

LRESULT GlossaryWindowFrame::on_user_edit_replace( LPARAM /* lParam */  )
{
	m_edit_find.set_search_params( m_edit_replace.get_search_params() ) ;
	return 0L ;
}

LRESULT GlossaryWindowFrame::OnUserAdd( LPARAM lParam )
{
	m_view_state->set_current(static_cast<size_t>(lParam)) ;
	m_listener->gloss_add_record( m_view_state->get_current_match()->get_record()->clone() ) ;

	return 0L ;
}

LRESULT GlossaryWindowFrame::OnUserPrev( LPARAM /* lParam */ )
{
	set_display_state( MATCH_DISPLAY_STATE ) ;
	show_view_content() ;
	return 0L ;
}



CString GlossaryWindowFrame::get_window_type_string()
{
	return resource_string(IDS_GLOSSARY) ;
}
bool GlossaryWindowFrame::is_glossary_window()
{
	return true;
}

void GlossaryWindowFrame::route_nav_command(LPMSG pMsg)
{
	switch( pMsg->wParam )
	{
	case IDC_EDIT:
		on_user_editEntry( pMsg->lParam ) ;
		break ;
	case IDC_DELETE:
		on_user_delete( static_cast<size_t>(pMsg->lParam) ) ;
		break ;
	case IDC_ADD:
		OnUserAdd( pMsg->lParam ) ;
		break ;
	case IDC_PREV:
		OnUserPrev( pMsg->lParam ) ;
		break ;
	default:
		logging::log_warn((format("Glossary window: unknown navigation message code (%i)") % pMsg->wParam).str()) ;
		ATLASSERT("Unknown glossary nav message" && FALSE) ;
	}

}

// Returns `true` to cancel, `false` otherwise.
bool GlossaryWindowFrame::OnBeforeNavigate2( _bstr_t url )
{
	try
	{
		// "#" is used for JavaScript links.
		wstring _url = BSTR2wstring(url) ;
		if(boost::ends_with(_url, L"#"))
		{
			return false ; // don't cancel
		}

		// If the link is a dropped file, open it.
		const CString possible_xml_file(static_cast< LPCWSTR >( url )) ;
		const file::CFileExtension ext(possible_xml_file) ;
		if (ext.is_xml() || ext.equals(_T(".fgloss")))
		{
			logging::log_debug("Glossary file dropped into window.") ;
			this->load(possible_xml_file) ;
			return true ; // should cancel
		}
		else if (ext.is_txt())
		{
			logging::log_debug("Text file dropped into glossary window.") ;
			this->import_multiterm(possible_xml_file) ;
			return true ; // should cancel
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
		logging::log_error("Program exception (Glossary Window)") ;
		logging::log_exception(e) ;
		e.notify_user(_T("Failed to process action (program error)\r\rIf the problem persists, please contact support.")) ;
		return true ;
	}
	catch (_com_error& e)
	{
		logging::log_error("COM error (Glossary Window)") ;
		logging::log_exception(e) ;
		CComException cx(e) ;
		cx.notify_user(_T("Failed to process action\rAn error occurred connecting to a COM server.")) ;
		return true ;
	}
	catch (std::exception& e)
	{
		logging::log_error("Standard library exception (Glossary Window)") ;
		logging::log_exception(e) ;
		const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND ;
		::MessageBox( m_hWnd, CA2T(e.what()), _T("C Runtime Error\r\rIf the problem persists, please contact support."), msg_flags ) ;  
	}

	return true ;
}

void GlossaryWindowFrame::set_listener( CGlossaryWinListener *listener)
{
	m_listener = listener ;
}


void GlossaryWindowFrame::put_show_marking( VARIANT_BOOL setting )
{
	if ( setting == VARIANT_FALSE ) 
	{
		m_search_matches.m_params.m_show_marking = false ;
		m_concordance_matches.m_params.m_show_marking = false ;
	}
	else
	{
		m_search_matches.m_params.m_show_marking = true ;
		m_concordance_matches.m_params.m_show_marking = true ;
	}
}

VARIANT_BOOL GlossaryWindowFrame::get_show_marking()
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

void GlossaryWindowFrame::show_post_edit_content()
{
	show_view_content() ;
	user_feedback( IDS_EDIT_COMPLETE ) ;
}


void GlossaryWindowFrame::set_ui_language()
{
	SENSE("set_ui_language") ;
#ifdef UNIT_TEST
	return ;
#else
	wait_until_view_not_busy() ;

	refresh_menu() ;

	refresh_windows() ;

	refresh_view_content() ;

	// set new window title
	set_window_title() ;

	wait_until_view_not_busy() ;
	// query user for color
	set_bg_color((COLORREF)m_properties_gloss->m_data.m_back_color);

	user_feedback( IDS_CHANGED_LANGUAGES ) ;
#endif
}


void GlossaryWindowFrame::show_view_content()
{
	if (! is_window())
	{
		return ;
	}
	SENSE("show_view_content") ;
#ifndef UNIT_TEST
	m_view_state->show_content() ;
#endif
}

LRESULT GlossaryWindowFrame::on_view_match( ) 
{
	set_display_state( MATCH_DISPLAY_STATE ) ;
	show_view_content() ;

	CheckMenuItem( GetMenu(), ID_VIEW_MATCH,  MF_CHECKED   ) ;
	CheckMenuItem( GetMenu(), ID_VIEW_SEARCH, MF_UNCHECKED ) ;

	return 0L ;
}


LRESULT GlossaryWindowFrame::on_view_search( ) 
{
	set_display_state( CONCORDANCE_DISPLAY_STATE ) ;
	show_view_content() ;

	CheckMenuItem( GetMenu(), ID_VIEW_MATCH,  MF_UNCHECKED ) ;
	CheckMenuItem( GetMenu(), ID_VIEW_SEARCH, MF_CHECKED   ) ;

	return 0L ;
}

LRESULT GlossaryWindowFrame::on_source_concordance()
{
	get_concordances(m_view_interface.get_selection_text()) ;
	return 0L ;
}


LRESULT GlossaryWindowFrame::on_trans_concordance()
{
	get_translation_concordances(m_view_interface.get_selection_text()) ;
	
	return 0L ;
}

bool GlossaryWindowFrame::get_concordances( const wstring query_string )
{
	m_is_trans_concordance = false ;
	// an empty string would retrieve everything - probably not what the user wants!
	if ( query_string.empty() )
	{
		user_feedback(IDS_EMPTY_QUERY) ;
		return false ;
	}

	prep_concordance_search(query_string);
	perform_concordance_search();
	
	show_concordance_results();
	m_view_interface.set_scroll_pos(0) ;

	return true ; 
}

void GlossaryWindowFrame::show_concordance_results()
{
	set_display_state ( CONCORDANCE_DISPLAY_STATE ) ;
	show_view_content() ;
}

void GlossaryWindowFrame::perform_concordance_search()
{
	search_match_container matches ;
	if (! m_concordance_matches.get_source_plain().empty())
	{
		m_model->perform_search( matches, m_concordance_matches.m_params ) ;
	}
	m_concordance_matches.set_matches( matches ) ;
}

void GlossaryWindowFrame::config_concordance_search_settings()
{
	m_concordance_matches.m_params.m_ignore_case = true ;
	m_concordance_matches.m_params.m_ignore_width =		!! m_properties_gloss->m_data.m_ignore_width ;
	m_concordance_matches.m_params.m_ignore_hira_kata =	!! m_properties_gloss->m_data.m_ignore_hir_kat ;
}

void GlossaryWindowFrame::prep_concordance_search(const std::wstring& query_string)
{
	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	// this will hold our matches
	m_concordance_matches.clear() ;
	m_concordance_matches.set_query( query_string ) ;

	config_concordance_search_settings();
}

bool GlossaryWindowFrame::get_translation_concordances(const wstring query_string)
{
	m_is_trans_concordance = true ;
	// an empty string would retrieve everything - probably not what the user wants!
	if ( query_string.empty() )
	{
		user_feedback(IDS_EMPTY_QUERY) ;
		return false ;
	}

	// this will hold our matches
	m_concordance_matches.clear() ;
	m_concordance_matches.set_trans( query_string ) ;

	m_concordance_matches.m_params.m_ignore_case = true ;

	perform_concordance_search() ;
	
	// remember where we were
	// in the future, make an array of states to allow Explorer-style page navigation
	set_display_state (CONCORDANCE_DISPLAY_STATE) ;
	show_view_content() ;

	// give the user feedback
	const wstring plain_trans = m_concordance_matches.get_trans_plain() ;
	user_feedback( system_message(IDS_FOUND_X_MATCHES_FOR_STRING, 
								int_arg( m_concordance_matches.size() ), 
								CString( plain_trans.c_str()))) ;
	return true ; 
}

LRESULT GlossaryWindowFrame::OnDrop(HDROP dropped)
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
LRESULT GlossaryWindowFrame::on_tools_memory_manager()
{
	SENSE("on_tools_memory_manager") ;

	if (m_listener->get_props_general()->m_data.m_old_mem_mgr)
	{
		m_old_manager_window.set_memories(this->get_model()->get_memories()) ;
		m_old_manager_window.DoModal() ;
		return 0L ;
	}

	m_manager_window.set_mem_model(m_listener->get_model()) ;
	m_manager_window.set_gloss_model(get_model()) ;

	if (! m_manager_window.IsWindow())
	{
		m_manager_window.Create(*this) ;
	}

	m_manager_window.ShowWindow(SW_SHOW) ;
	m_manager_window.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE) ;
	
	return 0L ;
}

// User wants to switch focus to memory window.
LRESULT GlossaryWindowFrame::on_view_switch()
{
	m_listener->gloss_view_switch(*this) ;
	return 0L ;
}

void GlossaryWindowFrame::set_up_initial_size()
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

void GlossaryWindowFrame::size_client_and_status_bar()
{
	static const int SB_WIDTH_THRESHOLD = 400 ;
	static const int SB_PANE2_WIDTH = 100 ;
	static const int SB_PANE2_PROPORTION = 4 ;
	// get the client rect
	CClientRect client_rect(*this) ;
	
#ifdef UNIT_TEST
	return ;
#else
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
#endif
}

void GlossaryWindowFrame::reflect_tb_vis()
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
void GlossaryWindowFrame::init_toolbar()
{
	// SEP_ID is for separators.
	std::vector< int > commands ;
	add_common_tb_commands(commands) ;
	std::vector< int > StdBitmaps ;
	add_common_std_bitmaps(StdBitmaps) ;

	m_toolbar = CToolBarXP().Create24BitToolBarCtrl(*this, commands, FALSE,
		ATL_SIMPLE_TOOLBAR_STYLE);

#ifdef UNIT_TEST
	return ;
#else
	m_toolbar.SetBitmapSize(BM_SIZE, BM_SIZE) ;
	// set the system icon list into the tree view
	CImageList images ;
	create_tb_imagelist(images, StdBitmaps) ;
	m_toolbar.SetImageList( images ) ;

	reflect_tb_vis();
#endif
}

void GlossaryWindowFrame::reflect_sb_vis()
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

bool GlossaryWindowFrame::init_status_bar()
{
	const int PANE_WIDTH = 100 ;

	// create status bar
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP ;
	m_hWndStatusBar = ::CreateStatusWindow( dwStyle, R2T( ATL_IDS_IDLEMESSAGE ), m_hWnd, ATL_IDW_STATUS_BAR ) ;
	
	ATLASSERT( CWindow( m_hWndStatusBar ).IsWindow() ) ;
	
#ifdef UNIT_TEST
	PANE_WIDTH ;
	return true ;
#else

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
#endif
}

void GlossaryWindowFrame::wait_until_view_not_busy()
{
	m_view_interface.set_accel(m_accelerator) ;
	m_view_interface.ensure_navigation_complete() ;
}

void GlossaryWindowFrame::set_up_recently_used_doclist()
{
	const int MAX_NUM_ENTRIES = 15 ;
	const int MAX_ITEM_LEN = 400 ; // pixels

	// init recent documents list
    m_mru.SetMenuHandle(::GetSubMenu(GetMenu(), 0));
    m_mru.ReadFromRegistry( R2T( IDS_REG_KEY_GLOSS ) );
	
    m_mru.SetMaxEntries(MAX_NUM_ENTRIES);	
	m_mru.SetMaxItemLength(MAX_ITEM_LEN) ;
}

void GlossaryWindowFrame::check_save_history()
{
	// We only save the history for the main glossary.
	if ( ! is_main() ) 
	{
		return ;
	}

	app_props::properties_loaded_history *history_props = &m_props->m_history_props ;

	history_props->m_loaded_remote_gloss.clear() ;
	history_props->m_loaded_gloss.clear() ;
	m_model->record_loaded_memories(history_props->m_loaded_gloss, history_props->m_loaded_remote_gloss) ;

	history_props->write_to_registry() ;
}

void GlossaryWindowFrame::refresh_menu()
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

void GlossaryWindowFrame::refresh_mru_doc_list(HMENU menu)
{
	// write our recent docs
	m_mru.WriteToRegistry( R2T( IDS_REG_KEY_GLOSS ) );
	// reset the menu handle for the recent docs list...
	m_mru.SetMenuHandle(::GetSubMenu(menu, 0));
	// read our doc history back for the new menu
	m_mru.ReadFromRegistry( R2T( IDS_REG_KEY_GLOSS ) );
}

// This is just to fool would-be crackers into going on a goose chase.
// Consider removing.
void GlossaryWindowFrame::seed_random_numbers()
{
	misc_wrappers::date current_date ;
	current_date.set_to_local_time() ;
	srand( current_date.wMilliseconds ) ;
}

bool GlossaryWindowFrame::check_for_clashes( memory_pointer mem )
{
	return IDCANCEL != m_listener->gloss_check_save_location( mem ) ;
}

void GlossaryWindowFrame::edit_record( record_pointer rec )
{
	memory_pointer mem = m_model->get_first_memory() ;
	show_edit_dialog( rec, mem->get_id() ) ;
}

LRESULT GlossaryWindowFrame::on_file_connect()
{
	CConnectionDlg dlg(m_props) ;
	if (IDCANCEL == dlg.DoModal(*this))
	{
		return 0L ;
	}

	return add_remote_memory(m_model, dlg.m_memory) ;
}

/** handle tooltip text ourselves to enable dynamic switching.
* (The default WTL way caches tooltip text)
* This way, if we switch UI languages we still get the correct tooltip
* text.
*/
LRESULT GlossaryWindowFrame::OnToolTipTextW(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	if( m_toolmap.empty() )
	{
		init_tooltip_map(m_toolmap);
		add_glossary_specific_tooltips();
	}

	handle_tooltip(pnmh, idCtrl, m_toolmap);

	return 0;
}

void GlossaryWindowFrame::add_glossary_specific_tooltips()
{
	m_toolmap[ID_EDIT_FIND] = IDS_SEARCH_TOOLBAR_GLOSS;
	m_toolmap[ID_MEMORY_CLOSE] = IDS_GLOSSARY_CLOSE;
	m_toolmap[ID_FILE_SAVE] = IDS_SAVE_GLOSSARY;
	m_toolmap[ID_FILE_SAVE_ALL] = IDS_SAVE_ALL_GLOSSARIES;
}

void GlossaryWindowFrame::delete_record(record_pointer rec)
{
	m_model->get_first_memory()->erase(rec) ;
}


void GlossaryWindowFrame::load_mousewheel_setting()
{
	m_mousewheel_count = m_props->m_view_props.get_gloss_mousewheel() ;
}

LRESULT GlossaryWindowFrame::on_view_zoom()
{
	CZoomDlg dlg(static_cast< CZoomInterface* >( this ), m_mousewheel_count) ;

	dlg.DoModal() ;

	return 0L ;
}

void GlossaryWindowFrame::load_history()
{
	ATLTRACE("Loading glossary history\n") ;
	app_props::properties_loaded_history *history_props = &m_props->m_history_props ;
	auto gen_props = &m_props->m_gen_props;

	m_model->clear() ;

	std::vector<wstring> items ;
	std::copy(history_props->m_loaded_gloss.begin(), history_props->m_loaded_gloss.end(), std::back_inserter(items)) ;
	std::reverse(items.begin(), items.end()) ;

	FOREACH(wstring filename, items)
	{
		TRACE(filename) ;
		load(filename.c_str(), false) ;
	}

	items.clear() ;
	std::copy(history_props->m_loaded_remote_gloss.begin(), history_props->m_loaded_remote_gloss.end(), std::back_inserter(items)) ;
	std::reverse(items.begin(), items.end()) ;

	FOREACH(wstring filename, items)
	{
		try
		{
			memory_remote *mem = new memory_remote(m_props) ;
			memory_pointer pmem(mem) ;
			wstring username, password;

			if (gen_props->credential_is_saved(filename))
			{
				username = gen_props->username_for_connection(filename);
				CredentialReader reader(filename);
				password = reader.get_password();
			}
			mem->connect(filename.c_str(), username.c_str(), password.c_str());
			this->add_glossary(pmem) ;
		}
		catch (CException& e)
		{
			logging::log_error("Failed to load remote glossary") ;
			logging::log_error(string2string(filename)) ;
			logging::log_exception(e) ;
			this->FlashWindow(FALSE) ;
		}
	}
	set_window_title() ;
}

void GlossaryWindowFrame::save_prefs()
{
	SENSE("save_prefs") ;
#ifdef UNIT_TEST
	return ;
#else
	m_appstate.write_to_registry() ;

	m_mru.WriteToRegistry( R2T( IDS_REG_KEY_GLOSS ) );

	check_save_history() ;
	save_window_settings( _T("MainGlossary") ) ;
#endif
}

void GlossaryWindowFrame::apply_reg_bg_color()
{
	set_bg_color((COLORREF)m_properties_gloss->m_data.m_back_color);
}

void GlossaryWindowFrame::apply_mousewheel_setting()
{
#ifdef UNIT_TEST
	return ;
#else
	if (m_mousewheel_count)
	{
		set_zoom_level(m_mousewheel_count) ;
	}
#endif
}

LRESULT GlossaryWindowFrame::on_new_search()
{
	m_search_window.set_mem_window(false) ;
	if (! m_search_window.IsWindow())
	{
		m_search_window.Create(NULL) ;
	}

	m_search_window.set_mem_controller( this->get_model() ) ;
	m_search_window.ShowWindow(SW_SHOW) ;
	m_search_window.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE) ;

	m_search_window.show_search_page() ;

	return 0L ;
}

LRESULT GlossaryWindowFrame::on_toggle_views()
{
	if (this->get_display_state() == MATCH_DISPLAY_STATE)
	{
		this->set_display_state(CONCORDANCE_DISPLAY_STATE) ;
	}
	else
	{
		this->set_display_state(MATCH_DISPLAY_STATE) ;
	}
	this->show_view_content() ;
	return 0L ;
}

CString GlossaryWindowFrame::get_memory_name( memory_pointer mem )
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
void GlossaryWindowFrame::set_doc_ui_handler()
{
#ifdef UNIT_TEST
	return ;
#else
	CComObject<CFelixMemDocUIHandler> *pUIH = NULL;
	HRESULT hr = CComObject<CFelixMemDocUIHandler>::CreateInstance (&pUIH);
	if (SUCCEEDED(hr))
	{
		pUIH->m_get_menu = boost::bind(&GlossaryWindowFrame::show_doc_context_menu, this) ;
		// Make our custom DocHostUIHandler the window.external handler
		CComQIPtr<IDocHostUIHandlerDispatch> pIUIH = pUIH;
		hr = m_view_interface.m_view.SetExternalUIHandler(pIUIH) ;
	}
	ATLASSERT(SUCCEEDED(hr)) ;
#endif
}

// Show context menu in response to right click in browser.
HRESULT GlossaryWindowFrame::show_doc_context_menu()
{
	BANNER("CGlossaryDialog::show_doc_context_menu") ;
	CMenu menu ;

	menu.CreatePopupMenu() ;
	MenuWrapper wrapper(menu, m_hWnd) ;
	wrapper.add_item(ID_EDIT_COPY, R2S(IDS_POPUP_COPY)) ;
	wrapper.add_separator() ;
	wrapper.add_item(ID_NEXT_PANE, R2S(IDS_POPUP_SWITCH_VIEWS)) ;
	wrapper.add_separator() ;
	wrapper.add_item(ID_EDIT_DELETE, R2S(IDS_POPUP_DELETE)) ;

	wrapper.show() ;

	return S_OK ;
}

/*
 Close the top memory on the stack, if any.
 Change window title to reflect new top memory, if any.
 */
LRESULT GlossaryWindowFrame::on_memory_close()
{
	BANNER("CGlossaryDialog::on_memory_close") ;
	// base case -- there are no memories
	if (this->get_model()->empty())
	{
		return 0L ;
	}

	// See if the top memory needs saving
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;
	if (! mem->is_saved())
	{
		if (check_save_memory(mem) == IDCANCEL)
		{
			return 0L ;
		}
	}

	// Now remove it.
	remove_memory(mem, IDS_CLOSED_GLOSSARY);
	return 0L ;
}

const GlossaryWindowFrame::MERGE_CHOICE GlossaryWindowFrame::get_merge_type( const bool check_empty )
{
	if (check_empty)
	{
		return check_empty_on_load() ;
	}
	return MERGE_CHOICE_SEPARATE ;
}

LRESULT GlossaryWindowFrame::on_edit_concordance()
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

LRESULT GlossaryWindowFrame::on_edit_edit()
{
	return on_user_editEntry(0) ;
}

LRESULT GlossaryWindowFrame::on_edit_delete()
{
	return on_user_delete( 0 ) ;
}

void GlossaryWindowFrame::set_bg_color_if_needed()
{
	const CColorRef color((COLORREF)m_properties_gloss->m_data.m_back_color) ;
	if (! color.is_white())
	{
		m_view_interface.set_bg_color(color.as_wstring()) ;
	}
}

void GlossaryWindowFrame::set_display_state( DISPLAY_STATE new_state )
{
	switch(new_state)
	{
	case NEW_RECORD_DISPLAY_STATE:
		m_view_state = &m_view_state_new ;
		break ;
	case INIT_DISPLAY_STATE:
		m_view_state = &m_view_state_initial ;
		break ;
	case MATCH_DISPLAY_STATE:
		m_view_state = &m_view_state_match;
		break ;
	case CONCORDANCE_DISPLAY_STATE:
		m_view_state = &m_view_state_concordance;
		break ;
	}
	m_display_state = new_state ;
	m_view_state->activate() ;
}

mem_engine::search_match_ptr GlossaryWindowFrame::get_current_match()
{
	return m_view_state->get_current_match() ;
}

void GlossaryWindowFrame::redo_lookup( mem_engine::search_match_ptr match, bool do_gloss /*= false */ )
{
	logging::log_warn("`CGlossaryDialog::redo_lookup` is not implemented") ;
	match ;
	do_gloss ;
}

edit_record_dlg_ptr GlossaryWindowFrame::get_editor()
{
	return m_editor ;
}

bool GlossaryWindowFrame::is_short_format()
{
	return m_is_short_format ;
}

bool GlossaryWindowFrame::is_single_page()
{
	return true ;
}

void GlossaryWindowFrame::set_menu_checkmark( int item_id, bool is_checked )
{
	CheckMenuItem( GetMenu(), item_id, ( is_checked ? MF_CHECKED : MF_UNCHECKED ) ) ;
	return ;
}

void GlossaryWindowFrame::save_memory_as( memory_pointer mem )
{
	CString original_file_name;
	if (mem->is_new() == false)
	{
		original_file_name = mem->get_fullpath();
	}

	CString dialog_title;
	dialog_title.FormatMessage(IDS_SAVE, resource_string(IDS_GLOSSARY));

	file_save_dialog dialog;
	dialog.set_title(dialog_title);
	dialog.set_file_filter(get_gloss_save_filter());
	dialog.set_filename(original_file_name);
	dialog.set_last_save(m_props->m_history_props.m_glossary_location);
	dialog.set_default_ext(L"fgloss");

	if (!dialog.show())
	{
		user_feedback(IDS_CANCELLED_ACTION);
		return;
	}

	CString save_as_file_name = dialog.get_save_destination() ;

	switch (dialog.get_selected_index())
	{
	case 1: case 7:
		logging::log_debug("Saving glossary as fgloss file") ;
		fileops::add_extension_as_needed( save_as_file_name,  _T( ".fgloss" ) ) ;
		break;

	case 2:
		logging::log_debug("Saving glossary as xml file") ;
		fileops::add_extension_as_needed( save_as_file_name,  _T( ".xml" ) ) ;
		break;

	case 3:
		logging::log_debug("Exporting glossary as Multiterm 5.5 file") ;
		fileops::add_extension_as_needed( save_as_file_name,  _T( ".txt" ) ) ;
		export_multiterm_55( mem, save_as_file_name ) ;
		return ;

	case 4:
		logging::log_debug("Exporting glossary as Multiterm 6.0 file") ;
		fileops::add_extension_as_needed( save_as_file_name,  _T( ".txt" ) ) ;
		export_multiterm_6( mem, save_as_file_name ) ;
		return ;

	case 5:
		{
			export_excel(save_as_file_name, mem);
			return ;
		}

	case 6:
		{
			export_tabbed_text(save_as_file_name, mem);
			return ;
		}

	default:
		logging::log_warn("Unknown file type selected for Glossary - Save As") ;
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
		logging::log_debug("Falling back to save glossary as fgloss file") ;
		fileops::add_extension_as_needed( save_as_file_name,  _T( ".fgloss" ) ) ;
		return ;
	}

	CString old_location = mem->get_location() ;
	mem->set_location( save_as_file_name ) ;

	// If we're saving to a different file name, change the memory's creator
	// to the current user
	if ( 0 != old_location.CompareNoCase( save_as_file_name ) )
	{
		mem->set_creator_to_current_user( ) ;
	}

	save_memory( mem ) ;

	m_props->m_history_props.m_glossary_location = 
		static_cast<LPCWSTR>(save_as_file_name);

	// the title of the window will change to reflect the new memory name
	set_window_title() ;
}

void GlossaryWindowFrame::open_mru_file( WORD wID, input_device_ptr input )
{
	CString file_name ;
	m_mru.GetFromList(wID, file_name ) ;
	ATLASSERT( ! file_name.IsEmpty() ) ;

	try
	{
		input->ensure_file_exists(file_name);
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
}

INT_PTR GlossaryWindowFrame::check_save_memory( mem_engine::memory_pointer mem )
{
	switch( user_wants_to_save( mem->get_location() ) ) 
	{
	case IDNO :

		mem->set_saved_flag( true ) ;
		return IDNO;

	case IDYES :

		if ( IDCANCEL == LetUserSaveMemory(mem) )
		{
			return IDCANCEL ;
		}
		return IDYES ;

	case IDCANCEL :

		return IDCANCEL ;

	default :

		ATLASSERT( "Unknown response!" && FALSE ) ;
		return IDCANCEL ;

	}
}

mem_engine::felix_query * GlossaryWindowFrame::get_current_matches()
{
	return &m_search_matches ;
}

input_device_ptr GlossaryWindowFrame::get_input_device()
{
	return m_input_device ;
}

output_device_ptr GlossaryWindowFrame::get_output_device()
{
	return m_output_device ;
}

void GlossaryWindowFrame::get_qc_messages( mem_engine::record_pointer record, std::vector<wstring> &messages )
{
	record ;
	messages ;
}

app_props::props_ptr GlossaryWindowFrame::get_properties()
{
	return m_props ;
}

void GlossaryWindowFrame::apply_window_settings( const int show_cmd )
{
#ifdef UNIT_TEST
	show_cmd ;
	return ;
#else
	CWindowSettings ws;

	if( ws.Load( resource_string(IDS_REG_KEY), _T("MainGlossary") ) )
	{
		ws.ApplyTo( *this, show_cmd ) ;
	}
	else
	{
		this->set_up_initial_size() ;
	}
#endif
}

HWND GlossaryWindowFrame::create( HWND parent )
{
	return this->Create(parent) ;
}

int GlossaryWindowFrame::get_local_index( const int index )
{
	int local_index = index - m_properties_gloss->get_numbering() ;

	if ( local_index == -1 )
	{
		local_index = 9 ; // 10th item by 0 index
	}
	return local_index ;
}

model_iface_ptr GlossaryWindowFrame::get_memory_model()
{
	return m_model ;
}

void GlossaryWindowFrame::add_glossary( mem_engine::memory_pointer mem )
{
	m_model->insert_memory( mem ) ;
	check_mousewheel() ;
	set_window_title() ;
}

BOOL GlossaryWindowFrame::handle_sw_exception( except::CSWException &e, const CString &failure_message )
{
	return CCommonWindowFunctionality::handle_sw_exception(e, failure_message) ;
}

void GlossaryWindowFrame::edit_edit_record( mem_engine::record_pointer new_record, LPARAM display_state )
{
	ATLASSERT( m_editor->get_memory_id() > 0 ) ;
	set_display_state( static_cast< DISPLAY_STATE >( display_state ) ) ;
	ATLASSERT( get_display_state() == display_state ) ;

	SENSE("edit_edit_record") ;

	ATLASSERT( m_editor->get_memory_id() > 0 ) ;

	m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
		new_record,
		false) ;

#ifdef UNIT_TEST
	return ;
#else
	show_view_content() ;
#endif
}

void GlossaryWindowFrame::add_edit_record( mem_engine::record_pointer new_record, LPARAM display_state )
{
	ATLASSERT( m_editor->get_memory_id() > 0 ) ;
	set_display_state( static_cast< DISPLAY_STATE >( display_state ) ) ;
	ATLASSERT( get_display_state() == display_state ) ;

	SENSE("add_edit_record") ;

	ATLASSERT( m_editor->get_memory_id() > 0 ) ;

	m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
		new_record,
		true) ;

#ifdef UNIT_TEST
	return ;
#else
	show_view_content() ;
#endif
}

LPCTSTR GlossaryWindowFrame::get_save_ext()
{
	static LPCTSTR memory_file_ext = _T("fgloss") ;

	return memory_file_ext ;
}

bool GlossaryWindowFrame::has_name_clash( CString location )
{
	return m_model->has_name_clash(location) ;
}
