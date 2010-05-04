/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "StdAfx.h"
#include "MenuAdder.h"
#include "stringex.h"
#include "ErrorTranslator.h"

/*!
 * CTOR.
 */
CMenuAdder::CMenuAdder(MSOffice::CommandBarControlPtr controlPtr)
{
	IDispatchPtr spDispatch = controlPtr->GetControl() ;

	MSOffice::CommandBarPopupPtr popup ;
	spDispatch->QueryInterface( __uuidof( MSOffice::CommandBarPopup ), (LPVOID*)&popup ) ;

	popup->Visible = VARIANT_TRUE ;

	m_menuControls = popup->GetControls() ;

}

/*!
 * DTOR.
 */
CMenuAdder::~CMenuAdder(void)
{
}

/*!
 * Creates a tag from a caption.
 */
_bstr_t getTag(_bstr_t& text)
{
	wstring idText = (LPCWSTR)text ;
	boost::replace_all( idText, L" ", L"_" ) ;
	boost::replace_all( idText, L"&", L"_" ) ;
	boost::replace_all( idText, L"\t", L"_" ) ;

	_bstr_t tag = idText.c_str( ) ;
	return tag ;
}

/*!
 * Adds a menu item.
  */
MSOffice::_CommandBarButtonPtr CMenuAdder::add_menu( _bstr_t text, _bstr_t tooltip, bool beginsGroup )
{
	MSOffice::CommandBarControlPtr menuControl = m_menuControls->Add
		(
		_variant_t( MSOffice::msoControlButton ), // type
		vtMissing,								// id
		vtMissing, 								// parameter
		vtMissing, 								// before
		_variant_t( VARIANT_TRUE )				// temporary
		) ;

	IDispatchPtr menuDispatch = menuControl->GetControl() ;

	MSOffice::_CommandBarButtonPtr button ;
	menuDispatch->QueryInterface( __uuidof(MSOffice::_CommandBarButton), (LPVOID*)&button ) ;

	button->Caption = text ;
	button->TooltipText = tooltip ;
	button->Tag = getTag(text) ;
	button->Visible = VARIANT_TRUE ;

	if (beginsGroup)
	{
		button->BeginGroup = VARIANT_TRUE ;
	}

	return button ;
}

/*!
 * Adds a popup (for the glossary function).
 */
MSOffice::CommandBarControlPtr CMenuAdder::add_popup( _bstr_t text, bool beginsGroup )
{
	MSOffice::CommandBarControlPtr menuControl = m_menuControls->Add
		(
		_variant_t( MSOffice::msoControlPopup ), // type
		vtMissing,								// id
		vtMissing, 								// parameter
		vtMissing, 								// before
		_variant_t( VARIANT_TRUE )				// temporary
		) ;

	menuControl->Caption = text ;
	menuControl->Visible = VARIANT_TRUE ;

	if (beginsGroup)
	{
		menuControl->BeginGroup = VARIANT_TRUE ;
	}

	return menuControl ;
}