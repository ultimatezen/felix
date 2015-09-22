#include "stdafx.h"
#include "menu_helper.h"

MenuWrapper::MenuWrapper( HMENU wrapped_menu, HWND wrapped_hwnd ) : 
	menu(wrapped_menu),
	hwnd(wrapped_hwnd)
{

}

bool MenuWrapper::add_item( unsigned int command_id, string text )
{
	wstring menu_text = string2wstring(text) ;

	CMenuItemInfo menu_item ;
	menu_item.fMask = MIIM_STRING | MIIM_ID ; // | MIIM_BITMAP;
	menu_item.fType = MFT_STRING ;
	menu_item.wID = command_id ;
	menu_item.cch = menu_text.size() ;
	menu_item.dwTypeData = const_cast<LPWSTR>(menu_text.c_str()) ;

	return !! menu.InsertMenuItem(menu.GetMenuItemCount(), FALSE, (MENUITEMINFO*)&menu_item) ;
}

bool MenuWrapper::add_separator()
{
	CMenuItemInfo menu_item ;
	menu_item.fMask = MIIM_FTYPE ;
	menu_item.fType = MFT_SEPARATOR ;

	return !! menu.InsertMenuItem(menu.GetMenuItemCount(), FALSE, (MENUITEMINFO*)&menu_item) ;
}

void MenuWrapper::show()
{
	POINT ptScreen ;
	::GetCursorPos(&ptScreen) ;
	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON,	ptScreen.x, ptScreen.y, hwnd, NULL) ;
}
