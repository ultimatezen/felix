#include "StdAfx.h"
#include "CommonWindowFunctionality.h"
#include "RegSettings.h" // CWindowSettings
#include "MemoryManagerDlg.h"	// CMemoryManagerDlg
#include "NumberFmt.h"
#include "DemoException.h"
#include "xpmenu/Tools.h"		// CWindowRect
#include "record_local.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

using namespace except ;
using namespace mem_engine ;
using namespace html ;

// CTOR
CCommonWindowFunctionality::CCommonWindowFunctionality( )
:	m_is_short_format( true ),
m_silent_mode( false ),
m_mousewheel_count(0),
m_editor(),
m_new_record(new mem_engine::record_local()),
m_review_record(new mem_engine::record_local()),
m_item_under_edit(new match_type(record_pointer(new mem_engine::record_local())))
{
}

void add_popup_separator(CMenu &menu) 
{
	CMenuItemInfo menu_item ;
	menu_item.fMask = MIIM_FTYPE ; // | MIIM_BITMAP;
	menu_item.fType = MFT_SEPARATOR ;

	::InsertMenuItem(menu, menu.GetMenuItemCount(), FALSE, (MENUITEMINFO*)&menu_item) ;
}

void add_popup_item( CMenu &menu, int command_id, int text_id ) 
{
	CMenuItemInfo menu_item ;
	menu_item.fMask = MIIM_STRING | MIIM_ID ; // | MIIM_BITMAP;
	menu_item.fType = MFT_STRING ;
	menu_item.wID = command_id ;
	CString copy_text ;
	copy_text.LoadString(text_id) ;
	menu_item.cch = copy_text.GetLength() ;
	menu_item.dwTypeData = copy_text.GetBuffer() ;

	::InsertMenuItem(menu, menu.GetMenuItemCount(), FALSE, (MENUITEMINFO*)&menu_item) ;
}

CString CCommonWindowFunctionality::get_location()
{
	boost::shared_ptr<mem_engine::memory_model> model = this->get_memory_model() ;
	memory_pointer mem = model->get_first_memory() ;
	return mem->get_location() ;
}

// Function name	: is_demo
bool CCommonWindowFunctionality::is_demo() const
{
	if ( armadillo::get_environment_var( R2A( IDS_USERKEY ) ).empty() )
	{
		return true ;
	}
	if ( armadillo::get_environment_var( "EXPIRED" ).empty() == false )
	{
		return true ;
	}

	return false ;
}

/************************************************************************/
/* User feedback                                                        */
/************************************************************************/
bool CCommonWindowFunctionality::user_feedback( const CString &feedback, int pane )
{
	SENSE(CStringA(feedback)) ;
	return m_statusbar.user_feedback(feedback, pane) ;
}
bool CCommonWindowFunctionality::user_feedback( const UINT id, int pane )
{
	SENSE(resource_string_a(id)) ;
	return m_statusbar.user_feedback(id, pane) ;
}
bool CCommonWindowFunctionality::user_feedback( const tstring &feedback, int pane )
{
	SENSE(string2string(feedback)) ;
	return m_statusbar.user_feedback(feedback, pane) ;
}
//////////////////////// == end user-feedback related == //////////////////

// get a list of memories
memory_list & CCommonWindowFunctionality::get_memories()
{
	boost::shared_ptr<mem_engine::memory_model> model = this->get_memory_model() ;
	return model->get_memories() ;
}

// Function name	: user_wants_to_save
// Make this a dedicated dialog.
INT_PTR CCommonWindowFunctionality::user_wants_to_save( const CString &f_name )
{
	CString user_prompt ;

	user_prompt.FormatMessage( IDS_PROMPT_SAVE_MODIFIED, 
							   get_window_type_string(), 
							   file::name( f_name ).file_name() ) ;
	
	ATLASSERT( user_prompt.IsEmpty() == false ) ;
	ATLASSERT( IsWindow() ) ;

	return ::MessageBox( 
			m_hWnd, 
			user_prompt, 
			R2T( IDS_SAVE_FILE_TITLE ), 
			MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_SETFOREGROUND 
		) ;
}

// Function name	: check_save
INT_PTR CCommonWindowFunctionality::check_save()
{
	memory_list memories_needing_saving ;
	this->get_memory_model()->get_memories_needing_saving( memories_needing_saving ) ;

	memory_iterator pos ;
	for ( pos = memories_needing_saving.begin() ;
			pos != memories_needing_saving.end() ;
			++pos )
	{
		memory_pointer mem = *pos ;
		switch( user_wants_to_save( mem->get_location() ) ) 
		{
		case IDNO :

			mem->set_saved_flag( true ) ;
			break;

		case IDYES :
 
			if ( IDCANCEL == LetUserSaveMemory(mem) )
			{
				return IDCANCEL ;
			}
			break ;

		case IDCANCEL :

			return IDCANCEL ;

		default :

			ATLASSERT( "Unknown response!" && FALSE ) ;
			return IDCANCEL ;

		}
	}

	return IDYES ;
}

INT_PTR CCommonWindowFunctionality::LetUserSaveMemory(mem_engine::memory_pointer& mem)
{
	if ( mem->is_new() )
	{
		windows_ui ui( m_hWnd ) ;
		CString msg ;
		msg.FormatMessage( IDS_SAVE, get_window_type_string() ) ;
		CString f_name = ui.get_save_file
			( 
			(LPCTSTR)msg, 
			get_save_filter(), 
			get_save_ext() 
			) ;

		if ( f_name.IsEmpty() )
		{
			return IDCANCEL ;
		}


		mem->set_location( f_name ) ;
	}

	if ( ! check_for_clashes( mem ) ) 
	{
		return IDCANCEL ;
	}

	do_save(mem) ;
	return IDYES ;
}

// Function name	: check_location
bool CCommonWindowFunctionality::check_location()
{
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;
	
	if ( mem->is_new() ) 
	{
		ATLASSERT( IsWindow() ) ;
		::SetFocus(m_hWnd) ;
	
		windows_ui ui( m_hWnd ) ;
		CString msg ;
		msg.FormatMessage( IDS_SAVE, get_window_type_string() ) ;

		CString file_name ;
		if ( ! ui.get_save_file( file_name, msg, XML_FILE_FILTER, XML_FILE_EXT ) ) 
		{
			return false ;
		}
		
		mem->set_location( file_name ) ;
		if ( ! mem->empty() )
		{
			mem->set_saved_flag( false ) ; // new location, so we have not saved to here before
		}
	}

	return true ;
}

// Function name	: on_user_replace_edit_record
LRESULT CCommonWindowFunctionality::on_user_replace_edit_record( )
{
	return 0L ;
}

// Function name	: init_edit_replace_window
void CCommonWindowFunctionality::init_edit_replace_window( int show_command /* = SW_HIDE */ )
{
	if ( ! m_edit_replace.IsWindow() )
	{
		ATLASSERT( IsWindow() ) ;	// let's make sure we are not passing around null HWNDs
		m_edit_replace.Create( m_hWnd ) ;
	}
	ATLASSERT( m_edit_replace.IsWindow() ) ;
	m_edit_replace.ShowWindow( show_command ) ;
}


// Function name	: init_edit_find_window
// Description	    : 
// Return type		: void 
// Argument         : int show_command = SW_HIDE
void CCommonWindowFunctionality::init_edit_find_window( int show_command /* = SW_HIDE */ )
{
	if ( ! m_edit_find.IsWindow() )
	{
		ATLASSERT( IsWindow() ) ;	// let's make sure we are not passing around null HWNDs
		m_edit_find.Create( m_hWnd ) ;
	}

	ATLASSERT( m_edit_find.IsWindow() ) ;
	m_edit_find.ShowWindow( show_command ) ;
}

HWND CCommonWindowFunctionality::init_view()
{
	ATLASSERT( IsWindow() ) ;

	HWND client = NULL ;
	m_view_interface.create( m_hWnd, client ) ;
	m_view_interface.set_listener( static_cast< CHtmlViewListener* >( this ) ) ;
	return client ;
}

// make sure that the user wants to delete the entry.
// TODO: Change to undoable action.
bool CCommonWindowFunctionality::check_delete()
{
	ATLASSERT( IsWindow() ) ;
	return IDYES == ::MessageBox( m_hWnd, R2T( IDS_PROMPT_DELETE ), R2T( IDS_DELETE_RECORD ), MB_YESNO | MB_ICONEXCLAMATION | MB_SETFOREGROUND ) ;
}


bool CCommonWindowFunctionality::show_edit_dialog_for_new_entry(const int title_id)
{
	init_edit_window() ;
	
	// set the record into the edit dialog
	this->set_new_record(record_pointer(new record_local())) ;
	m_editor.set_old_record( get_new_record() ) ;

	m_editor.set_new_record(record_pointer(new record_local())) ;
	
	// This is the memory that the entry will be added to
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;
	m_editor.set_memory_id( mem->get_id() ) ;

	search_match_ptr match(new mem_engine::search_match) ;
	match->set_record(get_new_record()) ;
	match->set_memory_id(mem->get_id()) ;
	this->set_item_under_edit(match) ;
	
	m_editor.set_display_state( NEW_RECORD_DISPLAY_STATE ) ;

	m_editor.SetWindowText( R2T( title_id ) ) ;
#ifndef UNIT_TEST
	m_editor.ShowWindow( SW_SHOW ) ;
#endif

	return true ;
}

// Function name	: CCommonWindowFunctionality::init_edit_window
bool CCommonWindowFunctionality::init_edit_window(int show_command /* = SW_HIDE */ )
{

#ifdef UNIT_TEST
	return true ;
#endif

	if ( ! m_editor.IsWindow() )
	{
		_Module.AddCreateWndData( &m_editor.m_thunk.cd, (CDialogImplBaseT< TWindow >*)&m_editor);

		DLGPROC lpDialogProc = (DLGPROC)m_editor.StartDialogProc ;
		int res_id = IDD_EDIT_RECORD ;

		instantiate_dlg( res_id, lpDialogProc ) ;

		ATLASSERT( m_editor.IsWindow() ) ;

		if ( ! m_editor.IsWindow() )
		{
			throw except::CException( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
		}
	}
	m_editor.ShowWindow( show_command ) ;
	m_editor.SetFocus() ;

	return ( !! m_editor.IsWindow() ) ;

}

// Create a dialog the hard way, because it was failing before.
void CCommonWindowFunctionality::instantiate_dlg(int res_id, DLGPROC lpDialogProc)
{
	HINSTANCE hInstance = _Module.GetResourceInstance() ;
	ATLASSERT( hInstance != NULL ) ;
	TRUE_ENFORCE( hInstance != NULL, _T("Resource Instance not found") ) ;

	ATLASSERT( IsWindow() ) ;
	HWND hWndParent = m_hWnd ;
	LPARAM dwInitParam = 1 ;

	ATLVERIFY(AtlAxWinInit());

	LPCTSTR lpTemplateName = MAKEINTRESOURCE( res_id ) ;
	HRSRC hDlg = ::FindResource(hInstance, lpTemplateName, (LPCTSTR)RT_DIALOG);

	HRSRC hDlgInit = ::FindResource(hInstance, lpTemplateName, (LPCTSTR)_ATL_RT_DLGINIT);
	ATLASSERT( GetLastError() == ERROR_SUCCESS || GetLastError() == 1813 ) ; // specified resource type not found in image file
	CLEAR_WINERRORS ;

	HGLOBAL hData = NULL;
	BYTE* pInitData = NULL;
	HWND hWnd = NULL ;

	if (hDlgInit)
	{
		hData = ::LoadResource(hInstance, hDlgInit);
		pInitData = (BYTE*) ::LockResource(hData);
	}
	if (hDlg)
	{
		HGLOBAL hResource = LoadResource(hInstance, hDlg);
		ATLASSERT(hResource) ;
		DLGTEMPLATE* pDlg = (DLGTEMPLATE*) LockResource(hResource);
		LPCDLGTEMPLATE lpDialogTemplate;
		lpDialogTemplate = _DialogSplitHelper::SplitDialogTemplate(pDlg, pInitData);
		hWnd = ::CreateDialogIndirectParamA(hInstance, lpDialogTemplate, hWndParent, lpDialogProc, dwInitParam);
		if (lpDialogTemplate != pDlg)
		{
			GlobalFree(GlobalHandle(lpDialogTemplate));
		}
		UnlockResource(hResource);
		FreeResource(hResource);
	}
	if (pInitData && hDlgInit)
	{
		UnlockResource(hData);
		FreeResource(hData);
	}

}

// Function name	: CCommonWindowFunctionality::init_find_window
bool CCommonWindowFunctionality::init_find_window(int show_command /* = SW_HIDE */, int title /* = 0 */ )
{

	ATLASSERT( IsWindow() ) ;
	if ( ! m_find.IsWindow() )
		m_find.Create( m_hWnd ) ;

	ATLASSERT( m_find.IsWindow() ) ;

	if ( title )
	{
		m_find.SetWindowText( R2T( title ) ) ;
	}

	m_find.ShowWindow( show_command ) ;

	return ( !! m_find.IsWindow() ) ;

}


// Function name	: get_size
size_t CCommonWindowFunctionality::get_size()
{ 
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;
	return mem->size() ;
}


// Function name	: show_edit_dialog
// Description	    : 
// Return type		: void 
// Argument         : record_pointer &record
// Argument         : UINT title_id = IDS_EDIT_GLOSS
void CCommonWindowFunctionality::show_edit_dialog( record_pointer &record, const int memory_id, UINT title_id /* = IDS_EDIT_GLOSS */ )
{
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;

	// make sure the window is created, 
	// and hide it while we do our magic
	init_edit_window( SW_HIDE ) ;
	m_editor.set_old_record( record ) ;
	m_editor.set_new_record( record_pointer((new record_local())) ) ;

	m_editor.set_memory_id( memory_id ) ;
	// remember our display state
	m_editor.set_display_state( get_display_state() ) ;
	m_editor.SetWindowText( R2T( title_id ) ) ;
#ifndef UNIT_TEST
	// show the dialog
	init_edit_window( SW_SHOW ) ;
#endif
}
// on_bold
// There is no default command for bold, so we have to intercept it 
// here and pass it down to our HTML view, in order to keep the view generic
// (i.e. no dependency on resource.h")
LRESULT CCommonWindowFunctionality::on_bold( )
{
	m_view_interface.do_bold() ;
	return 0L ;
}

// on_underline
// There is no default command for bold, so we have to intercept it 
// here and pass it down to our HTML view, in order to keep the view generic
// (i.e. no dependency on resource.h")
LRESULT CCommonWindowFunctionality::on_underline( )
{
	m_view_interface.do_underline() ;
	return 0L ;
}

// on_italic
// There is no default command for italic, so we have to intercept it 
// here and pass it down to our HTML view, in order to keep the view generic
// (i.e. no dependency on resource.h")
LRESULT CCommonWindowFunctionality::on_italic( )
{
	m_view_interface.do_italic() ;
	return 0L ;
}


// Function name	: get_display_sate
int CCommonWindowFunctionality::get_display_state()
{
	return m_display_state ;
}


// Function name	: prev_display_state
int CCommonWindowFunctionality::prev_display_state()
{

	if (get_display_state() != MATCH_DISPLAY_STATE)
	{
		set_display_state(MATCH_DISPLAY_STATE) ;
	}
	else
	{
		set_display_state(CONCORDANCE_DISPLAY_STATE) ;
	}
	return get_display_state() ;
}

// Function name	: next_display_state
int CCommonWindowFunctionality::next_display_state()
{
	return prev_display_state() ;
}

void CCommonWindowFunctionality::save_memory(memory_pointer mem)
{
	if ( mem->is_new() ) // no location
	{
		ATLASSERT( IsWindow() ) ;

		ATLVERIFY( SetForegroundWindow(m_hWnd) ) ;
		
		CString save_destination = get_save_destination();		
		if ( save_destination.IsEmpty() )
		{
			return ;
		}

		mem->set_location( save_destination ) ;
	}
	
	ATLASSERT( ! mem->is_new() ) ;

	mem->set_listener( static_cast< CProgressListener* >( this ) ) ;
	
	CString location = mem->get_location() ;
	const file::CFileExtension ext(location) ;
	const CString mem_ext = CString(".") + this->get_save_ext() ;
	if (! (ext.is_xml() || ext.equals(mem_ext)))
	{
		location += mem_ext ;
	}
	mem->set_location( location ) ;

	try
	{
		mem->save_memory() ;
	}
	catch ( CException &e )
	{
		e.notify_user( system_message( IDS_SAVE_FAILED, get_window_type_string() ) );
		return ;
	}
	
	m_mru.AddToList( mem->get_location() ) ;
	
}

CString CCommonWindowFunctionality::get_save_destination()
{
	windows_ui ui( m_hWnd ) ;
	CString save_destination = ui.get_save_file
		(
		(LPCTSTR)system_message( IDS_SAVE, get_window_type_string() ),
		this->get_save_filter(),
		this->get_save_ext()
		) ;
	return save_destination;
}
LRESULT CCommonWindowFunctionality::on_demo_check_excess_memories()
{
#ifdef UNIT_TEST
	return 0L ;
#endif
	// this is a trick to keep our code from being optimized or moved around
	// -- which could affect our defenses.
	if ( this != NULL )
	{
		size_t total_size = this->get_memory_model()->total_memory_size() ;

		if ( total_size < ( MAX_MEMORY_SIZE_FOR_DEMO * 2 ) + 100000 )
		{
			return 0L ;
		}

		if ( ! this->is_demo() )
		{
			return 0L ;
		}
		
		this->get_memory_model()->reduce_size_to( ( MAX_MEMORY_SIZE_FOR_DEMO * 2 ) ) ;
			
		throw CDemoException() ;
	}
	
	return 0L ;
}


/* Refresh the record-editor window after 
* we've switched GUI languages.
*/
void CCommonWindowFunctionality::refresh_editor_window()
{
	if ( ! m_editor.IsWindow() )
	{
		return ;
	}

	// fill in the new record from the dialog fields
	m_editor.fill_new_record() ;

	// remember it now...
	record_pointer record = m_editor.get_new_record() ;

	// remember if it was visible
	BOOL was_visible = m_editor.IsWindowVisible() ;
	RECT rc ;
	m_editor.GetWindowRect( &rc ) ;

	// destroy it
	m_editor.DestroyWindow() ;

	// ..and resurrect it
	// If it was visible, make it visible now
	init_edit_window( ( was_visible ? SW_SHOW : SW_HIDE ) ) ;
	m_editor.MoveWindow( &rc, !! was_visible ) ;

	// fill in the information
	m_editor.fill_from_record( record ) ;

}

/* Refresh the replace window after 
* we've switched GUI languages.
*/
void CCommonWindowFunctionality::refresh_replace_window()
{
	if ( ! m_edit_replace.IsWindow() )
	{
		return ;
	}

	const BOOL was_visible = m_edit_replace.IsWindowVisible() ;
	CWindowRect rect(m_edit_replace) ;
	m_edit_replace.DestroyWindow() ;
	// initialize
	init_edit_replace_window( was_visible ? SW_SHOW : SW_HIDE ) ;
	// put it back into place
	m_edit_replace.MoveWindow( &rect, TRUE ) ;
}

/* Refresh the edit-mode find window after 
* we've switched GUI languages.
*/
void CCommonWindowFunctionality::refresh_edit_find_window()
{
	if ( ! m_edit_find.IsWindow() )
	{
		return ;
	}

	const BOOL was_visible = m_edit_find.IsWindowVisible() ;
	CWindowRect rect(m_edit_find) ;
	m_edit_find.DestroyWindow() ;
	// initialize
	init_edit_find_window( was_visible ? SW_SHOW : SW_HIDE ) ;
	// put it back into place
	m_edit_find.MoveWindow( &rect, TRUE ) ;
}

/* Refresh the find window after 
* we've switched GUI languages.
*/
void CCommonWindowFunctionality::refresh_find_window()
{
	if ( ! m_find.IsWindow() )
	{
		return ;
	}

	const BOOL was_visible = m_find.IsWindowVisible() ;
	CWindowRect rect(m_find) ;
	m_find.DestroyWindow() ;
	// initialize
	init_find_window( was_visible ? SW_SHOW : SW_HIDE, IDS_MEMORY_SEARCH ) ;
	// put it back into place
	m_find.MoveWindow( &rect, TRUE ) ;
}

/* Refresh our various modeless dialogs after 
 * we've switched GUI languages.
 */
void CCommonWindowFunctionality::refresh_windows()
{
	refresh_find_window() ;
	refresh_edit_find_window() ;
	refresh_replace_window() ;
	refresh_editor_window() ;
}

/* Show the view content again after we've switched
 * GUI languages.
 */
void CCommonWindowFunctionality::refresh_view_content()
{
	// remember if we were in edit mode
	const bool was_enabled = m_view_interface.is_edit_mode() ;

	WindowsMessage message ;
	if ( was_enabled ) 
	{
		on_view_edit_mode( message ) ;
	}

	show_view_content() ;

	if ( was_enabled )
	{
		on_view_edit_mode( message ) ;
	}

}

// The user wants to search/replace while in edit mode
LRESULT CCommonWindowFunctionality::on_user_edit_search( LPARAM lParam  )
{
#ifdef UNIT_TEST
	return 0 ;
#endif

	if ( lParam == IDD_EDIT_FIND_DIALOG )
	{
		m_edit_find.set_search_params( m_edit_replace.get_search_params() ) ;
	}
	else
	{
		m_edit_replace.set_search_params( m_edit_find.get_search_params() ) ;
	}
	return 0L ;
}

// Was the file dropped in the client rect?
BOOL CCommonWindowFunctionality::dropped_in_client( CDropHandle dropped )
{
#ifdef UNIT_TEST
	dropped ;
	return TRUE ;
#endif
	CPoint p ;
	dropped.DragQueryPoint( &p ) ;

	CClientRect rect(*this) ;
	return rect.PtInRect( p ) ; 
}

/*
 * Handler for structured windows exception.
 * We used to show a crash dialog, but I got rid of that
 * because it was a failure point.
 */
BOOL CCommonWindowFunctionality::handle_sw_exception( CSWException &e, const CString &failure_message ) 
{
	user_feedback(failure_message) ;

	const CString message = get_swe_error_message(failure_message);
	e.notify_user( message, MB_OK, _T("Structured Windows Exception"), m_hWnd ) ;		 

	return FALSE ;											 
}

CString CCommonWindowFunctionality::get_swe_error_message(CString const&failure_message)
{
	CString window_text( _T("Exception") ) ;

	if ( IsWindow() )
	{
		FlashWindow( FALSE ) ;
		GetWindowText( window_text ) ;
	}
	CString message( window_text ) ;
	message += TEXT(":\r\n" ) + failure_message ;
	return message;
}

// The file was imported from a different format. 
// Does the user want to save in native format?
INT_PTR CCommonWindowFunctionality::wants_to_save_in_native_format()
{
	CString prompt ;
	prompt.FormatMessage( IDS_QUERY_NATIVE, get_window_type_string() ) ;

	CString title ;
	title.FormatMessage( IDS_SAVE, get_window_type_string() ) ;

	return MessageBox( prompt, title, MB_YESNOCANCEL ) ;
}


// So we can remember the size and position on startup
void CCommonWindowFunctionality::save_window_settings( LPCTSTR key_name )
{
	CWindowSettings ws;
	ws.GetFrom(*this);
	ws.Save( resource_string(IDS_REG_KEY), key_name );
}

// Show the Memory Manager dialog
bool CCommonWindowFunctionality::show_mem_mgr_dlg(int title_id)
{
	CMemoryManagerDlg dlg(title_id) ;

	dlg.set_memories( this->get_memory_model() ) ;
#ifdef UNIT_TEST
	return true ;
#endif
	if( IDOK != dlg.DoModal( ) )
	{
		return false ;
	}

	dlg.get_memories( this->get_memory_model() ) ;

	this->PostMessage( WM_COMMAND, MAKEWPARAM( IDC_DEMO_CHECK_EXCESS, 100 ), 0 ) ;

	return true ;
}

// Save all the memories for this window
void CCommonWindowFunctionality::save_all_memories()
{
	memory_list &memories = this->get_memory_model()->get_memories() ;

	foreach(memory_pointer mem, memories)
	{
		do_save( mem ) ;
	}

}

// File -> Save All
LRESULT CCommonWindowFunctionality::on_file_save_all()
{
	save_all_memories() ;

	set_window_title() ;

	return 0L ;
}

/*
* Respond to control-mouse wheel by increasing/decreasing the
* font display size. Each message only changes the size by
* one tick.
*/
LRESULT CCommonWindowFunctionality::OnMouseWheel( UINT, WPARAM wparam, LPARAM )
{
	SENSE("OnMouseWheel") ;

	if (! (LOWORD(wparam) & MK_CONTROL))
	{
		return 1L;
	}
	const short zDelta = ((short)HIWORD(wparam)) ;
	if (zDelta < 0)
	{
		m_view_interface.run_script("decreaseFont") ;
		m_mousewheel_count-- ;
	}
	else if (zDelta > 0)
	{
		m_view_interface.run_script("increaseFont") ;
		m_mousewheel_count++ ;
	}
	return 0L ;
}

/*
 * When new content is displayed, the <td> tags don't propagate the 
 * global font size. We thus need to adjust them via JavaScript.
 * Since this is a slow operation, we only perform it if the
 * mouse-wheel (zoom) value is non-zero (positive or negative).
 */
void CCommonWindowFunctionality::check_mousewheel()
{
	if (! m_mousewheel_count)
	{
		return ;
	}
	m_view_interface.run_script("normalizeFontSizes") ;
}

/************************************************************************/
/* Progress Bar Related                                                 */
/************************************************************************/

void CCommonWindowFunctionality::OnProgressInit( const CString &file_name, size_t min_val, size_t max_val )
{
	m_statusbar.OnProgressInit(file_name, min_val, max_val) ;
}

void CCommonWindowFunctionality::OnProgressDoneLoad( size_t final_val )
{
	m_statusbar.OnProgressDoneLoad(final_val) ;
	set_window_title() ;
}
void CCommonWindowFunctionality::OnProgressDoneWrite( size_t final_val )
{
	m_statusbar.OnProgressDoneWrite(final_val) ;
	set_window_title() ;
}
bool CCommonWindowFunctionality::OnProgressLoadUpdate( size_t current_val ) // true to continue
{
	return m_statusbar.OnProgressLoadUpdate(current_val) ;
}
bool CCommonWindowFunctionality::OnProgressWriteUpdate( size_t current_val ) // true to continue
{
	return m_statusbar.OnProgressWriteUpdate(current_val) ;
}
/////////////////////// == end progress-bar related == //////////////

CCommonWindowFunctionality::MERGE_CHOICE CCommonWindowFunctionality::get_merge_choice( CQueryMergeDlg &dlg )
{
	app_props::properties_general props ;
	props.read_from_registry() ;

	if (props.m_data.m_query_merge)
	{
		dlg.m_iMerge = props.m_data.m_merge_choice ;
		if ( 
#ifdef UNIT_TEST
			false
#else
			IDCANCEL == dlg.DoModal()
#endif			
			)
		{
			user_feedback(IDS_CANCELLED_ACTION) ;
			user_feedback(CString(), 2) ;
			return MERGE_CHOICE_CANCEL ;
		}
		props.m_data.m_merge_choice = dlg.m_iMerge ;
		props.m_data.m_query_merge = ! dlg.m_bDontAsk ;
		props.write_to_registry() ;
	}

	if (props.m_data.m_merge_choice == IDC_MERGE)
	{
		return MERGE_CHOICE_MERGE ;
	}
	return MERGE_CHOICE_SEPARATE ;
}

/*!
* Sets the background color of the HTML view.
*/
void CCommonWindowFunctionality::set_bg_color( COLORREF c )
{
	const CColorRef color( c ) ;
	// turn it into an HTML tag
	wstring color_str = color.as_wstring() ;
#ifndef UNIT_TEST
	CHtmlDocument doc = m_view_interface.get_document() ;
	doc.set_bg_color( color_str ) ;
#endif
}
