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
	app_props::properties_general m_props_general ;;

	FelixModelInterfaceFake m_model ;

	CGlossaryWinListenerFake() : 
		check_save_retval(IDCANCEL), 
		check_save_mem(new mem_engine::memory_local),
		added_rec(new mem_engine::record_local)
	{

	}	
		
	FelixModelInterface* get_model()
	{
		return &m_model ;
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
