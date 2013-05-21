#pragma once

template<class DLG_T>
LRESULT resize_html_view_dialog(DLG_T &dialog, UINT type, CSize &size, UINT box_id)
{
	dialog.SetMsgHandled( FALSE ) ;

	if(dialog.m_bGripper)
	{
		CWindow wndGripper = dialog.GetDlgItem(ATL_IDW_STATUS_BAR);
		if(type == SIZE_MAXIMIZED)
			wndGripper.ShowWindow(SW_HIDE);
		else if(type == SIZE_RESTORED)
			wndGripper.ShowWindow(SW_SHOW);
	}
	if(type != SIZE_MINIMIZED)
	{
		dialog.DlgResize_UpdateLayout(size.cx , size.cy );
	}

	if ( type == SIZE_MINIMIZED )
		return 0L ;

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