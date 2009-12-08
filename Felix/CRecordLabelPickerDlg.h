// This file was generated by WTL Dialog wizard 
// CRecordLabelPickerDlg.h : Declaration of the CCRecordLabelPickerDlg

#pragma once

#include "resource.h"       // main symbols
#include <atlddx.h>
#include <atlframe.h>
#include "atlctrlx.h"		// CCheckListViewCtrl

// CCRecordLabelPickerDlg

class CCRecordLabelPickerDlg : 
	public CDialogImpl<CCRecordLabelPickerDlg, TWindow>,
	public CWinDataExchange<CCRecordLabelPickerDlg>,
	public CDialogResize<CCRecordLabelPickerDlg>	
{
	CCheckListViewCtrl	m_list ;
public:
	static const int IDD = IDD_CRECORDLABELPICKERDLG ;
	DECLARE_SENSING_VAR ;

	CCRecordLabelPickerDlg();
	~CCRecordLabelPickerDlg();

    BEGIN_MSG_MAP(CCRecordLabelPickerDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_SELECT_ALL, OnCmdSelectAll)
		COMMAND_ID_HANDLER(IDC_DESELECT_ALL, OnCmdClearAll)
		CHAIN_MSG_MAP(CDialogResize<CCRecordLabelPickerDlg>)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CCRecordLabelPickerDlg)
	END_DDX_MAP()

	BEGIN_DLGRESIZE_MAP(CCRecordLabelPickerDlg)
		// WebBrowser controls
		BEGIN_DLGRESIZE_GROUP()
			// buttons
			DLGRESIZE_CONTROL(IDOK,						DLSZ_MOVE_X|DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDCANCEL,				DLSZ_MOVE_X|DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDC_SELECT_ALL,		DLSZ_MOVE_X|DLSZ_MOVE_Y)
			DLGRESIZE_CONTROL(IDC_DESELECT_ALL,		DLSZ_MOVE_X|DLSZ_MOVE_Y)

			// resizing controls
			DLGRESIZE_CONTROL(IDC_LIST,				DLSZ_SIZE_X | DLSZ_SIZE_Y )
		END_DLGRESIZE_GROUP()
	END_DLGRESIZE_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCmdSelectAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCmdClearAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


