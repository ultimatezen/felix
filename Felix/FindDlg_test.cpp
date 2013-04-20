#include "stdafx.h"
#include "FindDlg.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestCFindDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CFindDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_FIND_DLG, CFindDlg::IDD) ;
	}
	// CFindDlg message processing


BOOST_AUTO_TEST_SUITE_END()

#endif
