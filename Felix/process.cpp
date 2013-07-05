#include "stdafx.h"
#include "process.h"

void create_process( CString &command, CString error_message )
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process. 
	if( !CreateProcess(NULL,   // No module name (use command line). 
		command.GetBuffer(), // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		DETACHED_PROCESS, // DETACHED_PROCESS creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
	{
		throw except::CWinException(error_message) ;
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}