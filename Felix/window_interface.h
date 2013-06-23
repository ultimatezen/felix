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
};

class WindowWrapperFake : public WindowWrapper
{
public:
	BOOL m_is_window ;
	WindowWrapperFake() : m_is_window(FALSE){}
	BOOL IsWindow()
	{
		return m_is_window ;
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
