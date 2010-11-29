/*!
	@file HtmlEditView.h
	@brief interface of the CTestHtmlEditViewView class.
	@date 2005/06/25
	Time: 15:21:56
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#include "HtmlDocument.h"	// CHtmlDocument
				// CHtmlSelection
				// CHtmlTextRange
				// et al


#include <atlcom.h>		// IDispEventImpl
#include <atlhost.h>		// hosting activex controls

#include "Exceptions.h"
#include "StringEx.h"

#include "MessageMapDefines.h"	// for CMD_HANDER_EX et al

#pragma warning( disable : 4192 ) // automatically excluding 'name' while importing type library 'library'
#import "C:\Program Files (x86)\Common Files\microsoft shared\DhtmlEd\DHTMLED.OCX" \
	exclude("OLECMDEXECOPT") named_guids
#pragma warning( default : 4192 )

namespace html
{

	inline int get_unique_id()
	{
		static int i = 0 ;
		++i ;
		return i ;
	}

	#define ID_HTML_EDIT_EVENT_ID 1

	enum 
	{ 	
		ON_DOCUMENTCOMPLETE = 0x1,
		ON_DISPLAYCHANGED ,
		ON_SHOWCONTEXTMENU ,
		ON_CONTEXTMENUSELECTION ,
		ON_MOUSEDOWN,
		ON_MOUSEMOVE,
		ON_MOUSEUP,
		ON_MOUSEOUT,
		ON_MOUSEOVER,
		ON_READYSTATECHANGE,
		ON_CLICK,
		ON_DOUBLECLICK,
		ON_KEYDOWN,
		ON_KEYPRESS,
		ON_KEYUP,
		ON_BLUR
	} ;



	/**
		@class CHtmlEditListener
		@brief Listener for CHtmlEdit events.
	 */
	class __declspec(novtable) CHtmlEditListener
	{
	public:
		virtual bool HtmlEditOnDocumentComplete ()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnDisplayChanged ()
		{
			return true ;
		}

		virtual bool HtmlEditOnShowContextMenu( int x, int y )
		{
			x ;
			y ;

			return true ;
		}

		virtual bool HtmlEditOnMouseDown()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnMouseMove()
		{
			return true ;
		}

		virtual bool HtmlEditOnMouseUp()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnMouseOut()
		{
			return true ;
		}

		virtual bool HtmlEditOnMouseOver()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnReadyStateChange()
		{
			return true ;
		}

		virtual bool HtmlEditOnClick()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnDoubleClick()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnKeyDown()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnKeyPress()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnKeyUp()
		{
			return true ;
		}
		
		virtual bool HtmlEditOnBlur()
		{
			return true ;
		}
	};

	/**
		@class CHtmlEditView  
		@brief Host for DHTML Editing Control ActiveX object.
		<b>Usage:</b>
		Use like other views. E.g. m_view in mainframe, or pane in splitter view.
		Make sure project supports ActiveX hosting.

		<b>Notes:</b>
		<ol>
		<li>Object is not safe for scripting version -- allows Browse mode etc.
		<li>The caller is responsible to make sure that the ActiveX control is not busy, etc. before calling load_url(), etc.
		</ol>
	 */
	class CHtmlEditView : 
	   public CWindowImpl<CHtmlEditView, CAxWindowT<TWindow> >,
	   public IDispEventImpl<ID_HTML_EDIT_EVENT_ID, CHtmlEditView>
	{

		bool m_dirty ;

	public:

		// various typedefs
		typedef CComQIPtr< DHTMLEDLib::IDHTMLEdit >		edit_ptr ;
		typedef CComPtr< IDispatch >					dispatch_ptr ;
		
		CHtmlEditListener	*m_listener ;
		int	m_unique_id;

		void set_listener( CHtmlEditListener *listener )
		{
			ATLASSERT ( listener != NULL ) ; 
			
			m_listener = listener ;
		}
		void wait_until_not_busy();
		_bstr_t get_back_color();
		_bstr_t get_fore_color();
		void set_block_format( const CString &block_format )
		{
			set_block_format( _variant_t( (LPCTSTR)block_format ) ) ;
		}
		void set_block_format( const wstring &block_format );
		void set_block_format( const _bstr_t &block_format ) ;
		void set_block_format( const _variant_t &block_format ) ;
		_bstr_t get_block_format();
		DHTMLEDLib::DHTMLEDITCMDF get_unord_list();
		DHTMLEDLib::DHTMLEDITCMDF get_ord_list();
		DHTMLEDLib::DHTMLEDITCMDF get_right();
		DHTMLEDLib::DHTMLEDITCMDF get_left();
		DHTMLEDLib::DHTMLEDITCMDF get_center();
		DHTMLEDLib::DHTMLEDITCMDF get_underline();
		DHTMLEDLib::DHTMLEDITCMDF get_italic();
		DHTMLEDLib::DHTMLEDITCMDF get_bold();
		int get_font_size();
		_bstr_t get_font_name();
		void get_text( BSTR *text );

		CHtmlEditView( CHtmlEditListener *listener = NULL, int unique_id = get_unique_id() ) : 
			m_unique_id( unique_id ),
			m_listener( listener ),
			m_dirty(false)
		{
			BOOL success(FALSE) ;
			success = AtlAxWinInit() ;
			ATLASSERT( success ) ;
		}
		
		void set_selection( unsigned int start, unsigned int end );
		void select_all();
		void insert_text( const wstring &text );
		void insert_hyperlink();
		void insert_image( const wstring &file_name = L"" );
		void insert_table( int num_rows, 
				int num_cols, 
				const wstring &table_attr = L"", 
				const wstring &cell_attr = L"", 
				const wstring &title = L"");
		void do_unordered_list();
		void do_ordered_list();
		const _bstr_t get_location();
		event_obj_ptr get_last_event();
		
		bool get_browse_mode();
		void set_base_url( const _bstr_t &base_url ) ;
		void set_base_url( const wstring &base_url );
		void show_glyphs( bool setting );


		DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

		// -----------------------------------

		HWND Create(HWND hWndParent, RECT& rcPos = rcDefault, DWORD dwFlags = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ) ;

		// In order for AtlAdviseSinkMap() to work, we need this overload!?
		HWND GetDlgItem(int nID ) const ;

		// --------------------------------------

		BOOL PreTranslateMessage(MSG* pMsg) ;

		// --------------------------------------

		LRESULT on_create(UINT uMsg, WPARAM wParam, LPARAM lParam ) ;
		edit_ptr get_edit() ;
		void init_control() ;

		virtual ~CHtmlEditView()
		{
#ifndef UNIT_TEST
			ATLASSERT ( ! IsWindow() ) ; 
#endif
		}

		// ======================
		// formatting stuff
		// ======================

		void show_font_dialog() ;
		void set_font_name( const CString &font_name ) ;
		void set_font_name( const wstring &font_name ) ;
		void set_font_name( const _bstr_t &font_name ) ;
		void set_font_name( const _variant_t &font_name ) ;
		void set_font_size( int size ) ;
		void set_back_color( const wstring &color ) ;
		void set_fore_color( const wstring &color ) ;
		void set_bold( bool setting ) ;
		void set_italic( bool setting ) ;
		void set_underline( bool setting ) ;


		// ----------------------------
		// access control content
		// ----------------------------

		document_ptr get_document() ;
		window_ptr get_parent_window( ) ;
		void print() ;
		element_ptr get_body() ;
		_bstr_t get_text() ;
		_bstr_t get_title() ;
		void set_title( const wstring &title ) ;
		_bstr_t filter_text( const wstring &text ) ;
		_bstr_t filter_text( const _bstr_t &text ) ;
		_bstr_t get_body_text() ;

		// set_text
		void set_text( const wstring &text ) ;
		void set_text( const _bstr_t &bstr) ;

		// load_url
		void load_url( const wstring &url ) ;
		// load_document
		void load_document( const wstring &file_name ) ;
		void load_document( const _bstr_t &file_name ) ;
		// save document
		void save_document( const wstring &file_name ) 
		{
			_variant_t v( file_name.c_str() ) ;
			v.vt = VT_BSTR ;
			
			save_document( v )	;
		}
		void save_document( _variant_t &file_name ) ;
		// set_browse_mode
		void set_browse_mode( bool setting ) ;

		// --------------------------------------
		// events
		// --------------------------------------

		VOID __stdcall OnDisplayChanged() ;
		VOID __stdcall OnShowContextMenu( int x, int y ) ;
		VOID __stdcall OnDocumentComplete( ) ;
		VOID __stdcall OnMouseDown() ;
		VOID __stdcall OnMouseMove () ;
		VOID __stdcall OnMouseUp() ;
		VOID __stdcall OnMouseOut() ;
		VOID __stdcall OnMouseOver() ;
		VOID __stdcall OnReadyStateChange() ;
		VOID __stdcall OnClick() ;
		VOID __stdcall OnDoubleClick() ;
		VOID __stdcall OnKeyDown() ;
		VOID __stdcall OnKeyPress() ;
		VOID __stdcall OnKeyUp() ;
		VOID __stdcall OnBlur() ;
		// =========================
		// helpers for control interface
		// =========================
		_variant_t ExecCommand( DHTMLEDLib::DHTMLEDITCMDID cmd_id, OLECMDEXECOPT option = OLECMDEXECOPT_DONTPROMPTUSER, _variant_t vIn = _variant_t() ) ;
		DHTMLEDLib::DHTMLEDITCMDF QueryStatus( DHTMLEDLib::DHTMLEDITCMDID status = DHTMLEDLib::DECMD_FONT ) ;
		// =========================
		// MESSAGE HANDLERS
		// =========================

		// various edit operations
		LRESULT OnFormatFont( ) ;
		LRESULT OnEditUndo( ) ;
		LRESULT OnEditRedo( ) ;
		LRESULT OnEditFind( ) ;
		LRESULT OnEditCopy( ) ;
		LRESULT OnEditCut( ) ;
		LRESULT OnEditPaste( ) ;
		LRESULT OnEditDelete( ) ;
		LRESULT OnEditSelectAll( ) ;

		// --------------------------------
		void new_document() ;
		bool is_dirty() ;
		void set_dirty_off() ;
		void set_dirty_on() ;
		bool is_busy() ; 
		sel_obj_ptr get_selected_object() ;
		text_range_ptr get_selected_text_range( ) ;
		tstring code2tstring( int code ) ;

		BEGIN_MSG_MAP_EX(CHtmlEditView)
			// don't use MSG_HANDLER_0
			// we need the arguments to pass to DefWindowProc
			MESSAGE_HANDLER_EX(WM_CREATE, on_create) 
			//MSG_WM_DESTROY( OnDestroy )

			BEGIN_CMD_HANDLER_EX
				CMD_HANDLER_EX_0(ID_FORMAT_FONT,		OnFormatFont    )
				CMD_HANDLER_EX_0(ID_EDIT_UNDO,			OnEditUndo      )
				CMD_HANDLER_EX_0(ID_EDIT_REDO,			OnEditRedo      )
				CMD_HANDLER_EX_0(ID_EDIT_FIND,			OnEditFind      )
				CMD_HANDLER_EX_0(ID_EDIT_CLEAR,			OnEditDelete    )
				CMD_HANDLER_EX_0(ID_EDIT_COPY,			OnEditCopy      )
				CMD_HANDLER_EX_0(ID_EDIT_CUT,			OnEditCut       )
				CMD_HANDLER_EX_0(ID_EDIT_PASTE,			OnEditPaste     )
				CMD_HANDLER_EX_0(ID_EDIT_SELECT_ALL,	OnEditSelectAll )
			END_CMD_HANDLER_EX

		END_MSG_MAP()

		BEGIN_SINK_MAP(CHtmlEditView)
			// Sink events from the default ActiveX interface
			//Make sure the Event Handlers have VOID __stdcall calling convention
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_DOCUMENTCOMPLETE, OnDocumentComplete )
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_DISPLAYCHANGED, OnDisplayChanged )
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_SHOWCONTEXTMENU, OnShowContextMenu )
	//		SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_CONTEXTMENUSELECTION, OnContextMenuSelection )
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_MOUSEDOWN, OnMouseDown)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_MOUSEMOVE, OnMouseMove)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_MOUSEUP, OnMouseUp)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_MOUSEOUT, OnMouseOut)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_MOUSEOVER, OnMouseOver)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_READYSTATECHANGE, OnReadyStateChange)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_CLICK, OnClick)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_DOUBLECLICK, OnDoubleClick)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_KEYDOWN, OnKeyDown)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_KEYPRESS, OnKeyPress)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_KEYUP, OnKeyUp)
			SINK_ENTRY(ID_HTML_EDIT_EVENT_ID, ON_BLUR, OnBlur)
		END_SINK_MAP()
	} ;


}
