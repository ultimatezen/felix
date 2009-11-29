#include "StdAfx.h"
#include "tag_stripper.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG
namespace easyunit
{
	TEST( test_tag_stripper, test_foobar )
	{
		wstring rich = L"<b>foo</b><font size=3><i>bar</i></font> " ;
		wstring stripped = strip_tags( rich ) ;
		ASSERT_EQUALS( stripped, L"foobar " ) ;
	}
	TEST( test_tag_stripper, test_complex)
	{
		wstring rich = L"<TD>Here <FONT face=\"Monotype Corsiva\"><EM>is <U>some </U>test</EM> text "
			L"with</FONT> <U><FONT color=#ff0000>lots of "
			L"<STRONG>formatting</STRONG></FONT></U>!</TD>" ;
		wstring stripped = strip_tags(rich) ;
		ASSERT_TRUE ( stripped == L"Here is some test text with lots of formatting!" ) ; 
	}
	TEST( test_tag_stripper, test_variations)
	{
		wstring rich = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
		wstring stripped = strip_tags(rich) ;
		ASSERT_TRUE ( stripped == L"Variations on the theme..." ) ; 
	}
	TEST( test_tag_stripper, vertical_tab )
	{
		wstring rich = L"<b>Here</b> is a &#11; (vertical tab)" ;
		wstring stripped = strip_tags( rich ) ;
		ASSERT_EQUALS( stripped, wstring(L"Here is a ") + wchar_t(11) + wstring(L" (vertical tab)") ) ;
	}
}
#endif