#pragma once

#include "GlossaryDlgListener.h"
#include "memory_local.h"
#include "record_local.h"
#include "felix_model_fake.h"
#include "Felix_properties.h"

#ifdef UNIT_TEST

class CGlossaryWinListenerFake : public CGlossaryWinListener
{
public:
	DECLARE_SENSING_VAR ;

	INT_PTR check_save_retval ;
	mem_engine::memory_pointer check_save_mem ;
	mem_engine::record_pointer added_rec ;
	app_props::properties_general m_props_general ;
	app_props::properties_memory m_mem_props ;
	model_iface_ptr m_model ;

	CGlossaryWinListenerFake() : 
		check_save_retval(IDCANCEL), 
		added_rec(new mem_engine::record_local),
		m_model(new FelixModelInterfaceFake)
	{
		check_save_mem = mem_engine::memory_pointer(new mem_engine::memory_local(app_props::get_props())) ;

	}	
		
	model_iface_ptr get_model()
	{
		return m_model ;
	}


	INT_PTR gloss_check_save_location( mem_engine::memory_pointer mem )
	{
		SENSE("gloss_check_save_location") ;
		check_save_mem = mem ;
		return check_save_retval ;
	}
	bool gloss_add_record( mem_engine::record_pointer rec )
	{
		SENSE("gloss_add_record") ;
		added_rec = rec ;
		return true ;
	}
	void gloss_view_switch( HWND hwnd )
	{
		hwnd ;
		SENSE("gloss_view_switch") ;
	}
	app_props::properties_general* get_props_general()
	{
		return &m_props_general ;
	}
};

#endif
