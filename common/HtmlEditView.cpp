// HtmlEditView.cpp: implementation of the CHtmlView class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HtmlEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif


// -----------------------------------

namespace html
{

//! Pass on the creation command to the base class
HWND CHtmlEditView::Create(HWND hWndParent, RECT& rcPos, DWORD dwFlags )
{
	return CWindowImpl<CHtmlEditView, CAxWindowT<TWindow> >::Create( hWndParent, rcPos, _T("DHTMLEdit.DHTMLEdit"), dwFlags, 0, m_unique_id ) ;
}

// In order for AtlAdviseSinkMap() to work, we need this overload!?

// Function name	: CHtmlEditView::GetDlgItem
// Description	    : 
// Return type		: HWND 
// Argument         : int /*nID*/
HWND CHtmlEditView::GetDlgItem(int nID ) const 
{ 
	ATLASSERT( nID == m_unique_id || nID == ID_HTML_EDIT_EVENT_ID ) ;

	if ( nID == m_unique_id || nID == ID_HTML_EDIT_EVENT_ID )
		return m_hWnd; 
	return NULL ;
}

// --------------------------------------


BOOL CHtmlEditView::PreTranslateMessage(MSG* pMsg)
{
	ENSURE_FOCUS

	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		(pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
		return FALSE;

	// Give OCX a chance to translate this message
	return (BOOL)::SendMessage(m_hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);
}

// --------------------------------------


// Function name	: CHtmlEditView::on_create
// Description	    : 
// Return type		: LRESULT 
// Argument         : UINT uMsg
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CHtmlEditView::on_create(UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// Create the control for me
	ATLASSERT(this-> IsWindow() ) ;
	this->DefWindowProc(uMsg, wParam, lParam);

	init_control() ;

	return 0;
}

// Function name	: CHtmlEditView::get_edit
// Description	    : 
// Return type		: edit_ptr 
CHtmlEditView::edit_ptr CHtmlEditView::get_edit()
{	
	edit_ptr edit ;
#ifndef UNIT_TEST
	COM_ENFORCE( QueryControl(DHTMLEDLib::IID_IDHTMLEdit, out_ptr< DHTMLEDLib::IDHTMLEdit >(&edit) ), _T("Failed to get DHTML Editing Control")  ) ;
#endif
	return edit ;
}

// Function name	: CHtmlEditView::init_control
// Description	    : 
// Return type		: void 
void CHtmlEditView::init_control()
{
	// set up the edit control with our default values
	edit_ptr edit = get_edit() ;
#pragma warning( disable:4310 ) // cast truncates constant value
	edit->put_AbsoluteDropMode(VARIANT_TRUE);
	edit->put_SourceCodePreservation(VARIANT_TRUE);
	edit->put_UseDivOnCarriageReturn(VARIANT_FALSE);
#pragma warning( default:4310 ) // cast truncates constant value

  // Establish connection points
  COM_ENFORCE( AtlAdviseSinkMap(this, true), _T("Failed to set connection points for HTML Edit view") ) ; 
}

// ======================
// formatting stuff
// ======================


// Function name	: CHtmlEditView::show_font_dialog
// Description	    : 
// Return type		: void 
void CHtmlEditView::show_font_dialog()
{
	if ( QueryStatus( DHTMLEDLib::DECMD_FONT ) == DHTMLEDLib::DECMDF_DISABLED ) throw except::CException( _T("The selected operation is not available") ) ;	

	ExecCommand( DHTMLEDLib::DECMD_FONT, OLECMDEXECOPT_DODEFAULT ) ;
	m_dirty = true ;
}

// Function name	: CHtmlEditView::set_font_name
// Description	    : 
// Return type		: void 
// Argument         : wstring &font_name
void CHtmlEditView::set_font_name( const CString &font_name ) 
{
	set_font_name( _variant_t ( (LPCTSTR)font_name ) ) ;
}
void CHtmlEditView::set_font_name( const wstring &font_name ) 
{
	set_font_name( _variant_t ( font_name.c_str() ) ) ;
}
void CHtmlEditView::set_font_name( const _bstr_t &font_name ) 
{
	set_font_name( _variant_t ( font_name ) ) ;
}

void CHtmlEditView::set_font_name( const _variant_t &font_name )
{
	if ( QueryStatus( DHTMLEDLib::DECMD_SETFONTNAME ) == DHTMLEDLib::DECMDF_DISABLED ) throw except::CException( _T("The selected operation is not available")  ) ;	

	ExecCommand( DHTMLEDLib::DECMD_SETFONTNAME, OLECMDEXECOPT_DODEFAULT, font_name ) ;
	m_dirty = true ;
}

// Function name	: CHtmlEditView::set_font_size
// Description	    : 
// Return type		: void 
// Argument         : int size
void CHtmlEditView::set_font_size( int size )
{
	// size for html edit must be between 1 and 7
	if ( size < 1 || size > 7 ) throw except::CException( _T("The selected font size is out of bounds.\rSelect a size between 1 and 7.")  ) ;

	if ( QueryStatus( DHTMLEDLib::DECMD_SETFONTSIZE ) == DHTMLEDLib::DECMDF_DISABLED ) 
	{
		MessageBeep( MB_ICONHAND ) ;
		return ;
	}

	ExecCommand( DHTMLEDLib::DECMD_SETFONTSIZE, OLECMDEXECOPT_DODEFAULT, _variant_t ( size ) ) ;
	m_dirty = true ;
}

// Function name	: CHtmlEditView::set_back_color
// Description	    : 
// Return type		: void 
// Argument         : wstring color
void CHtmlEditView::set_back_color( const wstring &color )
{
	if ( QueryStatus( DHTMLEDLib::DECMD_SETBACKCOLOR ) == DHTMLEDLib::DECMDF_DISABLED ) 
	{
		MessageBeep( MB_ICONHAND ) ;
		return ;
	}

	ExecCommand( DHTMLEDLib::DECMD_SETBACKCOLOR, OLECMDEXECOPT_DODEFAULT, _variant_t ( color.c_str() ) ) ;
	m_dirty = true ;
}

// Function name	: CHtmlEditView::set_fore_color
// Description	    : 
// Return type		: void 
// Argument         : wstring color
void CHtmlEditView::set_fore_color( const wstring &color )
{
	if ( QueryStatus( DHTMLEDLib::DECMD_SETFORECOLOR ) == DHTMLEDLib::DECMDF_DISABLED ) 
	{
		MessageBeep( MB_ICONHAND ) ;
		return ;
	}
	
	ExecCommand( DHTMLEDLib::DECMD_SETFORECOLOR, OLECMDEXECOPT_DODEFAULT, _variant_t( color.c_str() ) ) ;
	m_dirty = true ;
}

// Function name	: CHtmlEditView::set_bold
// Description	    : 
// Return type		: void 
// Argument         : bool setting
void CHtmlEditView::set_bold( bool setting )
{
	DHTMLEDLib::DHTMLEDITCMDF retval = QueryStatus( DHTMLEDLib::DECMD_BOLD ) ;
	if ( retval == DHTMLEDLib::DECMDF_DISABLED )
	{
		MessageBeep( MB_ICONHAND ) ;
		return ;
	}

	// this command toggles the selection, so only do it
	// if our current setting is the oposite of what we want.
	if ( ( retval == DHTMLEDLib::DECMDF_ENABLED && setting == true ) ||
		( retval == DHTMLEDLib::DECMDF_LATCHED && setting == false ) )
	{
		ExecCommand( DHTMLEDLib::DECMD_BOLD, OLECMDEXECOPT_DODEFAULT ) ;
		m_dirty = true ;
	}
}

// Function name	: CHtmlEditView::set_italic
// Description	    : 
// Return type		: void 
// Argument         : bool setting
void CHtmlEditView::set_italic( bool setting )
{
	DHTMLEDLib::DHTMLEDITCMDF retval = QueryStatus( DHTMLEDLib::DECMD_ITALIC ) ;
	if ( retval == DHTMLEDLib::DECMDF_DISABLED )
	{
		MessageBeep( MB_ICONHAND ) ;
		return ;
	}

	// this command toggles the selection, so only do it
	// if our current setting is the oposite of what we want.
	if ( ( retval == DHTMLEDLib::DECMDF_ENABLED && setting == true ) ||
		( retval == DHTMLEDLib::DECMDF_LATCHED && setting == false ) )
	{
		ExecCommand( DHTMLEDLib::DECMD_ITALIC, OLECMDEXECOPT_DODEFAULT ) ;
		m_dirty = true ;
	}
}

// Function name	: CHtmlEditView::set_underline
// Description	    : 
// Return type		: void 
// Argument         : bool setting
void CHtmlEditView::set_underline( bool setting )
{ 
	DHTMLEDLib::DHTMLEDITCMDF retval = QueryStatus( DHTMLEDLib::DECMD_UNDERLINE ) ;
	if ( retval == DHTMLEDLib::DECMDF_DISABLED )
	{
		MessageBeep( MB_ICONHAND ) ;
		return ;
	}

	// this command toggles the selection, so only do it
	// if our current setting is the oposite of what we want.
	if ( ( retval == DHTMLEDLib::DECMDF_ENABLED && setting == true ) ||
		( retval == DHTMLEDLib::DECMDF_LATCHED && setting == false ) )
	{
		ExecCommand( DHTMLEDLib::DECMD_UNDERLINE, OLECMDEXECOPT_DODEFAULT ) ;
		m_dirty = true ;
	}
}


// ----------------------------
// access control content
// ----------------------------


// Function name	: CHtmlEditView::get_document
// Description	    : 
// Return type		: document_ptr 
html::document_ptr CHtmlEditView::get_document()
{
	document_ptr doc ;

	COM_ENFORCE( get_edit()->get_DOM( (MSHTML::IHTMLDocument2**)&doc ), _T("Failed to retrieve document object") ) ;	

	return doc ;
}

// Function name	: CHtmlEditView::get_parent_window
// Description	    : 
// Return type		: window_ptr 
html::window_ptr CHtmlEditView::get_parent_window( )
{
	window_ptr parent_window ;
	COM_ENFORCE( get_document()->get_parentWindow( &parent_window ), _T("Failed to retrieve parent window of DHTML document") ) ;

	return parent_window ;
}

// Function name	: CHtmlEditView::print
// Description	    : 
// Return type		: void 
void CHtmlEditView::print()
{
	_variant_t v ;
	COM_ENFORCE( get_edit()->PrintDocument( &v ), _T("Failed to print DHTML document") ) ;
}


// Function name	: CHtmlEditView::get_body
// Description	    : 
// Return type		: element_ptr 
html::element_ptr CHtmlEditView::get_body()
{
	CHtmlDocument doc = get_document() ;
	return doc.get_body() ;
}

// Function name	: CHtmlEditView::get_text
// Description	    : 
// Return type		: wstring 
_bstr_t CHtmlEditView::get_text()
{
	_bstr_t text ;
	get_text( text.GetAddress() ) ;
	return text ;
}

// Function name	: CHtmlEditView::get_title
// Description	    : 
// Return type		: wstring 
_bstr_t CHtmlEditView::get_title()
{
	CHtmlDocument doc( get_document() ) ;
	return doc.get_title() ;
}

// Function name	: CHtmlEditView::set_title
// Description	    : 
// Return type		: void 
// Argument         : const wstring &title
void CHtmlEditView::set_title( const wstring &title )
{
	CHtmlDocument doc( get_document() ) ;
	doc.set_title( title ) ;
}

// Function name	: CHtmlEditView::filter_text
// Description	    : 
// Return type		: wstring 
// Argument         : const wstring &text
_bstr_t CHtmlEditView::filter_text( const wstring &text )
{
	return filter_text( string2BSTR( text ) ) ;
}

// Function name	: CHtmlEditView::filter_text
// Description	    : 
// Return type		: wstring 
// Argument         : const _bstr_t &text
_bstr_t CHtmlEditView::filter_text( const _bstr_t &text )
{
	// FilterSourceCode will fail if body is empty
	if ( (LPCWSTR) text == NULL ) return _bstr_t() ;

	_bstr_t otext ;
	COM_ENFORCE( get_edit()->raw_FilterSourceCode( text, otext.GetAddress() ), _T("Failed to filter HTML text") ) ;
	return otext ;
}

// Function name	: CHtmlEditView::get_body_text
// Description	    : 
// Return type		: wstring 
_bstr_t CHtmlEditView::get_body_text()
{
	_bstr_t body_text ;
	COM_ENFORCE( get_body()->get_innerHTML( body_text.GetAddress() ), _T("Failed to retrieve document body text") ) ;

	return filter_text( body_text ) ;
}

// Function name	: CHtmlEditView::set_text
// Description	    : 
// Return type		: void 
// Argument         : const wstring &text
void CHtmlEditView::set_text( const wstring &text )
{
	set_text( _bstr_t( text.c_str() ) );
}

// Function name	: CHtmlEditView::set_text
// Description	    : 
// Return type		: void 
// Argument         : const _bstr_t &bstr
void CHtmlEditView::set_text( const _bstr_t &text )
{
	COM_ENFORCE( get_edit()->put_DocumentHTML(text), _T("Failed to set document text") ) ;
}


// Function name	: CHtmlEditView::load_url
// Description	    : 
// Return type		: void 
// Argument         : const wstring &url
void CHtmlEditView::load_url( const wstring &url )
{
	_bstr_t bstr( url.c_str() ) ;
	COM_ENFORCE( get_edit()->LoadURL( bstr ), CString("Failed to load url ") + string2tstring( url ).c_str() ) ;
}

// Function name	: CHtmlEditView::load_document
// Description	    : 
// Return type		: void 
// Argument         : const wstring file_name
void CHtmlEditView::load_document( const wstring &file_name )
{
	_variant_t bstr( file_name.c_str() ) ;
	bstr.vt = VT_BSTR ;
	COM_ENFORCE( get_edit()->LoadDocument( &bstr ), CString("Failed to open document url ") + string2tstring( file_name ).c_str() ) ;
}

void CHtmlEditView::load_document( const _bstr_t &file_name )
{
	_variant_t bstr( file_name ) ;
	bstr.vt = VT_BSTR ;
	COM_ENFORCE( get_edit()->LoadDocument( &bstr ), CString("Failed to open document url ") + CString( (LPCTSTR)file_name ) ) ;
}

// Function name	: CHtmlEditView::save_document
// Description	    : 
// Return type		: void 
// Argument         : const wstring &file_name
void CHtmlEditView::save_document( _variant_t &file_name )
{
	COM_ENFORCE( get_edit()->SaveDocument( &file_name, FALSE ), _T("Failed to save document to file ") + CString( (LPCWSTR)file_name.bstrVal ) ) ;
}
// set_browse_mode
void CHtmlEditView::set_browse_mode( bool setting )
{
	ATLTRACE( "CHtmlEditView::set_browse_mode ( %s )\n", ( setting ? "true" : "false") ) ;

#pragma warning( disable:4310 ) // cast truncates constant value
	COM_ENFORCE( get_edit()->put_BrowseMode( ( setting ? VARIANT_TRUE : VARIANT_FALSE ) ), _T("Failed to set dhtml editing control's browse mode to ") + CString( setting ? _T("true") : _T("false") ) ) ;
#pragma warning( default:4310 ) // cast truncates constant value
}

// --------------------------------------
// events
// --------------------------------------


// Function name	: CHtmlEditView::OnDisplayChanged
// Description	    : 
// Return type		: void __stdcall 
void __stdcall CHtmlEditView::OnDisplayChanged()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnDisplayChanged() ;
	}

}

// Function name	: CHtmlEditView::OnShowContextMenu
// Description	    : 
// Return type		: void __stdcall 
// Argument         :  int x
// Argument         : int y
void __stdcall CHtmlEditView::OnShowContextMenu( int x, int y )
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnShowContextMenu( x, y ) ;
	}

}


// Function name	: CHtmlEditView::OnDocumentComplete
// Description	    : 
// Return type		: void __stdcall 
void __stdcall CHtmlEditView::OnDocumentComplete( )
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnDocumentComplete() ;
	}
}

// Function name	: CHtmlEditView::OnMouseDown
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnMouseDown()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnMouseDown() ;
	}
}

// Function name	: CHtmlEditView::OnMouseMove 
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnMouseMove ()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnMouseMove() ;
	}
}

// Function name	: CHtmlEditView::OnMouseUp
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnMouseUp()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnMouseUp() ;
	}
}

// Function name	: CHtmlEditView::OnMouseOut
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnMouseOut()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnMouseOut() ;
	}
}

// Function name	: CHtmlEditView::OnMouseOver
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnMouseOver()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnMouseOver() ;
	}
}

// Function name	: CHtmlEditView::OnReadyStateChange
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnReadyStateChange()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnReadyStateChange() ;
	}
}

// Function name	: CHtmlEditView::OnClick
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnClick()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnClick() ;
	}
}

// Function name	: CHtmlEditView::OnDoubleClick
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnDoubleClick()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnDoubleClick() ;
	}
}

// Function name	: CHtmlEditView::OnKeyDown
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnKeyDown()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnKeyDown() ;
	}
}

// Function name	: CHtmlEditView::OnKeyPress
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnKeyPress()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnKeyPress() ;
	}
	m_dirty = true ;
}

// Function name	: CHtmlEditView::OnKeyUp
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnKeyUp()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnKeyUp() ;
	}
}

// Function name	: CHtmlEditView::OnBlur
// Description	    : 
// Return type		: VOID __stdcall 
VOID __stdcall CHtmlEditView::OnBlur()
{
	if ( m_listener != NULL ) 
	{
		m_listener->HtmlEditOnBlur() ;
	}
}


// =========================
// helpers for control interface
// =========================

// Function name	: CHtmlEditView::ExecCommand
// Description	    : 
// Return type		: _variant_t 
// Argument         :  DHTMLEDLib::DHTMLEDITCMDID cmd_id
// Argument         : OLECMDEXECOPT option
// Argument         : _variant_t &vIn
_variant_t CHtmlEditView::ExecCommand( DHTMLEDLib::DHTMLEDITCMDID cmd_id, OLECMDEXECOPT option, _variant_t vIn )
{
	_variant_t vOut ;
	COM_ENFORCE( get_edit()->raw_ExecCommand( cmd_id, option, &vIn, &vOut ), ( CString("Call to ExecCommand() function of DHTML editing control with code ") + int2tstring( cmd_id ).c_str() + CString("(") + code2tstring( cmd_id ).c_str() + CString(") failed") ) ) ;
	return vOut ;
}

// Function name	: CHtmlEditView::QueryStatus
// Description	    : 
// Return type		: DHTMLEDLib::DHTMLEDITCMDF 
// Argument         : DHTMLEDLib::DHTMLEDITCMDID status
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::QueryStatus( DHTMLEDLib::DHTMLEDITCMDID status )
{
	DHTMLEDLib::DHTMLEDITCMDF retval ;
	COM_ENFORCE( get_edit()->raw_QueryStatus( status, &retval ), ( CString("Call to QueryStatus() function of DHTML editing control with code ") + int2tstring( status ).c_str() + CString("(") + code2tstring( status ).c_str() + CString(") failed") ) ) ;
	return retval ;
}

// =========================
// MESSAGE HANDLERS
// various edit operations
// =========================

// Function name	: CHtmlEditView::OnFormatFont
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnFormatFont( )
{
	show_font_dialog() ;
	return 0 ;
}

// Function name	: CHtmlEditView::OnEditUndo
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditUndo( )
{
	ExecCommand( DHTMLEDLib::DECMD_UNDO ) ;
	return 0 ;
}

// Function name	: CHtmlEditView::OnEditRedo
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditRedo( )
{
	ExecCommand( DHTMLEDLib::DECMD_REDO ) ;
	return 0 ;
}

// Function name	: CHtmlEditView::OnEditFind
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditFind( )
{
	ExecCommand( DHTMLEDLib::DECMD_FINDTEXT, OLECMDEXECOPT_DODEFAULT ) ;
	return 0 ;
}


// Function name	: CHtmlEditView::OnEditSelectAll
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditSelectAll( )
{
	CHtmlTextRange range = get_selected_text_range() ;

	range.expand( L"Textedit") ;
	range.select() ;

	return 0L ;
}



// Function name	: CHtmlEditView::OnEditCopy
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditCopy( )
{
	ExecCommand( DHTMLEDLib::DECMD_COPY ) ;
	return 0 ;
}


// Function name	: CHtmlEditView::OnEditCut
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditCut( )
{
	ExecCommand( DHTMLEDLib::DECMD_CUT ) ;
	m_dirty = true ;
	return 0;
}


// Function name	: CHtmlEditView::OnEditPaste
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditPaste( )
{
	ExecCommand( DHTMLEDLib::DECMD_PASTE ) ;
	m_dirty = true ;
	return 0;
}


// Function name	: CHtmlEditView::OnEditDelete
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlEditView::OnEditDelete( )
{
	ExecCommand( DHTMLEDLib::DECMD_DELETE ) ;
	m_dirty = true ;
	return 0;
}

// --------------------------------

// Function name	: CHtmlEditView::new_document
// Description	    : 
// Return type		: void 
void CHtmlEditView::new_document()
{
	COM_ENFORCE( get_edit()->NewDocument(), _T("Failed to create new DHTML document") ) ;
}

// Function name	: CHtmlEditView::is_dirty
// Description	    : 
// Return type		: bool 
bool CHtmlEditView::is_dirty()
{
	return m_dirty ;
}

void CHtmlEditView::set_dirty_off()
{
	m_dirty = false ;
}

void CHtmlEditView::set_dirty_on()
{
	m_dirty = true ;
}

// Function name	: CHtmlEditView::is_busy
// Description	    : 
// Return type		: bool 
bool CHtmlEditView::is_busy() 
{
	VARIANT_BOOL vbBusy = VARIANT_FALSE ;
	COM_ENFORCE( get_edit()->get_Busy( &vbBusy ), _T("Status query to DHTML Editing Control failed") ) ;
	return ( vbBusy != VARIANT_FALSE ) ;
}


// Function name	: CHtmlEditView::get_selected_object
// Description	    : 
// Return type		: sel_obj_ptr 
html::sel_obj_ptr CHtmlEditView::get_selected_object()
{
	CHtmlDocument doc = get_document() ;
	return doc.get_selection() ;
}


// Function name	: CHtmlEditView::get_selected_text_range
// Description	    : 
// Return type		: text_range_ptr 
html::text_range_ptr CHtmlEditView::get_selected_text_range( )
{
	// get the selected object
	CHtmlSelection selection = get_selected_object() ;
	return selection.create_text_range() ;
}


// Function name	: CHtmlEditView::code2tstring
// Description	    : 
// Return type		: tstring 
// Argument         : int code
tstring CHtmlEditView::code2tstring( int code )
{
#ifdef CASE_STR
#error "CASE_STR is already defined. The define in CHtmlEditView::code2tstring could conflict with this setting."
#endif

#define CASE_STR( x ) case x:  { return tstring( CString(#x) ) ; }
using namespace DHTMLEDLib ;
	switch ( code )
	{
		CASE_STR( DECMD_BOLD )
		CASE_STR( DECMD_COPY )
		CASE_STR( DECMD_CUT )
		CASE_STR( DECMD_DELETE )		
		CASE_STR( DECMD_DELETECELLS )	
		CASE_STR( DECMD_DELETECOLS )	
		CASE_STR( DECMD_DELETEROWS )
		CASE_STR( DECMD_FINDTEXT )		
		CASE_STR( DECMD_FONT )			
		CASE_STR( DECMD_GETBACKCOLOR ) 
		CASE_STR( DECMD_GETBLOCKFMT )	
		CASE_STR( DECMD_GETBLOCKFMTNAMES ) 
		CASE_STR( DECMD_GETFONTNAME )  
		CASE_STR( DECMD_GETFONTSIZE )  
		CASE_STR( DECMD_GETFORECOLOR )  
		CASE_STR( DECMD_HYPERLINK )  
		CASE_STR( DECMD_IMAGE )  
		CASE_STR( DECMD_INDENT )  
		CASE_STR( DECMD_INSERTCELL )  
		CASE_STR( DECMD_INSERTCOL )  
		CASE_STR( DECMD_INSERTROW )   
		CASE_STR( DECMD_INSERTTABLE )   
		CASE_STR( DECMD_ITALIC )   
		CASE_STR( DECMD_JUSTIFYCENTER )   
		CASE_STR( DECMD_JUSTIFYLEFT )   
		CASE_STR( DECMD_JUSTIFYRIGHT )   
		CASE_STR( DECMD_LOCK_ELEMENT )   
		CASE_STR( DECMD_MAKE_ABSOLUTE )   
		CASE_STR( DECMD_MERGECELLS )   
		CASE_STR( DECMD_ORDERLIST )   
		CASE_STR( DECMD_OUTDENT )   
		CASE_STR( DECMD_PASTE )   
		CASE_STR( DECMD_REDO )   
		CASE_STR( DECMD_REMOVEFORMAT )   
		CASE_STR( DECMD_SELECTALL )   
		CASE_STR( DECMD_SEND_BACKWARD )   
		CASE_STR( DECMD_BRING_FORWARD )   
		CASE_STR( DECMD_SEND_BELOW_TEXT )   
		CASE_STR( DECMD_BRING_ABOVE_TEXT )   
		CASE_STR( DECMD_SEND_TO_BACK )   
		CASE_STR( DECMD_BRING_TO_FRONT )   
		CASE_STR( DECMD_SETBACKCOLOR )   
		CASE_STR( DECMD_SETBLOCKFMT )   
		CASE_STR( DECMD_SETFONTNAME )   
		CASE_STR( DECMD_SETFONTSIZE )   
		CASE_STR( DECMD_SETFORECOLOR )   
		CASE_STR( DECMD_SPLITCELL )   
		CASE_STR( DECMD_UNDERLINE )   
		CASE_STR( DECMD_UNDO )   
		CASE_STR( DECMD_UNLINK )   
		CASE_STR( DECMD_UNORDERLIST )   
		CASE_STR( DECMD_PROPERTIES )  
	}
	return tstring( _T("Unknown code") ) ;
}


// Function name	: CHtmlEditView::show_glyphs
// Description	    : 
// Return type		: void 
// Argument         : bool setting
void CHtmlEditView::show_glyphs(bool setting)
{
#pragma warning( disable:4310 ) // cast truncates constant value
	COM_ENFORCE( get_edit()->put_ShowDetails( ( setting ? VARIANT_TRUE : VARIANT_FALSE ) ), CString("Failed to set glyph view mode to ") + ( setting ? (_T("TRUE")) : (_T("FALSE") ) ) );
#pragma warning( default:4310 ) // cast truncates constant value
}


// Function name	: CHtmlEditView::set_base_url
// Description	    : 
// Return type		: void 
// Argument         : const wstring &base_url
void CHtmlEditView::set_base_url(const wstring &base_url)
{
	set_base_url( _bstr_t( base_url.c_str() ))	;

}

void CHtmlEditView::set_base_url( const _bstr_t &base_url )
{
	COM_ENFORCE( get_edit()->put_BaseURL( base_url ), 
		_T("Failed to set base url to ") + CString( (LPCTSTR)base_url ) ) ;
}

// Function name	: CHtmlEditView::get_browse_mode
// Description	    : 
// Return type		: bool 
bool CHtmlEditView::get_browse_mode()
{
	VARIANT_BOOL setting ;
	COM_ENFORCE( get_edit()->get_BrowseMode( &setting ), _T("Failed to get dhtml editing control's browse mode to ") + CString( setting ? _T("true") : _T("false") ) ) ;

	return setting != VARIANT_FALSE ;
}


// Function name	: CHtmlEditView::get_last_event
// Description	    : 
// Return type		: event_obj_ptr 
html::event_obj_ptr CHtmlEditView::get_last_event()
{
	document_ptr doc = get_document() ;

	window_ptr window ;

	COM_ENFORCE( doc->get_parentWindow( &window ), _T("Failed to get parent window of HTML document") ) ;

	event_obj_ptr event ;

	COM_ENFORCE( window->get_event( &event ), _T("Failed to get DHTML event") ) ;

	return event ;
}


// Function name	: CHtmlEditView::get_location
// Description	    : 
// Return type		: const wstring 
const _bstr_t CHtmlEditView::get_location()
{
	_bstr_t location ;

	HRESULT hr = get_edit()->get_CurrentDocumentPath( location.GetAddress() ) ;

	if ( FAILED( hr ) )
	{
		return _bstr_t() ;
	}

	if ( (BSTR)location == NULL )
		return _bstr_t() ;

	return location ;
}



// Function name	: CHtmlEditView::do_ordered_list
// Description	    : 
// Return type		: void 
void CHtmlEditView::do_ordered_list()
{
	ExecCommand( DHTMLEDLib::DECMD_ORDERLIST ) ;
}


// Function name	: CHtmlEditView::do_unordered_list
// Description	    : 
// Return type		: void 
void CHtmlEditView::do_unordered_list()
{
	ExecCommand( DHTMLEDLib:: DECMD_UNORDERLIST ) ;
}


// Function name	: CHtmlEditView::insert_table
// Description	    : 
// Return type		: void 
// Argument         : int num_rows
// Argument         : int num_cols
// Argument         : const wstring &table_attr
// Argument         : const wstring &cell_attr
// Argument         : const wstring &title
void CHtmlEditView::insert_table(int num_rows, int num_cols, const wstring &table_attr, const wstring &cell_attr, const wstring &title)
{
	CComPtr< DHTMLEDLib::IDEInsertTableParam > table_param ;

	COM_ENFORCE( table_param.CoCreateInstance( L"DEInsertTableParam.DEInsertTableParam"), _T("Failed to create insert table param object") ) ;

	table_param->put_NumRows( num_rows ) ;
	table_param->put_NumCols( num_cols ) ;
	if ( table_attr.empty() == false )
		table_param->put_TableAttrs ( _bstr_t( table_attr.c_str() ) ) ;
	if ( cell_attr.empty() == false )
		table_param->put_CellAttrs ( _bstr_t( cell_attr.c_str() ) ) ;
	if ( title.empty() == false )
		table_param->put_Caption ( _bstr_t( title.c_str() ) ) ;


	_variant_t vIn( table_param ) ;
	ExecCommand( DHTMLEDLib::DECMD_INSERTTABLE, OLECMDEXECOPT_DONTPROMPTUSER, vIn ) ;

}


// Function name	: CHtmlEditView::insert_image
// Description	    : 
// Return type		: void 
// Argument         : const wstring &file_name
void CHtmlEditView::insert_image(const wstring &file_name)
{
	if ( file_name.empty() )
		ExecCommand( DHTMLEDLib::DECMD_IMAGE, OLECMDEXECOPT_DODEFAULT ) ;
	else
	{
		_variant_t arg( file_name.c_str() ) ;
		ExecCommand( DHTMLEDLib::DECMD_IMAGE, OLECMDEXECOPT_PROMPTUSER, arg ) ;
	}

}


// Function name	: CHtmlEditView::insert_hyperlink
// Description	    : 
// Return type		: void 
void CHtmlEditView::insert_hyperlink()
{
	ExecCommand( DHTMLEDLib::DECMD_HYPERLINK, OLECMDEXECOPT_DODEFAULT ) ;
}


// Function name	: CHtmlEditView::insert_text
// Description	    : 
// Return type		: void 
// Argument         : const wstring &text
void CHtmlEditView::insert_text(const wstring &text)
{
	CHtmlDocument doc = get_document() ;
	CHtmlSelection selection = doc.get_selection() ;
	CHtmlTextRange range = selection.create_text_range() ;

	range.put_html_text( text ) ;
}


// Function name	: CHtmlEditView::select_all
// Description	    : 
// Return type		: void 
void CHtmlEditView::select_all()
{
	CHtmlDocument doc = get_document() ;
	CHtmlSelection selection = doc.get_selection() ;
	CHtmlTextRange range = selection.create_text_range() ;
	range.expand( L"Textedit" ) ;
	range.select() ;
}


// Function name	: CHtmlEditView::set_selection
// Description	    : 
// Return type		: void 
// Argument         : unsigned int start
// Argument         : unsigned int end
void CHtmlEditView::set_selection(unsigned int start, unsigned int end)
{
	ATLASSERT( start <= end ) ;

	CHtmlDocument doc = get_document() ;
	CHtmlSelection selection = doc.get_selection() ;
	CHtmlTextRange range = selection.create_text_range() ;
	range.expand( L"Textedit") ;
	range.collapse( true ) ; // true = collapse to start

	// We wanted unsigned int for this function's arguments, but the
	// actual move range functions can move them forward and backward,
	// so we need to cast back to long
	range.move_end( L"Character", static_cast< long >( end ) ) ;
	range.move_start(L"Character", static_cast< long >( start ) ) ;
	range.select() ;
}


// Function name	: CHtmlEditView::get_text
// Description	    : 
// Return type		: void 
// Argument         : BSTR *text
void CHtmlEditView::get_text(BSTR *text)
{
	// the DHTML tags are filtered out when we get the entire document html.
	COM_ENFORCE( get_edit()->get_DocumentHTML(text), _T("Failed to retrieve document text") ) ;
}

_bstr_t CHtmlEditView::get_font_name()
{
	switch ( QueryStatus( DHTMLEDLib::DECMD_GETFONTNAME ) )
	{
	case DHTMLEDLib::DECMDF_DISABLED :
	case DHTMLEDLib::DECMDF_NINCHED:
		return _bstr_t() ;
	}

	_variant_t font_name ;

	get_edit()->raw_ExecCommand( 
		DHTMLEDLib::DECMD_GETFONTNAME, 
		OLECMDEXECOPT_DODEFAULT, 
		NULL, 
		&font_name ) ;

	return _bstr_t( font_name.bstrVal ) ;

}


// Function name	: CHtmlEditView::get_font_size
// Description	    : 
// Return type		: int 
int CHtmlEditView::get_font_size()
{
	switch ( QueryStatus( DHTMLEDLib::DECMD_GETFONTSIZE ) )
	{
	case DHTMLEDLib::DECMDF_DISABLED :
	case DHTMLEDLib::DECMDF_NINCHED:
		return 0 ;
	}
	
	_variant_t font_size ;
	get_edit()->raw_ExecCommand( 
		DHTMLEDLib::DECMD_GETFONTSIZE, 
		OLECMDEXECOPT_DODEFAULT, 
		NULL, 
		&font_size ) ;
	

	return font_size.intVal ;

}


// Function name	: CHtmlEditView::get_bold
// Description	    : 
// Return type		: DHTMLEDLib::DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_bold()
{
	return QueryStatus( DHTMLEDLib::DECMD_BOLD ) ;
}


// Function name	: CHtmlEditView::get_italic
// Description	    : 
// Return type		: DHTMLEDLib::DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_italic()
{
	return QueryStatus( DHTMLEDLib::DECMD_ITALIC ) ;
}


// Function name	: CHtmlEditView::get_underline
// Description	    : 
// Return type		: DHTMLEDLib::DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_underline()
{
	return QueryStatus( DHTMLEDLib::DECMD_UNDERLINE ) ;
}



// Function name	: CHtmlEditView::get_center
// Description	    : 
// Return type		: DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_center()
{
	return QueryStatus( DHTMLEDLib::DECMD_JUSTIFYCENTER ) ;
}


// Function name	: CHtmlEditView::get_left
// Description	    : 
// Return type		: DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_left()
{
	return QueryStatus( DHTMLEDLib::DECMD_JUSTIFYLEFT ) ;
}


// Function name	: CHtmlEditView::get_right
// Description	    : 
// Return type		: DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_right()
{
	return QueryStatus( DHTMLEDLib::DECMD_JUSTIFYRIGHT ) ;
}


// Function name	: CHtmlEditView::get_ord_list
// Description	    : 
// Return type		: DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_ord_list()
{
	return QueryStatus( DHTMLEDLib::DECMD_ORDERLIST ) ;
}


// Function name	: CHtmlEditView::get_unord_list
// Description	    : 
// Return type		: DHTMLEDITCMDF 
DHTMLEDLib::DHTMLEDITCMDF CHtmlEditView::get_unord_list()
{
	return QueryStatus( DHTMLEDLib::DECMD_UNORDERLIST ) ;
}


// Function name	: CHtmlEditView::get_block_format
// Description	    : 
// Return type		: tstring 
_bstr_t CHtmlEditView::get_block_format()
{
	switch ( QueryStatus( DHTMLEDLib::DECMD_GETBLOCKFMT ) )
	{
	case DHTMLEDLib::DECMDF_DISABLED :
	case DHTMLEDLib::DECMDF_NINCHED:
		return _bstr_t() ;
	}
	
	_variant_t style_name ;
	
	get_edit()->raw_ExecCommand( 
		DHTMLEDLib::DECMD_GETBLOCKFMT, 
		OLECMDEXECOPT_DODEFAULT, 
		NULL, 
		&style_name ) ;
	
	return _bstr_t( style_name.bstrVal ) ;
}


// Function name	: CHtmlEditView::set_block_format
// Description	    : 
// Return type		: void 
// Argument         : const wstring &format_name
void CHtmlEditView::set_block_format(const wstring &format_name)
{
	set_block_format( _variant_t( format_name.c_str() ) ) ;
}
void CHtmlEditView::set_block_format(const _bstr_t &format_name)
{
	set_block_format( _variant_t( format_name ) ) ;
}
void CHtmlEditView::set_block_format(const _variant_t &format_name)
{
	if ( QueryStatus( DHTMLEDLib::DECMD_SETBLOCKFMT) == DHTMLEDLib::DECMDF_DISABLED ) throw except::CException( _T("The selected operation is not available")  ) ;	
	
	ExecCommand( DHTMLEDLib::DECMD_SETBLOCKFMT, OLECMDEXECOPT_DODEFAULT, format_name ) ;

}

_bstr_t CHtmlEditView::get_fore_color()
{
	switch ( QueryStatus( DHTMLEDLib::DECMD_GETFORECOLOR ) )
	{
	case DHTMLEDLib::DECMDF_DISABLED :
	case DHTMLEDLib::DECMDF_NINCHED:
		return _bstr_t() ;
	}
	
	_variant_t fore_color ;
	
	get_edit()->raw_ExecCommand( 
		DHTMLEDLib::DECMD_GETFORECOLOR, 
		OLECMDEXECOPT_DODEFAULT, 
		NULL, 
		&fore_color ) ;
	
	return _bstr_t( fore_color.bstrVal );
	
}

_bstr_t CHtmlEditView::get_back_color()
{
	switch ( QueryStatus( DHTMLEDLib::DECMD_GETBACKCOLOR) )
	{
	case DHTMLEDLib::DECMDF_DISABLED :
	case DHTMLEDLib::DECMDF_NINCHED:
		return _bstr_t() ;
	}
	
	_variant_t back_color ;
	
	get_edit()->raw_ExecCommand( 
		DHTMLEDLib::DECMD_GETBACKCOLOR, 
		OLECMDEXECOPT_DODEFAULT, 
		NULL, 
		&back_color ) ;
	
	return _bstr_t( back_color.bstrVal ) ;
}


void CHtmlEditView::wait_until_not_busy()
{
	MSG msg ;
	
	while( is_busy() )
	{
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{ 
			if(!PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		} 
	}

}

}