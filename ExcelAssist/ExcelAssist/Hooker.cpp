/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "stdafx.h"
#include "Hooker.h"
#include "logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

using namespace except ;

HHOOK	hkb ;
shortcuts::KeyboardShortcuts *hook_keys ;


/*!
 * Get the Excel interface.
 */
shortcuts::KeyboardShortcuts* getInterface() 
{
	return hook_keys ;
}

/*!
 * Key press spy for shortcuts.
 */
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

	if (hook_keys->receive_keypress(control_key_pressed, alt_key_pressed, static_cast<wchar_t>(wParam)))
	{
		return 1 ;
	}
	return CallNextHookEx(hkb, nCode, wParam, lParam); 

	/*
	if (! shortcuts_enabled)
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

#ifdef _DEBUG
	if ( control_key_pressed && wParam == VK_F2 )
	{
		hook_keys->OnUnitTest() ;
		return 1 ;
	}
	else if ( control_key_pressed && wParam == VK_F3 )
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

	if ( ! alt_key_pressed ) // alt key is up
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}
	else
	{
		switch ( wParam )

		{
			CASE( VK_RIGHT ) 
			{
				hook_keys->OnLookupNext() ;
				return 1 ;
			}
			CASE( VK_UP ) 
			{
				hook_keys->OnSet() ;
				return 1 ;
			}
			CASE( VK_DOWN ) 
			{
				hook_keys->OnGet() ;
				return 1 ;
			}
			CASE( _T('P') ) 
			{
				hook_keys->OnPrev() ;
				return 1 ;
			}
			CASE( _T('N') ) 
			{
				hook_keys->OnNext() ;
				return 1 ;
			}
			CASE( _T('G') ) 
			{
				hook_keys->OnGetAndNext() ;
				return 1 ;
			}
			CASE( _T('S') ) 
			{
				hook_keys->OnSetAndNext() ;
				return 1 ;
			}
			CASE( _T('L') ) 
			{
				hook_keys->OnLookup() ;
				return 1 ;
			}
			CASE( _T('Z') )
			{
				hook_keys->OnAutoTransSel() ;
			}
			CASE( _T('0') )
			{
				hook_keys->OnEntry( 0 ) ;
				return 1 ;
			}
			CASE( _T('1') )
			{
				hook_keys->OnEntry( 1 ) ;
				return 1 ;
			}
			CASE( _T('2') )
			{
				hook_keys->OnEntry( 2 ) ;
				return 1 ;
			}
			CASE( _T('3') )
			{
				hook_keys->OnEntry( 3 ) ;
				return 1 ;
			}
			CASE( _T('4') )
			{
				hook_keys->OnEntry( 4 ) ;
				return 1 ;
			}
			CASE( _T('5') )
			{
				hook_keys->OnEntry( 5 ) ;
				return 1 ;
			}
			CASE( _T('6') )
			{
				hook_keys->OnEntry( 6 ) ;
				return 1 ;
			}
			CASE( _T('7') )
			{
				hook_keys->OnEntry( 7 ) ;
				return 1 ;
			}
			CASE( _T('8') )
			{
				hook_keys->OnEntry( 8 ) ;
				return 1 ;
			}
			CASE( _T('9') )
			{
				hook_keys->OnEntry( 9 ) ;
				return 1 ;
			}
		}
	}

	return CallNextHookEx( hkb, nCode, wParam, lParam );
*/
}


BOOL installhook( shortcuts::KeyboardShortcuts *addy )
{
	hook_keys = addy ;
	hkb=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,_AtlModule.GetResourceInstance(),::GetCurrentThreadId() ); 

	return TRUE;
}


BOOL uninstallhook( shortcuts::KeyboardShortcuts *addy )
{
	addy ;

	ATLASSERT( addy == hook_keys ) ;

	return UnhookWindowsHookEx( hkb ) ;
}
