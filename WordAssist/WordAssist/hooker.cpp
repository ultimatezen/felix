//! Keyboard hooks to implement keyboard shortcuts.
#include "stdafx.h"
#include "hooker.h"

#include "DebugUtilities.h"
#include "KeyboardListener.h"
#include "easyunittester.h"
#include "logging.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

#define ENSURE_ACTIVE if ( ! kb_listener->IsActive() ) return false ; 

using namespace except ;

static HHOOK hkb = NULL ;
static CKeyboardListener *kb_listener = NULL ;
static bool classic_interface = false ;
static bool in_shift_state = false ;
static BOOL shortcuts_enabled = TRUE ;
static boost::function<void(BOOL)> shortcuts_callback = NULL ;

bool control_key_is_pressed() ;

bool classic_keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg ) ;
bool new_keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg ) ;
bool process_ctl_right() ;

void set_shortcuts_callback(boost::function<void(BOOL)> callback, BOOL current_setting)
{
	shortcuts_callback = callback ;
	shortcuts_enabled = current_setting ;
}

void set_classic_if_on()
{
	classic_interface = true ;
}
void set_classic_if_off()
{
	classic_interface = false ;
}

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
		WORD wKeystrokeMsg = (WORD)(lParam >> 16);

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

		// This turns the keyboard shortcuts off and on
		if (alt_key_is_pressed(wKeystrokeMsg) &&
			control_key_is_pressed() && 
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


		ATLTRACE( "*** KEYBOARD ACTION\n" ) ;

		if ( classic_interface ) 
		{
			if ( ! classic_keyboard_proc( wParam, wKeystrokeMsg ) ) 
			{
				return CallNextHookEx(hkb, nCode, wParam, lParam); 
			}
		}
		else 
		{
			if ( ! new_keyboard_proc( wParam, wKeystrokeMsg ) ) 
			{
				return CallNextHookEx(hkb, nCode, wParam, lParam); 
			}
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

void set_listener( CKeyboardListener *listener )
{
	kb_listener = listener ;
}

BOOL installhook( CKeyboardListener *listener )
{
	set_listener(listener) ;
    hkb=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,_AtlModule.GetResourceInstance(),::GetCurrentThreadId() ); 
	
    return TRUE;
}

BOOL uninstallhook( CKeyboardListener *listener )
{
	listener ;
	ATLASSERT( listener == kb_listener ) ;

	return UnhookWindowsHookEx( hkb ) ;
}

/************************************************************************/
/*  process_ctl_right                                                   */
/************************************************************************/
bool process_ctl_right()
{
	if ( in_shift_state) 
	{
		in_shift_state = false ;
		return false ;
	}
	
	if ( shift_key_is_pressed() )
	{
		return false ;
	}
	
	ENSURE_ACTIVE
		
	//	"&Extend Lookup Sentence (Ctrl + R ARR)"
	return kb_listener->OnExtendLookupAction( false ) ;
}

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*  new_keyboard_proc                                                   */
/************************************************************************/

bool new_keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg )
{
	bool control_key_pressed	= control_key_is_pressed() ;
	bool alt_key_pressed		= alt_key_is_pressed(wKeystrokeMsg) ;

#ifdef _DEBUG
	if ( control_key_pressed && wParam == VK_F2 )
	{
		ATLTRACE( "Unit testing app...\n" ) ;
		run_unit_tests() ;
		return true ;
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

	// Our only keyboard shortcut that does not require the alt key to be pressed
	// is "extend lookup" (Ctrl + right arrow).
	// This is a special case, because additionally if there is no selection (nothing to be extended!)
	// we want to revert to the default handling

	/************************************************************************/
	/* Exception: Control key is pressed                                    */
	/************************************************************************/

	if (	control_key_pressed		&& 
			wParam == VK_RIGHT		&& 
			! alt_key_pressed 
		)
	{
		return process_ctl_right() ;
	}

	/************************************************************************/
	/* We are in a shift state                                              */
	/************************************************************************/

	if ( in_shift_state ) 
	{
		in_shift_state = false ;
		
		// Use the shift key to set/get text as plaintext. Otherwise the default (html) is retrieved
		bool shift_key_pressed = shift_key_is_pressed() ;
		
		switch( wParam )
		{
			CASE( _T('C') )
				{
					// skip if doc not active
					ENSURE_ACTIVE
						
					return kb_listener->OnConcordanceAction( shift_key_pressed ) ;
				}
			CASE( _T('A') )
				{
					// skip if doc not active
					ENSURE_ACTIVE
						
					return kb_listener->OnAutoTransAction( shift_key_pressed ) ;
				}
			CASE( _T('G') )
				{
					// skip if doc not active
					ENSURE_ACTIVE
						
					return kb_listener->OnRegisterGlossAction( shift_key_pressed ) ;
				}
			CASE( _T('H') )
				{
					// skip if doc not active
					ENSURE_ACTIVE
						
					return kb_listener->OnGlossNAction( shift_key_pressed ) ;
				}
			CASE( _T('S') )
				{
					// skip if doc not active
					ENSURE_ACTIVE
						
					return kb_listener->OnSaveMemoryAction( ) ;
				}
			default:
				return false ; 
				
		}
	
	}
	
	// Since we have already handled the exceptions, the ALT key must be pressed for all other shortcuts
	
	if ( ! alt_key_pressed ) // alt key is up
	{
		return false; 
	}
	
	// alt_key_pressed tests the flag in our hook message; this asserts that
	// the alt key is actually depressed.
	ATLASSERT( ::GetKeyState(VK_MENU) & 0x8000 ) ;
	ATLASSERT( alt_key_pressed ) ;

	/************************************************************************/
	/* Alt key pressed                                                      */
	/************************************************************************/

	// Use the shift key to set/get text as plaintext. Otherwise the default (html) is retrieved
	bool shift_key_pressed = shift_key_is_pressed() ;
	
	switch ( wParam )
	{
		//	"&Look up Next Sentence (ALT + R ARR)"
		CASE( VK_RIGHT ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
				kb_listener->set_auto_off() ;
			return kb_listener->OnLookupNextAction( shift_key_pressed ) ;
		}
		//	"&Register Current Translation (ALT + UP ARR)"
		CASE( VK_UP ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			if(control_key_pressed)
			{
				return kb_listener->OnCorrectAction(shift_key_pressed) ;
			}
			return kb_listener->OnSetAction(shift_key_pressed) ;
		}
		//	"Get C&urrent Translation (ALT + DWN ARR)"
		CASE( VK_DOWN ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnGetAction( shift_key_pressed ) ;
		}
		CASE( _T('.') )
		CASE( VK_OEM_PERIOD )
		{
			// skip if doc not active
			ENSURE_ACTIVE

				kb_listener->set_auto_off() ;
			return kb_listener->OnToMaruAction( shift_key_pressed ) ;
		}
		//	"&Previous Translation (ALT + P)"
		CASE( _T('P') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnPrevAction( ) ;
		}
		//	"&Next Translation (ALT + N)"
		CASE( _T('N') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnNextAction( ) ;
		}
		//	"&Get And Next (ALT + G)"
		CASE( _T('G') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
				kb_listener->set_auto_off() ;
			return kb_listener->OnGetAndNextAction( shift_key_pressed ) ;
		}
		//	"&Set And Next (ALT + S)"
		CASE( _T('S') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
				kb_listener->set_auto_off() ;
			return kb_listener->OnSetAndNextAction( shift_key_pressed ) ;
		}
		//	"Look up &Current Selection (ALT + L)"
		CASE( _T('L') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
				kb_listener->set_auto_off() ;
			return kb_listener->OnLookupAction( shift_key_pressed ) ;
		}
		//	"Translate to Fu&zzy (ALT + Z)"
		CASE( _T('Z') )
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnAutoTransFuzzyAction( shift_key_pressed ) ;
		}
		//	"&Delete Translation (ALT + D)"
		CASE( _T('D') )
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnDeleteAction( ) ;
		}
		CASE( _T('M') )
		{
			in_shift_state = true ;
			kb_listener->OnShiftStateAction() ;
			return true ;
		}
		//	"Entry &0 (ALT + 0)"
		CASE( _T('0') )
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry0Action( shift_key_pressed ) ;
		}
		//	"Entry &1 (ALT + 1)"
		CASE( _T('1') )
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry1Action( shift_key_pressed ) ;
		}
		//	"Entry &2 (ALT + 2)"
		CASE( _T('2') )
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry2Action( shift_key_pressed ) ;
		}
		//	"Entry &3 (ALT + 3)"
		CASE( _T('3') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry3Action( shift_key_pressed ) ;
		}
		//	"Entry &4 (ALT + 4)"
		CASE( _T('4') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry4Action( shift_key_pressed ) ;
		}
		//	"Entry &5 (ALT + 5)"
		CASE( _T('5') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry5Action( shift_key_pressed ) ;
		}
		//	"Entry &6 (ALT + 6)"
		CASE( _T('6') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry6Action( shift_key_pressed ) ;
		}
		//	"Entry &7 (ALT + 7)"
		CASE( _T('7') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry7Action( shift_key_pressed ) ;
		}
		//	"Entry &8 (ALT + 8)"
		CASE( _T('8') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry8Action( shift_key_pressed ) ;
		}
		//	"Entry &9 (ALT + 9)"
		CASE( _T('9') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry9Action( shift_key_pressed ) ;
		}
	}
	return false ;

}

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*  classic_keyboard_proc                                               */
/************************************************************************/

bool classic_keyboard_proc( WPARAM wParam, WORD wKeystrokeMsg )
{
	static bool shift_state_with_control = false ;

	bool control_key_pressed	= control_key_is_pressed() ;
	bool alt_key_pressed		= alt_key_is_pressed(wKeystrokeMsg) ;


#ifdef _DEBUG
	if ( control_key_pressed && wParam == VK_F2 )
	{
		ATLTRACE( "Unit testing app...\n" ) ;
		run_unit_tests() ;
		return true ;
	}
#endif
	
	TRACE( control_key_pressed ) ;
	TRACE( alt_key_pressed ) ;

	// Our only keyboard shortcut that does not require the alt key to be pressed
	// is "extend lookup" (Ctrl + right arrow).
	// This is a special case, because additionally if there is no selection (nothing to be extended!)
	// we want to revert to the default handling

	/************************************************************************/
	/* Exception: Control key is pressed                                    */
	/************************************************************************/

	if (	control_key_pressed		&& 
			wParam == VK_RIGHT		&& 
			! alt_key_pressed 
		)
	{
		if ( in_shift_state ) 
		{
			in_shift_state = false ;
			return kb_listener->OnExtendTransLookupAction( false ) ;
		}

		return process_ctl_right() ;
	}

	/************************************************************************/
	/* If we are in a shift state                                           */
	/************************************************************************/

	if ( in_shift_state ) 
	{
		in_shift_state = false ;
		
		// Use the shift key to set/get text as plaintext. Otherwise the default (html) is retrieved
		bool shift_key_pressed = shift_key_is_pressed() ;

		ATLASSERT( ! ( ::GetKeyState( VK_MENU ) & 0x8000 ) ) ;
		ATLASSERT( ! ( ::GetKeyState( VK_CONTROL ) & 0x8000 ) );

		switch( wParam ) 
		{
		case _T('C'):
			{
				// skip if doc not active
				ENSURE_ACTIVE
				
				if ( shift_state_with_control ) 
				{
					return kb_listener->OnTransConcordanceAction( shift_key_pressed ) ;
				}
				else
				{
					return kb_listener->OnConcordanceAction( shift_key_pressed ) ;
				}
			}
		case _T('A'):
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
				return kb_listener->OnAutoTransAction( shift_key_pressed ) ;
			}
		case _T('G'):
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
				return kb_listener->OnRegisterGlossAction( shift_key_pressed ) ;
			}
		case _T('H'):
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
				return kb_listener->OnGlossNAction( shift_key_pressed ) ;
			}
		case _T('S'):
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
				return kb_listener->OnSaveMemoryAction( ) ;
			}

		default:
			return false ;
			
		}
	}
	

	// Since we have already handled the exceptions, the ALT key must be pressed for all other shortcuts

	if ( ! alt_key_pressed ) // alt key is up
	{
		return false; 
	}
	
	// alt_key_pressed tests the flag in our hook message; this asserts that
	// the alt key is actually depressed.
	ATLASSERT( ::GetKeyState(VK_MENU) & 0x8000 ) ;
	ATLASSERT( alt_key_pressed ) ;

	// Use the shift key to set/get text as plaintext. Otherwise the default (html) is retrieved
	bool shift_key_pressed = shift_key_is_pressed() ;

	if ( control_key_pressed ) 
	{
		switch ( wParam )
		{
			//	"&Look up Next Sentence (ALT + R ARR)"
			CASE( VK_RIGHT ) 
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
					return kb_listener->OnLookupNextTransAction( shift_key_pressed ) ;
			}
			//	"&Register Current Translation (ALT + UP ARR)"
			CASE( VK_UP ) 
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
					return kb_listener->OnCorrectAction( shift_key_pressed ) ;
			}
			//	"Get C&urrent Translation (ALT + DWN ARR)"
			CASE( VK_DOWN ) 
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
					return kb_listener->OnRestoreAction( shift_key_pressed ) ;
			}
			//	"&Get And Next (ALT + G)"
			CASE( _T('G') ) 
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
					return kb_listener->OnRestoreAndNextAction( shift_key_pressed ) ;
			}
			//	"&Set And Next (ALT + S)"
			CASE( _T('S') ) 
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
					return kb_listener->OnCorrectAndNextAction( shift_key_pressed ) ;
			}
			//	"Look up &Current Selection (ALT + L)"
			CASE( _T('L') ) 
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
					return kb_listener->OnLookupTransAction( shift_key_pressed ) ;
			}
			CASE( _T('E') ) 
			{
				// skip if doc not active
				ENSURE_ACTIVE
					
				return kb_listener->OnExtendTransLookupAction( shift_key_pressed ) ;
			}
		}
	}

	/************************************************************************/
	/* Alt key pressed                                                      */
	/************************************************************************/

	switch ( wParam )
	{
		//	"&Look up Next Sentence (ALT + R ARR)"
		CASE( VK_RIGHT ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnLookupNextAction( shift_key_pressed ) ;
		}
		//	"&Register Current Translation (ALT + UP ARR)"
		CASE( VK_UP ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnSetAction( shift_key_pressed ) ;
		}
		//	"Get C&urrent Translation (ALT + DWN ARR)"
		CASE( VK_DOWN ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnGetAction( shift_key_pressed ) ;
		}
		CASE( _T('.') )
		CASE( VK_OEM_PERIOD )
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnToMaruAction( shift_key_pressed ) ;
		}
		//	"&Previous Translation (ALT + P)"
		CASE( _T('P') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnPrevAction( ) ;
		}
		//	"&Next Translation (ALT + N)"
		CASE( _T('N') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnNextAction( ) ;
		}
		//	"&Get And Next (ALT + G)"
		CASE( _T('G') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnGetAndNextAction( shift_key_pressed ) ;
		}
		//	"&Set And Next (ALT + S)"
		CASE( _T('S') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnSetAndNextAction( shift_key_pressed ) ;
		}
		//	"Look up &Current Selection (ALT + L)"
		CASE( _T('L') ) 
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnLookupAction( shift_key_pressed ) ;
		}
		//	"Translate to Fu&zzy (ALT + Z)"
		CASE( _T('Z') )
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnAutoTransFuzzyAction( shift_key_pressed ) ;
		}
		//	"&Delete Translation (ALT + D)"
		CASE( _T('D') )
		{
			// skip if doc not active
			ENSURE_ACTIVE

			return kb_listener->OnDeleteAction( ) ;
		}
		CASE( _T('M') )
		{
			in_shift_state = true ;
			shift_state_with_control = control_key_pressed ;
			kb_listener->OnShiftStateAction() ;
			return true ;
		}
		//	"Entry &0 (ALT + 0)"
		CASE( VK_NUMPAD0 )
		CASE( _T('0') )
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry0Action( shift_key_pressed ) ;
		}
		//	"Entry &1 (ALT + 1)"
		CASE( VK_NUMPAD1 )
		CASE( _T('1') )
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry1Action( shift_key_pressed ) ;
		}
		//	"Entry &2 (ALT + 2)"
		CASE( VK_NUMPAD2 )
		CASE( _T('2') )
		{
			// skip if doc not active
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry2Action( shift_key_pressed ) ;
		}
		//	"Entry &3 (ALT + 3)"
		CASE( VK_NUMPAD3 )
		CASE( _T('3') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry3Action( shift_key_pressed ) ;
		}
		//	"Entry &4 (ALT + 4)"
		CASE( VK_NUMPAD4 )
		CASE( _T('4') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry4Action( shift_key_pressed ) ;
		}
		//	"Entry &5 (ALT + 5)"
		CASE( VK_NUMPAD5 )
		CASE( _T('5') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry5Action( shift_key_pressed ) ;
		}
		//	"Entry &6 (ALT + 6)"
		CASE( VK_NUMPAD6 )
		CASE( _T('6') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry6Action( shift_key_pressed ) ;
		}
		//	"Entry &7 (ALT + 7)"
		CASE( VK_NUMPAD7 )
		CASE( _T('7') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry7Action( shift_key_pressed ) ;
		}
		//	"Entry &8 (ALT + 8)"
		CASE( VK_NUMPAD8 )
		CASE( _T('8') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry8Action( shift_key_pressed ) ;
		}
		//	"Entry &9 (ALT + 9)"
		CASE( VK_NUMPAD9 )
		CASE( _T('9') )
		{
			ENSURE_ACTIVE
			
			return kb_listener->OnEntry9Action( shift_key_pressed ) ;
		}
	}

	return false ;
}
