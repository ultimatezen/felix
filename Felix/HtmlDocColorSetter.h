/*!
	@file HtmlDocColorSetter.h
	@brief Interface for CHtmlDocColorSetter class.
	@date 2005/06/25
	Time: 15:21:39
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "HtmlDocument.h"

namespace html
{

/**
	@class CHtmlDocColorSetter
	@brief A little class to set the background color in an HTML document.
 */
class CHtmlDocColorSetter
{
public:
	CHtmlDocColorSetter(void);
	~CHtmlDocColorSetter(void);
	int SetDocBGColor( CHtmlDocument &doc ) ;
};

}