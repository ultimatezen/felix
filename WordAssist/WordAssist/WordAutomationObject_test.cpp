#include "stdafx.h"
#include "WordAutomationObject.h"

#ifdef UNIT_TEST
#include "easyunit/testharness.h"

namespace easyunit
{
	// WordFont

	TEST( WordFontTest, Instantiate )
	{
		try
		{
			WordFont font ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordFont class in test harness" ) ;
		}
	}

	// WordRange

	TEST( WordRangeest, Instantiate )
	{
		try
		{
			WordRange range ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordRange class in test harness" ) ;
		}
	}

	// WordWords

	TEST( WordWordsTest, Instantiate )
	{
		try
		{
			WordWords words ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordWords class in test harness" ) ;
		}
	}

	// WordCharacters

	TEST( WordCharactersTest, Instantiate )
	{
		try
		{
			WordCharacters characters ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordCharacters class in test harness" ) ;
		}
	}

	// WordSelection

	TEST( WordSelectionTest, Instantiate )
	{
		try
		{
			WordSelection selection ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordSelection class in test harness" ) ;
		}
	}


	// WordBookmark

	TEST( WordBookmarkTest, Instantiate )
	{
		try
		{
			WordBookmark bookmark ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordBookmark class in test harness" ) ;
		}
	}

	// WordBookmarks

	TEST( WordBookmarksTest, Instantiate )
	{
		try
		{
			WordBookmarks bookmarks ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordBookmarks class in test harness" ) ;
		}
	}


	// WordDocument

	TEST( WordDocumentTest, Instantiate )
	{
		try
		{
			WordDocument document ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordDocument class in test harness" ) ;
		}
	}

	// WordApplication

	TEST( WordApplicationTest, Instantiate )
	{
		try
		{
			WordApplication application ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordApplication class in test harness" ) ;
		}
	}

}
#endif
