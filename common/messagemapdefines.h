/*!
	@file MessageMapDefines.h
	@brief Various macros to make writing message maps easier
	@date 2005/06/25
	Time: 15:24:33
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#include "atlcrack.h"

static const unsigned int UWM_USER_MESSAGE = ::RegisterWindowMessage( _T("UUWM_USER_MESSAGE_91B5ED77-0AAA-42ef-8471-580D61069061") ) ;

#define BEGIN_MAP_EX( x ) \
	BEGIN_MSG_MAP_EX( x) \
	MSG_TRY

#define END_MAP_EX( x ) \
	MSG_CATCH( x ) \
	END_MSG_MAP() 

// ===================
// Generic message handlers
// ===================

#define MSG_HANDLER_0(msg, func) \
	if(uMsg == msg) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = func( ); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

#define MSG_HANDLER_1(msg, func, arg) \
	if(uMsg == msg) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = func( arg ); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// ===================
// WM_COMMAND handlers
// ===================


#define BEGIN_CMD_HANDLER_EX \
	if ( uMsg == WM_COMMAND ) \
	{ \
		if ( false ) ; // start up our else ifs

#define CMD_HANDLER_EX_HWND(id, code, func) \
	else if (code == HIWORD(wParam) && id == LOWORD(wParam)) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = func( (HWND)lParam ); \
		if(IsMsgHandled()) \
			return TRUE; \
	}
#define CMD_HANDLER_EX(id, code, func) \
	else if (code == HIWORD(wParam) && id == LOWORD(wParam)) \
	{ \
	SetMsgHandled(TRUE); \
	lResult = func( ); \
	if(IsMsgHandled()) \
		return TRUE; \
	}

#define CMD_ID_HANDLER_EX(id, func) \
		else if ( id == LOWORD( wParam ) ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}

#define CMD_HANDLER_EX_0(id, func) \
		else if ( id == LOWORD( wParam ) ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func( ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}
#define CMD_HANDLER_EX_1(id, func, arg) \
		else if ( id == LOWORD( wParam ) ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func( arg ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}
#define CMD_HANDLER_EX_ID(id, func) \
		else if ( id == LOWORD( wParam ) ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func( LOWORD(wParam) ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}

#define NOTIFICATION_CMD_HANDLER_EX(id, func, cast) \
		else if ( id == HIWORD( wParam ) ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func( (cast)lParam ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}

#define NOTIFICATION_CMD_HANDLER_EX_ID(cmd, func ) \
	else if ( cmd == HIWORD( wParam ) ) \
		{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func( LOWORD(wParam) ); \
		if(IsMsgHandled()) \
		return TRUE; \
		}

#define NOTIFICATION_CMD_HANDLER_EX_0(id, func) \
		else if ( id == HIWORD( wParam ) ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func( ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}

#define NOTIFICATION_CMD_HANDLER_EX_WINDOW_0(window, id, func) \
	else if ( id == HIWORD( wParam ) && window.m_hWnd == (HWND)lParam) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func( ); \
			if(IsMsgHandled()) \
			return TRUE; \
		}

#define CMD_RANGE_HANDLER_EX(id1, id2, func) \
		else if ( LOWORD( wParam ) >= id1 && LOWORD( wParam ) <= id2 ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}
#define CMD_RANGE_HANDLER_EX_ID(id1, id2, func) \
		else if ( LOWORD( wParam ) >= id1 && LOWORD( wParam ) <= id2 ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = (LRESULT)func( LOWORD(wParam) ); \
			if(IsMsgHandled()) \
				return TRUE; \
		}

#define PASS_UP_COMMAND_EX(ID) \
		else if( ID == LOWORD(wParam)) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = ::SendMessage( GetParent(), WM_COMMAND, wParam, lParam ) ; \
			if(IsMsgHandled()) \
				return TRUE; \
		}

#define PASS_DOWN_COMMAND_EX(ID) \
		else if( ID == LOWORD(wParam) && m_hWndClient != NULL ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = ::SendMessage( m_hWndClient, WM_COMMAND, wParam, lParam ) ; \
			return TRUE ; \
		}

#define PASS_CMDS_TO_VIEW_EX( view ) \
		else if ( view.IsWindow() )\
		{ \
			if ( view.ProcessWindowMessage( hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID ) ) \
				return TRUE ; \
			SetMsgHandled(FALSE); \
		}


#define PASS_DOWN_REMAINING_CMDS_EX \
		else if ( m_hWndClient != NULL ) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = ::SendMessage( m_hWndClient, WM_COMMAND, wParam, lParam ) ; \
			return TRUE ; \
		}

#define END_CMD_HANDLER_EX \
	}

#define FORWARD_WM_MSG( msg_id ) if( uMsg == msg_id ) { lResult = ::SendMessage( GetParent(), uMsg, wParam, lParam ) ; return TRUE ; }
#define FORWARD_WM_DROPFILES FORWARD_WM_MSG( WM_DROPFILES )

// ===================
// UWM_USER_MESSAGE handlers
// ===================
// if ( uMsg == UWM_USER_MESSAGE )

#define BEGIN_USER_HANDLER_EX \
	if ( uMsg == UWM_USER_MESSAGE ) \
	{ \
		SetMsgHandled(FALSE); \
		switch( wParam )\
		{

#define USER_HANDLER_EX( w, func ) \
			case w:\
			{\
				SetMsgHandled(TRUE); \
				lResult = func( lParam ) ; \
				if(IsMsgHandled()) \
					return TRUE; \
				break ; \
			}

#define USER_HANDLER_EX_0( w, func ) \
			case w:\
			{\
				SetMsgHandled(TRUE); \
				lResult = func( ) ; \
				if(IsMsgHandled()) \
					return TRUE; \
				break ; \
			}

#define END_USER_HANDLER_EX \
		} \
	}
	
#define BEGIN_USER_HANDLER \
	if ( uMsg == UWM_USER_MESSAGE ) \
	{ \
		bHandled = FALSE ;\
		switch( wParam )\
		{
#define USER_HANDLER( w, func ) \
			case w:\
			{\
				bHandled = TRUE ; \
				lResult = func( lParam, bHandled ) ; \
				break ;\
			}
#define END_USER_HANDLER \
		}\
		WRITE_TABS ATLTRACE( "*** Unhandled UWM_USER_MESSAGE message passed: %d\n", wParam ) ;\
		if ( bHandled ) \
		return TRUE ; \
	}
	

// ===================
// WM_NOTIFY handlers
// ===================

#define BEGIN_NOTIFY_HANDLER_EX \
		if(uMsg == WM_NOTIFY) \
		{ \
			SetMsgHandled( FALSE ) ; \
			if ( false ) ;	// start up our else-if switches

#define HANDLE_NOTIFICATION_CMD_EX( notification_code, routine, cast ) \
			else if ( notification_code == HIWORD( wParam ) ) \
			{ \
				SetMsgHandled( TRUE ) ; \
				lResult = routine( (cast)lParam ); \
				if (IsMsgHandled()) \
					return TRUE ; \
			}


#define HANDLE_NOTIFICATION_EX( notification_code, routine, cast ) \
			else if ( notification_code == ((LPNMHDR)lParam)->code) \
			{ \
				SetMsgHandled( TRUE ) ; \
				lResult = routine( (cast)lParam ); \
				if (IsMsgHandled()) \
					return TRUE ; \
			}

#define HANDLE_NOTIFICATION_EX_ID( notification_code, id, routine, cast ) \
	else if ( notification_code == ((LPNMHDR)lParam)->code && wParam == id ) \
			{ \
				SetMsgHandled( TRUE ) ; \
				lResult = routine( (cast)lParam ); \
				if (IsMsgHandled()) \
				return TRUE ; \
			}

#define HANDLE_NOTIFICATION_EX_ID_0( notification_code, id, routine) \
	else if ( notification_code == ((LPNMHDR)lParam)->code && wParam == id ) \
			{ \
				SetMsgHandled( TRUE ) ; \
				lResult = routine( ); \
				if (IsMsgHandled()) \
				return TRUE ; \
			}

#define HANDLE_NOTIFICATION_EX_0( notification_code, routine ) \
			else if ( notification_code == ((LPNMHDR)lParam)->code) \
			{ \
				SetMsgHandled( TRUE ) ; \
				lResult = routine( ); \
				if (IsMsgHandled()) \
					return TRUE ; \
			}

#define PASS_UP_NOTIFICATION_EX( notification_code ) \
			else if ( notification_code == ((LPNMHDR)lParam)->code) \
			{ \
				SetMsgHandled( TRUE ) ; \
				lResult = ::SendMessage( GetParent(), WM_NOTIFY, wParam, lParam ) ; \
				return TRUE ; \
			}


#define BEGIN_NOTIFY_HANDLER \
		if(uMsg == WM_NOTIFY) \
		{ \
			bHandled = FALSE ; \
			if ( false ) ;

#define HANDLE_NOTIFICATION( notification_code, routine, cast ) \
			else if ( notification_code == ((LPNMHDR)lParam)->code) \
			{ \
				bHandled = TRUE ; \
				lResult = routine( (cast)lParam, bHandled); \
				return TRUE ; \
			}

#define PASS_UP_NOTIFICATION( notification_code ) \
			else if ( notification_code == ((LPNMHDR)lParam)->code) \
			{ \
				bHandled = TRUE ; \
				lResult = ::SendMessage( GetParent(), WM_NOTIFY, wParam, lParam ) ; \
				return TRUE ; \
			}
				
#define PASS_UP_NOTIFICATION_COMMAND(ID) \
	if(uMsg == WM_COMMAND && ID == HIWORD(wParam)) \
	{ \
		lResult = ::SendMessage( GetParent(), WM_COMMAND, MAKEWPARAM( HIWORD(wParam), LOWORD( wParam ) ), lParam ) ; \
		return TRUE ;\
	}

#define PASS_UP_COMMAND(ID) \
	if(uMsg == WM_COMMAND && ID == LOWORD(wParam)) \
	{ \
		ATLASSERT( IsWindow() ) ;\
		lResult = ::SendMessage( GetParent(), WM_COMMAND, wParam, lParam ) ; \
		return TRUE ;\
	}

#define END_NOTIFY_HANDLER \
		}

