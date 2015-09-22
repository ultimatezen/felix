// MemoryManagerDlg.cpp: implementation of the CMemoryManagerDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MemoryManagerDlg.h"

#include "ui.h"

#include "Path.h"
#include "File.h"

#include "background_processor.h"
#include "resource_string.h"
#include "memory_local.h"

#include "UserName.h"
#include "query.h"
#include "xpmenu/Tools.h"
#include "Drop.h"
#include "system_message.h"
#include "input_device_file.h"
#include "resizeable_dialog.h"
#include "file_dialog.h"

using namespace html ;
using namespace mem_engine ;

const wstring get_item(const wstring item)
{
	if (item.empty())
	{
		return wstring(L"Default") ;
	}
	return item ;
}


void CMemoryManagerDlg::OnProgressInit( const CString &, size_t min_val, size_t max_val )
{
	SENSE("OnProgressInit") ;
#ifdef UNIT_TEST
	min_val ;
	max_val ;

	return ;
#else
	ATLASSERT ( m_progress_bar.IsWindow() ) ; 

	m_progress_bar.ShowWindow( SW_SHOW ) ;

	m_progress_bar.SetRange( min_val, max_val ) ;
	m_progress_bar.SetPos( min_val ) ;
#endif
}
void CMemoryManagerDlg::OnProgressDoneLoad( size_t final_val )
{
	SENSE("OnProgressDoneLoad") ;
#ifdef UNIT_TEST
	final_val ;

	return ;
#else
	m_progress_bar.SetPos( final_val ) ;
	m_progress_bar.ShowWindow(SW_HIDE) ;
#endif
}
void CMemoryManagerDlg::OnProgressDoneWrite( size_t final_val )
{
	SENSE("OnProgressDoneWrite") ;
#ifdef UNIT_TEST
	final_val ;
	return ;
#else
	m_progress_bar.SetPos( final_val ) ;
	m_progress_bar.ShowWindow(SW_HIDE) ;
#endif
}

bool CMemoryManagerDlg::OnProgressLoadUpdate( size_t current_val ) // true to continue
{
	SENSE("OnProgressLoadUpdate") ;
#ifdef UNIT_TEST
	current_val ;
	return true;
#else
	m_progress_bar.SetPos( current_val ) ;

	return true ;
#endif
}
bool CMemoryManagerDlg::OnProgressWriteUpdate( size_t current_val ) // true to continue
{
	SENSE("OnProgressWriteUpdate") ;
#ifdef UNIT_TEST
	current_val ;
	return true;
#else
	m_progress_bar.SetPos( current_val ) ;

	return true ;
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryManagerDlg::CMemoryManagerDlg(app_props::props_ptr props, int title_id) : 
	m_props(props),
	m_title_id(title_id)
{

}

CMemoryManagerDlg::~CMemoryManagerDlg()
{

}

LRESULT CMemoryManagerDlg::OnSize(UINT type, CSize size)
{
	SENSE("CMemoryManagerDlg::OnSize") ;

	if (! handle_common_resize_tasks(*this, type, size))
	{
		return 0L ;
	}

	// get its position
	sizeInfoBox();

	const CClientRect clientrect(*this) ;

	RECT listrect = {0} ;
	m_list_box.GetWindowRect(&listrect) ;
	//ScreenToClient(&listrect) ;
	listrect.top = clientrect.top + 5 ;
	listrect.left = clientrect.left + 10 ;
	listrect.right = clientrect.right - 10 ;

	CWindow upbutton(GetDlgItem(IDC_UP)) ;

	CWindowRect uprect(upbutton) ;
	ScreenToClient(&uprect);
	listrect.bottom = uprect.top - 15 ;

	// move the html view into place
	m_list_box.SetWindowPos(NULL, &listrect, SWP_NOZORDER | SWP_NOACTIVATE);

	return 0L ;
}

void CMemoryManagerDlg::sizeInfoBox()
{
	const CClientRect clientrect(*this) ;

	CWindowRect infobox_rect(m_info_box) ;
	ScreenToClient( &infobox_rect ) ;
	m_info_box.ShowWindow( SW_HIDE ) ;

	const int padding = 5 ;
	infobox_rect.left = clientrect.left + padding ;
	infobox_rect.right = clientrect.right - padding ;

	CWindowRect editbutton_rect(CWindow(GetDlgItem(IDC_EDIT_MEMORY))) ;
	ScreenToClient(&editbutton_rect);
	infobox_rect.bottom = editbutton_rect.top - padding ;

	// move the html view into place
	m_info_view.SetWindowPos(NULL, &infobox_rect, SWP_NOZORDER | SWP_NOACTIVATE);
}

LRESULT CMemoryManagerDlg::OnDropFiles( HDROP dropped ) 
{
	const CDrop drop(dropped) ;

	if ( ! is_dropped_in_listbox(drop.DragQueryPoint()) )
	{
		SetMsgHandled( FALSE ) ;
		return 0L ;
	} 

	const UINT num_files = drop.NumDragFiles() ;
	
	for ( UINT current_file=0 ; current_file < num_files ; ++current_file )
	{
		dropFile(drop.DragQueryFile(current_file));	
	}

	return 0L ;
}

bool CMemoryManagerDlg::is_dropped_in_listbox(const POINT p)
{
	CRect listrect ;
	m_list_box.GetClientRect(&listrect) ;

	return !! listrect.PtInRect( p ) ;
}


void CMemoryManagerDlg::dropFile(const CString dropfile)
{
	add_memory_file( dropfile ) ;
}

LRESULT CMemoryManagerDlg::OnAdvanced()
{
	int sel = m_list_box.GetSelectedIndex() ;

	if ( sel == LB_ERR )
	{
		MessageBeep( MB_ICONERROR ) ;
		return 0L ;
	}

	memory_pointer mem = get_memory_at(sel);

	CAdvancedMemMgrDlg dlg ;

	setAdvDlgInfo(mem, dlg);

	if ( IDCANCEL == dlg.DoModal() )
	{
		return 0L ;
	}

	getAdvDlgInfo(mem, dlg);

	display_info_for_item( sel ) ;

	return 0L ;
}

void CMemoryManagerDlg::getAdvDlgInfo(memory_pointer& mem, CAdvancedMemMgrDlg& dlg)
{
	if ( dlg.get_lock() == BST_CHECKED )
	{
		mem->set_locked_on() ;
	}
	else if ( dlg.get_lock() == BST_UNCHECKED )
	{
		mem->set_locked_off() ;
	}

	mem->batch_set_reliability( min( dlg.get_reliability(), 10 ) ) ;

	if ( dlg.get_validation() == BST_CHECKED )
	{
		mem->batch_set_validation( true ) ;
	}
	else if ( dlg.get_validation() == BST_UNCHECKED )
	{
		mem->batch_set_validation( false ) ;
	}

}

void CMemoryManagerDlg::setAdvDlgInfo(memory_pointer& mem, CAdvancedMemMgrDlg& dlg)
{
	size_t low, high ; 
	double ave ;
	boost::tuples::tie(low, high, ave) = mem->get_reliability_stats() ;
	if ( low == high )
	{
		dlg.set_reliability( low );
	}
	else
	{
		dlg.set_reliability( 100 );
	}

	dlg.set_lock( mem->is_locked() ? BST_CHECKED : BST_UNCHECKED ) ;
	wstring vper =  mem->get_validated_percent() ;
	if ( vper == L"100%" )
	{
		dlg.set_validation( BST_CHECKED ) ;
	}
	else if ( vper == L"0%" ) 
	{
		dlg.set_validation( BST_UNCHECKED ) ;
	}
	else 
	{
		dlg.set_validation( BST_INDETERMINATE ) ;
	}

}

// list stuff
void setListLineColor( LPNMLVCUSTOMDRAW pnmcd )
{
	const COLORREF even_line_color = RGB(220,220,255) ;
	const COLORREF odd_line_color = RGB(255,255,255) ;

	if ( (pnmcd->nmcd.dwItemSpec % 2) == 0 )
	{
		pnmcd->clrTextBk = even_line_color ;
	}
	else
	{
		pnmcd->clrTextBk = odd_line_color ;
	}

}

DWORD CMemoryManagerDlg::OnCustomDrawList( LPNMLVCUSTOMDRAW pnmcd )
{
	ATLASSERT( pnmcd->nmcd.hdr.idFrom == IDC_LIST ) ;

	if ( pnmcd->nmcd.dwDrawStage == CDDS_PREPAINT )
	{
		return CDRF_NOTIFYITEMDRAW ;
	}

	if( pnmcd->nmcd.dwDrawStage != CDDS_ITEMPREPAINT  ) 
	{
		return CDRF_DODEFAULT ;
	}

	setListLineColor(pnmcd);

	return CDRF_DODEFAULT ;
}

LRESULT CMemoryManagerDlg::OnInitDialog()
{
	SENSE("OnInitDialog") ;
	// center us on our parent
	CenterWindow(GetParent());
	
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
	
    // Init the CDialogResize code
#ifndef UNIT_TEST
    DlgResize_Init( true, true ) ;
#endif
	CRect WindowRect ;
	GetWindowRect(&WindowRect) ;
	CRect DesktopRect ;
	CWindow DesktopWindow = GetDesktopWindow() ;
	DesktopWindow.GetWindowRect( &DesktopRect ) ;

	if ( WindowRect.Height() >= DesktopRect.Height() )
	{
		WindowRect.top = 10 ;
		WindowRect.bottom = DesktopRect.bottom - 20 ;

		MoveWindow(&WindowRect) ;
	}

	 m_progress_bar = GetDlgItem( IDC_PROGRESS_BAR ) ;

#ifdef UNIT_TEST
	 return TRUE ;
#else

	// set up static where html view will go
	m_info_box.Attach( GetDlgItem( IDC_INFO_BOX ) ) ;
	ATLASSERT( m_info_box.IsWindow() ) ;

	// get its position
	RECT rc ;
	m_info_box.GetWindowRect( &rc ) ;
	ScreenToClient( &rc ) ;

	// create the html view and move it into place
	m_info_view.create( m_hWnd ) ;
	ATLASSERT( m_info_view.IsWindow() ) ;
	m_info_view.MoveWindow( &rc, TRUE ) ;

	waitForNavComplete();

	setUpListView();

	if ( m_memories.empty() )
	{
		disableListButtons();	
	}
	else
	{
		m_list_box.SelectItem( 0 ) ;

		waitForNavComplete();

		display_info_for_item( 0 ) ;
	}

	if (m_title_id)
	{
		this->SetWindowText(resource_string(m_title_id)) ;
	}

	return TRUE ;
#endif
}

void CMemoryManagerDlg::disableListButtons()
{
	// remove
	CWindow but = GetDlgItem( IDC_REMOVE_MEMORY ) ;
	but.EnableWindow( FALSE ) ;
	// up
	but = GetDlgItem( IDC_UP ) ;
	but.EnableWindow( FALSE ) ;
	// down
	but = GetDlgItem( IDC_DOWN ) ;
	but.EnableWindow( FALSE ) ;

}

void CMemoryManagerDlg::setUpListView()
{
	m_list_box.SubclassWindow( GetDlgItem( IDC_LIST ) ) ;
	::DragAcceptFiles( m_list_box, TRUE ) ;

	fill_listview() ;

}

void CMemoryManagerDlg::waitForNavComplete()
{
	background_processor backer ;
	while ( m_info_view.is_navigation_complete() == false )
	{
		backer.check_iterations() ;
	}

}

LRESULT CMemoryManagerDlg::OnDestroy()
{
	SetMsgHandled( FALSE ) ;

	m_info_box.Detach() ;
	m_list_box.Detach() ;
	m_info_view.DestroyWindow() ;
	m_info_view.Release() ;

	return 1 ;
}

LRESULT CMemoryManagerDlg::OnCmdOK()
{
	SENSE("OnOK") ;
	reflect_checkstate() ;
#ifndef UNIT_TEST
	EndDialog(IDOK);
#endif
	return 0L ;
}

LRESULT CMemoryManagerDlg::OnCmdCancel()
{
	SENSE("OnCancel") ;
#ifndef UNIT_TEST
	EndDialog(IDCANCEL);
#endif
	return 0L ;
}

LRESULT CMemoryManagerDlg::OnCmdAddMemory()
{
	file_open_dialog dialog;
	dialog.set_title(resource_string(IDS_OPEN));
	dialog.allow_multi_select();

	LPCTSTR fileext_gloss = _T("flgoss") ;
	LPCTSTR fileext_mem = _T("ftm") ;

	if (m_title_id == IDS_GLOSSARY_MANAGER_TITLE)
	{
		dialog.set_file_filter(get_gloss_open_filter());
		dialog.set_default_ext(fileext_gloss);
	}
	else
	{
		dialog.set_file_filter(get_mem_open_filter());
		dialog.set_default_ext(fileext_mem);
	}


	if ( ! dialog.show() )
	{
		return 0L ;
	}
	add_memory_files(dialog.get_open_destinations());

	return 0L ;
}
void CMemoryManagerDlg::add_memory_files(std::vector<CString> import_files)
{
	input_device_ptr input(new InputDeviceFile) ;
	for(auto filename: import_files)
	{
		input->ensure_file_exists(filename);
		add_memory_file(filename) ;
	}
}

LRESULT CMemoryManagerDlg::OnCmdRemoveMemory()
{
	int index = m_list_box.GetSelectedIndex() ;

	if ( index == -1 ) 
	{ 
		MessageBeep( MB_ICONEXCLAMATION ) ;
		return 0L ;
	}

	ATLASSERT( static_cast< size_t >( index ) < m_memories.size() ) ;

	memory_pointer mem = get_memory_at(index) ;

	reflect_checkstate() ;
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
					return 0L ;
				}
			}

			save_memory(mem);
			break ;
		case IDCANCEL:
			SetMsgHandled( TRUE ) ;
			return 0L ;
		}
	}

	m_memories.erase(get_pos_at(index)) ;

	m_list_box.DeleteAllItems() ;
	fill_listview() ;

	if ( index >= m_list_box.GetItemCount() )
	{
		index = m_list_box.GetItemCount() - 1 ;
	}

	m_list_box.SelectItem( index ) ;
	OnSelChange( ) ;

	set_button_focus();

	return 0L ;
}

void CMemoryManagerDlg::perform_save(memory_pointer& mem)
{
	mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

	try
	{
		mem->save_memory() ;
	}
	catch ( ... ) 
	{
		mem->set_listener( nullptr ) ;
		m_list_box.DeleteAllItems() ;
		fill_listview() ;
		throw ;
	}

	mem->set_listener(nullptr);

}

void CMemoryManagerDlg::showSavingMessage(memory_pointer& mem)
{
	m_info_view.set_body_text( get_saving_feedback(mem) ) ;
}

bool CMemoryManagerDlg::getMemName(memory_pointer& mem)
{
	CString location = get_save_destination(mem);

	if (location.IsEmpty())
	{
		SetMsgHandled( TRUE ) ;
		return false ;
	}

	mem->set_location(location);

	return true ;
}

LRESULT CMemoryManagerDlg::OnCmdEditMemory()
{
	bool is_edit_mode = ! m_info_view.get_edit_mode() ;

	CWindow button = GetDlgItem( IDC_EDIT_MEMORY ) ;
	if ( is_edit_mode )
	{
		button.SetWindowText( resource_string( IDS_END_EDIT  ) )  ;
	}
	else
	{
		button.SetWindowText( resource_string( IDS_START_EDIT  ) )  ;
	}
	m_info_view.put_edit_mode( is_edit_mode ) ;

	if ( is_edit_mode )
	{
		return 0L ;
	}

	int index = m_list_box.GetSelectedIndex() ;
	if( index == -1 )
		return 0L ;
	memory_pointer mem = get_memory_at(index) ;

	// get the collection of HTML elements in the doc body
	CHtmlDocument doc = m_info_view.get_document() ;
	CHtmlElementCollection collection = doc.get_all() ;
	
	// get the length of the collection
	int len = collection.get_length() ;
	
	MemoryInfo *mem_info = mem->get_memory_info() ;

	// loop through each of the elements
	for ( int i=0 ; i<len ; ++i )
	{
		CHtmlElement element = collection.item( i ) ;
		
		wstring id ;
		element.get_id( id ) ;
		
		wstring setting ;
		if ( ! id.empty() )
		{
			element.get_text( setting ) ;
			if ( id == L"field" ) 
			{
				mem_info->set_field( setting ) ;
			}
			else if ( id == L"source_language" ) 
			{
				mem_info->set_source_language( setting ) ;
			}
			else if ( id == L"target_language"  )
			{
				mem_info->set_target_language( setting ) ;
			}
			else if ( id == L"client" ) 
			{
				mem_info->set_client( setting ) ;
			}
		} 
	} // increment i...


	display_info_for_item(index) ;

	return 0L ;
}

LRESULT CMemoryManagerDlg::OnCmdUp()
{
	int index = m_list_box.GetSelectedIndex() ;

	ATLASSERT( index != 0 && index != LB_ERR ) ;
	if ( index == 0 || index == LB_ERR )
	{
		return 0L ;
	}

	reflect_checkstate() ;

	swap_memories(index-1) ;

	m_list_box.DeleteAllItems() ;
	fill_listview() ;
	m_list_box.SelectItem( index-1 ) ;
	OnSelChange( ) ;
	return 0L ;
}

LRESULT CMemoryManagerDlg::OnCmdDown()
{
	int index = m_list_box.GetSelectedIndex() ;
	
	ATLASSERT( index != ( m_list_box.GetItemCount() - 1 ) && index != LB_ERR ) ;
	if ( index == ( m_list_box.GetItemCount() - 1 ) || index == LB_ERR )
	{
		return 0L ;
	}

	reflect_checkstate() ;
	swap_memories(index);

	m_list_box.DeleteAllItems() ;
	fill_listview() ;
	m_list_box.SelectItem( index+1 ) ;
	OnSelChange( ) ;
	return 0L ;
}


LRESULT CMemoryManagerDlg::OnSelChange( )
{
	int index = m_list_box.GetSelectedIndex() ;

	CWindow but ;
	but = GetDlgItem( IDC_UP ) ;
	if ( index == 0 || index == LB_ERR )
	{
		but.EnableWindow( FALSE ) ;
	}
	else
	{
		but.EnableWindow( TRUE ) ;
	}

	but = ( GetDlgItem( IDC_DOWN ) ) ;
	if ( index == m_list_box.GetItemCount() -1 || index == LB_ERR )
	{
		but.EnableWindow( FALSE ) ;
	}
	else
	{
		but.EnableWindow( TRUE ) ;
	}

	if( index == LB_ERR )
	{
		m_info_view.set_body_text( wstring() ) ;
		but = ( GetDlgItem( IDC_REMOVE_MEMORY ) ) ;
		but.EnableWindow( FALSE ) ;
		return 0L ;
	}

	but = ( GetDlgItem( IDC_REMOVE_MEMORY ) ) ;
	but.EnableWindow( TRUE ) ;

	display_info_for_item( index ) ;

	return 0L ;
}

void CMemoryManagerDlg::fill_listview()
{
	memory_iterator pos ;
	for ( pos = m_memories.begin() ; pos != m_memories.end() ; ++pos )
	{
		memory_pointer mem = *pos ;
		if( mem->is_new() )
		{
			m_list_box.InsertItem( m_list_box.GetItemCount(), resource_string( IDS_NEW ) ) ;
		}
		else
		{
			m_list_box.InsertItem( m_list_box.GetItemCount(), mem->get_location() ) ;
		}

		m_list_box.SetCheckState( m_list_box.GetItemCount()-1, !! mem->is_active() )	;
	}
}

void CMemoryManagerDlg::reflect_checkstate()
{
	int i = 0 ;
	memory_iterator pos ;
	for ( i = 0, pos = m_memories.begin() ; 
				pos != m_memories.end() ; 
				++pos, ++i )
	{
		memory_pointer mem = *pos ;
		if ( m_list_box.GetCheckState( i ) ) 
		{
			mem->set_active_on() ;
		}
		else
		{
			mem->set_active_off() ;
		}
	}
}

void CMemoryManagerDlg::add_memory_file(const CString &mem_file)
{
	m_list_box.InsertItem( m_list_box.GetItemCount(), mem_file ) ;

	memory_pointer mem(new mem_engine::memory_local(m_props)) ;

	m_info_view.set_body_text(get_loading_message(mem_file)) ;

	reflect_checkstate() ;

	try
	{
		mem->set_listener( static_cast< CProgressListener* >( this ) ) ;

		if ( ! mem->load( mem_file ) )
		{
			m_list_box.DeleteAllItems() ;
			fill_listview() ;
			return ;
		}
	}

	catch ( ... ) 
	{
		mem->set_listener(nullptr);
		m_list_box.DeleteAllItems() ;
		fill_listview() ;
		throw ;
	}

	mem->set_listener(nullptr);
	m_memories.push_back(mem) ;
	m_list_box.SelectItem( m_list_box.GetItemCount() - 1 ) ;
	m_list_box.SetCheckState( m_list_box.GetItemCount() - 1, !! mem->is_active() ) ;

	OnSelChange( ) ;
}

void CMemoryManagerDlg::display_info_for_item(int item)
{
	memory_pointer mem = get_memory_at(item) ;
	
	wstring desc = get_info_for_item(mem);

	background_processor backer ;
	while ( m_info_view.is_navigation_complete() == false || m_info_view.is_document_complete() == false )
	{
		backer.check_iterations() ;
	}
	
	m_info_view.set_body_text(desc) ;
}

CString CMemoryManagerDlg::get_save_prompt( memory_pointer mem )
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
void CMemoryManagerDlg::save_memory(memory_pointer mem)
{
	showSavingMessage(mem);
	reflect_checkstate() ;
	perform_save(mem);
}

void CMemoryManagerDlg::set_button_focus()
{
	if (m_list_box.GetItemCount() > 0)
	{
		CWindow but = ( GetDlgItem( IDC_REMOVE_MEMORY ) ) ;
		but.SetFocus() ;
	}
	else
	{
		CWindow but = ( GetDlgItem( IDOK ) ) ;
		but.SetFocus() ;
	}
}

void CMemoryManagerDlg::set_memories( std::shared_ptr<mem_engine::memory_model> controller )
{
	memory_list &memories = controller->get_memories() ;

	m_memories.assign(memories.begin(), memories.end()) ;
}

void CMemoryManagerDlg::get_memories( std::shared_ptr<mem_engine::memory_model> memories )
{
	memories->swap_memories( m_memories ) ;
}

memory_pointer CMemoryManagerDlg::get_memory_at( const int sel )
{
	return *(get_pos_at(sel)) ;
}

wstring CMemoryManagerDlg::get_saving_feedback( memory_pointer& mem )
{
	wstring text ;

	text = L"<b>" ;
	text += CT2W( system_message(IDS_SAVING_RECORDS, resource_string(IDS_MEMORY), (LPCTSTR)mem->get_location()) );
	text += L"</b>" ;
	return text ;
}

void CMemoryManagerDlg::swap_memories( const int index )
{
	memory_iterator pos = get_pos_at(index) ;
	memory_iterator pos2 = pos ; 
	++pos2 ;
	memory_pointer mem = *pos ;
	*pos = *pos2 ;
	*pos2 = mem ;
}

mem_engine::memory_iterator CMemoryManagerDlg::get_pos_at( const int sel )
{
	if (static_cast<size_t>(sel) >= m_memories.size())
	{
		throw std::out_of_range("Attempted to access memory out of range") ;
	}
	memory_iterator pos = m_memories.begin() ;
	std::advance(pos, static_cast<size_t>(sel)) ;
	return pos ;
}

wstring CMemoryManagerDlg::get_info_for_item( memory_pointer mem )
{
	MemoryInfo *mem_info = mem->get_memory_info() ;

	cpptempl::data_map data ;

	data[L"file_name"] = cpptempl::make_data(get_memory_name(mem)) ;
	data[L"creator"] = cpptempl::make_data(get_creator_name(mem_info)) ;
	data[L"field"] = cpptempl::make_data(get_field_name(mem_info)) ;
	data[L"created_on"] = cpptempl::make_data(get_created_on(mem_info)) ;
	data[L"source_language"] = cpptempl::make_data(get_source_lang(mem_info)) ;
	data[L"target_language"] = cpptempl::make_data(get_target_lang(mem_info)) ;
	data[L"client"] = cpptempl::make_data(get_client_name(mem_info)) ;
	data[L"mem_size"] = cpptempl::make_data(get_mem_size(mem)) ;
	data[L"file_size"] = cpptempl::make_data(get_file_size(mem)) ;
	data[L"reliability"] = cpptempl::make_data(get_reliability_range(mem)) ;
	data[L"validated"] = cpptempl::make_data(mem->get_validated_percent()) ;
	data[L"locked"] = cpptempl::make_data(bool2wstring( mem->is_locked() )) ;

	return cpptempl::parse(cpptempl::get_template_text(_T("item_info.txt")), data) ;
}

wstring CMemoryManagerDlg::get_memory_name( memory_pointer mem )
{
	if ( mem->is_new() ) 
	{
		return wstring(R2W( IDS_NEW )) ;
	}
	return get_fname_from_loc(static_cast<LPCWSTR>(mem->get_location())) ;
}

wstring CMemoryManagerDlg::get_creator_name( MemoryInfo *mem_info )
{
	return get_item(mem_info->get_creator()) ;
}

wstring CMemoryManagerDlg::get_field_name( MemoryInfo *mem_info )
{
	return get_item(mem_info->get_field()) ;
}

wstring CMemoryManagerDlg::get_created_on( MemoryInfo *mem_info )
{
	return get_item(mem_info->get_created_on()) ;
}

wstring CMemoryManagerDlg::get_source_lang( MemoryInfo *mem_info )
{
	return get_item(mem_info->get_source_language()) ;
}

wstring CMemoryManagerDlg::get_target_lang( MemoryInfo *mem_info )
{
	return get_item(mem_info->get_target_language()) ;
}

wstring CMemoryManagerDlg::get_client_name( MemoryInfo *mem_info )
{
	return get_item(mem_info->get_client()) ;
}

wstring CMemoryManagerDlg::get_mem_size( memory_pointer mem )
{
	return ulong2formatted_wstring( mem->size() );
}

wstring CMemoryManagerDlg::get_file_size( memory_pointer mem )
{
	file::CPath path( mem->get_location() ) ;
	if (! path.FileExists() )
	{
		return wstring(L"0") ;
	}
	else
	{
		input_device_ptr input(new InputDeviceFile) ;
		return file_size2wstring(input->get_size( path.Path() )) ;
	}
}

wstring CMemoryManagerDlg::get_reliability_range(memory_pointer mem)
{
	size_t low, high ;
	double ave ;
	boost::tuples::tie(low, high, ave) = mem->get_reliability_stats() ;

	wformat relformat = wformat(L"%d &ndash; %d (Ave: %3.1lf)")
		% low
		% high
		% ave ;

	return relformat.str() ;
}

wstring CMemoryManagerDlg::get_loading_message( const CString &mem_file )
{
	CString message ;
	message.FormatMessage( IDS_MSG_LOADING, (LPCTSTR)mem_file ) ;

	wstring text ;

	text = L"<b>" ;
	text += CT2W( message );
	text += L"</b>" ;

	return text ;
}

CString CMemoryManagerDlg::get_save_destination(mem_engine::memory_pointer mem)
{
	CString filename;
	if (file::CPath(mem->get_fullpath()).FileExists())
	{
		filename = mem->get_fullpath();
	}

	auto history = &app_props::get_props()->m_history_props;

	if (mem->get_is_memory())
	{
		return save_memory_file(
			history->m_glossary_location, // last_save
			filename // filename
			);
	}
	else
	{
		return save_glossary_file(
			history->m_memory_location, // last_save
			filename // filename
			);
	}
}
