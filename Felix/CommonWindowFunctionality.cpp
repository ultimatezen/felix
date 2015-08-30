#include "StdAfx.h"
#include "CommonWindowFunctionality.h"
#include "RegSettings.h" // CWindowSettings
#include "MemoryManagerDlg.h"	// CMemoryManagerDlg
#include "NumberFmt.h"
#include "DemoException.h"
#include "xpmenu/Tools.h"		// CWindowRect
#include "record_local.h"
#include "FileOpHandler.h"
#include "export_tabbed_text.h"
#include "ExcelExporter.h"
#include "ExcelInterfaceReal.h"
#include "input_device.h"
#include "output_device.h"
#include "file_dialog.h"

using namespace except ;
using namespace mem_engine ;
using namespace html ;

// stand-alone functions
void add_common_tb_commands(std::vector< int > &commands)
{
	commands += 
		ID_FILE_NEW,	ID_FILE_OPEN, ID_MEMORY_CLOSE,	SEP_ID,
		ID_FILE_SAVE,	ID_FILE_SAVE_ALL, SEP_ID,
		ID_NEXT_PANE,   SEP_ID,
		ID_EDIT_CUT,	ID_EDIT_COPY,	ID_EDIT_PASTE,	SEP_ID,
		ID_EDIT_FIND ;
}
void add_common_std_bitmaps(std::vector< int > &StdBitmaps)
{
	StdBitmaps += 
		IDB_NEW_DOCUMENT,	IDB_OPEN, IDB_MEMORY_CLOSE,	
		IDB_SAVE,	IDB_SAVEMANY,
		IDB_SWITCH_VIEWS,
		IDB_CUT,			IDB_COPY,	IDB_PASTE,	
		IDB_SEARCH ;
}
void create_tb_imagelist(CImageList &images, std::vector< int > &StdBitmaps)
{
	images.Create(BM_SIZE, BM_SIZE, ILC_COLOR24 | ILC_MASK, 0, StdBitmaps.size() + 1 ) ;
	FOREACH(int img_id, StdBitmaps)
	{
		CBitmap bmp ;
		ATLVERIFY(bmp.LoadBitmap(img_id)) ;
		images.Add(bmp, MAGENTA) ;
	}
}

bool is_menu_key_pressed() 
{
	const int key_mask = 0x8000 ;
	if (::GetKeyState(VK_MENU) & key_mask)
	{
		return true ;
	}
	return false ;
}

//////////////////////////////////////////////////////////////////////////
// CCommonWindowFunctionality class
//////////////////////////////////////////////////////////////////////////

// CTOR
CCommonWindowFunctionality::CCommonWindowFunctionality( ) 
{

}

CCommonWindowFunctionality::~CCommonWindowFunctionality()
{

}


CString CCommonWindowFunctionality::get_location()
{
	auto model = this->get_memory_model() ;
	memory_pointer mem = model->get_first_memory() ;
	return mem->get_location() ;
}

// Function name	: is_demo
bool CCommonWindowFunctionality::is_demo() const
{
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

	FOREACH (memory_pointer mem, memories_needing_saving)
	{
		if (check_save_memory(mem) == IDCANCEL)
		{
			return IDCANCEL	;
		}

	}

	return IDYES ;
}

INT_PTR CCommonWindowFunctionality::LetUserSaveMemory(mem_engine::memory_pointer& mem)
{
	if ( mem->is_new() )
	{
		CString location = get_save_destination();

		if ( location.IsEmpty() )
		{
			return IDCANCEL ;
		}


		mem->set_location(location);
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
		CString file_name = this->get_save_destination();
		if ( file_name.IsEmpty() ) 
		{
			return false ;
		}
		
		// The user chose a location at which to save the memory.
		mem->set_mem_location( file_name ) ;
	}

	return true ;
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
#ifdef UNIT_TEST
	return true ;
#else
	ATLASSERT( IsWindow() ) ;
	return IDYES == ::MessageBox( m_hWnd, R2T( IDS_PROMPT_DELETE ), R2T( IDS_DELETE_RECORD ), MB_YESNO | MB_ICONEXCLAMATION | MB_SETFOREGROUND ) ;
#endif
}


bool CCommonWindowFunctionality::show_edit_dialog_for_new_entry(const int title_id)
{
	init_edit_window() ;
	
	get_editor()->m_is_add = true ;
	// set the records into the edit dialog
	get_editor()->set_old_record(record_pointer(new record_local())) ;
	get_editor()->set_new_record(record_pointer(new record_local())) ;
	
	// This is the memory that the entry will be added to
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;
	get_editor()->set_memory_id( mem->get_id() ) ;

	search_match_ptr match(mem->make_match(get_new_record())) ;
	this->set_item_under_edit(match) ;
	
	// This is so that when we edit the record, the editor can tell us
	// the display state we were in when we started editing it.
	get_editor()->set_display_state( NEW_RECORD_DISPLAY_STATE ) ;

	get_editor()->SetWindowText( R2T( title_id ) ) ;
#ifndef UNIT_TEST
	get_editor()->ShowWindow( SW_SHOW ) ;
#endif

	return true ;
}

// Function name	: CCommonWindowFunctionality::init_edit_window
bool CCommonWindowFunctionality::init_edit_window(int show_command /* = SW_HIDE */ )
{

#ifdef UNIT_TEST
	show_command ;
	return true ;
#else

	if ( ! get_editor()->IsWindow() )
	{
		get_editor()->Create(*this) ;

		ATLASSERT( get_editor()->IsWindow() ) ;

		if ( ! get_editor()->IsWindow() )
		{
			throw except::CException( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
		}
	}
	get_editor()->ShowWindow( show_command ) ;
	get_editor()->SetFocus() ;

	return ( !! get_editor()->IsWindow() ) ;

#endif
}

// Create a dialog the hard way, because it was failing before.
void CCommonWindowFunctionality::instantiate_dlg(int res_id, DLGPROC lpDialogProc)
{
	HINSTANCE hInstance = _Module.GetResourceInstance() ;
	ATLASSERT( hInstance != NULL ) ;
	TRUE_ENFORCE( hInstance != NULL, _T("Resource Instance not found") ) ;

	ATLASSERT( IsWindow() ) ;
	HWND hWndParent = m_hWnd ;

	ATLVERIFY(AtlAxWinInit());

	LPCTSTR lpTemplateName = MAKEINTRESOURCE( res_id ) ;
	HRSRC hDlg = ::FindResource(hInstance, lpTemplateName, (LPCTSTR)RT_DIALOG);

	HRSRC hDlgInit = ::FindResource(hInstance, lpTemplateName, (LPCTSTR)_ATL_RT_DLGINIT);
	ATLASSERT( GetLastError() == ERROR_SUCCESS || GetLastError() == 1813 ) ; // specified resource type not found in image file
	CLEAR_WINERRORS ;

	HGLOBAL hData = NULL;
	BYTE* pInitData = NULL;

	if (hDlgInit)
	{
		hData = ::LoadResource(hInstance, hDlgInit);
		if (hData)
		{
			pInitData = (BYTE*) ::LockResource(hData);
		}
		else
		{
			logging::log_error("Failed to instantiate dialog with id " + int2string(res_id)) ;
			this->user_feedback(CString("Failed to instantiate dialog")) ;
			return ;
		}
	}
	if (hDlg)
	{
		HGLOBAL hResource = LoadResource(hInstance, hDlg);
		ATLASSERT(hResource) ;
		if (hResource)
		{
			LPARAM dwInitParam = 1 ;
			HWND hWnd = NULL ;
			DLGTEMPLATE* pDlg = (DLGTEMPLATE*) LockResource(hResource);
			LPCDLGTEMPLATE lpDialogTemplate;
			lpDialogTemplate = _DialogSplitHelper::SplitDialogTemplate(pDlg, pInitData);
			hWnd = ::CreateDialogIndirectParamA(hInstance, lpDialogTemplate, hWndParent, lpDialogProc, dwInitParam);
			if (lpDialogTemplate != pDlg)
			{
				GlobalFree(GlobalHandle(lpDialogTemplate));
			}
			UnlockResource(hResource);
			ATLASSERT(hResource) ;
			FreeResource(hResource);
			if(!hWnd)
			{
				return ;
			}
		}
	}
	if (pInitData && hDlgInit && hData)
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
void CCommonWindowFunctionality::show_edit_dialog( record_pointer record, const int memory_id, UINT title_id /* = IDS_EDIT_GLOSS */ )
{
	memory_pointer mem = this->get_memory_model()->get_first_memory() ;

	// make sure the window is created, 
	// and hide it while we do our magic
	get_editor()->m_is_add = false ;
	init_edit_window( SW_HIDE ) ;
	get_editor()->set_old_record( record ) ;
	get_editor()->set_new_record( record_pointer((new record_local())) ) ;

	get_editor()->set_memory_id( memory_id ) ;
	// remember our display state
	get_editor()->set_display_state( get_display_state() ) ;
	get_editor()->SetWindowText( R2T( title_id ) ) ;
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
	CString save_ext = L"ftm";
	if (this->is_glossary_window())
	{
		save_ext = L"fgloss";
	}

	const CString mem_ext = CString(".") + save_ext;

	// If the save filename isn't an XML file, and it
	// doesn't have the default save extension for the memory
	// type (ftm or fgloss), add the extension to the filename.
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
	CString filename;
	memory_pointer mem = this->get_memory_model()->get_first_memory();
	if (file::CPath(mem->get_fullpath()).FileExists())
	{
		filename = mem->get_fullpath();
	}

	auto history = &app_props::get_props()->m_history_props;

	if (this->is_glossary_window())
	{
		return save_glossary_file(
			history->m_memory_location, // last_save
			filename // filename
			);
	}
	else
	{
		return save_memory_file(
			history->m_glossary_location, // last_save
			filename // filename
			);
	}
}
LRESULT CCommonWindowFunctionality::on_demo_check_excess_memories()
{
#ifdef UNIT_TEST
	return 0L ;
#else
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
	
	return 0L ;
#endif
}


/* Refresh the record-editor window after 
* we've switched GUI languages.
*/
void CCommonWindowFunctionality::refresh_editor_window()
{
	if ( ! get_editor()->IsWindow() )
	{
		return ;
	}

	// fill in the new record from the dialog fields
	get_editor()->fill_new_record() ;

	// remember it now...
	record_pointer record = get_editor()->get_new_record() ;

	// remember if it was visible
	BOOL was_visible = get_editor()->IsWindowVisible() ;
	RECT rc ;
	get_editor()->GetWindowRect( &rc ) ;

	// destroy it
	get_editor()->DestroyWindow() ;

	// ..and resurrect it
	// If it was visible, make it visible now
	init_edit_window( ( was_visible ? SW_SHOW : SW_HIDE ) ) ;
	get_editor()->MoveWindow( &rc, !! was_visible ) ;

	// fill in the information
	get_editor()->fill_from_record( record ) ;

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
	lParam ;
	return 0 ;
#else

	if ( lParam == IDD_EDIT_FIND_DIALOG )
	{
		m_edit_find.set_search_params( m_edit_replace.get_search_params() ) ;
	}
	else
	{
		m_edit_replace.set_search_params( m_edit_find.get_search_params() ) ;
	}
	return 0L ;
#endif
}

// Was the file dropped in the client rect?
BOOL CCommonWindowFunctionality::dropped_in_client( CDropHandle dropped )
{
#ifdef UNIT_TEST
	dropped ;
	return TRUE ;
#else
	CPoint p ;
	dropped.DragQueryPoint( &p ) ;

	CClientRect rect(*this) ;
	return rect.PtInRect( p ) ; 
#endif
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
	CMemoryManagerDlg dlg(app_props::get_props(), title_id) ;

	dlg.set_memories( this->get_memory_model()->get_memories() ) ;
#ifdef UNIT_TEST
	return true ;
#else
	if( IDOK != dlg.DoModal( ) )
	{
		return false ;
	}

	dlg.get_memories( this->get_memory_model()->get_memories() ) ;

	this->PostMessage( WM_COMMAND, MAKEWPARAM( IDC_DEMO_CHECK_EXCESS, 100 ), 0 ) ;

	return true ;
#endif
}

// Save all the memories for this window
void CCommonWindowFunctionality::save_all_memories()
{
	memory_list &memories = this->get_memory_model()->get_memories()->get_memories() ;

	FOREACH(memory_pointer mem, memories)
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
#ifdef UNIT_TEST
	return ;
#else
	m_view_interface.run_script("normalizeFontSizes") ;
#endif
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

CCommonWindowFunctionality::MERGE_CHOICE CCommonWindowFunctionality::get_merge_choice( CQueryMergeDlg &dlg, app_props::properties_general *props )
{
	if (props->m_data.m_query_merge)
	{
		dlg.m_iMerge = props->m_data.m_merge_choice ;
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
		props->m_data.m_merge_choice = dlg.m_iMerge ;
		props->m_data.m_query_merge = ! dlg.m_bDontAsk ;
	}

	if (props->m_data.m_merge_choice == IDC_MERGE)
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

void CCommonWindowFunctionality::remove_memory( mem_engine::memory_pointer mem, int msgid )
{
	this->get_memory_model()->remove_memory_by_id(mem->get_id()) ;
	this->set_window_title() ;

	user_feedback( system_message(msgid, mem->get_location()) ) ;
}

void CCommonWindowFunctionality::initialize_values( void )
{
	m_is_short_format = true ;
	m_silent_mode = false ;
	m_mousewheel_count = 0;
	m_new_record = record_type(new mem_engine::record_local);
	m_review_match = match_ptr(new match_type(record_type(new mem_engine::record_local)));
	m_item_under_edit = match_ptr(new match_type(record_type(new mem_engine::record_local)));
}

void CCommonWindowFunctionality::init_state( ViewState *state )
{
	state->set_model(this->get_model()) ;
	state->set_window_listener(this) ;
	state->set_view(&m_view_interface) ;
}

void CCommonWindowFunctionality::init_tooltip_map( std::map<int, int> &toolmap )
{
	toolmap[ID_FILE_NEW] = IDS_NEW_TOOLBAR ;
	toolmap[ID_FILE_OPEN] = IDS_OPEN_TOOLBAR ;
	toolmap[ID_EDIT_CUT] = ID_EDIT_CUT ;
	toolmap[ID_EDIT_COPY] = ID_EDIT_COPY ;
	toolmap[ID_EDIT_PASTE] = ID_EDIT_PASTE ;
	toolmap[ID_TOOLS_PREFERENCES] = ID_TOOLS_PREFERENCES ;
	toolmap[ID_HELP] = ID_HELP  ;
	toolmap[ID_APP_ABOUT] = ID_APP_ABOUT ;
	toolmap[ID_NEXT_PANE] = ID_NEXT_PANE ;
}

void CCommonWindowFunctionality::handle_tooltip( LPNMHDR pnmh, int idCtrl, std::map<int, int> &toolmap )
{
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
}
/************************************************************************/
/* WindowInterface implementation                                       */
/************************************************************************/

long CCommonWindowFunctionality::get_height()
{
	SENSE("get_height") ;
	CWindowRect rect(*this) ;
	return rect.Height() ;
}

void CCommonWindowFunctionality::set_height( long height )
{
	SENSE("set_height") ;
	CWindowRect rect(*this) ;
	rect.bottom = rect.top + height ;
	this->MoveWindow(&rect) ;
}

long CCommonWindowFunctionality::get_width()
{
	SENSE("get_width") ;
	CWindowRect rect(*this) ;
	return rect.Width() ;
}

void CCommonWindowFunctionality::set_width( long width )
{
	SENSE("set_width") ;
	CWindowRect rect(*this) ;
	rect.right = rect.left + width ;
	this->MoveWindow(&rect) ;
}

long CCommonWindowFunctionality::get_left()
{
	SENSE("get_left") ;
	CWindowRect rect(*this) ;
	return rect.left ;
}

void CCommonWindowFunctionality::set_left( long left )
{
	SENSE("set_left") ;
	CWindowRect rect(*this) ;
	long width = rect.Width() ;
	rect.left = left ;
	rect.right = left + width ;
	this->MoveWindow(&rect) ;
}

long CCommonWindowFunctionality::get_top()
{
	SENSE("get_top") ;
	CWindowRect rect(*this) ;
	return rect.top ;
}

void CCommonWindowFunctionality::set_top( long top )
{
	SENSE("set_top") ;
	CWindowRect rect(*this) ;
	long height = rect.Height() ;
	rect.top = top ;
	rect.bottom = top + height ;
	this->MoveWindow(&rect) ;
}

void CCommonWindowFunctionality::raise()
{
	CWindowRect rect(*this) ;
	UINT flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW ;
	// The first call raises it to the top, the second call ensures
	// that it's not pegged to the top
	this->SetWindowPos(HWND_TOPMOST, &rect, flags) ;
	this->SetWindowPos(HWND_NOTOPMOST, &rect, flags) ;
}


void CCommonWindowFunctionality::export_excel( CString file_name, mem_engine::memory_pointer mem )
{
	logging::log_debug("Saving as Excel file") ;
	fileops::add_extension_as_needed( file_name,  _T( ".xls" ) ) ;
	CExcelExporter exporter ( static_cast< CProgressListener* >( this ),
		ExcelInterfacePtr(new ExcelInterfaceReal) ) ;
	exporter.export_excel( mem, file_name, get_input_device() ) ;
}

void CCommonWindowFunctionality::export_tabbed_text( CString save_as_file_name, mem_engine::memory_pointer mem )
{
	using namespace mem_engine;

	logging::log_debug("Saving as tab-separated text file") ;
	user_feedback( IDS_MSG_EXPORTING_RECORDS ) ;
	fileops::add_extension_as_needed( save_as_file_name,  _T( ".txt" ) ) ;
	output_device_ptr output(new OutputDeviceFile) ;
	output->open(save_as_file_name) ;
	tabbed_export::TabbedTextExporter exporter(output) ;
	
	trans_set records ;
	FOREACH(record_pointer record, mem->get_records() | ad::map_values)
	{
		records.insert(record) ;
	}
	exporter.write_memory(records.begin(), records.end()) ;
	output->close() ;
	CNumberFmt nf ;
	CString feedback ;
	file::CPath path(save_as_file_name) ;
	path.StripPath() ;
	feedback.FormatMessage( IDS_EXPORTED_RECORDS, 
		nf.Format( mem->size() ), 
		path.Path() ) ;
	user_feedback( feedback ) ;
}

// We have two find dialogs: 
// * one for edit mode (find in the browser);
// * and one for non-edit mode (quick search in glossary)
// Swap them when we change the edit mode.
void CCommonWindowFunctionality::SwapFindDialogs( const bool edit_mode_enabled )
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

CCommonWindowFunctionality::match_ptr CCommonWindowFunctionality::get_item_under_edit()
{
	return m_item_under_edit ;
}

void CCommonWindowFunctionality::set_item_under_edit( match_ptr match )
{
	m_item_under_edit = match ;
}

LRESULT CCommonWindowFunctionality::add_remote_memory( model_iface_ptr memories, mem_engine::memory_pointer mem )
{
	if (! mem)
	{
		logging::log_warn("Failed to connect to glossary"); 
		user_feedback(CString("Connection failed!")) ;
		return -1L ;
	}
	memories->insert_memory(mem) ;

	CString feedback ;
	feedback.FormatMessage(IDS_CONNECTED_MEMORY, (LPCTSTR)mem->get_location()) ;
	user_feedback(feedback) ;

	this->set_window_title() ;
	return 0L ;
}

void CCommonWindowFunctionality::set_zoom_level( int zoom_level )
{
	m_mousewheel_count = zoom_level ;
	m_view_interface.run_script("resetFontSizes") ;
	if (zoom_level)
	{
		CString command = get_mousewheel_command(zoom_level);

		for (int i = 0 ; i < abs(zoom_level); ++i)
		{
			m_view_interface.run_script(command) ;
		}
	}
}

/************************************************************************/
/* window_wrapper implementation                                       */
/************************************************************************/

BOOL CCommonWindowFunctionality::is_window()
{
	window_wrapper_ptr window = this->m_get_window(this->m_hWnd) ;
	return window->IsWindow() ;
}

BOOL CCommonWindowFunctionality::show_window( int show_command )
{
	window_wrapper_ptr window = this->m_get_window(this->m_hWnd) ;
	return window->ShowWindow(show_command) ;
}

HWND CCommonWindowFunctionality::set_focus()
{
	window_wrapper_ptr window = this->m_get_window(this->m_hWnd) ;
	return window->SetFocus() ;
}

CCommonWindowFunctionality::record_type CCommonWindowFunctionality::get_new_record()
{
	return m_new_record ;
}

CCommonWindowFunctionality::match_ptr CCommonWindowFunctionality::get_review_match()
{
	return m_review_match ;
}

void CCommonWindowFunctionality::set_new_record( record_type rec )
{
	m_new_record = rec ;
}

void CCommonWindowFunctionality::set_review_match( record_type rec, int mem_id )
{
	m_review_match->set_record(rec) ;
	m_review_match->set_values_to_record() ;
	m_review_match->set_memory_id(mem_id) ;
}

INT_PTR CCommonWindowFunctionality::prompt_user_for_overwrite( CString location )
{
	CString prompt ;
	prompt.FormatMessage( IDS_PROMPT_OVERWRITE_MEMORY, (LPCTSTR)location ) ;
	return (INT_PTR)MessageBox( prompt, 
		resource_string( IDS_PROMPT_OVERWRITE_MEMORY_TITLE ), 
		MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_SETFOREGROUND ) ;
}


