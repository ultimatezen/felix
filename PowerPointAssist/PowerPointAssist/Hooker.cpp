#include "stdafx.h"
#include "Hooker.h"
#include "logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

using namespace except;

HHOOK	hkb ;
CPowerPointInterface *addin ;
static BOOL shortcuts_enabled = TRUE ;
static boost::function<void(BOOL)> shortcuts_callback = NULL ;

void set_shortcuts_callback(boost::function<void(BOOL)> callback, BOOL current_setting)
{
	shortcuts_callback = callback ;
	shortcuts_enabled = current_setting ;
}

CPowerPointInterface* getInterface() 
{
	return addin ;
}

//////////////////////////////////////////////////////////////////////////
// Check for various key-press states
//////////////////////////////////////////////////////////////////////////

bool shift_key_is_pressed()
{
	return !! ( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;
}
bool control_key_is_pressed()
{
	return !! ( ::GetKeyState(VK_CONTROL) & 0x8000 ) ;
}

bool alt_key_is_pressed( WORD key_message )
{
	return !! ( key_message & KF_ALTDOWN ) ;
}

//////////////////////////////////////////////////////////////////////////
// Keyboard listener proc
//////////////////////////////////////////////////////////////////////////

LRESULT __declspec(dllexport)__stdcall  CALLBACK KeyboardProc(int nCode,
															  WPARAM wParam, 
															  LPARAM lParam)
{
	if ( HC_ACTION != nCode )
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	// get the first word
	const WORD wKeystrokeMsg = (WORD)(lParam >> 16);

	if ( wKeystrokeMsg & KF_UP ) // key is up
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	const bool control_key_pressed = control_key_is_pressed() ;

	const bool alt_key_pressed = alt_key_is_pressed(wKeystrokeMsg) ;

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
		addin->OnUnitTestAction() ;
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

	if (control_key_pressed && wParam == VK_RIGHT && ! alt_key_pressed )
	{
		if ( ::GetKeyState( VK_SHIFT ) & 0x8000 )
		{
			return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
		}

		if ( FAILED( addin->OnExtendLookupAction( shift_key_is_pressed() ) ) )
			return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
		return 1 ;
	}

	if ( ! alt_key_pressed ) // alt key is up
	{
		return CallNextHookEx(hkb, nCode, wParam, lParam); 
	}

	if ( control_key_pressed )
	{
		switch ( wParam )
		{
			CASE( VK_RIGHT ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnLookupNextTransAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( VK_UP ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnCorrectTransAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('E') ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnExtendTransLookupAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('L') ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnLookupTransAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('C') )
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnTransConcordanceAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
		}
	}
	else
	{
		switch ( wParam )
		{
			CASE( VK_RIGHT ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnLookupNextAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( VK_UP ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnSetAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( VK_DOWN ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnGetAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('P') ) 
			{
				if ( FAILED( addin->OnPrevAction( ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('N') ) 
			{
				if ( FAILED( addin->OnNextAction( ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('G') ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnGetAndNextAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('S') ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnSetAndNextAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('L') ) 
			{
				addin->m_is_auto = false ;
				if ( FAILED( addin->OnLookupAction( shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('C') )
			{
				if ( FAILED( addin->OnConcordanceAction( shift_key_is_pressed() ) )	)
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('Z') )
			{
				if ( FAILED( addin->OnAutoTransFuzzyAction( ) )	)
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('D') )
			{
				if ( FAILED( addin->OnDeleteAction( ) )	)
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('0') )
			{
				if ( FAILED( addin->OnEntryAction( 0, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('1') )
			{
				if ( FAILED( addin->OnEntryAction( 1, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('2') )
			{
				if ( FAILED( addin->OnEntryAction( 2, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('3') )
			{
				if ( FAILED( addin->OnEntryAction( 3, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('4') )
			{
				if ( FAILED( addin->OnEntryAction( 4, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('5') )
			{
				if ( FAILED( addin->OnEntryAction( 5, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('6') )
			{
				if ( FAILED( addin->OnEntryAction( 6, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('7') )
			{
				if ( FAILED( addin->OnEntryAction( 7, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('8') )
			{
				if ( FAILED( addin->OnEntryAction( 8, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
			CASE( _T('9') )
			{
				if ( FAILED( addin->OnEntryAction( 9, shift_key_is_pressed() ) ) )
					return CallNextHookEx( hkb, nCode, wParam, lParam ) ;
				return 1 ;
			}
		}
	}

	return CallNextHookEx( hkb, nCode, wParam, lParam );
}

// installhook
BOOL installhook( CPowerPointInterface *addy )
{
	addin = addy ;
	HINSTANCE res ;
#ifdef UNIT_TEST
	res = NULL ;
#else
	res = _AtlModule.GetResourceInstance()
#endif

	hkb=SetWindowsHookEx(WH_KEYBOARD,
						(HOOKPROC)KeyboardProc,
						res,
						::GetCurrentThreadId() ); 

	return TRUE;
}

// uninstallhook
BOOL uninstallhook( CPowerPointInterface *addy )
{
	addy ;

	ATLASSERT( addy == addin ) ;

	return UnhookWindowsHookEx( hkb ) ;
}
