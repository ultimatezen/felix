#pragma once

#include "resource.h"
#include "record.h"				// record_pointer
#include "MessageMapDefines.h"	// message handling enhancements

#include "WindowExceptionHandler.h"

#include "SourceAndHtmlEdit.h"

#include "GlossaryWindowFrame.h"	// gloss_window_pointer
#include "active_window.h"

wstring trim_and_normalize(const _bstr_t before) ;
wstring trim_text(const wstring before) ;
wstring normalize_spaces(const wstring before) ;

/**
@class CRegisterGlossDlg 
@brief Dialog for editing translation records.
*/
class CRegisterGlossDlg : 
	public CMessageFilter
	, public CAxDialogImpl< CRegisterGlossDlg, CWindow >
	, public CWindowExceptionHandler< CRegisterGlossDlg >
	, public ActiveWindow
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
	CStaticT<CWindow> m_SourceBox ;
	CStaticT<CWindow> m_TransBox ;
	CStaticT<CWindow> m_GlossSourceBox ;
	CStaticT<CWindow> m_GlossTransBox ;

	CStaticT<CWindow> m_SourceLabel ;
	CStaticT<CWindow> m_TransLabel ;
	CStaticT<CWindow> m_GlossSourceLabel ;
	CStaticT<CWindow> m_GlossTransLabel ;

	CButtonT<CWindow> m_AddSourceBtn ;
	CButtonT<CWindow> m_AddTransBtn ;
	CButtonT<CWindow> m_AdvancedBtn ;
	CButtonT<CWindow> m_RegisterBtn ;
	CButtonT<CWindow> m_CloseBtn ;

	CComboBoxT<CWindow> m_AddCombo ;
	CStaticT<CWindow> m_AddStatic ;

	CWindow			m_current_view ;

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

	void apply_window_settings();

	void set_combo_font();
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
	void resize_button(CButtonT<CWindow> &button, const CRect &ClientRect);
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
		MSG_HANDLER_WIN_MSG(WM_ACTIVATE, on_activate)

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
		return this->handle_sw_exception(sw_e, _T("Register Glossary Dialog")) ;
	} 
	catch ( _com_error &e ) 
	{ 
		return handle_exception( e, _T("Register Glossary Dialog - COM Exception") ) ; 
	} 
	catch ( except::CComException &e ) 
	{ 
		return handle_exception( e, _T("Register Glossary Dialog - Application COM Exception") ) ;
	} 
	catch ( except::CWinException &e ) 
	{ 
		return handle_exception( e, _T("Register Glossary Dialog - Windows Exception") ) ;
	} 
	catch ( except::CException &e ) 
	{ 
		return handle_exception( e, _T("Register Glossary Dialog - Application Exception") ) ; 
	} 
	catch ( std::exception &e ) 
	{ 
		return handle_exception( e, _T("Register Glossary Dialog -  C++ Library Exception") ) ; 
	}
	END_MSG_MAP()
};
