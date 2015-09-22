#pragma once


static const int BM_SIZE = 16 ;
static const int SEP_ID = 0 ;
static const COLORREF MAGENTA = RGB( 255, 0, 255 ) ;

class MenuWrapperBase
{
public:
	virtual bool add_item(unsigned int command_id, string text) = 0 ;
	virtual bool add_separator() = 0 ;
	virtual void show() = 0 ;
};

class MenuWrapper : public MenuWrapperBase
{
public:
	explicit MenuWrapper(HMENU wrapped_menu, HWND wrapped_hwnd);

	bool add_item(unsigned int command_id, string text);
	bool add_separator();
	void show();
private:
	CMenuHandle menu ;
	HWND hwnd ;
};
