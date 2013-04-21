//! Keyboard hooks to implement keyboard shortcuts.
#include "stdafx.h"
#include "logging.h"
#include "hooker.h"

#include "DebugUtilities.h"
#include "KeyboardListener.h"

using namespace except ;

static HHOOK hkb = NULL ;
shortcuts::KeyboardShortcuts *hook_keys = NULL ;
static bool shortcuts_enabled = true ;

bool control_key_is_pressed() ;

bool classic_keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg ) ;
bool new_keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg ) ;
bool process_ctl_right() ;

LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProc(int nCode,
															  WPARAM wParam, 
															  LPARAM lParam)
{
	static const int stop_flags = ( KF_DLGMODE | KF_UP | KF_MENUMODE ) ;

	if ( HC_ACTION != nCode )
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	try
	{
		// get the first word
		const WORD wKeystrokeMsg = (WORD)(lParam >> 16);

		// if a dialog is being displayed, quit processing
		// if the key is up, quit processing 
		// if we are manipulating a menu, quit processing
		if ( wKeystrokeMsg &  stop_flags )
		{
			return CallNextHookEx(hkb, nCode, wParam, lParam); 
		}

		if ( wParam == VK_MENU || wParam == VK_CONTROL ) 
		{
			return CallNextHookEx(hkb, nCode, wParam, lParam); 
		}

		if ( ! new_keyboard_proc( wParam, wKeystrokeMsg ) ) 
		{
			return CallNextHookEx(hkb, nCode, wParam, lParam); 
		}
	}
	catch(CException &e)
	{
		logging::log_error("Error in keyboard hook process") ;
		logging::log_exception(e) ;
	}
	catch(...)
	{
		logging::log_error("Error in keyboard hook process") ;
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	return 1 ;
}

void set_listener( shortcuts::KeyboardShortcuts *listener )
{
	hook_keys = listener ;
}

BOOL installhook( shortcuts::KeyboardShortcuts *listener )
{
	set_listener(listener) ;
    hkb=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,_AtlModule.GetResourceInstance(),::GetCurrentThreadId() ); 
	
    return TRUE;
}

BOOL uninstallhook( shortcuts::KeyboardShortcuts *listener )
{
	listener ;
	ATLASSERT( listener == hook_keys ) ;

	return UnhookWindowsHookEx( hkb ) ;
}


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*  new_keyboard_proc                                                   */
/************************************************************************/

bool new_keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg )
{
	const bool control_key_pressed	= control_key_is_pressed() ;
	const bool alt_key_pressed		= alt_key_is_pressed(wKeystrokeMsg) ;

#ifdef _DEBUG
if ( control_key_pressed && wParam == VK_F3 )
	{
		ATLTRACE( "Creating exception...\n" ) ;
		try
		{
			int x = 3 ;
			int y = 5 ;
			int z = y / (x - 3) ;
			z = 0 ;
		}
		catch (CSWException &e)
		{
			logging::send_report(L"Japanese", e.get_filename()) ;
		}
		return true ;
	}
#endif

	if (control_key_pressed && alt_key_pressed && wParam == VK_F9)
	{
		::MessageBeep(MB_ICONINFORMATION) ;
		shortcuts_enabled = ! shortcuts_enabled ;
		return true ;
	}
	else if (! shortcuts_enabled)
	{
		return false ;
	}

	return hook_keys->receive_keypress(control_key_pressed, alt_key_pressed, static_cast<wchar_t>(wParam)) ;
}