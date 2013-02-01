#include "stdafx.h"
#include "Hooker.h"
#include "logging.h"

using namespace except;

HHOOK	hkb ;
shortcuts::KeyboardShortcuts *hook_keys ;
static BOOL shortcuts_enabled = TRUE ;


shortcuts::KeyboardShortcuts* getInterface() 
{
	return hook_keys ;
}

//////////////////////////////////////////////////////////////////////////
// Keyboard listener proc
//////////////////////////////////////////////////////////////////////////

LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProc(int nCode,
															  WPARAM wParam, 
															  LPARAM lParam)
{
	static const int stop_flags = ( KF_DLGMODE | KF_UP | KF_MENUMODE ) ;
	if ( HC_ACTION != nCode )
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	// get the first word
	const WORD wKeystrokeMsg = (WORD)(lParam >> 16);

	if ( wKeystrokeMsg & stop_flags ) // key is up
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}
	if ( wParam == VK_MENU || wParam == VK_CONTROL ) 
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	const bool control_key_pressed = control_key_is_pressed() ;
	const bool alt_key_pressed = alt_key_is_pressed(wKeystrokeMsg) ;

	// This toggles keyboard shortcuts (hard coded...)
	if (control_key_pressed && alt_key_pressed && wParam == VK_F9)
	{
		::MessageBeep(MB_ICONINFORMATION) ;
		shortcuts_enabled = ! shortcuts_enabled ;
		return 1 ;
	}
	// Don't react to control + shift combo
	else if (control_key_pressed && shift_key_is_pressed())
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}
	// If shortcuts are disabled, ignore any keystrokes
	else if (! shortcuts_enabled)
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}


	if (hook_keys->receive_keypress(control_key_pressed, alt_key_pressed, static_cast<wchar_t>(wParam)))
	{
		return 1 ;
	}
	return CallNextHookEx(hkb, nCode, wParam, lParam); 

}

// installhook
BOOL installhook( shortcuts::KeyboardShortcuts *keys )
{
	hook_keys = keys ;
	HINSTANCE res ;
#ifdef UNIT_TEST
	res = NULL ;
#else
	res = _AtlModule.GetResourceInstance() ;
#endif

	hkb=SetWindowsHookEx(WH_KEYBOARD,
						(HOOKPROC)KeyboardProc,
						res,
						::GetCurrentThreadId() ); 

	return TRUE;
}

// uninstallhook
BOOL uninstallhook( shortcuts::KeyboardShortcuts *keys )
{
	keys ;

	ATLASSERT( hook_keys == keys ) ;

	return UnhookWindowsHookEx( hkb ) ;

}
