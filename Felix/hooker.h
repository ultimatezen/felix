/*!
	@file hooker.h
	@brief Keyboard listener interface definition file
	@date 2005/06/25
	Time: 13:12:22
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

/**
	@class CKeyboardListener
	@brief We listen for tabs because the wizard eats them.
 */
class CKeyboardListener
{
public:
	virtual bool OnTab( bool bPrev ) = 0 ;
};

BOOL installhook( CKeyboardListener *listener ) ;
BOOL uninstallhook( CKeyboardListener *listener ) ;

