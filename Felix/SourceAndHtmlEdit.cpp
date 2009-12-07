#include "StdAfx.h"
#include ".\sourceandhtmledit.h"
#include "resource.h"

#include "resource_string.h"	// R2T

#include "HtmlDocColorSetter.h"
#include "xpmenu/tools.h"
#include "text_templates.h"
#include "logging.h"

using namespace html ;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif


CSourceAndHtmlEdit::CSourceAndHtmlEdit(void)
{
}

CSourceAndHtmlEdit::~CSourceAndHtmlEdit(void)
{
	m_html_edit.Detach() ;
}

bool CSourceAndHtmlEdit::has_focus(HWND focus_hwnd) const
{
	if ( ! m_tabs.IsWindow() ) 
	{
		return false ;
	}

	if ( m_tabs.m_hWnd == focus_hwnd || m_tabs.IsChild( focus_hwnd ) ) 
	{
		return true ;
	}

	return false;
}

HWND CSourceAndHtmlEdit::get_active_view(void) const
{
	return m_tabs.GetActiveView() ;
}

HWND CSourceAndHtmlEdit::create(TWindow box_window, TWindow dlg_window, HWND top_pos )
{
#ifdef UNIT_TEST
	return NULL ;
#endif
	try
	{
		m_parent_dlg = dlg_window ;

		m_box.Attach( box_window ) ;

		CWindowRect rc(m_box) ;

		m_parent_dlg.ScreenToClient( &rc ) ;

		m_tabs.SetTabStyles(CTCS_TOOLTIPS);
		m_tabs.SetForwardNotifications( true ) ;
		m_tabs.Create(dlg_window, TWindow::rcDefault) ;
		m_tabs.MoveWindow( &rc, TRUE ) ;

		ATLASSERT(m_tabs.IsWindow()) ;

		m_html_edit.create(m_tabs, get_template_filename(_T("start_gloss.html")));
		ATLASSERT( m_html_edit.IsWindow() ) ;
		m_html_edit.ShowWindow( SW_SHOW ) ;

		// we needed to add ES_NOHIDESEL in order to show the selection while doing search & replace
		const DWORD te_style = WS_TABSTOP | WS_VSCROLL | WS_CHILD | WS_VISIBLE | ES_NOHIDESEL | ES_SAVESEL | ES_MULTILINE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
		m_text_edit.Create(m_tabs, TWindow::rcDefault, NULL, te_style, WS_EX_CLIENTEDGE ) ;
		ATLASSERT( m_text_edit.IsWindow() ) ;
		m_text_edit.set_up_standard_syntax_coloring() ;
		m_text_edit.SetCodePage( SC_CP_UTF8 ) ;
		m_text_edit.SetLexer( SCLEX_HTML ) ;

		m_tabs.AddTab( m_html_edit, R2T( IDS_EDITOR_TAB ) ) ;
		m_tabs.AddTab( m_text_edit, R2T( IDS_SOURCE_TAB )  ) ;

		m_tabs.SetWindowPos( top_pos, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;

		this->ensure_document_complete() ;
		m_html_edit.put_edit_mode(true) ;

		return m_tabs ;
	}
	catch (CException& e)
	{
		logging::log_error("Program exception creating source/html edit box") ;
		logging::log_exception(e) ;
		throw ;
	}
	catch (_com_error& e)
	{
		logging::log_error("COM exception creating source/html edit box") ;
		logging::log_exception(e) ;
		throw ;
	}
}

BOOL CSourceAndHtmlEdit::pre_translate(MSG* pMsg)
{
	HWND focus_item = get_active_view() ;
	if ( focus_item == m_html_edit )
	{
		if ( m_html_edit.PreTranslateMessage( pMsg ) )
		{
			return TRUE ;
		}
	}
	else if ( m_text_edit.PreTranslateMessage( pMsg ) )
	{
		return TRUE ;
	}

	return FALSE ;
}

void CSourceAndHtmlEdit::handle_size(void)
{
	RECT rc = { 0 } ;

	m_box.GetWindowRect( &rc ) ;
	m_parent_dlg.ScreenToClient( &rc ) ;
	m_box.ShowWindow( SW_HIDE ) ;

	// move the source view into place
	m_tabs.SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOACTIVATE);
}

void CSourceAndHtmlEdit::insert_symbol(void)
{
	CInputSymbolDlg dialog ;

	if ( dialog.DoModal() == IDCANCEL )
	{
		return ;
	}

	const wstring symbol = dialog.get_text() ;

	if ( symbol.empty() )
	{
		::MessageBeep( MB_ICONEXCLAMATION ) ;
		return ;
	}

	if ( m_html_edit == get_active_view() ) 
	{
		html::CHtmlSelection selection = m_html_edit.get_selection() ;
		html::CHtmlTextRange range = selection.create_text_range() ;
		range.put_html_text(symbol) ;
	}
	else
	{
		m_text_edit.ReplaceSel( string2string(symbol).c_str() ) ;
	}

}
_bstr_t CSourceAndHtmlEdit::get_selected_text(const bool all_if_none)
{

	TWindow active_window = get_active_view() ;

	_bstr_t text ;
	if ( active_window == m_html_edit )
	{
		html::CHtmlSelection selection = m_html_edit.get_selection() ;
		html::CHtmlTextRange range = selection.create_text_range() ;
		range.get_html_text(text) ;
	}
	else
	{
		m_text_edit.GetSelection( text ) ;
	}

	if ( text.length() == 0 && all_if_none) 
	{
		return GetText() ;
	}
	else
	{
		return text ;
	}
}

_bstr_t CSourceAndHtmlEdit::GetText()
{
	TWindow active_window = get_active_view() ;

	if ( active_window == m_html_edit )
	{
		return get_html_text();
	}
	else
	{
		_bstr_t text ;
		m_text_edit.GetText( text ) ;
		return text ;
	}
}
void CSourceAndHtmlEdit::SetText( const _bstr_t &text )
{
	this->ensure_document_complete() ;
	html::CHtmlSelection selection = m_html_edit.get_selection() ;
	html::CHtmlTextRange range = selection.create_text_range() ;
	range.expand(L"Textedit") ;
	range.put_html_text(text) ;

	m_text_edit.SetText( text ) ;
}


void CSourceAndHtmlEdit::set_html_back_color(const wstring color)
{
	html::CHtmlDocument doc = m_html_edit.get_document() ;
	doc.set_bg_color(color) ;

	html::collection_ptr collection = doc.get_all() ;

	CStringW tagbase ;
	tagbase.Format(L"background-color: %ls", color.c_str()) ;
	const _bstr_t bg_style = static_cast< LPCWSTR >( tagbase ) ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		if ( 0 == _wcsicmp(static_cast< LPCWSTR >( element->tagName ), L"TD" ))
		{
			element->style->cssText = bg_style ;
		} 
	}
}

void CSourceAndHtmlEdit::handle_format(void)
{
	if ( ! m_html_edit.IsWindow()	) 
	{
		return ;
	}
	HWND focus = ::GetFocus() ;

	if ( m_html_edit == focus || m_html_edit.IsChild( focus ) ) 
	{
		m_html_edit.OnFormatFont() ;
	}
}

void CSourceAndHtmlEdit::set_html_focus(void)
{
	BANNER("CSourceAndHtmlEdit::set_html_focus") ;
#ifdef UNIT_TEST
	return ;
#endif

	// m_html_edit.SetFocus() ;
	sendMouseClick(getDocumentHWND(), getHwnd()) ;
}

void CSourceAndHtmlEdit::lose_focus(void)
{
	html::CHtmlSelection selection = m_html_edit.get_selection() ;
	html::CHtmlTextRange range = selection.create_text_range() ;
	range.expand(L"Textedit") ;
	range.collapse(true) ;
	range.select() ;

	m_text_edit.SetSel(0,0) ;
}

void CSourceAndHtmlEdit::gain_focus(void)
{
	try
	{
		m_tabs.SetFocus() ;

		//HWND focus_hwnd = m_tabs.GetActiveView(); 
		//::SetFocus(focus_hwnd) ;

		m_html_edit.do_select_all() ;
		m_text_edit.SelectAll() ;
	}
	catch (CException& e)
	{
		logging::log_error("Failed to set focus on editor box") ;
		logging::log_exception(e) ;
		throw ;
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to set focus on editor box") ;
		logging::log_exception(e) ;
		throw ;
	}
}

HWND CSourceAndHtmlEdit::tab_sel_changing(LPNMCTC2ITEMS items)
{
	switch( items->iItem1 ) 
	{
	case TAB_VIEW_WYSIWYG :
		{
			load_html_into_source( ) ;

			return m_text_edit  ;

			break ;
		}
	case TAB_VIEW_SOURCE :
		{
			load_source_into_html( ) ;

			return m_html_edit ;

			break ;
		}

	default :
		return NULL ;
	}
}

void CSourceAndHtmlEdit::load_source_into_html( )
{
	_bstr_t text ;
	m_text_edit.GetText( text ) ;

	ensure_document_complete( ) ;

	CHtmlDocument doc = m_html_edit.get_document() ;
	CHtmlElement body = doc.get_body() ;
	body.put_inner_html( text ) ;
}

void CSourceAndHtmlEdit::load_html_into_source( )
{
	ensure_document_complete( ) ;

	const _bstr_t text = this->get_html_text() ;

	m_text_edit.SetText( text ) ;
}


void CSourceAndHtmlEdit::wrap_selection( LPCSTR tag )
{
	str::buffer buf ;
	m_text_edit.GetSelection( buf ) ;
	string wrapped ;
	wrapped << "<" << tag << ">" << buf.str() << "</" << tag << ">" ;
	m_text_edit.ReplaceSel( wrapped.c_str() ) ;
}


void CSourceAndHtmlEdit::handle_bold(void)
{
	wrap_selection( "strong" ) ;
}

void CSourceAndHtmlEdit::handle_italic(void)
{
	wrap_selection( "em" ) ;
}

void CSourceAndHtmlEdit::handle_underline(void)
{
	wrap_selection( "u" ) ;
}

void CSourceAndHtmlEdit::detach(void)
{
	m_box.Detach() ;
}

void CSourceAndHtmlEdit::handle_return_key(void)
{
	const bool control_is_down = ( ::GetKeyState( VK_CONTROL ) < 0 ) ;
	const bool alt_is_down = ( ::GetKeyState( VK_MENU ) < 0 ) ;

	if ( m_html_edit == get_active_view() ) 
	{
		_bstr_t text ;
		if ( control_is_down ) 
		{
			text = L"<p>" ;
		}
		else
		{
			text = L"<br />" ;
		}
		html::CHtmlSelection selection = m_html_edit.get_selection() ;
		html::CHtmlTextRange range = selection.create_text_range() ;
		range.put_html_text(text) ;
	}
	else
	{
		if ( control_is_down ) 
		{
			m_text_edit.ReplaceSelection( L"<p>" ) ;
		}
		else if (alt_is_down)
		{
			m_text_edit.ReplaceSelection( L"\n" ) ;
		}
		else
		{
			m_text_edit.ReplaceSelection( L"<br />" ) ;
		}
	}
}

void CSourceAndHtmlEdit::SizeBox()
{
	CWindowRect boxrect( m_box ) ;

	m_parent_dlg.ScreenToClient( &boxrect ) ;

	m_box.ShowWindow( SW_HIDE ) ;

	// move the html view into place
	m_tabs.SetWindowPos(NULL, &boxrect, SWP_NOZORDER | SWP_NOACTIVATE);
}


void CSourceAndHtmlEdit::ensure_document_complete()
{
#ifdef UNIT_TEST
	return ;
#endif

	MSG msg = {0};
	while ( m_html_edit.is_document_complete() == false || m_html_edit.is_navigation_complete() == false) 
	{
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{ 
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		} 
	}
}

_bstr_t CSourceAndHtmlEdit::get_html_text()
{
	html::CHtmlSelection selection = m_html_edit.get_selection() ;
	html::CHtmlTextRange range = selection.create_text_range() ;
	range.expand(L"Textedit") ;
	return range.get_html_text() ;
}

void CSourceAndHtmlEdit::sendMouseClick(TWindow bottomwin, TWindow topwin)
{
	CWindowRect rc( topwin ) ;
	POINT pt = {0} ;
	pt.x = rc.left+10 ;
	pt.y = rc.top+10 ;
	ATLASSERT(rc.PtInRect(pt)) ;

	bottomwin.SendMessage(WM_MOUSEACTIVATE, (WPARAM)topwin.m_hWnd, MAKELPARAM( HTCLIENT, WM_LBUTTONDOWN ) );
	bottomwin.PostMessage(WM_MOUSEMOVE, 0, MAKELPARAM( pt.x, pt.y ) );
	bottomwin.PostMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM( pt.x, pt.y ) );
	bottomwin.PostMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM( pt.x, pt.y ) );
}

HWND CSourceAndHtmlEdit::getDocumentHWND()
{
	html::document3_ptr doc = m_html_edit.get_document3() ;
	if( !doc )
	{
		return NULL;
	}

	CComPtr<IOleWindow> docWindow = NULL;
	if( FAILED( doc->QueryInterface( IID_IOleWindow, (void**)&docWindow ) ) )
	{
		return NULL;
	}

	HWND hwnd = NULL;
	docWindow->GetWindow( &hwnd );
	return hwnd;
}

HWND CSourceAndHtmlEdit::getHwnd() const
{
	return m_html_edit.m_hWnd ;
}