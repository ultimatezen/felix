#include "stdafx.h"
#include "html_processing.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_html_processing )

BOOST_AUTO_TEST_CASE( init )
{
	html_processing processing ;
	BOOST_CHECK ( ! processing.is_underline_specified() ) ; 
	BOOST_CHECK ( ! processing.is_italic_specified() ) ; 
	BOOST_CHECK ( ! processing.is_bold_specified() ) ; 
	BOOST_CHECK ( ! processing.is_font_size_specified() ) ; 
	BOOST_CHECK ( ! processing.is_fore_color_specified() ) ; 
}


BOOST_AUTO_TEST_SUITE_END()
