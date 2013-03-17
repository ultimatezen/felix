/*!
	@file EditModeFindDlg.h
	@brief Implementation file for edit-mode dialogs
	@date 2005/06/25
	Time: 9:12:56
	@author Ryan Ginstrom
 */

#pragma once

#include "resource.h"
#include "HtmlDocument.h"		// CHtmlDocument
#include "MessageMapDefines.h"	// more crackers
#include "Exceptions.h"			// CException
#include "Accelerator.h"		// CAccelerator

#include "WindowExceptionHandler.h"
#include "system_message.h"
#include "stringconversions.h"


/**
	@struct emode_search_params
	@brief Search parameters for edit-mode search/replace dialogs.
 */
struct emode_search_params
{
	bool		m_ignore_case ;
	bool		m_whole_words ;

	wstring		m_search_string ;
	wstring		m_replace_string ;

	int			m_search_type ;

	emode_search_params() ;
	emode_search_params(const emode_search_params &rhs) ;

	emode_search_params& operator= (const emode_search_params &rhs) ;
	void internal_copy(const emode_search_params &rhs) ;
} ;

// ======================
//
//		THE DIALOGS
//
// ======================

/**
	@class CEditModeCommonFunctions 
	@brief Common functionality for edit-mode searching/replacing.
 */
template< class T >
class CEditModeCommonFunctions : 
		public CMessageFilter, 
		public CDialogImpl<T, CWindow>, 
		public CWindowExceptionHandler< T >
{
protected:
	
	emode_search_params		m_params ;
	html::CHtmlDocument		m_doc ;

	CCommandEdit			m_find_edit ;
	CComboBox				m_search_type_combo ;

public:
	DECLARE_SENSING_VAR ;

	// Function name	: ~CEditModeCommonFunctions
	// Description	    : 
	// Return type		: 
	virtual ~CEditModeCommonFunctions() 
	{
	}

	LRESULT on_destroy()
	{
		SetMsgHandled( FALSE ) ;
		_Module.GetMessageLoop()->RemoveMessageFilter(this);

		m_find_edit.Detach() ;

		m_doc.uninstantiate() ;

		return 0L ;
	}

	BOOL PreTranslateMessage( MSG *pMsg ) 
	{
		ENSURE_FOCUS

		return IsDialogMessage( pMsg ) ;
	}

	// Selects the combo item matching the resource string
	void select_combo_item(const int resource_id)
	{
		BANNER( "CEditModeCommonFunctions::select_combo_item" ) ;

		const CString item_str = resource_string(resource_id) ;
		const int count = m_search_type_combo.GetCount() ;

		for ( int i = 0 ; i < count ; ++i )
		{
			if ( item_str == get_combo_string( i ) )
			{
				m_search_type_combo.SetCurSel( i ) ;
				return ;
			}
		}
	}

	// Function name	: get_combo_string
	CString get_combo_string( const int index ) const
	{
		BANNER( "get_combo_string" ) ;

		CString combo_string ;
		m_search_type_combo.GetLBText( index, combo_string ) ;
		return combo_string ;
	}

	// Function name	: init_combo
	// Description	    : 
	// Return type		: void 
	void init_combo()
	{
		m_search_type_combo.Attach( GetDlgItem( IDC_COMBO ) ) ;

#ifndef UNIT_TEST
		m_search_type_combo.AddString( resource_string( IDS_SEARCH_ALL ) ) ;
		m_search_type_combo.AddString( resource_string( IDS_SEARCH_SOURCE ) ) ;
		m_search_type_combo.AddString( resource_string( IDS_SEARCH_TRANS ) ) ;
		m_search_type_combo.AddString( resource_string( IDS_SEARCH_CONTEXT ) ) ;

		select_combo_item( m_params.m_search_type ) ;
#endif
	}


	// Function name	: init_find_edit
	// Description	    : 
	// Return type		: void 
	void init_find_edit() 
	{
		m_find_edit.Attach( GetDlgItem( IDC_FIND_BOX ) ) ;
		ATLASSERT ( m_find_edit.IsWindow() ) ; 
		// ===========================
		// set the text of the edit box
		// ===========================
		m_find_edit.SetText( m_params.m_search_string ) ;
		m_find_edit.SetSel( 0, -1 ) ;

		// make it first in the z, and hence tab, order
		m_find_edit.SetWindowPos( HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;
	}


	// Function name	: init_checkboxes
	// Description	    : 
	// Return type		: void 
	void init_checkboxes()
	{
		// match case checkbox
		CheckDlgButton( IDC_IGNORE_CASE_CHECK, ( m_params.m_ignore_case ? BST_CHECKED : BST_UNCHECKED ) ) ;
		// whole words
		CheckDlgButton( IDC_WHOLE_WORDS_CHECK, ( m_params.m_whole_words ? BST_CHECKED : BST_UNCHECKED ) ) ;
	}


	// Function name	: set_document
	// Description	    : 
	// Return type		: void 
	// Argument         : const CHtmlDocument &doc
	void set_document( const html::CHtmlDocument &doc ) 
	{
		BANNER( "set_document" ) ;

		m_doc = doc ;
	}

	// Function name	: get_search_params
	// Description	    : 
	// Return type		: emode_search_params& 
	emode_search_params &get_search_params()
	{ 
		BANNER( "&get_search_params" ) ;

		return m_params ; 
	}

	// Function name	: set_search_params
	// Description	    : 
	// Return type		: void 
	// Argument         : const emode_search_params &copy
	void set_search_params( const emode_search_params &copy )
	{
		m_params = copy ;
	}


	// Function name	: on_close_command
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnCloseCommand( ) 
	{
		SENSE( "OnClose" ) ;

		ShowWindow( SW_HIDE ) ;
		return 0 ;
	}

	// Function name	: OnSearch
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnSearch( )
	{
		SENSE( "OnSearch" ) ;

		::SendMessage( GetParent(), UWM_USER_MESSAGE, ID_EDIT_FIND, T::IDD ) ;

		// various search flags
		fill_checkboxes() ;
		fill_search_string() ;
		fill_search_type() ;

#ifdef UNIT_TEST
		return 0L ;
#endif
		if ( ! find_next_match() ) 
		{
			::MessageBeep( MB_ICONEXCLAMATION ) ;
		}
		else
		{
			html::CHtmlSelection selection	= m_doc.get_selection() ;
			html::CHtmlTextRange range		= selection.create_text_range() ;
			range.scroll_into_view() ;
		}
	
		return 0 ;
	}

	void check_make_lower(std::wstring& range_text, std::wstring& search_text)
	{
		if ( m_params.m_ignore_case )
		{
			boost::to_lower( range_text ) ;
			boost::to_lower( search_text ) ;
		}

	}

	html::text_range_ptr get_range()
	{
		html::CHtmlSelection selection = m_doc.get_selection() ;
		return selection.create_text_range() ;
	}

	wstring get_range_text()
	{
		html::CHtmlTextRange range = get_range() ;
		return BSTR2wstring( range.get_plain_text() ) ;
	}

	void load_id_to_find( _bstr_t &id_to_find )
	{
		CStringW id ;
		switch( m_params.m_search_type )
		{
		case IDS_SEARCH_SOURCE:
			if(! id.LoadString( IDS_SOURCE_ID ))
			{
				ATLASSERT("Failed to load string ID IDS_SOURCE_ID" && FALSE) ;
				id = L"source" ;
			}
			ATLASSERT( id == ( L"source" ) ) ;
			break ;
		case IDS_SEARCH_TRANS:
			if(! id.LoadString( IDS_TRANS_ID ))
			{
				ATLASSERT("Failed to load string ID IDS_TRANS_ID" && FALSE) ;
				id = L"trans" ;
			}
			ATLASSERT( id == (L"trans") ) ;
			break ;
		case IDS_SEARCH_CONTEXT:
			if(! id.LoadString( IDS_CONTEXT_ID ))
			{
				ATLASSERT("Failed to load string ID IDS_CONTEXT_ID" && FALSE) ;
				id = L"context" ;
			}
			ATLASSERT( id == ( L"context" ) ) ;
			break ;
		default:
			ATLASSERT( "Unknown search type!" && FALSE ) ;
		}
		id_to_find = (LPCWSTR)id ;
	}
	// Function name	: find_next_match
	// Description	    : 
	// Return type		: void 
	bool find_next_match()
	{
		BANNER( "find_next_match" ) ;

		// first, we get the current selection
		wstring range_text = get_range_text() ;
		// ...and the text we are searching for
		wstring search_text = m_find_edit.GetText() ;

		// if we are ignoring case, make them lower
		check_make_lower( range_text, search_text ) ;

		// if we have already selected this one, collapse 
		// end and look for the next one
		if ( range_text == search_text )
		{
			collapse_range();		
		}
		// Otherwise, collapse start
		else 
		{
			html::CHtmlTextRange range = get_range() ;
			range.collapse( true ) ; // true = collapse to start
			range.select() ;
		}

		// we set up the flags according to our search params
		const long flags = set_up_flags() ;
		// then we actually look for the text
		html::CHtmlTextRange range = get_range() ;
		const bool found = range.find_text( search_text, flags ) ;

		// if we found it...
		if ( found )
		{
			if ( ! is_in_desired_range( range ) )
			{
				range.collapse(false) ;
				range.select() ;
				return find_next_match() ;	// ... and look for another match
			}

			// select the range
			range.select() ;

		}		

		// ... and return the results
		return found ;
	}

	// Function name	: is_in_desired_range
	// Description	    : 
	// Return type		: bool 
	// Argument         :  CHtmlTextRange &range
	// Argument         : const wstring id_to_find
	bool is_in_desired_range( html::CHtmlTextRange &range )
	{
		if ( m_params.m_search_type == IDS_SEARCH_ALL ) 
		{
			return true ;
		}

		_bstr_t id_to_find ;
		load_id_to_find( id_to_find ) ;

		// get all the elements
		html::CHtmlElementCollection all_elements = m_doc.get_all() ;
		html::CHtmlElement element ;

		const int count = all_elements.get_length() ;

		for ( int i = 0 ; i<count ; ++i )
		{
			element = all_elements.item( i ) ;

			if ( element.get_id( ) == id_to_find )
			{
				html::CHtmlTextRange element_range = get_range() ;
#pragma warning (disable:4239)
				element_range.move_to_element_text( element.get_element() ) ;
#pragma warning (default:4239)
				if ( element_range.is_in_range( range ) )
					return true ;
			}
		}
		return false ;
	}


	// Function name	: fill_search_string
	// Description	    : 
	// Return type		: void
	void fill_search_string()	
	{
		// source
		if ( ! m_find_edit.IsWindow() )
		{
			return ;
		}

		m_params.m_search_string = m_find_edit.GetText() ;
	}
	// Function name	: fill_checkboxes
	// Description	    : 
	// Return type		: void 
	void fill_checkboxes() 
	{
		ATLASSERT( IsWindow() ) ;
		if ( ! IsWindow(  ) )
		{
			return ;
		}

#ifndef UNIT_TEST
		// the checkboxes should be windows as well...
		ATLASSERT( ::IsWindow( GetDlgItem( IDC_IGNORE_CASE_CHECK ) ) ) ;
		ATLASSERT( ::IsWindow( GetDlgItem( IDC_WHOLE_WORDS_CHECK ) ) ) ;
#endif
		m_params.m_ignore_case		= !! IsDlgButtonChecked( IDC_IGNORE_CASE_CHECK ) ;
		m_params.m_whole_words		= !! IsDlgButtonChecked( IDC_WHOLE_WORDS_CHECK ) ;
	}

	// Function name	: fill_search_type
	// Description	    : 
	// Return type		: void 
	void fill_search_type()
	{
		// search type
		if ( ! m_search_type_combo.IsWindow() )
		{
			return ;
		}
		
		const tstring selected_item = get_combo_string( m_search_type_combo.GetCurSel() ) ;

		if ( selected_item ==  R2T( IDS_SEARCH_ALL ) ) 
		{
			m_params.m_search_type = IDS_SEARCH_ALL ;
		}
		else if ( selected_item ==  R2T( IDS_SEARCH_SOURCE ) ) 
		{
			m_params.m_search_type = IDS_SEARCH_SOURCE ;
		}
		else if ( selected_item ==  R2T( IDS_SEARCH_TRANS ) ) 
		{
			m_params.m_search_type = IDS_SEARCH_TRANS ;
		}
		else if ( selected_item ==  R2T( IDS_SEARCH_CONTEXT ) ) 
		{
			m_params.m_search_type = IDS_SEARCH_CONTEXT ;
		}
		else
		{
#ifndef UNIT_TEST
			ATLASSERT( "Unknown search type!" && FALSE ) ;
#endif
			m_params.m_search_type = IDS_SEARCH_ALL ;
		}
	}


	// Function name	: OnCopy
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnCopy( ) const
	{
		BANNER( "CEditModeFindDlg::OnCopy" ) ;
		::SendMessage(::GetFocus(), WM_COPY, 0, 0L);
		return 0 ;
	}

	// Function name	: OnPaste
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnPaste( ) const
	{
		BANNER( "CEditModeFindDlg::OnPaste" ) ;
		::SendMessage(::GetFocus(), WM_PASTE, 0, 0L);
		return 0 ;
	}

	// Function name	: OnCut
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnCut( ) const
	{
		BANNER( "CEditModeFindDlg::OnCut" ) ;

		::SendMessage(::GetFocus(), WM_CUT, 0, 0L);
		return 0 ;
	}

	// Function name	: OnUndo
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnUndo( ) const
	{
		BANNER( "CEditModeFindDlg::OnUndo" ) ;

		::SendMessage(::GetFocus(), WM_UNDO, 0, 0L);
		return 0 ;
	}

	// Function name	: OnRedo
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnRedo( ) const
	{
		BANNER( "CEditModeFindDlg::OnRedo" ) ;

		::SendMessage(::GetFocus(), EM_REDO, 0, 0L);
		return 0 ;
	}


	// ========================
	// MESSAGE MAP
	// ========================
	BEGIN_MAP_EX( CEditModeCommonFunctions )
		MSG_HANDLER_0 ( WM_DESTROY,		on_destroy )

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_0(IDCANCEL, OnCloseCommand)
			CMD_HANDLER_EX_0(IDCLOSE, OnCloseCommand)
			CMD_HANDLER_EX_0(IDC_SEARCH, OnSearch )
			CMD_HANDLER_EX_0(ID_EDIT_COPY, OnCopy)
			CMD_HANDLER_EX_0(ID_EDIT_PASTE, OnPaste)
			CMD_HANDLER_EX_0(ID_EDIT_CUT, OnCut)
			CMD_HANDLER_EX_0(ID_EDIT_UNDO, OnUndo)
			CMD_HANDLER_EX_0(ID_EDIT_REDO, OnRedo)
		END_CMD_HANDLER_EX

	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )
	
private:

	void collapse_range()
	{
		html::CHtmlTextRange range = get_range() ;
		range.collapse( false ) ; // false = collapse to end
		range.select() ;

	}

	void select_range()
	{
		html::CHtmlTextRange range = get_range() ;
		range.select() ;	// select it
	}

	void check_collapse_direction( html::CHtmlTextRange& range, ATL::CString& range_text, ATL::CString& search_text) const
	{
		// false = collapse to end
		// true = collapse to start
		range.collapse( should_collapse_at_start( range_text, search_text ) ) ; 

		range.select() ;

	}
	bool should_collapse_at_start( const CString &lhs, const CString &rhs ) const
	{
		// if we have already selected this one, collapse 
		// end and look for the next one
		if ( lhs == rhs ) 
		{
			return false ;
		}
		return true ;
	}
	long set_up_flags()
	{
		long flags(0) ;

		if ( ! m_params.m_ignore_case )
			flags |= 4 ; // match case
		if ( m_params.m_whole_words )
			flags |= 2 ; // match whole words only

		return flags;
	}

} ;

/**
	@class CEditModeFindDlg 
	@brief Find dialog for edit mode.
 */
class CEditModeFindDlg  : 
	public CEditModeCommonFunctions< CEditModeFindDlg >
{

public:
	static const int IDD = IDD_EDIT_FIND_DIALOG ;

	LRESULT OnInitDialog( ) ;
	LRESULT OnActivate( WPARAM wParam ) ;

	BEGIN_MAP_EX( CEditModeFindDlg )
		MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )
 		MSG_HANDLER_1 ( WM_ACTIVATE,   OnActivate,    wParam )

		CHAIN_MSG_MAP( CEditModeCommonFunctions< CEditModeFindDlg > )

	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )

};

/**
	@class CEditModeReplaceDlg  
	@brief Replace dialog for edit mode.
 */
class CEditModeReplaceDlg  : 
	public CEditModeCommonFunctions< CEditModeReplaceDlg >
{
	typedef CEditModeCommonFunctions< CEditModeReplaceDlg > ParentClass ;
	CCommandEdit			m_replace_edit ;

public:
	
	static const int IDD = IDD_EDIT_MODE_REPLACE_DIALOG  ; // IDD_EDIT_MODE_REPLACE_DLG } ;

	LRESULT OnInitDialog( ) ;
	LRESULT OnReplace( ) ;
	LRESULT OnReplaceAll( ) ;
	LRESULT OnActivate( WPARAM wParam )  ;
	void fill_replace_string() ;
	LRESULT OnDestroy();

	BEGIN_MAP_EX( CEditModeReplaceDlg )
		MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )
 		MSG_HANDLER_1 ( WM_ACTIVATE,   OnActivate,    wParam )
		MSG_WM_DESTROY( OnDestroy )

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_0(IDC_REPLACE, OnReplace)
			CMD_HANDLER_EX_0(IDC_REPLACE_ALL, OnReplaceAll)
		END_CMD_HANDLER_EX

		CHAIN_MSG_MAP( ParentClass )

	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )

private:

	void do_replace(html::CHtmlTextRange& range);
	void try_search_again();
	void fill_values_from_dialog();
	void handle_activation();
	void collapse_range_to_beginning();
};
