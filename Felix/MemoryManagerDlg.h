/*!
	@file MemoryManagerDlg.h
	@brief interface for the CMemoryManagerDlg class.
 */

#pragma once

#include "HtmlView.h"
#include "TranslationMemory.h"
#include "resource.h"
#include "AdvancedMemMgrDlg.h"
#include "memory_model.h"
#include "ProgressListener.h"


/**
	@class CDropperCheckListViewCtrl 
	@brief Checklist control with support for file dropping.
 */
class CDropperCheckListViewCtrl : 
	public CCheckListViewCtrl
	,	public CWindowExceptionHandler< CDropperCheckListViewCtrl >
{
public:

	BEGIN_MAP_EX( CDropperCheckListViewCtrl )
		if( uMsg == WM_DROPFILES )
		{
			lResult = ::SendMessage( GetParent(), uMsg, wParam, lParam ) ;
			return TRUE ;
		}
	
		CHAIN_MSG_MAP( CCheckListViewCtrl )
	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )


};

const wstring get_item(const wstring item) ;

/**
	@class CMemoryManagerDlg 
	@brief Dialog to manage memories.
 */
class CMemoryManagerDlg: 
	public CDialogImpl< CMemoryManagerDlg, CWindow >
	, public CDialogResize< CMemoryManagerDlg >
	, public CWindowExceptionHandler< CMemoryManagerDlg >
	, public CProgressListener
{

	CDropperCheckListViewCtrl	m_list_box ;
	CStaticT<CWindow>			m_info_box ;
	html::CHtmlView					m_info_view ;

	mem_engine::memory_list					m_memories ;
	CProgressBarCtrl			m_progress_bar ;

	int	m_title_id ;
	app_props::props_ptr		m_props ;

public:
	static const int IDD = IDD_MEM_MGR_DLG ;
	DECLARE_SENSING_VAR ;

	CMemoryManagerDlg(app_props::props_ptr props, int title_id = 0);
	virtual ~CMemoryManagerDlg();

	// progress listener implementation
	void OnProgressInit( const CString &file_name, size_t min_val, size_t max_val ) ;
	void OnProgressDoneLoad( size_t final_val ) ;
	void OnProgressDoneWrite( size_t final_val ) ;
	bool OnProgressLoadUpdate( size_t current_val ) ; // true to continue
	bool OnProgressWriteUpdate( size_t current_val ) ; // true to continue

	void display_info_for_item( int item );

	wstring get_info_for_item(mem_engine::memory_pointer mem);

	wstring get_reliability_range(mem_engine::memory_pointer mem);
	wstring get_file_size(mem_engine::memory_pointer mem);
	wstring get_mem_size(mem_engine::memory_pointer mem);
	wstring get_client_name(mem_engine::MemoryInfo *mem_info);
	wstring get_target_lang(mem_engine::MemoryInfo *mem_info);
	wstring get_source_lang(mem_engine::MemoryInfo *mem_info);
	wstring get_created_on(mem_engine::MemoryInfo *mem_info);
	wstring get_field_name(mem_engine::MemoryInfo *mem_info);
	wstring get_creator_name(mem_engine::MemoryInfo *mem_info);
	wstring get_memory_name(mem_engine::memory_pointer mem);
	void add_memory_file(const CString &mem_file );

	wstring get_loading_message( const CString &mem_file );
	void reflect_checkstate();
	void fill_listview();
	void set_memories( std::shared_ptr<mem_engine::memory_model> controller );
	void get_memories( std::shared_ptr<mem_engine::memory_model> memories );
	mem_engine::memory_pointer get_memory_at( int sel );

	mem_engine::memory_iterator get_pos_at( int sel );

	LRESULT OnDropFiles( HDROP drop ) ;
	LRESULT OnSelChange( );
	LRESULT OnCmdOK();
	LRESULT OnCmdCancel();
	LRESULT OnCmdAddMemory();
	LRESULT OnCmdRemoveMemory();
	LRESULT OnCmdEditMemory();
	LRESULT OnCmdUp();
	LRESULT OnCmdDown();

	void swap_memories( int index );
	LRESULT OnDestroy() ;
	LRESULT OnInitDialog();

	LRESULT OnSize( UINT type, CSize size );
	void sizeInfoBox();

	LRESULT OnAdvanced() ;

	// list stuff
	DWORD OnCustomDrawList( LPNMLVCUSTOMDRAW pnmcd ) ;

	BEGIN_DLGRESIZE_MAP(CEditTransRecordDialog)
		BEGIN_DLGRESIZE_GROUP()
			// buttons
			DLGRESIZE_CONTROL(IDOK,					DLSZ_MOVE_X|DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDCANCEL,				DLSZ_MOVE_X|DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDC_ADD_MEMORY,		DLSZ_MOVE_X|DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDC_REMOVE_MEMORY,	DLSZ_MOVE_X|DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDC_EDIT_MEMORY,		DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDC_UP,				DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDC_DOWN,				DLSZ_MOVE_Y)
		
			// resizing controls
			DLGRESIZE_CONTROL(IDC_PROGRESS_BAR,	DLSZ_SIZE_X | DLSZ_MOVE_Y )
			DLGRESIZE_CONTROL(IDC_INFO_BOX,			DLSZ_SIZE_X | DLSZ_SIZE_Y )
			DLGRESIZE_CONTROL(IDC_LIST,				DLSZ_SIZE_X | DLSZ_SIZE_Y )
		END_DLGRESIZE_GROUP()
	
	END_DLGRESIZE_MAP()

	BEGIN_MAP_EX( CMemoryManagerDlg )
		MSG_HANDLER_0( WM_INITDIALOG, OnInitDialog )
		MSG_HANDLER_0(WM_DESTROY, OnDestroy)
		MSG_WM_SIZE( OnSize )
		MSG_WM_DROPFILES( OnDropFiles )

		BEGIN_NOTIFY_HANDLER_EX
		
			HANDLE_NOTIFICATION_EX_ID( NM_CUSTOMDRAW, IDC_LIST, OnCustomDrawList,	LPNMLVCUSTOMDRAW )
			HANDLE_NOTIFICATION_EX_0( LVN_ITEMCHANGED,			OnSelChange)
		
		END_NOTIFY_HANDLER

		BEGIN_USER_HANDLER_EX
			USER_HANDLER_EX_0(100, OnAdvanced)
		END_USER_HANDLER

		BEGIN_CMD_HANDLER_EX

			CMD_HANDLER_EX_0(IDOK,					OnCmdOK)
			CMD_HANDLER_EX_0(IDCANCEL,				OnCmdCancel)
			CMD_HANDLER_EX_0(IDC_ADD_MEMORY,		OnCmdAddMemory)
			CMD_HANDLER_EX_0(IDC_REMOVE_MEMORY,		OnCmdRemoveMemory)
			CMD_HANDLER_EX_0(IDC_EDIT_MEMORY,		OnCmdEditMemory)
			CMD_HANDLER_EX_0(IDC_UP,				OnCmdUp)
			CMD_HANDLER_EX_0(IDC_DOWN,				OnCmdDown)
			else if ( m_info_view.IsWindow() )
			{
				HWND hwnd = GetFocus() ;
				if ( m_info_view.m_hWnd == hwnd || m_info_view.IsChild( hwnd ) )
				{
					if ( m_info_view.ProcessWindowMessage( hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID ) )
					{
						SetMsgHandled(TRUE); 
						return TRUE ;
					}
					SetMsgHandled(FALSE); 
				}
			}

		END_CMD_HANDLER_EX

	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )

	void dropFile(const CString dropfile) ;

	void setAdvDlgInfo(mem_engine::memory_pointer& mem, CAdvancedMemMgrDlg& dlg);

	void getAdvDlgInfo(mem_engine::memory_pointer& mem, CAdvancedMemMgrDlg& dlg);

	void waitForNavComplete();

	void setUpListView();

	void disableListButtons();

	bool getMemName(mem_engine::memory_pointer& mem);

	void showSavingMessage(mem_engine::memory_pointer& mem);

	wstring get_saving_feedback( mem_engine::memory_pointer& mem );
	void perform_save(mem_engine::memory_pointer& mem);

	bool is_dropped_in_listbox(POINT p);
	CString  get_save_prompt( mem_engine::memory_pointer mem ) ;
	void save_memory(mem_engine::memory_pointer mem) ;
	void set_button_focus() ;

	void add_memory_files( file::OpenDlgList &import_files ) ;

	CString get_save_destination(mem_engine::memory_pointer mem);
};
