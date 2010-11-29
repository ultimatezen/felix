/*!
	@file HtmlView.h
	@brief interface for the CHtmlView class.
	@date 2005/06/25
	Time: 15:22:10
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once


#include "StringEx.h"		// string support
#include "stringconversions.h"

#include "HtmlDocument.h"	// wrappers for html doc & others
#include "destination_parser.h"		// wrapper for browser stuff
#include "mshtmcid.h"		// html commands
#include "Exceptions.h"		// exception support

#include "MessageMapDefines.h"	// for CMD_HANDER_EX et al

#include "atlres.h"	// ID_FORMAT_FONT et al
#include "logging.h"

#ifndef ERROR_CAN_NOT_COMPLETE
#define ERROR_CAN_NOT_COMPLETE _T("Could not complete requested action")
#endif

typedef CComPtr< IDispatch > dispatch_ptr ;

namespace html
{

static const int ID_HTML_CTL_EVENT_ID = 1 ;
static const int ID_HTML_CTL_ID = 2 ;

inline int next_htmlview_id()
{
	static int current_id = ID_HTML_CTL_ID ;
	return current_id++ ;
}

__declspec(selectany) _ATL_FUNC_INFO BeforeNavigate2Info =
{ 	
	CC_STDCALL, 
	VT_EMPTY, 
	7,
	{ 
		VT_DISPATCH, VT_VARIANT|VT_BYREF, VT_VARIANT|VT_BYREF,
			VT_VARIANT|VT_BYREF, VT_VARIANT|VT_BYREF, VT_VARIANT|VT_BYREF,
			VT_BOOL|VT_BYREF 
	}
};

__declspec(selectany) _ATL_FUNC_INFO DocumentCompleteInfo =
{ 	
	CC_STDCALL, 
	VT_EMPTY, 
	2,
	{ 
		VT_DISPATCH, VT_VARIANT|VT_BYREF 
	}
};

__declspec(selectany) _ATL_FUNC_INFO NavigateComplete2Info =
{ 
	CC_STDCALL, 
	VT_EMPTY, 
	2,
	{ 
		VT_DISPATCH, VT_VARIANT|VT_BYREF 
	}
};

__declspec(selectany) _ATL_FUNC_INFO StatusTextChangeInfo =
{ 
	CC_STDCALL, 
	VT_EMPTY, 
	1,
	{ 
		VT_BSTR 
	}
};

__declspec(selectany) _ATL_FUNC_INFO TitleChangeInfo =
{ 
	CC_STDCALL, 
	VT_EMPTY, 
	1,
	{ 
		VT_BSTR 
	}
};

/**
	@class CHtmlViewListener
	@brief Listener interface for CHtmlView.
 */
class __declspec(novtable) CHtmlViewListener
{
public:
	// return true to cancel!
	virtual bool OnBeforeNavigate2( _bstr_t url )
	{
		url ;
		return true ;
	}
	virtual void OnDocumentComplete( _bstr_t url )
	{
		url ;
	}
	virtual void OnNavigateComplete2( _bstr_t url )
	{
		url ;
	}
	virtual void OnStatusTextChange( _bstr_t status_text )
	{
		status_text ;
	}
	virtual void OnTitleChange( _bstr_t title )
	{
		title ;
	}
};

// --
typedef CComPtr< IUnknown >				unknown_ptr ;
typedef CComPtr< MSHTML::IHTMLWindow2 >			window_ptr ;
typedef CComPtr< MSHTML::IHTMLBodyElement >		body_ptr ;

/**
// @class CHtmlView
//
// @brief Event-aware View class for HTML view.
// usage:
//		CHtmlView view ;
//		view.create( m_hWnd ) ;
//		
//		view.set_body_text( L"Hello, world!" ) ;
*/
class CHtmlView 
	:	public CWindowImpl<CHtmlView, CAxWindowT<TWindow> >,
		public IDispEventSimpleImpl< ID_HTML_CTL_EVENT_ID, CHtmlView, &DIID_DWebBrowserEvents2 >
{
public:

	wstring			m_loaded_message ;
	wstring			m_status_text ;
	wstring			m_title_text ;

	destination_parser	m_parser ;
	bool				m_navigation_complete ;
	bool				m_document_complete ;

	bool				m_is_instantiated ;
	bool				m_allow_nav ;

	CHtmlViewListener	*m_listener ;

	int				m_ctl_id ;

	DECLARE_WND_SUPERCLASS(_T("Felix_CustomHtmlView"), CAxWindow::GetWndClassName())

	CHtmlView( bool allow_nav = true ) ;

	void set_listener( CHtmlViewListener *listener )
	{
		ATLASSERT ( listener != NULL ) ; 
		m_listener = listener ;
	}
	void SetFocusToHTML();

	// remember_scroll_pos
	bool set_scroll_pos( long pos ) ;
	window_ptr get_window( ) ;
	body_ptr get_doc_body( ) ;
	element_ptr get_body( ) ;
	// set_parser
	// create
	HWND create( HWND hWnd, const TCHAR *startup_page = TEXT("about:blank"), UINT id=0 ) ;

	// PreTranslateMessage
	BOOL PreTranslateMessage(MSG* pMsg) ;

	bool navigate(const string &url ) ;
	bool navigate(const wstring &url ) ;
	bool write(const wstring &content) ;


	// send_message_to_parent
	bool send_message_to_parent( LPMSG pMsg ) ;
	// LoadedHandler
	void LoadedHandler(const OLECHAR *message ) ;

	// instantiate
	bool instantiate() ;

	// is_instantiated
	bool is_instantiated() ;
	bool is_navigation_complete() ;

	// uninstantiate

	// get_selected_text
	wstring get_selected_text() ;

	// copy_to_clipboard

	// get a smart pointer to IWebBrowser
	browser_ptr get_browser() ;
	// get a smart pointer to IHTMLDocument
	document_ptr get_document( ) ;
	document3_ptr get_document3() ;

	// get a smart pointer to IHTMLSelection
	sel_obj_ptr get_selection( ) ;

private:
	// get a smart pointer to IDispatch
	dispatch_ptr get_document_dispatch() ;
	// get a smart pointer to IPersistStreamInit
	
public:
	LRESULT OnFilePrint();
	LRESULT OnFilePageSetup() ;
	LRESULT OnFilePrintPreview() ;

	LRESULT on_file_save() ;
	LRESULT on_file_save_as() ;

	bool is_document_complete();
	void set_body_text(const _bstr_t &text);
	bool navigate( const _bstr_t &url );
	HWND GetDlgItem( int nID ) const;
	wstring get_body_text();
	void set_body_text( const wstring &text );
	void load_from_resource( const CString &resource_name );
	command_target_ptr get_command_target();
	bool get_edit_mode();
	void put_edit_mode( const bool mode );
	void ensure_document_complete();

	// COMMAND HANDLERS

	LRESULT on_create(UINT uMsg, WPARAM wParam, LPARAM lParam) ;
	LRESULT on_destroy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void uninstantiate();
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnEditDelete( ) ;
	LRESULT OnEditSelectAll( ) ;
	LRESULT OnEditCopy( ) ;
	LRESULT OnEditCut( ) ;
	LRESULT OnEditPaste( ) ;
	LRESULT OnEditUndo( ) ;
	LRESULT OnEditRedo( ) ;
	LRESULT OnFormatFont( ) ;

	void do_delete() ;
	void do_bold() ;
	void do_italic() ;
	void do_underline() ;
	void do_select_all();

	// ================================================
	// Event implementations for IDocHostUIHandlerDispatch
	// ================================================


	// Function name	: DocumentComplete
	// Description	    : 
	// Return type		: void __stdcall 
	// Argument         :  IDispatch *pDisp
	// Argument         : VARIANT *URL
	void __stdcall DocumentComplete( IDispatch * /*pDisp*/, VARIANT *URL );

	// Function name	: BeforeNavigate2
	// Description	    : 
	// Return type		: void __stdcall 
	void __stdcall BeforeNavigate2(          
		IDispatch * /*pDisp*/,
		VARIANT *URL,
		VARIANT * /*Flags*/,
		VARIANT * /*TargetFrameName*/,
		VARIANT * /*PostData*/,
		VARIANT * /*Headers*/,
		VARIANT_BOOL *Cancel
		);

	// Function name	: NavigateComplete2
	// Description	    : 
	// Return type		: void __stdcall 
	// Argument         : const _bstr_t url
	void __stdcall NavigateComplete2( IDispatch * /*pDisp*/, VARIANT *URL );

	// Function name	: StatusTextChange
	// Description	    : 
	// Return type		: void __stdcall 
	// Argument         : const _bstr_t text
	void __stdcall StatusTextChange( BSTR text);

	// Function name	: TitleChange
	// Description	    : 
	// Return type		: void __stdcall 
	// Argument         : const _bstr_t title
	void __stdcall TitleChange( BSTR title);

	BEGIN_MSG_MAP_EX(CHtmlView)
		// don't use MSG_HANDLER_0
		// we need the arguments to pass to DefWindowProc
		MESSAGE_HANDLER_EX(WM_CREATE, on_create) 
		MESSAGE_HANDLER_EX(WM_DESTROY, on_destroy) 
		MESSAGE_HANDLER_EX(WM_SETFOCUS, OnSetFocus) 
		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_0( ID_FORMAT_FONT,		OnFormatFont	 )
			CMD_HANDLER_EX_0( ID_EDIT_UNDO,			OnEditUndo     )
			CMD_HANDLER_EX_0( ID_EDIT_REDO,			OnEditRedo     )

			CMD_HANDLER_EX_0( ID_EDIT_CLEAR,		OnEditDelete   )
			CMD_HANDLER_EX_0( ID_EDIT_COPY,			OnEditCopy     )
			CMD_HANDLER_EX_0( ID_EDIT_CUT,			OnEditCut		 )
			CMD_HANDLER_EX_0( ID_EDIT_PASTE,		OnEditPaste    )
			CMD_HANDLER_EX_0( ID_EDIT_SELECT_ALL,	OnEditSelectAll )
			return FALSE ;
		END_CMD_HANDLER_EX
			DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
		
	BEGIN_SINK_MAP( CHtmlView )
		SINK_ENTRY_INFO( ID_HTML_CTL_EVENT_ID, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2,	  BeforeNavigate2,   &BeforeNavigate2Info )		// hyperlink clicked on
		SINK_ENTRY_INFO( ID_HTML_CTL_EVENT_ID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE,  DocumentComplete,  &DocumentCompleteInfo )		// hyperlink clicked on
		SINK_ENTRY_INFO( ID_HTML_CTL_EVENT_ID, DIID_DWebBrowserEvents2, DISPID_TITLECHANGE,		  TitleChange,		 &TitleChangeInfo ) 
		SINK_ENTRY_INFO( ID_HTML_CTL_EVENT_ID, DIID_DWebBrowserEvents2, DISPID_STATUSTEXTCHANGE,  StatusTextChange,  &StatusTextChangeInfo )	
		SINK_ENTRY_INFO( ID_HTML_CTL_EVENT_ID, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, NavigateComplete2, &NavigateComplete2Info )	// UIActivate new document
	END_SINK_MAP()
};

} ;
