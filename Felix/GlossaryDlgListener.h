/*!
	@file GlossaryDlgListener.h
	@brief Interface for the CGlossaryDlgListener class.
	@date 2005/06/25
	Time: 15:35:20
	@author Ryan Ginstrom
 */

#pragma once

#include "record.h"	// translation_record
#include "Felix_properties.h"
#include "rule_placement.h"

/**
	@class CGlossaryDlgListener
	@brief Listener interface for glossary dialog events.
 */
class __declspec(novtable) CGlossaryWinListener
{
public:
	virtual INT_PTR gloss_check_save_location( mem_engine::memory_pointer mem ) = 0 ;
	virtual bool gloss_add_record( mem_engine::record_pointer rec ) = 0 ;
	virtual void gloss_view_switch( HWND hwnd ) = 0 ;
	virtual model_iface_ptr get_model() = 0 ;
	virtual mem_engine::placement::regex_rules *get_regex_rules() = 0 ;
	virtual app_props::properties_general* get_props_general() = 0 ;
};

