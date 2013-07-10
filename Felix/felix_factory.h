#pragma once

#include "MemoryWindowFrame.h"
#include "memory_local.h"
#include "FelixModel.h"

typedef boost::shared_ptr<MemoryWindowFrame> frame_ptr ;

class FelixFactory
{
public:
	app_props::props_ptr make_props()
	{
		return app_props::get_props() ;
	}

	static model_iface_ptr make_model_interface(app_props::props_ptr props)
	{
		return model_iface_ptr(new FelixModel(props)) ;
	}
	/** Get the MainFrame.
	*/
	frame_ptr make_app(void) 
	{ 
		app_props::props_ptr props = make_props(); 
		model_iface_ptr the_model = make_model_interface(props);
		return frame_ptr(new MemoryWindowFrame( the_model, props )) ;
	}

	mem_engine::memory_pointer make_memory()
	{
		app_props::props_ptr props = make_props(); 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(props)) ;
		mem->set_properties_glossary(&props->m_gloss_props) ;
		mem->set_properties_algo(&props->m_alg_props) ;
		return mem ;
	}
};

inline frame_ptr make_frame()
{
	return FelixFactory().make_app() ;
}