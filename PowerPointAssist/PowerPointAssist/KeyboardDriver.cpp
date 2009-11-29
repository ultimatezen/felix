#include "StdAfx.h"
#include "KeyboardDriver.h"

CKeyboardDriver::CKeyboardDriver(void)
: mSleepTime(10)
{
}

CKeyboardDriver::~CKeyboardDriver(void)
{
}

void CKeyboardDriver::clearControlKey()
{
	if ( ::GetKeyState( VK_CONTROL ) & 0x8000 )
	{
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		doSleep() ;
	}
}

void CKeyboardDriver::clearAltKey()
{
	if ( ::GetKeyState( VK_MENU ) & 0x8000 )
	{
		keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
		doSleep() ;
	}
}

void CKeyboardDriver::doShiftLeft()
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(VK_LEFT, 0, 0, 0);

	doSleep() ;

	// stop pressing "Alt-Tab"
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
}

void CKeyboardDriver::doRight()
{
	keybd_event(VK_RIGHT, 0, 0, 0);

	doSleep();

	// stop pressing right arrow
	keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
}

void CKeyboardDriver::doLeft()
{
	keybd_event(VK_LEFT, 0, 0, 0);

	doSleep() ;

	// stop pressing "Alt-Tab"
	keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
}

void CKeyboardDriver::doShiftRight()
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(VK_RIGHT, 0, 0, 0);

	doSleep() ;

	// stop pressing "Alt-Tab"
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
}

void CKeyboardDriver::doSleep()
{
	::Sleep(mSleepTime);
}

void CKeyboardDriver::doReturn(void)
{
	keybd_event(VK_RETURN, 0, 0, 0);

	doSleep() ;

	// stop pressing return key
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}
