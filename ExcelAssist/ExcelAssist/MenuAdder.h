/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "ExcelImport.h"

/*!
 * Adds the Felix menu.
 */
class CMenuAdder
{
	/*! Passed in from the connection object.
	 */
	MSOffice::CommandBarControlsPtr m_menuControls ;

public:
	CMenuAdder( MSOffice::CommandBarControlPtr  controlPtr );
	~CMenuAdder(void);

	MSOffice::_CommandBarButtonPtr add_menu( _bstr_t text, _bstr_t tooltip, bool beginsGroup = false ) ;
	MSOffice::CommandBarControlPtr add_popup( _bstr_t text, bool beginsGroup = false ) ;
};
