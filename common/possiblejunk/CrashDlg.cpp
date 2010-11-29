// CrashDlg.cpp: implementation of the CCrashDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrashDlg.h"
#include "File.h"
#include "atlapp.h"
#include "atlctrls.h"	// CStatic

#include <io.h>
#pragma warning(disable: 4228)
#include <mapi.h>
#pragma warning(default: 4228)


#include "shellapi.h"
#pragma message("\tautomatic link to shell32.lib")
#pragma comment( lib, "shell32.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif


#pragma warning(disable:4127)	// for _ASSERTE

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

char * GetFilePart(LPCSTR lpszFile)
{
	char *result = strrchr((char*)lpszFile, _T('\\'));
	if (result)
		result++;
	else
		result = (char *) lpszFile;
	return result;
}

BOOL SendEmail(HWND    hWnd,			// parent window, must not be NULL
					LPCSTR lpszTo,			// must NOT be NULL or empty
					LPCSTR lpszToName,		// may be NULL
					LPCSTR lpszSubject,		// may be NULL
					LPCSTR lpszMessage,		// may be NULL
					LPCSTR lpszAttachment)	// may be NULL
{
	ATLTRACE(_T("in SendEmail\n"));

	_ASSERTE(lpszTo && lpszTo[0] != _T('\0'));
	if (lpszTo == NULL || lpszTo[0] == _T('\0'))
		return FALSE;

	// ===== LOAD MAPI DLL =====

	HMODULE hMapi = ::LoadLibrary( _T("MAPI32.DLL") );

	_ASSERTE(hMapi);
	if (hMapi == NULL)
	{
		::MessageBox(NULL,
			_T("Failed to load MAPI32.DLL."), 
			_T("CrashRep"),
			MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	// get proc address for MAPISendMail
	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)lpfnSendMail = GetProcAddress(hMapi, "MAPISendMail" ) ;
	_ASSERTE(lpfnSendMail);
	if (lpfnSendMail == NULL)
	{
		::MessageBox(NULL,
			_T("Invalid MAPI32.DLL, cannot find MAPISendMail."),
			_T("CrashRep"),
			MB_OK|MB_ICONSTOP);
		::FreeLibrary(hMapi);
		return FALSE;
	}

	// ===== SET UP MAPI STRUCTS =====

	// ===== file description (for the attachment) =====

	MapiFileDesc fileDesc;
	memset(&fileDesc, 0, sizeof(fileDesc));

	// ----- attachment path
	char szTempName[_MAX_PATH*2];
	memset(szTempName, 0, sizeof(szTempName));
	if (lpszAttachment && lpszAttachment[0] != _T('\0'))
		strncpy(szTempName, lpszAttachment, _countof(szTempName)-2);

	// ----- attachment title
	char szTitle[_MAX_PATH*2];
	memset(szTitle, 0, sizeof(szTitle));
	if (lpszAttachment && lpszAttachment[0] != _T('\0'))
		strncpy(szTitle, GetFilePart(lpszAttachment), _countof(szTitle)-2);


	fileDesc.nPosition = (ULONG)-1;
	fileDesc.lpszPathName = szTempName;
	fileDesc.lpszFileName = szTitle;

	// ===== recipient =====

	MapiRecipDesc recip;
	memset(&recip, 0, sizeof(recip));

	// ----- name
	char szRecipName[_MAX_PATH*2];
	memset(szRecipName, 0, sizeof(szRecipName));
	if (lpszToName && lpszToName[0] != _T('\0'))
		strncpy(szRecipName, lpszToName, _countof(szRecipName)-2);

	if (lpszToName && lpszToName[0] != _T('\0'))
	{
		recip.lpszName = szRecipName;
	}

	// ----- address
	char szAddress[_MAX_PATH*2];
	memset(szAddress, 0, sizeof(szAddress));
	strncpy(szAddress, lpszTo, _countof(szAddress)-2);

	recip.lpszAddress = szAddress;

	recip.ulRecipClass = MAPI_TO;

	// ===== message =====

	MapiMessage message;
	memset(&message, 0, sizeof(message));

	// ----- recipient
	message.nRecipCount = 1;
	message.lpRecips = &recip;

	// ----- attachment
	if (lpszAttachment && lpszAttachment[0] != _T('\0'))
	{
		message.nFileCount = 1;
		message.lpFiles = &fileDesc;
	}

	// ----- subject
	char szSubject[_MAX_PATH*2];
	memset(szSubject, 0, sizeof(szSubject));
	if (lpszSubject && lpszSubject[0] != _T('\0'))
		strncpy(szSubject, lpszSubject, _countof(szSubject)-2);

	if (lpszSubject && lpszSubject[0] != _T('\0'))
	{
		message.lpszSubject = szSubject;
	}

	// ----- message
	// message may be large, so allocate buffer

	if ( lpszMessage )
	{
		message.lpszNoteText = (char *)lpszMessage;
	}


	// ===== SETUP FINISHED, READY TO SEND =====


	// some extra precautions are required to use MAPISendMail as it
	// tends to enable the parent window in between dialogs (after
	// the login dialog, but before the send note dialog).

	::SetCapture(hWnd);
	::SetFocus(NULL);
	::EnableWindow(hWnd, FALSE);

#pragma warning(disable:4311)	// truncation

	ULONG nError = lpfnSendMail(0, 
		(ULONG)( hWnd ),
		&message, 
		MAPI_LOGON_UI | MAPI_DIALOG, 
		0);

#pragma warning(default:4311)	// truncation

#ifdef _DEBUG
	char *cp = NULL;
	switch (nError)
	{
	case SUCCESS_SUCCESS:                 cp = "SUCCESS_SUCCESS"; break;
	case MAPI_E_USER_ABORT:               cp = "MAPI_E_USER_ABORT"; break;
	case MAPI_E_FAILURE:                  cp = "MAPI_E_FAILURE"; break;
	case MAPI_E_LOGON_FAILURE:            cp = "MAPI_E_LOGON_FAILURE"; break;
	case MAPI_E_DISK_FULL:                cp = "MAPI_E_DISK_FULL"; break;
	case MAPI_E_INSUFFICIENT_MEMORY:      cp = "MAPI_E_INSUFFICIENT_MEMORY"; break;
	case MAPI_E_ACCESS_DENIED:            cp = "MAPI_E_ACCESS_DENIED"; break;
	case MAPI_E_TOO_MANY_SESSIONS:        cp = "MAPI_E_TOO_MANY_SESSIONS"; break;
	case MAPI_E_TOO_MANY_FILES:           cp = "MAPI_E_TOO_MANY_FILES"; break;
	case MAPI_E_TOO_MANY_RECIPIENTS:      cp = "MAPI_E_TOO_MANY_RECIPIENTS"; break;
	case MAPI_E_ATTACHMENT_NOT_FOUND:     cp = "MAPI_E_ATTACHMENT_NOT_FOUND"; break;
	case MAPI_E_ATTACHMENT_OPEN_FAILURE:  cp = "MAPI_E_ATTACHMENT_OPEN_FAILURE"; break;
	case MAPI_E_ATTACHMENT_WRITE_FAILURE: cp = "MAPI_E_ATTACHMENT_WRITE_FAILURE"; break;
	case MAPI_E_UNKNOWN_RECIPIENT:        cp = "MAPI_E_UNKNOWN_RECIPIENT"; break;
	case MAPI_E_BAD_RECIPTYPE:            cp = "MAPI_E_BAD_RECIPTYPE"; break;
	case MAPI_E_NO_MESSAGES:              cp = "MAPI_E_NO_MESSAGES"; break;
	case MAPI_E_INVALID_MESSAGE:          cp = "MAPI_E_INVALID_MESSAGE"; break;
	case MAPI_E_TEXT_TOO_LARGE:           cp = "MAPI_E_TEXT_TOO_LARGE"; break;
	case MAPI_E_INVALID_SESSION:          cp = "MAPI_E_INVALID_SESSION"; break;
	case MAPI_E_TYPE_NOT_SUPPORTED:       cp = "MAPI_E_TYPE_NOT_SUPPORTED"; break;
	case MAPI_E_AMBIGUOUS_RECIPIENT:      cp = "MAPI_E_AMBIGUOUS_RECIPIENT"; break;
	case MAPI_E_MESSAGE_IN_USE:           cp = "MAPI_E_MESSAGE_IN_USE"; break;
	case MAPI_E_NETWORK_FAILURE:          cp = "MAPI_E_NETWORK_FAILURE"; break;
	case MAPI_E_INVALID_EDITFIELDS:       cp = "MAPI_E_INVALID_EDITFIELDS"; break;
	case MAPI_E_INVALID_RECIPS:           cp = "MAPI_E_INVALID_RECIPS"; break;
	case MAPI_E_NOT_SUPPORTED:            cp = "MAPI_E_NOT_SUPPORTED"; break;
	default:                              cp = "unknown error"; break;
	}

	if (nError == SUCCESS_SUCCESS)
	{
		ATLTRACE(_T("MAPISendMail ok\n"));
	}
	else
	{
		ATLTRACE(_T("ERROR - MAPISendMail failed:  %s\n"), cp);
	}
#endif // _DEBUG


	// ===== SEND COMPLETE, CLEAN UP =====

	// after returning from the MAPISendMail call, the window must be
	// re-enabled and focus returned to the frame to undo the workaround
	// done before the MAPI call.

	::ReleaseCapture();
	::EnableWindow(hWnd, TRUE);
	::SetActiveWindow(NULL);
	::SetActiveWindow(hWnd);
	::SetFocus(hWnd);

	if (hMapi)
		::FreeLibrary(hMapi);
	hMapi = NULL;

	BOOL bRet = TRUE;
	if (nError != SUCCESS_SUCCESS) // &&
		//nError != MAPI_USER_ABORT && 
		//nError != MAPI_E_LOGIN_FAILURE)
	{
		bRet = FALSE;
	}

	return bRet;
}

