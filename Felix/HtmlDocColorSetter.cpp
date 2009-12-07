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
		const wstring color_str = BSTR2wstring(doc.get_bg_color()) ;
		const CColorRef oldcolor( color_str ) ;

		// query user for color
		CColorDialog dialog( oldcolor.as_colorref() ) ; // current color is default

		if ( dialog.DoModal() == IDCANCEL )
		{
			return 0 ;
		}

		// get the color the user picked
		const CColorRef newcolor(dialog.GetColor()) ;

		// turn it into an HTML tag
		doc.set_bg_color(newcolor.as_wstring()) ;

		return (int)newcolor.as_colorref() ;
	}
}