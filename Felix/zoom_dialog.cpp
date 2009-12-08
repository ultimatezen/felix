#include "stdafx.h"
#include "zoom_dialog.h"

static const int ZOOM_MIN = -10 ;
static const int ZOOM_MAX = 10 ;
static const int TIC_FREQ = 1 ;

LRESULT CZoomDlg::OnInitDialog()
{
	SENSE("OnInitDialog") ;

	m_ZoomCtrl.Attach(GetDlgItem(IDC_SLIDER1)) ;
	m_ZoomCtrl.SetRange(ZOOM_MIN, ZOOM_MAX);
	m_ZoomCtrl.SetTicFreq(TIC_FREQ) ;
	m_ZoomCtrl.SetPos(m_zoom_level);
	m_ZoomCtrl.EnableWindow();
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	return TRUE;
}

LRESULT CZoomDlg::OnOK( WORD wID )
{
	SENSE("OnOK") ;

	EndDialog(wID);
	return 0;
}

LRESULT CZoomDlg::OnCancel( WORD wID )
{
	SENSE("OnCloseCommand") ;

	EndDialog(wID);
	return 0;
}

void CZoomDlg::OnZoomCtrl( int /*iType*/, short /*iTrackPos*/, HWND /*hWndTrackBar*/ )
{
	const int thepos = m_ZoomCtrl.GetPos() ;
	if (thepos != m_zoom_level)
	{
		m_zoom_level = thepos ;
		m_interface->set_zoom_level(thepos) ;
	}
}