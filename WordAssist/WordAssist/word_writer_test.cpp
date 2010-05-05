#include "stdafx.h"
#include "word_writer.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( word_writer_test )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		WordSelection selection ;
		word_writer writer(selection) ;
		BOOST_CHECK(writer.m_write_font_face) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
