#include "stdafx.h"
#include "ManagerWindow.h"
#include "RegSettings.h"
#include "background_processor.h"
#include "text_templates.h"
#include "EditTransRecordDialogModal.h"
#include "replacer.h"
#include "record_local.h"
#include "WebPage.h"
#include "numberfmt.h"
#include "system_message.h"

#include "ManagerViewBrowse.h"
#include "ManagerViewDetails.h"
#include "ManagerViewEdit.h"
#include "ManagerViewQCSettings.h"
#include "ManagerViewQCBrowse.h"
#include "ManagerViewStart.h"
#include "ManagerViewActions.h"
#include "EditFormParser.h"
#include "QCFormParser.h"
#include "ui.h"

#include "TMXReader.h"		// CTMXReader
#include "data_importer.h"	// trados_data_importer
#include "ImportDialog.h"
#include "ImportMultitermFile.h"
#include "TabbedTextImporter.h"

// undoable actions
#include "action_strip_tags.h"
#include "action_trim_spaces.h"
#include "action_delete_entry.h"

#include "input_device_file.h"

#include "file_dialog.h"

#ifdef UNIT_TEST
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#endif

using namespace mem_engine ;
using namespace action ;

CManagerWindow::CManagerWindow(app_props::props_ptr props,
							   int title_id, 
							   LPCTSTR key, 
							   FrameListener *listener) : 
	m_listener(listener),
	m_title_id(title_id),
	m_settings_key(key),
	m_current_item(0),
	m_is_memory(true),
	m_props(props),
	m_hWndStatusBar(NULL)
{

}

LRESULT CManagerWindow::OnCreate( UINT, WPARAM, LPARAM )
{
	SENSE("OnCreate") ;

	logging::log_debug("Creating TM manager window") ;

	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
	if (m_title_id)
	{
		this->SetWindowText(resource_string(m_title_id)) ;
	}

	m_view.set_listener(static_cast<CHtmlViewListener *>(this)) ;

	HWND client ;
	const CString filename = cpptempl::get_template_filename(_T("manager/start.html")) ;
	m_view.create(*this, client, filename) ;
	m_view.ensure_document_complete() ;

	CWindowSettings ws;
	if( ws.Load(resource_string(IDS_REG_KEY), m_settings_key))
	{
		ws.ApplyTo( *this ) ;
	}

	m_accelerator.LoadAccelerators(IDR_SEARCH_ACCEL) ;
	::PostMessage(m_hWnd, WM_COMMAND, MAKEWPARAM( IDC_STARTUP_CHECKS, 100 ), 0) ;

	init_status_bar() ;

#ifndef UNIT_TEST
	_Module.GetMessageLoop()->AddMessageFilter(this) ;
#endif
	return 0L ;
}

void CManagerWindow::save_window_settings()
{
	CWindowSettings ws;
	ws.GetFrom(*this);
	ws.Save( resource_string(IDS_REG_KEY), m_settings_key );
}

LRESULT CManagerWindow::OnDestroy( UINT, WPARAM, LPARAM )
{
	SENSE("OnDestroy") ;
	save_window_settings() ;
	m_view.destroy() ;

#ifndef UNIT_TEST
	m_accelerator.DestroyObject() ;
	_Module.GetMessageLoop()->RemoveMessageFilter(this) ;
#endif
	return 0L ;
}

LRESULT CManagerWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SENSE("OnSize") ;
	// get the client rect

	static const int SB_WIDTH_THRESHOLD = 400 ;
	static const int SB_PANE2_WIDTH = 100 ;
	static const int SB_PANE2_PROPORTION = 4 ;

	CClientRect client_rect(*this) ;

	if (m_statusbar.m_mp_sbar.IsWindow())
	{
		m_statusbar.m_mp_sbar.SendMessage( uMsg, wParam, lParam ) ;
		const CWindowRect statusbar_rect(m_statusbar.m_mp_sbar.m_hWnd) ;
		client_rect.bottom -= statusbar_rect.Height() ;

		const int width = statusbar_rect.Width() ;

		if ( width > SB_WIDTH_THRESHOLD )
		{
			m_statusbar.m_mp_sbar.SetPaneWidth( ID_PANE_2, SB_PANE2_WIDTH ) ;
		}
		else
		{
			m_statusbar.m_mp_sbar.SetPaneWidth( ID_PANE_2, ( width / SB_PANE2_PROPORTION )  ) ;
		}
	}

	// resize edit window
	m_view.Move( &client_rect ) ;
	return 0L ;
}

void CManagerWindow::wait_for_doc_complete()
{
	m_view.ensure_document_complete() ;
}

BOOL CManagerWindow::PreTranslateMessage( LPMSG pMsg )
{

	ENSURE_FOCUS

	if ( IsWindow() && m_accelerator.TranslateAccelerator( *this, pMsg ) )
	{
		return TRUE ;
	}

	// prevent the view from eating our menu shortcut keys...
	if (::GetKeyState(VK_MENU) & 0x8000)
	{
		return FALSE ;
	}

	if (m_view.PreTranslateMessage( pMsg ))
	{
		return TRUE ;
	}

	return FALSE ;
}

/*
 This is a callback from the CHtmlView class. 
 If we handle the URL ourselves, then we cancel navigation.
 Returns true to cancel, false to allow navigation.
 */
bool CManagerWindow::OnBeforeNavigate2( _bstr_t burl )
{
	SENSE("CManagerWindow::OnBeforeNavigate2") ;

	try
	{
		const string url = BSTR2string(burl) ;

		// "#" is used for JavaScript links.
		if(boost::ends_with(url, "#"))
		{
			return false ; // don't cancel
		}

		std::vector<string> tokens ;
		boost::split(tokens, url, boost::is_any_of("/\\")) ;
		std::reverse(tokens.begin(), tokens.end()) ;

		if (tokens.empty() || tokens[0] == "")
		{
			return nav_empty(tokens) ;
		}
		if (tokens[0] == "start")
		{
			return nav_start(tokens) ;
		}
		if (tokens[0] == "memories")
		{
			return nav_memories(tokens) ;
		}
		if (tokens[0] == "glossaries")
		{
			return nav_glossaries(tokens) ;
		}

		// actions
		if (tokens[0] == "actions")
		{
			return actions(tokens) ;
		}
		if (tokens[0] == "perform_action")
		{
			return perform_action(tokens) ;
		}

		// undo/redo
		if (tokens[0] == "undo")
		{
			return undo(tokens) ;
		}
		if (tokens[0] == "redo")
		{
			return redo(tokens) ;
		}

		// moving items in list
		if (tokens[0] == "moveup")
		{
			return nav_moveup(tokens) ;
		}
		if (tokens[0] == "movedown")
		{
			return nav_movedown(tokens) ;
		}

		// crud
		if (tokens[0] == "view")
		{
			return nav_view(tokens) ;
		}
		if (tokens[0] == "edit")
		{
			return nav_edit(tokens) ;
		}
		if (tokens[0] == "submit_edit")
		{
			return handle_edit_memory(tokens, this->get_doc3()) ;
		}
		if (tokens[0] == "qc_settings")
		{
			return nav_qc(tokens) ;
		}
		if (tokens[0] == "submit_qc")
		{
			return handle_qc_settings(this->get_doc3()) ;
		}
		if (tokens[0] == "browse")
		{
			return nav_browse(tokens) ;
		}
		if (tokens[0] == "browse_qc")
		{
			return nav_qc_browse(tokens) ;
		}
		if (tokens[0] == "browse_qc_page")
		{
			return nav_qc_browse_page(tokens) ;
		}
		if (tokens[0] == "remove")
		{
			return nav_remove(tokens) ;
		}
		if (tokens[0] == "addnew")
		{
			return nav_addnew(tokens) ;
		}
		if (tokens[0] == "load")
		{
			return nav_load(tokens) ;
		}
		if (tokens[0] == "removeall")
		{
			return remove_all(tokens) ;
		}
		
		// page navigation
		if (tokens[0] == "goto_page")
		{
			return nav_browse_page(tokens) ;
		}
		// record crud
		if (tokens[0] == "deleterecord")
		{
			return delete_record(tokens) ;
		}
		if (tokens[0] == "editrecord")
		{
			return edit_record(tokens) ;
		}
		if (tokens[0] == "undo_delete")
		{
			undo_delete_record();
			return true ;
		}
		if (tokens[0] == "redo_delete")
		{
			redo_delete_record();
			return true ;
		}

		// qc record crud
		if (tokens[0] == "qcdeleterecord")
		{
			return delete_record_qc(tokens) ;
		}
		if (tokens[0] == "qceditrecord")
		{
			return edit_record_qc(tokens) ;
		}

		// navigate to url (quick and dirty...)
		if (boost::ends_with(url, L".html"))
		{
			return false ;
		}
		else // is this not a recognized navigation link?
		{
			MessageBox(CString(url.c_str()), L"Unknown command") ;
			return true ;
		}
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
		logging::log_error("Standard library exception (Manager Window)") ;
		logging::log_exception(e) ;
		const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND ;
		::MessageBox( m_hWnd, CA2T(e.what()), _T("C Runtime Error in Manager Window"), msg_flags ) ;  
	}

	return true ;
}

/*
 The memory window will set this.
 It contains the list of memories/glossaries that we'll be searching.
 */
void CManagerWindow::set_mem_model(model_iface_ptr model)
{
	m_mem_model = model ;
}

void CManagerWindow::set_gloss_model(model_iface_ptr model)
{
	m_gloss_model = model ;
}

/*
 Get the document element from the web browser.
 We use this to get and set text in various elements by id.
 We serve up a fake doc3 element if we're unit testing.
 */
doc3_wrapper_ptr CManagerWindow::get_doc3()
{
#ifndef UNIT_TEST
	return make_doc3_wrapper(m_view.m_view.get_document3());
#else
	return get_fake_search_doc(); 
#endif
}

// Call the toggleHelp JavaScript function
LRESULT CManagerWindow::OnToggleHelp()
{
	html::CWebPage page ;
	page.SetDocument(m_view.get_document()) ;
	page.CallJScript("toggleHelp") ;

	return 0L ;
}

void CManagerWindow::set_active_state( mgr_state_ptr mgr_state )
{
	m_current_state = mgr_state ;
	m_current_state->set_mem_model(m_mem_model) ;
	m_current_state->set_gloss_model(m_gloss_model) ;
	m_current_state->set_view(&m_view) ;
	m_current_state->set_listener(this) ;
	m_current_state->activate() ;
}

/************************************************************************/
/* navigation methods (called from OnBeforeNavigate)                    */
/************************************************************************/

bool CManagerWindow::nav_empty( const std::vector<string> &tokens )
{
	SENSE("nav_empty"); 
	tokens ;
	return true ;
}

// moving items in list
bool CManagerWindow::nav_moveup(const std::vector<string> &tokens)
{
	SENSE("nav_moveup"); 
	bool is_mem = tokens[1] == "mem" ;
	size_t num = boost::lexical_cast<size_t>(tokens[2]) ;
	this->swap_memories(is_mem ? m_mem_model : m_gloss_model,
		num-1) ;
	this->m_current_state->show_content() ;
	m_listener->set_window_title() ;
	return true ;
}

bool CManagerWindow::nav_movedown(const std::vector<string> &tokens)
{
	SENSE("nav_movedown"); 
	bool is_mem = tokens[1] == "mem" ;
	size_t num = boost::lexical_cast<size_t>(tokens[2]) ;
	this->swap_memories(is_mem ? m_mem_model : m_gloss_model,
		num) ;
	this->m_current_state->show_content() ;
	m_listener->set_window_title() ;
	return true ;
}

// navigation to various category pages
bool CManagerWindow::nav_start(const std::vector<string> &tokens)
{
	SENSE("nav_start"); 
	tokens ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewStart)) ;
	m_current_state->show_content() ;
	return true ;
}

bool CManagerWindow::nav_memories(const std::vector<string> &tokens)
{
	SENSE("nav_memories"); 
	tokens ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewStartMem)) ;
	m_current_state->show_content() ;
	return true ;
}

bool CManagerWindow::nav_glossaries(const std::vector<string> &tokens)
{
	SENSE("nav_glossaries"); 
	tokens ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewStartGloss)) ;
	m_current_state->show_content() ;
	return true ;
}

// crud
bool CManagerWindow::nav_view(const std::vector<string> &tokens)
{
	SENSE("nav_view"); 
	bool is_memory = tokens[1] == "mem" ;
	size_t item = boost::lexical_cast<size_t>(tokens[2]) ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewDetails(item, is_memory))) ;
	m_current_state->show_content() ;
	return true ;
}

bool CManagerWindow::nav_edit(const std::vector<string> &tokens)
{
	SENSE("nav_edit"); 
	bool is_memory = tokens[1] == "mem" ;
	size_t item = boost::lexical_cast<size_t>(tokens[2]) ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewEdit(item, is_memory))) ;
	m_current_state->show_content() ;
	return true ;
}

bool CManagerWindow::nav_qc(const std::vector<string> &)
{
	SENSE("nav_qc"); 
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewQCSettings(m_props))) ;
	m_current_state->show_content() ;
	return true ;
}

bool CManagerWindow::handle_edit_memory(const std::vector<string> &tokens, doc3_wrapper_ptr doc)
{
	mem_engine::memory_pointer mem = get_mem(tokens[1],
									 		 boost::lexical_cast<size_t>(tokens[2]));

	mgrview::EditFormParser parser(doc) ;

	mem_engine::MemoryInfo *header = mem->get_memory_info() ;

	header->set_creator(parser.creator()) ;
	header->set_field(parser.field()) ;
	header->set_created_on(parser.created_on()) ;
	header->set_source_language(parser.source_language()) ;
	header->set_target_language(parser.target_language()) ;
	header->set_client(parser.client()) ;
	header->set_is_memory(parser.is_memory()) ;
	header->set_is_locked(parser.locked()) ;
	if (parser.is_active())
	{
		mem->set_active_on() ;
	}
	else
	{
		mem->set_active_off() ;
	}
	mem->set_default_context(parser.default_context()) ;
	m_message = L"Edited details for " + mgrview::get_memname(mem) ;
	return nav_view(tokens) ;
}

bool CManagerWindow::handle_qc_settings(doc3_wrapper_ptr doc)
{
	set_qc_prop_values(doc);
	
	m_props->save_prefs() ;

	m_message = L"Configured QC Settings" ;
	std::vector<string> tokens ;
	return nav_start(tokens) ;
}

bool CManagerWindow::nav_browse(const std::vector<string> &tokens)
{
	SENSE("nav_browse"); 
	m_is_memory = tokens[1] == "mem" ;
	size_t page = boost::lexical_cast<int>(tokens[2]) ;
	m_current_item = boost::lexical_cast<size_t>(tokens[3]) ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewBrowse(m_current_item, m_is_memory, page))) ;
	m_current_state->show_content() ;
	return true ;
}
bool CManagerWindow::nav_browse_page(const std::vector<string> &tokens) 
{
	SENSE("nav_browse_page"); 
	size_t page = boost::lexical_cast<int>(tokens[1]) ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewBrowse(m_current_item, m_is_memory, page))) ;
	m_current_state->show_content() ;
	return true ;
}


bool CManagerWindow::nav_qc_browse(const std::vector<string> &tokens)
{
	SENSE("nav_qc_browse"); 
	m_qc_matches.clear() ;
	const bool is_memory = tokens[1] == "mem" ;
	if (! is_memory)
	{
		return false ;
	}
	size_t page = boost::lexical_cast<int>(tokens[2]) ;
	m_current_item = boost::lexical_cast<size_t>(tokens[3]) ;
	mem_engine::memory_pointer mem = m_mem_model->memory_at(m_current_item) ;
	m_statusbar.set_up_prog_bar(mem->size()) ;
	for(size_t i = 0; i < mem->size() ; ++i )
	{
		m_statusbar.set_pos(i) ;
		mem_engine::record_pointer rec = mem->get_record_at(i) ;
		std::vector<wstring> messages ;
		m_listener->get_qc_messages(rec, messages) ;
		if (! messages.empty())
		{
			mem_engine::search_match_ptr match(new mem_engine::search_match(rec)) ;
			match->set_qc_messages(messages) ;
			match->set_memory_id(mem->get_id()) ;
			m_qc_matches.push_back(match) ;
			messages.clear() ;
		}
	}
	m_statusbar.m_mp_sbar.ProgDestroyWindow() ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewQCBrowse(m_current_item, page))) ;
	m_current_state->show_content() ;
	return true ;
}
bool CManagerWindow::nav_qc_browse_page(const std::vector<string> &tokens) 
{
	SENSE("nav_qc_browse_page"); 
	size_t page = boost::lexical_cast<int>(tokens[1]) ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewQCBrowse(m_current_item, page))) ;
	m_current_state->show_content() ;
	return true ;
}

// href="/{$index}/{$memtype}/{$record.num0}/deleterecord" (reversed)
bool CManagerWindow::delete_record( const std::vector<string> &tokens )
{
	const size_t record_number = boost::lexical_cast<size_t>(tokens[1]) ;

	memory_pointer mem = get_mem(tokens[2],
								 boost::lexical_cast<size_t>(tokens[3]));

	m_undo = undo_action_ptr(new ActionDeleteEntry(mem, mem->get_record_at(record_number))) ;
	m_undo->redo() ;
	string link = "\"/browse/undo_delete\"" ;
	CStringW msg = system_message_w(IDS_ACTION_UNDO_MSG, 
									CString(m_undo->name().c_str()), 
									CString(link.c_str()));
	m_message = wstring(static_cast<LPCWSTR>(msg)) ;
	m_current_state->show_content() ;
	return true ;
}

// href="/{$index}/{$memtype}/{$record.num0}/editrecord" (reversed)
bool CManagerWindow::edit_record( const std::vector<string> &tokens )
{
	const size_t record_number = boost::lexical_cast<size_t>(tokens[1]) ;

	mem_engine::memory_pointer mem = get_mem(tokens[2],
											 boost::lexical_cast<size_t>(tokens[3]));

	CEditTransRecordDialogModal editdlg ;

	editdlg.set_record(mem->get_record_at(record_number)) ;
	editdlg.set_memory_id(mem->get_id()) ;

#ifndef UNIT_TEST
	if (editdlg.DoModal(*this) == IDOK)
	{
#endif		
		m_current_state->show_content() ;
#ifndef UNIT_TEST
	}
#endif	

	return true ;
}

// href="/{$index}/{$memtype}/{$record.num0}/deleterecord" (reversed)
bool CManagerWindow::delete_record_qc( const std::vector<string> &tokens )
{
	size_t record_number = boost::lexical_cast<size_t>(tokens[1]) ;

	mem_engine::search_match_ptr match = m_qc_matches[record_number] ;
	mem_engine::memory_pointer mem = m_mem_model->get_memory_by_id(match->get_memory_id()) ;
	m_undo = undo_action_ptr(new ActionDeleteEntry(mem, match->get_record())) ;
	m_undo->redo() ;
	string link = "\"/browse/undo_delete\"" ;
	CStringW msg = system_message_w(IDS_ACTION_UNDO_MSG, 
		CString(m_undo->name().c_str()), 
		CString(link.c_str()));
	m_message = wstring(static_cast<LPCWSTR>(msg)) ;
	m_current_state->show_content() ;
	return true ;
}

// href="/{$index}/{$memtype}/{$record.num0}/editrecord" (reversed)
bool CManagerWindow::edit_record_qc( const std::vector<string> &tokens )
{
	size_t record_number = boost::lexical_cast<size_t>(tokens[1]) ;

	CEditTransRecordDialogModal editdlg ;

	mem_engine::search_match_ptr match = m_qc_matches[record_number] ;
	editdlg.set_record(match->get_record()) ;
	editdlg.set_memory_id(match->get_memory_id()) ;

#ifndef UNIT_TEST
	if (editdlg.DoModal(*this) == IDOK)
	{
#endif		
		mem_engine::record_pointer rec = match->get_record() ;
		std::vector<wstring> messages ;
		m_listener->get_qc_messages(rec, messages) ;
		match->set_qc_messages(messages) ;
		m_current_state->show_content() ;
#ifndef UNIT_TEST
	}
#endif	

	return true ;
}

mem_engine::memory_pointer CManagerWindow::get_mem( string memtype, size_t item_num )
{
	if (memtype == "mem")
	{
		return m_mem_model->memory_at(item_num) ;
	}
	return m_gloss_model->memory_at(item_num) ;
}

bool CManagerWindow::nav_remove(const std::vector<string> &tokens)
{
	SENSE("nav_remove"); 
	bool is_mem = tokens[1] == "mem" ;
	size_t item = boost::lexical_cast<size_t>(tokens[2]) ;

	mem_engine::memory_pointer mem = *this->get_mem_iter_at(is_mem ? m_mem_model : m_gloss_model,
		item );

	if ( ! mem->is_saved() )
	{
		CString user_prompt = get_save_prompt(mem);

		switch( MessageBox( 
			user_prompt, 
			resource_string( IDS_SAVE_FILE_TITLE ), 
			MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_SETFOREGROUND 
			)  )
		{
		case IDNO :
			mem->set_saved_flag( true ) ;
			break;
		case IDYES:
			if ( mem->is_new() )
			{
				if ( ! getMemName(mem) )
				{
					return false ;
				}
			}

			mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

			try
			{
				mem->save_memory() ;
			}
			catch ( ... ) 
			{
				mem->set_listener( NULL ) ;
				throw ;
			}

			mem->set_listener( NULL ) ;
			break ;
		case IDCANCEL:
			SetMsgHandled( TRUE ) ;
			return false ;
		}
	}
	if (is_mem)
	{
		m_mem_model->remove_memory(mem) ;
	}
	else
	{
		m_gloss_model->remove_memory(mem) ;
	}

	m_current_state->show_content() ;
	m_listener->set_window_title() ;
	return true ;
}
bool CManagerWindow::nav_addnew(const std::vector<string> &tokens)
{
	SENSE("nav_addnew"); 
	bool is_memory = tokens[1] == "mem" ;
	if (is_memory)
	{
		m_mem_model->add_memory() ;
	}
	else
	{
		m_gloss_model->add_memory() ;
	}
	m_current_state->show_content() ;
	return true ;
}

/* swap_memories
 swaps the memory at `index` with the one below it.
 For example, if you specify index=0, it will swap the memories at indices 0 and 1
 */
void CManagerWindow::swap_memories( model_iface_ptr model, const int index )
{
	memory_iterator pos1 = get_mem_iter_at(model, index) ;
	memory_iterator pos2 = pos1 ; 
	std::advance(pos2, 1) ;
	std::swap(*pos1, *pos2) ;
}
mem_engine::memory_iterator CManagerWindow::get_mem_iter_at( model_iface_ptr model,
									   int sel )
{
	memory_iterator pos = model->begin() ;
	std::advance(pos, static_cast<size_t>(sel)) ;
	return pos ;
}

CString CManagerWindow::get_save_prompt( mem_engine::memory_pointer mem )
{
	CString user_prompt ;
	user_prompt.FormatMessage
		( 
		IDS_PROMPT_SAVE_MODIFIED, 
		resource_string(IDS_MEMORY), 
		file::name( mem->get_location() ).file_name() 
		) ;
	ATLASSERT( user_prompt.IsEmpty() == false ) ;
	return user_prompt ;
}

bool CManagerWindow::getMemName( mem_engine::memory_pointer mem ) const
{
	CString original_file_name;
	if (mem->is_new() == false)
	{
		original_file_name = mem->get_fullpath();
	}

	CString dialog_title;
	dialog_title.FormatMessage(IDS_SAVE, resource_string(IDS_MEMORY));

	file_save_dialog dialog;
	dialog.set_filename(original_file_name);
	dialog.set_title(dialog_title);
	dialog.set_file_filter(get_mem_save_filter());
	dialog.set_default_ext(L"ftm");
	dialog.set_last_save(m_props->m_history_props.m_memory_location);

	if (!dialog.show())
	{
		return false;
	}

	CString save_as_file_name = dialog.get_save_destination();


	if (save_as_file_name.IsEmpty())
	{
		return false ;
	}
	mem->set_location(save_as_file_name);
	return true ;
}

// ========================
// CProgressListener
// ========================
void CManagerWindow::OnProgressInit( const CString &file_name, size_t min_val, size_t max_val )
{
	m_statusbar.OnProgressInit(file_name, min_val, max_val) ;
}
bool CManagerWindow::OnProgressLoadUpdate( size_t current_val ) // true to continue
{
	return m_statusbar.OnProgressLoadUpdate(current_val) ;
}
bool CManagerWindow::OnProgressWriteUpdate( size_t current_val )  // true to continue
{
	return m_statusbar.OnProgressWriteUpdate(current_val) ;
}
void CManagerWindow::OnProgressDoneWrite( size_t final_val ) 
{
	m_statusbar.OnProgressDoneWrite(final_val) ;
}
void CManagerWindow::OnProgressDoneLoad( size_t final_val ) 
{
	m_statusbar.OnProgressDoneLoad(final_val) ;
}

// command handlers

LRESULT CManagerWindow::OnInitView()
{
	m_view.ensure_navigation_complete() ;
	m_view.ensure_document_complete() ;
	if (m_title_id == IDS_GLOSSARY_MANAGER_TITLE)
	{
		this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewStartGloss)) ;
	}
	else
	{
		this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewStart)) ;
	}
	m_current_state->show_content() ;

	return 0L ;
}


// remove all memories                                                 
bool CManagerWindow::remove_all( const std::vector<string> &tokens )
{
	const bool is_memory = tokens[1] == "mem" ;
	model_iface_ptr controller ;
	if (is_memory)
	{
		controller = m_mem_model ;
	}
	else
	{
		controller = m_gloss_model ;
	}
	memory_list memories_needing_saving ;
	controller->get_memories_needing_saving( memories_needing_saving ) ;
	FOREACH (memory_pointer mem, memories_needing_saving)
	{
		if (m_listener->check_save_memory(mem) == IDCANCEL)
		{
			return false	;
		}

	}
	controller->clear() ;
	m_current_state->show_content() ;
	return true ;
}
/************************************************************************/
/* loading memories                                                     */
/************************************************************************/
bool CManagerWindow::nav_load(const std::vector<string> &tokens)
{
	SENSE("nav_load"); 
	const bool is_memory = tokens[1] == "mem" ;
	m_current_state->show_content() ;

	file_open_dialog dialog;
	dialog.allow_multi_select();

	if (is_memory)
	{
		dialog.set_title(R2T(IDS_OPEN));
		dialog.set_file_filter(get_mem_open_filter());
	}
	else
	{
		dialog.set_title(R2T(IDS_OPEN_GLOSS_FILE));
		dialog.set_file_filter(get_gloss_open_filter());
	}
#ifdef DEBUG
	return true;
#else

	if (!dialog.show())
	{
		return true;
	}

	if (is_memory)
	{
		switch (dialog.get_selected_index())
		{
		case 1: case 4:
			add_memory_files(m_mem_model, dialog.get_open_destinations());
			break ;

		case 2:
			import_tmx(dialog.get_open_destinations());
			break ;

		case 3:
			import_trados(dialog.get_open_destinations());
			break ;

		default:
			ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
			break ;

		}	
	}
	else
	{
		switch (dialog.get_selected_index())
		{
		case 1: case 4:
			add_memory_files(m_gloss_model, dialog.get_open_destinations());
			break ;

		case 2:
			import_multiterm(dialog.get_open_destinations());
			break ;

		case 3:
			{
				auto filenames = dialog.get_open_destinations();
				for(CString filename: filenames)
				{
					import_tabbed_text(filename) ;
				}
			}
			break ;

		default:
			ATLASSERT ( FALSE && "Unknown case in switch statement" ) ; 
			break ;
		}		
	}

	m_current_state->show_content() ;
	m_listener->set_window_title() ;
	return true ;
#endif
}


void CManagerWindow::add_memory_files(model_iface_ptr model,
	std::vector<CString> import_files)
{
	for(CString filename: import_files)
	{
		add_memory_file(model, filename);
	}
}

void CManagerWindow::add_memory_file( model_iface_ptr model, CString filename )
{
	const file::CPath path(filename) ;
	if ( ! path.FileExists() ) // whoops -- file not there
	{
		CString msg ;
		msg.FormatMessage( IDS_MSG_NO_FILE, filename ) ;
		THROW_WIN_EXCEPTION( msg ) ;
	}

	mem_engine::memory_pointer mem = model->add_memory() ;

	try
	{
		mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

		if ( ! mem->load( filename ) )
		{
			return ;
		}
	}
	catch ( ... ) 
	{
		mem->set_listener( NULL ) ;
		throw ;
	}
	m_listener->set_window_title() ;
}

bool CManagerWindow::import_tmx(std::vector<CString> files)
{
	for(CString filename: files)
	{
		import_tmx(filename) ;
	}

	return true ;
}

bool CManagerWindow::import_tmx( const CString &file_name )
{
	memory_pointer mem = m_mem_model->add_memory() ;

	CTMXReader reader( mem, static_cast< CProgressListener* >( this ) ) ;
	input_device_ptr input(new InputDeviceFile) ;
	reader.load_tmx_memory( file_name, input ) ;

	// if we failed to load any entries, remove the memory
	if ( mem->empty() ) 
	{
		m_mem_model->remove_memory_by_id( mem->get_id() ) ;
		return false ;
	}

	mem->set_location(file_name) ;

	m_listener->set_window_title() ;
	return true ;
}

bool CManagerWindow::import_trados(std::vector<CString> files)
{
	try
	{
		for(CString filename: files)
		{
			import_trados(filename) ;
		}
	}
	catch( except::CException &e )
	{
		logging::log_error("Failed to load trados memory from manager window") ;
		logging::log_exception(e) ;
		e.notify_user( IDS_IMPORT_ABORTED ) ;

		return false ;
	}

	return true ;
}

bool CManagerWindow::import_trados( const CString &trados_file_name )
{
	trados_data_importer importer( static_cast< CProgressListener* >( this ) ) ;

	CWaitCursor wait_cursor ;

	importer.open_data_source( trados_file_name ) ;

	// convert them to internal format
	trados_data_importer::language_code_set languages ;

	for ( int num_codes = 0 ; 
		importer.get_language_code( languages ) && num_codes < 100 ; 
		++num_codes)
	{
	}

	wait_cursor.Restore() ;

	CImportDialog import_dialog ;
	import_dialog.set_languages( languages ) ;
	if ( import_dialog.DoModal() == IDCANCEL )
	{
		return false ;
	}

	importer.set_source_language( import_dialog.get_source_plain() ) ;
	importer.set_target_language( import_dialog.get_trans_plain() ) ;

	memory_pointer mem = m_mem_model->add_memory() ;

	MemoryInfo *mem_info = mem->get_memory_info() ;
	mem_info->set_creation_tool( L"TradosText" ) ;
	mem_info->set_creation_tool_version( L"6.0" ) ;

	ATLASSERT ( mem->get_memory_info()->get_creation_tool() == L"TradosText" ) ; 
	ATLASSERT ( mem->get_memory_info()->get_creation_tool_version() == L"6.0" ) ; 

	if ( ! importer.load( trados_file_name, mem ) )
	{
		return false ;
	}
	else
	{
		mem->set_location( trados_file_name ) ;
		m_listener->set_window_title() ;
	}

	return true ;
}

void CManagerWindow::import_multiterm(std::vector<CString> import_files)
{
	for(CString filename: import_files)
	{
		import_multiterm(filename) ;
	}
	m_listener->set_window_title() ;
}

void CManagerWindow::import_multiterm( const CString &file_name )
{
	CImportMultitermFile importer(this, m_props) ;
	input_device_ptr input(new InputDeviceFile) ;
	importer.import(file_name, input) ;
	m_gloss_model->insert_memory(importer.m_memory) ;
	m_listener->set_window_title() ;
}

void CManagerWindow::import_tabbed_text( const CString &file_name )
{
	CTabbedTextImporter importer(this, m_props) ;
	input_device_ptr input(new InputDeviceFile) ;
	importer.load_file(file_name, input) ;
	m_gloss_model->insert_memory(importer.m_memory) ;
	m_listener->set_window_title() ;
}

bool CManagerWindow::actions( const std::vector<string> &tokens )
{
	SENSE("actions") ;
	bool is_memory = tokens[1] == "mem" ;
	size_t item = boost::lexical_cast<size_t>(tokens[2]) ;
	this->set_active_state(mgr_state_ptr(new mgrview::ManagerViewActions(item, is_memory))) ;
	m_current_state->show_content() ;
	return true ;
}
bool CManagerWindow::perform_action(const std::vector<string> &tokens)
{
	SENSE("perform_action") ;

	string action = tokens[1] ;
	string memtype = tokens[2] ;
	string item = tokens[3] ;
	set_undo_action(action, memtype, item);
	std::vector<string> nav_tokens ;
	nav_tokens += "redo", memtype, item ;
	return this->redo(nav_tokens) ;
}

// undo an action
bool CManagerWindow::undo( const std::vector<string> &tokens )
{
	SENSE("undo") ;
	m_message = create_redo_msg(tokens);
	m_undo->undo() ;
	std::vector<string> nav_tokens ;
	nav_tokens += "view", tokens[1], tokens[2] ;
	nav_view(nav_tokens) ;
	return true ;
}

// redo an action that was undone
// also used to actually perform the action.
bool CManagerWindow::redo(const std::vector<string> &tokens)
{
	SENSE("redo") ;
	m_message = create_undo_msg(tokens) ;
	m_undo->redo() ;
	std::vector<string> nav_tokens ;
	nav_tokens += "view", tokens[1], tokens[2] ;
	nav_view(nav_tokens) ;
	return true ;
}

// messages for undo/redo (with undo/redo links)
wstring CManagerWindow::create_undo_msg( const std::vector<string> &tokens ) const
{
	string link = "\"/" + tokens[2] + "/" + tokens[1] + "/undo\"" ;
	CStringW msg = system_message_w(IDS_ACTION_UNDO_MSG, CString(m_undo->name().c_str()), CString(link.c_str()));
	return wstring(static_cast<LPCWSTR>(msg)) ;
}
wstring CManagerWindow::create_redo_msg(const std::vector<string> &tokens) const 
{
	string link = "\"/" + tokens[2] + "/" + tokens[1] + "/redo\"" ;
	CStringW msg = system_message_w(IDS_ACTION_REDO_MSG, CString(m_undo->name().c_str()), CString(link.c_str())) ;
	return wstring(static_cast<LPCWSTR>(msg)) ;
}

void CManagerWindow::set_undo_action(const string &action, const string &memtype, const string &item)
{
#ifdef UNIT_TEST
	action ;
	memtype ;
	item ;
	return ;
#else
	mem_engine::memory_pointer mem = this->get_mem(memtype, 
						boost::lexical_cast<size_t>(item)) ;

	if (action == ACTION_NAME_TRIM)
	{
		this->m_undo = undo_action_ptr(new ActionTrimSpaces(mem)) ;
	}
	else if (action == ACTION_NAME_STRIP)
	{
		this->m_undo = undo_action_ptr(new ActionStripTags(mem)) ;
	}
#endif
}

wstring CManagerWindow::get_message()
{
	wstring message = m_message ;
	m_message.clear() ;
	return message ;
}

void CManagerWindow::undo_delete_record()
{
	m_undo->undo() ;
	string link = "\"/browse/redo_delete\"" ;
	CStringW msg = system_message_w(IDS_ACTION_REDO_MSG, 
		CString(m_undo->name().c_str()), 
		CString(link.c_str()));
	m_message = wstring(static_cast<LPCWSTR>(msg)) ;
	m_current_state->show_content() ;
}

void CManagerWindow::redo_delete_record()
{
	m_undo->redo() ;
	string link = "\"/browse/undo_delete\"" ;
	CStringW msg = system_message_w(IDS_ACTION_UNDO_MSG, 
		CString(m_undo->name().c_str()), 
		CString(link.c_str()));
	m_message = wstring(static_cast<LPCWSTR>(msg)) ;
	m_current_state->show_content() ;
}

bool CManagerWindow::init_status_bar()
{
	// create status bar
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP ;
	m_hWndStatusBar = ::CreateStatusWindow( dwStyle, R2T( ATL_IDS_IDLEMESSAGE ), m_hWnd, ATL_IDW_STATUS_BAR ) ;

#ifdef UNIT_TEST
	return true ;
#else
	ATLASSERT( CWindow( m_hWndStatusBar ).IsWindow() ) ;

	const int PANE_WIDTH = 100 ;

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
	m_statusbar.m_mp_sbar.ShowWindow( SW_SHOWNOACTIVATE ) ;
	return !! m_statusbar.m_mp_sbar.IsWindow() ;
#endif
}

void CManagerWindow::set_qc_prop_values( doc3_wrapper_ptr doc )
{
	mgrview::QCFormParser parser(doc) ;
	const bool check_numbers = parser.check_numbers() ;
	const bool check_all_caps = parser.check_all_caps() ;
	const bool check_gloss = parser.check_gloss() ;
	const bool live_check = parser.live_check() ;

	m_props->m_qc_props.m_data.m_check_numbers = check_numbers ? TRUE : FALSE ;
	m_props->m_qc_props.m_data.m_check_all_caps = check_all_caps ? TRUE : FALSE ;
	m_props->m_qc_props.m_data.m_check_gloss = check_gloss ? TRUE : FALSE ;
	m_props->m_qc_props.m_data.m_live_checking = live_check ? TRUE : FALSE ; 

	m_props->m_qc_props.m_qc_glosses.clear() ;
	for(size_t i = 0 ; i < m_gloss_model->size() ; ++i)
	{
		mem_engine::memory_pointer mem = m_gloss_model->memory_at(i) ;
		if (parser.is_id_checked(mem->get_id()))
		{
			m_props->m_qc_props.m_qc_glosses.push_back(static_cast<LPCWSTR>(mem->get_fullpath())) ;
		}
	}
}