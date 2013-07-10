#pragma once

class WindowInterface
{
public:
	virtual long get_height() = 0 ;
	virtual void set_height(long height) = 0 ;

	virtual long get_width() = 0 ;
	virtual void set_width(long width) = 0 ;

	virtual long get_left() = 0 ;
	virtual void set_left(long left) = 0 ;

	virtual long get_top() = 0 ;
	virtual void set_top(long top) = 0 ;

	virtual void raise() = 0 ;

};

class WindowWrapper
{
public:
	virtual BOOL IsWindow() = 0 ;
	virtual BOOL ShowWindow(int) = 0 ;
	virtual HWND SetFocus() = 0 ;
};

class WindowWrapperReal : public WindowWrapper
{
public:
	HWND m_window ;
	WindowWrapperReal(HWND hwnd) : m_window(hwnd){}
	BOOL IsWindow()
	{
		return ::IsWindow(m_window) ;
	}
	BOOL ShowWindow(int show_command)
	{
		return ::ShowWindow(m_window, show_command) ;
	}
	HWND SetFocus()
	{
		return ::SetFocus(m_window) ;
	}
};

class WindowWrapperFake : public WindowWrapper
{
public:
	BOOL m_is_window ;
	int m_show ;
	bool m_set_focus ;
	WindowWrapperFake() : m_is_window(FALSE), m_show(-1), m_set_focus(false) 
	{}
	BOOL IsWindow()
	{
		return m_is_window ;
	}
	BOOL ShowWindow(int show_command)
	{
		m_show = show_command ;
		return TRUE ;
	}
	HWND SetFocus()
	{
		m_set_focus = true ;
		return NULL ;
	}
};

typedef boost::shared_ptr<WindowWrapper> window_wrapper_ptr ;

inline window_wrapper_ptr get_window_real(HWND hwnd)
{
	return window_wrapper_ptr(new WindowWrapperReal(hwnd)) ;
}
inline window_wrapper_ptr get_window_fake(window_wrapper_ptr window, HWND)
{
	return window ;
}
