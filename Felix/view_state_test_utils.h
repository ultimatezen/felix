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
	model_iface_ptr model ;

	mem_engine::memory_pointer mem  ;
	app_props::properties_memory mem_props; 

	view_state_obj(ViewState *state) : 		
		model(new FelixModelInterfaceFake)
	{
		state->set_view(&view) ;
		state->set_window_listener(&listener) ;
		state->set_model(model) ;
		mem = model->add_memory() ;
	}

};

#endif
