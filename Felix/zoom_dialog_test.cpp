#include "stdafx.h"
#include "zoom_dialog.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	class FakeZoomDialogInterface : public CZoomInterface
	{
	public:
		int m_zoom_level ;
		FakeZoomDialogInterface() : m_zoom_level(0){}
		void set_zoom_level(int zoom_level)
		{
			m_zoom_level = zoom_level ;
		}
	};
	TEST( CZoomDlgTest, init_message )
	{
		CZoomDlg dialog ;
		LRESULT lresult = 0 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lresult) ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V("CZoomDlg::OnInitDialog", SimpleString(dialog.m_sensing_variable[0].c_str())) ;
	}
}
#endif
