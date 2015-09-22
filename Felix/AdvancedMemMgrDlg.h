/*!
	@file AdvancedMemMgrDlg.h
	@brief Advanced memory manager settings
	@date 2005/06/23
	Time: 9:43:24
	@author Ryan Ginstrom
 */
#pragma once

#include "Exceptions.h"			// CException

#include "MessageMapDefines.h"
#include "WindowExceptionHandler.h"
#include "resource.h"


/** Advanced memory manager settings.
	This class is only used in "classic" mode; the new
	memory manager supercedes it.
*/
class CAdvancedMemMgrDlg : 
	public CDialogImpl<CAdvancedMemMgrDlg, CWindow>
	, public CWindowExceptionHandler< CAdvancedMemMgrDlg >
{
	//  CButton                     m_idcancel;
	CButton                     m_idcLocked;
	CCommandEdit                m_idcReliabilityEdit;
	CUpDownCtrl                 m_idcReliabilitySpin;
	CButton                     m_idcValidity;
	//  CButton                     m_idok;

	int                          m_validation ;
	int                          m_reliability ;
	int                          m_lock ;

public:

	static const int IDD = IDD_ADVANCED_MEM_MGR_DLG ;
	DECLARE_SENSING_VAR ;

	CAdvancedMemMgrDlg() : 
		m_validation(BST_UNCHECKED),
		m_reliability(-1),
		m_lock(BST_UNCHECKED)
	{}

	int get_validation() const;
	size_t get_reliability() const;
	int get_lock() const;

	void set_validation(int setting);
	void set_reliability(int setting);
	void set_lock(int setting);

	int parse_reliability( CString reliability_text );

	BEGIN_MAP_EX(CAdvancedMemMgrDlg)

		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID(IDCANCEL, OnCancel)
			CMD_HANDLER_EX_ID(IDOK, OnIdok)
		END_CMD_HANDLER_EX

	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )

	LRESULT OnInitDialog( );
	LRESULT OnCancel(WORD wID) ;
	LRESULT OnIdok(WORD wID) ;

}; // class CAdvancedMemMgrDlg
