#pragma once

template<class DLG_T>
void handle_dialog_gripper(DLG_T &dialog, UINT type)
{
	if(! dialog.m_bGripper)
	{
		return ;
	}
	CWindow wndGripper = dialog.GetDlgItem(ATL_IDW_STATUS_BAR);
	if(type == SIZE_MAXIMIZED)
	{
		wndGripper.ShowWindow(SW_HIDE);
	}
	else if(type == SIZE_RESTORED)
	{
		wndGripper.ShowWindow(SW_SHOW);
	}
}

template<class DLG_T>
bool handle_common_resize_tasks(DLG_T &dialog, UINT type, CSize &size)
{
	dialog.SetMsgHandled( FALSE ) ;

	handle_dialog_gripper(dialog, type) ;
	if(type == SIZE_MINIMIZED)
	{
		return false ;
	}

	dialog.DlgResize_UpdateLayout(size.cx , size.cy );
	return true ;
}


template<class DLG_T>
LRESULT resize_html_view_dialog(DLG_T &dialog, UINT type, CSize &size, UINT box_id)
{
	if (! handle_common_resize_tasks(dialog, type, size))
	{
		return 0L ;
	}
	// set up static where html view will go
	CStatic msg_box ;
	msg_box.Attach( dialog.GetDlgItem( box_id ) ) ;
	// get its position
	RECT rc ;
	msg_box.GetWindowRect( &rc ) ;
	dialog.ScreenToClient( &rc ) ;
	msg_box.ShowWindow( SW_HIDE ) ;

	// move the html view into place
	dialog.position_view(rc) ;

	return 0L ;
}