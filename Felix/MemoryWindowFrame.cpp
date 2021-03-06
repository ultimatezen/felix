/*!
 * \date 2007/01/31
 * \author Ryan Ginstrom
 *
 * Detailed description here.
 *
 * Copyright (C) Ryan Ginstrom.
 */

#include "StdAfx.h"
#include "MemoryWindowFrame.h"

#include "winuser.h"

#include "ConcordanceDialog.h"

#include "aboutdlg.h"				// CAboutDlg
#include "PropertiesDlg.h"			// CPropertiesDlg
#include "data_importer.h"
#include "data_converter.h"
#include "ImportDialog.h"
#include "ui.h"						// windows_ui -- wrapper for common dialog boxes

#include "InputKeyDlg.h"			// CInputKeyDlg

#include "Path.h"
#include "RegSettings.h"	// CWindowSettings & CReBarSettings

#include "TMXReader.h"		// CTMXReader
#include "record_local.h"

#include "Drop.h"				// CDrop

#include "HtmlDocColorSetter.h"	// CHtmlDocColorSetter
#include "xpmenu/Tools.h"		// CWindowRect

#include "HtmlHelp.h"
#pragma comment(lib, "HtmlHelp.lib")

#include "NumberFmt.h"
#include "process.h"

#include "text_templates.h"
#include "ConnectionDlg.h"

#include "QueryMergeDlg.h"
#include "text_templates.h"
#include "FelixMemDocUIHandler.h"
#include "memory_local.h"
#include "memory_remote.h"
#include "FelixModel.h"

#include "CredVault.h"

// file I/O
#include "input_device_file.h"
#include "output_device.h"
#include "system_paths.h"
#include "file_dialog.h"

// QC
#include "qcrules/qc_checker.h"
#include "qcrules/allcaps_check.h"
#include "qcrules/number_check.h"
#include "qcrules/gloss_check.h"

// placement
#include "number_placement.h"

using namespace mem_engine ;
using namespace except ;
using namespace html ;
using namespace placement ;


/** Constructor. Takes model interface.
*/
MemoryWindowFrame::MemoryWindowFrame( model_iface_ptr model, app_props::props_ptr props ) : 
	m_model(model),
	m_props(props),
	m_editor(new CEditTransRecordDialog),
	m_manager_window(m_props, IDS_SEARCH_MANAGER_TITLE, _T("MemoryMangerWindow"), this),
	m_old_manager_window(props),
	m_search_window(this),
	m_input_device(new InputDeviceFile),
	m_output_device(new OutputDeviceFile),
	m_silent_memories(props)
{
	// assign functions
	m_get_window = &get_window_real ;
	m_pre_translate_msg = boost::bind(&MemoryWindowFrame::PreTranslateMessage, this, _1) ;

	// other props
	m_is_active = false ;
	initialize_values() ;
	m_editor->m_interface = this ;

	// Allow drag and drop in Windows 7
	ChangeWindowMessageFilter( WM_DROPFILES, 1);
	ChangeWindowMessageFilter( WM_COPYDATA , 1);
	ChangeWindowMessageFilter( 0x0049, 1);

	// initialize min view
	m_min_view.set_listener(this) ;
	m_min_view.set_matches(&m_trans_matches) ;

	// initialize states
	this->init_state(&m_view_state_initial) ;
	this->init_state(&m_view_state_new) ;
	m_view_state_new.set_qc_props(&(m_props->m_qc_props)) ;

	this->init_state(&m_view_state_concordance) ;
	m_view_state_concordance.set_search_matches(&m_search_matches) ;

	// match state
	this->init_state(&m_view_state_match) ;
	m_view_state_match.set_search_matches(&m_trans_matches) ;
	m_view_state_match.set_props(m_props) ;

	this->init_state(&m_view_state_review) ;
	m_view_state_review.set_search_matches(&m_trans_matches) ;

	// display state
	set_display_state( INIT_DISPLAY_STATE ) ;
	ATLASSERT(m_view_state == &m_view_state_initial) ;

#ifndef UNIT_TEST
	m_props->read_from_registry() ;
#endif
	const BOOL show_markup = m_props->m_gen_props.m_data.m_show_markup ;
	this->m_trans_matches.m_params.m_show_marking = !! show_markup ;
	// make sure that the username is reflected!
	const wstring user_name = CT2W(m_props->m_gen_props.m_data.m_user_name) ;
	set_record_username(user_name) ;

	addToMessageLoop();

	// register command handlers
	this->register_event_listener( WM_CREATE, boost::bind(&MemoryWindowFrame::on_create, this, _1 ) ) ; 
	this->register_event_listener( WM_DESTROY, boost::bind(&MemoryWindowFrame::on_destroy, this, _1 ) ) ; 
	this->register_event_listener( WM_CLOSE, boost::bind(&MemoryWindowFrame::on_close, this, _1 ) ) ; 
	this->register_event_listener( WM_DROPFILES, boost::bind(&MemoryWindowFrame::on_drop, this, _1 ) ) ; 

	this->register_event_listener( WM_ACTIVATE, boost::bind(&MemoryWindowFrame::on_activate, this, _1 ) ) ; 


	// user messages
	this->register_user_event_listener( USER_LOOKUP_SOURCE, boost::bind(&MemoryWindowFrame::on_user_lookup_source, this, _1 )) ;
	this->register_user_event_listener( USER_LOOKUP_TRANS, boost::bind(&MemoryWindowFrame::on_user_lookup_trans, this, _1 )) ;
	this->register_user_event_listener( USER_SAVE_MEMORIES, boost::bind(&MemoryWindowFrame::on_user_save, this, _1 )) ;

	this->register_user_event_listener( ID_EDIT_FIND, boost::bind(&MemoryWindowFrame::on_user_edit_search, this, _1 )) ;
	this->register_user_event_listener( ID_USER_SEARCH, boost::bind(&MemoryWindowFrame::on_user_search, this, _1 )) ;
	this->register_user_event_listener( IDC_REPLACE_EDIT_RECORD, boost::bind(&MemoryWindowFrame::on_user_replace_edit_record, this, _1 )) ;
	this->register_user_event_listener( ID_EDIT_REPLACE, boost::bind(&MemoryWindowFrame::on_user_edit_replace, this, _1 )) ;
	this->register_user_event_listener( IDC_MIN_VIEW_END, boost::bind(&MemoryWindowFrame::on_user_view_min_end, this, _1 )) ;

	// commands
	this->register_command_event_listener( IDC_DEMO_CHECK_EXCESS, boost::bind(&MemoryWindowFrame::on_demo_check_excess_memories, this, _1 )) ;
	this->register_command_event_listener( IDC_STARTUP_CHECKS, boost::bind(&MemoryWindowFrame::on_startup_checks, this, _1 )) ;
	this->register_command_event_listener( IDC_SOURCE_CONCORDANCE_SEL, boost::bind(&MemoryWindowFrame::on_source_concordance, this, _1 )) ;
	this->register_command_event_listener( IDC_TRANS_CONCORDANCE_SEL, boost::bind(&MemoryWindowFrame::on_trans_concordance, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_MEMORY_MGR, boost::bind(&MemoryWindowFrame::on_tools_memory_manager, this, _1 )) ;
	this->register_command_event_listener( IDC_UNDERLINE, boost::bind(&MemoryWindowFrame::on_underline, this, _1 )) ;
	this->register_command_event_listener( IDC_BOLD, boost::bind(&MemoryWindowFrame::on_bold, this, _1 )) ;
	this->register_command_event_listener( IDC_ITALIC, boost::bind(&MemoryWindowFrame::on_italic, this, _1 )) ;
	this->register_command_event_listener( ID_FORMAT_BGCOLOR, boost::bind(&MemoryWindowFrame::OnFormatBackgroundColor, this, _1 )) ;
	this->register_command_event_listener( ID_APP_EXIT, boost::bind(&MemoryWindowFrame::on_file_exit, this, _1 )) ;
	this->register_command_event_listener( ID_GLOSSARY_NEW, boost::bind(&MemoryWindowFrame::on_new_glossary, this, _1 )) ;

	this->register_command_event_listener( ID_FILE_NEW, boost::bind(&MemoryWindowFrame::on_file_new, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_OPEN, boost::bind(&MemoryWindowFrame::on_file_open, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_SAVE, boost::bind(&MemoryWindowFrame::on_file_save, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_SAVE_AS, boost::bind(&MemoryWindowFrame::on_file_save_as, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_SAVE_ALL, boost::bind(&MemoryWindowFrame::on_file_save_all, this, _1 )) ;

	this->register_command_event_listener( ID_MEMORY_CLOSE, boost::bind(&MemoryWindowFrame::on_memory_close, this, _1 )) ;


	this->register_command_event_listener( IDD_EDIT_ENTRY, boost::bind(&MemoryWindowFrame::on_edit_entry, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_DELETE, boost::bind(&MemoryWindowFrame::on_delete_entry, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_ADD, boost::bind(&MemoryWindowFrame::on_add, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_REGISTER, boost::bind(&MemoryWindowFrame::on_register_gloss, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_FIND, boost::bind(&MemoryWindowFrame::on_new_search, this, _1 )) ;

	this->register_command_event_listener( ID_FIND_QUICKSEARCH, boost::bind(&MemoryWindowFrame::on_find, this, _1 )) ;

	this->register_command_event_listener( IDD_CONCORDANCE, boost::bind(&MemoryWindowFrame::on_concordance, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_REPLACE, boost::bind(&MemoryWindowFrame::on_edit_replace, this, _1 )) ;

	this->register_command_event_listener( ID_VIEW_TOOLBAR, boost::bind(&MemoryWindowFrame::on_view_toolbar, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_STATUS_BAR, boost::bind(&MemoryWindowFrame::on_view_status_bar, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_EDIT_MODE, boost::bind(&MemoryWindowFrame::on_view_edit_mode, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_MATCH, boost::bind(&MemoryWindowFrame::on_view_match, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_SEARCH, boost::bind(&MemoryWindowFrame::on_view_search, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_SWITCH, boost::bind(&MemoryWindowFrame::on_view_switch, this, _1 )) ;
	this->register_command_event_listener( IDC_MIN_VIEW_BEGIN, boost::bind(&MemoryWindowFrame::on_view_min_begin, this, _1 )) ;
	this->register_command_event_listener( ID_APP_ABOUT, boost::bind(&MemoryWindowFrame::show_about_dialog, this, _1 )) ;
	this->register_command_event_listener( ID_HELP, boost::bind(&MemoryWindowFrame::on_help, this, _1 )) ;

	this->register_command_event_listener( ID_HELP_FAQ, boost::bind(&MemoryWindowFrame::on_help_faq, this, _1 )) ;
	this->register_command_event_listener(ID_HELP_CHECKUPDATES, boost::bind(&MemoryWindowFrame::on_help_check_updates, this, _1 )) ;

	this->register_command_event_listener( IDC_SET_GLOSS, boost::bind(&MemoryWindowFrame::on_register_gloss, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_PREFERENCES, boost::bind(&MemoryWindowFrame::on_tools_preferences, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_RULEMANAGER, boost::bind(&MemoryWindowFrame::on_tools_rule_manager, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_LANGUAGE, boost::bind(&MemoryWindowFrame::on_tools_switch_language, this, _1 )) ;

	this->register_command_event_listener( ID_TOOLS_LOADPREFERENCES, boost::bind(&MemoryWindowFrame::on_tools_load_preferences, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_SAVEPREFERENCES, boost::bind(&MemoryWindowFrame::on_tools_save_preferences, this, _1 )) ;

	this->register_command_event_listener( IDC_TEST_EXCEPTION, boost::bind(&MemoryWindowFrame::on_test_exception, this, _1 )) ;

	this->register_command_event_listener( ID_NEXT_PANE, boost::bind(&MemoryWindowFrame::on_toggle_views, this, _1 )) ;
	this->register_command_event_listener( ID_PREV_PANE, boost::bind(&MemoryWindowFrame::on_toggle_views, this, _1 )) ;

	this->register_command_event_listener( ID_VIEW_ZOOM, boost::bind(&MemoryWindowFrame::on_view_zoom, this, _1 )) ;
	for ( int i=ID_FILE_MRU_FIRST ; i <=ID_FILE_MRU_LAST ; ++i )
	{
		this->register_command_event_listener( i, boost::bind(&MemoryWindowFrame::on_mru_file_open, this, _1 )) ;
	}

}


//! DTOR
MemoryWindowFrame::~MemoryWindowFrame()
{
}

/**************************************************
*
*	Windows messaging routines
*
**************************************************/

#define WATCH(var, val) if (var == val) \
{\
	ATLTRACE("Got value of " #val " (%d)\n", var) ; \
}


/** We get a crack at messages before the are dispatched.
*/
BOOL MemoryWindowFrame::PreTranslateMessage(MSG* pMsg)
{
	WATCH(pMsg->message, WM_DROPFILES) ;

	if( m_min_view.IsWindow() && m_min_view.IsWindowVisible() )
	{
		return m_min_view.PreTranslateMessage( pMsg ) ;
	}

	// Go through our glossary windows, and see if we need to pass on 
	// this message to them...
	m_glossary_windows.remove_destroyed_gloss_windows() ;

	if (m_glossary_windows.pre_translate(pMsg))
	{
		return TRUE ;
	}

	ENSURE_ACTIVE

	// let the frame window have a try
	if( frame_class::PreTranslateMessage( pMsg ) )
	{
		return TRUE;
	}
	// prevent the view from eating our menu shortcut keys...
	if (is_menu_key_pressed())
	{
		return FALSE ;
	}
	return m_view_interface.PreTranslateMessage( pMsg ) ;
}


/** Idle handler.
*/
BOOL MemoryWindowFrame::OnIdle()
{
	SENSE("OnIdle") ;
#ifdef UNIT_TEST
	return 0L ;
#else
	UIUpdateToolBar();
	user_feedback(CString(), 2) ;
	return FALSE;
#endif
}

//! Gets the background color.
CColorRef MemoryWindowFrame::GetBackColor()
{
	return CColorRef( m_view_interface.get_bg_color() ) ;
}

//! Respond to user command to format the background color.
LRESULT MemoryWindowFrame::OnFormatBackgroundColor( WindowsMessage &message )
{
	message ;
	SENSE("OnFormatBackgroundColor") ;

	const CColorRef color = GetBackColor();

	SENSE(color.as_string()) ;

#ifdef UNIT_TEST
	return 0L ;
#else

	CColorDialog dialog( color.as_colorref() ) ; // current color is default

	if ( dialog.DoModal() == IDCANCEL )
	{
		return 0L ;
	}
	// get the color the user picked
	const CColorRef newcolor(dialog.GetColor()) ;

	m_props->m_view_props.m_data.m_back_color = (int)newcolor.as_colorref() ;
	m_props->m_view_props.write_to_registry() ;

	// turn it into an HTML tag
	m_view_interface.set_bg_color( newcolor.as_wstring() ) ;

	return 0L ;
#endif
}

//! Get the source and target languages for saving current memory as TMX.
void MemoryWindowFrame::set_exporter_src_and_target_langs(CExportDialog &dialog, CTMXWriter &exporter)
{
	exporter.set_src_lang( string2wstring( dialog.get_source_plain() ) ) ;
	exporter.set_target_lang( string2wstring( dialog.get_trans_plain() ) ) ;
}

//! Export the top memory as a TMX memory.
bool MemoryWindowFrame::export_tmx( const CString &file_name, mem_engine::memory_pointer mem )
{
	CExportDialog dialog ;

	if ( IDCANCEL == dialog.DoModal() )
	{
		return false ;
	}

	CTMXWriter exporter( static_cast< CProgressListener* >( this ),
						m_props,
						m_props->m_gen_props.get_user_name()) ;

	set_exporter_src_and_target_langs(dialog, exporter);
	file::CPath path(file_name) ;
	path.RemoveExtension() ;
	path.AddExtension( _T(".tmx" ) ) ;

	mem->set_location( path.Path() ) ;

	exporter.write_memory( mem ) ;

	return true ;
}

//! Exports the current memory as a trados text file
bool MemoryWindowFrame::export_trados( const CString &file_name, mem_engine::memory_pointer mem )
{
	ATLASSERT ( m_model->empty() == false ) ; 
	ATLASSERT ( ! file_name.IsEmpty() ) ; 

	CExportDialog dialog ;

	if ( IDCANCEL == dialog.DoModal() )
	{
		return false ;
	}

	// get a set of the fonts used in our current memory
	font_tabulator tabulator ;
	mem->tabulate_fonts( tabulator ) ;

	file::CPath path(file_name) ;
	path.RemoveExtension() ;
	path.AddExtension( _T(".txt" ) ) ;

	mem->set_location( path.Path() ) ;
	// create the exporter
	TradosDataExporter exporter( tabulator.get_font_set( ), 
								static_cast< CProgressListener* >( this ),
								this->m_props) ;
	if ( ! exporter.open_destination( path.Path() ) )
	{
		user_feedback( IDS_MSG_EXPORT_FAILED ) ;
		THROW_WIN_EXCEPTION( R2T( IDS_MSG_EXPORT_FAILED ) ) ;
	}

	exporter.set_source( dialog.get_source_plain() ) ;
	exporter.set_target( dialog.get_trans_plain() ) ;

	exporter.export_trados( mem ) ;

	return true ;
}

//! WM_CREATE message handler
LRESULT MemoryWindowFrame::on_create( WindowsMessage &message  )
{
	message ;
	SENSE("on_create") ;

	try
	{
		logging::log_debug("Creating mainframe window") ;
		// the global parser object
		ATLASSERT( IsWindow() ) ;

		// set big icon
		SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_ICON ) ), TRUE ) ;

		// read our properties from the registry
		m_appstate.read_from_registry() ;
		load_mousewheel_setting();
		input_device_ptr input(new InputDeviceFile) ;
		output_device_ptr output(new OutputDeviceFile) ;
		m_rules.load(input, output) ;

		// the view
		logging::log_debug("Initializing mainframe view") ;
		m_hWndClient = init_view() ;

		m_view_interface.set_accel(m_hAccel) ;

		ATLASSERT(m_view_state == &m_view_state_initial) ;

		set_display_state( INIT_DISPLAY_STATE ) ;
		ATLASSERT(m_view_state == &m_view_state_initial) ;

		m_view_state->show_content() ;

		set_up_recent_docs_list() ;
		set_up_command_bars() ;
		init_status_bar() ;

		// register object for message filtering and idle updates
#ifndef UNIT_TEST
		set_up_ui_state() ;
		CMessageLoop* pLoop = _Module.GetMessageLoop() ;
		ATLASSERT(pLoop != NULL) ;
		if (pLoop)
		{
			pLoop->AddIdleHandler(this) ;
		}
#endif

		// the glossary window
		logging::log_debug("Setting up the glossary window") ;
		add_glossary_window(m_props) ;

		// set the title
		set_window_title() ;

		set_up_window_size() ;

		UpdateLayout() ; // the toolbar

		init_item_colors();

		::DragAcceptFiles( m_hWnd, TRUE ) ;

		init_background_processor();

		SetFocus() ;

		::PostMessage( m_hWnd, WM_COMMAND, MAKEWPARAM( IDC_STARTUP_CHECKS, 100 ), 0 ) ;

		logging::log_debug("Checking load history") ;
		check_load_history() ;
		set_bg_color_if_needed() ;

		logging::log_debug("Checking command line") ;
		const int language = m_props->m_gen_props.m_data.m_preferred_gui_lang ;
		commandline_options options(GetCommandLine(), static_cast<WORD>(language)) ;

		check_command_line(options, get_input_device()) ;
		if (language != options.m_language)
		{
			SetUILanguage(options.m_language) ;
		}

		set_doc_ui_handler();
	}
	catch (CException&)
	{
		logging::log_error("Failure in mainframe initialization routine") ;
		throw ;
	}

	return 0L;
}

//! Check whether the preferences say to load the previous
//! memories, and load them if so.
void MemoryWindowFrame::check_load_history( )
{
	if ( ! m_props->m_gen_props.load_prev_mem_on_startup() ) 
	{
		return ;
	}

	load_history();
}

//! See if the command line has us loading any memories.
void MemoryWindowFrame::check_command_line(commandline_options &options, input_device_ptr input)
{
	FOREACH(tstring filename, options.m_tm_files)
	{
		CString cfilename = filename.c_str() ;
		load_felix_memory(false, cfilename);
	}
	FOREACH(tstring filename, options.m_glossary_files)
	{
		CString cfilename = filename.c_str() ;
		this->get_glossary_window()->load(cfilename, false) ;
	}
	FOREACH(tstring filename, options.m_xml_files)
	{
		memory_pointer mem(new memory_local(m_props)) ;
		mem->load(filename.c_str()) ;
		if (mem->get_memory_info()->is_memory())
		{
			this->add_memory(mem) ;
		}
		else
		{
			this->get_glossary_window()->add_glossary(mem) ;
		}
	}
	FOREACH(tstring filename, options.m_multiterm_files)
	{
		this->get_glossary_window()->import_multiterm(CString(filename.c_str())) ;
	}
	FOREACH(tstring filename, options.m_tmx_files)
	{
		this->import_tmx(CString(filename.c_str()), input) ;
	}
	FOREACH(tstring filename, options.m_trados_text_files)
	{
		this->import_trados(CString(filename.c_str())) ;
	}
	if (! options.m_prefs_file.empty())
	{
		if (options.m_new_prefs_format)
		{
			m_props->load_file(options.m_prefs_file) ;
		}
		else
		{
			this->load_old_preferences(options.m_prefs_file.c_str()) ;
		}
	}
}



//! Handle a user exit command
LRESULT MemoryWindowFrame::on_file_exit(  WindowsMessage &message )
{
	message ;
	SENSE("on_file_exit") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	PostMessage(WM_CLOSE);
	return 0L;
#endif
}

/** Deletes the current entry.
*/
LRESULT MemoryWindowFrame::on_delete_entry(  WindowsMessage &message )
{
	message ;
	SENSE("on_delete_entry") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	if ( m_view_interface.is_edit_mode() )
	{
		m_view_interface.do_delete() ;
	}
	else
	{
		delete_current_translation() ;
	}
	return 0L;
#endif
}


/** Peform a find.
* We use different dialogs depending on whether we're in edit mode.
*/
LRESULT MemoryWindowFrame::on_find( WindowsMessage &message  )
{
	message ;
	SENSE("on_find") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	if ( m_view_interface.is_edit_mode() )
	{
		if ( m_edit_replace.IsWindow() && m_edit_replace.IsWindowVisible() )
		{
			m_edit_replace.ShowWindow( SW_HIDE ) ;
		}
		m_edit_find.set_document( m_view_interface.get_document() ) ;
		// we will make sure that the edit find window
		// is created, and then show it
		init_edit_find_window( SW_SHOW ) ;
	}
	else
	{
		handle_find() ;
	}
	return 0L ;
#endif
}

/** The Replace dialog has told us that it's time to do a replace.
* We don't allow this message while not in edit mode.
*/
LRESULT MemoryWindowFrame::on_edit_replace(  WindowsMessage &message )
{
	message ;
	SENSE("on_edit_replace") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	if ( m_view_interface.is_edit_mode() == false )
	{
		on_new_search(message) ;
		m_search_window.show_replace_page() ;
	}
	else
	{
		if ( m_edit_find.IsWindow() && m_edit_find.IsWindowVisible() )
		{
			m_edit_find.ShowWindow( SW_HIDE ) ;
		}
		m_edit_replace.set_document( m_view_interface.get_document() ) ;
		init_edit_replace_window( SW_RESTORE ) ;
	}
	return 0L ;
#endif
}


/** The user has chosen to get concordance via the dialog.
* Edit -> Find -> Concordance.
*/
LRESULT MemoryWindowFrame::on_concordance(  WindowsMessage &message )
{
	message ;
	SENSE("on_concordance") ;

#ifdef UNIT_TEST
	return 0L ;
#else

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
#endif
}

/** Adds a new glossary window.
* File -> New -> Glossary.
*/
LRESULT MemoryWindowFrame::on_new_glossary(  WindowsMessage &message )
{
	message ;
	SENSE("on_new_glossary") ;

	add_glossary_window(m_props) ;
	return 0L ;
}

/** Adds a new glossary window.
*/
bool MemoryWindowFrame::add_glossary_window(app_props::props_ptr props)
{
	const int sw_command = get_gloss_show_command() ;

	gloss_window_pointer gloss_window = m_glossary_windows.m_add(props) ;

// we will work a seam into this
#ifndef UNIT_TEST
	if(!m_glossary_windows.create(gloss_window, m_hWnd))
	{
		return false ;
	}
#endif

	if ( m_glossary_windows.size() == 1 ) // we have just added the only one...
	{
		gloss_window->m_apply_settings(sw_command) ;
	}
#ifndef UNIT_TEST
	gloss_window->ShowWindow(sw_command);
#endif
	gloss_window->set_listener( static_cast< CGlossaryWinListener* >( this ) ) ;

	return true ;
}

/** Handles the new file command. Creates a new memory.
* File -> New -> Memory.
*/
LRESULT MemoryWindowFrame::on_file_new( WindowsMessage &message  )
{
	message ;
	SENSE("on_file_new") ;
	memory_pointer mem = m_model->create_memory() ;

	add_memory( mem ) ;

	user_feedback( IDS_NEW ) ;

	return 0L ;
}

/** Handles file open command.
*/
LRESULT MemoryWindowFrame::on_file_open(  WindowsMessage &message )
{
	message ;
	SENSE("on_file_open") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	// get the file name
	file_open_dialog dialog;

	dialog.set_title(R2T(IDS_OPEN));
	dialog.set_file_filter(get_mem_open_filter());
	dialog.allow_multi_select();
	if (!m_props->m_history_props.m_memory_location.empty())
	{
		dialog.set_last_save(m_props->m_history_props.m_memory_location);
	}

	user_feedback( IDS_OPEN ) ;

	if (!dialog.show())
	{
		user_feedback(IDS_CANCELLED_ACTION);
		return 0L;
	}

	auto filenames = dialog.get_open_destinations();

	switch (dialog.get_selected_index())
	{
	case 1: case 4:
		ATLTRACE( "Open Felix memory files.\n" ) ;
		break;

	case 2:
		import_tmx(filenames, get_input_device());
		return 0L ;

	case 3:
		import_trados(filenames);
		return 0L ;

	default:
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 

	}

	// They are regular memory files
	for(CString filename: filenames)
	{
		load(filename) ;
	}

	set_window_title() ;

	return 0L ;
#endif
}

/** Handle the BeforeNavigate2 event from the MSHTML view.
* If a file was dropped, load it.
*/
bool MemoryWindowFrame::OnBeforeNavigate2( _bstr_t url )
{
	SENSE("OnBeforeNavigate2") ;

	try
	{
		// "#" is used for JavaScript links.
		wstring _url = BSTR2wstring(url) ;
		if(boost::ends_with(_url, L"#"))
		{
			return false ; // don't cancel
		}

		const CString filename(static_cast< LPCWSTR >( url )) ;
		const file::CFileExtension ext(filename) ;
		if (ext.is_xml() || ext.equals(_T(".ftm")) || ext.equals(_T(".tmx")) || ext.is_txt())
		{
			this->load(filename) ;
			return true ; // should cancel
		}
		if (ext.equals(".fprefs"))
		{
			this->load_old_preferences(filename) ;
			return true ;
		}
		if (ext.equals(".fprefx"))
		{
			this->m_props->load_file((LPCWSTR)filename) ;
			return true ;
		}

		wstring raw_command = BSTR2wstring( url );

		/***********************************************************************
		There are two url navigation schemes here:
		- destination_parser
		- CNavUrl
		We've got to consolidate this into one scheme.
		Also, the newer components (SearchWindow, ManagerWindow) use the
		simpler tokenize-and-parse scheme. I think we should switch to this
		for the mainframe and glossary windows, too.
		*************************************************************************/
		destination_parser parser ;

		// will return NULL if fails to parse
		const LPMSG pMsg = parser.parse( raw_command ) ;

		if ( pMsg ) 
		{
			WindowsMessage message( pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam ) ;
			switch( pMsg->wParam )
			{
			case IDC_NAV:
				on_user_nav( pMsg->lParam ) ;
				return true ;
			case IDC_EDIT:
				on_user_edit( message ) ;
				return true ;
			case IDC_DELETE:
				on_user_delete( static_cast<size_t>(pMsg->lParam) ) ;
				return true ;
			case IDC_ADD_TO_GLOSSARY:
				on_user_add_to_glossary( pMsg->lParam ) ;
				return true ;
			case IDC_REGISTER:
				on_user_register( pMsg->lParam ) ;
				return true ;
			case IDC_TOGGLE_MARK:
				on_user_toggle_markup( message ) ;
				return true ;
			}

		}

		CNavUrl nav_url (static_cast< CNavInterface* >( this )) ;

		nav_url.process_url(url) ;

		return nav_url.should_cancel() ;
	}
	catch (CException& e)
	{
		logging::log_exception(e) ;
		e.notify_user(_T("Failed to process action")) ;
	}
	catch (_com_error& e)
	{
		CComException cx(e) ;
		logging::log_exception(cx) ;
		cx.notify_user(_T("Failed to process action\rAn error occurred connecting to a COM server.")) ;
	}
	catch (std::exception& e)
	{
		logging::log_error("Standard library exception (Main Window)") ;
		logging::log_exception(e) ;
		const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND ;
		::MessageBox( m_hWnd, CA2T(e.what()), _T("C Runtime Error in Main Window"), msg_flags ) ;  
	}
	return true ;
}


/** Save the top memory.
* File -> Save.
*/
LRESULT MemoryWindowFrame::on_file_save(WindowsMessage &)
{
	SENSE("on_file_save") ;

	if ( m_model->empty() )
	{
		SENSE("empty") ;
		return 0L ;
	}

	memory_pointer mem = m_model->get_first_memory() ; 

	if ( mem->is_new() ) 
	{
		WindowsMessage message ;
		on_file_save_as(message) ;
		return 0L ;
	}

	do_save( mem ) ;
	return 0L ;
}

/** Perform a file save. 
* We already have the location.
*/
void MemoryWindowFrame::do_save( memory_pointer mem )
{
	const file::CFileExtension ext = mem->get_location() ;

	if ( ext.equals( _T(".txt") ) || ext.equals( _T(".tmx") ) || ext.equals( _T(".xls") ) )
	{
		handle_foreign_file_save(mem, ext);	
	}
	else  
	{
		save_memory( mem ) ;
	}

	set_window_title() ;
}

/** Save a memory that was imported from another format.
*/
void MemoryWindowFrame::handle_foreign_file_save(memory_pointer& mem, const file::CFileExtension& ext)
{
	switch ( wants_to_save_in_native_format() ) 
	{
	case IDYES:
		if ( ext.equals( _T(".txt") ) )
		{
			export_trados( mem->get_location(), mem ) ;
		}
		else if ( ext.equals( _T(".xls") ) )
		{
			export_excel(mem->get_location(), mem) ;
		}
		else
		{
			export_tmx( mem->get_location(), mem ) ;
		}
		break ;
	case IDNO:
		save_memory( mem ) ;
		break ;
	case IDCANCEL:
		return ;
	}

}

/** Saves the top memory with a new name.
* File -> Save As.
*/
LRESULT MemoryWindowFrame::on_file_save_as(WindowsMessage &)
{
	SENSE("on_file_save_as") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	if ( m_model->empty() )
	{
		user_feedback(IDS_NO_MATCHES) ;
		::MessageBeep( MB_ICONINFORMATION ) ;
		return 0L ;
	}
	// clearing location won't work, because we want to offer the current location
	// as the default file name
	memory_pointer mem = m_model->get_first_memory() ;

	save_memory_as(mem);

	return 0L ;
#endif
}


/** Responds to WM_CLOSE message.
*/
LRESULT MemoryWindowFrame::on_close( WindowsMessage &message )
{
	SENSE("on_close") ;

#ifndef UNIT_TEST
	// if the user cancels during the save query, then take no action.
	if ( IDCANCEL == check_save() ) 
	{
		SetMsgHandled( TRUE ) ;
		return 1L ;
	}

	if ( false == gloss_win_shutdown_check() )
	{
		return 1L ;
	}

	// do it twice, in case we chose a location that clashes after the check...
	if ( false == gloss_win_shutdown_check() )
	{
		return 1L ;
	}

	save_settings_close();


#endif

	message.notHandled() ;

	return 0L;
}


/** Responds to WM_DESTROY message.
*/
LRESULT MemoryWindowFrame::on_destroy( WindowsMessage &message )
{
	// let the base window procs get a chance
	message.notHandled() ;

	SENSE("on_destroy") ;

#ifdef UNIT_TEST
	return 0L ;
#else

	ATLASSERT( IsWindow() ) ; // we still need to be a window because...

	if(m_search_window.IsWindow())
	{
		m_search_window.DestroyWindow() ;
	}
	if (m_manager_window.IsWindow())
	{
		m_manager_window.DestroyWindow() ;
	}

	save_settings_destroy();

	// no longer the first launch
	m_props->m_gen_props.set_not_first_launch() ;
	m_props->write_to_registry() ;

	m_statusbar.release() ;

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	if (pLoop)
	{
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
	}
	// if UI is the last thread, no need to wait
	if(_Module.GetLockCount() == 1)
	{
		_Module.m_dwTimeOut = 0L;
		_Module.m_dwPause = 0L;
	}
	_Module.Unlock();
	return 0L ;
#endif
}

/** See if we should save our memory/glossary history.
* If so, we'll load the same memories/glossaries on next startup.
*/
void MemoryWindowFrame::record_loaded_memory_history()
{
	app_props::properties_loaded_history *history_props = &m_props->m_history_props ;

	history_props->m_loaded_mems.clear() ;
	history_props->m_loaded_remote_mems.clear() ;
	
	m_model->record_loaded_memories(history_props->m_loaded_mems, history_props->m_loaded_remote_mems) ;

	history_props->write_to_registry() ;
}


/** Toggle toolbar visibility.
*/
LRESULT MemoryWindowFrame::on_view_toolbar(WindowsMessage &)
{
	SENSE("on_view_toolbar") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	m_appstate.m_is_toolbar_visible = !m_appstate.m_is_toolbar_visible;
	CReBarCtrl rebar = m_hWndToolBar;
	const int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, m_appstate.m_is_toolbar_visible);
	UISetCheck(ID_VIEW_TOOLBAR, m_appstate.m_is_toolbar_visible);
	UpdateLayout();
	return 0;
#endif
}


/** Toggle edit mode.
*/
LRESULT MemoryWindowFrame::on_view_edit_mode(WindowsMessage &)
{
	SENSE("on_view_edit_mode") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	const bool edit_mode_enabled = m_view_interface.is_edit_mode() ;


	UISetCheck(ID_VIEW_EDIT_MODE, ! edit_mode_enabled );

	// swap out the various find dialogs...
	SwapFindDialogs(edit_mode_enabled) ;

	m_view_state->handle_toggle_edit_mode() ;
	return 0L ;
#endif
}




/** Toggle status bar visibility.
*/
LRESULT MemoryWindowFrame::on_view_status_bar(  WindowsMessage &message )
{
	message ;
	SENSE("on_view_status_bar") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	m_appstate.m_is_statusbar_visible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, m_appstate.m_is_statusbar_visible ? SW_SHOWNOACTIVATE : SW_HIDE);

	UISetCheck(ID_VIEW_STATUS_BAR, m_appstate.m_is_statusbar_visible );
	UpdateLayout();
	return 0;
#endif
}

/** Switch to match view.
* View -> Current View -> Match View.
*/
LRESULT MemoryWindowFrame::on_view_match(  WindowsMessage &message ) 
{
	message ;
	SENSE("on_view_match") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	// the current match...
	if ( ! m_trans_matches.empty() ) // we have 0 matches, just a query
	{
		recalculate_match(m_trans_matches.current(), m_trans_matches.m_params);
	}
	set_display_state ( MATCH_DISPLAY_STATE ) ;
	show_view_content( ) ;
	m_view_interface.set_scroll_pos(0) ;

	return 0L ;
#endif
}

/** Switch to search view.
* View -> Current View -> Search View.
*/
LRESULT MemoryWindowFrame::on_view_search(  WindowsMessage &message ) 
{
	message ;
	SENSE("on_view_search") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	set_display_state( CONCORDANCE_DISPLAY_STATE ) ;
	show_view_content() ;
	m_view_interface.set_scroll_pos(0) ;

	return 0L ;
#endif
}

// ==================================
//  Help Menu Functions
// ==================================

/*!
* Show the About dialog.
*/
LRESULT MemoryWindowFrame::show_about_dialog(WindowsMessage &)
{
	SENSE("show_about_dialog") ;
	logging::log_debug("Showing the About dialog.") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	CAboutDialog dlg(cpptempl::get_template_filename(_T("about.html"))) ;
	dlg.DoModal();
	return 0L ;
#endif
}

/** Help -> Help.
* Show the Felix help.
*/
LRESULT MemoryWindowFrame::on_help(WindowsMessage &)
{
	CString filePath = get_help_file_path(get_docs_path()) ;
	SENSE("on_help") ;

#ifdef UNIT_TEST
	return 0L ;
#else

	HINSTANCE result = ::ShellExecute(
		m_hWnd,				// HWND hwnd, 
		_T("open"),			// LPCTSTR lpOperation,
		filePath,			// LPCTSTR lpFile, 
		NULL,				// LPCTSTR lpParameters, 
		NULL,				// LPCTSTR lpDirectory,
		SW_SHOW				// INT nShowCmd
		);	

	check_shell_execute_result((int)result, filePath);
	return 0L ;
#endif
}

/** Help -> FAQ.
* Show the FAQ.
*/
LRESULT MemoryWindowFrame::on_help_faq( WindowsMessage &)
{
	SENSE("on_help_faq") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	file::CPath faq_path ;
	faq_path.GetModulePath(_Module.GetModuleInstance()) ;
	faq_path.Append( _T("Docs\\Felix Help.chm") ) ;

	if ( faq_path.FileExists() )
	{
		CString current_directory = faq_path.Path() ;
		current_directory += _T("::/8.html") ;
		HtmlHelp(m_hWnd, current_directory, HH_DISPLAY_TOPIC, NULL ); 
		return 0L ;
	}

	HINSTANCE result = ::ShellExecute(
		NULL,					//	HWND hwnd, 
		_T("open"),				// LPCTSTR lpOperation,
		resource_string( IDS_FAQ_PATH ),	// LPCTSTR lpFile, 
		NULL,					// LPCTSTR lpParameters, 
		NULL,					// LPCTSTR lpDirectory,
		SW_SHOW					// INT nShowCmd
		);	

	CString file_path = resource_string( IDS_FAQ_PATH ) ;
	check_shell_execute_result((int)result, file_path) ;
	return 0L ;
#endif
}

/** Register glossary entries.
* Use the appropriate translation-memory entry as the base
* according to the current state.
*/
LRESULT MemoryWindowFrame::on_register_gloss(WindowsMessage &)
{
	switch( get_display_state() )
	{
	case NEW_RECORD_DISPLAY_STATE:
		SENSE("[NEW_RECORD_DISPLAY_STATE]") ;
		ATLASSERT(m_view_state == &m_view_state_new) ;
		on_user_register( 0 ) ;
		return 0L ;
	case MATCH_DISPLAY_STATE:
		SENSE("[MATCH_DISPLAY_STATE]") ;
		on_user_register( m_trans_matches.current_pos() ) ;
		return 0L ;
	case CONCORDANCE_DISPLAY_STATE:
		SENSE("[CONCORDANCE_DISPLAY_STATE]") ;
		on_user_register( m_search_matches.current_pos() ) ;
		return 0L ;
	default:
		SENSE("[Other display state]") ;
		on_user_register( 0 ) ;
		return 0L ;
	}
}

/** Get the translation matches.
* Use params as the parameters and put the matches in matches.
*/
void MemoryWindowFrame::get_matches(trans_match_container &matches, search_query_params &params)
{
	const double MATCH_THRESHOLD = 0.9999 ;

	m_model->find_matches(matches, params) ;

	if (!params.m_place_numbers && 
		!params.m_place_gloss && 
		!params.m_place_rules &&
		!params.m_show_gloss_matches)
	{
		logging::log_debug("no placement options selected; bailing") ;
		return ;
	}

	trans_match_container placed_numbers ;
	trans_match_container placed_gloss ;
	trans_match_container placed_rules;

	FOREACH(search_match_ptr match, matches)
	{
		if ( match->get_score() < MATCH_THRESHOLD )
		{
			if (params.m_place_numbers)
			{
				check_placement_numbers(placed_numbers, match);
			}
			if (params.m_place_gloss)
			{
				check_placement_gloss(placed_gloss, match);
			}
			if (params.m_place_rules)
			{
				check_placement_rules(placed_rules, match);
			}
		}
		if(params.m_show_gloss_matches)
		{
			show_gloss_matches(this->m_glossary_windows.get_current_matches(), match);
		}
	}
	// Now place the gloss in the number matches.
	// This will get unwieldy with too many more types of
	// placement...
	// Got to redo this logic!
	FOREACH(search_match_ptr match, placed_numbers)
	{
		check_placement_gloss(placed_gloss, match);
	}

	// now add in all our placements
	FOREACH(search_match_ptr match, placed_numbers)
	{
		matches.insert(match) ;
		if(params.m_show_gloss_matches)
		{
			show_gloss_matches(this->m_glossary_windows.get_current_matches(), match);
		}
	}
	FOREACH(search_match_ptr match, placed_gloss)
	{
		matches.insert(match) ;
		if(params.m_show_gloss_matches)
		{
			show_gloss_matches(this->m_glossary_windows.get_current_matches(), match);
		}
	}
	FOREACH(search_match_ptr match, placed_rules)
	{
		matches.insert(match) ;
		if(params.m_show_gloss_matches)
		{
			show_gloss_matches(this->m_glossary_windows.get_current_matches(), match);
		}
	}
}

void MemoryWindowFrame::show_gloss_matches(mem_engine::felix_query *query, search_match_ptr match)
{
	logging::log_debug("MemoryWindowFrame: showing gloss matches in query") ;
	if(!query)
	{
		logging::log_warn("NULL query received from gloss window collection") ;
		return ; 
	}
	
	pairings_t &pairings = match->match_pairing().get() ;

	mem_engine::gloss_match_set gloss_sources ;
	for(size_t i = 0 ; i < query->size() ; ++i)
	{
		search_match_ptr gloss_match = query->at(i) ;
		const wstring source = gloss_match->get_record()->get_source_plain() ;
		gloss_sources.insert(source) ;
	}
	mem_engine::placement::mark_up_gloss_matches(pairings, gloss_sources) ;
	match->match_pairing().set(pairings) ;
	match->get_markup()->SetQuery( match->match_pairing().mark_up_query() ) ;
}

/** Initializes the transaction matches for a new lookup.
*/
void MemoryWindowFrame::init_trans_matches_for_lookup( const wstring  query )
{
	m_trans_matches.clear() ;
	m_trans_matches.set_query( query ) ;
	init_lookup_properties(m_props, m_trans_matches.m_params);
}

/** Look up a query string.
*/
bool MemoryWindowFrame::lookup(const wstring query)
{
	if ( query.empty() )
	{
		user_feedback( IDS_EMPTY_QUERY ) ;
		::MessageBeep( MB_ICONINFORMATION ) ;
		return false ;
	}

	m_model->set_reverse_lookup(false)  ;

	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	// do glossary lookup as well
	m_glossary_windows.look_up( query ) ;

	// now do TM lookup
	init_trans_matches_for_lookup(query);

	trans_match_container matches ;
	get_matches(matches, m_trans_matches.m_params);
	m_trans_matches.set_matches(matches) ;

	// give the user feedback
	provide_user_trans_feedback();

	set_display_state ( MATCH_DISPLAY_STATE ) ;
	show_view_content() ;

	return true ;
}


/** Gets the currently displayed translation.
*/
wstring MemoryWindowFrame::get_current_translation()
{
	if ( m_trans_matches.empty() )
	{
		return wstring( ) ;
	}
	search_match_ptr current = m_trans_matches.current() ;

	return get_record_translation(current->get_record());
}


/** Shows the next match.
*/
bool MemoryWindowFrame::next_match()
{
	if ( m_trans_matches.empty() )
	{
		return false ;
	}
	m_trans_matches.forward() ;

	if ( m_model->is_reverse_lookup() )
	{
		look_up_current_source_in_gloss();
	}
	show_view_content() ;

	return true ; 
}


/** Shows the previous match.
*/
bool MemoryWindowFrame::prev_match()
{
	if ( m_trans_matches.empty() )
	{
		return false ;
	}
	m_trans_matches.back() ;

	if ( m_model->is_reverse_lookup() )
	{
		look_up_current_source_in_gloss();
	}

	show_view_content() ;

	return true ; 
}


/** Gets the current query.
*/
wstring MemoryWindowFrame::get_current_query()
{
	if (  m_model->is_reverse_lookup() ) 
	{
		return m_trans_matches.get_source_rich() ;
	}

	return m_trans_matches.get_query_rich() ;
}


/** Sets the translation for the current query.
*/
bool MemoryWindowFrame::set_translation( const wstring translation)
{
	try
	{
		TRUE_ENFORCE( ! m_trans_matches.get_query_rich().empty(), IDS_NO_QUERIES ) ;
		TRUE_ENFORCE( ! translation.empty(), IDS_EMPTY_TRANSLATION ) ;

		if (get_display_state() == NEW_RECORD_DISPLAY_STATE)
		{
			ATLASSERT(m_view_state == &m_view_state_new) ;
			return this->correct_trans(translation) ;
		}
		record_pointer record(new record_local()) ; // a new record

		// the query is the source for our translation
		record->set_source( m_trans_matches.get_query_rich() ) ;
		// the user has kindly furnished a translation
		record->set_trans( translation ) ;

		// sanity checking
		ATLASSERT( record->get_source_rich().empty() == false ) ;
		ATLASSERT( record->get_trans_rich().empty() == false ) ;

		if (record->get_source_rich().empty())
		{
			throw except::CException(IDS_EMPTY_QUERY) ;
		}
		if (record->get_trans_rich().empty())
		{
			throw except::CException(IDS_EMPTY_TRANSLATION) ;
		}

		// it was created now
		record->create() ;

		add_record_to_memory( record ) ;

		// If the max add length is 0, then we don't add the entry to the glossary.
		// Made this explicit in the code below.
		if ( should_add_record_to_glossary(record)) 
		{
			m_glossary_windows.add_record(record) ;
		}
		// now, set the display content
		m_new_record = record ;

		set_display_state ( NEW_RECORD_DISPLAY_STATE ) ;
		m_view_state = &m_view_state_new ;

		show_view_content( ) ;

		return true ;
	}
	catch( CException &e )
	{
		logging::log_exception(e) ;
		e.notify_user( IDS_TRANS_SET_FAILED ) ; // TEXT("Failed to set translation.") ) ;
		return false ;
	}
}


/*!
* Registers a new glossary entry with the query as source, and trans as the translation.
*/
bool MemoryWindowFrame::register_trans_to_glossary(const wstring trans)
{
	if ( m_trans_matches.m_params.get_source_rich().empty() )
	{
		MessageBox( resource_string( IDS_NO_QUERIES ), resource_string( IDS_INVALID_ACTION ), MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND ) ;
		return false ;
	}
	record_pointer record(new mem_engine::record_local);
	record->set_source( m_trans_matches.m_params.get_source_rich() ) ;
	record->set_trans( trans ) ;
	record->create() ;

	m_glossary_windows.add_record(record) ;

	// give the user feedback
	user_feedback( IDS_MSG_ADDED_GLOSS_ENTRY_TITLE ) ;

	return true ;
}


/** Delete the current translation.
*
* Also deletes matching glossary entries.
* 
* \todo
* Make undoable.
*/
void MemoryWindowFrame::delete_current_translation()
{
	on_user_delete( m_view_state->get_current() ) ;
}


/** Get concordance for query_string.
* Concordances are co-locations of query_string in the source
* fields of memory records.
*/
bool MemoryWindowFrame::get_concordances(const wstring query_string )
{
	// an empty string would retrieve everything - probably not what the user wants!
	if ( query_string.empty() )
	{
		user_feedback(IDS_EMPTY_QUERY) ;
		return false ;
	}

	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	// remember where we were
	// in the future, make an array of states to allow Explorer-style page navigation
	set_display_state ( CONCORDANCE_DISPLAY_STATE ) ;

	// this will hold our matches
	m_search_matches.clear() ;
	m_search_matches.set_query( query_string ) ;

	m_search_matches.m_params.m_ignore_case = true ;
	m_search_matches.m_params.m_ignore_width =		!! m_props->m_gloss_props.m_data.m_ignore_width ;
	m_search_matches.m_params.m_ignore_hira_kata =	!! m_props->m_gloss_props.m_data.m_ignore_hir_kat ;

	perform_concordance_search(m_search_matches.m_params);

	show_view_content() ;
	m_view_interface.set_scroll_pos(0) ;

	// give the user feedback
	source_concordance_feedback();
	return true ;
}


/** Get the glossary entry at index from the main glossary window.
*/
wstring MemoryWindowFrame::get_glossary_entry(short index)
{
	return m_glossary_windows.get_glossary_entry(index) ;
}

/** Get the score for a given match.
* If index is -1, returns the score for the current match.
*/
double MemoryWindowFrame::get_score( const short index )
{
	if ( m_trans_matches.size() == 0 )
	{
		return 0.0 ;
	}

	if ( index < 0 ) // less than 0 means "current match"
	{
		return m_trans_matches.current()->get_score() ;
	}
	else
	{
		if ( static_cast<size_t>( index ) >= m_trans_matches.size() )
		{
			MessageBeep(MB_ICONEXCLAMATION) ;
			user_feedback(IDS_OUT_OF_RANGE) ;
			return 0.0 ;
		}
		return m_trans_matches.at( static_cast<size_t>(index) )->get_score() ;
	}
}

/** Callback from glossary window telling us to add a record.
*/
bool MemoryWindowFrame::gloss_add_record( record_pointer rec )
{
	add_record(rec) ;

	return true ;
}

/** Make sure that we aren't saving the glossary to a filename
* already taken by a memory.
*/
INT_PTR MemoryWindowFrame::gloss_check_save_location( memory_pointer mem )
{
	const CString mem_loc = mem->get_location() ;

	if (! m_model->has_name_clash(mem_loc))
	{
		return IDYES ;
	}

	return prompt_user_for_overwrite(mem->get_location());
}

/** Do a search using the parameters in the find dialog.
* The user has clicked "Find" in the find dialog.
*/
LRESULT MemoryWindowFrame::on_user_search(WindowsMessage &)
{	
	SENSE("on_user_search") ;

	perform_user_search();

	show_user_search_results();

	// give the user feedback
	provide_user_search_feedback() ;

	return 0 ;
}

/** Transfers the parameters from the replace dialog to the
* find dialog.
*/
LRESULT MemoryWindowFrame::on_user_edit_replace(WindowsMessage &)
{
	SENSE("on_user_edit_replace") ;

	m_edit_find.set_search_params( m_edit_replace.get_search_params() ) ;

	return 0 ;
}


/** The user has told us to edit an entry.
* If the index is out of range, call up the add record dialog instead.
*/
LRESULT MemoryWindowFrame::on_user_edit(WindowsMessage &message)
{
	SENSE("on_user_edit") ;

	const size_t num = static_cast<size_t>(message.lParam) ;
	m_view_state->set_current(num) ;
	m_editor->m_is_add = false ;
	m_view_state->on_user_edit() ;

	return 0L ;
}


/** Responds to user command to delete memory entry.
*/
LRESULT MemoryWindowFrame::on_user_delete(size_t num )
{
	SENSE("on_user_delete") ;

	m_view_state->delete_match(num) ;
	return 0L ;
}


/** Shows register glossary entry dialog.
*/
LRESULT MemoryWindowFrame::on_user_register(LPARAM num )
{
	SENSE("on_user_register") ;
	record_pointer rec = get_reg_gloss_record(static_cast<size_t>(num));

	m_reg_gloss_dlg.set_record( rec ) ;

	if ( m_glossary_windows.empty() ) 
	{
		add_glossary_window(m_props) ;
	}

	ATLASSERT ( m_glossary_windows.empty() == false ) ; 
	m_reg_gloss_dlg.set_gloss_window(m_glossary_windows.first()) ;

#ifdef UNIT_TEST
	return 0 ;
#else

	if ( ! m_reg_gloss_dlg.IsWindow() )
	{
		create_reg_gloss_window();
	}

	m_reg_gloss_dlg.ShowWindow( SW_SHOW ) ;

	wstring selection = m_view_interface.get_selection_text() ;
	if (! selection.empty())
	{
		m_reg_gloss_dlg.set_initial_source(string2BSTR(selection)) ;
	}
	else
	{
		m_reg_gloss_dlg.fresh_record_focus() ;
	}

	return 0L ;
#endif
}


/** Adds a new record to the active memory.
*/
bool MemoryWindowFrame::add_record( const record_pointer record )
{
	SENSE("add_record") ;
	ATLASSERT( record->is_valid_record() ) ;

	add_record_to_memory( record ) ;

	m_new_record = record ;

	set_display_state ( NEW_RECORD_DISPLAY_STATE ) ;
	m_view_state = &m_view_state_new ;

	this->show_view_content() ;
	return true ;
}



/** Retrieves the active glossary window
*/
gloss_window_pointer MemoryWindowFrame::get_glossary_window()
{
	if ( m_glossary_windows.empty() )
	{
		add_glossary_window(m_props) ;
	}
	return m_glossary_windows.first() ;
}

/** Show the view content.
*/
void MemoryWindowFrame::show_view_content()
{

	if (! IsWindow())
	{
		logging::log_debug("Window is not created; no content to display.") ;
		return ;
	}
	if ( m_min_view.IsWindow() && m_min_view.IsWindowVisible() )
	{
		m_min_view.show_content() ;
		return ;
	}

	m_view_state_match.set_gloss_matches(get_glossary_window()->get_current_matches()) ;
	m_view_state->show_content() ;
}


/** Check for existing memories loaded.
*
* Check whether the user wants to merge the memories if so.
*/

MemoryWindowFrame::MERGE_CHOICE MemoryWindowFrame::check_empty_on_load()
{
	if ( m_model->empty() ) 
	{
		return MERGE_CHOICE_SEPARATE ;
	}

	user_feedback( IDS_MEMORY_OPEN ) ;

	memory_pointer mem = m_model->get_first_memory() ;

	CQueryMergeDlg dlg(IDS_MERGE_MEM_TITLE, 
		IDS_MERGE_MEM_TEXT, 
		file::CPath(mem->get_location()).Path()) ;

	return get_merge_choice(dlg, &m_props->m_gen_props);
}


/** Get the translation for the entry at index.
*/
wstring MemoryWindowFrame::get_translation_at(short index)
{
	const int CURRENT_INDEX = -1 ;
	if ( index == CURRENT_INDEX )
	{
		return get_current_translation() ;
	}

	if ( m_trans_matches.empty() )
	{
		return wstring( ) ;
	}
	if ( static_cast<size_t>(index) >= m_trans_matches.size() )
	{
		MessageBeep(MB_ICONEXCLAMATION) ;
		user_feedback(IDS_OUT_OF_RANGE) ;
		return wstring() ;
	}

	search_match_ptr current = m_trans_matches.at( index ) ;
	return get_record_translation(current->get_record());
}


/** Report how many records we have loaded.
*/
void MemoryWindowFrame::report_memory_after_load(size_t original_num)
{
	memory_pointer mem = m_model->get_first_memory() ;

	// get number of records loaded
	const size_t num_records_after_load = mem->size() ;
	const size_t num_records_loaded = num_records_after_load - original_num ;
	CString arg1 ;
	arg1.Format( _T("%u"), num_records_loaded ) ;
	CString message ;
	message.FormatMessage( IDS_MSG_ADDED_RECORDS, arg1, file::name( get_location() ).file_name() ) ;
	user_feedback( message ) ; 
}


/** Handles user navigation.
*/
LRESULT MemoryWindowFrame::on_user_nav(const LPARAM lParam )
{
	SENSE("on_user_nav") ;

	switch( lParam )
	{
	case IDC_PREV:
		prev_display_state() ;
		show_view_content() ;
		return 0L ;
	case IDC_PREV_MATCH:
		prev_match() ;
		return 0L ;
	case IDC_NEXT_MATCH:
		next_match() ;
		return 0L ;
	case IDC_MORE:
		m_is_short_format = false ;
		show_view_content() ;
		return 0L ;
	case IDC_LESS:
		m_is_short_format = true ;
		show_view_content() ;
		return 0L ;
	}
	ATLASSERT( "We should never get here" && FALSE ) ;
	return 0L ;

}


/** Add the entry at index lParam to the glossary.
* If the index is out of range, then give a beep and feedback in the
* status bar (we used to throw an exception).
*/
LRESULT MemoryWindowFrame::on_user_add_to_glossary(const LPARAM lParam )
{
	SENSE("on_user_add_to_glossary") ;

	const size_t index = static_cast<size_t>(lParam) ;
	m_view_state->set_current(index) ;
	record_pointer rec = m_view_state->get_current_match()->get_record();

	get_glossary_window()->add_record(rec->clone());
	return 0L ;
}


//! Exit without showing confirmation dialogs. 
bool MemoryWindowFrame::exit_silently()
{
	SENSE("exit_silently") ;

	m_glossary_windows.exit_silently() ;

	memory_list memories ;
	m_model->get_memories_needing_saving( memories ) ;

	FOREACH(memory_pointer mem, memories)
	{
		mem->set_saved_flag(true) ;
	}

	PostMessage(WM_CLOSE);

	return true ;
}

/** Clears the active memory.
*/
bool MemoryWindowFrame::clear_memory()
{
	if ( ! m_model->empty() ) 
	{
		memory_pointer mem = m_model->get_first_memory() ;
		mem->clear_memory() ;
	}
	m_glossary_windows.clear_glossaries() ;

	m_view_interface.set_text( wstring() ) ;
	check_mousewheel() ;
	set_window_title() ;
#ifndef UNIT_TEST
	m_view_interface.set_scroll_pos(0) ;
#endif
	CString msg ;
	msg.FormatMessage( IDS_CLEARED_MEMORY, get_window_type_string() ) ;
	user_feedback( msg );

	// clear the matches
	m_search_matches.clear() ;
	m_trans_matches.clear() ;

	return true ;
}


//============================
// Message handlers
//============================

LRESULT MemoryWindowFrame::on_tools_rule_manager(WindowsMessage &) 
{
	SENSE("on_tools_rule_manager") ;
	logging::log_debug("Launching Rule Manager dialog.") ;

	user_feedback( IDS_MANAGE_RULES );

#ifndef UNIT_TEST

	// Call the COM server that will call up the rule manager.
	// After it returns, we reload the rules. They may or may not have changed.
	try
	{
		CDispatchWrapper wrapper(L"Felix.Preferences"); 
		CComVariant language = L"English";
		CComVariant prog = L"felix" ;

		if( m_appstate.m_preferred_gui_lang == LANG_JAPANESE )
		{
			language = L"Japanese" ;
		}

		wrapper.method(L"RuleManager", prog, language) ;
		wrapper.m_app = NULL ;

		input_device_ptr input(new InputDeviceFile) ;
		output_device_ptr output(new OutputDeviceFile) ;
		m_rules.load(input, output) ;
	}
	catch (_com_error& err)
	{
		logging::log_error("Call to rule manager failed") ;
		ATLASSERT(FALSE && "Raised exception in file_logger") ;
		except::CComException ce(err) ;
		logging::log_exception(ce) ;
		ce.notify_user(_T("Rule Manager Error")) ;
	}		
	catch(except::CException &e)
	{
		logging::log_error("Call to rule manager failed") ;
		logging::log_exception(e) ;
		e.notify_user(_T("Rule Manager Error")) ;
	}
	user_feedback( IDS_PREFS_REGISTERED ) ;

#endif
	return 0L ;
}


/** Responds to Tools > Preferences menu selection.
*/
LRESULT MemoryWindowFrame::on_tools_preferences(WindowsMessage &)
{
	SENSE("on_tools_preferences") ;
	logging::log_debug("Launching Preferences dialog.") ;

	user_feedback( IDS_SETTING_PREFS );

#ifndef UNIT_TEST
	m_props->write_to_registry() ;

	CPropertiesDlg props ;

	INT_PTR result = props.DoModal() ;
	if ( IDCANCEL == result || result <= 0 )
	{
		user_feedback( IDS_MSG_ACTION_CANCELLED ) ;
		return 0L ;
	}

	*m_props = *props.get_properties() ;

	m_props->write_to_registry() ;

	reflect_preferences() ;

	const wstring user_name = CT2W(m_props->m_gen_props.m_data.m_user_name) ;
	set_record_username(user_name) ;

	user_feedback( IDS_PREFS_REGISTERED ) ;
#endif

	return 0L ;
}


/* This generates a divide-by-zero exception for
* testing purposes. It tests the exception logging and reporting
* feature.
*/
LRESULT MemoryWindowFrame::on_test_exception(WindowsMessage &)
{
	logging::log_debug("Generating test exception") ;
	int x = 3 ;
	return (LRESULT)(5 / (x - 3)) ;
}

/** Responds to Tools > Language menu selection.
* Toggles UI language between Japanese and English.
*/
LRESULT MemoryWindowFrame::on_tools_switch_language(WindowsMessage &)
{
	SENSE("on_tools_switch_language") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	ATLASSERT( ! m_min_view.IsWindow() || ! m_min_view.IsWindowVisible() ) ;

	switch( m_appstate.m_preferred_gui_lang )
	{
	case LANG_JAPANESE :
		logging::log_debug("Switching UI language to English.") ;
		SetUILanguage( LANG_ENGLISH ) ;
		break ;
	case LANG_ENGLISH :
		logging::log_debug("Switching UI language to Japanese.") ;
		SetUILanguage( LANG_JAPANESE ) ;
		break ;
	default:
		ATLTRACE("** Unknown gui language: %d\n", m_appstate.m_preferred_gui_lang) ;
		SetUILanguage( LANG_ENGLISH ) ;
	}

	return 0L ;
#endif
}


/** Show the find dialog.
*/
LRESULT MemoryWindowFrame::handle_find()
{
	init_find_window( SW_RESTORE, IDS_MEMORY_SEARCH ) ;

	return 0L ;
}

/** Edit the current translation match.
*/
LRESULT MemoryWindowFrame::on_edit_entry(WindowsMessage &)
{
	SENSE("on_edit_entry") ;

	WindowsMessage message( NULL, 0, 0, m_trans_matches.current_pos() ) ;
	on_user_edit( message ) ;
	return 0L ;
}
/** Show the add entry dialog.
*/
LRESULT MemoryWindowFrame::on_add(WindowsMessage &)
{
	SENSE("on_add") ;
	m_editor->m_is_add = true ;
	show_edit_dialog_for_new_entry( IDS_ADD_ENTRY ) ;
	return 0L ;
}

/** Set the window title.
* Complicated, ain't it?
*/
bool MemoryWindowFrame::set_window_title()
{
	CString title = resource_string(IDS_MEMORY) +
		_T(" [") +
		get_active_mem_name() +
		_T("]") ;

	try
	{
		if (this->is_demo())
		{
			title += _T(" - ") ;
			title += resource_string( IDS_DEMO_VER ) ;
		}
	}
	catch (...) 
	{
		logging::log_error("Failed to retrieve demo status") ;
		ATLASSERT(FALSE && "Error getting demo status") ;
		SetWindowText( title ) ;
		return false ;
	}

	// refresh glossary window title as well
	this->get_glossary_window()->set_window_title() ;

#ifndef UNIT_TEST
	return FALSE != SetWindowText( title ) ;
#else
	return true;
#endif
}


/** Reloads the UI text elements after switching GUI languages.
*/
void MemoryWindowFrame::set_ui_to_current_language()
{
	logging::log_debug("Setting UI to current language") ;
	try
	{
		refresh_command_bar() ;

		set_lang_of_gloss_windows() ;

		refresh_windows() ;

		refresh_view_content();

		// set new window title
		set_window_title() ;

		m_view_interface.set_accel(m_hAccel) ;
		m_view_interface.ensure_document_complete( ) ;
		// query user for color
		set_bg_color( static_cast< COLORREF >( m_props->m_view_props.m_data.m_back_color ) );	

		// give user feedback
		user_feedback( IDS_CHANGED_LANGUAGES ) ;
	}
	catch (CException& e)
	{
		logging::log_error("Failed to set current language") ;
		logging::log_exception(e) ;
	}

}


/** Sets the translation for the current query.
*/
void MemoryWindowFrame::set_translation_at(short index, const wstring translation )
{
	const int CURRENT_INDEX = -1 ;

	if ( m_trans_matches.empty() )
	{
		user_feedback(IDS_OUT_OF_RANGE) ;
		return ;
	}
	search_match_ptr current ;
	if ( index == CURRENT_INDEX )
	{
		current = m_trans_matches.current() ;
	}
	else
	{
		if ( static_cast<size_t>( index ) >= m_trans_matches.size() )
		{
			MessageBeep(MB_ICONEXCLAMATION) ;
			user_feedback(IDS_OUT_OF_RANGE) ;
			return ;
		}
		current = m_trans_matches.at( index ) ;
	}
	current->get_record()->set_trans( translation ) ;
}

/** import a list of tmx files.
*/
bool MemoryWindowFrame::import_tmx(std::vector<CString> import_files, input_device_ptr input)
{
	for(CString filename: import_files)
	{
		import_tmx(filename, input) ;
	}

	return true ;
}

/** import a single tmx file.
*/
bool MemoryWindowFrame::import_tmx( const CString &file_name, input_device_ptr input )
{
	memory_pointer mem = m_model->add_memory() ;
	mem->set_is_memory(true) ;

	CTMXReader reader( mem, static_cast< CProgressListener* >( this ) ) ;

	reader.load_tmx_memory( file_name, input ) ;

	// if we failed to load any entries, remove the memory
	if ( mem->empty() ) 
	{
		m_model->remove_memory_by_id( mem->get_id() ) ;
		user_feedback( get_load_failure_msg(file_name) ) ;
		return false ;
	}

	mem->set_location(file_name) ;

	// give feedback
	feedback_loaded_mem( mem );
	m_mru.AddToList( file_name ) ;
	mem->set_is_memory(true) ;
	mem->set_saved_flag(true) ;

	set_window_title() ;

	if (! reader.m_errors.empty())
	{
		CString msg ;
		msg.Format(_T("%u errors during import. Check log for details."), reader.m_errors.size()) ;
		user_feedback(msg) ;
	}

	return true ;

}

/** Give feedback for a loaded memory.
*/
void MemoryWindowFrame::feedback_loaded_mem( const memory_pointer mem )
{
	const CString file_name = mem->get_location() ;
	CString msg ;
	msg.FormatMessage( IDS_DONE_LOADING, int_arg(mem->size()), file::name( file_name ).file_name() ) ;
	user_feedback( msg ) ;

}
/** Imports a list of Trados memories.
*/
bool MemoryWindowFrame::import_trados(std::vector<CString> import_files)
{
	try
	{
		for(CString filename: import_files)
		{
			import_trados(filename) ;
		}
	}
	catch( CException &e )
	{
		logging::log_exception(e) ;
		user_feedback( IDS_IMPORT_ABORTED ) ;
		e.notify_user( IDS_IMPORT_ABORTED ) ;

		return false ;
	}

	return true ;
}


/** Imports a Trados memory.
*/
bool MemoryWindowFrame::import_trados(const CString &trados_file_name)
{
	const int PROGRESS_START = 0 ;
	const int PROGRESS_END = 100 ;

	trados_data_importer importer( static_cast< CProgressListener* >( this ) ) ;

	user_feedback(IDS_RETRIEVING_LANG_CODES) ;
	CWaitCursor wait_cursor ;

	importer.open_data_source( trados_file_name ) ;

	m_statusbar.init_progress_bar(PROGRESS_START, PROGRESS_END) ;

	// convert them to internal format
	trados_data_importer::language_code_set languages ;

	for ( int num_codes = 0 ; 
		importer.get_language_code( languages ) && num_codes < PROGRESS_END ; 
		++num_codes)
	{
		m_statusbar.set_pos( num_codes++ ) ;
	}

	wait_cursor.Restore() ;
	m_statusbar.m_mp_sbar.ProgDestroyWindow() ;

	CImportDialog import_dialog ;
	import_dialog.set_languages( languages ) ;
	if ( import_dialog.DoModal() == IDCANCEL )
	{
		user_feedback( IDS_IMPORT_ABORTED ) ;
		return false ;
	}

	importer.set_source_language( import_dialog.get_source_plain() ) ;
	importer.set_target_language( import_dialog.get_trans_plain() ) ;

	memory_pointer mem = m_model->add_memory() ;
	mem->set_is_memory(true) ;

	MemoryInfo *mem_info = mem->get_memory_info() ;
	mem_info->set_creation_tool( L"TradosText" ) ;
	mem_info->set_creation_tool_version( L"6.0" ) ;

	ATLASSERT ( mem->get_memory_info()->get_creation_tool() == L"TradosText" ) ; 
	ATLASSERT ( mem->get_memory_info()->get_creation_tool_version() == L"6.0" ) ; 

	if ( ! importer.load( trados_file_name, mem ) )
	{
		user_feedback( IDS_MSG_PARSE_FAILED ) ;
		return false ;
	}
	else
	{
		m_mru.AddToList(  trados_file_name ) ;

		CString int_arg ;
		int_arg.Format( _T("%u"), mem->size() ) ;
		CString msg ;
		msg.FormatMessage( IDS_DONE_LOADING, int_arg, file::name( trados_file_name ).file_name() ) ;
		user_feedback( msg ) ;
		mem->set_location( trados_file_name ) ;
		mem->set_is_memory(true) ;
		mem->set_saved_flag(true) ;
		set_window_title() ;
	}

	return true ;
}


/** Loads a memory.
*/
bool MemoryWindowFrame::load(const CString file_name, const bool check_empty )
{
	input_device_ptr input = get_input_device() ;
	input->ensure_file_exists(file_name);

	// put message in status bar
	loading_file_feedback(file_name);

	const file::CFileExtension ext = file_name ;

	bool success = false ;
	if ( ext.equals( _T(".txt") ) )
	{
		success = import_trados( file_name ) ;
	}
	else if ( ext.equals( _T(".tmx") ) )
	{
		success = import_tmx( file_name, input ) ;
	}
	else  
	{
		success = load_felix_memory(check_empty, file_name);
	}

	if ( success ) 
	{
		set_window_title() ;
		m_mru.AddToList( file_name ) ;
		m_props->m_history_props.m_memory_location = 
			static_cast<LPCWSTR>(file_name) ;
		return true ;
	}
	else
	{
		load_failed_feedback(file_name);
		return false ;
	}


}

/** Looks up a translation in the memory.
*/
bool MemoryWindowFrame::lookup_trans(const wstring query)
{
	if ( query.empty() )
	{
		user_feedback( IDS_EMPTY_QUERY ) ;
		::MessageBeep( MB_ICONINFORMATION ) ;
		return false ;
	}

	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	init_trans_matches_for_lookup(query) ;

	trans_match_container matches ;
	m_model->find_trans_matches( matches, m_trans_matches.m_params ) ;

	m_trans_matches.set_matches( matches ) ;

	if ( m_trans_matches.empty() == false )
	{
		m_model->set_reverse_lookup(true)  ;
		look_up_current_source_in_gloss() ;
	}

	// give the user feedback
	provide_user_trans_feedback() ;

	set_display_state ( MATCH_DISPLAY_STATE ) ;
	show_view_content() ;

	return true ;
}


/** Correct the translation for the current match.
*
* \todo
* Correct any glossary entries with matching source/translation
* - Prompt user?
*/
bool MemoryWindowFrame::correct_trans(const wstring trans)
{
	if ( m_model->empty() )
	{
		user_feedback(IDS_NO_QUERIES) ;
		return false ;
	}
	if (trans.empty())
	{
		user_feedback(IDS_EMPTY_QUERY) ;
		return false ;
	}

	try
	{
		if (get_display_state() == TRANS_REVIEW_STATE)
		{
			memory_pointer mem = m_model->get_memory_by_id(m_review_match->get_memory_id()) ;
			record_pointer rec = m_review_match->get_record() ;
			mem->erase( rec ) ;
			rec->set_trans( trans ) ;
			mem->add_record( rec ) ;
			show_view_content() ;
			user_feedback( IDS_CORRECTED_TRANS ) ;
			return true ;
		}

		// must not be empty!
		if (m_trans_matches.get_query_rich().empty())
		{
			MessageBeep(MB_ICONEXCLAMATION) ;
			user_feedback(IDS_NO_QUERIES) ;
			return false ;
		}

		if ( get_display_state() == NEW_RECORD_DISPLAY_STATE ) 
		{
			ATLASSERT(m_view_state == &m_view_state_new) ;
			memory_pointer mem = m_model->get_first_memory() ;
			mem->erase( m_new_record ) ;
			m_new_record->set_trans( trans ) ;
			mem->add_record( m_new_record ) ;
			show_view_content() ;
			user_feedback( IDS_CORRECTED_TRANS ) ;
			return true ;
		}

		if (m_trans_matches.empty())
		{
			MessageBeep(MB_ICONEXCLAMATION) ;
			user_feedback(IDS_NO_QUERIES) ;
			return false ;
		}

		// the match
		search_match_ptr current = m_trans_matches.current() ;
		memory_pointer mem = m_model->get_memory_by_id(current->get_memory_id()) ;
		// the record
		record_pointer record = current->get_record() ;
		mem->erase( record ) ;

		record->set_trans( trans ) ;
		record->set_refcount( 0 ) ;
		record->increment_refcount() ;
		mem->add_record( record ) ;

		current->set_record(record);
		current->set_values_to_record() ;
		m_trans_matches.set_query(trans) ;

		// show it!
		set_display_state( MATCH_DISPLAY_STATE ) ;
		user_feedback( IDS_CORRECTED_TRANS ) ;
		show_view_content() ;
		return true ;
	}
	catch( CException &e )
	{
		logging::log_error("Failed to correct translation") ;
		logging::log_exception(e) ;
		user_feedback(IDS_TRANS_SET_FAILED) ;
		e.notify_user( IDS_TRANS_SET_FAILED ) ; // TEXT("Failed to set translation.") ) ;
		return false ;
	}

}

/** Translation concordance.
*/
bool MemoryWindowFrame::get_translation_concordances(const wstring query_string)
{
	// an empty string would retrieve everything - probably not what the user wants!
	if ( query_string.empty() )
	{
		user_feedback(IDS_EMPTY_QUERY) ;
		::MessageBeep( MB_ICONINFORMATION ) ;
		return false ;
	}

	// only do searching when edit mode is off
	m_view_interface.put_edit_mode( false ) ;

	// this will hold our matches
	m_search_matches.clear() ;
	m_search_matches.set_trans(query_string) ;

	m_search_matches.m_params.m_ignore_case = true ;

	perform_concordance_search(m_search_matches.m_params) ;

	// remember where we were
	// in the future, make an array of states to allow Explorer-style page navigation
	set_display_state ( CONCORDANCE_DISPLAY_STATE ) ;
	show_view_content() ;
	m_view_interface.set_scroll_pos(0) ;

	// give the user feedback
	translation_concordance_feedback();
	return true ; 
}


/** Alt + C.
* Get concordance in the source fields.
*/
LRESULT MemoryWindowFrame::on_source_concordance(WindowsMessage &)
{
	SENSE("on_source_concordance") ;

	get_concordances(m_view_interface.get_selection_text()) ;
	return 0 ;
}

/** Ctrl + Alt + C.
* Get concordance in the translation fields.
*/
LRESULT MemoryWindowFrame::on_trans_concordance(WindowsMessage &)
{
	SENSE("on_trans_concordance") ;

	get_translation_concordances(m_view_interface.get_selection_text()) ;
	return 0 ;
}

/** Toggle markup (matches) between on and off.
*/
LRESULT MemoryWindowFrame::on_user_toggle_markup(WindowsMessage &)
{
	SENSE("on_user_toggle_markup") ;

	const BOOL show_markup = m_props->m_gen_props.m_data.m_show_markup ;
	if (show_markup)
	{
		put_show_marking( VARIANT_FALSE ) ;
	}
	else
	{
		put_show_marking( VARIANT_TRUE ) ;
	}
	m_props->m_gen_props.m_data.m_show_markup = ! show_markup ;

	m_props->write_to_registry() ;

	return 0L ;
}

/** File -> Save.
* The user wants to save the top memory.
*/
LRESULT MemoryWindowFrame::on_user_save(WindowsMessage &message)
{
	SENSE("on_user_save") ;

	on_file_save(message) ;
	m_glossary_windows.on_file_save() ;
	return 0L ;
}

/** Respond to dropped files.
*/
LRESULT MemoryWindowFrame::on_drop(WindowsMessage &message)
{
	HDROP dropped = (HDROP)message.wParam ;
	SENSE("on_drop") ;

	if ( ! dropped_in_client( dropped ) ) 
	{
		SetMsgHandled( FALSE ) ;
		return 0L ;
	}

	const CDrop drop( dropped ) ;

	const UINT num_files = drop.NumDragFiles() ;

	if ( ! num_files ) 
	{
#ifndef UNIT_TEST
		SetMsgHandled( FALSE ) ;
#endif		
		return 0L ;
	}

	for ( UINT current_file=0 ; current_file < num_files ; ++current_file )
	{
		const CString filename = drop.DragQueryFile( current_file ) ;
		const file::CFileExtension ext = filename ;
		if (ext.equals(".fprefs"))
		{
			this->load_old_preferences(filename) ;
			return 0L ;
		}
		else if(ext.equals(".fprefx"))
		{
			const WORD old_language = m_appstate.m_preferred_gui_lang ;
			load_new_preferences(filename, old_language);
			return 0L ;
		}
		load(filename) ;
	}

	return 0L ;
}

/** Tools -> Manage Memories.
* Shows memory manager dialog.
*/
LRESULT MemoryWindowFrame::on_tools_memory_manager(WindowsMessage &)
{
	SENSE("on_tools_memory_manager") ;

	if (m_props->m_gen_props.m_data.m_old_mem_mgr)
	{
		m_old_manager_window.set_memories(this->m_model->get_memories()) ;
		m_old_manager_window.DoModal() ;
		return 0L ;
	}

	m_manager_window.set_mem_model( this->get_model() ) ;
	m_manager_window.set_gloss_model(this->get_glossary_window()->get_model()) ;

	if (! m_manager_window.IsWindow())
	{
		m_manager_window.Create(*this) ;
	}

	m_manager_window.ShowWindow(SW_SHOW) ;
	m_manager_window.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE) ;

	return 0L ;
}


LRESULT MemoryWindowFrame::on_toggle_views(WindowsMessage &)
{
	logging::log_debug("Toggling view between match and concordance") ;

	if (this->get_display_state() == MATCH_DISPLAY_STATE)
	{
		this->set_display_state(CONCORDANCE_DISPLAY_STATE) ;
	}
	else
	{
		this->set_display_state(MATCH_DISPLAY_STATE) ;
		// the current match...
		if ( ! m_trans_matches.empty() )
		{
			recalculate_match(m_trans_matches.current(), m_trans_matches.m_params);
		}
		else // we have 0 matches, just a query
		{
			trans_match_container matches ;
			if (m_model->is_reverse_lookup())
			{
				m_model->find_trans_matches( matches, m_trans_matches.m_params ) ;
			}
			else
			{
				get_matches(matches, m_trans_matches.m_params);
			}
			m_trans_matches.set_matches(matches) ;
		}
	}
	this->show_view_content() ;
	return 0L ;
}


/** Switches focus to the glossary window.
*/
LRESULT MemoryWindowFrame::on_view_switch(WindowsMessage &)
{
	SENSE("on_view_switch") ;

	if (m_glossary_windows.empty())
	{
		return 0L ;
	}

	m_glossary_windows.first()->SetFocus() ;

	return 0L ;
}

/** Loop through the glossary views (which has focus).
*/
void MemoryWindowFrame::gloss_view_switch(HWND child)
{
	if (! m_glossary_windows.gloss_view_switch(child))
	{
		SetFocus() ;
	}
}

/** View -> Compact View.
* Hide the min view.
*/
LRESULT MemoryWindowFrame::on_user_view_min_end(WindowsMessage &)
{
	SENSE("on_user_view_min_end") ;

	ShowWindow( SW_SHOW ) ;

	m_glossary_windows.put_visibility(SW_SHOW); 

	show_view_content() ;

	return 0L ;
}

/** View -> Compact View.
* Show the min view.
*/
LRESULT MemoryWindowFrame::on_view_min_begin( WindowsMessage &)
{
	const int DEFAULT_SIZE = 100 ;

	SENSE("on_view_min_begin") ;

	m_glossary_windows.put_visibility(SW_HIDE); 

	ShowWindow( SW_HIDE ) ;

	if( ! m_min_view.IsWindow() )
	{
		m_min_view.set_parent(*this) ;
		m_min_view.Create(*this) ;
	}

	ATLASSERT( m_min_view.IsWindow() ) ;

	CRect rc(DEFAULT_SIZE, DEFAULT_SIZE, DEFAULT_SIZE, DEFAULT_SIZE) ;
	m_min_view.ShowWindow( SW_RESTORE ) ;
	m_min_view.SetWindowPos( HWND_TOPMOST, &rc, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOSIZE ) ;

	ATLASSERT( m_min_view.IsWindowVisible() ) ;

	return 0L ;
}


/** Sets whether the window is visible.
* Also applied to all glossary windows.
*/
void MemoryWindowFrame::put_visible(int visibility)
{
	if ( m_min_view.IsWindow() && m_min_view.IsWindowVisible() )
	{
		m_min_view.ShowWindow( visibility ) ;
	}
	else
	{
		ShowWindow( visibility ) ;
	}

	m_glossary_windows.put_visibility(visibility) ;
}

/** Load the appropriate resource file according to the 
* language code.
*/
void MemoryWindowFrame::SetUILanguage(WORD lang_id)
{
	logging::log_debug((format("Setting language id to %1%") % lang_id).str()) ;
	set_module_library(lang_id);

	if (m_appstate.m_preferred_gui_lang != lang_id)
	{
		m_appstate.m_preferred_gui_lang = lang_id ;
		m_props->m_gen_props.m_data.m_preferred_gui_lang = lang_id ;
		m_appstate.write_to_registry() ;
		m_props->m_gen_props.write_to_registry() ;
	}

	if ( m_min_view.IsWindow() )
	{
		m_min_view.DestroyWindow() ;
	}

	set_ui_to_current_language() ;
}

/** As a WORD (LANG_ENGLISH/LANG_JAPANESE).
*/
WORD MemoryWindowFrame::get_current_gui_language()
{
	return m_appstate.m_preferred_gui_lang ;
}

/** Called after the preference dialog is exited with "OK".
*/
void MemoryWindowFrame::reflect_preferences()
{
	m_glossary_windows.show_view_content() ;

	set_bg_color( static_cast< COLORREF >( m_props->m_view_props.m_data.m_back_color ) );	
	m_trans_matches.m_query_color =	m_props->m_view_props.m_data.m_query_color  ;
	m_trans_matches.m_source_color = m_props->m_view_props.m_data.m_source_color  ;
	m_trans_matches.m_trans_color =	m_props->m_view_props.m_data.m_trans_color ;
}

void MemoryWindowFrame::reflect_loaded_preferences( const WORD old_language )
{
	set_up_ui_state() ;

	// set the title
	set_window_title() ;

	set_up_window_size() ;

	init_item_colors();

	set_bg_color( static_cast< COLORREF >( m_props->m_view_props.m_data.m_back_color ) ) ;

	load_history() ;
	load_mousewheel_setting() ;

	if (old_language != m_appstate.m_preferred_gui_lang)
	{
		switch( m_appstate.m_preferred_gui_lang )
		{
		case LANG_JAPANESE :
			SetUILanguage( LANG_JAPANESE ) ;
			break ;
		default:
			SetUILanguage( LANG_ENGLISH ) ;
		}
	}

	if (! m_glossary_windows.empty())
	{
		gloss_window_pointer gloss = m_glossary_windows.first();
		CWindowSettings ws;

		if( ws.Load( resource_string(IDS_REG_KEY), _T("MainGlossary") ) )
		{
			ws.ApplyTo( *gloss ) ;
		}
		else
		{
			gloss->set_up_initial_size() ;
		}
		gloss->load_history() ;
		gloss->load_mousewheel_setting() ;
		gloss->reflect_sb_vis() ;
		gloss->reflect_tb_vis();
		gloss->apply_reg_bg_color() ;
		gloss->apply_mousewheel_setting() ;
	}

	// ===============
	logging::log_debug("Loaded preferences") ;
	user_feedback( IDS_PREFS_LOADED ) ;
}

/** This is called with a PostMessage from OnCreate.
*/
LRESULT MemoryWindowFrame::on_startup_checks(WindowsMessage &)
{
	SENSE("on_startup_checks") ;
	logging::log_debug("Doing post-launch checks") ;

#ifdef UNIT_TEST
	return 0L ;
#else

	check_mousewheel_count();

	if ( ! IsWindowVisible())
	{
		return 0L ;
	}

	// only do this if it is not the first launch
	if (! m_props->m_gen_props.is_first_launch())
	{
		try
		{
			logging::log_debug("Checking for updates") ;
			CDispatchWrapper utilities(L"Felix.Utilities") ;
			CComBSTR langcode ;
			langcode.LoadStringW(IDS_LANG_CODE) ;
			CComVariant arg1(langcode) ;
			utilities.method(L"CheckUpdates", arg1) ;
		}
		catch (_com_error& e)
		{
			logging::log_error("Failed to auto check for updates") ;
			logging::log_exception(e) ;
		}
		return 0L ;
	}

	// We should have returned if it's not the first launch.
	ATLASSERT(m_props->m_gen_props.is_first_launch()) ;
	SetFocus() ;

	set_window_title() ;
	SetFocus() ;

	return 0L ;
#endif
}

/** Adds a memory.
*/
void MemoryWindowFrame::add_memory(memory_pointer mem)
{
	m_model->insert_memory( mem ) ;

	m_view_interface.set_text( wstring() ) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;

	set_window_title() ;
}

/** If we don't have a size saved, use the default.
*/
void MemoryWindowFrame::set_up_default_initial_size()
{
	const int PADDING = 5 ;

	// get dimensions of desktop
	const CWindowRect desktop_rect(GetDesktopWindow()) ;
	// dimensions of our top glossary window
	const CWindowRect dialog_rect(get_glossary_window()->m_hWnd) ;

	// get dimensions of the mainframe
	CWindowRect frame_window_rect(*this) ;

	// calculate dialog size
	const int width = ( desktop_rect.Width() ) - (dialog_rect.Width() + PADDING) ;
	const int height = dialog_rect.Height() ;
	frame_window_rect.right = PADDING + desktop_rect.left + width ;
	frame_window_rect.left = PADDING ;
	frame_window_rect.top = desktop_rect.top ;
	frame_window_rect.bottom = frame_window_rect.top + height ;

	// justify window left, place it under parent
	ATLVERIFY(MoveWindow( &frame_window_rect, TRUE )) ;
}

/** Add our tool bars and menu.
*/
void MemoryWindowFrame::set_up_command_bars()
{
	// Set the 24-bit color images in the toolbar's image list
	// SEP_ID is for separators.
	std::vector< int > commands ;
	add_common_tb_commands(commands) ;
	commands += 
		ID_TOOLS_PREFERENCES,	ID_TOOLS_MEMORY_MGR, SEP_ID,
		ID_HELP,		ID_APP_ABOUT;
	std::vector< int > StdBitmaps ;
	add_common_std_bitmaps(StdBitmaps) ;
	StdBitmaps += 
		IDB_PROPERTIES,		IDB_MEM_MGR,
		IDB_HELP,			IDB_INFORMATION ;


#ifdef UNIT_TEST
	return ;
#else
	CWindow toolbarWnd = m_stdToolbar.Create24BitToolBarCtrl(*this, commands, FALSE );

	m_stdToolbar.SubclassWindow( toolbarWnd, MAKEINTRESOURCE(IDR_MAINFRAME));
	m_stdToolbar.SetBitmapSize(BM_SIZE, BM_SIZE) ;

	CImageList images ;
	create_tb_imagelist(images, StdBitmaps) ;
	m_stdToolbar.SetImageList(images) ;

	create_command_bar();

	// remove old menu
	SetMenu(NULL);

	// Add menu bitmaps
	AddMenuBitmap(IDB_NEW_DOCUMENT, ID_FILE_NEW) ;
	AddMenuBitmap(IDB_NETWORK, ID_FILE_CONNECT) ;
	AddMenuBitmap(IDB_OPEN, ID_FILE_OPEN) ;
	AddMenuBitmap(IDB_SAVE, ID_FILE_SAVE) ;
	AddMenuBitmap(IDB_SAVEMANY, ID_FILE_SAVE_ALL) ;
	AddMenuBitmap(IDB_MEMORY_CLOSE, ID_MEMORY_CLOSE) ;
	AddMenuBitmap(IDB_CUT, ID_EDIT_CUT) ;
	AddMenuBitmap(IDB_COPY, ID_EDIT_COPY) ;
	AddMenuBitmap(IDB_PASTE, ID_EDIT_PASTE) ;
	AddMenuBitmap(IDB_SEARCH, ID_EDIT_FIND) ;

	AddMenuBitmap(IDB_SAVE_PREFS, ID_TOOLS_SAVEPREFERENCES) ;
	AddMenuBitmap(IDB_OPEN_PREFS, ID_TOOLS_LOADPREFERENCES) ;

	AddMenuBitmap(IDB_INFORMATION, ID_APP_ABOUT) ;
	AddMenuBitmap(IDB_HELP, ID_HELP) ;
	AddMenuBitmap(IDB_MEM_MGR, ID_TOOLS_MEMORY_MGR) ;
	AddMenuBitmap(IDB_PROPERTIES, ID_TOOLS_PREFERENCES) ;
	AddMenuBitmap(IDB_FONT, ID_FORMAT_FONT) ;
	AddMenuBitmap(IDB_FILL_COLOR, ID_FORMAT_BGCOLOR) ;
	AddMenuBitmap(IDB_DELETE, ID_EDIT_DELETE) ;

	// Create the rebar, and add the menu and toolbar to it.
	ATLVERIFY(CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE)) ;
	AddSimpleReBarBand(m_CmdBar);
	AddSimpleReBarBand(m_stdToolbar, NULL, m_appstate.m_rebar_has_linebreak);
	SizeSimpleReBarBands() ;
	ATLVERIFY(UIAddToolBar(m_stdToolbar)) ;
#endif
}
void MemoryWindowFrame::create_command_bar()
{
	// create command bar window
	m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	m_CmdBar.RemoveAllImages() ;
	m_CmdBar.SetImageMaskColor(MAGENTA) ;
	m_CmdBar.SetImageSize(BM_SIZE, BM_SIZE) ;

	// attach menu
	ATLVERIFY(m_CmdBar.AttachMenu(GetMenu())) ;
}
//! Add a bitmap menu item.
void MemoryWindowFrame::AddMenuBitmap( const int BitmapId, const int CmdId ) 
{
	CBitmap bmp ;
	bmp.LoadBitmap( BitmapId ) ;
	m_CmdBar.AddBitmap( bmp, CmdId  ) ;
}

/** Load the MRU.
*/
void MemoryWindowFrame::set_up_recent_docs_list()
{
	const int MAX_NUM_ENTRIES = 15 ;
	const int MAX_ITEM_LEN = 400 ; // pixels

	// init recent documents list
	m_mru.SetMenuHandle(::GetSubMenu(GetMenu(), 0 ) ) ;
	m_mru.ReadFromRegistry( resource_string( IDS_REG_KEY ) ) ;
	m_mru.SetMaxItemLength(MAX_ITEM_LEN) ;
	m_mru.SetMaxEntries(MAX_NUM_ENTRIES) ;
}

/** Create the status bar.
*/
void MemoryWindowFrame::init_status_bar()
{
#ifdef UNIT_TEST
	return ;
#else
	// create the status bar
	if(! CreateSimpleStatusBar())
	{
		logging::log_error("Failed to create status bar in main window.") ;
	}
	ATLASSERT( ::IsWindow(m_hWndStatusBar) ) ;

	ATLVERIFY(m_statusbar.m_mp_sbar.SubclassWindow( m_hWndStatusBar )) ;
	ATLASSERT( m_statusbar.m_mp_sbar.IsWindow() ) ;

	int arrParts[] = 
	{ 
		ID_DEFAULT_PANE, 
		ID_PANE_1,
		ID_PANE_2
	} ;

	ATLVERIFY(m_statusbar.m_mp_sbar.SetPanes(arrParts, sizeof(arrParts) / sizeof(arrParts[0]), false)) ;
	CString version_info ;
	version_info.Format(_T("Felix v. %s"), string2wstring(VERSION).c_str()) ;
	user_feedback(version_info, 1) ;
#endif
}

/** Set up the various menu checkmarks and such.
*/
void MemoryWindowFrame::set_up_ui_state()
{
	ATLASSERT( ::IsWindow( m_hWndToolBar ) ) ;
	CReBarCtrl rebar = m_hWndToolBar ;
	const int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1) ;	// toolbar is 2nd added band

	// toolbar
	if( m_appstate.m_is_toolbar_visible )
	{
		ATLVERIFY(UISetCheck(ID_VIEW_TOOLBAR, TRUE)) ;
		ATLVERIFY(rebar.ShowBand(nBandIndex, TRUE)) ;
	}
	else
	{
		ATLVERIFY(UISetCheck(ID_VIEW_TOOLBAR, FALSE)) ;
		ATLVERIFY(rebar.ShowBand(nBandIndex, FALSE)) ;
	}

	// status bar
	if( m_appstate.m_is_statusbar_visible )
	{
		::ShowWindow(m_hWndStatusBar, SW_SHOW) ;
		ATLVERIFY(UISetCheck(ID_VIEW_STATUS_BAR, TRUE)) ;
	}
	else
	{
		::ShowWindow(m_hWndStatusBar, SW_HIDE) ;
		ATLVERIFY(UISetCheck(ID_VIEW_STATUS_BAR, FALSE)) ;
	}

}

/** Set up the mainframe window size.

If we have already run the app, we will have persisted the app state in the registry.
If not, we will set the window size to the default.

CReBarSettings is used to save/load the command bar settings in the registry.
*/
void MemoryWindowFrame::set_up_window_size()
{
#ifdef UNIT_TEST
	return ;
#else

	try
	{
		CWindowSettings ws;

		if( ws.Load( resource_string(IDS_REG_KEY), _T("MainFrame") ) )
		{
			ws.ApplyTo(*this) ;
		}
		else
		{
			set_up_default_initial_size() ;
		}

		CReBarSettings rs;
		if(rs.Load( resource_string(IDS_REG_KEY), _T("ReBar") ) )
		{
			CReBarCtrl rbc = m_hWndToolBar ;
			rs.ApplyTo(rbc);
		}
	}
	catch (CException& e)
	{
		logging::log_error("Failed to initialize Window size") ;
		logging::log_exception(e) ;
	}

#endif
}

/** Add the record to the top memory, and give feedback.
*/
void MemoryWindowFrame::add_record_to_memory(record_pointer record)
{
	memory_pointer mem = m_model->get_first_memory() ;

	if ( ! mem->add_record( record ) )
	{
		if ( mem->is_locked() )
		{
			user_feedback( IDS_MEMORY_LOCKED ) ;
		}
		else
		{
			// Entry already existed. Updated with any new information. 
			user_feedback( IDS_ENTRY_EXISTED ) ; 
		}
		return ;
	}

	CNumberFmt fm ;
	CString msg ;
	msg.FormatMessage( IDS_CURRENT_SIZE, resource_string( IDS_MEMORY ), fm.Format( mem->size() ) ) ;

	const CString content = resource_string( IDS_ADDED_TRANSLATION ) + _T(" ") + msg ;
	user_feedback( content ) ;
}

/** Perform the lookup again. This is needed after we've edited the record.
*/
void MemoryWindowFrame::redo_lookup( search_match_ptr match, bool do_gloss )
{
	match_maker matcher( 0.0 ) ; // show score no matter what

	g_cmp_maker.m_ignore_case = m_trans_matches.m_params.m_ignore_case ;
	g_cmp_maker.m_ignore_hira_kata = m_trans_matches.m_params.m_ignore_hira_kata ;
	g_cmp_maker.m_ignore_width = m_trans_matches.m_params.m_ignore_width ;

	const wstring search_segment = m_trans_matches.get_query_rich();

	const Segment query(&g_cmp_maker, search_segment) ;
	record_pointer rec = match->get_record() ;

	int match_algo = m_trans_matches.m_params.m_match_algo ;
	if ( match_algo == IDC_ALGO_AUTO )
	{
		match_algo = detect_match_algo(query.cmp()) ;
	}

	if ( m_model->is_reverse_lookup() )
	{
		// look up based on translation
		const Segment source(&g_cmp_maker, rec->get_trans_rich()) ;
		matcher.get_score_trans(query, 
			source, 
			match_algo,
			match) ;
	}
	else
	{
		// look up normally (using source)
		const Segment source(&g_cmp_maker, rec->get_source_rich()) ;
		matcher.get_score(query, 
			source, 
			match_algo,
			match) ;
	}

	if ( do_gloss ) 
	{
		m_glossary_windows.look_up(search_segment) ;
	}
}

/** Tell the user how many matches we found.
*/
void MemoryWindowFrame::provide_user_search_feedback()
{
	match_count_feedback(m_search_matches.size()) ;
}

/** Provides feedback on found matches.
*/
void MemoryWindowFrame::provide_user_trans_feedback()
{
	match_count_feedback(m_trans_matches.size()) ;
}

/** Template: Found x match(es).
*/
void MemoryWindowFrame::match_count_feedback(size_t num)
{
	if ( num == 1 )
	{
		user_feedback(IDS_FOUND_1_MATCH) ;
	}
	else
	{
		user_feedback(system_message(IDS_FOUND_X_MATCHES, int_arg(num))) ;
	}
}

/** Perform a search on the memories with the parameters from the find
* dialog.
*/
void MemoryWindowFrame::perform_user_search()
{
	m_search_matches.clear() ;
	m_search_matches.m_params = m_find.get_search_params() ;

	perform_concordance_search(m_search_matches.m_params) ;
}
/** This is kind of a useless method, isn't it?
*/
void MemoryWindowFrame::show_user_search_results()
{
	set_display_state ( CONCORDANCE_DISPLAY_STATE );
	show_view_content() ;
	m_view_interface.set_scroll_pos(0) ;
}

/** Refresh the glossary window text after switching GUI languages.
*/
void MemoryWindowFrame::set_lang_of_gloss_windows()
{
	m_glossary_windows.set_ui_language() ;
}
/** Reload the menu bar after switching GUI languages.
*/
void MemoryWindowFrame::refresh_command_bar()
{
	HINSTANCE h = _Module.GetResourceInstance() ;
	HMENU menu = ::LoadMenu( h, MAKEINTRESOURCE( IDR_MAINFRAME ) ) ;
	ATLASSERT( menu != NULL ) ;
	m_CmdBar.AttachMenu( menu );

	refresh_mru_doc_list(menu);
}

/** We need to reload the MRU list after switching GUI languages.
*/
void MemoryWindowFrame::refresh_mru_doc_list(HMENU menu)
{
	// write our recent docs
	m_mru.WriteToRegistry( resource_string( IDS_REG_KEY ) );
	// reset the menu handle for the recent docs list...
	m_mru.SetMenuHandle(::GetSubMenu(menu, 0));
	// read our doc history back for the new menu
	m_mru.ReadFromRegistry( resource_string( IDS_REG_KEY ) );
}


/** Allow the glossary windows to tell us not to shut down.
*/
bool MemoryWindowFrame::gloss_win_shutdown_check()
{
	if (m_glossary_windows.pre_shutdown_save_check())
	{
		return true ;
	}
	SetMsgHandled( TRUE ) ;
	return false ;
}

/** Saves the rebar settings to the register, so we can remember them
* at next startup. (We're just nice that way)
*/
void MemoryWindowFrame::save_rebar_settings()
{
	CReBarCtrl rbc = m_hWndToolBar;
	ATLASSERT(rbc.IsWindow()) ;

	CReBarSettings rs;
	rs.GetFrom(rbc);
	rs.Save( resource_string(IDS_REG_KEY), _T("ReBar") );
}

/** Sets whether to show markup for matches.
*/
void MemoryWindowFrame::put_show_marking( const VARIANT_BOOL setting )
{
	if ( setting == VARIANT_FALSE ) 
	{
		m_trans_matches.m_params.m_show_marking = false ;
	}
	else
	{
		m_trans_matches.m_params.m_show_marking = true ;
	}
	m_glossary_windows.put_show_marking(setting) ;

	show_view_content() ;
}

/** Returns whether markup is being shown.
*/
VARIANT_BOOL MemoryWindowFrame::get_show_marking()
{
	if ( m_trans_matches.m_params.m_show_marking == false ) 
	{
		return VARIANT_FALSE ;
	}
	else
	{
		return VARIANT_TRUE ;
	}
}

/** Set the location of the top memory.
*/
bool MemoryWindowFrame::set_location( const CString &location ) 
{
	memory_pointer mem = m_model->get_first_memory() ;
	try
	{
		mem->set_location( location ) ;
		return true ;
	}
	catch( ... )
	{
		logging::log_error("Error while setting memory location. Removing memory.") ;
		m_model->remove_memory_by_id( mem->get_id() ) ;
	}
	return false ;
}

/** Get the type string for the memory window (e.g. "Memory").
*/
CString MemoryWindowFrame::get_window_type_string() 
{
	return resource_string(IDS_MEMORY) ;
}
bool MemoryWindowFrame::is_glossary_window()
{
	return false;
}

//! This is our poor-man's multithreading.
void MemoryWindowFrame::init_background_processor()
{
	m_background_processor.set_accelerator(m_hAccel) ;
	m_background_processor.set_hwnd(*this) ;
}

//! Load color preferences from registry.
void MemoryWindowFrame::init_item_colors()
{
	app_props::properties_view::props_data *prop_data = &m_props->m_view_props.m_data ;

	m_trans_matches.m_query_color = (COLORREF)prop_data->m_query_color ;
	m_trans_matches.m_source_color = (COLORREF)prop_data->m_source_color ;
	m_trans_matches.m_trans_color = (COLORREF)prop_data->m_trans_color ;
}

//! See if we want to save our memory/glossary loaded history.
BOOL MemoryWindowFrame::should_save_memory_history()
{
	return m_props->m_gen_props.load_prev_mem_on_startup();
}

//! Respond to italic command.
LRESULT MemoryWindowFrame::on_italic(WindowsMessage &)
{
	SENSE("on_italic") ;
	return CCommonWindowFunctionality::on_italic( ) ;
}

//! Respond to an underline command.
LRESULT MemoryWindowFrame::on_underline(WindowsMessage &)
{
	SENSE("on_underline") ;
	return CCommonWindowFunctionality::on_underline( ) ;
}

//! Respond to bold command.
LRESULT MemoryWindowFrame::on_bold(WindowsMessage &)
{
	SENSE("on_bold") ;
	return CCommonWindowFunctionality::on_bold( ) ;
}

//! Save all open memory files.
LRESULT MemoryWindowFrame::on_file_save_all(WindowsMessage &)
{
	SENSE("on_file_save_all") ;
	return CCommonWindowFunctionality::on_file_save_all( ) ;
}

//! Replace the record with the one beind edited.
LRESULT MemoryWindowFrame::on_user_replace_edit_record(WindowsMessage &)
{
	SENSE("on_user_replace_edit_record") ;
	return 0L ;
}

//! Make sure that we haven't exceeded the demo constraints.
LRESULT MemoryWindowFrame::on_demo_check_excess_memories(WindowsMessage &)
{
	SENSE("on_demo_check_excess_memories") ;
	return CCommonWindowFunctionality::on_demo_check_excess_memories( ) ;
}

//! User wants to search in edit mode.
LRESULT MemoryWindowFrame::on_user_edit_search(WindowsMessage &message)
{
	SENSE("on_user_edit_search") ;
	return CCommonWindowFunctionality::on_user_edit_search( message.lParam ) ;
}

/* 
* Get the appropriate record as basis for registering glossary entries.
* If the index is out of range, then just return an empty record. 
* The user should be able to register glossary entries without a template record.
*/
mem_engine::record_pointer MemoryWindowFrame::get_reg_gloss_record( const size_t num )
{
	return m_view_state->get_specified_record(num) ;
}

//! Open a file from the MRU list
LRESULT MemoryWindowFrame::on_mru_file_open(WindowsMessage &message)
{
	SENSE("on_mru_file_open") ;

	const int index = static_cast<int>(LOWORD( message.wParam )) ;
	CString fname ;
	m_mru.GetFromList(index, fname) ;

	try
	{
		if (! this->load(fname))
		{
			m_mru.RemoveFromList( index ) ;
		}
	}
	catch (...)
	{
		logging::log_error("Error removing item from MRU list") ;
#ifndef UNIT_TEST
		m_mru.RemoveFromList( index ) ;
#endif		
		throw ;
	}
	return 0L;
}


//! We got rid of the WTL switch statement style to add flexibility
BOOL MemoryWindowFrame::ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD /*dwMsgMapID*/ /*= 0*/ )
{
	try
	{
		BOOL bHandled = FALSE ;
		COMMAND_ID_HANDLER_EX(ID_FILE_CONNECT, on_file_connect)
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFOW, OnToolTipTextW)
		MESSAGE_HANDLER_EX(WM_MOUSEWHEEL /* 0x020A */, OnMouseWheel)

		const messageMapType *theMessageMap = this->get_message_map( uMsg ) ;
		const UINT key = this->get_message_key( uMsg, wParam ) ;

		messageMapType::const_iterator pos = theMessageMap->find( key ) ;
		if ( pos != theMessageMap->end() )
		{
			SENSE("Found message key") ;

			WindowsMessage message( hWnd, uMsg, wParam, lParam ) ;
			lResult = (pos->second)( message ) ;
			if ( message.isHandled() )
			{
				return TRUE ;
			}
		}

		// chain to members
		if( m_view_interface.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult) ) 
		{
			return TRUE; 
		}

		CHAIN_MSG_MAP(CUpdateUI<MemoryWindowFrame>) ;

#ifndef UNIT_TEST
		CHAIN_MSG_MAP(frame_class) ;
#endif
	}
	catch ( CSWException &sw_e ) 
	{ 
		string err_msg = "Felix Window - Structured Windows Exception" ;

		report_structured_windows_exception(err_msg, sw_e) ;
		return FALSE ;
	} 
	catch ( _com_error &e ) 
	{ 
		logging::log_error("COM Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": COM ERROR") ;
		sb_text(m_hWndStatusBar, fail_msg ) ; 
		return handle_exception( e, fail_msg ) ; 
	} 
	catch ( CComException &e ) 
	{ 
		logging::log_error("Application COM Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": COM EXCEPTION") ;
		sb_text(m_hWndStatusBar, fail_msg ) ; 
		return handle_exception( e, fail_msg ) ;
	} 
	catch ( CWinException &e ) 
	{ 
		logging::log_error("Windows Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": WINDOWS ERROR") ;
		sb_text(m_hWndStatusBar, fail_msg ) ; 
		return handle_exception( e, fail_msg ) ;
	} 
	catch ( CException &e ) 
	{ 
		logging::log_error("Application Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": EXCEPTION") ;
		sb_text(m_hWndStatusBar, fail_msg ) ; 
		return handle_exception( e, fail_msg ) ; 
	} 
	catch ( std::exception &e ) 
	{ 
		logging::log_error("C++ Library Exception") ;
		logging::log_error(e.what()) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": RUNTIME ERROR") ;
		sb_text(m_hWndStatusBar, fail_msg ) ; 
		return handle_exception( e, fail_msg ) ; 
	}

	return FALSE;
}

//! Get the correct key to use with the message map
UINT MemoryWindowFrame::get_message_key( UINT message, WPARAM wParam )
{
	if ( message == UWM_USER_MESSAGE )
	{
		return wParam ;
	}
	if ( message == WM_COMMAND )
	{
		return LOWORD( wParam ) ;
	}
	return message ;
}

//! Get the correct message map depending on the message type
std::map< UINT, boost::function< LRESULT( WindowsMessage& ) >  > * MemoryWindowFrame::get_message_map( UINT message )
{
	if ( message == UWM_USER_MESSAGE )
	{
		return &this->m_user_message_map ;
	}
	if ( message == WM_COMMAND )
	{
		return &this->m_command_message_map ;
	}
	return &this->m_message_map ;
}

/** Recalculates the match after switching from another view state.
* This is needed because we might have edited the record.
*/
void MemoryWindowFrame::recalculate_match( search_match_ptr match, search_query_params &params )
{
	// initialize the match
	match->match_pairing().clear() ;
	match->set_values_to_record() ;

	m_trans_matches.m_params = params ;
	this->redo_lookup(match, false) ;
}

//! File -> Connect
LRESULT MemoryWindowFrame::on_file_connect( UINT, int, HWND )
{
	CConnectionDlg dlg(m_props) ;
	if (IDCANCEL == dlg.DoModal(*this))
	{
		return 0L ;
	}

	return add_remote_memory(m_model, dlg.m_memory) ;
}

//! Set the background color unless it's white (the default)
void MemoryWindowFrame::set_bg_color_if_needed()
{
#ifdef UNIT_TEST
	return ;
#else
	const CColorRef color((COLORREF)m_props->m_view_props.m_data.m_back_color) ;
	if (! color.is_white())
	{
		m_view_interface.set_bg_color(color.as_wstring()) ;
	}
#endif
}

/** handle tooltip text ourselves to enable dynamic switching.
* (The default WTL way caches tooltip text)
*/
LRESULT MemoryWindowFrame::OnToolTipTextW( int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	if( m_toolmap.empty() )
	{
		init_tooltip_map(m_toolmap);

		add_frame_specific_tooltips();
	}

	handle_tooltip(pnmh, idCtrl, m_toolmap);

	return 0L;
}

void MemoryWindowFrame::add_frame_specific_tooltips()
{
	m_toolmap[ID_EDIT_FIND] = IDS_SEARCH_TOOLBAR;
	m_toolmap[ID_MEMORY_CLOSE] = IDS_MEMORY_CLOSE;
	m_toolmap[ID_FILE_SAVE] = IDS_SAVE_MEMORY;
	m_toolmap[ID_FILE_SAVE_ALL] = IDS_SAVE_ALL_MEMORIES;
}

//! Tell the user that we failed to load the memory
void MemoryWindowFrame::load_failed_feedback( const CString & file_name )
{
	// we failed to load the memory
	user_feedback( get_load_failure_msg(file_name) ) ; //
}

//! Tell the user that we're loading a file
void MemoryWindowFrame::loading_file_feedback( const CString & file_name )
{
	const file::CPath path( file_name ) ;
	user_feedback(system_message( IDS_MSG_LOADING, path.FindFileName()));
}

/** Loads a Felix memory.
*/
bool MemoryWindowFrame::load_felix_memory( bool check_empty, const CString & file_name )
{
	memory_pointer mem ;
	bool make_dirty = false ;

	// merge or add?
	MERGE_CHOICE should_merge = MERGE_CHOICE_SEPARATE ;
	if ( check_empty )
	{
		should_merge = check_empty_on_load() ;
		if (should_merge == MERGE_CHOICE_CANCEL)
		{
			return true ;
		}
		if (should_merge == MERGE_CHOICE_SEPARATE)
		{
			mem = m_model->add_memory() ;
		}
		else
		{
			ATLASSERT(should_merge == MERGE_CHOICE_MERGE) ;
			mem = m_model->get_first_memory() ;
			make_dirty = true ;
		}
	}
	else
	{
		mem = m_model->add_memory() ;
	}

	mem->set_is_memory(true) ;
	mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

	try
	{
		const bool success = mem->load( file_name )  ;
		if (make_dirty)
		{
			mem->set_saved_flag(false) ;
		}
		return success ;
	}
	catch ( ... ) 
	{
		logging::log_error("Failed to load memory") ;
		if (should_merge == MERGE_CHOICE_SEPARATE)
		{
			mem->set_listener(nullptr);
			m_model->remove_memory_by_id( mem->get_id() ) ;
		}
		throw ;
	}
}

//! Set search params from registry settings.
void MemoryWindowFrame::init_lookup_properties( const app_props::props_ptr source, search_query_params &dest )
{
	dest.m_ignore_case = !! source->m_mem_props.m_data.m_ignore_case ;
	dest.m_ignore_width = !! source->m_mem_props.m_data.m_ignore_width ;
	dest.m_ignore_hira_kata = !! source->m_mem_props.m_data.m_ignore_hir_kat ;

	dest.m_assess_format_penalty = !! source->m_mem_props.m_data.m_assess_format_penalty ;
	dest.m_match_algo = source->m_alg_props.m_data.m_match_algo ;

	dest.m_place_numbers = !! source->m_mem_props.m_data.m_place_numbers ;
	dest.m_place_gloss = !! source->m_mem_props.m_data.m_place_gloss ;
	dest.m_place_rules = !! source->m_mem_props.m_data.m_place_rules ;
	dest.m_show_gloss_matches = !! source->m_view_props.m_data.m_show_gloss_matches;
}

//! Tell the user that we found x matches for the search string.
void MemoryWindowFrame::source_concordance_feedback()
{
	const wstring plain_text = m_search_matches.get_source_plain() ;
	concordance_feedback(plain_text, m_search_matches.size()) ;
}

//! Tell the user that we found translation x matches for the search string.
void MemoryWindowFrame::translation_concordance_feedback()
{
	const wstring plain_text = m_search_matches.get_trans_plain() ;
	concordance_feedback(plain_text, m_search_matches.size()) ;
}

//! Show concordance match count in status bar.
void MemoryWindowFrame::concordance_feedback(const wstring plain_text, size_t num)
{
	user_feedback(system_message(IDS_FOUND_X_MATCHES_FOR_STRING, 
		int_arg(num), 
		plain_text.c_str())) ;
}

//! Tell the user that we deleted a new record.
void MemoryWindowFrame::deleted_new_record_feedback()
{
	const wstring feedback = L"<center><h1>" + resource_string_w( IDS_DELETED_ENTRY ) + L"</h1></center>" ;
	m_view_interface.set_text(feedback) ;
	user_feedback(IDS_DELETED_ENTRY) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;
}


//! See if we can create a placement for this match.
void MemoryWindowFrame::check_placement_numbers( trans_match_container &PlacedMatches, 
								 search_match_ptr match )
{

	record_pointer rec = match->get_record() ;
	const wstring trans = rec->get_trans_plain() ;

	trans_pair Transpair( trans, trans ) ;
	match_string_pairing newPairing( match->match_pairing() ) ;
	number_placer placer ;
	pairings_t &pairings = newPairing.get() ;
	if ( placer.place( pairings, Transpair ) )
	{
		search_match_ptr new_match = create_placement_match(match, Transpair.first);

		placement_score(new_match, pairings, match->get_formatting_penalty());

		// new query/source
		pairing_query_source(new_match, pairings, Transpair.second);
		new_match->match_pairing().set(pairings);

		PlacedMatches.insert( new_match ) ;
	}
}


void MemoryWindowFrame::add_placement_match( search_match_ptr match, trans_pair &trans_segs, pairings_t & pairings, trans_match_container &PlacedMatches )
{
	search_match_ptr new_match = create_placement_match(match, trans_segs.first);

	set_placement_penalty(new_match, pairings, match->get_formatting_penalty());

	wstring newsource = get_new_source(pairings);

	new_match->get_record()->set_source(newsource) ;

	// new query/source
	pairing_query_source(new_match, pairings, trans_segs.second);
	new_match->match_pairing().set(pairings);

	PlacedMatches.insert( new_match ) ;
}
void MemoryWindowFrame::set_placement_penalty( search_match_ptr new_match, pairings_t & pairings, double format_penalty )
{
	const double PLACEMENT_PENALTY = 0.00001 ;
	new_match->set_base_score( calc_score_gloss(pairings) - PLACEMENT_PENALTY) ;
	new_match->set_formatting_penalty( format_penalty ) ;
}

wstring MemoryWindowFrame::get_new_source( pairings_t & pairings )
{
	// This hack obliterates our formatting info
	wstring newsource ;
	FOREACH(pairing_entity entity, pairings)
	{
		if(entity.source())
		{
			newsource += entity.source() ;
		}
	}
	return newsource ;
}

void MemoryWindowFrame::check_placement_gloss( trans_match_container &PlacedMatches, 
	search_match_ptr match )
{

	record_pointer rec = match->get_record() ;
	const wstring trans = rec->get_trans_plain() ;
	wstring after = rec->get_trans_plain() ;

	pairings_t &pairings = match->match_pairing().get() ;

	placement::gloss_placer placer(get_glossary_window()->get_model()->get_memories()->get_memories()) ;
	trans_pair trans_segs( trans, trans ) ;

	hole_pair_t holes ;
	hole_finder finder ;
	if (! finder.find_hole(pairings, holes))
	{
		return ;
	}

	if ( placer.place(pairings, trans_segs, holes) )
	{
		add_placement_match(match, trans_segs, pairings, PlacedMatches);
	}

	return ;
}

void MemoryWindowFrame::check_placement_rules( trans_match_container &PlacedMatches, search_match_ptr match)
{

	record_pointer rec = match->get_record() ;
	const wstring trans = rec->get_trans_plain() ;
	wstring after = rec->get_trans_plain() ;

	pairings_t &pairings = match->match_pairing().get() ;

	placement::rule_placer placer(m_rules) ;
	trans_pair trans_segs( trans, trans ) ;

	hole_pair_t holes ;
	hole_finder finder ;
	if (! finder.find_hole(pairings, holes))
	{
		return ;
	}

	if ( placer.place(pairings, trans_segs, holes) )
	{
		add_placement_match(match, trans_segs, pairings, PlacedMatches);
	}

	return ;
}
mem_engine::search_match_ptr MemoryWindowFrame::create_placement_match( search_match_ptr match, const wstring &trans ) const
{
	search_match_ptr new_match(new search_match) ;
	new_match->set_memory_id(match->get_memory_id()) ;
	new_match->set_memory_location(match->get_memory_location()) ;
	// record
	record_pointer old_rec = match->get_record() ;
	record_pointer new_rec = record_pointer(old_rec->clone()) ;
	new_rec->set_validated_off() ;
	new_rec->reset_refcount() ;
	new_rec->set_trans( trans ) ;

	new_match->set_record( new_rec ) ;
	new_match->set_values_to_record() ;
	return new_match ;
}

void MemoryWindowFrame::pairing_query_source( search_match_ptr new_match, pairings_t &pairings, const wstring after ) const
{
	mem_engine::markup_ptr Markup = new_match->get_markup() ;
	Markup->SetQuery(mark_up(pairings, QUERY)) ;
	Markup->SetSource(mark_up(pairings, SOURCE)) ;
	Markup->SetTrans( after ) ;

	new_match->set_placement_on() ;
}

void MemoryWindowFrame::placement_score( mem_engine::search_match_ptr new_match, mem_engine::placement::pairings_t &pairings, double fmt_penalty ) const
{
	// HACK -- to make sure that the placed matches sort below the non-placed ones
	const double PLACEMENT_PENALTY = 0.00001 ;
	new_match->set_base_score( calc_score(pairings) - PLACEMENT_PENALTY) ;
	new_match->set_formatting_penalty( fmt_penalty ) ;
}


//! Make sure that ShellExecute didn't return an error code.
void MemoryWindowFrame::check_shell_execute_result( int result, 
											const CString & filePath )
{
	const int ERR_THRESHOLD = 32 ;

	if ( result <= ERR_THRESHOLD )
	{
		throw CWinException(system_message(IDS_SHOW_HELP_FAILED, (LPCTSTR)filePath), result ) ;
	}
}

void MemoryWindowFrame::OnNavEdit( long index )
{
	SENSE("OnNavEdit") ;
	WindowsMessage message( NULL, 0, 0, index ) ;
	on_user_edit( message ) ;
}

void MemoryWindowFrame::OnNavDelete( long index )
{
	SENSE("OnNavDelete") ;
	on_user_delete( static_cast<size_t>(index) ) ;
}

//! Register glossary entries based on entry at index.
void MemoryWindowFrame::OnNavAddGloss( long index )
{
	SENSE("OnNavAddGloss") ;
	on_user_register( index ) ;
}

//! Add the entry at index to the glossary.
void MemoryWindowFrame::OnNavAddToGloss( long index )
{
	SENSE("OnNavAddToGloss") ;
	on_user_add_to_glossary( index ) ;
}

void MemoryWindowFrame::remove_record_from_mem_id( record_pointer rec, int mem_id )
{
	try
	{
		m_model->get_memory_by_id(mem_id)->erase( rec ) ;
	}
	catch (CProgramException& e)
	{
		logging::log_error("Failed to delete record") ;
		logging::log_exception(e) ;
		e.notify_user("Failed to delete record: memory not found") ;
	}
}

void MemoryWindowFrame::remove_match_record( search_match_ptr match )
{
	record_pointer rec = match->get_record() ;

	remove_record_from_mem_id(rec, match->get_memory_id());
	m_glossary_windows.remove_gloss_record(rec) ;
}

void MemoryWindowFrame::view_by_id( size_t recid, wstring source, wstring trans )
{
	memory_pointer mem = get_memory_model()->get_memory_by_id(m_review_match->get_memory_id()) ;
	m_review_match->set_record(mem->add_by_id(recid, source, trans)) ;
	this->set_display_state(TRANS_REVIEW_STATE) ;
	this->user_feedback(IDS_SHOW_TRANS) ;

	m_view_interface.set_text(get_review_content(mem)) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;
}

void MemoryWindowFrame::add_by_id( size_t recid, wstring source, wstring trans )
{
	if (source.empty() || trans.empty())
	{
		logging::log_warn("Source or translation empty in add_by_id") ;
		return ;
	}

	memory_pointer mem = m_model->create_memory() ;

	try
	{
		mem = get_memory_model()->get_first_memory() ;
	}
	catch (CException& e)
	{
		logging::log_error("Failed to get review memory") ;
		logging::log_exception(e) ;
		user_feedback(IDS_TRANS_REVIEW_FAILED) ;
		return ;
	}

	m_review_match->set_record(mem->add_by_id(recid, source, trans)) ;
	m_review_match->set_memory_id(mem->get_id()) ;

	this->set_display_state(TRANS_REVIEW_STATE) ;
	this->user_feedback(IDS_REFLECTED_TRANS) ;

	m_view_interface.set_text(get_review_content(mem)) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;
}

mem_engine::search_match_ptr MemoryWindowFrame::get_current_match()
{
	return m_view_state->get_current_match() ;
}

// Content when using translation history
wstring MemoryWindowFrame::get_review_content( memory_pointer mem )
{
	search_match_ptr match(mem->make_match(m_review_match->get_record())) ;
	match->set_values_to_record() ;

	cpptempl::data_map data ;
	m_trans_matches.fill_match_template_params(data, match);

	// fill in the template
	wstring content ;
	if ( m_is_short_format )
	{
		content = cpptempl::parse(cpptempl::get_template_text(_T("review.txt")), data) ;
	}
	else
	{
		content = cpptempl::parse(cpptempl::get_template_text(_T("review_full.txt")), data) ;
	}
	return content ;
}

// Get the mousewheel setting
void MemoryWindowFrame::load_mousewheel_setting()
{
	m_mousewheel_count = m_props->m_view_props.get_mem_mousewheel() ;
}

// Show the zoom dialog.
LRESULT MemoryWindowFrame::on_view_zoom( WindowsMessage & )
{
	CZoomDlg dlg(static_cast< CZoomInterface* >( this ), m_mousewheel_count) ;

	dlg.DoModal() ;

	return 0L ;
}

// Load the translation history.
void MemoryWindowFrame::load_history()
{
	this->m_model->clear() ;

	app_props::properties_loaded_history *history_props = &m_props->m_history_props ;
	auto gen_props = &m_props->m_gen_props;

	std::vector<wstring> &loaded_mems = history_props->m_loaded_mems ;

	std::vector<wstring> items ;
	std::copy(loaded_mems.rbegin(), loaded_mems.rend(), std::back_inserter(items)) ;

	FOREACH(wstring filename, items)
	{
		LOG_VERBOSE(string("Loading from history: ") + string2string(filename)) ;
		load( filename.c_str(), false) ;
	}

	std::vector<wstring> remote_items ;
	std::copy(history_props->m_loaded_remote_mems.rbegin(), history_props->m_loaded_remote_mems.rend(), std::back_inserter(remote_items)) ;

	FOREACH(wstring filename, remote_items)
	{
		try
		{
			LOG_VERBOSE(string("Loading remote from history: ") + string2string(filename)) ;
			memory_remote *mem = new memory_remote(m_props) ;
			memory_pointer pmem(mem) ;
			wstring username, password;

			if (gen_props->credential_is_saved(filename))
			{
				username = gen_props->username_for_connection(filename);
				CredentialReader reader(filename);
				password = reader.get_password();
			}
			mem->connect(filename.c_str(), username.c_str(), password.c_str()) ;
			this->add_memory(pmem) ;
		}
		catch (CException& e)
		{
			logging::log_error("Failed to load remote memory") ;
			logging::log_error(string2string(filename)) ;
			logging::log_exception(e) ;
			this->FlashWindow(FALSE) ;
		}
	}
	set_window_title() ;
}

// Load user preferences
// Todo: restore background color preferences
LRESULT MemoryWindowFrame::on_tools_load_preferences(WindowsMessage &)
{
	logging::log_debug("Loading user preferences.") ;
	// get the file name
	file_open_dialog dialog;

	dialog.set_title(R2T(IDS_LOAD_PREFS_TITLE));
	dialog.set_file_filter(get_prefs_filter());

	user_feedback( IDS_LOAD_PREFS_TITLE ) ;

	if (!dialog.show())
	{
		user_feedback(IDS_CANCELLED_ACTION);
		return 0L;
	}

	const WORD old_language = m_appstate.m_preferred_gui_lang ;

	switch (dialog.get_selected_index())
	{
	case 1:
		ATLTRACE( "Load new preferences format\n" ) ;
		load_new_preferences(dialog.get_open_destination(), old_language);
		break;

	case 2:
		load_old_preferences(dialog.get_open_destination());
		break;

	default:
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
	}

	return 0L ;
}

// Serialize preferences to file
LRESULT MemoryWindowFrame::on_tools_save_preferences(WindowsMessage &)
{
	logging::log_debug("Saving user preferences.") ;
	user_feedback(IDS_SAVE_PREFS_TITLE);

	file_save_dialog dialog ;
	dialog.set_title(R2T(IDS_SAVE_PREFS_TITLE));
	dialog.set_file_filter(get_prefs_filter());
	dialog.set_last_save(m_props->m_history_props.m_preferences_location);
	dialog.set_default_ext(L"fprefx");

	if (!dialog.show())
	{
		user_feedback(IDS_CANCELLED_ACTION);
		return 0L;
	}

	CString save_as_file_name = dialog.get_save_destination();


	m_glossary_windows.save_prefs() ;
	save_settings_close() ;
	save_settings_destroy() ;

	switch (dialog.get_selected_index())
	{
	case 1:
		fileops::add_extension_as_needed(save_as_file_name, _T(".fprefx"));
		ATLTRACE( "Save new preferences format\n" ) ;
		break;

	default:
		LOG_VERBOSE("Unknown file type for prefs file. Saving raw filename.") ;
	}
	m_props->save_file(save_as_file_name);

	user_feedback( IDS_PREFS_SAVED ) ;

	return 0L ;
}



void MemoryWindowFrame::save_settings_close()
{
	save_window_settings(_T("MainFrame"));

	save_rebar_settings() ;

	CReBarCtrl ReBar = m_hWndToolBar ;
	m_appstate.m_rebar_has_linebreak = ReBar.GetRowCount() > 1 ;

	m_appstate.write_to_registry() ;
	m_props->write_to_registry() ;
}

void MemoryWindowFrame::save_settings_destroy()
{
	// write our recent docs
	m_mru.WriteToRegistry( resource_string( IDS_REG_KEY ) );

	record_loaded_memory_history() ;

}

wstring MemoryWindowFrame::get_record_translation(record_pointer record)
{
	return m_view_state->retrieve_record_trans(record,
		record_string_prefs( m_props->m_mem_props.is_plaintext())) ;
}

/* Show the updates dialog.
* This is in a separate process, so that in the future we can have this
* process shut us down, and download/install the update automatically.
*/
LRESULT MemoryWindowFrame::on_help_check_updates(WindowsMessage &)
{
	logging::log_debug("Checking for updates") ;

	file::CPath path ;
	path.GetModulePath(_Module.GetModuleInstance()) ;
	path.Append(_T("pytools")) ;
	path.Append(_T("CheckUpdates.exe")) ;

	CString filepath = path.Path() ;
#ifdef _DEBUG
	filepath = "D:\\Program Files (x86)\\Assistant Suite\\Felix\\pytools\\CheckUpdates.exe" ;
#endif

	HINSTANCE result = ::ShellExecute(
		m_hWnd,				// HWND hwnd, 
		_T("open"),			// LPCTSTR lpOperation,
		filepath,			// LPCTSTR lpFile, 
		NULL,				// LPCTSTR lpParameters, 
		NULL,				// LPCTSTR lpDirectory,
		SW_SHOW				// INT nShowCmd
		);	

	check_shell_execute_result((int)result, filepath);
	return 0L ;
}

void MemoryWindowFrame::set_module_library( WORD lang_id )
{
	switch( lang_id )
	{
	case LANG_JAPANESE:
		logging::log_debug("Setting UI language to Japanese") ;
		if ( _Module.get_library() == _T("lang\\JpnResource.dll") )  
		{
			logging::log_debug("Language is already Japanese") ;
			m_appstate.m_preferred_gui_lang = lang_id ;
			return ;
		}
		if ( ! _Module.set_library( _T("lang\\JpnResource.dll") ) )
		{
			/** @todo Get resource library names dynamically */
			CString msg ;
			msg.FormatMessage( IDS_SET_LIB_FAILED, 
				_T("lang\\JpnResource.dll") ) ;
			throw except::CException( msg ) ;
		}
		break ;
	case LANG_ENGLISH:
		logging::log_debug("Setting UI language to English") ;
		if ( _Module.get_library() == _T("lang\\EngResource.dll") )  
		{
			logging::log_debug("Language is already English") ;
			m_appstate.m_preferred_gui_lang = lang_id ;
			return ;
		}
		if ( ! _Module.set_library( _T("lang\\EngResource.dll") ) )
		{
			CString msg ;
			msg.FormatMessage( IDS_SET_LIB_FAILED, 
				_T("lang\\EngResource.dll") ) ;
			throw except::CException( msg ) ;
		}
		break ;
	default:
		logging::log_warn("Unknown UI language setting") ;
		logging::log_debug("Setting UI language to English") ;
		ATLASSERT( FALSE && "Unknown language code!") ;
		m_appstate.m_preferred_gui_lang = LANG_ENGLISH ;
		if ( ! _Module.set_library( _T("lang\\EngResource.dll") ) )
		{
			CString msg ;
			msg.FormatMessage( IDS_SET_LIB_FAILED, 
				_T("lang\\EngResource.dll") ) ;
			throw except::CException( msg ) ;
		}
	}
}

void MemoryWindowFrame::load_old_preferences( const CString filename )
{
	const WORD old_language = m_appstate.m_preferred_gui_lang ;

	this->clear_memory() ;
	m_model->clear() ;

	logging::log_debug("Loading preferences") ;

	CString command ;
	command.Format(_T("REG IMPORT \"%s\""), static_cast<LPCTSTR>(filename)) ;
	CString error_message = _T("Failed to load preferences") ;
	// create the process
	create_process(command, error_message);

	// ==============
	// read our properties from the registry
	m_appstate.read_from_registry() ;
	// get our default properties
	m_props->read_from_registry() ;

	reflect_loaded_preferences(old_language);

}

LRESULT MemoryWindowFrame::on_new_search( WindowsMessage &)
{
	m_search_window.set_mem_window(true) ;
	if (! m_search_window.IsWindow())
	{
		m_search_window.Create(*this) ;
	}

	m_search_window.set_mem_controller( this->get_model() ) ;
	m_search_window.ShowWindow(SW_SHOW) ;
	m_search_window.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE) ;

	m_search_window.show_search_page() ;

	return 0L ;
}

// This lets us override the context menu.
void MemoryWindowFrame::set_doc_ui_handler()
{
#ifdef UNIT_TEST
	return ;
#else
	logging::log_debug("Setting the doc UI handler") ;
	CComObject<CFelixMemDocUIHandler> *pUIH = NULL;
	HRESULT hr = CComObject<CFelixMemDocUIHandler>::CreateInstance (&pUIH);
	if (SUCCEEDED(hr))
	{
		pUIH->m_get_menu = boost::bind(&MemoryWindowFrame::show_doc_context_menu, this) ;
		// Make our custom DocHostUIHandler the window.external handler
		CComQIPtr<IDocHostUIHandlerDispatch> pIUIH = pUIH;
		hr = m_view_interface.m_view.SetExternalUIHandler(pIUIH) ;
	}
	else
	{
		CComException com_exception(hr) ;
		logging::log_error("Failed to set doc UI handler") ;
		logging::log_exception(com_exception) ;
	}
	ATLASSERT(SUCCEEDED(hr)) ;
#endif
}

/* Display the context menu in response to a right click in the browser window.
Todo:
* Dynamic content
* More items
* images?
*/
HRESULT MemoryWindowFrame::show_doc_context_menu()
{
	BANNER("CMainFrame::show_doc_context_menu") ;
	CMenu menu ;

	menu.CreatePopupMenu() ;
	MenuWrapper wrapper(menu, m_hWnd) ;
	wrapper.add_item(ID_EDIT_COPY, R2S(IDS_POPUP_COPY)) ;
	wrapper.add_separator() ;
	wrapper.add_item(ID_NEXT_PANE,  R2S(IDS_POPUP_SWITCH_VIEWS)) ;
	wrapper.add_item(ID_EDIT_REGISTER,  R2S(IDS_POPUP_REGISTER_GLOSS)) ;
	wrapper.add_separator() ;
	wrapper.add_item(IDC_SOURCE_CONCORDANCE_SEL,  R2S(IDS_SOURCE_CONCORDANCE)) ;
	wrapper.add_item(IDC_TRANS_CONCORDANCE_SEL,  R2S(IDS_TRANS_CONCORDANCE)) ;
	wrapper.add_separator() ;
	wrapper.add_item(ID_EDIT_DELETE,  R2S(IDS_POPUP_DELETE)) ;

	// Show the menu at the cursor position
	wrapper.show() ;
	return S_OK ;
}

/*
Close the top memory on the stack, if any.
Change window title to reflect new top memory, if any.
*/
LRESULT MemoryWindowFrame::on_memory_close(WindowsMessage &)
{
	BANNER("CMainFrame::on_memory_close") ;
	// base case -- there are no memories
	if (m_model->empty())
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
	remove_memory(mem, IDS_CLOSED_MEMORY);

	return 0L ;
}

void MemoryWindowFrame::addToMessageLoop()
{
#ifndef UNIT_TEST
	// register object for message filtering
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	if (pLoop)
	{
		pLoop->AddMessageFilter(this);
	}
#endif
}

CString MemoryWindowFrame::get_active_mem_name()
{
	if ( ! m_model->empty() )
	{
		memory_pointer mem = m_model->get_first_memory() ;
		if (! mem->is_local())
		{
			return mem->get_location() ;
		}
		else
		{
			const file::name fname =  mem->get_location() ;
			return fname.file_name_part() ;
		}
	} 
	return resource_string( IDS_NEW ) ;
}

LRESULT MemoryWindowFrame::on_user_lookup_source( WindowsMessage& )
{
	SENSE("CMainFrame::on_user_lookup_source") ;
	if (m_deferred_query.empty())
	{
		return 0L ; 
	}
	this->lookup(m_deferred_query) ;
	m_deferred_query.clear() ;
	ATLASSERT(m_deferred_query.empty()) ;
	return 0L ; 
}

LRESULT MemoryWindowFrame::on_user_lookup_trans( WindowsMessage& )
{
	SENSE("CMainFrame::on_user_lookup_trans") ;
	if (m_deferred_query.empty())
	{
		return 0L ; 
	}
	this->lookup_trans(m_deferred_query) ;
	m_deferred_query.clear() ;
	ATLASSERT(m_deferred_query.empty()) ;
	return 0L ; 
}

void MemoryWindowFrame::set_display_state( DISPLAY_STATE new_state )
{
	switch(new_state)
	{
	case NEW_RECORD_DISPLAY_STATE:
		m_view_state = &m_view_state_new ;
		break ;
	case INIT_DISPLAY_STATE:
		m_view_state = &m_view_state_initial ;
		break ;
	case TRANS_REVIEW_STATE:
		m_view_state = &m_view_state_review;
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

edit_record_dlg_ptr MemoryWindowFrame::get_editor()
{
	return m_editor ;
}

bool MemoryWindowFrame::is_short_format()
{
	return m_is_short_format ;
}

bool MemoryWindowFrame::is_single_page()
{
	return !! m_props->m_view_props.m_data.m_single_screen_matches ;
}

void MemoryWindowFrame::set_menu_checkmark( int item_id, bool is_checked )
{
	if (! this->IsWindow())
	{
		logging::log_debug("Window is not visible; ignoring menu checkmark.") ;
		return ;
	}
	UISetCheck(item_id, !! is_checked);
	UpdateLayout( FALSE ) ;
}

void MemoryWindowFrame::perform_concordance_search(mem_engine::search_query_params &params)
{
	search_match_container matches ;
	m_model->perform_search( matches, params ) ;
	m_search_matches.set_matches( matches ) ;
}

void MemoryWindowFrame::create_reg_gloss_window()
{
	_Module.AddCreateWndData( &m_reg_gloss_dlg.m_thunk.cd, (CDialogImplBaseT< CWindow >*)&m_reg_gloss_dlg);

	DLGPROC lpDialogProc = (DLGPROC)m_reg_gloss_dlg.StartDialogProc ;

	instantiate_dlg(IDD_ADD_GLOSS, lpDialogProc) ;

	ATLASSERT( m_reg_gloss_dlg.IsWindow() ) ;

	if ( ! m_reg_gloss_dlg.IsWindow() )
	{
		throw except::CException( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
	}
}

void MemoryWindowFrame::save_memory_as( memory_pointer mem )
{
	CString original_file_name;
	if (mem->is_new() == false)
	{
		original_file_name = mem->get_fullpath();
	}

	CString dialog_title;
	dialog_title.FormatMessage(IDS_SAVE, resource_string(IDS_MEMORY));

	file_save_dialog dialog;
	dialog.set_title(dialog_title);
	dialog.set_file_filter(get_mem_save_filter());
	dialog.set_filename(original_file_name);
	dialog.set_last_save(m_props->m_history_props.m_memory_location);
	dialog.set_default_ext(L"ftm");

	if (!dialog.show())
	{
		user_feedback(IDS_CANCELLED_ACTION);
		return;
	}

	CString save_as_file_name = dialog.get_save_destination();

	switch (dialog.get_selected_index())
	{
	case 1: case 7:
		logging::log_debug("Saving memory as ftm file") ;
		fileops::add_extension_as_needed(save_as_file_name, _T(".ftm"));
		break;

	case 2:
		logging::log_debug("Saving memory as xml file") ;
		fileops::add_extension_as_needed(save_as_file_name, _T(".xml"));
		break;

	case 3:
		logging::log_debug("Saving memory as tmx file") ;
		fileops::add_extension_as_needed(save_as_file_name, _T(".tmx"));
		export_tmx(save_as_file_name, mem);
		return ;

	case 4:
		logging::log_debug("Saving memory as Trados text file") ;
		fileops::add_extension_as_needed(save_as_file_name, _T(".txt"));
		export_trados(save_as_file_name, mem);
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
		logging::log_warn("Unknown case in switch statement") ;
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
		logging::log_debug("Saving memory as tmx file") ;
		fileops::add_extension_as_needed(save_as_file_name, _T(".tmx"));
		export_tmx(save_as_file_name, mem);
		break;
	}

	// If the name changes, then we make the current user into the creator
	const CString old_location = mem->get_location() ;

	mem->set_location(save_as_file_name);

	save_memory( mem ) ;

	m_props->m_history_props.m_memory_location = 
		static_cast<LPCWSTR>(save_as_file_name);

	if (0 != old_location.CompareNoCase(save_as_file_name))
	{
		mem->set_creator_to_current_user( ) ;
	}

	set_window_title() ;
}

void MemoryWindowFrame::get_qc_messages( mem_engine::record_pointer record, std::vector<wstring> &messages )
{
	app_props::properties_qc *props = &(m_props->m_qc_props) ;

	if (! props->qc_enabled())
	{
		return ;
	}

	std::vector<qc::rule_ptr> rules ;
	if (props->check_all_caps())
	{
		rules.push_back(qc::rule_ptr(new qc::AllCapsCheckRule)) ;
	}
	if (props->check_numbers())
	{
		rules.push_back(qc::rule_ptr(new qc::NumberCheckRule)) ;
	}
	if (props->check_gloss())
	{
		search_query_params params ;
		params.m_ignore_case = !! m_props->m_gloss_props.m_data.m_ignore_case ;
		params.m_ignore_width = !! m_props->m_gloss_props.m_data.m_ignore_width ;
		params.m_ignore_hira_kata = !! m_props->m_gloss_props.m_data.m_ignore_hir_kat ;
		params.set_source(record->get_source_rich()) ;

		std::shared_ptr<memory_model> memories = get_glossary_window()->get_memory_model()->get_memories() ;

		search_match_container matches ;

		for(size_t i = 0 ; i < memories->size() ; ++i)
		{
			mem_engine::memory_pointer mem = memories->get_memory_at(i) ;
			if (props->check_gloss_name(static_cast<LPCWSTR>(mem->get_fullpath())))
			{
				mem->get_glossary_matches(matches, params) ;
			}
		}

		std::vector<qc::gloss_pair> gloss_matches ;
		FOREACH(match_ptr match, matches)
		{
			record_pointer rec = match->get_record() ;
			gloss_matches.push_back(qc::gloss_pair(rec->get_source_plain(), rec->get_trans_plain())) ;
		}

		rules.push_back(qc::rule_ptr(new qc::GlossCheckRule(gloss_matches))) ;
	}

	qc::QcChecker checker(rules) ;
	checker.check(record->get_source_plain(), record->get_trans_plain()) ;
	checker.get_error_msgs(messages) ;
}

INT_PTR MemoryWindowFrame::check_save_memory( mem_engine::memory_pointer mem )
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

void MemoryWindowFrame::save_old_prefs_file( CString filename )
{

	CString command ;
	command.Format(_T("REG EXPORT hkcu\\software\\assistantsuite\\felix \"%s\""), static_cast<LPCTSTR>(filename)) ;

	if(is_vista_or_later())
	{
		command += _T(" /y") ;
	}

	CString error_message = _T("Failed to save preferences") ;
	// create the process
	create_process(command, error_message);
}

void MemoryWindowFrame::check_mousewheel_count()
{
	if (m_mousewheel_count)
	{
		set_zoom_level(m_mousewheel_count) ;
	}
}

mem_engine::placement::regex_rules * MemoryWindowFrame::get_regex_rules()
{
	return &m_rules ;
}

int MemoryWindowFrame::get_gloss_show_command()
{
	return m_glossary_windows.get_show_command(is_window());
}

bool MemoryWindowFrame::should_add_record_to_glossary( record_pointer record )
{
	return  record->get_source_plain().length() <= m_props->m_gloss_props.get_max_add() 
		&& m_props->m_gloss_props.get_max_add() > 0;
}

// Gets the glossary matches from the active glossary. NULL if there are no glossaries.
mem_engine::felix_query * MemoryWindowFrame::get_current_gloss_matches()
{
	return m_glossary_windows.get_current_matches() ;
}

void MemoryWindowFrame::look_up_current_source_in_gloss()
{
	search_match_ptr match = m_trans_matches.current() ;
	record_pointer rec = match->get_record() ;
	wstring entry_source = rec->get_source_plain() ;
	m_glossary_windows.look_up( entry_source ) ;
}

input_device_ptr MemoryWindowFrame::get_input_device()
{
	return m_input_device ;
}

output_device_ptr MemoryWindowFrame::get_output_device()
{
	return m_output_device ;
}

model_iface_ptr MemoryWindowFrame::get_model()
{
	return m_model ;
}

app_props::properties_general* MemoryWindowFrame::get_props_general()
{
	return &(m_props->m_gen_props) ;
}

app_props::props_ptr MemoryWindowFrame::get_properties()
{
	return m_props ;
}

void MemoryWindowFrame::register_event_listener( UINT id, boost::function< LRESULT( WindowsMessage& ) > listenerFunction )
{
	this->m_message_map[id] = listenerFunction ;
}

void MemoryWindowFrame::register_user_event_listener( UINT id, boost::function< LRESULT( WindowsMessage& ) > listenerFunction )
{
	this->m_user_message_map[id] = listenerFunction ;
}

void MemoryWindowFrame::register_command_event_listener( UINT id, boost::function< LRESULT( WindowsMessage& ) > listenerFunction )
{
	this->m_command_message_map[id] = listenerFunction ;
}

long MemoryWindowFrame::get_num_matches()
{
	return m_trans_matches.size() ;
}

void MemoryWindowFrame::add_edit_record( mem_engine::record_pointer new_record, LPARAM display_state )
{
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

void MemoryWindowFrame::edit_edit_record( mem_engine::record_pointer new_record, LPARAM display_state )
{
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

BOOL MemoryWindowFrame::handle_sw_exception( except::CSWException &e, const CString &failure_message )
{
	return CCommonWindowFunctionality::handle_sw_exception(e, failure_message) ;
}

bool MemoryWindowFrame::check_for_clashes( memory_type mem )
{
	if (m_glossary_windows.has_clashes(mem->get_location()))
	{
		return IDCANCEL != prompt_user_for_overwrite( mem->get_location() ) ;
	}
	return true ;
}

model_iface_ptr MemoryWindowFrame::get_memory_model()
{
	return m_model ;
}

mem_engine::felix_query * MemoryWindowFrame::get_current_matches()
{
	return &m_trans_matches ;
}
