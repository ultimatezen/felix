/*!
	@brief Implementation file for edit-mode search dialogs
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "EditModeFindDlg.h"
#include "reg_msg_filter.h"

using namespace html ;

/** CTOR.
 */
emode_search_params::emode_search_params() : 
	m_ignore_case( false ),
	m_whole_words( false ),
	m_search_type( IDS_SEARCH_ALL )
{

}
emode_search_params::emode_search_params(const emode_search_params &rhs)
{
	internal_copy(rhs) ;
}

/** assignment operator.
 */
emode_search_params& emode_search_params::operator= (const emode_search_params &rhs)
{
	internal_copy(rhs) ;
	return *this ;
}
void emode_search_params::internal_copy(const emode_search_params &rhs)
{
	m_ignore_case		= rhs.m_ignore_case ;
	m_whole_words		= rhs.m_whole_words ; 
	m_search_string		= rhs.m_search_string ;
	m_replace_string	= rhs.m_replace_string ;
	m_search_type		= rhs.m_search_type ;
}

/** OnInitDialog
	@brief Init the dialog.
		NOTE: We set up the data in the find dialog, because we may
		switch the UI languages, and destroy/create the
		find dialog to get the new UI. This will destroy the
		info in the edit boxes and such, but not the params.
*/
LRESULT CEditModeFindDlg::OnInitDialog( )
{
	SENSE( "OnInitDialog" ) ;

	// set our message filter...
#ifndef UNIT_TEST
	register_message_filter(this) ;
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	// checkbox setup
	init_checkboxes() ;

	// search edit setup
	init_find_edit() ;

	// combo box setup
	init_combo() ;
#endif

	return TRUE ;
}

/** OnActivate
	@brief What to do when activated. 
   @retval LRESULT 
   @param WPARAM wParam. TRUE if being activated, false otherwise.
*/
LRESULT CEditModeFindDlg::OnActivate( WPARAM wParam ) 
{
	SENSE( "OnActivate" ) ;

	if ( LOWORD( wParam ) ) // set to TRUE if being activated
	{
		// update checkboxes
		init_checkboxes() ;

		// select right combo item
		select_combo_item( m_params.m_search_type  ) ;
	
		// update text in find edit
		m_find_edit.SetText( m_params.m_search_string ) ;
		m_find_edit.SetSel(0, -1) ;

		return 0 ;
	}

	// fill in the info
	fill_search_string() ;
	fill_checkboxes() ;
	fill_search_type() ;

	return 0 ;
}



/** OnInitDialog
   @brief Init the dialog.
		NOTE: We set up the data in the find dialog, because we may
		switch the UI languages, and destroy/create the
		find dialog to get the new UI. This will destroy the
		info in the edit boxes and such, but not the params.
   @retval LRESULT 
*/
LRESULT CEditModeReplaceDlg::OnInitDialog( )
{
	SENSE( "OnInitDialog" ) ;

#ifndef UNIT_TEST
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	// set our message filter...
	register_message_filter(this) ;

	// checkbox setup
	init_checkboxes() ;

	// search edit setup
	init_find_edit() ;

	// combo box setup
	init_combo() ;

	// ===============
	// replace string setup
	// ===============

	m_replace_edit.Attach( GetDlgItem( IDC_REPLACE_BOX ) ) ;

	// set the text of the edit boxe
	m_replace_edit.SetText( m_params.m_replace_string ) ;

	// make it second in the z, and hence tab, order
	ATLASSERT( m_find_edit.IsWindow() ) ;
	m_replace_edit.SetWindowPos( m_find_edit, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;
#endif

	return TRUE ;
}

/** OnReplace
	@brief 
	@retval LRESULT 
*/
LRESULT CEditModeReplaceDlg::OnReplace( )
{
	SENSE( "OnReplace" ) ;
	fill_values_from_dialog();

	::SendMessage( GetParent(), UWM_USER_MESSAGE, ID_EDIT_REPLACE, 0L ) ;

	wstring range_text = get_range_text() ;

	wstring search_text = m_find_edit.GetText() ;
	ATLASSERT ( m_params.m_search_string == search_text ) ; 

	check_make_lower(range_text, search_text);	
	
	if ( range_text == search_text )
	{
		CHtmlTextRange range = get_range() ;
		do_replace(range);	
	}
	else
	{
		::MessageBeep( MB_ICONSTOP ) ;
		return 0L ;
	}

	try_search_again() ;

	return 0L ;
}


/** OnReplaceAll
   @brief 
   @retval LRESULT 
	*/
LRESULT CEditModeReplaceDlg::OnReplaceAll( )
{
	SENSE( "OnReplaceAll" ) ;

	::SendMessage( GetParent(), UWM_USER_MESSAGE, ID_EDIT_REPLACE, 0L ) ;

	collapse_range_to_beginning() ;

	fill_values_from_dialog() ;

	int count( 0 ) ;
	while( find_next_match() )
	{
		count++ ;

		CHtmlTextRange range = get_range() ;
		do_replace( range ) ;
	}

	if ( count > 0 )
	{
		CHtmlTextRange range = get_range() ;
		range.scroll_into_view() ;
	}

	MessageBox( system_message(IDS_MADE_REPLACEMENTS, int_arg(count)), 
				resource_string( IDS_MADE_REPLACEMENTS_TITLE ) ) ;

	return 0L ;
}

/** OnActivate
	@brief What to do when activated. 
	@retval LRESULT 
	@param WPARAM wParam. TRUE if being activated, false otherwise.
*/
LRESULT CEditModeReplaceDlg::OnActivate( WPARAM wParam ) 
{
	SENSE( "OnActivate" ) ;
	if ( LOWORD( wParam ) ) // set to TRUE if being activated
	{
		handle_activation();
		return 0 ;
	}

	// fill in the info
	fill_values_from_dialog() ;

	return 0 ;
}

/** fill_replace_string
   @brief 
   @retval void 
*/
void CEditModeReplaceDlg::fill_replace_string()
{
	BANNER( "fill_replace_string" ) ;
	if ( ! m_replace_edit.IsWindow() )
		return ;
	m_params.m_replace_string = m_replace_edit.GetText() ;
}


void CEditModeReplaceDlg::do_replace(CHtmlTextRange& range)
{
	range.put_plain_text( m_params.m_replace_string ) ;
	range.collapse( false ) ; // false = collapse to end
	range.select() ;

}
void CEditModeReplaceDlg::try_search_again()
{
	if ( ! find_next_match() ) 
	{
		::MessageBeep( MB_ICONEXCLAMATION ) ;
	}
	else
	{
		CHtmlSelection cur_selection = m_doc.get_selection() ;
		CHtmlTextRange cur_range = cur_selection.create_text_range() ;
		cur_range.scroll_into_view() ;
	}

}
void CEditModeReplaceDlg::fill_values_from_dialog()
{
	fill_search_string() ;
	fill_replace_string() ;
	fill_checkboxes() ;
	fill_search_type() ;

}

void CEditModeReplaceDlg::handle_activation()
{
	init_checkboxes() ;

	select_combo_item( m_params.m_search_type  ) ;

	m_replace_edit.SetText( m_params.m_replace_string ) ;
	m_find_edit.SetText( m_params.m_search_string ) ;
	m_find_edit.SetSel(0, -1) ;

}
void CEditModeReplaceDlg::collapse_range_to_beginning()
{
	CHtmlTextRange range = get_range() ;

	range.expand( L"Textedit" ) ; // select entire textedit
	range.collapse( true ) ; // collapse to start
	range.select() ; // this tells the find function to start here...
}

LRESULT CEditModeReplaceDlg::OnDestroy()
{
	m_replace_edit.Detach() ;
	return ParentClass::on_destroy() ;
}

