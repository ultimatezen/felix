#include "StdAfx.h"
#include "EditTransRecordDialogRegGloss.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

CEditTransRecordDialogRegGloss::CEditTransRecordDialogRegGloss(const bool make_defaults) : m_make_defaults(make_defaults)
{
}

// Function name	: CEditTransRecordDialogRegGloss::OnInitDialog
LRESULT CEditTransRecordDialogRegGloss::OnInitDialog( )
{
	SENSE("OnInitDialog") ;

	initialize_dialog() ;

	fill_from_record( m_old_record ) ;

	// set focus to source edit
	m_source_edit.set_html_focus() ;

	return TRUE ;
}


LRESULT CEditTransRecordDialogRegGloss::OnCancel( )
{
	SENSE("OnCancel"); 
	EndDialog(IDCANCEL);
	return 0L;
}

LRESULT CEditTransRecordDialogRegGloss::OnOK( )
{
	SENSE("OnOK") ;

	try
	{
		m_make_defaults = !! IsDlgButtonChecked(IDC_DEFAULTS_CHECK) ;
		// source
		fill_record() ;
	}
	catch( CException &e )
	{
		e.add_to_message( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
		e.notify_user(0) ;
		return 0L ;
	}

	EndDialog(IDOK);
	return 0L;
}

void CEditTransRecordDialogRegGloss::fill_record()
{
	fill_rec_from_gui(m_old_record) ;

	m_old_record->modify() ;
}

void CEditTransRecordDialogRegGloss::set_record( record_type record )
{
	m_old_record = record ; 
}


CEditTransRecordDialogRegGloss::record_type CEditTransRecordDialogRegGloss::get_record()
{
	return m_old_record ;
}

LRESULT CEditTransRecordDialogRegGloss::OnEditStrings()
{
	edit_strings(m_old_record) ;
	return 0L ;
}

LRESULT CEditTransRecordDialogRegGloss::OnAddString()
{
	add_string(m_old_record) ;
	return 0L ;
}

LRESULT CEditTransRecordDialogRegGloss::OnSize( UINT type, CSize size )
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

	// size defaults checkbox
	TWindow context_box = GetDlgItem(IDC_CONTEXT_BOX) ;
	CWindowRect context_rect(context_box) ;
	ScreenToClient(&context_rect) ;
	TWindow defaults_check = GetDlgItem(IDC_DEFAULTS_CHECK) ;
	CWindowRect defaults_rect(defaults_check) ;
	ScreenToClient(&defaults_rect) ;
	const int height = defaults_rect.Height() ;
	const int width = defaults_rect.Width() ;
	const int padding = 7 ;
	defaults_rect.left = context_rect.left ;
	defaults_rect.top = context_rect.bottom + padding ;
	defaults_rect.bottom = defaults_rect.top + height ;
	defaults_rect.right = defaults_rect.left + width ;
	defaults_check.SetWindowPos(NULL, &defaults_rect, SWP_NOZORDER | SWP_NOACTIVATE);

	m_extra_strings_view.handle_size() ;

	return 0L ;
}