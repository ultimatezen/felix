
#pragma once

#include "resource.h"
#include "record.h"				// record_pointer
#include "MessageMapDefines.h"	// message handling enhancements

#include "Accelerator.h"		// CAccelerator

#include "WindowExceptionHandler.h"

#include "SourceAndHtmlEdit.h"
#include "ExtraStringsView.h"
#include "WideRichEdit.h"

#include "UserStringDlg.h"
#include "resource_string.h"
#include "ColorRef.h"
#include "reg_msg_filter.h"


#define CHECK_PASS(CLASS_NAME, FUNCTION) if (CLASS_NAME.m_hWnd == focus || CLASS_NAME.IsChild(focus)) { return CLASS_NAME.FUNCTION() ; }

template <class TBase >
class CEditTransRecordDialogCommon : 
	public CMessageFilter, 
	public CAxDialogImpl< CEditTransRecordDialogCommon<TBase>, TWindow >, 
	public CDialogResize< CEditTransRecordDialogCommon<TBase> >,
	public CWindowExceptionHandler< CEditTransRecordDialogCommon<TBase> >
{
public:
	typedef memory_engine::record_pointer record_type ;
	typedef CEditTransRecordDialogCommon<TBase> TheClass ;

	record_type m_old_record ;

	// edits
	CSourceAndHtmlEdit m_source_edit ;
	CSourceAndHtmlEdit m_trans_edit ;
	CSourceAndHtmlEdit m_context_edit ;

	// other stuff
	CStatic			m_created_box ;
	CStatic			m_modified_box ;
	CCommandEdit	m_reliability_edit ;

	CExtraStringsView m_extra_strings_view ;

	TWindow			m_current_view ;

	CAccelerator	m_accel ;

	int				m_memory_id ;

	DECLARE_SENSING_VAR ;

	bool			m_is_glossary ;

	const static int IDD = IDD_EDIT_RECORD ;

	CEditTransRecordDialogCommon() : 
		m_is_glossary(false) 
		, m_memory_id(0)
	{
		m_old_record = record_type(new record_local) ;
		m_accel.LoadAccelerators(IDC_EDIT_ACCELERATORS) ;
	}

	void set_memory_id(const int id)
	{
		m_memory_id = id ;
	}

	int get_memory_id()
	{
		return m_memory_id ;
	}


	CSourceAndHtmlEdit* get_active_edit()
	{
		HWND focus_hwnd = ::GetFocus() ;

		if (m_source_edit.has_focus(focus_hwnd)) 
		{
			return &m_source_edit ;
		}
		if (m_trans_edit.has_focus(focus_hwnd)) 
		{
			return &m_trans_edit ;
		}
		if (m_context_edit.has_focus(focus_hwnd)) 
		{
			return &m_context_edit ;
		}

		return NULL ;
	}

	void initialize_dialog()
	{
		ATLASSERT(IsWindow()) ;

		// center us on our parent
		CenterWindow(GetParent());

		// set icon
		SetIcon(LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE) ;

#ifdef UNIT_TEST
		return ;
#endif

		register_message_filter(this) ;
		// Init the CDialogResize code
		DlgResize_Init(true, true) ;

		// =========
		// reliability
		// =========

		m_reliability_edit.Attach(GetDlgItem(IDC_RELIABILITY_EDIT)) ;
		CUpDownCtrlT<TWindow> reliability_spin = GetDlgItem(IDC_RELIABILITY_SPIN) ;
		reliability_spin.SetRange(0, 9) ;
		reliability_spin.SetPos(0) ;

		// =========
		// modified
		// =========

		m_modified_box.Attach(GetDlgItem(IDC_MODIFIED_BOX)) ;

		// =========
		// created
		// =========
		m_created_box.Attach(GetDlgItem(IDC_CREATED_BOX)) ;
		SetWindowText(R2T(IDS_EDIT_RECORD_TITLE)) ;

		// =========
		// source
		// =========
		try
		{
			m_current_view = m_source_edit.create(GetDlgItem(IDC_SOURCE_BOX), 
													*this, 
													HWND_TOP) ;
			ATLASSERT(m_current_view.IsWindow()) ;
			::SetFocus(GetDlgItem(IDC_SOURCE_BOX));
		}
		catch (CException& e)
		{
			logging::log_error("Program exception -- creating source edit") ;
			logging::log_exception(e) ;
			e.notify_user("Failed to create source edit box") ;
		}
		catch (_com_error& e)
		{
			logging::log_error("COM exception -- creating source edit") ;
			logging::log_exception(e) ;
			CComException ce(e) ;
			ce.notify_user("Failed to create source edit box") ;
		}
		// =========
		// trans
		// =========
		TWindow trans_window ;
		try
		{
			trans_window = m_trans_edit.create(GetDlgItem(IDC_TRANS_BOX), 
				*this, 
				m_current_view) ;
			ATLASSERT(trans_window.IsWindow()) ;
		}
		catch (CException& e)
		{
			logging::log_error("Program exception -- creating translation edit") ;
			logging::log_exception(e) ;
			e.notify_user("Failed to create translation edit box") ;
		}
		catch (_com_error& e)
		{
			logging::log_error("COM exception -- creating translation edit") ;
			logging::log_exception(e) ;
			CComException ce(e) ;
			ce.notify_user("Failed to create translation edit box") ;
		}

		// =========
		// context
		// =========
		try
		{
			TWindow context_window = m_context_edit.create(GetDlgItem(IDC_CONTEXT_BOX), 
				*this, 
				trans_window) ;
			ATLASSERT(context_window.IsWindow()) ;
		}
		catch (CException& e)
		{
			logging::log_error("Program exception -- creating context edit") ;
			logging::log_exception(e) ;
			e.notify_user("Failed to create context edit box") ;
		}
		catch (_com_error& e)
		{
			logging::log_error("COM exception -- creating context edit") ;
			logging::log_exception(e) ;
			CComException ce(e) ;
			ce.notify_user("Failed to create context edit box") ;
		}

		// =========
		// extra strings
		// =========
		m_extra_strings_view.create(GetDlgItem(IDC_EXTRA_STRING_BOX), *this) ;
		m_extra_strings_view.show_extra_strings(m_old_record) ;
	}

	void fill_rec_from_gui(record_type &rec)
	{
		// validated
		if (FALSE == IsDlgButtonChecked(IDC_VALIDATED_CHECK))
		{
			rec->set_validated_off() ;
		}
		else
		{
			rec->set_validated_on() ;
		}
#ifndef UNIT_TEST
		// source
		rec->set_source(BSTR2wstring(m_source_edit.GetText())) ;
		// trans
		rec->set_trans(BSTR2wstring(m_trans_edit.GetText())) ;
		// context
		rec->set_context(BSTR2wstring(m_context_edit.GetText())) ;

		// reliability
		CString rel_string ;
		m_reliability_edit.GetWindowText(rel_string) ;

		if (! rel_string.IsEmpty())
		{
			rec->set_reliability(_tcstoul(rel_string, NULL, 10)) ;
		}
#endif

		m_extra_strings_view.fill_user_strings(rec) ;
	}
	void fill_from_record(record_type record)
	{
		SENSE("fill_from_record") ;

		// reliability
		CUpDownCtrl reliability_spin = GetDlgItem(IDC_RELIABILITY_SPIN) ;
		reliability_spin.SetPos((int) record->get_reliability()) ;

		// validated
		CheckDlgButton(IDC_VALIDATED_CHECK, (record->is_validated() ? BST_CHECKED : BST_UNCHECKED)) ;

		// modified
		m_modified_box.SetWindowText((R2T(IDS_MODIFIED) + string2tstring(record->get_modified().get_date_time_string())).c_str()) ;
		// created
		m_created_box.SetWindowText ((R2T(IDS_CREATED)  + string2tstring(record->get_created().get_date_time_string() )).c_str()) ;

		// trans
		_bstr_t trans_text = string2BSTR(record->get_trans_rich()) ;
		m_trans_edit.SetText(trans_text) ;

		// context
		_bstr_t context_text = string2BSTR(record->get_context_rich()) ;
		m_context_edit.SetText(context_text) ;

		// source
		_bstr_t source_text = string2BSTR(record->get_source_rich()) ;
		m_source_edit.SetText(source_text) ;

		m_extra_strings_view.show_extra_strings(record) ;
	}
	// Message handlers

	LRESULT OnInsertSymbol()
	{
		BANNER("CEditTransRecordDialog::OnInsertSymbol") ;

		CSourceAndHtmlEdit* active_edit = get_active_edit() ;

		if (active_edit != NULL) 
		{
			active_edit->insert_symbol() ;
		}

		return 0L ;
	}

	LRESULT OnDestroy()
	{
		BANNER("CEditTransRecordDialog::OnDestroy") ;

		// These are just wrappers for the control -- they
		// won't get a WM_DESTROY message, so the HWND will be non-null
		// when the object is destroyed, creating an assert.
		// Go ahead and set the HWNDs to NULL here
		m_source_edit.detach() ;
		m_trans_edit.detach() ;
		m_context_edit.detach() ;

		m_reliability_edit.Detach() ;

		m_created_box.Detach() ;
		m_modified_box.Detach() ;

		m_extra_strings_view.cleanup() ;

		_Module.GetMessageLoop()->RemoveMessageFilter(this);

		return 0;
	}

	LRESULT OnFont()
	{
		BANNER("CEditTransRecordDialog::OnFont") ;

		CSourceAndHtmlEdit* active_edit = this->get_active_edit() ;
		if (active_edit)
		{
			active_edit->handle_format(); 
		}
		else
		{
			m_source_edit.gain_focus() ;
			m_source_edit.handle_format() ;
		}

		return 0 ;
	}

	void process_return_key()
	{
		BANNER("CEditTransRecordDialog::process_return_key") ;

		if (::GetKeyState(VK_CONTROL) < 0 || ::GetKeyState(VK_SHIFT) < 0 ) // high-order bit set to 1 if pressed
		{
			CSourceAndHtmlEdit* active_edit = get_active_edit() ;

			if (active_edit) 
			{
				active_edit->handle_return_key() ;
				return ;
			}
		}

		ATLASSERT(IsWindow()) ;
		this->SendMessage(WM_COMMAND, IDOK, 0) ;
	}

	bool process_tab_key()
	{
		BANNER("process_tab_key") ;

		HWND focus_hwnd = ::GetFocus() ;
		if (m_source_edit.has_focus(focus_hwnd) )
		{
			ATLTRACE("::FOCUS:: source -> trans\n") ;
			m_source_edit.lose_focus() ;
			m_trans_edit.gain_focus() ;

			m_current_view = m_trans_edit.get_active_view() ;
			//m_current_view.SetFocus() ;

			return true ;
		}
		if (m_trans_edit.has_focus(focus_hwnd))
		{
			ATLTRACE("::FOCUS:: trans -> context\n") ;
			m_trans_edit.lose_focus() ;
			m_context_edit.gain_focus() ;

			m_current_view = m_context_edit.get_active_view() ;
			//m_current_view.SetFocus() ;
			return true ;
		}
		if (m_context_edit.has_focus(focus_hwnd) )
		{
			ATLTRACE("::FOCUS:: context -> extra\n") ;
			m_context_edit.lose_focus() ;
			m_extra_strings_view.gain_focus() ;
			return true ;
		}
		if (m_extra_strings_view.has_focus(focus_hwnd))
		{
			ATLTRACE("::FOCUS:: extra -> edit\n") ;
			TWindow edit_strings = GetDlgItem(IDC_EDIT_STRINGS) ;
			ATLASSERT(edit_strings.IsWindow()) ;
			edit_strings.SetFocus() ;

			return true ;
		}
		if (focus_hwnd == GetDlgItem(IDCANCEL))
		{
			ATLTRACE("::FOCUS:: cancel -> source\n") ;
			m_source_edit.gain_focus() ;

			m_current_view = m_source_edit.get_active_view() ;
			//m_current_view.SetFocus() ;

			return true ;
		}

		return false ;
	}

	LRESULT OnTabSelChanging(LPNMCTC2ITEMS items)
	{
		if ( m_source_edit.has_focus(items->hdr.hwndFrom) ) 
		{
			m_current_view = m_source_edit.tab_sel_changing(items) ;
		}

		if ( m_trans_edit.has_focus(items->hdr.hwndFrom) ) 
		{
			m_current_view = m_trans_edit.tab_sel_changing(items) ;
		}

		if ( m_context_edit.has_focus(items->hdr.hwndFrom) ) 
		{
			m_current_view = m_context_edit.tab_sel_changing(items) ;
		}

		CSourceAndHtmlEdit* edit = this->get_active_edit() ;
		if (edit)
		{
			edit->gain_focus(); 
		}
		if (m_current_view.IsWindow()) 
		{
			m_current_view.SetFocus() ;
		}

		return 0L ;
	}

	LRESULT OnFormatBold()
	{

		CSourceAndHtmlEdit *active_edit = get_active_edit() ;

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

	LRESULT OnFormatItalic()
	{
		CSourceAndHtmlEdit *active_edit = get_active_edit() ;

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

	LRESULT OnFormatUnderline()
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

	virtual LRESULT OnEditStrings() = 0 ;
	void edit_strings(record_type &rec)
	{
		bool is_edit_mode = m_extra_strings_view.is_edit_mode() ;

		CButton but ;
		but.Attach(GetDlgItem(IDC_EDIT_STRINGS)) ;

		if (is_edit_mode)
		{
			but.SetWindowText(R2T(IDS_END_EDIT ))  ;
		}
		else
		{
			but.SetWindowText(R2T(IDS_EDIT_STRINGS ))  ;
		}

		m_extra_strings_view.put_edit_mode(is_edit_mode) ;
		but.Detach() ;

		if (is_edit_mode)
		{
			return ;
		}

		m_extra_strings_view.fill_user_strings(rec) ;
		m_extra_strings_view.show_extra_strings(rec) ;
	}

	virtual LRESULT OnAddString() = 0 ;
	void add_string(record_type &rec)
	{
		CUserStringDlg dlg ;

		if (IDCANCEL == dlg.DoModal())
		{
			return ;
		}

		// get the current user strings
		m_extra_strings_view.fill_user_strings(rec) ;

		// we will eliminate whitespace and punctuation
		wstring key ;
		m_extra_strings_view.eliminate_whitespace_and_punctuation(dlg.GetKey(), key);

		// add the new one
		rec->set_item(key, dlg.GetValue()) ;

		// and show the updated list
		m_extra_strings_view.show_extra_strings(rec) ;
	}

	void fix_screwy_box_sizes()
	{
		TWindow source_box = GetDlgItem(IDC_SOURCE_BOX) ;
		TWindow trans_box = GetDlgItem(IDC_TRANS_BOX) ;
		TWindow context_box = GetDlgItem(IDC_CONTEXT_BOX) ;

		CRect source_rect ;
		source_box.GetWindowRect(&source_rect) ;
		ScreenToClient(&source_rect) ;
		CRect trans_rect ;
		trans_box.GetWindowRect(&trans_rect) ;
		ScreenToClient(&trans_rect) ;
		CRect context_rect ;
		context_box.GetWindowRect(&context_rect) ;
		ScreenToClient(&context_rect) ;

		source_rect.right = context_rect.right ;
		trans_rect.right = context_rect.right ;
		source_box.SetWindowPos(NULL, &source_rect, SWP_NOZORDER | SWP_NOACTIVATE);
		trans_box.SetWindowPos(NULL, &trans_rect, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	virtual LRESULT OnInitDialog() = 0 ;
	virtual LRESULT OnCancel() = 0 ;
	virtual LRESULT OnOK() = 0 ;

	LRESULT OnSize(UINT type, CSize size)
	{
		BANNER("CAboutDlg::OnSize") ;

		SetMsgHandled(FALSE) ;

		if(m_bGripper)
		{
			TWindow wndGripper = GetDlgItem(ATL_IDW_STATUS_BAR);
			if(type == SIZE_MAXIMIZED)
				wndGripper.ShowWindow(SW_HIDE);
			else if(type == SIZE_RESTORED)
				wndGripper.ShowWindow(SW_SHOW);
		}

		if(type != SIZE_MINIMIZED)
		{
			DlgResize_UpdateLayout(size.cx , size.cy);
		}
		else
		{
			return 0L ;
		}

		fix_screwy_box_sizes();

		// =============
		// source
		// =============
		m_source_edit.handle_size() ;

		// =============
		// trans
		// =============
		m_trans_edit.handle_size() ;

		// =============
		// context
		// =============
		m_context_edit.handle_size() ;

		// =============
		// extra strings
		// =============

		m_extra_strings_view.handle_size() ;

		return 0L ;
	}
	LRESULT OnFormatBackgroundColor()
	{
		SENSE("OnFormatBackgroundColor") ;

#ifdef UNIT_TEST
		return 0L ;
#endif

		CColorDialog dialog(RGB(255,255,255)) ; // current color is default

		if (dialog.DoModal() == IDCANCEL)
			return 0 ;

		// get the color the user picked
		CColorRef color(dialog.GetColor()) ;

		CSourceAndHtmlEdit *active_view = get_active_edit() ;
		if (active_view)
		{
			active_view->set_html_back_color(color.as_wstring()) ;
		}
		else
		{
			m_source_edit.set_html_back_color(color.as_wstring()) ;
		}

		return 0L ;
	}

	LRESULT OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		BANNER("OnGetDlgCode") ;
		LRESULT nRet = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);

		if (lParam != NULL)
		{
			LPMSG pMsg = (LPMSG) lParam;

			// We only want WM_KEYDOWN messages that are for VK_RETURNs.
			if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
			{
				nRet |= DLGC_WANTMESSAGE;
			}
		}
		return nRet;
	}

	BEGIN_DLGRESIZE_MAP(TheClass)
		// buttons
		DLGRESIZE_CONTROL(IDOK,     DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X|DLSZ_MOVE_Y)

		// other moving controls
		DLGRESIZE_CONTROL(IDC_RELIABILITY_EDIT, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_RELIABILITY_STATIC, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_RELIABILITY_SPIN,   DLSZ_MOVE_X|DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_VALIDATED_CHECK,  DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CREATED_BOX,      DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_MODIFIED_BOX,     DLSZ_MOVE_X|DLSZ_MOVE_Y)

		// WebBrowser controls
		BEGIN_DLGRESIZE_GROUP()

		DLGRESIZE_CONTROL(IDC_SOURCE_STATIC,		DLSZ_SIZE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_TRANS_STATIC,			DLSZ_SIZE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CONTEXT_STATIC,		DLSZ_SIZE_X|DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_SOURCE_BOX,			DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_TRANS_BOX,				DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_CONTEXT_BOX,			DLSZ_SIZE_X|DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(IDC_ADD_STRING,			DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_STRINGS,		DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_USER_STRINGS,		DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EXTRA_STRING_BOX,	DLSZ_SIZE_X|DLSZ_SIZE_Y)	
		END_DLGRESIZE_GROUP()

	END_DLGRESIZE_MAP()


	BEGIN_MSG_MAP_EX(TheClass)
	try
	{
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		MSG_HANDLER_0(WM_DESTROY, OnDestroy)

		MSG_WM_SIZE(OnSize)
		//MESSAGE_HANDLER_EX(WM_GETDLGCODE, OnGetDlgCode)

		BEGIN_NOTIFY_HANDLER_EX
			HANDLE_NOTIFICATION_EX(CTCN_SELCHANGING, OnTabSelChanging,	LPNMCTC2ITEMS)
		END_NOTIFY_HANDLER

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_0(IDOK,				OnOK)
			CMD_HANDLER_EX_0(IDCANCEL,			OnCancel)
			CMD_HANDLER_EX_0(IDCLOSE,			OnCancel)

			CMD_HANDLER_EX_0(ID_INSERT_SYMBOL,	OnInsertSymbol)
			CMD_HANDLER_EX_0(ID_FORMAT_FONT,	OnFont)
			CMD_HANDLER_EX_0(ID_FORMAT_BGCOLOR, OnFormatBackgroundColor)
			CMD_HANDLER_EX_0(IDC_BOLD,			OnFormatBold)
			CMD_HANDLER_EX_0(IDC_ITALIC,		OnFormatItalic)
			CMD_HANDLER_EX_0(IDC_UNDERLINE,		OnFormatUnderline)

			CMD_HANDLER_EX_0(IDC_EDIT_STRINGS,	OnEditStrings) 
			CMD_HANDLER_EX_0(IDC_ADD_STRING,	OnAddString)

			else
			{
				ATLTRACE( "Breaking into commands...\n" ) ;

				CSourceAndHtmlEdit* active_edit = get_active_edit() ;

				if (active_edit != NULL) 
				{
					TWindow client_window = active_edit->get_active_view() ;
					lResult = client_window.SendMessage(WM_COMMAND, wParam, lParam) ;
					return TRUE ;
				}

				if (m_extra_strings_view.has_focus(::GetFocus())) 
				{
					lResult = m_extra_strings_view.send_command(wParam, lParam) ;				
					return TRUE ;
				}

			}
		END_CMD_HANDLER_EX

	}
	catch ( CSWException &sw_e ) 
	{ 
		logging::log_error("Edit Trans Record Dialog - Structured Windows Exception") ;
		logging::log_exception(sw_e) ;
		CString language = L"English" ;
		const CString lang_code = resource_string(IDS_LANG_CODE);
		if(lang_code == L"jp")
		{
			language = L"Japanese" ;
		}
		logging::send_report(language, sw_e.get_filename()) ;
		return FALSE ;
	} 
	catch ( _com_error &e ) 
	{ 
		logging::log_error("Edit Trans Record Dialog - COM Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": COM ERROR") ;
		return handle_exception( e, fail_msg ) ; 
	} 
	catch ( CComException &e ) 
	{ 
		logging::log_error("Edit Trans Record Dialog - Application COM Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": COM EXCEPTION") ;
		return handle_exception( e, fail_msg ) ;
	} 
	catch ( CWinException &e ) 
	{ 
		logging::log_error("Edit Trans Record Dialog - Windows Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": WINDOWS ERROR") ;
		return handle_exception( e, fail_msg ) ;
	} 
	catch ( CException &e ) 
	{ 
		logging::log_error("Edit Trans Record Dialog - Application Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": EXCEPTION") ;
		return handle_exception( e, fail_msg ) ; 
	} 
	catch ( std::exception &e ) 
	{ 
		logging::log_error("Edit Trans Record Dialog - C++ Library Exception") ;
		logging::log_error(e.what()) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": RUNTIME ERROR") ;
		return handle_exception( e, fail_msg ) ; 
	}
	END_MSG_MAP()

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		// no banner, because we would be swamped with trace statements!

		ENSURE_FOCUS

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
				{
					return TRUE ;
				}
			}	
		}

		HWND focus_hwnd = ::GetFocus() ;

		if (m_source_edit.has_focus(focus_hwnd))
		{
			if (m_source_edit.pre_translate(pMsg)) 
			{
				return TRUE ;
			}
		}

		else if (m_trans_edit.has_focus(focus_hwnd))
		{
			if (m_trans_edit.pre_translate(pMsg)) 
			{
				return TRUE ;
			}
		}

		else if (m_context_edit.has_focus(focus_hwnd))
		{
			if (m_context_edit.pre_translate(pMsg)) 
			{
				return TRUE ;
			}
		}	

		else if (m_extra_strings_view.has_focus(focus_hwnd))
		{
			if (m_extra_strings_view.pre_translate(pMsg))
				return TRUE ;
		}

		if(! m_accel.IsNull() && m_accel.TranslateAccelerator(*this, pMsg))
		{
			return TRUE;
		}

		if(CAxDialogImpl<TheClass, TWindow>::IsDialogMessage(pMsg))
			return TRUE ;

		return FALSE ;
	}

} ;