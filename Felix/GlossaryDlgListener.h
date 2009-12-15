/*!
	@file GlossaryDlgListener.h
	@brief Interface for the CGlossaryDlgListener class.
	@date 2005/06/25
	Time: 15:35:20
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "atlstr.h" // CString
#include "record.h"	// translation_record


/**
	@class CGlossaryDlgListener
	@brief Listener interface for glossary dialog events.
 */
class __declspec(novtable) CGlossaryWinListener
{
public:
	virtual INT_PTR gloss_check_save_location( memory_engine::memory_pointer mem ) = 0 ;
	virtual bool gloss_add_record( memory_engine::record_pointer rec ) = 0 ;
	virtual void gloss_view_switch( HWND hwnd ) = 0 ;
};
