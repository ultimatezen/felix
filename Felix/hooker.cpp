/********************************************************************
	created:	2004/11/09
	created:	9:11:2004   11:17
	filename: 	C:\Programming\Programming Projects\Felix Interface for Word\hooker.cpp
	file path:	C:\Programming\Programming Projects\Felix Interface for Word
	file base:	hooker
	file ext:	cpp
	author:		Ryan Ginstrom
	
	purpose:	Keyboard hook
*********************************************************************/

#include "stdafx.h"
#include ".\hooker.h"

#include "DebugUtilities.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

static HHOOK hkb = NULL ;
static CKeyboardListener *kb_listener = NULL ;

bool shift_key_is_pressed() ;
bool control_key_is_pressed() ;
bool alt_key_is_pressed( WORD key_message ) ;

bool keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg ) ;

bool shift_key_is_pressed()
{
	bool shift_key_state = false ;
	shift_key_state = !! ( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;
	return shift_key_state ;
}

bool control_key_is_pressed()
{
	bool control_key_state = false ;
	control_key_state = !! ( ::GetKeyState(VK_CONTROL) & 0x8000 ) ;
	return control_key_state ;
}

bool alt_key_is_pressed( WORD key_message )
{
	bool alt_key_state = false ;
	alt_key_state = !! ( key_message & KF_ALTDOWN ) ;
	return alt_key_state ;
}

LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProc(int nCode,WPARAM wParam, 
															  LPARAM lParam)
{
	static const int stop_flags = ( KF_UP | KF_MENUMODE ) ;

	if ( HC_ACTION != nCode )
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	// get the first word
	WORD wKeystrokeMsg = (WORD)(lParam >> 16);

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
	
	ATLTRACE( "*** KEYBOARD ACTION\n" ) ;

	if ( ! keyboard_proc( wParam, wKeystrokeMsg ) ) 
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	return 1 ;

}

BOOL installhook( CKeyboardListener *listener )
{
	ATLASSERT ( kb_listener == NULL ) ; 
	
	kb_listener = listener ;
   hkb = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,_Module.GetModuleInstance(),::GetCurrentThreadId() ); 
	
   return TRUE;
}

BOOL uninstallhook( CKeyboardListener *listener )
{
	listener ;
	ATLASSERT( listener == kb_listener ) ;

	kb_listener = NULL ;

	return UnhookWindowsHookEx( hkb ) ;
}


/*****************************************************************/
/*  keyboard_proc                                                */
/*****************************************************************/

bool keyboard_proc( WPARAM wParam, WORD /*wKeystrokeMsg*/ )
{
	if ( wParam == VK_TAB)
	{
		return kb_listener->OnTab( shift_key_is_pressed() ) ;
	}

	return false ;
}
