/*!
	@file system_message.h
	@brief interface for system_message class
	@date 2004/12/28
	Time: 4:16:53
	@author Ryan Ginstrom
	@dir c:\programming\mylibrary
 */

#include "stdafx.h"
#include "system_message.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_system_message )

BOOST_AUTO_TEST_CASE( test_found_x_matches )
{
	BOOST_CHECK ( _T("Found 2 matches." ) == system_message( IDS_FOUND_X_MATCHES, int_arg( 2 ) ) ) ;
	BOOST_CHECK ( "Found 2 matches." == system_message_a( IDS_FOUND_X_MATCHES, int_arg_a( 2 ) ) ) ;
	BOOST_CHECK ( L"Found 2 matches." == system_message_w( IDS_FOUND_X_MATCHES, int_arg_w( 2 ) ) ) ;
}

BOOST_AUTO_TEST_SUITE_END()
