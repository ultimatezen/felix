#include "StdAfx.h"
#include "ClipboardBackup.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( CClipboardBackupTest )


	BOOST_AUTO_TEST_CASE( test_Initialize )
	{
		BOOST_CHECK_NO_THROW(CClipboardBackup()) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif