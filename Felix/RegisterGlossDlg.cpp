// RegisterGlossDlg.cpp: implementation of the RegisterGlossDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "RegisterGlossDlg.h"
#include "EditTransRecordDialogRegGloss.h"

#include "UserStringDlg.h"

#include "background_processor.h"
#include "resource_string.h"
#include "record_local.h"

#include "reg_msg_filter.h"

#include "xpmenu/Tools.h"
#include "RegSettings.h"	// CWindowSettings & CReBarSettings

#define REG_DLG_WINSETTINGS_KEY _T("RegGlossDialog")

using namespace mem_engine ;
using namespace except ;

wstring normalize_spaces(const wstring before)
{
	// normalize whitespace
	std::vector<wstring> tokens ;
	boost::split(tokens, before, boost::is_space(), boost::token_compress_on) ;
	return boost::join(tokens, L" ") ;
}

wstring trim_and_normalize( const _bstr_t before )
{
	return trim_text(normalize_spaces(BSTR2wstring(before))) ;
}
wstring trim_text(const wstring before)
{
	if (before.empty())
	{
		return wstring() ;
	}

	wstring text = before ;
	for (size_t i = text.size()-1 ; ! text.empty() ; --i)
	{
		if (text[i] == L'>')
		{
			while (text[i] != L'<')
			{
				--i ;
			}
			if (! i)
			{
				break ;
			}
		}
		else if (! iswspace(text[i]))
		{
			break ;
		}
		else
		{
			text.erase(i, 1) ;
		}
	}
	wstring out ;
	for (size_t i = 0 ; i < text.size() ; ++i)
	{
		if (text[i] == L'<')
		{
			while (i < text.size() && text[i] != L'>')
			{
				out += text[i] ;
				++i ;
			}
			ATLASSERT(text[i] == L'>') ;
			out += text[i] ;
		}
		else if (! iswspace(text[i]))
		{
			return out + text.substr(i) ;
		}
	}

	return wstring() ;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegisterGlossDlg::CRegisterGlossDlg() :
m_current_add_pos(0),
m_make_defaults(false)
{
	m_mem_record = record_pointer(new record_local()) ;
	m_gloss_record = record_pointer(new record_local()) ;
	m_accel.LoadAccelerators(IDC_EDIT_ACCELERATORS) ;
}

// message handlers
LRESULT CRegisterGlossDlg::OnSize(UINT type, CSize size)
{
	SENSE("OnSize") ;
	size ;

	const int PaddingConst = 7 ;

	if(type == SIZE_MINIMIZED)
	{
		SENSE("SIZE_MINIMIZED") ;
		return 0L ;
	}

	CClientRect client_rect(*this) ;

	this->InvalidateRect(&client_rect) ;
	// pad rectangle
	pad_rectangle(client_rect, PaddingConst);

	//////////////////////////////////////////////////////////////////////////
	// bottom buttons
	//////////////////////////////////////////////////////////////////////////

	resize_close_button(client_rect);
	resize_client_rect_close(client_rect, PaddingConst);

	resize_register_button(client_rect);

	resize_advanced_button(client_rect);
	resize_client_rect_advanced(client_rect, PaddingConst);

	// add combo
	const CWindowRect add_combobox_rect(m_AddCombo) ;
	CRect new_add_combobox_rect(client_rect) ;

	new_add_combobox_rect.top = new_add_combobox_rect.bottom - add_combobox_rect.Height() ;
	const int add_combo_width = add_combobox_rect.Width() ;
	new_add_combobox_rect.right = new_add_combobox_rect.right - PaddingConst ;
	new_add_combobox_rect.left = new_add_combobox_rect.right - add_combo_width ;
	m_AddCombo.MoveWindow(&new_add_combobox_rect, TRUE) ;
	m_AddCombo.ShowWindow(SW_SHOW) ;
	m_AddCombo.InvalidateRect(&new_add_combobox_rect) ;

	CWindowRect add_static_rect(m_AddStatic) ;
	const int add_static_width = add_static_rect.Width() ;
	add_static_rect.top = new_add_combobox_rect.top ;
	add_static_rect.right = new_add_combobox_rect.left - PaddingConst ;
	add_static_rect.left = add_static_rect.right - add_static_width ;
	add_static_rect.bottom = new_add_combobox_rect.bottom ;

	m_AddStatic.MoveWindow(&add_static_rect, TRUE) ;
	m_AddStatic.ShowWindow(SW_SHOW) ;
	m_AddStatic.InvalidateRect(&add_static_rect) ;

	CWindowRect add_combo_rect(m_AddCombo) ;
	client_rect.bottom -= (add_combo_rect.Height() + PaddingConst) ;

	//////////////////////////////////////////////////////////////////////////
	// Boxes
	//////////////////////////////////////////////////////////////////////////

	int VertMidPoint = client_rect.Width() / 2 ;
	int HorMidPoint = client_rect.Height() / 2 ;

	CWindowRect AddSourceRect(m_AddSourceBtn) ;

	int CenterPadding = (AddSourceRect.Width() / 2) + PaddingConst ;

	// right and left columns
	CRect LeftColumnRect(client_rect) ;
	CRect RightColumnRect(client_rect) ;

	LeftColumnRect.right = LeftColumnRect.left + VertMidPoint ;
	LeftColumnRect.right -= CenterPadding ;

	RightColumnRect.left = RightColumnRect.right - VertMidPoint ;
	RightColumnRect.left += CenterPadding ;

	// four corners

	// left
	CRect UpperLeftRect(LeftColumnRect) ;
	CRect LowerLeftRect(LeftColumnRect) ;

	UpperLeftRect.bottom = UpperLeftRect.top + HorMidPoint ;
	UpperLeftRect.bottom -= 3 ;

	LowerLeftRect.top = LowerLeftRect.bottom - HorMidPoint ;
	LowerLeftRect.bottom -= 3 ;

	// right
	CRect UpperRightRect(RightColumnRect) ;
	CRect LowerRightRect(RightColumnRect) ;

	UpperRightRect.bottom = UpperRightRect.top + HorMidPoint ;
	UpperRightRect.bottom -= 3 ;

	LowerRightRect.top = LowerRightRect.bottom - HorMidPoint ;
	LowerRightRect.bottom -= 3 ;

	//////////////////////////////////////////////////////////////////////////
	// Source
	//////////////////////////////////////////////////////////////////////////

	// source labels
	CWindowRect SourceLabelRect(m_SourceLabel) ;
	int LabelHeight = SourceLabelRect.Height() ;
	
	CRect NewSourceLabelRect(UpperLeftRect) ;
	CRect NewSourceGlossLabelRect(UpperRightRect) ;

	NewSourceLabelRect.bottom = NewSourceLabelRect.top + LabelHeight ;
	NewSourceGlossLabelRect.bottom = NewSourceGlossLabelRect.top + LabelHeight ;

	m_SourceLabel.MoveWindow(&NewSourceLabelRect, TRUE) ;
	m_GlossSourceLabel.MoveWindow(&NewSourceGlossLabelRect, TRUE) ;

	// offset the label heights
	UpperLeftRect.top += LabelHeight + 3 ;
	UpperRightRect.top += LabelHeight + 3 ;

	// source boxes
	m_SourceBox.MoveWindow(&UpperLeftRect, TRUE) ;
	m_GlossSourceBox.MoveWindow(&UpperRightRect, TRUE) ;

	// add source button

	int UpperLeftRectCenter = UpperLeftRect.Height() / 2 ;

	CRect NewAddSourceRect(UpperLeftRect) ;

	NewAddSourceRect.top += UpperLeftRectCenter ;
	NewAddSourceRect.top -= AddButtonHeight / 2 ;

	NewAddSourceRect.bottom -= UpperLeftRectCenter ;
	NewAddSourceRect.bottom += AddButtonHeight / 2 ;

	NewAddSourceRect.left = UpperLeftRect.right + PaddingConst ;
	NewAddSourceRect.right = UpperRightRect.left - PaddingConst ;

	m_AddSourceBtn.MoveWindow(&NewAddSourceRect, TRUE) ;
	m_AddSourceBtn.ShowWindow(SW_SHOW) ;
	//////////////////////////////////////////////////////////////////////////
	// Trans
	//////////////////////////////////////////////////////////////////////////

	// Trans labels
	CWindowRect TransLabelRect(m_TransLabel) ;
	int TransLabelHeight = TransLabelRect.Height() ;

	CRect NewTransLabelRect(LowerLeftRect) ;
	CRect NewTransGlossLabelRect(LowerRightRect) ;

	NewTransLabelRect.bottom = NewTransLabelRect.top + TransLabelHeight ;
	NewTransGlossLabelRect.bottom = NewTransGlossLabelRect.top + TransLabelHeight ;

	m_TransLabel.MoveWindow(&NewTransLabelRect, TRUE) ;
	m_GlossTransLabel.MoveWindow(&NewTransGlossLabelRect, TRUE) ;

	// offset the label heights
	LowerLeftRect.top += TransLabelHeight + 3 ;
	LowerRightRect.top += TransLabelHeight + 3 ;

	// Trans boxes
	m_TransBox.MoveWindow(&LowerLeftRect, TRUE) ;
	m_GlossTransBox.MoveWindow(&LowerRightRect, TRUE) ;

	// add trans button
	CWindowRect AddTransRect(m_AddTransBtn) ;

	CRect NewAddTransRect(LowerLeftRect) ;
	int LowerLeftRectCenter = LowerLeftRect.Height() / 2 ;

	NewAddTransRect.top += LowerLeftRectCenter ;
	NewAddTransRect.top -= AddButtonHeight / 2 ;

	NewAddTransRect.bottom -= LowerLeftRectCenter ;
	NewAddTransRect.bottom += AddButtonHeight / 2 ;

	NewAddTransRect.left = LowerLeftRect.right + PaddingConst ;
	NewAddTransRect.right = LowerRightRect.left - PaddingConst ;

	m_AddTransBtn.MoveWindow(&NewAddTransRect, TRUE) ;
	m_AddTransBtn.ShowWindow(SW_SHOW) ;

	// get its position
	SizeBoxes();

	return 0L ;
}

void CRegisterGlossDlg::SizeBoxes()
{
	m_rec_source_edit.SizeBox() ;
	m_rec_trans_edit.SizeBox() ;
	m_gloss_source_edit.SizeBox() ;
	m_gloss_trans_edit.SizeBox()  ;
}



// Function name	: CRegisterGlossDlg::OnInitDialog
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnInitDialog()
{
	SENSE("OnInitDialog") ;

	ATLASSERT(IsWindow()) ;
	
	// center us on our parent
	CenterWindow(GetParent());

	// set icon
	SetIcon(LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE) ;

#ifdef UNIT_TEST
	return TRUE ;
#else

	register_message_filter(this) ;

	m_SourceLabel.Attach(GetDlgItem(IDC_SOURCE_STATIC)) ;
	m_TransLabel.Attach(GetDlgItem(IDC_TRANS_STATIC)) ;
	m_GlossSourceLabel.Attach(GetDlgItem(IDC_SOURCE_GLOSS_STATIC)) ;
	m_GlossTransLabel.Attach(GetDlgItem(IDC_TRANS_GLOSS_STATIC)) ;

	m_AddSourceBtn.Attach(GetDlgItem(IDC_ADD_SOURCE)) ;
	m_AddTransBtn.Attach(GetDlgItem(IDC_ADD_TRANS)) ;
	m_AdvancedBtn.Attach(GetDlgItem(IDC_ADVANCED)) ;
	m_RegisterBtn.Attach(GetDlgItem(IDC_REGISTER)) ;
	m_CloseBtn.Attach(GetDlgItem(IDCLOSE)) ;

	m_AddStatic.Attach(GetDlgItem(IDC_ADD_STATIC)) ;
	m_AddCombo.Attach(GetDlgItem(IDC_ADD_TO_COMBO)) ;
	set_combo_font();
	fill_add_combo();

	CWindowRect AddButtonRect(m_AddSourceBtn) ;
	AddButtonHeight = AddButtonRect.Height() ;

	// =========
	// rec source
	// =========
	m_SourceBox.Attach(GetDlgItem(IDC_SOURCE_BOX)) ;
	m_current_view = m_rec_source_edit.create(m_SourceBox, 
												*this, 
												HWND_TOP) ;

	m_rec_source_edit.SetText(string2BSTR(m_mem_record->get_source_rich())) ;
	ATLASSERT(m_current_view.IsWindow()) ;

	// =========
	// rec trans
	// =========
	
	m_TransBox.Attach(GetDlgItem(IDC_TRANS_BOX)) ;
	CWindow rec_trans_window = m_rec_trans_edit.create(m_TransBox, 
														*this, 
														m_current_view);
	m_rec_trans_edit.SetText(string2BSTR(m_mem_record->get_trans_rich())) ;
	ATLASSERT(rec_trans_window.IsWindow()) ;
	// =========
	// gloss source
	// =========

	m_GlossSourceBox.Attach(GetDlgItem(IDC_SOURCE_GLOSS_BOX)) ;
	CWindow gloss_source_window = m_gloss_source_edit.create(m_GlossSourceBox, 
															*this, 
															rec_trans_window) ;
	ATLASSERT(gloss_source_window.IsWindow()) ;
	// =========
	// gloss trans
	// =========

	m_GlossTransBox.Attach(GetDlgItem(IDC_TRANS_GLOSS_BOX)) ;
	CWindow gloss_trans_window = m_gloss_trans_edit.create(m_GlossTransBox, 
															*this, 
															gloss_source_window) ;
	ATLASSERT(gloss_trans_window.IsWindow()) ;

	// set focus to source edit
	m_rec_source_edit.set_html_focus() ;

	apply_window_settings();


	return TRUE ;
#endif
}


// Function name	: CRegisterGlossDlg::OnCancel
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnCancel()
{
	SENSE("OnCancel") ;
	ShowWindow(SW_HIDE) ;
	return 0;
}

// Function name	: CRegisterGlossDlg::on_close
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnClose()
{
	SENSE("OnClose") ;
	refresh_current_add_pos();
	ShowWindow(SW_HIDE) ;
	return 0;
}

// Function name	: CRegisterGlossDlg::OnOK
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnOK()
{
	BANNER("CRegisterGlossDlg::OnOK") ;

	ATLTRACE("CRegisterGlossDlg::OnOK\n") ;
	add_gloss_entry();

	return 0;
}


// Function name	: CRegisterGlossDlg::PreTranslateMessage
// Description	    : 
// Return type		: BOOL 
// Argument         : MSG* pMsg
BOOL CRegisterGlossDlg::PreTranslateMessage(MSG* pMsg)
{
	// no banner, because we would be swamped with trace statements!

	ENSURE_ACTIVE

	// intercept enter key in order to shut down dialog even when
	// the DHTML edit controls have focus
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			process_return_key() ;
			return TRUE ;
		case VK_ESCAPE:
			ATLASSERT(IsWindow()) ;
			::SendMessage(m_hWnd, WM_COMMAND, IDCANCEL, 0) ;
			return TRUE ;
		case VK_TAB:
			if (process_tab_key())
				return TRUE ;
		}	
	}

	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL && active_edit->pre_translate(pMsg)) 
	{
			return TRUE ;
	}

	if(! m_accel.IsNull() && m_accel.TranslateAccelerator(*this, pMsg))
	{
		return TRUE;
	}
	
	if(CAxDialogImpl<CRegisterGlossDlg, CWindow>::IsDialogMessage(pMsg))
		return TRUE ;

	return FALSE ;
}

// Function name	: CRegisterGlossDlg::OnFormatBackgroundColor
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnFormatBackgroundColor()
{
	BANNER("CRegisterGlossDlg::OnFormatBackgroundColor") ;

	if (! this->IsWindowVisible())
	{
		return 0L ;
	}

	CColorDialog dialog(RGB(255,255,255)) ; // current color is default

	if (dialog.DoModal() == IDCANCEL)
		return 0 ;

	// get the color the user picked
	CColorRef color(dialog.GetColor()) ;
	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL) 
	{
		active_edit->set_html_back_color(color.as_wstring()) ;
	}

	return 0L ;
}


// Function name	: CRegisterGlossDlg::OnInsertSymbol
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnInsertSymbol()
{
	BANNER("CRegisterGlossDlg::OnInsertSymbol") ;

	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL) 
	{
		active_edit->insert_symbol() ;
	}

	return 0L ;
}


// Function name	: CRegisterGlossDlg::on_destroy
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnDestroy()
{
	BANNER("CRegisterGlossDlg::OnDestroy") ;

	CWindowSettings ws;
	ws.GetFrom(*this);
	ws.Save( resource_string(IDS_REG_KEY), REG_DLG_WINSETTINGS_KEY );

	// These are just wrappers for the control -- they
	// won't get a WM_DESTROY message, so the HWND will be non-null
	// when the object is destroyed, creating an assert.
	// Go ahead and set the HWNDs to NULL here
	m_rec_source_edit.detach() ;
	m_rec_trans_edit.detach() ;
	m_gloss_source_edit.detach() ;
	m_gloss_trans_edit.detach() ;

	_Module.GetMessageLoop()->RemoveMessageFilter(this);
	
	return 0;
}


// Function name	: CRegisterGlossDlg::OnFont
// Description	    : 
// Return type		: LRESULT 
LRESULT CRegisterGlossDlg::OnFont()
{
	BANNER("CRegisterGlossDlg::OnFont") ;

	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL) 
	{
		active_edit->handle_format() ;
	}

	return 0 ;
}

// Function name	: CRegisterGlossDlg::process_return_key
// Description	    : 
// Return type		: void 
void CRegisterGlossDlg::process_return_key()
{
	BANNER("CRegisterGlossDlg::process_return_key") ;

	if (::GetKeyState(VK_CONTROL) < 0 || ::GetKeyState(VK_SHIFT) < 0 ) // high-order bit set to 1 if pressed
	{
		CSourceAndHtmlEdit* active_edit = get_active_edit() ;

		if (active_edit != NULL) 
		{
			active_edit->handle_return_key() ;
			return ;
		}
	}

	ATLASSERT(IsWindow()) ;
	this->SendMessage(WM_COMMAND, IDOK, 0) ;
}


// Function name	: CRegisterGlossDlg::process_tab_key
// Description	    : 
// Return type		: bool 
bool CRegisterGlossDlg::process_tab_key()
{
	BANNER("CRegisterGlossDlg::process_tab_key") ;

	bool shift_is_pressed = (::GetKeyState(VK_SHIFT) < 0) ;

	HWND focus_hwnd = ::GetFocus() ;
	if (m_rec_source_edit.has_focus(focus_hwnd) )
	{
		m_rec_source_edit.lose_focus() ;

		if (shift_is_pressed) 
		{
			CWindow close_button = this->GetDlgItem(IDCLOSE) ;
			ATLASSERT (close_button.IsWindow()) ; 
			close_button.SetFocus() ;
		}
		else
		{
			ATLTRACE("::FOCUS:: source -> trans\n") ;
			m_rec_trans_edit.gain_focus() ;

			m_current_view = m_rec_trans_edit.get_active_view() ;
		}

		return true ;
	}
	else if (m_rec_trans_edit.has_focus(focus_hwnd))
	{
		m_rec_trans_edit.lose_focus() ;

		if (shift_is_pressed) 
		{
			m_rec_source_edit.gain_focus() ;
		}
		else
		{
			CWindow add_source_btn = this->GetDlgItem(IDC_ADD_SOURCE) ;
			ATLASSERT (add_source_btn.IsWindow()) ; 
			add_source_btn.SetFocus() ;
		}

		return true ;
	}
	else if (focus_hwnd == this->GetDlgItem(IDC_ADD_TRANS))
	{
		if (shift_is_pressed) // button to button
		{
			return false ; // let the default handle this
		}
		else
		{
			m_gloss_source_edit.gain_focus() ;
			m_current_view = m_gloss_source_edit.get_active_view() ;
		}

		return true ;
	}
	else if (m_gloss_source_edit.has_focus(focus_hwnd)) 
	{
		m_gloss_source_edit.lose_focus() ;

		if (shift_is_pressed) 
		{
			CWindow add_trans_btn = this->GetDlgItem(IDC_ADD_TRANS) ;
			ATLASSERT (add_trans_btn.IsWindow()) ; 
			add_trans_btn.SetFocus() ;
		}
		else
		{
			ATLTRACE("::FOCUS:: gloss source -> gloss trans\n") ;
			m_gloss_trans_edit.gain_focus() ;
			m_current_view = m_gloss_trans_edit.get_active_view() ;
		}

		return true ;
	}
	else if (m_gloss_trans_edit.has_focus(focus_hwnd)) 
	{
		m_gloss_trans_edit.lose_focus() ;

		if (shift_is_pressed) 
		{
			m_gloss_source_edit.gain_focus() ;
			m_current_view = m_gloss_source_edit.get_active_view() ;
		}
		else
		{
			CWindow advanced_button = this->GetDlgItem(IDC_ADVANCED) ;
			ATLASSERT (advanced_button.IsWindow()) ; 
			m_AddCombo.SetFocus() ;
		}

		return true ;
	}
	else if (focus_hwnd == m_AddCombo)
	{
		if (shift_is_pressed) // button to button
		{
			m_gloss_trans_edit.gain_focus() ;
			m_current_view = m_gloss_trans_edit.get_active_view() ;
		}
		else
		{
			return false ; // let the default handle this
		}

		return true ;
	}
	else if (focus_hwnd == this->GetDlgItem(IDCLOSE))
	{
		if (shift_is_pressed) // button to button
		{
			return false ; // let the default handle this
		}
		else
		{
			m_rec_source_edit.gain_focus() ;

			m_current_view = m_rec_source_edit.get_active_view() ;
		}

		return true ;
	}

	return false ;
}

LRESULT CRegisterGlossDlg::OnTabSelChanging(LPNMCTC2ITEMS items)
{
	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL) 
	{
		m_current_view = active_edit->tab_sel_changing(items) ;
	}

	if (m_current_view.IsWindow()) 
	{
		m_current_view.SetFocus() ;
	}

	return 0L ;
}

LRESULT CRegisterGlossDlg::OnFormatBold()
{
	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL) 
	{
		active_edit->handle_bold() ;
	}
	else
	{
		SetMsgHandled(FALSE) ;
	}

	return 0L ;
}

LRESULT CRegisterGlossDlg::OnFormatItalic()
{
	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL) 
	{
		active_edit->handle_italic() ;
	}
	else
	{
		SetMsgHandled(FALSE) ;
	}

	return 0L ;
}

LRESULT CRegisterGlossDlg::OnFormatUnderline()
{
	CSourceAndHtmlEdit* active_edit = get_active_edit() ;

	if (active_edit != NULL) 
	{
		active_edit->handle_underline() ;
	}
	else
	{
		SetMsgHandled(FALSE) ;
	}

	return 0L ;
}

CSourceAndHtmlEdit* CRegisterGlossDlg::get_active_edit()
{
	HWND focus_hwnd = ::GetFocus() ;

	if (m_rec_source_edit.has_focus(focus_hwnd)) 
	{
		return &m_rec_source_edit ;
	}
	if (m_gloss_source_edit.has_focus(focus_hwnd)) 
	{
		return &m_gloss_source_edit ;
	}
	if (m_rec_trans_edit.has_focus(focus_hwnd)) 
	{
		return &m_rec_trans_edit ;
	}
	if (m_gloss_trans_edit.has_focus(focus_hwnd)) 
	{
		return &m_gloss_trans_edit ;
	}
	return NULL ;
}

LRESULT CRegisterGlossDlg::OnCmdAddSource()
{
	const _bstr_t text = string2BSTR(trim_and_normalize(m_rec_source_edit.get_selected_text())) ;
	m_gloss_source_edit.SetText(text) ;
	return 0L ;
}
LRESULT CRegisterGlossDlg::OnCmdAddTrans()
{
	const _bstr_t text = string2BSTR(trim_and_normalize(m_rec_trans_edit.get_selected_text())) ;
	m_gloss_trans_edit.SetText(text) ;
	return 0L ;
}



LRESULT CRegisterGlossDlg::OnCmdAdvanced()
{
	CEditTransRecordDialogRegGloss editdlg(m_make_defaults) ;

	m_gloss_record->set_source(BSTR2wstring(m_gloss_source_edit.GetText())) ;
	m_gloss_record->set_trans(BSTR2wstring(m_gloss_trans_edit.GetText())) ;
	editdlg.set_record(m_gloss_record) ;

	if (editdlg.DoModal(*this) != IDOK)
	{
		return 0L;
	}
	m_make_defaults = editdlg.m_make_defaults ;
	m_gloss_source_edit.SetText(string2BSTR(m_gloss_record->get_source_rich())) ;
	m_gloss_trans_edit.SetText(string2BSTR(m_gloss_record->get_trans_rich())) ;

	return 0L ;
}
LRESULT CRegisterGlossDlg::OnCmdRegister()
{
	add_gloss_entry();

	return 0L ;
}

void CRegisterGlossDlg::set_gloss_window(gloss_window_pointer gloss)
{
	m_gloss_window = gloss ;
}

void CRegisterGlossDlg::set_record(record_pointer rec)
{
	m_mem_record = rec ;
#ifdef UNIT_TEST
	return ;
#else
	if	(IsWindow())
	{
		// first clear the source and trans text (to clear any formatting)...
		m_rec_source_edit.SetText(L"") ;
		m_rec_trans_edit.SetText(L"") ;

		// and then set the text.
		m_rec_source_edit.SetText(string2BSTR(m_mem_record->get_source_rich())) ;
		m_rec_trans_edit.SetText(string2BSTR(m_mem_record->get_trans_rich())) ;
	}
#endif
}

void CRegisterGlossDlg::pad_rectangle(CRect &rect, const int PaddingConst)
{
	rect.left += PaddingConst ;
	rect.top += PaddingConst ;
	rect.right -= PaddingConst ;
	rect.bottom -= PaddingConst ;
}


void CRegisterGlossDlg::resize_register_button(const CRect &ClientRect)
{
	resize_button(m_RegisterBtn, ClientRect) ;
}

void CRegisterGlossDlg::resize_close_button(const CRect &ClientRect)
{
	resize_button(m_CloseBtn, ClientRect) ;
}

void CRegisterGlossDlg::resize_button(CButtonT<CWindow> &button, const CRect &ClientRect)
{
	CWindowRect button_rect(button) ;

	CRect new_button_rect(ClientRect) ;

	resize_row_button(new_button_rect, button_rect.Height(), button_rect.Width());

	button.MoveWindow(&new_button_rect, TRUE) ;
	button.InvalidateRect(&new_button_rect) ;
}

void CRegisterGlossDlg::resize_row_button(CRect &rect, int button_height, int button_width)
{
	rect.top = rect.bottom - button_height ;
	rect.left = rect.right - button_width ;
}

void CRegisterGlossDlg::resize_advanced_button(const CRect &ClientRect)
{
	CWindowRect AdvancedButtonRect(m_AdvancedBtn) ;
	CRect NewAdvancedButtonRect(ClientRect) ;

	resize_advanced_rect(NewAdvancedButtonRect, AdvancedButtonRect);

	m_AdvancedBtn.MoveWindow(&NewAdvancedButtonRect, TRUE) ;
	m_AdvancedBtn.InvalidateRect(&NewAdvancedButtonRect, TRUE) ;
}

void CRegisterGlossDlg::resize_advanced_rect(CRect &NewAdvancedButtonRect, const CRect &AdvancedButtonRect)
{
	NewAdvancedButtonRect.top = NewAdvancedButtonRect.bottom - AdvancedButtonRect.Height() ;
	NewAdvancedButtonRect.right = NewAdvancedButtonRect.left + AdvancedButtonRect.Width() ;
}

void CRegisterGlossDlg::resize_client_rect_close(CRect &ClientRect, const int PaddingConst)
{
	CWindowRect button_rect(m_CloseBtn) ;
	ClientRect.bottom -= (button_rect.Height() + PaddingConst) ;
}
void CRegisterGlossDlg::resize_client_rect_advanced(CRect &ClientRect, const int PaddingConst)
{
	CWindowRect adv_button_rect(m_AdvancedBtn) ;
	ClientRect.bottom -= (adv_button_rect.Height() + PaddingConst) ;
}

CString CRegisterGlossDlg::get_mem_name(mem_engine::memory_pointer mem)
{
	if (! mem->is_local())
	{
		return mem->get_location() ;
	}
	else
	{
		file::name fname =  mem->get_location() ;
		return fname.file_name_part() ;
	}
}

/************************************
Fill in the add combo box with the names of the glossaries, and select the current item.
If there are no glossaries, add one.
************************************/
void CRegisterGlossDlg::fill_add_combo()
{
	model_iface_ptr model = m_gloss_window->get_memory_model() ;
	if (model->empty())
	{
		model->add_memory() ;
	}

	// Re-fill the combo contents.
	// This is in case the number/names of glossaries changed.
	std::vector<wstring> names ;
	model->get_memory_names(names) ;
	m_AddCombo.ResetContent() ;
	FOREACH(wstring name, names)
	{
		m_AddCombo.AddString(name.c_str()) ;
	}

	m_AddCombo.SetCurSel(m_current_add_pos) ;
}

LRESULT CRegisterGlossDlg::OnShowWindow(BOOL bShow, UINT)
{
	if (! bShow)
	{
		return 0L ;
	}
	fill_add_combo() ;
	return 0L ;
}

/* Get the source and translation from the right-hand dialog boxes,
 * set them to the record, and then add that record to the glossary.
 * Reset the record and right-hand combo edits afterward.
 * If the right-hand edit box is empty, then use the selection on the left-hand
 * side, if any.
 */
void CRegisterGlossDlg::add_gloss_entry()
{
	try
	{
		wstring source = BSTR2wstring(m_gloss_source_edit.GetText()) ;
		if (source.empty())
		{
			source = trim_and_normalize(m_rec_source_edit.get_selected_text(false)) ;
		}
		m_gloss_record->set_source(source) ;

		wstring trans = BSTR2wstring(m_gloss_trans_edit.GetText()) ;
		if (trans.empty())
		{
			trans = trim_and_normalize(m_rec_trans_edit.get_selected_text(false)) ;
		}
		m_gloss_record->set_trans(trans) ;

		if (! m_gloss_record->is_valid_record())
		{
			throw except::CException(R2T(IDS_MSG_NO_EMPTY_S_GLOSS)) ;
		}

		// update the item selected in the "Register to" list box
		refresh_current_add_pos();
		m_gloss_window->add_record(m_gloss_record->clone(), m_current_add_pos) ;
		if (! m_make_defaults)
		{
			m_gloss_record = record_pointer(new record_local()) ;
		}

		m_gloss_source_edit.SetText(L"") ;
		m_gloss_trans_edit.SetText(L"") ;
		m_gloss_record->set_source(L"") ;
		m_gloss_record->set_trans(L"") ;
	}
	catch(CException &e)
	{
		e.add_to_message(R2T(IDS_MSG_EDIT_REC_FAILED)) ;
		logging::log_exception(e) ;
		throw except::CException (e) ;
	}
}

void CRegisterGlossDlg::refresh_current_add_pos()
{
	if (m_AddCombo.GetCount() > 1)
	{
		m_current_add_pos = m_AddCombo.GetCurSel();
	}
	else
	{
		m_current_add_pos = 0;
	}
}

void CRegisterGlossDlg::set_initial_source( const _bstr_t selection )
{
	m_gloss_source_edit.SetText(selection) ;
	_bstr_t blank = L"" ;
	m_gloss_trans_edit.SetText(blank) ;
	m_gloss_trans_edit.set_html_focus() ;
}

void CRegisterGlossDlg::fresh_record_focus()
{
	_bstr_t blank = L"" ;
	m_gloss_source_edit.SetText(blank) ;
	m_gloss_trans_edit.SetText(blank) ;
	m_rec_source_edit.set_html_focus() ;
}

// We need to change the font of the combo box
// so that DBC text appears correctly in the edit control.
void CRegisterGlossDlg::set_combo_font()
{
	HFONT hFont = CreateFont(
		0, //  int nHeight,
		0, //  int nWidth,
		0, //  int nEscapement,
		0, //  int nOrientation,
		FW_DONTCARE, //  int fnWeight,
		FALSE, //  DWORD fdwItalic,
		FALSE, //  DWORD fdwUnderline,
		FALSE, //  DWORD fdwStrikeOut,
		DEFAULT_CHARSET, //  DWORD fdwCharSet,
		OUT_OUTLINE_PRECIS, //  DWORD fdwOutputPrecision,
		CLIP_DEFAULT_PRECIS, //  DWORD fdwClipPrecision,
		CLEARTYPE_QUALITY, //  DWORD fdwQuality,
		VARIABLE_PITCH, //  DWORD fdwPitchAndFamily,
		TEXT("MS UI Gothic") //  LPCTSTR lpszFace
		);

	if (! hFont)
	{
		hFont = CreateFont(
			0, //  int nHeight,
			0, //  int nWidth,
			0, //  int nEscapement,
			0, //  int nOrientation,
			FW_DONTCARE, //  int fnWeight,
			FALSE, //  DWORD fdwItalic,
			FALSE, //  DWORD fdwUnderline,
			FALSE, //  DWORD fdwStrikeOut,
			DEFAULT_CHARSET, //  DWORD fdwCharSet,
			OUT_OUTLINE_PRECIS, //  DWORD fdwOutputPrecision,
			CLIP_DEFAULT_PRECIS, //  DWORD fdwClipPrecision,
			CLEARTYPE_QUALITY, //  DWORD fdwQuality,
			VARIABLE_PITCH, //  DWORD fdwPitchAndFamily,
			TEXT("MS Dialog Shell") //  LPCTSTR lpszFace
			);
	}

	ATLASSERT(hFont) ;
	if (hFont)
	{
		BOOL redraw = FALSE ;
		m_AddCombo.SetFont(hFont, redraw) ;
	}
}

void CRegisterGlossDlg::apply_window_settings()
{
	CWindowSettings ws;
	if (ws.Load(resource_string(IDS_REG_KEY), REG_DLG_WINSETTINGS_KEY))
	{
		ws.ApplyTo(*this);
	}
	else
	{
		logging::log_warn("Failed to retrieve window settings for Register Glossary Dialog");
	}
}
