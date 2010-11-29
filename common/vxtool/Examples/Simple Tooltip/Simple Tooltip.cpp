////////////////////////////////////////////////////////////////////////////
// VXPLib C++ Examples
//
// Simple Tooltip
//
// Last Modified on 27/03/2003
//
// This example is intellectual property of Vitaly Tomilov
//
// To find out more about the VXPLib project
// visit www.Tooltips.NET
//
////////////////////////////////////////////////////////////////////////////

// This is the simplest possible example of a windows API application that
// uses tooltips. The application creates a simple window, and then attaches
// a tooltip to it.

#include "stdafx.h"
#include "VXPLib.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	// Initializing STA to make it possible to use COM:
	::CoInitialize(NULL);
	// Creating a button window class:
	HWND hWnd = ::CreateWindow("BUTTON", "Simple Tooltip", WS_VISIBLE|WS_SYSMENU, 300, 200, 300, 300, NULL, NULL, hInstance, NULL);
	// Showing the window:
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	// Updating the window:
	::UpdateWindow(hWnd);
	// Declaring a Tooltip Manager variable:

	CVXPTooltipManager manager;
	// Attaching a tooltip to the window:
	manager.windows.Add(hWnd, "<body bgcolor=lightsalmon gfc=skyblue text=black><table><tr><td><font size=28 face='Webdings' charset=symbol color=yellow>i</font></td><td>This is probably the World Simplest <u>Windows API</u><br>example of creating a window that has such an elaborate<br>tooltip attached to it. <b>Only 10 lines of code overall!</b></td></tr></table><hr size=1 color=darkred><table><tr><td>Did you ever see anything simpler than this? - Please, tell us<br>about it on <font color=darkblue weight=700>www.tooltips.net</font>, we are eager to hear that...</td><td><font size=20 face='Webdings' charset=symbol color=White>O</font></td></tr></table>");

	// Organizing a message loop to show the window:
	MSG msg;
	while(IsWindow(hWnd) && ::IsWindowVisible(hWnd) && GetMessage(&msg, NULL, 0, 0) && !(msg.message == WM_SYSCOMMAND && msg.wParam == SC_CLOSE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return 0;
}
