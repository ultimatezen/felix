// HtmlView.cpp: implementation of the CHtmlView class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HtmlView.h"
#include "MsHtmHst.h"
#include "Path.h"
#include "logging.h"

namespace html
{
	using namespace except ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtmlView::CHtmlView( bool allow_nav /* = true */ ) : 
	m_allow_nav( allow_nav ), 
	m_is_instantiated( false ), 
	m_document_complete( false ), 
	m_navigation_complete( false ),
	m_listener( NULL ),
	m_ctl_id(next_htmlview_id())
{
	BOOL success(FALSE) ;
	success = AtlAxWinInit() ;
	ATLASSERT( success ) ;
}


// Function name	: CHtmlView::set_scroll_pos
bool CHtmlView::set_scroll_pos( long pos )
{
#ifdef UNIT_TEST
	pos ;
	return true ;
#else	

	window_ptr window = get_window() ;

	COM_ENFORCE( window->scrollTo( 0, pos ), _T("Failed to set browser scroll position") ) ;
	return true ;
#endif
}


// Function name	: > CHtmlView::get_window
// Description	    : 
// Return type		: CComPtr< IHTMLWindow2 
CComPtr< MSHTML::IHTMLWindow2 > CHtmlView::get_window()
{
	ATLTRACE( "> CHtmlView::get_window\n" ) ;

	document_ptr doc = get_document() ;

	window_ptr window ;
	COM_ENFORCE( doc->get_parentWindow( &window ), _T("Failed to get browser window") ) ;

	return window ;

}


// Function name	: > CHtmlView::get_body
// Description	    : 
// Return type		: CComPtr< IHTMLElement 
CComPtr< MSHTML::IHTMLElement > CHtmlView::get_body( )
{
	ATLTRACE( "> CHtmlView::get_body\n" ) ;

	CHtmlDocument doc = get_document() ;
	return doc.get_body() ;
}


// Function name	: > CHtmlView::get_doc_body
// Description	    : 
// Return type		: CComPtr< IHTMLBodyElement 
CComPtr< MSHTML::IHTMLBodyElement > CHtmlView::get_doc_body( )
{
	ATLTRACE( "> CHtmlView::get_doc_body\n" ) ;

	browser_ptr browser = get_browser() ;
	CComQIPtr< MSHTML::IHTMLBodyElement > body( browser ) ;

	return body ;
}

// create
HWND CHtmlView::create( HWND hWnd, const TCHAR *startup_page, UINT id )
{
	AddRef() ;
	if (id)
	{
		m_ctl_id = id ;
	}
	DWORD flags = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL ;
	return Create(hWnd, 
				rcDefault, 
				startup_page, 
				flags,
				WS_EX_CLIENTEDGE, 
				m_ctl_id ) ;
}
 
// PreTranslateMessage
BOOL CHtmlView::PreTranslateMessage(MSG* pMsg)
{
	// just ignore anything but mouse and keyboard messages
	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
	   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
	{
		return FALSE ;
	}

	BOOL bRet = FALSE;
	// give HTML page a chance to translate this message
	if(pMsg->hwnd == m_hWnd || IsChild(pMsg->hwnd))
	{
		bRet = (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
	}

	return bRet;
}

// navigate
bool CHtmlView::navigate(const string &url )
{
	return navigate( string2wstring( url ) ) ;
}

// navigate
bool CHtmlView::navigate(const wstring &url )
{
	ATLTRACE( "CHtmlView::navigate\n" ) ;

	m_navigation_complete = false ;
	m_document_complete = false ;

#ifdef UNIT_TEST
	url ;
	return true ;
#else	

	if ( ! is_instantiated() )
	{
		instantiate() ;
		if ( ! is_instantiated() )
		{
			return false ;
		}
	}

	CHtmlWebBrowser browser( get_browser() ) ;
	browser.navigate( url ) ;
	return true ;
#endif
}

// set_content
bool CHtmlView::write(const wstring &content)
{
	ATLTRACE("CHtmlView::write\n") ;

	CHtmlDocument doc = get_document() ;
	doc.write( content ) ;

	return true ;
}


// send_message_to_parent
bool CHtmlView::send_message_to_parent( LPMSG pMsg )
{
	CWindow parent = GetParent() ;
	parent.SendMessage( UWM_USER_MESSAGE, pMsg->wParam, pMsg->lParam ) ;
	return true ;
}

// LoadedHandler
void CHtmlView::LoadedHandler(const OLECHAR *message)
{ 
	ATLTRACE( "CHtmlView::Loadedhandler()\n" ) ;

	m_loaded_message = message ;
	m_navigation_complete = true ;
}

// instantiate
bool CHtmlView::instantiate()
{
#ifdef UNIT_TEST
	return true ;
#else

	try
	{
		ATLASSERT( IsWindow() ) ;
		// Connect events
		COM_ENFORCE( AtlAdviseSinkMap(this, true), _T("Failed to instantiate HTML widget") ) ; 
	
		m_is_instantiated = true ;
	}
	catch (CException& e)
	{
		logging::log_error("Program exception - Failed to instantiate browser object") ;
		logging::log_exception(e) ;
		ATLASSERT(FALSE && "Failed to instantiate browser object") ;
		return false ;
	}
	catch (_com_error& e)
	{
		logging::log_error("COM exception - Failed to instantiate browser object") ;
		logging::log_exception(e) ;
		ATLASSERT(FALSE && "Failed to instantiate browser object") ;
		return false ;
	}
	return true ;
#endif
}

// is_instantiated
bool CHtmlView::is_instantiated() 
{ 
	return m_is_instantiated ; 
}
bool CHtmlView::is_navigation_complete() 
{ 
#ifdef UNIT_TEST
	return true ;
#else
	return m_navigation_complete ; 
#endif
}
bool CHtmlView::is_document_complete() 
{
#ifdef UNIT_TEST
	return true ;
#else
	return m_document_complete ; 
#endif
}

// get_selected_text
wstring CHtmlView::get_selected_text()
{
	ATLTRACE( "CHtmlView::get_selected_text()\n" ) ;

	// get the selection
	CHtmlSelection selection = get_selection() ;
	CHtmlTextRange range = selection.create_text_range() ;
	return BSTR2wstring(range.get_html_text()) ;
}

// get a smart pointer to IWebBrowser
browser_ptr CHtmlView::get_browser()
{
	browser_ptr browser ;

#ifdef UNIT_TEST
	return browser ;
#else

	COM_ENFORCE( QueryControl( &browser ), _T("Failed to obtain browser object") ) ;
	ATLASSERT( browser ) ;

	return browser ;
#endif
}
// get a smart pointer to IDispatch
dispatch_ptr CHtmlView::get_document_dispatch()
{

	ensure_document_complete() ;
	// get the browser
	browser_ptr browser = get_browser() ;

	// get the dispatch
	dispatch_ptr dispatch ;
#ifndef UNIT_TEST
	COM_ENFORCE( browser->get_Document( &dispatch ), _T("Failed to obtain document dispatch for browser") ) ;
	ATLASSERT( dispatch != NULL ) ;
#endif

	return dispatch ;
}
// get a smart pointer to IHTMLDocument
document_ptr CHtmlView::get_document( )
{
	// get the document
	CComQIPtr< MSHTML::IHTMLDocument2 > document( get_document_dispatch() ) ;

#ifndef UNIT_TEST
	ATLASSERT( document != NULL ) ;
#endif

	return document ;
}
document3_ptr CHtmlView::get_document3()
{
	ATLTRACE( "CHtmlView::get_document3()\n" ) ;

	// get the document
	CComQIPtr< MSHTML::IHTMLDocument3 > document( get_document_dispatch() ) ;

	ATLASSERT( document != NULL ) ;

	return document ;

}

// get a smart pointer to IHTMLDocument
sel_obj_ptr CHtmlView::get_selection( )
{

	// get the document
	CHtmlDocument document = get_document() ;

	return document.get_selection() ;
}

// COMMAND HANDLERS

LRESULT CHtmlView::on_create(UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	uMsg ;
	wParam ;
	lParam ;

	
#ifndef UNIT_TEST
	// Create the control for me
	ATLASSERT( m_hWnd != NULL ) ;
	DefWindowProc(uMsg, wParam, lParam);

	instantiate() ;
	// Set host flag to indicate that we handle themes
	CComPtr<IAxWinAmbientDispatch> spHost;
	HRESULT hRet = QueryHost(IID_IAxWinAmbientDispatch, (void**)&spHost);
	if(SUCCEEDED(hRet))
	{
		const DWORD _DOCHOSTUIFLAG_THEME = 0x40000;
		hRet = spHost->put_DocHostFlags(DOCHOSTUIFLAG_NO3DBORDER | _DOCHOSTUIFLAG_THEME);
		ATLASSERT(SUCCEEDED(hRet));
	}
#endif
	
	return 0;
}

// Function name	: CHtmlView::OnEditCopy
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnEditCopy( )
{
	ATLTRACE( "CHtmlView::OnEditCopy\n" ) ;
	command_target_ptr command_target = get_command_target() ;

	// Copy the current selection
	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to copy browser selction into clipboard") ) ;
	return 0L;
}


// Function name	: CHtmlView::OnEditSelectAll
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnEditSelectAll( ) 
{
	ATLTRACE( "CHtmlView::OnEditSelectAll\n" ) ;

	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_SELECTALL, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to cut browser selction into clipboard") ) ;
	return 0L;
}


// Function name	: CHtmlView::OnEditCut
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnEditCut( ) 
{
	ATLTRACE( "CHtmlView::OnEditCut\n" ) ;

	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_CUT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to cut browser selction into clipboard") ) ;
	return 0L;
}


// Function name	: CHtmlView::OnEditDelete
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnEditDelete( ) 
{
	ATLTRACE( "CHtmlView::OnEditDelete\n" ) ;

	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_DELETE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to cut browser selction into clipboard") ) ;
	return 0L;
}


// Function name	: CHtmlView::OnEditPaste
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnEditPaste( ) 
{
	ATLTRACE( "CHtmlView::OnEditPaste\n" ) ;

	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_PASTE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to paste clipboard contents into browser") ) ;
	return 0L;
}

// Function name	: CHtmlView::OnEditUndo
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnEditUndo( ) 
{
	ATLTRACE( "CHtmlView::OnEditUndo\n" ) ;

	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_UNDO, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to undo action") ) ;
	return 0L;
}

// Function name	: CHtmlView::OnEditRedo
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnEditRedo( ) 
{
	ATLTRACE( "CHtmlView::OnEditRedo\n" ) ;

	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_REDO, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to redo action") ) ;
	return 0L;
}

// Function name	: CHtmlView::OnFormatFont
// Description	    : 
// Return type		: LRESULT 
LRESULT CHtmlView::OnFormatFont( ) 
{
	ATLTRACE( "CHtmlView::OnFormatFont\n" ) ;

	command_target_ptr command_target = get_command_target() ;

	// note: this one needs OLECMDEXECOPT_PROMPTUSER
	if (FAILED( command_target->Exec(&CGID_MSHTML, IDM_FONT, OLECMDEXECOPT_PROMPTUSER, NULL, NULL)))
	{
		MessageBeep(MB_ICONINFORMATION) ;
	}
	return 0L;
}


// Function name	: CHtmlView::do_bold
// Description	    : 
// Return type		: void 
void CHtmlView::do_bold() 
{
	ATLTRACE( "CHtmlView::do_bold\n" ) ;

#ifndef UNIT_TEST
	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_BOLD, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to toggle selection's bold setting") ) ;
#endif
}
void CHtmlView::do_delete()
{
	ATLTRACE( "CHtmlView::do_delete\n" ) ;

#ifndef UNIT_TEST
	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_DELETE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to toggle selection's bold setting") ) ;
#endif
}

// Function name	: CHtmlView::do_underline
// Description	    : 
// Return type		: void 
void CHtmlView::do_underline() 
{
	ATLTRACE( "CHtmlView::do_underline\n" ) ;

#ifndef UNIT_TEST
	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_UNDERLINE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to toggle selection's underline setting") ) ;
#endif
}

// Function name	: CHtmlView::do_italic
// Description	    : 
// Return type		: void 
void CHtmlView::do_italic() 
{
	ATLTRACE( "CHtmlView::do_italic\n" ) ;
#ifndef UNIT_TEST
	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_ITALIC, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to toggle selection's italic setting") ) ;
#endif
}

void CHtmlView::do_select_all()
{
	BANNER( "CHtmlView::do_select_all" ) ;
#ifndef UNIT_TEST
	command_target_ptr command_target = get_command_target() ;

	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_SELECTALL, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL), _T("Failed to select all") ) ;
#endif
}

// Function name	: CHtmlView::put_edit_mode
// Description	    : 
// Return type		: void 
// Argument         : const bool mode
void CHtmlView::put_edit_mode(const bool mode)
{
#ifdef UNIT_TEST
	mode; 
	return ;
#else
	 
	if (mode)
	{
		command_target_ptr command_target = get_command_target() ;
		CComVariant var(VARIANT_TRUE);
		var.vt = VT_BOOL; // Set the type to BOOL

		HRESULT hr = command_target->Exec(&CGID_MSHTML, IDM_RESPECTVISIBILITY_INDESIGN, OLECMDEXECOPT_DONTPROMPTUSER, &var, NULL) ;
		if (FAILED(hr))
		{
			logging::log_debug("Setting 'Respect visibility in design' option returned " + int2string(hr, 16)) ;
		}
	}

	document_ptr doc = get_document() ;
	ATLASSERT(doc) ;
	element_ptr element = doc->body ;
	ATLASSERT(element) ;
	CComQIPtr< MSHTML::IHTMLElement3 > element3(element) ;
	ATLASSERT(element3) ;
	COM_ENFORCE( element3->put_contentEditable( ( mode ? L"true" : L"false" ) ), 
		CString("Failed to set HTML content editable flag to ") + bool2tstring( mode ).c_str() ) ;
#endif
}


// Function name	: CHtmlView::get_edit_mode
// Description	    : 
// Return type		: bool 
bool CHtmlView::get_edit_mode()
{
	ATLTRACE( "CHtmlView::get_edit_mode\n" ) ;

	document_ptr doc = get_document() ;

	CComPtr< MSHTML::IHTMLElement > element ;
	doc->get_body( &element ) ;

	CComQIPtr< MSHTML::IHTMLElement3 > element3( element ) ; 
	if (! element3)
	{
		return false ;
	}

	const wstring mode = BSTR2wstring(element3->contentEditable) ;
	return boost::iequals( mode, L"true" );
 
}

CComPtr< IOleCommandTarget > CHtmlView::get_command_target()
{
	CHtmlWebBrowser browser = get_browser() ;
	return browser.get_command_target() ;
}

void CHtmlView::load_from_resource(const CString &resource_name)
{
	ATLASSERT( resource_name.IsEmpty() == false ) ;
	
	// get the instance
	HINSTANCE hInstance =  _Module.GetResourceInstance() ;
	ATLASSERT(hInstance != NULL);

	// get the module file name
	file::CPath mod_path ;
	mod_path.GetModFileName( hInstance ) ;

	// get the resource as a url
	CString resource_url ;
	resource_url += TEXT("res://") ;
	resource_url += mod_path.Path() ;
	resource_url += _T("/") ;
	resource_url += resource_name  ;
	ATLASSERT( ! resource_url.IsEmpty() ) ;

	m_navigation_complete = false ;
	m_document_complete = false ;

	// navigate to the resource url
	navigate( _bstr_t( (LPCTSTR)resource_url ) ) ;
}

void CHtmlView::set_body_text(const wstring &text)
{
#ifdef UNIT_TEST
	text ;
#else
	CHtmlDocument doc = get_document() ;
	ATLASSERT( doc.is_instantiated() ) ;

	CHtmlElement body = doc.get_body() ;
	ATLASSERT( body.is_instantiated() ) ;

	body.put_inner_html( text ) ;
#endif
}

wstring CHtmlView::get_body_text()
{
	CHtmlDocument doc = get_document() ;

	CHtmlElement body = doc.get_body() ;

	_bstr_t html_text =  body.get_inner_html() ;
	return wstring( (LPCWSTR)html_text ) ;
}


/*
 This gets called when we're used as a dialog control.
 */
CWindow CHtmlView::GetDlgItem(int nID) const
{
	ATLTRACE( "CHtmlView::GetDlgItem\n" ) ;

	ATLASSERT( nID == ID_HTML_CTL_EVENT_ID || nID == m_ctl_id ) ;
	
	if ( nID == ID_HTML_CTL_EVENT_ID )
		return m_hWnd; 
	if ( nID == m_ctl_id )
		return m_hWnd ;

	return CWindow() ;
}

bool CHtmlView::navigate(const _bstr_t &url)
{

	url ;

	m_navigation_complete = false ;
	
	if ( ! is_instantiated() )
	{
		instantiate() ;
		if ( ! is_instantiated() )
			return false ;
	}
	
#ifndef UNIT_TEST
	CHtmlWebBrowser browser( get_browser() ) ;
	
	browser.navigate( url ) ;
#endif
	
	return true ;

}

void CHtmlView::set_body_text(const _bstr_t &text)
{
	CHtmlDocument doc = get_document() ;
	
	CHtmlElement body = doc.get_body() ;
	
	body.put_inner_html( text ) ;
}


LRESULT CHtmlView::OnFilePrint()
{
	command_target_ptr command_target = get_command_target() ;
	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_PRINT, OLECMDEXECOPT_PROMPTUSER, NULL, NULL), _T("Failed to print web view content") ) ;
	return 0L ;
}

LRESULT CHtmlView::OnFilePageSetup() 
{
	command_target_ptr command_target = get_command_target() ;
	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_PAGESETUP, OLECMDEXECOPT_PROMPTUSER, NULL, NULL), _T("Failed to print web view content") ) ;
	return 0L ;
}
LRESULT CHtmlView::OnFilePrintPreview() 
{
	command_target_ptr command_target = get_command_target() ;
	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_PRINTPREVIEW, OLECMDEXECOPT_PROMPTUSER, NULL, NULL), _T("Failed to print web view content") ) ;
	return 0L ;
}

LRESULT CHtmlView::on_file_save( )
{
	command_target_ptr command_target = get_command_target() ;
	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_SAVE, OLECMDEXECOPT_PROMPTUSER, NULL, NULL), _T("Failed to print web view content") ) ;
	return 0L ;
}

LRESULT CHtmlView::on_file_save_as( )
{
	command_target_ptr command_target = get_command_target() ;
	COM_ENFORCE( command_target->Exec(&CGID_MSHTML, IDM_SAVEAS, OLECMDEXECOPT_PROMPTUSER, NULL, NULL), _T("Failed to print web view content") ) ;
	return 0L ;
}

LRESULT CHtmlView::on_destroy( UINT, WPARAM, LPARAM )
{
	this->SetMsgHandled(FALSE) ;
#ifdef UNIT_TEST
	return 1 ;
#else

	uninstantiate();
	return 1L ;
#endif
}

void CHtmlView::SetFocusToHTML()
{
	CComPtr<IWebBrowser2> spWebBrowser;
	HRESULT hRet = QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser);
	if(SUCCEEDED(hRet) && spWebBrowser != NULL)
	{
		CComPtr<IDispatch> spDocument;
		hRet = spWebBrowser->get_Document(&spDocument);
		if(SUCCEEDED(hRet) && spDocument != NULL)
		{
			CComQIPtr<IHTMLDocument2> spHtmlDoc = spDocument;
			if(spHtmlDoc != NULL)
			{
				CComPtr<IHTMLWindow2> spParentWindow;
				hRet = spHtmlDoc->get_parentWindow(&spParentWindow);
				if(spParentWindow != NULL && SUCCEEDED(hRet))
					spParentWindow->focus();
			}
		}
	}
}

LRESULT CHtmlView::OnSetFocus( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
	SetFocusToHTML();

	return lRet;
}

void CHtmlView::ensure_document_complete()
{
#ifdef UNIT_TEST
	return ;
#else

	MSG msg ;
	while ( is_document_complete() == false || is_navigation_complete() == false) 
	{
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{ 
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		} 
	}
#endif
}

void __stdcall CHtmlView::DocumentComplete( IDispatch * /*pDisp*/, VARIANT *URL )
{

	m_document_complete = true ;

	ATLASSERT( URL != NULL ) ;
	if (!URL)
	{
		logging::log_warn("NULL URL received in DocumentComplete") ;
		return ;
	}
	ATLASSERT( URL->vt == VT_BSTR ) ;

	if ( m_listener ) 
	{
		m_listener->OnDocumentComplete( URL->bstrVal ) ;
		return ;
	}
	LoadedHandler(URL->bstrVal) ;
}

void __stdcall CHtmlView::BeforeNavigate2( IDispatch * /*pDisp*/, 
										  VARIANT *URL, 
										  VARIANT * /*Flags*/, 
										  VARIANT * /*TargetFrameName*/, 
										  VARIANT * /*PostData*/, 
										  VARIANT * /*Headers*/, 
										  VARIANT_BOOL *Cancel )
{

	ATLASSERT( URL != NULL ) ;
	if (!URL)
	{
		logging::log_warn("NULL URL received in BeforeNavigate2") ;
		*Cancel = VARIANT_TRUE ;
		return ;
	}
	ATLASSERT( URL->vt == VT_BSTR ) ;

	*Cancel = VARIANT_FALSE ;

	if ( m_listener ) 
	{
		bool cancel = m_listener->OnBeforeNavigate2( URL->bstrVal ) ;
		if ( cancel ) 
		{
			*Cancel = VARIANT_TRUE ;
		}
		return ;
	}

	wstring raw_command = (LPCWSTR)( URL->bstrVal );
	// will return NULL if fails to parse
	LPMSG pMsg = m_parser.parse( raw_command ) ;

	// if we didn't parse the url, and we are
	// allowing navigation, then return now.
	// cancel is set to false, so the navigation will continue
	if ( ! pMsg && m_allow_nav )
	{
		return ;
	}

	// cancel nav
	*Cancel = VARIANT_TRUE ;

	if ( pMsg ) // send parsed message to parent
	{
		send_message_to_parent( pMsg ) ;
	}
}

void __stdcall CHtmlView::NavigateComplete2( IDispatch * /*pDisp*/, VARIANT *URL )
{

	ATLASSERT( URL != NULL && URL->vt == VT_BSTR ) ;

	m_navigation_complete = true ;

	if ( ! URL )
	{
		logging::log_warn("NULL URL received in BeforeNavigate2") ;
		return ;
	}
	m_status_text = BSTR2wstring( URL->bstrVal ) ;

	if ( m_listener ) 
	{
		m_listener->OnNavigateComplete2( URL->bstrVal ) ;
	}
}

void __stdcall CHtmlView::StatusTextChange( const BSTR text )
{
	m_status_text = (LPCWSTR)( text ) ;

	if ( m_listener ) 
	{
		m_listener->OnStatusTextChange( text ) ;
		return ;
	}
}

void __stdcall CHtmlView::TitleChange( const BSTR title )
{

	m_title_text = BSTR2wstring( title ) ;

	if ( m_listener ) 
	{
		m_listener->OnStatusTextChange( title ) ;
		return ;
	}
}

	void CHtmlView::uninstantiate()
	{
		// Disconnect events
		try
		{
			ATLASSERT(IsWindow()) ;
			COM_ENFORCE( AtlAdviseSinkMap(this, false), _T("Failed to unadvise connection points for HTML view") ) ; 
			m_is_instantiated = false ;
			m_document_complete = false ;
			m_navigation_complete = false ;
			//m_ctl_id = next_htmlview_id() ;
		}
		catch (CException& e)
		{
			logging::log_error("Program exception cleaning up Web browser component") ;
			logging::log_exception(e) ;
		}
		catch (_com_error& e)
		{
			logging::log_error("COM exception cleaning up Web browser component") ;
			logging::log_exception(e) ;
		}
	}
}
