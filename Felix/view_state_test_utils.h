#pragma once

#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "felix_model_fake.h"
#include "view_state.h"
#include "memory_local.h"
#include "Felix_properties.h"

#ifdef UNIT_TEST

struct view_state_obj
{
	view_interface_fake view ;
	WindowListenerFake listener; 
	FelixModelInterfaceFake model ;

	mem_engine::memory_pointer mem  ;
	app_props::properties_memory mem_props; 

	view_state_obj(ViewState *state)
	{
		mem = memory_pointer(new mem_engine::memory_local(&mem_props)) ;
		state->set_view(&view) ;
		state->set_window_listener(&listener) ;
		state->set_model(&model) ;
		model.get_memories()->insert_memory(mem) ;
	}

};

#endif
