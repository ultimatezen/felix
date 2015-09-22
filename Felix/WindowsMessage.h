#pragma once

class WindowsMessage
{
public:
	HWND hWnd ;
	UINT uMsg ;
	WPARAM wParam ;
	LPARAM lParam ;
	bool bHandled ;

	WindowsMessage(HWND hWnd=NULL, 
				UINT uMsg=0, 
				WPARAM wParam=0, 
				LPARAM lParam=0,
				bool handled=true) :

		hWnd( hWnd ),
		uMsg(uMsg),
		wParam(wParam),
		lParam(lParam),
		bHandled(handled)
	{
	}

	~WindowsMessage(void)
	{
	}
	WORD w_low()
	{
		return LOWORD(wParam) ;
	}
	WORD w_high()
	{
		return HIWORD(wParam) ;
	}
	void notHandled()
	{
		bHandled = false ;
	}
	bool isHandled()
	{
		return bHandled ;
	}
};
