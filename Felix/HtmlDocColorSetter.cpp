#include "StdAfx.h"
#include ".\htmldoccolorsetter.h"

namespace html
{
	CHtmlDocColorSetter::CHtmlDocColorSetter(void)
	{
	}

	CHtmlDocColorSetter::~CHtmlDocColorSetter(void)
	{
	}

	int CHtmlDocColorSetter::SetDocBGColor( CHtmlDocument &doc )
	{
		wstring color_str = doc.get_bg_color() ;
		CColorRef color( color_str ) ;

		// query user for color
		CColorDialog dialog( color.as_colorref() ) ; // current color is default

		if ( dialog.DoModal() == IDCANCEL )
			return false ;

		// get the color the user picked
		color = dialog.GetColor() ;

		// turn it into an HTML tag
		doc.set_bg_color( color.as_wstring() ) ;

		return (int)color.as_colorref() ;
	}
}