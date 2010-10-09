#pragma once

#include "resource.h"
#include "record.h"				// record_pointer
#include "MessageMapDefines.h"	// message handling enhancements

#include "WindowExceptionHandler.h"

#include "SourceAndHtmlEdit.h"

#include "GlossaryDialog.h"	// gloss_window_pointer

wstring trim_and_normalize(const _bstr_t before) ;
wstring trim_text(const wstring before) ;
wstring normalize_spaces(const wstring before) ;

/**
@class CRegisterGlossDlg 
@brief Dialog for editing translation records.
*/
class CRegisterGlossDlg : 
	public CMessageFilter
	, public CAxDialogImpl< CRegisterGlossDlg, TWindow >
	, public CWindowExceptionHandler< CRegisterGlossDlg >
	//, public CDialogLayout< CRegisterGlossDlg >
{
	VISIBLE_TO_TESTS

	mem_engine::record_pointer m_mem_record ;
	mem_engine::record_pointer m_gloss_record ;

	// record source
	CSourceAndHtmlEdit m_rec_source_edit ;

	// gloss source
	CSourceAndHtmlEdit m_gloss_source_edit ;

	// record trans
	CSourceAndHtmlEdit m_rec_trans_edit ;

	// gloss trans
	CSourceAndHtmlEdit m_gloss_trans_edit ;

	// static boxes
	CStaticT<TWindow> m_SourceBox ;
	CStaticT<TWindow> m_TransBox ;
	CStaticT<TWindow> m_GlossSourceBox ;
	CStaticT<TWindow> m_GlossTransBox ;

	CStaticT<TWindow> m_SourceLabel ;
	CStaticT<TWindow> m_TransLabel ;
	CStaticT<TWindow> m_GlossSourceLabel ;
	CStaticT<TWindow> m_GlossTransLabel ;

	CButtonT<TWindow> m_AddSourceBtn ;
	CButtonT<TWindow> m_AddTransBtn ;
	CButtonT<TWindow> m_AdvancedBtn ;
	CButtonT<TWindow> m_RegisterBtn ;
	CButtonT<TWindow> m_CloseBtn ;

	CComboBoxT<TWindow> m_AddCombo ;
	CStaticT<TWindow> m_AddStatic ;

	TWindow			m_current_view ;

	CAccelerator	m_accel ;

	gloss_window_pointer m_gloss_window ;

	bool m_make_defaults ;
	int AddButtonHeight ;
	int m_current_add_pos ;
public:

	const static int IDD = IDD_ADD_GLOSS ;

	DECLARE_SENSING_VAR ;

	CRegisterGlossDlg();

	void set_gloss_window( gloss_window_pointer gloss );

	void set_record( mem_engine::record_pointer rec );

	// PreTranslateMessage
	BOOL PreTranslateMessage(MSG* pMsg) ;
	// helpers
	bool process_tab_key();
	void process_return_key( );

	CSourceAndHtmlEdit* get_active_edit() ;
	// message handlers

	LRESULT OnDestroy( ) ;
	LRESULT OnInitDialog( );

	void fill_add_combo();
	CString get_mem_name(mem_engine::memory_pointer mem);
	LRESULT OnCancel( ) ;
	LRESULT OnOK( ) ;

	void add_gloss_entry();
	void refresh_current_add_pos();
	LRESULT OnClose( ) ;

	LRESULT OnInsertSymbol( );
	LRESULT OnFont( ) ;
	LRESULT OnFormatBackgroundColor( );
	LRESULT OnFormatUnderline();
	LRESULT OnFormatItalic();
	LRESULT OnFormatBold();

	LRESULT OnCmdAddSource( ); 
	LRESULT OnCmdAddTrans( );
	LRESULT OnCmdAdvanced ( );
	LRESULT OnCmdRegister( );

	LRESULT OnTabSelChanging(LPNMCTC2ITEMS items);
	void pad_rectangle( CRect &rect, const int PaddingConst );
	void resize_row_button(CRect &rect, int button_height, int button_width);

	LRESULT OnSize( UINT type, CSize size );
	LRESULT OnShowWindow(BOOL bShow, UINT status);

	void resize_client_rect_close(CRect &ClientRect, const int PaddingConst) ;
	void resize_client_rect_advanced(CRect &ClientRect, const int PaddingConst) ;

	void resize_advanced_button(const CRect &ClientRect);

	void resize_advanced_rect( CRect &NewAdvancedButtonRect, const CRect &AdvancedButtonRect );
	void resize_close_button(const CRect &ClientRect);
	void resize_register_button(const CRect &ClientRect);
	void resize_button(CButtonT<TWindow> &button, const CRect &ClientRect);
	void SizeBoxes();
	void sizeGripper(UINT type);

	void set_initial_source(const _bstr_t selection);
	void fresh_record_focus();
	BEGIN_MSG_MAP_EX( CRegisterGlossDlg )

	try
	{
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		MSG_HANDLER_0(WM_DESTROY, OnDestroy)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		MSG_WM_SIZE(OnSize)

		BEGIN_NOTIFY_HANDLER_EX

			HANDLE_NOTIFICATION_EX( CTCN_SELCHANGING, OnTabSelChanging,	LPNMCTC2ITEMS)

		END_NOTIFY_HANDLER

		BEGIN_CMD_HANDLER_EX

			CMD_HANDLER_EX_0(IDOK,				OnOK )
			CMD_HANDLER_EX_0(IDCLOSE,			OnClose )
			CMD_HANDLER_EX_0(IDCANCEL,			OnCancel )

			CMD_HANDLER_EX_0(ID_INSERT_SYMBOL,	OnInsertSymbol )
			CMD_HANDLER_EX_0(ID_FORMAT_FONT,		OnFont )
			CMD_HANDLER_EX_0(ID_FORMAT_BGCOLOR, OnFormatBackgroundColor )
			CMD_HANDLER_EX_0( IDC_BOLD,			OnFormatBold )
			CMD_HANDLER_EX_0( IDC_ITALIC,			OnFormatItalic )
			CMD_HANDLER_EX_0( IDC_UNDERLINE,		OnFormatUnderline )

			CMD_HANDLER_EX_0( IDC_ADD_SOURCE, OnCmdAddSource ) 
			CMD_HANDLER_EX_0( IDC_ADD_TRANS,  OnCmdAddTrans )
			CMD_HANDLER_EX_0( IDC_ADVANCED,  OnCmdAdvanced )
			CMD_HANDLER_EX_0( IDC_REGISTER,  OnCmdRegister )

			else
			{
				ATLTRACE( "Breaking into commands...\n" ) ;

				CSourceAndHtmlEdit *active_edit = get_active_edit() ;

				if ( active_edit != NULL ) 
				{
					m_current_view = active_edit->get_active_view() ;
					lResult = m_current_view.SendMessage( WM_COMMAND, wParam, lParam ) ;
					return TRUE ;
				}
			}

		END_CMD_HANDLER_EX
	}
	catch ( except::CSWException &sw_e ) 
	{ 
		logging::log_error("Register Gloss Dlg - Structured Windows Exception") ;
		logging::log_exception(sw_e) ;
		CString language = L"English" ;
		const CString lang_code = resource_string(IDS_LANG_CODE) ;
		if(lang_code == L"jp")
		{
			language = L"Japanese" ;
		}
		logging::send_report(language, sw_e.get_filename()) ;
		return FALSE ;
	} 
	catch ( _com_error &e ) 
	{ 
		logging::log_error("Register Gloss Dlg - COM Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": COM ERROR") ;
		return handle_exception( e, fail_msg ) ; 
	} 
	catch ( except::CComException &e ) 
	{ 
		logging::log_error("Register Gloss Dlg - Application COM Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": COM EXCEPTION") ;
		return handle_exception( e, fail_msg ) ;
	} 
	catch ( except::CWinException &e ) 
	{ 
		logging::log_error("Register Gloss Dlg - Windows Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": WINDOWS ERROR") ;
		return handle_exception( e, fail_msg ) ;
	} 
	catch ( except::CException &e ) 
	{ 
		logging::log_error("Register Gloss Dlg - Application Exception") ;
		logging::log_exception(e) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": EXCEPTION") ;
		return handle_exception( e, fail_msg ) ; 
	} 
	catch ( std::exception &e ) 
	{ 
		logging::log_error("Register Gloss Dlg - C++ Library Exception") ;
		logging::log_error(e.what()) ;
		CString fail_msg( R2T( IDS_MSG_ACTION_FAILED ) ) ; 
		fail_msg += _T(": RUNTIME ERROR") ;
		return handle_exception( e, fail_msg ) ; 
	}
	END_MSG_MAP()
};