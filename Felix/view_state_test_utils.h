#pragma once

#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "felix_model_fake.h"
#include "view_state.h"
#include "memory_local.h"

#ifdef UNIT_TEST

struct view_state_obj
{
	view_interface_fake view ;
	WindowListenerFake listener; 
	FelixModelInterfaceFake model ;

	mem_engine::memory_pointer mem  ;

	view_state_obj(ViewState *state) :
		mem(new mem_engine::memory_local)
	{
		state->set_view(&view) ;
		state->set_window_listener(&listener) ;
		state->set_model(&model) ;
		model.get_memories()->insert_memory(mem) ;
	}

};

#endif
