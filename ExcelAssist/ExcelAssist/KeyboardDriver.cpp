/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "StdAfx.h"
#include "KeyboardDriver.h"
#include "KeyPresser.h"

/*!
 * \brief
 * Write brief comment for CKeyboardDriver here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for CKeyboardDriver here.
 * 
 * \remarks
 * Write remarks for CKeyboardDriver here.
 * 
 * \see
 * Separate items with the '|' character.
 */
CKeyboardDriver::CKeyboardDriver(void)
: mSleepTime(10)
{
}

/*!
 * \brief
 * Write brief comment for ~CKeyboardDriver here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for ~CKeyboardDriver here.
 * 
 * \remarks
 * Write remarks for ~CKeyboardDriver here.
 * 
 * \see
 * Separate items with the '|' character.
 */
CKeyboardDriver::~CKeyboardDriver(void)
{
}

/*!
 * \brief
 * Write brief comment for clearControlKey here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for clearControlKey here.
 * 
 * \remarks
 * Write remarks for clearControlKey here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::clearControlKey()
{
	if ( ::GetKeyState( VK_CONTROL ) & 0x8000 )
	{
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		doSleep() ;
	}
}

/*!
 * \brief
 * Write brief comment for clearAltKey here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for clearAltKey here.
 * 
 * \remarks
 * Write remarks for clearAltKey here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::clearAltKey()
{
	if ( ::GetKeyState( VK_MENU ) & 0x8000 )
	{
		keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
		doSleep() ;
	}
}

/*!
 * \brief
 * Write brief comment for doShiftLeft here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for doShiftLeft here.
 * 
 * \remarks
 * Write remarks for doShiftLeft here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::doShiftLeft()
{
	CKeyPresser shift( VK_SHIFT ) ;
	CKeyPresser left( VK_LEFT ) ;

	doSleep() ;
}

/*!
 * \brief
 * Write brief comment for doRight here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for doRight here.
 * 
 * \remarks
 * Write remarks for doRight here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::doRight()
{
	CKeyPresser right( VK_RIGHT ) ;

	doSleep();
}

/*!
 * \brief
 * Write brief comment for doLeft here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for doLeft here.
 * 
 * \remarks
 * Write remarks for doLeft here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::doLeft()
{
	CKeyPresser left( VK_LEFT ) ;

	doSleep() ;
}

/*!
 * \brief
 * Write brief comment for doShiftRight here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for doShiftRight here.
 * 
 * \remarks
 * Write remarks for doShiftRight here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::doShiftRight()
{
	CKeyPresser shift( VK_SHIFT ) ;
	CKeyPresser right( VK_RIGHT ) ;

	doSleep() ;
}

/*!
 * \brief
 * Write brief comment for doReturn here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for doReturn here.
 * 
 * \remarks
 * Write remarks for doReturn here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::doReturn(void)
{
	CKeyPresser returnKey( VK_RETURN ) ;

	doSleep() ;
}

/*!
 * \brief
 * Write brief comment for doSleep here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for doSleep here.
 * 
 * \remarks
 * Write remarks for doSleep here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::doSleep()
{
	::Sleep(mSleepTime);
}

/*!
 * \brief
 * Write brief comment for doTab here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for doTab here.
 * 
 * \remarks
 * Write remarks for doTab here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CKeyboardDriver::doTab(void)
{
	CKeyPresser tab( VK_TAB ) ;

	doSleep() ;
}
