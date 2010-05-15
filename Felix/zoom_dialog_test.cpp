#include "stdafx.h"
#include "zoom_dialog.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CZoomDlgTest )

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
	BOOST_AUTO_TEST_CASE( init_message )
	{
		CZoomDlg dialog ;
		LRESULT lresult = 0 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lresult) ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("CZoomDlg::OnInitDialog", dialog.m_sensing_variable[0]) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif
