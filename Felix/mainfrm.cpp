/*!
 * \date 2007/01/31
 * \author Ryan Ginstrom
 *
 * Detailed description here.
 *
 * Copyright (C) Ryan Ginstrom.
 */

#include "StdAfx.h"
#include "MainFrm.h"

#include "resource.h"				// resource IDs

#include "ConcordanceDialog.h"

#include "aboutdlg.h"				// CAboutDlg
#include "PropertiesDlg.h"			// CPropertiesDlg
#include "data_importer.h"
#include "data_converter.h"
#include "ImportDialog.h"
#include "ui.h"						// windows_ui -- wrapper for common dialog boxes

#include "NagDialog.h"				// CNagDialog
#include "InputKeyDlg.h"			// CInputKeyDlg

#include "Path.h"
#include "RegSettings.h"	// CWindowSettings & CReBarSettings
#include "ColorRef.h"

#include "ConvertUTF.h"		// ConvertUTF8toUTF16

#include "TMXReader.h"		// CTMXReader
#include "record_local.h"

#include "Drop.h"				// CDrop

#include "HtmlDocColorSetter.h"	// CHtmlDocColorSetter
#include "xpmenu/Tools.h"		// CWindowRect

#include "HtmlHelp.h"
#pragma comment(lib, "HtmlHelp.lib")

#include "NumberFmt.h"

#include "text_templates.h"
#include "ConnectionDlg.h"
#include "DemoException.h"

#include "QueryMergeDlg.h"
#include "text_templates.h"
#include "FelixMemDocUIHandler.h"
#include "memory_local.h"

#include "cpptempl.h"
#include <shellapi.h>
#include "input_device_file.h"
#include "output_device.h"

#include "qcrules/qc_checker.h"
#include "qcrules/allcaps_check.h"
#include "qcrules/number_check.h"
#include "qcrules/gloss_check.h"

#define ZOOM_KEY CComVariant(L"MainFrameZoom")

using namespace mem_engine ;
using namespace except ;
using namespace html ;

CString get_help_file_path( CString path )
{
	file::CPath filePath( path ) ;
	filePath.Append( resource_string(IDS_HELPFILE_NAME) ) ;

	if ( ! filePath.FileExists() ) 
	{
		filePath.ReplaceFileSpec(resource_string(IDS_PDF_HELPFILE_NAME)) ;
	}

	if ( ! filePath.FileExists() ) 
	{
		return resource_string(IDS_ONLINE_HELPFILE_URL) ;
	}

	return filePath.Path() ;
}
CString get_docs_path()
{
	// get the instance
	HINSTANCE hInstance = _Module.GetModuleInstance() ;
	ATLASSERT(hInstance != NULL) ;

	// get the module file name
	file::CPath path ;
	path.GetModulePath( hInstance ) ;

	path.Append( _T("DOCS") ) ;
	path.AddBackslash() ;

	return path.Path() ;
}


/** Constructor. Takes model interface.
*/
CMainFrame::CMainFrame( model_iface_ptr model, app_props::props_ptr props ) : 
	m_model(model),
	m_props(props),
	m_editor(new CEditTransRecordDialog),
	m_manager_window(m_props, IDS_SEARCH_MANAGER_TITLE, _T("MemoryMangerWindow"), this),
	m_search_window(this),
	m_input_device(new InputDeviceFile),
	m_output_device(new OutputDeviceFile),
	m_silent_memories(&props->m_mem_props, &props->m_gloss_props, &props->m_alg_props)
{
	initialize_values() ;

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

	m_props->m_gen_props.read_from_registry() ;
	const BOOL show_markup = m_props->m_gen_props.m_data.m_show_markup ;
	this->m_trans_matches.m_params.m_show_marking = !! show_markup ;
	// make sure that the username is reflected!
	const wstring user_name = CT2W(m_props->m_gen_props.m_data.m_user_name) ;
	set_record_username(user_name) ;

	addToMessageLoop();

	// register command handlers
	this->register_event_listener( WM_CREATE, boost::bind(&CMainFrame::on_create, this, _1 ) ) ; 
	this->register_event_listener( WM_DESTROY, boost::bind(&CMainFrame::on_destroy, this, _1 ) ) ; 
	this->register_event_listener( WM_CLOSE, boost::bind(&CMainFrame::on_close, this, _1 ) ) ; 
	this->register_event_listener( WM_DROPFILES, boost::bind(&CMainFrame::on_drop, this, _1 ) ) ; 

	// user messages
	this->register_user_event_listener( USER_LOOKUP_SOURCE, boost::bind(&CMainFrame::on_user_lookup_source, this, _1 )) ;
	this->register_user_event_listener( USER_LOOKUP_TRANS, boost::bind(&CMainFrame::on_user_lookup_trans, this, _1 )) ;
	this->register_user_event_listener( USER_SAVE_MEMORIES, boost::bind(&CMainFrame::on_user_save, this, _1 )) ;

	this->register_user_event_listener( IDC_RETRIEVE_EDIT_RECORD, boost::bind(&CMainFrame::on_user_retrieve_edit_record, this, _1 )) ;
	this->register_user_event_listener( ID_EDIT_FIND, boost::bind(&CMainFrame::on_user_edit_search, this, _1 )) ;
	this->register_user_event_listener( ID_USER_SEARCH, boost::bind(&CMainFrame::on_user_search, this, _1 )) ;
	this->register_user_event_listener( IDC_REPLACE_EDIT_RECORD, boost::bind(&CMainFrame::on_user_replace_edit_record, this, _1 )) ;
	this->register_user_event_listener( ID_EDIT_REPLACE, boost::bind(&CMainFrame::on_user_edit_replace, this, _1 )) ;
	this->register_user_event_listener( IDC_MIN_VIEW_END, boost::bind(&CMainFrame::on_user_view_min_end, this, _1 )) ;

	// commands
	this->register_command_event_listener( IDC_DEMO_CHECK_EXCESS, boost::bind(&CMainFrame::on_demo_check_excess_memories, this, _1 )) ;
	this->register_command_event_listener( IDC_CHECK_DEMO, boost::bind(&CMainFrame::on_check_demo, this, _1 )) ;
	this->register_command_event_listener( IDC_SOURCE_CONCORDANCE_SEL, boost::bind(&CMainFrame::on_source_concordance, this, _1 )) ;
	this->register_command_event_listener( IDC_TRANS_CONCORDANCE_SEL, boost::bind(&CMainFrame::on_trans_concordance, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_MEMORY_MGR, boost::bind(&CMainFrame::on_tools_memory_manager, this, _1 )) ;
	this->register_command_event_listener( IDC_UNDERLINE, boost::bind(&CMainFrame::on_underline, this, _1 )) ;
	this->register_command_event_listener( IDC_BOLD, boost::bind(&CMainFrame::on_bold, this, _1 )) ;
	this->register_command_event_listener( IDC_ITALIC, boost::bind(&CMainFrame::on_italic, this, _1 )) ;
	this->register_command_event_listener( ID_FORMAT_BGCOLOR, boost::bind(&CMainFrame::OnFormatBackgroundColor, this, _1 )) ;
	this->register_command_event_listener( ID_APP_EXIT, boost::bind(&CMainFrame::on_file_exit, this, _1 )) ;
	this->register_command_event_listener( ID_GLOSSARY_NEW, boost::bind(&CMainFrame::on_new_glossary, this, _1 )) ;

	this->register_command_event_listener( ID_FILE_NEW, boost::bind(&CMainFrame::on_file_new, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_OPEN, boost::bind(&CMainFrame::on_file_open, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_SAVE, boost::bind(&CMainFrame::on_file_save, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_SAVE_AS, boost::bind(&CMainFrame::on_file_save_as, this, _1 )) ;
	this->register_command_event_listener( ID_FILE_SAVE_ALL, boost::bind(&CMainFrame::on_file_save_all, this, _1 )) ;

	this->register_command_event_listener( ID_MEMORY_CLOSE, boost::bind(&CMainFrame::on_memory_close, this, _1 )) ;


	this->register_command_event_listener( IDD_EDIT_ENTRY, boost::bind(&CMainFrame::on_edit_entry, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_DELETE, boost::bind(&CMainFrame::on_delete_entry, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_ADD, boost::bind(&CMainFrame::on_add, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_REGISTER, boost::bind(&CMainFrame::on_register_gloss, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_FIND, boost::bind(&CMainFrame::on_new_search, this, _1 )) ;

	this->register_command_event_listener( ID_FIND_QUICKSEARCH, boost::bind(&CMainFrame::on_find, this, _1 )) ;

	this->register_command_event_listener( IDD_CONCORDANCE, boost::bind(&CMainFrame::on_concordance, this, _1 )) ;
	this->register_command_event_listener( ID_EDIT_REPLACE, boost::bind(&CMainFrame::on_edit_replace, this, _1 )) ;

	this->register_command_event_listener( ID_VIEW_TOOLBAR, boost::bind(&CMainFrame::on_view_toolbar, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_STATUS_BAR, boost::bind(&CMainFrame::on_view_status_bar, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_EDIT_MODE, boost::bind(&CMainFrame::on_view_edit_mode, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_MATCH, boost::bind(&CMainFrame::on_view_match, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_SEARCH, boost::bind(&CMainFrame::on_view_search, this, _1 )) ;
	this->register_command_event_listener( ID_VIEW_SWITCH, boost::bind(&CMainFrame::on_view_switch, this, _1 )) ;
	this->register_command_event_listener( IDC_MIN_VIEW_BEGIN, boost::bind(&CMainFrame::on_view_min_begin, this, _1 )) ;
	this->register_command_event_listener( ID_APP_ABOUT, boost::bind(&CMainFrame::show_about_dialog, this, _1 )) ;
	this->register_command_event_listener( ID_HELP, boost::bind(&CMainFrame::on_help, this, _1 )) ;

	this->register_command_event_listener( ID_HELP_FAQ, boost::bind(&CMainFrame::on_help_faq, this, _1 )) ;
	this->register_command_event_listener( ID_HELP_REGISTER, boost::bind(&CMainFrame::on_help_register, this, _1 )) ;
	this->register_command_event_listener(ID_HELP_CHECKUPDATES, boost::bind(&CMainFrame::on_help_check_updates, this, _1 )) ;

	this->register_command_event_listener( IDC_SET_GLOSS, boost::bind(&CMainFrame::on_register_gloss, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_PREFERENCES, boost::bind(&CMainFrame::on_tools_preferences, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_LANGUAGE, boost::bind(&CMainFrame::on_tools_switch_language, this, _1 )) ;

	this->register_command_event_listener( ID_TOOLS_LOADPREFERENCES, boost::bind(&CMainFrame::on_tools_load_preferences, this, _1 )) ;
	this->register_command_event_listener( ID_TOOLS_SAVEPREFERENCES, boost::bind(&CMainFrame::on_tools_save_preferences, this, _1 )) ;

	this->register_command_event_listener( IDC_TEST_EXCEPTION, boost::bind(&CMainFrame::on_test_exception, this, _1 )) ;

	this->register_command_event_listener( ID_NEXT_PANE, boost::bind(&CMainFrame::on_toggle_views, this, _1 )) ;
	this->register_command_event_listener( ID_PREV_PANE, boost::bind(&CMainFrame::on_toggle_views, this, _1 )) ;

	this->register_command_event_listener( ID_VIEW_ZOOM, boost::bind(&CMainFrame::on_view_zoom, this, _1 )) ;
	for ( int i=ID_FILE_MRU_FIRST ; i <=ID_FILE_MRU_LAST ; ++i )
	{
		this->register_command_event_listener( i, boost::bind(&CMainFrame::on_mru_file_open, this, _1 )) ;
	}

}


//! DTOR
CMainFrame::~CMainFrame()
{
#ifndef UNIT_TEST
	ATLASSERT( FALSE == m_CmdBar.IsWindow() ) ;
#endif
	if ( m_CmdBar.m_hWnd )
	{
		m_CmdBar.m_hWnd = NULL ;
	}
#ifndef UNIT_TEST
	ATLASSERT( FALSE == IsWindow() ) ;
#endif

	if ( m_hWnd )
	{
		m_hWnd = NULL ;
	}
}

/**************************************************
*
*	Windows messaging routines
*
**************************************************/


/** We get a crack at messages before the are dispatched.
*/
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{

	if( m_min_view.IsWindow() && m_min_view.IsWindowVisible() )
	{
		return m_min_view.PreTranslateMessage( pMsg ) ;
	}

	// Go through our glossary windows, and see if we need to pass on 
	// this message to them...
	for( auto pos = m_glossary_windows.begin() ; pos != m_glossary_windows.end() ; )
	{
		if ( ! (*pos)->IsWindow() )
		{
			m_glossary_windows.erase( pos ) ;
			// reset to avoid problems
			pos = m_glossary_windows.begin() ;
		}
		else 
		{
			HWND focus = pMsg->hwnd ;
			if ( (*pos)->m_hWnd == focus || (*pos)->IsChild( focus ) )
			{
				return (*pos)->PreTranslateMessage(pMsg) ;
			}
			++pos ;
		}
	}

	// let the frame window have a try
	if( frame_class::PreTranslateMessage( pMsg ) )
	{
		return TRUE;
	}
	// prevent the view from eating our menu shortcut keys...
	const int key_mask = 0x8000 ;
	if (::GetKeyState(VK_MENU) & key_mask)
	{
		return FALSE ;
	}
	return m_view_interface.PreTranslateMessage( pMsg ) ;
}


/** Idle handler.
*/
BOOL CMainFrame::OnIdle()
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
CColorRef CMainFrame::GetBackColor()
{
	return CColorRef( m_view_interface.get_bg_color() ) ;
}

//! Respond to user command to format the background color.
LRESULT CMainFrame::OnFormatBackgroundColor( WindowsMessage &message )
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
void CMainFrame::set_exporter_src_and_target_langs(CExportDialog &dialog, CTMXWriter &exporter)
{
	exporter.set_src_lang( string2wstring( dialog.get_source_plain() ) ) ;
	exporter.set_target_lang( string2wstring( dialog.get_trans_plain() ) ) ;
}

//! Export the top memory as a TMX memory.
bool CMainFrame::export_tmx( const CString &file_name, mem_engine::memory_pointer mem )
{
	CExportDialog dialog ;

	if ( IDCANCEL == dialog.DoModal() )
	{
		return false ;
	}

	CTMXWriter exporter( static_cast< CProgressListener* >( this ),
						&m_props->m_mem_props,
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
bool CMainFrame::export_trados( const CString &file_name, mem_engine::memory_pointer mem )
{
	ATLASSERT ( m_model->get_memories()->empty() == false ) ; 
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
								&this->m_props->m_mem_props) ;
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
LRESULT CMainFrame::on_create( WindowsMessage &message  )
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
		load_util_settings();

		// get our default properties
		m_props->read_from_registry() ;

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
		set_up_ui_state() ;

		// register object for message filtering and idle updates
#ifndef UNIT_TEST
		CMessageLoop* pLoop = _Module.GetMessageLoop() ;
		ATLASSERT(pLoop != NULL) ;
		if (pLoop)
		{
			pLoop->AddIdleHandler(this) ;
		}
#endif

		// the glossary window
		logging::log_debug("Setting up the glossary window") ;
		add_glossary_window(gloss_window_pointer(new CGlossaryDialog(m_props))) ;

		// set the title
		set_window_title() ;

		set_up_window_size() ;

		UpdateLayout() ; // the toolbar

		init_item_colors();

		::DragAcceptFiles( m_hWnd, TRUE ) ;

		init_background_processor();

		SetFocus() ;

		::PostMessage( m_hWnd, WM_COMMAND, MAKEWPARAM( IDC_CHECK_DEMO, 100 ), 0 ) ;

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
void CMainFrame::check_load_history( )
{
	if ( ! m_props->m_gen_props.m_data.m_load_prev_mem_on_startup ) 
	{
		return ;
	}

	load_history();
}

//! See if the command line has us loading any memories.
void CMainFrame::check_command_line(commandline_options &options, input_device_ptr input)
{
	if (! options.m_prefs_file.empty())
	{
		this->load_old_preferences(options.m_prefs_file.c_str()) ;
		return ;
	}
	foreach(tstring filename, options.m_tm_files)
	{
		memory_local *rawmem = new memory_local(&m_props->m_mem_props) ;
		memory_pointer mem(rawmem) ;
		rawmem->load(filename.c_str()) ;
		this->add_memory(mem) ;
	}
	foreach(tstring filename, options.m_glossary_files)
	{
		memory_local *rawmem = new memory_local(&m_props->m_mem_props) ;
		memory_pointer mem(rawmem) ;
		rawmem->load(filename.c_str()) ;
		this->get_glossary_window()->add_glossary(mem) ;
	}
	foreach(tstring filename, options.m_xml_files)
	{
		memory_local *rawmem = new memory_local(&m_props->m_mem_props) ;
		memory_pointer mem(rawmem) ;
		rawmem->load(filename.c_str()) ;
		if (mem->get_memory_info()->is_memory())
		{
			this->add_memory(mem) ;
		}
		else
		{
			this->get_glossary_window()->add_glossary(mem) ;
		}
	}
	foreach(tstring filename, options.m_multiterm_files)
	{
		this->get_glossary_window()->import_multiterm(CString(filename.c_str())) ;
	}
	foreach(tstring filename, options.m_tmx_files)
	{
		this->import_tmx(CString(filename.c_str()), input) ;
	}
	foreach(tstring filename, options.m_trados_text_files)
	{
		this->import_trados(CString(filename.c_str())) ;
	}
}


//! Add a record after editing.
LRESULT CMainFrame::on_user_retrieve_edit_record( WindowsMessage &message)
{
	const LPARAM lparam = message.lParam ;
	set_display_state( static_cast< DISPLAY_STATE >( lparam ) ) ;
	ATLASSERT( get_display_state() == lparam ) ;

	SENSE("on_user_retrieve_edit_record") ;

	ATLASSERT( m_editor->get_memory_id() > 0 ) ;

	m_view_state->retrieve_edit_record(m_editor->get_memory_id(),
		m_editor->get_new_record()) ;

#ifdef UNIT_TEST
	return 0 ;
#else

	show_view_content() ;
	return 0L ;
#endif
}


//! Handle a user exit command
LRESULT CMainFrame::on_file_exit(  WindowsMessage &message )
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
LRESULT CMainFrame::on_delete_entry(  WindowsMessage &message )
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
LRESULT CMainFrame::on_find( WindowsMessage &message  )
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
LRESULT CMainFrame::on_edit_replace(  WindowsMessage &message )
{
	message ;
	SENSE("on_edit_replace") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	if ( m_view_interface.is_edit_mode() == false )
	{
		on_new_search(message) ;
		m_search_window.handle_gotoreplace() ;
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
LRESULT CMainFrame::on_concordance(  WindowsMessage &message )
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
LRESULT CMainFrame::on_new_glossary(  WindowsMessage &message )
{
	message ;
	SENSE("on_new_glossary") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	add_glossary_window(gloss_window_pointer(new CGlossaryDialog(m_props))) ;
	return 0L ;
#endif
}

/** Adds a new glossary window.
*/
bool CMainFrame::add_glossary_window(gloss_window_pointer gloss_window, int show_cmd)
{
	show_cmd ;
	if ( m_glossary_windows.empty() )
	{
		gloss_window->set_main_on() ;
	}

#ifndef UNIT_TEST
	gloss_window->Create( m_hWnd ) ;
	ATLASSERT( gloss_window->IsWindow() ) ;
#endif

	gloss_window->set_listener( static_cast< CGlossaryWinListener* >( this ) ) ;

	// the glossary window reads these itself
	//	gloss_window->set_properties_gloss( m_properties->m_gloss_props ) ;
	//	gloss_window->set_properties_algo( m_properties->m_alg_props ) ;

	m_glossary_windows += gloss_window ;
	get_glossary_window()->set_main( true ) ;

#ifdef UNIT_TEST
	return true ;
#else

	if ( m_glossary_windows.size() == 1 ) // we have just added the only one...
	{
		CWindowSettings ws;

		if( ws.Load( resource_string(IDS_REG_KEY), _T("MainGlossary") ) )
		{
			ws.ApplyTo( *gloss_window ) ;
		}
		else
		{
			gloss_window->set_up_initial_size() ;
		}
	}

	if ( ! IsWindowVisible() )
	{
		gloss_window->ShowWindow( SW_HIDE ) ;
	}
	else
	{
		// show it after it has been sized and stuff
		gloss_window->ShowWindow( show_cmd ) ;
	}

	return true ;
#endif
}

/** Handles the new file command. Creates a new memory.
* File -> New -> Memory.
*/
LRESULT CMainFrame::on_file_new( WindowsMessage &message  )
{
	message ;
	SENSE("on_file_new") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	memory_pointer mem(new mem_engine::memory_local(&m_props->m_mem_props)) ;

	mem->set_is_memory( true ) ;

	add_memory( mem ) ;

	user_feedback( IDS_NEW ) ;

	return 0L ;
#endif
}

/** Handles file open command.
*/
LRESULT CMainFrame::on_file_open(  WindowsMessage &message )
{
	message ;
	SENSE("on_file_open") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	// get the file name
	open_file_dlg dialog ;

	dialog.set_prompt( R2T( IDS_OPEN ) ) ;

	dialog.set_filter( get_open_filter() ) ;

	file::OpenDlgList import_files ;

	user_feedback( IDS_OPEN ) ;

	if ( ! dialog.get_open_files( import_files ) ) 
	{
		user_feedback( IDS_CANCELLED_ACTION ) ;
		return 0L ;
	}

	const int selected_index = dialog.get_selected_index() ;

	switch( selected_index ) 
	{
	case 1: case 4:
		ATLTRACE( "Open Felix memory files.\n" ) ;
		break;

	case 2:
		import_tmx( import_files, get_input_device() ) ;
		return 0L ;

	case 3:
		import_trados( import_files ) ;
		return 0L ;

	default:
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 

	}

	// They are regular memory files
	foreach(CString filename, import_files.m_filenames)
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
bool CMainFrame::OnBeforeNavigate2( _bstr_t url )
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
	return true ;
}


/** Save the top memory.
* File -> Save.
*/
LRESULT CMainFrame::on_file_save(WindowsMessage &)
{
	SENSE("on_file_save") ;

	if ( m_model->get_memories()->empty() )
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
void CMainFrame::do_save( memory_pointer mem )
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
void CMainFrame::handle_foreign_file_save(memory_pointer& mem, const file::CFileExtension& ext)
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
LRESULT CMainFrame::on_file_save_as(WindowsMessage &)
{
	SENSE("on_file_save_as") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	if ( m_model->get_memories()->empty() )
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
LRESULT CMainFrame::on_close( WindowsMessage &message )
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
LRESULT CMainFrame::on_destroy( WindowsMessage &message )
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
	app_props::properties_general gen_props ;
	gen_props.read_from_registry() ;
	gen_props.m_data.m_first_launch = FALSE ;
	gen_props.write_to_registry() ;

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
void CMainFrame::check_save_history()
{
	app_props::properties_loaded_history *history_props = &m_props->m_history_props ;
	history_props->read_from_registry() ;

	history_props->m_loaded_mems.clear() ;
	history_props->m_loaded_remote_mems.clear() ;
	
	for ( memory_iterator pos = m_model->get_memories()->begin() ; 
		pos != m_model->get_memories()->end() ; 
		++pos )
	{
		memory_pointer mem = *pos ;
		tstring location = (LPCTSTR)mem->get_fullpath();
		if (! mem->is_local())
		{
			history_props->m_loaded_remote_mems.push_back(location);
		}
		else if (::PathFileExists(location.c_str()))
		{
			history_props->m_loaded_mems.push_back(location);
		}
	}

	size_t mem_num = 0 ;
	size_t remote_num = 0 ;
	for ( memory_iterator pos = m_model->get_memories()->begin() ; 
		has_more_memory_history(pos, mem_num) ; ++pos )
	{
		memory_pointer mem = *pos ;
		const CString location = mem->get_fullpath() ;
		if ( ::PathFileExists(location) && mem->is_local()) 
		{
			tstring mem_title = (LPCTSTR)location;
			_tcsncpy_s(history_props->m_data.m_mems[mem_num], 
				MAX_PATH, 
				(LPCTSTR)mem_title.c_str(), 
				mem_title.size() ) ;

			ATLASSERT ( mem_num + remote_num < m_model->get_memories()->size() ) ;
			mem_num++ ;
		}
		else if (! mem->is_local())
		{
			tstring mem_title = (LPCTSTR)location;
			_tcsncpy_s(history_props->m_data.m_remote_mems[remote_num], 
				MAX_PATH, 
				(LPCTSTR)mem_title.c_str(), 
				mem_title.size() ) ;
			ATLASSERT ( remote_num + mem_num < m_model->get_memories()->size() ) ;
			remote_num++ ;
		}
	}
	history_props->m_data.m_num_mems = mem_num ;
	history_props->m_data.m_num_remote_mems = remote_num ;
	history_props->write_to_registry() ;
}
//! See if we have more memories to save to our history.
bool CMainFrame::has_more_memory_history( memory_iterator pos, 
										 const size_t mem_num )
{
	return (pos != m_model->get_memories()->end() && 
		mem_num < app_props::NumMems) ;
}

/** Toggle toolbar visibility.
*/
LRESULT CMainFrame::on_view_toolbar(WindowsMessage &)
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
LRESULT CMainFrame::on_view_edit_mode(WindowsMessage &)
{
	SENSE("on_view_edit_mode") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	const bool edit_mode_enabled = m_view_interface.is_edit_mode() ;


	UISetCheck(ID_VIEW_EDIT_MODE, ! edit_mode_enabled );

	// swap out the various find dialogs...
	if ( edit_mode_enabled )
	{
		if ( m_edit_find.IsWindow() && m_edit_find.IsWindowVisible() )
		{
			m_edit_find.ShowWindow( SW_HIDE ) ;
			handle_find() ;
		}
		if ( m_edit_replace.IsWindow() && m_edit_replace.IsWindowVisible() )
		{
			m_edit_replace.ShowWindow( SW_HIDE ) ;
		}
	}
	else
	{
		if ( m_find.IsWindow() && m_find.IsWindowVisible() )
		{
			m_find.ShowWindow( SW_HIDE ) ;
			m_edit_find.ShowWindow( SW_SHOW ) ;
		}
	}

	m_view_state->handle_toggle_edit_mode() ;
	return 0L ;
#endif
}




/** Toggle status bar visibility.
*/
LRESULT CMainFrame::on_view_status_bar(  WindowsMessage &message )
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
LRESULT CMainFrame::on_view_match(  WindowsMessage &message ) 
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
LRESULT CMainFrame::on_view_search(  WindowsMessage &message ) 
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
LRESULT CMainFrame::show_about_dialog(WindowsMessage &)
{
	SENSE("show_about_dialog") ;

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
LRESULT CMainFrame::on_help(WindowsMessage &)
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
LRESULT CMainFrame::on_help_faq( WindowsMessage &)
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
LRESULT CMainFrame::on_register_gloss(WindowsMessage &)
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
void CMainFrame::get_matches(trans_match_container &matches, search_query_params &params)
{
	const double MATCH_THRESHOLD = 0.9999 ;

	m_model->get_memories()->find_matches(matches, params) ;

	if (!params.m_place_numbers)
	{
		return ;
	}

	trans_match_container PlacedMatches ;

	foreach(search_match_ptr match, matches)
	{
		if ( match->get_score() < MATCH_THRESHOLD )
		{
			check_placement(PlacedMatches, match);
		}
	}
	// now add in all our placements
	foreach(search_match_ptr match, PlacedMatches)
	{
		matches.insert(match) ;
	}
}

/** Initializes the transaction matches for a new lookup.
*/
void CMainFrame::init_trans_matches_for_lookup( const wstring  query )
{
	m_trans_matches.clear() ;
	m_trans_matches.set_query_rich( query ) ;
	init_lookup_properties(m_props, m_trans_matches.m_params);
}

/** Look up a query string.
*/
bool CMainFrame::lookup(const wstring query)
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

	init_trans_matches_for_lookup(query);

	trans_match_container matches ;
	get_matches(matches, m_trans_matches.m_params);
	m_trans_matches.set_matches(matches) ;

	// give the user feedback
	provide_user_trans_feedback();

	set_display_state ( MATCH_DISPLAY_STATE ) ;
	show_view_content() ;

	// do glossary lookup as well
	look_up_in_glossaries( query ) ;

	return true ;
}


/** Gets the currently displayed translation.
*/
wstring CMainFrame::get_current_translation()
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
bool CMainFrame::next_match()
{
	if ( m_trans_matches.empty() )
	{
		return false ;
	}
	m_trans_matches.forward() ;

	if ( m_model->is_reverse_lookup() )
	{
		search_match_ptr match = m_trans_matches.current() ;
		record_pointer rec = match->get_record() ;
		wstring entry_source = rec->get_source_plain() ;
		// do glossary lookup as well
		look_up_in_glossaries( entry_source ) ;

	}
	show_view_content() ;

	return true ; 
}


/** Shows the previous match.
*/
bool CMainFrame::prev_match()
{
	if ( m_trans_matches.empty() )
	{
		return false ;
	}
	m_trans_matches.back() ;

	if ( m_model->is_reverse_lookup() )
	{
		search_match_ptr match = m_trans_matches.current() ;
		record_pointer rec = match->get_record() ;
		wstring entry_source = rec->get_source_plain() ;
		// do glossary lookup as well
		look_up_in_glossaries( entry_source ) ;

	}

	show_view_content() ;

	return true ; 
}


/** Gets the current query.
*/
wstring CMainFrame::get_current_query()
{
	if (  m_model->is_reverse_lookup() ) 
	{
		return m_trans_matches.get_source_rich() ;
	}

	return m_trans_matches.get_query_rich() ;
}


/** Sets the translation for the current query.
*/
bool CMainFrame::set_translation( const wstring translation)
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
		if ( record->get_source_plain().length() <= m_props->m_gloss_props.get_max_add() 
			&& m_props->m_gloss_props.get_max_add() > 0) 
		{
			if ( false == m_glossary_windows.empty() )
			{
				record_pointer gloss_record = record_pointer(record->clone()) ;
				get_glossary_window()->add_record( gloss_record ) ;
			}
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
bool CMainFrame::register_trans_to_glossary(const wstring trans)
{
	if ( m_trans_matches.m_params.m_rich_source.empty() )
	{
		MessageBox( resource_string( IDS_NO_QUERIES ), resource_string( IDS_INVALID_ACTION ), MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND ) ;
		return false ;
	}
	record_pointer record ;
	record->set_source( m_trans_matches.m_params.m_rich_source ) ;
	record->set_trans( trans ) ;
	record->create() ;

	if ( false == m_glossary_windows.empty() )
		get_glossary_window()->add_record( record ) ;

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
void CMainFrame::delete_current_translation()
{
	on_user_delete( m_view_state->get_current() ) ;
}


/** Get concordance for query_string.
* Concordances are co-locations of query_string in the source
* fields of memory records.
*/
bool CMainFrame::get_concordances(const wstring query_string )
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
	m_search_matches.set_query_rich( query_string ) ;

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
wstring CMainFrame::get_glossary_entry(short index)
{
	if ( false == m_glossary_windows.empty() )
	{
		return get_glossary_window()->get_glossary_entry( index ) ;
	}
	return wstring( ) ;
}

/** Get the score for a given match.
* If index is -1, returns the score for the current match.
*/
double CMainFrame::get_score( const short index )
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
bool CMainFrame::gloss_add_record( record_pointer rec )
{
	add_record(rec) ;

	return true ;
}

/** Make sure that we aren't saving the glossary to a filename
* already taken by a memory.
*/
INT_PTR CMainFrame::gloss_check_save_location( memory_pointer mem )
{
	memory_iterator pos ;

	const CString mem_loc = mem->get_location() ;

	memory_list &memories = m_model->get_memories()->get_memories() ;
	for ( pos = memories.begin() ; pos != memories.end() ; ++pos )
	{
		memory_pointer my_mem = *pos ;
		if ( 0 == mem_loc.CompareNoCase( my_mem->get_location() ) && mem->get_id() != my_mem->get_id() )
		{
			// Todo: make this a custom dialog
			CString prompt ;
			prompt.FormatMessage( IDS_PROMPT_OVERWRITE_MEMORY, (LPCTSTR)mem->get_location( ) ) ;
			return (LRESULT)MessageBox( prompt, 
				resource_string( IDS_PROMPT_OVERWRITE_MEMORY_TITLE ), 
				MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_SETFOREGROUND ) ;
		}
	}

	return IDYES ;
}

/** Do a search using the parameters in the find dialog.
* The user has clicked "Find" in the find dialog.
*/
LRESULT CMainFrame::on_user_search(WindowsMessage &)
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
LRESULT CMainFrame::on_user_edit_replace(WindowsMessage &)
{
	SENSE("on_user_edit_replace") ;

	m_edit_find.set_search_params( m_edit_replace.get_search_params() ) ;

	return 0 ;
}


/** The user has told us to edit an entry.
* If the index is out of range, call up the add record dialog instead.
*/
LRESULT CMainFrame::on_user_edit(WindowsMessage &message)
{
	SENSE("on_user_edit") ;

	const size_t num = static_cast<size_t>(message.lParam) ;
	m_view_state->set_current(num) ;
	m_view_state->on_user_edit() ;

	return 0L ;
}


/** Responds to user command to delete memory entry.
*/
LRESULT CMainFrame::on_user_delete(size_t num )
{
	SENSE("on_user_delete") ;

	m_view_state->delete_match(num) ;
	return 0L ;
}


/** Shows register glossary entry dialog.
*/
LRESULT CMainFrame::on_user_register(LPARAM num )
{
	SENSE("on_user_register") ;
	record_pointer rec = get_reg_gloss_record(static_cast<size_t>(num));

	m_reg_gloss_dlg.set_record( rec ) ;

	if ( m_glossary_windows.empty() ) 
	{
		add_glossary_window(gloss_window_pointer(new CGlossaryDialog(m_props))) ;
	}

	ATLASSERT ( m_glossary_windows.empty() == false ) ; 
	m_reg_gloss_dlg.set_gloss_window(*m_glossary_windows.begin()) ;

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
bool CMainFrame::add_record( const record_pointer record )
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
gloss_window_pointer CMainFrame::get_glossary_window()
{
	if ( m_glossary_windows.empty() )
	{
		add_glossary_window(gloss_window_pointer(new CGlossaryDialog(m_props))) ;
	}
	return m_glossary_windows[0] ;
}


/** Gets the glossary window with focus
*/
int CMainFrame::get_focus_glossary(HWND focus_hwnd)
{
	ATLASSERT( ::IsWindow( focus_hwnd ) ) ;

	const size_t num_glossaries = m_glossary_windows.size() ;
	for ( size_t index=0 ; index<num_glossaries ; ++index )
	{
		if ( m_glossary_windows[index]->IsWindow() )
		{
			if ( 
				m_glossary_windows[index]->m_hWnd == focus_hwnd || 
				m_glossary_windows[index]->IsChild( focus_hwnd ) 
				)
			{
				return index ;
			}
		}
	}

	return -1 ;
}

/** Show the view content.
*/
void CMainFrame::show_view_content()
{

	if (! IsWindow())
	{
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

CMainFrame::MERGE_CHOICE CMainFrame::check_empty_on_load()
{
	if ( m_model->get_memories()->empty() ) 
	{
		return MERGE_CHOICE_SEPARATE ;
	}

	user_feedback( IDS_MEMORY_OPEN ) ;

	memory_pointer mem = m_model->get_first_memory() ;

	CQueryMergeDlg dlg(IDS_MERGE_MEM_TITLE, 
		IDS_MERGE_MEM_TEXT, 
		file::CPath(mem->get_location()).Path()) ;

	return get_merge_choice(dlg);
}


/** Get the translation for the entry at index.
*/
wstring CMainFrame::get_translation_at(short index)
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
void CMainFrame::report_memory_after_load(size_t original_num)
{
	memory_pointer mem = m_model->get_first_memory() ;

	// get number of records loaded
	const size_t num_records_after_load = mem->size() ;
	const size_t num_records_loaded = num_records_after_load - original_num ;
	CString arg1 ;
	arg1.Format( _T("%d"), num_records_loaded ) ;
	CString message ;
	message.FormatMessage( IDS_MSG_ADDED_RECORDS, arg1, file::name( get_location() ).file_name() ) ;
	user_feedback( message ) ; 
}

//! Destroy all the glossary windows (prior to shutdown).
void CMainFrame::destroy_all_gloss_windows()
{
	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		if (gloss->IsWindow())
		{
			gloss->DestroyWindow() ;
		}
	}
	m_glossary_windows.clear() ;
}


/** Handles user navigation.
*/
LRESULT CMainFrame::on_user_nav(const LPARAM lParam )
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
LRESULT CMainFrame::on_user_add_to_glossary(const LPARAM lParam )
{
	SENSE("on_user_add_to_glossary") ;

	const size_t index = static_cast<size_t>(lParam) ;
	m_view_state->set_current(index) ;
	record_pointer rec = m_view_state->get_current_match()->get_record();

	get_glossary_window()->add_record(rec->clone());
	return 0L ;
}


//! Exit without showing confirmation dialogs. 
bool CMainFrame::exit_silently()
{
	SENSE("exit_silently") ;

	foreach( gloss_window_pointer gloss_win, m_glossary_windows)
	{
		if (gloss_win->IsWindow())
		{
			gloss_win->exit_silently() ;
		}
	}

	memory_list memories ;
	m_model->get_memories()->get_memories_needing_saving( memories ) ;

	foreach(memory_pointer mem, memories)
	{
		mem->set_saved_flag(true) ;
	}

	PostMessage(WM_CLOSE);

	return true ;
}

/** Clears the active memory.
*/
bool CMainFrame::clear_memory()
{
	if ( ! m_model->get_memories()->empty() ) 
	{
		memory_pointer mem = m_model->get_first_memory() ;
		mem->clear_memory() ;
	}
	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->clear_memory() ;
	}

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

/** Responds to Tools > Preferences menu selection.
*/
LRESULT CMainFrame::on_tools_preferences(WindowsMessage &)
{
	SENSE("on_tools_preferences") ;

	user_feedback( IDS_SETTING_PREFS );

#ifdef UNIT_TEST
	return 0L ;
#else
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

	return 0L ;
#endif
}


/* This generates a divide-by-zero exception for
* testing purposes. It tests the exception logging and reporting
* feature.
*/
LRESULT CMainFrame::on_test_exception(WindowsMessage &)
{
	logging::log_debug("Generating test exception") ;
	int x = 3 ;
	return (LRESULT)(5 / (x - 3)) ;
}

/** Responds to Tools > Language menu selection.
* Toggles UI language between Japanese and English.
*/
LRESULT CMainFrame::on_tools_switch_language(WindowsMessage &)
{
	SENSE("on_tools_switch_language") ;

#ifdef UNIT_TEST
	return 0L ;
#else
	ATLASSERT( ! m_min_view.IsWindow() || ! m_min_view.IsWindowVisible() ) ;

	switch( m_appstate.m_preferred_gui_lang )
	{
	case LANG_JAPANESE :
		SetUILanguage( LANG_ENGLISH ) ;
		break ;
	case LANG_ENGLISH :
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
LRESULT CMainFrame::handle_find()
{
	init_find_window( SW_RESTORE, IDS_MEMORY_SEARCH ) ;

	return 0L ;
}

/** Edit the current translation match.
*/
LRESULT CMainFrame::on_edit_entry(WindowsMessage &)
{
	SENSE("on_edit_entry") ;

	WindowsMessage message( NULL, 0, 0, m_trans_matches.current_pos() ) ;
	on_user_edit( message ) ;
	return 0L ;
}
/** Show the add entry dialog.
*/
LRESULT CMainFrame::on_add(WindowsMessage &)
{
	SENSE("on_add") ;
	show_edit_dialog_for_new_entry( IDS_ADD_ENTRY ) ;
	return 0L ;
}

/** Set the window title.
* Complicated, ain't it?
*/
bool CMainFrame::set_window_title()
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

	return FALSE != SetWindowText( title ) ;
}


/** Reloads the UI text elements after switching GUI languages.
*/
void CMainFrame::set_ui_to_current_language()
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
void CMainFrame::set_translation_at(short index, const wstring translation )
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
bool CMainFrame::import_tmx( const file::OpenDlgList &files, input_device_ptr input )
{
	foreach(CString filename, files.m_filenames)
	{
		import_tmx(filename, input) ;
	}

	return true ;
}

/** import a single tmx file.
*/
bool CMainFrame::import_tmx( const CString &file_name, input_device_ptr input )
{
	memory_pointer mem = m_model->get_memories()->add_memory() ;

	CTMXReader reader( mem, static_cast< CProgressListener* >( this ) ) ;

	reader.load_tmx_memory( file_name, input ) ;

	// if we failed to load any entries, remove the memory
	if ( mem->empty() ) 
	{
		m_model->get_memories()->remove_memory_by_id( mem->get_id() ) ;
		user_feedback( get_load_failure_msg(file_name) ) ;
		return false ;
	}

	mem->set_location(file_name) ;

	// give feedback
	feedback_loaded_mem( mem );
	m_mru.AddToList( file_name ) ;
	mem->set_is_memory(true) ;

	set_window_title() ;
	return true ;

}

/** Give feedback for a loaded memory.
*/
void CMainFrame::feedback_loaded_mem( const memory_pointer mem )
{
	const CString file_name = mem->get_location() ;
	CString msg ;
	msg.FormatMessage( IDS_DONE_LOADING, int_arg(mem->size()), file::name( file_name ).file_name() ) ;
	user_feedback( msg ) ;

}
/** Imports a list of Trados memories.
*/
bool CMainFrame::import_trados( const file::OpenDlgList &files )
{
	try
	{
		foreach(CString filename, files.m_filenames)
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
bool CMainFrame::import_trados(const CString &trados_file_name)
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

	memory_pointer mem = m_model->get_memories()->add_memory() ;

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
		int_arg.Format( _T("%d"), mem->size() ) ;
		CString msg ;
		msg.FormatMessage( IDS_DONE_LOADING, int_arg, file::name( trados_file_name ).file_name() ) ;
		user_feedback( msg ) ;
		mem->set_location( trados_file_name ) ;
		mem->set_is_memory(true) ;
		set_window_title() ;
	}

	return true ;
}


/** Loads a memory.
*/
bool CMainFrame::load(const CString file_name, const bool check_empty )
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
		// success
		::PostMessage( m_hWnd, WM_COMMAND, MAKEWPARAM( IDC_DEMO_CHECK_EXCESS, 100 ), 0 ) ;
		m_mru.AddToList( file_name ) ;
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
bool CMainFrame::lookup_trans(const wstring query)
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
	m_model->get_memories()->find_trans_matches( matches, m_trans_matches.m_params ) ;

	m_trans_matches.set_matches( matches ) ;

	if ( m_trans_matches.empty() == false )
	{
		m_model->set_reverse_lookup(true)  ;
		search_match_ptr match = m_trans_matches.current() ;
		record_pointer rec = match->get_record() ;
		// do glossary lookup as well
		look_up_in_glossaries( rec->get_source_plain() ) ;
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
bool CMainFrame::correct_trans(const wstring trans)
{
	if ( m_model->get_memories()->empty() )
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
		m_trans_matches.set_query_rich(trans) ;

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
bool CMainFrame::get_translation_concordances(const wstring query_string)
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
LRESULT CMainFrame::on_source_concordance(WindowsMessage &)
{
	SENSE("on_source_concordance") ;

	get_concordances(m_view_interface.get_selection_text()) ;
	return 0 ;
}

/** Ctrl + Alt + C.
* Get concordance in the translation fields.
*/
LRESULT CMainFrame::on_trans_concordance(WindowsMessage &)
{
	SENSE("on_trans_concordance") ;

	get_translation_concordances(m_view_interface.get_selection_text()) ;
	return 0 ;
}

/** Help -> Register.
* Show the register dialog.
*/
LRESULT CMainFrame::on_help_register(WindowsMessage &)
{
	SENSE("on_help_register") ;

#ifdef UNIT_TEST
	return 0L ;
#else

	// prompt him!
	CInputKeyDlg input_key_dlg ;

	if ( IDOK == input_key_dlg.DoModal( ) )
	{
		MessageBox( resource_string( IDS_REGISTERED_USER ), resource_string( IDS_REGISTERED_USER_TITLE ) ) ;
		user_feedback( IDS_REGISTERED_USER_TITLE ) ;
	}
	else
	{
		try
		{
			if ( this->is_demo() ) 
			{
				MessageBox( resource_string( IDS_NOT_REGISTERED_USER ), resource_string( IDS_NOT_REGISTERED_USER_TITLE ) ) ;
				user_feedback( IDS_NOT_REGISTERED_USER_TITLE ) ;
			}
		}
		catch(...)
		{
			logging::log_error("An error occurred while checking user registration") ;
			user_feedback( IDS_NOT_REGISTERED_USER_TITLE ) ;
			set_window_title() ;
			return -1 ;
		}
	}

	set_window_title() ;
	return 0L ;
#endif
}


/** Toggle markup (matches) between on and off.
*/
LRESULT CMainFrame::on_user_toggle_markup(WindowsMessage &)
{
	SENSE("on_user_toggle_markup") ;

	m_props->m_gen_props.read_from_registry() ;

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

	m_props->m_gen_props.write_to_registry() ;

	return 0L ;
}

/** File -> Save.
* The user wants to save the top memory.
*/
LRESULT CMainFrame::on_user_save(WindowsMessage &message)
{
	SENSE("on_user_save") ;

	on_file_save(message) ;
	foreach( gloss_window_pointer gloss_win, m_glossary_windows)
	{
		if ( gloss_win->IsWindow())
		{
			gloss_win->on_file_save() ;
		}
	}
	return 0L ;
}

/** Respond to dropped files.
*/
LRESULT CMainFrame::on_drop(WindowsMessage &message)
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
		load(filename) ;
	}

	return 0L ;
}

/** Tools -> Manage Memories.
* Shows memory manager dialog.
*/
LRESULT CMainFrame::on_tools_memory_manager(WindowsMessage &)
{
	SENSE("on_tools_memory_manager") ;

	if (m_props->m_gen_props.m_data.m_old_mem_mgr)
	{
		m_old_manager_window.set_memories(this->get_model()->get_memories()) ;
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


LRESULT CMainFrame::on_toggle_views(WindowsMessage &)
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
				m_model->get_memories()->find_trans_matches( matches, m_trans_matches.m_params ) ;
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
LRESULT CMainFrame::on_view_switch(WindowsMessage &)
{
	SENSE("on_view_switch") ;

	if (m_glossary_windows.empty())
	{
		return 0L ;
	}

	get_glossary_window()->SetFocus() ;

	return 0L ;
}

/** Loop through the glossary views (which has focus).
*/
void CMainFrame::gloss_view_switch(HWND child)
{
	gloss_window_iterator pos ;
	for ( pos = m_glossary_windows.begin() ; pos != m_glossary_windows.end() ; ++pos )
	{
		gloss_window_pointer gloss = *pos ;
		if ( gloss->m_hWnd == child )
		{
			++pos ;
			while ( pos != m_glossary_windows.end() )
			{
				gloss = *pos ;
				if ( gloss->IsWindow() )
				{
					gloss->SetFocus() ;
					return ;
				}
			}
			SetFocus() ;
			return ;
		}
	}

	SetFocus() ;
	return ;
}

/** View -> Compact View.
* Hide the min view.
*/
LRESULT CMainFrame::on_user_view_min_end(WindowsMessage &)
{
	SENSE("on_user_view_min_end") ;

	ShowWindow( SW_SHOW ) ;

	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		if ( gloss->IsWindow() )
		{
			gloss->ShowWindow( SW_SHOW ) ;
		}
	}

	show_view_content() ;

	return 0L ;
}

/** View -> Compact View.
* Show the min view.
*/
LRESULT CMainFrame::on_view_min_begin( WindowsMessage &)
{
	const int DEFAULT_SIZE = 100 ;

	SENSE("on_view_min_begin") ;

	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		if ( gloss->IsWindow() )
		{
			gloss->ShowWindow( SW_HIDE ) ;
		}
	}

	ShowWindow( SW_HIDE ) ;

	if( ! m_min_view.IsWindow() )
	{
		m_min_view.set_parent(*this) ;
		m_min_view.Create( NULL ) ;
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
void CMainFrame::put_visible(int visibility)
{
	if ( m_min_view.IsWindow() && m_min_view.IsWindowVisible() )
	{
		m_min_view.ShowWindow( visibility ) ;
	}
	else
	{
		ShowWindow( visibility ) ;
	}

	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->ShowWindow(visibility) ;
	}
}

/** Load the appropriate resource file according to the 
* language code.
*/
void CMainFrame::SetUILanguage(WORD lang_id)
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
WORD CMainFrame::get_current_gui_language()
{
	return m_appstate.m_preferred_gui_lang ;
}

/** Called after the preference dialog is exited with "OK".
*/
void CMainFrame::reflect_preferences()
{
	m_props->read_from_registry() ;

	foreach( gloss_window_pointer gloss_win, m_glossary_windows)
	{
		gloss_win->show_view_content() ;
	}

	set_bg_color( static_cast< COLORREF >( m_props->m_view_props.m_data.m_back_color ) );	
	m_trans_matches.m_query_color =	m_props->m_view_props.m_data.m_query_color  ;
	m_trans_matches.m_source_color = m_props->m_view_props.m_data.m_source_color  ;
	m_trans_matches.m_trans_color =	m_props->m_view_props.m_data.m_trans_color ;
}


/** This is called with a PostMessage from OnCreate.
*/
LRESULT CMainFrame::on_check_demo(WindowsMessage &)
{
	SENSE("on_check_demo") ;
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
	if (! m_props->m_gen_props.m_data.m_first_launch)
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
	ATLASSERT(m_props->m_gen_props.m_data.m_first_launch) ;
	logging::log_debug("Doing first-launch checks") ;
	SetFocus() ;
	if ( ! ::GetFocus())
	{
		return 0L ;
	}

	if ( ! this->is_demo() )
	{
		return 0L ;
	}

	// Demo version on first launch: nag him!
	CNagDialog nagger ;
	if ( IDCANCEL == nagger.DoModal( ) )
	{
		return 0L ;
	}

	// User wants to register: prompt him!
	CInputKeyDlg input_key_dlg ;
	INT_PTR	input_key_result = input_key_dlg.DoModal( ) ;

	if ( IDOK == input_key_result )
	{
		user_feedback( IDS_REGISTERED_USER_TITLE ) ;
		MessageBox( resource_string( IDS_REGISTERED_USER ), resource_string( IDS_REGISTERED_USER_TITLE ) ) ;
	}
	else
	{
		user_feedback( IDS_NOT_REGISTERED_USER_TITLE ) ;
		MessageBox( resource_string( IDS_NOT_REGISTERED_USER ), resource_string( IDS_NOT_REGISTERED_USER_TITLE ) ) ;
	}

	set_window_title() ;
	SetFocus() ;

	return 0L ;
#endif
}

/** Adds a memory.
*/
void CMainFrame::add_memory(memory_pointer mem)
{
	m_model->get_memories()->insert_memory( mem ) ;

	m_view_interface.set_text( wstring() ) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;

	set_window_title() ;
}

/** If we don't have a size saved, use the default.
*/
void CMainFrame::set_up_default_initial_size()
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
void CMainFrame::set_up_command_bars()
{
	// Set the 24-bit color images in the toolbar's image list
	std::vector< int > commands ;
	commands += 
		ID_FILE_NEW,	ID_FILE_OPEN, ID_MEMORY_CLOSE,	SEP_ID,
		ID_FILE_SAVE,	ID_FILE_SAVE_ALL,	SEP_ID,
		ID_NEXT_PANE,	SEP_ID,
		ID_EDIT_CUT,	ID_EDIT_COPY,	ID_EDIT_PASTE,		SEP_ID,
		ID_EDIT_FIND,	
		ID_TOOLS_PREFERENCES,	SEP_ID,
		ID_HELP,		ID_APP_ABOUT;
	std::vector< int > StdBitmaps ;
	StdBitmaps += 
		IDB_NEW_DOCUMENT,	IDB_OPEN, IDB_MEMORY_CLOSE,
		IDB_SAVE,	IDB_SAVEMANY,
		IDB_SWITCH_VIEWS,
		IDB_CUT,			IDB_COPY,	IDB_PASTE,
		IDB_SEARCH,			
		IDB_PROPERTIES,
		IDB_HELP,			IDB_INFORMATION ;


#ifdef UNIT_TEST
	return ;
#else
	TWindow toolbarWnd = m_stdToolbar.Create24BitToolBarCtrl(*this, commands, FALSE );

	m_stdToolbar.SubclassWindow( toolbarWnd, MAKEINTRESOURCE(IDR_MAINFRAME));
	m_stdToolbar.SetBitmapSize(BM_SIZE, BM_SIZE) ;

	CImageList images ;
	images.Create(BM_SIZE, BM_SIZE, ILC_COLOR24 | ILC_MASK, 0, StdBitmaps.size() + 1 ) ;
	foreach(int img_id, StdBitmaps)
	{
		CBitmap bmp ;
		bmp.LoadBitmap(img_id) ;
		images.Add(bmp, MAGENTA) ;
	}
	m_stdToolbar.SetImageList(images) ;

	// create command bar window
	m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	m_CmdBar.RemoveAllImages() ;
	m_CmdBar.SetImageMaskColor(MAGENTA) ;
	m_CmdBar.SetImageSize(BM_SIZE, BM_SIZE) ;

	// attach menu
	ATLVERIFY(m_CmdBar.AttachMenu(GetMenu())) ;

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

//! Add a bitmap menu item.
void CMainFrame::AddMenuBitmap( const int BitmapId, const int CmdId ) 
{
	CBitmap bmp ;
	bmp.LoadBitmap( BitmapId ) ;
	m_CmdBar.AddBitmap( bmp, CmdId  ) ;
}

/** Load the MRU.
*/
void CMainFrame::set_up_recent_docs_list()
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
void CMainFrame::init_status_bar()
{
#ifdef UNIT_TEST
	return ;
#else
	// create the status bar
	ATLVERIFY(CreateSimpleStatusBar());
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
	version_info.Format(_T("Felix v. %s"), string2tstring(VERSION).c_str()) ;
	user_feedback(version_info, 1) ;
#endif
}

/** Set up the various menu checkmarks and such.
*/
void CMainFrame::set_up_ui_state()
{
	if( m_appstate.m_is_toolbar_visible )
	{
		ATLVERIFY(UISetCheck(ID_VIEW_TOOLBAR, TRUE)) ;
	}
	else
	{
		ATLVERIFY(UISetCheck(ID_VIEW_TOOLBAR, FALSE)) ;
		ATLASSERT( ::IsWindow( m_hWndToolBar ) ) ;

		CReBarCtrl rebar = m_hWndToolBar ;
		const int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1) ;	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, FALSE ) ;
	}


	if( m_appstate.m_is_statusbar_visible )
	{
		ATLVERIFY(UISetCheck(ID_VIEW_STATUS_BAR, TRUE)) ;
	}
	else
	{
		// Returns: If the window was previously visible, the return value is nonzero.
		::ShowWindow(m_hWndStatusBar, SW_HIDE ) ;

		ATLVERIFY(UISetCheck(ID_VIEW_STATUS_BAR, FALSE)) ;
	}

}

/** Set up the mainframe window size.

If we have already run the app, we will have persisted the app state in the registry.
If not, we will set the window size to the default.

CReBarSettings is used to save/load the command bar settings in the registry.
*/
void CMainFrame::set_up_window_size()
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

/** Look up the query in all of our glossary windows.
*/
void CMainFrame::look_up_in_glossaries(const wstring query)
{
	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		ATLASSERT(gloss->IsWindow()) ;
		if ( gloss->IsWindow() )
		{
			gloss->handle_glossary_lookup( query ) ;
		}
	}
}

/** Add the record to the top memory, and give feedback.
*/
void CMainFrame::add_record_to_memory(record_pointer record)
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

	CString content = resource_string( IDS_ADDED_TRANSLATION ) ;
	content += _T(" ") ;

	CNumberFmt fm ;
	CString msg ;
	msg.FormatMessage( IDS_CURRENT_SIZE, resource_string( IDS_MEMORY ), fm.Format( mem->size() ) ) ;
	content += msg ;
	user_feedback( content ) ;
}

/** Perform the lookup again. This is needed after we've edited the record.
*/
void CMainFrame::redo_lookup( search_match_ptr match, bool do_gloss )
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
		matcher.get_trans_score(query, 
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
		look_up_in_glossaries( search_segment ) ;
	}
}

/** Tell the user how many matches we found.
*/
void CMainFrame::provide_user_search_feedback()
{
	match_count_feedback(m_search_matches.size()) ;
}

/** Provides feedback on found matches.
*/
void CMainFrame::provide_user_trans_feedback()
{
	match_count_feedback(m_trans_matches.size()) ;
}

/** Template: Found x match(es).
*/
void CMainFrame::match_count_feedback(size_t num)
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
void CMainFrame::perform_user_search()
{
	m_search_matches.clear() ;
	m_search_matches.m_params = m_find.get_search_params() ;

	perform_concordance_search(m_search_matches.m_params) ;
}
/** This is kind of a useless method, isn't it?
*/
void CMainFrame::show_user_search_results()
{
	set_display_state ( CONCORDANCE_DISPLAY_STATE );
	show_view_content() ;
	m_view_interface.set_scroll_pos(0) ;
}

/** Refresh the glossary window text after switching GUI languages.
*/
void CMainFrame::set_lang_of_gloss_windows()
{
	// now set each of our glossaries...
	const size_t num_glossaries = m_glossary_windows.size() ;
	for ( size_t i=0 ; i<num_glossaries ; ++i )
	{
		if ( m_glossary_windows[i]->IsWindow() )
		{
			m_glossary_windows[i]->set_ui_language() ;
		}
	}

}
/** Reload the menu bar after switching GUI languages.
*/
void CMainFrame::refresh_command_bar()
{
	HINSTANCE h = _Module.GetResourceInstance() ;
	HMENU menu = ::LoadMenu( h, MAKEINTRESOURCE( IDR_MAINFRAME ) ) ;
	ATLASSERT( menu != NULL ) ;
	m_CmdBar.AttachMenu( menu );

	refresh_mru_doc_list(menu);
}

/** We need to reload the MRU list after switching GUI languages.
*/
void CMainFrame::refresh_mru_doc_list(HMENU menu)
{
	// write our recent docs
	m_mru.WriteToRegistry( resource_string( IDS_REG_KEY ) );
	// reset the menu handle for the recent docs list...
	m_mru.SetMenuHandle(::GetSubMenu(menu, 0));
	// read our doc history back for the new menu
	m_mru.ReadFromRegistry( resource_string( IDS_REG_KEY ) );
}

/** This is so we can call this from CCommonWindowFunctionality
* 
* Todo: localize
* \see
* CCommonWindowFunctionality | CGlossaryDialog
*/
LPCTSTR CMainFrame::get_save_filter()
{
	return get_mem_save_filter() ;
}

/** This is so we can call this from CCommonWindowFunctionality
* 
* Todo: localize
* \see
* CCommonWindowFunctionality | CGlossaryDialog
*/
LPCTSTR CMainFrame::get_open_filter()
{
	return get_mem_open_filter() ;
}



/** Allow the glossary windows to tell us not to shut down.
*/
bool CMainFrame::gloss_win_shutdown_check()
{
	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		if ( false == gloss->pre_shutdown_save_check() )
		{
			SetMsgHandled( TRUE ) ;
			return false ;
		}
	}

	return true ;
}


/** Saves the rebar settings to the register, so we can remember them
* at next startup. (We're just nice that way)
*/
void CMainFrame::save_rebar_settings()
{
	CReBarCtrl rbc = m_hWndToolBar;
	ATLASSERT(rbc.IsWindow()) ;

	CReBarSettings rs;
	rs.GetFrom(rbc);
	rs.Save( resource_string(IDS_REG_KEY), _T("ReBar") );
}

/** Sets whether to show markup for matches.
*/
void CMainFrame::put_show_marking( const VARIANT_BOOL setting )
{
	if ( setting == VARIANT_FALSE ) 
	{
		m_trans_matches.m_params.m_show_marking = false ;
	}
	else
	{
		m_trans_matches.m_params.m_show_marking = true ;
	}

	foreach(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->put_show_marking(setting) ;
	}

	show_view_content() ;
}

/** Returns whether markup is being shown.
*/
VARIANT_BOOL CMainFrame::get_show_marking()
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
bool CMainFrame::set_location( const CString &location ) 
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
		m_model->get_memories()->remove_memory_by_id( mem->get_id() ) ;
	}
	return false ;
}

/** Get the type string for the memory window (e.g. "Memory").
*/
CString CMainFrame::get_window_type_string() 
{
	return resource_string(IDS_MEMORY) ;
}




//! This is our poor-man's multithreading.
void CMainFrame::init_background_processor()
{
	m_background_processor.set_accelerator(m_hAccel) ;
	m_background_processor.set_hwnd(*this) ;
}

//! Load color preferences from registry.
void CMainFrame::init_item_colors()
{
	app_props::properties_view::props_data *prop_data = &m_props->m_view_props.m_data ;

	m_trans_matches.m_query_color = (COLORREF)prop_data->m_query_color ;
	m_trans_matches.m_source_color = (COLORREF)prop_data->m_source_color ;
	m_trans_matches.m_trans_color = (COLORREF)prop_data->m_trans_color ;
}

//! See if we want to save our memory/glossary loaded history.
BOOL CMainFrame::should_save_memory_history()
{
	app_props::properties_general gen_props ;
	gen_props.read_from_registry() ;

	return gen_props.m_data.m_load_prev_mem_on_startup;
}

//! Respond to italic command.
LRESULT CMainFrame::on_italic(WindowsMessage &)
{
	SENSE("on_italic") ;
	return CCommonWindowFunctionality::on_italic( ) ;
}

//! Respond to an underline command.
LRESULT CMainFrame::on_underline(WindowsMessage &)
{
	SENSE("on_underline") ;
	return CCommonWindowFunctionality::on_underline( ) ;
}

//! Respond to bold command.
LRESULT CMainFrame::on_bold(WindowsMessage &)
{
	SENSE("on_bold") ;
	return CCommonWindowFunctionality::on_bold( ) ;
}

//! Save all open memory files.
LRESULT CMainFrame::on_file_save_all(WindowsMessage &)
{
	SENSE("on_file_save_all") ;
	return CCommonWindowFunctionality::on_file_save_all( ) ;
}

//! Replace the record with the one beind edited.
LRESULT CMainFrame::on_user_replace_edit_record(WindowsMessage &)
{
	SENSE("on_user_replace_edit_record") ;
	return CCommonWindowFunctionality::on_user_replace_edit_record( ) ;
}

//! Make sure that we haven't exceeded the demo constraints.
LRESULT CMainFrame::on_demo_check_excess_memories(WindowsMessage &)
{
	SENSE("on_demo_check_excess_memories") ;
	return CCommonWindowFunctionality::on_demo_check_excess_memories( ) ;
}

//! User wants to search in edit mode.
LRESULT CMainFrame::on_user_edit_search(WindowsMessage &message)
{
	SENSE("on_user_edit_search") ;
	return CCommonWindowFunctionality::on_user_edit_search( message.lParam ) ;
}

/* 
* Get the appropriate record as basis for registering glossary entries.
* If the index is out of range, then just return an empty record. 
* The user should be able to register glossary entries without a template record.
*/
mem_engine::record_pointer CMainFrame::get_reg_gloss_record( const size_t num )
{
	m_view_state->set_current(num) ;
	return m_view_state->get_current_match()->get_record() ;
}

//! Open a file from the MRU list
LRESULT CMainFrame::on_mru_file_open(WindowsMessage &message)
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
BOOL CMainFrame::ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD /*dwMsgMapID*/ /*= 0*/ )
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

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>) ;

#ifndef UNIT_TEST
		CHAIN_MSG_MAP(frame_class) ;
#endif
	}
	catch( CDemoException &e)
	{
		logging::log_error("A demo exception was thrown.") ;
		logging::log_exception(e) ;
		if (e.notify_user(_T("This is the Demo version of Felix")) == IDOK)
		{
			// prompt him!
			CInputKeyDlg input_key_dlg ;

			if ( IDOK == input_key_dlg.DoModal( ) )
			{
				MessageBox( resource_string( IDS_REGISTERED_USER ), resource_string( IDS_REGISTERED_USER_TITLE ) ) ;
				user_feedback( IDS_REGISTERED_USER_TITLE ) ;
				set_window_title() ;
			}
			else
			{
				SetMsgHandled(FALSE) ;
				return -1L ;
			}
		}

	}
	catch ( CSWException &sw_e ) 
	{ 
		logging::log_error("Structured Windows Exception") ;
		logging::log_exception(sw_e) ;
		CString language = L"English" ;
		const CString lang_code = resource_string(IDS_LANG_CODE);
		if(lang_code == L"jp")
		{
			language = L"Japanese" ;
		}
		logging::send_report(language, sw_e.get_filename()) ;
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
UINT CMainFrame::get_message_key( UINT message, WPARAM wParam )
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
std::map< UINT, boost::function< LRESULT( WindowsMessage& ) >  > * CMainFrame::get_message_map( UINT message )
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
void CMainFrame::recalculate_match( search_match_ptr match, search_query_params &params )
{
	// initialize the match
	match->MatchPairing().clear() ;
	match->set_values_to_record() ;

	m_trans_matches.m_params = params ;
	this->redo_lookup(match, false) ;
}

//! File -> Connect
LRESULT CMainFrame::on_file_connect( UINT, int, HWND )
{
	CConnectionDlg dlg(&m_props->m_mem_props) ;
	if (IDCANCEL == dlg.DoModal(*this))
	{
		return 0L ;
	}
	memory_pointer mem = dlg.m_memory ;
	m_model->get_memories()->insert_memory(mem) ;

	user_feedback(system_message(IDS_CONNECTED_MEMORY, (LPCTSTR)mem->get_location())) ;

	this->set_window_title() ;
	return 0L ;
}

//! Set the background color unless it's white (the default)
void CMainFrame::set_bg_color_if_needed()
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
LRESULT CMainFrame::OnToolTipTextW( int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	if( m_toolmap.empty() )
	{
		init_tooltip_map(m_toolmap);

		m_toolmap[ID_EDIT_FIND] = IDS_SEARCH_TOOLBAR ;
		m_toolmap[ID_MEMORY_CLOSE] = IDS_MEMORY_CLOSE ;
		m_toolmap[ID_FILE_SAVE] = IDS_SAVE_MEMORY ;
		m_toolmap[ID_FILE_SAVE_ALL] = IDS_SAVE_ALL_MEMORIES ;
	}

	handle_tooltip(pnmh, idCtrl, m_toolmap);

	return 0L;
}

//! Tell the user that we failed to load the memory
void CMainFrame::load_failed_feedback( const CString & file_name )
{
	// we failed to load the memory
	user_feedback( get_load_failure_msg(file_name) ) ; //
}

//! Tell the user that we're loading a file
void CMainFrame::loading_file_feedback( const CString & file_name )
{
	const file::CPath path( file_name ) ;
	user_feedback(system_message( IDS_MSG_LOADING, path.FindFileName()));
}

/** Loads a Felix memory.
*/
bool CMainFrame::load_felix_memory( bool check_empty, const CString & file_name )
{
	MERGE_CHOICE should_merge = MERGE_CHOICE_SEPARATE ;
	if ( check_empty )
	{
		should_merge = check_empty_on_load() ;
		if (should_merge == MERGE_CHOICE_CANCEL)
		{
			return true ;
		}
	}

	// merge or add?
	memory_pointer mem ;
	if (should_merge == MERGE_CHOICE_SEPARATE)
	{
		mem = m_model->get_memories()->add_memory() ;
	}
	else
	{
		ATLASSERT(should_merge == MERGE_CHOICE_MERGE) ;
		mem = m_model->get_first_memory() ;
	}

	mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

	try
	{
		const bool success = mem->load( file_name )  ;
		mem->set_is_memory(true) ;
		return success ;
	}
	catch ( ... ) 
	{
		logging::log_error("Failed to load memory") ;
		if (should_merge == MERGE_CHOICE_SEPARATE)
		{
			m_model->get_memories()->remove_memory_by_id( mem->get_id() ) ;
		}
		throw ;
	}
}

//! Set search params from registry settings.
void CMainFrame::init_lookup_properties( const app_props::props_ptr source, search_query_params &dest )
{
	dest.m_ignore_case = !! source->m_mem_props.m_data.m_ignore_case ;
	dest.m_ignore_width = !! source->m_mem_props.m_data.m_ignore_width ;
	dest.m_ignore_hira_kata = !! source->m_mem_props.m_data.m_ignore_hir_kat ;

	dest.m_assess_format_penalty = !! source->m_mem_props.m_data.m_assess_format_penalty ;
	dest.m_match_algo = source->m_alg_props.m_data.m_match_algo ;

	dest.m_place_numbers = !! source->m_mem_props.m_data.m_place_numbers ;
	dest.m_place_gloss = !! source->m_mem_props.m_data.m_place_gloss ;
}

//! Tell the user that we found x matches for the search string.
void CMainFrame::source_concordance_feedback()
{
	const wstring plain_text = m_search_matches.get_source_plain() ;
	concordance_feedback(plain_text, m_search_matches.size()) ;
}

//! Tell the user that we found translation x matches for the search string.
void CMainFrame::translation_concordance_feedback()
{
	const wstring plain_text = m_search_matches.get_trans_plain() ;
	concordance_feedback(plain_text, m_search_matches.size()) ;
}

//! Show concordance match count in status bar.
void CMainFrame::concordance_feedback(const wstring plain_text, size_t num)
{
	user_feedback(system_message(IDS_FOUND_X_MATCHES_FOR_STRING, 
		int_arg(num), 
		plain_text.c_str())) ;
}

//! Tell the user that we deleted a new record.
void CMainFrame::deleted_new_record_feedback()
{
	const wstring feedback = L"<center><h1>" + resource_string_w( IDS_DELETED_ENTRY ) + L"</h1></center>" ;
	m_view_interface.set_text(feedback) ;
	user_feedback(IDS_DELETED_ENTRY) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;
}

//! See if we can create a placement for this match.
void CMainFrame::check_placement( trans_match_container &PlacedMatches, 
								 search_match_ptr match )
{
	const double PLACEMENT_PENALTY = 0.00001 ;

	record_pointer rec = match->get_record() ;
	const wstring trans = rec->get_trans_plain() ;

	mem_engine::markup_ptr mark(new markup_strings) ;
	mark->SetQuery(match->MatchPairing().MarkupQuery()) ;
	mark->SetSource(match->MatchPairing().MarkupSource()) ;
	mark->SetTrans(trans) ;
	mark->SetContext(rec->get_context_plain()) ;

	std::pair< wstring, wstring > Transpair( trans, trans ) ;
	CMatchStringPairing newPairing( match->MatchPairing() ) ;
	if ( newPairing.PlaceNumbers( Transpair ) )
	{
		search_match_ptr NewMatch(new search_match) ;
		NewMatch->set_memory_id(match->get_memory_id()) ;
		NewMatch->set_memory_location(match->get_memory_location()) ;
		// record
		record_pointer NewRec = record_pointer(rec->clone()) ;
		NewRec->set_validated_off() ;
		NewRec->reset_refcount() ;
		NewRec->set_trans( Transpair.first ) ;

		NewMatch->set_record( NewRec ) ;
		NewMatch->set_values_to_record() ;

		// score
		// HACK -- to make sure that the placed matches sort below the non-placed ones
		NewMatch->set_base_score( newPairing.CalcScore() - PLACEMENT_PENALTY) ;
		NewMatch->set_formatting_penalty( match->get_formatting_penalty() ) ;

		// new query/source
		mem_engine::markup_ptr Markup = NewMatch->get_markup() ;
		Markup->SetQuery( newPairing.MarkupQuery() ) ;
		Markup->SetSource( newPairing.MarkupSource() ) ;
		Markup->SetTrans( Transpair.second ) ;

		NewMatch->Placement() ;

		PlacedMatches.insert( NewMatch ) ;
	}
}


//! Make sure that ShellExecute didn't return an error code.
void CMainFrame::check_shell_execute_result( int result, 
											const CString & filePath )
{
	const int ERR_THRESHOLD = 32 ;

	if ( result <= ERR_THRESHOLD )
	{
		throw CWinException(system_message(IDS_SHOW_HELP_FAILED, (LPCTSTR)filePath), result ) ;
	}
}

void CMainFrame::OnNavEdit( long index )
{
	SENSE("OnNavEdit") ;
	WindowsMessage message( NULL, 0, 0, index ) ;
	on_user_edit( message ) ;
}

void CMainFrame::OnNavDelete( long index )
{
	SENSE("OnNavDelete") ;
	on_user_delete( static_cast<size_t>(index) ) ;
}

//! Register glossary entries based on entry at index.
void CMainFrame::OnNavAddGloss( long index )
{
	SENSE("OnNavAddGloss") ;
	on_user_register( index ) ;
}

//! Add the entry at index to the glossary.
void CMainFrame::OnNavAddToGloss( long index )
{
	SENSE("OnNavAddToGloss") ;
	on_user_add_to_glossary( index ) ;
}

void CMainFrame::remove_record_from_mem_id( record_pointer rec, int mem_id )
{
	try
	{
		m_model->get_memories()->remove_record( rec, mem_id ) ;
	}
	catch (CProgramException& e)
	{
		logging::log_error("Failed to delete record") ;
		logging::log_exception(e) ;
		e.notify_user("Failed to delete record: memory not found") ;
	}
}

void CMainFrame::remove_match_record( search_match_ptr match )
{
	record_pointer rec = match->get_record() ;

	remove_record_from_mem_id(rec, match->get_memory_id());
	remove_record_from_glossaries(rec);
}

void CMainFrame::remove_record_from_glossaries( record_pointer rec )
{
	if (! m_glossary_windows.empty() && m_props->m_gloss_props.get_max_add())
	{
		get_glossary_window()->delete_record(rec) ;
	}
}

void CMainFrame::view_by_id( size_t recid, wstring source, wstring trans )
{

	memory_pointer mem = get_memory_model()->get_memory_by_id(m_review_match->get_memory_id()) ;
	m_review_match->set_record(mem->add_by_id(recid, source, trans)) ;
	this->set_display_state(TRANS_REVIEW_STATE) ;
	this->user_feedback(IDS_SHOW_TRANS) ;

	m_view_interface.set_text(get_review_content(mem)) ;
	check_mousewheel() ;
	m_view_interface.set_scroll_pos(0) ;
}

void CMainFrame::add_by_id( size_t recid, wstring source, wstring trans )
{
	if (source.empty() || trans.empty())
	{
		logging::log_warn("Source or translation empty in add_by_id") ;
		return ;
	}

	memory_pointer mem ;

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

mem_engine::search_match_ptr CMainFrame::get_current_match()
{
	return m_view_state->get_current_match() ;
}

// Content when using translation history
wstring CMainFrame::get_review_content( memory_pointer mem )
{
	search_match_ptr match(mem->make_match()) ;
	match->set_record(m_review_match->get_record()) ;
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

// Get the UTIL settings from our COM server.
// Got to consolidate all prefs (using boost?).
void CMainFrame::load_util_settings()
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

// Serialize prefs in COM sever.
// Got to consolidate all prefs (using boost?).
void CMainFrame::save_util_settings()
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

// Show the zoom dialog.
LRESULT CMainFrame::on_view_zoom( WindowsMessage & )
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

// Set zoom level in response to the zoom dialog, 
// or loading from preferences.
void CMainFrame::set_zoom_level( int zoom_level )
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

void CMainFrame::load_history()
{
	app_props::properties_loaded_history *history_props = &m_props->m_history_props ;
	history_props->read_from_registry() ;

	std::vector<wstring> items ;
	std::copy(history_props->m_loaded_mems.begin(), history_props->m_loaded_mems.end(), std::back_inserter(items)) ;
	std::reverse(items.begin(), items.end()) ;

	foreach(wstring filename, items)
	{
		load( filename.c_str(), false) ;
	}

	items.clear() ;
	std::copy(history_props->m_loaded_remote_mems.begin(), history_props->m_loaded_remote_mems.end(), std::back_inserter(items)) ;
	std::reverse(items.begin(), items.end()) ;

	foreach(wstring filename, items)
	{
		try
		{
			memory_remote *mem = new memory_remote(&m_props->m_mem_props) ;
			memory_pointer pmem(mem) ;
			mem->connect(filename.c_str()) ;
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
LRESULT CMainFrame::on_tools_load_preferences(WindowsMessage &)
{
	// get the file name
	open_file_dlg dialog ;

	dialog.set_prompt( R2T( IDS_LOAD_PREFS_TITLE ) ) ;

	LPCTSTR prefs_filter = get_prefs_filter() ;
	dialog.set_filter( prefs_filter ) ;

	user_feedback( IDS_LOAD_PREFS_TITLE ) ;

	CString filename = dialog.get_open_file() ;
	if (filename.IsEmpty())
	{
		user_feedback( IDS_CANCELLED_ACTION ) ;
		return 0L ;
	}

	const int selected_index = dialog.get_selected_index() ;

	switch( selected_index ) 
	{
	case 1:
		ATLTRACE( "Load new preferences format\n" ) ;
		m_props->load_file(static_cast<LPCTSTR>(filename)) ;
		break;

	case 2:
		load_old_preferences(filename);
		break;

	default:
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
	}

	return 0L ;
}

LRESULT CMainFrame::on_tools_save_preferences(WindowsMessage &)
{
	save_file_dlg dialog ;
	dialog.set_prompt( R2T( IDS_SAVE_PREFS_TITLE ) ) ;

	LPCTSTR prefs_filter = get_prefs_filter() ;
	dialog.set_filter( prefs_filter ) ;

	user_feedback( IDS_SAVE_PREFS_TITLE ) ;

	CString filename = dialog.get_save_file() ;

	if ( filename.IsEmpty() )
	{
		user_feedback( IDS_CANCELLED_ACTION ) ;
		return 0L ;
	}
	logging::log_debug("Saving preferences") ;

	if (! m_glossary_windows.empty())
	{
		gloss_window_pointer gloss = get_glossary_window();
		gloss->save_prefs() ;
	}
	save_settings_close() ;
	save_settings_destroy() ;


	const int selected_index = dialog.get_selected_index() ;

	switch( selected_index ) 
	{
	case 1:
		fileops::addExtensionAsNeeded( filename,  _T( ".fprefx" ) ) ;
		m_props->save_file(filename) ;
		ATLTRACE( "Save new preferences format\n" ) ;
		break;

	case 2:
		fileops::addExtensionAsNeeded( filename,  _T( ".fprefs" ) ) ;
		save_old_prefs_file(filename);
		break;

	default:
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
	}

	user_feedback( IDS_PREFS_SAVED ) ;

	return 0L ;
}

void CMainFrame::create_process( CString &command, CString error_message )
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process. 
	if( !CreateProcess(NULL,   // No module name (use command line). 
		command.GetBuffer(), // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		DETACHED_PROCESS,                // DETACHED_PROCESS creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
	{
		throw CWinException(error_message) ;
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}

void CMainFrame::save_settings_close()
{
	save_window_settings(_T("MainFrame"));

	save_rebar_settings() ;

	CReBarCtrl ReBar = m_hWndToolBar ;
	m_appstate.m_rebar_has_linebreak = ReBar.GetRowCount() > 1 ;

	m_appstate.write_to_registry() ;
	m_props->m_gloss_props.read_from_registry() ;
	m_props->write_to_registry() ;
}

void CMainFrame::save_settings_destroy()
{
	// write our recent docs
	m_mru.WriteToRegistry( resource_string( IDS_REG_KEY ) );

	check_save_history() ;

	save_util_settings();
}

wstring CMainFrame::get_record_translation(record_pointer record)
{
	return m_view_state->retrieve_record_trans(record,
		record_string_prefs( m_props->m_mem_props.is_plaintext())) ;
}

/* Show the updates dialog.
* This is in a separate process, so that in the future we can have this
* process shut us down, and download/install the update automatically.
*/
LRESULT CMainFrame::on_help_check_updates(WindowsMessage &)
{
	logging::log_debug("Checking for updates") ;

	file::CPath path ;
	path.GetModulePath(_Module.GetModuleInstance()) ;
	path.Append(_T("pytools")) ;
	path.Append(_T("CheckUpdates.exe")) ;

	CString filepath = path.Path() ;
#ifdef _DEBUG
	filepath = "C:\\Program Files (x86)\\Assistant Suite\\Felix\\pytools\\CheckUpdates.exe" ;
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

void CMainFrame::set_module_library( WORD lang_id )
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

void CMainFrame::load_old_preferences( const CString filename )
{
	const WORD old_language = m_appstate.m_preferred_gui_lang ;

	this->clear_memory() ;
	m_model->get_memories()->clear() ;

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

	set_up_ui_state() ;

	// set the title
	set_window_title() ;

	set_up_window_size() ;

	init_item_colors();

	set_bg_color( static_cast< COLORREF >( m_props->m_view_props.m_data.m_back_color ) ) ;

	load_history() ;
	load_util_settings() ;

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
		gloss_window_pointer gloss = get_glossary_window();
		CWindowSettings ws;

		if( ws.Load( resource_string(IDS_REG_KEY), _T("MainGlossary") ) )
		{
			ws.ApplyTo( *gloss ) ;
		}
		else
		{
			gloss->set_up_initial_size() ;
		}
		gloss->load_reg_settings() ;
		gloss->load_history() ;
		gloss->load_util_settings() ;
		gloss->reflect_sb_vis() ;
		gloss->reflect_tb_vis();
		gloss->apply_reg_bg_color() ;
		gloss->apply_mousewheel_setting() ;
	}

	// ===============
	logging::log_debug("Loaded preferences") ;
	user_feedback( IDS_PREFS_LOADED ) ;
}

LRESULT CMainFrame::on_new_search( WindowsMessage &)
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
void CMainFrame::set_doc_ui_handler()
{
#ifdef UNIT_TEST
	return ;
#else
	logging::log_debug("Setting the doc UI handler") ;
	CComObject<CFelixMemDocUIHandler> *pUIH = NULL;
	HRESULT hr = CComObject<CFelixMemDocUIHandler>::CreateInstance (&pUIH);
	if (SUCCEEDED(hr))
	{
		pUIH->m_get_menu = boost::bind(&CMainFrame::get_doc_context_menu, this) ;
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
HRESULT CMainFrame::get_doc_context_menu()
{
	BANNER("CMainFrame::get_doc_context_menu") ;
	CMenu menu ;

	CImageList images ;
	images.Create(BM_SIZE, BM_SIZE, ILC_COLOR24 | ILC_MASK, 0, 2 ) ;

	menu.CreatePopupMenu() ;
	add_popup_item(menu, ID_EDIT_COPY, IDS_POPUP_COPY) ;
	add_popup_separator(menu) ;
	add_popup_item(menu, ID_NEXT_PANE, IDS_POPUP_SWITCH_VIEWS) ;
	add_popup_item(menu, ID_EDIT_REGISTER, IDS_POPUP_REGISTER_GLOSS) ;
	add_popup_separator(menu) ;
	add_popup_item(menu, IDC_SOURCE_CONCORDANCE_SEL, IDS_SOURCE_CONCORDANCE) ;
	add_popup_item(menu, IDC_TRANS_CONCORDANCE_SEL, IDS_TRANS_CONCORDANCE) ;
	add_popup_separator(menu) ;
	add_popup_item(menu, ID_EDIT_DELETE, IDS_POPUP_DELETE) ;

	// Show the menu at the cursor position
	POINT ptScreen ;
	::GetCursorPos(&ptScreen) ;
	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON,	ptScreen.x, ptScreen.y, m_hWnd, NULL) ;
	return S_OK ;
}

/*
Close the top memory on the stack, if any.
Change window title to reflect new top memory, if any.
*/
LRESULT CMainFrame::on_memory_close(WindowsMessage &)
{
	BANNER("CMainFrame::on_memory_close") ;
	// base case -- there are no memories
	if (m_model->get_memories()->empty())
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

void CMainFrame::addToMessageLoop()
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

CString CMainFrame::get_active_mem_name()
{
	if ( ! m_model->get_memories()->empty() )
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

LRESULT CMainFrame::on_user_lookup_source( WindowsMessage& )
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

LRESULT CMainFrame::on_user_lookup_trans( WindowsMessage& )
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

void CMainFrame::set_display_state( DISPLAY_STATE new_state )
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

edit_record_dlg_ptr CMainFrame::get_editor()
{
	return m_editor ;
}

bool CMainFrame::is_short_format()
{
	return m_is_short_format ;
}

bool CMainFrame::is_single_page()
{
	return !! m_props->m_view_props.m_data.m_single_screen_matches ;
}

void CMainFrame::set_menu_checkmark( int item_id, bool is_checked )
{
	if (! this->IsWindow())
	{
		return ;
	}
	UISetCheck(item_id, !! is_checked);
	UpdateLayout( FALSE ) ;
}

void CMainFrame::perform_concordance_search(mem_engine::search_query_params &params)
{
	search_match_container matches ;
	m_model->get_memories()->perform_search( matches, params ) ;
	m_search_matches.set_matches( matches ) ;
}

void CMainFrame::create_reg_gloss_window()
{
	_Module.AddCreateWndData( &m_reg_gloss_dlg.m_thunk.cd, (CDialogImplBaseT< TWindow >*)&m_reg_gloss_dlg);

	DLGPROC lpDialogProc = (DLGPROC)m_reg_gloss_dlg.StartDialogProc ;

	instantiate_dlg(IDD_ADD_GLOSS, lpDialogProc) ;

	ATLASSERT( m_reg_gloss_dlg.IsWindow() ) ;

	if ( ! m_reg_gloss_dlg.IsWindow() )
	{
		throw except::CException( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
	}
}

void CMainFrame::save_memory_as( memory_pointer mem )
{
	save_file_dlg dialog ;

	if ( ! mem->is_new() ) 
	{
		file::CPath path( mem->get_location() ) ;
		path.RemoveExtension() ;
		dialog.set_default_file( (LPCTSTR)path.Path() ) ;
	}

	CString dialog_title ;
	dialog_title.FormatMessage( IDS_SAVE, resource_string( IDS_MEMORY) ) ;
	dialog.set_prompt( (LPCTSTR)dialog_title ) ;

	dialog.set_filter( get_save_filter() ) ;

	CString file_name = dialog.get_save_file() ;

	if ( file_name.IsEmpty() )
	{
		return ;
	}

	file::CPath path( file_name ) ;

	const int selected_index = dialog.get_selected_index() ;

	switch( selected_index ) 
	{
	case 1: case 7:
		logging::log_debug("Saving memory as ftm file") ;
		fileops::addExtensionAsNeeded( file_name,  _T( ".ftm" ) ) ;
		break;

	case 2:
		logging::log_debug("Saving memory as xml file") ;
		fileops::addExtensionAsNeeded( file_name,  _T( ".xml" ) ) ;
		break;

	case 3:
		logging::log_debug("Saving memory as tmx file") ;
		fileops::addExtensionAsNeeded( file_name,  _T( ".tmx" ) ) ;
		export_tmx( file_name, mem ) ;
		return ;

	case 4:
		logging::log_debug("Saving memory as Trados text file") ;
		fileops::addExtensionAsNeeded( file_name,  _T( ".txt" ) ) ;
		export_trados( file_name, mem ) ;
		return ;

	case 5:
		{
			export_excel(file_name, mem);
			return ;
		}

	case 6:
		{
			export_tabbed_text(file_name, mem);
			return ;
		}

	default:
		logging::log_warn("Unknown case in switch statement") ;
		ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
		logging::log_debug("Saving memory as tmx file") ;
		fileops::addExtensionAsNeeded( file_name,  _T( ".tmx" ) ) ;
		export_tmx( file_name, mem ) ;
		break;
	}

	// If the name changes, then we make the current user into the creator
	const CString old_location = mem->get_location() ;

	mem->set_location( file_name ) ;

	save_memory( mem ) ;

	if ( 0 != old_location.CompareNoCase( file_name ) )
	{
		mem->set_creator_to_current_user( ) ;
	}

	set_window_title() ;
}

void CMainFrame::get_qc_messages( mem_engine::record_pointer record, std::vector<wstring> &messages )
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
		params.m_rich_source = record->get_source_rich() ;
		params.m_source = record->get_source_plain() ;

		boost::shared_ptr<memory_model> memories = get_glossary_window()->get_memory_model() ;

		search_match_container matches ;
		memories->get_glossary_matches(matches, params) ;

		std::vector<qc::gloss_pair> gloss_matches ;
		foreach(match_ptr match, matches)
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

INT_PTR CMainFrame::check_save_memory( mem_engine::memory_pointer mem )
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

void CMainFrame::save_old_prefs_file( CString filename )
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

void CMainFrame::check_mousewheel_count()
{
	if (m_mousewheel_count)
	{
		CString command = _T("decreaseFont") ;
		if (m_mousewheel_count > 0)
		{
			command = _T("increaseFont") ;
		}
		logging::log_debug("Mousewheel command: " + string((LPCSTR)CT2A(command))) ;
		for (int i = 0 ; i < abs(m_mousewheel_count); ++i)
		{
			m_view_interface.run_script(command) ;
		}
	}
}