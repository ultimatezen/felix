#pragma once



#include "CustomTabCtrl.h"
#include "DotNetTabCtrl.h"
#include "TabbedFrame.h"

#include "HtmlView.h"		// CHtmlView
#include "ScintillaEdit.h"	// CScintillaEdit

#include "InputSymbolDlg.h"	// CInputSymbolDlg

class CSourceAndHtmlEdit
{
	enum { TAB_VIEW_WYSIWYG, TAB_VIEW_SOURCE } ;
	enum { SCINTILLA_MARGIN_LINENUM, SCINTILLA_MARGIN_SELECTION, SCINTILLA_MARGIN_FOLD };

	typedef CTabbedChildWindow< CDotNetTabCtrl<CTabViewTabItem> > tab_member ;

public:
	DECLARE_SENSING_VAR ;

	CStaticT<CWindow>		m_box ;
	tab_member				m_tabs ;

	html::CHtmlView		m_html_edit  ;
	CScintillaEdit		m_text_edit ;

	CWindow				m_parent_dlg ;

	CSourceAndHtmlEdit(void);
	~CSourceAndHtmlEdit(void);

	void SizeBox();
	bool has_focus(HWND focus_hwnd) const;
	HWND get_active_view(void) const;
	HWND create(CWindow box_window, CWindow dlg_window, HWND top_pos );
	BOOL pre_translate(MSG* pMsg);

	void ensure_document_complete();


	_bstr_t GetText() ;
	_bstr_t GetSelectionText();

	_bstr_t get_html_text();
	void SetText( const _bstr_t &text ) ;

	_bstr_t get_selected_text(const bool all_if_none=true) ;

	void handle_size(void);
	void insert_symbol(void);
	void set_html_back_color(const wstring color);
	void handle_format(void);
	void set_html_focus(void);
	void lose_focus(void);
	void gain_focus(void);
	HWND tab_sel_changing(LPNMCTC2ITEMS items);

	void wrap_selection( LPCSTR tag );

	void load_html_into_source( );
	void load_source_into_html( );

	void handle_delete(void);
	void handle_bold(void);
	void handle_italic(void);
	void handle_underline(void);
	void detach(void);
	void handle_return_key(void);

	void sendMouseClick(CWindow bottomwin, CWindow topwin);
	HWND getDocumentHWND();
	HWND getHwnd() const;
};
