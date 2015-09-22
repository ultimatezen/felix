#include "stdafx.h"
#include "WordAutomationObject.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST


BOOST_AUTO_TEST_SUITE( test_symbol2unicode )

	BOOST_AUTO_TEST_CASE( test_0x22 )
	{
		_variant_t symbol(0x22 - 0x1000) ;

		wchar_t expected = static_cast<wchar_t>(0x2200) ;
		wchar_t actual = symbol2unicode(symbol) ;
		TRACE(actual) ;
		BOOST_MESSAGE("Testing symbol2unicode") ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( WordFontTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordFont()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordRangeTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordRange()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordWordsTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordWords()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordCharactersTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordCharacters()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordSelectionTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordSelection()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordBookmarkTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordBookmark()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordBookmarksTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordBookmarks()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordDocumentTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordDocument()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( WordApplicationTest )

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(WordApplication()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
