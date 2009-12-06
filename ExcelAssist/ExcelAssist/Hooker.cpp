/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "stdafx.h"
#include "Hooker.h"
#include "easyunittester.h"
#include "logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

HHOOK	hkb ;
CFelixExcelIF *addin ;
static BOOL shortcuts_enabled = TRUE ;
static boost::function<void(BOOL)> shortcuts_callback = NULL ;

void set_shortcuts_callback(boost::function<void(BOOL)> callback, BOOL current_setting)
{
	shortcuts_callback = callback ;
	shortcuts_enabled = current_setting ;
}

/*!
 * Get the Excel interface.
 */
CFelixExcelIF* getInterface() 
{
	return addin ;
}

bool alt_key_is_pressed( WORD key_message )
{
	bool alt_key_state = false ;
	alt_key_state = !! ( key_message & KF_ALTDOWN ) ;
	return alt_key_state ;
}

/*!
 * Key press spy for shortcuts.
 */
LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProc(int nCode,
													WPARAM wParam, 
													LPARAM lParam)
{
	if ( HC_ACTION != nCode )
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	// get the first word
	WORD wKeystrokeMsg = (WORD)(lParam >> 16);

	if ( wKeystrokeMsg & KF_UP ) // key is up
		return CallNextHookEx(hkb, nCode, wParam, lParam); 


	bool control_key_pressed = !!( ::GetKeyState(VK_CONTROL) & 0x8000 ) ;
	bool alt_key_pressed = alt_key_is_pressed(wKeystrokeMsg) ;

	if (alt_key_pressed &&
		control_key_pressed && 
		wParam == VK_F9)
	{
		shortcuts_enabled = ! shortcuts_enabled ;
		shortcuts_callback(shortcuts_enabled) ;
		MessageBeep(MB_ICONASTERISK) ;
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	if (! shortcuts_enabled)
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

#ifdef _DEBUG
	if ( control_key_pressed && wParam == VK_F2 )
	{
		addin->OnUnitTest() ;
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
				addin->OnLookupNext() ;
				return 1 ;
			}
			CASE( VK_UP ) 
			{
				addin->OnSet() ;
				return 1 ;
			}
			CASE( VK_DOWN ) 
			{
				addin->OnGet() ;
				return 1 ;
			}
			CASE( _T('P') ) 
			{
				addin->OnPrev() ;
				return 1 ;
			}
			CASE( _T('N') ) 
			{
				addin->OnNext() ;
				return 1 ;
			}
			CASE( _T('G') ) 
			{
				addin->OnGetAndNext() ;
				return 1 ;
			}
			CASE( _T('S') ) 
			{
				addin->OnSetAndNext() ;
				return 1 ;
			}
			CASE( _T('L') ) 
			{
				addin->OnLookup() ;
				return 1 ;
			}
			CASE( _T('Z') )
			{
				addin->OnAutoTransSel() ;
			}
			CASE( _T('0') )
			{
				addin->OnEntry( 0 ) ;
				return 1 ;
			}
			CASE( _T('1') )
			{
				addin->OnEntry( 1 ) ;
				return 1 ;
			}
			CASE( _T('2') )
			{
				addin->OnEntry( 2 ) ;
				return 1 ;
			}
			CASE( _T('3') )
			{
				addin->OnEntry( 3 ) ;
				return 1 ;
			}
			CASE( _T('4') )
			{
				addin->OnEntry( 4 ) ;
				return 1 ;
			}
			CASE( _T('5') )
			{
				addin->OnEntry( 5 ) ;
				return 1 ;
			}
			CASE( _T('6') )
			{
				addin->OnEntry( 6 ) ;
				return 1 ;
			}
			CASE( _T('7') )
			{
				addin->OnEntry( 7 ) ;
				return 1 ;
			}
			CASE( _T('8') )
			{
				addin->OnEntry( 8 ) ;
				return 1 ;
			}
			CASE( _T('9') )
			{
				addin->OnEntry( 9 ) ;
				return 1 ;
			}
		}
	}

	return CallNextHookEx( hkb, nCode, wParam, lParam );
}


BOOL installhook( CFelixExcelIF *addy )
{
	addin = addy ;
	hkb=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,_AtlModule.GetResourceInstance(),::GetCurrentThreadId() ); 

	return TRUE;
}


BOOL uninstallhook( CFelixExcelIF *addy )
{
	addy ;

	ATLASSERT( addy == addin ) ;

	return UnhookWindowsHookEx( hkb ) ;
}
